#ifndef CQGnuPlotCanvas_H
#define CQGnuPlotCanvas_H

#include <QWidget>

class CQGnuPlotWindow;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlotWindow *window);
 ~CQGnuPlotCanvas();

 private:
  void paintEvent(QPaintEvent *);

  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

 private:
  CQGnuPlotWindow *window_;
};

#endif
