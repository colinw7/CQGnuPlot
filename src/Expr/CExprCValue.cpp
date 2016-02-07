#include <CExprI.h>

bool
CExprComplexValue::
getStringValue(std::string &s) const
{
  s = "{" + CStrUtil::toString(c_.real()) + ", " + CStrUtil::toString(c_.imag()) + "}";

  return true;
}

CExprValuePtr
CExprComplexValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CExprOpType::UNARY_PLUS:
      return CExprInst->createComplexValue(c_);
    case CExprOpType::UNARY_MINUS:
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
    case CExprOpType::PLUS:
      return CExprInst->createComplexValue(c_ + rc);
    case CExprOpType::MINUS:
      return CExprInst->createComplexValue(c_ - rc);
    case CExprOpType::TIMES:
      return CExprInst->createComplexValue(c_ * rc);
    case CExprOpType::DIVIDE:
      return CExprInst->createComplexValue(c_ / rc);
    default:
      return CExprValuePtr();
  }
}
