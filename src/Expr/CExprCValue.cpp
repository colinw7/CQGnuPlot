#include <CExprI.h>

CExprValuePtr
CExprComplexValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CEXPR_OP_UNARY_PLUS:
      return CExprInst->createComplexValue(c_);
    case CEXPR_OP_UNARY_MINUS:
      return CExprInst->createComplexValue(-c_);
    default:
      return CExprValuePtr();
  }
}

CExprValuePtr
CExprComplexValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  std::complex<double> rc;

  if (! rhs->getComplexValue(rc))
    return CExprValuePtr();

  //---

  std::string result;

  switch (op) {
    case CEXPR_OP_PLUS:
      return CExprInst->createComplexValue(c_ + rc);
    case CEXPR_OP_MINUS:
      return CExprInst->createComplexValue(c_ - rc);
    case CEXPR_OP_TIMES:
      return CExprInst->createComplexValue(c_ * rc);
    case CEXPR_OP_DIVIDE:
      return CExprInst->createComplexValue(c_ / rc);
    default:
      return CExprValuePtr();
  }
}
