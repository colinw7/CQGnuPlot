#ifndef CGnuPlotUsingCols_H
#define CGnuPlotUsingCols_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <CExpr.h>
#include <CGnuPlotTicLabel.h>

typedef CRefPtr<CExprValue> CExprValueP;

class CGnuPlot;

//---

struct CGnuPlotUsingCol {
  CGnuPlotUsingCol(int i);

  CGnuPlotUsingCol(const std::string &str1);

  std::string str;
  bool        isInt { true };
  int         ival  { 0 };
};

//---

class CGnuPlotUsingCols {
 public:
  typedef std::vector<std::string>          StringArray;
  typedef std::map<std::string,std::string> Params;
  typedef std::vector<CExprValueP>          Values;

 public:
  CGnuPlotUsingCols();

  void init(const std::string &str);

  uint numCols() const { return cols_.size(); }

  void addCol(const std::string &str) {
    cols_.push_back(CGnuPlotUsingCol(str));
  }

  const CGnuPlotUsingCol &getCol(int i) const { return cols_[i]; }

  int decodeValues(CGnuPlot *plot, const Values &fieldValues, bool &bad,
                   Values &values, Params &params) const;

  CExprValueP decodeValue(const Values &fieldValues, const CGnuPlotUsingCol &col,
                          int &ns, bool &ignore, Params &params) const;

  CExprValueP getFieldValue(const Values &fieldValues, int ival, int &ns) const;

  std::string toString() const;

  void print(std::ostream &os) const;

 private:
  typedef std::vector<CGnuPlotUsingCol> Cols;

  Cols              cols_;
  mutable CGnuPlot *plot_;
};

#endif
