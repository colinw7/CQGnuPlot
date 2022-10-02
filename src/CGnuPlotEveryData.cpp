#include <CGnuPlotEveryData.h>
#include <CExpr.h>
#include <CStrUtil.h>

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

CGnuPlotEveryData::
CGnuPlotEveryData(CExpr *expr, const std::string &str)
{
  if (str != "")
    (void) parse(expr, str);
}

bool
CGnuPlotEveryData::
parse(CExpr *expr, const std::string &str)
{
  assert(expr);

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

        if (value.isValid() && value->getIntegerValue(l))
          var = int(l);
      }
    }
  };

  parseInd(0, pointStep_ , 1);
  parseInd(1, blockStep_ , 1);
  parseInd(2, pointStart_, 0);
  parseInd(3, blockStart_, 0);
  parseInd(4, pointEnd_  , std::numeric_limits<int>::max());
  parseInd(5, blockEnd_  , std::numeric_limits<int>::max());

  return true;
}
