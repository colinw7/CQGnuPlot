#include <CGnuPlotUsingCols.h>
#include <CGnuPlot.h>
#include <CParseLine.h>
#include <CStrUtil.h>

class CGnuPlotUsingColsFnObj : public CExprFunctionObj {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotUsingColsFnObj(CGnuPlotUsingCols *cols) : cols_(cols) { }

 protected:
  CGnuPlotUsingCols *cols_;
};

class CGnuPlotStringColumnFn : public CGnuPlotUsingColsFnObj {
 public:
  CGnuPlotStringColumnFn(CGnuPlotUsingCols *cols) : CGnuPlotUsingColsFnObj(cols) { }

  CExprValuePtr operator()(CExpr *expr, const Values &values) {
    assert(values.size() == 1);

    std::string str;

    long col = 0;

    if (values[0]->getIntegerValue(col)) {
      int ns;

      CExprValuePtr value = cols_->getFieldValue(col, ns);

      if (value.isValid() && value->isStringValue())
        (void) value->getStringValue(str);
    }

    return expr->createStringValue(str);
  }
};

class CGnuPlotColumnFn : public CGnuPlotUsingColsFnObj {
 public:
  CGnuPlotColumnFn(CGnuPlotUsingCols *cols) : CGnuPlotUsingColsFnObj(cols) { }

  CExprValuePtr operator()(CExpr *expr, const Values &values) {
    long icol = 0;

    if      (values[0]->isIntegerValue()) {
      (void) values[0]->getIntegerValue(icol);
    }
    else if (values[0]->isStringValue()) {
      std::string str;

      (void) values[0]->getStringValue(str);

      icol = cols_->getColumnIndex(str);
    }
    else
      return expr->createRealValue(0.0);

    //---

    CExprValuePtr value;

    if      (icol == 0)
      value = expr->createRealValue(cols_->pointNum());
    else if (icol == -2)
      value = expr->createRealValue(cols_->setNum());
    else {
      if (cols_->isParsePlotTitle()) {
        cols_->setKeyColumnHeadNum(icol);

        value = expr->createStringValue("");
      }
      else {
        int ns;

        value = cols_->getFieldValue(icol, ns);

        if (! value.isValid())
          value = expr->createStringValue("column(" + std::to_string(icol) + ")");
      }
    }

    return value;
  }
};

class CGnuPlotStringValidFn : public CGnuPlotUsingColsFnObj {
 public:
  CGnuPlotStringValidFn(CGnuPlotUsingCols *cols) : CGnuPlotUsingColsFnObj(cols) { }

  CExprValuePtr operator()(CExpr *expr, const Values &values) {
    assert(values.size() == 1);

    std::string str;

    bool valid = false;

    long icol = 0;

    if (values[0]->getIntegerValue(icol)) {
      int ns;

      CExprValuePtr value = cols_->getFieldValue(icol, ns);

      valid = value.isValid();
    }

    return expr->createIntegerValue(valid ? 0 : 1);
  }
};

class CGnuPlotTimeColumnFn : public CGnuPlotUsingColsFnObj {
 public:
  CGnuPlotTimeColumnFn(CGnuPlotUsingCols *cols) : CGnuPlotUsingColsFnObj(cols) { }

  CExprValuePtr operator()(CExpr *expr, const Values &values) {
    std::string fmt;
    long        col = 0;
    bool        hasCol = false;

    if      (values.size() == 2) {
      if (values[0]->getIntegerValue(col) && values[1]->getStringValue(fmt))
        hasCol = true;
    }
    else if (values.size() == 1) {
      CGnuPlot *plot = cols_->plot();

      if (plot)
        fmt = plot->timeFmt().getValue("%d/%m/%y,%H:%M");
      else
        fmt = "%d/%m/%y,%H:%M";

      if (values[0]->getIntegerValue(col))
        hasCol = true;
    }
    else {
      assert(false);
    }

    //---

    time_t t;

    if (hasCol) {
      int ns;

      CExprValuePtr value = cols_->getFieldValue(col, ns);

      if (value->isStringValue()) {
        std::string str;

        (void) value->getStringValue(str);

        struct tm tm1;

        memset(&tm1, 0, sizeof(tm));

        /*char *p =*/ strptime(str.c_str(), fmt.c_str(), &tm1);

        t = mktime(&tm1);
      }
    }

    return expr->createIntegerValue(t);
  }

