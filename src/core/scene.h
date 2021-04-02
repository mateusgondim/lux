#ifndef LUX_CORE_SCENE_H_
#define LUX_CORE_SCENE_H_

#include <vector>
#include <memory>

namespace lux { class Shape; }

namespace lux {
  class Scene {
    public:
      bool intersect;
      bool intersect_p;

    private:
      std::vector<std::shared_ptr<Shape>> m_pshapes;
  };
}


#endif
