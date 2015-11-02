#ifndef CQGnuPlotCircle_H
#define CQGnuPlotCircle_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotCircle.h>

#include <QPointF>

class CQGnuPlotCircle : public CQGnuPlotAnnotation, public CGnuPlotCircle {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QPointF center    READ getCenter    WRITE setCenter)
  Q_PROPERTY(double  radius    READ getRadius    WRITE setRadius)
  Q_PROPERTY(double  lineWidth READ getLineWidth WRITE setLineWidth)

 public:
  CQGnuPlotCircle(CQGnuPlotGroup *group);

  QPointF getCenter() const;
  void setCenter(const QPointF &p);

  double getRadius() const;
  void setRadius(double r);

  double getLineWidth() const;
  void setLineWidth(double r);

  virtual void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
