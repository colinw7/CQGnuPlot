#include <CQGnuPlotKey.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotUtil.h>
#include <CGnuPlotRenderer.h>
#include <CQUtil.h>

CQGnuPlotKey::
CQGnuPlotKey(CQGnuPlotGroup *group) :
 CGnuPlotKey(group)
{
}

CQGnuPlotKey::
~CQGnuPlotKey()
{
}

QString
CQGnuPlotKey::
title() const
{
  return CGnuPlotKey::title().c_str();
}

void
CQGnuPlotKey::
setTitle(const QString &s)
{
  CGnuPlotKey::setTitle(s.toStdString());
}

int
CQGnuPlotKey::
boxLineStyle() const
{
  return CGnuPlotKey::boxLineStyle().getValue(-1);
}

CQGnuPlotEnum::HAlignType
CQGnuPlotKey::
halign() const
{
  return CQGnuPlotEnum::halignConv(CGnuPlotKey::getHAlign());
}

void
CQGnuPlotKey::
setHAlign(const CQGnuPlotEnum::HAlignType &a)
{
  CGnuPlotKey::setHAlign(CQGnuPlotEnum::halignConv(a));
}

CQGnuPlotEnum::VAlignType
CQGnuPlotKey::
valign() const
{
  return CQGnuPlotEnum::valignConv(CGnuPlotKey::getVAlign());
}

void
CQGnuPlotKey::
setVAlign(const CQGnuPlotEnum::VAlignType &a)
{
  CGnuPlotKey::setVAlign(CQGnuPlotEnum::valignConv(a));
}

QFont
CQGnuPlotKey::
getFont() const
{
  return CQUtil::toQFont(CGnuPlotKey::getFont());
}

void
CQGnuPlotKey::
setFont(const QFont &f)
{
  CGnuPlotKey::setFont(CQUtil::fromQFont(f));
}

void
CQGnuPlotKey::
draw(CGnuPlotRenderer *renderer)
{
  if (! isDisplayed())
    return;

  CGnuPlotKey::draw(renderer);

  if (isSelected()) {
    renderer->drawRect(bbox(), CRGBA(1,0,0), 2);
  }
}

bool
CQGnuPlotKey::
mousePress(const CGnuPlotMouseEvent &mouseEvent)
{
  CGnuPlotRenderer *renderer = app()->renderer();

  CPoint2D p;

  renderer->pixelToWindow(mouseEvent.pixel(), p);

  CGnuPlotPlotP plot = group()->getPlotForId(CGnuPlotKey::plotAtPos(p));
  if (! plot) return false;

  plot->setDisplayed(! plot->isDisplayed());

  CQGnuPlotWindow *qwindow = dynamic_cast<CQGnuPlotWindow *>(group()->window());

  qwindow->redraw();

  return true;
}
