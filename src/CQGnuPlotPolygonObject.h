#ifndef CQGnuPlotPolygonObject_H
#define CQGnuPlotPolygonObject_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPolygonObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotPolygonObject : public CQGnuPlotObject, public CGnuPlotPolygonObject {
  Q_OBJECT

  Q_PROPERTY(QString text      READ getText      WRITE setText     )
  Q_PROPERTY(QColor  fillColor READ getFillColor WRITE setFillColor)
  Q_PROPERTY(QColor  lineColor READ getLineColor WRITE setLineColor)

 public:
  CQGnuPlotPolygonObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPolygonObject();

  QString getText() const;
  void setText(const QString &str);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
