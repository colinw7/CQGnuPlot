#include <CQGnuPlotToolBar.h>
#include <CQIntegerSpin.h>
#include <QContextMenuEvent>
#include <QWidgetAction>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include <QStyle>

class CQWidgetAction : public QWidgetAction {
 public:
  CQWidgetAction(QWidget *w) :
   QWidgetAction(nullptr), w_(w) {
  }

 ~CQWidgetAction() {
    delete w_;
  }

  QWidget *widget() const { return w_; }

  QWidget *createWidget(QWidget *parent) override {
    w_->setParent(parent);

    return w_;
  }

 private:
  QWidget *w_ { nullptr };
};

//---

class CQLabelIntegerSpin : public QWidget {
 public:
  CQLabelIntegerSpin(const QString &label, CQIntegerSpin *spin) {
   QHBoxLayout *layout = new QHBoxLayout(this);
   layout->setSpacing(2); layout->setMargin(2);

   layout->addWidget(new QLabel(label));
   layout->addWidget(spin);
  }
};

//---

CQGnuPlotToolBar::
CQGnuPlotToolBar(const QString &text) :
 QToolBar(text)
{
  int is = style()->pixelMetric(QStyle::PM_LargeIconSize);

  setIconSize(QSize(is, is));
}

void
CQGnuPlotToolBar::
contextMenuEvent(QContextMenuEvent *e)
{
  QMenu *menu = new QMenu;

  CQIntegerSpin *sizeSpin = new CQIntegerSpin;

  sizeSpin->setRange(8, 40);
  sizeSpin->setValue(iconSize().width());

  connect(sizeSpin, SIGNAL(valueChanged(int)), this, SLOT(sizeSlot(int)));

  CQLabelIntegerSpin *labelSizePin = new CQLabelIntegerSpin("Size", sizeSpin);

  menu->addAction(new CQWidgetAction(labelSizePin));

  menu->exec(e->globalPos());
}

void
CQGnuPlotToolBar::
sizeSlot(int s)
{
  setIconSize(QSize(s, s));
}
