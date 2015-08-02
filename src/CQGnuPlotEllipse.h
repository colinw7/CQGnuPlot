#ifndef CQGnuPlotEllipse_H
#define CQGnuPlotEllipse_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotEllipse.h>

#include <QPointF>

class CQGnuPlotEllipse : public CQGnuPlotAnnotation, public CGnuPlotEllipse {
  Q_OBJECT

  Q_PROPERTY(QPointF center READ getCenter WRITE setCenter)
  Q_PROPERTY(double  rx     READ getRX     WRITE setRX    )
  Q_PROPERTY(double  ry     READ getRY     WRITE setRY    )
  Q_PROPERTY(double  angle  READ getAngle  WRITE setAngle )

 public:
  CQGnuPlotEllipse(CQGnuPlotGroup *group);

  QPointF getCenter() const;
  void setCenter(const QPointF &p);

  virtual void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
