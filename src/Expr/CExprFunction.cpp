#include <CExprI.h>
#include <cmath>
#include <complex.h>
#include <cstdlib>
#include <cstring>

// NOTE: types are only needed if normal conversion rules don't handle the type correctly

#ifdef GNUPLOT_EXPR
namespace {

double invnorm(double x) {
  return sqrt(2)/erf(2*x - 1);
}

// TODO
std::complex<double> cerf(const std::complex<double> &c) {
  double r = ::erf(c.real());
  return std::complex<double>(r, 0);
}

// TODO
std::complex<double> cerfc(const std::complex<double> &c) {
  double r = ::erfc(c.real());
  return std::complex<double>(r, 0);
}

double RadToDeg(double x) {
  return 180.0*x/M_PI;
}

double DegToRad(double x) {
  return M_PI*x/180.0;
}

}

class CExprSubStr {
 public:
  std::string operator()(const std::string &str, int i1, int i2) {
    if (i1 < 1              ) i1 = 1;
    if (i1 > int(str.size())) i1 = str.size();

    if (i2 < 0              ) i2 = str.size();
    if (i2 > int(str.size())) i2 = str.size();

    std::string str1;

    for (int j = i1 - 1; j <= i2 - 1; ++j)
      str1 += str[j];

    return str1;
  }
};

class CExprStrStrT {
 public:
  // find position of str2 in str1
  int operator()(const std::string &str1, const std::string &str2) {
    auto p = str1.find(str2);

    if (p == std::string::npos)
      return 0;
    else
      return p + 1;
  }
};

class CExprWords {
 public:
  int operator()(const std::string &str) {
    std::vector<std::string> words;

    (void) CStrUtil::addWords(str, words);

    return words.size();
  }
};

class CExprWord {
 public:
  std::string operator()(const std::string &str, int i) {
    std::vector<std::string> words;

    (void) CStrUtil::addWords(str, words);

    if (i > 0 && i <= int(words.size()))
      return words[i - 1];
    else
      return "";
  }
};

class CExprStrLen {
 public:
  int operator()(const std::string &str) {
    return str.size();
  }
};

class CExprSystem {
 public:
  std::string operator()(const std::string &str) {
    FILE *fp = popen(str.c_str(), "r");
    if (! fp) return "";

    std::string res;

    int c;

    while ((c = fgetc(fp)) != EOF)
      res += char(c);

    pclose(fp);

    return res;
  }
};
#endif

struct CExprBuiltinFunction {
  const char        *name;
  const char        *args;
  CExprFunctionProc  proc;
};

static CExprValuePtr
CExprFunctionSqrt(const CExprValueArray &values)
{
  assert(values.size() == 1);

  double real;

  if (! values[0]->getRealValue(real))
    return CExprValuePtr();

  if (real >= 0.0) {
    double real1 = ::sqrt(real);

    return CExprInst->createRealValue(real1);
  }
  else {
    double real1 = ::sqrt(-real);

    return CExprInst->createComplexValue(std::complex<double>(0, real1));
  }
}

static CExprValuePtr
CExprFunctionExp(const CExprValueArray &values)
{
  assert(values.size() == 1);

  double               r;
  std::complex<double> c;

  if      (values[0]->getComplexValue(c)) {
    double r1 = exp(c.real())*cos(c.imag());
    double c1 = exp(c.real())*sin(c.imag());

    return CExprInst->createComplexValue(std::complex<double>(r1, c1));
  }
  else if (values[0]->getRealValue(r)) {
    double r1 = exp(r);

    return CExprInst->createRealValue(r1);
  }
  else
    return CExprValuePtr();
}

#define CEXPR_REAL_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r = 0.0; \
  if (values[0]->getRealValue(r)) { \
  } \
  else \
    return CExprValuePtr(); \
  double r1 = F(r); \
  return CExprInst->createRealValue(r1); \
}

#define CEXPR_REALC_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r = 0.0; \
  std::complex<double> c; \
  if (values[0]->getComplexValue(c)) { \
    r = c.real(); \
  } \
  else if (values[0]->getRealValue(r)) { \
  } \
  else \
    return CExprValuePtr(); \
  double r1 = F(r); \
  return CExprInst->createRealValue(r1); \
}

