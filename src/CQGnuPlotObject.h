#ifndef CQGnuPlotObject_H
#define CQGnuPlotObject_H

#include <QObject>

class CQGnuPlotObject : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

 public:
  CQGnuPlotObject() { }

  virtual ~CQGnuPlotObject() { }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

 protected:
  bool selected_ { false };
};

#endif
