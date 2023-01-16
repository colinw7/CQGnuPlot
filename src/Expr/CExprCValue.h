#ifndef CExprComplexValue_H
#define CExprComplexValue_H

#include <complex>

class CExprComplexValue : public CExprValueBase {
 public:
  CExprComplexValue(const std::complex<double> &c) :
   c_(c) {
  }

  CExprComplexValue *dup() const override {
    return new CExprComplexValue(c_);
  }

  bool getBooleanValue(bool   &b) const override {
    b = (c_.real() != 0 || c_.imag() != 0); return true; }
  bool getIntegerValue(long   &l) const override {
    l = long(c_.real()) ; return true; }
  bool getRealValue   (double &r) const override {
    r = c_.real(); return true; }

  bool getStringValue (std::string &s) const override;

  bool getComplexValue(std::complex<double> &c) const override { c = c_; return true; }

  void setComplexValue(const std::complex<double> &c) override { c_ = c; }

  CExprValuePtr execUnaryOp (CExpr *expr, CExprOpType op) const override;
  CExprValuePtr execBinaryOp(CExpr *expr, CExprValuePtr rhs, CExprOpType op) const override;

  void print(std::ostream &os) const override {
    os << "{" << c_.real() << ", " << c_.imag() << "}";
  }

 private:
  std::complex<double> c_;
};

#endif
