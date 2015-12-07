#include <CGnuPlotFunctions.h>
#include <CGnuPlot.h>

class CGnuPlotFn : public CExprFunctionObj {
 public:
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotFn(CGnuPlot *plot) : plot_(plot) { }

 protected:
  CGnuPlot *plot_;
};

class CGnuPlotAssertFn : public CGnuPlotFn {
 public:
  CGnuPlotAssertFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    long value = 0;

    if (! values[0]->getIntegerValue(value))
      assert(false);

    assert(value);

    return CExprValuePtr();
  }
};

class CGnuPlotColumnFn : public CGnuPlotFn {
 public:
  CGnuPlotColumnFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    long icol = 0;

    if      (values[0]->isIntegerValue()) {
      (void) values[0]->getIntegerValue(icol);
    }
    else if (values[0]->isStringValue()) {
      std::string str;

      (void) values[0]->getStringValue(str);

      icol = plot_->getColumnIndex(str);
    }
    else
      return CExprInst->createRealValue(0.0);

    //---

    CExprValuePtr value;

    if      (icol == 0)
      value = CExprInst->createRealValue(plot_->pointNum());
    else if (icol == -2)
      value = CExprInst->createRealValue(plot_->setNum());
    else {
      if (plot_->isParsePlotTitle()) {
        plot_->setKeyColumnHeadNum(icol);

        value = CExprInst->createStringValue("");
      }
      else {
        value = plot_->fieldValue(icol);

        if (! value.isValid())
          value = CExprInst->createStringValue("column(" + std::to_string(icol) + ")");
      }
    }

    return value;
  }
};

class CGnuPlotExistsFn : public CGnuPlotFn {
 public:
  CGnuPlotExistsFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    std::string name;

    if (! values[0]->getStringValue(name))
      return CExprInst->createIntegerValue(0);

    CExprVariablePtr var = CExprInst->getVariable(name);

    return CExprInst->createIntegerValue(var.isValid() ? 1 : 0);
  }
};

class CGnuPlotStringColumnFn : public CGnuPlotFn {
 public:
  CGnuPlotStringColumnFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    assert(values.size() == 1);

    std::string str;

    long col = 0;

    if (values[0]->getIntegerValue(col)) {
      CExprValuePtr value = plot_->fieldValue(col);

      if (value->isStringValue())
        (void) value->getStringValue(str);
    }

    return CExprInst->createStringValue(str);
  }
};

class CGnuPlotStringValidFn : public CGnuPlotFn {
 public:
  CGnuPlotStringValidFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    assert(values.size() == 1);

    std::string str;

    bool valid = false;

    long col = 0;

    if (values[0]->getIntegerValue(col)) {
      CExprValuePtr value = plot_->fieldValue(col);

      valid = value.isValid();
    }

    return CExprInst->createIntegerValue(valid ? 0 : 1);
  }
};

class CGnuPlotValueFn : public CGnuPlotFn {
 public:
  CGnuPlotValueFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    std::string name;

    if (! values[0]->getStringValue(name))
      return CExprValuePtr();

    CExprVariablePtr var = CExprInst->getVariable(name);

    if (! var.isValid())
      return CExprValuePtr();

    return var->getValue();
  }
};

class CGnuPlotValueHSV2RGBFn : public CGnuPlotFn {
 public:
  CGnuPlotValueHSV2RGBFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 3) return CExprValuePtr();

    double h, s, v;

    if (! values[0]->getRealValue(h) ||
        ! values[1]->getRealValue(s) ||
        ! values[2]->getRealValue(v))
      return CExprValuePtr();

    CHSV hsv(h*360, s, v);

    CRGB rgb = CRGBUtil::HSVtoRGB(hsv).clamp();

    return CExprInst->createIntegerValue(rgb.encodeRGB());
  }
};

class CGnuPlotValueStrFTime : public CGnuPlotFn {
 public:
  CGnuPlotValueStrFTime(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 2) return CExprValuePtr();

    std::string s;
    long        i;

    if (! values[0]->getStringValue (s) ||
        ! values[1]->getIntegerValue(i))
      return CExprValuePtr();

    time_t t = i;

    struct tm *tm1 = localtime(&t);

    static char buffer[256];

