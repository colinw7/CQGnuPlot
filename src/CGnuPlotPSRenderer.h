#ifndef CGnuPlotPSRenderer_H
#define CGnuPlotPSRenderer_H

#include <CGnuPlot.h>
#include <CGnuPlotRenderer.h>

class CGnuPlotPSDevice;

class CGnuPlotPSRenderer : public CGnuPlotRenderer {
 public:
  CGnuPlotPSRenderer(CGnuPlotPSDevice *device);
 ~CGnuPlotPSRenderer() override;

  std::ostream &os();

  void setFont(const CFontPtr &font) override;

  void clear(const CRGBA &c) override;

  void drawPoint  (const CPoint2D &p, const CRGBA &c) override;
  void drawSymbol (const CPoint2D &p, SymbolType type, double size,
                   const CRGBA &c, double lw, bool pixelSize) override;
  void drawPath   (const std::vector<CPoint2D> &points, const CGnuPlotStroke &stroke) override;
  void drawLine   (const CPoint2D &p1, const CPoint2D &p2, const CRGBA &c, double width,
                   const CLineDash &dash) override;

  void drawRect   (const CBBox2D &rect, const CRGBA &c, double width,
                   const CLineDash &dash) override;
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
                  const CRGBA &c, double width, const CLineDash &d) override;

  void drawPolygon(const std::vector<CPoint2D> &points, const CRGBA &c, double width,
                   const CLineDash &dash) override;
  void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c) override;

  void patternPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pattern,
                      const CRGBA &fg, const CRGBA &bg) override;

  void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) override;

  void drawRotatedText(const CPoint2D &p, const std::string &text, double ta,
                       const HAlignPos &halignPos, const VAlignPos &valignPos,
                       const CRGBA &c) override;

  void drawPieSlice(const CPoint2D &pc, double ro, double ri, double angle1, double angle2,
                    bool wedge, const CRGBA &c, double width, const CLineDash &dash) override;
  void fillPieSlice(const CPoint2D &pc, double ro, double ri, double angle1, double angle2,
                    const CRGBA &c) override;

  void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
               const CRGBA &c) override;

  void drawChord(const CPoint2D &p, double r, double a1, double a2,
                 const CRGBA &fc, const CRGBA &lc) override;

  void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                        double a21, double a22, const CRGBA &fc, const CRGBA &lc) override;

  void drawImage(const CPoint2D &, const CImagePtr &) override;

 private:
  void pointsToXY(const std::vector<CPoint2D> &points,
                  std::vector<double> &x, std::vector<double> &y);

  void windowToPixel1(const CBBox2D &w, CBBox2D &p);
  void windowToPixel1(const CPoint2D &w, CPoint2D &p);

 private:
  CGnuPlotPSDevice *device_;
};

#endif
