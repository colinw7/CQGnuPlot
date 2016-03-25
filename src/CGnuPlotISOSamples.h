#ifndef CGnuPlotISOSamples_H
#define CGnuPlotISOSamples_H

class CGnuPlotISOSamples {
 public:
  CGnuPlotISOSamples() { }

  void get(int &s1, int &s2) const { s1 = samples1_; s2 = samples2_; }
  void set(int s1, int s2) { samples1_ = s1; samples2_ = s2; }

  void unset() { set(10, 10); }

  void save(std::ostream &os) const {
    os << "set isosamples " << samples1_ << ", " << samples2_ << std::endl;
  }

  void show(std::ostream &os) const {
    os << "iso sampling rate is " << samples1_ << ", " << samples2_ << std::endl;
  }

 private:
  int samples1_ { 10 };
  int samples2_ { 10 };
};

#endif
