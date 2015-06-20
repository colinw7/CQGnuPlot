#include <CGnuPlotLineType.h>

const CRGBA &
CGnuPlotLineType::
calcColor(const CRGBA &c) const
{
  if (lineColor_.isValid()) {
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
  return lineDash_.calcDash(plot, d);
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
    os << "linecolor " << lineColor();
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
