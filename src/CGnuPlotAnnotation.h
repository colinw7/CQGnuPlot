#ifndef CGnuPlotAnnotation_H
#define CGnuPlotAnnotation_H

#include <CGnuPlotObject.h>

class CGnuPlotGroupAnnotation : public CGnuPlotGroupObject {
 public:
  typedef CGnuPlotTypes::ObjectType   ObjectType;
  typedef CGnuPlotTypes::VariableName VariableName;

 public:
  static const char *getName() { return "object"; }

  CGnuPlotGroupAnnotation(CGnuPlotGroup *group);

  virtual ~CGnuPlotGroupAnnotation() { }

  CGnuPlotGroupAnnotation *setData(const CGnuPlotGroupAnnotation *ann);

  virtual ObjectType type() const { return CGnuPlotTypes::ObjectType::NONE; }

  virtual VariableName varName() const { return CGnuPlotTypes::VariableName::NONE; }

  int getInd() const { return ind_; }
  void setInd(int t) { ind_ = t; }

  const COptRGBA &getStrokeColor() const { return strokeColor_; }
  void setStrokeColor(const CRGBA &c) { strokeColor_ = c; }

  const CGnuPlotColorSpec &getFillColor() const { return fillColor_; }
  void setFillColor(const CGnuPlotColorSpec &c) { fillColor_ = c; }

  const DrawLayer &getLayer() const { return layer_; }
  void setLayer(const DrawLayer &l) { layer_ = l; }

  bool isClip() const { return clip_; }
  void setClip(bool b) { clip_ = b; }

  void autoSetInd();

  virtual void initClip() = 0;

  virtual bool inside(const CGnuPlotMouseEvent &p) const = 0;

  virtual void print(std::ostream &os) const = 0;

 protected:
  int               ind_   { -1 };
  COptRGBA          strokeColor_;
  CGnuPlotColorSpec fillColor_;
  DrawLayer         layer_ { CGnuPlotTypes::DrawLayer::BACK };
  bool              clip_ { true };
};

typedef std::shared_ptr<CGnuPlotGroupAnnotation> CGnuPlotGroupAnnotationP;

#endif
