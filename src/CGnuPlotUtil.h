#ifndef CGnuPlotUtil_H
#define CGnuPlotUtil_H

#include <algorithm>

namespace {
  inline double Deg2Rad(double d) { return M_PI*d/180.0; }
  inline double Rad2Deg(double d) { return 180.0*d/M_PI; }
}

namespace CGnuPlotUtil {
  inline double avg(std::initializer_list<double> l) {
    return std::accumulate(l.begin(), l.end(), 0.0)/l.size();
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
}

#endif
