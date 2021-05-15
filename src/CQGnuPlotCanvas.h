#ifndef CQGnuPlotCanvas_H
#define CQGnuPlotCanvas_H

#include <QWidget>
#include <CPoint2D.h>

class CQGnuPlotMainWindow;

class CQGnuPlotCanvas : public QWidget {
  Q_OBJECT

 public:
  CQGnuPlotCanvas(CQGnuPlotMainWindow *window);
 ~CQGnuPlotCanvas();

  CQGnuPlotMainWindow *qwindow() const { return window_; }

 private:
  void paintEvent(QPaintEvent *) override;

  void mousePressEvent  (QMouseEvent *) override;
  void mouseMoveEvent   (QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;

  void wheelEvent(QWheelEvent *e) override;

  void keyPressEvent(QKeyEvent *) override;

  bool pixelToWindow(const CPoint2D &p, CPoint2D &w);

  bool event(QEvent *e) override;

 private:
  CQGnuPlotMainWindow *window_;
  bool                 pressed_;
};

#endif
