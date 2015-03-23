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
    os << "key is OFF" << std::endl;
    return;
  }

  os << "key is ON, position:";

  if (pos_.isValid()) {
    os << " " << pos_.getValue();
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
  os << std::endl;

  os << "key is";
  os << (justify_ == CHALIGN_TYPE_RIGHT ? " right" : " left") << " justified,";
  os << (reverse_  ? " reversed" : " not reversed") << ",";
  os << (invert_   ? " inverted" : " not inverted") << ",";
  os << (enhanced_ ? " enhanced" : " not enhanced");

  if (box_) {
    os << " boxed" << std::endl;

    os << "with linetype " << boxLineType_.getValue(-1) <<
          " linestyle " << boxLineStyle_.getValue(-1) <<
          " linewidth " << boxLineWidth_ << std::endl;

    os << "key box is " << (opaque_ ? "opaque" : "not opaque") <<
          "and drawn in front of the graph" << std::endl;
  }
  else
    os << " not boxed" << std::endl;

  os << "sample length is " << sampLen_.getValue(4) << " characters" << std::endl;
  os << "vertical spacing is " << spacing_.getValue(1) << " characters" << std::endl;
  os << "width adjustment is " << widthIncrement_ << " characters" << std::endl;
  os << "height adjustment is " << heightIncrement_ << " characters" << std::endl;

  if (autotitle_) {
    if (columnhead_)
      os << "curves are automatically titled with column header" << std::endl;
    else
      os << "curves are automatically titled with filename" << std::endl;
  }
  else
    os << "curves are not automatically titled" << std::endl;

  os << "maximum number of columns is ";

  if (! maxCols_.isValid())
    os << "calculated automatically" << std::endl;
  else
    os << maxCols_.getValue() << " for horizontal alignment" << std::endl;

  os << "maximum number of rows is ";

  if (! maxRows_.isValid())
    os << "calculated automatically" << std::endl;
  else
    os << maxRows_.getValue() << " for vertical alignment" << std::endl;

  os << "key title is \"" << title_.getValue("") << "\"" << std::endl;
}
