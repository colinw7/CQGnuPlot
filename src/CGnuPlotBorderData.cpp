#include <CGnuPlotBorderData.h>
#include <CStrUniqueMatch.h>

void
CGnuPlotBorderData::
show(std::ostream &os) const
{
  if (! sides)
    os << "border is not drawn" << std::endl;
  else
    os << "border " << sides << " is drawn in" <<
          CStrUniqueMatch::valueToString<DrawLayer>(layer) << " layer with" <<
          " linecolor " << lineStyle <<
          " linewidth " << lineWidth <<
          " lineType " << lineType << std::endl;
}

void
CGnuPlotBorderData::
save(std::ostream &os) const
{
  os << "set border " << sides << " " <<
        CStrUniqueMatch::valueToString<DrawLayer>(layer) <<
        " lt " << lineType <<
        " linewidth " << lineWidth <<
        " dashtype solid" << std::endl;
}
