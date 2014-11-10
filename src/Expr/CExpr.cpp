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
  functionMgr_ = new CExprFunctionMgr;
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
  return variableMgr_->createVariable(name, CExprValue::createRealValue(x));
}

void
CExpr::
removeVariable(const std::string &name)
{
  variableMgr_->removeVariable(variableMgr_->getVariable(name));
}

CExprFunctionPtr
CExpr::
lookupFunction(const std::string &name)
{
  return functionMgr_->lookupFunction(name);
}

CExprFunctionPtr
CExpr::
addFunction(const std::string &name, const std::vector<std::string> &args,
            const std::string &proc)
{
  return functionMgr_->addUserFunction(name, args, proc);
}

bool
CExpr::
isOperatorChar(char c) const
{
  return operatorMgr_->isOperatorChar(c);
}

CExprOperatorPtr
CExpr::
getOperator(CExprOpType type) const
{
  return operatorMgr_->getOperator(type);
}
