#ifndef LUX_MAIN_MAIN_H_
#define LUX_MAIN_MAIN_H_

#include "core/rgb_spectrum.h"

namespace lux { class Ray; class Sampler; }

lux::RGB_spectrum skybox(const lux::Ray & r);
lux::RGB_spectrum trace(const lux::Ray & r, lux::Sampler & sampler);
#endif
