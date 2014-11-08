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

  bool getBooleanValue(bool        &b) const { b = CStrUtil::toBool   (str_); return true; }
  bool getIntegerValue(long        &l) const { l = CStrUtil::toInteger(str_); return true; }
  bool getRealValue   (double      &r) const { r = CStrUtil::toReal   (str_); return true; }
  bool getStringValue (std::string &s) const { s = str_; return true; }

  void setStringValue(const std::string &s) { str_ = s; }

  CExprValuePtr execUnaryOp(CExprOpType op) const;
  CExprValuePtr execBinaryOp(CExprValuePtr rhs, CExprOpType op) const;

  void print(std::ostream &os) const {
    os << str_;
  }

 private:
  std::string str_;
};

#endif
