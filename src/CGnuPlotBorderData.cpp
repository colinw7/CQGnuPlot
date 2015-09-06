#include <CGnuPlotBorderData.h>
#include <CStrUniqueMatch.h>

void
CGnuPlotBorderData::
show(std::ostream &os) const
{
  int s = sides().getValue(31);

  if (s)
    os << "border is not drawn" << std::endl;
  else
    os << "border " << s << " is drawn in" <<
          CStrUniqueMatch::valueToString<DrawLayer>(layer()) << " layer with" <<
          " linecolor " << lineStyle() <<
          " linewidth " << lineWidth() <<
          " lineType " << lineType() << std::endl;
}

void
CGnuPlotBorderData::
save(std::ostream &os) const
{
  int s = sides().getValue(31);

  os << "set border " << s << " " <<
        CStrUniqueMatch::valueToString<DrawLayer>(layer()) <<
        " lt " << lineType() <<
        " linewidth " << lineWidth() <<
        " dashtype solid" << std::endl;
}
