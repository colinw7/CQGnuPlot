#ifndef CQGnuPlotPolygonObject_H
#define CQGnuPlotPolygonObject_H

#include <CGnuPlotPolygonObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotPolygonObject : public QObject, public CGnuPlotPolygonObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString text READ getText WRITE setText)

 public:
  CQGnuPlotPolygonObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPolygonObject();

  QString getText() const;
  void setText(const QString &str);

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void setFillValues  (const CQGnuPlotFill   &fill);
  void setStrokeValues(const CQGnuPlotStroke &stroke);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
