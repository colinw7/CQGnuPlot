#ifndef CQGnuPlotPathObject_H
#define CQGnuPlotPathObject_H

#include <CGnuPlotPathObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotPathObject : public QObject, public CGnuPlotPathObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)
  Q_PROPERTY(bool clipped     READ isClipped     WRITE setClipped    )

 public:
  CQGnuPlotPathObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPathObject();

  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
