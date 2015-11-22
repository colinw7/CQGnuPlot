#include <CGnuPlotPathObject.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotRenderer.h>
#include <CMathGeom2D.h>
#include <CIVector.h>

CGnuPlotPathObject::
CGnuPlotPathObject(CGnuPlotPlot *plot) :
 CGnuPlotPlotObject(plot)
{
  stroke_ = plot->createStroke();
}

void
CGnuPlotPathObject::
setPoints2D(const Points2D &points)
{
  points2D_ = points;

  bbox_ = CBBox2D();

  for (const auto &p : points2D_)
    bbox_ += p;
}

void
CGnuPlotPathObject::
setPoints3D(const Points3D &points)
{
  points3D_ = points;

  bbox_ = CBBox2D();

  for (const auto &p : points3D_)
    bbox_ += CPoint2D(p.x, p.y);
}

bool
CGnuPlotPathObject::
inside(const CGnuPlotMouseEvent &mouseEvent) const
{
  if (! bbox_.inside(mouseEvent.window()))
    return false;

  CLine2D line;

  for (const auto &p : CIVector(ppoints_)) {
    line.setStart(line.end());
    line.setEnd  (p.second  );

    if (p.first) {
      double d;

      CMathGeom2D::PointLineDistance(mouseEvent.pixel(), line, &d);

      if (d < 4)
        return true;
    }
  }

  return false;
}

void
CGnuPlotPathObject::
draw(CGnuPlotRenderer *renderer) const
{
  bool highlighted = (isHighlighted() || isSelected());

  CGnuPlotStrokeP stroke = stroke_;

  if (highlighted) {
    stroke = stroke_->dup();

    stroke->setEnabled(true);
    stroke->setColor(CRGBA(1,0,0));
    stroke->setWidth(2);
  }

  if (! points2D_.empty()) {
    if (isClipped()) {
      renderer->strokeClippedPath(points2D_, *stroke);
    }
    else {
      renderer->strokePath(points2D_, *stroke);
    }

    ppoints_.resize(points2D_.size());

    for (const auto &p : CIVector(points2D_)) {
      double px, py;

      renderer->windowToPixel(p.second.x, p.second.y, &px, &py);

      ppoints_[p.first] = CPoint2D(px, py);
    }
  }
  else {
    if (isClipped()) {
      renderer->strokeClippedPath(points3D_, *stroke);
    }
    else {
      renderer->strokePath(points3D_, *stroke);
    }

    ppoints_.resize(points3D_.size());

    for (const auto &p : CIVector(points3D_)) {
      CPoint2D p1 = renderer->transform(p.second);

      double px, py;

      renderer->windowToPixel(p1.x, p1.y, &px, &py);

      ppoints_[p.first] = CPoint2D(px, py);
    }
  }
}

CGnuPlotTipData
CGnuPlotPathObject::
tip() const
{
  std::string tipText;

  if (! points2D_.empty())
    tipText = CStrUtil::strprintf("%d Points", points2D_.size());
  else
    tipText = CStrUtil::strprintf("%d Points", points3D_.size());

  CGnuPlotTipData tip;

  tip.setXStr(tipText);
  tip.setYStr("");

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  tip.setBBox(bbox_);

  return tip;
}
