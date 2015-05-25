#ifndef CQGnuPlotCanvas_H
#define CQGnuPlotCanvas_H

#include <QWidget>
#include <CPoint2D.h>

class CQGnuPlotWindow;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlotWindow *window);
 ~CQGnuPlotCanvas();

  CQGnuPlotWindow *qwindow() const { return window_; }

 private:
  void paintEvent(QPaintEvent *);

  void mousePressEvent  (QMouseEvent *);
  void mouseMoveEvent   (QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void keyPressEvent(QKeyEvent *);

  bool pixelToWindow(const CPoint2D &p, CPoint2D &w);

  bool event(QEvent *e);

 private:
  CQGnuPlotWindow *window_;
  bool             pressed_;
};

#endif
