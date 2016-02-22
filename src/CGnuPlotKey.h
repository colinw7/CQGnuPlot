#ifndef CGnuPlotKey_H
#define CGnuPlotKey_H

#include <CGnuPlotKeyData.h>
#include <CGnuPlotMouseEvent.h>
#include <CDirectionType.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CFont.h>
#include <string>
#include <vector>
#include <memory>

#include <sys/types.h>

class CGnuPlot;
class CGnuPlotGroup;

class CGnuPlotKeyLabel {
 public:
  CGnuPlotKeyLabel(const std::string &text) :
   text_(text) {
  }

  CGnuPlotKeyLabel(const std::string &text, const CRGBA &color) :
   text_(text), color_(color) {
  }

  const std::string &text() const { return text_; }
  void setText(const std::string &v) { text_ = v; }

  const COptRGBA &color() const { return color_; }
  void setColor(const CRGBA &c) { color_ = c; }

 private:
  std::string text_;
  COptRGBA    color_;
};

//---

class CGnuPlotKey {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  typedef CGnuPlotKeyData::Columns Columns;

 public:
  CGnuPlotKey(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotKey();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  bool isDisplayed() const { return keyData_.displayed(); }
  void setDisplayed(bool b) { keyData_.setDisplayed(b); }

  bool isOutside() const { return keyData_.outside(); }
  void setOutside(bool b) { keyData_.setOutside(b); }

  bool isLMargin() const { return keyData_.lmargin(); }
  void setLMargin(bool b) { keyData_.setLMargin(b); }

  bool isRMargin() const { return keyData_.rmargin(); }
  void setRMargin(bool b) { keyData_.setRMargin(b); }

  bool isTMargin() const { return keyData_.tmargin(); }
  void setTMargin(bool b) { keyData_.setTMargin(b); }

  bool isBMargin() const { return keyData_.bmargin(); }
  void setBMargin(bool b) { keyData_.setBMargin(b); }

  bool inMargin() const { return isLMargin() || isRMargin() || isTMargin() || isBMargin(); }

  std::string title() const { return keyData_.title().getValue(""); }
  void setTitle(const std::string &s) { return keyData_.setTitle(s); }
  bool hasTitle() const { return keyData_.title().isValid(); }

  //---

  bool getFillBox() const { return keyData_.opaque(); }
  void setFillBox(bool b) { keyData_.setOpaque(b); }

  bool getDrawBox() const { return keyData_.hasBox(); }
  void setDrawBox(bool b) { keyData_.setBox(b); }

  const COptInt &boxLineType() const { return keyData_.boxLineType(); }
  void setBoxLineType(int lt) { keyData_.setBoxLineType(lt); }
  void resetBoxLineType() { keyData_.resetBoxLineType(); }

  const COptInt &boxLineStyle() const { return keyData_.boxLineStyle(); }
  void setBoxLineStyle(int lt) { keyData_.setBoxLineStyle(lt); }
  void resetBoxLineStyle() { keyData_.resetBoxLineStyle(); }

  double boxLineWidth() const { return keyData_.boxLineWidth(); }
  void setBoxLineWidth(double w) { keyData_.setBoxLineWidth(w); }

  const CGnuPlotFillStyle &boxFillStyle() const { return keyData_.boxFillStyle(); }
  void setBoxFillStyle(const CGnuPlotFillStyle &fs) { keyData_.setBoxFillStyle(fs); }

  //---

  bool hasLineType() const { return keyData_.boxLineType().isValid(); }
  int getLineType() const { return keyData_.boxLineType().getValue(-1); }

  bool isReverse() const { return keyData_.reverse(); }
  void setReverse(bool b) { keyData_.setReverse(b); }

  bool isVariableTextColor() const { return keyData_.textColor().isVariable(); }
  bool isIndexTextColor   () const { return keyData_.textColor().isIndex(); }
  bool isRGBTextColor     () const { return keyData_.textColor().isRGB(); }

  int textColorIndex() const { return keyData_.textColor().index(); }
  const CRGBA &textColorRGB() const { return keyData_.textColor().color(); }

  CHAlignType getHAlign() const { return keyData_.halign(); }
  void setHAlign(CHAlignType a) { keyData_.setHAlign(a); }

  CVAlignType getVAlign() const { return keyData_.valign(); }
  void setVAlign(CVAlignType a) { keyData_.setVAlign(a); }

  bool vertical() const { return keyData_.vertical(); }
  void setVertical(bool b) { keyData_.setVertical(b); }

  const CBBox2D &bbox() const { return bbox_; }

  const CFontPtr &getFont() const { return keyData_.font(); }
  void setFont(const CFontPtr &f) { keyData_.setFont(f); }

  bool isEnhanced() const { return keyData_.isEnhanced(); }
  void setEnhanced(bool b) { keyData_.setEnhanced(b); }

  const Columns &columns() const { return keyData_.columns(); }
  void setColumns(const Columns &c) { keyData_.setColumns(c); }

  const COptReal &sampLen() const { return keyData_.sampLen(); }
  void setSampLen(double r) { keyData_.setSampLen(r); }

  const COptReal &spacing() const { return keyData_.spacing(); }
  void setSpacing(double r) { keyData_.setSpacing(r); }

  void clearPlotRects() { prects_.clear(); }

  void addPlotRect(CGnuPlotPlot *plot, const CBBox2D &rect);

  virtual void draw(CGnuPlotRenderer *renderer);

  int plotAtPos(const CPoint2D &pos) const;

  bool inside(const CGnuPlotMouseEvent &p) const;

 private:
  void drawClustered();
  void drawColumnStacked();

  void drawBox();

  void calcBBox(const CSize2D &size);

  void drawHAlignedText(const CPoint2D &pos, const HAlignPos &halignPos, const VAlignPos &valignPos,
                        const std::string &str, const CRGBA &c);

 private:
  typedef std::vector<double>   TickSpaces;
  typedef std::map<int,CBBox2D> PlotRects;

  CGnuPlotGroup*            group_ { 0 };
  CGnuPlotKeyData           keyData_;
  CBBox2D                   bbox_ { 0, 0, 1, 1};
  mutable PlotRects         prects_;
  mutable CGnuPlotRenderer* renderer_ { 0 };
};

typedef CRefPtr<CGnuPlotKey> CGnuPlotKeyP;

#endif
