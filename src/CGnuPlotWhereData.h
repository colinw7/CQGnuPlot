#ifndef CGnuPlotWhereData_H
#define CGnuPlotWhereData_H

#include <CExpr.h>
#include <string>

class CGnuPlotWhereData {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotWhereData(const std::string &exprStr="");

  bool parse(const std::string &exprStr);

  bool isValid(CExpr *expr, int setNum, int pointNum, const Values &values) const;

  CExprValuePtr getFieldValue(CExpr *expr, int setNum, int pointNum,
                              const Values &fieldValues, int icol) const;

 private:
  std::string exprStr_;
};

#endif
