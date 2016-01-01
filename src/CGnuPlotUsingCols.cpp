#include <CGnuPlotUsingCols.h>
#include <CGnuPlot.h>
#include <CParseLine.h>
#include <CStrUtil.h>

namespace {

bool evaluateExpression(const std::string &expr, CExprValuePtr &value, bool quiet=false) {
  if (! CExprInst->evaluateExpression(expr, value))
    value = CExprValuePtr();

  if (! value.isValid() && ! quiet)
    std::cerr << "Eval failed: " << expr << std::endl;

  return true;
}

}

CGnuPlotUsingCols::
CGnuPlotUsingCols(CGnuPlot *plot) :
 str_(), plot_(plot)
{
}

void
CGnuPlotUsingCols::
init(const std::string &str)
{
  str_ = str;

  cols_.clear();

  colMin_.setInvalid();
  colMax_.setInvalid();

  axisTicLabel_.clear();

  format_   = "";
  keyLabel_ = "";

  //---

  CParseLine line(str_);

  CGnuPlotUsingColData usingData;
  StringArray          usingStrs;

  int rbrackets = 0;

  while (line.isValid()) {
    if (rbrackets == 0) {
      if      (line.isChar(':')) {
        line.skipChar();

        usingData.usingStr = CStrUtil::stripSpaces(usingData.usingStr);

        if (! usingData.usingStr.empty())
          processUsingStr(usingData, usingStrs);

        usingData.reset();
      }
      else if (line.isChar('"')) {
        int pos = line.pos();

        line.skipChar();

        while (line.isValid()) {
          if      (line.isChar('\\')) {
            line.skipChar();

            if (line.isValid())
              line.skipChar();
          }
          else if (line.isChar('\"')) {
            line.skipChar();

            break;
          }
          else
            line.skipChar();
        }

        usingData.usingStr += line.substr(pos, line.pos() - pos);
        usingData.value    += line.substr(pos, line.pos() - pos);

        line.skipSpace();
      }
      else {
        if (line.isChar('(')) {
          usingData.identifier = usingData.usingStr;

          ++rbrackets;

          if (usingData.usingStr.empty()) {
            line.skipChar();

            usingData.isExpr = true;
          }
          else {
            usingData.usingStr += line.getChar();

            usingData.isFunc = true;
          }
        }
        else
          usingData.usingStr += line.getChar();
      }
    }
    else {
      if      (line.isChar('('))
        ++rbrackets;
      else if (line.isChar(')'))
        --rbrackets;

      if (rbrackets == 0) {
        if (usingData.isExpr)
          line.skipChar();
        else
          usingData.usingStr += line.getChar();
      }
      else {
        char c = line.getChar();

        usingData.usingStr += c;
        usingData.value    += c;
      }
    }
  }

  if (! usingData.usingStr.empty())
    processUsingStr(usingData, usingStrs);

  for (auto &str : usingStrs)
    str = CStrUtil::stripSpaces(str);

  line.skipSpace();

  for (const auto &usingStr : usingStrs)
    addCol(usingStr);
}

