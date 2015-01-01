#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>

#include <QObject>
#include <QString>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotPlot;

class CQGnuPlot : public QObject, public CGnuPlot {
  Q_OBJECT

  Q_ENUMS(SymbolType)

 public:
  enum SymbolType {
    SymbolPoint,
    SymbolPlus,
    SymbolCross,
    SymbolStar,
    SymbolBox,
    SymbolFilledBox,
    SymbolCircle,
    SymbolFilledCircle,
    SymbolTriangle,
    SymbolFilledTriangle,
    SymbolITriangle,
    SymbolFilledITriangle,
    SymbolDiamond,
    SymbolFilledDiamond
  };

  struct TipRect {
    QString str;
    QRectF  rect;
  };

 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CGnuPlotWindow    *createWindow() override;
  CGnuPlotPlot      *createPlot(CGnuPlotWindow *window) override;
  CGnuPlotLineStyle *createLineStyle() override;

  CGnuPlotArrow     *createArrow();
  CGnuPlotLabel     *createLabel();
  CGnuPlotEllipse   *createEllipse();
  CGnuPlotPolygon   *createPolygon();
  CGnuPlotRectangle *createRectangle();

  void timeout() override;

  void load(const std::string &filename);
};

#endif
