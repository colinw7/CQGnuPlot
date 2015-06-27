#ifndef CExprOperator_H
#define CExprOperator_H

#include <map>

class CExprOperator;

typedef CRefPtr<CExprOperator> CExprOperatorPtr;

class CExprOperatorMgr {
 public:
 ~CExprOperatorMgr() { }

  CExprOperatorPtr getOperator(CExprOpType type) const;

 private:
  friend class CExpr;

  CExprOperatorMgr();

 private:
  typedef std::map<CExprOpType,CExprOperatorPtr> OperatorMap;

  OperatorMap operator_map_;
};

//---

class CExprOperator {
 public:
  static bool isOperatorChar(char c);
  static bool isOperatorStr (char c);

  CExprOperator(CExprOpType type, const std::string &name);

  CExprOpType getType() const { return type_; }
  std::string getName() const { return name_; }

  void print(std::ostream &os) const {
    os << name_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprOperator &op) {
    op.print(os);

    return os;
  }

 private:
  CExprOpType type_;
  std::string name_;
};

#endif
