#include <CExprI.h>

class CExprCompileImpl {
 public:
  CExprTokenStack compileIToken(CExprITokenPtr itoken);

 private:
  void compileIToken1                 (CExprITokenPtr itoken);
  void compileExpression              (CExprITokenPtr itoken);
  void compileAssignmentExpression    (CExprITokenPtr itoken);
  void compileConditionalExpression   (CExprITokenPtr itoken);
  void compileLogicalOrExpression     (CExprITokenPtr itoken);
  void compileLogicalAndExpression    (CExprITokenPtr itoken);
  void compileInclusiveOrExpression   (CExprITokenPtr itoken);
  void compileExclusiveOrExpression   (CExprITokenPtr itoken);
  void compileAndExpression           (CExprITokenPtr itoken);
  void compileEqualityExpression      (CExprITokenPtr itoken);
  void compileRelationalExpression    (CExprITokenPtr itoken);
  void compileShiftExpression         (CExprITokenPtr itoken);
  void compileAdditiveExpression      (CExprITokenPtr itoken);
  void compileMultiplicativeExpression(CExprITokenPtr itoken);
  void compileUnaryExpression         (CExprITokenPtr itoken);
  void compilePowerExpression         (CExprITokenPtr itoken);
  void compilePostfixExpression       (CExprITokenPtr itoken);
  void compilePrimaryExpression       (CExprITokenPtr itoken);
  void compileArgumentExpressionList  (CExprITokenPtr itoken);

  void compileIdentifier(CExprITokenPtr itoken);
  void compileOperator  (CExprITokenPtr itoken);
  void compileInteger   (CExprITokenPtr itoken);
  void compileReal      (CExprITokenPtr itoken);
  void compileString    (CExprITokenPtr itoken);
  void compileComplex   (CExprITokenPtr itoken);
  void compileValue     (CExprITokenPtr itoken);
#if 0
  void compileITokenChildren(CExprITokenPtr itoken);
#endif

  void stackFunction  (CExprFunctionPtr function);
  void stackDummyValue();
  void stackCToken    (const CExprTokenBaseP &base);

