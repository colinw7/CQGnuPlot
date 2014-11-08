#include <CExprI.h>
#include <cerrno>
#include <cmath>

CExprValuePtr
CExprIntegerValue::
execUnaryOp(CExprOpType op) const
{
  switch (op) {
    case CEXPR_OP_UNARY_PLUS:
      return CExprValue::createIntegerValue(integer_);
    case CEXPR_OP_UNARY_MINUS:
      return CExprValue::createIntegerValue(-integer_);
    case CEXPR_OP_BIT_NOT:
      return CExprValue::createIntegerValue(~integer_);
    default:
      return CExprValuePtr();
  }
}

CExprValuePtr
CExprIntegerValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  long irhs;

  if (! rhs->getIntegerValue(irhs))
    return CExprValuePtr();

  switch (op) {
    case CEXPR_OP_POWER: {
      int error_code;

      long integer = integerPower(integer_, irhs, &error_code);

      if (error_code != 0)
        return CExprValuePtr();
      else
        return CExprValue::createIntegerValue(integer);
    }
    case CEXPR_OP_TIMES:
      return CExprValue::createIntegerValue(integer_ * irhs);
    case CEXPR_OP_DIVIDE:
      return CExprValue::createIntegerValue(integer_ / irhs);
    case CEXPR_OP_MODULUS:
      return CExprValue::createIntegerValue(integer_ % irhs);
    case CEXPR_OP_PLUS:
      return CExprValue::createIntegerValue(integer_ + irhs);
    case CEXPR_OP_MINUS:
      return CExprValue::createIntegerValue(integer_ - irhs);
    case CEXPR_OP_BIT_LSHIFT:
      return CExprValue::createIntegerValue(integer_ << irhs);
    case CEXPR_OP_BIT_RSHIFT:
      return CExprValue::createIntegerValue(integer_ >> irhs);
    case CEXPR_OP_LESS:
      return CExprValue::createBooleanValue(integer_ < irhs);
    case CEXPR_OP_LESS_EQUAL:
      return CExprValue::createBooleanValue(integer_ <= irhs);
    case CEXPR_OP_GREATER:
      return CExprValue::createBooleanValue(integer_ > irhs);
    case CEXPR_OP_GREATER_EQUAL:
      return CExprValue::createBooleanValue(integer_ >= irhs);
    case CEXPR_OP_EQUAL:
      return CExprValue::createBooleanValue(integer_ == irhs);
    case CEXPR_OP_NOT_EQUAL:
      return CExprValue::createBooleanValue(integer_ != irhs);
    case CEXPR_OP_BIT_AND:
      return CExprValue::createIntegerValue(integer_ & irhs);
    case CEXPR_OP_BIT_XOR:
      return CExprValue::createIntegerValue(integer_ ^ irhs);
    case CEXPR_OP_BIT_OR:
      return CExprValue::createIntegerValue(integer_ | irhs);
    default:
      return CExprValuePtr();
  }
}

long
CExprIntegerValue::
integerPower(long integer1, long integer2, int *error_code) const
{
  *error_code = 0;

  if (integer1 == 0 && integer2 < 0) {
    *error_code = CEXPR_ERROR_ZERO_TO_NEG_POWER_UNDEF;
    return 0;
  }

  double real;

  errno = 0;

  if (integer2 < 0.0)
    real = 1.0/pow((double) integer1, (double) -integer2);
  else
    real = pow((double) integer1, (double) integer2);

  if (errno != 0) {
    *error_code = CEXPR_ERROR_POWER_FAILED;
    return 0;
  }

  long integer = realToInteger(real, error_code);

  if (*error_code != 0)
    return 0;

  return integer;
}

long
CExprIntegerValue::
realToInteger(double real, int *error_code) const
{
  long integer = (long) real;

  double real1 = (double) integer;

  if (fabs(real1 - real) >= 1.0)
    *error_code = CEXPR_ERROR_REAL_TOO_BIG_FOR_INTEGER;

  return integer;
}
