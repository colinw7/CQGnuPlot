#ifndef CQGnuPlotBoxObject_H
#define CQGnuPlotBoxObject_H

#include <CGnuPlotBoxObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;
class CQGnuPlotMark;

class CQGnuPlotBoxObject : public QObject, public CGnuPlotBoxObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x         READ x)
  Q_PROPERTY(double y         READ y)
  Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)
  Q_PROPERTY(double boxWidth  READ boxWidth  WRITE setBoxWidth )

  Q_PROPERTY(double minValue    READ minValue)
  Q_PROPERTY(double maxValue    READ maxValue)
  Q_PROPERTY(double medianValue READ medianValue)
  Q_PROPERTY(double lowerValue  READ lowerValue)
  Q_PROPERTY(double upperValue  READ upperValue)

 public:
  CQGnuPlotBoxObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBoxObject();

  CQGnuPlotFill   *fill       () const;
  CQGnuPlotStroke *stroke     () const;
  CQGnuPlotMark   *outlierMark() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
