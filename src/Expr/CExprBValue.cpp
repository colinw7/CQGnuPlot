#include <CExprI.h>

CExprValuePtr
CExprBooleanValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CExprOpType::LOGICAL_NOT:
      return CExprInst->createBooleanValue(! boolean_);
    default:
      return CExprValuePtr();
  }
}

CExprValuePtr
CExprBooleanValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  bool rboolean = false;

  if (! rhs->getBooleanValue(rboolean))
    return CExprValuePtr();

  switch (op) {
    case CExprOpType::LOGICAL_AND:
      return CExprInst->createBooleanValue(boolean_ && rboolean);
    case CExprOpType::LOGICAL_OR:
      return CExprInst->createBooleanValue(boolean_ || rboolean);
    default:
      return CExprValuePtr();
  }
}
