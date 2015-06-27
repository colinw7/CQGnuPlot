#ifndef CExprCompile_H
#define CExprCompile_H

class CExprCompileImpl;

class CExprCompile {
 public:
 ~CExprCompile();

  CExprTokenStack compileIToken(CExprITokenPtr itoken);

 private:
  friend class CExpr;

  CExprCompile();

 private:
  CAutoPtr<CExprCompileImpl> impl_;
};

#endif
