#ifndef CQGnuPlotMark_H
#define CQGnuPlotMark_H

#include <QObject>
#include <CGnuPlotMark.h>
#include <CQGnuPlotEnum.h>
#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotMark : public QObject, public CGnuPlotMark {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::SymbolType type READ type WRITE setType)

  Q_PROPERTY(double size      READ size        WRITE setSize     )
  Q_PROPERTY(QColor color     READ color       WRITE setColor    )
  Q_PROPERTY(double width     READ width       WRITE setWidth    )
  Q_PROPERTY(bool   pixelSize READ isPixelSize WRITE setPixelSize)

 public:
  CQGnuPlotMark(CQGnuPlotPlot *plot);

  CQGnuPlotMark *dup() const override;

  CQGnuPlotEnum::SymbolType type() const;
  void setType(const CQGnuPlotEnum::SymbolType &type);

  QColor color() const;
  void setColor(const QColor &c);
};

#endif
