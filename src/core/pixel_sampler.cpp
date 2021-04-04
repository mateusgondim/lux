#include "core/pixel_sampler.h"

#include <cstdint>

#include <vector>

#include "core/vec2.h"
#include "core/rng.h"
#include "core/error.h"

namespace lux {
  Pixel_sampler::Pixel_sampler(const std::uint64_t samples_per_pixel,
                               const unsigned dimensions_per_sample)
      : Sampler(samples_per_pixel),
        m_samples_1D(),
        m_samples_2D(),
        m_rng(),
        m_current_1D_dimension(0),
        m_current_2D_dimension(0)
  {

    // Stores samples in format samples[dimension][pixel_sample]
    for (unsigned i = 0; i != dimensions_per_sample; ++i) {
      m_samples_1D.push_back(std::vector<float>(samples_per_pixel));
      m_samples_2D.push_back(std::vector<Vec2>(samples_per_pixel));
    }
  }

  void Pixel_sampler::start_pixel() {
    m_current_1D_dimension = m_current_2D_dimension = 0;
    Sampler::start_pixel();
  }

  bool Pixel_sampler::start_next_sample() {
    m_current_1D_dimension = m_current_2D_dimension = 0;
    return Sampler::start_next_sample();
  }

  float Pixel_sampler::get_1D() {
    const std::uint64_t current_pixel_sample_index = get_current_pixel_sample_index();
    ASSERT(current_pixel_sample_index < m_samples_per_pixel,
           "You have already used all of your samples for the current pixel");

    if (m_current_1D_dimension != m_samples_1D.size()) {
      return m_samples_1D[m_current_1D_dimension++][current_pixel_sample_index];
    }
    else {
      return m_rng();
    }
  }

  Vec2 Pixel_sampler::get_2D() {
    const std::uint64_t current_pixel_sample_index = get_current_pixel_sample_index();
    ASSERT(current_pixel_sample_index < m_samples_per_pixel,
           "You have already used all of your samples for the current pixel");

    if (m_current_2D_dimension != m_samples_2D.size()) {
      return m_samples_2D[m_current_2D_dimension++][current_pixel_sample_index];
    }
    else {
      return Vec2(m_rng(), m_rng());
    }
  }
}
