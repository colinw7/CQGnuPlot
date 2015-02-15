TEMPLATE = app

QT += widgets

TARGET = CQGnuPlot

DEPENDPATH += .

INCLUDEPATH += . ../include

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR

CONFIG += debug

# Input
SOURCES += \
CQGnuPlot.cpp \
CQGnuPlotAnnotation.cpp \
CQGnuPlotAxis.cpp \
CQGnuPlotBar.cpp \
CQGnuPlotBubble.cpp \
CQGnuPlotCanvas.cpp \
CQGnuPlotDevice.cpp \
CQGnuPlotGroup.cpp \
CQGnuPlotKey.cpp \
CQGnuPlotLineStyle.cpp \
CQGnuPlotPie.cpp \
CQGnuPlotPlot.cpp \
CQGnuPlotRenderer.cpp \
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
\
CGnuPlot.cpp \
CGnuPlotAxis.cpp \
CGnuPlotBar.cpp \
CGnuPlotBubble.cpp \
CGnuPlotContour.cpp \
CGnuPlotCoordValue.cpp \
CGnuPlotDevice.cpp \
CGnuPlotFile.cpp \
CGnuPlotGroup.cpp \
CGnuPlotKey.cpp \
CGnuPlotObject.cpp \
CGnuPlotPie.cpp \
CGnuPlotPlot.cpp \
CGnuPlotPoint.cpp \
CGnuPlotPosition.cpp \
CGnuPlotRenderer.cpp \
CGnuPlotSVGDevice.cpp \
CGnuPlotSVGRenderer.cpp \
CGnuPlotWindow.cpp \
\
CQPropertyDelegate.cpp \
CQPropertyEditor.cpp \
CQPropertyItem.cpp \
CQPropertyTree.cpp \
CQPaletteChooser.cpp \
CQPaletteDialog.cpp \
\
CMathGeom2D.cpp \
CArcToBezier.cpp \
CSymbol2D.cpp \
\
Expr/CExprBValue.cpp \
Expr/CExprCompile.cpp \
Expr/CExpr.cpp \
Expr/CExprExecute.cpp \
Expr/CExprFunction.cpp \
Expr/CExprInterp.cpp \
Expr/CExprIValue.cpp \
Expr/CExprOperator.cpp \
Expr/CExprParse.cpp \
Expr/CExprRValue.cpp \
Expr/CExprStrgen.cpp \
Expr/CExprSValue.cpp \
Expr/CExprValue.cpp \
Expr/CExprVariable.cpp \
\
MouseMode/CQMouseMode.cpp \
MouseMode/CQPanMouseMode.cpp \
MouseMode/CQZoomMouseMode.cpp \

HEADERS += \
CQGnuPlot.h \
CQGnuPlotAnnotation.h \
CQGnuPlotAxis.h \
CQGnuPlotBar.h \
CQGnuPlotBubble.h \
CQGnuPlotCanvas.h \
CQGnuPlotDevice.h \
CQGnuPlotGroup.h \
CQGnuPlotKey.h \
CQGnuPlotLineStyle.h \
CQGnuPlotObject.h \
CQGnuPlotPie.h \
CQGnuPlotPlot.h \
CQGnuPlotRenderer.h \
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
\
CGnuPlotArrowStyle.h \
CGnuPlotAxis.h \
CGnuPlotBar.h \
CGnuPlotBubble.h \
CGnuPlotCache.h \
CGnuPlotColorSpec.h \
CGnuPlotContour.h \
CGnuPlotCoordValue.h \
CGnuPlotDevice.h \
CGnuPlotFile.h \
CGnuPlotFillStyle.h \
CGnuPlotGroup.h \
CGnuPlot.h \
CGnuPlotKeyData.h \
CGnuPlotKey.h \
CGnuPlotLineStyle.h \
CGnuPlotNameValues.h \
CGnuPlotObject.h \
CGnuPlotPie.h \
CGnuPlotPlot.h \
CGnuPlotPoint.h \
CGnuPlotPosition.h \
CGnuPlotPrefValue.h \
CGnuPlotRenderer.h \
CGnuPlotStyle.h \
CGnuPlotSVGDevice.h \
CGnuPlotSVGRenderer.h \
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
CMathGeom2D.h \
CArcToBezier.h \
CSymbol2D.h \
CStrUniqueMatch.h \
\
Expr/CExprBValue.h \
Expr/CExprCompile.h \
Expr/CExprError.h \
Expr/CExprExecute.h \
Expr/CExprFunction.h \
Expr/CExpr.h \
Expr/CExprI.h \
Expr/CExprInterp.h \
Expr/CExprIValue.h \
Expr/CExprOperator.h \
Expr/CExprParse.h \
Expr/CExprRValue.h \
Expr/CExprSValue.h \
Expr/CExprValue.h \
Expr/CExprVariable.h \
\
MouseMode/CQMouseMode.h \
MouseMode/CQPanMouseMode.h \
MouseMode/CQPanZoomIFace.h \
MouseMode/CQZoomMouseMode.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. MouseMode Expr ../include \
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
