#include <CGnuPlotUsingCols.h>
#include <CGnuPlot.h>
#include <CParseLine.h>
#include <CStrUtil.h>

namespace {

bool evaluateExpression(const std::string &expr, CExprValueP &value, bool quiet=false) {
  if (! CExprInst->evaluateExpression(expr, value))
    value = CExprValueP();

  if (! value.isValid() && ! quiet)
    std::cerr << "Eval failed: " << expr << std::endl;

  return true;
}

}

CGnuPlotUsingCols::
CGnuPlotUsingCols()
{
}

void
CGnuPlotUsingCols::
init(const std::string &str)
{
  // split using string into integer range
  auto splitUsingFn = [] (const std::string &usingStr, StringArray &usingStrs) -> bool {
    auto p = usingStr.find('-');

    if (p == std::string::npos)
      return false;

    std::string lhs = usingStr.substr(0, p);
    std::string rhs = usingStr.substr(p + 1);

    int col1, col2;

    if (CStrUtil::toInteger(lhs, &col1) && CStrUtil::toInteger(rhs, &col2)) {
      for (int i = col1; i <= col2; ++i)
        usingStrs.push_back(CStrUtil::toString(i));

      return true;
    }

    return false;
  };

  //---

  CParseLine line(str);

  std::string             usingStr;
  StringArray             usingStrs;
  bool                    isExpr = false;
  bool                    isFunc = false;
  std::string             identifier, value;
  CGnuPlotTypes::AxisType ticId;
  int                     ticInd;

  int rbrackets = 0;

  while (line.isValid()) {
    if (rbrackets == 0) {
      if (line.isChar(':')) {
        line.skipChar();

        usingStr = CStrUtil::stripSpaces(usingStr);

        if (! usingStr.empty()) {
          if      (isExpr)
            usingStr = "(" + usingStr + ")";
          else if (isFunc && CGnuPlotTicLabel::isTicLabel(identifier, ticId, ticInd)) {
            setTicLabel(ticId, ticInd, value);

            usingStr = "";
          }
          else {
            if (splitUsingFn(usingStr, usingStrs))
              usingStr = "";
          }

          if (! usingStr.empty())
            usingStrs.push_back(usingStr);
        }

        usingStr   = "";
        isExpr     = false;
        isFunc     = false;
        identifier = "";
        value      = "";
      }
      else {
        if (line.isChar('(')) {
          identifier = usingStr;

          ++rbrackets;

          if (usingStr.empty()) {
            line.skipChar();

            isExpr = true;
          }
          else {
            usingStr += line.getChar();

            isFunc = true;
          }
        }
        else
          usingStr += line.getChar();
      }
    }
    else {
      if      (line.isChar('('))
        ++rbrackets;
      else if (line.isChar(')'))
        --rbrackets;

      if (rbrackets == 0) {
        if (isExpr)
          line.skipChar();
        else
          usingStr += line.getChar();
      }
      else {
        char c = line.getChar();

        usingStr += c;
        value    += c;
      }
    }
  }

  if (! usingStr.empty()) {
    if      (isExpr)
      usingStr = "(" + usingStr + ")";
    else if (isFunc && CGnuPlotTicLabel::isTicLabel(identifier, ticId, ticInd)) {
      setTicLabel(ticId, ticInd, value);

      usingStr = "";
    }
    else {
      if (splitUsingFn(usingStr, usingStrs))
        usingStr = "";
    }

    if (! usingStr.empty())
      usingStrs.push_back(usingStr);
  }

  for (auto &str : usingStrs)
    str = CStrUtil::stripSpaces(str);

  line.skipSpace();

  for (const auto &str : usingStrs)
    addCol(str);
}

int
CGnuPlotUsingCols::
decodeValues(CGnuPlot *plot, int pointNum, const Values &fieldValues, bool &bad,
             Values &values, Params &params) const
{
  plot_ = plot;

  bad = false;

  int ns = 0;

  for (uint i = 0; i < numCols(); ++i) {
    bool ignore = false;

    const CGnuPlotUsingCol &col = getCol(i);

    CExprValueP value = decodeValue(fieldValues, col, ns, ignore, params);

    if (! ignore) {
      if (! value.isValid()) bad = true;

      values.push_back(value);
    }
  }

  //---

  for (int ind = 0; ind < 4; ++ind) {
    CGnuPlotTypes::AxisType type =
      (ind == 0 || ind == 2 ? CGnuPlotTypes::AxisType::X : CGnuPlotTypes::AxisType::Y);

    int ind1 = ind/2 + 1;

    std::string str;

    if (! this->getTicLabel(type, ind1, str))
      continue;

    CExprValueP value;

    if (! evaluateExpression(str, value, true))
      continue;

    int         ns;
    long        icol;
    std::string str1;

    if (! value.isValid() || ! value->getIntegerValue(icol))
      continue;

    CExprValueP value1 = getFieldValue(fieldValues, icol, ns);

    if (! value1.isValid() || ! value1->getStringValue(str1))
      continue;

    if (type == CGnuPlotTypes::AxisType::X)
      plot->xaxis(ind1).setTicLabel(pointNum, str1, 0);
    else
      plot->yaxis(ind1).setTicLabel(pointNum, str1, 0);
  }

  return ns;
}

