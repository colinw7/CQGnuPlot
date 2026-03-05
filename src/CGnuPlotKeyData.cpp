#include <CGnuPlotKeyData.h>
#include <CFontMgr.h>

CGnuPlotKeyData::
CGnuPlotKeyData()
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}

void
CGnuPlotKeyData::
show(std::ostream &os) const
{
  if (! displayed_) {
    os << "key is OFF\n";
    return;
  }

  os << "key is ON, position:";

  if (pos_) {
    os << " " << pos_.value();
  }
  else {
    if      (valign_ == CVALIGN_TYPE_TOP)
      os << " top";
    else if (valign_ == CVALIGN_TYPE_CENTER)
      os << " center";
    else if (valign_ == CVALIGN_TYPE_BOTTOM)
      os << " bottom";

    if      (halign_ == CHALIGN_TYPE_LEFT)
      os << " left";
    else if (halign_ == CHALIGN_TYPE_CENTER)
      os << " center";
    else if (halign_ == CHALIGN_TYPE_RIGHT)
      os << " right";
  }

  os << (vertical_ ? " vertical" : " horizontal");
  os << (outside_  ? " outside"  : " inside");
  os << "\n";

  os << "key is";
  os << (justify_ == CHALIGN_TYPE_RIGHT ? " right" : " left") << " justified,";
  os << (reverse_  ? " reversed" : " not reversed") << ",";
  os << (invert_   ? " inverted" : " not inverted") << ",";
  os << (enhanced_ ? " enhanced" : " not enhanced");

  if (box_) {
    os << " boxed\n";

    os << "with linetype " << boxLineType_.value_or(-1) <<
          " linestyle " << boxLineStyle_.value_or(-1) <<
          " linewidth " << boxLineWidth_ << "\n";

    os << "key box is " << (opaque_ ? "opaque" : "not opaque") <<
          "and drawn in front of the graph\n";
  }
  else
    os << " not boxed\n";

  os << "sample length is " << sampLen_.value_or(4) << " characters\n";
  os << "vertical spacing is " << spacing_.value_or(1) << " characters\n";
  os << "width adjustment is " << widthIncrement_ << " characters\n";
  os << "height adjustment is " << heightIncrement_ << " characters\n";

  if (autotitle_) {
    if (columnhead_)
      os << "curves are automatically titled with column header\n";
    else
      os << "curves are automatically titled with filename\n";
  }
  else
    os << "curves are not automatically titled\n";

  os << "maximum number of columns is ";

  if (! maxCols_)
    os << "calculated automatically\n";
  else
    os << maxCols_.value() << " for horizontal alignment\n";

  os << "maximum number of rows is ";

  if (! maxRows_)
    os << "calculated automatically\n";
  else
    os << maxRows_.value() << " for vertical alignment\n";

  os << "key title is \"" << title_.value_or("") << "\"\n";
}
