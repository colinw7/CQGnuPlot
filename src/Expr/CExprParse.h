#ifndef CExprParse_H
#define CExprParse_H

#include <CExpr.h>
#include <deque>
#include <iostream>

enum CExprPTokenType {
  CEXPR_PTOKEN_UNKNOWN    = -1,
  CEXPR_PTOKEN_IDENTIFIER = 1,
  CEXPR_PTOKEN_OPERATOR   = 2,
  CEXPR_PTOKEN_INTEGER    = 3,
  CEXPR_PTOKEN_REAL       = 4,
  CEXPR_PTOKEN_STRING     = 5,
  CEXPR_PTOKEN_COMPLEX    = 6,
  CEXPR_PTOKEN_VALUE      = 7
};

class CExprPTokenBase {
 public:
  virtual ~CExprPTokenBase() { }

  virtual void print(std::ostream &os) const = 0;
};

class CExprPTokenIdentifier : public CExprPTokenBase {
 public:
  CExprPTokenIdentifier(const std::string &identifier) :
   identifier_(identifier) {
  }

  const std::string &getIdentifier() const { return identifier_; }

  void print(std::ostream &os) const;

 private:
  std::string identifier_;
};

class CExprPTokenOperator : public CExprPTokenBase {
 public:
  CExprPTokenOperator(CExprOperatorPtr op) :
   op_(op) {
  }

  CExprOperatorPtr getOperator() const { return op_; }

  void print(std::ostream &os) const;

 private:
  CExprOperatorPtr op_;
};

class CExprPTokenInteger : public CExprPTokenBase {
 public:
  CExprPTokenInteger(long integer) :
   integer_(integer) {
  }

  long getInteger() const { return integer_; }

  void print(std::ostream &os) const;

 private:
  long integer_;
};

class CExprPTokenReal : public CExprPTokenBase {
 public:
  CExprPTokenReal(double real) :
   real_(real) {
  }

  double getReal() const { return real_; }

  void print(std::ostream &os) const;

 private:
  double real_;
};

class CExprPTokenString : public CExprPTokenBase {
 public:
  CExprPTokenString(const std::string &str) :
   str_(str) {
  }

  const std::string &getString() const { return str_; }

  void print(std::ostream &os) const;

 private:
  std::string str_;
};

class CExprPTokenComplex : public CExprPTokenBase {
 public:
  CExprPTokenComplex(const std::complex<double> &c) :
   c_(c) {
  }

  const std::complex<double> &getComplex() const { return c_; }

  void print(std::ostream &os) const;

 private:
  std::complex<double> c_;
};

class CExprPToken {
 public:
  static CExprPTokenPtr createIdentifierToken(const std::string &identifier) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_IDENTIFIER;
    ptoken->base_ = new CExprPTokenIdentifier(identifier);

    return ptoken;
  }

  static CExprPTokenPtr createOperatorToken(CExprOperatorPtr op) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_OPERATOR;
    ptoken->base_ = new CExprPTokenOperator(op);

    return ptoken;
  }

  static CExprPTokenPtr createIntegerToken(long integer) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_INTEGER;
    ptoken->base_ = new CExprPTokenInteger(integer);

    return ptoken;
  }

  static CExprPTokenPtr createRealToken(double real) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_REAL;
    ptoken->base_ = new CExprPTokenReal(real);

    return ptoken;
  }

  static CExprPTokenPtr createStringToken(const std::string &str) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_STRING;
    ptoken->base_ = new CExprPTokenString(str);

    return ptoken;
  }

  static CExprPTokenPtr createComplexToken(const std::complex<double> &c) {
    CExprPTokenPtr ptoken(new CExprPToken);

    ptoken->type_ = CEXPR_PTOKEN_COMPLEX;
    ptoken->base_ = new CExprPTokenComplex(c);

    return ptoken;
  }

  static CExprPTokenPtr createUnknownToken() {
    return CExprPTokenPtr(new CExprPToken);
  }

  CExprPToken() :
   type_(CEXPR_PTOKEN_UNKNOWN), base_() {
  }

 ~CExprPToken() { }

  CExprPTokenType getType() const { return type_; }

  const std::string &getIdentifier() const {
    assert(type_ == CEXPR_PTOKEN_IDENTIFIER);

    return base_.cast<CExprPTokenIdentifier>()->getIdentifier();
  }

  CExprOperatorPtr getOperator() const {
    assert(type_ == CEXPR_PTOKEN_OPERATOR);

    return base_.cast<CExprPTokenOperator>()->getOperator();
  }

  long getInteger() const {
    assert(type_ == CEXPR_PTOKEN_INTEGER);

    return base_.cast<CExprPTokenInteger>()->getInteger();
  }

  double getReal() const {
    assert(type_ == CEXPR_PTOKEN_REAL);

    return base_.cast<CExprPTokenReal>()->getReal();
  }

  const std::string &getString() const {
    assert(type_ == CEXPR_PTOKEN_STRING);

    return base_.cast<CExprPTokenString>()->getString();
  }

  const std::complex<double> &getComplex() const {
    assert(type_ == CEXPR_PTOKEN_COMPLEX);

    return base_.cast<CExprPTokenComplex>()->getComplex();
  }

  void printQualified(std::ostream &os) const;

  void print(std::ostream &os) const {
    base_->print(os);
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprPToken &token) {
    token.print(os);

    return os;
  }

 private:
  CExprPTokenType           type_;
  CAutoPtr<CExprPTokenBase> base_;
};

class CExprPTokenStack {
 public:
  CExprPTokenStack() { }

 ~CExprPTokenStack() {
    clear();
  }

  uint getNumTokens() const {
    return stack_.size();
  }

  void addToken(CExprPTokenPtr token) {
    stack_.push_back(token);
  }

  CExprPTokenPtr getToken(uint i) const {
    return stack_[i];
  }

  CExprPTokenPtr lastToken() const {
    if (stack_.empty()) return CExprPTokenPtr();

    return stack_[stack_.size() - 1];
  }

  void clear() {
    stack_.clear();
  }

  CExprPTokenPtr pop_front() {
    CExprPTokenPtr ptoken = stack_.front();

    stack_.pop_front();

    return ptoken;
  }

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprPTokenStack &stack) {
    stack.print(os);

    return os;
  }

 private:
  typedef std::deque <CExprPTokenPtr> Stack;

  Stack stack_;
};

class CExprParseImpl;

class CExprParse {
 public:
 ~CExprParse();

  CExprPTokenStack parseFile(const std::string &filename);
  CExprPTokenStack parseFile(FILE *fp);
  CExprPTokenStack parseLine(const std::string &str);

  bool skipExpression(const std::string &str, uint &i);

 private:
  friend class CExpr;

  CExprParse();

 private:
  CAutoPtr<CExprParseImpl> impl_;
};

namespace CExprParseUtil {
  bool stringToNumber(const std::string &str, uint *i, long &integer, double &real, bool &is_int);
}

#endif
