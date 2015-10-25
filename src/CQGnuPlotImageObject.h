#ifndef CQGnuPlotImageObject_H
#define CQGnuPlotImageObject_H

#include <CGnuPlotImageObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotImageObject : public QObject, public CGnuPlotImageObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x READ x)
  Q_PROPERTY(double y READ y)

  Q_PROPERTY(double angle READ angle   WRITE setAngle)
  Q_PROPERTY(bool   flipY READ isFlipY WRITE setFlipY)

 public:
  CQGnuPlotImageObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotImageObject();

  double x() const { return CGnuPlotImageObject::bbox().getXMin(); }
  double y() const { return CGnuPlotImageObject::bbox().getYMin(); }

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
