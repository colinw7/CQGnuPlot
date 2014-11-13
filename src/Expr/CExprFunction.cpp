#include <CExprI.h>
#include <cmath>
#include <cstdlib>

struct CExprBuiltinFunction {
  const char        *name;
  const char        *args;
  CExprFunctionProc  proc;
};

#define CEXPR_REAL_1_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) \
    return CExprValue::createRealValue(F(real)); \
  return CExprValuePtr(); \
}

#define CEXPR_ANGLE_1_FUNC(NAME, F) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  double real; \
  if (values[0]->getRealValue(real)) { \
    if (CExprInst->getDegrees()) \
      real = M_PI*real/180.0; \
    return CExprValue::createRealValue(F(real)); \
  } \
  return CExprValuePtr(); \
}

#define CEXPR_REAL_INTEGER_1_FUNC(NAME, RF, IF) \
static CExprValuePtr \
CExprFunction##NAME(const CExprFunction::Values &values) { \
  assert(values.size() == 1); \
  if      (values[0]->isRealValue()) { \
    double real; \
    if (values[0]->getRealValue(real)) \
      return CExprValue::createRealValue(RF(real)); \
  } \
  else if (values[0]->isIntegerValue()) { \
    long integer; \
    if (values[0]->getIntegerValue(integer)) \
      return CExprValue::createIntegerValue(IF(integer)); \
  } \
  else { \
    assert(false); \
  } \
  return CExprValuePtr(); \
}

CEXPR_REAL_1_FUNC(Sqrt , ::sqrt)
CEXPR_REAL_1_FUNC(Exp  , ::exp)
CEXPR_REAL_1_FUNC(Log  , ::log)
CEXPR_REAL_1_FUNC(Log10, ::log10)

CEXPR_ANGLE_1_FUNC(Sin, ::sin)
CEXPR_ANGLE_1_FUNC(Cos, ::cos)
CEXPR_ANGLE_1_FUNC(Tan, ::tan)

CEXPR_REAL_INTEGER_1_FUNC(Abs, ::fabs, ::abs)

static CExprBuiltinFunction
builtinFns[] = {
  { "sqrt" , "r" , CExprFunctionSqrt  },
  { "exp"  , "r" , CExprFunctionExp   },
  { "log"  , "r" , CExprFunctionLog   },
  { "log10", "r" , CExprFunctionLog10 },
  { "sin"  , "r" , CExprFunctionSin   },
  { "cos"  , "r" , CExprFunctionCos   },
  { "tan"  , "r" , CExprFunctionTan   },
  { "abs"  , "ri", CExprFunctionAbs   },
  { ""     , ""  , 0                  }
};

CExprFunctionMgr::
CExprFunctionMgr()
{
  for (uint i = 0; builtinFns[i].proc; ++i) {
    CExprFunctionPtr function =
      addProcFunction(builtinFns[i].name, builtinFns[i].args, builtinFns[i].proc);

    function->setBuiltin(true);
  }
}

CExprFunctionPtr
CExprFunctionMgr::
getFunction(const std::string &name)
{
  for (FunctionList::const_iterator p = functions_.begin(); p != functions_.end(); ++p)
    if ((*p)->name() == name)
      return *p;

  return CExprFunctionPtr();
}

CExprFunctionPtr
CExprFunctionMgr::
addProcFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc)
{
  Args args;

  (void) parseArgs(argsStr, args);

  CExprFunctionPtr function(new CExprProcFunction(name, args, proc));

  functions_.push_back(function);

  return function;
}

CExprFunctionPtr
CExprFunctionMgr::
addObjFunction(const std::string &name, const std::string &argsStr, CExprFunctionObj &proc)
{
  Args args;

  (void) parseArgs(argsStr, args);

  CExprFunctionPtr function(new CExprObjFunction(name, args, proc));

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
  for (FunctionList::const_iterator p = functions_.begin(); p != functions_.end(); ++p)
    names.push_back((*p)->name());
}

bool
CExprFunctionMgr::
parseArgs(const std::string &argsStr, Args &args)
{
  bool rc = true;

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
      else
        rc = false;
    }

    args[i].type = (CExprValueType) types;
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
CExprObjFunction(const std::string &name, const Args &args, CExprFunctionObj &proc) :
 CExprFunction(name), args_(args), proc_(proc)
{
}

CExprValuePtr
CExprObjFunction::
exec(const Values &values)
{
  assert(values.size() == numArgs());

  return proc_(values);
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
