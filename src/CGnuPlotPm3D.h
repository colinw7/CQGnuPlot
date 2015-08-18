#ifndef CGnuPlotPm3D_H
#define CGnuPlotPm3D_H

#include <CGnuPlotPm3DData.h>
#include <memory>

class CGnuPlotGroup;

class CGnuPlotPm3D {
 public:
  CGnuPlotPm3D(CGnuPlotGroup *group) :
   group_(group) {
  }

  void setData(const CGnuPlotPm3DData &data) {
    data_= data;
  }

  bool isEnabled() const { return data_.isEnabled(); }
  void setEnabled(bool b) { return data_.setEnabled(b); }

  bool ftriangles() const { return data_.ftriangles(); }
  void setFTriangles(bool b) { data_.setFTriangles(b); }

  bool isClipIn() const { return data_.isClipIn(); }
  void setClipIn(bool b) { data_.setClipIn(b); }

 private:
  CGnuPlotGroup    *group_;
  CGnuPlotPm3DData  data_;
};

typedef std::shared_ptr<CGnuPlotPm3D> CGnuPlotPm3DP;

#endif
