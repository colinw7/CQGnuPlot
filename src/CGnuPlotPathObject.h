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
  typedef std::vector<CPoint2D> Points2D;
  typedef std::vector<CPoint3D> Points3D;

 public:
  CGnuPlotPathObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPathObject() { }

  CGnuPlotPathObject(const CGnuPlotPathObject &) = delete;
  CGnuPlotPathObject &operator=(const CGnuPlotPathObject &) = delete;

  const Points2D &points2D() const { return points2D_; }
  void setPoints2D(const Points2D &points);

  const Points3D &points3D() const { return points3D_; }
  void setPoints3D(const Points3D &points);

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotTypes::InsideData &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  Points2D         points2D_;
  Points3D         points3D_;
  bool             clipped_ { false };
  CGnuPlotStrokeP  stroke_;
  mutable CBBox2D  bbox_;
  mutable Points2D ppoints_;
};

#endif
