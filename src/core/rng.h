#ifndef LUX_CORE_RNG_H_
#define LUX_CORE_RNG_H_

// Uses the linear congruence algorithm to generate 
// a random number in the interval [0, 1).

namespace lux {
  class RNG final {
    public:
      RNG(unsigned long long seed = 7564231ULL)
      {
        m_seed = seed;
        m_mult = 62089911ULL;
        m_llong_max = 4294967295ULL;
        m_float_max = 4294967295.0f;
      }

      RNG(const RNG & rng) = default;
      ~RNG() = default;

      RNG & operator=(const RNG & rng) = default;


      float operator()()
      {
        m_seed *= m_mult;
        return static_cast<float>(m_seed % m_llong_max) / m_float_max;
      }

    private:
      unsigned long long m_seed;
      unsigned long long m_mult;
      unsigned long long m_llong_max;
      float m_float_max;
  };
}
#endif