CExprValueP
CGnuPlotUsingCols::
decodeValue(const Values &fieldValues, const CGnuPlotUsingCol &col,
            int &ns, bool &ignore, Params &params) const
{
  CExprValueP value;

  ignore = false;

  if (col.isInt)
    value = getFieldValue(fieldValues, col.ival, ns);
  else {
    std::string expr = col.str;

    // replace $N variables
    // TODO: easier to define $1 variables
    auto pos = expr.find('$');

    while (pos != std::string::npos) {
      int pos1 = ++pos;

      while (isdigit(expr[pos1]))
        ++pos1;

      std::string numStr = expr.substr(pos, pos1 - pos);

      int icol = 0;

      (void) CStrUtil::toInteger(numStr, &icol);

      int ns1 = 0;

      CExprValueP value1 = getFieldValue(fieldValues, icol, ns1);

      double x1;

      if (value1.isValid() && value1->getRealValue(x1)) {
        std::string rstr = CStrUtil::toString(x1);

        auto p = rstr.find('.');

        if (p == std::string::npos)
          rstr += ".";

        expr = expr.substr(0, pos - 1) + rstr + expr.substr(pos1);
      }
      else
        expr = expr.substr(0, pos - 1) + "NaN" + expr.substr(pos1);

      pos = expr.find('$');
    }

    // check for parameter function
    pos = expr.find('(');

    if (pos > 0 && pos != std::string::npos) {
      bool valid = false;

      std::string name  = expr.substr(0, pos);
      std::string name1 = expr.substr(pos + 1);

      uint j = 0;

      int brackets = 1;

      for ( ; j < name1.size(); ++j) {
        if      (name1[j] == '(')
          brackets++;
        else if (name1[j] == ')') {
          --brackets;

          if (brackets == 0)
            break;
        }
      }

      if (j < name1.size())
        name1 = name1.substr(0, j);

      CExprValueP value;

      if (! evaluateExpression(name1, value, true))
        value = CExprValueP();

      long        icol;
      std::string str1;

      if (value.isValid() && value->getIntegerValue(icol)) {
        CExprValueP value1 = getFieldValue(fieldValues, icol, ns);

        if (value1.isValid() && value1->getStringValue(str1))
          valid = true;
      }

      if (valid)
        params[name] = str1;

      ignore = true;
      expr   = "";
    }

    //----

    if (expr != "") {
      if (! evaluateExpression(expr, value, true))
        value = CExprValueP();
    }
  }

  return value;
}

CExprValueP
CGnuPlotUsingCols::
getFieldValue(const Values &fieldValues, int icol, int &ns) const
{
  CExprValueP value;

  int nf = fieldValues.size();

  if      (icol == 0)
    value = CExprInst->createRealValue(plot_->pointNum());
  else if (icol == -2)
    value = CExprInst->createRealValue(plot_->setNum());
  else if (icol > 0 && icol <= nf) {
    value = fieldValues[icol - 1];

    if (! value.isValid()) {
      ++ns;
      return CExprValueP();
    }
  }

  return value;
}

std::string
CGnuPlotUsingCols::
toString() const
{
  std::ostringstream ostr;

  print(ostr);

  return ostr.str();
}

void
CGnuPlotUsingCols::
print(std::ostream &os) const
{
  os << "using ";

  int i = 0;

  for (const auto &col : cols_) {
    if (i > 0) os << ":";

    if (col.isInt)
      os << col.ival;
    else
      os << col.str;

    ++i;
  }

  os << std::endl;
}

//-----

CGnuPlotUsingCol::
CGnuPlotUsingCol(int i) :
 str(""), isInt(true), ival(i)
{
}

CGnuPlotUsingCol::
CGnuPlotUsingCol(const std::string &str1) :
 str(str1), isInt(false), ival(-1)
{
  isInt = CStrUtil::toInteger(str1, &ival);

  if (! isInt)
    parseString();
}

bool
CGnuPlotUsingCol::
parseString()
{
  CParseLine line(str);

  if (! line.isAlpha())
    return false;

  std::string identifier;

  identifier += line.getChar();

  while (line.isAlNum())
    identifier += line.getChar();

  if (! line.isChar('('))
    return false;

  line.skipChar();

  std::string value;

  while (line.isDigit())
    value += line.getChar();

  if (! line.isChar(')'))
    return false;

  line.skipChar();

  return ticLabel.setTicLabel(identifier, value);
}
