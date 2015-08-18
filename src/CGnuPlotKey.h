#ifndef CGnuPlotKey_H
#define CGnuPlotKey_H

#include <CGnuPlotKeyData.h>
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

  bool showTitle() const { return keyData_.title().isValid(); }

  std::string getTitle() const { return keyData_.title().getValue(""); }

  bool getFillBox() const { return keyData_.opaque(); }
  void setFillBox(bool b) { keyData_.setOpaque(b); }

  bool getDrawBox() const { return keyData_.hasBox(); }
  void setDrawBox(bool b) { keyData_.setBox(b); }

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

  const CBBox2D &getBBox() const { return bbox_; }

  const CFontPtr &getFont() const { return keyData_.font(); }
  void setFont(const CFontPtr &f) { keyData_.setFont(f); }

  const Columns &columns() const { return keyData_.columns(); }
  void setColumns(const Columns &c) { keyData_.setColumns(c); }

  virtual void draw(CGnuPlotRenderer *renderer);

 private:
  typedef std::vector<double> TickSpaces;

  CGnuPlotGroup*  group_ { 0 };
  CGnuPlotKeyData keyData_;
  CBBox2D         bbox_ { 0, 0, 1, 1};
};

typedef std::unique_ptr<CGnuPlotKey> CGnuPlotKeyP;

#endif
