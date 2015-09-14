#ifndef CGnuPlotStyleData_H
#define CGnuPlotStyleData_H

#include <CGnuPlotTextBoxStyle.h>
#include <CGnuPlotEllipseStyle.h>
#include <CGnuPlotTextStyle.h>
#include <CGnuPlotPieStyle.h>

struct CGnuPlotStyleData {
  CGnuPlotArrowStyle   arrow;
  CGnuPlotTextBoxStyle textBox;
  CGnuPlotEllipseStyle ellipse;
  CGnuPlotTextStyle    text;
  CGnuPlotLabelStyle   label;
  CGnuPlotPieStyle     pie;
};

#endif
