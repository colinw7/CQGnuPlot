#ifndef CExprFunction_H
#define CExprFunction_H

class CExprFunctionObj {
 public:
  CExprFunctionObj() { }

  virtual ~CExprFunctionObj() { }

  virtual CExprValuePtr operator()(const std::vector<CExprValuePtr> &values) = 0;
};

struct CExprFunctionArg {
  CExprFunctionArg() : type(CEXPR_VALUE_NONE) { }

  CExprValueType type;
};

class CExprFunctionMgr {
 public:
  typedef std::vector<CExprFunctionArg> Args;

 public:
 ~CExprFunctionMgr() { }

  CExprFunctionPtr getFunction(const std::string &name);

  CExprFunctionPtr addProcFunction(const std::string &name, const std::string &args,
                                   CExprFunctionProc proc);
  CExprFunctionPtr addObjFunction(const std::string &name, const std::string &args,
                                  CExprFunctionObj &proc);
  CExprFunctionPtr addUserFunction(const std::string &name, const std::vector<std::string> &args,
                                   const std::string &proc);

  void removeFunction(CExprFunctionPtr function);

  void getFunctionNames(std::vector<std::string> &names) const;

  bool parseArgs(const std::string &argsStr, Args &args);

 private:
  friend class CExpr;

  CExprFunctionMgr();

 private:
  typedef std::list<CExprFunctionPtr> FunctionList;

  FunctionList functions_;
};

class CExprFunction {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CExprFunction(const std::string &name) :
   name_(name), builtin_(false) {
  }

  virtual ~CExprFunction() { }

  const std::string &name() const { return name_; }

  bool isBuiltin() const { return builtin_; }
  void setBuiltin(bool b) { builtin_ = b; }

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
  bool        builtin_;
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
  Args              args_;
  CExprFunctionProc proc_;
};

class CExprObjFunction : public CExprFunction {
 public:
  typedef std::vector<CExprFunctionArg> Args;

 public:
  CExprObjFunction(const std::string &name, const Args &args, CExprFunctionObj &proc);

  uint numArgs() const { return args_.size(); }

  CExprValueType argType(uint i) const { return args_[i].type; }

  CExprValuePtr exec(const Values &values);

 private:
  void parseArgs(const std::string &argsStr, Args &args);

 private:
  Args              args_;
  CExprFunctionObj &proc_;
};

class CExprUserFunction : public CExprFunction {
 public:
  typedef std::vector<std::string> Args;

 public:
  CExprUserFunction(const std::string &name, const Args &args, const std::string &proc);

  uint numArgs() const { return args_.size(); }

  CExprValuePtr exec(const Values &values);

  void print(std::ostream &os) const {
    os << name_ << "(";

    for (uint i = 0; i < args_.size(); ++i) {
      if (i > 0) os << ", ";

      os << args_[i];
    }

    os << ") = " << proc_;
  }

 private:
  Args        args_;
  std::string proc_;
};

#endif
