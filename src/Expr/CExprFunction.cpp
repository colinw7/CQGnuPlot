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
builtinFns[] = {
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
  for (uint i = 0; builtinFns[i].proc; ++i)
    addProcFunction(builtinFns[i].name, builtinFns[i].args, builtinFns[i].proc);
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
addProcFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc)
{
  CExprFunctionPtr function(new CExprProcFunction(name, argsStr, proc));

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

//----------

CExprProcFunction::
CExprProcFunction(const std::string &name, const Args &args, CExprFunctionProc proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}

CExprProcFunction::
CExprProcFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc) :
 CExprFunction(name), args_(), proc_(proc)
{
  parseArgs(argsStr, args_);
}

void
CExprProcFunction::
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

CExprValuePtr
CExprProcFunction::
exec(const Values &values)
{
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
  for (VarValues::const_iterator p = varValues.begin(); p != varValues.end(); ++p) {
    const std::string varName = (*p).first;
    CExprValuePtr     value   = (*p).second;

    if (value.isValid()) {
      CExprVariablePtr var = CExprInst->getVariable(varName);

      var->setValue(value);
    }
    else
      CExprInst->removeVariable(varName);
  }

  return value;
}
