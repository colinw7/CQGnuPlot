#ifndef CQGnuPlotEnum_H
#define CQGnuPlotEnum_H

#include <CGnuPlotTypes.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <QObject>

class CQGnuPlotEnum : public QObject {
  Q_OBJECT

  Q_ENUMS(HAlignType)
  Q_ENUMS(VAlignType)
  Q_ENUMS(LineCapType)
  Q_ENUMS(LineJoinType)
  Q_ENUMS(SymbolType)
  Q_ENUMS(PlotStyle)
  Q_ENUMS(FillType)
  Q_ENUMS(HistogramStyle)
  Q_ENUMS(FillPattern)
  Q_ENUMS(BoxWidthType)
  Q_ENUMS(ColorModelType)
  Q_ENUMS(DrawLayerType)
  Q_ENUMS(PaletteColorType)
  Q_ENUMS(ArrowCoordType)
  Q_ENUMS(ObjectType)
  Q_ENUMS(BoxLabels)
  Q_ENUMS(BoxType)

  Q_PROPERTY(SymbolType     symbolType     READ symbolType     WRITE setSymbolType    )
  Q_PROPERTY(PlotStyle      plotStyle      READ plotStyle      WRITE setPlotStyle     )
  Q_PROPERTY(FillType       fillType       READ fillType       WRITE setFillType      )
  Q_PROPERTY(HistogramStyle histogramStyle READ histogramStyle WRITE setHistogramStyle)
  Q_PROPERTY(FillPattern    fillPattern    READ fillPattern    WRITE setFillPattern   )
  Q_PROPERTY(ObjectType     objectType     READ objectType     WRITE setObjectType    )
  Q_PROPERTY(BoxLabels      boxLabels      READ boxLabels      WRITE setBoxLabels     )
  Q_PROPERTY(BoxType        boxType        READ boxType        WRITE setBoxType       )

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

  enum LineCapType {
    LineCapButt,
    LineCapRound,
    LineCapSquare
  };

  enum LineJoinType {
    LineJoinMitre,
    LineJoinRound,
    LineJoinBevel,
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
    PlotAdjacency,
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

  enum ObjectType {
    ObjectTypeNone,
    ObjectTypeArrow,
    ObjectTypeCircle,
    ObjectTypeEllipse,
    ObjectTypeLabel,
    ObjectTypePolygon,
    ObjectTypeRectangle
  };

  enum BoxLabels {
    BoxLabelOff,
    BoxLabelAuto,
    BoxLabelX,
    BoxLabelX2
  };

  enum BoxType {
    BoxTypeCandleSticks,
    BoxTypeFinanceBars
  };

 public:
  CQGnuPlotEnum() { }

  //---

  const HAlignType &halignType() { return halignType_; }
  void setHAlignType(const HAlignType &v) { halignType_ = v; }

  static HAlignType halignConv(const CHAlignType &type);
  static CHAlignType halignConv(const HAlignType &type);

  //---

  const VAlignType &valignType() { return valignType_; }
  void setVAlignType(const VAlignType &v) { valignType_ = v; }

  static VAlignType valignConv(const CVAlignType &type);
  static CVAlignType valignConv(const VAlignType &type);

  //---

  const LineCapType &lineCapType() { return lineCapType_; }
  void setLineCapType(const LineCapType &v) { lineCapType_ = v; }

  static LineCapType lineCapConv(const CLineCapType &type);
  static CLineCapType lineCapConv(const LineCapType &type);

  //---

  const LineJoinType &lineJoinType() { return lineJoinType_; }
  void setLineJoinType(const LineJoinType &v) { lineJoinType_ = v; }

  static LineJoinType lineJoinConv(const CLineJoinType &type);
  static CLineJoinType lineJoinConv(const LineJoinType &type);

  //---

  const SymbolType &symbolType() const { return symbolType_; }
  void setSymbolType(const SymbolType &v) { symbolType_ = v; }

  static SymbolType symbolConv(const CGnuPlotTypes::SymbolType &type);
  static CGnuPlotTypes::SymbolType symbolConv(const SymbolType &type);

  //---

  const PlotStyle &plotStyle() const { return plotStyle_; }
  void setPlotStyle(const PlotStyle &v) { plotStyle_ = v; }

  static PlotStyle plotStyleConv(const CGnuPlotTypes::PlotStyle &type);
  static CGnuPlotTypes::PlotStyle plotStyleConv(const PlotStyle &type);

  //---

  const FillType &fillType() const { return fillType_; }
  void setFillType(const FillType &v) { fillType_ = v; }

  static FillType fillTypeConv(const CGnuPlotTypes::FillType &type);
  static CGnuPlotTypes::FillType fillTypeConv(const FillType &type);

  //---

  const HistogramStyle &histogramStyle() const { return histogramStyle_; }
  void setHistogramStyle(const HistogramStyle &v) { histogramStyle_ = v; }

  static HistogramStyle histogramStyleConv(const CGnuPlotTypes::HistogramStyle &type);
  static CGnuPlotTypes::HistogramStyle histogramStyleConv(const HistogramStyle &type);

  //---

  const FillPattern &fillPattern() const { return fillPattern_; }
  void setFillPattern(const FillPattern &v) { fillPattern_ = v; }

  static FillPattern fillPatternConv(const CGnuPlotTypes::FillPattern &type);
  static CGnuPlotTypes::FillPattern fillPatternConv(const FillPattern &type);

  //---

  const ObjectType &objectType() const { return objectType_; }
  void setObjectType(const ObjectType &v) { objectType_ = v; }

  static ObjectType objectTypeConv(const CGnuPlotTypes::ObjectType &type);
  static CGnuPlotTypes::ObjectType objectTypeConv(const ObjectType &type);

  //---

  const BoxLabels &boxLabels() const { return boxLabels_; }
  void setBoxLabels(const BoxLabels &v) { boxLabels_ = v; }

  static BoxLabels boxLabelsConv(const CGnuPlotTypes::BoxLabels &type);
  static CGnuPlotTypes::BoxLabels boxLabelsConv(const BoxLabels &type);

  //---

  const BoxType &boxType() const { return boxType_; }
  void setBoxType(const BoxType &v) { boxType_ = v; }

  static BoxType boxTypeConv(const CGnuPlotTypes::BoxType &type);
  static CGnuPlotTypes::BoxType boxTypeConv(const BoxType &type);

  //---

 private:
  HAlignType     halignType_;
  VAlignType     valignType_;
  LineCapType    lineCapType_;
  LineJoinType   lineJoinType_;
  SymbolType     symbolType_;
  PlotStyle      plotStyle_;
  FillType       fillType_;
  HistogramStyle histogramStyle_;
  FillPattern    fillPattern_;
  ObjectType     objectType_;
  BoxLabels      boxLabels_;
  BoxType        boxType_;
};

#endif
