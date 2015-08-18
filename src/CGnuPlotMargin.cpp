#include <CGnuPlotMargin.h>
#include <CGnuPlotRenderer.h>

void
CGnuPlotMargin::
updateDefaultValues(CGnuPlotRenderer *renderer, double lm, double bm, double rm, double tm)
{
  if (! lmargin_.value().isValid() || ! bmargin_.value().isValid() ||
      ! rmargin_.value().isValid() || ! tmargin_.value().isValid()) {
    CFontPtr font = renderer->getFont();

    double pw = font->getStringWidth("X");
    double ph = font->getCharHeight();

    double w = renderer->pixelWidthToWindowWidthNoMargin  (pw);
    double h = renderer->pixelHeightToWindowHeightNoMargin(ph);

    if (! lmargin_.value().isValid()) {
      lmargin_.setDefValue(lm/w + 1); lmargin_.setScreen(false);
    }
    if (! bmargin_.value().isValid()) {
      bmargin_.setDefValue(bm/h + 1); bmargin_.setScreen(false);
    }
    if (! rmargin_.value().isValid()) {
      rmargin_.setDefValue(rm/w + 1); rmargin_.setScreen(false);
    }
    if (! tmargin_.value().isValid()) {
      tmargin_.setDefValue(tm/h + 1); tmargin_.setScreen(false);
    }
  }
}

//------

double
CGnuPlotMarginValue::
xValue(CGnuPlotRenderer *renderer) const
{
  if (! isScreen()) {
    CFontPtr font = renderer->getFont();

    double w = font->getStringWidth("X");

    return value().getValue(defValue())*w;
  }
  else {
    double px1, py1, px2, py2;

    renderer->pixelToWindowNoMargin(                    0,                      0, &px1, &py1);
    renderer->pixelToWindowNoMargin(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

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

    return value().getValue(defValue())*h;
  }
  else {
    double px1, py1, px2, py2;

    renderer->pixelToWindowNoMargin(                    0,                      0, &px1, &py1);
    renderer->pixelToWindowNoMargin(renderer->width() - 1, renderer->height() - 1, &px2, &py2);

    return CGnuPlotUtil::map(value().getValue(0.1), 0, 1, py1, py2);
  }
}
