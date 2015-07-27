#ifndef CGnuPlotTicLabel_H
#define CGnuPlotTicLabel_H

#include <CGnuPlotTypes.h>

struct CGnuPlotTicLabel {
  typedef CGnuPlotTypes::AxisType AxisType;

  bool        valid;
  AxisType    id;
  int         ind;
  std::string str;

  CGnuPlotTicLabel() {
    valid = false;
    id    = AxisType::X;
    ind   = 1;
    str   = "";
  }

  bool setTicLabel(const std::string &name, const std::string &value) {
    if (! isTicLabel(name, id, ind))
      return false;

    valid = true;
    str   = value;

    return true;
  }

  static bool isTicLabel(const std::string &name, AxisType &id, int &ind) {
    if (name == "xtic"  || name == "xticlabel"  || name == "xticlabels"  ||
        name == "ytic"  || name == "yticlabel"  || name == "yticlabels"  ||
        name == "x2tic" || name == "x2ticlabel" || name == "x2ticlabels" ||
        name == "y2tic" || name == "y2ticlabel" || name == "y2ticlabels") {
      if      (name.substr(0, 4) == "xtic") {
        id = AxisType::X; ind = 1;
      }
      else if (name.substr(0, 5) == "x2tic") {
        id = AxisType::X; ind = 2;
      }
      else if (name.substr(0, 4) == "ytic") {
        id = AxisType::Y; ind = 1;
      }
      else if (name.substr(0, 5) == "y2tic") {
        id = AxisType::Y; ind = 2;
      }

      return true;
    }
    else
      return false;
  }
};

#endif
