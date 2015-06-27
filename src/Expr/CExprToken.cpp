#include <CExprI.h>

const std::string &
CExprTokenBase::
getIdentifier() const
{
  assert(type() == CEXPR_TOKEN_IDENTIFIER);

  return static_cast<const CExprTokenIdentifier *>(this)->getIdentifier();
}

CExprOpType
CExprTokenBase::
getOperator() const
{
  assert(type() == CEXPR_TOKEN_OPERATOR);

  return static_cast<const CExprTokenOperator *>(this)->getType();
}

long
CExprTokenBase::
getInteger() const
{
  assert(type() == CEXPR_TOKEN_INTEGER);

  return static_cast<const CExprTokenInteger *>(this)->getInteger();
}

double
CExprTokenBase::
getReal() const
{
  assert(type() == CEXPR_TOKEN_REAL);

  return static_cast<const CExprTokenReal *>(this)->getReal();
}

const std::string &
CExprTokenBase::
getString() const
{
  assert(type() == CEXPR_TOKEN_STRING);

  return static_cast<const CExprTokenString *>(this)->getString();
}

const std::complex<double> &
CExprTokenBase::
getComplex() const
{
  assert(type() == CEXPR_TOKEN_COMPLEX);

  return static_cast<const CExprTokenComplex *>(this)->getComplex();
}

CExprFunctionPtr
CExprTokenBase::
getFunction() const
{
  assert(type() == CEXPR_TOKEN_FUNCTION);

  return static_cast<const CExprTokenFunction *>(this)->getFunction();
}

CExprValuePtr
CExprTokenBase::
getValue() const
{
  assert(type() == CEXPR_TOKEN_VALUE);

  return static_cast<const CExprTokenValue *>(this)->getValue();
}

const CExprTokenStack &
CExprTokenBase::
getBlock() const
{
  assert(type() == CEXPR_TOKEN_BLOCK);

  return static_cast<const CExprTokenBlock *>(this)->stack();
}

void
CExprTokenBase::
printQualified(std::ostream &os) const
{
  switch (type()) {
    case CEXPR_TOKEN_IDENTIFIER:
      os << "<identifier>";
      break;
    case CEXPR_TOKEN_OPERATOR:
      os << "<operator>";
      break;
    case CEXPR_TOKEN_INTEGER:
      os << "<integer>";
      break;
    case CEXPR_TOKEN_REAL:
      os << "<real>";
      break;
    case CEXPR_TOKEN_STRING:
      os << "<string>";
      break;
    case CEXPR_TOKEN_COMPLEX:
      os << "<complex>";
      break;
    case CEXPR_TOKEN_FUNCTION:
      os << "<function>";
      break;
    case CEXPR_TOKEN_VALUE:
      os << "<value>";
      break;
    case CEXPR_TOKEN_BLOCK:
      os << "<block>";
      break;
    default:
      os << "<-?->";
      break;
  }

  print(os);
}

//----

void
CExprTokenOperator::
print(std::ostream &os) const
{
  os << CExprInst->getOperatorName(type_);
}

void
CExprTokenFunction::
print(std::ostream &os) const
{
  function_->print(os, /*expanded*/false);
}

//------

void
CExprTokenStack::
print(std::ostream &os) const
{
  uint len = stack_.size();

  for (uint i = 0; i < len; ++i) {
    if (i > 0) os << " ";

    stack_[i]->printQualified(os);
  }
}
