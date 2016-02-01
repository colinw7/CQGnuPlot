#ifndef CGnuPlotRenderer_H
#define CGnuPlotRenderer_H

#include <CGnuPlot.h>
#include <CGnuPlotTextRenderer.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>
#include <CGnuPlotMark.h>
#include <CLineDash.h>
#include <CBBox2D.h>
#include <CFont.h>
#include <CImagePtr.h>
#include <CRefPtr.h>

class CGnuPlotWindow;
class CGnuPlotFill;
class CGnuPlotStroke;
class CGnuPlotRenderer;

class CGnuPlotHiddenObject {
 public:
  typedef std::vector<CPoint2D> Points2D;

 public:
  CGnuPlotHiddenObject(const CRGBA &c=CRGBA(0,0,0)) :
   c_(c) {
  }

  virtual ~CGnuPlotHiddenObject() { }

  int mapping() const { return mapping_; }
  void setMapping(int i) { mapping_ = i; }

  virtual void draw(CGnuPlotRenderer *renderer) = 0;

  CRGBA scaledColor(CGnuPlotRenderer *renderer);

 protected:
  CRGBA c_;
  bool  mapping_ { true };
};

class CGnuPlotHiddenPatternPolygon : public CGnuPlotHiddenObject {
 public:
  typedef CGnuPlotTypes::FillPattern FillPattern;

 public:
  CGnuPlotHiddenPatternPolygon(const Points2D &points, FillPattern pattern,
                               const CRGBA &fg, const CRGBA &bg) :
   CGnuPlotHiddenObject(), points_(points), pattern_(pattern), fg_(fg), bg_(bg) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  Points2D    points_;
  FillPattern pattern_;
  CRGBA       fg_;
  CRGBA       bg_;
};

class CGnuPlotHiddenFillPolygon : public CGnuPlotHiddenObject {
 public:
  CGnuPlotHiddenFillPolygon(const Points2D &points, const CRGBA &c) :
   CGnuPlotHiddenObject(c), points_(points) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  Points2D points_;
};

class CGnuPlotHiddenDrawPolygon : public CGnuPlotHiddenObject {
 public:
  CGnuPlotHiddenDrawPolygon(const Points2D &points, double w, const CRGBA &c, const CLineDash &d) :
   CGnuPlotHiddenObject(c), points_(points), w_(w), d_(d) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  Points2D  points_;
  double    w_;
  CLineDash d_;
};

class CGnuPlotHiddenLine : public CGnuPlotHiddenObject {
 public:
  CGnuPlotHiddenLine(const CPoint2D &p1, const CPoint2D &p2, double w, const CRGBA &c,
                     const CLineDash &d) :
   CGnuPlotHiddenObject(c), p1_(p1), p2_(p2), w_(w), d_(d) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  CPoint2D  p1_, p2_;
  double    w_;
  CLineDash d_;
};

class CGnuPlotHiddenPath : public CGnuPlotHiddenObject {
 public:
  CGnuPlotHiddenPath(const Points2D &points, double w, const CRGBA &c, const CLineDash &d) :
   CGnuPlotHiddenObject(c), points_(points), w_(w), d_(d) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  Points2D  points_;
  double    w_;
  CLineDash d_;
};

class CGnuPlotHiddenHAlignedText : public CGnuPlotHiddenObject {
 public:
  typedef CGnuPlotTypes::HAlignPos HAlignPos;
  typedef CGnuPlotTypes::VAlignPos VAlignPos;

 public:
  CGnuPlotHiddenHAlignedText(const CPoint2D &pos, const HAlignPos &halignPos,
                             const VAlignPos &valignPos, const std::string &str,
                             const CFontPtr &font, const CRGBA &c, double a) :
   CGnuPlotHiddenObject(c), pos_(pos), halignPos_(halignPos), valignPos_(valignPos),
   str_(str), font_(font), a_(a) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  CPoint2D    pos_;
  HAlignPos   halignPos_;
  VAlignPos   valignPos_;
  std::string str_;
  CFontPtr    font_;
  double      a_;
};

