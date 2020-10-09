TEMPLATE = app

TARGET = CQGnuPlot

QT += widgets

DEPENDPATH += .

QMAKE_CXXFLAGS += \
-std=c++14 -DGNUPLOT_EXPR -DRGB_UTIL \
-DCQUTIL_FONT -DCQUTIL_IMAGE -DCQUTIL_EVENT

MOC_DIR = .moc

CONFIG += debug

# Input
SOURCES += \
CQGnuPlotTest.cpp \
CQGnuPlot.cpp \
CQGnuPlotAnnotation.cpp \
CQGnuPlotArrow.cpp \
CQGnuPlotArrowObject.cpp \
CQGnuPlotAxis.cpp \
CQGnuPlotBoxBarObject.cpp \
CQGnuPlotBoxObject.cpp \
CQGnuPlotBoxPlot.cpp \
CQGnuPlotBubbleObject.cpp \
CQGnuPlotCamera.cpp \
CQGnuPlotCanvas.cpp \
CQGnuPlotCircle.cpp \
CQGnuPlotColorBox.cpp \
CQGnuPlotDevice.cpp \
CQGnuPlotEllipse.cpp \
CQGnuPlotEllipseObject.cpp \
CQGnuPlotEndBar.cpp \
CQGnuPlotEnum.cpp \
CQGnuPlotErrorBarObject.cpp \
CQGnuPlotFill.cpp \
CQGnuPlotFinanceBarObject.cpp \
CQGnuPlotGroup.cpp \
CQGnuPlotImageObject.cpp \
CQGnuPlotKey.cpp \
CQGnuPlotLabel.cpp \
CQGnuPlotLabelObject.cpp \
CQGnuPlotLineStyle.cpp \
CQGnuPlotMark.cpp \
CQGnuPlotMultiplot.cpp \
CQGnuPlotPalette.cpp \
CQGnuPlotPathObject.cpp \
CQGnuPlotPieObject.cpp \
CQGnuPlotPlot.cpp \
CQGnuPlotPm3D.cpp \
CQGnuPlotPNGDevice.cpp \
CQGnuPlotPNGRenderer.cpp \
CQGnuPlotPointObject.cpp \
CQGnuPlotPolygon.cpp \
CQGnuPlotPolygonObject.cpp \
CQGnuPlotRectangle.cpp \
CQGnuPlotRectObject.cpp \
CQGnuPlotRenderer.cpp \
CQGnuPlotStroke.cpp \
CQGnuPlotTimeStamp.cpp \
CQGnuPlotTitle.cpp \
CQGnuPlotToolBar.cpp \
CQGnuPlotFilename.cpp \
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
\
CQGnuPlotLoadFileDialog.cpp \
CQGnuPlotLoadFunctionDialog.cpp \
CQGnuPlotManageFunctionsDialog.cpp \
CQGnuPlotManageVariablesDialog.cpp \
CQGnuPlotCreateDialog.cpp \
CQGnuPlotDataDialog.cpp \
CQGnuPlotSaveDialog.cpp \
CQGnuPlotPaletteDialog.cpp \
CQGnuPlotTreeView.cpp \
CQGnuPlotTreeWidget.cpp \
CQGnuPlotDataTree.cpp \
CQGnuPlotDataModel.cpp \
CQGnuPlotPositionEdit.cpp \
\
CQGnuPlotBoxPlotStyleValue.cpp \
\
CGnuPlot.cpp \
CGnuPlotAnnotation.cpp \
CGnuPlotArrow.cpp \
CGnuPlotArrowData.cpp \
CGnuPlotArrowObject.cpp \
CGnuPlotArrowStyle.cpp \
CGnuPlotAxis.cpp \
CGnuPlotAxisData.cpp \
CGnuPlotBoxBarObject.cpp \
CGnuPlotBoxObject.cpp \
CGnuPlotBoxPlot.cpp \
CGnuPlotBBoxRenderer.cpp \
CGnuPlotBinaryFormat.cpp \
CGnuPlotBorderData.cpp \
CGnuPlotBubbleObject.cpp \
CGnuPlotCamera.cpp \
CGnuPlotCircle.cpp \
CGnuPlotColorBox.cpp \
CGnuPlotColorSpec.cpp \
CGnuPlotContour.cpp \
CGnuPlotCoordValue.cpp \
CGnuPlotDash.cpp \
CGnuPlotDevice.cpp \
CGnuPlotEllipse.cpp \
CGnuPlotEllipseObject.cpp \
CGnuPlotEndBar.cpp \
CGnuPlotErrorBarObject.cpp \
CGnuPlotErrorBarData.cpp \
CGnuPlotFile.cpp \
CGnuPlotFill.cpp \
CGnuPlotFillStyle.cpp \
CGnuPlotFinanceBarObject.cpp \
CGnuPlotFunctions.cpp \
CGnuPlotGroup.cpp \
CGnuPlotImageObject.cpp \
CGnuPlotKey.cpp \
CGnuPlotKeyData.cpp \
CGnuPlotLabel.cpp \
CGnuPlotLabelData.cpp \
CGnuPlotLabelObject.cpp \
CGnuPlotLineStyle.cpp \
CGnuPlotLineType.cpp \
CGnuPlotLogDevice.cpp \
CGnuPlotLogRenderer.cpp \
CGnuPlotMargin.cpp \
CGnuPlotMark.cpp \
CGnuPlotMultiplot.cpp \
CGnuPlotObject.cpp \
CGnuPlotPalette.cpp \
CGnuPlotPathObject.cpp \
CGnuPlotPieObject.cpp \
CGnuPlotPlot.cpp \
CGnuPlotPm3D.cpp \
CGnuPlotPoint.cpp \
CGnuPlotPointObject.cpp \
CGnuPlotPolygon.cpp \
CGnuPlotPolygonObject.cpp \
CGnuPlotPosition.cpp \
CGnuPlotPrintFile.cpp \
CGnuPlotRectangle.cpp \
CGnuPlotRectObject.cpp \
CGnuPlotRenderer.cpp \
CGnuPlotStroke.cpp \
CGnuPlotStyle.cpp \
CGnuPlotSurface.cpp \
CGnuPlotTimeStamp.cpp \
CGnuPlotUsingCols.cpp \
\
CGnuPlotBoxErrorBarsStyleValue.cpp \
CGnuPlotBoxesStyleValue.cpp \
CGnuPlotBoxPlotStyleValue.cpp \
CGnuPlotCandlesticksStyleValue.cpp \
CGnuPlotVectorsStyleValue.cpp \
\
CGnuPlotEveryData.cpp \
CGnuPlotIndexData.cpp \
CGnuPlotWhereData.cpp \
\
CGnuPlotStyleBase.cpp \
CGnuPlotStyleAdjacency.cpp \
CGnuPlotStyleBivariate.cpp \
CGnuPlotStyleBoxErrorBars.cpp \
CGnuPlotStyleBoxes.cpp \
CGnuPlotStyleBoxPlot.cpp \
CGnuPlotStyleBoxXYErrorBars.cpp \
CGnuPlotStyleBubblePlot.cpp \
CGnuPlotStyleCandlesticks.cpp \
CGnuPlotStyleChordDiagram.cpp \
CGnuPlotStyleCircles.cpp \
CGnuPlotStyleDelaunay.cpp \
CGnuPlotStyleDendrogram.cpp \
CGnuPlotStyleDots.cpp \
CGnuPlotStyleEllipses.cpp \
CGnuPlotStyleErrorBars.cpp \
CGnuPlotStyleFilledCurves.cpp \
CGnuPlotStyleFinanceBars.cpp \
CGnuPlotStyleForceDirected.cpp \
CGnuPlotStyleHierBubblePlot.cpp \
CGnuPlotStyleImage.cpp \
CGnuPlotStyleImpulses.cpp \
CGnuPlotStyleLabels.cpp \
CGnuPlotStyleLines.cpp \
CGnuPlotStyleLinesPoints.cpp \
CGnuPlotStyleParallelAxes.cpp \
CGnuPlotStylePieChart.cpp \
CGnuPlotStylePoints.cpp \
CGnuPlotStylePolygons.cpp \
CGnuPlotStyleRadar.cpp \
CGnuPlotStyleStackedArea.cpp \
CGnuPlotStyleSteps.cpp \
CGnuPlotStyleSunburst.cpp \
CGnuPlotStyleTestPalette.cpp \
CGnuPlotStyleTestTerminal.cpp \
CGnuPlotStyleTreeMap.cpp \
CGnuPlotStyleVectors.cpp \
CGnuPlotStyleXErrorBars.cpp \
CGnuPlotStyleXErrorLines.cpp \
CGnuPlotStyleXYErrorBars.cpp \
CGnuPlotStyleXYErrorLines.cpp \
CGnuPlotStyleYErrorBars.cpp \
CGnuPlotStyleYErrorLines.cpp \
\
CGnuPlotSVGDevice.cpp \
CGnuPlotSVGRenderer.cpp \
CGnuPlotPSDevice.cpp \
CGnuPlotPSRenderer.cpp \
\
CGnuPlotSize.cpp \
CGnuPlotTitle.cpp \
CGnuPlotWindow.cpp \
\
CGnuPlotText.cpp \
CGnuPlotTextRenderer.cpp \
\
CQHeaderView.cpp \
CQFloatLabel.cpp \
CQRotatedText.cpp \
CQIntegerSpin.cpp \
CQCursor.cpp \
CQRubberBand.cpp \
CQDialog.cpp \
CQEnumCombo.cpp \
\
CAdjacency.cpp \
CDelaunay.cpp \
ChordDiagram.cpp  \
CHull3D.cpp \
CSunburst.cpp \
CTreeMap.cpp \
CDendrogram.cpp \
CPrint.cpp \
\
Expr/CExpr.cpp \
Expr/CExprBValue.cpp \
Expr/CExprCValue.cpp \
Expr/CExprCompile.cpp \
Expr/CExprExecute.cpp \
Expr/CExprFunction.cpp \
Expr/CExprIValue.cpp \
Expr/CExprInterp.cpp \
Expr/CExprOperator.cpp \
Expr/CExprParse.cpp \
Expr/CExprRValue.cpp \
Expr/CExprSValue.cpp \
Expr/CExprStrgen.cpp \
Expr/CExprToken.cpp \
Expr/CExprTokenStack.cpp \
Expr/CExprValue.cpp \
Expr/CExprVariable.cpp \

