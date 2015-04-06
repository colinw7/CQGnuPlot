#include <CGnuPlotFillStyle.h>
#include <CGnuPlotNameValues.h>

void
CGnuPlotFillStyle::
show(std::ostream &os) const
{
  os << "Fill style";

  if      (style_ == FillType::EMPTY)
    os << " is empty";
  else if (style_ == FillType::SOLID) {
    os << " uses";

    if (transparent_)
      os << " transparent";

    os << " solid color with density " << density_;
  }
  else if (style_ == FillType::PATTERN) {
    os << " uses";

    if (transparent_)
      os << " transparent";

    os << " patterns starting at " << int(pattern_);
  }

  if (border_) {
    os << " with border";

    if      (borderLineType_.isValid())
      os << " " << borderLineType_.getValue();
    else if (borderColor_.isValid())
      os << " " << borderColor_.getValue();
  }
  else
    os << " with no border";

  os << std::endl;
}

void
CGnuPlotFillStyle::
print(std::ostream &os) const
{
  os << CStrUniqueMatch::valueToString<CGnuPlotTypes::FillType>(style_);

  os << " " << density_;

  if (pattern_ != CGnuPlotTypes::FillPattern::NONE)
    os << " pattern " << CStrUniqueMatch::valueToString<FillPattern>(pattern_);

  if (transparent_) os << " transparent";

  if (border_) {
    os << " border";

    if (borderLineType_.isValid())
      os << " " << borderLineType_.getValue();
  }
}
