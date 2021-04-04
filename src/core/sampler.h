#ifndef LUX_CORE_SAMPLER_H_
#define LUX_CORE_SAMPLER_H_

#include <cstdint>

#include "core/vec2.h"

namespace lux {
  class Sampler {
    public:
      Sampler(const std::uint64_t samples_per_pixel);

      virtual ~Sampler() {}

      virtual void start_pixel(const int x, const int y);
      virtual bool start_next_sample();

      virtual float get_1D() = 0;
      virtual Vec2  get_2D() = 0;
    protected:
      std::uint64_t get_current_pixel_sample_index() const;
      const std::uint64_t m_samples_per_pixel;
    private:
        std::uint64_t m_current_pixel_sample_index;
  };

  inline std::uint64_t Sampler::get_current_pixel_sample_index() const
  {
    return m_current_pixel_sample_index;
  }
}
#endif
