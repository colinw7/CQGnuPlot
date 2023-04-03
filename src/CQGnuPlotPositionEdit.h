#ifndef CQGnuPlotPositionEdit_H
#define CQGnuPlotPositionEdit_H

#include <CQIconCombo.h>
#include <CGnuPlotPosition.h>
#include <QFrame>
#include <QToolButton>
#include <set>

class QStackedWidget;
class CQGnuPlotCoordSys;
class CQRealSpin;
class CQGnuPlotPositionTypeCombo;
class CQGnuPlotPositionGrabMgr;
class CQGnuPlotPositionGrab;

class CQGnuPlotPositionEdit : public QFrame {
  Q_OBJECT

 public:
  CQGnuPlotPositionEdit(QWidget *w=nullptr);

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
  CQGnuPlotPositionGrab      *grab_;
};

#define CQGnuPlotPositionGrabMgrInst CQGnuPlotPositionGrabMgr::instance()

class CQGnuPlotPositionGrabMgr {
 public:
  static CQGnuPlotPositionGrabMgr *instance();

  CQGnuPlotPositionGrab *currentGrab() const { return grab_; }

 protected:
  friend class CQGnuPlotPositionGrab;

  void addGrab   (CQGnuPlotPositionGrab *grab);
  void removeGrab(CQGnuPlotPositionGrab *grab);

  void grab  (CQGnuPlotPositionGrab *grab);
  void ungrab(CQGnuPlotPositionGrab *grab);

 private:
  CQGnuPlotPositionGrabMgr();

 private:
  typedef std::set<CQGnuPlotPositionGrab *> Grabs;

  Grabs                  grabs_;
  CQGnuPlotPositionGrab *grab_ { 0 };
};

class CQGnuPlotPositionGrab : public QToolButton {
  Q_OBJECT

 public:
  CQGnuPlotPositionGrab(CQGnuPlotPositionEdit *edit);
 ~CQGnuPlotPositionGrab();

  CQGnuPlotPositionEdit *edit() const { return edit_; }

 private slots:
  void grabSlot(bool b);

 private:
  CQGnuPlotPositionEdit *edit_;
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
  CQGnuPlotPositionTypeCombo(QWidget *w=nullptr);
};

#endif
