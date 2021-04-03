#include "core/integrator.h"

#include "core/util.h"
#include "core/vec2.h"
#include "core/sampler.h"
#include "core/material.h"
#include "core/shape.h"
#include "core/scene.h"

namespace lux {

  RGB_spectrum uniform_sample_one_light(const Scene & scene,
                                        const Surface_interaction & interaction,
                                        Sampler & sampler)
  {
    // Randomly choose a light to sample
    const std::vector<std::shared_ptr<Shape>> & lights = scene.get_lights();
    unsigned num_lights = lights.size();
    if (num_lights == 0) return RGB_spectrum(0.0f);
    unsigned light_index = std::min(static_cast<unsigned>(sampler.get_1D() * num_lights),
                                    num_lights - 1);
    const std::shared_ptr<Shape> plight = lights[light_index];

    Vec2 light_sample(sampler.get_2D());
    Vec2 scattering_sample(sampler.get_2D());

    return num_lights * estimate_direct(scene, interaction, scattering_sample, *plight,
                                        light_sample);
  }

  RGB_spectrum estimate_direct(const Scene & scene, const Surface_interaction & interaction,
                               const Vec2 & scattering_sample, const Shape & light,
                               const Vec2 & light_sample)
  {
    RGB_spectrum Ld(0.0f);

    // Sample light source
    Vec3 wi_world;
    float light_pdf;
    float scattering_pdf;
    Vec3 point_on_light;
    RGB_spectrum Li(0.0f);
    Li = light.sample_li(interaction, light_sample, &wi_world, &point_on_light, &light_pdf);
    bool reflect = dot(wi_world, interaction.n) * dot(interaction.wo_world, interaction.n) > 0;
    if (light_pdf > 0.0f && !Li.is_black() && reflect) {
      // Evaluate BRDF for the light sampling strategy
      RGB_spectrum f(0.0f);
      f = interaction.pmaterial->f(interaction.wo_world, wi_world) *
                                 abs_dot(wi_world, interaction.n);

      scattering_pdf = interaction.pmaterial->PDF(interaction.wo_world, wi_world);

      // Only if some of the incident light Li reflects back in the direction wo,
      // should we follow through
      if (!f.is_black()) {
        //TODO remove point_on_light
        const Vec3 d = point_on_light - interaction.hit_point;
        Ray shadow_ray(interaction.hit_point, normalize(d), magnitude(d) - kshadow_epsilon);

        const bool is_occluded = scene.intersect_p(shadow_ray);
        if (!is_occluded) {
          const float kweight = power_heuristic(1, light_pdf, 1, scattering_pdf);
          Ld += f * Li * kweight / light_pdf;
        }
      }
    }

    // Sample the BRDF
    RGB_spectrum f(0.0f);
    f = interaction.pmaterial->sample_f(interaction.wo_world, &wi_world, scattering_sample,
                                        &scattering_pdf);

    f *= abs_dot(wi_world, interaction.n);

    if (!f.is_black() && scattering_pdf > 0.0f) {
      float weight = 1.0f;
      Material_type material_type = interaction.pmaterial->get_type();

      // This block should only be used if the surface is NOT specular!
      if (material_type != Material_type::kspecular) {
        light_pdf = light.PDF(interaction, wi_world);
        if (light_pdf == 0.0f) return Ld;
        weight = power_heuristic(1, scattering_pdf, 1, light_pdf);
      }

      // Check if the ray intersects the light source
      Surface_interaction light_interaction;
      Ray r(interaction.hit_point, wi_world);
      bool found_intersection = scene.intersect(r, &light_interaction);
      if (!found_intersection) return Ld;
      if (&light != light_interaction.pshape) return Ld;
      RGB_spectrum Li = light.le(light_interaction, -wi_world);
      if (!Li.is_black()) Ld += f * Li * weight / scattering_pdf;
    }

    return Ld;
  }

}
