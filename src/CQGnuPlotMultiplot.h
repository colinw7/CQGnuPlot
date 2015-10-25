#ifndef CQGnuPlotMultiplot_H
#define CQGnuPlotMultiplot_H

#include <QObject>
#include <QFont>
#include <CGnuPlotMultiplot.h>

class CQGnuPlot;

class CQGnuPlotMultiplot : public QObject, public CGnuPlotMultiplot {
  Q_OBJECT

  Q_PROPERTY(bool    enabled   READ isEnabled   WRITE setEnabled)
  Q_PROPERTY(bool    autoFit   READ isAutoFit   WRITE setAutoFit)
  Q_PROPERTY(bool    enhanced  READ isEnhanced  WRITE setEnhanced)
  Q_PROPERTY(int     rows      READ rows        WRITE setRows)
  Q_PROPERTY(int     cols      READ cols        WRITE setCols)
  Q_PROPERTY(bool    rowsFirst READ isRowsFirst WRITE setRowsFirst)
  Q_PROPERTY(bool    downward  READ isDownward  WRITE setDownward)
  Q_PROPERTY(double  xscale    READ xScale      WRITE setXScale)
  Q_PROPERTY(double  yscale    READ yScale      WRITE setYScale)
  Q_PROPERTY(double  xoffset   READ xOffset     WRITE setXOffset)
  Q_PROPERTY(double  yoffset   READ yOffset     WRITE setYOffset)
  Q_PROPERTY(double  xspacing  READ xSpacing    WRITE setXSpacing)
  Q_PROPERTY(double  yspacing  READ ySpacing    WRITE setYSpacing)
  Q_PROPERTY(double  lmargin   READ lmargin     WRITE setLMargin)
  Q_PROPERTY(double  rmargin   READ rmargin     WRITE setRMargin)
  Q_PROPERTY(double  tmargin   READ tmargin     WRITE setTMargin)
  Q_PROPERTY(double  bmargin   READ bmargin     WRITE setBMargin)
  Q_PROPERTY(QString title     READ title       WRITE setTitle)
  Q_PROPERTY(QFont   titleFont READ titleFont   WRITE setTitleFont)

 public:
  CQGnuPlotMultiplot(CQGnuPlot *plot);

  QString title() const;
  void setTitle(const QString &title);

  QFont titleFont() const;
  void setTitleFont(const QFont &f);
};

#endif
