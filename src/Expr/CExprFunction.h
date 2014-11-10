#ifndef CExprFunction_H
#define CExprFunction_H

class CExprFunctionMgr {
 public:
 ~CExprFunctionMgr() { }

  CExprFunctionPtr lookupFunction(const std::string &name);

  CExprFunctionPtr addProcFunction(const std::string &name, const std::string &args,
                                   CExprFunctionProc proc);

  CExprFunctionPtr addUserFunction(const std::string &name, const std::vector<std::string> &args,
                                   const std::string &proc);

  void removeFunction(CExprFunctionPtr function);

 private:
  friend class CExpr;

  CExprFunctionMgr();

 private:
  typedef std::list<CExprFunctionPtr> FunctionList;

  FunctionList functions_;
};

struct CExprFunctionArg {
  CExprFunctionArg() : type(CEXPR_VALUE_NONE) { }

  CExprValueType type;
};

class CExprFunction {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CExprFunction(const std::string &name) :
   name_(name) {
  }

  virtual ~CExprFunction() { }

  const std::string &getName() const { return name_; }

  virtual uint numArgs() const = 0;

  virtual CExprValueType argType(uint) const { return CEXPR_VALUE_ANY; }

  virtual CExprValuePtr exec(const Values &values) = 0;

  friend std::ostream &operator<<(std::ostream &os, const CExprFunction &fn) {
    fn.print(os);

    return os;
  }

  virtual void print(std::ostream &os) const {
    os << name_ << "(" << ")";
  }

 protected:
  std::string name_;
};

class CExprProcFunction : public CExprFunction {
 public:
  typedef std::vector<CExprFunctionArg> Args;

 public:
  CExprProcFunction(const std::string &name, const Args &args, CExprFunctionProc proc);
  CExprProcFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc);

  uint numArgs() const { return args_.size(); }

  CExprValueType argType(uint i) const { return args_[i].type; }

  CExprValuePtr exec(const Values &values);

 private:
  void parseArgs(const std::string &argsStr, Args &args);

 private:
  Args              args_;
  CExprFunctionProc proc_;
};

class CExprUserFunction : public CExprFunction {
 public:
  typedef std::vector<std::string> Args;

 public:
  CExprUserFunction(const std::string &name, const Args &args, const std::string &proc);

  uint numArgs() const { return args_.size(); }

  CExprValuePtr exec(const Values &values);

  void print(std::ostream &os) const {
    CExprFunction::print(os);

    os << "= " << proc_;
  }

 private:
  Args        args_;
  std::string proc_;
};

#endif
