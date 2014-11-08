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
  VariableList::const_iterator p1, p2;

  for (p1 = variables_.begin(), p2 = variables_.end(); p1 != p2; ++p1)
    if ((*p1)->name_ == name)
      return *p1;

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
    value_ = CExprValue::createRealValue(x);
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
