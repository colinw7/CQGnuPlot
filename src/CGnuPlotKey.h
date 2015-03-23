#ifndef CGnuPlotKey_H
#define CGnuPlotKey_H

#include <CGnuPlotKeyData.h>
#include <COrientation.h>
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

class CGnuPlotKey {
 public:
  CGnuPlotKey(CGnuPlotGroup *group=0);

  virtual ~CGnuPlotKey();

  CGnuPlot *app() const;

  CGnuPlotGroup *group() const { return group_; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  bool isDisplayed() const { return keyData().displayed(); }
  void setDisplayed(bool b) { keyData_.setDisplayed(b); }

  bool isOutside() const { return keyData().outside(); }
  void setOutside(bool b) { keyData_.setOutside(b); }

  bool showTitle() const { return keyData().title().isValid(); }

  std::string getTitle() const { return keyData().title().getValue(""); }

  bool getFillBox() const { return keyData().opaque(); }
  void setFillBox(bool b) { keyData_.setOpaque(b); }

  bool getDrawBox() const { return keyData().hasBox(); }
  void setDrawBox(bool b) { keyData_.setBox(b); }

  bool hasLineType() const { return keyData().boxLineType().isValid(); }
  int getLineType() const { return keyData().boxLineType().getValue(-1); }

  bool isReverse() const { return keyData().reverse(); }
  void setReverse(bool b) { keyData_.setReverse(b); }

  bool isVariableTextColor() const { return keyData().textColor().isVariable(); }
  bool isIndexTextColor   () const { return keyData().textColor().isIndex(); }
  bool isRGBTextColor     () const { return keyData().textColor().isRGB(); }

  int textColorIndex() const { return keyData().textColor().index(); }
  const CRGBA &textColorRGB() const { return keyData().textColor().color(); }

  CHAlignType getHAlign() const { return keyData().halign(); }
  void setHAlign(CHAlignType a) { keyData_.setHAlign(a); }

  CVAlignType getVAlign() const { return keyData().valign(); }
  void setVAlign(CVAlignType a) { keyData_.setVAlign(a); }

  const CBBox2D &getBBox() const { return bbox_; }

  const CFontPtr &getFont() const { return keyData().font(); }
  void setFont(const CFontPtr &f) { keyData_.setFont(f); }

  virtual void draw();

 private:
  typedef std::vector<double> TickSpaces;

  CGnuPlotGroup*  group_ { 0 };
  CGnuPlotKeyData keyData_;
  CBBox2D         bbox_ { 0, 0, 1, 1};
};

typedef std::unique_ptr<CGnuPlotKey> CGnuPlotKeyP;

#endif
