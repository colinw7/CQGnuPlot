#include <CGnuPlotStyleTestTerminal.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotRenderer.h>
#include <CGnuPlotUtil.h>

// TODO: Enhanced text, rounded end cap, ticscale
CGnuPlotStyleTestTerminal::
CGnuPlotStyleTestTerminal() :
 CGnuPlotStyleBase(CGnuPlot::PlotStyle::TEST_TERMINAL)
{
}

void
CGnuPlotStyleTestTerminal::
draw2D(CGnuPlotPlot *plot, CGnuPlotRenderer *renderer)
{
  if (renderer->isPseudo()) {
    renderer->drawRect(CBBox2D(0,0,1,1), CRGBA(0,0,0), 1);
    return;
  }

  //---

  static double grid_dashes[]   = {1, 3};
  static uint   num_grid_dashes = 2;

  //---

  renderer->setRegion(CBBox2D(0, 0, 1, 1));

  double px1, py1, px2, py2;

  renderer->windowToPixel(0.0, 0.0, &px1, &py1);
  renderer->windowToPixel(1.0, 1.0, &px2, &py2);

  double pxm = CGnuPlotUtil::avg({px1, px2});
  double pym = CGnuPlotUtil::avg({py1, py2});

  double pw = px2 - px1;
  double ph = py1 - py2;

  CFontPtr font = renderer->getFont();

  double font_size = font->getCharAscent() + font->getCharDescent();

  //---

  CLineDash lineDash(grid_dashes, num_grid_dashes);

  renderer->drawLine(CPoint2D(0.5, 0.0), CPoint2D(0.5, 1.0), CRGBA(0,0,0), 1, lineDash);
  renderer->drawLine(CPoint2D(0.0, 0.5), CPoint2D(1.0, 0.5), CRGBA(0,0,0), 1, lineDash);

  //---

  {
  CPoint2D p1;

  renderer->pixelToWindow(CPoint2D(px1 + 4, py2 + 4), p1);

  std::string terminalStr = plot->app()->device()->name() + " terminal test";

  CRGBA c(0, 0, 0);

  renderer->drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_LEFT, 0),
                             VAlignPos(CVALIGN_TYPE_TOP, 0), terminalStr, c);
  }

  //---

  // symbols on right
  int    nlines = int(ph/font_size);
  double dy     = 1.0/nlines;

  double lw = 1.0/30;
  int    sw = 24;

  for (int i = 0; i < nlines; ++i) {
    std::string str = CGnuPlotUtil::toString(i - 1);

    double w = font->getStringWidth(str);

    CRGBA                     c      = CGnuPlotStyleInst->indexColor(i - 1);
    CGnuPlotTypes::SymbolType symbol = CGnuPlotStyleInst->indexSymbol(i - 1);
    double                    width  = 1.0;
    CLineDash                 dash   = CGnuPlotStyleInst->indexDash(i - 1);

    double x, y;

    renderer->pixelToWindow(px2 - w - sw - 4, py2 + font->getCharAscent(), &x, &y);

    renderer->drawText(CPoint2D(x - lw, y - i*dy), str, c);

    double x1, y1;

    renderer->pixelToWindow(px2 - sw, py2 + font->getCharAscent()/2, &x1, &y1);

    renderer->drawLine(CPoint2D(x1 - lw, y1 - i*dy), CPoint2D(x1, y1 - i*dy), c, width, dash);

    renderer->pixelToWindow(px2 - sw/2, py2 + font->getCharAscent()/2, &x1, &y1);

    renderer->drawSymbol(CPoint2D(x1, y1 - i*dy), symbol, width, c, 1, true);
  }

  //---

  // text in center
  {
  CPoint2D p1, p2, p3;

  renderer->pixelToWindow(CPoint2D(pxm, pym - 5*font_size), p1);
  renderer->pixelToWindow(CPoint2D(pxm, pym - 4*font_size), p2);
  renderer->pixelToWindow(CPoint2D(pxm, pym - 3*font_size), p3);

  renderer->drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_LEFT  , 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0), "left justified", CRGBA(0,0,0));
  renderer->drawHAlignedText(p2, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0), "centre+d text", CRGBA(0,0,0));
  renderer->drawHAlignedText(p3, HAlignPos(CHALIGN_TYPE_RIGHT , 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0), "right justified", CRGBA(0,0,0));

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(pxm, pym - font_size), p4);

  renderer->drawHAlignedText(p4, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0),
                             "test of character width", CGnuPlotStyleInst->indexColor(4));
  renderer->drawHAlignedText(CPoint2D(0.5, 0.5), HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0),
                             "12345678901234567890", CGnuPlotStyleInst->indexColor(4));

  double w = font->getStringWidth("12345678901234567890");

  double x1, y1, x2, y2;

  renderer->pixelToWindow(pxm - w/2, pym + font_size/2, &x1, &y1);
  renderer->pixelToWindow(pxm + w/2, pym - font_size/2, &x2, &y2);

  renderer->drawRect(CBBox2D(x1, y1, x2, y2), CGnuPlotStyleInst->indexColor(4), 1);
  }

  //---

  {
  // rotated text on left
  CPoint2D p1, p2, p3;

  renderer->pixelToWindow(CPoint2D(px1 + 1*font_size, pym), p1);
  renderer->pixelToWindow(CPoint2D(px1 + 2*font_size, pym), p2);
  renderer->pixelToWindow(CPoint2D(px1 + 3*font_size, pym), p3);

  renderer->drawVAlignedText(p1, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_CENTER, 0),
                             "rotated ce+ntered text", CRGBA(0,1,0));

  renderer->drawRotatedText(p2, "rotated by +45 deg",  45,
                            HAlignPos(CHALIGN_TYPE_LEFT  , 0),
                            VAlignPos(CVALIGN_TYPE_BOTTOM, 0), CRGBA(0,1,0));
  renderer->drawRotatedText(p3, "rotated by -45 deg", -45,
                            HAlignPos(CHALIGN_TYPE_LEFT  , 0),
                            VAlignPos(CVALIGN_TYPE_BOTTOM, 0), CRGBA(0,1,0));
  }

  //---

  // filled polygons
  {
  double pl = pw/16;

  CPoint2D pp1 = CPoint2D(pxm + pw/4, pym - ph/4);
  CPoint2D pp2 = pp1 + CPoint2D(pl, pl);

  CPoint2D p1;

  renderer->pixelToWindow(pp1 - CPoint2D(0, pl), p1);

  renderer->drawHAlignedText(p1, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_BOTTOM, 0),
                             "filled polygons:", CRGBA(0,0,0));

  std::vector<CPoint2D> points1, points2;

  for (int i = 0; i < 6; ++i) {
    double a = CAngle::Deg2Rad(i*60);

    CPoint2D pw1, pw2;

    renderer->pixelToWindow(pp1 + CPoint2D(pl*cos(a), pl*sin(a)), pw1);
    renderer->pixelToWindow(pp2 + CPoint2D(pl*cos(a), pl*sin(a)), pw2);

    points1.push_back(pw1);
    points2.push_back(pw2);
  }

  renderer->fillPolygon(points1, CRGBA(0,0,1));
  renderer->fillPolygon(points2, CRGBA(0,1,0,0.5));
  }

  //---

  // arrows
  {
  CPoint2D ac;
  double   al = 50;

  renderer->pixelToWindow(CPoint2D(pxm - 100, pym + 100), ac);

  for (int i = 0; i < 8; ++i) {
    CGnuPlotArrow arrow(plot->group());

    double a = i*CAngle::Deg2Rad(45);

    double dax =  al*cos(a);
    double day = -al*sin(a);

    CPoint2D ac1;

    renderer->pixelToWindow(CPoint2D(pxm - 100 + dax, pym + 100 + day), ac1);

    arrow.setFrom(CPoint3D(ac .x, ac .y, 0));
    arrow.setTo  (CPoint3D(ac1.x, ac1.y, 0));

    arrow.setHeadLength(renderer->pixelWidthToWindowWidth(al/5));

    arrow.setHeadAngle(30);

    arrow.setFHead(i == 7);
    arrow.setTHead(i != 3 && i != 7);

    arrow.setHeadFilled(i == 2);
    arrow.setHeadEmpty (i == 1 || i == 4 || i == 5 || i == 6 || i == 7);

    arrow.setLineColor(CRGBA(1,0,0));

    arrow.draw(renderer);
  }
  }

  //---

  {
  // pattern fill
  double ptw = pw/30;
  double pth = ph/8;
  double ptb = 4;

  for (int i = 0; i <= 9; ++i) {
    double px = pxm + i*(ptw + ptb);

    CPoint2D p1, p2;

    renderer->pixelToWindow(CPoint2D(px      , py1 - pth), p1);
    renderer->pixelToWindow(CPoint2D(px + ptw, py1      ), p2);

    renderer->patternRect(CBBox2D(p1, p2), CGnuPlotStyleInst->indexPattern(i),
                           CRGBA(0,0,0), CRGBA(1,1,1));

    renderer->drawRect(CBBox2D(p1, p2), CRGBA(0,0,0), 1);

    CPoint2D p3;

    renderer->pixelToWindow(CPoint2D(px + ptw/2, py1 - pth), p3);

    renderer->drawHAlignedText(p3, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                               VAlignPos(CVALIGN_TYPE_BOTTOM, 0),
                               CStrUtil::strprintf("%d", i), CRGBA(0,0,0));
  }

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(pxm + 4*(ptw + ptb), py1 - pth - font_size), p4);

  renderer->drawHAlignedText(p4, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_BOTTOM, 0),
                             "pattern fill", CRGBA(0,0,0));
  }

  //---

  {
  // line width bottom left
  double ll  = pw/8;
  double lb1 = 8;
  double lb2 = 8;

  for (int i = 1; i <= 6; ++i) {
    CPoint2D p1, p2;

    renderer->pixelToWindow(CPoint2D(px1 + lb1     , py1 - i*font_size), p1);
    renderer->pixelToWindow(CPoint2D(px1 + lb1 + ll, py1 - i*font_size), p2);

    renderer->drawLine(p1, p2, CRGBA(0,0,0), i);

    CPoint2D p3;

    renderer->pixelToWindow(CPoint2D(px1 + lb1 + lb2 + ll, py1 - i*font_size), p3);

    renderer->drawHAlignedText(p3, HAlignPos(CHALIGN_TYPE_LEFT, 0),
                               VAlignPos(CVALIGN_TYPE_CENTER, 0),
                               CStrUtil::strprintf("lw %d", i), CRGBA(0,0,0));
  }

  CPoint2D p4;

  renderer->pixelToWindow(CPoint2D(px1 + lb1 + ll/2, py1 - 7*font_size - 4), p4);

  renderer->drawHAlignedText(p4, HAlignPos(CHALIGN_TYPE_CENTER, 0),
                             VAlignPos(CVALIGN_TYPE_TOP, 0),
                             "linewidth", CRGBA(0,0,0));
  }
}
