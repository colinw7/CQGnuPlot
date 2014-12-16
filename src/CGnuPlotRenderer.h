#ifndef CGnuPlotRenderer_H
#define CGnuPlotRenderer_H

#include <CGnuPlot.h>
#include <CLineDash.h>
#include <CBBox2D.h>
#include <CFont.h>

class CGnuPlotRenderer {
 public:
  CGnuPlotRenderer() { }

  virtual ~CGnuPlotRenderer() { }

  const CBBox2D &region() { return region_; }
  void setRegion(const CBBox2D &region) { region_ = region; }

  const CRange2D &margin() { return margin_; }
  void setMargin(const CRange2D &b) { margin_ = b; }

  const CBBox2D &range() { return range_; }
  void setRange(const CBBox2D &r) { range_ = r; }

  virtual CFontPtr getFont() const = 0;

  virtual void setFont(CFontPtr font) = 0;

  virtual void setLineDash(const CLineDash &line_dash) = 0;

  virtual void drawPoint  (const CPoint2D &p, const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawSymbol (const CPoint2D &p, CGnuPlot::SymbolType type, double size,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width=1.0,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawRect   (const CBBox2D &rect, const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawPolygon(const std::vector<CPoint2D> &points, double w,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void patternRect(const CBBox2D &rect,
                           CGnuPlot::PatternType pattern=CGnuPlot::PATTERN_NONE,
                           const CRGBA &fg=CRGBA(0,0,0), const CRGBA &bg=CRGBA(1,1,1)) = 0;
  virtual void fillRect   (const CBBox2D &rect, const CRGBA &c) = 0;
  virtual void drawBezier (const CPoint2D &p1, const CPoint2D &p2,
                           const CPoint2D &p3, const CPoint2D &p4) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text,
                        const CRGBA &c=CRGBA(0,0,0)) = 0;

  virtual int pixelWidth () = 0;
  virtual int pixelHeight() = 0;

  virtual double pixelWidthToWindowWidth  (double w) = 0;
  virtual double pixelHeightToWindowHeight(double h) = 0;

  virtual double windowWidthToPixelWidth  (double w) = 0;
  virtual double windowHeightToPixelHeight(double h) = 0;

  virtual void windowToPixel(double x, double y, double *px, double *py) = 0;
  virtual void pixelToWindow(double px, double py, double *wx, double *wy) = 0;

  virtual void setMapping(bool b) = 0;

  //---

  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                        CVAlignType valign, int y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                        CVAlignType valign, int y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));

  void drawRotatedText(const CPoint2D &p, const std::string &text,
                       const CRGBA &c=CRGBA(0,0,0), double a=90);

 protected:
  CBBox2D  region_; // window region (0,0) -> (1,1)
  CRange2D margin_; // margin for plot
  CBBox2D  range_ ; // data range
};

#endif
