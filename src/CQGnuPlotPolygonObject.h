#ifndef CQGnuPlotPolygonObject_H
#define CQGnuPlotPolygonObject_H

#include <CGnuPlotPolygonObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotPolygonObject : public QObject, public CGnuPlotPolygonObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

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
