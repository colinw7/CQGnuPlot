#ifndef CExprInterp_H
#define CExprInterp_H

enum CExprITokenType {
  CEXPR_ITOKEN_NONE = 0,

  CEXPR_ITOKEN_IDENTIFIER = 1,
  CEXPR_ITOKEN_OPERATOR   = 2,
  CEXPR_ITOKEN_INTEGER    = 4,
  CEXPR_ITOKEN_REAL       = 5,
  CEXPR_ITOKEN_STRING     = 6,

  CEXPR_EXPRESSION                = 100,
  CEXPR_ASSIGNMENT_EXPRESSION     = 101,
  CEXPR_CONDITIONAL_EXPRESSION    = 102,
  CEXPR_LOGICAL_OR_EXPRESSION     = 103,
  CEXPR_LOGICAL_AND_EXPRESSION    = 104,
  CEXPR_INCLUSIVE_OR_EXPRESSION   = 105,
  CEXPR_EXCLUSIVE_OR_EXPRESSION   = 106,
  CEXPR_AND_EXPRESSION            = 107,
  CEXPR_EQUALITY_EXPRESSION       = 108,
  CEXPR_RELATIONAL_EXPRESSION     = 109,
  CEXPR_SHIFT_EXPRESSION          = 110,
  CEXPR_ADDITIVE_EXPRESSION       = 111,
  CEXPR_MULTIPLICATIVE_EXPRESSION = 112,
  CEXPR_POWER_EXPRESSION          = 113,
  CEXPR_UNARY_EXPRESSION          = 114,
  CEXPR_POSTFIX_EXPRESSION        = 115,
  CEXPR_PRIMARY_EXPRESSION        = 116,
  CEXPR_ARGUMENT_EXPRESSION_LIST  = 117
};

class CExprITokenBase {
 public:
  virtual ~CExprITokenBase() { }

  virtual CExprITokenBase *dup() const = 0;

  virtual void print(std::ostream &os) const = 0;
};

class CExprITokenIdentifier : public CExprITokenBase {
 public:
  CExprITokenIdentifier(const std::string &identifier) :
   identifier_(identifier) {
  }

  const std::string &getIdentifier() const { return identifier_; }

  CExprITokenIdentifier *dup() const {
    return new CExprITokenIdentifier(identifier_);
  }

  void print(std::ostream &os) const {
    os << identifier_;
  }

 private:
  std::string identifier_;
};

class CExprITokenOperator : public CExprITokenBase {
 public:
  CExprITokenOperator(CExprOperatorPtr op) :
   op_(op) {
  }

  CExprOperatorPtr getOperator() const { return op_; }

  CExprITokenOperator *dup() const {
    return new CExprITokenOperator(op_);
  }

  void print(std::ostream &os) const {
    os << *op_;
  }

 private:
  CExprOperatorPtr op_;
};

class CExprITokenInteger : public CExprITokenBase {
 public:
  CExprITokenInteger(long integer) :
   integer_(integer) {
  }

  long getInteger() const { return integer_; }

  CExprITokenInteger *dup() const {
    return new CExprITokenInteger(integer_);
  }

  void print(std::ostream &os) const {
    os << integer_;
  }

 private:
  long integer_;
};

class CExprITokenReal : public CExprITokenBase {
 public:
  CExprITokenReal(double real) :
   real_(real) {
  }

  double getReal() const { return real_; }

  CExprITokenReal *dup() const {
    return new CExprITokenReal(real_);
  }

  void print(std::ostream &os) const {
    os << real_;
  }

 private:
  double real_;
};

class CExprITokenString : public CExprITokenBase {
 public:
  CExprITokenString(const std::string &str) :
   str_(str) {
  }

  const std::string &getString() const { return str_; }

  CExprITokenString *dup() const {
    return new CExprITokenString(str_);
  }

  void print(std::ostream &os) const {
    os << str_;
  }

 private:
  std::string str_;
};

class CExprIToken {
 public:
  static CExprITokenPtr createIToken(CExprITokenType type=CEXPR_ITOKEN_NONE) {
    return CExprITokenPtr(new CExprIToken(type));
  }

  static CExprITokenPtr createIToken(CExprPTokenPtr ptoken) {
    return CExprITokenPtr(new CExprIToken(ptoken));
  }

  CExprIToken(CExprITokenType type=CEXPR_ITOKEN_NONE);
  CExprIToken(CExprPTokenPtr ptoken);

 ~CExprIToken() { }

  CExprITokenType getType() const { return type_; }

  CExprIToken *dup() const;

  const std::string &getIdentifier() const {
    assert(type_ == CEXPR_ITOKEN_IDENTIFIER);

    return base_.cast<CExprITokenIdentifier>()->getIdentifier();
  }

  CExprOperatorPtr getOperator() const {
    assert(type_ == CEXPR_ITOKEN_OPERATOR);

    return base_.cast<CExprITokenOperator>()->getOperator();
  }

  long getInteger() const {
    assert(type_ == CEXPR_ITOKEN_INTEGER);

    return base_.cast<CExprITokenInteger>()->getInteger();
  }

  double getReal() const {
    assert(type_ == CEXPR_ITOKEN_REAL);

    return base_.cast<CExprITokenReal>()->getReal();
  }

  const std::string &getString() const {
    assert(type_ == CEXPR_ITOKEN_STRING);

    return base_.cast<CExprITokenString>()->getString();
  }

  uint getNumChildren() const {
    return children_.size();
  }

  CExprITokenPtr getChild(uint i) {
    return children_[i];
  }

  void addChild(CExprITokenPtr child) {
    children_.push_back(child);
  }

  void removeAllChildren() {
    children_.clear();
  }

  uint countITokenChildrenOfType(CExprITokenType type);

  void print(std::ostream &os, bool children=true) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprIToken &token) {
    token.print(os);

    return os;
  }

 private:
  typedef std::vector<CExprITokenPtr> ITokenList;

  CExprITokenType           type_;
  CAutoPtr<CExprITokenBase> base_;
  ITokenList                children_;
};

class CExprITokenStack {
 public:
  CExprITokenStack() { }

  void addToken(CExprITokenPtr token) {
    stack_.push_back(token);
  }

  CExprITokenPtr pop_back() {
    if (stack_.empty()) return CExprITokenPtr();

    CExprITokenPtr token = stack_.back();

    stack_.pop_back();

    return token;
  }

  uint getNumTokens() const {
    return stack_.size();
  }

  CExprITokenPtr getToken(uint i) const {
    return stack_[i];
  }

  void clear() {
    stack_.clear();
  }

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CExprITokenStack &stack) {
    stack.print(os);

    return os;
  }

 private:
  typedef std::vector<CExprITokenPtr> Stack;

  Stack stack_;
};

class CExprInterpImpl;

class CExprInterp {
 public:
 ~CExprInterp();

  CExprITokenPtr interpPTokenStack(const CExprPTokenStack &stack);

 private:
  friend class CExpr;

  CExprInterp();

 private:
  CAutoPtr<CExprInterpImpl> impl_;
};

#endif
