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
#endif
  CEXPR_OP_COMMA             = 46
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
class CExprPToken;
class CExprIToken;
class CExprCToken;
class CExprEToken;
class CExprVariable;
class CExprFunction;
class CExprOperator;

#include <CRefPtr.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>
#include <complex>

typedef CRefPtr<CExprValue>    CExprValuePtr;
typedef CRefPtr<CExprPToken>   CExprPTokenPtr;
typedef CRefPtr<CExprCToken>   CExprCTokenPtr;
typedef CRefPtr<CExprIToken>   CExprITokenPtr;
typedef CRefPtr<CExprEToken>   CExprETokenPtr;
typedef CRefPtr<CExprVariable> CExprVariablePtr;
typedef CRefPtr<CExprFunction> CExprFunctionPtr;
typedef CRefPtr<CExprOperator> CExprOperatorPtr;

typedef CExprValuePtr (*CExprFunctionProc)(const std::vector<CExprValuePtr> &values);

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

  virtual CExprValuePtr subscript(const std::vector<CExprValuePtr> &) const {
    return CExprValuePtr();
  }

  virtual void print(std::ostream &os) const {
    os << "<null>";
  }
};

//-------

#include <CExprBValue.h>
#include <CExprIValue.h>
#include <CExprRValue.h>
#include <CExprSValue.h>
#include <CExprCValue.h>
#include <CExprValue.h>
#include <CExprVariable.h>
#include <CExprFunction.h>
#include <CExprOperator.h>
#include <CExprParse.h>
#include <CExprInterp.h>
#include <CExprCompile.h>
#include <CExprExecute.h>
#include <CExprError.h>

//-------

#define CExprInst CExpr::instance()

class CExpr {
 public:
  static CExpr *instance();

 ~CExpr() { }

  bool getDebug() const { return debug_; }
  void setDebug(bool b) { debug_ = b; }

  bool getTrace() const { return trace_; }
  void setTrace(bool b) { trace_ = b; }

  bool getDegrees() const { return degrees_; }
  void setDegrees(bool b) { degrees_ = b; }

  bool evaluateExpression(const std::string &str, std::vector<CExprValuePtr> &values);
  bool evaluateExpression(const std::string &str, CExprValuePtr &value);

  bool executePTokenStack(const CExprPTokenStack &stack, std::vector<CExprValuePtr> &values);
  bool executePTokenStack(const CExprPTokenStack &stack, CExprValuePtr &value);

  CExprPTokenStack parseLine(const std::string &line);
  CExprITokenPtr   interpPTokenStack(const CExprPTokenStack &stack);
  CExprCTokenStack compileIToken(CExprITokenPtr itoken);

  bool skipExpression(const std::string &line, uint &i);

  bool executeCTokenStack(const CExprCTokenStack &stack, std::vector<CExprValuePtr> &values);
  bool executeCTokenStack(const CExprCTokenStack &stack, CExprValuePtr &value);

  void saveCompileState();
  void restoreCompileState();

  CExprVariablePtr getVariable(const std::string &name) const;
  CExprVariablePtr createVariable(const std::string &name, CExprValuePtr value);
  void removeVariable(const std::string &name);
  void getVariableNames(std::vector<std::string> &names) const;

  CExprVariablePtr createRealVariable   (const std::string &name, double x);
  CExprVariablePtr createIntegerVariable(const std::string &name, long l);
  CExprVariablePtr createStringVariable (const std::string &name, const std::string &str);
  CExprVariablePtr createComplexVariable(const std::string &name, const std::complex<double> &c);

  CExprFunctionPtr getFunction(const std::string &name);
  CExprFunctionPtr addFunction(const std::string &name, const std::vector<std::string> &args,
                               const std::string &proc);
  CExprFunctionPtr addFunction(const std::string &name, const std::string &argsStr,
                               CExprFunctionObj *proc);
  void getFunctionNames(std::vector<std::string> &names) const;

  CExprOperatorPtr getOperator(CExprOpType type) const;

  CExprValuePtr createBooleanValue(bool b);
  CExprValuePtr createIntegerValue(long i);
  CExprValuePtr createRealValue   (double r);
  CExprValuePtr createStringValue (const std::string &s);
  CExprValuePtr createComplexValue(const std::complex<double> &c);

  std::string printf(const std::string &fmt, const std::vector<CExprValuePtr> &values) const;

  void errorMsg(const std::string &msg) const;

 private:
  CExpr();

 private:
  typedef std::vector<CExprCompile *> Compiles;
  typedef std::vector<CExprExecute *> Executes;

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
