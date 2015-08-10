#include <CGnuPlotMargin.h>
#include <CGnuPlotRenderer.h>

double
CGnuPlotMarginValue::
xValue(CGnuPlotRenderer *renderer) const
{
  if (! isScreen()) {
    CFontPtr font = renderer->getFont();

    double w = font->getStringWidth("X");

    return value().getValue(2)*w;
  }
  else {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    return CGnuPlotUtil::map(value().getValue(0.1), 0, 1, px1, px2);
  }
}

double
CGnuPlotMarginValue::
yValue(CGnuPlotRenderer *renderer) const
{
  if (! isScreen()) {
    CFontPtr font = renderer->getFont();

    double h = font->getCharHeight();

    return value().getValue(2)*h;
  }
  else {
    double px1, py1, px2, py2;

    renderer->pixelToWindow(                    0,                      0, &px1, &py1);
    renderer->pixelToWindow(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    return CGnuPlotUtil::map(value().getValue(0.1), 0, 1, py1, py2);
  }
}
