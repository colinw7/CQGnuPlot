#ifndef CExprStringValue_H
#define CExprStringValue_H

class CExprStringValue : public CExprValueBase {
 public:
  CExprStringValue(const std::string &str) :
   str_(str) {
  }

  CExprStringValue *dup() const {
    return new CExprStringValue(str_);
  }

  bool getBooleanValue(bool        &b) const { return CStrUtil::toBool   (str_, &b); }
  bool getIntegerValue(long        &l) const { return CStrUtil::toInteger(str_, &l); }
  bool getRealValue   (double      &r) const { return CStrUtil::toReal   (str_, &r); }
  bool getStringValue (std::string &s) const { s = str_; return true; }

  void setStringValue(const std::string &s) { str_ = s; }

  CExprValuePtr execUnaryOp (CExprOpType op) const;
  CExprValuePtr execBinaryOp(CExprValuePtr rhs, CExprOpType op) const;

  CExprValuePtr subscript(const CExprValueArray &values) const;

  void print(std::ostream &os) const {
    os << str_;
  }

 private:
  std::string str_;
};

#endif
