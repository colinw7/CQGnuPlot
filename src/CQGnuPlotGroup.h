#ifndef CQGnuPlotGroup_H
#define CQGnuPlotGroup_H

#include <CQGnuPlot.h>
#include <CGnuPlotGroup.h>
#include <CQGnuPlotObject.h>

class CQGnuPlotWindow;
class QPainter;

class CQGnuPlotGroup : public CQGnuPlotObject, public CGnuPlotGroup {
  Q_OBJECT

  Q_PROPERTY(QString title READ title WRITE setTitle)

  Q_PROPERTY(double regionLeft   READ getRegionLeft   WRITE setRegionLeft  )
  Q_PROPERTY(double regionRight  READ getRegionRight  WRITE setRegionRight )
  Q_PROPERTY(double regionTop    READ getRegionTop    WRITE setRegionTop   )
  Q_PROPERTY(double regionBottom READ getRegionBottom WRITE setRegionBottom)

  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)

  Q_PROPERTY(double ratio READ getRatio WRITE setRatio)

  Q_PROPERTY(double xmin READ getXMin WRITE setXMin)
  Q_PROPERTY(double xmax READ getXMax WRITE setXMax)
  Q_PROPERTY(double ymin READ getYMin WRITE setYMin)
  Q_PROPERTY(double ymax READ getYMax WRITE setYMax)

  Q_PROPERTY(bool    xtics       READ getXTics       WRITE setXTics)
  Q_PROPERTY(bool    ytics       READ getYTics       WRITE setYTics)
  Q_PROPERTY(bool    xticsMirror READ getXTicsMirror WRITE setXTicsMirror)
  Q_PROPERTY(bool    yticsMirror READ getYTicsMirror WRITE setYTicsMirror)
  Q_PROPERTY(bool    xgrid       READ getXGrid       WRITE setXGrid)
  Q_PROPERTY(bool    ygrid       READ getYGrid       WRITE setYGrid)

  Q_PROPERTY(int     borders     READ getBorders     WRITE setBorders    )
  Q_PROPERTY(double  borderWidth READ getBorderWidth WRITE setBorderWidth)

  Q_PROPERTY(CQGnuPlot::CQHistogramStyle histogramStyle READ histogramStyle WRITE setHistogramStyle)

 public:
  CQGnuPlotGroup(CQGnuPlotWindow *window);

 ~CQGnuPlotGroup() override;

  CQGnuPlotWindow *qwindow() const { return window_; }

  void setPainter(QPainter *p);

  QString title() const;
  void setTitle(const QString &s);

  QString getXLabel() const { return CGnuPlotGroup::getXLabel().c_str(); }
  void setXLabel(const QString &s) { CGnuPlotGroup::setXLabel(s.toStdString()); }
  QString getYLabel() const { return CGnuPlotGroup::getYLabel().c_str(); }
  void setYLabel(const QString &s) { CGnuPlotGroup::setYLabel(s.toStdString()); }

  CQGnuPlot::CQHistogramStyle histogramStyle() const;
  void setHistogramStyle(const CQGnuPlot::CQHistogramStyle &s);

  void draw() override;

  void mousePress(const QPoint &qp);
  void mouseMove (const QPoint &qp);
  bool mouseTip  (const QPoint &qp, CQGnuPlot::TipRect &tip);

 private:
  CQGnuPlotWindow *window_;
};

#endif
