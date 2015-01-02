#ifndef CQGnuPlotLineStyle_H
#define CQGnuPlotLineStyle_H

#include <QObject>
#include <QColor>

#include <CQGnuPlot.h>
#include <CGnuPlotLineStyle.h>

class CQGnuPlotLineStyle : public QObject, public CGnuPlotLineStyle {
  Q_OBJECT

  Q_PROPERTY(double                  width     READ width     WRITE setWidth    )
  Q_PROPERTY(QColor                  color     READ color     WRITE setColor    )
  Q_PROPERTY(CQGnuPlot::CQSymbolType pointType READ pointType WRITE setPointType)
  Q_PROPERTY(double                  pointSize READ pointSize WRITE setPointSize)

 public:
  CQGnuPlotLineStyle();

  QColor color() const;
  void setColor(const QColor &c);

  CQGnuPlot::CQSymbolType pointType() const;
  void setPointType(const CQGnuPlot::CQSymbolType &type);
};

#endif
