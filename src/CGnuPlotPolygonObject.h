#ifndef CGnuPlotPolygonObject_H
#define CGnuPlotPolygonObject_H

#include <COptVal.h>
#include <CPoint2D.h>
#include <CRGBA.h>
#include <string>

#include <CGnuPlotObject.h>
#include <CGnuPlotFill.h>
#include <CGnuPlotStroke.h>

class CGnuPlotPlot;

class CGnuPlotPolygonObject : public CGnuPlotPlotObject {
 public:
  typedef std::vector<CPoint2D>         Points;
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotPolygonObject(CGnuPlotPlot *plot);

  virtual ~CGnuPlotPolygonObject() { }

  CGnuPlotPolygonObject(const CGnuPlotPolygonObject &) = delete;
  CGnuPlotPolygonObject &operator=(const CGnuPlotPolygonObject &) = delete;

  const Points &points() const { return points_; }
  void setPoints(const Points &points);

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  bool isAliased() const { return aliased_; }
  void setAliased(bool b) { aliased_ = b; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  const CGnuPlotFillP &fill() const { return fill_; }
  void setFill(const CGnuPlotFillP &f) { fill_ = f; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  bool inside(const CGnuPlotMouseEvent &p) const override;

  const CBBox2D &bbox() const override { return bbox_; }

  CGnuPlotTipData tip() const override;

  void draw(CGnuPlotRenderer *renderer) const override;

 protected:
  Points           points_;
  bool             clipped_ { false };
  bool             aliased_ { true };
  std::string      text_    { "" };
  CGnuPlotFillP    fill_ ;
  CGnuPlotStrokeP  stroke_;
  mutable CBBox2D  bbox_;
  mutable CPoint2D center_;
};

#endif
