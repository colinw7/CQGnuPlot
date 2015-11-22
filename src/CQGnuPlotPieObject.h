#ifndef CQGnuPlotPieObject_H
#define CQGnuPlotPieObject_H

#include <CGnuPlotPieObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotPieObject : public QObject, public CGnuPlotPieObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString name        READ getName      WRITE setName       )
  Q_PROPERTY(double  value       READ value        WRITE setValue      )
  Q_PROPERTY(double  innerRadius READ innerRadius  WRITE setInnerRadius)
  Q_PROPERTY(double  labelRadius READ labelRadius  WRITE setLabelRadius)
  Q_PROPERTY(bool    exploded    READ isExploded   WRITE setExploded   )

 public:
  CQGnuPlotPieObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPieObject();

  QString getName() const;
  void setName(const QString &str);

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
