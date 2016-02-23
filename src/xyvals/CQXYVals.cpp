#include <CQXYVals.h>
#include <CXYVals.h>

#include <CQApp.h>
#include <CQUtil.h>

#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQXYValsTest test;

  test.resize(800, 800);

  test.show();

  return app.exec();
}

CQXYValsTest::
CQXYValsTest()
{
  setObjectName("test");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  canvas_ = new CQXYValsCanvas(this);

  layout->addWidget(canvas_);
}

//-----------

CQXYValsCanvas::
CQXYValsCanvas(CQXYValsTest *test) :
 QWidget(0), test_(test)
{
  setObjectName("canvas");

  xyvals_ = new CXYValsInside;

  //setMouseTracking(true);
}

void
CQXYValsCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), Qt::white);

  if (pressed_) {
    int x1 = std::min(pressPos_.x(), releasePos_.x());
    int y1 = std::min(pressPos_.y(), releasePos_.y());
    int x2 = std::max(pressPos_.x(), releasePos_.x());
    int y2 = std::max(pressPos_.y(), releasePos_.y());

    int w = x2 - x1;
    int h = y2 - y1;

    if (w > 0 && h > 0) {
      QRect r(x1, y1, w, h);

      painter.setPen(Qt::red);

      painter.drawRect(r);
    }
  }

  //---

  const std::vector<double> &xv = xyvals_->xvals();
  const std::vector<double> &yv = xyvals_->yvals();

  painter.setPen(Qt::black);

  QColor fc(QColor(20, 40, 255));

  for (uint iy = 1; iy < yv.size(); ++iy) {
    int y1 = yv[iy - 1];
    int y2 = yv[iy    ];

    int h = y2 - y1;

    for (uint ix = 1; ix < xv.size(); ++ix) {
      int x1 = xv[ix - 1];
      int x2 = xv[ix    ];

      int w = x2 - x1;

      QRect r(x1, y1, w, h);

      if (xyvals_->isInside(ix - 1, iy - 1))
        painter.fillRect(r, fc);

      painter.setPen(Qt::black);

      painter.drawRect(r);
    }
  }

  //---

  QPen pen(Qt::green);
  pen.setWidth(3);

  painter.setPen(pen);

  painter.drawPolygon(poly_);
}

void
CQXYValsCanvas::
mousePressEvent(QMouseEvent *me)
{
  pressed_    = true;
  pressPos_   = me->pos();
  releasePos_ = pressPos_;

  update();
}

void
CQXYValsCanvas::
mouseMoveEvent(QMouseEvent *me)
{
  releasePos_ = me->pos();

  update();
}

void
CQXYValsCanvas::
mouseReleaseEvent(QMouseEvent *me)
{
  pressed_    = false;
  releasePos_ = me->pos();

  double x1 = pressPos_  .x();
  double y1 = pressPos_  .y();
  double x2 = releasePos_.x();
  double y2 = releasePos_.y();

  std::vector<double> x { x1, x2, x2, x1};
  std::vector<double> y { y1, y1, y2, y2};

  if (xyvals_->getNumXVals() > 0) {
    xyvals_->print(std::cerr); std::cerr << std::endl;

    CXYValsInside xyvals1;

    xyvals1.initValues(*xyvals_, x, y);

    xyvals1.print(std::cerr); std::cerr << std::endl;

    xyvals1.combineInside();

    xyvals1.print(std::cerr); std::cerr << std::endl;

    *xyvals_ = xyvals1;
  }
  else {
    xyvals_->initValues(x, y);

    xyvals_->print(std::cerr); std::cerr << std::endl;
  }

  std::vector<double> xo, yo;

  if (xyvals_->getPoly(xo, yo)) {
    for (uint i = 0; i < xo.size(); ++i)
      poly_.push_back(QPoint(xo[i], yo[i]));
    poly_.push_back(QPoint(xo[0], yo[0]));
  }
  else
    poly_ = QPolygon();

  update();
}
