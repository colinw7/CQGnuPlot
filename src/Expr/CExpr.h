#ifndef CEXPR_H
#define CEXPR_H

enum CExprOpType {
  CEXPR_OP_UNKNOWN           = -1,
  CEXPR_OP_OPEN_RBRACKET     = 0,
  CEXPR_OP_CLOSE_RBRACKET    = 1,
  CEXPR_OP_LOGICAL_NOT       = 2,
  CEXPR_OP_BIT_NOT           = 3,
  CEXPR_OP_INCREMENT         = 4,
  CEXPR_OP_DECREMENT         = 5,
  CEXPR_OP_UNARY_PLUS        = 6,
  CEXPR_OP_UNARY_MINUS       = 7,
  CEXPR_OP_POWER             = 8,
  CEXPR_OP_TIMES             = 9,
  CEXPR_OP_DIVIDE            = 10,
  CEXPR_OP_MODULUS           = 11,
  CEXPR_OP_PLUS              = 12,
  CEXPR_OP_MINUS             = 13,
  CEXPR_OP_BIT_LSHIFT        = 14,
  CEXPR_OP_BIT_RSHIFT        = 15,
  CEXPR_OP_LESS              = 16,
  CEXPR_OP_LESS_EQUAL        = 17,
  CEXPR_OP_GREATER           = 18,
  CEXPR_OP_GREATER_EQUAL     = 19,
  CEXPR_OP_EQUAL             = 20,
  CEXPR_OP_NOT_EQUAL         = 21,
  CEXPR_OP_APPROX_EQUAL      = 22,
  CEXPR_OP_BIT_AND           = 23,
  CEXPR_OP_BIT_XOR           = 24,
  CEXPR_OP_BIT_OR            = 25,
  CEXPR_OP_LOGICAL_AND       = 26,
  CEXPR_OP_LOGICAL_OR        = 27,
  CEXPR_OP_QUESTION          = 28,
  CEXPR_OP_COLON             = 29,
  CEXPR_OP_EQUALS            = 30,
  CEXPR_OP_PLUS_EQUALS       = 31,
  CEXPR_OP_MINUS_EQUALS      = 32,
  CEXPR_OP_TIMES_EQUALS      = 33,
  CEXPR_OP_DIVIDE_EQUALS     = 34,
  CEXPR_OP_MODULUS_EQUALS    = 35,
  CEXPR_OP_BIT_AND_EQUALS    = 36,
  CEXPR_OP_BIT_XOR_EQUALS    = 37,
  CEXPR_OP_BIT_OR_EQUALS     = 38,
  CEXPR_OP_BIT_LSHIFT_EQUALS = 39,
  CEXPR_OP_BIT_RSHIFT_EQUALS = 40,
#ifdef GNUPLOT_EXPR
  CEXPR_OP_OPEN_SBRACKET     = 41,
  CEXPR_OP_CLOSE_SBRACKET    = 42,
  CEXPR_OP_STR_EQUAL         = 43,
  CEXPR_OP_STR_NOT_EQUAL     = 44,
  CEXPR_OP_STR_CONCAT        = 45,
  CEXPR_OP_FACTORIAL         = 46,
#endif
  CEXPR_OP_COMMA             = 47,
  CEXPR_OP_START_BLOCK       = 48,
  CEXPR_OP_END_BLOCK         = 49
};

enum CExprValueType {
  CEXPR_VALUE_NONE    = 0,
  CEXPR_VALUE_BOOLEAN = (1<<0),
  CEXPR_VALUE_INTEGER = (1<<1),
  CEXPR_VALUE_REAL    = (1<<2),
  CEXPR_VALUE_STRING  = (1<<3),
  CEXPR_VALUE_COMPLEX = (1<<4),
  CEXPR_VALUE_NULL    = (1<<5),

  CEXPR_VALUE_ANY = (CEXPR_VALUE_BOOLEAN | CEXPR_VALUE_INTEGER |
                     CEXPR_VALUE_REAL | CEXPR_VALUE_STRING | CEXPR_VALUE_COMPLEX)
};

class CExprValue;
class CExprIToken;
class CExprVariable;
class CExprFunction;

#include <CRefPtr.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>
#include <complex>

typedef CRefPtr<CExprValue>    CExprValuePtr;
typedef CRefPtr<CExprIToken>   CExprITokenPtr;
typedef CRefPtr<CExprVariable> CExprVariablePtr;
typedef CRefPtr<CExprFunction> CExprFunctionPtr;

typedef std::vector<CExprValuePtr> CExprValueArray;

typedef CExprValuePtr (*CExprFunctionProc)(const CExprValueArray &values);

