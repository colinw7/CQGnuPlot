#ifndef CGnuPlotStyleLabels_H
#define CGnuPlotStyleLabels_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleLabels : public CGnuPlotStyleBase {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotStyleLabels();

  int numUsing() const override { return 3; }

  bool has3D() const override { return true; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
  void draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
