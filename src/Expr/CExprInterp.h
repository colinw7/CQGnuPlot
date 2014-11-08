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

  CExprIToken(CExprITokenType type=CEXPR_ITOKEN_NONE) :
   type_(type), base_(0) {
  }

  CExprIToken(CExprPTokenPtr ptoken) :
   type_(CEXPR_ITOKEN_NONE), base_(0) {
    switch (ptoken->getType()) {
      case CEXPR_PTOKEN_IDENTIFIER:
        type_ = CEXPR_ITOKEN_IDENTIFIER;
        base_ = new CExprITokenIdentifier(ptoken->getIdentifier());

        break;
      case CEXPR_PTOKEN_OPERATOR:
        type_ = CEXPR_ITOKEN_OPERATOR;
        base_ = new CExprITokenOperator(ptoken->getOperator());

        break;
      case CEXPR_PTOKEN_INTEGER:
        type_ = CEXPR_ITOKEN_INTEGER;
        base_ = new CExprITokenInteger(ptoken->getInteger());

        break;
      case CEXPR_PTOKEN_REAL:
        type_ = CEXPR_ITOKEN_REAL;
        base_ = new CExprITokenReal(ptoken->getReal());

        break;
      case CEXPR_PTOKEN_STRING:
        type_ = CEXPR_ITOKEN_STRING;
        base_ = new CExprITokenString(ptoken->getString());

        break;
      default:
        break;
    }
  }

 ~CExprIToken() { }

  CExprITokenType getType() const { return type_; }

  CExprIToken *dup() const {
    CExprIToken *itoken = new CExprIToken;

    itoken->type_ = type_;
    itoken->base_ = (base_ ? base_->dup() : 0);

    uint num_children = children_.size();

    for (uint i = 0; i < num_children; ++i)
      itoken->children_.push_back(children_[i]);

    return itoken;
  }

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

  uint countITokenChildrenOfType(CExprITokenType type) {
    uint num_children1 = 0;

    uint num_children = getNumChildren();

    for (uint i = 0; i < num_children; i++) {
      if      (getChild(i)->getType() == type)
        ++num_children1;
      else if (getChild(i)->getNumChildren() > 0) {
        num_children1 += getChild(i)->countITokenChildrenOfType( type);
      }
    }

    return num_children1;
  }

  static const char *getTypeName(CExprITokenType type) {
    switch (type) {
      case CEXPR_EXPRESSION               : return "expression";
      case CEXPR_ASSIGNMENT_EXPRESSION    : return "assignment_expression";
      case CEXPR_CONDITIONAL_EXPRESSION   : return "conditional_expression";
      case CEXPR_LOGICAL_OR_EXPRESSION    : return "logical_or_expression";
      case CEXPR_LOGICAL_AND_EXPRESSION   : return "logical_and_expression";
      case CEXPR_INCLUSIVE_OR_EXPRESSION  : return "inclusive_or_expression";
      case CEXPR_EXCLUSIVE_OR_EXPRESSION  : return "exclusive_or_expression";
      case CEXPR_AND_EXPRESSION           : return "and_expression";
      case CEXPR_EQUALITY_EXPRESSION      : return "equality_expression";
      case CEXPR_RELATIONAL_EXPRESSION    : return "relational_expression";
      case CEXPR_SHIFT_EXPRESSION         : return "shift_expression";
      case CEXPR_ADDITIVE_EXPRESSION      : return "additive_expression";
      case CEXPR_MULTIPLICATIVE_EXPRESSION: return "multiplicative_expression";
      case CEXPR_POWER_EXPRESSION         : return "power_expression";
      case CEXPR_UNARY_EXPRESSION         : return "unary_expression";
      case CEXPR_POSTFIX_EXPRESSION       : return "postfix_expression";
      case CEXPR_PRIMARY_EXPRESSION       : return "primary_expression";
      case CEXPR_ARGUMENT_EXPRESSION_LIST : return "argument_expression_list";
      case CEXPR_ITOKEN_IDENTIFIER        : return "identifier";
      case CEXPR_ITOKEN_OPERATOR          : return "operator";
      case CEXPR_ITOKEN_INTEGER           : return "integer";
      case CEXPR_ITOKEN_REAL              : return "real";
      case CEXPR_ITOKEN_STRING            : return "string";
      default                             : return "<-?->";
    }
  }

  void print(std::ostream &os, bool children=true) const {
    if (base_)
      base_->print(os);
    else
      os << "<" << getTypeName(type_) << ">";

    if (children) {
      uint num_children = children_.size();

      if (num_children > 0) {
        os << " [";

        for (uint i = 0; i < num_children; i++) {
          if (i > 0) os << " ";

          children_[i]->print(os);
        }

        os << "]";
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprIToken &token) {
    token.print(os);

    return os;
  }

 private:
  typedef std::vector<CExprITokenPtr> ITokenList;

  CExprITokenType  type_;
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

  void print(std::ostream &os) const {
    uint len = stack_.size();

    for (uint i = 0; i < len; ++i) {
      if (i > 0) os << " ";

      stack_[i]->print(os);
    }
  }

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
