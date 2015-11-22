#ifndef CQGnuPlotErrorBarObject_H
#define CQGnuPlotErrorBarObject_H

#include <CGnuPlotErrorBarObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotStroke;
class CQGnuPlotMark;

class CQGnuPlotErrorBarObject : public QObject, public CGnuPlotErrorBarObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x         READ x)
  Q_PROPERTY(double y         READ y)
  Q_PROPERTY(bool   xDir      READ isXDirection)
  Q_PROPERTY(bool   yDir      READ isYDirection)
  Q_PROPERTY(double xLow      READ xLow)
  Q_PROPERTY(double xHigh     READ xHigh)
  Q_PROPERTY(double yLow      READ yLow)
  Q_PROPERTY(double yHigh     READ yHigh)
  Q_PROPERTY(double barWidth  READ barWidth  WRITE setBarWidth )
  Q_PROPERTY(double barHeight READ barHeight WRITE setBarHeight)

 public:
  CQGnuPlotErrorBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotErrorBarObject();

  CQGnuPlotStroke *stroke() const;
  CQGnuPlotMark   *mark  () const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
