#ifndef CQPOINT2D_EDIT_H
#define CQPOINT2D_EDIT_H

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <CPoint2D.h>

class CQPoint2DEdit : public QFrame {
  Q_OBJECT

  Q_PROPERTY(bool spin READ isSpin WRITE setSpin)

 public:
  CQPoint2DEdit(QWidget *parent, const CPoint2D &value=CPoint2D(0,0), bool spin=false);
  CQPoint2DEdit(const CPoint2D &value=CPoint2D(0,0), bool spin=false);

  CQPoint2DEdit(QWidget *parent, const QPointF &value, bool spin=false);
  CQPoint2DEdit(const QPointF &value, bool spin=false);

  virtual ~CQPoint2DEdit() { }

  bool isSpin() const { return spin_; }
  void setSpin(bool b);

  void setValue(const CPoint2D &point);
  void setValue(const QPointF &point);

  void setMinimum(const CPoint2D &point);
  void setMaximum(const CPoint2D &point);

  const CPoint2D &getValue() const;
  QPointF getQValue() const;

 private:
  void init(const CPoint2D &value);

 private slots:
  void editingFinishedI();

 signals:
  void valueChanged();

 private:
  void updateRange();

  void pointToWidget();
  void widgetToPoint();

 private:
  CPoint2D          point_;
  bool              spin_;
  CPoint2D          min_, max_;
  QDoubleValidator *x_validator_;
  QDoubleValidator *y_validator_;
  QLineEdit        *x_edit_;
  QLineEdit        *y_edit_;
  QDoubleSpinBox   *x_spin_;
  QDoubleSpinBox   *y_spin_;
  QHBoxLayout      *layout_;
};

#endif
