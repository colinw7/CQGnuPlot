#ifndef CGnuPlotWhereData_H
#define CGnuPlotWhereData_H

#include <CExpr.h>
#include <string>

class CGnuPlotWhereData {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotWhereData(const std::string &expr="");

  bool parse(const std::string &expr);

  bool isValid(int setNum, int pointNum, const Values &values) const;

  CExprValuePtr getFieldValue(int setNum, int pointNum,
                              const Values &fieldValues, int icol) const;

 private:
  std::string expr_;
};

#endif
