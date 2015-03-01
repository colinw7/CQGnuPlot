#ifndef CQGnuPlotBar_H
#define CQGnuPlotBar_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotBar.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotBar : public CQGnuPlotObject, public CGnuPlotBar {
  Q_OBJECT

  Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)
  Q_PROPERTY(QColor fillColor READ getFillColor WRITE setFillColor)
  Q_PROPERTY(bool   border    READ hasBorder    WRITE setBorder   )

 public:
  CQGnuPlotBar(CQGnuPlotPlot *plot);
 ~CQGnuPlotBar();

  QColor getLineColor() const;
  void setLineColor(const QColor &c);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  void draw() const override;
};

#endif
