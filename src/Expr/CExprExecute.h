#ifndef CExprExecute_H
#define CExprExecute_H

class CExprExecuteImpl;

class CExprExecute {
 public:
 ~CExprExecute();

  bool executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values);
  bool executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value);

 private:
  friend class CExpr;

  CExprExecute();

 private:
  CAutoPtr<CExprExecuteImpl> impl_;
};

#endif
