#ifndef CQGnuPlotAnnotation_H
#define CQGnuPlotAnnotation_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotGroup;

class CQGnuPlotAnnotation : public CQGnuPlotObject {
  Q_OBJECT

  Q_PROPERTY(QColor                     strokeColor READ getStrokeColor WRITE setStrokeColor)
  Q_PROPERTY(QColor                     fillColor   READ getFillColor   WRITE setFillColor  )
  Q_PROPERTY(QColor                     fillColor   READ getFillColor   WRITE setFillColor  )
  Q_PROPERTY(CQGnuPlot::CQDrawLayerType drawLayer   READ getDrawLayer   WRITE setDrawLayer  )

 public:
  CQGnuPlotAnnotation(CGnuPlotGroupAnnotation *obj);

  QColor getStrokeColor() const;
  void setStrokeColor(const QColor &c);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  CQGnuPlot::CQDrawLayerType getDrawLayer() const;
  void setDrawLayer(const CQGnuPlot::CQDrawLayerType &layer);

 private:
  CGnuPlotGroupAnnotation *obj_;
};

#endif
