#ifndef CExprToken_H
#define CExprToken_H

#include <deque>
#include <iostream>

class CExprTokenStack;

class CExprTokenBase {
 public:
  CExprTokenBase(CExprTokenType type) :
   type_(type) {
  }

  virtual ~CExprTokenBase() { }

  CExprTokenType type() const { return type_; }

  //virtual CExprTokenBase *dup() const = 0;

  const std::string          &getIdentifier() const;
  CExprOpType                 getOperator  () const;
  long                        getInteger   () const;
  double                      getReal      () const;
  const std::string          &getString    () const;
  const std::complex<double> &getComplex   () const;
  CExprFunctionPtr            getFunction  () const;
  CExprValuePtr               getValue     () const;
  const CExprTokenStack       &getBlock    () const;

  void printQualified(std::ostream &os) const;

  virtual void print(std::ostream &os) const = 0;

 protected:
  CExprTokenType type_;
};

typedef CRefPtr<CExprTokenBase> CExprTokenBaseP;

//---

class CExprTokenUnknown : public CExprTokenBase {
 public:
  CExprTokenUnknown() : CExprTokenBase(CEXPR_TOKEN_UNKNOWN) { }

  //CExprTokenUnknown *dup() const { return new CExprTokenUnknown(); }

  void print(std::ostream &os) const { os << "unknown"; }
};

class CExprTokenIdentifier : public CExprTokenBase {
 public:
  CExprTokenIdentifier(const std::string &identifier) :
   CExprTokenBase(CEXPR_TOKEN_IDENTIFIER), identifier_(identifier) {
  }

  const std::string &getIdentifier() const { return identifier_; }

  //CExprTokenIdentifier *dup() const { return new CExprTokenIdentifier(identifier_); }

  void print(std::ostream &os) const {
     os << identifier_;
  }

 private:
  std::string identifier_;
};

//---

class CExprTokenOperator : public CExprTokenBase {
 public:
  CExprTokenOperator(CExprOpType type) :
   CExprTokenBase(CEXPR_TOKEN_OPERATOR), type_(type) {
  }

  CExprOpType getType() const { return type_; }

  //CExprTokenOperator *dup() const { return new CExprTokenOperator(type_); }

  void print(std::ostream &os) const;

 private:
  CExprOpType type_;
};

//---

class CExprTokenInteger : public CExprTokenBase {
 public:
  CExprTokenInteger(long integer) :
   CExprTokenBase(CEXPR_TOKEN_INTEGER), integer_(integer) {
  }

  long getInteger() const { return integer_; }

  //CExprTokenInteger *dup() const { return new CExprTokenInteger(integer_); }

  void print(std::ostream &os) const {
    os << integer_;
  }

 private:
  long integer_;
};

//---

class CExprTokenReal : public CExprTokenBase {
 public:
  CExprTokenReal(double real) :
   CExprTokenBase(CEXPR_TOKEN_REAL), real_(real) {
  }

  double getReal() const { return real_; }

  //CExprTokenReal *dup() const { return new CExprTokenReal(real_); }

  void print(std::ostream &os) const {
    os << real_;
  }

 private:
  double real_;
};

//---

class CExprTokenString : public CExprTokenBase {
 public:
  CExprTokenString(const std::string &str) :
   CExprTokenBase(CEXPR_TOKEN_STRING), str_(str) {
  }

  const std::string &getString() const { return str_; }

  //CExprTokenString *dup() const { return new CExprTokenString(str_); }

  void print(std::ostream &os) const {
    os << "\"" << str_ << "\"";
  }

 private:
  std::string str_;
};

//---

class CExprTokenComplex : public CExprTokenBase {
 public:
  CExprTokenComplex(const std::complex<double> &c) :
   CExprTokenBase(CEXPR_TOKEN_COMPLEX), c_(c) {
  }

  const std::complex<double> &getComplex() const { return c_; }

  //CExprTokenComplex *dup() const { return new CExprTokenComplex(c_); }

  void print(std::ostream &os) const {
    os << "{" << c_.real() << ", " << c_.imag() << "}";
  }

