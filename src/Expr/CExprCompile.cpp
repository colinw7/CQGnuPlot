#include <CExprI.h>

class CExprCompileImpl {
 public:
  CExprCTokenStack compileIToken(CExprITokenPtr itoken);

 private:
  void        compileIToken1(CExprITokenPtr itoken);
  void        compileExpression(CExprITokenPtr itoken);
  void        compileAssignmentExpression(CExprITokenPtr itoken);
  void        compileConditionalExpression(CExprITokenPtr itoken);
  void        compileLogicalOrExpression(CExprITokenPtr itoken);
  void        compileLogicalAndExpression(CExprITokenPtr itoken);
  void        compileInclusiveOrExpression(CExprITokenPtr itoken);
  void        compileExclusiveOrExpression(CExprITokenPtr itoken);
  void        compileAndExpression(CExprITokenPtr itoken);
  void        compileEqualityExpression(CExprITokenPtr itoken);
  void        compileRelationalExpression(CExprITokenPtr itoken);
  void        compileShiftExpression(CExprITokenPtr itoken);
  void        compileAdditiveExpression(CExprITokenPtr itoken);
  void        compileMultiplicativeExpression(CExprITokenPtr itoken);
  void        compilePowerExpression(CExprITokenPtr itoken);
  void        compileUnaryExpression(CExprITokenPtr itoken);
  void        compilePostfixExpression(CExprITokenPtr itoken);
  void        compilePrimaryExpression(CExprITokenPtr itoken);
  void        compileArgumentExpressionList(CExprITokenPtr itoken);
  void        compileIdentifier(CExprITokenPtr itoken);
  void        compileOperator(CExprITokenPtr itoken);
  void        compileInteger(CExprITokenPtr itoken);
  void        compileReal(CExprITokenPtr itoken);
  void        compileString(CExprITokenPtr itoken);
#if 0
  void        compileITokenChildren(CExprITokenPtr itoken);
#endif
  void        stackIdentifier(const std::string &identifier);
  void        stackOperator(CExprOperatorPtr op);
  void        stackInteger(long);
  void        stackReal(double);
  void        stackString(const std::string &str);
  void        stackFunction(CExprFunctionPtr function);
  void        stackValue(CExprValuePtr);
  void        setLastError(const std::string &msg);
  bool        isError();
  std::string getLastError();

 private:
  CExprCTokenStack ctoken_stack_;
  bool             error_flag;
  std::string      last_error_message;
};

//------

CExprCompile::
CExprCompile()
{
  impl_ = new CExprCompileImpl;
}

CExprCompile::
~CExprCompile()
{
}

CExprCTokenStack
CExprCompile::
compileIToken(CExprITokenPtr itoken)
{
  return impl_->compileIToken(itoken);
}

//------

CExprCTokenStack
CExprCompileImpl::
compileIToken(CExprITokenPtr itoken)
{
  ctoken_stack_.clear();

  if (! itoken.isValid())
    return ctoken_stack_;

  setLastError("");

  compileIToken1(itoken);

  if (isError()) {
    std::cerr << getLastError() << std::endl;
    return CExprCTokenStack();
  }

  return ctoken_stack_;
}

