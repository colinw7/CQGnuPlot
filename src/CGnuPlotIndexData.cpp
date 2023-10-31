#include <CGnuPlotIndexData.h>
#include <CExpr.h>
#include <CStrUtil.h>
#include <limits>

namespace {

bool evaluateExpression(CExpr *expr, const std::string &exprStr, CExprValuePtr &value) {
  bool oldQuiet = expr->getQuiet();

  expr->setQuiet(true);

  if (! expr->evaluateExpression(exprStr, value))
    value = CExprValuePtr();

  expr->setQuiet(oldQuiet);

  return true;
}

}

//---

CGnuPlotIndexData::
CGnuPlotIndexData(CExpr *expr, const std::string &str)
{
  if (str != "")
    (void) parse(expr, str);
}

bool
CGnuPlotIndexData::
parse(CExpr *expr, const std::string &str)
{
  std::vector<std::string> inds;

  CStrUtil::addFields(str, inds, ":");

  //---

  auto parseInd = [&](int i, int &var, int def) {
    var = def;

    if (int(inds.size()) > i) {
      CExprValuePtr value;

      var = def;

      if (inds[i] != "" && evaluateExpression(expr, inds[i], value)) {
        long l;

        if (value && value->getIntegerValue(l))
          var = int(l);
      }
    }
  };

  parseInd(0, start_, 0);
  parseInd(1, end_  , start_ /*std::numeric_limits<int>::max()*/);
  parseInd(2, step_ , 1);

  return true;
}
