#ifndef LUX_CORE_MATH_H_
#define LUX_CORE_MATH_H_

namespace lux {
  const float kpi  = 3.14159265358979323846;
  const float kinv_pi = 0.31830988618379067154;
  const float kpi_over_two = 1.57079632679489661923;
  const float kpi_over_four = 0.78539816339744830961;
  const float kpi_times_two = 6.283185307f;

  inline float degrees_to_radians(const float theta) { return theta * (kpi / 180.0f) ;} 
  inline float radians_to_degrees(const float theta) { return theta * (180.0f / kpi) ;}
}
#endif