void
CExprCompileImpl::
compileIToken1(CExprITokenPtr itoken)
{
  switch (itoken->getType()) {
    case CEXPR_EXPRESSION:
      compileExpression(itoken);

      break;
    case CEXPR_ASSIGNMENT_EXPRESSION:
      compileAssignmentExpression(itoken);

      break;
    case CEXPR_CONDITIONAL_EXPRESSION:
      compileConditionalExpression(itoken);

      break;
    case CEXPR_LOGICAL_OR_EXPRESSION:
      compileLogicalOrExpression(itoken);

      break;
    case CEXPR_LOGICAL_AND_EXPRESSION:
      compileLogicalAndExpression(itoken);

      break;
    case CEXPR_INCLUSIVE_OR_EXPRESSION:
      compileInclusiveOrExpression(itoken);

      break;
    case CEXPR_EXCLUSIVE_OR_EXPRESSION:
      compileExclusiveOrExpression(itoken);

      break;
    case CEXPR_AND_EXPRESSION:
      compileAndExpression(itoken);

      break;
    case CEXPR_EQUALITY_EXPRESSION:
      compileEqualityExpression(itoken);

      break;
    case CEXPR_RELATIONAL_EXPRESSION:
      compileRelationalExpression(itoken);

      break;
    case CEXPR_SHIFT_EXPRESSION:
      compileShiftExpression(itoken);

      break;
    case CEXPR_ADDITIVE_EXPRESSION:
      compileAdditiveExpression(itoken);

      break;
    case CEXPR_MULTIPLICATIVE_EXPRESSION:
      compileMultiplicativeExpression(itoken);

      break;
    case CEXPR_POWER_EXPRESSION:
      compilePowerExpression(itoken);

      break;
    case CEXPR_UNARY_EXPRESSION:
      compileUnaryExpression(itoken);

      break;
    case CEXPR_POSTFIX_EXPRESSION:
      compilePostfixExpression(itoken);

      break;
    case CEXPR_PRIMARY_EXPRESSION:
      compilePrimaryExpression(itoken);

      break;
    case CEXPR_ARGUMENT_EXPRESSION_LIST:
      compileArgumentExpressionList(itoken);

      break;
    case CEXPR_ITOKEN_IDENTIFIER:
      compileIdentifier(itoken);

      break;
    case CEXPR_ITOKEN_OPERATOR:
      compileOperator(itoken);

      break;
    case CEXPR_ITOKEN_INTEGER:
      compileInteger(itoken);

      break;
    case CEXPR_ITOKEN_REAL:
      compileReal(itoken);

      break;
    case CEXPR_ITOKEN_STRING:
      compileString(itoken);

      break;
    default:
      break;
  }
}

/*
 * <expression>:= <assignment_expression>
 * <expression>:= <expression> , <assignment_expression>
 */

void
CExprCompileImpl::
compileExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileExpression(itoken->getChild(0));

    compileAssignmentExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_COMMA));
  }
  else
    compileAssignmentExpression(itoken->getChild(0));
}

/*
 * <assignment_expression>:= <conditional_expression>
 * <assignment_expression>:= <unary_expression>   = <assignment_expression>
 * <assignment_expression>:= <unary_expression>  *= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  /= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  %= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  += <assignment_expression>
 * <assignment_expression>:= <unary_expression>  -= <assignment_expression>
 * <assignment_expression>:= <unary_expression> <<= <assignment_expression>
 * <assignment_expression>:= <unary_expression> >>= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  &= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  ^= <assignment_expression>
 * <assignment_expression>:= <unary_expression>  |= <assignment_expression>
 */

void
CExprCompileImpl::
compileAssignmentExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileUnaryExpression(itoken->getChild(0));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    switch (op->getType()) {
      case CEXPR_OP_EQUALS:
        compileAssignmentExpression(itoken->getChild(2));

        break;
      case CEXPR_OP_TIMES_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_TIMES));

        break;
      case CEXPR_OP_DIVIDE_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_DIVIDE));

        break;
      case CEXPR_OP_MODULUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_MODULUS));

        break;
      case CEXPR_OP_PLUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_PLUS));

        break;
      case CEXPR_OP_MINUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_MINUS));

        break;
      case CEXPR_OP_BIT_LSHIFT_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_LSHIFT));

        break;
      case CEXPR_OP_BIT_RSHIFT_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_RSHIFT));

        break;
      case CEXPR_OP_BIT_AND_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_AND));

        break;
      case CEXPR_OP_BIT_XOR_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_XOR));

        break;
      case CEXPR_OP_BIT_OR_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_OR));

        break;
      default:
        break;
    }

    stackOperator(CExprInst->getOperator(CEXPR_OP_EQUALS));
  }
  else
    compileConditionalExpression(itoken->getChild(0));
}

/*
 * <conditional_expression>:= <logical_or_expression>
 * <conditional_expression>:= <logical_or_expression> ?
 *                            <expression> : <conditional_expression>
 */