 private:
  CExprTokenStack tokenStack_;
  CExprErrorData  errorData_;
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

CExprTokenStack
CExprCompile::
compileIToken(CExprITokenPtr itoken)
{
  return impl_->compileIToken(itoken);
}

//------

CExprTokenStack
CExprCompileImpl::
compileIToken(CExprITokenPtr itoken)
{
  tokenStack_.clear();

  if (! itoken.isValid())
    return tokenStack_;

  errorData_.setLastError("");

  compileIToken1(itoken);

  if (errorData_.isError()) {
    CExprInst->errorMsg(errorData_.getLastError());
    return CExprTokenStack();
  }

  return tokenStack_;
}

void
CExprCompileImpl::
compileIToken1(CExprITokenPtr itoken)
{
  switch (itoken->getIType()) {
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
    case CEXPR_TOKEN_TYPE: {
      switch (itoken->getType()) {
        case CEXPR_TOKEN_IDENTIFIER:
          compileIdentifier(itoken);

          break;
        case CEXPR_TOKEN_OPERATOR:
          compileOperator(itoken);

          break;
        case CEXPR_TOKEN_INTEGER:
          compileInteger(itoken);

          break;
        case CEXPR_TOKEN_REAL:
          compileReal(itoken);

          break;
        case CEXPR_TOKEN_STRING:
          compileString(itoken);

          break;
        case CEXPR_TOKEN_COMPLEX:
          compileComplex(itoken);

          break;
        default:
          assert(false);
          break;
      }
      break;
    }
    default:
      assert(false);
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

  if      (num_children == 3) {
    compileExpression(itoken->getChild(0));

    compileAssignmentExpression(itoken->getChild(2));

    stackCToken(itoken->getChild(1)->base());
  }
  else if (num_children == 1)
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

    CExprOpType op = itoken1->getOperator();

    switch (op) {
      case CEXPR_OP_EQUALS:
        compileAssignmentExpression(itoken->getChild(2));

        break;
      case CEXPR_OP_TIMES_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_TIMES));

        break;
      case CEXPR_OP_DIVIDE_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_DIVIDE));

        break;
      case CEXPR_OP_MODULUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_MODULUS));

        break;
      case CEXPR_OP_PLUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_PLUS));

        break;
      case CEXPR_OP_MINUS_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_MINUS));

        break;
      case CEXPR_OP_BIT_LSHIFT_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_LSHIFT));

        break;
      case CEXPR_OP_BIT_RSHIFT_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_RSHIFT));

        break;
      case CEXPR_OP_BIT_AND_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_AND));

        break;
      case CEXPR_OP_BIT_XOR_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_XOR));

        break;
      case CEXPR_OP_BIT_OR_EQUALS:
        compileUnaryExpression(itoken->getChild(0));

        compileAssignmentExpression(itoken->getChild(2));

        stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_OR));

        break;
      default:
        assert(false);
        break;
    }

    stackCToken(CExprInst->getOperator(CEXPR_OP_EQUALS));
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
    // 0 boolean, 2 = lhs, 4 = rhs

    // stack lhs expression
    stackCToken(CExprInst->getOperator(CEXPR_OP_START_BLOCK));

    compileExpression(itoken->getChild(2));

    stackCToken(CExprInst->getOperator(CEXPR_OP_END_BLOCK));

    //---

    // stack lhs expression
    stackCToken(CExprInst->getOperator(CEXPR_OP_START_BLOCK));

    compileConditionalExpression(itoken->getChild(4));

    stackCToken(CExprInst->getOperator(CEXPR_OP_END_BLOCK));

    //---

    // stack conditional
    compileLogicalOrExpression(itoken->getChild(0));

    stackCToken(CExprInst->getOperator(CEXPR_OP_QUESTION));
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

    stackCToken(CExprInst->getOperator(CEXPR_OP_LOGICAL_OR));
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

    stackCToken(CExprInst->getOperator(CEXPR_OP_LOGICAL_AND));
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

    stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_OR));
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

    stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_XOR));
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

    stackCToken(CExprInst->getOperator(CEXPR_OP_BIT_AND));
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

    CExprOpType op = itoken1->getOperator();

    if      (op == CEXPR_OP_EQUAL || op == CEXPR_OP_STR_EQUAL) {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackCToken(itoken1->base());
    }
    else if (op == CEXPR_OP_NOT_EQUAL || op == CEXPR_OP_STR_NOT_EQUAL) {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackCToken(itoken1->base());
    }
    else {
      compileEqualityExpression(itoken->getChild(0));

      compileRelationalExpression(itoken->getChild(2));

      stackCToken(CExprInst->getOperator(CEXPR_OP_APPROX_EQUAL));
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

    stackCToken(itoken1->base());
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

    CExprOpType op = itoken1->getOperator();

    if (op == CEXPR_OP_BIT_LSHIFT)
      stackCToken(itoken1->base());
    else
      stackCToken(itoken1->base());
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

    CExprOpType op = itoken1->getOperator();

    if      (op == CEXPR_OP_PLUS)
      stackCToken(itoken1->base());
    else if (op == CEXPR_OP_MINUS)
      stackCToken(itoken1->base());
#ifdef GNUPLOT_EXPR
    else if (op == CEXPR_OP_STR_CONCAT)
      stackCToken(itoken1->base());
#endif
  }
  else
    compileMultiplicativeExpression(itoken->getChild(0));
}

/*
 * <multiplicative_expression>:= <unary_expression>
 * <multiplicative_expression>:= <multiplicative_expression> * <unary_expression>
 * <multiplicative_expression>:= <multiplicative_expression> / <unary_expression>
 * <multiplicative_expression>:= <multiplicative_expression> % <unary_expression>
 */

void
CExprCompileImpl::
compileMultiplicativeExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compileMultiplicativeExpression(itoken->getChild(0));

    compileUnaryExpression(itoken->getChild(2));

    CExprITokenPtr itoken1 = itoken->getChild(1);

    CExprOpType op = itoken1->getOperator();

    if      (op == CEXPR_OP_TIMES)
      stackCToken(itoken1->base());
    else if (op == CEXPR_OP_DIVIDE)
      stackCToken(itoken1->base());
    else
      stackCToken(itoken1->base());
  }
  else
    compileUnaryExpression(itoken->getChild(0));
}

/*
 * <unary_expression>:= <power_expression>
 * <unary_expression>:= ++ <unary_expression>
 * <unary_expression>:= -- <unary_expression>
 * <unary_expression>:= +  <unary_expression>
 * <unary_expression>:= -  <unary_expression>
 * <unary_expression>:= ~  <unary_expression>
 * <unary_expression>:= !  <unary_expression>
 * <unary_expression>:= <power_expression> !
 */

