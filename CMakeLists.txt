cmake_minimum_required(VERSION 3.12)

Project(lux CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Using C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(main_dir src/main)
set(core_dir src/core)
set(shapes_dir src/shapes)
set(materials_dir src/materials)
set(samplers_dir src/samplers)

if(DEBUG_BUILD)
  add_definitions(-DASSERTIONS_ENABLED)
endif()

if(RELEASE_BUILD)
  remove_definitions(-DASSERTIONS_ENABLED)
endif()

set(source_files ${main_dir}/main.cpp ${core_dir}/camera.cpp ${shapes_dir}/triangle.cpp
                 ${shapes_dir}/sphere.cpp ${core_dir}/sampler.cpp ${core_dir}/pixel_sampler.cpp
                 ${samplers_dir}/random.cpp  ${core_dir}/vec2.cpp ${core_dir}/filter.cpp
                 ${core_dir}/vec3.cpp ${core_dir}/transform.cpp ${samplers_dir}/stratified.cpp
                 ${core_dir}/rgb_spectrum.cpp ${core_dir}/material.cpp
                 ${core_dir}/scene.cpp ${materials_dir}/mirror.cpp)

set(include_files ${core_dir}/vec2.h ${core_dir}/vec3.h ${core_dir}/ray.h ${core_dir}/mat4.h
                  ${core_dir}/math.h ${core_dir}/shape.h ${shapes_dir}/sphere.h
                  ${shapes_dir}/triangle.h ${core_dir}/bounds2.h ${core_dir}/camera.h
                  ${core_dir}/error.h ${core_dir}/sampler.h ${core_dir}/pixel_sampler.h
                  ${core_dir}/filter.h ${samplers_dir}/random.h ${samplers_dir}/stratified.h
                  ${core_dir}/util.h ${core_dir}/transform.h ${core_dir}/material.h
                  ${materials_dir}/lambertian.h ${core_dir}/rgb_spectrum.h
                  ${materials_dir}/mirror.h ${core_dir}/scene.h ${core_dir}/integrator.h
                  ${main_dir}/main.h)


add_executable(lux ${include_files} ${source_files})
target_include_directories(lux PUBLIC src)

