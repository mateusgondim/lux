#include "core/sampler.h"

#include <cstdint>

namespace lux {
  Sampler::Sampler(const std::uint64_t samples_per_pixel)
      : m_samples_per_pixel(samples_per_pixel), m_current_pixel_sample_index(0) {} 

  void Sampler::start_pixel() {
    m_current_pixel_sample_index = 0;
  }

  bool Sampler::start_next_sample() {
    return ++m_current_pixel_sample_index != m_samples_per_pixel;
  }
}
