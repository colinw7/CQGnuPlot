#ifndef CExprExecute_H
#define CExprExecute_H

enum CExprETokenType {
  CEXPR_ETOKEN_NONE       = 0,
  CEXPR_ETOKEN_IDENTIFIER = 1,
  CEXPR_ETOKEN_VALUE      = 2
};

class CExprETokenBase {
 public:
  virtual ~CExprETokenBase() { }

  virtual CExprETokenBase *dup() const = 0;

  virtual void print(std::ostream &os) const = 0;
};

class CExprETokenIdentifier : public CExprETokenBase {
 public:
  CExprETokenIdentifier(const std::string &identifier) :
   identifier_(identifier) {
  }

  const std::string &getIdentifier() const { return identifier_; }

  CExprETokenIdentifier *dup() const {
    return new CExprETokenIdentifier(identifier_);
  }

  void print(std::ostream &os) const {
    os << identifier_;
  }

 private:
  std::string identifier_;
};

class CExprETokenValue : public CExprETokenBase {
 public:
  CExprETokenValue(CExprValuePtr value) :
   value_(value) {
  }

  CExprValuePtr getValue() const { return value_; }

  CExprETokenValue *dup() const {
    return new CExprETokenValue(value_);
  }

  void print(std::ostream &os) const {
    os << *value_;
  }

 private:
  CExprValuePtr value_;
};

class CExprEToken {
 public:
  static CExprETokenPtr createIdentifierToken(const std::string &identifier) {
    CExprETokenPtr etoken(new CExprEToken);

    etoken->type_ = CEXPR_ETOKEN_IDENTIFIER;
    etoken->base_ = new CExprETokenIdentifier(identifier);

    return etoken;
  }

  static CExprETokenPtr createValueToken(CExprValuePtr value) {
    CExprETokenPtr etoken(new CExprEToken);

    etoken->type_ = CEXPR_ETOKEN_VALUE;
    etoken->base_ = new CExprETokenValue(value);

    return etoken;
  }

  CExprEToken() :
   type_(CEXPR_ETOKEN_NONE), base_(0) {
  }

  CExprETokenType getType() const { return type_; }

  const std::string &getIdentifier() const {
    assert(type_ == CEXPR_ETOKEN_IDENTIFIER);

    return base_.cast<CExprETokenIdentifier>()->getIdentifier();
  }

  CExprValuePtr getValue() const {
    assert(type_ == CEXPR_ETOKEN_VALUE);

    return base_.cast<CExprETokenValue>()->getValue();
  }

  void print(std::ostream &os) const {
    switch (type_) {
      case CEXPR_ETOKEN_IDENTIFIER:
        os << "<identifier>";
        break;
      case CEXPR_ETOKEN_VALUE:
        os << "<value>";
        break;
      default:
        os << "<-?->";
        break;
    }

    base_->print(os);

    os << " ";
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprEToken &token) {
    token.print(os);

    return os;
  }

 private:
  CExprETokenType           type_;
  CAutoPtr<CExprETokenBase> base_;
};

class CExprETokenStack {
 public:
  CExprETokenStack() { }

 ~CExprETokenStack() {
    clear();
  }

  void addToken(CExprETokenPtr token) {
    stack_.push_back(token);
  }

  CExprETokenPtr pop_back() {
    if (stack_.empty()) return CExprETokenPtr();

    CExprETokenPtr token = stack_.back();

    stack_.pop_back();

    return token;
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

  friend std::ostream &operator<<(std::ostream &os, const CExprETokenStack &stack) {
    stack.print(os);

    return os;
  }

 private:
  typedef std::vector<CExprETokenPtr> Stack;

  Stack stack_;
};

class CExprExecuteImpl;

class CExprExecute {
 public:
 ~CExprExecute();

  CExprValuePtr executeCTokenStack(const CExprCTokenStack &);

 private:
  friend class CExpr;

  CExprExecute();

 private:
  CAutoPtr<CExprExecuteImpl> impl_;
};

#endif
