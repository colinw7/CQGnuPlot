#ifndef CGnuPlotUsingCols_H
#define CGnuPlotUsingCols_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <CExpr.h>
#include <CGnuPlotTicLabel.h>

class CGnuPlot;
class CParseLine;

//---

class CGnuPlotUsingCol {
 public:
  CGnuPlotUsingCol(int i);

  CGnuPlotUsingCol(CExpr *expr, const std::string &str1);

  const std::string &str() const { return str_; }

  bool isInt() const { return isInt_; }
  bool isStr() const { return isStr_; }

  int ival() const { return ival_; }

  void updateColumnStr(CGnuPlot *plot) const;

 private:
  bool parseString(CParseLine &line);

 private:
  std::string str_;
  bool        isInt_  { true };
  bool        isStr_  { false };
  int         ival_   { 0 };

  CGnuPlotTicLabel ticLabel_;
};

//---

struct CGnuPlotUsingColData {
  void reset() {
    usingStr   = "";
    isExpr     = false;
    isFunc     = false;
    identifier = "";
    value      = "";
  }

  std::string             usingStr;
  bool                    isExpr = false;
  bool                    isFunc = false;
  std::string             identifier;
  std::string             value;
  CGnuPlotTypes::AxisType ticId = CGnuPlotTypes::AxisType::X;
  int                     ticInd = 0;
};

//---

class CGnuPlotUsingCols {
 public:
  typedef std::map<std::string,CExprValuePtr>        Params;
  typedef std::vector<CExprValuePtr>                 Values;
  typedef std::map<int,std::string>                  TicLabel;
  typedef std::map<CGnuPlotTypes::AxisType,TicLabel> AxisTicLabel;
  typedef std::vector<std::string>                   StringArray;

 public:
  CGnuPlotUsingCols(CGnuPlot *plot=0);
 ~CGnuPlotUsingCols();

  const std::string &str() const { return str_; }
  void setStr(const std::string &s) { str_ = s; }

  void parse(const std::string &str);

  uint numCols() const { return cols_.size(); }

  void addCol(const std::string &str);

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
    if (! usingCol.isInt()) return false;

    return (usingCol.ival() >= low && usingCol.ival() <= high);
  }

  const COptInt &colMin() const { return colMin_; }
  void setColMin(const COptInt &v) { colMin_ = v; }

  const COptInt &colMax() const { return colMax_; }
  void setColMax(const COptInt &v) { colMax_ = v; }

  const std::string &format() const { return format_; }
  void setFormat(const std::string &format) { format_ = format; }

  std::string columnTitle(const std::vector<std::string> &columns) const;

  int decodeValues(CGnuPlot *plot, int setNum, int pointNum, const Values &fieldValues,
                   bool &bad, Values &values, Params &params) const;

  CGnuPlot *plot() const { return plot_; }

  int setNum() const { return setNum_; }

  int pointNum() const { return pointNum_; }

  CExprValuePtr getFieldValue(int ival, int &ns) const;

  int getColumnIndex(const std::string &str) const;

  bool isParsePlotTitle() const;

  void setKeyColumnHeadNum(int icol);

  std::string toString() const;

  void print(std::ostream &os) const;

 private:
  CExprValuePtr decodeValue(const CGnuPlotUsingCol &col, int &ns,
                            bool &ignore, Params &params) const;

  void processUsingStr(CGnuPlotUsingColData &usingData, StringArray &usingStrs);

 private:
  typedef std::vector<CGnuPlotUsingCol> Cols;

  std::string       str_;
  Cols              cols_;
  COptInt           colMin_, colMax_;
  AxisTicLabel      axisTicLabel_;
  std::string       format_;
  std::string       keyLabel_;
  mutable CGnuPlot *plot_     { 0 };
  CExpr*            expr_ { 0 };
  mutable int       setNum_   { 0 };
  mutable int       pointNum_ { 0 };
  mutable Values    fieldValues_;
};

#endif