#define CEXPR_REALC_TO_REALC_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r; \
  if (values[0]->isComplexValue()) { \
    std::complex<double> c; \
    if (! values[0]->getComplexValue(c)) return CExprValuePtr(); \
    errno = 0; \
    std::complex<double> c1 = F(c); \
    if (errno != 0) return CExprValuePtr(); \
    return CExprInst->createComplexValue(c1); \
  } \
  else if (values[0]->getRealValue(r)) { \
    double r1 = F(r); \
    return CExprInst->createRealValue(r1); \
  } \
  else \
    return CExprValuePtr(); \
}

#define CEXPR_ANGLE_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) { \
    if (CExprInst->getDegrees()) \
      real = DegToRad(real); \
    double real1 = F(real); \
    return CExprInst->createRealValue(real1); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_REALC_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r; \
  std::complex<double> c; \
  if (values[0]->getComplexValue(c)) { \
    errno = 0; \
    std::complex<double> c1 = F(c); \
    if (errno != 0) return CExprValuePtr(); \
    if (CExprInst->getDegrees()) \
      c1 = std::complex<double>(RadToDeg(c1.real()), RadToDeg(c1.imag())); \
    return CExprInst->createComplexValue(c1); \
  } \
  else if (values[0]->getRealValue(r)) { \
    errno = 0; \
    double r1 = F(r); \
    if (errno != 0) return CExprValuePtr(); \
    if (CExprInst->getDegrees()) \
      r1 = RadToDeg(r1); \
    return CExprInst->createRealValue(r1); \
  } \
  else \
    return CExprValuePtr(); \
}

#define CEXPR_REAL2_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 2); \
  double real1, real2; \
  if (values[0]->getRealValue(real1) && values[1]->getRealValue(real2)) { \
    double real = F(real1, real2); \
    if (CExprInst->getDegrees()) \
      real = RadToDeg(real); \
    return CExprInst->createRealValue(real); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_COMPLEX_TO_COMPLEX_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprValueArray &values) { \
  assert(values.size() == 1); \
  std::complex<double> c; \
  if (! values[0]->getComplexValue(c)) { \
    double r; \
    if (! values[0]->getRealValue(r)) \
      return CExprValuePtr(); \
    c = std::complex<double>(r, 0); \
  } \
  std::complex<double> c1 = F(c); \
  return CExprInst->createComplexValue(c1); \
}

class CExprFunctionAbs : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    if      (values[0]->isRealValue()) {
      double real;
      if (values[0]->getRealValue(real))
        return CExprInst->createRealValue(std::abs(real));
    }
    else if (values[0]->isIntegerValue()) {
      long integer;
      if (values[0]->getIntegerValue(integer))
        return CExprInst->createIntegerValue(std::abs(integer));
    }
    else if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return CExprInst->createRealValue(std::abs(c));
    }
    return CExprValuePtr();
  }
};

class CExprFunctionCArg : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c)) {
        double r = std::arg(c);
        if (CExprInst->getDegrees())
          r = DegToRad(r);
        return CExprInst->createRealValue(r);
      }
    }
    return CExprValuePtr();
  }
};

class CExprFunctionImag : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return CExprInst->createRealValue(c.imag());
    }
    return CExprValuePtr();
  }
};

class CExprFunctionSign : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    if      (values[0]->isRealValue()) {
      double real;
      if (values[0]->getRealValue(real))
        return CExprInst->createIntegerValue(real >= 0 ? (real == 0 ? 0 : 1) : -1);
    }
    else if (values[0]->isIntegerValue()) {
      long integer;
      if (values[0]->getIntegerValue(integer))
        return CExprInst->createIntegerValue(integer >= 0 ? (integer == 0 ? 0 : 1) : -1);
    }
    else if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return CExprInst->createIntegerValue(c.real() >= 0 ? (c.real() == 0 ? 0 : 1) : -1);
    }
    return CExprValuePtr();
  }
};

class CExprFunctionExpr : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);

    std::string expr;

    if (! values[0]->getStringValue(expr))
      return CExprValuePtr();

    CExprInst->saveCompileState();

    CExprValuePtr value;

    if (! CExprInst->evaluateExpression(expr, value))
      value = CExprValuePtr();

    CExprInst->restoreCompileState();

    return value;
  }
};