void
CExprCompileImpl::
compileConditionalExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 5) {
    compileConditionalExpression(itoken->getChild(4));

    compileExpression(itoken->getChild(2));

    compileLogicalOrExpression(itoken->getChild(0));

    stackOperator(CExprInst->getOperator(CEXPR_OP_QUESTION));
  }
  else
    compileLogicalOrExpression(itoken->getChild(0));
}

/*
 * <logical_or_expression>:= <logical_and_expression>
 * <logical_or_expression>:= <logical_or_expression> || <logical_and_expression>
 */

void
CExprCompileImpl::
compileLogicalOrExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileLogicalOrExpression(itoken->getChild(0));

    compileLogicalAndExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_LOGICAL_OR));
  }
  else
    compileLogicalAndExpression(itoken->getChild(0));
}

/*
 * <logical_and_expression>:= <inclusive_or_expression>
 * <logical_and_expression>:= <logical_and_expression> && <inclusive_or_expression>
 */

void
CExprCompileImpl::
compileLogicalAndExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileLogicalAndExpression(itoken->getChild(0));

    compileInclusiveOrExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_LOGICAL_AND));
  }
  else
    compileInclusiveOrExpression(itoken->getChild(0));
}

/*
 * <inclusive_or_expression>:= <exclusive_or_expression>
 * <inclusive_or_expression>:= <inclusive_or_expression> | <exclusive_or_expression>
 */

void
CExprCompileImpl::
compileInclusiveOrExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileInclusiveOrExpression(itoken->getChild(0));

    compileExclusiveOrExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_OR));
  }
  else
    compileExclusiveOrExpression(itoken->getChild(0));
}

/*
 * <exclusive_or_expression>:= <and_expression>
 * <exclusive_or_expression>:= <exclusive_or_expression> ^ <and_expression>
 */

void
CExprCompileImpl::
compileExclusiveOrExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileExclusiveOrExpression(itoken->getChild(0));

    compileAndExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_XOR));
  }
  else
    compileAndExpression(itoken->getChild(0));
}

/*
 * <and_expression>:= <equality_expression>
 * <and_expression>:= <and_expression> & <equality_expression>
 */

void
CExprCompileImpl::
compileAndExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileAndExpression(itoken->getChild(0));

    compileEqualityExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_AND));
  }
  else
    compileEqualityExpression(itoken->getChild(0));
}

/*
 * <equality_expression>:= <relational_expression>
 * <equality_expression>:= <equality_expression> == <relational_expression>
 * <equality_expression>:= <equality_expression> != <relational_expression>
 * <equality_expression>:= <equality_expression> ~= <relational_expression>
 */

void
CExprCompileImpl::
compileEqualityExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    if      (op->getType() == CEXPR_OP_EQUAL) {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackOperator(CExprInst->getOperator(CEXPR_OP_EQUAL));
    }
    else if (op->getType() == CEXPR_OP_NOT_EQUAL) {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackOperator(CExprInst->getOperator(CEXPR_OP_NOT_EQUAL));
    }
    else {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackOperator(CExprInst->getOperator(CEXPR_OP_APPROX_EQUAL));
    }
  }
  else
    compileRelationalExpression(itoken->getChild(0));
}

/*
 * <relational_expression>:= <shift_expression>
 * <relational_expression>:= <relational_expression> <  <shift_expression>
 * <relational_expression>:= <relational_expression> >  <shift_expression>
 * <relational_expression>:= <relational_expression> <= <shift_expression>
 * <relational_expression>:= <relational_expression> >= <shift_expression>
 */

void
CExprCompileImpl::
compileRelationalExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileRelationalExpression(itoken->getChild(0));

    compileShiftExpression(itoken->getChild(2));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    stackOperator(op);
  }
  else
    compileShiftExpression(itoken->getChild(0));
}

/*
 * <shift_expression>:= <additive_expression>
 * <shift_expression>:= <shift_expression> << <additive_expression>
 * <shift_expression>:= <shift_expression> >> <additive_expression>
 */

void
CExprCompileImpl::
compileShiftExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileShiftExpression(itoken->getChild(0));

    compileAdditiveExpression(itoken->getChild(2));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    if (op->getType() == CEXPR_OP_BIT_LSHIFT)
      stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_LSHIFT));
    else
      stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_RSHIFT));
  }
  else
    compileAdditiveExpression(itoken->getChild(0));
}

