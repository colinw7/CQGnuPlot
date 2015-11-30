#ifndef CQFloatLabel_H
#define CQFloatLabel_H

#include <QFrame>

class QLabel;
class QVBoxLayout;

class CQFloatLabel : public QFrame {
 Q_OBJECT

 public:
  CQFloatLabel(QWidget *parent=0);
 ~CQFloatLabel();

  void setHtml(const QString &html);

  void show(const QPoint &pos);

 protected:
  void paintEvent(QPaintEvent *);

 private:
  QLabel      *label_;
  QVBoxLayout *layout_;
  int          margin_;
  double       opacity_;
};

#endif
