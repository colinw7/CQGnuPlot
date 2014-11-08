#include <CExprI.h>

CExprValue::
CExprValue() :
 type_(CEXPR_VALUE_NONE), base_(0)
{
}

CExprValue::
CExprValue(const CExprBooleanValue &boolean) :
 type_(CEXPR_VALUE_BOOLEAN), base_(boolean.dup())
{
}

CExprValue::
CExprValue(const CExprIntegerValue &integer) :
 type_(CEXPR_VALUE_INTEGER), base_(integer.dup())
{
}

CExprValue::
CExprValue(const CExprRealValue &real) :
 type_(CEXPR_VALUE_REAL), base_(real.dup())
{
}

CExprValue::
CExprValue(const CExprStringValue &str) :
 type_(CEXPR_VALUE_STRING), base_(str.dup())
{
}

CExprValue::
~CExprValue()
{
}

CExprValue *
CExprValue::
dup() const
{
  CExprValue *value1 = new CExprValue;

  value1->type_ = type_;

  if (base_)
    value1->base_ = base_->dup();

  return value1;
}

bool
CExprValue::
isBooleanValue() const
{
  return (type_ == CEXPR_VALUE_BOOLEAN);
}

bool
CExprValue::
isIntegerValue() const
{
  return (type_ == CEXPR_VALUE_INTEGER);
}

bool
CExprValue::
isRealValue() const
{
  return (type_ == CEXPR_VALUE_REAL);
}

bool
CExprValue::
isStringValue() const
{
  return (type_ == CEXPR_VALUE_STRING);
}

bool
CExprValue::
getBooleanValue(bool &b) const
{
  return base_->getBooleanValue(b);
}

bool
CExprValue::
getIntegerValue(long &l) const
{
  return base_->getIntegerValue(l);
}

bool
CExprValue::
getRealValue(double &r) const
{
  return base_->getRealValue(r);
}

bool
CExprValue::
getStringValue(std::string &s) const
{
  return base_->getStringValue(s);
}

void
CExprValue::
setBooleanValue(bool b)
{
  assert(isBooleanValue());

  base_->setBooleanValue(b);
}

void
CExprValue::
setIntegerValue(long l)
{
  assert(isIntegerValue());

  base_->setIntegerValue(l);
}

void
CExprValue::
setRealValue(double r)
{
  assert(isRealValue());

  base_->setRealValue(r);
}

void
CExprValue::
setStringValue(const std::string &s)
{
  assert(isStringValue());

  base_->setStringValue(s);
}

bool
CExprValue::
convToType(CExprValueType type)
{
  if      (type == CEXPR_VALUE_BOOLEAN)
    return convToBoolean();
  else if (type == CEXPR_VALUE_INTEGER)
    return convToInteger();
  else if (type == CEXPR_VALUE_REAL)
    return convToReal();
  else if (type == CEXPR_VALUE_STRING)
    return convToString();
  else
    return false;
}

bool
CExprValue::
convToBoolean()
{
  if (type_ == CEXPR_VALUE_BOOLEAN) return true;

  bool boolean;

  if (! getBooleanValue(boolean))
    return false;

  type_ = CEXPR_VALUE_BOOLEAN;
  base_ = new CExprBooleanValue(boolean);

  return true;
}

bool
CExprValue::
convToInteger()
{
  if (type_ == CEXPR_VALUE_INTEGER) return true;

  long integer;

  if (! getIntegerValue(integer))
    return false;

  type_ = CEXPR_VALUE_INTEGER;
  base_ = new CExprIntegerValue(integer);

  return true;
}

bool
CExprValue::
convToReal()
{
  if (type_ == CEXPR_VALUE_REAL) return true;

  double real;

  if (! getRealValue(real))
    return false;

  type_ = CEXPR_VALUE_INTEGER;
  base_ = new CExprRealValue(real);

  return true;
}

bool
CExprValue::
convToString()
{
  if (type_ == CEXPR_VALUE_STRING) return true;

  std::string str;

  if (! getStringValue(str))
    return false;

  type_ = CEXPR_VALUE_STRING;
  base_ = new CExprStringValue(str);

  return true;
}

CExprValuePtr
CExprValue::
execUnaryOp(CExprOpType op) const
{
  return base_->execUnaryOp(op);
}

CExprValuePtr
CExprValue::
execBinaryOp(CExprValuePtr rhs, CExprOpType op) const
{
  return base_->execBinaryOp(rhs, op);
}

void
CExprValue::
print(std::ostream &os) const
{
  base_->print(os);
}
