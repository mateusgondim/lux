#include "main/main.h"

#include <cmath>
#include <cstdint>

#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <memory>

#include "core/camera.h"
#include "core/mat4.h"
#include "core/ray.h"
#include "core/vec2.h"
#include "core/vec3.h"
#include "core/rgb_spectrum.h"
#include "core/transform.h"
#include "core/shape.h"
#include "core/error.h"
#include "core/filter.h"

#include "materials/lambertian.h"
#include "materials/mirror.h"

#include "shapes/sphere.h"
#include "shapes/triangle.h"

#include "samplers/random.h"
#include "samplers/stratified.h"


struct rgb {
  int r;
  int g;
  int b;
};

std::vector<lux::Shape *> shapes;
std::vector<lux::Shape *> lights;

const unsigned kmax_depth = 5; 
const bool g_direct_light_only = false;

lux::RGB_spectrum skybox(const lux::Ray & r)
{
  lux::Vec3 normalized_dir = normalize(r.get_direction());
  // scale to [0, 1] interval and do a LERP
  float t = (normalized_dir.y + 1) * 0.5f;
  const lux::RGB_spectrum s0(1.0f);
  const lux::RGB_spectrum s1(0.5f, 0.7f, 1.0f);

  return lux::lerp(t, s0, s1);
}


bool intersect_p(const lux::Ray & ray, std::vector<lux::Shape*> & shapes)
{
  std::vector<lux::Shape*>::const_iterator iter = shapes.cbegin();
  for(; iter != shapes.cend(); ++iter) {
    if ((*iter)->intersect_p(ray)) {
      return true;
    }
  }
  return false;
}

lux::RGB_spectrum estimate_direct(const lux::Surface_interaction & interaction, 
                                        const lux::Vec2 & scattering_sample,
                                        const lux::Shape & light,
                                        const lux::Vec2 & light_sample)
{

  lux::RGB_spectrum Ld(0.0f);
  // Sample light source
  lux::Vec3 wi_world;
  float light_pdf;
  float scattering_pdf;
  lux::Vec3 point_on_light;
  lux::RGB_spectrum Li(0.0f);
  Li = light.sample_li(interaction, light_sample, &wi_world, &point_on_light, &light_pdf);
  bool reflect = dot(wi_world, interaction.n) * dot(interaction.wo_world, interaction.n) > 0;
  if (light_pdf > 0.0f && !Li.is_black() && reflect) {
    // Evaluate BRDF for the light sampling strategy
    lux::RGB_spectrum f(0.0f);
    f = interaction.pmaterial->f(interaction.wo_world, wi_world) * abs_dot(wi_world, interaction.n);
    scattering_pdf = interaction.pmaterial->PDF(interaction.wo_world, wi_world);

    // Only if some of the incident light Li reflects back in the direction wo, 
    // should we follow through
    if (!f.is_black()) {
      const lux::Vec3 d = point_on_light - interaction.hit_point;
      lux::Ray shadow_ray(interaction.hit_point, lux::normalize(d),
                                lux::magnitude(d) - lux::kshadow_epsilon);

      const bool isOccluded = intersect_p(shadow_ray, shapes); 
      if (!isOccluded) {
        const float kweight = lux::power_heuristic(1, light_pdf, 1, scattering_pdf);
        Ld += f * Li * kweight / light_pdf;
      }
    }
  }

  // Sample the BRDF
  lux::RGB_spectrum f(0.0f);
  f = interaction.pmaterial->sample_f(interaction.wo_world, &wi_world, scattering_sample,
      &scattering_pdf); 

  f *= lux::abs_dot(wi_world, interaction.n);

  if (!f.is_black() && scattering_pdf > 0.0f) {
    float weight = 1.0f;
    lux::Material_type material_type = interaction.pmaterial->get_type();

    // This block should only be used if the surface is NOT specular!
    if (material_type != lux::Material_type::kspecular) {
      light_pdf = light.PDF(interaction, wi_world);
      if (light_pdf == 0.0f) return Ld;
      weight = lux::power_heuristic(1, scattering_pdf, 1, light_pdf);
    }

    // Check if the ray intersects the light source
    lux::Surface_interaction light_interaction;
    lux::Ray r(interaction.hit_point, wi_world);
    bool found_intersection = intersect(r, &light_interaction, shapes);
    if (!found_intersection) return Ld;
    if (&light != light_interaction.pshape) return Ld;
    lux::RGB_spectrum Li = light.le(light_interaction, -wi_world);
    if (!Li.is_black()) Ld += f * Li * weight / scattering_pdf;
  }

  return Ld;
}

