#ifndef CExprOperatorMgr_H
#define CExprOperatorMgr_H

#include <map>

class CExprOperatorMgr {
 public:
  CExprOperatorMgr(CExpr *expr);
 ~CExprOperatorMgr() { }

  CExprOperatorPtr getOperator(CExprOpType type) const;

 private:
  typedef std::map<CExprOpType,CExprOperatorPtr> OperatorMap;

  CExpr*      expr_ { nullptr };
  OperatorMap operator_map_;
};

#endif
