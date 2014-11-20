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
  for (VariableList::const_iterator p = variables_.begin(); p != variables_.end(); ++p)
    if ((*p)->name() == name)
      return *p;

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
  for (VariableList::const_iterator p = variables_.begin(); p != variables_.end(); ++p)
    names.push_back((*p)->name());
}

//------

CExprVariable::
CExprVariable(const std::string &name, CExprValuePtr value) :
 name_(name), value_(value)
{
}

CExprVariable::
~CExprVariable()
{
}

void
CExprVariable::
setValue(CExprValuePtr value)
{
  value_ = value;
}

void
CExprVariable::
setRealValue(double x)
{
  if (value_->isRealValue())
    value_->setRealValue(x);
  else
    value_ = CExprInst->createRealValue(x);
}

CExprValuePtr
CExprVariable::
getValue() const
{
  return value_;
}

CExprValueType
CExprVariable::
getValueType() const
{
  return value_->getType();
}
