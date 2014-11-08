#ifndef CExprVariable_H
#define CExprVariable_H

class CExprVariableMgr {
 public:
 ~CExprVariableMgr();

  CExprVariablePtr createVariable(const std::string &name, CExprValuePtr value);

  CExprVariablePtr getVariable(const std::string &name) const;

 private:
  friend class CExpr;

  CExprVariableMgr();

  void addVariable   (CExprVariablePtr variable);
  void removeVariable(CExprVariablePtr variable);

 private:
  typedef std::list<CExprVariablePtr> VariableList;

  VariableList variables_;
};

class CExprVariable {
 public:
 ~CExprVariable();

  void setValue(CExprValuePtr value);
  void setRealValue(double x);

  CExprValuePtr getValue() const;

  CExprValueType getValueType() const;

 private:
  CExprVariable(const std::string &name, CExprValuePtr value);

 private:
  friend class CExprVariableMgr;

  std::string   name_;
  CExprValuePtr value_;
};

#endif
