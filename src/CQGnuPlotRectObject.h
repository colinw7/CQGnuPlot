#ifndef CQGnuPlotRectObject_H
#define CQGnuPlotRectObject_H

#include <CGnuPlotRectObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotRectObject : public QObject, public CGnuPlotRectObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString text READ getText WRITE setText)

 public:
  CQGnuPlotRectObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotRectObject();

  QString getText() const;
  void setText(const QString &str);

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
