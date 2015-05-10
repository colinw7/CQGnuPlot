#ifndef CGnuPlotStyleImage_H
#define CGnuPlotStyleImage_H

#include <CGnuPlotStyleBase.h>
#include <CRGBA.h>

class CGnuPlotStyleImageBase : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleImageBase(CGnuPlotTypes::PlotStyle style);

  int numUsing() const override { return 2; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

  CBBox2D fit(CGnuPlotPlot *plot) override;

 protected:
  void drawBinaryImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer);

  void drawImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer);

  double decodeImageUsingColor(CGnuPlotPlot *plot, int col, const CRGBA &c) const;

  double indexImageColor(int i, const CRGBA &c) const;
};

class CGnuPlotStyleImage : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleImage();
};

class CGnuPlotStyleRGBImage : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleRGBImage();
};

class CGnuPlotStyleRGBAlpha : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleRGBAlpha();
};

#endif
