#ifndef CExprValue_H
#define CExprValue_H

class CExprValue {
 public:
  CExprValue();

  CExprValue(const CExprBooleanValue &boolean);
  CExprValue(const CExprIntegerValue &integer);
  CExprValue(const CExprRealValue    &real);
  CExprValue(const CExprStringValue  &str);
  CExprValue(const CExprComplexValue &c);

 ~CExprValue();

  CExprValueType getType() const { return type_; }

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

  CExprValuePtr execUnaryOp(CExprOpType op) const;
  CExprValuePtr execBinaryOp(CExprValuePtr rhs, CExprOpType op) const;
  CExprValuePtr subscript(const std::vector<CExprValuePtr> &values) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprValue &value) {
    value.print(os);

    return os;
  }

 private:
  CExprValueType           type_;
  CAutoPtr<CExprValueBase> base_;
};

#endif