void
CGnuPlotUsingCols::
processUsingStr(CGnuPlotUsingColData &usingData, StringArray &usingStrs)
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

  if      (usingData.isExpr)
    usingData.usingStr = "(" + usingData.usingStr + ")";
  else if (usingData.isFunc &&
           CGnuPlotTicLabel::isTicLabel(usingData.identifier, usingData.ticId, usingData.ticInd)) {
    setTicLabel(usingData.ticId, usingData.ticInd, usingData.value);

    usingData.usingStr = "";
  }
  else if (usingData.isFunc && usingData.identifier == "key") {
    keyLabel_ = usingData.value;

    usingData.usingStr = "";
  }
  else {
    if (splitUsingFn(usingData.usingStr, usingStrs))
      usingData.usingStr = "";
  }

  if (! usingData.usingStr.empty())
    usingStrs.push_back(usingData.usingStr);
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

    CExprValuePtr value = decodeValue(fieldValues, col, ns, ignore, params);

    if (! ignore) {
      if (! value.isValid())
        bad = true;

      values.push_back(value);
    }
  }

  //---

  // set plot axis labels
  for (int ind = 0; ind < 4; ++ind) {
    CGnuPlotTypes::AxisType type =
      (ind == 0 || ind == 2 ? CGnuPlotTypes::AxisType::X : CGnuPlotTypes::AxisType::Y);

    int ind1 = ind/2 + 1;

    std::string str;

    if (! this->getTicLabel(type, ind1, str))
      continue;

    CExprValuePtr value;

    if (! evaluateExpression(str, value, true))
      continue;

    int         ns;
    long        icol;
    std::string str1;

    if (! value.isValid() || ! value->getIntegerValue(icol))
      continue;

    CExprValuePtr value1 = getFieldValue(fieldValues, icol, ns);

    if (! value1.isValid() || ! value1->getStringValue(str1))
      continue;

    if (type == CGnuPlotTypes::AxisType::X) {
      int x = plot->histogramPointOffset() + pointNum;

      plot->xaxis(ind1).setTicLabel(x, str1, 0);
    }
    else {
      int y = pointNum;

      plot->yaxis(ind1).setTicLabel(y, str1, 0);
    }
  }

  // set key label
  if (keyLabel_ != "") {
    CExprValuePtr value;

    if (evaluateExpression(keyLabel_, value, true)) {
      int         ns;
      long        icol;
      std::string str1;

      if (value.isValid() && value->getIntegerValue(icol)) {
        CExprValuePtr value1 = getFieldValue(fieldValues, icol, ns);

        if (value1.isValid() && value1->getStringValue(str1))
          plot->setKeyPointLabel(pointNum, str1);
      }
    }
  }

  return ns;
}

std::string
CGnuPlotUsingCols::
columnTitle(const std::vector<std::string> &columns) const
{
  for (uint i = 0; i < numCols(); ++i) {
    const CGnuPlotUsingCol &col = getCol(i);

    if (col.isStr())
      return col.str().substr(1, col.str().size() - 2);

    if (col.isInt()) {
      int icol = col.ival();

      if (icol > 0 && icol <= int(columns.size()))
        return columns[icol - 1];
      else
        return ""; // TODO: special values (0, -2) ?
    }
  }

  if      (columns.size() > 1)
    return columns[1];
  else if (columns.size() > 0)
    return columns[0];

  return "";
}

CExprValuePtr
CGnuPlotUsingCols::
decodeValue(const Values &fieldValues, const CGnuPlotUsingCol &col,
            int &ns, bool &ignore, Params &params) const
{
  col.updateColumnStr(plot_);

  CExprValuePtr value;

  ignore = false;

  if (col.isInt())
    value = getFieldValue(fieldValues, col.ival(), ns);
  else {
    std::string expr = col.str();

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

      CExprValuePtr value1 = getFieldValue(fieldValues, icol, ns1);

      if (value1.isValid()) {
        double      x1;
        std::string s1;

        if (value1->getRealValue(x1)) {
          std::string rstr = CStrUtil::toString(x1);

          auto p = rstr.find('.');

          if (p == std::string::npos)
            rstr += ".";

          expr = expr.substr(0, pos - 1) + rstr + expr.substr(pos1);
        }
        else if (value1->getStringValue(s1)) {
          expr = expr.substr(0, pos - 1) + "\"" + s1 + "\"" + expr.substr(pos1);
        }
        else
          expr = expr.substr(0, pos - 1) + "NaN" + expr.substr(pos1);
      }
      else
        expr = expr.substr(0, pos - 1) + "NaN" + expr.substr(pos1);

      pos = expr.find('$');
    }

    // check for parameter function
    pos = expr.find('(');

    if (pos > 0 && pos != std::string::npos) {
      std::string name  = expr.substr(0, pos);
      std::string name1 = expr.substr(pos + 1);

      uint j = 0;

      int brackets = 1;

      for ( ; j < name1.size(); ++j) {
        if      (name1[j] == '(')
          ++brackets;
        else if (name1[j] == ')') {
          --brackets;

          if (brackets == 0)
            break;
        }
      }

      if (j < name1.size())
        name1 = name1.substr(0, j);

      CExprValuePtr value;

      if (! evaluateExpression(name1, value, true))
        value = CExprValuePtr();

#if 0
      bool valid = false;

      long        icol;
      std::string str1;

      if (value.isValid() && value->getIntegerValue(icol)) {
        CExprValuePtr value1 = getFieldValue(fieldValues, icol, ns);

        if (value1.isValid() && value1->getStringValue(str1))
          valid = true;
      }

      if (valid)
        params[name] = str1;
#endif
      params[name] = value;

      ignore = true;
      expr   = "";
    }

    //----

    if (expr != "") {
      if (! evaluateExpression(expr, value, true))
        value = CExprValuePtr();
    }
  }

  return value;
}

