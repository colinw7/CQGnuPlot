#include <CGnuPlotLineStyle.h>
#include <CGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>

CGnuPlotLineStyle::
CGnuPlotLineStyle(CGnuPlot *plot) :
 plot_(plot) {
}

void
CGnuPlotLineStyle::
setLineType(int type)
{
  lineType_ = type;

  unsetLineColor();
}

double
CGnuPlotLineStyle::
calcWidth(double w) const
{
  if      (lineWidth_)
    return lineWidth_.value();
  else if (lineType_) {
    auto lineType = plot_->getLineTypeInd(lineType_.value());

    if (lineType)
      return lineType->lineWidth();
    else
      return w;
  }
  else
    return w;
}

CLineDash
CGnuPlotLineStyle::
calcDash(const CLineDash &d) const
{
  if      (lineDash_.isValid())
    return lineDash_.calcDash(plot_, d);
  else if (lineType_) {
    auto lineType = plot_->getLineTypeInd(lineType_.value());

    if (lineType)
      return lineType->calcDash(plot_, d);
    else
      return d;
  }
  else
    return d;
}

CRGBA
CGnuPlotLineStyle::
calcColor(CGnuPlotPlot *plot, double x) const
{
  CGnuPlotGroup *group = plot->group();

  if      (lineColor_)
    return lineColor_.value().calcColor(group, x);
  else if (lineType_) {
    CGnuPlotLineTypeP lineType;

    if (plot_)
      lineType = plot_->getLineTypeInd(lineType_.value());

    if (lineType)
      return lineType->lineColor().calcColor(group, x);
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
  if      (lineColor_) {
    const auto &color = lineColor_.value();

    if      (color.type() == CGnuPlotColorSpec::Type::RGB ||
             color.type() == CGnuPlotColorSpec::Type::INDEX)
      return color.color();
    else if (ind_)
      return CGnuPlotStyleInst->indexColor(ind_.value());
    else
      return c;
  }
  else if (lineType_) {
    int lt = lineType_.value();

    if      (lt == 0 || lt == -1 || lt == -2)
      return CGnuPlotStyleInst->indexColor(lt);
    else if (lt == -3) {
      if (group)
        return group->window()->backgroundColor();
      else
        return CRGBA(1,1,1);
    }
    else {
      CGnuPlotLineTypeP lineType = plot_->getLineTypeInd(lt);

      if (lineType)
        return lineType->calcColor(group, c);
      else
        return c;
    }
  }
  else
    return c;
}

CGnuPlotTypes::SymbolType
CGnuPlotLineStyle::
calcPointType(const SymbolType &t) const
{
  if      (pointType_) {
    if (pointType_.value() < 0 || pointType_.value() > int(SymbolType::LAST))
      return SymbolType::PLUS;
    else
      return SymbolType(pointType_.value());
  }
  else if (pointTypeStr_ != "") {
    return SymbolType::STRING;
  }
  else if (lineType_) {
    int lt = lineType_.value();

    CGnuPlotLineTypeP lineType = plot_->getLineTypeInd(lt);

    if (lineType)
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
  if      (pointSize_)
    return pointSize_.value();
  else if (lineType_) {
    auto lineType = plot_->getLineTypeInd(lineType_.value());

    if (lineType)
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
  if      (pointInterval_)
    return pointInterval_.value();
  else if (lineType_) {
    auto lineType = plot_->getLineTypeInd(lineType_.value());

    if (lineType)
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
  return (lineColor() && lineColor().value().isCalc());
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
  lineType_     .reset();
  lineWidth_    .reset();
  lineDash_     .reset();
  lineColor_    .reset();
  pointType_    .reset();
  pointSize_    .reset();
  pointInterval_.reset();

  palette_ = false;
}

void
CGnuPlotLineStyle::
print(std::ostream &os) const
{
  if (lineType_)
    os << "linetype " << lineType_.value() << " ";

  if (lineWidth_)
    os << "linewidth " << lineWidth_.value() << " ";

  if (lineColor_)
    os << "linecolor " << lineColor_.value() << " ";

  if (pointType_)
    os << " pointtype " << int(pointType_.value()) << " ";

  if (pointSize_ || lineType_)
    os << calcPointSize();
  else
    os << "default";

  if (pointInterval_)
    os << " pointinterval " << pointInterval_.value() << " ";
}

void
CGnuPlotLineStyle::
show(std::ostream &os) const
{
  if (lineType_) {
    os << "lt " << lineType_.value();
  }
  else {
    os << "linecolor ";

    if      (palette_)
      os << "palette z";
    else if (lineColor_)
      os << lineColor().value();
    else
    os << CGnuPlotColorSpec();
  }

  os << " linewidth " << calcWidth();

  os << " dashtype " << calcDash();

  os << " pointtype " << int(calcPointType());

  os << " pointsize ";

  if (calcPointSize() >= 0)
    os << calcPointSize();
  else
    os << "default";

  os << " pointinterval " << calcPointInterval() << "\n";
}
