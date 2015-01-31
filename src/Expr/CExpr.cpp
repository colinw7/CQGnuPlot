#include <CExprI.h>

CExpr *
CExpr::
instance()
{
  static CAutoPtr<CExpr> instance;

  if (! instance)
    instance = new CExpr;

  return instance;
}

CExpr::
CExpr() :
 debug_(false), trace_(false), degrees_(false)
{
  parse_   = new CExprParse;
  interp_  = new CExprInterp;
  compile_ = new CExprCompile;
  execute_ = new CExprExecute;

  operatorMgr_ = new CExprOperatorMgr;
  variableMgr_ = new CExprVariableMgr;
  functionMgr_ = new CExprFunctionMgr(this);

  functionMgr_->addFunctions();
}

bool
CExpr::
evaluateExpression(const std::string &str, std::vector<CExprValuePtr> &values)
{
  CExprPTokenStack pstack = parseLine(str);
  CExprITokenPtr   itoken = interpPTokenStack(pstack);
  CExprCTokenStack cstack = compileIToken(itoken);

  return executeCTokenStack(cstack, values);
}

CExprValuePtr
CExpr::
evaluateExpression(const std::string &str)
{
  CExprPTokenStack pstack = parseLine(str);
  CExprITokenPtr   itoken = interpPTokenStack(pstack);
  CExprCTokenStack cstack = compileIToken(itoken);

  return executeCTokenStack(cstack);
}

CExprPTokenStack
CExpr::
parseLine(const std::string &line)
{
  CExprPTokenStack pstack = parse_->parseLine(line);

  if (getDebug())
    std::cerr << "PTokenStack:" << pstack << std::endl;

  return pstack;
}

CExprITokenPtr
CExpr::
interpPTokenStack(const CExprPTokenStack &stack)
{
  CExprITokenPtr itoken = interp_->interpPTokenStack(stack);

  if (getDebug())
    std::cerr << "IToken:" << itoken << std::endl;

  return itoken;
}

CExprCTokenStack
CExpr::
compileIToken(CExprITokenPtr itoken)
{
  CExprCTokenStack cstack = compile_->compileIToken(itoken);

  if (getDebug())
    std::cerr << "CTokenStack:" << cstack << std::endl;

  return cstack;
}

bool
CExpr::
executeCTokenStack(const CExprCTokenStack &stack, std::vector<CExprValuePtr> &values)
{
  if (! execute_->executeCTokenStack(stack, values))
    return false;

  if (getDebug()) {
    std::cerr << "Values:";
    for (uint i = 0; i < values.size(); ++i)
      std::cerr << " " << values[i] << std::endl;
    std::cerr << std::endl;
  }

  return true;
}

CExprValuePtr
CExpr::
executeCTokenStack(const CExprCTokenStack &stack)
{
  CExprValuePtr value = execute_->executeCTokenStack(stack);

  if (getDebug())
    std::cerr << "Value: " << value << std::endl;

  return value;
}

CExprVariablePtr
CExpr::
getVariable(const std::string &name) const
{
  return variableMgr_->getVariable(name);
}

CExprVariablePtr
CExpr::
createVariable(const std::string &name, CExprValuePtr value)
{
  return variableMgr_->createVariable(name, value);
}

CExprVariablePtr
CExpr::
createRealVariable(const std::string &name, double x)
{
  return variableMgr_->createVariable(name, createRealValue(x));
}

CExprVariablePtr
CExpr::
createIntegerVariable(const std::string &name, long l)
{
  return variableMgr_->createVariable(name, createIntegerValue(l));
}

CExprVariablePtr
CExpr::
createStringVariable(const std::string &name, const std::string &str)
{
  return variableMgr_->createVariable(name, createStringValue(str));
}

void
CExpr::
removeVariable(const std::string &name)
{
  variableMgr_->removeVariable(variableMgr_->getVariable(name));
}

void
CExpr::
getVariableNames(std::vector<std::string> &names) const
{
  variableMgr_->getVariableNames(names);
}

CExprFunctionPtr
CExpr::
getFunction(const std::string &name)
{
  return functionMgr_->getFunction(name);
}

CExprFunctionPtr
CExpr::
addFunction(const std::string &name, const std::vector<std::string> &args,
            const std::string &proc)
{
  return functionMgr_->addUserFunction(name, args, proc);
}

CExprFunctionPtr
CExpr::
addFunction(const std::string &name, const std::string &argsStr, CExprFunctionObj *func)
{
  return functionMgr_->addObjFunction(name, argsStr, func);
}

void
CExpr::
getFunctionNames(std::vector<std::string> &names) const
{
  functionMgr_->getFunctionNames(names);
}

CExprOperatorPtr
CExpr::
getOperator(CExprOpType type) const
{
  return operatorMgr_->getOperator(type);
}

CExprValuePtr
CExpr::
createBooleanValue(bool boolean)
{
  return CExprValuePtr(new CExprValue(CExprBooleanValue(boolean)));
}

CExprValuePtr
CExpr::
createIntegerValue(long integer)
{
  return CExprValuePtr(new CExprValue(CExprIntegerValue(integer)));
}

CExprValuePtr
CExpr::
createRealValue(double real)
{
  return CExprValuePtr(new CExprValue(CExprRealValue(real)));
}

CExprValuePtr
CExpr::
createStringValue(const std::string &str)
{
  return CExprValuePtr(new CExprValue(CExprStringValue(str)));
}
