#ifndef CGnuPlotAxesData_H
#define CGnuPlotAxesData_H

#include <CGnuPlotAxisData.h>
#include <CGnuPlotGridData.h>
#include <CGnuPlotBorderData.h>

class CGnuPlotAxesData {
 public:
  typedef CGnuPlotTypes::DrawLayer       DrawLayer;
  typedef CGnuPlotTypes::AxisType        AxisType;
  typedef std::map<int,CGnuPlotAxisData> IAxisMap;
  typedef std::map<AxisType,IAxisMap>    TIAxisMap;

 public:
  CGnuPlotAxesData() { }

  //---

  const DrawLayer &getGridLayer() const { return grid_.layer(); }
  void setGridLayer(const DrawLayer &l) { grid_.setLayer(l); }

  double gridPolarAngle() const { return grid_.polarAngle(); }
  void setGridPolarAngle(double r) { grid_.setPolarAngle(r); }

  int gridMajorLineStyle() const { return grid_.majorLineStyle(); }
  void setGridMajorLineStyle(int i) { grid_.setMajorLineStyle(i); }

  int gridMinorLineStyle() const { return grid_.minorLineStyle(); }
  void setGridMinorLineStyle(int i) { grid_.setMinorLineStyle(i); }

  int gridMajorLineType() const { return grid_.majorLineType(); }
  void setGridMajorLineType(int i) { grid_.setMajorLineType(i); }

  int gridMinorLineType() const { return grid_.minorLineType(); }
  void setGridMinorLineType(int i) { grid_.setMinorLineType(i); }

  double gridMajorLineWidth() const { return grid_.majorLineWidth(); }
  void setGridMajorLineWidth(double r) { grid_.setMajorLineWidth(r); }

  double gridMinorLineWidth() const { return grid_.minorLineWidth(); }
  void setGridMinorLineWidth(double r) { grid_.setMinorLineWidth(r); }

  //---

  CGnuPlotAxisData &xaxis(int ind) { return axis(AxisType::X, ind); }
  CGnuPlotAxisData &yaxis(int ind) { return axis(AxisType::Y, ind); }
  CGnuPlotAxisData &zaxis(int ind) { return axis(AxisType::Z, ind); }
  CGnuPlotAxisData &paxis(int ind) { return axis(AxisType::P, ind); }
  CGnuPlotAxisData &taxis(int ind) { return axis(AxisType::T, ind); }
  CGnuPlotAxisData &raxis()        { return axis(AxisType::R, 1); }
  CGnuPlotAxisData &uaxis()        { return axis(AxisType::U, 1); }
  CGnuPlotAxisData &vaxis()        { return axis(AxisType::V, 1); }

  CGnuPlotAxisData &axis(AxisType type, int ind) {
    auto p = taxis_.find(type);

    if (p == taxis_.end())
      p = taxis_.insert(p, TIAxisMap::value_type(type, IAxisMap()));

    IAxisMap &axis = (*p).second;

    auto p1 = axis.find(ind);

    if (p1 == axis.end())
      p1 = axis.insert(p1, IAxisMap::value_type(ind, CGnuPlotAxisData(type, ind)));

    return (*p1).second;
  }

  //---

  int getBorderSides(int defSides=31) const { return border_.sides().getValue(defSides); }
  void setBorderSides(int s) { border_.setSides(s); }
  void resetBorderSides() { border_.resetSides(); }

  const DrawLayer &getBorderLayer() const { return border_.layer(); }
  void setBorderLayer(const DrawLayer &l) { border_.setLayer(l); }

  double getBorderWidth() const { return border_.lineWidth(); }
  void setBorderWidth(double w) { border_.setLineWidth(w); }

  int getBorderStyle() const { return border_.lineStyle().getValue(-1); }
  void setBorderStyle(int ls) { border_.setLineStyle(ls); }

  int getBorderType() const { return border_.lineType().getValue(-1); }
  void setBorderType(int lt) { border_.setLineType(lt); }

  void unsetBorder() { border_.unset(); }

  void showBorder(std::ostream &os) { border_.show(os); }

  void saveBorder(std::ostream &os) { border_.save(os); }

 private:
  CGnuPlotGridData   grid_;
  TIAxisMap          taxis_;
  CGnuPlotBorderData border_;
};

#endif
