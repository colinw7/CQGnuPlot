#include <CExprI.h>
#include <CPrintF.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

#ifdef GNUPLOT_EXPR
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
  int operator()(const std::string &str1, const std::string &str2) {
    for (uint i = 0; i < str1.size(); ++i)
      if (str2.find(str1[i]) != std::string::npos)
        return i + 1;
    return 0;
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
  int operator()(const std::string &str) {
    return system(str.c_str());
  }
};
#endif

struct CExprBuiltinFunction {
  const char        *name;
  const char        *args;
  CExprFunctionProc  proc;
};

#define CEXPR_REAL_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) \
    return CExprInst->createRealValue(F(real)); \
  return CExprValuePtr(); \
}

#define CEXPR_ANGLE_TO_REAL_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) { \
    if (CExprInst->getDegrees()) \
      real = M_PI*real/180.0; \
    return CExprInst->createRealValue(F(real)); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_REAL_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) { \
    real = F(real); \
    if (CExprInst->getDegrees()) \
      real = 180.0*real/M_PI; \
    return CExprInst->createRealValue(real); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_REAL2_TO_ANGLE_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 2); \
  double real1, real2; \
  if (values[0]->getRealValue(real1) && values[1]->getRealValue(real2)) { \
    double real = F(real1, real2); \
    if (CExprInst->getDegrees()) \
      real = 180.0*real/M_PI; \
    return CExprInst->createRealValue(real); \
  } \
  return CExprValuePtr(); \
}

class CExprFunctionAbs : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprFunction::Values &values) {
    assert(values.size() == 1);
    if      (values[0]->isRealValue()) {
      double real;
      if (values[0]->getRealValue(real))
        return CExprInst->createRealValue(::fabs(real));
    }
    else if (values[0]->isIntegerValue()) {
      long integer;
      if (values[0]->getIntegerValue(integer))
        return CExprInst->createIntegerValue(::abs(integer));
    }
    return CExprValuePtr();
  }
};

class CExprFunctionSign : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprFunction::Values &values) {
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
    return CExprValuePtr();
  }
};

#ifdef GNUPLOT_EXPR
class CExprFunctionRand : public CExprFunctionObj {
 public:
  CExprValuePtr operator()(const CExprFunction::Values &values) {
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

#define CEXPR_REAL_TO_REAL_FOBJ(NAME, F) \
class CExprFunction##NAME : public CExprFunctionObj { \
 public: \
  CExprValuePtr operator()(const CExprFunction::Values &values) { \
    assert(values.size() == 1); \
    if (values[0]->isRealValue()) { \
      double real; \
      if (values[0]->getRealValue(real)) \
        return CExprInst->createRealValue(F(real)); \
    } \
    return CExprValuePtr(); \
  } \
};

CEXPR_REAL_TO_REAL_FOBJ(Ceil , ::ceil)
CEXPR_REAL_TO_REAL_FOBJ(Floor, ::floor)
CEXPR_REAL_TO_REAL_FOBJ(Int  , static_cast<int>)

#ifdef GNUPLOT_EXPR
class CExprFunctionSPrintF : public CExprFunctionObj, public CPrintF {
 public:
  CExprValuePtr operator()(const CExprFunction::Values &values) {
    assert(values.size() >= 1);
    std::string s;
    if (! values[0]->getStringValue(s))
      return CExprValuePtr();
    setFormatString(s);

    values_ = values;
    iv_     = 1;

    return CExprInst->createStringValue(format());
  }

  int         getInt     () const { return getLong(); }
  long        getLongLong() const { return getLong(); }

  long getLong() const {
    long l = 0;
    if (iv_ < values_.size()) {
      values_[iv_]->getIntegerValue(l);
      ++iv_;
    }
    return l;
  }

  double getDouble() const {
    double r = 0.0;
    if (iv_ < values_.size()) {
      values_[iv_]->getRealValue(r);
      ++iv_;
    }
    return r;
  }

  std::string getString() const {
    std::string s;
    if (iv_ < values_.size()) {
      values_[iv_]->getStringValue(s);
      ++iv_;
    }
    return s;
  }

