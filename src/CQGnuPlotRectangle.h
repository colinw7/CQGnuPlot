#ifndef CQGnuPlotRectangle_H
#define CQGnuPlotRectangle_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotRectangle.h>
#include <CQGnuPlot.h>

#include <QPointF>

class CQGnuPlotRectangle : public CQGnuPlotAnnotation, public CGnuPlotRectangle {
  Q_OBJECT

  Q_PROPERTY(QPointF               from      READ getFrom      WRITE setFrom     )
  Q_PROPERTY(QPointF               to        READ getTo        WRITE setTo       )
  Q_PROPERTY(double                lineWidth READ getLineWidth WRITE setLineWidth)
  Q_PROPERTY(CQGnuPlot::CQFillType fillType  READ fillType     WRITE setFillType )

 public:
  CQGnuPlotRectangle(CQGnuPlotGroup *group);

  QPointF getFrom() const;
  void setFrom(const QPointF &p);

  QPointF getTo() const;
  void setTo(const QPointF &p);

  double getLineWidth() const;

  CQGnuPlot::CQFillType fillType() const;
  void setFillType(const CQGnuPlot::CQFillType &type);

  void draw() const override;
};

#endif
