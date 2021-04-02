#ifndef LUX_CORE_CAMERA_H_
#define LUX_CORE_CAMERA_H_

#include "core/vec2.h"
#include "core/vec3.h"
#include "core/ray.h"
#include "core/bounds2.h"
#include "core/transform.h"

namespace lux {

  struct Camera_sample {
    Vec2 raster_coord;
    Vec2 lens_coord;
  };

  class Camera {
    public:
      Camera(const Vec2 & image_resolution, const Transform & cam_to_world, const float fov = 90.0f,
             const float lens_radius = 0.0f, const float focal_distance = 1e6);
      Ray generate_ray(const Camera_sample & camera_sample) const;
    private:
      Vec2 m_image_resolution;
      Bounds2 m_screen_window;
      float m_lens_radius;
      float m_focal_distance;
      float m_near;
      Vec3 m_camera_space_lower_left;
      Vec3 m_horizontal;
      Vec3 m_vertical;
      Transform m_camera_to_world;
  };
}

#endif
