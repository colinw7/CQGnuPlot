#include <CGnuPlotEveryData.h>
#include <CExpr.h>
#include <CStrUtil.h>

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

CGnuPlotEveryData::
CGnuPlotEveryData(const std::string &str)
{
  if (str != "")
    (void) parse(str);
}

bool
CGnuPlotEveryData::
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

  parseInd(0, pointStep_ , 1);
  parseInd(1, blockStep_ , 1);
  parseInd(2, pointStart_, 0);
  parseInd(3, blockStart_, 0);
  parseInd(4, pointEnd_  , std::numeric_limits<int>::max());
  parseInd(5, blockEnd_  , std::numeric_limits<int>::max());

  return true;
}
