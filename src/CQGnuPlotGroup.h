#ifndef CQGnuPlotGroup_H
#define CQGnuPlotGroup_H

#include <CQGnuPlot.h>
#include <CGnuPlotGroup.h>

#include <QObject>

class CQGnuPlotWindow;
class QPainter;

class CQGnuPlotGroup : public QObject, public CGnuPlotGroup {
  Q_OBJECT

  Q_PROPERTY(double regionLeft   READ getRegionLeft   WRITE setRegionLeft  )
  Q_PROPERTY(double regionRight  READ getRegionRight  WRITE setRegionRight )
  Q_PROPERTY(double regionTop    READ getRegionTop    WRITE setRegionTop   )
  Q_PROPERTY(double regionBottom READ getRegionBottom WRITE setRegionBottom)

  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)

  Q_PROPERTY(double ratio READ getRatio WRITE setRatio)

  Q_PROPERTY(QString xlabel      READ getXLabel      WRITE setXLabel)
  Q_PROPERTY(QString ylabel      READ getYLabel      WRITE setYLabel)
  Q_PROPERTY(bool    xtics       READ getXTics       WRITE setXTics)
  Q_PROPERTY(bool    ytics       READ getYTics       WRITE setYTics)
  Q_PROPERTY(bool    xticsMirror READ getXTicsMirror WRITE setXTicsMirror)
  Q_PROPERTY(bool    yticsMirror READ getYTicsMirror WRITE setYTicsMirror)
  Q_PROPERTY(bool    xgrid       READ getXGrid       WRITE setXGrid)
  Q_PROPERTY(bool    ygrid       READ getYGrid       WRITE setYGrid)

  Q_PROPERTY(int     borders     READ getBorders     WRITE setBorders    )
  Q_PROPERTY(double  borderWidth READ getBorderWidth WRITE setBorderWidth)

  Q_PROPERTY(bool                    keyDisplayed READ getKeyDisplayed WRITE setKeyDisplayed)
  Q_PROPERTY(CQGnuPlot::CQHAlignType keyHAlign    READ keyHAlign       WRITE setKeyHAlign   )
  Q_PROPERTY(CQGnuPlot::CQVAlignType keyVAlign    READ keyVAlign       WRITE setKeyVAlign   )
  Q_PROPERTY(bool                    keyBox       READ getKeyBox       WRITE setKeyBox      )
  Q_PROPERTY(bool                    keyReverse   READ getKeyReverse   WRITE setKeyReverse  )

  Q_PROPERTY(CQGnuPlot::CQHistogramStyle histogramStyle READ histogramStyle WRITE setHistogramStyle)

 public:
  CQGnuPlotGroup(CQGnuPlotWindow *window);

 ~CQGnuPlotGroup() override;

  CQGnuPlotWindow *qwindow() const { return window_; }

  void setPainter(QPainter *p);

  QString getXLabel() const { return CGnuPlotGroup::getXLabel().c_str(); }
  void setXLabel(const QString &s) { CGnuPlotGroup::setXLabel(s.toStdString()); }
  QString getYLabel() const { return CGnuPlotGroup::getYLabel().c_str(); }
  void setYLabel(const QString &s) { CGnuPlotGroup::setYLabel(s.toStdString()); }

  CQGnuPlot::CQHAlignType keyHAlign() const;
  void setKeyHAlign(const CQGnuPlot::CQHAlignType &a);

  CQGnuPlot::CQVAlignType keyVAlign() const;
  void setKeyVAlign(const CQGnuPlot::CQVAlignType &a);

  CQGnuPlot::CQHistogramStyle histogramStyle() const;
  void setHistogramStyle(const CQGnuPlot::CQHistogramStyle &s);

  void mouseMove(const CPoint2D &p);
  bool mouseTip(const CPoint2D &p, CQGnuPlot::TipRect &tip);

 private:
  CQGnuPlotWindow *window_;
};

#endif
