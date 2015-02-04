#ifndef CGnuPlotPrefValue_H
#define CGnuPlotPrefValue_H

template<typename T>
class CGnuPlotPrefValue : public CGnuPlot::PrefValueBase {
 public:
  CGnuPlotPrefValue(const std::string &name, const T &init) :
   name_(name), value_(init), init_(init) {
  }

  const T &get() const { return value_; }

  void set(const T &value) { value_ = value; }

  void set(const std::string &str) {
    T value;

    if (CStrUniqueMatch::stringToValueWithErr(str, value, name_))
      set(value);
  }

  void reset() {
    value_ = init_;
  }

  void print(std::ostream &os) const {
    os << name_ << " is " << CStrUniqueMatch::valueToString(value_) << std::endl;
  }

 private:
  std::string name_;
  T           value_;
  T           init_;
};

#endif
