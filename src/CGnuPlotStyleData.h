#ifndef CGnuPlotStyleData_H
#define CGnuPlotStyleData_H

#include <CGnuPlotBoxWidth.h>
#include <CGnuPlotBoxPlotStyleValue.h>
#include <CGnuPlotCirclesStyleValue.h>
#include <CGnuPlotEllipsesStyleValue.h>
#include <CGnuPlotPieChartStyleValue.h>
#include <CGnuPlotTextBoxStyle.h>
#include <CGnuPlotTextStyle.h>
#include <CGnuPlotVectorsStyleValue.h>

struct CGnuPlotStyleData {
  CGnuPlotBoxWidth            boxWidth;
  CGnuPlotBoxPlotStyleValue   boxPlotStyleValue;
  CGnuPlotCirclesStyleValue   circlesStyleValue;
  CGnuPlotEllipsesStyleValue  ellipsesStyleValue;
  CGnuPlotLabelStyle          label;
  CGnuPlotPieChartStyleValue  pieChartStyleValue;
  CGnuPlotTextBoxStyle        textBox;
  CGnuPlotTextStyle           text;
  CGnuPlotVectorsStyleValue   vectorsStyleValue;
};

#endif
