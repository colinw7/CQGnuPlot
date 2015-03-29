#ifndef CQGnuPlotCircle_H
#define CQGnuPlotCircle_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotCircle.h>

#include <QPointF>

class CQGnuPlotCircle : public CQGnuPlotAnnotation, public CGnuPlotCircle {
  Q_OBJECT

  Q_PROPERTY(QPointF center READ getCenter WRITE setCenter)
  Q_PROPERTY(double  radius READ getRadius WRITE setRadius)

 public:
  CQGnuPlotCircle(CQGnuPlotGroup *group);

  QPointF getCenter() const;
  void setCenter(const QPointF &p);

  virtual void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