#ifdef GNUPLOT_EXPR
class CExprFunctionRand : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isIntegerValue()) {
      long integer = 0;
      (void) values[0]->getIntegerValue(integer);
      if     (integer < 0)
        srand(0);
      else if (integer > 0)
        srand(integer);
      double r = (1.0*rand())/RAND_MAX;
      return CExprInst->createRealValue(r);
    }
    return CExprValuePtr();
  }
};
#endif

#define CEXPR_REALC_TO_REAL_FOBJ(NAME, F) \
class CExprFunction##NAME : public CExprFunctionObj { \
 public: \
  CExprValuePtr operator()(const CExprValueArray &values) { \
    assert(values.size() == 1); \
    double r = 0.0; \
    if      (values[0]->isRealValue()) { \
      if (! values[0]->getRealValue(r)) return CExprValuePtr(); \
    } \
    else if (values[0]->isIntegerValue()) { \
      long i = 0; \
      if (! values[0]->getIntegerValue(i)) return CExprValuePtr(); \
      r = i; \
    } \
    else if (values[0]->isComplexValue()) { \
      std::complex<double> c; \
      if (! values[0]->getComplexValue(c)) return CExprValuePtr(); \
      r = c.real(); \
    } \
    else { \
      return CExprValuePtr(); \
    } \
    double r1 = F(r); \
    return CExprInst->createRealValue(r1); \
  } \
};

CEXPR_REALC_TO_REAL_FOBJ(Ceil , std::ceil)
CEXPR_REALC_TO_REAL_FOBJ(Floor, std::floor)
CEXPR_REALC_TO_REAL_FOBJ(Int  , static_cast<int>)
CEXPR_REALC_TO_REAL_FOBJ(Real , static_cast<double>)

#ifdef GNUPLOT_EXPR
class CExprFunctionSPrintF : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() >= 1);

    std::string fmt;

    if (! values[0]->getStringValue(fmt))
      return CExprValuePtr();

    CExprValueArray values1;

    for (uint i = 1; i < values.size(); ++i)
      values1.push_back(values[i]);

    std::string res = CExprInst->printf(fmt, values1);

    return CExprInst->createStringValue(res);
  }
};
#endif

template<typename T, typename R, typename FUNC>
class CExprFunctionObjT1 : public CExprFunctionObj {
 public:
  CExprFunctionObjT1(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T>::argTypeStr();

    mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 1);
    T v;
    if (CExprUtil<T>::getTypeValue(values[0], v))
      return CExprUtil<R>::createValue(f_(v));
    return CExprValuePtr();
  }

 private:
  FUNC f_;
};

template<typename T1, typename T2, typename R, typename FUNC>
class CExprFunctionObjT2 : public CExprFunctionObj {
 public:
  CExprFunctionObjT2(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T1>::argTypeStr() + "," + CExprUtil<T2>::argTypeStr();

    mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 2);
    T1 v1; T2 v2;
    if (CExprUtil<T1>::getTypeValue(values[0], v1) &&
        CExprUtil<T2>::getTypeValue(values[1], v2))
      return CExprUtil<R>::createValue(f_(v1, v2));
    return CExprValuePtr();
  }

 private:
  FUNC f_;
};