    strftime(buffer, 256, s.c_str(), tm1);

    return CExprInst->createStringValue(buffer);
  }
};

class CGnuPlotValueStrPTime : public CGnuPlotFn {
 public:
  CGnuPlotValueStrPTime(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 2) return CExprValuePtr();

    std::string s1, s2;

    if (! values[0]->getStringValue(s1) || ! values[1]->getStringValue(s2))
      return CExprValuePtr();

    struct tm tm1;

    memset(&tm1, 0, sizeof(tm));

    /*char *p =*/ strptime(s2.c_str(), s1.c_str(), &tm1);

    time_t t1 = mktime(&tm1);

    memset(&tm1, 0, sizeof(tm));

    time_t t2 = mktime(&tm1);

    return CExprInst->createIntegerValue(difftime(t1, t2));
  }
};

class CGnuPlotValueTmValue : public CGnuPlotFn {
 public:
  CGnuPlotValueTmValue(CGnuPlot *plot, const std::string &value) :
   CGnuPlotFn(plot), value_(value) {
  }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 1) return CExprValuePtr();

    long i;

    if (! values[0]->getIntegerValue(i))
      return CExprValuePtr();

    time_t t = i;

    struct tm *tm1 = localtime(&t);

    int value = 0;

    if      (value_ == "sec" ) value = tm1->tm_sec;
    else if (value_ == "min" ) value = tm1->tm_min;
    else if (value_ == "hour") value = tm1->tm_hour;
    else if (value_ == "mday") value = tm1->tm_mday;
    else if (value_ == "mon" ) value = tm1->tm_mon;
    else if (value_ == "year") value = tm1->tm_year + 1900;
    else if (value_ == "wday") value = tm1->tm_wday;
    else if (value_ == "yday") value = tm1->tm_yday;

    return CExprInst->createIntegerValue(value);
  }

 private:
  std::string value_;
};

class CGnuPlotValueTime : public CGnuPlotFn {
 public:
  CGnuPlotValueTime(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 1) return CExprValuePtr();

    time_t t = time(0);

    return CExprInst->createIntegerValue(t);
  }
};

class CGnuPlotTimeColumnFn : public CGnuPlotFn {
 public:
  CGnuPlotTimeColumnFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    time_t t;

    if      (values.size() == 2) {
      std::string fmt;

      long col = 0;

      if (values[0]->getIntegerValue(col) && values[1]->getStringValue(fmt)) {
        CExprValuePtr value = plot_->fieldValue(col);

        if (value->isStringValue()) {
          std::string str;

          (void) value->getStringValue(str);

          struct tm tm1;

          memset(&tm1, 0, sizeof(tm));

          /*char *p =*/ strptime(str.c_str(), fmt.c_str(), &tm1);

          t = mktime(&tm1);
        }
      }
    }
    else if (values.size() == 1) {
      std::string fmt = plot_->timeFmt().getValue("%d/%m/%y,%H:%M");

      long col = 0;

      if (values[0]->getIntegerValue(col)) {
        CExprValuePtr value = plot_->fieldValue(col);

        if (value->isStringValue()) {
          std::string str;

          (void) value->getStringValue(str);

          struct tm tm1;

          memset(&tm1, 0, sizeof(tm));

          /*char *p =*/ strptime(str.c_str(), fmt.c_str(), &tm1);

          t = mktime(&tm1);
        }
      }
    }
    else {
      assert(false);
    }

    return CExprInst->createIntegerValue(t);
  }

  bool isOverload() const override { return true; }
};

