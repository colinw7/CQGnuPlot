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

  Q_ENUMS(SymbolType)
  Q_ENUMS(HAlignType)
  Q_ENUMS(VAlignType)
  Q_ENUMS(PlotStyle)
  Q_ENUMS(HistogramStyle)
  Q_ENUMS(FillType)
  Q_ENUMS(FillPattern)
  Q_ENUMS(BoxWidthType)
  Q_ENUMS(DrawLayerType)
  Q_ENUMS(PaletteColorType)
  Q_ENUMS(ArrowCoordType)

 public:
  enum HAlignType {
    AlignLeft,
    AlignRight,
    AlignHCenter
  };

  enum VAlignType {
    AlignTop,
    AlignBottom,
    AlignVCenter
  };

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

  enum PlotStyle {
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
    PlotRadar,
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

  enum HistogramStyle {
    HistogramNone,
    HistogramClustered,
    HistogramErrorBars,
    HistogramRowStacked,
    HistogramColumnStacked,
  };

  enum FillType {
    FillNone,
    FillSolid,
    FillPatterned
  };

  enum FillPattern {
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

  enum BoxWidthType {
    BoxWidthAuto,
    BoxWidthAbsolute,
    BoxWidthRelative,
  };

  enum DrawLayerType {
    LayerDefault,
    LayerFront,
    LayerBack,
    LayerBehind
  };

  enum PaletteColorType {
    PaletteColorModel,
    PaletteColorDefined
  };

  enum ArrowCoordType {
    ArrowCoordFromTo,
    ArrowCoordFromRTo,
    ArrowCoordFromAngle
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
