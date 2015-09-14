#include <CGnuPlotPoint.h>
#include <CGnuPlotStyle.h>
#include <CRGBName.h>

CGnuPlotPoint::
CGnuPlotPoint(const Values &values, bool discontinuity, bool bad, const Params &params) :
 values_(values), discontinuity_(discontinuity), bad_(bad), params_(params)
{
}

CGnuPlotPoint::
~CGnuPlotPoint()
{
}

bool
CGnuPlotPoint::
getX(double &x) const
{
  return getValue(1, x);
}

bool
CGnuPlotPoint::
getY(double &y) const
{
  return getValue(2, y);
}

bool
CGnuPlotPoint::
getZ(double &z) const
{
  return getValue(3, z);
}

double
CGnuPlotPoint::
getX() const
{
  double x = 0.0;

  getX(x);

  return x;
}

double
CGnuPlotPoint::
getY() const
{
  double y = 0.0;

  getY(y);

  return y;
}

double
CGnuPlotPoint::
getZ() const
{
  double z = 0.0;

  getZ(z);

  return z;
}

bool
CGnuPlotPoint::
getValue(int n, double &r) const
{
  if (int(values_.size()) < n || ! values_[n - 1].isValid())
    return false;

  return values_[n - 1]->getRealValue(r);
}

bool
CGnuPlotPoint::
getValue(int n, int &i) const
{
  if (int(values_.size()) < n || ! values_[n - 1].isValid())
    return false;

  long l;

  if (! values_[n - 1]->getIntegerValue(l))
    return false;

  i = l;

  return true;
}

bool
CGnuPlotPoint::
getValue(int n, std::string &str) const
{
  if (int(values_.size()) < n || ! values_[n - 1].isValid())
    return false;

  return values_[n - 1]->getStringValue(str);
}

std::string
CGnuPlotPoint::
getParamString(const std::string &name) const
{
  std::string str;

  CExprValuePtr value = getParam(name);

  if (! value.isValid())
    return str;

  if (value->isStringValue())
    (void) value->getStringValue(str);

  return str;
}

CRGBA
CGnuPlotPoint::
getParamColor(const std::string &name) const
{
  CRGBA c(0,0,0);

  CExprValuePtr value = getParam(name);

  if (! value.isValid())
    return c;

  if (value->isStringValue()) {
    std::string str;

    if (value->getStringValue(str))
      c = CRGBName::toRGBA(str);
  }
  else if (value->isIntegerValue() || value->isRealValue()) {
    long i;

    if (value->getIntegerValue(i))
      c = CGnuPlotStyleInst->indexColor(i);
  }

  return c;
}

int
CGnuPlotPoint::
cmp(const CGnuPlotPoint &p) const
{
  double x1, y1, x2, y2;

  (void) getX(x1);
  (void) getY(y1);

  (void) p.getX(x2);
  (void) p.getY(y2);

  if (x1 < x2) return -1;
  if (x1 > x2) return  1;
  if (y1 < y2) return -1;
  if (y1 > y2) return  1;

  return 0;
}

void
CGnuPlotPoint::
print(std::ostream &os) const
{
  os << "(";

  for (uint i = 0; i < values_.size(); ++i) {
    if (i > 0) os << ",";

    os << values_[i];
  }

  os << ")";
}
