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
#include "core/scene.h"
#include "core/integrator.h"

#include "materials/lambertian.h"
#include "materials/mirror.h"

#include "shapes/sphere.h"
#include "shapes/triangle.h"

#include "samplers/random.h"
#include "samplers/stratified.h"

#include "integrators/path_tracer.h"


struct rgb {
  int r;
  int g;
  int b;
};

const unsigned g_kmax_depth = 5;
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

  lux::Scene scene;
  // floor
  lux::RGB_spectrum kblack(0.0f);
  scene.add_shape(std::make_shared<lux::Triangle>(lux::Transform(), lambertian_white, kblack,
                                     kfloor[0], kfloor[1], kfloor[2]));
  scene.add_shape(std::make_shared<lux::Triangle>(lux::Transform(), lambertian_white, kblack,
                                     kfloor[2], kfloor[3], kfloor[0]));
  // top
  lux::Vec3 delta(0, kbox_width, 0.0f);
  lux::Transform R = lux::rotate_x(180.0f);
  lux::Transform T = lux::translate(delta);
  lux::Transform obj_to_world = R * T;
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_white, kblack,
                    kfloor[0], kfloor[1], kfloor[2]));
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_white, kblack,
                                     kfloor[2], kfloor[3], kfloor[0]));

  // left
  delta = lux::Vec3(-khalf_box_width, khalf_box_width, 0.0f);
  R = lux::rotate_z(-90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_red, kblack,
                    kfloor[0], kfloor[1], kfloor[2]));
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_red, kblack,
                                     kfloor[2], kfloor[3], kfloor[0]));
  // right
  delta = lux::Vec3(khalf_box_width, khalf_box_width, 0.0f);
  R = lux::rotate_z(90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_blue, kblack,
                    kfloor[0], kfloor[1], kfloor[2]));
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_blue, kblack,
                                     kfloor[2], kfloor[3], kfloor[0]));

  // back
  delta = lux::Vec3(0.0f, khalf_box_width, khalf_box_width);
  R = lux::rotate_x(-90.0f);
  T = lux::translate(delta);
  obj_to_world = R * T;
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_white, kblack,
                    kfloor[0], kfloor[1], kfloor[2]));
  scene.add_shape(std::make_shared<lux::Triangle>(obj_to_world, lambertian_white, kblack,
                                     kfloor[2], kfloor[3], kfloor[0]));

  const float kradius = 0.7f;
  const float klight_radius = 0.18f;
  const lux::Vec3 light_sphere_pos(0.0f, kbox_width - (klight_radius * 1.6f), 0.0f);
  const lux::Vec3 sphere_pos(1.0f, kradius, 0.0f);
  std::shared_ptr<lux::Lambertian> lambertian;

  lambertian = std::make_shared<lux::Lambertian>(lux::RGB_spectrum(1.0f));

  scene.add_shape(std::make_shared<lux::Sphere>(lux::translate(light_sphere_pos), lambertian,
                                                lux::RGB_spectrum(115.0f), klight_radius));
  scene.add_shape(std::make_shared<lux::Sphere>(lux::translate(sphere_pos),lambertian, kblack, kradius));
  scene.add_shape(std::make_shared<lux::Sphere>(lux::translate(lux::Vec3(-0.8f, kradius, khalf_box_width * 0.5f)),
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

  lux::Sampler *pintegrator_sampler = new lux::Stratified_sampler(ksamples_x, ksamples_y,
                                                                  g_kmax_depth * 3, true);

  lux::Path_tracer path_tracer(pintegrator_sampler, g_kmax_depth);

  const int num_pixels = resolution.x * resolution.y;
  std::string progress_bar("\r[");
  progress_bar += std::string(100, '-') + "]";
   
  float prev = 0.0f;

  lux::Vec2 (*pfilter) (const lux::Vec2 &) = lux::box_filter;
  lux::Camera_sample camera_sample;
  for (unsigned h = 0; h != resolution.y; ++h) {
    for (unsigned w = 0; w != resolution.x; ++w) {
      stratified_sampler.start_pixel(w, h);
      lux::RGB_spectrum pixel_color;
      lux::RGB_spectrum sample_color;

      do {
        const lux::Vec2 k2D_sample = stratified_sampler.get_2D();
        const lux::Vec2 k2D_filtered_sample = pfilter(k2D_sample);

        camera_sample.raster_coord = lux::Vec2(w + k2D_filtered_sample.x,
                                                     h + (1.0f - k2D_filtered_sample.y));  
        camera_sample.lens_coord = stratified_sampler.get_2D();

        ray = cam.generate_ray(camera_sample);

        sample_color = lux::clamp(path_tracer.li(scene, ray));
        pixel_color += sample_color * kinv_samples_per_pixel; 
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
