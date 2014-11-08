#ifndef CExprCompile_H
#define CExprCompile_H

enum CExprCTokenType {
  CEXPR_CTOKEN_NONE       = 0,
  CEXPR_CTOKEN_IDENTIFIER = 1,
  CEXPR_CTOKEN_OPERATOR   = 2,
  CEXPR_CTOKEN_INTEGER    = 4,
  CEXPR_CTOKEN_REAL       = 5,
  CEXPR_CTOKEN_STRING     = 6,
  CEXPR_CTOKEN_FUNCTION   = 7,
  CEXPR_CTOKEN_VALUE      = 8
};

class CExprValue;

class CExprCTokenBase {
 public:
  virtual ~CExprCTokenBase() { }

  virtual CExprCTokenBase *dup() const = 0;

  virtual void print(std::ostream &os) const = 0;
};

class CExprCTokenIdentifier : public CExprCTokenBase {
 public:
  CExprCTokenIdentifier(const std::string &identifier) :
   identifier_(identifier) {
  }

  const std::string &getIdentifier() const { return identifier_; }

  CExprCTokenIdentifier *dup() const {
    return new CExprCTokenIdentifier(identifier_);
  }

  void print(std::ostream &os) const {
    os << identifier_;
  }

 private:
  std::string identifier_;
};

class CExprCTokenOperator : public CExprCTokenBase {
 public:
  CExprCTokenOperator(CExprOperatorPtr op) :
   op_(op) {
  }

  CExprOperatorPtr getOperator() const { return op_; }

  CExprCTokenOperator *dup() const {
    return new CExprCTokenOperator(op_);
  }

  void print(std::ostream &os) const {
    os << *op_;
  }

 private:
  CExprOperatorPtr op_;
};

class CExprCTokenInteger : public CExprCTokenBase {
 public:
  CExprCTokenInteger(long integer) :
   integer_(integer) {
  }

  long getInteger() const { return integer_; }

  CExprCTokenInteger *dup() const {
    return new CExprCTokenInteger(integer_);
  }

  void print(std::ostream &os) const {
    os << integer_;
  }

 private:
  long integer_;
};

class CExprCTokenReal : public CExprCTokenBase {
 public:
  CExprCTokenReal(double real) :
   real_(real) {
  }

  double getReal() const { return real_; }

  CExprCTokenReal *dup() const {
    return new CExprCTokenReal(real_);
  }

  void print(std::ostream &os) const {
    os << real_;
  }

 private:
  double real_;
};

class CExprCTokenString : public CExprCTokenBase {
 public:
  CExprCTokenString(const std::string &str) :
   str_(str) {
  }

  const std::string &getString() const { return str_; }

  CExprCTokenString *dup() const {
    return new CExprCTokenString(str_);
  }

  void print(std::ostream &os) const {
    os << str_;
  }

 private:
  std::string str_;
};

class CExprCTokenFunction : public CExprCTokenBase {
 public:
  CExprCTokenFunction(CExprFunctionPtr function) :
   function_(function) {
  }

  CExprFunctionPtr getFunction() const { return function_; }

  CExprCTokenFunction *dup() const {
    return new CExprCTokenFunction(function_);
  }

  void print(std::ostream &os) const {
    os << *function_;
  }

 private:
  CExprFunctionPtr function_;
};

class CExprCTokenValue : public CExprCTokenBase {
 public:
  CExprCTokenValue(CExprValuePtr value) :
   value_(value) {
  }

  CExprValuePtr getValue() const { return value_; }

  CExprCTokenValue *dup() const {
    return new CExprCTokenValue(value_);
  }

  void print(std::ostream &os) const {
    os << *value_;
  }

 private:
  CExprValuePtr value_;
};

class CExprCToken {
 public:
  static CExprCTokenPtr createIdentifierToken(const std::string &identifier) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_IDENTIFIER;
    ctoken->base_ = new CExprCTokenIdentifier(identifier);

