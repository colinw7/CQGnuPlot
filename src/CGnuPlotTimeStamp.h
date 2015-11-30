#ifndef CGnuPlotTimeStamp_H
#define CGnuPlotTimeStamp_H

#include <CGnuPlotTimeStampData.h>

class CGnuPlotGroup;
class CGnuPlotRenderer;

class CGnuPlotTimeStamp {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotTimeStamp(CGnuPlotGroup *group);

  virtual ~CGnuPlotTimeStamp();

  CGnuPlotGroup *group() const { return group_; }

  const CGnuPlotTimeStampData &data() const { return data_; }
  void setData(const CGnuPlotTimeStampData &data) { data_ = data; }

  bool isTop() const { return data_.isTop(); }
  void setTop(bool b) { data_.setTop(b); }

  virtual void draw(CGnuPlotRenderer *renderer);

 private:
  CGnuPlotGroup         *group_;
  CGnuPlotTimeStampData  data_;
};

typedef CRefPtr<CGnuPlotTimeStamp> CGnuPlotTimeStampP;

#endif