CExprValuePtr
CGnuPlotUsingCols::
getFieldValue(const Values &fieldValues, int icol, int &ns) const
{
  CExprValuePtr value;

  int nf = fieldValues.size();

  if      (icol == 0)
    value = CExprInst->createRealValue(plot_->pointNum());
  else if (icol == -2)
    value = CExprInst->createRealValue(plot_->setNum());
  else if (icol > 0 && icol <= nf) {
    value = fieldValues[icol - 1];

    if (! value.isValid()) {
      ++ns;
      return CExprValuePtr();
    }
  }

  return value;
}

void
CGnuPlotUsingCols::
addCol(const std::string &str)
{
  CGnuPlotUsingCol col(str);

  cols_.push_back(col);

  if (col.isInt()) {
    colMin_.updateMin(col.ival());
    colMax_.updateMax(col.ival());
  }
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

    if (col.isInt())
      os << col.ival();
    else
      os << col.str();

    ++i;
  }

  os << std::endl;
}

//-----

CGnuPlotUsingCol::
CGnuPlotUsingCol(int i) :
 str_(""), isInt_(true), ival_(i)
{
}

CGnuPlotUsingCol::
CGnuPlotUsingCol(const std::string &str) :
 str_(str), isInt_(false), ival_(-1)
{
  CExprValuePtr value;

  bool isExpr = (str.size() && str[0] == '(' && str[str.size() - 1] == ')');

  if (! isExpr) {
    //isInt = CStrUtil::toInteger(str, &ival_);
    bool oldQuiet = CExprInst->getQuiet();

    CExprInst->setQuiet(true);

    long i;

    if (CExprInst->evaluateExpression(str, value) &&
        value.isValid() && value->getIntegerValue(i)) {
      ival_  = i;
      isInt_ = true;
    }
    else {
      CParseLine line(str);

      line.skipSpace();

      if (line.isChar('"'))
        isStr_ = true;
      else
        parseString(line);
    }

    CExprInst->setQuiet(oldQuiet);
  }
}

void
CGnuPlotUsingCol::
updateColumnStr(CGnuPlot *plot) const
{
  if (! isStr_) return;

  CGnuPlotUsingCol *th = const_cast<CGnuPlotUsingCol *>(this);

  const CGnuPlotKeyData &keyData = plot->keyData();

  const CGnuPlotKeyData::Columns &cols = keyData.columns();

  for (uint i = 0; i < cols.size(); ++i) {
    std::string str = "\"" + cols[i] + "\"";

    if (str == th->str_) {
      th->isInt_ = true;
      th->ival_  = i + 1;
      th->isStr_ = false;
      return;
    }
  }
}

bool
CGnuPlotUsingCol::
parseString(CParseLine &line)
{
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

  return ticLabel_.setTicLabel(identifier, value);
}
