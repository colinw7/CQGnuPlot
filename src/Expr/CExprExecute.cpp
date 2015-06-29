#include <CExprI.h>

class CExprExecuteImpl {
 public:
  CExprExecuteImpl() { }

 ~CExprExecuteImpl() { }

  bool executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values);
  bool executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value);

 private:
  bool executeToken                (const CExprTokenBaseP &ctoken);
  bool executeOperator             (const CExprTokenBaseP &ctoken);
  void executeQuestionOperator     ();
  void executeUnaryOperator        (CExprOpType type);
  void executeLogicalUnaryOperator (CExprOpType type);
  void executeBitwiseUnaryOperator (CExprOpType type);
  bool executeBinaryOperator       (CExprOpType type);
  void executeLogicalBinaryOperator(CExprOpType type);
  void executeBitwiseBinaryOperator(CExprOpType type);
  void executeColonOperator        ();
  void executeCommaOperator        ();
#ifdef GNUPLOT_EXPR
  void executeSubscriptOperator    ();
#endif
  void executeEqualsOperator       ();
  bool executeFunction             (const CExprFunctionPtr &function, CExprValuePtr &value);
  bool executeBlock                (const CExprTokenStack &stack, CExprValuePtr &value);

  CExprValuePtr  etokenToValue(const CExprTokenBaseP &etoken);
#if 0
  CExprValueType etokenToValueType(const CExprTokenBaseP &etoken);
#endif

  void stackValue (const CExprValuePtr &value);
  void stackBlock ();
  void stackEToken(const CExprTokenBaseP &etoken);

  CExprValuePtr   unstackValue ();
  CExprTokenBaseP unstackEToken();

 private:
  CExprTokenStack ctoken_stack_;
  uint            ctoken_pos_;
  uint            num_ctokens_;
  CExprTokenStack etoken_stack_;
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
executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values)
{
  return impl_->executeCTokenStack(stack, values);
}

bool
CExprExecute::
executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value)
{
  return impl_->executeCTokenStack(stack, value);
}

//------------

bool
CExprExecuteImpl::
executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values)
{
  ctoken_stack_ = stack;

  etoken_stack_.clear();

  num_ctokens_ = ctoken_stack_.getNumTokens();
  ctoken_pos_  = 0;

  while (ctoken_pos_ < num_ctokens_) {
    CExprTokenBaseP ctoken = ctoken_stack_.getToken(ctoken_pos_++);

    if (! executeToken(ctoken))
      return false;

    if (CExprInst->getDebug())
      std::cerr << "EToken Stack:" << etoken_stack_ << std::endl;
  }

  std::deque<CExprValuePtr> values1;

  bool rc = true;

  while (! etoken_stack_.empty()) {
    CExprValuePtr value = unstackValue();

    if (value.isValid())
      values1.push_front(value);
    else
      rc = false;
  }

  std::copy(values1.begin(), values1.end(), std::back_inserter(values));

  return rc;
}

bool
CExprExecuteImpl::
executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value)
{
  CExprValueArray values;

  if (! executeCTokenStack(stack, values))
    return false;

  if (values.empty())
    value = CExprValuePtr();
  else
    value = values.back();

  return true;
}

bool
CExprExecuteImpl::
executeToken(const CExprTokenBaseP &ctoken)
{
  switch (ctoken->type()) {
    case CEXPR_TOKEN_IDENTIFIER:
      stackEToken(ctoken);

      break;
    case CEXPR_TOKEN_OPERATOR:
      if (! executeOperator(ctoken))
        return false;

      break;
    case CEXPR_TOKEN_INTEGER: {
      CExprValuePtr value = CExprInst->createIntegerValue(ctoken->getInteger());

      stackValue(value);

      break;
    }
    case CEXPR_TOKEN_REAL: {
      CExprValuePtr value = CExprInst->createRealValue(ctoken->getReal());

      stackValue(value);

      break;
    }
    case CEXPR_TOKEN_STRING: {
      CExprValuePtr value = CExprInst->createStringValue(ctoken->getString());

      stackValue(value);

      break;
    }
    case CEXPR_TOKEN_COMPLEX: {
      CExprValuePtr value = CExprInst->createComplexValue(ctoken->getComplex());

      stackValue(value);

      break;
    }
    case CEXPR_TOKEN_FUNCTION: {
      stackEToken(ctoken);

      break;
    }
    case CEXPR_TOKEN_VALUE:
      stackEToken(ctoken);

      break;
    default:
      assert(false);
      break;
  }

  return true;
}

