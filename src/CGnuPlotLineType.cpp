#include <CGnuPlotLineType.h>
#include <CGnuPlotGroup.h>

CGnuPlotLineType::
CGnuPlotLineType()
{
}

const CGnuPlotDash &
CGnuPlotLineType::
lineDash() const
{
  if (ind_ <= 0 && ! lineDash_.isValid()) {
    CGnuPlotLineType *th = const_cast<CGnuPlotLineType *>(this);

    th->lineDash_.setDash(CGnuPlotStyleInst->indexDash(ind_));
  }

  return lineDash_;
}

const CGnuPlotColorSpec &
CGnuPlotLineType::
lineColor() const
{
  if (ind_ <= 0 && ! lineColor_.isValid()) {
    CGnuPlotLineType *th = const_cast<CGnuPlotLineType *>(this);

    th->lineColor_.setRGB(CGnuPlotStyleInst->indexColor(ind_));
  }

  return lineColor_;
}

CRGBA
CGnuPlotLineType::
calcColor(const CGnuPlotGroup *group, const CRGBA &c) const
{
  if      (ind_ < 0) {
    CGnuPlotColorSpec c = lineColor();

    return c.color();
  }
  else if (ind_ == -3) {
    if (group)
      return group->backgroundColor();
    else
      return CRGBA(1,1,1);
  }
  else if (lineColor_.isValid()) {
    if      (lineColor_.type() == CGnuPlotColorSpec::Type::RGB ||
             lineColor_.type() == CGnuPlotColorSpec::Type::INDEX)
      return lineColor_.color();
    else
      return c;
  }
  else if (ind_ > 0)
    return CGnuPlotStyleInst->indexColor(ind_);
  else
    return c;
}

CLineDash
CGnuPlotLineType::
calcDash(CGnuPlot *plot, const CLineDash &d) const
{
  return lineDash().calcDash(plot, d);
}

void
CGnuPlotLineType::
init(int ind)
{
  unset();

  ind_ = ind;
}

CGnuPlotLineType::SymbolType
CGnuPlotLineType::
symbolType() const
{
  if (pointType_ < 0 || pointType_ > int(SymbolType::LAST))
    return SymbolType::PLUS;

  return SymbolType(pointType_);
}

void
CGnuPlotLineType::
unset()
{
  lineWidth_     = 1;
  lineDash_      = CGnuPlotDash();
  lineColor_     = CGnuPlotColorSpec();
  pointType_     = -1;
  pointSize_     = -1;
  pointInterval_ = 0;
  palette_       = false;
}

void
CGnuPlotLineType::
show(std::ostream &os) const
{
  if (lineColor_.isValid())
    os << "linecolor " << calcColor();
  else {
    CRGBA c = CGnuPlotStyleInst->indexColor(ind_);

    os << "linecolor rgb \"" << c.getRGB().stringEncode() << "\"";
  }

  os << " linewidth " << lineWidth();

  os << " dashtype " << lineDash();

  if (pointType_ >= 0)
    os << " pointtype " << pointType();
  else
    os << " pointtype " << ind_;

  if (pointSize_ >= 0)
    os << " pointsize " << pointSize();
  else
    os << " pointsize default";

  os << " pointinterval " << pointInterval() << std::endl;
}
