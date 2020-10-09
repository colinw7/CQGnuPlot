#include <CGnuPlotRenderer.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

#include <CFontMgr.h>
#include <CMathGeom2D.h>

CGnuPlotRenderer::
CGnuPlotRenderer()
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);

  updateMatrix();
}

CGnuPlotRenderer::
~CGnuPlotRenderer()
{
}

void
CGnuPlotRenderer::
setRange(const CBBox2D &r)
{
  range1_ = r;
  range2_ = r;

  updateMatrix();
}

void
CGnuPlotRenderer::
setRange(const CBBox2D &r1, const CBBox2D &r2)
{
  range1_ = r1;
  range2_ = r2;

  updateMatrix();
}

void
CGnuPlotRenderer::
setFont(const CFontPtr &font)
{
  font_ = font;
}

double
CGnuPlotRenderer::
fontSize() const
{
  return font_->getSize();
}

void
CGnuPlotRenderer::
setFontSize(double s)
{
  font_ = font_->dup(font_->getFamily(), font_->getStyle(), s);
}

void
CGnuPlotRenderer::
setXScale(double x)
{
  xscale_ = x;

  updateMatrix();
}

void
CGnuPlotRenderer::
setYScale(double y)
{
  yscale_ = y;

  updateMatrix();
}

void
CGnuPlotRenderer::
setScale(double x, double y)
{
  xscale_ = x;
  yscale_ = y;

  updateMatrix();
}

void
CGnuPlotRenderer::
setOffset(const CPoint2D &o)
{
  offset_ = o;

  updateMatrix();
}

void
CGnuPlotRenderer::
updateMatrix()
{
  CPoint2D c  = (range1_.isSet() ? range1_.getCenter() : CPoint2D(0, 0));
  CPoint2D c1 = c + offset_;

  CMatrix2D matrix1, matrix2, matrix3;

  matrix1.setTranslation(c.x, c.y);
  matrix2.setScale(xscale_, yscale_);
  matrix3.setTranslation(-c1.x, -c1.y);

  matrix_  = matrix1*matrix2*matrix3;
  imatrix_ = matrix_.inverse();
}

//---

void
CGnuPlotRenderer::
initHidden()
{
  hiddenObjects_.clear();
}

void
CGnuPlotRenderer::
drawHidden(bool grayDepth)
{
  if (hiddenObjects_.empty())
    return;

  double zmin = hiddenObjects_. begin()->first;
  double zmax = hiddenObjects_.rbegin()->first;

  for (auto &zo : hiddenObjects_) {
    hiddenZ_ = (grayDepth ? CGnuPlotUtil::map(zo.first, zmin, zmax, 0.25, 1) : 1);

    for (auto &o : zo.second) {
      o->draw(this);
    }
  }
}

//---

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
fillHiddenPolygon(const Points2D &points, double z, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    return patternHiddenPolygon(points, z, fill.pattern(), fill.color(), fill.background());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    return fillHiddenPolygon(z, points, fill.color());
  }
  else
    return CGnuPlotHiddenObjectP();
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
fillHiddenPolygon(double z, const Points2D &points, const CRGBA &c)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenFillPolygon(points, c));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
fillClippedPolygon(const Points3D &points, const CGnuPlotFill &fill)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  fillClippedPolygon(points1, fill);
}

void
CGnuPlotRenderer::
fillClippedPolygon(const Points2D &points, const CGnuPlotFill &fill)
{
  if (! isPseudo() && clip().isValid()) {
    Points2D ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip().getValue(), ipoints))
      fillPolygon(ipoints, fill);
  }
  else
    fillPolygon(points, fill);
}

void
CGnuPlotRenderer::
fillClippedPolygon(const Points3D &points, const CRGBA &c)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  fillClippedPolygon(points1, c);
}

void
CGnuPlotRenderer::
fillPolygon(const Points3D &points, const CGnuPlotFill &fill)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  fillPolygon(points1, fill);
}

void
CGnuPlotRenderer::
fillPolygon(const Points2D &points, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    patternPolygon(points, fill.pattern(), fill.color(), fill.background());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    fillPolygon(points, fill.color());
  }
}

void
CGnuPlotRenderer::
fillClippedPolygon(const Points2D &points, const CRGBA &c)
{
  if (! isPseudo() && clip().isValid()) {
    Points2D ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip().getValue(), ipoints))
      fillPolygon(ipoints, c);
  }
  else
    fillPolygon(points, c);
}

void
CGnuPlotRenderer::
fillPolygon(const Points3D &points, const CRGBA &c)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  fillPolygon(points1, c);
}

//---

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
strokeHiddenPolygon(const Points2D &points, double z, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    return drawHiddenPolygon(z, points, stroke.color(), stroke.width(), stroke.lineDash());
  else
    return CGnuPlotHiddenObjectP();
}

void
CGnuPlotRenderer::
strokeClippedPolygon(const Points3D &points, const CGnuPlotStroke &stroke)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  strokeClippedPolygon(points1, stroke);
}

void
CGnuPlotRenderer::
strokeClippedPolygon(const Points2D &points, const CGnuPlotStroke &stroke)
{
  if (! isPseudo() && clip().isValid()) {
    Points2D ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip().getValue(), ipoints))
      strokePolygon(ipoints, stroke);
  }
  else
    strokePolygon(points, stroke);
}

void
CGnuPlotRenderer::
strokePolygon(const Points3D &points, const CGnuPlotStroke &stroke)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  strokePolygon(points1, stroke);
}

void
CGnuPlotRenderer::
strokePolygon(const Points2D &points, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawPolygon(points, stroke.color(), stroke.width(), stroke.lineDash());
}

//------

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenPolygon(double z, const Points2D &points, const CRGBA &c,
                  double width, const CLineDash &dash)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenDrawPolygon(points, c, width, dash));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawClippedPolygon(const Points3D &points, const CRGBA &c, double width, const CLineDash &dash)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  drawClippedPolygon(points1, c, width, dash);
}

void
CGnuPlotRenderer::
drawClippedPolygon(const Points2D &points, const CRGBA &c, double width, const CLineDash &dash)
{
  if (! isPseudo() && clip().isValid()) {
    Points2D ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip().getValue(), ipoints))
      drawPolygon(ipoints, c, width, dash);
  }
  else
    drawPolygon(points, c, width, dash);
}

