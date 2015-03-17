#ifndef CGnuPlotPrefValue_H
#define CGnuPlotPrefValue_H

#include <CStrUniqueMatch.h>

class CGnuPlotPrefValueBase {
 public:
  CGnuPlotPrefValueBase() { }

  virtual ~CGnuPlotPrefValueBase() { }

  virtual void set(const std::string &str) = 0;

  virtual void reset() = 0;

  virtual void show(std::ostream &os) const = 0;

  virtual void print(std::ostream &os) const = 0;
};

template<typename T>
class CGnuPlotPrefValue : public CGnuPlotPrefValueBase {
 public:
  static T fromString(const std::string &str) {
    T value;
    CStrUniqueMatch::stringToValue(str, value);
    return T();
  }

  static std::string toString(const T &value) {
    return CStrUniqueMatch::valueToString(value);
  }

  CGnuPlotPrefValue(const std::string &name, const std::string &pref, const T &init) :
   name_(name), pref_(pref), value_(init), init_(init) {
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

  std::string toString() const {
    return toString(value_);
  }

  void show(std::ostream &os) const {
    os << "set " << pref_ << " " << toString(value_) << std::endl;
  }

  void print(std::ostream &os) const {
    os << name_ << " is " << toString(value_) << std::endl;
  }

 private:
  std::string name_;
  std::string pref_;
  T           value_;
  T           init_;
};

#endif
