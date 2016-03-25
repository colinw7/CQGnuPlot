#ifndef CGnuPlotDummyVars_H
#define CGnuPlotDummyVars_H

class CGnuPlotDummyVars {
 public:
  CGnuPlotDummyVars() { }

  void dummyVars(std::string &dummyVar1, std::string &dummyVar2,
                 bool isParametric, bool isPolar, bool is3D) const {
    CGnuPlotDummyVars *th = const_cast<CGnuPlotDummyVars *>(this);

    if      (isParametric) {
      if (! is3D) {
        dummyVar1 = th->varMap_["t"]; if (dummyVar1 == "") dummyVar1 = "t";
        dummyVar2 = th->varMap_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
      }
      else {
        dummyVar1 = th->varMap_["u"]; if (dummyVar1 == "") dummyVar1 = "u";
        dummyVar2 = th->varMap_["v"]; if (dummyVar2 == "") dummyVar2 = "v";
      }
    }
    else if (isPolar) {
      if (! is3D) {
        dummyVar1 = th->varMap_["t"]; if (dummyVar1 == "") dummyVar1 = "t";
        dummyVar2 = th->varMap_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
      }
      else {
        dummyVar1 = th->varMap_["u"]; if (dummyVar1 == "") dummyVar1 = "u";
        dummyVar2 = th->varMap_["v"]; if (dummyVar2 == "") dummyVar2 = "v";
      }
    }
    else {
      dummyVar1 = th->varMap_["x"]; if (dummyVar1 == "") dummyVar1 = "x";
      dummyVar2 = th->varMap_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
    }
  }

  void setDummyVar(const std::string &name, const std::string &value) {
    varMap_[name] = value;
  }

  void setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2,
                    bool isParametric, bool isPolar) {
    if      (isParametric) {
      if (dummyVar1 != "") varMap_["t"] = dummyVar1;
      if (dummyVar2 != "") varMap_["y"] = dummyVar2;
    }
    else if (isPolar) {
      if (dummyVar1 != "") varMap_["t"] = dummyVar1;
      if (dummyVar2 != "") varMap_["y"] = dummyVar2;
    }
    else {
      if (dummyVar1 != "") varMap_["x"] = dummyVar1;
      if (dummyVar2 != "") varMap_["y"] = dummyVar2;
    }
  }

  void resetDummyVars() {
    varMap_.clear();
  }

 private:
  typedef std::map<std::string,std::string> VarMap;

  VarMap varMap_;
};

#endif