class CGnuPlotSumRangeFn : public CGnuPlotFn {
 public:
  CGnuPlotSumRangeFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValuePtr operator()(const Values &values) {
    if (values.size() != 4) return CExprValuePtr();

    std::string var;

    if (! values[0]->getStringValue(var))
      return CExprValuePtr();

    long start = 0;

    if (! values[1]->getIntegerValue(start))
      return CExprValuePtr();

    long end = start;

    if (! values[2]->getIntegerValue(end))
      return CExprValuePtr();

    std::string expr;

    if (! values[3]->getStringValue(expr))
      return CExprValuePtr();

    //---

    CExprInst->saveCompileState();

    if (expr_ != expr) {
      expr_ = expr;

      CExprTokenStack pstack = CExprInst->parseLine(expr_);
      CExprITokenPtr  itoken = CExprInst->interpPTokenStack(pstack);

      if (itoken.isValid())
        cstack_ = CExprInst->compileIToken(itoken);
      else
        cstack_ = CExprTokenStack();
    }

    //---

    CExprVariablePtr varPtr = CExprInst->createIntegerVariable(var, 0);

    double sum = 0;

    for (int i = start; i <= end; ++i) {
      varPtr->setIntegerValue(i);

      CExprValuePtr rvalue;

      if (! CExprInst->executeCTokenStack(cstack_, rvalue))
        continue;

      double r = 0;

      if (! rvalue.isValid() || ! rvalue->getRealValue(r))
        continue;

      sum += r;
    }

    CExprInst->restoreCompileState();

    return CExprInst->createRealValue(sum);
  }

 private:
  mutable CExprTokenStack cstack_;
  mutable std::string     expr_;
};

//------

namespace CGnuPlotFunctions {

void
init(CGnuPlot *plot)
{
  CExprInst->addFunction("assert", "i", new CGnuPlotAssertFn(plot))->setBuiltin(true);

  CExprInst->addFunction("column"      , "is",
                         new CGnuPlotColumnFn(plot))->setBuiltin(true);
  CExprInst->addFunction("exists"      , "s" ,
                         new CGnuPlotExistsFn(plot))->setBuiltin(true);
  CExprInst->addFunction("stringcolumn", "i" ,
                         new CGnuPlotStringColumnFn(plot))->setBuiltin(true);
  CExprInst->addFunction("strcol"      , "i" ,
                         new CGnuPlotStringColumnFn(plot))->setBuiltin(true);
  CExprInst->addFunction("valid"       , "i" ,
                         new CGnuPlotStringValidFn(plot))->setBuiltin(true);
  CExprInst->addFunction("value"       , "s" ,
                         new CGnuPlotValueFn(plot))->setBuiltin(true);

  CExprInst->addFunction("hsv2rgb", "r,r,r",
                         new CGnuPlotValueHSV2RGBFn(plot))->setBuiltin(true);

  CExprInst->addFunction("strftime", "s,i",
                         new CGnuPlotValueStrFTime(plot))->setBuiltin(true);
  CExprInst->addFunction("strptime", "s,s",
                         new CGnuPlotValueStrPTime(plot))->setBuiltin(true);
  CExprInst->addFunction("tm_hour" , "i"  ,
                         new CGnuPlotValueTmValue(plot, "hour"))->setBuiltin(true);
  CExprInst->addFunction("tm_mday" , "i"  ,
                         new CGnuPlotValueTmValue(plot, "mday"))->setBuiltin(true);
  CExprInst->addFunction("tm_min"  , "i"  ,
                         new CGnuPlotValueTmValue(plot, "min" ))->setBuiltin(true);
  CExprInst->addFunction("tm_mon"  , "i"  ,
                         new CGnuPlotValueTmValue(plot, "mon" ))->setBuiltin(true);
  CExprInst->addFunction("tm_sec"  , "i"  ,
                         new CGnuPlotValueTmValue(plot, "sec" ))->setBuiltin(true);
  CExprInst->addFunction("tm_wday" , "i"  ,
                         new CGnuPlotValueTmValue(plot, "wday"))->setBuiltin(true);
  CExprInst->addFunction("tm_yday" , "i"  ,
                         new CGnuPlotValueTmValue(plot, "yday"))->setBuiltin(true);
  CExprInst->addFunction("tm_year" , "i"  ,
                         new CGnuPlotValueTmValue(plot, "year"))->setBuiltin(true);
  CExprInst->addFunction("time"    , "irs",
                         new CGnuPlotValueTime(plot))->setBuiltin(true);

  CExprInst->addFunction("timecolumn", "i" ,
                         new CGnuPlotTimeColumnFn(plot))->setBuiltin(true);
  CExprInst->addFunction("timecolumn", "i,s" ,
                         new CGnuPlotTimeColumnFn(plot))->setBuiltin(true);

  CExprInst->addFunction("sum_range", "s,i,i,s",
                         new CGnuPlotSumRangeFn(plot))->setBuiltin(true);
}

}
