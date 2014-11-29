#include <CExprI.h>
#include <cstring>

/*
 *   Operator Token        | Precedence   | Associativity
 * ------------------------+--------------+--------------
 *                         |              |
 *  ( )                    |  16          |  L -> R
 *  **                     |  15          |  R -> L
 *  !   ~   ++  --  +   -  |  14 (unary)  |  R -> L
 *  *   /   %              |  13          |  L -> R
 *  +   -                  |  12          |  L -> R
 *  <<  >>                 |  11          |  L -> R
 *  <   <=  >   >=         |  10          |  L -> R
 *  ==  !=  ~=             |   9          |  L -> R
 *  &                      |   8          |  L -> R
 *  ^                      |   7          |  L -> R
 *  |                      |   6          |  L -> R
 *  &&                     |   5          |  L -> R
 *  ||                     |   4          |  L -> R
 *  ?:                     |   3          |  R -> L
 *  =   +=  -=   *=  /= %= |   2          |  R -> L
 *  &=  ^=  |=  <<= >>=    |   2          |  R -> L
 *  ,                      |   1          |  R -> L
 *
 */

struct CExprOperatorData {
  CExprOpType  type;
  const char  *name;
};

static CExprOperatorData
operator_data[] = {
  { CEXPR_OP_OPEN_RBRACKET    , "("  , },
  { CEXPR_OP_CLOSE_RBRACKET   , ")"  , },
  { CEXPR_OP_LOGICAL_NOT      , "!"  , },
  { CEXPR_OP_BIT_NOT          , "~"  , },
  { CEXPR_OP_INCREMENT        , "++" , },
  { CEXPR_OP_DECREMENT        , "--" , },
  { CEXPR_OP_UNARY_PLUS       , "+"  , },
  { CEXPR_OP_UNARY_MINUS      , "-"  , },
  { CEXPR_OP_POWER            , "**" , },
  { CEXPR_OP_TIMES            , "*"  , },
  { CEXPR_OP_DIVIDE           , "/"  , },
  { CEXPR_OP_MODULUS          , "%"  , },
  { CEXPR_OP_PLUS             , "+"  , },
  { CEXPR_OP_MINUS            , "-"  , },
  { CEXPR_OP_BIT_LSHIFT       , "<<" , },
  { CEXPR_OP_BIT_RSHIFT       , ">>" , },
  { CEXPR_OP_LESS             , "<"  , },
  { CEXPR_OP_LESS_EQUAL       , "<=" , },
  { CEXPR_OP_GREATER          , ">"  , },
  { CEXPR_OP_GREATER_EQUAL    , ">=" , },
  { CEXPR_OP_EQUAL            , "==" , },
  { CEXPR_OP_NOT_EQUAL        , "!=" , },
  { CEXPR_OP_APPROX_EQUAL     , "~=" , },
  { CEXPR_OP_BIT_AND          , "&"  , },
  { CEXPR_OP_BIT_XOR          , "^"  , },
  { CEXPR_OP_BIT_OR           , "|"  , },
  { CEXPR_OP_LOGICAL_AND      , "&&" , },
  { CEXPR_OP_LOGICAL_OR       , "||" , },
  { CEXPR_OP_QUESTION         , "?"  , },
  { CEXPR_OP_COLON            , ":"  , },
  { CEXPR_OP_EQUALS           , "="  , },
  { CEXPR_OP_PLUS_EQUALS      , "+=" , },
  { CEXPR_OP_MINUS_EQUALS     , "-=" , },
  { CEXPR_OP_TIMES_EQUALS     , "*=" , },
  { CEXPR_OP_DIVIDE_EQUALS    , "/=" , },
  { CEXPR_OP_MODULUS_EQUALS   , "%=" , },
  { CEXPR_OP_BIT_AND_EQUALS   , "&=" , },
  { CEXPR_OP_BIT_XOR_EQUALS   , "^=" , },
  { CEXPR_OP_BIT_OR_EQUALS    , "|=" , },
  { CEXPR_OP_BIT_LSHIFT_EQUALS, "<<=", },
  { CEXPR_OP_BIT_RSHIFT_EQUALS, ">>=", },
#ifdef GNUPLOT_EXPR
  { CEXPR_OP_OPEN_SBRACKET    , "["  , },
  { CEXPR_OP_CLOSE_SBRACKET   , "]"  , },
  { CEXPR_OP_CONCAT           , "."  , },
#endif
  { CEXPR_OP_COMMA            , ","  , },
  { CEXPR_OP_UNKNOWN          , 0    , }
};

CExprOperatorMgr::
CExprOperatorMgr()
{
  for (uint i = 0; operator_data[i].name != 0; ++i)
    operator_map_[operator_data[i].type] =
      CExprOperatorPtr(new CExprOperator(operator_data[i].type, operator_data[i].name));
}

CExprOperatorPtr
CExprOperatorMgr::
getOperator(CExprOpType type) const
{
  OperatorMap::const_iterator p = operator_map_.find(type);

  if (p != operator_map_.end())
    return (*p).second;

  return CExprOperatorPtr();
}

//------

CExprOperator::
CExprOperator(CExprOpType type, const std::string &name) :
 type_(type), name_(name)
{
}

bool
CExprOperator::
isOperatorChar(char c)
{
#ifdef GNUPLOT_EXPR
  static char operator_chars[] = "()!~*/%+-<>=!&^|?:,[].";
#else
  static char operator_chars[] = "()!~*/%+-<>=!&^|?:,";
#endif

  return (strchr(operator_chars, c) != 0);
}

bool
CExprOperator::
isOperatorStr(char c)
{
  return (c == 'n' || c == 'e');
}