HEADERS += \
CQGnuPlot.h \
CQGnuPlotAnnotation.h \
CQGnuPlotArrow.h \
CQGnuPlotArrowObject.h \
CQGnuPlotAxis.h \
CQGnuPlotBoxBarObject.h \
CQGnuPlotBoxObject.h \
CQGnuPlotBoxPlot.h \
CQGnuPlotBubbleObject.h \
CQGnuPlotCamera.h \
CQGnuPlotCanvas.h \
CQGnuPlotCircle.h \
CQGnuPlotColorBox.h \
CQGnuPlotDevice.h \
CQGnuPlotEllipse.h \
CQGnuPlotEllipseObject.h \
CQGnuPlotEndBar.h \
CQGnuPlotEnum.h \
CQGnuPlotErrorBarObject.h \
CQGnuPlotFill.h \
CQGnuPlotFinanceBarObject.h \
CQGnuPlotGroup.h \
CQGnuPlotImageObject.h \
CQGnuPlotKey.h \
CQGnuPlotLabel.h \
CQGnuPlotLabelObject.h \
CQGnuPlotLineStyle.h \
CQGnuPlotMark.h \
CQGnuPlotMultiplot.h \
CQGnuPlotObject.h \
CQGnuPlotPalette.h \
CQGnuPlotPathObject.h \
CQGnuPlotPieObject.h \
CQGnuPlotPlot.h \
CQGnuPlotPm3D.h \
CQGnuPlotPNGDevice.h \
CQGnuPlotPNGRenderer.h \
CQGnuPlotPointObject.h \
CQGnuPlotPolygon.h \
CQGnuPlotPolygonObject.h \
CQGnuPlotRectangle.h \
CQGnuPlotRectObject.h \
CQGnuPlotRenderer.h \
CQGnuPlotStroke.h \
CQGnuPlotTimeStamp.h \
CQGnuPlotTitle.h \
CQGnuPlotToolBar.h \
CQGnuPlotFilename.h \
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
\
CQGnuPlotAdjacencyStyleValue.h \
CQGnuPlotBivariateStyleValue.h \
CQGnuPlotBoxErrorBarsStyleValue.h \
CQGnuPlotBoxesStyleValue.h \
CQGnuPlotBoxPlotStyleValue.h \
CQGnuPlotBubbleStyleValue.h \
CQGnuPlotCandlesticksStyleValue.h \
CQGnuPlotChordDiagramStyleValue.h \
CQGnuPlotCirclesStyleValue.h \
CQGnuPlotDelaunayStyleValue.h \
CQGnuPlotDendrogramStyleValue.h \
CQGnuPlotEllipsesStyleValue.h \
CQGnuPlotForceDirectedStyleValue.h \
CQGnuPlotHierBubbleStyleValue.h \
CQGnuPlotLabelsStyleValue.h \
CQGnuPlotPieChartStyleValue.h \
CQGnuPlotRadarStyleValue.h \
CQGnuPlotSunburstStyleValue.h \
CQGnuPlotTreeMapStyleValue.h \
CQGnuPlotVectorsStyleValue.h \
\
CQGnuPlotLoadFileDialog.h \
CQGnuPlotLoadFunctionDialog.h \
CQGnuPlotManageFunctionsDialog.h \
CQGnuPlotManageVariablesDialog.h \
CQGnuPlotCreateDialog.h \
CQGnuPlotDataDialog.h \
CQGnuPlotSaveDialog.h \
CQGnuPlotPaletteDialog.h \
CQGnuPlotTreeView.h \
CQGnuPlotTreeWidget.h \
CQGnuPlotDataTree.h \
CQGnuPlotDataModel.h \
CQGnuPlotPositionEdit.h \
\
CGnuPlot.h \
CGnuPlotArrow.h \
CGnuPlotArrowData.h \
CGnuPlotArrowObject.h \
CGnuPlotArrowStyle.h \
CGnuPlotAxis.h \
CGnuPlotAxisData.h \
CGnuPlotBBoxRenderer.h \
CGnuPlotBoxBarObject.h \
CGnuPlotBoxObject.h \
CGnuPlotBoxPlot.h \
CGnuPlotBinaryFormat.h \
CGnuPlotBubbleObject.h \
CGnuPlotCache.h \
CGnuPlotCamera.h \
CGnuPlotCircle.h \
CGnuPlotColorBox.h \
CGnuPlotColorSpec.h \
CGnuPlotContour.h \
CGnuPlotCoordValue.h \
CGnuPlotDevice.h \
CGnuPlotEllipse.h \
CGnuPlotEllipseObject.h \
CGnuPlotEndBar.h \
CGnuPlotErrorBarObject.h \
CGnuPlotErrorBarData.h \
CGnuPlotFile.h \
CGnuPlotFill.h \
CGnuPlotFillStyle.h \
CGnuPlotFinanceBarObject.h \
CGnuPlotFunctions.h \
CGnuPlotGroup.h \
CGnuPlotImageObject.h \
CGnuPlotKey.h \
CGnuPlotKeyData.h \
CGnuPlotLabel.h \
CGnuPlotLabelData.h \
CGnuPlotLabelObject.h \
CGnuPlotLineStyle.h \
CGnuPlotLineType.h \
CGnuPlotLogDevice.h \
CGnuPlotLogRenderer.h \
CGnuPlotMark.h \
CGnuPlotMultiplot.h \
CGnuPlotNameValues.h \
CGnuPlotObject.h \
CGnuPlotPalette.h \
CGnuPlotPathObject.h \
CGnuPlotPieObject.h \
CGnuPlotPlot.h \
CGnuPlotPm3D.h \
CGnuPlotPoint.h \
CGnuPlotPointObject.h \
CGnuPlotPointStyle.h \
CGnuPlotPolygon.h \
CGnuPlotPolygonObject.h \
CGnuPlotPosition.h \
CGnuPlotPrefValue.h \
CGnuPlotPrintFile.h \
CGnuPlotRectangle.h \
CGnuPlotRectObject.h \
CGnuPlotRenderer.h \
\
CGnuPlotSVGDevice.h \
CGnuPlotSVGRenderer.h \
CGnuPlotPSDevice.h \
CGnuPlotPSRenderer.h \
\
CGnuPlotSize.h \
CGnuPlotStroke.h \
CGnuPlotSurface.h \
CGnuPlotTimeStamp.h \
CGnuPlotUsingCols.h \
CGnuPlotEveryData.h \
CGnuPlotIndexData.h \
CGnuPlotWhereData.h \
\
CGnuPlotStyleAdjacency.h \
CGnuPlotStyleBase.h \
CGnuPlotStyleBoxErrorBars.h \
CGnuPlotStyleBoxes.h \
CGnuPlotStyleBoxPlot.h \
CGnuPlotStyleBoxXYErrorBars.h \
CGnuPlotStyleBubblePlot.h \
CGnuPlotStyleCandlesticks.h \
CGnuPlotStyleChordDiagram.h \
CGnuPlotStyleCircles.h \
CGnuPlotStyleDelaunay.h \
CGnuPlotStyleDots.h \
CGnuPlotStyleEllipses.h \
CGnuPlotStyleErrorBars.h \
CGnuPlotStyleFilledCurves.h \
CGnuPlotStyleFinanceBars.h \
CGnuPlotStyleForceDirected.h \
CGnuPlotStyle.h \
CGnuPlotStyleImage.h \
CGnuPlotStyleImpulses.h \
CGnuPlotStyleLabels.h \
CGnuPlotStyleLines.h \
CGnuPlotStyleLinesPoints.h \
CGnuPlotStyleParallelAxes.h \
CGnuPlotStylePieChart.h \
CGnuPlotStylePoints.h \
CGnuPlotStylePolygons.h \
CGnuPlotStyleRadar.h \
CGnuPlotStyleSteps.h \
CGnuPlotStyleSunburst.h \
CGnuPlotStyleTestPalette.h \
CGnuPlotStyleTestTerminal.h \
CGnuPlotStyleTreeMap.h \
CGnuPlotStyleVectors.h \
CGnuPlotStyleXErrorBars.h \
CGnuPlotStyleXErrorLines.h \
CGnuPlotStyleXYErrorBars.h \
CGnuPlotStyleXYErrorLines.h \
CGnuPlotStyleYErrorBars.h \
CGnuPlotStyleYErrorLines.h \
\
CGnuPlotTitle.h \
CGnuPlotTypes.h \
CGnuPlotUtil.h \
CGnuPlotWindow.h \
\
CGnuPlotText.h \
CGnuPlotTextRenderer.h \
\
CQHeaderView.h \
CQFloatLabel.h \
CQRotatedText.h \
CQIntegerSpin.h \
CQCursor.h \
CQRubberBand.h \
CQDialog.h \
CQEnumCombo.h \
\
CAdjacency.h \
CDelaunay.h \
ChordDiagram.h \
CHull3D.h \
CSunburst.h \
CTreeMap.h \
CPrint.h \
\
CStrUniqueMatch.h \
\
Expr/CExpr.h \
Expr/CExprBValue.h \
Expr/CExprCValue.h \
Expr/CExprCompile.h \
Expr/CExprError.h \
Expr/CExprExecute.h \
Expr/CExprFunction.h \
Expr/CExprI.h \
Expr/CExprInterp.h \
Expr/CExprIValue.h \
Expr/CExprOperator.h \
Expr/CExprParse.h \
Expr/CExprRValue.h \
Expr/CExprSValue.h \
Expr/CExprValue.h \
Expr/CExprVariable.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
Expr \
../include \
../../CQApp/include \
../../CQUtil/include \
../../CQPropertyTree/include \
../../CQGradientPalette/include \
../../CRenderer/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRGBName/include \
../../CXML/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CQGradientPalette/lib \
-L../../CQPropertyTree/lib \
-L../../CXML/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CRenderer/lib \
-L../../CImageLib/lib \
-L../../CMath/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCQGradientPalette -lCQPropertyTree -lCQUtil \
-lCXML -lCRenderer -lCReadLine -lCUtil -lCFont -lCImageLib -lCConfig \
-lCMath -lCRGBName -lCFileUtil -lCFile -lCUtil -lCOS -lCStrUtil -lCRegExp \
-lfreetype -lpng -ljpeg -ltre -lreadline
