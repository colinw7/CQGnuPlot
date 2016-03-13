#ifndef CGnuPlotStyleValueMgr_H
#define CGnuPlotStyleValueMgr_H

#include <CGnuPlotStyleValue.h>
#include <CTypeInfo.h>
#include <map>

#include <CGnuPlotPlot.h>

#define CGnuPlotStyleValueMgrInst CGnuPlotStyleValueMgr::instance()

class CGnuPlotStyleValueMgr {
 public:
  static CGnuPlotStyleValueMgr *instance() {
    static CGnuPlotStyleValueMgr *inst;

    if (! inst)
      inst = new CGnuPlotStyleValueMgr;

    return inst;
  }

  template<typename T>
  void setId(const std::string &id) {
    CTypeInfo info = typeid(T);

    typeId_[info] = id;
  }

  template<typename T>
  const std::string &id() const {
    CTypeInfo info = typeid(T);

    auto p = typeId_.find(info);
    assert(p != typeId_.end());

    return (*p).second;
  }

  template<typename T>
  T *getValue(CGnuPlotPlot *plot) {
    return dynamic_cast<T *>(plot->styleValue(id<T>()));
  }

  template<typename T>
  void setValue(CGnuPlotPlot *plot, T *value) {
    plot->setStyleValue(id<T>(), value);
  }

 private:
  typedef std::map<CTypeInfo,std::string> TypeId;

  TypeId typeId_;
};

#endif
