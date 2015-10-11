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
setPoints(const Points &points)
{
  points_ = points;

  bbox_ = CBBox2D();

  for (const auto &p : points_)
    bbox_ += p;
}

bool
CGnuPlotPathObject::
inside(const CGnuPlotTypes::InsideData &data) const
{
  if (! bbox_.inside(data.window))
    return false;

  CLine2D line;

  for (const auto &p : CIVector(ppoints_)) {
    line.setStart(line.end());
    line.setEnd  (p.second  );

    if (p.first) {
      double d;

      CMathGeom2D::PointLineDistance(data.pixel, line, &d);

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

  if (isClipped()) {
    renderer->strokeClippedPath(points_, *stroke);
  }
  else {
    renderer->strokePath(points_, *stroke);
  }

  ppoints_.resize(points_.size());

  for (const auto &p : CIVector(points_)) {
    double px, py;

    renderer->windowToPixel(p.second.x, p.second.y, &px, &py);

    ppoints_[p.first] = CPoint2D(px, py);
  }
}

CGnuPlotTipData
CGnuPlotPathObject::
tip() const
{
  std::string tipText = CStrUtil::strprintf("%d Points", points_.size());

  CGnuPlotTipData tip;

  tip.setXStr(tipText);
  tip.setYStr("");

  tip.setBorderColor(stroke_->color());
  tip.setXColor     (stroke_->color());

  tip.setRect(bbox_);

  return tip;
}
