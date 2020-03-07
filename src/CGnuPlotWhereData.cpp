#include <CGnuPlotWhereData.h>
#include <CGnuPlotUtil.h>

CGnuPlotWhereData::
CGnuPlotWhereData(const std::string &exprStr)
{
  if (exprStr != "")
    (void) parse(exprStr);
}

bool
CGnuPlotWhereData::
parse(const std::string &exprStr)
{
  exprStr_ = exprStr;

  return true;
}

bool
CGnuPlotWhereData::
isValid(CExpr *expr, int setNum, int pointNum, const Values &fieldValues) const
{
  std::string exprStr = exprStr_;

  auto pos = exprStr.find('$');

  while (pos != std::string::npos) {
    int pos1 = ++pos;

    while (isdigit(exprStr[pos1]))
      ++pos1;

    std::string numStr = exprStr.substr(pos, pos1 - pos);

    int icol = 0;

    (void) CStrUtil::toInteger(numStr, &icol);

    CExprValuePtr value1 = getFieldValue(expr, setNum, pointNum, fieldValues, icol);

    std::string lstr = exprStr.substr(0, pos - 1);
    std::string rstr = exprStr.substr(pos1);

    std::string midStr;

    if (value1.isValid()) {
      double      x1;
      std::string s1;

      if (value1->getRealValue(x1)) {
        if (! COSNaN::is_nan(x1)) {
          std::string realStr = CGnuPlotUtil::toString(x1);

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

    exprStr = lstr + midStr + rstr;

    pos = exprStr.find('$');
  }

  CExprValuePtr value;

  if (exprStr != "") {
    if (! CGnuPlotUtil::evaluateExpression(expr, exprStr, value, true))
      value = CExprValuePtr();
  }

  long l;

  if (! value.isValid() || ! value->getIntegerValue(l))
    return false;

  return (l != 0);
}

CExprValuePtr
CGnuPlotWhereData::
getFieldValue(CExpr *expr, int setNum, int pointNum, const Values &fieldValues, int icol) const
{
  CExprValuePtr value;

  int nf = fieldValues.size();

  if      (icol == 0) {
    value = expr->createRealValue(pointNum);
  }
  else if (icol == -2) {
    value = expr->createRealValue(setNum);
  }
  else if (icol > 0 && icol <= nf) {
    value = fieldValues[icol - 1];

    if (! value.isValid())
      return CExprValuePtr();
  }

  return value;
}
