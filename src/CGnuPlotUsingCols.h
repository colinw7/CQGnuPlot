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

  CGnuPlotTicLabel ticLabel;

  bool parseString();
};

//---

class CGnuPlotUsingCols {
 public:
  typedef std::vector<std::string>                   StringArray;
  typedef std::map<std::string,std::string>          Params;
  typedef std::vector<CExprValueP>                   Values;
  typedef std::map<int,std::string>                  TicLabel;
  typedef std::map<CGnuPlotTypes::AxisType,TicLabel> AxisTicLabel;

 public:
  CGnuPlotUsingCols();

  void init(const std::string &str);

  uint numCols() const { return cols_.size(); }

  void addCol(const std::string &str) {
    cols_.push_back(CGnuPlotUsingCol(str));
  }

  void setTicLabel(CGnuPlotTypes::AxisType type, int ind, const std::string &value) {
    axisTicLabel_[type][ind] = value;
  }

  bool getTicLabel(CGnuPlotTypes::AxisType type, int ind, std::string &value) const {
    auto p = axisTicLabel_.find(type);
    if (p == axisTicLabel_.end()) return false;

    const TicLabel &ticLabel = (*p).second;

    auto p1 = ticLabel.find(ind);
    if (p1 == ticLabel.end()) return false;

    value = (*p1).second;

    return true;
  }

  const CGnuPlotUsingCol &getCol(int i) const { return cols_[i]; }

  bool isColIntInRange(int col, int low, int high) const {
    const CGnuPlotUsingCol &usingCol = getCol(col);
    if (! usingCol.isInt) return false;

    return (usingCol.ival >= low && usingCol.ival <= high);
  }

  const std::string &format() const { return format_; }
  void setFormat(const std::string &format) { format_ = format; }

  int decodeValues(CGnuPlot *plot, int pointNum, const Values &fieldValues, bool &bad,
                   Values &values, Params &params) const;

  CExprValueP decodeValue(const Values &fieldValues, const CGnuPlotUsingCol &col,
                          int &ns, bool &ignore, Params &params) const;

  CExprValueP getFieldValue(const Values &fieldValues, int ival, int &ns) const;

  std::string toString() const;

  void print(std::ostream &os) const;

 private:
  typedef std::vector<CGnuPlotUsingCol> Cols;

  Cols              cols_;
  AxisTicLabel      axisTicLabel_;
  std::string       format_;
  mutable CGnuPlot *plot_;
};

#endif
