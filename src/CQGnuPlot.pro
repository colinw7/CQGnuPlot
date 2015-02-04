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
CQGnuPlotCanvas.cpp \
CQGnuPlotDevice.cpp \
CQGnuPlotGroup.cpp \
CQGnuPlotLineStyle.cpp \
CQGnuPlotObject.cpp \
CQGnuPlotPlot.cpp \
CQGnuPlotRenderer.cpp \
CQGnuPlotUtil.cpp \
CQGnuPlotWindow.cpp \
CQGnuPlotAxis.cpp \
\
CGnuPlot.cpp \
CGnuPlotAxis.cpp \
CGnuPlotContour.cpp \
CGnuPlotCoordValue.cpp \
CGnuPlotGroup.cpp \
CGnuPlotObject.cpp \
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
CQGnuPlotCanvas.h \
CQGnuPlotDevice.h \
CQGnuPlotGroup.h \
CQGnuPlotLineStyle.h \
CQGnuPlotObject.h \
CQGnuPlotPlot.h \
CQGnuPlotRenderer.h \
CQGnuPlotUtil.h \
CQGnuPlotWindow.h \
CQGnuPlotAxis.h \
\
CGnuPlot.h \
CGnuPlotArrowStyle.h \
CGnuPlotAxis.h \
CGnuPlotColorSpec.h \
CGnuPlotContour.h \
CGnuPlotCoordValue.h \
CGnuPlotDevice.h \
CGnuPlotFillStyle.h \
CGnuPlotGroup.h \
CGnuPlotKeyData.h \
CGnuPlotLineStyle.h \
CGnuPlotNameValues.h \
CGnuPlotObject.h \
CGnuPlotPlot.h \
CGnuPlotPoint.h \
CGnuPlotPosition.h \
CGnuPlotPrefValue.h \
CGnuPlotRenderer.h \
CGnuPlotSVGDevice.h \
CGnuPlotSVGRenderer.h \
CGnuPlotStyle.h \
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
