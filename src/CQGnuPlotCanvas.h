#ifndef CQGnuPlotCanvas_H
#define CQGnuPlotCanvas_H

#include <QWidget>

class CQGnuPlotWindow;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlotWindow *window);
 ~CQGnuPlotCanvas();

  CQGnuPlotWindow *qwindow() const { return window_; }

 private:
  void paintEvent(QPaintEvent *);

  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

  bool event(QEvent *e);

 private:
  CQGnuPlotWindow *window_;
};

#endif