void
CGnuPlotRenderer::
drawPolygon(const Points3D &points, const CRGBA &c, double lw, const CLineDash &dash)
{
  Points2D points1;

  for (const auto &p : points) {
    CPoint2D p1 = transform2D(p);

    points1.push_back(p1);
  }

  drawPolygon(points1, c, lw, dash);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
patternHiddenPolygon(const Points2D &points, double z, FillPattern pattern,
                     const CRGBA &fg, const CRGBA &bg)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenPatternPolygon(points, pattern, fg, bg));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
patternClippedPolygon(const Points2D &points, FillPattern pattern,
                      const CRGBA &fg, const CRGBA &bg)
{
  if (! isPseudo() && clip().isValid()) {
    Points2D ipoints;

    if (CMathGeom2D::IntersectPolygon(points, clip().getValue(), ipoints))
      patternPolygon(ipoints, pattern, fg, bg);
  }
  else
    patternPolygon(points, pattern, fg, bg);
}

void
CGnuPlotRenderer::
drawClippedEllipse(const CPoint2D &p, double rx, double ry, double a,
                   const CRGBA &c, double width, const CLineDash &dash)
{
  if (! isPseudo() && clip().isValid()) {
    if (a == 0) {
      int    na = 120;
      double da = 2*M_PI/na;

      Points2D points;

      for (int i = 0; i < na; ++i) {
        double a1 = i*da;

        double x, y;

        CMathGeom2D::EllipsePointAtAngle(p.x, p.y, rx, ry, a1, &x, &y);

        points.push_back(CPoint2D(x, y));
      }

      drawClippedPolygon(points, c, width, dash);
    }
    else
      drawEllipse(p, rx, ry, a, c, width, dash);
  }
  else
    drawEllipse(p, rx, ry, a, c, width, dash);
}

void
CGnuPlotRenderer::
fillClippedEllipse(const CPoint2D &p, double rx, double ry, double a, const CRGBA &c)
{
  if (! isPseudo() && clip().isValid()) {
    if (a == 0) {
      int    na = 120;
      double da = 2*M_PI/na;

      Points2D points;

      for (int i = 0; i < na; ++i) {
        double a1 = i*da;

        double x, y;

        CMathGeom2D::EllipsePointAtAngle(p.x, p.y, rx, ry, a1, &x, &y);

        points.push_back(CPoint2D(x, y));
      }

      fillClippedPolygon(points, c);
    }
    else
      fillEllipse(p, rx, ry, a, c);
  }
  else
    fillEllipse(p, rx, ry, a, c);
}

void
CGnuPlotRenderer::
drawRect(const CBBox3D &rect, const CRGBA &c, double width, const CLineDash &dash)
{
  CPoint3D p11(rect.getXMin(), rect.getYMin(), rect.getZMin());
  CPoint3D p21(rect.getXMax(), rect.getYMin(), rect.getZMin());
  CPoint3D p31(rect.getXMax(), rect.getYMax(), rect.getZMin());
  CPoint3D p41(rect.getXMin(), rect.getYMax(), rect.getZMin());
  CPoint3D p12(rect.getXMin(), rect.getYMin(), rect.getZMax());
  CPoint3D p22(rect.getXMax(), rect.getYMin(), rect.getZMax());
  CPoint3D p32(rect.getXMax(), rect.getYMax(), rect.getZMax());
  CPoint3D p42(rect.getXMin(), rect.getYMax(), rect.getZMax());

  CPoint2D pt11 = transform2D(p11);
  CPoint2D pt21 = transform2D(p21);
  CPoint2D pt31 = transform2D(p31);
  CPoint2D pt41 = transform2D(p41);
  CPoint2D pt12 = transform2D(p12);
  CPoint2D pt22 = transform2D(p22);
  CPoint2D pt32 = transform2D(p32);
  CPoint2D pt42 = transform2D(p42);

  drawLine(pt11, pt21, c, width, dash);
  drawLine(pt21, pt31, c, width, dash);
  drawLine(pt31, pt41, c, width, dash);
  drawLine(pt41, pt11, c, width, dash);

  drawLine(pt11, pt12, c, width, dash);
  drawLine(pt21, pt22, c, width, dash);
  drawLine(pt31, pt32, c, width, dash);
  drawLine(pt41, pt42, c, width, dash);

  drawLine(pt12, pt22, c, width, dash);
  drawLine(pt22, pt32, c, width, dash);
  drawLine(pt32, pt42, c, width, dash);
  drawLine(pt42, pt12, c, width, dash);
}

void
CGnuPlotRenderer::
drawClippedRect(const CBBox2D &rect, const CRGBA &c, double width, const CLineDash &dash)
{
  if (clip().isValid() && ! isPseudo()) {
    if      (clip().getValue().inside(rect))
      drawRect(rect, c, width, dash);
    else if (clip().getValue().intersect(rect)) {
      drawClipLine(rect.getLL(), rect.getLR(), c, width, dash);
      drawClipLine(rect.getLR(), rect.getUR(), c, width, dash);
      drawClipLine(rect.getUR(), rect.getUL(), c, width, dash);
      drawClipLine(rect.getUL(), rect.getLL(), c, width, dash);
    }
  }
  else
    drawRect(rect, c, width, dash);
}

//----

void
CGnuPlotRenderer::
fillClippedRect(const CBBox2D &rect, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    patternRect(rect, fill.pattern(), fill.color(), fill.background());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    fillClippedRect(rect, fill.color());
  }
}

void
CGnuPlotRenderer::
fillRect(const CBBox2D &rect, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    patternRect(rect, fill.pattern(), fill.color(), fill.background());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    fillRect(rect, fill.color());
  }
}

void
CGnuPlotRenderer::
fillClippedRect(const CBBox2D &rect, const CRGBA &c)
{
  if (clip().isValid() && ! isPseudo()) {
    CBBox2D crect;

    if      (clip().getValue().inside(rect))
      fillRect(rect, c);
    else if (clip().getValue().intersect(rect, crect))
      fillRect(crect, c);
  }
  else
    fillRect(rect, c);
}

void
CGnuPlotRenderer::
patternClippedRect(const CBBox2D &rect, FillPattern pattern, const CRGBA &fg, const CRGBA &bg)
{
  if (clip().isValid() && ! isPseudo()) {
    CBBox2D crect;

    if      (clip().getValue().inside(rect))
      patternRect(rect, pattern, fg, bg);
    else if (clip().getValue().intersect(rect, crect))
      patternRect(crect, pattern, fg, bg);
  }
  else
    patternRect(rect, pattern, fg, bg);
}

//---

void
CGnuPlotRenderer::
strokeRect(const CBBox2D &rect, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawRect(rect, stroke.color(), stroke.width(), stroke.lineDash());
}

