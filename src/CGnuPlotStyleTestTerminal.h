#ifndef CGnuPlotStyleTestTerminal_H
#define CGnuPlotStyleTestTerminal_H

#include <CGnuPlotStyleBase.h>

class CGnuPlotStyleTestTerminal : public CGnuPlotStyleBase {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotStyleTestTerminal();

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
};

#endif
