#include <CExprI.h>
#include <CMathGen.h>
#include <cerrno>

CExprValuePtr
CExprRealValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CEXPR_OP_UNARY_PLUS:
      return CExprInst->createRealValue(real_);
    case CEXPR_OP_UNARY_MINUS:
      return CExprInst->createRealValue(-real_);
    default:
      return CExprValuePtr();
  }
}

CExprValuePtr
CExprRealValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  double rrhs;

  if (! rhs->getRealValue(rrhs))
    return CExprValuePtr();

  switch (op) {
    case CEXPR_OP_POWER: {
      int error_code;

      double real = realPower(real_, rrhs, &error_code);

      if (error_code != 0) {
#ifdef GNUPLOT_EXPR
        real = CMathGen::getNaN();
#else
        return CExprValuePtr();
#endif
      }

      return CExprInst->createRealValue(real);
    }
    case CEXPR_OP_TIMES:
      return CExprInst->createRealValue(real_ * rrhs);
    case CEXPR_OP_DIVIDE:
      return CExprInst->createRealValue(real_ / rrhs);
    case CEXPR_OP_MODULUS: {
      int error_code;

      double real = realModulus(real_, rrhs, &error_code);

      if (error_code != 0) {
#ifdef GNUPLOT_EXPR
        real = CMathGen::getNaN();
#else
        return CExprValuePtr();
#endif
      }

      return CExprInst->createRealValue(real);
    }
    case CEXPR_OP_PLUS:
      return CExprInst->createRealValue(real_ + rrhs);
    case CEXPR_OP_MINUS:
      return CExprInst->createRealValue(real_ - rrhs);
    case CEXPR_OP_LESS:
      return CExprInst->createBooleanValue(real_ < rrhs);
    case CEXPR_OP_LESS_EQUAL:
      return CExprInst->createBooleanValue(real_ <= rrhs);
    case CEXPR_OP_GREATER:
      return CExprInst->createBooleanValue(real_ > rrhs);
    case CEXPR_OP_GREATER_EQUAL:
      return CExprInst->createBooleanValue(real_ >= rrhs);
    case CEXPR_OP_EQUAL:
      return CExprInst->createBooleanValue(real_ == rrhs);
    case CEXPR_OP_NOT_EQUAL:
      return CExprInst->createBooleanValue(real_ != rrhs);
    default:
      return CExprValuePtr();
  }
}

double
CExprRealValue::
realPower(double real1, double real2, int *error_code) const
{
  *error_code = 0;

  if (IsNaN(real1) || IsNaN(real2)) {
    *error_code = CEXPR_ERROR_NAN_OPERATION;
    return CMathGen::getNaN();
  }

  bool is_int = ((((int) real2) == real2));

  if (real1 < 0.0 && ! is_int) {
    *error_code = CEXPR_ERROR_NON_INTEGER_POWER_OF_NEGATIVE;
    return CMathGen::getNaN();
  }

  if (real1 == 0.0 && real2 < 0.0) {
    *error_code = CEXPR_ERROR_ZERO_TO_NEG_POWER_UNDEF;
    return CMathGen::getNaN();
  }

  errno = 0;

  double real;

  if (real2 < 0.0)
    real = 1.0/pow(real1, -real2);
  else
    real = pow(real1, real2);

  if (errno != 0) {
    *error_code = CEXPR_ERROR_POWER_FAILED;
    return CMathGen::getNaN();
  }

  return real;
}

double
CExprRealValue::
realModulus(double real1, double real2, int *error_code) const
{
  *error_code = 0;

  if (IsNaN(real1) || IsNaN(real2)) {
    *error_code = CEXPR_ERROR_NAN_OPERATION;
    return CMathGen::getNaN();
  }

  if (real2 == 0.0) {
    *error_code = CEXPR_ERROR_DIVIDE_BY_ZERO;
    return CMathGen::getNaN();
  }

  int factor = int(real1/real2);

  double result = real1 - (real2*factor);

  return result;
}
