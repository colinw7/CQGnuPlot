#include <CGnuPlotFillStyle.h>
#include <CStrUniqueMatch.h>

void
CGnuPlotFillStyle::
print(std::ostream &os) const
{
  os <<  CStrUniqueMatch::valueToString<FillType>(style_);

  os << " " << density_;

  if (pattern_ != CGnuPlotTypes::FillPattern::NONE)
    os << " pattern " << CStrUniqueMatch::valueToString<FillPattern>(pattern_);

  if (transparent_) os << " transparent";

  if (border_) {
    os << " border";

    if (borderLineType_ > 0)
      os << " " << borderLineType_;
  }
}