/*
 * <additive_expression>:= <multiplicative_expression>
 * <additive_expression>:= <additive_expression> +
 *                         <multiplicative_expression>
 * <additive_expression>:= <additive_expression> -
 *                         <multiplicative_expression>
 */
#ifdef GNUPLOT_EXPR
/*
 * <additive_expression>:= <additive_expression> .
 *                         <multiplicative_expression>
 */
#endif

void
CExprCompileImpl::
compileAdditiveExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileAdditiveExpression(itoken->getChild(0));

    compileMultiplicativeExpression(itoken->getChild(2));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    if      (op->getType() == CEXPR_OP_PLUS)
      stackOperator(CExprInst->getOperator(CEXPR_OP_PLUS ));
    else if (op->getType() == CEXPR_OP_MINUS)
      stackOperator(CExprInst->getOperator(CEXPR_OP_MINUS));
#ifdef GNUPLOT_EXPR
    else if (op->getType() == CEXPR_OP_CONCAT)
      stackOperator(CExprInst->getOperator(CEXPR_OP_CONCAT));
#endif
  }
  else
    compileMultiplicativeExpression(itoken->getChild(0));
}

/*
 * <multiplicative_expression>:= <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> *
 *                               <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> /
 *                               <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> %
 *                               <power_expression>
 */

void
CExprCompileImpl::
compileMultiplicativeExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileMultiplicativeExpression(itoken->getChild(0));

    compilePowerExpression(itoken->getChild(2));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOperatorPtr op = itoken1->getOperator();

    if      (op->getType() == CEXPR_OP_TIMES)
      stackOperator(CExprInst->getOperator(CEXPR_OP_TIMES));
    else if (op->getType() == CEXPR_OP_DIVIDE)
      stackOperator(CExprInst->getOperator(CEXPR_OP_DIVIDE));
    else
      stackOperator(CExprInst->getOperator(CEXPR_OP_MODULUS));
  }
  else
    compilePowerExpression(itoken->getChild(0));
}

/*
 * <power_expression>:= <unary_expression>
 * <power_expression>:= <unary_expression> ** <power_expression>
 */

void
CExprCompileImpl::
compilePowerExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileUnaryExpression(itoken->getChild(0));

    compilePowerExpression(itoken->getChild(2));

    stackOperator(CExprInst->getOperator(CEXPR_OP_POWER));
  }
  else
    compileUnaryExpression(itoken->getChild(0));
}

/*
 * <unary_expression>:= <postfix_expression>
 * <unary_expression>:= ++ <unary_expression>
 * <unary_expression>:= -- <unary_expression>
 * <unary_expression>:= +  <unary_expression>
 * <unary_expression>:= -  <unary_expression>
 * <unary_expression>:= ~  <unary_expression>
 * <unary_expression>:= !  <unary_expression>
 */

void
CExprCompileImpl::
compileUnaryExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 2) {
    CExprITokenPtr itoken0 = itoken->getChild(0);

    CExprOperatorPtr op = itoken0->getOperator();

    switch (op->getType()) {
      case CEXPR_OP_INCREMENT:
        compileUnaryExpression(itoken->getChild(1));

        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_PLUS));

        stackOperator(CExprInst->getOperator(CEXPR_OP_EQUALS));

        break;
      case CEXPR_OP_DECREMENT:
        compileUnaryExpression(itoken->getChild(1));

        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_MINUS));

        stackOperator(CExprInst->getOperator(CEXPR_OP_EQUALS));

        break;
      case CEXPR_OP_PLUS:
        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_UNARY_PLUS));

        break;
      case CEXPR_OP_MINUS:
        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_UNARY_MINUS));

        break;
      case CEXPR_OP_BIT_NOT:
        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_BIT_NOT));

        break;
      case CEXPR_OP_LOGICAL_NOT:
        compileUnaryExpression(itoken->getChild(1));

        stackOperator(CExprInst->getOperator(CEXPR_OP_LOGICAL_NOT));

        break;
      default:
        break;
    }
  }
  else
    compilePostfixExpression(itoken->getChild(0));
}

