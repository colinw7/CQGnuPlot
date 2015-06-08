#include <CQIntegerSpin.h>

CQIntegerSpin::
CQIntegerSpin(QWidget *parent, int value) :
 QSpinBox(parent)
{
  setRange(-INT_MAX, INT_MAX);

  setValue(value);
}

CQIntegerSpin::
CQIntegerSpin(int value) :
 QSpinBox(NULL)
{
  setRange(-INT_MAX, INT_MAX);

  setValue(value);
}

int
CQIntegerSpin::
getValue() const
{
  return value();
}
