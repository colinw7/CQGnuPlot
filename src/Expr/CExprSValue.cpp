#include <CExprI.h>

CExprValuePtr
CExprStringValue::
execUnaryOp(CExprOpType) const
{
  return CExprValuePtr();
}

CExprValuePtr
CExprStringValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  std::string rstr;

  if (! rhs->getStringValue(rstr))
    return CExprValuePtr();

  std::string result;

  switch (op) {
    case CEXPR_OP_PLUS:
      return CExprValue::createStringValue(str_ + rstr);
    default:
      return CExprValuePtr();
  }
}
