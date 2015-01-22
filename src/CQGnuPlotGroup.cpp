#include <CQGnuPlotGroup.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotRenderer.h>
#include <CQGnuPlotUtil.h>
#include <CQGnuPlotRenderer.h>

CQGnuPlotGroup::
CQGnuPlotGroup(CQGnuPlotWindow *window) :
 CGnuPlotGroup(window), window_(window)
{
}

CQGnuPlotGroup::
~CQGnuPlotGroup()
{
}

void
CQGnuPlotGroup::
setPainter(QPainter *p)
{
  CQGnuPlotRenderer *renderer = qwindow()->qapp()->qdevice()->qrenderer();

  renderer->setPainter(p);
}

QString
CQGnuPlotGroup::
title() const
{
  CGnuPlot::Title title = CGnuPlotGroup::title();

  return title.str.c_str();
}

void
CQGnuPlotGroup::
setTitle(const QString &s)
{
  CGnuPlot::Title title = CGnuPlotGroup::title();

  title.str = s.toStdString();

  CGnuPlotGroup::setTitle(title);
}

CQGnuPlot::CQHistogramStyle
CQGnuPlotGroup::
histogramStyle() const
{
  return CQGnuPlotUtil::histogramStyleConv(CGnuPlotGroup::getHistogramStyle());
}

void
CQGnuPlotGroup::
setHistogramStyle(const CQGnuPlot::CQHistogramStyle &s)
{
  CGnuPlotGroup::setHistogramStyle(CQGnuPlotUtil::histogramStyleConv(s));
}

CQGnuPlot::CQHAlignType
CQGnuPlotGroup::
keyHAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotGroup::getKeyHAlign());
}

void
CQGnuPlotGroup::
setKeyHAlign(const CQGnuPlot::CQHAlignType &a)
{
  CGnuPlotGroup::setKeyHAlign(CQGnuPlotUtil::alignConv(a));
}

CQGnuPlot::CQVAlignType
CQGnuPlotGroup::
keyVAlign() const
{
  return CQGnuPlotUtil::alignConv(CGnuPlotGroup::getKeyVAlign());
}

void
CQGnuPlotGroup::
setKeyVAlign(const CQGnuPlot::CQVAlignType &a)
{
  CGnuPlotGroup::setKeyVAlign(CQGnuPlotUtil::alignConv(a));
}

void
CQGnuPlotGroup::
mouseMove(const CPoint2D &p)
{
  for (auto &plot : plots()) {
    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    qplot->mouseMove(p);
  }
}

bool
CQGnuPlotGroup::
mouseTip(const CPoint2D &p, CQGnuPlot::TipRect &tip)
{
  for (auto &plot : plots()) {
    CQGnuPlotPlot *qplot = static_cast<CQGnuPlotPlot *>(plot);

    if (qplot->mouseTip(p, tip))
      return true;
  }

  return false;
}
