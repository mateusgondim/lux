#ifndef LUX_SAMPLERS_STRATIFIED_H_
#define LUX_SAMPLERS_STRATIFIED_H_

#include <cstdint>

#include "core/pixel_sampler.h"

namespace lux {
  class Stratified_sampler : public Pixel_sampler {
    public:
      Stratified_sampler(const unsigned x_pixel_samples, const unsigned y_pixel_samples,
                         const unsigned dimensions_per_sample, const bool jittered_samples);

      virtual void start_pixel() override;

    private:
      void stratify_1D_samples(float * samples_1D);
      void stratify_2D_samples(Vec2 * samples_2D);

      const unsigned m_x_pixel_samples;
      const unsigned m_y_pixel_samples;
      const bool m_jittered_samples;
  };
}
#endif
