#include "integrators/path_tracer.h"

#include "core/rgb_spectrum.h"
#include "core/ray.h"
#include "core/sampler.h"
#include "core/material.h"
#include "core/scene.h"

namespace lux {
  Path_tracer::Path_tracer(Sampler * psampler, unsigned max_depth)
    : m_psampler(psampler), m_kmax_depth(max_depth)
  {
    psampler -> start_pixel(0, 0);
  }

  RGB_spectrum Path_tracer::li(const Scene & scene, const Ray & r)
  {
    RGB_spectrum L(0.0f);
    RGB_spectrum beta(1.0f);
    Ray ray(r);

    Material_type material_type = Material_type::kdiffuse;
    for (unsigned bounces = 0; bounces != m_kmax_depth ; ++bounces) {
      Surface_interaction surface_interaction;
      bool found_intersection = scene.intersect(ray, &surface_interaction);
      if (!found_intersection) break;

      // Acount for the first intersection to be with a emissive surface
      if (bounces == 0 || material_type == Material_type::kspecular) {
        L += beta * surface_interaction.pshape->le(surface_interaction, -ray.get_direction());
      }

      // Initialize Materials' shading space
      surface_interaction.pmaterial->init_shading_space(surface_interaction);

      // Compute estimate of direct lighting on current vertex
      L += beta * uniform_sample_one_light(scene, surface_interaction, *m_psampler);

      Vec3 wo_world = -ray.get_direction(), wi_world;
      float pdf;
      RGB_spectrum f = surface_interaction.pmaterial->sample_f(wo_world, &wi_world,
                                                               m_psampler->get_2D(), &pdf);

      if (f.is_black() || pdf == 0.0f) break;

      beta *= f * abs_dot(wi_world, surface_interaction.n) / pdf;
      ray = Ray(surface_interaction.hit_point, normalize(wi_world));

      material_type = surface_interaction.pmaterial->get_type();

      // Russian Roullete
      if (bounces > 3) {
        const float q = std::max(0.05f, 1 - beta.y());
        if (m_psampler->get_1D() < q) break;
        beta /= 1 - q;
      }
    }

    if(!m_psampler->start_next_sample()) m_psampler->start_pixel(0, 0);

    return L;
  }

  Path_tracer::~Path_tracer()
  {
    delete m_psampler;
  }
}