void
CExprCompileImpl::
compileUnaryExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 2) {
    CExprITokenPtr itoken0 = itoken->getChild(0);

    if (itoken0->base().isValid()) {
      CExprOpType op = itoken0->getOperator();

      switch (op) {
        case CEXPR_OP_INCREMENT:
          compileUnaryExpression(itoken->getChild(1));

          compileUnaryExpression(itoken->getChild(1));

          stackCToken(CExprInst->getOperator(CEXPR_OP_PLUS));
          stackCToken(CExprInst->getOperator(CEXPR_OP_EQUALS));

          break;
        case CEXPR_OP_DECREMENT:
          compileUnaryExpression(itoken->getChild(1));

          compileUnaryExpression(itoken->getChild(1));

          stackCToken(CExprInst->getOperator(CEXPR_OP_MINUS));
          stackCToken(CExprInst->getOperator(CEXPR_OP_EQUALS));

          break;
        case CEXPR_OP_PLUS:
          compileUnaryExpression(itoken->getChild(1));

          stackCToken(CExprInst->getOperator(CEXPR_OP_UNARY_PLUS));

          break;
        case CEXPR_OP_MINUS:
          compileUnaryExpression(itoken->getChild(1));

          stackCToken(CExprInst->getOperator(CEXPR_OP_UNARY_MINUS));

          break;
        case CEXPR_OP_BIT_NOT:
          compileUnaryExpression(itoken->getChild(1));

          stackCToken(itoken0->base());

          break;
        case CEXPR_OP_LOGICAL_NOT:
          compileUnaryExpression(itoken->getChild(1));

          stackCToken(itoken0->base());

          break;
        default:
          assert(false);
          break;
      }
    }
    else {
      compilePowerExpression(itoken->getChild(0));

      CExprITokenPtr itoken1 = itoken->getChild(1);

      stackCToken(itoken1->base());
    }
  }
  else
    compilePowerExpression(itoken->getChild(0));
}

/*
 * <power_expression>:= <postfix_expression>
 * <power_expression>:= <postfix_expression> ** <power_expression>
 */

void
CExprCompileImpl::
compilePowerExpression(CExprITokenPtr itoken)
{
  uint num_children = itoken->getNumChildren();

  if (num_children == 3) {
    compilePostfixExpression(itoken->getChild(0));

    compilePowerExpression(itoken->getChild(2));

    stackCToken(CExprInst->getOperator(CEXPR_OP_POWER));
  }
  else
    compilePostfixExpression(itoken->getChild(0));
}

/*
 * <postfix_expression>:= <primary_expression>
#ifdef GNUPLOT_EXPR
 * <postfix_expression>:= <primary_expression> [ <expression> ]
 * <postfix_expression>:= <primary_expression> [ <expression> : <expression> ]
#endif
 * <postfix_expression>:= <identifier> ( <argument_expression_list>(opt) )
 * <postfix_expression>:= <postfix_expression> ++
 * <postfix_expression>:= <postfix_expression> --
 */

void
CExprCompileImpl::
compilePostfixExpression(CExprITokenPtr itoken)
{
  CExprITokenPtr itoken0 = itoken->getChild(0);

  uint num_children = itoken->getNumChildren();

  if (num_children == 1) {
    compilePrimaryExpression(itoken0);
    return;
  }

  CExprITokenPtr itoken1 = itoken->getChild(1);

  CExprOpType op = itoken1->getOperator();

  if      (num_children == 4 && itoken0->getIType() == CEXPR_PRIMARY_EXPRESSION &&
           op == CEXPR_OP_OPEN_SBRACKET) {
    compilePrimaryExpression(itoken0);

    stackCToken(CExprInst->getOperator(CEXPR_OP_OPEN_SBRACKET));

    if (itoken->getChild(2)->getIType() != CEXPR_DUMMY_EXPRESSION)
      compileExpression(itoken->getChild(2));
    else
      stackCToken(CExprInst->getOperator(CEXPR_OP_UNKNOWN));

    stackCToken(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));

    return;
  }
  else if (num_children == 6 && itoken0->getIType() == CEXPR_PRIMARY_EXPRESSION &&
           op == CEXPR_OP_OPEN_SBRACKET) {
    compilePrimaryExpression(itoken0);

    stackCToken(CExprInst->getOperator(CEXPR_OP_OPEN_SBRACKET));

    if (itoken->getChild(2)->getIType() != CEXPR_DUMMY_EXPRESSION)
      compileExpression(itoken->getChild(2));
    else
      stackCToken(CExprInst->getOperator(CEXPR_OP_UNKNOWN));

    if (itoken->getChild(4)->getIType() != CEXPR_DUMMY_EXPRESSION)
      compileExpression(itoken->getChild(4));
    else
      stackCToken(CExprInst->getOperator(CEXPR_OP_UNKNOWN));

    stackCToken(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));

    return;
  }

  //----

  if      (op == CEXPR_OP_OPEN_RBRACKET) {
    stackCToken(itoken1->base());

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
      errorData_.setLastError("Invalid Function '" + identifier + "'");
      return;
    }

    uint function_num_args = function->numArgs();

    for (uint i = num_args; i < function_num_args; ++i) {
      if (! (function->argType(i) & CEXPR_VALUE_NULL))
        break;

      stackDummyValue();

      ++num_args;
    }

    if (function->isVariableArgs()) {
      if (function_num_args > num_args) {
        errorData_.setLastError("Function called with too few arguments");
        return;
      }
    }
    else {
      if (function_num_args != num_args) {
        errorData_.setLastError("Function called with wrong number of arguments");
        return;
      }
    }

    stackFunction(function);
  }