/*
 * <postfix_expression>:= <primary_expression>
#ifdef GNUPLOT_EXPR
 * <postfix_expression>:= <identifier> [ <expression> ]
#endif
 * <postfix_expression>:= <identifier> ( <argument_expression_list>(opt) )
 * <postfix_expression>:= <postfix_expression> ++
 * <postfix_expression>:= <postfix_expression> --
 */

void
CExprCompileImpl::
compilePostfixExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 1) {
    compilePrimaryExpression(itoken->getChild(0));

    return;
  }

  CExprITokenPtr itoken1 = itoken->getChild(1);

  CExprOperatorPtr op = itoken1->getOperator();

  if      (op->getType() == CEXPR_OP_OPEN_RBRACKET) {
    stackOperator(op);

    uint num_args = 0;

    if (num_children == 4) {
      num_args = itoken->getChild(2)->
        countITokenChildrenOfType(CEXPR_ASSIGNMENT_EXPRESSION);

      compileArgumentExpressionList(itoken->getChild(2));
    }

    CExprITokenPtr itoken0 = itoken->getChild(0);

    const std::string &identifier = itoken0->getIdentifier();

    CExprFunctionPtr function = CExprInst->getFunction(identifier);

    if (! function.isValid()) {
      setLastError("Invalid Function");
      return;
    }

    uint function_num_args = function->numArgs();

    for (uint i = num_args; i < function_num_args; ++i) {
      if (! (function->argType(i) & CEXPR_VALUE_NULL))
        break;

      stackValue(CExprValuePtr());

      ++num_args;
    }

    if (function->isVariableArgs()) {
      if (function_num_args > num_args) {
        setLastError("Function called with too few arguments");
        return;
      }
    }
    else {
      if (function_num_args != num_args) {
        setLastError("Function called with wrong number of arguments");
        return;
      }
    }

    stackFunction(function);
  }
#ifdef GNUPLOT_EXPR
  else if (op->getType() == CEXPR_OP_OPEN_SBRACKET) {
    // <var> [ <ind> ]
    if (num_children == 4) {
      CExprITokenPtr itoken0 = itoken->getChild(0);

      const std::string &identifier = itoken0->getIdentifier();

      stackIdentifier(identifier);

      stackOperator(CExprInst->getOperator(CEXPR_OP_OPEN_SBRACKET));

      compileExpression(itoken->getChild(2));

      stackOperator(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));
    }
    // <var> [ <ind> : <ind> ]
    else {
      CExprITokenPtr itoken0 = itoken->getChild(0);

      const std::string &identifier = itoken0->getIdentifier();

      stackIdentifier(identifier);

      stackOperator(CExprInst->getOperator(CEXPR_OP_OPEN_SBRACKET));

      compileExpression(itoken->getChild(2));
      compileExpression(itoken->getChild(4));

      stackOperator(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));
    }
  }
#endif
  else if (op->getType() == CEXPR_OP_INCREMENT) {
    compilePostfixExpression(itoken->getChild(0));

    compilePostfixExpression(itoken->getChild(0));

    stackOperator(CExprInst->getOperator(CEXPR_OP_EQUALS));

    stackOperator(CExprInst->getOperator(CEXPR_OP_PLUS));
  }
  else if (op->getType() == CEXPR_OP_DECREMENT) {
    compilePostfixExpression(itoken->getChild(0));

    compilePostfixExpression(itoken->getChild(0));

    stackOperator(CExprInst->getOperator(CEXPR_OP_EQUALS));

    stackOperator(CExprInst->getOperator(CEXPR_OP_MINUS));
  }
}

/*
 * <primary_expression>:= <integer>
 * <primary_expression>:= <real>
 * <primary_expression>:= <string>
 * <primary_expression>:= <identifier>
 * <primary_expression>:= ( <expression> )
 */

