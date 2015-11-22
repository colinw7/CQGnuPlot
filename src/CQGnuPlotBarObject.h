#ifndef CQGnuPlotBoxBarObject_H
#define CQGnuPlotBoxBarObject_H

#include <CGnuPlotBoxBarObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotBoxBarObject : public QObject, public CGnuPlotBoxBarObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x READ xvalue)
  Q_PROPERTY(double y READ yvalue)

 public:
  CQGnuPlotBoxBarObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBoxBarObject();

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
