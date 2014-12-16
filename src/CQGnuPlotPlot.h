#ifndef CQGnuPlotPlot_H
#define CQGnuPlotPlot_H

#include <CGnuPlotPlot.h>
#include <QObject>
#include <QColor>

class CQGnuPlotWindow;
class CQGnuPlotRenderer;
class CQGnuPlotCanvas;
class QPainter;

class CQGnuPlotPlot : public QObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_PROPERTY(double regionLeft   READ getRegionLeft   WRITE setRegionLeft  )
  Q_PROPERTY(double regionRight  READ getRegionRight  WRITE setRegionRight )
  Q_PROPERTY(double regionTop    READ getRegionTop    WRITE setRegionTop   )
  Q_PROPERTY(double regionBottom READ getRegionBottom WRITE setRegionBottom)

  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)

  Q_PROPERTY(double xmin READ getXMin WRITE setXMin)
  Q_PROPERTY(double ymin READ getYMin WRITE setYMin)
  Q_PROPERTY(double xmax READ getXMax WRITE setXMax)
  Q_PROPERTY(double ymax READ getYMax WRITE setYMax)

  Q_PROPERTY(QString xlabel  READ getXLabel WRITE setXLabel)
  Q_PROPERTY(QString ylabel  READ getYLabel WRITE setYLabel)
  Q_PROPERTY(bool    xtics   READ getXTics WRITE setXTics)
  Q_PROPERTY(bool    ytics   READ getYTics WRITE setYTics)
  Q_PROPERTY(int     borders READ getBorders WRITE setBorders)

  Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
  Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

 public:
  CQGnuPlotPlot(CQGnuPlotWindow *window);

  CQGnuPlotCanvas *canvas() const;

  void setPainter(QPainter *p);

  QString getXLabel() const { return CGnuPlotPlot::getXLabel().c_str(); }
  void setXLabel(const QString &s) { CGnuPlotPlot::setXLabel(s.toStdString()); }
  QString getYLabel() const { return CGnuPlotPlot::getYLabel().c_str(); }
  void setYLabel(const QString &s) { CGnuPlotPlot::setYLabel(s.toStdString()); }

  QColor lineColor() const;
  void setLineColor(const QColor &c);

 private:
  CQGnuPlotWindow   *window_;
  CQGnuPlotRenderer *renderer_;
};

#endif
