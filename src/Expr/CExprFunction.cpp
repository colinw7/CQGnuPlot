#include <CExprI.h>
#include <boost/math/special_functions/erf.hpp>
#include <CMathGen.h>
#include <CInvNorm.h>
#include <COSNaN.h>

#include <cmath>
#include <ccomplex>
#include <cstdlib>
#include <cstring>

// NOTE: types are only needed if normal conversion rules don't handle the type correctly

#ifdef GNUPLOT_EXPR
namespace {

double invnorm(double x) {
  //return sqrt(2)/erf(2*x - 1);
  double y = CInvNorm::calc(x);
  if (COSNaN::is_pos_inf(y) || COSNaN::is_neg_inf(y))
    COSNaN::set_nan(y);
  return y;
}

double norm(double x)
{
  x = 0.5*sqrt(2)*x;
  x = 0.5*erfc(-x);

  return x;
}

double inverf(double x) {
  try {
    return boost::math::erf_inv(x);
  } catch (...) {
    return CMathGen::getNaN();
  }
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
CExprFunctionSqrt(CExpr *expr, const CExprValueArray &values)
{
  assert(values.size() == 1);

  double real;

  if (! values[0]->getRealValue(real))
    return CExprValuePtr();

  if (real >= 0.0) {
    double real1 = ::sqrt(real);

    return expr->createRealValue(real1);
  }
  else {
    double real1 = ::sqrt(-real);

    return expr->createComplexValue(std::complex<double>(0, real1));
  }
}

static CExprValuePtr
CExprFunctionExp(CExpr *expr, const CExprValueArray &values)
{
  assert(values.size() == 1);

  double               r;
  std::complex<double> c;

  if      (values[0]->isComplexValue()) {
    if (values[0]->getComplexValue(c)) {
      double r1 = exp(c.real())*cos(c.imag());
      double c1 = exp(c.real())*sin(c.imag());

      return expr->createComplexValue(std::complex<double>(r1, c1));
    }
    else
      return CExprValuePtr();
  }
  else if (values[0]->getRealValue(r)) {
    double r1 = exp(r);

    return expr->createRealValue(r1);
  }
  else
    return CExprValuePtr();
}

#define CEXPR_REAL_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r = 0.0; \
  if (values[0]->getRealValue(r)) { \
  } \
  else \
    return CExprValuePtr(); \
  double r1 = F(r); \
  return expr->createRealValue(r1); \
}

#define CEXPR_REALC_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r = 0.0; \
  if (values[0]->isComplexValue()) { \
    std::complex<double> c; \
    if (values[0]->getComplexValue(c)) { \
      r = c.real(); \
    } \
    else \
      return CExprValuePtr(); \
  } \
  else if (values[0]->getRealValue(r)) { \
  } \
  else \
    return CExprValuePtr(); \
  double r1 = F(r); \
  return expr->createRealValue(r1); \
}

#define CEXPR_REALC_TO_REALC_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r; \
  if (values[0]->isComplexValue()) { \
    std::complex<double> c; \
    if (values[0]->getComplexValue(c)) { \
      errno = 0; \
      std::complex<double> c1 = F(c); \
      if (errno != 0) return CExprValuePtr(); \
      return expr->createComplexValue(c1); \
    } \
      return CExprValuePtr(); \
  } \
  else if (values[0]->getRealValue(r)) { \
    double r1 = F(r); \
    return expr->createRealValue(r1); \
  } \
  else \
    return CExprValuePtr(); \
}

#define CEXPR_ANGLE_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) { \
    if (expr->getDegrees()) \
      real = DegToRad(real); \
    double real1 = F(real); \
    return expr->createRealValue(real1); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_REALC_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  double r; \
  if (values[0]->isComplexValue()) { \
    std::complex<double> c; \
    if (values[0]->getComplexValue(c)) { \
      errno = 0; \
      std::complex<double> c1 = F(c); \
      if (errno != 0) return CExprValuePtr(); \
      if (expr->getDegrees()) \
        c1 = std::complex<double>(RadToDeg(c1.real()), RadToDeg(c1.imag())); \
      return expr->createComplexValue(c1); \
    } \
    else \
      return CExprValuePtr(); \
  } \
  else if (values[0]->getRealValue(r)) { \
    errno = 0; \
    double r1 = F(r); \
    if (errno == 0) { \
      if (expr->getDegrees()) \
        r1 = RadToDeg(r1); \
      return expr->createRealValue(r1); \
    } \
    else if (errno == EDOM) { \
      std::complex<double> c(r,0); \
      errno = 0; \
      std::complex<double> c1 = F(c); \
      if (errno != 0) return CExprValuePtr(); \
      if (expr->getDegrees()) \
        c1 = std::complex<double>(RadToDeg(c1.real()), RadToDeg(c1.imag())); \
      return expr->createComplexValue(c1); \
    } \
    else { \
      return CExprValuePtr(); \
    } \
  } \
  else \
    return CExprValuePtr(); \
}