class CGnuPlotHiddenSymbol : public CGnuPlotHiddenObject {
 public:
  typedef CGnuPlotTypes::SymbolType SymbolType;

 public:
  CGnuPlotHiddenSymbol(const CPoint2D &p, SymbolType type, double size, const CRGBA &c,
                       double lw, bool pixelSize) :
   CGnuPlotHiddenObject(c), p_(p), type_(type), size_(size), lw_(lw), pixelSize_(pixelSize) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  CPoint2D   p_;
  SymbolType type_;
  double     size_;
  double     lw_;
  bool       pixelSize_;
};

class CGnuPlotHiddenTextAtPoint : public CGnuPlotHiddenObject {
 public:
  typedef CGnuPlotTypes::HAlignPos HAlignPos;
  typedef CGnuPlotTypes::VAlignPos VAlignPos;

 public:
  CGnuPlotHiddenTextAtPoint(const CPoint2D &p, const std::string &str, bool enhanced,
                            const HAlignPos &halignPos, const VAlignPos &valignPos,
                            const CFontPtr &font, const CRGBA &c, double angle) :
   CGnuPlotHiddenObject(c), p_(p), str_(str), enhanced_(enhanced), halignPos_(halignPos),
   valignPos_(valignPos), font_(font), angle_(angle) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  CPoint2D    p_;
  std::string str_;
  bool        enhanced_;
  HAlignPos   halignPos_;
  VAlignPos   valignPos_;
  CFontPtr    font_;
  double      angle_;
};

class CGnuPlotHiddenEnhancedText : public CGnuPlotHiddenObject {
 public:
  typedef CGnuPlotTypes::HAlignPos HAlignPos;

 public:
  CGnuPlotHiddenEnhancedText(const CBBox2D &bbox, const std::string &str,
                             const HAlignPos &halignPos, const CFontPtr &font, const CRGBA &c) :
   CGnuPlotHiddenObject(c), bbox_(bbox), str_(str), halignPos_(halignPos), font_(font) {
  }

  void draw(CGnuPlotRenderer *renderer) override;

 private:
  CBBox2D     bbox_;
  std::string str_;
  HAlignPos   halignPos_;
  CFontPtr    font_;
};

typedef CRefPtr<CGnuPlotHiddenObject> CGnuPlotHiddenObjectP;

//------

class CGnuPlotRenderer : public CGnuPlotTextRenderer {
 public:
  typedef CGnuPlotTypes::SymbolType  SymbolType;
  typedef CGnuPlotTypes::FillPattern FillPattern;
  typedef CGnuPlotTypes::HAlignPos   HAlignPos;
  typedef CGnuPlotTypes::VAlignPos   VAlignPos;
  typedef std::vector<CPoint2D>      Points2D;
  typedef std::vector<CPoint3D>      Points3D;

 public:
  typedef CGnuPlotHiddenObjectP           HiddenObjP;
  typedef std::vector<HiddenObjP>         HiddenObjects;
  typedef std::map<double, HiddenObjects> ZHiddenObjects;

 public:
  CGnuPlotRenderer();

  virtual ~CGnuPlotRenderer();

  void setWindow(CGnuPlotWindow *w) { window_ = w; }
  CGnuPlotWindow *window() const { return window_; }

  CGnuPlotCamera *camera() const { return camera_; }
  void setCamera(CGnuPlotCamera *c) { camera_ = c; }
  void unsetCamera() { camera_ = 0; }

  bool is3D() const { return camera_; }

