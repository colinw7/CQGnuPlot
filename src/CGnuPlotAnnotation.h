#ifndef CGnuPlotAnnotation_H
#define CGnuPlotAnnotation_H

#include <CGnuPlotObject.h>

class CGnuPlotGroupAnnotation : public CGnuPlotGroupObject {
 public:
  typedef COptValT<CRGBA> OptColor;

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

  const OptColor &getStrokeColor() const { return strokeColor_; }
  void setStrokeColor(const CRGBA &c) { strokeColor_ = c; }

  const CGnuPlotColorSpec &getFillColor() const { return fillColor_; }
  void setFillColor(const CGnuPlotColorSpec &c) { fillColor_ = c; }

  const DrawLayer &getLayer() const { return layer_; }
  void setLayer(const DrawLayer &l) { layer_ = l; }

  virtual bool inside(const CPoint2D &p) const = 0;

  virtual void print(std::ostream &os) const = 0;

 protected:
  int               ind_   { -1 };
  OptColor          strokeColor_;
  CGnuPlotColorSpec fillColor_;
  DrawLayer         layer_ { CGnuPlotTypes::DrawLayer::BACK };
};

typedef std::shared_ptr<CGnuPlotGroupAnnotation> CGnuPlotGroupAnnotationP;

#endif