  bool isOverload() const override { return true; }
};

//---

CGnuPlotUsingCols::
CGnuPlotUsingCols(CGnuPlot *plot) :
 str_(), plot_(plot), expr_(0)
{
  if (! plot_)
    expr_ = new CExpr;
}

CGnuPlotUsingCols::
~CGnuPlotUsingCols()
{
  if (plot_)
    delete expr_;
}

void
CGnuPlotUsingCols::
parse(const std::string &str)
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

namespace {

template<typename FUNC>
void addFunction(CGnuPlotUsingCols *cols, CExpr *expr,
                 const std::string &name, const std::string &args) {
  expr->addFunction(name, args, new FUNC(cols))->setBuiltin(true);
}

}

int
CGnuPlotUsingCols::
decodeValues(CGnuPlot *plot, int setNum, int pointNum, const Values &fieldValues,
             bool &bad, Values &values, Params &params) const
{
  plot_        = plot;
  setNum_      = setNum;
  pointNum_    = pointNum;
  fieldValues_ = fieldValues;

  //---

  CGnuPlotUsingCols *th = const_cast<CGnuPlotUsingCols *>(this);

  addFunction<CGnuPlotStringColumnFn>(th, expr_, "stringcolumn", "i"  );
  addFunction<CGnuPlotStringColumnFn>(th, expr_, "strcol"      , "i"  );
  addFunction<CGnuPlotColumnFn      >(th, expr_, "column"      , "is" );
  addFunction<CGnuPlotStringValidFn >(th, expr_, "valid"       , "i"  );
  addFunction<CGnuPlotTimeColumnFn  >(th, expr_, "timecolumn"  , "i"  );
  addFunction<CGnuPlotTimeColumnFn  >(th, expr_, "timecolumn"  , "i,s");

  //---

  bad = false;

  int ns = 0;

  for (uint i = 0; i < numCols(); ++i) {
    bool ignore = false;

    const CGnuPlotUsingCol &col = getCol(i);

    CExprValuePtr value = decodeValue(col, ns, ignore, params);
    if (ignore) continue;

    if (! value.isValid() || value->isMissing())
      bad = true;

    values.push_back(value);
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

    if (! CGnuPlotUtil::evaluateExpression(expr_, str, value, true))
      continue;

    long icol;

    if (! value.isValid() || ! value->getIntegerValue(icol))
      continue;

    int ns1;

    CExprValuePtr value1 = getFieldValue(icol, ns1);

    std::string str1;

    if (! value1.isValid() || ! value1->getStringValue(str1))
      continue;

    if (type == CGnuPlotTypes::AxisType::X) {
      int x = (plot_ ? plot_->histogramPointOffset() : 0) + this->pointNum();

      if (plot_)
        plot_->xaxis(ind1).setTicLabel(x, str1, 0);
    }
    else {
      int y = this->pointNum();

      if (plot_)
        plot_->yaxis(ind1).setTicLabel(y, str1, 0);
    }
  }

  // set key label
  if (keyLabel_ != "") {
    CExprValuePtr value;

    if (CGnuPlotUtil::evaluateExpression(expr_, keyLabel_, value, true)) {
      long icol;

      if (value.isValid() && value->getIntegerValue(icol)) {
        int ns1;

        CExprValuePtr value1 = getFieldValue(icol, ns1);

        std::string str1;

        if (value1.isValid() && value1->getStringValue(str1)) {
          if (plot_)
            plot_->setKeyPointLabel(this->pointNum(), str1);
        }
      }
    }
  }

  expr_->removeFunction("stringcolumn");
  expr_->removeFunction("strcol");
  expr_->removeFunction("column");
  expr_->removeFunction("valid");
  expr_->removeFunction("timecolumn");

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
decodeValue(const CGnuPlotUsingCol &col, int &ns, bool &ignore, Params &params) const
{
  if (plot_)
    col.updateColumnStr(plot_);

  CExprValuePtr value;

  ignore = false;

  if (col.isInt())
    value = getFieldValue(col.ival(), ns);
  else {
    std::string exprStr = col.str();

    // replace $N variables
    // TODO: easier to define $1 variables
    auto pos = exprStr.find('$');

    while (pos != std::string::npos) {
      int pos1 = ++pos;

      while (isdigit(exprStr[pos1]))
        ++pos1;

      std::string numStr = exprStr.substr(pos, pos1 - pos);

      int icol = 0;

      (void) CStrUtil::toInteger(numStr, &icol);

      int ns1 = 0;

      CExprValuePtr value1 = getFieldValue(icol, ns1);

      std::string lstr = exprStr.substr(0, pos - 1);
      std::string rstr = exprStr.substr(pos1);

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

      exprStr = lstr + midStr + rstr;

      pos = exprStr.find('$');
    }

    // check for parameter function
    pos = exprStr.find('(');

    if (pos > 0 && pos != std::string::npos) {
      std::string name  = exprStr.substr(0, pos);
      std::string name1 = exprStr.substr(pos + 1);

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

      if (! CGnuPlotUtil::evaluateExpression(expr_, name1, value, true))
        value = CExprValuePtr();

#if 0
      bool valid = false;

      long        icol;
      std::string str1;

      if (value.isValid() && value->getIntegerValue(icol)) {
        CExprValuePtr value1 = getFieldValue(icol, ns);

        if (value1.isValid() && value1->getStringValue(str1))
          valid = true;
      }

      if (valid)
        params[name] = str1;
#endif
      params[name] = value;

      ignore  = true; // ignore parameter values
      exprStr = "";
    }

    //----

    if (exprStr != "") {
      if (! CGnuPlotUtil::evaluateExpression(expr_, exprStr, value, true))
        value = CExprValuePtr();
    }
  }

  return value;
}

CExprValuePtr
CGnuPlotUsingCols::
getFieldValue(int icol, int &ns) const
{
  CExprValuePtr value;

  int nf = fieldValues_.size();

  if      (icol == 0) {
    int pointNum = this->pointNum();

    value = expr_->createRealValue(pointNum);
  }
  else if (icol == -2) {
    int setNum = this->setNum();

    value = expr_->createRealValue(setNum);
  }
  else if (icol > 0 && icol <= nf) {
    value = fieldValues_[icol - 1];

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
  CGnuPlotUsingCol col(expr_, str);

  cols_.push_back(col);

  if (col.isInt()) {
    colMin_.updateMin(col.ival());
    colMax_.updateMax(col.ival());
  }
}

int
CGnuPlotUsingCols::
getColumnIndex(const std::string &str) const
{
  if (plot_)
    return plot_->getColumnIndex(str);

  std::cerr << "No plot for CGnuPlotUsingCol::getColumnIndex" << std::endl;

  return -1;
}

bool
CGnuPlotUsingCols::
isParsePlotTitle() const
{
  if (plot_)
    return plot_->isParsePlotTitle();

  std::cerr << "No plot for CGnuPlotUsingCol::isParsePlotTitle" << std::endl;

  return false;
}

void
CGnuPlotUsingCols::
setKeyColumnHeadNum(int icol)
{
  if (plot_)
    plot_->setKeyColumnHeadNum(icol);
  else
    std::cerr << "No plot for CGnuPlotUsingCol::setKeyColumnHeadNum" << std::endl;
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
CGnuPlotUsingCol(CExpr *expr, const std::string &str) :
 str_(str), isInt_(false), ival_(-1)
{
  CExprValuePtr value;

  bool isExpr = (str.size() && str[0] == '(' && str[str.size() - 1] == ')');

  if (! isExpr) {
    //isInt = CStrUtil::toInteger(str, &ival_);
    bool oldQuiet = expr->getQuiet();

    expr->setQuiet(true);

    long i;

    if (expr->evaluateExpression(str, value) &&
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

    expr->setQuiet(oldQuiet);
  }
}

void
CGnuPlotUsingCol::
updateColumnStr(CGnuPlot *plot) const
{
  assert(plot);

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
