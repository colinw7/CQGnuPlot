#ifndef CQGnuPlotBarObject_H
#define CQGnuPlotBarObject_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotBarObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotBarObject : public CQGnuPlotObject, public CGnuPlotBarObject {
  Q_OBJECT

  Q_PROPERTY(double value       READ value                          )
  Q_PROPERTY(QColor fillColor   READ getFillColor WRITE setFillColor)
  Q_PROPERTY(bool   border      READ hasBorder    WRITE setBorder   )
  Q_PROPERTY(QColor lineColor   READ getLineColor WRITE setLineColor)

  Q_PROPERTY(CQGnuPlotEnum::FillType    fillType    READ getFillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern fillPattern READ getFillPattern WRITE setFillPattern)

 public:
  CQGnuPlotBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBarObject();

  CQGnuPlotEnum::FillType getFillType() const;
  void setFillType(const CQGnuPlotEnum::FillType &t);

  CQGnuPlotEnum::FillPattern getFillPattern() const;
  void setFillPattern(const CQGnuPlotEnum::FillPattern &p);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
