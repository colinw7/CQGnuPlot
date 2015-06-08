#ifndef CQINTEGER_SPIN_H
#define CQINTEGER_SPIN_H

#include <QSpinBox>

class CQIntegerSpin : public QSpinBox {
  Q_OBJECT

 public:
  CQIntegerSpin(QWidget *parent, int value=0);
  CQIntegerSpin(int value=0);

  virtual ~CQIntegerSpin() { }

  int getValue() const;
};

#endif