    return ctoken;
  }

  static CExprCTokenPtr createOperatorToken(CExprOperatorPtr op) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_OPERATOR;
    ctoken->base_ = new CExprCTokenOperator(op);

    return ctoken;
  }

  static CExprCTokenPtr createIntegerToken(long integer) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_INTEGER;
    ctoken->base_ = new CExprCTokenInteger(integer);

    return ctoken;
  }

  static CExprCTokenPtr createRealToken(double real) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_REAL;
    ctoken->base_ = new CExprCTokenReal(real);

    return ctoken;
  }

  static CExprCTokenPtr createStringToken(const std::string &str) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_STRING;
    ctoken->base_ = new CExprCTokenString(str);

    return ctoken;
  }

  static CExprCTokenPtr createFunctionToken(CExprFunctionPtr function) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_FUNCTION;
    ctoken->base_ = new CExprCTokenFunction(function);

    return ctoken;
  }

  static CExprCTokenPtr createValueToken(CExprValuePtr value) {
    CExprCTokenPtr ctoken(new CExprCToken);

    ctoken->type_ = CEXPR_CTOKEN_VALUE;
    ctoken->base_ = new CExprCTokenValue(value);

    return ctoken;
  }

  CExprCToken() :
   type_(CEXPR_CTOKEN_NONE), base_(0) {
  }

  CExprCTokenType getType() const { return type_; }

  const std::string &getIdentifier() const {
    assert(type_ == CEXPR_CTOKEN_IDENTIFIER);

    return base_.cast<CExprCTokenIdentifier>()->getIdentifier();
  }

  CExprOperatorPtr getOperator() const {
    assert(type_ == CEXPR_CTOKEN_OPERATOR);

    return base_.cast<CExprCTokenOperator>()->getOperator();
  }

  long getInteger() const {
    assert(type_ == CEXPR_CTOKEN_INTEGER);

    return base_.cast<CExprCTokenInteger>()->getInteger();
  }

  double getReal() const {
    assert(type_ == CEXPR_CTOKEN_REAL);

    return base_.cast<CExprCTokenReal>()->getReal();
  }

  const std::string &getString() const {
    assert(type_ == CEXPR_CTOKEN_STRING);

    return base_.cast<CExprCTokenString>()->getString();
  }

  CExprFunctionPtr getFunction() const {
    assert(type_ == CEXPR_CTOKEN_FUNCTION);

    return base_.cast<CExprCTokenFunction>()->getFunction();
  }

  CExprValuePtr getValue() const {
    assert(type_ == CEXPR_CTOKEN_VALUE);

    return base_.cast<CExprCTokenValue>()->getValue();
  }

  void print(std::ostream &os) const {
    switch (type_) {
      case CEXPR_CTOKEN_IDENTIFIER:
        os << "<identifier>";
        break;
      case CEXPR_CTOKEN_OPERATOR:
        os << "<operator>";
        break;
      case CEXPR_CTOKEN_INTEGER:
        os << "<integer>";
        break;
      case CEXPR_CTOKEN_REAL:
        os << "<real>";
        break;
      case CEXPR_CTOKEN_STRING:
        os << "<string>";
        break;
      case CEXPR_CTOKEN_FUNCTION:
        os << "<function>";
        break;
      case CEXPR_CTOKEN_VALUE:
        os << "<value>";
        break;
      default:
        os << "<-?->";
        break;
    }

    base_->print(os);

    os << " ";
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprCToken &token) {
    token.print(os);

    return os;
  }

 private:
  CExprCTokenType           type_;
  CAutoPtr<CExprCTokenBase> base_;
};

class CExprCTokenStack {
 public:
  CExprCTokenStack() { }

 ~CExprCTokenStack() {
    clear();
  }

  void addToken(CExprCTokenPtr ctoken) {
    stack_.push_back(ctoken);
  }

  uint getNumTokens() const {
    return stack_.size();
  }

  CExprCTokenPtr getToken(uint i) const {
    return stack_[i];
  }

  void clear() {
    stack_.clear();
  }

  void print(std::ostream &os) const {
    uint len = stack_.size();

    for (uint i = 0; i < len; ++i) {
      if (i > 0) os << " ";

      stack_[i]->print(os);
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprCTokenStack &stack) {
    stack.print(os);

    return os;
  }

 private:
  typedef std::vector<CExprCTokenPtr> Stack;

  Stack stack_;
};

class CExprCompileImpl;

class CExprCompile {
 public:
 ~CExprCompile();

  CExprCTokenStack compileIToken(CExprITokenPtr itoken);

 private:
  friend class CExpr;

  CExprCompile();

 private:
  CAutoPtr<CExprCompileImpl> impl_;
};

#endif
