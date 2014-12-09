#include <CExprI.h>

class CExprExecuteImpl {
 public:
  CExprExecuteImpl() { }

 ~CExprExecuteImpl() { }

  bool executeCTokenStack(const CExprCTokenStack &stack, std::vector<CExprValuePtr> &values);

  CExprValuePtr executeCTokenStack(const CExprCTokenStack &stack);

 private:
  bool           executeToken(const CExprCTokenPtr &ctoken);
  bool           executeOperator(const CExprOperatorPtr &op);
  void           executeQuestionOperator();
  void           executeUnaryOperator(CExprOpType type);
  void           executeLogicalUnaryOperator(CExprOpType type);
  void           executeBitwiseUnaryOperator(CExprOpType type);
  bool           executeBinaryOperator(CExprOpType type);
  void           executeLogicalBinaryOperator(CExprOpType type);
  void           executeBitwiseBinaryOperator(CExprOpType type);
  void           executeColonOperator();
  void           executeCommaOperator();
#ifdef GNUPLOT_EXPR
  void           executeSubscriptOperator();
#endif
  void           executeEqualsOperator();
  CExprValuePtr  executeFunction(const CExprFunctionPtr &function);
  CExprValuePtr  etokenToValue(const CExprETokenPtr &etoken);
#if 0
  CExprValueType etokenToValueType(CExprETokenPtr);
#endif
  void           stackIdentifier(const std::string &identifier);
  void           stackValue(CExprValuePtr value);
  void           stackOperator(CExprOperatorPtr op);
  void           stackEToken(CExprETokenPtr etoken);
  CExprETokenPtr unstackEToken();

 private:
  CExprCTokenStack ctoken_stack_;
  CExprETokenStack etoken_stack_;
};

//------------

CExprExecute::
CExprExecute()
{
  impl_ = new CExprExecuteImpl;
}

CExprExecute::
~CExprExecute()
{
}

bool
CExprExecute::
executeCTokenStack(const CExprCTokenStack &stack, std::vector<CExprValuePtr> &values)
{
  return impl_->executeCTokenStack(stack, values);
}

CExprValuePtr
CExprExecute::
executeCTokenStack(const CExprCTokenStack &stack)
{
  return impl_->executeCTokenStack(stack);
}

//------------

bool
CExprExecuteImpl::
executeCTokenStack(const CExprCTokenStack &stack, std::vector<CExprValuePtr> &values)
{
  ctoken_stack_ = stack;

  etoken_stack_.clear();

  uint num_ctokens = ctoken_stack_.getNumTokens();

  for (uint i = 0; i < num_ctokens; ++i) {
    CExprCTokenPtr ctoken = ctoken_stack_.getToken(i);

    if (! executeToken(ctoken))
      return false;

    if (CExprInst->getDebug())
      std::cerr << "EToken Stack:" << etoken_stack_ << std::endl;
  }

  std::deque<CExprValuePtr> values1;

  bool rc = true;

  while (! etoken_stack_.empty()) {
    CExprETokenPtr etoken = unstackEToken();

    if (etoken.isValid()) {
      CExprValuePtr value = etokenToValue(etoken);

      if (value.isValid())
        values1.push_front(value);
      else
        rc = false;
    }
    else
      rc = false;
  }

  std::copy(values1.begin(), values1.end(), std::back_inserter(values));

  return rc;
}

CExprValuePtr
CExprExecuteImpl::
executeCTokenStack(const CExprCTokenStack &stack)
{
  std::vector<CExprValuePtr> values;

  (void) executeCTokenStack(stack, values);

  if (values.empty())
    return CExprValuePtr();

  return values.back();
}

