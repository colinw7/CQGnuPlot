#ifndef CGnuPlotObject_H
#define CGnuPlotObject_H

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotSize.h>

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

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

 protected:
  CGnuPlot *plot_ { 0 };
};

//---

class CGnuPlotPlotObject {
 public:
  CGnuPlotPlotObject(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  virtual ~CGnuPlotPlotObject() { }

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

 protected:
  CGnuPlotPlot *plot_ { 0 };
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

  virtual void draw(CGnuPlotRenderer *renderer) const = 0;

 protected:
  CGnuPlotGroup *group_ { 0 };
};

#endif
