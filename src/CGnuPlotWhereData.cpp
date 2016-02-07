#include <CGnuPlotWhereData.h>
#include <CGnuPlotUtil.h>

CGnuPlotWhereData::
CGnuPlotWhereData(const std::string &expr)
{
  if (expr != "")
    (void) parse(expr);
}

bool
CGnuPlotWhereData::
parse(const std::string &expr)
{
  expr_ = expr;

  return true;
}

bool
CGnuPlotWhereData::
isValid(int setNum, int pointNum, const Values &fieldValues) const
{
  std::string expr = expr_;

  auto pos = expr.find('$');

  while (pos != std::string::npos) {
    int pos1 = ++pos;

    while (isdigit(expr[pos1]))
      ++pos1;

    std::string numStr = expr.substr(pos, pos1 - pos);

    int icol = 0;

    (void) CStrUtil::toInteger(numStr, &icol);

    CExprValuePtr value1 = getFieldValue(setNum, pointNum, fieldValues, icol);

    std::string lstr = expr.substr(0, pos - 1);
    std::string rstr = expr.substr(pos1);

    std::string midStr;

    if (value1.isValid()) {
      double      x1;
      std::string s1;

      if (value1->getRealValue(x1)) {
        if (! IsNaN(x1)) {
          std::string realStr = CStrUtil::toString(x1);

          auto p = realStr.find('.');

          if (p == std::string::npos)
            realStr += ".";

          midStr = realStr;
        }
        else
          midStr = "NaN";
      }
      else if (value1->getStringValue(s1)) {
        midStr = "\"" + s1 + "\"";
      }
      else
        midStr = "NaN";
    }
    else
      midStr = "NaN";

    expr = lstr + midStr + rstr;

    pos = expr.find('$');
  }

  CExprValuePtr value;

  if (expr != "") {
    if (! CGnuPlotUtil::evaluateExpression(expr, value, true))
      value = CExprValuePtr();
  }

  long l;

  if (! value.isValid() || ! value->getIntegerValue(l))
    return false;

  return (l != 0);
}

CExprValuePtr
CGnuPlotWhereData::
getFieldValue(int setNum, int pointNum, const Values &fieldValues, int icol) const
{
  CExprValuePtr value;

  int nf = fieldValues.size();

  if      (icol == 0) {
    value = CExprInst->createRealValue(pointNum);
  }
  else if (icol == -2) {
    value = CExprInst->createRealValue(setNum);
  }
  else if (icol > 0 && icol <= nf) {
    value = fieldValues[icol - 1];

    if (! value.isValid())
      return CExprValuePtr();
  }

  return value;
}
