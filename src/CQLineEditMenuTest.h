#include <QDialog>
#include <QFrame>

class CQLineEditMenuTest : public QDialog {
  Q_OBJECT

 public:
  CQLineEditMenuTest();
};

class CQLineEditMenu;

class MenuWidget : public QFrame {
 public:
  MenuWidget(CQLineEditMenu *area);
};
