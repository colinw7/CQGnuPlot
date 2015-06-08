#ifndef CGnuPlotUtil_H
#define CGnuPlotUtil_H

#include <algorithm>

namespace CGnuPlotUtil {
  inline double avg(std::initializer_list<double> l) {
    return std::accumulate(l.begin(), l.end(), 0.0)/l.size();
  }

  template<typename T>
  inline T avgT(std::initializer_list<T> l, const T &z=T(0)) {
    T z1 = z; return std::accumulate(l.begin(), l.end(), z1)/l.size();
  }

  inline double norm(double x, double low, double high) {
    return (x - low)/(high - low);
  }

  inline double lerp(double value1, double value2, double amt) {
    return value1 + (value2 - value1)*amt;
  }

  inline double map(double value, double low1, double high1, double low2, double high2) {
    return lerp(low2, high2, norm(value, low1, high1));
  }

  inline double clamp(double val, double low, double high) {
    if (val < low ) return low;
    if (val > high) return high;
    return val;
  }

  template<typename T>
  inline T sign(const T &t) {
    if (t > T(0)) return  1;
    if (t < T(0)) return -1;
    return 0;
  }

  inline bool realEq(double r1, double r2) {
    return (fabs(r2 - r1) < 1E-6);
  }
}

#endif