//---

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
strokeHiddenLine(const CPoint2D &p1, const CPoint2D &p2, double z, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    return drawHiddenLine(p1, p2, z, stroke.color(), stroke.width(), stroke.lineDash());
  else
    return CGnuPlotHiddenObjectP();
}

void
CGnuPlotRenderer::
strokeClipLine(const CPoint2D &p1, const CPoint2D &p2, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawClipLine(p1, p2, stroke.color(), stroke.width(), stroke.lineDash());
}

void
CGnuPlotRenderer::
drawClipLine(const CPoint2D &p1, const CPoint2D &p2, const CRGBA &c, double width,
             const CLineDash &dash)
{
  if (! isPseudo()) {
    CPoint2D p11 = p1;
    CPoint2D p21 = p2;

    if (clipLine(p11, p21))
      drawLine(p11, p21, c, width, dash);
  }
  else
    drawLine(p1, p2, c, width, dash);
}

void
CGnuPlotRenderer::
drawRotatedRect(const CBBox2D &rect, double a, const CRGBA &c, double width,
                const CLineDash &dash, const COptPoint2D &o)
{
  CPoint2D o1 = (o.isValid() ? o.getValue() : rect.getCenter());

  CPoint2D p1 = rotatePoint(rect.getLL(), a, o1);
  CPoint2D p2 = rotatePoint(rect.getLR(), a, o1);
  CPoint2D p3 = rotatePoint(rect.getUR(), a, o1);
  CPoint2D p4 = rotatePoint(rect.getUL(), a, o1);

  drawLine(p1, p2, c, width, dash);
  drawLine(p2, p3, c, width, dash);
  drawLine(p3, p4, c, width, dash);
  drawLine(p4, p1, c, width, dash);
}

void
CGnuPlotRenderer::
fillRotatedRect(const CBBox2D &rect, double a, const CRGBA &c, const COptPoint2D &o)
{
  CPoint2D o1 = (o.isValid() ? o.getValue() : rect.getCenter());

  CPoint2D p1 = rotatePoint(rect.getLL(), a, o1);
  CPoint2D p2 = rotatePoint(rect.getLR(), a, o1);
  CPoint2D p3 = rotatePoint(rect.getUR(), a, o1);
  CPoint2D p4 = rotatePoint(rect.getUL(), a, o1);

  Points2D points({p1, p2, p3, p4});

  fillPolygon(points, c);
}

bool
CGnuPlotRenderer::
clipLine(CPoint2D &p1, CPoint2D &p2)
{
  if (! clip().isValid() || isPseudo())
    return true;

  double x1 = p1.x, y1 = p1.y;
  double x2 = p2.x, y2 = p2.y;

  if (! CMathGeom2D::clipLine(clip().getValue().getXMin(), clip().getValue().getYMin(),
                              clip().getValue().getXMax(), clip().getValue().getYMax(),
                              &x1, &y1, &x2, &y2))
    return false;

  p1 = CPoint2D(x1, y1);
  p2 = CPoint2D(x2, y2);

  return true;
}

//---

void
CGnuPlotRenderer::
fillEllipse(const CPoint2D &p, double rx, double ry, double a, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    patternEllipse(p, rx, ry, a, fill.pattern(), fill.color(), fill.background());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    fillEllipse(p, rx, ry, a, fill.color());
  }
}

void
CGnuPlotRenderer::
fillEllipse(const CBBox2D &rect, const CRGBA &c)
{
  fillEllipse(rect.getCenter(), rect.getWidth()/2, rect.getHeight()/2, 0, c);
}

//---

 void
CGnuPlotRenderer::
strokeEllipse(const CPoint2D &p, double rx, double ry, double a, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawEllipse(p, rx, ry, a, stroke.color(), stroke.width(), stroke.lineDash());
}

void
CGnuPlotRenderer::
drawEllipse(const CPoint3D &pos, double dx, double ry, double angle, const CRGBA &c,
            double width, const CLineDash &dash)
{
  CPoint2D pos1 = transform2D(pos);

  drawEllipse(pos1, dx, ry, angle, c, width, dash);
}

void
CGnuPlotRenderer::
drawEllipse(const CBBox2D &rect, const CRGBA &c, double width, const CLineDash &dash)
{
  drawEllipse(rect.getCenter(), rect.getWidth()/2, rect.getHeight()/2, 0, c, width, dash);
}

//---

CBBox2D
CGnuPlotRenderer::
drawTextBox(const CPoint2D &p, const std::string &str, bool enhanced, const CPoint2D &offset,
            const CRGBA &bg, const CRGBA &fg, double width)
{
  CFontPtr font = getFont();

  CBBox2D bbox;

  CGnuPlotText text(str);

  double tw = pixelWidthToWindowWidth  (font->getStringWidth(str));
  double th = pixelHeightToWindowHeight(font->getCharHeight());

  if (enhanced) {
    bbox = text.calcBBox(this).moveBy(p);

    bbox.setYMax(p.y);
  }
  else {
    bbox = CBBox2D(p, p + CPoint2D(tw, th));
  }

  CPoint2D d(-bbox.getWidth()/2, -bbox.getHeight()/2);

  CPoint2D of(0,0);

  if (offset.x || offset.y) {
    double tw1 = pixelWidthToWindowWidth(font->getStringWidth("X"));

    of = offset*CPoint2D(tw1, th);
  }

  bbox.moveBy(d + of);

  fillRect(bbox, bg);

  CLineDash dash;

  drawRect(bbox, fg, width, dash);

  return bbox;
}

