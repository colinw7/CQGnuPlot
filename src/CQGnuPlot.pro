TEMPLATE = app

QT += widgets

TARGET = CQGnuPlot

DEPENDPATH += .

INCLUDEPATH += . ../include

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR -DRGB_UTIL

CONFIG += debug

# Input
SOURCES += \
CQGnuPlot.cpp \
CQGnuPlotAnnotation.cpp \
CQGnuPlotArrow.cpp \
CQGnuPlotAxis.cpp \
CQGnuPlotBarObject.cpp \
CGnuPlotBinaryFormat.cpp \
CQGnuPlotBubbleObject.cpp \
CQGnuPlotCamera.cpp \
CQGnuPlotCanvas.cpp \
CQGnuPlotCircle.cpp \
CQGnuPlotColorBox.cpp \
CQGnuPlotDevice.cpp \
CQGnuPlotEllipse.cpp \
CQGnuPlotEllipseObject.cpp \
CQGnuPlotGroup.cpp \
CQGnuPlotKey.cpp \
CQGnuPlotLabel.cpp \
CQGnuPlotLineStyle.cpp \
CQGnuPlotPalette.cpp \
CQGnuPlotPieObject.cpp \
CQGnuPlotPlot.cpp \
CQGnuPlotPolygon.cpp \
CQGnuPlotPolygonObject.cpp \
CQGnuPlotRectangle.cpp \
CQGnuPlotRectObject.cpp \
CQGnuPlotRenderer.cpp \
CQGnuPlotTitle.cpp \
CQGnuPlotToolBar.cpp \
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
\
CGnuPlot.cpp \
CGnuPlotArrow.cpp \
CGnuPlotArrowStyle.cpp \
CGnuPlotAxis.cpp \
CGnuPlotBBoxRenderer.cpp \
CGnuPlotBarObject.cpp \
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
CGnuPlotFile.cpp \
CGnuPlotFillStyle.cpp \
CGnuPlotGroup.cpp \
CGnuPlotKey.cpp \
CGnuPlotKeyData.cpp \
CGnuPlotLabel.cpp \
CGnuPlotLineStyle.cpp \
CGnuPlotLineType.cpp \
CGnuPlotPalette.cpp \
CGnuPlotPieObject.cpp \
CGnuPlotPlot.cpp \
CGnuPlotPoint.cpp \
CGnuPlotPolygon.cpp \
CGnuPlotPolygonObject.cpp \
CGnuPlotPosition.cpp \
CGnuPlotPrintFile.cpp \
CGnuPlotRectangle.cpp \
CGnuPlotRectObject.cpp \
CGnuPlotRenderer.cpp \
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
CGnuPlotText.cpp \
CGnuPlotTitle.cpp \
CGnuPlotWindow.cpp \
CGnuPlot3DRenderer.cpp \
\
CQPropertyDelegate.cpp \
CQPropertyEditor.cpp \
CQPropertyItem.cpp \
CQPropertyTree.cpp \
CQPaletteChooser.cpp \
CQPaletteDialog.cpp \
\
CQRotatedText.cpp \
CQComboSlider.cpp \
CQSlider.cpp \
CQAngleSpinBox.cpp \
CQIntegerSpin.cpp \
CQLineDash.cpp \
CQPoint2DEdit.cpp \
CQIconCombo.cpp \
\
CAdjacency.cpp \
CDelaunay.cpp \
ChordDiagram.cpp  \
CHull3D.cpp \
CSunburst.cpp \
CTreeMap.cpp \
\
CMathGeom2D.cpp \
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
CQGnuPlotAxis.h \
CQGnuPlotBarObject.h \
CQGnuPlotBubbleObject.h \
CQGnuPlotCamera.h \
CQGnuPlotCanvas.h \
CQGnuPlotCircle.h \
CQGnuPlotColorBox.h \
CQGnuPlotDevice.h \
CQGnuPlotEllipse.h \
CQGnuPlotEllipseObject.h \
CQGnuPlotEnum.h \
CQGnuPlotGroup.h \
CQGnuPlotKey.h \
CQGnuPlotLabel.h \
CQGnuPlotLineStyle.h \
CQGnuPlotObject.h \
CQGnuPlotPalette.h \
CQGnuPlotPieObject.h \
CQGnuPlotPlot.h \
CQGnuPlotPolygon.h \
CQGnuPlotPolygonObject.h \
CQGnuPlotRectangle.h \
CQGnuPlotRectObject.h \
CQGnuPlotRenderer.h \
CQGnuPlotTitle.h \
CQGnuPlotToolBar.h \
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
CGnuPlot3DRenderer.h \
\
CGnuPlot.h \
CGnuPlotArrow.h \
CGnuPlotArrowStyle.h \
CGnuPlotAxis.h \
CGnuPlotAxisData.h \
CGnuPlotBBoxRenderer.h \
CGnuPlotBarObject.h \
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
CGnuPlotFile.h \
CGnuPlotFillStyle.h \
CGnuPlotGroup.h \
CGnuPlotKey.h \
CGnuPlotKeyData.h \
CGnuPlotLabel.h \
CGnuPlotLineStyle.h \
CGnuPlotLineType.h \
CGnuPlotNameValues.h \
CGnuPlotObject.h \
CGnuPlotPalette.h \
CGnuPlotPieObject.h \
CGnuPlotPlot.h \
CGnuPlotPoint.h \
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
CGnuPlotText.h \
CGnuPlotTitle.h \
CGnuPlotTypes.h \
CGnuPlotUtil.h \
CGnuPlotWindow.h \
\
CQPropertyDelegate.h \
CQPropertyEditor.h \
CQPropertyItem.h \
CQPropertyTree.h \
CQPaletteChooser.h \
CQPaletteDialog.h \
\
CQRotatedText.h \
CQComboSlider.h \
CQSlider.h \
CQAngleSpinBox.h \
CQIntegerSpin.h \
CQLineDash.h \
CQPoint2DEdit.h \
CQIconCombo.h \
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