void
CExprCompileImpl::
compilePrimaryExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileExpression(itoken->getChild(1));
  }
  else {
    switch (itoken->getChild(0)->getType()) {
      case CEXPR_ITOKEN_INTEGER:
        compileInteger(itoken->getChild(0));

        break;
      case CEXPR_ITOKEN_REAL:
        compileReal(itoken->getChild(0));

        break;
      case CEXPR_ITOKEN_STRING:
        compileString(itoken->getChild(0));

        break;
      case CEXPR_ITOKEN_IDENTIFIER:
        compileIdentifier(itoken->getChild(0));

        break;
      default:
        break;
    }
  }
}

/*
 * <argument_expression_list>:= <assignment_expression>
 * <argument_expression_list>:= <argument_expression_list> , <assignment_expression>
 */

void
CExprCompileImpl::
compileArgumentExpressionList(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileArgumentExpressionList(itoken->getChild(0));

    compileAssignmentExpression(itoken->getChild(2));
  }
  else
    compileAssignmentExpression(itoken->getChild(0));
}

void
CExprCompileImpl::
compileIdentifier(CExprITokenPtr itoken)
{
  stackIdentifier(itoken->getIdentifier());
}

void
CExprCompileImpl::
compileOperator(CExprITokenPtr itoken)
{
  stackOperator(itoken->getOperator());
}

void
CExprCompileImpl::
compileInteger(CExprITokenPtr itoken)
{
  stackInteger(itoken->getInteger());
}

void
CExprCompileImpl::
compileReal(CExprITokenPtr itoken)
{
  stackReal(itoken->getReal());
}

void
CExprCompileImpl::
compileString(CExprITokenPtr itoken)
{
  stackString(itoken->getString());
}

#if 0
void
CExprCompileImpl::
compileITokenChildren(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  for (uint i = 0; i < num_children; i++)
    compileIToken1(itoken->getChild(i));
}
#endif

void
CExprCompileImpl::
stackIdentifier(const std::string &identifier)
{
  CExprCTokenPtr ctoken = CExprCToken::createIdentifierToken(identifier);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackOperator(CExprOperatorPtr op)
{
  CExprCTokenPtr ctoken = CExprCToken::createOperatorToken(op);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackInteger(long integer)
{
  CExprCTokenPtr ctoken = CExprCToken::createIntegerToken(integer);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackReal(double real)
{
  CExprCTokenPtr ctoken = CExprCToken::createRealToken(real);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackString(const std::string &str)
{
  CExprCTokenPtr ctoken = CExprCToken::createStringToken(str);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackFunction(CExprFunctionPtr function)
{
  CExprCTokenPtr ctoken = CExprCToken::createFunctionToken(function);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
stackValue(CExprValuePtr value)
{
  CExprCTokenPtr ctoken = CExprCToken::createValueToken(value);

  ctoken_stack_.addToken(ctoken);
}

void
CExprCompileImpl::
setLastError(const std::string &message)
{
  if (message == "") {
    last_error_message = "";

    error_flag = false;

    return;
  }

  if (error_flag)
    return;

  last_error_message = message;

  error_flag = true;
}

bool
CExprCompileImpl::
isError()
{
  return error_flag;
}

std::string
CExprCompileImpl::
getLastError()
{
  return last_error_message;
}

//------

void
CExprCToken::
print(std::ostream &os) const
{
  switch (type_) {
    case CEXPR_CTOKEN_IDENTIFIER:
      os << "<identifier>";
      break;
    case CEXPR_CTOKEN_OPERATOR:
      os << "<operator>";
      break;
    case CEXPR_CTOKEN_INTEGER:
      os << "<integer>";
      break;
    case CEXPR_CTOKEN_REAL:
      os << "<real>";
      break;
    case CEXPR_CTOKEN_STRING:
      os << "<string>";
      break;
    case CEXPR_CTOKEN_FUNCTION:
      os << "<function>";
      break;
    case CEXPR_CTOKEN_VALUE:
      os << "<value>";
      break;
    default:
      os << "<-?->";
      break;
  }

  base_->print(os);

  os << " ";
}

//------

void
CExprCTokenStack::
print(std::ostream &os) const
{
  uint len = stack_.size();

  for (uint i = 0; i < len; ++i) {
    if (i > 0) os << " ";

    stack_[i]->print(os);
  }
}
