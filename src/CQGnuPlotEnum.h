#ifndef CQGnuPlotEnum_H
#define CQGnuPlotEnum_H

#include <CGnuPlotTypes.h>
#include <QObject>

class CQGnuPlotEnum : public QObject {
  Q_OBJECT

  Q_ENUMS(HAlignType)
  Q_ENUMS(VAlignType)
  Q_ENUMS(SymbolType)
  Q_ENUMS(PlotStyle)
  Q_ENUMS(HistogramStyle)
  Q_ENUMS(FillType)
  Q_ENUMS(FillPattern)
  Q_ENUMS(BoxWidthType)
  Q_ENUMS(ColorModelType)
  Q_ENUMS(DrawLayerType)
  Q_ENUMS(PaletteColorType)
  Q_ENUMS(ArrowCoordType)

  Q_PROPERTY(SymbolType symbolType READ symbolType WRITE setSymbolType)
  Q_PROPERTY(PlotStyle  plotStyle  READ plotStyle  WRITE setPlotStyle )

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
    SymbolNone,
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
    PlotChordDiagram,
    PlotCircles,
    PlotDelaunay,
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
    PlotPolygons,
    PlotPm3d,
    PlotPoints,
    PlotRadar,
    PlotRgbAlpha,
    PlotRgbImage,
    PlotSteps,
    PlotSunburst,
    PlotTreeMap,
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

  enum ColorModelType {
    ColorModelRGB,
    ColorModelHSV,
    ColorModelCMY,
    ColorModelYIQ,
    ColorModelXYZ,
  };

  enum DrawLayerType {
    LayerDefault,
    LayerFront,
    LayerBack,
    LayerBehind
  };

  enum PaletteColorType {
    PaletteColorModel,
    PaletteColorDefined,
    PaletteColorFunctions,
    PaletteColorCubeHelix
  };

  enum ArrowCoordType {
    ArrowCoordFromTo,
    ArrowCoordFromRTo,
    ArrowCoordFromAngle
  };

 public:
  CQGnuPlotEnum() { }

  const SymbolType &symbolType() const { return symbolType_; }
  void setSymbolType(const SymbolType &v) { symbolType_ = v; }

  static SymbolType symbolConv(const CGnuPlotTypes::SymbolType &type);
  static CGnuPlotTypes::SymbolType symbolConv(const SymbolType &type);

  const PlotStyle &plotStyle() const { return plotStyle_; }
  void setPlotStyle(const PlotStyle &v) { plotStyle_ = v; }

  static PlotStyle plotStyleConv(const CGnuPlotTypes::PlotStyle &type);
  static CGnuPlotTypes::PlotStyle plotStyleConv(const PlotStyle &type);

 private:
  SymbolType symbolType_;
  PlotStyle  plotStyle_;
};

#endif