  int width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  int height() const { return height_; }
  void setHeight(int h) { height_ = h; }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &region) { region_ = region; }

  const CGnuPlotMargin &margin() const { return margin_; }
  void setMargin(const CGnuPlotMargin &m) { margin_ = m; }
  void resetMargin() { margin_.reset(); }

  const CBBox2D &range() const { return range_; }
  void setRange(const CBBox2D &r) { range_ = r; }

  const COptReal &ratio() const { return ratio_; }
  void setRatio(const COptReal &r) { ratio_ = r; }
  void unsetRatio() { ratio_.setInvalid(); }

  bool mapping() const { return mapping_; }
  void setMapping(bool b) { mapping_ = b; }

  const COptBBox2D &clip() const { return clip_; }
  void setClip(const CBBox2D &r) { clip_ = r; }
  void resetClip() { clip_.setInvalid(); }

  bool reverseX() const { return reverseX_; }
  bool reverseY() const { return reverseY_; }
  void setReverse(bool reverseX, bool reverseY) { reverseX_ = reverseX; reverseY_ = reverseY; }

  virtual bool isPseudo() const override { return false; }

  virtual bool isInside(const CPoint2D &) const { return true; }

  virtual void setCBValue(double) { }

  double hiddenZ() const { return hiddenZ_; }

  //---

  HiddenObjP fillHiddenPolygon(const Points2D &points, double z, const CGnuPlotFill &fill);
  HiddenObjP fillHiddenPolygon(double z, const Points2D &points, const CRGBA &c);

  HiddenObjP patternHiddenPolygon(const Points2D &points, double z, FillPattern pattern,
                                  const CRGBA &fg, const CRGBA &bg);

  HiddenObjP drawHiddenPolygon(double z, const Points2D &points, double w,
                               const CRGBA &c, const CLineDash &d);

  HiddenObjP drawHiddenLine(const CPoint3D &p1, const CPoint3D &p2, double w,
                            const CRGBA &c, const CLineDash &d);

  HiddenObjP drawHiddenLine(const CPoint2D &p1, const CPoint2D &p2,
                            double z, double w, const CRGBA &c, const CLineDash &d);

  CBBox2D drawTextBox(const CPoint2D &p, const std::string &str, bool enhanced,
                      const CPoint2D &offset, const CRGBA &bg, const CRGBA &fg, double w);

  HiddenObjP drawHiddenEnhancedText(const CBBox2D &bbox, double z, const HAlignPos &halignPos,
                                    const std::string &str, const CFontPtr &font,
                                    const CRGBA &c);
  void drawEnhancedText(const CBBox2D &bbox, const HAlignPos &halignPos, const std::string &str,
                        const CFontPtr &font, const CRGBA &c);

  HiddenObjP drawHiddenHAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                                    const VAlignPos &valignPos, const std::string &str,
                                    const CFontPtr &font, const CRGBA &c, double a);
  HiddenObjP drawHiddenHAlignedText(const CPoint2D &pos, double z, const HAlignPos &halignPos,
                                    const VAlignPos &valignPos, const std::string &str,
                                    const CFontPtr &font, const CRGBA &c, double a);

  void initHidden();
  void drawHidden(bool grayDepth);

  //---

  virtual CFontPtr getFont() const override { return font_; }
  virtual void setFont(const CFontPtr &font) override;

  virtual double fontSize() const;
  virtual void setFontSize(double s);

  virtual void setAntiAlias(bool) { }

  virtual void clear(const CRGBA &c) = 0;

  virtual void drawPoint(const CPoint2D &p, const CRGBA &c) = 0;

  virtual void drawSymbol(const CPoint2D &p, SymbolType type, double size,
                          const CRGBA &c, double lw, bool pixelSize) = 0;

  virtual void drawLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                        const CRGBA &c, const CLineDash &dash=CLineDash()) = 0;
  virtual void drawPath(const Points2D &points, const CGnuPlotStroke &stroke) = 0;

  virtual void drawRect   (const CBBox2D &rect, const CRGBA &c, double w,
                           const CLineDash &dash=CLineDash()) = 0;
  virtual void fillRect   (const CBBox2D &rect, const CRGBA &c) = 0;
  virtual void patternRect(const CBBox2D &rect, FillPattern pattern,
                           const CRGBA &fg, const CRGBA &bg) = 0;

  virtual void drawPolygon(const Points2D &points, double w, const CRGBA &c,
                           const CLineDash &d) = 0;
  virtual void fillPolygon(const Points2D &points, const CRGBA &c) = 0;

  virtual void patternPolygon(const Points2D &points, FillPattern pattern,
                              const CRGBA &fg, const CRGBA &bg) = 0;

  virtual void drawEllipse(const CPoint2D &p, double rx, double ry, double a,
                           const CRGBA &c, double w, const CLineDash &dash=CLineDash()) = 0;
  virtual void fillEllipse(const CPoint2D &p, double rx, double ry, double a,
                           const CRGBA &c) = 0;
  virtual void patternEllipse(const CPoint2D &p, double rx, double ry, double a,
                              FillPattern pattern, const CRGBA &fg, const CRGBA &bg) = 0;

  virtual void drawBezier(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                          const CPoint2D &p4, double width, const CRGBA &c,
                          const CLineDash &dash=CLineDash()) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &text, const CRGBA &c) override = 0;

  virtual void drawRotatedText(const CPoint2D &p, const std::string &text, double ta,
                               const HAlignPos &halignPos, const VAlignPos &valignPos,
                               const CRGBA &c) override = 0;

  virtual void drawPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
                            double angle2, double width, const CRGBA &c,
                            const CLineDash &dash=CLineDash()) = 0;
  virtual void fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
                            double angle2, const CRGBA &c) = 0;

  virtual void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
                       const CRGBA &c) = 0;

  virtual void drawChord(const CPoint2D &p, double r, double a1, double a2,
                         const CRGBA &fc, const CRGBA &lc) = 0;

  virtual void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                                double a21, double a22, const CRGBA &fc, const CRGBA &lc) = 0;

  virtual void drawImage(const CPoint2D &p, const CImagePtr &image) = 0;

  //---

  virtual void drawPixelLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                             const CRGBA &c, const CLineDash &dash=CLineDash());

  //---

  void drawPoint(const CPoint3D &p, const CRGBA &c);

  void strokeClippedPath(const Points2D &points, const CGnuPlotStroke &stroke);
  void strokeClippedPath(const Points3D &points, const CGnuPlotStroke &stroke);

  void drawHiddenClippedPath(const Points3D &points, double width,
                             const CRGBA &c, const CLineDash &dash=CLineDash());

  HiddenObjP drawHiddenPath(const Points2D &points, double z, double width,
                            const CRGBA &c, const CLineDash &dash=CLineDash());

  void drawClippedPath(const Points3D &points, double width,
                       const CRGBA &c, const CLineDash &dash=CLineDash());
  void drawClippedPath(const Points2D &points, double width,
                       const CRGBA &c, const CLineDash &dash=CLineDash());

  HiddenObjP strokeHiddenPath(const Points2D &points, double z, const CGnuPlotStroke &stroke);

  void strokePath(const Points3D &points, const CGnuPlotStroke &stroke);
  void strokePath(const Points2D &points, const CGnuPlotStroke &stroke);

  void drawPath(const Points3D &points, const CGnuPlotStroke &stroke);
  void drawLine(const CPoint3D &p1, const CPoint3D &p2, double width,
                const CRGBA &c, const CLineDash &dash=CLineDash());

  void drawMark(const CPoint3D &p, const CGnuPlotMark &mark);
  void drawMark(const CPoint2D &p, const CGnuPlotMark &mark);

  HiddenObjP drawHiddenSymbol(const CPoint3D &p, SymbolType type, double size,
                              const CRGBA &c, double lw, bool pixelSize);
  HiddenObjP drawHiddenSymbol(const CPoint2D &p, double z, SymbolType type, double size,
                              const CRGBA &c, double lw, bool pixelSize);

  void drawSymbol(const CPoint3D &p, SymbolType type, double size,
                  const CRGBA &c, double lw, bool pixelSize);

  //---

  void windowToPixel(const CPoint2D &w, CPoint2D &p);
  void pixelToWindow(const CPoint2D &p, CPoint2D &w);

  void windowToPixel(double wx, double wy, double *px, double *py) override;
  void pixelToWindow(double px, double py, double *wx, double *wy) override;

  void pixelToWindowNoMargin(double px, double py, double *wx, double *wy);

  //---

  void fillClippedPolygon(const Points3D &points, const CGnuPlotFill &fill);
  void fillClippedPolygon(const Points2D &points, const CGnuPlotFill &fill);
  void fillClippedPolygon(const Points3D &points, const CRGBA &c);

  void fillPolygon(const Points3D &points, const CGnuPlotFill &fill);
  void fillPolygon(const Points2D &points, const CGnuPlotFill &fill);

  void drawClippedEllipse(const CPoint2D &p, double rx, double ry, double a,
                          const CRGBA &c, double w, const CLineDash &dash=CLineDash());
  void fillClippedEllipse(const CPoint2D &p, double rx, double ry, double a, const CRGBA &c);

  void fillClippedRect(const CBBox2D &rect, const CGnuPlotFill &fill);
  void fillRect(const CBBox2D &rect, const CGnuPlotFill &fill);

  //---

  HiddenObjP strokeHiddenPolygon(const Points2D &points, double z, const CGnuPlotStroke &stroke);

  void strokeClippedPolygon(const Points3D &points, const CGnuPlotStroke &stroke);
  void strokeClippedPolygon(const Points2D &points, const CGnuPlotStroke &stroke);

  void strokePolygon(const Points3D &points, const CGnuPlotStroke &stroke);
  void strokePolygon(const Points2D &points, const CGnuPlotStroke &stroke);

  void strokeRect(const CBBox2D &rect, const CGnuPlotStroke &stroke);

  //---

  void drawRect(const CBBox3D &rect, const CRGBA &c, double w,
                const CLineDash &dash=CLineDash());

  void drawClippedRect   (const CBBox2D &rect, const CRGBA &c, double w,
                          const CLineDash &dash=CLineDash());
  void fillClippedRect   (const CBBox2D &rect, const CRGBA &c);
  void patternClippedRect(const CBBox2D &rect, FillPattern pattern,
                          const CRGBA &fg, const CRGBA &bg);

  void drawRotatedRect(const CBBox2D &rect, double a, const CRGBA &c, double w,
                       const CLineDash &dash=CLineDash(), const COptPoint2D &o=COptPoint2D());
  void fillRotatedRect(const CBBox2D &rect, double a, const CRGBA &c,
                       const COptPoint2D &o=COptPoint2D());

  void strokeEllipse(const CPoint2D &p, double rx, double ry, double a,
                     const CGnuPlotStroke &stroke);

  void drawEllipse(const CPoint3D &p, double dx, double ry, double a,
                   const CRGBA &c, double w, const CLineDash &dash=CLineDash());
  void drawEllipse(const CBBox2D &rect, const CRGBA &c, double w,
                   const CLineDash &dash=CLineDash());

  void fillEllipse(const CPoint2D &p, double rx, double ry, double a, const CGnuPlotFill &fill);
  void fillEllipse(const CBBox2D &rect, const CRGBA &c);

  void fillClippedPolygon(const Points2D &points, const CRGBA &c);
  void fillPolygon(const Points3D &points, const CRGBA &c);

  void drawClippedPolygon(const Points3D &points, double w, const CRGBA &c,
                          const CLineDash &dash);
  void drawClippedPolygon(const Points2D &points, double w, const CRGBA &c,
                          const CLineDash &dash);
  void drawPolygon(const Points3D &points, double lw, const CRGBA &c, const CLineDash &dash);

  void patternClippedPolygon(const Points2D &points, FillPattern pat,
                             const CRGBA &fg, const CRGBA &bg);

  HiddenObjP strokeHiddenLine(const CPoint2D &p1, const CPoint2D &p2, double z,
                              const CGnuPlotStroke &stroke);

  void strokeClipLine(const CPoint2D &p1, const CPoint2D &p2, const CGnuPlotStroke &stroke);

  void drawClipLine(const CPoint2D &p1, const CPoint2D &p2, double width,
                    const CRGBA &c, const CLineDash &dash=CLineDash());

  bool clipLine(CPoint2D &p1, CPoint2D &p2);

  void calcTextRectAtPoint(const CPoint2D &pos, const std::string &str, bool enhanced,
                           const HAlignPos &halignPos, const VAlignPos &valignPos, double a,
                           CBBox2D &bbox, CBBox2D &rbbox);

  HiddenObjP drawHiddenTextAtPoint(const CPoint3D &p, const std::string &str, bool enhanced,
                                   const HAlignPos &halignPos, const VAlignPos &valignPos,
                                   const CFontPtr &font, const CRGBA &c, double angle);
  HiddenObjP drawHiddenTextAtPoint(const CPoint2D &p, double z, const std::string &str,
                                   bool enhanced, const HAlignPos &halignPos,
                                   const VAlignPos &valignPos, const CFontPtr &font,
                                   const CRGBA &c, double angle);

  void drawTextAtPoint(const CPoint3D &pos, const std::string &str, bool enhanced,
                       const HAlignPos &halignPos, const VAlignPos &valignPos,
                       const CRGBA &c, double a);
  void drawTextAtPoint(const CPoint2D &pos, const std::string &str, bool enhanced,
                       const HAlignPos &halignPos, const VAlignPos &valignPos,
                       const CRGBA &c, double a);

  void drawHAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                        const VAlignPos &valignPos, const std::string &str,
                        const CRGBA &c, double a=0);
  void drawHAlignedText(const CPoint2D &pos, const HAlignPos &halignPos,
                        const VAlignPos &valignPos, const std::string &str,
                        const CRGBA &c, double a=0);

  void drawVAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                        const VAlignPos &valignPos, const std::string &str,
                        const CRGBA &c, double a=0);
  void drawVAlignedText(const CPoint2D &pos, const HAlignPos &halignPos,
                        const VAlignPos &valignPos, const std::string &str,
                        const CRGBA &c, double a=0);

  void drawHTextInBox(const CBBox2D &bbox, const std::string &str,
                      const HAlignPos &halignPos, const CRGBA &c);

  CBBox2D getHAlignedTextBBox(const std::string &str);

  void strokePieSlice(const CPoint2D &pc, double ri, double ro, double angle1, double angle2,
                      const CGnuPlotStroke &stroke);
  void fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
                    double angle2, const CGnuPlotFill &fill);

  //---

  double pixelWidthToWindowWidth  (double p);
  double pixelHeightToWindowHeight(double p);

  double pixelWidthToWindowWidthNoMargin  (double p);
  double pixelHeightToWindowHeightNoMargin(double p);

  double windowWidthToPixelWidth  (double w);
  double windowHeightToPixelHeight(double w);

  void regionToPixel(const CPoint2D &r, CPoint2D &p);

  void windowToRegion(double wx, double wy, double *rx, double *ry);
  void regionToWindow(double rx, double ry, double *wx, double *wy);

  CPoint3D transform(const CPoint3D &p) const;

  CPoint2D transform2D(const CPoint3D &p) const;

  CPoint2D rotatePoint(const CPoint2D &p, double a, const CPoint2D &o);

 private:
  void pixelToWindowI(double px, double py, double *wx, double *wy, bool margin);

 protected:
  CGnuPlotWindow* window_ { 0 };     // current window
  CGnuPlotCamera* camera_ { 0 };     // camera
  int             width_ { 100 };    // pixel width
  int             height_ { 100 };   // pixel height
  bool            mapping_ { true }; // mapping enabled
  CBBox2D         region_;           // window region (0,0) -> (1,1)
  CGnuPlotMargin  margin_;           // margin for plot
  CBBox2D         range_;            // data range
  COptReal        ratio_;            // aspect ratio
  COptBBox2D      clip_;             // clip area
  CFontPtr        font_;             // font
  bool            reverseX_ { false };
  bool            reverseY_ { false };
  ZHiddenObjects  hiddenObjects_;
  double          hiddenZ_ { 1.0 };
};

#endif
