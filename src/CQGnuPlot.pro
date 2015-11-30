TEMPLATE = app

QT += widgets

TARGET = CQGnuPlot

DEPENDPATH += .

INCLUDEPATH += . ../include

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR -DRGB_UTIL

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
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
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
CGnuPlotStyleAdjacency.cpp \
CGnuPlotStyleBoxErrorBars.cpp \
CGnuPlotStyleBoxes.cpp \
CGnuPlotStyleBoxPlot.cpp \
CGnuPlotStyleBoxXYErrorBars.cpp \
CGnuPlotStyleBubblePlot.cpp \
CGnuPlotStyleCandlesticks.cpp \
CGnuPlotStyleChordDiagram.cpp \
CGnuPlotStyleCircles.cpp \
CGnuPlotStyleDelaunay.cpp \
CGnuPlotStyleDots.cpp \
CGnuPlotStyleEllipses.cpp \
CGnuPlotStyleErrorBars.cpp \
CGnuPlotStyleFilledCurves.cpp \
CGnuPlotStyleFinanceBars.cpp \
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
CGnuPlotSize.cpp \
CGnuPlotTitle.cpp \
CGnuPlotWindow.cpp \
\
CGnuPlotText.cpp \
CGnuPlotTextRenderer.cpp \
\
CQPropertyDelegate.cpp \
CQPropertyEditor.cpp \
CQPropertyItem.cpp \
CQPropertyTree.cpp \
CQPaletteChooser.cpp \
CQPaletteDialog.cpp \
\
CQToolTip.cpp \
CQFloatLabel.cpp \
CQRotatedText.cpp \
CQComboSlider.cpp \
CQSlider.cpp \
CQAngleSpinBox.cpp \
CQIntegerSpin.cpp \
CQRealSpin.cpp \
CQLineDash.cpp \
CQPoint2DEdit.cpp \
CQIconCombo.cpp \
CQCursor.cpp \
\
CAdjacency.cpp \
CDelaunay.cpp \
ChordDiagram.cpp  \
CHull3D.cpp \
CSunburst.cpp \
CTreeMap.cpp \
\
CMathGeom2D.cpp \
CInvNorm.cpp \
CArcToBezier.cpp \
CSymbol2D.cpp \
\
Expr/CExpr.cpp \
Expr/CExprBValue.cpp \
Expr/CExprCompile.cpp \
Expr/CExprCValue.cpp \
Expr/CExprExecute.cpp \
Expr/CExprFunction.cpp \
Expr/CExprInterp.cpp \
Expr/CExprIValue.cpp \
Expr/CExprOperator.cpp \
Expr/CExprParse.cpp \
Expr/CExprRValue.cpp \
Expr/CExprStrgen.cpp \
Expr/CExprSValue.cpp \
Expr/CExprToken.cpp \
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
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
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
CGnuPlotSVGDevice.h \
CGnuPlotSVGRenderer.h \
CGnuPlotSize.h \
CGnuPlotStroke.h \
CGnuPlotSurface.h \
CGnuPlotTimeStamp.h \
CGnuPlotUsingCols.h \
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
CQPropertyDelegate.h \
CQPropertyEditor.h \
CQPropertyItem.h \
CQPropertyTree.h \
CQPaletteChooser.h \
CQPaletteDialog.h \
\
CQToolTip.h \
CQFloatLabel.h \
CQRotatedText.h \
CQComboSlider.h \
CQSlider.h \
CQAngleSpinBox.h \
CQIntegerSpin.h \
CQRealSpin.h \
CQLineDash.h \
CQPoint2DEdit.h \
CQIconCombo.h \
CQCursor.h \
\
CAdjacency.h \
CDelaunay.h \
ChordDiagram.h \
CHull3D.h \
CSunburst.h \
CTreeMap.h \
\
CMathGeom2D.h \
CArcToBezier.h \
CSymbol2D.h \
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
. Expr ../include \
../../CQApp/include \
../../CQUtil/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRGBName/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCQUtil -lCReadLine -lCUtil -lCFont -lCImageLib -lCConfig \
-lCRGBName -lCFile -lCOS -lCStrUtil -lCRegExp \
-lpng -ljpeg -ltre -lreadline
