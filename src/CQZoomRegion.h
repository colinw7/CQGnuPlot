#ifndef CQZoomRegion_H
#define CQZoomRegion_H

#include <QWidget>
#include <QPainter>

class CQZoomRegion : public QWidget {
 public:
  CQZoomRegion(QWidget *parent=0) :
   QWidget(parent), tl_(0, 0), br_(0, 0) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    hide();
  }

  void setGeometry(const QRect &r) {
    tl_ = r.topLeft    ();
    br_ = r.bottomRight();

    QWidget::setGeometry(r);
  }

  const QPointF &start() const { return start_; }
  const QPointF &end  () const { return end_  ; }

  void setStart(const QPointF &start) { start_ = start; }
  void setEnd  (const QPointF &end  ) { end_   = end  ; }

 protected:
  void paintEvent(QPaintEvent *) override {
    QPainter p(this);

    p.fillRect(rect(), QColor(80, 80, 255, 128));

    p.setPen(QColor(0, 0, 0, 128));

    QFontMetrics fm(font());

    QString str1 = QString("%1,%2").arg(start_.x()).arg(start_.y());
    QString str2 = QString("%1,%2").arg(end_  .x()).arg(end_  .y());

//  int fw1 = fm.horizontalAdvance(str1);
    int fw2 = fm.horizontalAdvance(str2);

    int fa = fm.ascent();
    int fd = fm.descent();

    int w = width ();
    int h = height();

    p.drawText(QPoint(2    , 2    ) + QPoint(  0, fa), str1);
    p.drawText(QPoint(w - 3, h - 3) - QPoint(fw2, fd), str2);
  }

 private:
  QPoint  tl_;
  QPoint  br_;
  QPointF start_;
  QPointF end_;
};

#endif
