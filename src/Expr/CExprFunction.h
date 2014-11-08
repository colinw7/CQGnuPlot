#ifndef CExprFunction_H
#define CExprFunction_H

class CExprFunctionMgr {
 public:
 ~CExprFunctionMgr() { }

  CExprFunctionPtr lookupFunction(const std::string &name);

  CExprFunctionPtr addFunction(const std::string &name, const std::string &args,
                               CExprFunctionProc proc);

  void removeFunction(CExprFunctionPtr function) {
    functions_.remove(function);
  }

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
  typedef std::vector<CExprFunctionArg> Args;
  typedef std::vector<CExprValuePtr>    Values;

 public:
  CExprFunction(const std::string &name, const Args &args, CExprFunctionProc proc);
  CExprFunction(const std::string &name, const std::string &argsStr, CExprFunctionProc proc);

  const std::string &getName() const { return name_; }

  uint getNumArgs() const { return args_.size(); }

  CExprValueType getArgType(uint i) const { return args_[i].type; }

  CExprValuePtr exec(const Values &values) {
    return (*proc_)(values);
  }

  friend std::ostream &operator<<(std::ostream &os, const CExprFunction &fn) {
    os << fn.name_ << "(" << ")";

    return os;
  }

 private:
  void parseArgs(const std::string &argsStr, Args &args);

 private:
  std::string       name_;
  Args              args_;
  CExprFunctionProc proc_;
};

#endif
