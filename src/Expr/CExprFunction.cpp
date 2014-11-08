#include <CExprI.h>
#include <cmath>

struct CExprBuiltinFunction {
  const char        *name;
  const char        *args;
  CExprFunctionProc  proc;
};

#define CEXPR_REAL1_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  double real; \
  if (values.size() < 1 || ! values[0]->getRealValue(real)) \
    return CExprValuePtr(); \
  double result = F(real); \
  return CExprValue::createRealValue(result); \
}

#define CEXPR_ANGLE1_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  double real; \
  if (values.size() < 1 || ! values[0]->getRealValue(real)) \
    return CExprValuePtr(); \
  if (CExprInst->getDegrees()) \
    real = M_PI*real/180.0; \
  double result = F(real); \
  return CExprValue::createRealValue(result); \
}

CEXPR_REAL1_FUNC(Sqrt , ::sqrt)
CEXPR_REAL1_FUNC(Exp  , ::exp)
CEXPR_REAL1_FUNC(Log  , ::log)
CEXPR_REAL1_FUNC(Log10, ::log10)

CEXPR_ANGLE1_FUNC(Sin, ::sin)
CEXPR_ANGLE1_FUNC(Cos, ::cos)
CEXPR_ANGLE1_FUNC(Tan, ::tan)

static CExprBuiltinFunction
builtin_functions[] = {
  { "sqrt" , "r" , CExprFunctionSqrt  },
  { "exp"  , "r" , CExprFunctionExp   },
  { "log"  , "r" , CExprFunctionLog   },
  { "log10", "r" , CExprFunctionLog10 },
  { "sin"  , "r" , CExprFunctionSin   },
  { "cos"  , "r" , CExprFunctionCos   },
  { "tan"  , "r" , CExprFunctionTan   },
  { ""     , ""  , 0                  }
};

CExprFunctionMgr::
CExprFunctionMgr()
{
  for (uint i = 0; builtin_functions[i].proc != 0; ++i)
    addFunction(builtin_functions[i].name, builtin_functions[i].args, builtin_functions[i].proc);
}

CExprFunctionPtr
CExprFunctionMgr::
lookupFunction(const std::string &name)
{
  FunctionList::const_iterator p1, p2;

  for (p1 = functions_.begin(), p2 = functions_.end(); p1 != p2; ++p1)
    if ((*p1)->getName() == name)
      return *p1;

  return CExprFunctionPtr();
}

CExprFunctionPtr
CExprFunctionMgr::
addFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc)
{
  CExprFunctionPtr function(new CExprFunction(name, argsStr, proc));

  functions_.push_back(function);

  return function;
}

//----------

CExprFunction::
CExprFunction(const std::string &name, const Args &args, CExprFunctionProc proc) :
 name_(name), args_(args), proc_(proc)
{
}

CExprFunction::
CExprFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc) :
 name_(name), args_(), proc_(proc)
{
  parseArgs(argsStr, args_);
}

void
CExprFunction::
parseArgs(const std::string &argsStr, Args &args)
{
  std::vector<std::string> args1;

  CStrUtil::addTokens(argsStr, args1, ", ");

  uint num_args = args1.size();

  args.resize(num_args);

  for (uint i = 0; i < num_args; ++i) {
    uint types = CEXPR_VALUE_NONE;

    uint len = args1[i].size();

    for (uint j = 0; j < len; j++) {
      if      (args1[i][j] == 'b')
        types |= CEXPR_VALUE_BOOLEAN;
      else if (args1[i][j] == 'i')
        types |= CEXPR_VALUE_INTEGER;
      else if (args1[i][j] == 'r')
        types |= CEXPR_VALUE_REAL;
      else if (args1[i][j] == 's')
        types |= CEXPR_VALUE_STRING;
      else if (args1[i][j] == 'n')
        types |= CEXPR_VALUE_NULL;
    }

    args[i].type = (CExprValueType) types;
  }
}
