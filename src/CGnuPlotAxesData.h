#ifndef CGnuPlotAxesData_H
#define CGnuPlotAxesData_H

#include <CGnuPlotGridData.h>
#include <CGnuPlotBorderData.h>

class CGnuPlotAxesData {
 public:
  typedef CGnuPlotTypes::DrawLayer       DrawLayer;
  typedef std::map<int,CGnuPlotAxisData> IAxisMap;

 public:
  CGnuPlotAxesData() { }

  //---

  const DrawLayer &getGridLayer() const { return grid_.layer; }
  void setGridLayer(const DrawLayer &l) { grid_.layer = l; }

  bool isGridEnabled() const { return grid_.enabled; }
  void setGridEnabled(bool b) { grid_.enabled = b; }

  double gridPolarAngle() const { return grid_.polarAngle; }
  void setGridPolarAngle(double r) { grid_.polarAngle = r; }

  int gridMajorLineStyle() const { return grid_.majorLineStyle; }
  void setGridMajorLineStyle(int i) { grid_.majorLineStyle = i; }

  int gridMinorLineStyle() const { return grid_.minorLineStyle; }
  void setGridMinorLineStyle(int i) { grid_.minorLineStyle = i; }

  int gridMajorLineType() const { return grid_.majorLineType; }
  void setGridMajorLineType(int i) { grid_.majorLineType = i; }

  int gridMinorLineType() const { return grid_.minorLineType; }
  void setGridMinorLineType(int i) { grid_.minorLineType = i; }

  double gridMajorLineWidth() const { return grid_.majorLineWidth; }
  void setGridMajorLineWidth(double r) { grid_.majorLineWidth = r; }

  double gridMinorLineWidth() const { return grid_.minorLineWidth; }
  void setGridMinorLineWidth(double r) { grid_.minorLineWidth = r; }

  //---

  CGnuPlotAxisData &xaxis(int ind) {
    IAxisMap::iterator p = xaxis_.find(ind);

    if (p == xaxis_.end())
      p = xaxis_.insert(p, IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &yaxis(int ind) {
    IAxisMap::iterator p = yaxis_.find(ind);

    if (p == yaxis_.end())
      p = yaxis_.insert(p, IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &zaxis(int ind) {
    IAxisMap::iterator p = zaxis_.find(ind);

    if (p == zaxis_.end())
      p = zaxis_.insert(p, IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &paxis(int ind) { return paxis_[ind]; }
  CGnuPlotAxisData &taxis(int ind) { return taxis_[ind]; }

  CGnuPlotAxisData &raxis() { return raxis_; }
  CGnuPlotAxisData &uaxis() { return uaxis_; }
  CGnuPlotAxisData &vaxis() { return vaxis_; }

  //---

  int getBorderSides() const { return border_.sides; }
  void setBorderSides(int b) { border_.sides = b; }
  void resetBorderSides() { border_.sides = 0xFF; }

  const DrawLayer &getBorderLayer() const { return border_.layer; }
  void setBorderLayer(const DrawLayer &l) { border_.layer = l; }

  int getBorderWidth() const { return border_.lineWidth; }
  void setBorderWidth(double w) { border_.lineWidth = w; }

  int getBorderStyle() const { return border_.lineStyle.getValue(-1); }
  void setBorderStyle(int ls) { border_.lineStyle = ls; }

  int getBorderType() const { return border_.lineType.getValue(-1); }
  void setBorderType(int lt) { border_.lineType = lt; }

  void unsetBorder() { border_.unset(); }

  void showBorder(std::ostream &os) { border_.show(os); }

  void saveBorder(std::ostream &os) { border_.save(os); }

 private:
  CGnuPlotGridData   grid_;
  IAxisMap           xaxis_;
  IAxisMap           yaxis_;
  IAxisMap           zaxis_;
  IAxisMap           paxis_;
  IAxisMap           taxis_;
  CGnuPlotAxisData   raxis_;
  CGnuPlotAxisData   uaxis_;
  CGnuPlotAxisData   vaxis_;
  CGnuPlotBorderData border_;
};

#endif
