#ifndef CQGnuPlotEllipseObject_H
#define CQGnuPlotEllipseObject_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotEllipseObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotEllipseObject : public CQGnuPlotObject, public CGnuPlotEllipseObject {
  Q_OBJECT

  Q_PROPERTY(QString text      READ getText      WRITE setText     )
  Q_PROPERTY(QColor  fillColor READ getFillColor WRITE setFillColor)
  Q_PROPERTY(QColor  lineColor READ getLineColor WRITE setLineColor)

 public:
  CQGnuPlotEllipseObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotEllipseObject();

  QString getText() const;
  void setText(const QString &str);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
