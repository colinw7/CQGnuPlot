#ifndef CQGnuPlotAnnotation_H
#define CQGnuPlotAnnotation_H

#include <CGnuPlotObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotGroup;

class CQGnuPlotAnnotation : public QObject {
  Q_OBJECT

  Q_PROPERTY(QColor strokeColor READ getStrokeColor WRITE setStrokeColor)
  Q_PROPERTY(QColor fillColor   READ getFillColor   WRITE setFillColor  )
  Q_PROPERTY(QColor fillColor   READ getFillColor   WRITE setFillColor  )

  Q_PROPERTY(CQGnuPlotEnum::DrawLayerType drawLayer READ getDrawLayer WRITE setDrawLayer)

 public:
  typedef CQGnuPlotEnum::DrawLayerType DrawLayerType;

 public:
  CQGnuPlotAnnotation(CQGnuPlotGroup *group, CGnuPlotGroupAnnotation *obj);

  const CQGnuPlotGroup *qgroup() const { return qgroup_; }
  void setQGroup(CQGnuPlotGroup *p) { qgroup_ = p; }

  QColor getStrokeColor() const;
  void setStrokeColor(const QColor &c);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  DrawLayerType getDrawLayer() const;
  void setDrawLayer(const DrawLayerType &layer);

  bool mouseTip(const CPoint2D &pixel, const CPoint2D &window, CGnuPlotTipData &tip);

 private:
  CQGnuPlotGroup          *qgroup_;
  CGnuPlotGroupAnnotation *obj_;
};

#endif