//---

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenEnhancedText(const CBBox2D &bbox, double z, const HAlignPos &halignPos,
                       const std::string &str, const CFontPtr &font, const CRGBA &c)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenEnhancedText(bbox, str, halignPos, font, c));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawEnhancedText(const CBBox2D &bbox, const HAlignPos &halignPos, const std::string &str,
                 const CFontPtr &font, const CRGBA &c)
{
  setFont(font);

  CGnuPlotText text(str);

  text.draw(this, bbox, halignPos.first, c);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenHAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                       const VAlignPos &valignPos, const std::string &str,
                       const CFontPtr &font, const CRGBA &c, double a)
{
  setFont(font);

  CPoint3D pos1 = transform(pos);

  return drawHiddenHAlignedText(pos1.toPoint2D(), pos1.z, halignPos, valignPos, str, font, c, a);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenHAlignedText(const CPoint2D &pos, double z, const HAlignPos &halignPos,
                       const VAlignPos &valignPos, const std::string &str,
                       const CFontPtr &font, const CRGBA &c, double a)
{
  CGnuPlotHiddenObjectP obj(
     new CGnuPlotHiddenHAlignedText(pos, halignPos, valignPos, str, font, c, a));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                 const VAlignPos &valignPos, const std::string &str,
                 const CRGBA &c, double a)
{
  CPoint2D pos1 = transform2D(pos);

  drawHAlignedText(pos1, halignPos, valignPos, str, c, a);
}

void
CGnuPlotRenderer::
calcTextRectAtPoint(const CPoint2D &pos, const std::string &str, bool enhanced,
                    const HAlignPos &halignPos, const VAlignPos &valignPos,
                    double a, CBBox2D &bbox, CBBox2D &rbbox)
{
  if (str == "")
    return;

  //---

  CPoint2D pos1;

  windowToRegion(pos.x, pos.y, &pos1.x, &pos1.y);

  //---

  CBBox2D        range  = this->range();
//CGnuPlotMargin margin = this->margin();

  this->setRange(CBBox2D(0, 0, 1, 1));
//this->resetMargin();

  CGnuPlotText text(str);

  text.setEnhanced(enhanced);

  text.calcRectAtPoint(this, pos1, halignPos.first, valignPos.first, a, bbox, rbbox);

  this->setRange (range);
//this->setMargin(margin);

  //---

  if (bbox.isSet()) {
    CPoint2D rpos1, rpos2;

    regionToWindow(bbox.getXMin(), bbox.getYMin(), &rpos1.x, &rpos1.y);
    regionToWindow(bbox.getXMax(), bbox.getYMax(), &rpos2.x, &rpos2.y);

    bbox = CBBox2D(rpos1, rpos2);
  }

  if (rbbox.isSet()) {
    CPoint2D rpos1, rpos2;

    regionToWindow(rbbox.getXMin(), rbbox.getYMin(), &rpos1.x, &rpos1.y);
    regionToWindow(rbbox.getXMax(), rbbox.getYMax(), &rpos2.x, &rpos2.y);

    rbbox = CBBox2D(rpos1, rpos2);
  }
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenTextAtPoint(const CPoint3D &p, const std::string &str, bool enhanced,
                      const HAlignPos &halignPos, const VAlignPos &valignPos, const CFontPtr &font,
                      const CRGBA &c, double angle)
{
  CPoint3D pt = transform(p);

  return drawHiddenTextAtPoint(pt.toPoint2D(), pt.z, str, enhanced, halignPos, valignPos,
                               font, c, angle);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenTextAtPoint(const CPoint2D &p, double z, const std::string &str, bool enhanced,
                      const HAlignPos &halignPos, const VAlignPos &valignPos, const CFontPtr &font,
                      const CRGBA &c, double angle)
{
  CGnuPlotHiddenObjectP obj(
    new CGnuPlotHiddenTextAtPoint(p, str, enhanced, halignPos, valignPos, font, c, angle));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawTextAtPoint(const CPoint3D &pos, const std::string &str, bool enhanced,
                const HAlignPos &halignPos, const VAlignPos &valignPos,
                const CRGBA &c, double angle)
{
  CPoint3D pos1 = transform(pos);

  drawTextAtPoint(pos1.toPoint2D(), str, enhanced, halignPos, valignPos, c, angle);
}

void
CGnuPlotRenderer::
drawTextAtPoint(const CPoint2D &pos, const std::string &str, bool enhanced,
                const HAlignPos &halignPos, const VAlignPos &valignPos,
                const CRGBA &c, double angle)
{
  if (isPseudo()) {
    drawPoint(pos, c);
    return;
  }

  if (str == "")
    return;

  //---

  CPoint2D pos1;

  windowToRegion(pos.x, pos.y, &pos1.x, &pos1.y);

  //---

  CBBox2D        range  = this->range();
//CGnuPlotMargin margin = this->margin();

  this->setRange(CBBox2D(0, 0, 1, 1));
//this->resetMargin();

  CGnuPlotText text(str);

  text.setEnhanced(enhanced);

  text.drawAtPoint(this, pos1, halignPos.first, valignPos.first, c, angle);

  this->setRange (range);
//this->setMargin(margin);
}

void
CGnuPlotRenderer::
drawHAlignedText(const CPoint2D &pos, const HAlignPos &halignPos,
                 const VAlignPos &valignPos, const std::string &str,
                 const CRGBA &c, double a)
{
  if (fabs(a) > 1E-6)
    return drawRotatedText(pos, str, a, halignPos, valignPos, c);

  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharAscent() + font->getCharDescent();
//double font_size = font->getCharHeight();

  CBBox2D bbox = getHAlignedTextBBox(str);

//double width1  = bbox.getWidth();
  double height1 = bbox.getHeight();

  //------

  // Draw each line using X and Y alignment

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  double x1 = 0, y1 = 0;

  if      (valignPos.first == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valignPos.first == CVALIGN_TYPE_CENTER)
    y1 = pos.y + height1/2;
  else if (valignPos.first == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y + height1;

  y1 -= pixelHeightToWindowHeight(font->getCharAscent());

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    if      (halignPos.first == CHALIGN_TYPE_LEFT)
      x1 = pos.x;
    else if (halignPos.first == CHALIGN_TYPE_CENTER)
      x1 = pos.x - pixelWidthToWindowWidth(width2/2);
    else if (halignPos.first == CHALIGN_TYPE_RIGHT)
      x1 = pos.x - pixelWidthToWindowWidth(width2);

    CPoint2D p1(x1 + pixelWidthToWindowWidth  (halignPos.second),
                y1 - pixelHeightToWindowHeight(valignPos.second));

    drawText(p1, str2, c);

    y1 -= pixelHeightToWindowHeight(font_size);

    str1 = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  if      (halignPos.first == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halignPos.first == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(width2/2);
  else if (halignPos.first == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(width2);

  CPoint2D p1(x1 + pixelWidthToWindowWidth  (halignPos.second),
              y1 - pixelHeightToWindowHeight(valignPos.second));

  drawText(p1, str1, c);
}

void
CGnuPlotRenderer::
drawHTextInBox(const CBBox2D &bbox, const std::string &str, const HAlignPos &halignPos,
               const CRGBA &c)
{
  CFontPtr font = getFont();

  double fa = pixelHeightToWindowHeight(font->getCharAscent());
  double fd = pixelHeightToWindowHeight(font->getCharDescent());

  //------

  // Draw each line using X and Y alignment

  std::string str1 = str;

  auto p1 = str1.find('\n');

  double y = bbox.getTop() - fa;

  while (p1 != std::string::npos) {
    std::string str2 = str1.substr(0, p1);

    double w = pixelWidthToWindowWidth(font->getStringWidth(str2));

    double x = bbox.getLeft();

    if      (halignPos.first == CHALIGN_TYPE_LEFT)
      ;
    else if (halignPos.first == CHALIGN_TYPE_CENTER)
      x = bbox.getCenter().x - w/2;
    else if (halignPos.first == CHALIGN_TYPE_RIGHT)
      x = bbox.getRight() - w;

    drawText(CPoint2D(x, y), str2, c);

    y -= fa + fd;

    str1 = str1.substr(p1 + 1);
    p1   = str1.find('\n');
  }

  double w = pixelWidthToWindowWidth(font->getStringWidth(str1));

  double x = bbox.getLeft();

  if      (halignPos.first == CHALIGN_TYPE_LEFT)
    ;
  else if (halignPos.first == CHALIGN_TYPE_CENTER)
    x = bbox.getCenter().x - w/2;
  else if (halignPos.first == CHALIGN_TYPE_RIGHT)
    x = bbox.getRight() - w;

  drawText(CPoint2D(x, y), str1, c);
}

void
CGnuPlotRenderer::
drawVAlignedText(const CPoint3D &pos, const HAlignPos &halignPos,
                 const VAlignPos &valignPos, const std::string &str,
                 const CRGBA &c, double a)
{
  CPoint2D pos1 = transform2D(pos);

  drawVAlignedText(pos1, halignPos, valignPos, str, c, a);
}

void
CGnuPlotRenderer::
drawVAlignedText(const CPoint2D &pos, const HAlignPos &halignPos,
                 const VAlignPos &valignPos, const std::string &str,
                 const CRGBA &c, double a)
{
  if (fabs(a) > 1E-6)
    return drawRotatedText(pos, str, a, halignPos, valignPos, c);

  CFontPtr font = getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharHeight();

  double width1  = 0;
  double height1 = 0;

  //------

  // Draw each line using X and Y alignment

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size + 1;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1 = 0, y1 = 0;

  if      (halignPos.first == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halignPos.first == CHALIGN_TYPE_CENTER)
    x1 = pos.x - pixelWidthToWindowWidth(height1/2);
  else if (halignPos.first == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - pixelWidthToWindowWidth(height1);

  x1 += pixelWidthToWindowWidth(font->getCharAscent());

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width3 = font->getStringWidth(str2);

    if      (valignPos.first == CVALIGN_TYPE_TOP)
      y1 = pos.y;
    else if (valignPos.first == CVALIGN_TYPE_CENTER)
      y1 = pos.y - pixelHeightToWindowHeight(width3/2);
    else if (valignPos.first == CVALIGN_TYPE_BOTTOM)
      y1 = pos.y - pixelHeightToWindowHeight(width3);

    CPoint2D w(x1 + pixelWidthToWindowWidth  (halignPos.second),
               y1 - pixelHeightToWindowHeight(valignPos.second));

    drawText(w, str2, c);

    x1 += pixelWidthToWindowWidth(font_size + 1);

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getStringWidth(str1);

  if      (valignPos.first == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valignPos.first == CVALIGN_TYPE_CENTER)
    y1 = pos.y - pixelHeightToWindowHeight(width2/2);
  else if (valignPos.first == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y - pixelHeightToWindowHeight(width2);

  CPoint2D w(x1 + pixelWidthToWindowWidth  (halignPos.second),
             y1 - pixelHeightToWindowHeight(valignPos.second));

  CFontPtr rfont = font->rotated(90);

  setFont(rfont);

  drawText(w, str1, c);

  setFont(font);
}

CBBox2D
CGnuPlotRenderer::
getHAlignedTextBBox(const std::string &str)
{
  CPoint2D pos(0, 0);
  CBBox2D  bbox(0, 0, 0, 0);

  CFontPtr font = getFont();

  double fa = pixelHeightToWindowHeight(font->getCharAscent());
  double fd = pixelHeightToWindowHeight(font->getCharDescent());

  std::string str1 = str;

  auto pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double w = pixelWidthToWindowWidth(font->getStringWidth(str2));

    bbox.add(CPoint2D(pos.x + w, pos.y - fd));
    bbox.add(CPoint2D(pos.x + w, pos.y + fa));

    pos.y -= fa + fd;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double w = pixelWidthToWindowWidth(font->getStringWidth(str1));

  bbox.add(CPoint2D(pos.x + w, pos.y - fd));
  bbox.add(CPoint2D(pos.x + w, pos.y + fa));

  return bbox;
}

//------

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
strokeHiddenPath(const Points2D &points, double z, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    return drawHiddenPath(points, z, stroke.color(), stroke.width(), stroke.lineDash());
  else
    return CGnuPlotHiddenObjectP();
}

void
CGnuPlotRenderer::
strokePath(const Points3D &points, const CGnuPlotStroke &stroke)
{
  Points2D points1;

  for (const auto &p : points)
    points1.push_back(transform2D(p));

  strokePath(points1, stroke);
}

void
CGnuPlotRenderer::
strokePath(const Points2D &points, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawPath(points, stroke);
}

void
CGnuPlotRenderer::
strokeClippedPath(const Points3D &points, const CGnuPlotStroke &stroke)
{
  Points2D points1;

  for (const auto &p : points)
    points1.push_back(transform2D(p));

  strokeClippedPath(points1, stroke);
}

void
CGnuPlotRenderer::
strokeClippedPath(const Points2D &points, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawClippedPath(points, stroke.color(), stroke.width(), stroke.lineDash());
}

//------

void
CGnuPlotRenderer::
drawHiddenClippedPath(const Points3D &points, const CRGBA &c, double width, const CLineDash &dash)
{
  for (uint i = 0; i < points.size() - 1; ++i)
    drawHiddenLine(points[i], points[i + 1], c, width, dash);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenPath(const Points2D &points, double z, const CRGBA &c, double width,
               const CLineDash &dash)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenPath(points, c, width, dash));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawClippedPath(const Points3D &points, const CRGBA &c, double width, const CLineDash &dash)
{
  CGnuPlotStroke stroke;

  stroke.setColor   (c);
  stroke.setWidth   (width);
  stroke.setLineDash(dash);

  if (clip().isValid() && ! isPseudo()) {
    Points2D points1;

    for (const auto &p : points) {
      CPoint2D p1 = transform2D(p);

      if (clip().getValue().inside(p1))
        points1.push_back(p1);
    }

    drawPath(points1, stroke);
  }
  else
    drawPath(points, stroke);
}

void
CGnuPlotRenderer::
drawClippedPath(const Points2D &points, const CRGBA &c, double width, const CLineDash &dash)
{
  int np = points.size();
  if (np < 2) return;

  CGnuPlotStroke stroke;

  stroke.setColor   (c);
  stroke.setWidth   (width);
  stroke.setLineDash(dash);

  Points2D points1;

  for (int i = 1; i < np; ++i) {
    CPoint2D p1 = points[i - 1];
    CPoint2D p2 = points[i    ];

    if (clipLine(p1, p2)) {
      if (points1.empty() || points1.back() != p1)
        points1.push_back(p1);

      points1.push_back(p2);
    }
    else {
      if (! points1.empty())
        drawPath(points1, stroke);

      points1.clear();
    }
  }

  if (! points1.empty())
    drawPath(points1, stroke);
}

void
CGnuPlotRenderer::
drawPath(const Points3D &points, const CGnuPlotStroke &stroke)
{
  Points2D points1;

  for (const auto &p : points)
    points1.push_back(transform2D(p));

  drawPath(points1, stroke);
}

//------

void
CGnuPlotRenderer::
drawMark(const CPoint3D &p, const CGnuPlotMark &mark)
{
  drawMark(transform2D(p), mark);
}

void
CGnuPlotRenderer::
drawMark(const CPoint2D &p, const CGnuPlotMark &mark)
{
  drawSymbol(p, mark.type(), mark.size(), mark.color(), mark.width(), mark.isPixelSize());
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenSymbol(const CPoint3D &p, SymbolType type, double size, const CRGBA &c,
                 double lw, bool pixelSize)
{
  CPoint3D pt = transform(p);

  return drawHiddenSymbol(pt.toPoint2D(), pt.z, type, size, c, lw, pixelSize);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenSymbol(const CPoint2D &p, double z, SymbolType type, double size, const CRGBA &c,
                 double lw, bool pixelSize)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenSymbol(p, type, size, c, lw, pixelSize));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawSymbol(const CPoint3D &p, SymbolType type, double size, const CRGBA &c,
           double lw, bool pixelSize)
{
  drawSymbol(transform2D(p), type, size, c, lw, pixelSize);
}

void
CGnuPlotRenderer::
strokePieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
               double angle2, bool wedge, const CGnuPlotStroke &stroke)
{
  if (stroke.isEnabled())
    drawPieSlice(pc, ri, ro, angle1, angle2, wedge,
                 stroke.color(), stroke.width(), stroke.lineDash());
}

void
CGnuPlotRenderer::
fillPieSlice(const CPoint2D &pc, double ri, double ro, double angle1,
             double angle2, const CGnuPlotFill &fill)
{
  if      (fill.type() == CGnuPlotTypes::FillType::PATTERN) {
    // TODO: pattern ??
    fillPieSlice(pc, ri, ro, angle1, angle2, fill.color());
  }
  else if (fill.type() == CGnuPlotTypes::FillType::SOLID) {
    fillPieSlice(pc, ri, ro, angle1, angle2, fill.color());
  }
}

void
CGnuPlotRenderer::
drawPixelLine(const CPoint2D &p1, const CPoint2D &p2, const CRGBA &c, double width,
              const CLineDash &dash)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(p1.x, p1.y, &wx1, &wy1);
  windowToPixel(p2.x, p2.y, &wx2, &wy2);

  drawLine(CPoint2D(wx1, wy1), CPoint2D(wx2, wy2), c, width, dash);
}

void
CGnuPlotRenderer::
drawPoint(const CPoint3D &p, const CRGBA &c)
{
  drawPoint(transform2D(p), c);
}

//------

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c, double width,
               const CLineDash &dash)
{
  CPoint3D pt1 = transform(p1);
  CPoint3D pt2 = transform(p2);

  double zm = (pt1.z + pt2.z)/2;

  return drawHiddenLine(pt1.toPoint2D(), pt2.toPoint2D(), zm, c, width, dash);
}

CGnuPlotHiddenObjectP
CGnuPlotRenderer::
drawHiddenLine(const CPoint2D &p1, const CPoint2D &p2, double z, const CRGBA &c,
               double width, const CLineDash &dash)
{
  CGnuPlotHiddenObjectP obj(new CGnuPlotHiddenLine(p1, p2, c, width, dash));

  hiddenObjects_[-z].push_back(obj);

  return obj;
}

void
CGnuPlotRenderer::
drawLine(const CPoint3D &p1, const CPoint3D &p2, const CRGBA &c, double width,
         const CLineDash &dash)
{
  drawLine(transform2D(p1), transform2D(p2), c, width, dash);
}

//------

double
CGnuPlotRenderer::
pixelWidthToWindowWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(w, w, &wx2, &wy2);

  return CGnuPlotUtil::sign(w)*fabs(wx2 - wx1);
}

double
CGnuPlotRenderer::
pixelHeightToWindowHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindow(0, 0, &wx1, &wy1);
  pixelToWindow(h, h, &wx2, &wy2);

  return CGnuPlotUtil::sign(h)*fabs(wy1 - wy2);
}

double
CGnuPlotRenderer::
pixelWidthToWindowWidthNoMargin(double w)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindowNoMargin(0, 0, &wx1, &wy1);
  pixelToWindowNoMargin(w, w, &wx2, &wy2);

  return CGnuPlotUtil::sign(w)*fabs(wx2 - wx1);
}

double
CGnuPlotRenderer::
pixelHeightToWindowHeightNoMargin(double h)
{
  double wx1, wy1, wx2, wy2;

  pixelToWindowNoMargin(0, 0, &wx1, &wy1);
  pixelToWindowNoMargin(h, h, &wx2, &wy2);

  return CGnuPlotUtil::sign(h)*fabs(wy1 - wy2);
}

double
CGnuPlotRenderer::
windowWidthToPixelWidth(double w)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(w, w, &wx2, &wy2);

  return CGnuPlotUtil::sign(w)*fabs(wx2 - wx1);
}

double
CGnuPlotRenderer::
windowHeightToPixelHeight(double h)
{
  double wx1, wy1, wx2, wy2;

  windowToPixel(0, 0, &wx1, &wy1);
  windowToPixel(h, h, &wx2, &wy2);

  return CGnuPlotUtil::sign(h)*fabs(wy2 - wy1);
}

void
CGnuPlotRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  windowToPixel(w.x, w.y, &p.x, &p.y);
}

void
CGnuPlotRenderer::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  pixelToWindow(p.x, p.y, &w.x, &w.y);
}

void
CGnuPlotRenderer::
windowToPixel(double wx, double wy, double *px, double *py)
{
  double wx1, wy1;

  matrix_.multiplyPoint(wx, wy, &wx1, &wy1);

  //---

  if (! mapping() || ! region_.isSet()) {
    *px = wx1;
    *py = wy1;
    return;
  }

  // place on screen
  CPoint2D p1, p2;

  regionToPixel(region_.getLL(), p1);
  regionToPixel(region_.getUR(), p2);

  double pxmin = p1.x, pymin = p2.y;
  double pxmax = p2.x, pymax = p1.y;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double lmargin = margin_.left  ().xValue(this, margin_);
  double rmargin = margin_.right ().xValue(this, margin_);
  double tmargin = margin_.top   ().yValue(this, margin_);
  double bmargin = margin_.bottom().yValue(this, margin_);

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  if (reverseX_)
    *px = CGnuPlotUtil::map(wx1, range1_.getXMin(), range1_.getXMax(), pxmax, pxmin);
  else
    *px = CGnuPlotUtil::map(wx1, range1_.getXMin(), range1_.getXMax(), pxmin, pxmax);

  if (reverseY_)
    *py = CGnuPlotUtil::map(wy1, range1_.getYMin(), range1_.getYMax(), pymin, pymax);
  else
    *py = CGnuPlotUtil::map(wy1, range1_.getYMin(), range1_.getYMax(), pymax, pymin);
}

void
CGnuPlotRenderer::
pixelToWindow(double px, double py, double *wx, double *wy)
{
  pixelToWindowI(px, py, wx, wy, true);
}

void
CGnuPlotRenderer::
pixelToWindowNoMargin(double px, double py, double *wx, double *wy)
{
  pixelToWindowI(px, py, wx, wy, false);
}

void
CGnuPlotRenderer::
pixelToWindowI(double px, double py, double *wx, double *wy, bool margin)
{
  if (! mapping() || ! region_.isSet()) {
    double wx1 = px;
    double wy1 = py;

    imatrix_.multiplyPoint(wx1, wy1, wx, wy);

    return;
  }

  // place on screen
  CPoint2D p1, p2;

  regionToPixel(region_.getLL(), p1);
  regionToPixel(region_.getUR(), p2);

  double pxmin = p1.x, pymin = p2.y;
  double pxmax = p2.x, pymax = p1.y;

  double pw = pxmax - pxmin;
  double ph = pymax - pymin;

  // add margin
  double lmargin, rmargin, tmargin, bmargin;

  if (margin) {
    lmargin = margin_.left  ().xValue(this, margin_);
    rmargin = margin_.right ().xValue(this, margin_);
    tmargin = margin_.top   ().yValue(this, margin_);
    bmargin = margin_.bottom().yValue(this, margin_);
  }
  else {
    lmargin = 0;
    rmargin = 0;
    tmargin = 0;
    bmargin = 0;
  }

  pxmin += lmargin; pxmax -= rmargin;
  pymin += tmargin; pymax -= bmargin;

  // adjust for ratio
  if (ratio_.isValid() && ratio_.getValue() > 0.0) {
    double ratio = ratio_.getValue();

    pw = pxmax - pxmin;
    ph = pymax - pymin;

    double pr = (1.0*pw)/ph;

    double pw1 = pw*ratio/pr;
    double ph1 = ph;

    if (pw1 > pw) {
      ph1 *= pw/pw1;
      pw1  = pw;
    }

    double pxc = (pxmin + pxmax)/2.0;
    double pyc = (pymin + pymax)/2.0;

    pxmin = pxc - pw1/2.0; pxmax = pxc + pw1/2.0;
    pymin = pyc - ph1/2.0; pymax = pyc + ph1/2.0;
  }

  // map
  double wx1, wy1;

  if (reverseX_)
    wx1 = CGnuPlotUtil::map(px, pxmax, pxmin, range1_.getXMin(), range1_.getXMax());
  else
    wx1 = CGnuPlotUtil::map(px, pxmin, pxmax, range1_.getXMin(), range1_.getXMax());

  if (reverseY_)
    wy1 = CGnuPlotUtil::map(py, pymin, pymax, range1_.getYMin(), range1_.getYMax());
  else
    wy1 = CGnuPlotUtil::map(py, pymax, pymin, range1_.getYMin(), range1_.getYMax());

  imatrix_.multiplyPoint(wx1, wy1, wx, wy);
}

void
CGnuPlotRenderer::
regionToPixel(const CPoint2D &r, CPoint2D &p)
{
  p = CPoint2D(r.x*(width() - 1), (1 - r.y)*(height() - 1));
}

void
CGnuPlotRenderer::
windowToRegion(double wx, double wy, double *rx, double *ry)
{
  double wx1, wy1;

  matrix_.multiplyPoint(wx, wy, &wx1, &wy1);

  *rx = CGnuPlotUtil::map(wx1, range1_.getXMin(), range1_.getXMax(), 0, 1);
  *ry = CGnuPlotUtil::map(wy1, range1_.getYMin(), range1_.getYMax(), 0, 1);
}

void
CGnuPlotRenderer::
regionToWindow(double rx, double ry, double *wx, double *wy)
{
  double wx1 = CGnuPlotUtil::map(rx, 0, 1, range1_.getXMin(), range1_.getXMax());
  double wy1 = CGnuPlotUtil::map(ry, 0, 1, range1_.getYMin(), range1_.getYMax());

  imatrix_.multiplyPoint(wx1, wy1, wx, wy);
}

void
CGnuPlotRenderer::
windowToScreen(const CPoint2D &w, CPoint2D &s)
{
  double px1, py1, px2, py2;

  pixelToWindow(          0,            0, &px1, &py2);
  pixelToWindow(width() - 1, height() - 1, &px2, &py1);

  double sx = CGnuPlotUtil::map(w.x, px1, px2, 0, 1);
  double sy = CGnuPlotUtil::map(w.y, py1, py2, 0, 1);

  s = CPoint2D(sx, sy);
}

void
CGnuPlotRenderer::
screenToWindow(const CPoint2D &s, CPoint2D &w)
{
  double px1, py1, px2, py2;

  pixelToWindow(          0,            0, &px1, &py2);
  pixelToWindow(width() - 1, height() - 1, &px2, &py1);

  double wx = CGnuPlotUtil::map(s.x, 0, 1, px1, px2);
  double wy = CGnuPlotUtil::map(s.y, 0, 1, py1, py2);

  w = CPoint2D(wx, wy);
}

void
CGnuPlotRenderer::
windowToGraph(const CPoint2D &w, CPoint2D &g)
{
  const CBBox2D &range = this->range();

  if (range.isSet()) {
    double gx = CGnuPlotUtil::map(w.x, range.getXMin(), range.getXMax(), 0, 1);
    double gy = CGnuPlotUtil::map(w.y, range.getYMin(), range.getYMax(), 0, 1);

    g = CPoint2D(gx, gy);
  }
  else
    g = w;
}

void
CGnuPlotRenderer::
graphToWindow(const CPoint2D &g, CPoint2D &w)
{
  const CBBox2D &range = this->range();

  if (range.isSet()) {
    double wx = CGnuPlotUtil::map(g.x, 0, 1, range.getXMin(), range.getXMax());
    double wy = CGnuPlotUtil::map(g.y, 0, 1, range.getYMin(), range.getYMax());

    w = CPoint2D(wx, wy);
  }
  else
    w = g;
}

void
CGnuPlotRenderer::
windowToChar(const CPoint2D &w, CPoint2D &c)
{
  CPoint2D w1;

  screenToWindow(CPoint2D(0,0), w1);

  CPoint2D p1, p2;

  windowToPixel(w , p1);
  windowToPixel(w1, p2);

  double cx = (p2.x - p1.x)/charPixelWidth ();
  double cy = (p1.y - p2.y)/charPixelHeight();

  c = CPoint2D(cx, cy);
}

void
CGnuPlotRenderer::
charToWindow(const CPoint2D &c, CPoint2D &w)
{
  CPoint2D w1;

  screenToWindow(CPoint2D(0,0), w1);

  CPoint2D p1;

  windowToPixel(w1, p1);

  CFontPtr font = getFont();

  double cx = c.x*charPixelWidth ();
  double cy = c.y*charPixelHeight();

  CPoint2D p2 = p1 + CPoint2D(cx, -cy);

  pixelToWindow(p2, w);
}

void
CGnuPlotRenderer::
windowToSecond(const CPoint2D &w, CPoint2D &s)
{
  double sx = CGnuPlotUtil::map(w.x, range1_.getXMin(), range1_.getXMax(),
                                range2_.getXMin(), range2_.getXMax());
  double sy = CGnuPlotUtil::map(w.y, range1_.getYMin(), range1_.getYMax(),
                                range2_.getYMin(), range2_.getYMax());

  s = CPoint2D(sx, sy);
}

void
CGnuPlotRenderer::
secondToWindow(const CPoint2D &s, CPoint2D &w)
{
  double wx = CGnuPlotUtil::map(s.x, range2_.getXMin(), range2_.getXMax(),
                                range1_.getXMin(), range1_.getXMax());
  double wy = CGnuPlotUtil::map(s.y, range2_.getYMin(), range2_.getYMax(),
                                range1_.getYMin(), range1_.getYMax());

  w = CPoint2D(wx, wy);
}

CPoint3D
CGnuPlotRenderer::
transform(const CPoint3D &p) const
{
  if (camera_)
    return camera_->transform(p);
  else
    return p;
}

CPoint2D
CGnuPlotRenderer::
transform2D(const CPoint3D &p) const
{
  return transform(p).toPoint2D();
}

CPoint2D
CGnuPlotRenderer::
rotatePoint(const CPoint2D &p, double a, const CPoint2D &o)
{
  double a1 = CAngle::Deg2Rad(a);

  CPoint2D po;

  windowToPixel(o, po);

  CPoint2D p1;

  windowToPixel(p, p1);

  CPoint2D pr1 = CMathGeom2D::RotatePoint(p1, -a1, po);

  CPoint2D pr;

  pixelToWindow(pr1, pr);

  return pr;
}

double
CGnuPlotRenderer::
charWidth()
{
  return pixelWidthToWindowWidth(charPixelWidth());
}

double
CGnuPlotRenderer::
charHeight()
{
  return pixelHeightToWindowHeight(charPixelHeight());
}

double
CGnuPlotRenderer::
charPixelWidth()
{
  CFontPtr font = getFont();

  return font->getStringWidth("X");
}

double
CGnuPlotRenderer::
charPixelHeight()
{
  CFontPtr font = getFont();

  return font->getCharHeight();
}

//------

void
CGnuPlotHiddenFillPolygon::
draw(CGnuPlotRenderer *renderer)
{
  renderer->fillPolygon(points_, scaledColor(renderer));
}

void
CGnuPlotHiddenDrawPolygon::
draw(CGnuPlotRenderer *renderer)
{
  renderer->drawPolygon(points_, scaledColor(renderer), w_, d_);
}

void
CGnuPlotHiddenPatternPolygon::
draw(CGnuPlotRenderer *renderer)
{
  renderer->patternPolygon(points_, pattern_, fg_, bg_);
}

void
CGnuPlotHiddenLine::
draw(CGnuPlotRenderer *renderer)
{
  renderer->setMapping(mapping_);

  renderer->drawLine(p1_, p2_, scaledColor(renderer), w_, d_);

  renderer->setMapping(true);
}

void
CGnuPlotHiddenPath::
draw(CGnuPlotRenderer *renderer)
{
  renderer->setMapping(mapping_);

  for (uint i = 0; i < points_.size() - 1; ++i)
    renderer->drawLine(points_[i], points_[i + 1], scaledColor(renderer), w_, d_);

  renderer->setMapping(true);
}

void
CGnuPlotHiddenHAlignedText::
draw(CGnuPlotRenderer *renderer)
{
  renderer->drawHAlignedText(pos_, halignPos_, valignPos_, str_, scaledColor(renderer), a_);
}

void
CGnuPlotHiddenSymbol::
draw(CGnuPlotRenderer *renderer)
{
  renderer->drawSymbol(p_, type_, size_, scaledColor(renderer), lw_, pixelSize_);
}

void
CGnuPlotHiddenTextAtPoint::
draw(CGnuPlotRenderer *renderer)
{
  renderer->setFont(font_);

  renderer->drawTextAtPoint(p_, str_, enhanced_, halignPos_, valignPos_, c_, angle_);
}

void
CGnuPlotHiddenEnhancedText::
draw(CGnuPlotRenderer *renderer)
{
  renderer->drawEnhancedText(bbox_, halignPos_, str_, font_, c_);
}

CRGBA
CGnuPlotHiddenObject::
scaledColor(CGnuPlotRenderer *renderer)
{
  return c_.getLightRGBA(renderer->hiddenZ());
}
