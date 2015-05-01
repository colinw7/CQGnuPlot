#include <CQLineEditMenuTest.h>
#include <CQLineEditMenu.h>
#include <CQLinkLabel.h>
#include <CQDocumentLabel.h>
#include <CQApp.h>

#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQLineEditMenuTest *test = new CQLineEditMenuTest;

  test->show();

  return app.exec();
}

void
setBackground(QWidget *widget, const QColor &color)
{
  QPalette palette = widget->palette();

  palette.setColor(widget->backgroundRole(), color);

  widget->setPalette(palette);

  widget->setAutoFillBackground(true);
}

class Splitter : public QFrame {
 public:
  Splitter() {
    setFrameStyle(QFrame::HLine | QFrame::Raised);

    setLineWidth(2);
  }
};

CQLineEditMenuTest::
CQLineEditMenuTest()
{
  //setBackground(this, QColor(140,140,140));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  CQLineEditMenu *area = new CQLineEditMenu(this);

  MenuWidget *menu = new MenuWidget(area);

  area->setMenuWidget(menu);

  layout->addWidget(area);

  layout->addWidget(new QLineEdit);

  layout->addStretch();
}

MenuWidget::
MenuWidget(CQLineEditMenu *area) :
 QFrame(0)
{
  //setBackground(this, QColor(140,140,140));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  layout->addWidget(new CQLineEditContainer(this));

  layout->addWidget(new QLabel("START IN"));

  QComboBox *startCombo = new QComboBox;

  startCombo->addItem("Accounts");
  startCombo->addItem("Stock Trade");
  startCombo->addItem("Bill Pay");

  layout->addWidget(startCombo);

  layout->addWidget(new CQDocumentLabel(
    CQDocumentLabelDocument("@[image lock.png] @[link id]Forgot User ID or Password@[]")));

  layout->addWidget(new Splitter());

  layout->addWidget(new CQLinkLabel("Set up Online Account Access"));
  layout->addWidget(new CQLinkLabel("Digital Security ID token Expired"));
  layout->addWidget(new CQLinkLabel("Access you Tax Documents"));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->setMargin(2); buttonLayout->setSpacing(2);

  QPushButton *button = new QPushButton("Close");

  buttonLayout->addWidget(button);

  connect(button, SIGNAL(clicked()), area, SLOT(hideMenu()));

  buttonLayout->addStretch();

  layout->addLayout(buttonLayout);
}
