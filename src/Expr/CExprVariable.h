#ifndef CExprVariable_H
#define CExprVariable_H

class CExprVariable {
 public:
  CExprVariable(const std::string &name, const CExprValuePtr &value);
 ~CExprVariable();

  const std::string &name () const { return name_ ; }
  CExprValuePtr      value() const { return value_; }

  CExprValuePtr getValue() const;
  void setValue(const CExprValuePtr &value);

  void setRealValue   (CExpr *expr, double r);
  void setIntegerValue(CExpr *expr, long   i);

  CExprValueType getValueType() const;

  void print(std::ostream &os) const { os << name_; }

 private:
  std::string   name_;
  CExprValuePtr value_;
};

#endif
