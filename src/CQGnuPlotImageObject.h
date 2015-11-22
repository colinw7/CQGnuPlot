#ifndef CQGnuPlotImageObject_H
#define CQGnuPlotImageObject_H

#include <CGnuPlotImageObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotImageObject : public QObject, public CGnuPlotImageObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QSize   size   READ size    WRITE setSize)
  Q_PROPERTY(QPointF origin READ origin  WRITE setOrigin)
  Q_PROPERTY(QPointF center READ center  WRITE setCenter)
  Q_PROPERTY(QSizeF  delta  READ delta   WRITE setDelta)
  Q_PROPERTY(double  angle  READ angle   WRITE setAngle)
  Q_PROPERTY(bool    flipX  READ isFlipX WRITE setFlipX)
  Q_PROPERTY(bool    flipY  READ isFlipY WRITE setFlipY)

  Q_PROPERTY(bool nearest  READ isNearest  WRITE setNearest )
  Q_PROPERTY(bool original READ isOriginal WRITE setOriginal)

  Q_PROPERTY(double  x      READ x)
  Q_PROPERTY(double  y      READ y)

 public:
  CQGnuPlotImageObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotImageObject();

  QSize size() const;
  void setSize(const QSize &v);

  QPointF origin() const;
  void setOrigin(const QPointF &o);

  QPointF center() const;
  void setCenter(const QPointF &o);

  QSizeF delta() const;
  void setDelta(const QSizeF &v);

  double x() const;
  double y() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
