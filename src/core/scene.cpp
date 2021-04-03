#include "core/scene.h"

#include <vector>
#include <memory>

#include "core/ray.h"
#include "core/shape.h"

namespace lux {
  void Scene::add_shape(std::shared_ptr<Shape> pshape)
  {
    m_shapes.push_back(pshape);
    if (pshape->is_area_light()) m_lights.push_back(pshape);

    return;
  }

  bool Scene::intersect(const Ray & ray, Surface_interaction * psurface_interaction) const
  {
    float hit_parameter = 0.0f;
    std::vector<std::shared_ptr<Shape>>::const_iterator iter = m_shapes.cbegin();
    for(; iter != m_shapes.cend(); ++iter) {
      if ((*iter)->intersect(ray, &hit_parameter, psurface_interaction)) {
        ray.set_t_max(hit_parameter);
      }
    }
    return (hit_parameter > 0.0f) ? true : false;
  }

  bool Scene::intersect_p(const Ray & ray) const
  {
    std::vector<std::shared_ptr<Shape>>::const_iterator iter = m_shapes.cbegin();
    for(; iter != m_shapes.cend(); ++iter) {
      if ((*iter)->intersect_p(ray)) {
        return true;
      }
    }
    return false;
  }

}
