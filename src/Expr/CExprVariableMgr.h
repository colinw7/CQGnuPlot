#ifndef CExprVariableMgr_H
#define CExprVariableMgr_H

#include <list>

class CExprVariableMgr {
 public:
 ~CExprVariableMgr();

  CExprVariablePtr createVariable(const std::string &name, CExprValuePtr value);

  CExprVariablePtr getVariable(const std::string &name) const;

  void getVariableNames(std::vector<std::string> &names) const;

 private:
  friend class CExpr;

  CExprVariableMgr();

  void addVariable   (CExprVariablePtr variable);
  void removeVariable(CExprVariablePtr variable);

 private:
  typedef std::list<CExprVariablePtr> VariableList;

  VariableList variables_;
};

#endif
