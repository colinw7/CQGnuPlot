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
 ~CQGnuPlotRenderer();

  void setPainter(QPainter *painter);

  void drawPoint  (const CPoint2D &p, const CRGBA &c);
  void drawSymbol (const CPoint2D &p, CGnuPlot::SymbolType type, double size, const CRGBA &c);
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c);
  void drawRect   (const CBBox2D &rect, const CRGBA &c);
  void patternRect(const CBBox2D &rect, CGnuPlot::PatternType pattern,
                   const CRGBA &fg, const CRGBA &bg);
  void fillRect   (const CBBox2D &rect, const CRGBA &c);
  void drawBezier (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4);

  void drawPolygon(const std::vector<CPoint2D> &points, double w, const CRGBA &c);
  void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c);

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c);

  int pixelWidth () { return pw_; }
  int pixelHeight() { return ph_; }

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

  void setMapping(bool b) { mapping_ = b; }

  CFontPtr getFont() const { return font_; }
  void setFont(CFontPtr font);

  void setLineDash(const CLineDash &line_dash);

 private:
  CQGnuPlotCanvas *canvas_;
  QPainter        *painter_;
  CFontPtr         font_;
  int              pw_, ph_;
  bool             mapping_;
};

#endif
