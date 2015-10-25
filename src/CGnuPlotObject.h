#ifndef CGnuPlotObject_H
#define CGnuPlotObject_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotSize.h>
#include <CGnuPlotTipData.h>

#include <CRGBA.h>
#include <CBBox2D.h>
#include <CSize2D.h>
#include <CPoint2D.h>
#include <CAlignType.h>
#include <COptVal.h>
#include <vector>
#include <memory>

class CGnuPlot;
class CGnuPlotPlot;
class CGnuPlotGroup;
class CGnuPlotRenderer;

//---

class CGnuPlotObject {
 public:
  CGnuPlotObject(CGnuPlot *app);

  virtual ~CGnuPlotObject() { }

  CGnuPlot *app() const { return app_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  bool isHighlighted() const { return highlighted_; }
  void setHighlighted(bool b) { highlighted_ = b; }

  const std::string &tipText() const { return tipText_; }
  void setTipText(const std::string &v) { tipText_ = v; }

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

  virtual const CBBox2D &bbox() const = 0;
  virtual void setBBox(const CBBox2D &bbox);

  virtual bool inside(const CGnuPlotTypes::InsideData &p) const;

  virtual CGnuPlotTipData tip() const = 0;

 protected:
  CGnuPlot    *app_         { 0     };
  bool         displayed_   { true  };
  bool         selected_    { false };
  bool         highlighted_ { false };
  std::string  tipText_     { ""    };
};

//---

class CGnuPlotGroupObject : public CGnuPlotObject {
 public:
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

 public:
  CGnuPlotGroupObject(CGnuPlotGroup *group);

  virtual ~CGnuPlotGroupObject() { }

  CGnuPlotGroup *group() const { return group_; }

 protected:
  CGnuPlotGroup *group_ { 0 };
};

//---

class CGnuPlotPlotObject : public CGnuPlotObject {
 public:
  CGnuPlotPlotObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPlotObject() { }

  CGnuPlotPlot *plot() const { return plot_; }

  bool isUsed() const { return used_; }
  void setUsed(bool b) { used_ = b; }
  bool testAndSetUsed() { if (used_) return true; used_ = true; return false; }

 protected:
  CGnuPlotPlot *plot_ { 0 };
  bool          used_ { false };
};

#endif
