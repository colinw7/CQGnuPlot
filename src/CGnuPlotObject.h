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

//---

class CGnuPlotGroupAnnotation : public CGnuPlotGroupObject {
 public:
  static const char *getName() { return "object"; }

  CGnuPlotGroupAnnotation(CGnuPlotGroup *group) :
   CGnuPlotGroupObject(group) {
  }

  virtual ~CGnuPlotGroupAnnotation() { }

  CGnuPlotGroupAnnotation *setData(const CGnuPlotGroupAnnotation *ann) {
    ind_         = ann->ind_;
    strokeColor_ = ann->strokeColor_;
    fillColor_   = ann->fillColor_;
    layer_       = ann->layer_;

    return this;
  }

  virtual CGnuPlotTypes::ObjectType type() const { return CGnuPlotTypes::ObjectType::NONE; }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const CRGBA &getStrokeColor() const { return strokeColor_; }
  void setStrokeColor(const CRGBA &c) { strokeColor_ = c; }

  const CGnuPlotColorSpec &getFillColor() const { return fillColor_; }
  void setFillColor(const CGnuPlotColorSpec &c) { fillColor_ = c; }

  const DrawLayer &getLayer() const { return layer_; }
  void setLayer(const DrawLayer &l) { layer_ = l; }

  virtual bool inside(const CPoint2D &p) const = 0;

  virtual void print(std::ostream &os) const = 0;

 protected:
  int               ind_         { -1 };
  CRGBA             strokeColor_ { 0, 0, 0 };
  CGnuPlotColorSpec fillColor_;
  DrawLayer         layer_       { CGnuPlotTypes::DrawLayer::BACK };
};

typedef std::shared_ptr<CGnuPlotGroupAnnotation> CGnuPlotGroupAnnotationP;

#endif