#define CEXPR_REAL2_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 2); \
  double real1, real2; \
  if (values[0]->getRealValue(real1) && values[1]->getRealValue(real2)) { \
    double real = F(real1, real2); \
    if (expr->getDegrees()) \
      real = RadToDeg(real); \
    return expr->createRealValue(real); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_COMPLEX_TO_COMPLEX_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(CExpr *expr, const CExprValueArray &values) { \
  assert(values.size() == 1); \
  std::complex<double> c; \
  if (! values[0]->getComplexValue(c)) { \
    double r; \
    if (! values[0]->getRealValue(r)) \
      return CExprValuePtr(); \
    c = std::complex<double>(r, 0); \
  } \
  std::complex<double> c1 = F(c); \
  return expr->createComplexValue(c1); \
}

class CExprFunctionAbs : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    if      (values[0]->isRealValue()) {
      double real;
      if (values[0]->getRealValue(real))
        return expr->createRealValue(std::abs(real));
    }
    else if (values[0]->isIntegerValue()) {
      long integer;
      if (values[0]->getIntegerValue(integer))
        return expr->createIntegerValue(std::abs(integer));
    }
    else if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return expr->createRealValue(std::abs(c));
    }
    return CExprValuePtr();
  }
};

class CExprFunctionCArg : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c)) {
        double r = std::arg(c);
        if (expr->getDegrees())
          r = DegToRad(r);
        return expr->createRealValue(r);
      }
    }
    return CExprValuePtr();
  }
};

class CExprFunctionImag : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return expr->createRealValue(c.imag());
    }
    return CExprValuePtr();
  }
};

class CExprFunctionSign : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    if      (values[0]->isRealValue()) {
      double real;
      if (values[0]->getRealValue(real))
        return expr->createIntegerValue(real >= 0 ? (real == 0 ? 0 : 1) : -1);
    }
    else if (values[0]->isIntegerValue()) {
      long integer;
      if (values[0]->getIntegerValue(integer))
        return expr->createIntegerValue(integer >= 0 ? (integer == 0 ? 0 : 1) : -1);
    }
    else if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (values[0]->getComplexValue(c))
        return expr->createIntegerValue(c.real() >= 0 ? (c.real() == 0 ? 0 : 1) : -1);
    }
    return CExprValuePtr();
  }
};

class CExprFunctionExpr : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);

    std::string exprStr;

    if (! values[0]->getStringValue(exprStr))
      return CExprValuePtr();

    expr->saveCompileState();

    CExprValuePtr value;

    if (! expr->evaluateExpression(exprStr, value))
      value = CExprValuePtr();

    expr->restoreCompileState();

    return value;
  }
};

#ifdef GNUPLOT_EXPR
class CExprFunctionRand : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    if (values[0]->isIntegerValue()) {
      long integer = 0;
      (void) values[0]->getIntegerValue(integer);
      if     (integer < 0)
        srand(0);
      else if (integer > 0)
        srand(integer);
      double r = (1.0*rand())/RAND_MAX;
      return expr->createRealValue(r);
    }
    return CExprValuePtr();
  }
};
#endif

#define CEXPR_REALC_TO_REAL_FOBJ(NAME, F) \
class CExprFunction##NAME : public CExprFunctionObj { \
 public: \
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) { \
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
    return expr->createRealValue(r1); \
  } \
};

class CExprFunctionInt : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    double r = 0.0;
    if      (values[0]->isRealValue()) {
      if (! values[0]->getRealValue(r)) return CExprValuePtr();
    }
    else if (values[0]->isIntegerValue()) {
      long i = 0;
      if (! values[0]->getIntegerValue(i)) return CExprValuePtr();
      r = i;
    }
    else if (values[0]->isComplexValue()) {
      std::complex<double> c;
      if (! values[0]->getComplexValue(c)) return CExprValuePtr();
      r = c.real();
    }
    else {
      return CExprValuePtr();
    }
    int i1 = static_cast<int>(r);
    return expr->createIntegerValue(i1);
  }
};

CEXPR_REALC_TO_REAL_FOBJ(Ceil , std::ceil)
CEXPR_REALC_TO_REAL_FOBJ(Floor, std::floor)
CEXPR_REALC_TO_REAL_FOBJ(Real , static_cast<double>)

#ifdef GNUPLOT_EXPR
class CExprFunctionSPrintF : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() >= 1);

    std::string fmt;

    if (! values[0]->getStringValue(fmt))
      return CExprValuePtr();

    CExprValueArray values1;

    for (uint i = 1; i < values.size(); ++i)
      values1.push_back(values[i]);

    std::string res = expr->printf(fmt, values1);

    return expr->createStringValue(res);
  }
};
#endif

