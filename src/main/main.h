#ifndef LUX_MAIN_MAIN_H_
#define LUX_MAIN_MAIN_H_

#include "core/rgb_spectrum.h"

namespace lux { class Ray; class Shape; struct Surface_interaction; 
                      class Sampler; struct Vec2; struct Vec3; }

lux::RGB_spectrum skybox(const lux::Ray & r);
bool intersect_p(const lux::Ray & ray, std::vector<lux::Shape*> & pshapes);

lux::RGB_spectrum estimate_direct(const lux::Surface_interaction & interaction, 
                                        const lux::Vec2 & scattering_sample,
                                        const lux::Shape & light,
                                        const lux::Vec2 & light_sample,
                                        lux::Sampler & sampler);

lux::RGB_spectrum uniform_sample_one_light(const lux::Surface_interaction & interaction, 
                                                 lux::Sampler & sampler);

bool intersect(const lux::Ray & ray, lux::Surface_interaction * psurface_interaction,
               std::vector<lux::Shape*> & pshapes);

lux::RGB_spectrum trace(const lux::Ray & r, lux::Sampler & sampler);

#endif
