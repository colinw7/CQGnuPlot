#ifndef CGnuPlotWindow_H
#define CGnuPlotWindow_H

#include <vector>

#include <CGnuPlot.h>
#include <CGnuPlotGroup.h>

class CGnuPlotWindow {
 public:
  typedef std::vector<CGnuPlotGroupP> Groups;

 public:
  CGnuPlotWindow(CGnuPlot *plot=0);

  virtual ~CGnuPlotWindow();

  CGnuPlot *app() const { return plot_; }

  void setApp(CGnuPlot *app);

  void clear();

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  const CISize2D &size() const { return size_; }
  void setSize(const CISize2D &s) { size_ = s; }

  //---

  CGnuPlotGroupP createNewGroup();

  void addGroup(CGnuPlotGroupP &group);

  const Groups &groups() const { return groups_; }
  uint numGroups() const { return groups_.size(); }

  void fitGroups();

  //---

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  //---

  CRGBA getLightRGBA(const CRGBA &c) const;

  //---

  void reset3D();

  void draw();

 private:
  CGnuPlot* plot_     { 0 };
  CISize2D  size_     { 100, 100 };
  bool      is3D_     { false };
  Groups    groups_;
  CRGBA     backgroundColor_ { 1, 1, 1};
};

#endif
