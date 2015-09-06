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

  const CGnuPlotCameraP &camera() const { return camera_; }
  void setCamera(const CGnuPlotCameraP &c) { camera_ = c; }
  void unsetCamera() { camera_ = CGnuPlotCameraP(); }

  int width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  int height() const { return height_; }
  void setHeight(int h) { height_ = h; }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &region) { region_ = region; }

  const CGnuPlotMargin &margin() const { return margin_; }
  void setMargin(const CGnuPlotMargin &m) { margin_ = m; }

  const CBBox2D &range() const { return range_; }
  void setRange(const CBBox2D &r) { range_ = r; }

  const COptReal &ratio() const { return ratio_; }
  void setRatio(const COptReal &r) { ratio_ = r; }
  void unsetRatio() { ratio_.setInvalid(); }

  bool mapping() const { return mapping_; }
  void setMapping(bool b) { mapping_ = b; }

  const CBBox2D &clip() const { return clip_; }
  void setClip(const CBBox2D &r) { clip_ = r; }

  bool reverseX() const { return reverseX_; }
  bool reverseY() const { return reverseY_; }
  void setReverse(bool reverseX, bool reverseY) { reverseX_ = reverseX; reverseY_ = reverseY; }

  virtual bool isPseudo() const { return false; }

  virtual bool isInside(const CPoint2D &) const { return true; }

  virtual void setCBValue(double) { }

  //---

  virtual CFontPtr getFont() const { return font_; }
  virtual void setFont(CFontPtr font);

  virtual double fontSize() const;
  virtual void setFontSize(double s);

  virtual void setAntiAlias(bool) { }

  virtual void clear(const CRGBA &c) = 0;

  virtual void drawPoint(const CPoint2D &p, const CRGBA &c) = 0;

  virtual void drawSymbol(const CPoint2D &p, SymbolType type, double size,
                          const CRGBA &c, double lw=0) = 0;

  virtual void drawLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                        const CRGBA &c, const CLineDash &dash=CLineDash()) = 0;
  virtual void drawPath(const std::vector<CPoint2D> &points, double width,
                        const CRGBA &c, const CLineDash &dash=CLineDash()) = 0;

  virtual void drawRect   (const CBBox2D &rect, const CRGBA &c, double w) = 0;
  virtual void fillRect   (const CBBox2D &rect, const CRGBA &c) = 0;
  virtual void patternRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
                           const CRGBA &fg, const CRGBA &bg) = 0;

  virtual void drawPolygon   (const std::vector<CPoint2D> &points, double w, const CRGBA &c) = 0;
  virtual void fillPolygon   (const std::vector<CPoint2D> &points, const CRGBA &c) = 0;
  virtual void patternPolygon(const std::vector<CPoint2D> &points,
                              CGnuPlotTypes::FillPattern pattern,
                              const CRGBA &fg, const CRGBA &bg) = 0;

  virtual void drawEllipse(const CPoint2D &p, double rx, double ry, double a,
                           const CRGBA &c, double w, const CLineDash &dash=CLineDash()) = 0;
  virtual void fillEllipse(const CPoint2D &p, double rx, double ry, double a,
                           const CRGBA &c) = 0;

  virtual void drawBezier(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                          const CPoint2D &p4, double width, const CRGBA &c) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) = 0;

  virtual void drawRotatedText(const CPoint2D &p, const std::string &text, double ta,
                               CHAlignType halign, CVAlignType valign, const CRGBA &c) = 0;

  virtual void drawPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
                            double angle2, double width, const CRGBA &c) = 0;
  virtual void fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
                            double angle2, const CRGBA &c) = 0;

  virtual void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
                       const CRGBA &c) = 0;

  virtual void drawChord(const CPoint2D &p, double r, double a1, double a2,
                         const CRGBA &c) = 0;
  virtual void drawChord(const CPoint2D &p, double r, double a11, double a12,
                         double a21, double a22, const CRGBA &c) = 0;

  //---

  virtual void drawPixelLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                             const CRGBA &c, const CLineDash &dash=CLineDash());

  //---

  void drawPoint(const CPoint3D &p, const CRGBA &c);

  void drawClippedPath(const std::vector<CPoint2D> &points, double width,
                       const CRGBA &c, const CLineDash &dash=CLineDash());
  void drawClippedPath(const std::vector<CPoint3D> &points, double width,
                       const CRGBA &c, const CLineDash &dash=CLineDash());

  void drawPath  (const std::vector<CPoint3D> &points, double width,
                  const CRGBA &c, const CLineDash &dash=CLineDash());
  void drawSymbol(const CPoint3D &p, SymbolType type, double size,
                  const CRGBA &c, double lw);
  void drawLine  (const CPoint3D &p1, const CPoint3D &p2, double width,
                  const CRGBA &c, const CLineDash &dash=CLineDash());

  //---

  void windowToPixel(const CPoint2D &w, CPoint2D &p);
  void pixelToWindow(const CPoint2D &p, CPoint2D &w);

  void windowToPixel(double x, double y, double *px, double *py);
  void pixelToWindow(double px, double py, double *wx, double *wy);

  void pixelToWindowNoMargin(double px, double py, double *wx, double *wy);

  //---

  void drawClippedRect   (const CBBox2D &rect, const CRGBA &c, double w);
  void fillClippedRect   (const CBBox2D &rect, const CRGBA &c);
  void patternClippedRect(const CBBox2D &rect, CGnuPlotTypes::FillPattern pattern,
                          const CRGBA &fg, const CRGBA &bg);

  void drawRotatedRect(const CBBox2D &rect, double a, const CRGBA &c, double w,
                       const COptPoint2D &o=COptPoint2D());

  void fillEllipse(const CBBox2D &rect, const CRGBA &c);
  void drawEllipse(const CBBox2D &rect, const CRGBA &c, double w,
                   const CLineDash &dash=CLineDash());

  void fillPolygon(const std::vector<CPoint3D> &points, const CRGBA &c);
  void drawPolygon(const std::vector<CPoint3D> &points, double lw, const CRGBA &c);

  void drawClippedPolygon   (const std::vector<CPoint2D> &points, double w, const CRGBA &c);
  void fillClippedPolygon   (const std::vector<CPoint2D> &points, const CRGBA &c);
  void patternClippedPolygon(const std::vector<CPoint2D> &points, CGnuPlotTypes::FillPattern pat,
                             const CRGBA &fg, const CRGBA &bg);

  void drawClipLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                    const CRGBA &c, const CLineDash &dash=CLineDash());

  bool clipLine(CPoint2D &p1, CPoint2D &p2);

  void drawEllipse(const CPoint3D &p, double dx, double ry, double a,
                   const CRGBA &c, double w, const CLineDash &dash=CLineDash());

  void drawHAlignedText(const CPoint3D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c, double a=0);
  void drawHAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c, double a=0);

  void drawVAlignedText(const CPoint3D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c, double a=0);
  void drawVAlignedText(const CPoint2D &pos, CHAlignType halign, double x_offset,
                        CVAlignType valign, double y_offset, const std::string &str,
                        const CRGBA &c, double a=0);

  void drawHTextInBox(const CBBox2D &bbox, const std::string &str,
                      CHAlignType halign, const CRGBA &c);

  CBBox2D getHAlignedTextBBox(const std::string &str);

  //---

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double pixelWidthToWindowWidthNoMargin  (double p);
  double pixelHeightToWindowHeightNoMargin(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

  void regionToPixel(const CPoint2D &r, CPoint2D &p);

  CPoint2D transform(const CPoint3D &p) const;

  CPoint2D rotatePoint(const CPoint2D &p, double a, const CPoint2D &o);

 private:
  void pixelToWindowI(double px, double py, double *wx, double *wy, bool margin);

 protected:
  CGnuPlotWindow  *window_ { 0 };     // current window
  CGnuPlotCameraP  camera_;           // camera
  int              width_ { 100 };    // pixel width
  int              height_ { 100 };   // pixel height
  bool             mapping_ { true }; // mapping enabled
  CBBox2D          region_;           // window region (0,0) -> (1,1)
  CGnuPlotMargin   margin_;           // margin for plot
  CBBox2D          range_;            // data range
  COptReal         ratio_;            // aspect ratio
  CBBox2D          clip_;             // clip area
  CFontPtr         font_;             // font
  bool             reverseX_ { false };
  bool             reverseY_ { false };
};

#endif