 private:
  CExprFunction::Values values_;
  mutable uint          iv_;
};
#endif

template<typename T, typename R, typename FUNC>
class CExprFunctionObjT1 : public CExprFunctionObj {
 public:
  CExprFunctionObjT1(CExprFunctionMgr *mgr, const std::string &name) {
    std::string argsStr = CExprUtil<T>::argTypeStr();

    mgr->addObjFunction(name, argsStr, this);
  }

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &values) {
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

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &values) {
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

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &values) {
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

CEXPR_REAL_TO_REAL_FUNC(Sqrt , ::sqrt)
CEXPR_REAL_TO_REAL_FUNC(Exp  , ::exp)
CEXPR_REAL_TO_REAL_FUNC(Log  , ::log)
CEXPR_REAL_TO_REAL_FUNC(Log10, ::log10)

CEXPR_ANGLE_TO_REAL_FUNC(Sin, ::sin)
CEXPR_ANGLE_TO_REAL_FUNC(Cos, ::cos)
CEXPR_ANGLE_TO_REAL_FUNC(Tan, ::tan)

CEXPR_REAL_TO_ANGLE_FUNC (ASin , ::asin )
CEXPR_REAL_TO_ANGLE_FUNC (ACos , ::acos )
CEXPR_REAL_TO_ANGLE_FUNC (ATan , ::atan )
CEXPR_REAL2_TO_ANGLE_FUNC(ATan2, ::atan2)

CEXPR_REAL_TO_REAL_FUNC(SinH , ::sinh)
CEXPR_REAL_TO_REAL_FUNC(CosH , ::cosh)
CEXPR_REAL_TO_REAL_FUNC(TanH , ::tanh)
CEXPR_REAL_TO_REAL_FUNC(ASinH, ::asinh)
CEXPR_REAL_TO_REAL_FUNC(ACosH, ::acosh)
CEXPR_REAL_TO_REAL_FUNC(ATanH, ::atanh)

#ifdef GNUPLOT_EXPR
// TODO: besj0, besy0, besj1, besy1
CEXPR_REAL_TO_REAL_FUNC(Erf   , ::erf)
CEXPR_REAL_TO_REAL_FUNC(ErfC  , ::erfc)
// TODO: inverf, invnorm, norm
CEXPR_REAL_TO_REAL_FUNC(Gamma , ::gamma)
// TODO: igamma
CEXPR_REAL_TO_REAL_FUNC(LGamma, ::lgamma)
// TODO: lambertw
// TODO: abs (complex), arg(complex), imag(complex), real(complex)
#endif

static CExprBuiltinFunction
builtinFns[] = {
  { "sqrt"  , "r"  , CExprFunctionSqrt   },
  { "exp"   , "r"  , CExprFunctionExp    },
  { "log"   , "r"  , CExprFunctionLog    },
  { "log10" , "r"  , CExprFunctionLog10  },
  { "sin"   , "r"  , CExprFunctionSin    },
  { "cos"   , "r"  , CExprFunctionCos    },
  { "tan"   , "r"  , CExprFunctionTan    },
  { "asin"  , "r"  , CExprFunctionASin   },
  { "acos"  , "r"  , CExprFunctionACos   },
  { "atan"  , "r"  , CExprFunctionATan   },
  { "atan2" , "r,r", CExprFunctionATan2  },
  { "sinh"  , "r"  , CExprFunctionSinH   },
  { "cosh"  , "r"  , CExprFunctionCosH   },
  { "tanh"  , "r"  , CExprFunctionTanH   },
  { "asinh" , "r"  , CExprFunctionASinH  },
  { "acosh" , "r"  , CExprFunctionACosH  },
  { "atanh" , "r"  , CExprFunctionATanH  },
#ifdef GNUPLOT_EXPR
  { "erf"   , "r"  , CExprFunctionErf    },
  { "erfc"  , "r"  , CExprFunctionErfC   },
  { "gamma" , "r"  , CExprFunctionGamma  },
  { "lgamma", "r"  , CExprFunctionLGamma },
#endif
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

  addObjFunction("abs"  , "ri", new CExprFunctionAbs);
  addObjFunction("ceil" , "r" , new CExprFunctionCeil);
  addObjFunction("floor", "r" , new CExprFunctionFloor);
  addObjFunction("int"  , "r" , new CExprFunctionInt);
  addObjFunction("sgn"  , "ri", new CExprFunctionSign);

#ifdef GNUPLOT_EXPR
  addObjFunction("rand", "i", new CExprFunctionRand);

  new CExprFunctionObjT1<std::string,long,CExprStrLen>                 (this, "strlen");
  new CExprFunctionObjT3<std::string,long,long,std::string,CExprSubStr>(this, "substr");
  new CExprFunctionObjT2<std::string,std::string,long,CExprStrStrT>    (this, "strstrt");
  new CExprFunctionObjT1<std::string,long,CExprWords>                  (this, "words");
  new CExprFunctionObjT2<std::string,long,std::string,CExprWord>       (this, "word");
  new CExprFunctionObjT1<std::string,long,CExprSystem>                 (this, "system");

  // gprintf ?
  addObjFunction("sprintf", "s,...", new CExprFunctionSPrintF);
#endif
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

  functions_.push_back(function);

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

  functions_.push_back(function);

  return function;
}

CExprFunctionPtr
CExprFunctionMgr::
addUserFunction(const std::string &name, const std::vector<std::string> &args,
                const std::string &proc)
{
  CExprFunctionPtr function(new CExprUserFunction(name, args, proc));

  functions_.push_back(function);

  return function;
}

void
CExprFunctionMgr::
removeFunction(CExprFunctionPtr function)
{
  functions_.remove(function);
}

void
CExprFunctionMgr::
getFunctionNames(std::vector<std::string> &names) const
{
  for (const auto &func : functions_)
    names.push_back(func->name());
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
      else if (c == 'n') types |= CEXPR_VALUE_NULL;
      else {
        std::cerr << "Invalid argument type char '" << c << "'" << std::endl;
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

CExprValuePtr
CExprProcFunction::
exec(const Values &values)
{
  assert(values.size() == numArgs());

  return (*proc_)(values);
}

//----------

CExprObjFunction::
CExprObjFunction(const std::string &name, const Args &args, CExprFunctionObj *proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}

CExprValuePtr
CExprObjFunction::
exec(const Values &values)
{
  if (isVariableArgs())
    assert(values.size() >= numArgs());
  else
    assert(values.size() == numArgs());

  return (*proc_)(values);
}

//----------

CExprUserFunction::
CExprUserFunction(const std::string &name, const Args &args, const std::string &proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}


CExprValuePtr
CExprUserFunction::
exec(const Values &values)
{
  typedef std::map<std::string,CExprValuePtr> VarValues;

  VarValues varValues;

  assert(values.size() == numArgs());

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
  CExprValuePtr value = CExprInst->evaluateExpression(proc_);

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
