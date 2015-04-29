#ifndef CQGnuPlotBarObject_H
#define CQGnuPlotBarObject_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotBarObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotBarObject : public CQGnuPlotObject, public CGnuPlotBarObject {
  Q_OBJECT

  Q_PROPERTY(double                 value       READ value)
  Q_PROPERTY(CQGnuPlot::FillType    fillType    READ getFillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlot::FillPattern fillPattern READ getFillPattern WRITE setFillPattern)
  Q_PROPERTY(QColor                 fillColor   READ getFillColor   WRITE setFillColor  )
  Q_PROPERTY(bool                   border      READ hasBorder      WRITE setBorder     )
  Q_PROPERTY(QColor                 lineColor   READ getLineColor   WRITE setLineColor  )

 public:
  CQGnuPlotBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBarObject();

  CQGnuPlot::FillType getFillType() const;
  void setFillType(const CQGnuPlot::FillType &t);

  CQGnuPlot::FillPattern getFillPattern() const;
  void setFillPattern(const CQGnuPlot::FillPattern &p);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
