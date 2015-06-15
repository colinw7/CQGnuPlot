#include <CGnuPlotLineStyle.h>
#include <CGnuPlot.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotWindow.h>

CGnuPlotLineStyle::
CGnuPlotLineStyle(CGnuPlot *plot) :
 plot_(plot) {
}

void
CGnuPlotLineStyle::
setLineType(int type)
{
  lineType_ = type;

  if (lineType_ < 1)
    unsetLineColor();
}

double
CGnuPlotLineStyle::
calcWidth(double w) const
{
  if (lineWidth_.isValid())
    return lineWidth_.getValue();
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->lineWidth();
    else
      return w;
  }
  else
    return w;
}

int
CGnuPlotLineStyle::
calcDashInd(int d) const
{
  if (lineDash_.isValid())
    return lineDash_.getValue();
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->lineDash();
    else
      return d;
  }
  else
    return d;
}

CLineDash
CGnuPlotLineStyle::
calcDash(const CLineDash &d) const
{
  if (lineDash_.isValid() || lineType_.isValid())
    return plot_->getLineDash(calcDashInd());
  else
    return d;
}

CRGBA
CGnuPlotLineStyle::
calcColor(CGnuPlotPlot *plot, double x) const
{
  if (lineColor_.isValid())
    return lineColor_.getValue().calcColor(plot, x);
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType;

    if (plot)
      lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->lineColor().calcColor(plot, x);
    else
      return CRGBA(0,0,0);
  }
  else
    return CRGBA(0,0,0);
}

CRGBA
CGnuPlotLineStyle::
calcColor(CGnuPlotGroup *group, const CRGBA &c) const
{
  if (lineColor_.isValid()) {
    const CGnuPlotColorSpec &color = lineColor_.getValue();

    if      (color.type() == CGnuPlotColorSpec::Type::RGB ||
             color.type() == CGnuPlotColorSpec::Type::INDEX)
      return color.color();
    else if (ind_ > 0)
      return CGnuPlotStyleInst->indexColor(ind_);
    else
      return c;
  }
  else if (lineType_.isValid()) {
    int lt = lineType_.getValue();

    if      (lt == -1)
      return CRGBA(0,0,0);
    else if (lt == -2)
      return CRGBA(0,0,0,0);
    else if (lt == -3) {
      if (group)
        return group->window()->backgroundColor();
      else
        return CRGBA(1,1,1);
    }
    else {
      CGnuPlotLineTypeP lineType = plot_->lineType(lt);

      if (lineType.isValid())
        return lineType->calcColor(c);
      else
        return c;
    }
  }
  else
    return c;
}

CGnuPlotLineStyle::SymbolType
CGnuPlotLineStyle::
calcPointType(const SymbolType &t) const
{
  if (pointType_.isValid()) {
    if (pointType_.getValue() < 0 || pointType_.getValue() > int(SymbolType::LAST))
      return SymbolType::PLUS;
    else
      return SymbolType(pointType_.getValue());
  }
  else if (pointTypeStr_ != "") {
    return SymbolType::STRING;
  }
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->symbolType();
    else
      return t;
  }
  else
    return t;
}

double
CGnuPlotLineStyle::
calcPointSize(double s) const
{
  if (pointSize_.isValid())
    return pointSize_.getValue();
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->pointSize();
    else
      return s;
  }
  else
    return s;
}

int
CGnuPlotLineStyle::
calcPointInterval(int i) const
{
  if (pointInterval_.isValid())
    return pointInterval_.getValue();
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      return lineType->pointInterval();
    else
      return i;
  }
  else
    return i;
}

bool
CGnuPlotLineStyle::
isCalcColor() const
{
  return (lineColor().isValid() && lineColor().getValue().isCalc());
}

//---

void
CGnuPlotLineStyle::
init(int ind)
{
  unset();

  ind_ = ind;
}

void
CGnuPlotLineStyle::
unset()
{
  lineType_     .setInvalid();
  lineWidth_    .setInvalid();
  lineDash_     .setInvalid();
  lineColor_    .setInvalid();
  pointType_    .setInvalid();
  pointSize_    .setInvalid();
  pointInterval_.setInvalid();

  palette_ = false;
}

void
CGnuPlotLineStyle::
print(std::ostream &os) const
{
  if (lineType_.isValid())
    os << "linetype " << lineType_.getValue() << " ";

  if (lineWidth_.isValid())
    os << "linewidth " << lineWidth_.getValue() << " ";

  if (lineColor_.isValid())
    os << "linecolor " << lineColor_.getValue() << " ";

  if (pointType_.isValid())
    os << " pointtype " << int(pointType_.getValue()) << " ";

  if (pointSize_.isValid() || lineType_.isValid())
    os << calcPointSize();
  else
    os << "default";

  if (pointInterval_.isValid())
    os << " pointinterval " << pointInterval_.getValue() << " ";
}

void
CGnuPlotLineStyle::
show(std::ostream &os) const
{
  os << "linecolor ";

  if      (palette_)
    os << "palette z";
  else if (lineColor_.isValid())
    os << lineColor();
  else if (lineType_.isValid()) {
    CGnuPlotLineTypeP lineType = plot_->lineType(lineType_.getValue());

    if (lineType.isValid())
      os << lineType->lineColor();
    else
      os << CGnuPlotColorSpec();
  }
  else
    os << CGnuPlotColorSpec();

  os << " linewidth " << calcWidth();

  if (calcDashInd() != 0)
    os << " dashtype " << calcDashInd();
  else
    os << " dashtype solid";

  os << " pointtype " << int(calcPointType());

  os << " pointsize ";

  if (calcPointSize() >= 0)
    os << calcPointSize();
  else
    os << "default";

  os << " pointinterval " << calcPointInterval() << std::endl;
}