bool
CExprExecuteImpl::
executeOperator(const CExprTokenBaseP &ctoken)
{
  CExprOpType type = ctoken->getOperator();

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
    case CEXPR_OP_STR_EQUAL:
    case CEXPR_OP_NOT_EQUAL:
    case CEXPR_OP_STR_NOT_EQUAL:
#ifdef GNUPLOT_EXPR
    case CEXPR_OP_STR_CONCAT:
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
      stackEToken(ctoken);
      break;
#ifdef GNUPLOT_EXPR
    case CEXPR_OP_OPEN_SBRACKET:
      stackEToken(ctoken);
      break;
    case CEXPR_OP_CLOSE_SBRACKET:
      executeSubscriptOperator();
      break;
#endif
    case CEXPR_OP_START_BLOCK:
      stackBlock();
      break;
    default: {
      CExprInst->errorMsg("Invalid operator for 'executeOperator'");
      return false;
    }
  }

  return true;
}

void
CExprExecuteImpl::
executeQuestionOperator()
{
  // pop boolean
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  bool flag;

  if (! value->getBooleanValue(flag))
    flag = false;

  //---

  // pop second token (rhs=false)
  CExprTokenBaseP etoken2 = unstackEToken();

  //---

  // pop first token (lhs=tue)
  CExprTokenBaseP etoken1 = unstackEToken();

  //---

  CExprValuePtr value1;

  if (flag) {
    if (etoken1.isValid())
      value1 = etokenToValue(etoken1);
  }
  else {
    if (etoken2.isValid())
      value1 = etokenToValue(etoken2);
  }

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */
void
CExprExecuteImpl::
executeUnaryOperator(CExprOpType type)
{
  CExprValuePtr value = unstackValue();

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
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  if (! value->isBooleanValue()) {
    value = value->dup();

    if (! value->convToBoolean())
      return;
  }

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
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  if (! value->isBooleanValue()) {
    value = value->dup();

    if (! value->convToBoolean())
      return;
  }

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
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return false;

  if (value1->isComplexValue() || value2->isComplexValue()) {
    if (! value1->isComplexValue()) value1 = value1->dup();
    if (! value2->isComplexValue()) value2 = value2->dup();

    if (! value1->convToComplex()) return false;
    if (! value2->convToComplex()) return false;
  }

  if (value1->isRealValue() || value2->isRealValue()) {
    if (! value1->isRealValue()) value1 = value1->dup();
    if (! value2->isRealValue()) value2 = value2->dup();

    if (! value1->convToReal()) return false;
    if (! value2->convToReal()) return false;
  }

#ifdef GNUPLOT_EXPR
  if (value2->isStringValue() && ! value1->isStringValue()) {
    if (! value1->isStringValue()) value1 = value1->dup();

    if (! value1->convToString()) return false;
  }
#endif

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  stackValue(value);

  return true;
}

/* <value1> <value2> <binary_op> */
void
CExprExecuteImpl::
executeLogicalBinaryOperator(CExprOpType type)
{
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->isBooleanValue()) {
    value1 = value1->dup();

    if (! value1->convToBoolean())
      return;
  }

  if (! value2->isBooleanValue()) {
    value2 = value2->dup();

    if (! value2->convToBoolean())
      return;
  }

  CExprValuePtr value = value1->execBinaryOp(value2, type);

  stackValue(value);
}

/* <value1> <value2> <binary_op> */
void
CExprExecuteImpl::
executeBitwiseBinaryOperator(CExprOpType type)
{
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->isIntegerValue()) {
    value1 = value1->dup();

    if (! value1->convToInteger())
      return;
  }

  if (! value2->isIntegerValue()) {
    value2 = value2->dup();

    if (! value2->convToInteger())
      return;
  }

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
  // get subscript values (popped in reverse order)
  std::deque<CExprValuePtr> values;

  CExprTokenBaseP etoken1 = unstackEToken();

  while (etoken1.isValid() && etoken1->type() != CEXPR_TOKEN_OPERATOR) {
    CExprValuePtr value1 = etokenToValue(etoken1);

    if (! value1.isValid())
      return;

    values.push_front(value1);

    etoken1 = unstackEToken();
  }

  CExprValueArray values1;

  std::copy(values.begin(), values.end(), std::back_inserter(values1));

  //---

  // unscript value
  CExprValuePtr value2 = unstackValue();

  if (! value2.isValid())
    return;

  //---

  CExprValuePtr value = value2->subscript(values1);

  stackValue(value);
}
#endif

