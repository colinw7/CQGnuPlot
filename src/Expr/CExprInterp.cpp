#include <CExprI.h>

#ifdef CEXPR_DEBUG
#  define DEBUG_ENTER(a) Trace(a)
#  define DEBUG_PRINT(a) CExprPrintTypeIToken(a)
#else
#  define DEBUG_ENTER(a)
#  define DEBUG_PRINT(a)
#endif

class Trace {
 private:
  std::string name_;

 public:
  Trace(const std::string &name) : name_(name) {
    std::cerr << ">" << name_ << std::endl;
  }

 ~Trace() {
    std::cerr << "<" << name_ << std::endl;
  }
};

class CExprInterpImpl {
 private:
  CExprPTokenStack ptoken_stack_;
  CExprITokenStack itoken_stack_;
  bool             error_flag;
  std::string      last_error_message;

 public:
  CExprITokenPtr interpStack(const CExprPTokenStack &stack);

 private:
  CExprITokenPtr readExpression();
  CExprITokenPtr readAssignmentExpression();
  CExprITokenPtr readConditionalExpression();
  CExprITokenPtr readLogicalOrExpression();
  CExprITokenPtr readLogicalAndExpression();
  CExprITokenPtr readInclusiveOrExpression();
  CExprITokenPtr readExclusiveOrExpression();
  CExprITokenPtr readAndExpression();
  CExprITokenPtr readEqualityExpression();
  CExprITokenPtr readRelationalExpression();
  CExprITokenPtr readShiftExpression();
  CExprITokenPtr readAdditiveExpression();
  CExprITokenPtr readMultiplicativeExpression();
  CExprITokenPtr readPowerExpression();
  CExprITokenPtr readUnaryExpression();
  CExprITokenPtr readPostfixExpression();
  CExprITokenPtr readPrimaryExpression();
  CExprITokenPtr readArgumentExpressionList();
  CExprITokenPtr readITokenOfType(CExprITokenType);
  bool           isLastToken();
  void           addITokenToType(CExprITokenPtr, CExprITokenPtr);
  CExprITokenPtr collapseITokenToType(CExprITokenPtr, CExprITokenType);
  void           deleteCollapsedIToken(CExprITokenPtr, CExprITokenPtr);
  bool           isOperatorIToken(CExprITokenPtr, CExprOpType);
  bool           isIdentifierIToken(CExprITokenPtr);
  bool           isIntegerIToken(CExprITokenPtr);
  bool           isRealIToken(CExprITokenPtr);
  bool           isStringIToken(CExprITokenPtr);
  void           stackIToken(CExprITokenPtr);
  CExprITokenPtr unstackIToken();
  void           printTrackBack(std::ostream &os);
  #ifdef CEXPR_DEBUG
  void           printTypeIToken(std::ostream &os, CExprITokenPtr);
  void           printTypeIToken1(std::ostream &os, CExprITokenPtr);
  std::string    tokenToString(CExprITokenPtr itoken);
  void           concatITokenString(std::string &, CExprITokenPtr);
  #endif
  void           setLastError(const std::string &str);
  bool           isError();
  std::string    getLastError();
};

//-----------

CExprInterp::
CExprInterp()
{
  impl_ = new CExprInterpImpl;
}

CExprInterp::
~CExprInterp()
{
}

CExprITokenPtr
CExprInterp::
interpPTokenStack(const CExprPTokenStack &stack)
{
  return impl_->interpStack(stack);
}

//-----------

CExprITokenPtr
CExprInterpImpl::
interpStack(const CExprPTokenStack &stack)
{
  setLastError("");

  if (stack.getNumTokens() == 0)
    return CExprITokenPtr();

  ptoken_stack_ = stack;

  itoken_stack_.clear();

  CExprITokenPtr itoken = readExpression();

  if (! isLastToken()) {
    stackIToken(itoken);

    printTrackBack(std::cerr);

    itoken = CExprITokenPtr();
  }

  itoken_stack_.clear();

  return itoken;
}

