#ifndef CGnuPlotIndexData_H
#define CGnuPlotIndexData_H

#include <string>
#include <limits>

class CGnuPlotIndexData {
 public:
  CGnuPlotIndexData(const std::string &str="");

  bool parse(const std::string &str);

  int start() const { return start_; }
  void setStart(int i) { start_ = i; }

  int end() const { return end_; }
  void setEnd(int i) { end_ = i; }

  int step() const { return step_; }
  void setStep(int i) { step_ = i; }

  bool validIndex(int i) const {
    return (i >= start_ && i <= end_ && ((i - start_) % step_) == 0);
  }

  void reset() {
    start_ = 0; end_ = std::numeric_limits<int>::max(); step_ = 1;
  }

 private:
  int start_ { 0 };
  int end_   { std::numeric_limits<int>::max() };
  int step_  { 1 };
};

#endif
