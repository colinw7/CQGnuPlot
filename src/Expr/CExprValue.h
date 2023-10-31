#ifndef CExprValue_H
#define CExprValue_H

#include <memory>

class CExprValue {
 public:
  enum class AttributeType {
    NONE    = 0,
    MISSING = (1<<0)
  };

 public:
  CExprValue();

  CExprValue(const CExprBooleanValue &boolean);
  CExprValue(const CExprIntegerValue &integer);
  CExprValue(const CExprRealValue    &real);
  CExprValue(const CExprStringValue  &str);
  CExprValue(const CExprComplexValue &c);

 ~CExprValue();

  CExprValueType getType() const { return type_; }
  bool isType(CExprValueType type) const { return (type_ == type); }

  bool hasType(uint type, CExprValueType subType) const {
    return (type & uint(subType));
  }

  bool isMissing() const { return (attributes_ & uint(AttributeType::MISSING)); }

  void setMissing(bool b) {
    if (b)
      attributes_ |= uint(AttributeType::MISSING);
    else
      attributes_ &= ~uint(AttributeType::MISSING);
  }

  CExprValue *dup() const;

  bool isBooleanValue() const;
  bool isIntegerValue() const;
  bool isRealValue   () const;
  bool isStringValue () const;
  bool isComplexValue() const;

  bool getBooleanValue(bool &b) const;
  bool getIntegerValue(long &l) const;
  bool getRealValue   (double &r) const;
  bool getStringValue (std::string &s) const;
  bool getComplexValue(std::complex<double> &c) const;

  void setBooleanValue(bool b);
  void setIntegerValue(long l);
  void setRealValue   (double r);
  void setStringValue (const std::string &s);
  void setComplexValue(const std::complex<double> &c);

  bool convToType(CExprValueType type);

  bool convToBoolean();
  bool convToInteger();
  bool convToReal   ();
  bool convToString ();
  bool convToComplex();

  CExprValuePtr execUnaryOp (CExpr *expr, CExprOpType op) const;
  CExprValuePtr execBinaryOp(CExpr *expr, CExprValuePtr rhs, CExprOpType op) const;

  CExprValuePtr subscript(CExpr *expr, const CExprValueArray &values) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprValue &value) {
    value.print(os);

    return os;
  }

 private:
  using CExprValueBaseP = std::unique_ptr<CExprValueBase>;

  CExprValueType  type_ { CExprValueType::NONE };
  CExprValueBaseP base_;
  uint            attributes_ { 0 };
};

#endif
