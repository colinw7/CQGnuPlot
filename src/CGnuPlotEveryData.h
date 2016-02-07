#ifndef CGnuPlotEveryData_H
#define CGnuPlotEveryData_H

#include <string>
#include <limits>

class CGnuPlotEveryData {
 public:
  CGnuPlotEveryData(const std::string &str="");

  bool parse(const std::string &str);

  int pointStart() const { return pointStart_; }
  void setPointStart(int i) { pointStart_ = i; }

  int pointEnd() const { return pointEnd_; }
  void setPointEnd(int i) { pointEnd_ = i; }

  int pointStep() const { return pointStep_; }
  void setPointStep(int i) { pointStep_ = i; }

  int blockStart() const { return blockStart_; }
  void setBlockStart(int i) { blockStart_ = i; }

  int blockEnd() const { return blockEnd_; }
  void setBlockEnd(int i) { blockEnd_ = i; }

  int blockStep() const { return blockStep_; }
  void setBlockStep(int i) { blockStep_ = i; }

  bool validPointIndex(int i) const {
    return (i >= pointStart_ && i <= pointEnd_ && ((i - pointStart_) % pointStep_) == 0);
  }

  bool validBlockIndex(int i) const {
    return (i >= blockStart_ && i <= blockEnd_ && ((i - blockStart_) % blockStep_) == 0);
  }

  void reset() {
    pointStart_ = 0; pointEnd_ = std::numeric_limits<int>::max(); pointStep_ = 1;
    blockStart_ = 0; blockEnd_ = std::numeric_limits<int>::max(); blockStep_ = 1;
  }

 private:
  int pointStart_ { 0 };
  int pointEnd_   { std::numeric_limits<int>::max() };
  int pointStep_  { 1 };
  int blockStart_ { 0 };
  int blockEnd_   { std::numeric_limits<int>::max() };
  int blockStep_  { 1 };
};

#endif
