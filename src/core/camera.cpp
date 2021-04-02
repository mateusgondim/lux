#include "core/camera.h"

#include <cmath>

#include "core/util.h"
#include "core/math.h"
#include "core/vec2.h"
#include "core/vec3.h"
#include "core/ray.h"
#include "core/transform.h"

namespace lux {
  Camera::Camera(const Vec2 & image_resolution, const Transform & cam_to_world, const float fov,
                 const float lens_radius, const float focal_distance)
      : m_image_resolution(image_resolution),
        m_screen_window(),
        m_lens_radius(lens_radius),
        m_focal_distance(focal_distance),
        m_near(1.0f),
        m_camera_space_lower_left(),
        m_horizontal(),
        m_vertical(),
        m_camera_to_world(cam_to_world)
  {
    const float aspect_ratio = image_resolution.x / image_resolution.y;
    const float tangent_fov_over_two = tan(degrees_to_radians(fov/2.0f));

    if (aspect_ratio > 1.0f) {
      m_screen_window.p_min.x = -aspect_ratio * tangent_fov_over_two;
      m_screen_window.p_max.x = -m_screen_window.p_min.x;

      m_screen_window.p_min.y = -1.0f * tangent_fov_over_two;
      m_screen_window.p_max.y = -m_screen_window.p_min.y;
    }
    else {
      m_screen_window.p_min.y = - 1.0f / aspect_ratio * tangent_fov_over_two;
      m_screen_window.p_max.y = -m_screen_window.p_min.y;

      m_screen_window.p_min.x = -1.0f * tangent_fov_over_two;
      m_screen_window.p_max.x = -m_screen_window.p_min.x;
    }

    m_camera_space_lower_left = Vec3(m_screen_window.p_min.x, m_screen_window.p_min.y, m_near);

    m_horizontal = Vec3(2.0f * m_screen_window.p_max.x, 0.0f, 0.0f);
    m_vertical = Vec3(0.0f, 2.0f * m_screen_window.p_max.y, 0.0f);
  }

  Ray Camera::generate_ray(const Camera_sample & camera_sample) const
  {
    Vec2 normalized_raster_coord(camera_sample.raster_coord.x / (m_image_resolution.x - 1),
                                 camera_sample.raster_coord.y / (m_image_resolution.y - 1));

    Vec3 cam_space_coord = m_camera_space_lower_left + normalized_raster_coord.x * m_horizontal;
    cam_space_coord += (1.0f - normalized_raster_coord.y) * m_vertical;

    Ray ray(Vec3(0.0f, 0.0f, 0.0f), normalize(cam_space_coord));

    if (m_lens_radius > 0.0f) {
      const Vec2 klens_sample = m_lens_radius * concentric_sample_disk(camera_sample.lens_coord);
      const float khit_parameter = m_focal_distance / -ray.get_direction().z;
      const Vec3 kplane_of_focus_intersect = ray(khit_parameter);

      ray.set_origin(Vec3(klens_sample.x, klens_sample.y, 0.0f));
      ray.set_direction(normalize(kplane_of_focus_intersect - ray.get_origin()));
    }

    return m_camera_to_world.apply_on_ray(ray);
  }
}