lux::RGB_spectrum uniform_sample_one_light(const lux::Surface_interaction & interaction, 
                                                 lux::Sampler & sampler)
{
  // Randomly choose a light to sample
  unsigned num_lights = lights.size();
  if (num_lights == 0) return lux::RGB_spectrum(0.0f);
  unsigned light_index = std::min(static_cast<unsigned>(sampler.get_1D() * num_lights),
                                  num_lights - 1);
  const lux::Shape * plight = lights[light_index];

  lux::Vec2 light_sample(sampler.get_2D());
  lux::Vec2 scattering_sample(sampler.get_2D());

  return num_lights * estimate_direct(interaction, scattering_sample, *plight, light_sample);
}

bool intersect(const lux::Ray & ray, lux::Surface_interaction * psurface_interaction,
               std::vector<lux::Shape*> & shapes)
{
  float hit_parameter = 0.0f;
  std::vector<lux::Shape*>::const_iterator iter = shapes.cbegin();
  for(; iter != shapes.cend(); ++iter) {
    if ((*iter)->intersect(ray, &hit_parameter, psurface_interaction)) {
      ray.set_t_max(hit_parameter);
    }
  }
  return (hit_parameter > 0.0f) ? true : false;
}

lux::RGB_spectrum trace(const lux::Ray & r, lux::Sampler & sampler)
{
  lux::RGB_spectrum L(0.0f);
  lux::RGB_spectrum beta(1.0f);
  lux::Ray ray(r);

  lux::Material_type material_type = lux::Material_type::kdiffuse;
  for (unsigned bounces = 0; bounces != kmax_depth ; ++bounces) {
    lux::Surface_interaction surface_interaction;
    bool found_intersection = intersect(ray, &surface_interaction, shapes);
    if (!found_intersection) {
      // skybox accounts for a ambient radiance
      //L += beta * skybox(ray);
      break;
    }

    // Acount for the first intersection to be with a emissive surface 
    if (bounces == 0 || material_type == lux::Material_type::kspecular) {
      L += beta * surface_interaction.pshape->le(surface_interaction, -ray.get_direction());
     }

    // Initialize Materials' shading space
    surface_interaction.pmaterial->init_shading_space(surface_interaction);

    // Compute estimate of direct lighting on current vertex 
    L += beta * uniform_sample_one_light(surface_interaction, sampler);

    lux::Vec3 wo_world = -ray.get_direction(), wi_world;
    float pdf;
    lux::RGB_spectrum f = surface_interaction.pmaterial->sample_f(wo_world, &wi_world,
                                                                        sampler.get_2D(), &pdf);

    // Check if this condition makes sense for light! do lights scatter light?
    if (f.is_black() || pdf == 0.0f) {
      break;
    }
    beta *= f * lux::abs_dot(wi_world, surface_interaction.n) / pdf;
    ray = lux::Ray(surface_interaction.hit_point, lux::normalize(wi_world));

    material_type = surface_interaction.pmaterial->get_type();
    // Russian Roullete
    if (bounces > 3) {
      //std::cout << "Exit at bounce = " << bounces;
      const float q = std::max(0.05f, 1 - beta.y());
      if (sampler.get_1D() < q) break;
      beta /= 1 - q;
    }
  }
  return L;
}