class CExprValueBase {
 public:
  virtual ~CExprValueBase() { }

  virtual CExprValueBase *dup() const { return 0; }

  virtual bool getBooleanValue(bool                 &) const { return false; }
  virtual bool getIntegerValue(long                 &) const { return false; }
  virtual bool getRealValue   (double               &) const { return false; }
  virtual bool getStringValue (std::string          &) const { return false; }
  virtual bool getComplexValue(std::complex<double> &) const { return false; }

  virtual void setBooleanValue(bool                        ) { assert(false); }
  virtual void setIntegerValue(long                        ) { assert(false); }
  virtual void setRealValue   (double                      ) { assert(false); }
  virtual void setStringValue (const std::string          &) { assert(false); }
  virtual void setComplexValue(const std::complex<double> &) { assert(false); }

  virtual CExprValuePtr execUnaryOp(CExprOpType) const {
    return CExprValuePtr();
  }

  virtual CExprValuePtr execBinaryOp(CExprValuePtr, CExprOpType) const {
    return CExprValuePtr();
  }

  virtual CExprValuePtr subscript(const CExprValueArray &) const {
    return CExprValuePtr();
  }

  virtual void print(std::ostream &os) const {
    os << "<null>";
  }
};

//-------

enum CExprTokenType {
  CEXPR_TOKEN_UNKNOWN = -1,

  CEXPR_TOKEN_NONE = 0,

  CEXPR_TOKEN_IDENTIFIER = 1,
  CEXPR_TOKEN_OPERATOR   = 2,
  CEXPR_TOKEN_INTEGER    = 4,
  CEXPR_TOKEN_REAL       = 5,
  CEXPR_TOKEN_STRING     = 6,
  CEXPR_TOKEN_COMPLEX    = 7,
  CEXPR_TOKEN_FUNCTION   = 8,
  CEXPR_TOKEN_VALUE      = 9,
  CEXPR_TOKEN_BLOCK      = 10
};

enum CExprITokenType {
  CEXPR_TOKEN_TYPE                = 100,
  CEXPR_EXPRESSION                = 101,
  CEXPR_ASSIGNMENT_EXPRESSION     = 102,
  CEXPR_CONDITIONAL_EXPRESSION    = 103,
  CEXPR_LOGICAL_OR_EXPRESSION     = 104,
  CEXPR_LOGICAL_AND_EXPRESSION    = 105,
  CEXPR_INCLUSIVE_OR_EXPRESSION   = 106,
  CEXPR_EXCLUSIVE_OR_EXPRESSION   = 107,
  CEXPR_AND_EXPRESSION            = 108,
  CEXPR_EQUALITY_EXPRESSION       = 109,
  CEXPR_RELATIONAL_EXPRESSION     = 110,
  CEXPR_SHIFT_EXPRESSION          = 111,
  CEXPR_ADDITIVE_EXPRESSION       = 112,
  CEXPR_MULTIPLICATIVE_EXPRESSION = 113,
  CEXPR_POWER_EXPRESSION          = 114,
  CEXPR_UNARY_EXPRESSION          = 115,
  CEXPR_POSTFIX_EXPRESSION        = 116,
  CEXPR_PRIMARY_EXPRESSION        = 117,
  CEXPR_ARGUMENT_EXPRESSION_LIST  = 118,
  CEXPR_DUMMY_EXPRESSION          = 119,
};

//-------

#include <CExprBValue.h>
#include <CExprIValue.h>
#include <CExprRValue.h>
#include <CExprSValue.h>
#include <CExprCValue.h>
#include <CExprValue.h>
#include <CExprOperator.h>
#include <CExprToken.h>
#include <CExprParse.h>
#include <CExprVariable.h>
#include <CExprInterp.h>
#include <CExprCompile.h>
#include <CExprFunction.h>
#include <CExprExecute.h>
#include <CExprStrgen.h>
#include <CExprError.h>

//-------

#define CExprInst CExpr::instance()

class CExpr {
 public:
  typedef std::vector<CExprFunctionPtr> Functions;
  typedef std::vector<std::string>      StringArray;

 public:
  static CExpr *instance();

 ~CExpr() { }

  bool getQuiet() const { return quiet_; }
  void setQuiet(bool b) { quiet_ = b; }

  bool getDebug() const { return debug_; }
  void setDebug(bool b) { debug_ = b; }

  bool getTrace() const { return trace_; }
  void setTrace(bool b) { trace_ = b; }

  bool getDegrees() const { return degrees_; }
  void setDegrees(bool b) { degrees_ = b; }

  bool evaluateExpression(const std::string &str, CExprValueArray &values);
  bool evaluateExpression(const std::string &str, CExprValuePtr &value);