void
CExprExecuteImpl::
executeEqualsOperator()
{
  // rhs
  CExprValuePtr value1 = unstackValue();

  if (! value1.isValid())
    return;

  //---

  CExprTokenBaseP etoken2 = unstackEToken();

  if (! etoken2.isValid())
    return;

  //---

  CExprValuePtr value;

  if (etoken2->type() == CEXPR_TOKEN_IDENTIFIER) {
    CExprVariablePtr variable = CExprInst->createVariable(etoken2->getIdentifier(), value1);

    value = variable->getValue();
  }
  else {
    CExprInst->errorMsg("Non lvalue for asssignment");
    return;
  }

  stackValue(value);
}

bool
CExprExecuteImpl::
executeFunction(const CExprFunctionPtr &function, CExprValuePtr &value)
{
  //uint num_args = function->numArgs();

  std::deque<CExprValuePtr> values;

  CExprTokenBaseP etoken = unstackEToken();

  if (! etoken.isValid())
    return false;

  while (etoken->type() != CEXPR_TOKEN_OPERATOR) {
    CExprValuePtr value1 = etokenToValue(etoken);

    values.push_front(value1);

    etoken = unstackEToken();
    assert(etoken.isValid());
  }

  for (uint i = 0; i < values.size(); ++i) {
    CExprValuePtr value1 = values[i];

    CExprValueType argType = function->argType(i);

    if (! value1.isValid()) {
      if (! (argType & CEXPR_VALUE_NULL)) {
        CExprInst->errorMsg("Invalid type for function argument");
        return false;
      }
    }
    else {
      if (! (argType & CEXPR_VALUE_NULL)) {
        if (! (value1->getType() & argType))
          value1 = value1->dup();

        if (! value1->convToType(argType)) {
          CExprInst->errorMsg("Invalid type for function argument");
          return false;
        }
      }
    }
  }

  assert(etoken->type() == CEXPR_TOKEN_OPERATOR);

  CExprValueArray values1;

  std::copy(values.begin(), values.end(), std::back_inserter(values1));

  if (! function->checkValues(values1)) {
    std::stringstream ostr;
    ostr << "Invalid function values : ";
    function->print(ostr);
    CExprInst->errorMsg(ostr.str());
    return false;
  }

  value = function->exec(values1);

  return true;
}

bool
CExprExecuteImpl::
executeBlock(const CExprTokenStack &stack, CExprValuePtr &value)
{
  CExprExecuteImpl impl;

  return impl.executeCTokenStack(stack, value);
}

CExprValuePtr
CExprExecuteImpl::
etokenToValue(const CExprTokenBaseP &etoken)
{
  switch (etoken->type()) {
    case CEXPR_TOKEN_IDENTIFIER: {
      CExprVariablePtr variable = CExprInst->getVariable(etoken->getIdentifier());

      if (variable.isValid())
        return variable->getValue();

      break;
    }
    case CEXPR_TOKEN_FUNCTION: {
      CExprValuePtr value;

      if (executeFunction(etoken->getFunction(), value))
        return value;

      break;
    }
    case CEXPR_TOKEN_BLOCK: {
      CExprValuePtr value;

      if (executeBlock(etoken->getBlock(), value))
        return value;

      break;
    }
    case CEXPR_TOKEN_VALUE:
      return etoken->getValue();
    default:
      break;
  }

  return CExprValuePtr();
}

void
CExprExecuteImpl::
stackValue(const CExprValuePtr &value)
{
  if (! value.isValid()) return;

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackEToken(base);
}

void
CExprExecuteImpl::
stackBlock()
{
  CExprTokenStack stack;

  int brackets = 1;

  while (ctoken_pos_ < num_ctokens_) {
    CExprTokenBaseP ctoken = ctoken_stack_.getToken(ctoken_pos_++);

    if (! ctoken.isValid())
      break;

    if (ctoken->type() == CEXPR_TOKEN_OPERATOR) {
      CExprOpType op = ctoken->getOperator();

      if      (op == CEXPR_OP_START_BLOCK)
        ++brackets;
      else if (op == CEXPR_OP_END_BLOCK) {
        --brackets;

        if (brackets <= 0)
          break;
      }
    }

    stack.addToken(ctoken);
  }

  CExprTokenBaseP base(new CExprTokenBlock(stack));

  stackEToken(base);
}

void
CExprExecuteImpl::
stackEToken(const CExprTokenBaseP &base)
{
  etoken_stack_.addToken(base);
}

CExprValuePtr
CExprExecuteImpl::
unstackValue()
{
  CExprTokenBaseP etoken = unstackEToken();

  if (! etoken.isValid())
    return CExprValuePtr();

  return etokenToValue(etoken);
}

CExprTokenBaseP
CExprExecuteImpl::
unstackEToken()
{
  CExprTokenBaseP etoken = etoken_stack_.pop_back();

  return etoken;
}
