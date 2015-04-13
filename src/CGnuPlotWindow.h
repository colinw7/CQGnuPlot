#ifndef CGnuPlotWindow_H
#define CGnuPlotWindow_H

#include <vector>

#include <CGnuPlot.h>

class CGnuPlotWindow {
 public:
  typedef std::vector<CGnuPlotGroup *> Groups;
  typedef CGnuPlot::Pm3DData           Pm3DData;

 public:
  CGnuPlotWindow(CGnuPlot *plot);

  virtual ~CGnuPlotWindow();

  CGnuPlot *app() const { return plot_; }

  void clear();

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  const CISize2D &size() const { return size_; }
  void setSize(const CISize2D &s) { size_ = s; }

  //---

  void addGroup(CGnuPlotGroup *group);

  const Groups &groups() const { return groups_; }
  uint numGroups() const { return groups_.size(); }

  void fitGroups();

  //---

  bool hidden3D() const { return hidden3D_; }
  void setHidden3D(bool b) { hidden3D_ = b; }

  bool surface3D() const { return surface3D_; }
  void setSurface3D(bool b) { surface3D_ = b; }

  bool contour3D() const { return contour3D_; }
  void setContour3D(bool b) { contour3D_ = b; }

  const Pm3DData &pm3D() const { return pm3D_; }
  void setPm3D(const Pm3DData &p) { pm3D_ = p; }

  //---

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  //---

  void reset3D();

  void draw();

 private:
  CGnuPlot* plot_      { 0 };
  CISize2D  size_      { 100, 100 };
  bool      is3D_      { false };
  Groups    groups_;
  bool      hidden3D_  { false };
  bool      surface3D_ { true  };
  bool      contour3D_ { false };
  Pm3DData  pm3D_;
  CRGBA     backgroundColor_ { 1, 1, 1};
};

#endif
