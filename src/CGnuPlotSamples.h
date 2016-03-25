#ifndef CGnuPlotSamples_H
#define CGnuPlotSamples_H

class CGnuPlotSamples {
 public:
  CGnuPlotSamples() { }

  void get(int &s1, int &s2) const { s1 = samples1_; s2 = samples2_; }
  void set(int s1, int s2) { samples1_ = s1; samples2_ = s2; }

  void unset() { set(100, 100); }

  void save(std::ostream &os) const {
    os << "set samples " << samples1_ << ", " << samples2_ << std::endl;
  }

  void show(std::ostream &os) {
    os << "sampling rate is " << samples1_ << ", " << samples2_ << std::endl;
  }

 private:
  int samples1_ { 100 };
  int samples2_ { 100 };
};

#endif
