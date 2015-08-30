#ifndef CQGnuPlotObject_H
#define CQGnuPlotObject_H

#include <QObject>

class CQGnuPlotObject : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

 public:
  CQGnuPlotObject() { }

  virtual ~CQGnuPlotObject() { }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  bool isHighlighted() const { return highlighted_; }
  void setHighlighted(bool b) { highlighted_ = b; }

 protected:
  bool selected_    { false };
  bool highlighted_ { false };
};

#endif