template<typename T1, typename T2, typename T3, typename R, typename FUNC>
class CExprFunctionObjT3 : public CExprFunctionObj {
 public:
  CExprFunctionObjT3(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T1>::argTypeStr() + "," +
      CExprUtil<T2>::argTypeStr() + "," + CExprUtil<T3>::argTypeStr();

    mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(const CExprValueArray &values) {
    assert(values.size() == 3);
    T1 v1; T2 v2; T3 v3;
    if (CExprUtil<T1>::getTypeValue(values[0], v1) &&
        CExprUtil<T2>::getTypeValue(values[1], v2) &&
        CExprUtil<T2>::getTypeValue(values[2], v3))
      return CExprUtil<R>::createValue(f_(v1, v2, v3));
    return CExprValuePtr();
  }

 private:
  FUNC f_;
};

CEXPR_REALC_TO_REALC_FUNC(Log  , std::log)
CEXPR_REALC_TO_REALC_FUNC(Log10, std::log10)

CEXPR_ANGLE_TO_REAL_FUNC(Sin, ::sin)
CEXPR_ANGLE_TO_REAL_FUNC(Cos, ::cos)
CEXPR_ANGLE_TO_REAL_FUNC(Tan, ::tan)

CEXPR_REALC_TO_ANGLE_FUNC(ACos, std::acos)
CEXPR_REALC_TO_ANGLE_FUNC(ASin, std::asin)
CEXPR_REALC_TO_ANGLE_FUNC(ATan, std::atan)

CEXPR_REAL2_TO_ANGLE_FUNC(ATan2, ::atan2)

CEXPR_REALC_TO_REALC_FUNC(SinH , std::sinh)
CEXPR_REALC_TO_REALC_FUNC(CosH , std::cosh)
CEXPR_REALC_TO_REALC_FUNC(TanH , std::tanh)
CEXPR_REALC_TO_REALC_FUNC(ASinH, std::asinh)
CEXPR_REALC_TO_REALC_FUNC(ACosH, std::acosh)
CEXPR_REALC_TO_REALC_FUNC(ATanH, std::atanh)

#ifdef GNUPLOT_EXPR
// TODO: besy0, besy1
CEXPR_REAL_TO_REAL_FUNC(BesJ0  , ::j0)
CEXPR_REAL_TO_REAL_FUNC(BesJ1  , ::j1)

CEXPR_REALC_TO_REAL_FUNC(Erf    , ::erf)
CEXPR_REALC_TO_REAL_FUNC(ErfC   , ::erfc)

CEXPR_COMPLEX_TO_COMPLEX_FUNC(CErf , ::cerf)
CEXPR_COMPLEX_TO_COMPLEX_FUNC(CErfC, ::cerfc)

// TODO: inverf, invnorm, norm
CEXPR_REALC_TO_REAL_FUNC(Gamma  , ::gamma)
// TODO: igamma
CEXPR_REALC_TO_REAL_FUNC(LGamma , ::lgamma)
CEXPR_REALC_TO_REAL_FUNC(InvNorm, ::invnorm)
// TODO: lambertw
#endif

static CExprBuiltinFunction
builtinFns[] = {
  { "sqrt"   , "r"  , CExprFunctionSqrt    },
  { "exp"    , "rc" , CExprFunctionExp     },
  { "log"    , "rc" , CExprFunctionLog     },
  { "log10"  , "rc" , CExprFunctionLog10   },
  { "sin"    , "rc" , CExprFunctionSin     },
  { "cos"    , "rc" , CExprFunctionCos     },
  { "tan"    , "rc" , CExprFunctionTan     },
  { "asin"   , "rc" , CExprFunctionASin    },
  { "acos"   , "rc" , CExprFunctionACos    },
  { "atan"   , "rc" , CExprFunctionATan    },
  { "atan2"  , "r,r", CExprFunctionATan2   },
  { "sinh"   , "rc" , CExprFunctionSinH    },
  { "cosh"   , "rc" , CExprFunctionCosH    },
  { "tanh"   , "rc" , CExprFunctionTanH    },
  { "asinh"  , "rc" , CExprFunctionASinH   },
  { "acosh"  , "rc" , CExprFunctionACosH   },
  { "atanh"  , "rc" , CExprFunctionATanH   },
#ifdef GNUPLOT_EXPR
  // EllipticK, EllipticE, EllipticPi
  { "besj0"  , "r"  , CExprFunctionBesJ0   },
  { "besj1"  , "r"  , CExprFunctionBesJ1   },
  // besy0, besy1
  { "erf"    , "rc" , CExprFunctionErf     },
  { "erfc"   , "rc" , CExprFunctionErfC    },
  { "cerf"   , "c"  , CExprFunctionCErf    },
  { "cerfc"  , "c"  , CExprFunctionCErfC   },
  { "gamma"  , "r"  , CExprFunctionGamma   },
  { "lgamma" , "r"  , CExprFunctionLGamma  },
  { "invnorm", "r"  , CExprFunctionInvNorm },
#endif
  { "", "", 0 }
};

//------

CExprFunctionMgr::
CExprFunctionMgr(CExpr *expr) :
 expr_(expr)
{
}

CExprFunctionMgr::
~CExprFunctionMgr()
{
}

void
CExprFunctionMgr::
addFunctions()
{
  for (uint i = 0; builtinFns[i].proc; ++i) {
    CExprFunctionPtr function =
      addProcFunction(builtinFns[i].name, builtinFns[i].args, builtinFns[i].proc);

    function->setBuiltin(true);
  }

  addObjFunction("abs"  , "ric", new CExprFunctionAbs);
  addObjFunction("arg"  , "c"  , new CExprFunctionCArg);
  addObjFunction("ceil" , "rc" , new CExprFunctionCeil);
  addObjFunction("floor", "rc" , new CExprFunctionFloor);
  addObjFunction("int"  , "ric", new CExprFunctionInt); // TODO: use conversion rules
  addObjFunction("real" , "ric", new CExprFunctionReal); // TODO: use conversion rules
  addObjFunction("imag" , "c"  , new CExprFunctionImag);
  addObjFunction("sgn"  , "ric", new CExprFunctionSign);

#ifdef GNUPLOT_EXPR
  addObjFunction("rand", "i", new CExprFunctionRand);

  // input types ..., return type, function
  new CExprFunctionObjT1<std::string,long,CExprStrLen>                 (this, "strlen");
  new CExprFunctionObjT2<std::string,std::string,long,CExprStrStrT>    (this, "strstrt");
  new CExprFunctionObjT3<std::string,long,long,std::string,CExprSubStr>(this, "substr");
  new CExprFunctionObjT1<std::string,std::string,CExprSystem>          (this, "system");
  new CExprFunctionObjT2<std::string,long,std::string,CExprWord>       (this, "word");
  new CExprFunctionObjT1<std::string,long,CExprWords>                  (this, "words");

  // gprintf ?
  addObjFunction("sprintf", "s,...", new CExprFunctionSPrintF);
#endif

  addObjFunction("expr", "s", new CExprFunctionExpr);
}

CExprFunctionPtr
CExprFunctionMgr::
getFunction(const std::string &name)
{
  for (const auto &func : functions_)
    if (func->name() == name)
      return func;

  return CExprFunctionPtr();
}

CExprFunctionPtr
CExprFunctionMgr::
addProcFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc)
{
  Args args;
  bool variableArgs;

  (void) parseArgs(argsStr, args, variableArgs);

  CExprFunctionPtr function(new CExprProcFunction(name, args, proc));

  function->setVariableArgs(variableArgs);

  removeFunction(name);

  functions_.push_back(function);

  resetCompiled();

  return function;
}

