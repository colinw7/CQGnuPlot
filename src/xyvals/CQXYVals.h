#include <QWidget>

class CQXYValsTest;
class CXYValsInside;

class CQXYValsCanvas : public QWidget {
  Q_OBJECT

 public:
  CQXYValsCanvas(CQXYValsTest *text);

 private:
  void paintEvent(QPaintEvent *);

  void mousePressEvent  (QMouseEvent *me);
  void mouseMoveEvent   (QMouseEvent *me);
  void mouseReleaseEvent(QMouseEvent *me);

 private:
  CQXYValsTest  *test_;
  CXYValsInside *xyvals_;
  bool           pressed_ { false };
  QPoint         pressPos_;
  QPoint         releasePos_;
  QPolygon       poly_;
};

class CQXYValsTest : public QWidget {
  Q_OBJECT

 public:
  CQXYValsTest();

 private:
  CQXYValsCanvas *canvas_;
};
