#ifndef CQGnuPlotPositionEdit_H
#define CQGnuPlotPositionEdit_H

#include <CQIconCombo.h>
#include <CGnuPlotPosition.h>
#include <QFrame>

class QStackedWidget;
class CQGnuPlotCoordSys;
class CQRealSpin;
class CQGnuPlotPositionTypeCombo;

class CQGnuPlotPositionEdit : public QFrame {
  Q_OBJECT

 public:
  CQGnuPlotPositionEdit(QWidget *w=0);

  const CGnuPlotPosition &position() const { return position_; }
  void setPosition(const CGnuPlotPosition &v);

  bool is2D() const { return is2D_; }
  void set2D(bool b);

 private slots:
  void updateState();
  void updatePosition();
  void updateStack();

 private:
  CGnuPlotPosition            position_;
  bool                        is2D_ { true };
  QStackedWidget             *stack_;
  CQGnuPlotCoordSys          *systemXYZ_;
  CQRealSpin                 *xSingleEdit_;
  CQRealSpin                 *ySingleEdit_;
  CQRealSpin                 *zSingleEdit_;
  CQGnuPlotCoordSys          *systemX_;
  CQGnuPlotCoordSys          *systemY_;
  CQGnuPlotCoordSys          *systemZ_;
  CQRealSpin                 *xMultiEdit_;
  CQRealSpin                 *yMultiEdit_;
  CQRealSpin                 *zMultiEdit_;
  CQGnuPlotPositionTypeCombo *type_;
};

class CQGnuPlotCoordSys : public CQIconCombo {
  Q_OBJECT

 public:
  CQGnuPlotCoordSys(const char *name);

  CGnuPlotPosition::CoordSys coordSys() const;
  void setCoordSys(CGnuPlotPosition::CoordSys cs);
};

class CQGnuPlotPositionTypeCombo : public CQIconCombo {
  Q_OBJECT

 public:
  CQGnuPlotPositionTypeCombo(QWidget *w=0);
};

#endif