template<typename T, typename R, typename FUNC>
class CExprFunctionObjT1 : public CExprFunctionObj {
 public:
  CExprFunctionObjT1(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T>::argTypeStr();

    func_ = mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 1);
    T v;
    if (CExprUtil<T>::getTypeValue(values[0], v))
      return CExprUtil<R>::createValue(expr, f_(v));
    return CExprValuePtr();
  }

  void setBuiltin(bool b) {
    func_->setBuiltin(b);
  }

 private:
  FUNC             f_;
  CExprFunctionPtr func_;
};

template<typename T1, typename T2, typename R, typename FUNC>
class CExprFunctionObjT2 : public CExprFunctionObj {
 public:
  CExprFunctionObjT2(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T1>::argTypeStr() + "," + CExprUtil<T2>::argTypeStr();

    func_ = mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 2);
    T1 v1; T2 v2;
    if (CExprUtil<T1>::getTypeValue(values[0], v1) &&
        CExprUtil<T2>::getTypeValue(values[1], v2))
      return CExprUtil<R>::createValue(expr, f_(v1, v2));
    return CExprValuePtr();
  }

  void setBuiltin(bool b) {
    func_->setBuiltin(b);
  }

 private:
  FUNC             f_;
  CExprFunctionPtr func_;
};

template<typename T1, typename T2, typename T3, typename R, typename FUNC>
class CExprFunctionObjT3 : public CExprFunctionObj {
 public:
  CExprFunctionObjT3(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T1>::argTypeStr() + "," +
      CExprUtil<T2>::argTypeStr() + "," + CExprUtil<T3>::argTypeStr();

    func_ = mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(CExpr *expr, const CExprValueArray &values) {
    assert(values.size() == 3);
    T1 v1; T2 v2; T3 v3;
    if (CExprUtil<T1>::getTypeValue(values[0], v1) &&
        CExprUtil<T2>::getTypeValue(values[1], v2) &&
        CExprUtil<T2>::getTypeValue(values[2], v3))
      return CExprUtil<R>::createValue(expr, f_(v1, v2, v3));
    return CExprValuePtr();
  }

  void setBuiltin(bool b) {
    func_->setBuiltin(b);
  }

 private:
  FUNC             f_;
  CExprFunctionPtr func_;
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

CEXPR_REALC_TO_REAL_FUNC(Erf   , ::erf)
CEXPR_REALC_TO_REAL_FUNC(ErfC  , ::erfc)
CEXPR_REALC_TO_REAL_FUNC(InvErf, ::inverf)

CEXPR_COMPLEX_TO_COMPLEX_FUNC(CErf , ::cerf)
CEXPR_COMPLEX_TO_COMPLEX_FUNC(CErfC, ::cerfc)

// TODO: invnorm, norm
CEXPR_REALC_TO_REAL_FUNC(Gamma  , ::gamma)
// TODO: igamma
CEXPR_REALC_TO_REAL_FUNC(LGamma , ::lgamma)
CEXPR_REALC_TO_REAL_FUNC(Norm   , ::norm)
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
  { "inverf" , "rc" , CExprFunctionInvErf  },
  { "cerf"   , "c"  , CExprFunctionCErf    },
  { "cerfc"  , "c"  , CExprFunctionCErfC   },
  { "gamma"  , "r"  , CExprFunctionGamma   },
  { "lgamma" , "r"  , CExprFunctionLGamma  },
#endif
  { "norm"   , "r"  , CExprFunctionNorm    },
  { "invnorm", "r"  , CExprFunctionInvNorm },
  { "", "", 0 }
};

//------

CExprFunctionMgr::
CExprFunctionMgr(CExpr *expr) :
 expr_(expr)
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

  addObjFunction("abs"  , "ric", new CExprFunctionAbs  )->setBuiltin(true);
  addObjFunction("arg"  , "c"  , new CExprFunctionCArg )->setBuiltin(true);
  addObjFunction("ceil" , "rc" , new CExprFunctionCeil )->setBuiltin(true);
  addObjFunction("floor", "rc" , new CExprFunctionFloor)->setBuiltin(true);

  // TODO: use conversion rules
  addObjFunction("int"  , "ric", new CExprFunctionInt  )->setBuiltin(true);
  addObjFunction("real" , "ric", new CExprFunctionReal )->setBuiltin(true);
  addObjFunction("imag" , "c"  , new CExprFunctionImag )->setBuiltin(true);

  addObjFunction("sgn"  , "ric", new CExprFunctionSign )->setBuiltin(true);

#ifdef GNUPLOT_EXPR
  addObjFunction("rand", "i", new CExprFunctionRand)->setBuiltin(true);

