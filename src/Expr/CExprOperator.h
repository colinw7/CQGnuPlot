#ifndef CExprOperator_H
#define CExprOperator_H

#include <map>

class CExprOperatorMgr {
 public:
 ~CExprOperatorMgr() { }

  CExprOperatorPtr getOperator(CExprOpType type) const;

  bool isOperatorChar(char c) const;

 private:
  friend class CExpr;

  CExprOperatorMgr();

 private:
  typedef std::map<CExprOpType,CExprOperatorPtr> OperatorMap;

  OperatorMap operator_map_;
};

class CExprOperator {
 public:
  CExprOperator(CExprOpType type, const std::string &name);

  CExprOpType getType() const { return type_; }

  friend std::ostream &operator<<(std::ostream &os, const CExprOperator &op) {
    os << op.name_;

    return os;
  }

 private:
  CExprOpType type_;
  std::string name_;
};

#endif
