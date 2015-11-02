#ifndef CQGnuPlotEllipse_H
#define CQGnuPlotEllipse_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotEllipse.h>

#include <QPointF>

class CQGnuPlotEllipse : public CQGnuPlotAnnotation, public CGnuPlotEllipse {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QPointF center READ getCenter WRITE setCenter)
  Q_PROPERTY(QSizeF  size   READ getSize   WRITE setSize  )
  Q_PROPERTY(double  angle  READ getAngle  WRITE setAngle )

 public:
  CQGnuPlotEllipse(CQGnuPlotGroup *group);

  QPointF getCenter() const;
  void setCenter(const QPointF &p);

  QSizeF getSize() const;
  void setSize(const QSizeF &s);

  virtual void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