CExprFunctionPtr
CExprFunctionMgr::
addObjFunction(const std::string &name, const std::string &argsStr, CExprFunctionObj *proc)
{
  Args args;
  bool variableArgs;

  (void) parseArgs(argsStr, args, variableArgs);

  CExprFunctionPtr function(new CExprObjFunction(name, args, proc));

  function->setVariableArgs(variableArgs);

  removeFunction(name);

  functions_.push_back(function);

  resetCompiled();

  return function;
}

CExprFunctionPtr
CExprFunctionMgr::
addUserFunction(const std::string &name, const std::vector<std::string> &args,
                const std::string &proc)
{
  CExprFunctionPtr function(new CExprUserFunction(name, args, proc));

  removeFunction(name);

  functions_.push_back(function);

  resetCompiled();

  return function;
}

void
CExprFunctionMgr::
removeFunction(const std::string &name)
{
  removeFunction(getFunction(name));
}

void
CExprFunctionMgr::
removeFunction(CExprFunctionPtr function)
{
  if (function.isValid())
    functions_.remove(function);
}

void
CExprFunctionMgr::
getFunctionNames(std::vector<std::string> &names) const
{
  for (const auto &func : functions_)
    names.push_back(func->name());
}

void
CExprFunctionMgr::
resetCompiled()
{
  for (const auto &func : functions_)
    func->reset();
}

