#include "samplers/stratified.h"

#include "core/vec2.h"
#include "core/rng.h"
#include "core/util.h"

namespace lux {
  Stratified_sampler::Stratified_sampler(const unsigned x_pixel_samples,
                                         const unsigned y_pixel_samples,
                                         const unsigned dimensions_per_sample,
                                         const bool jittered_samples)
      : Pixel_sampler(x_pixel_samples * y_pixel_samples, dimensions_per_sample),
        m_x_pixel_samples(x_pixel_samples),
        m_y_pixel_samples(y_pixel_samples),
        m_jittered_samples(jittered_samples) {}

  void Stratified_sampler::start_pixel()
  {
    for (unsigned i = 0; i != m_samples_1D.size(); ++i) {
      stratify_1D_samples(&m_samples_1D[i][0]);
      shuffle(&m_samples_1D[i][0], m_x_pixel_samples * m_y_pixel_samples, m_rng);
    }

    for (unsigned i = 0; i != m_samples_2D.size(); ++i) {
      stratify_2D_samples(&m_samples_2D[i][0]);
      shuffle(&m_samples_2D[i][0], m_x_pixel_samples * m_y_pixel_samples, m_rng);
    }

    Pixel_sampler::start_pixel();
  }

  void Stratified_sampler::stratify_1D_samples(float * samples_1D)
  {
    const std::uint64_t knum_samples = m_x_pixel_samples * m_y_pixel_samples;
    const float kinv_num_samples = 1.0f / knum_samples;

    for (std::uint64_t i = 0; i != knum_samples; ++i) {
      float sample_value = i + (m_jittered_samples ? m_rng() : 0.5f);
      samples_1D[i] = sample_value * kinv_num_samples;
    }
  }

  void Stratified_sampler::stratify_2D_samples(Vec2 * samples_2D)
  {
    const float kinv_num_x_samples = 1.0f / m_x_pixel_samples;
    const float kinv_num_y_samples = 1.0f / m_y_pixel_samples;

    for (unsigned y = 0; y != m_y_pixel_samples; ++y) {
      for (unsigned x = 0; x != m_x_pixel_samples; ++x) {
        const float kx_sample_value = x + (m_jittered_samples ? m_rng() : 0.5f); 
        const float ky_sample_value = y + (m_jittered_samples ? m_rng() : 0.5f); 
        samples_2D->x = kx_sample_value * kinv_num_x_samples;
        samples_2D->y = ky_sample_value * kinv_num_y_samples;
        ++samples_2D;
      }
    }
  }
}
