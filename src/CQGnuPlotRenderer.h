#ifndef CQGnuPlotRenderer_H
#define CQGnuPlotRenderer_H

#include <CGnuPlot.h>
#include <CGnuPlotRenderer.h>
#include <QColor>

class CQGnuPlotCanvas;
class QPainter;

namespace {
  inline QColor toQColor(const CRGBA &c) {
    return QColor(c.getRedI(),c.getGreenI(),c.getBlueI(),c.getAlphaI());
  }

  inline CRGBA fromQColor(const QColor &c) {
    return CRGBA(c.redF(),c.greenF(),c.blueF(),c.alphaF());
  }
}

class CQGnuPlotRenderer : public CGnuPlotRenderer {
 public:
  CQGnuPlotRenderer(CQGnuPlotCanvas *canvas);
 ~CQGnuPlotRenderer() override;

  QPainter *painter() const { return painter_; }
  void setPainter(QPainter *painter);

  void drawPoint  (const CPoint2D &p, const CRGBA &c) override;
  void drawSymbol (const CPoint2D &p, SymbolType type, double size, const CRGBA &c) override;
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c) override;
  void drawRect   (const CBBox2D &rect, const CRGBA &c) override;
  void patternRect(const CBBox2D &rect, CGnuPlot::FillPattern pattern,
                   const CRGBA &fg, const CRGBA &bg) override;
  void fillRect   (const CBBox2D &rect, const CRGBA &c) override;
  void drawBezier (const CPoint2D &p1, const CPoint2D &p2,
                   const CPoint2D &p3, const CPoint2D &p4) override;

  void drawPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c) override;
  void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c) override;

  void drawEllipse(const CPoint2D &center, double rx, double ry, const CRGBA &c) override;
  void fillEllipse(const CPoint2D &center, double rx, double ry, const CRGBA &c) override;

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) override;

  int pixelWidth () override { return pw_; }
  int pixelHeight() override { return ph_; }

  void windowToPixel(double x, double y, double *px, double *py) override;
  void pixelToWindow(double px, double py, double *wx, double *wy) override;

  double pixelWidthToWindowWidth  (double p) override;
  double pixelHeightToWindowHeight(double p) override;

  double windowWidthToPixelWidth  (double w) override;
  double windowHeightToPixelHeight(double w) override;

  void setMapping(bool b) override { mapping_ = b; }

  CFontPtr getFont() const override { return font_; }
  void setFont(CFontPtr font) override;

  void setLineDash(const CLineDash &line_dash) override;

 private:
  CQGnuPlotCanvas *canvas_;
  QPainter        *painter_;
  CFontPtr         font_;
  int              pw_, ph_;
  bool             mapping_;
};

#endif
