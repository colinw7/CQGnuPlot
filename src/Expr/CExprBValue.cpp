#include <CExprI.h>

CExprValuePtr
CExprBooleanValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CEXPR_OP_LOGICAL_NOT:
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
    case CEXPR_OP_LOGICAL_AND:
      return CExprInst->createBooleanValue(boolean_ && rboolean);
    case CEXPR_OP_LOGICAL_OR:
      return CExprInst->createBooleanValue(boolean_ || rboolean);
    default:
      return CExprValuePtr();
  }
}
