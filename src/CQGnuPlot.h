#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>

#include <QObject>
#include <QString>
#include <QRect>

class CQGnuPlotDevice;
class CQGnuPlotRenderer;

class CQGnuPlot : public QObject, public CGnuPlot {
  Q_OBJECT

  Q_ENUMS(CQSymbolType)
  Q_ENUMS(CQHAlignType)
  Q_ENUMS(CQVAlignType)
  Q_ENUMS(CQPlotStyle)
  Q_ENUMS(CQHistogramStyle)
  Q_ENUMS(CQFillType)
  Q_ENUMS(CQFillPattern)
  Q_ENUMS(CQBoxWidthType)
  Q_ENUMS(CQDrawLayerType)
  Q_ENUMS(PaletteColorType)

 public:
  enum CQHAlignType {
    AlignLeft,
    AlignRight,
    AlignHCenter
  };

  enum CQVAlignType {
    AlignTop,
    AlignBottom,
    AlignVCenter
  };

  enum CQSymbolType {
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

  enum CQPlotStyle {
    PlotNone,
    PlotBoxErrorBars,
    PlotBoxes,
    PlotBoxPlot,
    PlotBoxXYErrorBars,
    PlotBubbleChart,
    PlotCandlesticks,
    PlotCircles,
    PlotDots,
    PlotEllipses,
    PlotErrorBars,
    PlotErrorLines,
    PlotFilledCurves,
    PlotFillSteps,
    PlotFinanceBars,
    PlotFSteps,
    PlotHiSteps,
    PlotHistograms,
    PlotImage,
    PlotImpulses,
    PlotLabels,
    PlotLines,
    PlotLinesPoints,
    PlotParallelAxes,
    PlotPieChart,
    PlotPm3d,
    PlotPoints,
    PlotRgbAlpha,
    PlotRgbImage,
    PlotSteps,
    PlotVectors,
    PlotXErrorBars,
    PlotXErrorLines,
    PlotXYErrorBars,
    PlotXYErrorLines,
    PlotYErrorBars,
    PlotYErrorLines,
    PlotTestTerminal,
    PlotTestPalette,
  };

  enum CQHistogramStyle {
    HistogramNone,
    HistogramClustered,
    HistogramErrorBars,
    HistogramRowStacked,
    HistogramColumnStacked,
  };

  enum CQFillType {
    FillNone,
    FillSolid,
    FillPatterned
  };

  enum CQFillPattern {
    PatternNone,
    PatternHatch,
    PatternDense,
    PatternFg,
    PatternFDiag,
    PatternBDiag,
    PatternFDiag1,
    PatternBDiag1,
    PatternBg
  };

  enum CQBoxWidthType {
    BoxWidthAuto,
    BoxWidthAbsolute,
    BoxWidthRelative,
  };

  enum CQDrawLayerType {
    LayerFront,
    LayerBack,
    LayerBehind
  };

  enum PaletteColorType {
    PaletteColorModel,
    PaletteColorDefined
  };

 public:
  struct TipRect {
    QString str;
    QRectF  rect;
  };

 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CQGnuPlotDevice *qdevice() const { return device_; }

  CQGnuPlotRenderer *qrenderer() const;

 private:
  CQGnuPlotDevice *device_;
};

#endif
