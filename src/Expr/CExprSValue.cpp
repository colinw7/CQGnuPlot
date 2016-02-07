#include <CExprI.h>

bool
CExprStringValue::
getBooleanValue(bool &b) const
{
  return CStrUtil::toBool(str_, &b);
}

bool
CExprStringValue::
getIntegerValue(long &l) const
{
  return CStrUtil::toInteger(str_, &l);
}

bool
CExprStringValue::
getRealValue(double &r) const
{
  return CStrUtil::toReal(str_, &r);
}

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

  //---

  std::string result;

  switch (op) {
    case CExprOpType::LESS:
      return CExprInst->createBooleanValue(str_ < rstr);
    case CExprOpType::LESS_EQUAL:
      return CExprInst->createBooleanValue(str_ <= rstr);
    case CExprOpType::GREATER:
      return CExprInst->createBooleanValue(str_ > rstr);
    case CExprOpType::GREATER_EQUAL:
      return CExprInst->createBooleanValue(str_ >= rstr);
    // TODO: disable for gnuplot ?
    case CExprOpType::EQUAL:
      return CExprInst->createBooleanValue(str_ == rstr);
    case CExprOpType::NOT_EQUAL:
      return CExprInst->createBooleanValue(str_ != rstr);
#ifdef GNUPLOT_EXPR
    case CExprOpType::STR_EQUAL:
      return CExprInst->createBooleanValue(str_ == rstr);
    case CExprOpType::STR_NOT_EQUAL:
      return CExprInst->createBooleanValue(str_ != rstr);
    case CExprOpType::STR_CONCAT:
      return CExprInst->createStringValue(str_ + rstr);
    default: {
      uint   i1       = 0    , i2       = 0;
      long   integer1 = 0    , integer2 = 0;
      double real1    = 0.0  , real2    = 0.0;
      bool   is_int1  = false, is_int2  = false;

      CStrUtil::skipSpace(str_, &i1);
      CStrUtil::skipSpace(rstr, &i2);

      if (! CExprStringToNumber(str_, &i1, integer1, real1, is_int1) ||
          ! CExprStringToNumber(rstr, &i2, integer2, real2, is_int2))
        return CExprValuePtr();

      CStrUtil::skipSpace(str_, &i1);
      CStrUtil::skipSpace(rstr, &i2);

      if (i1 != str_.size() || i2 != rstr.size())
        return CExprValuePtr();

      if (is_int1 && is_int2) {
        CExprValuePtr ival1 = CExprInst->createIntegerValue(integer1);
        CExprValuePtr ival2 = CExprInst->createIntegerValue(integer2);

        return ival1->execBinaryOp(ival2, op);
      }
      else {
        CExprValuePtr rval1 = CExprInst->createRealValue(real1);
        CExprValuePtr rval2 = CExprInst->createRealValue(real2);

        return rval1->execBinaryOp(rval2, op);
      }
    }
#else
    case CExprOpType::PLUS:
      return CExprInst->createStringValue(str_ + rstr);
    default:
      return CExprValuePtr();
#endif
  }
}

CExprValuePtr
CExprStringValue::
subscript(const CExprValueArray &values) const
{
#ifdef GNUPLOT_EXPR
  if      (values.size() == 1) {
    long i1 = 1;

    if (values[0].isValid()) {
      if (! values[0]->getIntegerValue(i1))
        return CExprValuePtr();

      if (i1 < 1               ) i1 = 1;
      if (i1 > int(str_.size())) i1 = str_.size();
    }

    std::string str;

    str += str_[i1 - 1];

    return CExprInst->createStringValue(str);
  }
  else if (values.size() == 2) {
    long i1 = 1, i2 = str_.size();

    if (values[0].isValid()) {
      if (! values[0]->getIntegerValue(i1))
        return CExprValuePtr();

      if (i1 < 1               ) i1 = 1;
      if (i1 > int(str_.size())) i1 = str_.size();
    }

    if (values[1].isValid()) {
      if (! values[1]->getIntegerValue(i2))
        return CExprValuePtr();

      if (i2 < 0               ) i2 = str_.size();
      if (i2 > int(str_.size())) i2 = str_.size();
    }

    std::string str;

    for (int j = i1 - 1; j <= i2 - 1; ++j)
      str += str_[j];

    return CExprInst->createStringValue(str);
  }
  else {
    return CExprValuePtr();
  }
#else
  return CExprValuePtr();
#endif
}
