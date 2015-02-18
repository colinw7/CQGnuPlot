#include <CQGnuPlotLabel.h>
#include <CQGnuPlotGroup.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotLabel::
CQGnuPlotLabel(CQGnuPlotGroup *group) :
 CQGnuPlotAnnotation(this), CGnuPlotLabel(group)
{
  setObjectName("label");
}

QString
CQGnuPlotLabel::
getText() const
{
  return CGnuPlotLabel::getText().c_str();
}

void
CQGnuPlotLabel::
setText(const QString &s)
{
  CGnuPlotLabel::setText(s.toStdString());
}

QPointF
CQGnuPlotLabel::
getPos() const
{
  return CQUtil::toQPoint(CGnuPlotLabel::getPos());
}

void
CQGnuPlotLabel::
setPos(const QPointF &p)
{
  CGnuPlotLabel::setPos(CQUtil::fromQPoint(p));
}

QFont
CQGnuPlotLabel::
getFont() const
{
  return CQUtil::toQFont(CGnuPlotLabel::getFont());
}

void
CQGnuPlotLabel::
setFont(const QFont &f)
{
  CGnuPlotLabel::setFont(CQUtil::fromQFont(f));
}

void
CQGnuPlotLabel::
draw() const
{
  CGnuPlotLabel::draw();

  if (isSelected()) {
    CGnuPlotRenderer *renderer = group_->app()->renderer();

    //CVAlignType valign = (getFront() ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);
    CVAlignType valign = CVALIGN_TYPE_CENTER;

    renderer->drawHAlignedText(CGnuPlotLabel::getPos(), getAlign(), 0, valign, 0,
                               CGnuPlotLabel::getText(), CRGBA(1,0,0));
  }
}
