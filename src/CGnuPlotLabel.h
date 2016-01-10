#ifndef CGnuPlotLabel_H
#define CGnuPlotLabel_H

#include <CGnuPlotAnnotation.h>
#include <CGnuPlotLabelData.h>

class CGnuPlotLabel : public CGnuPlotGroupAnnotation {
 public:
  static const char *getName() { return "label"; }

  CGnuPlotLabel(CGnuPlotGroup *group);

  virtual ~CGnuPlotLabel() { }

  CGnuPlotLabel *setData(const CGnuPlotLabel *label);

  ObjectType type() const override { return CGnuPlotTypes::ObjectType::LABEL; }

  VariableName varName() const override { return CGnuPlotTypes::VariableName::LABEL; }

  const CGnuPlotLabelData *data() const { return data_; }
  void setData(CGnuPlotLabelData *data);

  //---

  const std::string &getText() const { return data_->getText(); }
  void setText(const std::string &text) { data_->setText(text); }

  const CGnuPlotPosition &getPos() const { return data_->getPos(); }
  void setPos(const CGnuPlotPosition &p) { data_->setPos(p); }

  const CGnuPlotColorSpec &textColor() const { return data_->textColor(); }
  void setTextColor(const CGnuPlotColorSpec &c) { data_->setTextColor(c); }

  const COptReal &getAngle() const { return data_->getAngle(); }
  void setAngle(double a) { data_->setAngle(a); }
  void resetAngle() { data_->resetAngle(); }

  bool isEnhanced() const { return data_->isEnhanced(); }
  void setEnhanced(bool b) { data_->setEnhanced(b); }

  //---

  const CHAlignType &getAlign() const { return data_->getAlign(); }
  void setAlign(const CHAlignType &a) { data_->setAlign(a); }

  const CFontPtr &getFont() const { return data_->getFont(); }
  void setFont(const CFontPtr &f) { data_->setFont(f); }

  const COptPoint2D &getOffset() const { return data_->getOffset(); }
  void setOffset(const CPoint2D &o) { data_->setOffset(o); }

  int lineType() const { return data_->lineType(); }
  void setLineType(int i) { data_->setLineType(i); }

  //---

  CGnuPlotFill *boxFill() const { return data_->boxFill(); }
  void setBoxFill(const CGnuPlotFill &f) { data_->setBoxFill(f); }

  CGnuPlotStroke *boxStroke() const { return data_->boxStroke(); }
  void setBoxStroke(const CGnuPlotStroke &s) { data_->setBoxStroke(s); }

  //---

  bool showPoint() const { return data_->showPoint(); }
  void setShowPoint(bool b) { data_->setShowPoint(b); }

  int pointType() const { return data_->pointType(); }
  void setPointType(int i) { data_->setPointType(i); }

  double pointSize() const { return data_->pointSize(); }
  void setPointSize(double s) { data_->setPointSize(s); }

  double pointWidth() const { return data_->pointWidth(); }
  void setPointWidth(double s) { data_->setPointWidth(s); }

  //---

  bool hasHypertext() const { return data_->hasHypertext(); }
  void setHypertext(bool b) { data_->setHypertext(b); }

  //---

  void initClip() override;

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override;

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

  void print(std::ostream &os) const override;

 protected:
  CGnuPlotLabelData *data_;
};

typedef std::shared_ptr<CGnuPlotLabel> CGnuPlotLabelP;

#endif
