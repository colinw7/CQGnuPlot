#include <CExprI.h>

class CExprExecuteImpl {
 public:
  CExprExecuteImpl() { }

 ~CExprExecuteImpl() { }

  CExprValuePtr executeCTokenStack(const CExprCTokenStack &stack);

 private:
  void           executeToken(CExprCTokenPtr);
  void           executeOperator(CExprOperatorPtr);
  void           executeQuestionOperator();
  void           executeUnaryOperator(CExprOpType type);
  void           executeLogicalUnaryOperator(CExprOpType type);
  void           executeBitwiseUnaryOperator(CExprOpType type);
  void           executeBinaryOperator(CExprOpType type);
  void           executeLogicalBinaryOperator(CExprOpType type);
  void           executeBitwiseBinaryOperator(CExprOpType type);
  void           executeColonOperator();
  void           executeCommaOperator();
  void           executeEqualsOperator();
  CExprValuePtr  executeFunction(CExprFunctionPtr function);
  CExprValuePtr  etokenToValue(CExprETokenPtr);
#if 0
  CExprValueType etokenToValueType(CExprETokenPtr);
#endif
  void           stackIdentifier(const std::string &identifier);
  void           stackValue(CExprValuePtr value);
  void           stackEToken(CExprETokenPtr);
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

CExprValuePtr
CExprExecute::
executeCTokenStack(const CExprCTokenStack &stack)
{
  return impl_->executeCTokenStack(stack);
}

//------------

CExprValuePtr
CExprExecuteImpl::
executeCTokenStack(const CExprCTokenStack &stack)
{
  ctoken_stack_ = stack;

  etoken_stack_.clear();

  uint num_ctokens = ctoken_stack_.getNumTokens();

  for (uint i = 0; i < num_ctokens; ++i) {
    CExprCTokenPtr ctoken = ctoken_stack_.getToken(i);

    executeToken(ctoken);

    if (CExprInst->getDebug())
      std::cerr << "Stack:" << etoken_stack_ << std::endl;
  }

  CExprETokenPtr etoken = unstackEToken();

  if (! etoken.isValid())
    return CExprValuePtr();

  return etokenToValue(etoken);
}

void
CExprExecuteImpl::
executeToken(CExprCTokenPtr ctoken)
{
  switch (ctoken->getType()) {
    case CEXPR_CTOKEN_IDENTIFIER:
      stackIdentifier(ctoken->getIdentifier());

      break;
    case CEXPR_CTOKEN_OPERATOR:
      executeOperator(ctoken->getOperator());

      break;
    case CEXPR_CTOKEN_INTEGER: {
      CExprValuePtr value = CExprValue::createIntegerValue(ctoken->getInteger());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_REAL: {
      CExprValuePtr value = CExprValue::createRealValue(ctoken->getReal());

      stackValue(value);

      break;
    }
    case CEXPR_CTOKEN_STRING: {
      CExprValuePtr value = CExprValue::createStringValue(ctoken->getString());

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
}

void
CExprExecuteImpl::
executeOperator(CExprOperatorPtr op)
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
      executeBinaryOperator(type);
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
    default:
      std::cerr << "Invalid operator for 'executeOperator'" << std::endl;
      break;
  }
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

void
CExprExecuteImpl::
executeBinaryOperator(CExprOpType type)
{
  CExprETokenPtr etoken2 = unstackEToken();
  CExprETokenPtr etoken1 = unstackEToken();

  if (! etoken1.isValid() || ! etoken2.isValid())
    return;

  CExprValuePtr value1 = etokenToValue(etoken1);
  CExprValuePtr value2 = etokenToValue(etoken2);

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (value1->isRealValue() || value2->isRealValue()) {
    if (! value1->convToReal()) return;
    if (! value2->convToReal()) return;
  }

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  stackValue(value);
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
executeFunction(CExprFunctionPtr function)
{
  std::vector<CExprValuePtr> values;

  uint i = 0;

  uint num_args = function->numArgs();

  for ( ; i < num_args; i++) {
    CExprETokenPtr etoken = unstackEToken();

    if (! etoken.isValid())
      break;

    CExprValuePtr value1 = etokenToValue(etoken);

    if (! value1.isValid() && ! (function->argType(i) & CEXPR_VALUE_NULL))
      break;

    if (! value1.isValid() && ! value1->convToType(function->argType(i)))
      break;

    values.push_back(value1);
  }

  if (i < num_args)
    return CExprValuePtr();

  return function->exec(values);
}

CExprValuePtr
CExprExecuteImpl::
etokenToValue(CExprETokenPtr etoken)
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

#if 0
CExprValueType
ExprExecute::
etokenToValueType(CExprETokenPtr etoken)
{
  CExprValueType type = CEXPR_VALUE_NONE;

  switch (etoken->getType()) {
    case CEXPR_ETOKEN_IDENTIFIER: {
      CExprVariablePtr variable = CExprInst->getVariable(etoken->getIdentifier());

      if (variable)
        type = variable->getValueType();

      break;
    }
    case CEXPR_ETOKEN_VALUE:
      type = etoken->getValue()->getType();

      break;
    default:
      break;
  }

  return type;
}
#endif

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
