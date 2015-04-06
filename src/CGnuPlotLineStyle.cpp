#include <CGnuPlotLineStyle.h>

void
CGnuPlotLineStyle::
unset()
{
  type_          = COptInt();
  width_         = 1;
  dash_          = CLineDash();
  color_         = CGnuPlotColorSpec();
  pointType_     = static_cast<CGnuPlotTypes::SymbolType>(ind_);
  pointSize_     = COptReal();
  pointInterval_ = 0;
  palette_       = false;
}

void
CGnuPlotLineStyle::
print(std::ostream &os) const
{
  if (type_.isValid())
    os << "linetype " << type_.getValue();
  else
    os << "linecolor " << color_ << ",";

  os << " linewidth " << width_;
  os << " pointtype " << int(pointType_);
  os << " pointsize " << pointSizeStr();
  os << " pointinterval " << pointInterval_;
}

void
CGnuPlotLineStyle::
show(std::ostream &os) const
{
  os << "linecolor ";

  if      (palette_)
    os << "palette z";
  else if (type_.isValid()) {
    CGnuPlotColorSpec cs;

    cs.setRGB(CGnuPlotStyleInst->indexColor(type_.getValue()));

    os << cs << ",";
  }
  else if (color_.isValid())
    os << color_ << ",";
  else
    os << ind_ << ",";

  os << " linewidth " << width_ << " dashtype " << dash_;
  os << " pointtype " << int(pointType_);
  os << " pointsize " << (! pointSize_.isValid() ? std::string("default") :
                          CStrUtil::toString(pointSize_.getValue(1)));
  os << " pointinterval " << pointInterval_ << std::endl;
}