/*
 * <expression>:= <assignment_expression>
 * <expression>:= <expression> , <assignment_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readExpression()
{
  DEBUG_ENTER("readExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readAssignmentExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_COMMA)) {
      CExprITokenPtr itoken2 = readAssignmentExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else {
        setLastError("Missing assignment expression after comma");
        break;
      }

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);

    if (isError()) {
      stackIToken(itoken);

      itoken = CExprITokenPtr();
    }
  }

  DEBUG_PRINT(itoken);

  return itoken;
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

CExprITokenPtr
CExprInterpImpl::
readAssignmentExpression()
{
  DEBUG_ENTER("readAssignmentExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_ASSIGNMENT_EXPRESSION);

  if (itoken.isValid())
    return itoken;

  CExprITokenPtr itoken1 = readConditionalExpression();

  if (itoken1.isValid()) {
    CExprITokenPtr itoken2 = collapseITokenToType(itoken1, CEXPR_UNARY_EXPRESSION);

    if (itoken2.isValid()) {
      CExprITokenPtr itoken3 = unstackIToken();

      if (isOperatorIToken(itoken3, CEXPR_OP_EQUALS           ) ||
          isOperatorIToken(itoken3, CEXPR_OP_TIMES_EQUALS     ) ||
          isOperatorIToken(itoken3, CEXPR_OP_DIVIDE_EQUALS    ) ||
          isOperatorIToken(itoken3, CEXPR_OP_MODULUS_EQUALS   ) ||
          isOperatorIToken(itoken3, CEXPR_OP_PLUS_EQUALS      ) ||
          isOperatorIToken(itoken3, CEXPR_OP_MINUS_EQUALS     ) ||
          isOperatorIToken(itoken3, CEXPR_OP_BIT_LSHIFT_EQUALS) ||
          isOperatorIToken(itoken3, CEXPR_OP_BIT_RSHIFT_EQUALS) ||
          isOperatorIToken(itoken3, CEXPR_OP_BIT_AND_EQUALS   ) ||
          isOperatorIToken(itoken3, CEXPR_OP_BIT_XOR_EQUALS   ) ||
          isOperatorIToken(itoken3, CEXPR_OP_BIT_OR_EQUALS    )) {
        CExprITokenPtr itoken4 = readAssignmentExpression();

        if (itoken4.isValid()) {
          deleteCollapsedIToken(itoken1, itoken2);

          itoken = CExprIToken::createIToken(CEXPR_ASSIGNMENT_EXPRESSION);

          addITokenToType(itoken2, itoken);
          addITokenToType(itoken3, itoken);
          addITokenToType(itoken4, itoken);
        }
        else
          stackIToken(itoken3);
      }
      else
        stackIToken(itoken3);
    }

    if (! itoken.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_ASSIGNMENT_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }

    return itoken;
  }

  itoken1 = readUnaryExpression();

  if (! itoken1.isValid())
    return itoken;

  CExprITokenPtr itoken2 = unstackIToken();

  if (! isOperatorIToken(itoken2, CEXPR_OP_EQUALS           ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_TIMES_EQUALS     ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_DIVIDE_EQUALS    ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_MODULUS_EQUALS   ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_PLUS_EQUALS      ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_MINUS_EQUALS     ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_BIT_LSHIFT_EQUALS) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_BIT_RSHIFT_EQUALS) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_BIT_AND_EQUALS   ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_BIT_XOR_EQUALS   ) &&
      ! isOperatorIToken(itoken2, CEXPR_OP_BIT_OR_EQUALS    )) {
    stackIToken(itoken2);
    stackIToken(itoken1);

    return itoken;
  }

  CExprITokenPtr itoken3 = readAssignmentExpression();

  if (! itoken3.isValid()) {
    stackIToken(itoken2);
    stackIToken(itoken1);

    return itoken;
  }

  itoken = CExprIToken::createIToken(CEXPR_ASSIGNMENT_EXPRESSION);

  addITokenToType(itoken1, itoken);
  addITokenToType(itoken2, itoken);
  addITokenToType(itoken3, itoken);

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <conditional_expression>:= <logical_or_expression>
 * <conditional_expression>:= <logical_or_expression> ?  <expression> : <conditional_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readConditionalExpression()
{
  DEBUG_ENTER("readConditionalExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_CONDITIONAL_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readLogicalOrExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_CONDITIONAL_EXPRESSION);

      addITokenToType(itoken1, itoken);

      CExprITokenPtr itoken2 = unstackIToken();

      if (isOperatorIToken(itoken2, CEXPR_OP_QUESTION)) {
        CExprITokenPtr itoken3 = readExpression();

        if (itoken3.isValid()) {
          CExprITokenPtr itoken4 = unstackIToken();

          if (isOperatorIToken(itoken4, CEXPR_OP_COLON)) {
            CExprITokenPtr itoken5 = readConditionalExpression();

            if (itoken5.isValid()) {
              addITokenToType(itoken2, itoken);
              addITokenToType(itoken3, itoken);
              addITokenToType(itoken4, itoken);
              addITokenToType(itoken5, itoken);
            }
            else {
              stackIToken(itoken4);
              stackIToken(itoken3);
              stackIToken(itoken2);
              stackIToken(itoken1);
            }
          }
          else {
            setLastError("Missing colon for '?:'");

            stackIToken(itoken4);
            stackIToken(itoken3);
            stackIToken(itoken2);
            stackIToken(itoken1);
          }
        }
        else {
          stackIToken(itoken2);
          stackIToken(itoken1);
        }
      }
      else
        stackIToken(itoken2);
    }
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <logical_or_expression>:= <logical_and_expression>
 * <logical_or_expression>:= <logical_or_expression> || <logical_and_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readLogicalOrExpression()
{
  DEBUG_ENTER("readLogicalOrExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_LOGICAL_OR_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readLogicalAndExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_LOGICAL_OR_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_LOGICAL_OR)) {
      CExprITokenPtr itoken2 = readLogicalAndExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_LOGICAL_OR_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <logical_and_expression>:= <inclusive_or_expression>
 * <logical_and_expression>:= <logical_and_expression> && <inclusive_or_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readLogicalAndExpression()
{
  DEBUG_ENTER("readLogicalAndExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_LOGICAL_AND_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readInclusiveOrExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_LOGICAL_AND_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_LOGICAL_AND)) {
      CExprITokenPtr itoken2 = readInclusiveOrExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_LOGICAL_AND_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <inclusive_or_expression>:= <exclusive_or_expression>
 * <inclusive_or_expression>:= <inclusive_or_expression> | <exclusive_or_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readInclusiveOrExpression()
{
  DEBUG_ENTER("readInclusiveOrExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_INCLUSIVE_OR_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readExclusiveOrExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_INCLUSIVE_OR_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_BIT_OR)) {
      CExprITokenPtr itoken2 = readExclusiveOrExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_INCLUSIVE_OR_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <exclusive_or_expression>:= <and_expression>
 * <exclusive_or_expression>:= <exclusive_or_expression> ^ <and_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readExclusiveOrExpression()
{
  DEBUG_ENTER("readExclusiveOrExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_EXCLUSIVE_OR_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readAndExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_EXCLUSIVE_OR_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_BIT_XOR)) {
      CExprITokenPtr itoken2 = readAndExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_EXCLUSIVE_OR_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <and_expression>:= <equality_expression>
 * <and_expression>:= <and_expression> & <equality_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readAndExpression()
{
  DEBUG_ENTER("readAndExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_AND_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readEqualityExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_AND_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_BIT_AND)) {
      CExprITokenPtr itoken2 = readEqualityExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_AND_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <equality_expression>:= <relational_expression>
 * <equality_expression>:= <equality_expression> == <relational_expression>
 * <equality_expression>:= <equality_expression> != <relational_expression>
 * <equality_expression>:= <equality_expression> ~= <relational_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readEqualityExpression()
{
  DEBUG_ENTER("readEqualityExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_EQUALITY_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readRelationalExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_EQUALITY_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_EQUAL       ) ||
           isOperatorIToken(itoken1, CEXPR_OP_NOT_EQUAL   ) ||
           isOperatorIToken(itoken1, CEXPR_OP_APPROX_EQUAL)) {
      CExprITokenPtr itoken2 = readRelationalExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_EQUALITY_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <relational_expression>:= <shift_expression>
 * <relational_expression>:= <relational_expression> <  <shift_expression>
 * <relational_expression>:= <relational_expression> >  <shift_expression>
 * <relational_expression>:= <relational_expression> <= <shift_expression>
 * <relational_expression>:= <relational_expression> >= <shift_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readRelationalExpression()
{
  DEBUG_ENTER("readRelationalExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_RELATIONAL_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readShiftExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_RELATIONAL_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_LESS         ) ||
           isOperatorIToken(itoken1, CEXPR_OP_GREATER      ) ||
           isOperatorIToken(itoken1, CEXPR_OP_LESS_EQUAL   ) ||
           isOperatorIToken(itoken1, CEXPR_OP_GREATER_EQUAL)) {
      CExprITokenPtr itoken2 = readShiftExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_RELATIONAL_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <shift_expression>:= <additive_expression>
 * <shift_expression>:= <shift_expression> << <additive_expression>
 * <shift_expression>:= <shift_expression> >> <additive_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readShiftExpression()
{
  DEBUG_ENTER("readShiftExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_SHIFT_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readAdditiveExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_SHIFT_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_BIT_LSHIFT) ||
           isOperatorIToken(itoken1, CEXPR_OP_BIT_RSHIFT)) {
      CExprITokenPtr itoken2 = readAdditiveExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_SHIFT_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <additive_expression>:= <multiplicative_expression>
 * <additive_expression>:= <additive_expression> + <multiplicative_expression>
 * <additive_expression>:= <additive_expression> - <multiplicative_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readAdditiveExpression()
{
  DEBUG_ENTER("readAdditiveExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_ADDITIVE_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readMultiplicativeExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_ADDITIVE_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_PLUS ) ||
           isOperatorIToken(itoken1, CEXPR_OP_MINUS)) {
      CExprITokenPtr itoken2 = readMultiplicativeExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_ADDITIVE_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else {
        if (isOperatorIToken(itoken1, CEXPR_OP_PLUS))
          setLastError("Missing right expression for '+'");
        else
          setLastError("Missing right expression for '*'");

        stackIToken(itoken1);
        stackIToken(itoken);

        return CExprITokenPtr();
      }

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <multiplicative_expression>:= <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> * <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> / <power_expression>
 * <multiplicative_expression>:= <multiplicative_expression> % <power_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readMultiplicativeExpression()
{
  DEBUG_ENTER("readMultiplicativeExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_MULTIPLICATIVE_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readPowerExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_MULTIPLICATIVE_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_TIMES  ) ||
           isOperatorIToken(itoken1, CEXPR_OP_DIVIDE ) ||
           isOperatorIToken(itoken1, CEXPR_OP_MODULUS)) {
      CExprITokenPtr itoken2 = readPowerExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_MULTIPLICATIVE_EXPRESSION);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <power_expression>:= <unary_expression>
 * <power_expression>:= <unary_expression> ** <power_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readPowerExpression()
{
  DEBUG_ENTER("readPowerExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_POWER_EXPRESSION);

  if (itoken.isValid())
    return itoken;

  CExprITokenPtr itoken1 = readUnaryExpression();

  if (! itoken1.isValid())
    return itoken;

  CExprITokenPtr itoken2 = unstackIToken();

  if (isOperatorIToken(itoken2, CEXPR_OP_POWER)) {
    CExprITokenPtr itoken3 = readPowerExpression();

    if (itoken3.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_POWER_EXPRESSION);

      addITokenToType(itoken1, itoken);
      addITokenToType(itoken2, itoken);
      addITokenToType(itoken3, itoken);
    }
    else {
      stackIToken(itoken2);
      stackIToken(itoken1);
    }
  }
  else {
    stackIToken(itoken2);

    itoken = CExprIToken::createIToken(CEXPR_POWER_EXPRESSION);

    addITokenToType(itoken1, itoken);
  }

  DEBUG_PRINT(itoken);

  return itoken;
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

CExprITokenPtr
CExprInterpImpl::
readUnaryExpression()
{
  DEBUG_ENTER("readUnaryExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_UNARY_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    if (isOperatorIToken(itoken1, CEXPR_OP_INCREMENT  ) ||
        isOperatorIToken(itoken1, CEXPR_OP_DECREMENT  ) ||
        isOperatorIToken(itoken1, CEXPR_OP_PLUS       ) ||
        isOperatorIToken(itoken1, CEXPR_OP_MINUS      ) ||
        isOperatorIToken(itoken1, CEXPR_OP_BIT_NOT    ) ||
        isOperatorIToken(itoken1, CEXPR_OP_LOGICAL_NOT)) {
      CExprITokenPtr itoken2 = readUnaryExpression();

      if (itoken2.isValid()) {
        itoken = CExprIToken::createIToken(CEXPR_UNARY_EXPRESSION);

        addITokenToType(itoken1, itoken);
        addITokenToType(itoken2, itoken);

        return itoken;
      }
    }

    stackIToken(itoken1);

    itoken1 = readPostfixExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_UNARY_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <postfix_expression>:= <primary_expression>
 * <postfix_expression>:= <identifier> ( <argument_expression_list>(opt) )
 * <postfix_expression>:= <postfix_expression> ++
 * <postfix_expression>:= <postfix_expression> --
 */

