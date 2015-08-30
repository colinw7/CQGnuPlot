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
  CGnuPlotObject(CGnuPlot *plot) :
   plot_(plot) {
  }

  virtual ~CGnuPlotObject() { }

  CGnuPlot *plot() const { return plot_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  virtual bool inside(const CPoint2D &p) const = 0;

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

  virtual CGnuPlotTipData tip() const = 0;

 protected:
  CGnuPlot *plot_ { 0 };
  bool      displayed_ { true };
};

//---

class CGnuPlotPlotObject {
 public:
  CGnuPlotPlotObject(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  virtual ~CGnuPlotPlotObject() { }

  CGnuPlotPlot *plot() const { return plot_; }

  bool isModified() const { return modified_; }
  void setModified(bool b) { modified_ = b; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  virtual bool inside(const CPoint2D &p) const = 0;

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

  virtual CGnuPlotTipData tip() const = 0;

 protected:
  CGnuPlotPlot *plot_     { 0 };
  bool          modified_ { false };
  bool          displayed_ { true };
};

//---

class CGnuPlotGroupObject {
 public:
  typedef CGnuPlotTypes::DrawLayer DrawLayer;

 public:
  CGnuPlotGroupObject(CGnuPlotGroup *group) :
   group_(group) {
  }

  virtual ~CGnuPlotGroupObject() { }

  CGnuPlotGroup *group() const { return group_; }

  bool isDisplayed() const { return displayed_; }
  void setDisplayed(bool b) { displayed_ = b; }

  virtual bool inside(const CPoint2D &p) const = 0;

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

  virtual CGnuPlotTipData tip() const = 0;

 protected:
  CGnuPlotGroup *group_ { 0 };
  bool           displayed_ { true };
};

#endif