bool
CExprFunctionMgr::
parseArgs(const std::string &argsStr, Args &args, bool &variableArgs)
{
  variableArgs = false;

  bool rc = true;

  std::vector<std::string> args1;

  CStrUtil::addTokens(argsStr, args1, ", ");

  uint num_args = args1.size();

  for (uint i = 0; i < num_args; ++i) {
    const std::string &arg = args1[i];

    if (arg == "..." && i == num_args -1) {
      variableArgs = true;
      break;
    }

    uint types = CEXPR_VALUE_NONE;

    uint len = arg.size();

    for (uint j = 0; j < len; j++) {
      char c = arg[j];

      if      (c == 'b') types |= CEXPR_VALUE_BOOLEAN;
      else if (c == 'i') types |= CEXPR_VALUE_INTEGER;
      else if (c == 'r') types |= CEXPR_VALUE_REAL;
      else if (c == 's') types |= CEXPR_VALUE_STRING;
      else if (c == 'c') types |= CEXPR_VALUE_COMPLEX;
      else if (c == 'n') types |= CEXPR_VALUE_NULL;
      else {
        CExprInst->errorMsg("Invalid argument type char '" + std::string(&c, 1) + "'");
        rc = false;
      }
    }

    args.push_back(CExprFunctionArg((CExprValueType) types));
  }

  return rc;
}

//----------

CExprProcFunction::
CExprProcFunction(const std::string &name, const Args &args, CExprFunctionProc proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}

bool
CExprProcFunction::
checkValues(const CExprValueArray &values) const
{
  return (values.size() == numArgs());
}

CExprValuePtr
CExprProcFunction::
exec(const CExprValueArray &values)
{
  assert(checkValues(values));

  return (*proc_)(values);
}

//----------

CExprObjFunction::
CExprObjFunction(const std::string &name, const Args &args, CExprFunctionObj *proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}

CExprObjFunction::
~CExprObjFunction()
{
  delete proc_;
}

bool
CExprObjFunction::
checkValues(const CExprValueArray &values) const
{
  if (isVariableArgs())
    return (values.size() >= numArgs());
  else
    return (values.size() == numArgs());
}

CExprValuePtr
CExprObjFunction::
exec(const CExprValueArray &values)
{
  assert(checkValues(values));

  return (*proc_)(values);
}

//----------

CExprUserFunction::
CExprUserFunction(const std::string &name, const Args &args, const std::string &proc) :
 CExprFunction(name), args_(args), proc_(proc), compiled_(false)
{
}

bool
CExprUserFunction::
checkValues(const CExprValueArray &values) const
{
  return (values.size() >= numArgs());
}

void
CExprUserFunction::
reset()
{
  compiled_ = false;

  pstack_.clear();
  cstack_.clear();

  itoken_ = CExprITokenPtr();
}

CExprValuePtr
CExprUserFunction::
exec(const CExprValueArray &values)
{
  assert(checkValues(values));

  //---

  if (! compiled_) {
    pstack_ = CExprInst->parseLine(proc_);
    itoken_ = CExprInst->interpPTokenStack(pstack_);
    cstack_ = CExprInst->compileIToken(itoken_);

    compiled_ = true;
  }

  //---

  typedef std::map<std::string,CExprValuePtr> VarValues;

  VarValues varValues;

  // set arg values (save previous values)
  for (uint i = 0; i < numArgs(); ++i) {
    const std::string &arg = args_[i];

    CExprVariablePtr var = CExprInst->getVariable(arg);

    if (var.isValid()) {
      varValues[arg] = var->getValue();

      var->setValue(values[i]);
    }
    else {
      varValues[arg] = CExprValuePtr();

      CExprInst->createVariable(arg, values[i]);
    }
  }

  // run proc
  CExprInst->saveCompileState();

  CExprValuePtr value;

//if (! CExprInst->evaluateExpression(proc_, value))
//  value = CExprValuePtr();
//if (! CExprInst->executePTokenStack(pstack_, value))
//  value = CExprValuePtr();
  if (! CExprInst->executeCTokenStack(cstack_, value))
    value = CExprValuePtr();

  CExprInst->restoreCompileState();

  // restore variables
  for (const auto &v : varValues) {
    const std::string varName = v.first;
    CExprValuePtr     value   = v.second;

    if (value.isValid()) {
      CExprVariablePtr var = CExprInst->getVariable(varName);

      var->setValue(value);
    }
    else
      CExprInst->removeVariable(varName);
  }

  return value;
}