CExprITokenPtr
CExprInterpImpl::
readPostfixExpression()
{
  DEBUG_ENTER("readPostfixExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_POSTFIX_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    if (isIdentifierIToken(itoken1)) {
      CExprITokenPtr itoken2 = unstackIToken();

      if (isOperatorIToken(itoken2, CEXPR_OP_OPEN_RBRACKET)) {
        CExprITokenPtr itoken3 = readArgumentExpressionList();

        if (itoken3.isValid() || ! isError()) {
          CExprITokenPtr itoken4 = unstackIToken();

          if (isOperatorIToken(itoken4, CEXPR_OP_CLOSE_RBRACKET)) {
            itoken = CExprIToken::createIToken(CEXPR_POSTFIX_EXPRESSION);

            addITokenToType(itoken1, itoken);
            addITokenToType(itoken2, itoken);
            addITokenToType(itoken3, itoken);
            addITokenToType(itoken4, itoken);
          }
          else {
            setLastError("Missing close round bracket");

            stackIToken(itoken4);
            stackIToken(itoken3);
            stackIToken(itoken2);
            stackIToken(itoken1);
          }
        }
      }
      else {
        stackIToken(itoken2);
        stackIToken(itoken1);
      }
    }
    else
      stackIToken(itoken1);
  }

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readPrimaryExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_POSTFIX_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_INCREMENT) ||
           isOperatorIToken(itoken1, CEXPR_OP_DECREMENT)) {
      CExprITokenPtr itoken2 = CExprIToken::createIToken(CEXPR_POSTFIX_EXPRESSION);

      addITokenToType(itoken , itoken2);
      addITokenToType(itoken1, itoken2);

      itoken = itoken2;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <primary_expression>:= <integer>
 * <primary_expression>:= <real>
 * <primary_expression>:= <string>
 * <primary_expression>:= <identifier>
 * <primary_expression>:= ( <expression> )
 */

CExprITokenPtr
CExprInterpImpl::
readPrimaryExpression()
{
  DEBUG_ENTER("readPrimaryExpression");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_PRIMARY_EXPRESSION);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    if      (isIntegerIToken(itoken1) || isStringIToken(itoken1) || isRealIToken(itoken1)) {
      itoken = CExprIToken::createIToken(CEXPR_PRIMARY_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
    else if (isIdentifierIToken(itoken1)) {
      itoken = CExprIToken::createIToken(CEXPR_PRIMARY_EXPRESSION);

      addITokenToType(itoken1, itoken);
    }
    else if (isOperatorIToken(itoken1, CEXPR_OP_OPEN_RBRACKET)) {
      CExprITokenPtr itoken2 = readExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = unstackIToken();

        if (isOperatorIToken(itoken3, CEXPR_OP_CLOSE_RBRACKET)) {
          itoken = CExprIToken::createIToken(CEXPR_PRIMARY_EXPRESSION);

          addITokenToType(itoken1, itoken);
          addITokenToType(itoken2, itoken);
          addITokenToType(itoken3, itoken);
        }
        else {
          setLastError("Missing close round bracket");

          stackIToken(itoken3);
          stackIToken(itoken2);
          stackIToken(itoken1);
        }
      }
      else {
        setLastError("Missing expression after open round bracket");
        stackIToken(itoken1);
      }
    }
    else
      stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

/*
 * <argument_expression_list>:= <assignment_expression>
 * <argument_expression_list>:= <argument_expression_list> , <assignment_expression>
 */

CExprITokenPtr
CExprInterpImpl::
readArgumentExpressionList()
{
  DEBUG_ENTER("readArgumentExpressionList");

  CExprITokenPtr itoken = readITokenOfType(CEXPR_ARGUMENT_EXPRESSION_LIST);

  if (! itoken.isValid()) {
    CExprITokenPtr itoken1 = readAssignmentExpression();

    if (itoken1.isValid()) {
      itoken = CExprIToken::createIToken(CEXPR_ARGUMENT_EXPRESSION_LIST);

      addITokenToType(itoken1, itoken);
    }
  }

  if (itoken.isValid()) {
    CExprITokenPtr itoken1 = unstackIToken();

    while (isOperatorIToken(itoken1, CEXPR_OP_COMMA)) {
      CExprITokenPtr itoken2 = readAssignmentExpression();

      if (itoken2.isValid()) {
        CExprITokenPtr itoken3 = CExprIToken::createIToken(CEXPR_ARGUMENT_EXPRESSION_LIST);

        addITokenToType(itoken , itoken3);
        addITokenToType(itoken1, itoken3);
        addITokenToType(itoken2, itoken3);

        itoken = itoken3;
      }
      else
        break;

      itoken1 = unstackIToken();
    }

    stackIToken(itoken1);
  }

  DEBUG_PRINT(itoken);

  return itoken;
}

CExprITokenPtr
CExprInterpImpl::
readITokenOfType(CExprITokenType type)
{
  CExprITokenPtr itoken = unstackIToken();

  if (itoken.isValid()) {
    if (itoken->getType() == type)
      return itoken;

    stackIToken(itoken);
  }

  return CExprITokenPtr();
}

bool
CExprInterpImpl::
isLastToken()
{
  CExprITokenPtr itoken = unstackIToken();

  if (! itoken.isValid())
    return true;

  stackIToken(itoken);

  return false;
}

void
CExprInterpImpl::
addITokenToType(CExprITokenPtr child, CExprITokenPtr itoken)
{
  if (! itoken.isValid() || ! child.isValid())
    return;

  itoken->addChild(child);
}

CExprITokenPtr
CExprInterpImpl::
collapseITokenToType(CExprITokenPtr itoken, CExprITokenType type)
{
  CExprITokenPtr itoken1 = itoken;

  while (itoken1.isValid()) {
    if (itoken1->getType() == type)
      break;

    if (itoken1->getNumChildren() == 1)
      itoken1 = itoken1->getChild(0);
    else
      itoken1 = CExprITokenPtr();
  }

  return itoken1;
}

void
CExprInterpImpl::
deleteCollapsedIToken(CExprITokenPtr itoken, CExprITokenPtr itoken1)
{
  CExprITokenPtr itoken2 = itoken;

  while (itoken2 != itoken1) {
    CExprITokenPtr itoken3 = itoken2->getChild(0);

    itoken2->removeAllChildren();

    itoken2 = itoken3;
  }
}

bool
CExprInterpImpl::
isOperatorIToken(CExprITokenPtr itoken, CExprOpType id)
{
  if (! itoken.isValid())
    return false;

  if (itoken->getType() != CEXPR_ITOKEN_OPERATOR)
    return false;

  return (itoken->getOperator()->getType() == id);
}

bool
CExprInterpImpl::
isIdentifierIToken(CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return false;

  return (itoken->getType() == CEXPR_ITOKEN_IDENTIFIER);
}

bool
CExprInterpImpl::
isIntegerIToken(CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return false;

  return (itoken->getType() == CEXPR_ITOKEN_INTEGER);
}

bool
CExprInterpImpl::
isRealIToken(CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return false;

  return (itoken->getType() == CEXPR_ITOKEN_REAL);
}

bool
CExprInterpImpl::
isStringIToken(CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return false;

  return (itoken->getType() == CEXPR_ITOKEN_STRING);
}

void
CExprInterpImpl::
stackIToken(CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return;

  itoken_stack_.addToken(itoken);
}

CExprITokenPtr
CExprInterpImpl::
unstackIToken()
{
  CExprITokenPtr itoken = itoken_stack_.pop_back();

  if (itoken.isValid())
    return itoken;

  if (ptoken_stack_.getNumTokens() == 0)
    return CExprITokenPtr();

  CExprPTokenPtr ptoken = ptoken_stack_.pop_front();

  return CExprIToken::createIToken(ptoken);
}

void
CExprInterpImpl::
printTrackBack(std::ostream &os)
{
  std::string error_message = getLastError();

  if (error_message != "")
    os << error_message << std::endl;
  else
    os << "Syntax Error" << std::endl;

  uint size = itoken_stack_.getNumTokens();

  for (uint i = size - 1; i >= 1; --i) {
    CExprITokenPtr itoken = itoken_stack_.getToken(i);

    if (i < size)
      os << " ";

    os << itoken;
  }

  CExprITokenPtr itoken = itoken_stack_.getToken(0);

  if (itoken.isValid()) {
    if (size > 1)
      os << " ";

    os << ">>" << itoken << "<<";
  }

  size = ptoken_stack_.getNumTokens();

  for (uint i = 0; i < size; ++i) {
    CExprPTokenPtr ptoken = ptoken_stack_.getToken(i);

    os << " " << ptoken;
  }

  os << std::endl;
}

#ifdef CEXPR_DEBUG
void
CExprInterpImpl::
printTypeIToken(std::ostream &os, CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return;

  if (! CExprInst->getDebug())
    return;

  printTypeIToken1(os, itoken);

  os << std::endl;
}

void
CExprInterpImpl::
printTypeIToken1(std::ostream &os, CExprITokenPtr itoken)
{
  os << "<" << CIToken::getTypeName(itoken->getType()) << ">";

  itoken->print(os, false);

  os << " ";

  uint num_children = itoken->getNumChildren();

  for (uint i = 0; i < num_children; i++)
    printTypeIToken1(os, itoken->getChild(i));
}

std::string
CExprInterpImpl::
iTokenToString(CExprITokenPtr itoken)
{
  std::string str = "";

  concatITokenString(str, itoken);

  return str;
}

void
CExprInterpImpl::
concatITokenString(std::string &str, CExprITokenPtr itoken)
{
  if (! itoken.isValid())
    return;

  if (itoken->getType() == CEXPR_ITOKEN_STRING) {
    char c = str[str.size() - 1];

    if (c != '\0')
      str += " ";

    std::string str2 = itoken->getString();
    std::string str1 = CStrUtil::insertEscapeCodes(str2);

    str += "\"";

    if (str1 != "")
      str += str1;
    else
      str += str2;

    str += "\"";

    uint num_children = itoken->getNumChildren();

    for (uint i = 0; i < num_children; i++)
      concatITokenString(str, itoken->getChild(i));

    return;
  }

  std::string str2 = "";

  bool add_space = true;

  switch (itoken->getType()) {
    case CEXPR_ITOKEN_IDENTIFIER:
      str2 = itoken->getIdentifier();

      break;
    case CEXPR_ITOKEN_OPERATOR:
      str2 = itoken->getOperator()->text;

      if (str2[0] == '.' || str2[0] == ',' || str2[0] == ';' || str2[0] == '[' || str2[0] == ']')
        add_space = false;

      break;
    case CEXPR_ITOKEN_INTEGER:
      str2 = CStrUtil::toString(itoken->getInteger());

      break;
    case CEXPR_ITOKEN_REAL:
      str2 = CStrUtil::toString(itoken->getReal());

      break;
    default:
      break;
  }

  if (str2 != "") {
    char c = str[str.size() - 1];

    if (c == '\0' || c == '.')
      add_space = false;

    if (add_space)
      str += " ";

    str += str2;
  }

  uint num_children = itoken->getNumChildren();

  for (uint i = 0; i < num_children; i++)
    concatITokenString(str, itoken->getChild(i));
}
#endif

void
CExprInterpImpl::
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
CExprInterpImpl::
isError()
{
  return error_flag;
}

std::string
CExprInterpImpl::
getLastError()
{
  return last_error_message;
}
