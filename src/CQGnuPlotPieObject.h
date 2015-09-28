#ifndef CQGnuPlotPieObject_H
#define CQGnuPlotPieObject_H

#include <CGnuPlotPieObject.h>
#include <CQGnuPlot.h>

#include <QColor>

using std::string;

class CQGnuPlotPlot;

class CQGnuPlotPieObject : public QObject, public CGnuPlotPieObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString name        READ getName      WRITE setName       )
  Q_PROPERTY(double  value       READ value        WRITE setValue      )
  Q_PROPERTY(double  innerRadius READ innerRadius  WRITE setInnerRadius)
  Q_PROPERTY(double  labelRadius READ labelRadius  WRITE setLabelRadius)
  Q_PROPERTY(QColor  lineColor   READ getLineColor WRITE setLineColor  )
  Q_PROPERTY(QColor  fillColor   READ getFillColor WRITE setFillColor  )
  Q_PROPERTY(bool    exploded    READ isExploded   WRITE setExploded   )

 public:
  CQGnuPlotPieObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPieObject();

  QString getName() const;
  void setName(const QString &str);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
