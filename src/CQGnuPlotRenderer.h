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
  CQGnuPlotRenderer(CQGnuPlotCanvas *canvas=0);
 ~CQGnuPlotRenderer() override;

  void setCanvas(CQGnuPlotCanvas *canvas);

  QPainter *painter() const { return painter_; }
  void setPainter(QPainter *painter);

  void setAntiAlias(bool b) override;

  void clear(const CRGBA &c) override;

  void drawPoint  (const CPoint2D &p, const CRGBA &c) override;
  void drawSymbol (const CPoint2D &p, SymbolType type, double size,
                   const CRGBA &c, double lw, bool pixelSize) override;
  void drawPath   (const std::vector<CPoint2D> &points, double width, const CRGBA &c,
                   const CLineDash &dash) override;
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c,
                   const CLineDash &dash) override;

  void drawPixelLine(const CPoint2D &p1, const CPoint2D &p2, double width, const CRGBA &c,
                     const CLineDash &dash) override;

  void drawRect   (const CBBox2D &rect, const CRGBA &c, double width) override;
  void fillRect   (const CBBox2D &rect, const CRGBA &c) override;
  void patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
                   const CRGBA &fg, const CRGBA &bg) override;

  void drawEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c,
                   double width, const CLineDash &dash) override;
  void fillEllipse(const CPoint2D &center, double rx, double ry, double a, const CRGBA &c) override;
  void patternEllipse(const CPoint2D &center, double rx, double ry, double a,
                      CGnuPlotTypes::FillPattern pattern,
                      const CRGBA &fg, const CRGBA &bg) override;

  void drawBezier(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4,
                  double width, const CRGBA &c) override;

  void drawPolygon   (const std::vector<CPoint2D> &points, double w, const CRGBA &c,
                      const CLineDash &dash) override;
  void fillPolygon   (const std::vector<CPoint2D> &points, const CRGBA &c) override;
  void patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pattern,
                      const CRGBA &fg, const CRGBA &bg) override;

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) override;

  void drawRotatedText(const CPoint2D &p, const std::string &text, double ta,
                       CHAlignType halign, CVAlignType valign, const CRGBA &c) override;

  void drawPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
                    double w, const CRGBA &c) override;
  void fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
                    const CRGBA &c) override;

  void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
               const CRGBA &c) override;

  void drawChord(const CPoint2D &p, double r, double a1, double a2,
                 const CRGBA &c) override;

  void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                        double a21, double a22, const CRGBA &c) override;

  void setFont(const CFontPtr &font);

 private:
  CQGnuPlotCanvas *canvas_;
  QPainter        *painter_ { 0 };
};

#endif
