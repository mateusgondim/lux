#ifndef LUX_CORE_SCENE_H_
#define LUX_CORE_SCENE_H_

#include <vector>
#include <memory>

namespace lux { class Ray; struct Surface_interaction; class Shape; }

namespace lux {
  class Scene final {
    public:
      Scene() = default;
      Scene(const Scene &) = delete;

      ~Scene() = default;

      Scene & operator=(const Scene &) = delete;

      void add_shape(std::shared_ptr<Shape> pshape);

      const std::vector<std::shared_ptr<Shape>> & get_shapes() const { return m_shapes; }
      const std::vector<std::shared_ptr<Shape>> & get_lights() const { return m_lights; }

      bool intersect(const Ray & ray, Surface_interaction * psurface_interaction) const;
      bool intersect_p(const Ray & ray) const;

    private:
      std::vector<std::shared_ptr<Shape>> m_shapes;
      std::vector<std::shared_ptr<Shape>> m_lights;
  };
}


#endif
