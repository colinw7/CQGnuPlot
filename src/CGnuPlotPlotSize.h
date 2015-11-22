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

    xsize.setInvalid();
    ysize.setInvalid();

    xratio.setInvalid();
    yratio.setInvalid();
  }

  void unsetOrigin() {
    xo.setInvalid();
    yo.setInvalid();
  }

  void showSize(std::ostream &os) {
    if (square)
      os << "size is scaled by 1,1" << std::endl;
    else
      os << "size is scaled by " << xsize.getValue(1) << "," << ysize.getValue(1) << std::endl;

    if (! xratio.isValid() && ! yratio.isValid())
      os << "No attempt to control aspect ratio" << std::endl;
    else
      os << "Try to set aspect ratio to " <<
            xratio.getValue(1) << ":" << yratio.getValue(1) << std::endl;
  }

  void showOrigin(std::ostream &os) {
    os << "origin is set to " << xo.getValue(0) << ", " << yo.getValue(0) << std::endl;
  }

  bool     square { false };
  COptReal xo;
  COptReal yo;
  COptReal xsize;
  COptReal ysize;
  COptReal xratio;
  COptReal yratio;
};

#endif
