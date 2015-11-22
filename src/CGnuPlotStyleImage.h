#ifndef CGnuPlotStyleImage_H
#define CGnuPlotStyleImage_H

#include <CGnuPlotStyleBase.h>
#include <CRGBA.h>

class CGnuPlotStyleImageBase : public CGnuPlotStyleBase {
 public:
  CGnuPlotStyleImageBase(CGnuPlotTypes::PlotStyle style);

  int numUsing() const override { return 2; }

  bool has3D() const override { return true; }

  void draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;
  void draw3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer) override;

 protected:
  void drawBinaryImage(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer);

  void drawImage2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer);
  void drawImage3D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer);

  double decodeImageUsingColor(CGnuPlotPlot *plot, int col, const CRGBA &c) const;

  double indexImageColor(int i, const CRGBA &c) const;
};

class CGnuPlotStyleImage : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleImage();

  int numUsing() const override { return 2; }
};

class CGnuPlotStyleRGBImage : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleRGBImage();

  int numUsing() const override { return 5; }
};

class CGnuPlotStyleRGBAlpha : public CGnuPlotStyleImageBase {
 public:
  CGnuPlotStyleRGBAlpha();

  int numUsing() const override { return 6; }
};

#endif