 private:
  std::complex<double> c_;
};

//---

class CExprTokenFunction : public CExprTokenBase {
 public:
  CExprTokenFunction(CExprFunctionPtr function) :
   CExprTokenBase(CEXPR_TOKEN_FUNCTION), function_(function) {
  }

  CExprFunctionPtr getFunction() const { return function_; }

  //CExprTokenFunction *dup() const { return new CExprTokenFunction(function_); }

  void print(std::ostream &os) const;

 private:
  CExprFunctionPtr function_;
};

//---

class CExprTokenValue : public CExprTokenBase {
 public:
  CExprTokenValue(CExprValuePtr value) :
   CExprTokenBase(CEXPR_TOKEN_VALUE), value_(value) {
  }

  CExprValuePtr getValue() const { return value_; }

  //CExprTokenValue *dup() const { return new CExprTokenValue(value_); }

  void print(std::ostream &os) const {
    os << *value_;
  }

 private:
  CExprValuePtr value_;
};

//---

class CExprTokenStack {
 public:
  CExprTokenStack() { }

 ~CExprTokenStack() {
    clear();
  }

  bool empty() const {
    return stack_.empty();
  }

  uint getNumTokens() const {
    return stack_.size();
  }

  void addToken(const CExprTokenBaseP &token) {
    stack_.push_back(token);
  }

  const CExprTokenBaseP &getToken(uint i) const {
    return stack_[i];
  }

  CExprTokenBaseP lastToken() const {
    if (stack_.empty())
      return CExprTokenBaseP();

    return stack_[stack_.size() - 1];
  }

  void clear() {
    stack_.clear();
  }

  CExprTokenBaseP pop_front() {
    CExprTokenBaseP token = stack_.front();

    stack_.pop_front();

    return token;
  }

  CExprTokenBaseP pop_back() {
    if (stack_.empty()) return CExprTokenBaseP();

    CExprTokenBaseP token = stack_.back();

    stack_.pop_back();

    return token;
  }

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprTokenStack &stack) {
    stack.print(os);

    return os;
  }

 private:
  typedef std::deque<CExprTokenBaseP> Stack;

  Stack stack_;
};

//---

class CExprTokenBlock : public CExprTokenBase {
 public:
  CExprTokenBlock(const CExprTokenStack &stack=CExprTokenStack()) :
   CExprTokenBase(CEXPR_TOKEN_BLOCK), stack_(stack) {
  }

  const CExprTokenStack &stack() const { return stack_; }

  //CExprTokenBlock *dup() const { return new CExprTokenBlock(stack_); }

  void print(std::ostream &os) const {
    stack_.print(os);
  }

 private:
  CExprTokenStack stack_;
};

//---

#define CExprTokenMgrInst CExprTokenMgr::instance()

class CExprTokenMgr {
 public:
  static CExprTokenMgr *instance() {
    static CExprTokenMgr *instance;

    if (! instance)
      instance = new CExprTokenMgr;

    return instance;
  }

  CExprTokenIdentifier *createIdentifierToken(const std::string &identifier) {
    return new CExprTokenIdentifier(identifier);
  }

  CExprTokenOperator *createOperatorToken(CExprOpType id) {
    return new CExprTokenOperator(id);
  }

  CExprTokenInteger *createIntegerToken(long integer) {
    return new CExprTokenInteger(integer);
  }

  CExprTokenReal *createRealToken(double real) {
    return new CExprTokenReal(real);
  }

  CExprTokenString *createStringToken(const std::string &str) {
    return new CExprTokenString(str);
  }

  CExprTokenComplex *createComplexToken(const std::complex<double> &c) {
    return new CExprTokenComplex(c);
  }

  CExprTokenFunction *createFunctionToken(CExprFunctionPtr function) {
    return new CExprTokenFunction(function);
  }

  CExprTokenValue *createValueToken(CExprValuePtr value) {
    return new CExprTokenValue(value);
  }

  CExprTokenUnknown *createUnknownToken() {
    return new CExprTokenUnknown();
  }
};

#endif