  // input types ..., return type, function
  (new CExprFunctionObjT1<std::string,long,CExprStrLen>
         (this, "strlen" ))->setBuiltin(true);
  (new CExprFunctionObjT2<std::string,std::string,long,CExprStrStrT>
         (this, "strstrt"))->setBuiltin(true);
  (new CExprFunctionObjT3<std::string,long,long,std::string,CExprSubStr>
         (this, "substr" ))->setBuiltin(true);
  (new CExprFunctionObjT1<std::string,std::string,CExprSystem>
         (this, "system" ))->setBuiltin(true);
  (new CExprFunctionObjT2<std::string,long,std::string,CExprWord>
         (this, "word"   ))->setBuiltin(true);
  (new CExprFunctionObjT1<std::string,long,CExprWords>
         (this, "words"  ))->setBuiltin(true);

  // gprintf ?
  addObjFunction("sprintf", "s,...", new CExprFunctionSPrintF)->setBuiltin(true);
#endif

  addObjFunction("expr", "s", new CExprFunctionExpr)->setBuiltin(true);
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

void
CExprFunctionMgr::
getFunctions(const std::string &name, Functions &functions)
{
  for (const auto &func : functions_)
    if (func->name() == name)
      functions.push_back(func);
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

  resetCompiled(name);

  return function;
}

CExprFunctionPtr
CExprFunctionMgr::
addObjFunction(const std::string &name, const std::string &argsStr,
               CExprFunctionObj *proc, bool resetCompiled)
{
  Args args;
  bool variableArgs;

  (void) parseArgs(argsStr, args, variableArgs);

  CExprFunctionPtr function(new CExprObjFunction(name, args, proc));

  function->setVariableArgs(variableArgs);

  if (! proc->isOverload())
    removeFunction(name);

  functions_.push_back(function);

  if (resetCompiled)
    this->resetCompiled(name);

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

  resetCompiled(name);

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
resetCompiled(const std::string &name)
{
  for (const auto &func : functions_) {
    if (func->hasFunction(name))
      func->reset();
  }
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

    if (arg == "..." && i == num_args - 1) {
      variableArgs = true;
      break;
    }

    uint types = uint(CExprValueType::NONE);

    uint len = arg.size();

    for (uint j = 0; j < len; j++) {
      char c = arg[j];

      if      (c == 'b') types |= uint(CExprValueType::BOOLEAN);
      else if (c == 'i') types |= uint(CExprValueType::INTEGER);
      else if (c == 'r') types |= uint(CExprValueType::REAL);
      else if (c == 's') types |= uint(CExprValueType::STRING);
      else if (c == 'c') types |= uint(CExprValueType::COMPLEX);
      else if (c == 'n') types |= uint(CExprValueType::NUL);
      else {
        CExpr::instance()->
          errorMsg("Invalid argument type char '" + std::string(&c, 1) + "'");
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
exec(CExpr *expr, const CExprValueArray &values)
{
  assert(checkValues(values));

  return (*proc_)(expr, values);
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
exec(CExpr *expr, const CExprValueArray &values)
{
  assert(checkValues(values));

  return (*proc_)(expr, values);
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
exec(CExpr *expr, const CExprValueArray &values)
{
  assert(checkValues(values));

  //---

  if (! compiled_) {
    pstack_ = expr->parseLine(proc_);
    itoken_ = expr->interpPTokenStack(pstack_);
    cstack_ = expr->compileIToken(itoken_);

    compiled_ = true;
  }

  //---

  typedef std::map<std::string,CExprValuePtr> VarValues;

  VarValues varValues;

  // set arg values (save previous values)
  for (uint i = 0; i < numArgs(); ++i) {
    const std::string &arg = args_[i];

    CExprVariablePtr var = expr->getVariable(arg);

    if (var.isValid()) {
      varValues[arg] = var->getValue();

      var->setValue(values[i]);
    }
    else {
      varValues[arg] = CExprValuePtr();

      expr->createVariable(arg, values[i]);
    }
  }

  // run proc
  expr->saveCompileState();

  CExprValuePtr value;

//if (! expr->evaluateExpression(proc_, value))
//  value = CExprValuePtr();
//if (! expr->executePTokenStack(pstack_, value))
//  value = CExprValuePtr();
  if (! expr->executeCTokenStack(cstack_, value))
    value = CExprValuePtr();

  expr->restoreCompileState();

  // restore variables
  for (const auto &v : varValues) {
    const std::string varName = v.first;
    CExprValuePtr     vvalue  = v.second;

    if (vvalue.isValid()) {
      CExprVariablePtr var = expr->getVariable(varName);

      var->setValue(vvalue);
    }
    else
      expr->removeVariable(varName);
  }

  return value;
}
