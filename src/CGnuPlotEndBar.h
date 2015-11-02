#ifndef CGnuPlotEndBar_H
#define CGnuPlotEndBar_H

#include <CPoint2D.h>

#include <CGnuPlotStroke.h>

class CGnuPlotPlot;
class CGnuPlotBoxBarObject;
class CGnuPlotRenderer;

class CGnuPlotEndBar {
 public:
  CGnuPlotEndBar(CGnuPlotPlot *plot);

  virtual ~CGnuPlotEndBar() { }

  CGnuPlotEndBar(const CGnuPlotEndBar &) = delete;
  const CGnuPlotEndBar &operator=(const CGnuPlotEndBar &) = delete;

  void setBarObject(CGnuPlotBoxBarObject *bar) { bar_ = bar; }

  const CPoint2D &start() const { return start_; }
  void setStart(const CPoint2D &v) { start_ = v; }

  const CPoint2D &end() const { return end_; }
  void setEnd(const CPoint2D &v) { end_ = v; }

  bool isStartLine() const { return startLine_; }
  void setStartLine(bool b) { startLine_ = b; }

  bool isEndLine() const { return endLine_; }
  void setEndLine(bool b) { endLine_ = b; }

  double endWidth() const { return endWidth_; }
  void setEndWidth(double r) { endWidth_ = r; }

  const CGnuPlotStrokeP &stroke() const { return stroke_; }
  void setStroke(const CGnuPlotStrokeP &s) { stroke_ = s; }

  virtual void draw(CGnuPlotRenderer *renderer) const;

 protected:
  CGnuPlotPlot*         plot_      { 0 };
  CGnuPlotBoxBarObject* bar_       { 0 };
  CPoint2D              start_;
  CPoint2D              end_;
  bool                  startLine_ { false };
  bool                  endLine_   { false };
  double                endWidth_  { 0 };
  CGnuPlotStrokeP       stroke_;
};

typedef CRefPtr<CGnuPlotEndBar> CGnuPlotEndBarP;

#endif
