#ifndef CGnuPlotPlotSize_H
#define CGnuPlotPlotSize_H

struct CGnuPlotPlotSize {
  CGnuPlotPlotSize() { }

  void reset() {
    unsetSize();
    unsetOrigin();
  }

  void unsetSize() {
    square = false;

    xsize.reset();
    ysize.reset();

    xratio.reset();
    yratio.reset();
  }

  void unsetOrigin() {
    xo.reset();
    yo.reset();
  }

  void showSize(std::ostream &os) {
    if (square)
      os << "size is scaled by 1,1\n";
    else
      os << "size is scaled by " << xsize.value_or(1) << "," << ysize.value_or(1) << "\n";

    if (! xratio && ! yratio)
      os << "No attempt to control aspect ratio\n";
    else
      os << "Try to set aspect ratio to " <<
            xratio.value_or(1) << ":" << yratio.value_or(1) << "\n";
  }

  void showOrigin(std::ostream &os) {
    os << "origin is set to " << xo.value_or(0) << ", " << yo.value_or(0) << "\n";
  }

  bool                  square { false };
  std::optional<double> xo;
  std::optional<double> yo;
  std::optional<double> xsize;
  std::optional<double> ysize;
  std::optional<double> xratio;
  std::optional<double> yratio;
};

#endif
