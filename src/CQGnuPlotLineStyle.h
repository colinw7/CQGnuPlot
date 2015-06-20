#ifndef CQGnuPlotLineStyle_H
#define CQGnuPlotLineStyle_H

#include <QObject>
#include <QColor>

#include <CQGnuPlot.h>
#include <CGnuPlotLineStyle.h>

class CQGnuPlotLineStyle : public QObject, public CGnuPlotLineStyle {
  Q_OBJECT

  Q_PROPERTY(int    lineType      READ lineType      WRITE setLineType     )
  Q_PROPERTY(double lineWidth     READ lineWidth     WRITE setLineWidth    )
  Q_PROPERTY(int    lineDash      READ lineDash      WRITE setLineDash     )
  Q_PROPERTY(QColor lineColor     READ lineColor     WRITE setLineColor    )
  Q_PROPERTY(double pointSize     READ pointSize     WRITE setPointSize    )
  Q_PROPERTY(int    pointInterval READ pointInterval WRITE setPointInterval)
  Q_PROPERTY(bool   palette       READ palette       WRITE setPalette      )

  Q_PROPERTY(CQGnuPlotEnum::SymbolType pointType READ pointType WRITE setPointType)

 public:
  CQGnuPlotLineStyle(CGnuPlot *plot);
 ~CQGnuPlotLineStyle();

  int lineType() const;

  double lineWidth() const;

  int lineDash() const;
  void setLineDash(int ind);

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  CQGnuPlotEnum::SymbolType pointType() const;
  void setPointType(const CQGnuPlotEnum::SymbolType &type);

  int pointInterval() const;

  double pointSize() const;
};

#endif
