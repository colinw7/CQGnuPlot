#ifndef CGnuPlotValueKey_H
#define CGnuPlotValueKey_H

class CGnuPlotValueKey {
 public:
  enum class Type {
    NONE,
    STRING,
    REAL,
    INTEGER
  };

 public:
  CGnuPlotValueKey() { }

  //---

  bool isString() const { return type_ == Type::STRING; }
  const std::string &string() const { assert(isString()); return s_; }
  void setString(const std::string &str) { type_ = Type::STRING; s_ = str; }

  //---

  bool isReal() const { return type_ == Type::REAL; }
  double real() const { assert(isReal()); return r_; }
  void setReal(double r) { type_ = Type::REAL; r_ = r; }

  //---

  bool isInteger() const { return type_ == Type::INTEGER; }
  int integer() const { assert(isInteger()); return i_; }
  void setInteger(int i) { type_ = Type::INTEGER; i_ = i; }

  //---

  void reset() { type_ = Type::NONE; }

  //---

  friend bool operator<(const CGnuPlotValueKey &key1, const CGnuPlotValueKey &key2) {
    if (key1.type_ != key2.type_)
      return (key1.type_ < key2.type_);

    if      (key1.type_ == Type::STRING)
      return (key1.s_ < key2.s_);
    else if (key1.type_ == Type::REAL)
      return (key1.r_ < key2.r_);
    else if (key1.type_ == Type::INTEGER)
      return (key1.i_ < key2.i_);
    else {
      assert(false);
      return false;
    }
  }

  friend bool operator==(const CGnuPlotValueKey &key1, const CGnuPlotValueKey &key2) {
    if (key1.type_ != key2.type_)
      return false;

    if      (key1.type_ == Type::STRING)
      return (key1.s_ == key2.s_);
    else if (key1.type_ == Type::REAL)
      return (key1.r_ == key2.r_);
    else if (key1.type_ == Type::INTEGER)
      return (key1.i_ == key2.i_);
    else {
      assert(false);
      return false;
    }
  }

 private:
  Type        type_ { Type::NONE };
  std::string s_    { "" };
  double      r_    { 0 };
  int         i_    { 0 };
};

#endif
