#ifndef CGnuPlotRenderer_H
#define CGnuPlotRenderer_H

#include <CGnuPlot.h>
#include <CLineDash.h>
#include <CBBox2D.h>
#include <CFont.h>

class CGnuPlotWindow;

class CGnuPlotRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotRenderer();

  virtual ~CGnuPlotRenderer();

  void setWindow(CGnuPlotWindow *w) { window_ = w; }
  CGnuPlotWindow *window() const { return window_; }

  const CBBox2D &region() { return region_; }
  void setRegion(const CBBox2D &region) { region_ = region; }

  const CRange2D &margin() { return margin_; }
  void setMargin(const CRange2D &b) { margin_ = b; }

  const CBBox2D &range() { return range_; }
  void setRange(const CBBox2D &r) { range_ = r; }

  void setRatio(double r) { ratio_ = r; }
  void unsetRatio() { ratio_.setInvalid(); }

  bool mapping() const { return mapping_; }
  void setMapping(bool b) { mapping_ = b; }

  //---

  virtual CFontPtr getFont() const = 0;

  virtual void setFont(CFontPtr font) = 0;

  virtual void setLineDash(const CLineDash &line_dash) = 0;

  virtual void clear(const CRGBA &c) = 0;

  virtual void drawPoint  (const CPoint2D &p, const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawSymbol (const CPoint2D &p, SymbolType type, double size,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawLine   (const CPoint2D &p1, const CPoint2D &p2, double width=1.0,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawRect   (const CBBox2D &rect, const CRGBA &c=CRGBA(0,0,0), double w=1.0) = 0;
  virtual void drawPolygon(const std::vector<CPoint2D> &points, double w=1.0,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void fillPolygon(const std::vector<CPoint2D> &points, const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void patternRect(const CBBox2D &rect,
                           CGnuPlotTypes::FillPattern pattern=CGnuPlotTypes::FillPattern::NONE,
                           const CRGBA &fg=CRGBA(0,0,0), const CRGBA &bg=CRGBA(1,1,1)) = 0;
  virtual void fillRect   (const CBBox2D &rect, const CRGBA &c) = 0;
  virtual void drawBezier (const CPoint2D &p1, const CPoint2D &p2,
                           const CPoint2D &p3, const CPoint2D &p4, double width=1.0,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void drawEllipse(const CPoint2D &p, double dx, double ry, double a=0,
                           const CRGBA &c=CRGBA(0,0,0)) = 0;
  virtual void fillEllipse(const CPoint2D &p, double dx, double ry, double a=0,
                           const CRGBA &c=CRGBA(1,1,1)) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text,
                        const CRGBA &c=CRGBA(0,0,0)) = 0;

  //---

  void windowToPixel(const CPoint2D &w, CPoint2D &p);
  void pixelToWindow(const CPoint2D &p, CPoint2D &w);

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  //---

  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c=CRGBA(0,0,0));

  void drawRotatedText(const CPoint2D &p, const std::string &text,
                       const CRGBA &c=CRGBA(0,0,0), double a=90);

  //---

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

 protected:
  CGnuPlotWindow *window_ { 0 };     // current window
  bool            mapping_ { true }; // mapping enabled
  CBBox2D         region_;           // window region (0,0) -> (1,1)
  CRange2D        margin_;           // margin for plot
  CBBox2D         range_;            // data range
  COptReal        ratio_;            // aspect ratio
};

#endif
