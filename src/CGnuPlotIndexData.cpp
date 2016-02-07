#include <CGnuPlotIndexData.h>
#include <CExpr.h>
#include <CStrUtil.h>
#include <limits>

namespace {

bool evaluateExpression(const std::string &expr, CExprValuePtr &value) {
  bool oldQuiet = CExprInst->getQuiet();

  CExprInst->setQuiet(true);

  if (! CExprInst->evaluateExpression(expr, value))
    value = CExprValuePtr();

  CExprInst->setQuiet(oldQuiet);

  return true;
}

}

//---

CGnuPlotIndexData::
CGnuPlotIndexData(const std::string &str)
{
  if (str != "")
    (void) parse(str);
}

bool
CGnuPlotIndexData::
parse(const std::string &str)
{
  std::vector<std::string> inds;

  CStrUtil::addFields(str, inds, ":");

  //---

  auto parseInd = [&](int i, int &var, int def) {
    var = def;

    if (int(inds.size()) > i) {
      CExprValuePtr value;

      var = def;

      if (inds[i] != "" && evaluateExpression(inds[i], value)) {
        long l;

        if (value.isValid() && value->getIntegerValue(l))
          var = l;
      }
    }
  };

  parseInd(0, start_, 0);
  parseInd(1, end_  , start_ /*std::numeric_limits<int>::max()*/);
  parseInd(2, step_ , 1);

  return true;
}
