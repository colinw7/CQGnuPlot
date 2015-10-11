#ifndef CQGnuPlotBarObject_H
#define CQGnuPlotBarObject_H

#include <CGnuPlotBarObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotBarObject : public QObject, public CGnuPlotBarObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x READ xvalue)
  Q_PROPERTY(double y READ yvalue)

 public:
  CQGnuPlotBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBarObject();

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
