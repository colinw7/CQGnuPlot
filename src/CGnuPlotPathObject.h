#ifndef CGnuPlotPathObject_H
#define CGnuPlotPathObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotObject.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotPathObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D> Points;

 public:
  CGnuPlotPathObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPathObject() { }

  CGnuPlotPathObject(const CGnuPlotPathObject &) = delete;
  CGnuPlotPathObject &operator=(const CGnuPlotPathObject &) = delete;

  const Points &points() const { return points_; }
  void setPoints(const Points &points);

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  Points          points_;
  bool            clipped_ { false };
  CGnuPlotStrokeP stroke_;
  mutable CBBox2D bbox_;
  mutable Points  ppoints_;
};

#endif
