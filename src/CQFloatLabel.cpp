#include <CQFloatLabel.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionFrame>

CQFloatLabel::
CQFloatLabel(QWidget *parent) :
 QFrame(parent, Qt::Window | Qt::FramelessWindowHint),
 margin_(style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth)),
 opacity_(style()->styleHint(QStyle::SH_ToolTipLabel_Opacity)/255.0)
{
  setAttribute(Qt::WA_TranslucentBackground);

  //setPalette(QToolTip::palette());
  setWindowOpacity(opacity_);

  layout_ = new QVBoxLayout(this);
  layout_->setMargin(margin_); layout_->setSpacing(0);

  label_ = new QLabel;

  layout_->addWidget(label_);
}

CQFloatLabel::
~CQFloatLabel()
{
}

void
CQFloatLabel::
setHtml(const QString &html)
{
  label_->setText(html);
}

void
CQFloatLabel::
show(const QPoint &pos)
{
  int h = sizeHint().height() + 4;

  move(pos - QPoint(0, h));

  QWidget::show();
}

void
CQFloatLabel::
paintEvent(QPaintEvent *)
{
  QStylePainter painter(this);

  QStyleOptionFrame opt;

  opt.initFrom(this);

  painter.drawPrimitive(QStyle::PE_PanelTipLabel, opt);
}
