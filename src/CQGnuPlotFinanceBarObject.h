#ifndef CQGnuPlotFinanceBarObject_H
#define CQGnuPlotFinanceBarObject_H

#include <CGnuPlotFinanceBarObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotStroke;
class CQGnuPlotMark;

class CQGnuPlotFinanceBarObject : public QObject, public CGnuPlotFinanceBarObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double value    READ value)
  Q_PROPERTY(double low      READ low)
  Q_PROPERTY(double high     READ high)
  Q_PROPERTY(double open     READ open)
  Q_PROPERTY(double close    READ close)
  Q_PROPERTY(double barWidth READ barWidth WRITE setBarWidth)

 public:
  CQGnuPlotFinanceBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotFinanceBarObject();

  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
