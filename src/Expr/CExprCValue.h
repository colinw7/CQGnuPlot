#ifndef CExprComplexValue_H
#define CExprComplexValue_H

#include <complex>

class CExprComplexValue : public CExprValueBase {
 public:
  CExprComplexValue(const std::complex<double> &c) :
   c_(c) {
  }

  CExprComplexValue *dup() const {
    return new CExprComplexValue(c_);
  }

  bool getBooleanValue(bool   &b) const { b = (c_.real() != 0 || c_.imag() != 0); return true; }
  bool getIntegerValue(long   &l) const { l = long(c_.real()) ; return true; }
  bool getRealValue   (double &r) const { r = c_.real(); return true; }

  bool getStringValue(std::string &s) const {
    s = "{" + CStrUtil::toString(c_.real()) + ", " + CStrUtil::toString(c_.imag()) + "}";
    return true;
  }

  bool getComplexValue(std::complex<double> &c) const { c = c_; return true; }

  void setComplexValue(const std::complex<double> &c) { c_ = c; }

  CExprValuePtr execUnaryOp (CExprOpType op) const;
  CExprValuePtr execBinaryOp(CExprValuePtr rhs, CExprOpType op) const;

  void print(std::ostream &os) const {
    os << "{" << c_.real() << ", " << c_.imag() << "}";
  }

 private:
  std::complex<double> c_;
};

#endif
