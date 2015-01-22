#ifndef CBoxWhisker_H
#define CBoxWhisker_H

class CBoxWhisker {
 public:
  typedef std::vector<double> Values;
  typedef std::vector<int>    Outliers;

 public:
  CBoxWhisker(const Values &values) :
   values_(values) {
    calc();
  }

  int numValues() const { return int(values_.size()); }

  double value(int i) const { return values_[i]; }

  void addValue(double value) {
    values_.push_back(value);

    calc();
  }

  void addValues(std::initializer_list<double> values) {
    for (auto value : values)
      values_.push_back(value);

    calc();
  }

  double median() const { return median_; }

  double min() const { return min_; }
  double max() const { return max_; }

  double lower() const { return lower_; }
  double upper() const { return upper_; }

  const Outliers &outliers() const { return outliers_; }

 private:
  void calc() {
    if (values_.empty()) return;

    std::sort(values_.begin(), values_.end());

    int nv = values_.size();

    if (nv > 0) {
      // median
      int nv1, nv2;

      medianInd(0, nv - 1, nv1, nv2);

      median_ = (values_[nv1] + values_[nv2])/2.0;

      // lower median
      int nl1, nl2;

      medianInd(0, nv1 - 1, nl1, nl2);

      lower_ = (values_[nl1] + values_[nl2])/2.0;

      // upper median
      int nu1, nu2;

      medianInd(nv2 + 1, nv - 1, nu1, nu2);

      upper_ = (values_[nu1] + values_[nu2])/2.0;

      // outliers outside 1.5*(upper - lower)
      double routlier = upper_ - lower_;
      double loutlier = lower_ - 1.5*routlier;
      double uoutlier = upper_ + 1.5*routlier;

      //---

      min_ = lower_;
      max_ = min_;

      outliers_.clear();

      int i = 0;

      for (auto v : values_) {
        if (v < loutlier || v > uoutlier)
          outliers_.push_back(i);
        else {
          min_ = std::min(v, min_);
          max_ = std::max(v, max_);
        }

        ++i;
      }
    }
    else {
      median_ = 0.0;
      min_    = 0.0;
      max_    = 0.0;
      lower_  = 0.0;
      upper_  = 0.0;
    }
  }

  void medianInd(int i1, int i2, int &n1, int &n2) {
    int n = i2 - i1 + 1;

    if (n & 1) {
      n1 = i1 + n/2;
      n2 = n1;
    }
    else {
      n2 = i1 + n/2;
      n1 = n2 - 1;
    }
  }

 private:
  Values   values_;
  double   median_;
  double   min_, max_;
  double   lower_, upper_;
  Outliers outliers_;
};

#endif
