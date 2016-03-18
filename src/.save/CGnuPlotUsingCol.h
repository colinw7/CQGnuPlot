#ifndef CGnuPlotUsingCol_H
#define CGnuPlotUsingCol_H

struct CGnuPlotUsingCol {
  CGnuPlotUsingCol(int i) :
   str(""), isInt(true), ival(i) {
  }

  CGnuPlotUsingCol(const std::string &str1) :
   str(str1), isInt(false), ival(-1) {
    isInt = CStrUtil::toInteger(str1, &ival);
  }

  std::string str;
  bool        isInt { true };
  int         ival  { 0 };
};

typedef std::vector<CGnuPlotUsingCol> CGnuPlotUsingCols;

#endif
