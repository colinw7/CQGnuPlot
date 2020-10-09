#ifndef CGnuPlotPm3D_H
#define CGnuPlotPm3D_H

#include <CGnuPlotPm3DData.h>
#include <memory>

class CGnuPlotGroup;

class CGnuPlotPm3D {
 public:
  CGnuPlotPm3D(CGnuPlotGroup *group);

  virtual ~CGnuPlotPm3D() { }

  CGnuPlotGroup *group() const { return group_; }

  void setData(const CGnuPlotPm3DData &data);

  bool isEnabled() const { return data_.isEnabled(); }
  void setEnabled(bool b) { return data_.setEnabled(b); }

  bool ftriangles() const { return data_.ftriangles(); }
  void setFTriangles(bool b) { data_.setFTriangles(b); }

  int clipIn() const { return data_.clipIn(); }
  void setClipIn(int i) { data_.setClipIn(i); }

  int lineType() const { return data_.linetype(); }
  void setLineType(int lt) { data_.setLineType(lt); }

  bool isImplicit() { return data_.isImplicit(); }
  void setImplicit(bool b) { data_.setImplicit(b); }

 private:
  CGnuPlotGroup*   group_ { nullptr };
  CGnuPlotPm3DData data_;
};

typedef std::shared_ptr<CGnuPlotPm3D> CGnuPlotPm3DP;

#endif