  bool executePTokenStack(const CExprTokenStack &stack, CExprValueArray &values);
  bool executePTokenStack(const CExprTokenStack &stack, CExprValuePtr &value);

  CExprTokenStack parseLine(const std::string &line);
  CExprITokenPtr  interpPTokenStack(const CExprTokenStack &stack);
  CExprTokenStack compileIToken(CExprITokenPtr itoken);

  bool skipExpression(const std::string &line, uint &i);

  bool executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values);
  bool executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value);

  void saveCompileState();
  void restoreCompileState();

  CExprVariablePtr getVariable     (const std::string &name) const;
  CExprVariablePtr createVariable  (const std::string &name, CExprValuePtr value);
  void             removeVariable  (const std::string &name);
  void             getVariableNames(StringArray &names) const;

  CExprVariablePtr createRealVariable   (const std::string &name, double x);
  CExprVariablePtr createIntegerVariable(const std::string &name, long l);
  CExprVariablePtr createStringVariable (const std::string &name, const std::string &str);
  CExprVariablePtr createComplexVariable(const std::string &name, const std::complex<double> &c);

  CExprFunctionPtr getFunction (const std::string &name);
  void             getFunctions(const std::string &name, Functions &functions);

  CExprFunctionPtr addFunction(const std::string &name, const StringArray &args,
                               const std::string &proc);
  CExprFunctionPtr addFunction(const std::string &name, const std::string &argsStr,
                               CExprFunctionObj *proc);
  void getFunctionNames(StringArray &names) const;

  CExprTokenBaseP getOperator(CExprOpType id);

  std::string getOperatorName(CExprOpType type) const;

  CExprValuePtr createBooleanValue(bool b);
  CExprValuePtr createIntegerValue(long i);
  CExprValuePtr createRealValue   (double r);
  CExprValuePtr createStringValue (const std::string &s);
  CExprValuePtr createComplexValue(const std::complex<double> &c);

  std::string printf(const std::string &fmt, const CExprValueArray &values) const;

  void errorMsg(const std::string &msg) const;

 private:
  CExpr();

 private:
  typedef std::vector<CExprCompile *> Compiles;
  typedef std::vector<CExprExecute *> Executes;

  bool                       quiet_;
  bool                       debug_;
  bool                       trace_;
  bool                       degrees_;
  CAutoPtr<CExprParse>       parse_;
  CAutoPtr<CExprInterp>      interp_;
  CAutoPtr<CExprCompile>     compile_;
  CAutoPtr<CExprExecute>     execute_;
  Compiles                   compiles_;
  Executes                   executes_;
  CAutoPtr<CExprOperatorMgr> operatorMgr_;
  CAutoPtr<CExprVariableMgr> variableMgr_;
  CAutoPtr<CExprFunctionMgr> functionMgr_;
};

//------

template<typename T>
class CExprUtil {
 public:
  static bool getTypeValue(CExprValuePtr value, T &v);

  static CExprValuePtr createValue(const T &v);

  static std::string argTypeStr();
};

template<>
class CExprUtil<bool> {
 public:
  static bool getTypeValue(CExprValuePtr value, bool &b) { return value->getBooleanValue(b); }

  static CExprValuePtr createValue(const bool &b) { return CExprInst->createBooleanValue(b); }

  static std::string argTypeStr() { return "b"; }
};

template<>
class CExprUtil<long> {
 public:
  static bool getTypeValue(CExprValuePtr value, long &i) { return value->getIntegerValue(i); }

  static CExprValuePtr createValue(const long &i) { return CExprInst->createIntegerValue(i); }

  static std::string argTypeStr() { return "i"; }
};

template<>
class CExprUtil<double> {
 public:
  static bool getTypeValue(CExprValuePtr value, double &r) { return value->getRealValue(r); }

  static CExprValuePtr createValue(const double &r) { return CExprInst->createRealValue(r); }

  static std::string argTypeStr() { return "r"; }
};

template<>
class CExprUtil<std::string> {
 public:
  static bool getTypeValue(CExprValuePtr value, std::string &s) { return value->getStringValue(s); }

  static CExprValuePtr createValue(const std::string &s) { return CExprInst->createStringValue(s); }

  static std::string argTypeStr() { return "s"; }
};

template<>
class CExprUtil< std::complex<double> > {
 public:
  static bool getTypeValue(CExprValuePtr value, std::complex<double> &c) {
    return value->getComplexValue(c);
  }

  static CExprValuePtr createValue(const std::complex<double> &c) {
    return CExprInst->createComplexValue(c);
  }

  static std::string argTypeStr() { return "c"; }
};

#endif