int main(int argc, char * argv[])
{
  std::shared_ptr<lux::Lambertian> lambertian_red;
  std::shared_ptr<lux::Lambertian> lambertian_blue;
  std::shared_ptr<lux::Lambertian> lambertian_white;
  std::shared_ptr<lux::Mirror> mirror;

  lambertian_red = std::make_shared<lux::Lambertian>(lux::RGB_spectrum(.75f, .25f, .25f));
  lambertian_blue = std::make_shared<lux::Lambertian>(lux::RGB_spectrum(.25f, .25f, .75f));
  lambertian_white = std::make_shared<lux::Lambertian>(lux::RGB_spectrum(.75f));
  mirror = std::make_shared<lux::Mirror>(lux::RGB_spectrum(0.999f));

  // create Box
  const float kbox_width = 4.0f;
  const float khalf_box_width = kbox_width / 2.0f;

  const lux::Vec3 kfloor[4] = {
    lux::Vec3(khalf_box_width, 0.0f, -khalf_box_width),
    lux::Vec3(-khalf_box_width, 0.0f, -khalf_box_width),
    lux::Vec3(-khalf_box_width, 0.0f, khalf_box_width),
    lux::Vec3(khalf_box_width, 0.0f, khalf_box_width)
  };

  // floor
  lux::RGB_spectrum kblack(0.0f);
  shapes.push_back(new lux::Triangle(lux::Transform(), lambertian_white,
                                           kblack, kfloor[0], kfloor[1], kfloor[2]));
  shapes.push_back(new lux::Triangle(lux::Transform(), lambertian_white,
                                           kblack, kfloor[2], kfloor[3], kfloor[0]));
  // top
  lux::Vec3 delta(0, kbox_width, 0.0f);
  lux::Transform R = lux::rotate_x(180.0f);
  lux::Transform T = lux::translate(delta);
  lux::Transform obj_to_world = R * T;
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_white,
                                           kblack, kfloor[0], kfloor[1], kfloor[2]));
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_white,
                                           kblack, kfloor[2], kfloor[3], kfloor[0]));

  // left
  delta = lux::Vec3(-khalf_box_width, khalf_box_width, 0.0f);
  R = lux::rotate_z(-90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_red,
                                           kblack, kfloor[0], kfloor[1], kfloor[2]));
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_red,
                                           kblack, kfloor[2], kfloor[3], kfloor[0]));
  // right
  delta = lux::Vec3(khalf_box_width, khalf_box_width, 0.0f);
  R = lux::rotate_z(90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_blue, kblack,
                                           kfloor[0], kfloor[1], kfloor[2]));
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_blue, kblack,
                                           kfloor[2], kfloor[3], kfloor[0]));

  // back
  delta = lux::Vec3(0.0f, khalf_box_width, khalf_box_width);
  R = lux::rotate_x(-90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_white, kblack,
                                           kfloor[0], kfloor[1], kfloor[2]));
  shapes.push_back(new lux::Triangle(obj_to_world, lambertian_white, kblack,
                                           kfloor[2], kfloor[3], kfloor[0]));

  const float kradius = 0.7f;
  const float klight_radius = 0.18f;
  const lux::Vec3 light_sphere_pos(0.0f, kbox_width - (klight_radius * 1.6f), 0.0f);
  const lux::Vec3 sphere_pos(1.0f, kradius, 0.0f);
  std::shared_ptr<lux::Lambertian> lambertian;

  lambertian = std::make_shared<lux::Lambertian>(lux::RGB_spectrum(1.0f));

  lux::Shape *plight = new lux::Sphere(lux::translate(light_sphere_pos),
                                                   lambertian, lux::RGB_spectrum(115.0f), klight_radius);

  lights.push_back(plight);
  shapes.push_back(plight);
  shapes.push_back(new lux::Sphere(lux::translate(sphere_pos),lambertian, kblack, kradius));
  shapes.push_back(new lux::Sphere(lux::translate(lux::Vec3(-0.8f, kradius, khalf_box_width * 0.5f)),
                                         mirror, kblack, kradius));
  

  // Set up image to render
  const float kfov = 51.3f;
  lux::Vec3 eye(0.0f, kbox_width * 0.56f, -khalf_box_width - 3.8f);
  lux::Vec3 look(0.0f, kbox_width * 0.52f, khalf_box_width + 3.8f);
  lux::Transform cam_to_world = look_at(eye, look);
  //std::cout << cam_to_world << std::endl;
  lux::Vec2 resolution(600, 600);
  lux::Camera cam(resolution, cam_to_world, kfov);
  rgb rendered_image[static_cast<int>(resolution.x)][static_cast<int>(resolution.y)];

  lux::Ray ray;

  // 256 samples = 16 x 16
  // 64 samples = 8 x 8
  const int ksamples_per_pixel = 64;
  const int ksamples_x = 8;
  const int ksamples_y = 8;

  const float kinv_samples_per_pixel = 1.0f / static_cast<float>(ksamples_per_pixel);

  lux::Stratified_sampler stratified_sampler(ksamples_x, ksamples_y, 2, true);
  lux::Stratified_sampler monte_carlo_sampler(ksamples_x, ksamples_y, kmax_depth * 3, true);

  const int num_pixels = resolution.x * resolution.y;
  std::string progress_bar("\r[");
  progress_bar += std::string(100, '-') + "]";

   
  float prev = 0.0f;

  lux::Vec2 (*pfilter) (const lux::Vec2 &) = lux::box_filter;
  lux::Camera_sample camera_sample;
  for (unsigned h = 0; h != resolution.y; ++h) {
    for (unsigned w = 0; w != resolution.x; ++w) {
      stratified_sampler.start_pixel(w, h);
      monte_carlo_sampler.start_pixel(w,h);
      lux::RGB_spectrum pixel_color;
      lux::RGB_spectrum sample_color;

      do {
        const lux::Vec2 k2D_sample = stratified_sampler.get_2D();
        const lux::Vec2 k2D_filtered_sample = pfilter(k2D_sample);

        camera_sample.raster_coord = lux::Vec2(w + k2D_filtered_sample.x,
                                                     h + (1.0f - k2D_filtered_sample.y));  
        camera_sample.lens_coord = stratified_sampler.get_2D();

        ray = cam.generate_ray(camera_sample);

        sample_color = lux::clamp(trace(ray, monte_carlo_sampler));
        pixel_color += sample_color * kinv_samples_per_pixel; 

        monte_carlo_sampler.start_next_sample();
      } while (stratified_sampler.start_next_sample());
      
      rendered_image[w][h].r = static_cast<int>(sqrt(pixel_color[0]) * 255.9f);
      rendered_image[w][h].g = static_cast<int>(sqrt(pixel_color[1]) * 255.9f);
      rendered_image[w][h].b = static_cast<int>(sqrt(pixel_color[2]) * 255.9f);

      int pixels_written = h * resolution.x + w + 1;
      float remaining = static_cast<float>(pixels_written * 100) / num_pixels;
      if (remaining  - prev >= 0.2f) {
        progress_bar[remaining +2] = '+';
        fputs(progress_bar.c_str(), stdout);
        fflush(stdout);
        //progress_bar.replace(1, remaining, remaining, '#');
        // std::cout << "\r" << progress_bar << " " <<  std::setprecision(4) << remaining 
        //   << std::flush;
        prev = remaining;
      }
    }
  }

  const std::string file_name = "parallel_cornell_box_" + std::to_string(ksamples_per_pixel) + ".ppm";
  std::ofstream file(file_name);
  file << "P3\n" << static_cast<int>(resolution.x) << " " << static_cast<int>(resolution.y)
       << "\n255\n";
  for (unsigned h = 0; h != resolution.y; ++h) {
    for (unsigned w = 0; w != resolution.x; ++w) {
      file << rendered_image[w][h].r << " "
           << rendered_image[w][h].g << " "
           << rendered_image[w][h].b << '\n';
    }
  }

  std::cout << std::endl;

  return 0;
}