bool
CExprExecuteImpl::
executeToken(const CExprCTokenPtr &ctoken)
{
  switch (ctoken->getType()) {
    case CEXPR_CTOKEN_IDENTIFIER:
      stackIdentifier(ctoken->getIdentifier());

      break;
    case CEXPR_CTOKEN_OPERATOR:
      if (! executeOperator(ctoken->getOperator()))
        return false;

      break;
    case CEXPR_CTOKEN_INTEGER: {
      CExprValuePtr value = CExprInst->createIntegerValue(ctoken->getInteger());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_REAL: {
      CExprValuePtr value = CExprInst->createRealValue(ctoken->getReal());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_STRING: {
      CExprValuePtr value = CExprInst->createStringValue(ctoken->getString());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_FUNCTION: {
      CExprValuePtr value = executeFunction(ctoken->getFunction());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_VALUE:
      stackValue(ctoken->getValue());

      break;
    default:
      break;
  }

  return true;
}

bool
CExprExecuteImpl::
executeOperator(const CExprOperatorPtr &op)
{
  CExprOpType type = op->getType();

  switch (type) {
    case CEXPR_OP_LOGICAL_NOT:
      executeLogicalUnaryOperator(type);
      break;
    case CEXPR_OP_BIT_NOT:
      executeBitwiseUnaryOperator(type);
      break;
    case CEXPR_OP_UNARY_PLUS:
    case CEXPR_OP_UNARY_MINUS:
      executeUnaryOperator(type);
      break;
    case CEXPR_OP_POWER:
    case CEXPR_OP_TIMES:
    case CEXPR_OP_DIVIDE:
    case CEXPR_OP_MODULUS:
    case CEXPR_OP_PLUS:
    case CEXPR_OP_MINUS:
    case CEXPR_OP_LESS:
    case CEXPR_OP_LESS_EQUAL:
    case CEXPR_OP_GREATER:
    case CEXPR_OP_GREATER_EQUAL:
    case CEXPR_OP_EQUAL:
    case CEXPR_OP_NOT_EQUAL:
#ifdef GNUPLOT_EXPR
    case CEXPR_OP_CONCAT:
#endif
      if (! executeBinaryOperator(type))
        return false;

      break;
    case CEXPR_OP_LOGICAL_AND:
    case CEXPR_OP_LOGICAL_OR:
      executeLogicalBinaryOperator(type);
      break;
    case CEXPR_OP_BIT_LSHIFT:
    case CEXPR_OP_BIT_RSHIFT:
    case CEXPR_OP_BIT_AND:
    case CEXPR_OP_BIT_XOR:
    case CEXPR_OP_BIT_OR:
      executeBitwiseBinaryOperator(type);
      break;
    case CEXPR_OP_QUESTION:
      executeQuestionOperator();
      break;
    case CEXPR_OP_COLON:
      executeColonOperator();
      break;
    case CEXPR_OP_EQUALS:
      executeEqualsOperator();
      break;
    case CEXPR_OP_COMMA:
      executeCommaOperator();
      break;
    case CEXPR_OP_OPEN_RBRACKET:
      stackOperator(op);
      break;
#ifdef GNUPLOT_EXPR
    case CEXPR_OP_OPEN_SBRACKET:
      stackOperator(op);
      break;
    case CEXPR_OP_CLOSE_SBRACKET:
      executeSubscriptOperator();
      break;
#endif
    default: {
      std::cerr << "Invalid operator for 'executeOperator'" << std::endl;
      return false;
    }
  }

  return true;
}

void
CExprExecuteImpl::
executeQuestionOperator()
{
  CExprETokenPtr etoken1 = unstackEToken();
  CExprETokenPtr etoken2 = unstackEToken();
  CExprETokenPtr etoken3 = unstackEToken();

  if (! etoken1.isValid())
    return;

  CExprValuePtr value1 = etokenToValue(etoken1);

  if (! value1.isValid())
    return;

  bool flag;

  if (! value1->getBooleanValue(flag))
    flag = false;

  CExprValuePtr value2;

  if (flag) {
    if (! etoken2.isValid())
      return;

    value2 = etokenToValue(etoken2);
  }
  else {
    if (! etoken3.isValid())
      return;

    value2 = etokenToValue(etoken3);
  }

  if (! value2.isValid())
    return;

  stackValue(value2);
}

/* <value> <unary_op> */

void
CExprExecuteImpl::
executeUnaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken = unstackEToken();

  if (! etoken.isValid())
    return;

  CExprValuePtr value = etokenToValue(etoken);

  if (! value.isValid())
    return;

  CExprValuePtr value1 = value->execUnaryOp(type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */

void
CExprExecuteImpl::
executeLogicalUnaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken = unstackEToken();

  if (! etoken.isValid())
    return;

  CExprValuePtr value = etokenToValue(etoken);

  if (! value.isValid())
    return;

  if (! value->convToBoolean())
    return;

  CExprValuePtr value1 = value->execUnaryOp(type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */

void
CExprExecuteImpl::
executeBitwiseUnaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken = unstackEToken();

  if (! etoken.isValid())
    return;

  CExprValuePtr value = etokenToValue(etoken);

  if (! value.isValid())
    return;

  if (! value->convToInteger())
    return;

  CExprValuePtr value1 = value->execUnaryOp(type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value1> <value2> <binary_op> */

bool
CExprExecuteImpl::
executeBinaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken2 = unstackEToken();
  CExprETokenPtr etoken1 = unstackEToken();

  if (! etoken1.isValid() || ! etoken2.isValid())
    return false;

  CExprValuePtr value1 = etokenToValue(etoken1);
  CExprValuePtr value2 = etokenToValue(etoken2);

  if (! value1.isValid() || ! value2.isValid())
    return false;

  if (value1->isRealValue() || value2->isRealValue()) {
    if (! value1->convToReal()) return false;
    if (! value2->convToReal()) return false;
  }

#ifdef GNUPLOT_EXPR
  if (value2->isStringValue() && ! value1->isStringValue()) {
    if (! value1->convToString()) return false;
  }
#endif

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  if (! value.isValid())
    return false;

  stackValue(value);

  return true;
}

/* <value1> <value2> <binary_op> */

void
CExprExecuteImpl::
executeLogicalBinaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken2 = unstackEToken();
  CExprETokenPtr etoken1 = unstackEToken();

  if (! etoken1.isValid() || ! etoken2.isValid())
    return;

  CExprValuePtr value1 = etokenToValue(etoken1);
  CExprValuePtr value2 = etokenToValue(etoken2);

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->convToBoolean()) return;
  if (! value2->convToBoolean()) return;

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  stackValue(value);
}

/* <value1> <value2> <binary_op> */

void
CExprExecuteImpl::
executeBitwiseBinaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken2 = unstackEToken();
  CExprETokenPtr etoken1 = unstackEToken();

  if (! etoken1.isValid() || ! etoken2.isValid())
    return;

  CExprValuePtr value1 = etokenToValue(etoken1);
  CExprValuePtr value2 = etokenToValue(etoken2);

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->convToInteger()) return;
  if (! value2->convToInteger()) return;

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  stackValue(value);
}

void
CExprExecuteImpl::
executeColonOperator()
{
}

void
CExprExecuteImpl::
executeCommaOperator()
{
}

#ifdef GNUPLOT_EXPR
void
CExprExecuteImpl::
executeSubscriptOperator()
{
  std::deque<CExprETokenPtr> etokens;

  CExprETokenPtr etoken1 = unstackEToken();

  while (etoken1.isValid() && etoken1->getType() != CEXPR_ETOKEN_OPERATOR) {
    etokens.push_front(etoken1);

    etoken1 = unstackEToken();
  }

  CExprETokenPtr etoken2 = unstackEToken();

  if (! etoken2.isValid())
    return;

  std::vector<CExprValuePtr> values;

  for (uint i = 0; i < etokens.size(); ++i) {
    CExprValuePtr value = etokenToValue(etokens[i]);

    if (! value.isValid())
      return;

    values.push_back(value);
  }

  CExprValuePtr value2 = etokenToValue(etoken2);

  if (! value2.isValid())
    return;

  CExprValuePtr value = value2->subscript(values);

  stackValue(value);
}
#endif

void
CExprExecuteImpl::
executeEqualsOperator()
{
  CExprETokenPtr etoken1 = unstackEToken();
  CExprETokenPtr etoken2 = unstackEToken();

  if (! etoken1.isValid() || ! etoken2.isValid())
    return;

  CExprValuePtr value1 = etokenToValue(etoken1);

  if (! value1.isValid())
    return;

  CExprValuePtr value;

  if (etoken2->getType() == CEXPR_ETOKEN_IDENTIFIER) {
    CExprVariablePtr variable = CExprInst->createVariable(etoken2->getIdentifier(), value1);

    value = variable->getValue();
  }
  else {
    std::cerr << "Non lvalue for asssignment" << std::endl;
    return;
  }

  stackValue(value);
}

CExprValuePtr
CExprExecuteImpl::
executeFunction(const CExprFunctionPtr &function)
{
  //uint num_args = function->numArgs();

  std::deque<CExprValuePtr> values;

  CExprETokenPtr etoken = unstackEToken();
  assert(etoken.isValid());

  while (etoken->getType() != CEXPR_ETOKEN_OPERATOR) {
    CExprValuePtr value1 = etokenToValue(etoken);

    CExprValueType argType = function->argType(values.size());

    if (! value1.isValid()) {
      if (! (argType & CEXPR_VALUE_NULL))
        return CExprValuePtr();
    }
    else {
      if (! (argType & CEXPR_VALUE_NULL) && ! value1->convToType(argType)) {
        std::cerr << "Invalid type for function argument" << std::endl;
        return CExprValuePtr();
      }
    }

    values.push_front(value1);

    etoken = unstackEToken();
    assert(etoken.isValid());
  }

  assert(etoken->getType() == CEXPR_ETOKEN_OPERATOR);

  std::vector<CExprValuePtr> values1;

  std::copy(values.begin(), values.end(), std::back_inserter(values1));

  return function->exec(values1);
}

CExprValuePtr
CExprExecuteImpl::
etokenToValue(const CExprETokenPtr &etoken)
{
  switch (etoken->getType()) {
    case CEXPR_ETOKEN_IDENTIFIER: {
      CExprVariablePtr variable = CExprInst->getVariable(etoken->getIdentifier());

      if (variable.isValid())
        return variable->getValue();

      break;
    }
    case CEXPR_ETOKEN_VALUE:
      return etoken->getValue();
    default:
      break;
  }

  return CExprValuePtr();
}

void
CExprExecuteImpl::
stackIdentifier(const std::string &identifier)
{
  CExprETokenPtr etoken = CExprEToken::createIdentifierToken(identifier);

  stackEToken(etoken);
}

void
CExprExecuteImpl::
stackValue(CExprValuePtr value)
{
  if (! value.isValid()) return;

  CExprETokenPtr etoken = CExprEToken::createValueToken(value);

  stackEToken(etoken);
}

void
CExprExecuteImpl::
stackOperator(CExprOperatorPtr op)
{
  if (! op.isValid()) return;

  CExprETokenPtr etoken = CExprEToken::createOperatorToken(op);

  stackEToken(etoken);
}

void
CExprExecuteImpl::
stackEToken(CExprETokenPtr etoken)
{
  etoken_stack_.addToken(etoken);
}

CExprETokenPtr
CExprExecuteImpl::
unstackEToken()
{
  CExprETokenPtr etoken = etoken_stack_.pop_back();

  return etoken;
}

//------

CExprETokenPtr
CExprEToken::
createIdentifierToken(const std::string &identifier)
{
  CExprETokenPtr etoken(new CExprEToken);

  etoken->type_ = CEXPR_ETOKEN_IDENTIFIER;
  etoken->base_ = new CExprETokenIdentifier(identifier);

  return etoken;
}

CExprETokenPtr
CExprEToken::
createValueToken(CExprValuePtr value)
{
  CExprETokenPtr etoken(new CExprEToken);

  etoken->type_ = CEXPR_ETOKEN_VALUE;
  etoken->base_ = new CExprETokenValue(value);

  return etoken;
}

CExprETokenPtr
CExprEToken::
createOperatorToken(CExprOperatorPtr op)
{
  CExprETokenPtr etoken(new CExprEToken);

  etoken->type_ = CEXPR_ETOKEN_OPERATOR;
  etoken->base_ = new CExprETokenOperator(op);

  return etoken;
}

void
CExprEToken::
print(std::ostream &os) const
{
  switch (type_) {
    case CEXPR_ETOKEN_IDENTIFIER:
      os << "<identifier>";
      break;
    case CEXPR_ETOKEN_VALUE:
      os << "<value>";
      break;
    case CEXPR_ETOKEN_OPERATOR:
      os << "<operator>";
      break;
    default:
      os << "<-?->";
      break;
  }

  base_->print(os);

  os << " ";
}
