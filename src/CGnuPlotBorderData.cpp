#include <CGnuPlotBorderData.h>
#include <CStrUniqueMatch.h>

void
CGnuPlotBorderData::
show(std::ostream &os) const
{
  int s = sides().value_or(31);

  if (s)
    os << "border is not drawn\n";
  else
    os << "border " << s << " is drawn in" <<
          CStrUniqueMatch::valueToString<DrawLayer>(layer()) << " layer with" <<
          " linecolor " << lineStyle().value() <<
          " linewidth " << lineWidth() <<
          " lineType " << lineType().value() << "\n";
}

void
CGnuPlotBorderData::
save(std::ostream &os) const
{
  int s = sides().value_or(31);

  os << "set border " << s << " " <<
        CStrUniqueMatch::valueToString<DrawLayer>(layer()) <<
        " lt " << lineType().value() <<
        " linewidth " << lineWidth() <<
        " dashtype solid\n";
}
