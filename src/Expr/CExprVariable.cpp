#include <CExprI.h>

CExprVariableMgr::
CExprVariableMgr()
{
}

CExprVariableMgr::
~CExprVariableMgr()
{
}

CExprVariablePtr
CExprVariableMgr::
createVariable(const std::string &name, CExprValuePtr value)
{
  CExprVariablePtr variable = getVariable(name);

  if (! variable.isValid()) {
    variable = CExprVariablePtr(new CExprVariable(name, value));

    addVariable(variable);
  }
  else
    variable->setValue(value);

  return variable;
}

CExprVariablePtr
CExprVariableMgr::
getVariable(const std::string &name) const
{
  for (const auto &var : variables_)
    if (var->name() == name)
      return var;

  return CExprVariablePtr();
}

void
CExprVariableMgr::
addVariable(CExprVariablePtr variable)
{
  variables_.push_back(variable);
}

void
CExprVariableMgr::
removeVariable(CExprVariablePtr variable)
{
  variables_.remove(variable);
}

void
CExprVariableMgr::
getVariableNames(std::vector<std::string> &names) const
{
  for (const auto &var : variables_)
    names.push_back(var->name());
}

//------

CExprVariable::
CExprVariable(const std::string &name, const CExprValuePtr &value) :
 name_(name), value_(value)
{
}

CExprVariable::
~CExprVariable()
{
}

void
CExprVariable::
setValue(const CExprValuePtr &value)
{
  value_ = value;
}

void
CExprVariable::
setRealValue(double r)
{
  if (value_->isRealValue())
    value_->setRealValue(r);
  else
    value_ = CExprInst->createRealValue(r);
}

void
CExprVariable::
setIntegerValue(int i)
{
  if (value_->isIntegerValue())
    value_->setIntegerValue(i);
  else
    value_ = CExprInst->createIntegerValue(i);
}

CExprValueType
CExprVariable::
getValueType() const
{
  return value_->getType();
}
