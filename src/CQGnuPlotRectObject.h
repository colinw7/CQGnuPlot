#ifndef CQGnuPlotRectObject_H
#define CQGnuPlotRectObject_H

#include <CGnuPlotRectObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotRectObject : public QObject, public CGnuPlotRectObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString text      READ getText      WRITE setText     )
  Q_PROPERTY(QColor  fillColor READ getFillColor WRITE setFillColor)
  Q_PROPERTY(QColor  lineColor READ getLineColor WRITE setLineColor)

 public:
  CQGnuPlotRectObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotRectObject();

  QString getText() const;
  void setText(const QString &str);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