#ifdef GNUPLOT_EXPR
  else if (op == CEXPR_OP_OPEN_SBRACKET) {
    // <var> [ <ind> ]
    if (num_children == 4) {
      CExprITokenPtr itoken0 = itoken->getChild(0);

      stackCToken(itoken0->base());

      stackCToken(itoken1->base());

      compileExpression(itoken->getChild(2));

      stackCToken(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));
    }
    // <var> [ <ind> : <ind> ]
    else {
      CExprITokenPtr itoken0 = itoken->getChild(0);

      stackCToken(itoken0->base());

      stackCToken(itoken1->base());

      if (itoken->getChild(2)->getIType() != CEXPR_DUMMY_EXPRESSION)
        compileExpression(itoken->getChild(2));
      else
        stackCToken(CExprInst->getOperator(CEXPR_OP_UNKNOWN));

      if (itoken->getChild(4)->getIType() != CEXPR_DUMMY_EXPRESSION)
        compileExpression(itoken->getChild(4));
      else
        stackCToken(CExprInst->getOperator(CEXPR_OP_UNKNOWN));

      stackCToken(CExprInst->getOperator(CEXPR_OP_CLOSE_SBRACKET));
    }
  }
#endif
  else if (op == CEXPR_OP_INCREMENT) {
    compilePostfixExpression(itoken->getChild(0));
    compilePostfixExpression(itoken->getChild(0));

    stackCToken(CExprInst->getOperator(CEXPR_OP_EQUALS));
    stackCToken(CExprInst->getOperator(CEXPR_OP_PLUS));
  }
  else if (op == CEXPR_OP_DECREMENT) {
    compilePostfixExpression(itoken->getChild(0));
    compilePostfixExpression(itoken->getChild(0));

    stackCToken(CExprInst->getOperator(CEXPR_OP_EQUALS));
    stackCToken(CExprInst->getOperator(CEXPR_OP_MINUS));
  }
}

/*
 * <primary_expression>:= <integer>
 * <primary_expression>:= <real>
 * <primary_expression>:= <string>
 * <primary_expression>:= <complex>
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
      case CEXPR_TOKEN_INTEGER:
        compileInteger(itoken->getChild(0));

        break;
      case CEXPR_TOKEN_REAL:
        compileReal(itoken->getChild(0));

        break;
      case CEXPR_TOKEN_STRING:
        compileString(itoken->getChild(0));

        break;
      case CEXPR_TOKEN_COMPLEX:
        compileComplex(itoken->getChild(0));

        break;
      case CEXPR_TOKEN_IDENTIFIER:
        compileIdentifier(itoken->getChild(0));

        break;
      default:
        assert(false);
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
  stackCToken(itoken->base());
}

void
CExprCompileImpl::
compileOperator(CExprITokenPtr itoken)
{
  stackCToken(itoken->base());
}

void
CExprCompileImpl::
compileInteger(CExprITokenPtr itoken)
{
  CExprValuePtr value = CExprInst->createIntegerValue(itoken->getInteger());

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackCToken(base);
}

void
CExprCompileImpl::
compileReal(CExprITokenPtr itoken)
{
  CExprValuePtr value = CExprInst->createRealValue(itoken->getReal());

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackCToken(base);
}

void
CExprCompileImpl::
compileString(CExprITokenPtr itoken)
{
  CExprValuePtr value = CExprInst->createStringValue(itoken->getString());

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackCToken(base);
}

void
CExprCompileImpl::
compileComplex(CExprITokenPtr itoken)
{
  CExprValuePtr value = CExprInst->createComplexValue(itoken->getComplex());

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackCToken(base);
}

void
CExprCompileImpl::
compileValue(CExprITokenPtr itoken)
{
  stackCToken(itoken->base());
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
stackFunction(CExprFunctionPtr function)
{
  CExprTokenBaseP base(CExprTokenMgrInst->createFunctionToken(function));

  stackCToken(base);
}

void
CExprCompileImpl::
stackDummyValue()
{
  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(CExprValuePtr()));

  stackCToken(base);
}

void
CExprCompileImpl::
stackCToken(const CExprTokenBaseP &base)
{
  tokenStack_.addToken(base);
}
