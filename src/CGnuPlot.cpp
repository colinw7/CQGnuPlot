#include <CGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotStyle.h>

#include <CUnixFile.h>
#include <CParseLine.h>
#include <CDir.h>
#include <CReadLine.h>
#include <CStrUniqueMatch.h>
#include <CLineDash.h>
#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>

#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>

//------

namespace CGnuPlotUtil {
  void initNameValues(CStrUniqueMatch<CGnuPlot::CommandName> &nameValues) {
    nameValues.addValue("bind"   , CGnuPlot::CommandName::BIND);
    nameValues.addValue("call"   , CGnuPlot::CommandName::CALL);
    nameValues.addValue("cd"     , CGnuPlot::CommandName::CD);
    nameValues.addValue("clear"  , CGnuPlot::CommandName::CLEAR);
    nameValues.addValue("exit"   , CGnuPlot::CommandName::EXIT);
    nameValues.addValue("fit"    , CGnuPlot::CommandName::FIT);
    nameValues.addValue("help"   , CGnuPlot::CommandName::HELP);
    nameValues.addValue("history", CGnuPlot::CommandName::HISTORY);
    nameValues.addValue("if"     , CGnuPlot::CommandName::IF);
    nameValues.addValue("load"   , CGnuPlot::CommandName::LOAD);
    nameValues.addValue("lower"  , CGnuPlot::CommandName::LOWER);
    nameValues.addValue("pause"  , CGnuPlot::CommandName::PAUSE);
    nameValues.addValue("plot"   , CGnuPlot::CommandName::PLOT);
    nameValues.addValue("print"  , CGnuPlot::CommandName::PRINT);
    nameValues.addValue("pwd"    , CGnuPlot::CommandName::PWD);
    nameValues.addValue("quit"   , CGnuPlot::CommandName::QUIT);
    nameValues.addValue("raise"  , CGnuPlot::CommandName::RAISE);
    nameValues.addValue("replot" , CGnuPlot::CommandName::REPLOT);
    nameValues.addValue("reread" , CGnuPlot::CommandName::REREAD);
    nameValues.addValue("reset"  , CGnuPlot::CommandName::RESET);
    nameValues.addValue("save"   , CGnuPlot::CommandName::SAVE);
    nameValues.addValue("set"    , CGnuPlot::CommandName::SET);
    nameValues.addValue("shell"  , CGnuPlot::CommandName::SHELL);
    nameValues.addValue("show"   , CGnuPlot::CommandName::SHOW);
    nameValues.addValue("splot"  , CGnuPlot::CommandName::SPLOT);
    nameValues.addValue("system" , CGnuPlot::CommandName::SYSTEM);
    nameValues.addValue("test"   , CGnuPlot::CommandName::TEST);
    nameValues.addValue("unset"  , CGnuPlot::CommandName::UNSET);
    nameValues.addValue("update" , CGnuPlot::CommandName::UPDATE);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::VariableName> &nameValues) {
    nameValues.addValue("datafile"   , CGnuPlot::VariableName::DATAFILE);
    nameValues.addValue("decimalsign", CGnuPlot::VariableName::DECIMALSIGN);
    nameValues.addValue("encoding"   , CGnuPlot::VariableName::ENCODING);
    nameValues.addValue("fit"        , CGnuPlot::VariableName::FIT);
    nameValues.addValue("fontpath"   , CGnuPlot::VariableName::FONTPATH);
    nameValues.addValue("historysize", CGnuPlot::VariableName::HISTORYSIZE);
    nameValues.addValue("loadpath"   , CGnuPlot::VariableName::LOADPATH);
    nameValues.addValue("macros"     , CGnuPlot::VariableName::MACROS);
    nameValues.addValue("mouse"      , CGnuPlot::VariableName::MOUSE);

    nameValues.addValue("angles"     , CGnuPlot::VariableName::ANGLES);
    nameValues.addValue("dummy"      , CGnuPlot::VariableName::DUMMY);
    nameValues.addValue("mapping"    , CGnuPlot::VariableName::MAPPING);
    nameValues.addValue("parametric" , CGnuPlot::VariableName::PARAMETRIC);
    nameValues.addValue("polar"      , CGnuPlot::VariableName::POLAR);
    nameValues.addValue("samples"    , CGnuPlot::VariableName::SAMPLES);
    nameValues.addValue("zero"       , CGnuPlot::VariableName::ZERO);

    nameValues.addValue("border"     , CGnuPlot::VariableName::BORDER);
    nameValues.addValue("clip"       , CGnuPlot::VariableName::CLIP);
    nameValues.addValue("logscale"   , CGnuPlot::VariableName::LOGSCALE);
    nameValues.addValue("bmargin"    , CGnuPlot::VariableName::BMARGIN);
    nameValues.addValue("lmargin"    , CGnuPlot::VariableName::LMARGIN);
    nameValues.addValue("rmargin"    , CGnuPlot::VariableName::RMARGIN);
    nameValues.addValue("tmargin"    , CGnuPlot::VariableName::TMARGIN);
    nameValues.addValue("multiplot"  , CGnuPlot::VariableName::MULTIPLOT);
    nameValues.addValue("offsets"    , CGnuPlot::VariableName::OFFSETS);
    nameValues.addValue("origin"     , CGnuPlot::VariableName::ORIGIN);
    nameValues.addValue("size"       , CGnuPlot::VariableName::SIZE);

    nameValues.addValue("arrow"      , CGnuPlot::VariableName::ARROW);
    nameValues.addValue("key"        , CGnuPlot::VariableName::KEY);
    nameValues.addValue("label"      , CGnuPlot::VariableName::LABEL);
    nameValues.addValue("object"     , CGnuPlot::VariableName::OBJECT);
    nameValues.addValue("timestamp"  , CGnuPlot::VariableName::TIMESTAMP);
    nameValues.addValue("title"      , CGnuPlot::VariableName::TITLE);

    nameValues.addValue("style"      , CGnuPlot::VariableName::STYLE);

    nameValues.addValue("bars"       , CGnuPlot::VariableName::BARS);
    nameValues.addValue("boxwidth"   , CGnuPlot::VariableName::BOXWIDTH);
    nameValues.addValue("pointsize"  , CGnuPlot::VariableName::POINTSIZE);

    nameValues.addValue("autoscale"  , CGnuPlot::VariableName::AUTOSCALE);
    nameValues.addValue("format"     , CGnuPlot::VariableName::FORMAT);
    nameValues.addValue("grid"       , CGnuPlot::VariableName::GRID);
    nameValues.addValue("xlabel"     , CGnuPlot::VariableName::XLABEL); // x2, y2, z, cb
    nameValues.addValue("ylabel"     , CGnuPlot::VariableName::YLABEL);
    nameValues.addValue("xrange"     , CGnuPlot::VariableName::XRANGE); // x2, y2, z, cb
    nameValues.addValue("yrange"     , CGnuPlot::VariableName::YRANGE); // y, x2, y2, z, cb
    nameValues.addValue("tics"       , CGnuPlot::VariableName::TICS);
    nameValues.addValue("xtics"      , CGnuPlot::VariableName::XTICS); // x2, y2, z, cb
    nameValues.addValue("ytics"      , CGnuPlot::VariableName::YTICS);
    nameValues.addValue("mxtics"     , CGnuPlot::VariableName::MXTICS);
    nameValues.addValue("ticscale"   , CGnuPlot::VariableName::TICSCALE);
    nameValues.addValue("zeroaxis"   , CGnuPlot::VariableName::ZEROAXIS);

    nameValues.addValue("locale"     , CGnuPlot::VariableName::LOCALE);
    nameValues.addValue("timefmt"    , CGnuPlot::VariableName::TIMEFMT);
    nameValues.addValue("xdata"      , CGnuPlot::VariableName::XDATA);
    nameValues.addValue("xdtics"     , CGnuPlot::VariableName::XDTICS);
    nameValues.addValue("xmtics"     , CGnuPlot::VariableName::XMTICS);

    nameValues.addValue("dgrid3d"    , CGnuPlot::VariableName::DGRID3D);
    nameValues.addValue("hidden3d"   , CGnuPlot::VariableName::HIDDEN3D);
    nameValues.addValue("isosamples" , CGnuPlot::VariableName::ISOSAMPLES);
    nameValues.addValue("surface"    , CGnuPlot::VariableName::SURFACE);
    nameValues.addValue("view"       , CGnuPlot::VariableName::VIEW);
    nameValues.addValue("xyplane"    , CGnuPlot::VariableName::XYPLANE);

    nameValues.addValue("cplane"     , CGnuPlot::VariableName::CPLANE);
    nameValues.addValue("cntrparam"  , CGnuPlot::VariableName::CNTRPARAM);
    nameValues.addValue("contour"    , CGnuPlot::VariableName::CONTOUR);

    nameValues.addValue("colorbox"   , CGnuPlot::VariableName::COLORBOX);
    nameValues.addValue("palette"    , CGnuPlot::VariableName::PALETTE);
    nameValues.addValue("pm3d"       , CGnuPlot::VariableName::PM3D);

    nameValues.addValue("output"     , CGnuPlot::VariableName::OUTPUT);
    nameValues.addValue("print"      , CGnuPlot::VariableName::PRINT);
    nameValues.addValue("table"      , CGnuPlot::VariableName::TABLE);
    nameValues.addValue("terminal"   , CGnuPlot::VariableName::TERMINAL);
    nameValues.addValue("termoption" , CGnuPlot::VariableName::TERMOPTION);

    nameValues.addValue("variables"  , CGnuPlot::VariableName::VARIABLES);
    nameValues.addValue("functions"  , CGnuPlot::VariableName::FUNCTIONS);
    nameValues.addValue("version"    , CGnuPlot::VariableName::VERSION);

    nameValues.addValue("debug"      , CGnuPlot::VariableName::DEBUG);
    nameValues.addValue("edebug"     , CGnuPlot::VariableName::EDEBUG);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::DataFileVar> &nameValues) {
    nameValues.addValue("separator"   , CGnuPlot::DataFileVar::SEPARATOR);
    nameValues.addValue("commentschar", CGnuPlot::DataFileVar::COMMENTS_CHAR);
    nameValues.addValue("missing"     , CGnuPlot::DataFileVar::MISSING);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::StyleVar> &nameValues) {
    nameValues.addValue("data"     , CGnuPlot::StyleVar::DATA);
    nameValues.addValue("function" , CGnuPlot::StyleVar::FUNCTION);
    nameValues.addValue("increment", CGnuPlot::StyleVar::INCREMENT);
    nameValues.addValue("line"     , CGnuPlot::StyleVar::LINE);
    nameValues.addValue("fill"     , CGnuPlot::StyleVar::FILL);
    nameValues.addValue("histogram", CGnuPlot::StyleVar::HISTOGRAM);
    nameValues.addValue("rectangle", CGnuPlot::StyleVar::RECTANGLE);
    nameValues.addValue("circle"   , CGnuPlot::StyleVar::CIRCLE);
    nameValues.addValue("ellipse"  , CGnuPlot::StyleVar::ELLIPSE);
    nameValues.addValue("arrow"    , CGnuPlot::StyleVar::ARROW);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotVar> &nameValues) {
    nameValues.addValue("using"    , CGnuPlot::PlotVar::USING);
    nameValues.addValue("index"    , CGnuPlot::PlotVar::INDEX);
    nameValues.addValue("every"    , CGnuPlot::PlotVar::EVERY);
    nameValues.addValue("with"     , CGnuPlot::PlotVar::WITH);
    nameValues.addValue("title"    , CGnuPlot::PlotVar::TITLE);
    nameValues.addValue("notitle"  , CGnuPlot::PlotVar::NOTITLE);
    nameValues.addValue("smooth"   , CGnuPlot::PlotVar::SMOOTH);
    nameValues.addValue("pointsize", CGnuPlot::PlotVar::POINTSIZE);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotStyle> &nameValues) {
    nameValues.addValue("lines"         , CGnuPlot::PlotStyle::LINES);
    nameValues.addValue("l"             , CGnuPlot::PlotStyle::LINES);
    nameValues.addValue("points"        , CGnuPlot::PlotStyle::POINTS);
    nameValues.addValue("p"             , CGnuPlot::PlotStyle::POINTS);
    nameValues.addValue("linespoints"   , CGnuPlot::PlotStyle::LINES_POINTS);
    nameValues.addValue("dots"          , CGnuPlot::PlotStyle::DOTS);
    nameValues.addValue("impulses"      , CGnuPlot::PlotStyle::IMPULSES);
    nameValues.addValue("labels"        , CGnuPlot::PlotStyle::LABELS);
    nameValues.addValue("steps"         , CGnuPlot::PlotStyle::STEPS);
    nameValues.addValue("fsteps"        , CGnuPlot::PlotStyle::FSTEPS);
    nameValues.addValue("histeps"       , CGnuPlot::PlotStyle::HISTEPS);
    nameValues.addValue("errorbars"     , CGnuPlot::PlotStyle::ERRORBARS);
    nameValues.addValue("errorlines"    , CGnuPlot::PlotStyle::ERRORLINES);
    nameValues.addValue("financebars"   , CGnuPlot::PlotStyle::FINANCEBARS);
    nameValues.addValue("xerrorbars"    , CGnuPlot::PlotStyle::XERRORBARS);
    nameValues.addValue("xerrorlines"   , CGnuPlot::PlotStyle::XERRORLINES);
    nameValues.addValue("xyerrorbars"   , CGnuPlot::PlotStyle::XYERRORBARS);
    nameValues.addValue("xyerrorlines"  , CGnuPlot::PlotStyle::XYERRORLINES);
    nameValues.addValue("yerrorbars"    , CGnuPlot::PlotStyle::YERRORBARS);
    nameValues.addValue("yerrorlines"   , CGnuPlot::PlotStyle::YERRORLINES);
    nameValues.addValue("vectors"       , CGnuPlot::PlotStyle::VECTORS);

    nameValues.addValue("boxes"         , CGnuPlot::PlotStyle::BOXES);
    nameValues.addValue("boxerrorbars"  , CGnuPlot::PlotStyle::BOXERRORBARS);
    nameValues.addValue("boxxyerrorbars", CGnuPlot::PlotStyle::BOXXYERRORBARS);
    nameValues.addValue("boxplot"       , CGnuPlot::PlotStyle::BOXPLOT);
    nameValues.addValue("candlesticks"  , CGnuPlot::PlotStyle::CANDLESTICKS);
    nameValues.addValue("filledcurves"  , CGnuPlot::PlotStyle::FILLEDCURVES);
    nameValues.addValue("histograms"    , CGnuPlot::PlotStyle::HISTOGRAMS);
    nameValues.addValue("image"         , CGnuPlot::PlotStyle::IMAGE);
    nameValues.addValue("rgbimage"      , CGnuPlot::PlotStyle::RGBIMAGE);
    nameValues.addValue("rgbalpha"      , CGnuPlot::PlotStyle::RGBALPHA);
    nameValues.addValue("circles"       , CGnuPlot::PlotStyle::CIRCLES);
    nameValues.addValue("ellipses"      , CGnuPlot::PlotStyle::ELLIPSES);
    nameValues.addValue("pm3d"          , CGnuPlot::PlotStyle::PM3D);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::Smooth> &nameValues) {
    nameValues.addValues({{"unique"   , CGnuPlot::Smooth::UNIQUE},
                          {"frequency", CGnuPlot::Smooth::FREQUENCY},
                          {"bezier"   , CGnuPlot::Smooth::BEZIER},
                          {"sbezier"  , CGnuPlot::Smooth::SBEZIER},
                          {"csplines" , CGnuPlot::Smooth::CSPLINES},
                          {"acsplines", CGnuPlot::Smooth::ACSPLINES}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::FillType> &nameValues) {
    nameValues.addValues({{"empty"  , CGnuPlot::FillType::EMPTY},
                          {"solid"  , CGnuPlot::FillType::SOLID},
                          {"pattern", CGnuPlot::FillType::PATTERN}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::HistogramStyle> &nameValues) {
    nameValues.addValues({{"clustered"    , CGnuPlot::HistogramStyle::CLUSTERED},
                          {"errorbars"    , CGnuPlot::HistogramStyle::ERRORBARS},
                          {"rowstacked"   , CGnuPlot::HistogramStyle::ROWSTACKED},
                          {"columnstacked", CGnuPlot::HistogramStyle::COLUMNSTACKED}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::AngleType> &nameValues) {
    nameValues.addValues({{"radians", CGnuPlot::AngleType::RADIANS},
                          {"degrees", CGnuPlot::AngleType::DEGREES}});
  }

  //------

  template<typename T>
  CStrUniqueMatch<T> &getNameValues() {
    static CStrUniqueMatch<T> nameValues;

    if (! nameValues.numValues())
      initNameValues(nameValues);

    return nameValues;
  }

  template<typename T>
  bool stringToValue(const std::string &str, T &value) {
    return getNameValues<T>().match(str, value);
  }

  template<typename T>
  bool stringToValueWithErr(const std::string &str, T &value, const std::string &err) {
    if (! stringToValue(str, value)) {
      std::cerr << "Invalid " << err << " '" << str << "'" << std::endl;
      return false;
    }
    return true;
  }

  template<typename T>
  std::string valueToString(const T &value) {
    return getNameValues<T>().lookup(value);
  }

  template<typename T>
  std::string valueStrings() {
    std::vector<std::string> strs;

    getNameValues<T>().values(strs);

    std::string cstr;

    for (const auto &str : strs) {
      if (! cstr.empty()) str += ", ";

      cstr += "'" + str + "'";
    }

    return cstr;
  }

  //------

  CExprValuePtr decodeUsingCol(const CGnuPlot::UsingCol &col, int setNum, int pointNum,
                               const std::vector<std::string> &fields,
                               const std::string &missing, bool &skip, bool debug) {
    skip = false;

    int nf = fields.size();

    if (col.isInt) {
      if      (col.ival == 0)
        return CExprInst->createRealValue(pointNum);
      else if (col.ival == -2)
        return CExprInst->createRealValue(setNum);
      else if (col.ival > 0 && col.ival <= nf) {
        double val;

        const std::string &field = fields[col.ival - 1];

        if (field == missing) {
          skip = true;
          return CExprValuePtr();
        }

        if (CStrUtil::toReal(field, &val))
          return CExprInst->createRealValue(val);
        else
          return CExprInst->createStringValue(field);
      }
      else
        return CExprValuePtr();
    }
    else {
      std::string expr = col.str;

      // TODO: easier to define $1 variables
      auto pos = expr.find('$');

      while (pos != std::string::npos) {
        int pos1 = ++pos;

        while (isdigit(expr[pos1]))
          ++pos1;

        std::string numStr = expr.substr(pos, pos1 - pos);

        int icol = 0;

        (void) CStrUtil::toInteger(numStr, &icol);

        COptReal x1;

        if      (icol == 0)
          x1 = pointNum;
        else if (icol == -2)
          x1 = setNum;
        else if (icol > 0 && icol <= nf) {
          double x2 = 0.0;

          const std::string &field = fields[icol - 1];

          if (field != missing && CStrUtil::toReal(field, &x2))
            x1 = x2;
        }

        if (x1.isValid())
          expr = expr.substr(0, pos - 1) + CStrUtil::toString(x1.getValue()) + expr.substr(pos1);
        else
          expr = expr.substr(0, pos - 1) + "NaN" + expr.substr(pos1);

        pos = expr.find('$');
      }

      if (debug)
        std::cerr << "expr " << expr << std::endl;

      return CExprInst->evaluateExpression(expr);
    }
  }
}

//------

class CGnuPlotReadLine : public CReadLine {
 public:
  CGnuPlotReadLine(CGnuPlot *plot) :
   plot_(plot) {
  }

  void timeout() {
    plot_->timeout();
  }

 private:
  CGnuPlot *plot_;
};

//------

class CGnuPlotColumnFn : public CExprFunctionObj {
 public:
  CGnuPlotColumnFn(CGnuPlot *plot) : plot_(plot) { }

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &) {
    return CExprInst->createRealValue(0.0);
  }

 private:
  CGnuPlot *plot_;
};

class CGnuPlotStringColumnFn : public CExprFunctionObj {
 public:
  CGnuPlotStringColumnFn(CGnuPlot *plot) : plot_(plot) { }

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &values) {
    assert(values.size() == 1);

    std::string str;

    long col = 0;

    if (values[0]->getIntegerValue(col))
      str = plot_->getField(col);

    return CExprInst->createStringValue(str);
  }

 private:
  CGnuPlot *plot_;
};

class CGnuPlotStringValidFn : public CExprFunctionObj {
 public:
  CGnuPlotStringValidFn(CGnuPlot *plot) : plot_(plot) { }

  CExprValuePtr operator()(const std::vector<CExprValuePtr> &) {
    return CExprInst->createRealValue(0.0);
  }

 private:
  CGnuPlot *plot_;
};

//------

CGnuPlot::
CGnuPlot()
{
  CExprInst->createRealVariable("pi", M_PI);

  // exists
  CExprInst->addFunction("column"      , "i", new CGnuPlotColumnFn(this));
  CExprInst->addFunction("stringcolumn", "i", new CGnuPlotStringColumnFn(this));
  CExprInst->addFunction("valid"       , "i", new CGnuPlotStringValidFn(this));
  // timecolumn
}

CGnuPlot::
~CGnuPlot()
{
}

void
CGnuPlot::
setDebug(bool b)
{
  debug_ = b;
}

void
CGnuPlot::
setExprDebug(bool b)
{
  edebug_ = b;

  CExprInst->setDebug(edebug_);
}

bool
CGnuPlot::
load(const std::string &filename)
{
  // open file
  CUnixFile file(filename);

  if (! file.open())
    return false;

  // process each line in file
  std::string line;

  while (file.readLine(line)) {
    if (line.empty()) continue;

    parseLine(line);
  }

  return true;
}

void
CGnuPlot::
loop()
{
  if (! readLine_.isValid()) {
    readLine_ = new CGnuPlotReadLine(this);

    readLine_->enableTimeoutHook(1);
  }

  for (;;) {
    std::string line = readLine_->readLine();

    parseLine(line);

    readLine_->addHistory(line);
  }
}

bool
CGnuPlot::
parseLine(const std::string &str)
{
  auto p = str.find(';');

  if (p != std::string::npos) {
    if (! parseLine(str.substr(0, p)))
      return false;

    return parseLine(str.substr(p + 1));
  }

  //---

  // get first space separated word
  CParseLine line(str);

  line.skipSpace();

  std::string identifier;

  while (line.isValid()) {
    char c = line.lookChar();

    if (identifier.empty()) {
      if (! isalpha(c))
        break;
    }
    else {
      if (! isalnum(c) && c != '_')
        break;
    }

    identifier += line.getChar();
  }

  line.skipSpace();

  // empty line is ok
  if (identifier.empty())
    return true;

  // variable assignment
  if      (line.isChar('=')) {
    line.skipChar();

    line.skipSpace();

    CExprValuePtr value = CExprInst->evaluateExpression(line.substr());

    if (! value.isValid())
      return false;

    CExprInst->createVariable(identifier, value);

    return true;
  }
  // function definition
  else if (line.isChar('(')) {
    line.skipChar();

    line.skipSpace();

    std::string              arg;
    std::vector<std::string> args;

    while (line.isValid()) {
      if      (line.isChar(')')) {
        line.skipChar();

        break;
      }
      else if (line.isChar(',')) {
        if (! arg.empty())
          args.push_back(arg);

        arg = "";

        line.skipChar();
      }
      else
        arg += line.getChar();
    }

    if (! arg.empty())
      args.push_back(arg);

    line.skipSpace();

    if (line.isChar('=')) {
      line.skipChar();

      line.skipSpace();

      CExprInst->addFunction(identifier, args, line.substr());

      return true;
    }
  }

  //---

  std::string args = line.substr();

  CommandName commandName;

  if (! CGnuPlotUtil::stringToValue(identifier, commandName))
    commandName = CommandName::NONE;

  // process command
  switch (commandName) {
    case CommandName::EXIT   : exitCmd   (args); break;
    case CommandName::HELP   : helpCmd   (args); break;
    case CommandName::HISTORY: historyCmd(args); break;
    case CommandName::PRINT  : printCmd  (args); break;
    case CommandName::QUIT   : quitCmd   (args); break;
    case CommandName::CD     : cdCmd     (args); break;
    case CommandName::PWD    : pwdCmd    (args); break;

    case CommandName::CALL   : callCmd  (args); break;
    case CommandName::LOAD   : loadCmd  (args); break;
    case CommandName::SAVE   : saveCmd  (args); break;

    case CommandName::PLOT   : plotCmd  (args); break;
    case CommandName::REPLOT : replotCmd(args); break;
    case CommandName::SPLOT  : splotCmd (args); break;

    case CommandName::SET    : setCmd   (args); break;
    case CommandName::SHOW   : showCmd  (args); break;
    case CommandName::RESET  : resetCmd (args); break;
    case CommandName::UNSET  : unsetCmd (args); break;

    case CommandName::SHELL  : shellCmd (args); break;
    case CommandName::SYSTEM : systemCmd(args); break;

    case CommandName::CLEAR  : clearCmd (args); break;
    case CommandName::LOWER  : lowerCmd (args); break;
    case CommandName::RAISE  : raiseCmd (args); break;
    case CommandName::TEST   : testCmd  (args); break;

    case CommandName::FIT    : fitCmd   (args); break;
    case CommandName::UPDATE : updateCmd(args); break;

    case CommandName::BIND   : bindCmd  (args); break;
    case CommandName::IF     : ifCmd    (args); break;
    case CommandName::PAUSE  : pauseCmd (args); break;
    case CommandName::REREAD : rereadCmd(args); break;
    default: {
      std::cerr << "Invalid command: " << identifier << std::endl;
      return false;
    }
  }

  return true;
}

// exit
void
CGnuPlot::
exitCmd(const std::string &)
{
  exit(0);
}

// help [{topic}]
void
CGnuPlot::
helpCmd(const std::string &)
{
}

// history [quiet] [{max:i}] ["<filename>" [append]]
// history ?"<cmd>"
// history !"<cmd>"
void
CGnuPlot::
historyCmd(const std::string &)
{
  // TODO: process args
  if (! readLine_.isValid())
    return;

  CReadLine::HistoryEntries entries;

  readLine_->getHistoryEntries(entries);

  for (uint i = 0; i < entries.size(); ++i) {
    std::cerr << entries[i].line_num << ": " << entries[i].line << std::endl;
  }
}

// print {expression} [, {expression} ]
void
CGnuPlot::
printCmd(const std::string &args)
{
  std::vector<CExprValuePtr> values;

  if (CExprInst->evaluateExpression(args, values)) {
    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << " ";
      std::cerr << values[i];
    }
    std::cerr << std::endl;
  }
  else
    std::cerr << "Invalid expression" << std::endl;
}

// quit
void
CGnuPlot::
quitCmd(const std::string &)
{
  exit(0);
}

// cd "<path"
void
CGnuPlot::
cdCmd(const std::string &args)
{
  CParseLine line(args);

  std::string str;

  if (! parseString(line, str))
    return;

  CStrUtil::stripSpaces(str);

  if (str.empty()) {
    std::cerr << "Missing directory name" << std::endl;
    return;
  }

  if (str[0] != '/')
    CDir::changeDir(CDir::getCurrent() + "/" + str);
  else
    CDir::changeDir(str);
}

// pwd
void
CGnuPlot::
pwdCmd(const std::string &)
{
  std::cout << CDir::getCurrent() << std::endl;
}

// call "<filename>" [ {val}, ... ]
void
CGnuPlot::
callCmd(const std::string &args)
{
  CParseLine line(args);

  std::string file;

  if (! parseString(line, file))
    return;

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    args1.push_back(arg);
  }
}

// load "<filename>"
void
CGnuPlot::
loadCmd(const std::string &args)
{
  CParseLine line(args);

  std::string filename;

  if (! parseString(line, filename))
    return;

  load(filename);
}

// save "<filename>"
// save [ functions | variables | set | terminal ] "<filename>"
void
CGnuPlot::
saveCmd(const std::string &)
{
}

// plot [ {ranges} ]
//      [ (function) | "{str:filename}" ]
//                   [ index {int:start}[:{int:end}][:(int:step}] ]
//                   [ every {int:step}[::{int:start}[::(int:end}]] ]
//                   [ using {columnspec} ]
//                   [ axes [x1y1 | x1y2 | x2y1 | x2y2 ] ]
//                   [ title [ "{str:explanation}" | {int:col} ] | notitle ]
//                   [ smooth [ unique | frequency | bezier | sbezier | csplines | acsplines ] ]
//                   [ with {stylespec} ]
//      [, ...]
void
CGnuPlot::
plotCmd(const std::string &args)
{
  typedef std::vector<CGnuPlotPlot *> Plots;

  Plots plots;

  CBBox2D region;

  CGnuPlotWindow *window;

  if (multiplot().enabled) {
    if (! windows_.empty())
      window = windows_.back();
    else {
      window = createWindow();

      window->set3D(false);

      windows_.push_back(window);
    }

    int n = window->numPlots();

    double dx = 1.0/multiplot().cols;
    double dy = 1.0/multiplot().rows;

    int ix = n % multiplot().cols;
    int iy = n / multiplot().cols;

    double x1 = plotSize_.x.getValue(ix*dx);
    double y1 = plotSize_.y.getValue(iy*dy);
    double x2 = x1 + plotSize_.xsize.getValue(dx);
    double y2 = y1 + plotSize_.ysize.getValue(dy);

    region = CBBox2D(x1, y1, x2, y2);
  }
  else {
    window = createWindow();

    window->set3D(false);

    double x1 = plotSize_.x.getValue(0.0);
    double y1 = plotSize_.y.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region = CBBox2D(x1, y1, x2, y2);

    windows_.push_back(window);
  }

  //----

  CParseLine line(args);

  std::string lastFilename;

  setFitX(true);
  setFitY(true);

  while (true) {
    // Get Range
    //  [XMIN:XMAX][YMIN:YMAX]

    if (line.isChar('[')) {
      line.skipChar();

      std::string xrange, yrange;

      while (line.isValid() && ! line.isChar(']'))
        xrange += line.getChar();

      if (line.isValid())
        line.skipChar();

      if (line.isChar('[')) {
        line.skipChar();

        while (line.isValid() && ! line.isChar(']'))
          yrange += line.getChar();

        if (line.isValid())
          line.skipChar();
      }

      if (isDebug())
        std::cerr << "Range [" << xrange << "][" << yrange << "]" << std::endl;

      std::vector<std::string> xfields, yfields;

      CStrUtil::addFields(xrange, xfields, ":");
      CStrUtil::addFields(yrange, yfields, ":");

      if (xfields.size() == 2) {
        double xmin = -10, xmax = 10;
        (void) CStrUtil::toReal(xfields[0], &xmin);
        (void) CStrUtil::toReal(xfields[1], &xmax);
        if (isDebug())
          std::cerr << "XRange=(" << xmin << "," << xmax << ")" << std::endl;
        axesData().xaxis.min = xmin;
        axesData().xaxis.max = xmax;
        setFitX(false);
      }

      if (yfields.size() == 2) {
        double ymin = -1, ymax = 1;
        (void) CStrUtil::toReal(yfields[0], &ymin);
        (void) CStrUtil::toReal(yfields[1], &ymax);
        if (isDebug())
          std::cerr << "YRange=(" << ymin << "," << ymax << ")" << std::endl;
        axesData().yaxis.min = ymin;
        axesData().yaxis.max = ymax;
        setFitY(false);
      }

      line.skipSpace();
    }

    //----

    // TODO: newhistogram <title> , ...

    //----

    // Get filename or expression
    //  "<filename>" | <expression>

    PlotStyle style(PlotStyle::POINTS);

    std::string filename, function;

    if (line.isChar('\"')) {
      // filename
      line.skipChar();

      while (line.isValid()) {
        char c = line.getChar();

        if      (c == '\\') {
          if (line.isValid()) {
            char c1 = line.getChar();

            filename += c1;
          }
          else
            filename += c;
        }
        else if (c == '\"')
          break;
        else
          filename += c;
      }

      if (filename.empty())
        filename = lastFilename;

      if (isDebug())
        std::cerr << "Filename: " << filename << std::endl;

      lastFilename = filename;

      style = getDataStyle();
    }
    else {
      // function
      if (! parseFunction(line, function))
        return;

      if (isDebug())
        std::cerr << "Function: " << function << std::endl;

      style = getFunctionStyle();
    }

    line.skipSpace();

    //---

    UsingCols usingCols;
    Index     index;
    Every     every;

    setSmooth(Smooth::NONE);

    // Read individual plot:
    //   <plot_data> [, <plot_data> ...]
    // TODO: using must come before title and with
    while (line.isValid() && ! line.isChar(',')) {
      std::string option = readNonSpace(line);

      PlotVar plotVar;

      if (! CGnuPlotUtil::stringToValueWithErr(option, plotVar, "plot var"))
        break;

      line.skipSpace();

      // Read using columns
      // using <col1>[:<col2>[...]]
      if      (plotVar == PlotVar::USING) {
        std::string              usingStr;
        std::vector<std::string> usingStrs;

        int rbrackets = 0;

        while (line.isValid()) {
          if (rbrackets == 0) {
            if (line.isSpace() || line.isChar(','))
              break;

            if (line.isChar(':')) {
              line.skipChar();

              usingStr = CStrUtil::stripSpaces(usingStr);

              if (! usingStr.empty())
                usingStrs.push_back(usingStr);

              usingStr = "";
            }
            else {
              if (line.isChar('('))
                ++rbrackets;

              if (rbrackets == 0)
                usingStr += line.getChar();
              else
                line.skipChar();
            }
          }
          else {
            if      (line.isChar('('))
              ++rbrackets;
            else if (line.isChar(')'))
              --rbrackets;

            if (rbrackets > 0)
              usingStr += line.getChar();
            else
              line.skipChar();
          }
        }

        usingStr = CStrUtil::stripSpaces(usingStr);

        if (! usingStr.empty())
          usingStrs.push_back(usingStr);

        if (isDebug()) {
          std::cerr << "using ";

          for (uint i = 0; i < usingStrs.size(); ++i) {
            if (i > 0) std::cerr << ":";

            std::cerr << usingStrs[i];
          }

          std::cerr << std::endl;
        }

        line.skipSpace();

        for (uint i = 0; i < usingStrs.size(); ++i)
          usingCols.push_back(UsingCol(usingStrs[i]));
      }
      // read index range
      // <start>[:<end>[:<step>]]
      else if (plotVar == PlotVar::INDEX) {
        std::string indexStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "index " << indexStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(indexStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          (void) CStrUtil::toInteger(inds[1], &index.end);
          (void) CStrUtil::toInteger(inds[2], &index.step);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          (void) CStrUtil::toInteger(inds[1], &index.end);
          index.step = 1;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          index.end  = index.start;
          index.step = 1;
        }
      }
      // read every range
      // <step>[:<start>[:<end>]]
      else if (plotVar == PlotVar::EVERY) {
        std::string everyStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "every " << everyStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(everyStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          (void) CStrUtil::toInteger(inds[1], &every.start);
          (void) CStrUtil::toInteger(inds[2], &every.end);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          (void) CStrUtil::toInteger(inds[1], &every.start);
          every.end = INT_MAX;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          every.start = 0;
          every.end   = INT_MAX;
        }
      }
      // with <style> { {linestyle | ls <line_style>} |
      //                {{linetype | lt <line_type>}
      //                 {linewidth | lw <line_width>}
      //                 {linecolor | lc <colorspec>}
      //                 {pointtype | pt <point_type>}
      //                 {pointsize | ps <point_size>}
      //                 {fill | fs <fillstyle>}
      //                 {nohidden3d} {nocontours} {nosurface}
      //                 {palette}}
      //              }
      // <style<[:<style>[...]]]
      else if (plotVar == PlotVar::WITH) {
        std::string withStr = readNonSpaceNonComma(line);

        if (CGnuPlotUtil::stringToValueWithErr(withStr, style, "plot style")) {
          if (isDebug())
            std::cerr << "with " << CGnuPlotUtil::valueToString(style) << std::endl;
        }

        line.skipSpace();

        bool modifiers = true;

        while (modifiers) {
          if      (line.isString("linestyle") || line.isString("ls")) {
            line.skipNonSpace();

            std::string styleStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(styleStr, &l))
              setLineStyleNum(COptInt(l));
          }
          else if (line.isString("linetype") || line.isString("lt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              lineStyle().setType(l);
          }
          else if (line.isString("linewidth") || line.isString("lw")) {
            double r;

            if (parseReal(line, r))
              lineStyle().setWidth(r);
          }
          else if (line.isString("linecolor") || line.isString("lc")) {
            CRGBA c;

            if (parseColorSpec(line, c))
              lineStyle().setColor(c);
          }
          else if (line.isString("pointtype") || line.isString("pt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              pointStyle().setType(static_cast<SymbolType>(l));
          }
          else if (line.isString("pointsize") || line.isString("ps")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);

            double s;

            if      (style == "variable" || style == "var")
              pointStyle().setVarSize(true);
            else if (CStrUtil::toReal(style, &s))
              pointStyle().setSize(s);
          }
          else if (line.isString("fill") || line.isString("fs")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);
          }
          else if (line.isString("nohidden3d")) {
            line.skipNonSpace();
          }
          else if (line.isString("nocontours")) {
            line.skipNonSpace();
          }
          else if (line.isString("nosurface")) {
            line.skipNonSpace();
          }
          else if (line.isString("palette")) {
            line.skipNonSpace();
          }
          else
            modifiers = false;

          line.skipSpace();
        }
      }
      // title string
      else if (plotVar == PlotVar::TITLE) {
        std::string titleStr;
        int         columnNum;

        if      (parseString(line, titleStr)) {
          if (isDebug())
            std::cerr << "title " << titleStr << std::endl;

          keyData_.title = titleStr;
        }
        else if (parseInteger(line, columnNum)) {
          keyData_.columnNum  = columnNum;
          keyData_.columnhead = true;
        }

        line.skipSpace();
      }
      // disable title
      else if (plotVar == PlotVar::NOTITLE) {
      }
      // smooth type
      else if (plotVar == PlotVar::SMOOTH) {
        std::string smoothStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "smooth " << smoothStr << std::endl;

        Smooth smooth;

        if (CGnuPlotUtil::stringToValueWithErr(smoothStr, smooth, "smooth type"))
          setSmooth(smooth);

        line.skipSpace();
      }
      // pointsize <size>
      else if (plotVar == PlotVar::POINTSIZE) {
        // TODO: just for this plot ?
        double r;

        if (parseReal(line, r))
          pointStyle().setSize(r > 0 ? r : 1.0);
      }
    }

    //---

    if      (! filename.empty()) {
      if      (usingCols.empty()) {
        usingCols.push_back(UsingCol(1));
        usingCols.push_back(UsingCol(2));
      }
      else if (usingCols.size() == 1) {
        usingCols.push_back(UsingCol(0));

        std::swap(usingCols[0], usingCols[1]);
      }

      Plots plots1 = addFile2D(window, filename, region, style, usingCols, index, every);

      std::copy(plots1.begin(), plots1.end(), std::back_inserter(plots));
    }
    else if (! function.empty()) {
      CGnuPlotPlot *plot = addFunction2D(window, function, region, style);

      if (plot)
        plots.push_back(plot);
    }
    else
      break;

    //---

    // move to next plot if any
    if (! line.isChar(','))
      break;

    line.skipChar();

    line.skipSpace();
  }

  window->setTitle(title());

  //---

  if (plots.empty())
    return;

  //---

  for (auto plot : plots) {
    plot->fit();

    plot->smooth();
  }

  //---

  if (plots.size() > 1) {
    CGnuPlotPlot *plot = createPlot(window);

    plot->setStyle  (plots[0]->getStyle());
    plot->setKeyData(plots[0]->keyData());

    plot->addSubPlots(plots);

    plot->setHistogramStyle(histogramStyle());

    plot->setFitX(true);
    plot->setFitY(true);

    plot->fit();

    window->addPlot(plot);
  }
  else
    window->addPlot(plots[0]);

  //---

  window->stateChanged(ChangeState::PLOT_ADDED);
}

// replot
void
CGnuPlot::
replotCmd(const std::string &)
{
}

// splot [ {ranges} ]
//       [ (function) | "{str:filename}" ]
//                    [ index {int:start}[:{int:end}][:(int:step}] ]
//                    [ every {int:step}[::{int:start}[::(int:end}]] ]
//                    [ using {columnspec} ]
//                    [ title [ "{str:explanation}" | {int:col} ] | notitle ]
//                    [ with {stylespec} ]
//       [, ...]
void
CGnuPlot::
splotCmd(const std::string &args)
{
  CGnuPlotWindow *window;

  if (multiplot().enabled && ! windows_.empty())
    window = windows_.back();
  else {
    window = createWindow();

    windows_.push_back(window);

    window->set3D(true);
  }

  //----

  CParseLine line(args);

  std::string lastFilename;

  setFitX(true);
  setFitY(true);
  setFitZ(true);

  while (true) {
    // Get Range
    //  [XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]

    if (line.isChar('[')) {
      line.skipChar();

      std::string xrange, yrange, zrange;

      while (line.isValid() && ! line.isChar(']'))
        xrange += line.getChar();

      if (line.isValid()) {
        line.skipChar();

        if (line.isChar('[')) {
          line.skipChar();

          while (line.isValid() && ! line.isChar(']'))
            yrange += line.getChar();

          if (line.isValid()) {
            line.skipChar();

            if (line.isChar('[')) {
              line.skipChar();

              while (line.isValid() && ! line.isChar(']'))
                zrange += line.getChar();

              if (line.isValid())
                line.skipChar();
            }
          }
        }
      }

      if (isDebug())
        std::cerr << "Range [" << xrange << "][" << yrange << "][" << zrange << "]" << std::endl;

      std::vector<std::string> xfields, yfields, zfields;

      CStrUtil::addFields(xrange, xfields, ":");
      CStrUtil::addFields(yrange, yfields, ":");
      CStrUtil::addFields(zrange, zfields, ":");

      if (xfields.size() == 2) {
        double xmin = -10, xmax = 10;
        (void) CStrUtil::toReal(xfields[0], &xmin);
        (void) CStrUtil::toReal(xfields[1], &xmax);
        if (isDebug())
          std::cerr << "XRange=(" << xmin << "," << xmax << ")" << std::endl;
        axesData().xaxis.min = xmin;
        axesData().xaxis.max = xmax;
        setFitX(false);
      }

      if (yfields.size() == 2) {
        double ymin = -1, ymax = 1;
        (void) CStrUtil::toReal(yfields[0], &ymin);
        (void) CStrUtil::toReal(yfields[1], &ymax);
        if (isDebug())
          std::cerr << "YRange=(" << ymin << "," << ymax << ")" << std::endl;
        axesData().yaxis.min = ymin;
        axesData().yaxis.max = ymax;
        setFitY(false);
      }

      if (zfields.size() == 2) {
        double zmin = -1, zmax = 1;
        (void) CStrUtil::toReal(zfields[0], &zmin);
        (void) CStrUtil::toReal(zfields[1], &zmax);
        if (isDebug())
          std::cerr << "ZRange=(" << zmin << "," << zmax << ")" << std::endl;
        axesData().zaxis.min = zmin;
        axesData().zaxis.max = zmax;
        setFitZ(false);
      }

      line.skipSpace();
    }

    //----

    // Get filename or expression
    //  "<filename>" | <expression>

    PlotStyle style(PlotStyle::POINTS);

    std::string filename, function;

    if (line.isChar('\"')) {
      // filename
      line.skipChar();

      while (line.isValid()) {
        char c = line.getChar();

        if      (c == '\\') {
          if (line.isValid()) {
            char c1 = line.getChar();

            filename += c1;
          }
          else
            filename += c;
        }
        else if (c == '\"')
          break;
        else
          filename += c;
      }

      if (filename.empty())
        filename = lastFilename;

      if (isDebug())
        std::cerr << "Filename: " << filename << std::endl;

      lastFilename = filename;

      style = getDataStyle();
    }
    else {
      // function
      if (! parseFunction(line, function))
        return;

      if (isDebug())
        std::cerr << "Function: " << function << std::endl;

      style = getFunctionStyle();
    }

    line.skipSpace();

    //---

    UsingCols usingCols;
    Index     index;
    Every     every;

    setSmooth(Smooth::NONE);

    // Read individual plot:
    //   <plot_data> [, <plot_data> ...]
    // TODO: using must come before title and with
    while (line.isValid() && ! line.isChar(',')) {
      std::string option = readNonSpace(line);

      PlotVar plotVar;

      if (! CGnuPlotUtil::stringToValueWithErr(option, plotVar, "plot var"))
        break;

      line.skipSpace();

      // Read using columns
      // using <col1>[:<col2>[:<col3>[:...]]
      if      (plotVar == PlotVar::USING) {
        std::string              usingStr;
        std::vector<std::string> usingStrs;

        int rbrackets = 0;

        while (line.isValid()) {
          if (rbrackets == 0) {
            if (line.isSpace() || line.isChar(','))
              break;

            if (line.isChar(':')) {
              line.skipChar();

              usingStr = CStrUtil::stripSpaces(usingStr);

              if (! usingStr.empty())
                usingStrs.push_back(usingStr);

              usingStr = "";
            }
            else {
              if (line.isChar('('))
                ++rbrackets;

              if (rbrackets == 0)
                usingStr += line.getChar();
              else
                line.skipChar();
            }
          }
          else {
            if      (line.isChar('('))
              ++rbrackets;
            else if (line.isChar(')'))
              --rbrackets;

            if (rbrackets > 0)
              usingStr += line.getChar();
            else
              line.skipChar();
          }
        }

        usingStr = CStrUtil::stripSpaces(usingStr);

        if (! usingStr.empty())
          usingStrs.push_back(usingStr);

        if (isDebug()) {
          std::cerr << "using ";

          for (uint i = 0; i < usingStrs.size(); ++i) {
            if (i > 0) std::cerr << ":";

            std::cerr << usingStrs[i];
          }

          std::cerr << std::endl;
        }

        line.skipSpace();

        for (uint i = 0; i < usingStrs.size(); ++i)
          usingCols.push_back(UsingCol(usingStrs[i]));
      }
      // read index range
      // <start>[:<end>[:<step>]]
      else if (plotVar == PlotVar::INDEX) {
        std::string indexStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "index " << indexStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(indexStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          (void) CStrUtil::toInteger(inds[1], &index.end);
          (void) CStrUtil::toInteger(inds[2], &index.step);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          (void) CStrUtil::toInteger(inds[1], &index.end);
          index.step = 1;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &index.start);
          index.end  = index.start;
          index.step = 1;
        }
      }
      // read every range
      // <step>[:<start>[:<end>]]
      else if (plotVar == PlotVar::EVERY) {
        std::string everyStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "every " << everyStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(everyStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          (void) CStrUtil::toInteger(inds[1], &every.start);
          (void) CStrUtil::toInteger(inds[2], &every.end);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          (void) CStrUtil::toInteger(inds[1], &every.start);
          every.end = INT_MAX;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &every.step);
          every.start = 0;
          every.end   = INT_MAX;
        }
      }
      // with <style> { {linestyle | ls <line_style>} |
      //                {{linetype | lt <line_type>}
      //                 {linewidth | lw <line_width>}
      //                 {linecolor | lc <colorspec>}
      //                 {pointtype | pt <point_type>}
      //                 {pointsize | ps <point_size>}
      //                 {fill | fs <fillstyle>}
      //                 {nohidden3d} {nocontours} {nosurface}
      //                 {palette}}
      //              }
      // <style<[:<style>[...]]]
      else if (plotVar == PlotVar::WITH) {
        std::string withStr = readNonSpaceNonComma(line);

        if (CGnuPlotUtil::stringToValueWithErr(withStr, style, "plot style")) {
          if (isDebug())
            std::cerr << "with " << CGnuPlotUtil::valueToString(style) << std::endl;
        }

        line.skipSpace();

        bool modifiers = true;

        while (modifiers) {
          if      (line.isString("linestyle") || line.isString("ls")) {
            line.skipNonSpace();

            std::string styleStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(styleStr, &l))
              setLineStyleNum(COptInt(l));
          }
          else if (line.isString("linetype") || line.isString("lt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              lineStyle().setType(l);
          }
          else if (line.isString("linewidth") || line.isString("lw")) {
            double r;

            if (parseReal(line, r))
              lineStyle().setWidth(r);
          }
          else if (line.isString("linecolor") || line.isString("lc")) {
            CRGBA c;

            if (parseColorSpec(line, c))
              lineStyle().setColor(c);
          }
          else if (line.isString("pointtype") || line.isString("pt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              pointStyle().setType(static_cast<SymbolType>(l));
          }
          else if (line.isString("pointsize") || line.isString("ps")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);

            double s;

            if      (style == "variable" || style == "var")
              pointStyle().setVarSize(true);
            else if (CStrUtil::toReal(style, &s))
              pointStyle().setSize(s);
          }
          else if (line.isString("fill") || line.isString("fs")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);
          }
          else if (line.isString("nohidden3d")) {
            line.skipNonSpace();
          }
          else if (line.isString("nocontours")) {
            line.skipNonSpace();
          }
          else if (line.isString("nosurface")) {
            line.skipNonSpace();
          }
          else if (line.isString("palette")) {
            line.skipNonSpace();
          }
          else
            modifiers = false;

          line.skipSpace();
        }
      }
      // title string
      else if (plotVar == PlotVar::TITLE) {
        std::string titleStr;
        int         columnNum;

        if      (parseString(line, titleStr)) {
          if (isDebug())
            std::cerr << "title " << titleStr << std::endl;

          keyData_.title = titleStr;
        }
        else if (parseInteger(line, columnNum)) {
          keyData_.columnNum  = columnNum;
          keyData_.columnhead = true;
        }

        line.skipSpace();
      }
      // disable title
      else if (plotVar == PlotVar::NOTITLE) {
      }
      // smooth type
      else if (plotVar == PlotVar::SMOOTH) {
        std::string smoothStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "smooth " << smoothStr << std::endl;

        Smooth smooth;

        if (CGnuPlotUtil::stringToValueWithErr(smoothStr, smooth, "smooth type"))
          setSmooth(smooth);

        line.skipSpace();
      }
      // pointsize <size>
      else if (plotVar == PlotVar::POINTSIZE) {
        // TODO: just for this plot ?
        double r;

        if (parseReal(line, r))
          pointStyle().setSize(r > 0 ? r : 1.0);
      }
    }

    //---

    if      (! filename.empty()) {
      if  (usingCols.empty())
        usingCols.push_back(UsingCol(1));

      if      (usingCols.size() == 1) {
        usingCols.push_back(UsingCol(1));
        usingCols.push_back(UsingCol(0));

        std::swap(usingCols[0], usingCols[2]);
      }
      else if (usingCols.size() == 2) {
        usingCols.push_back(UsingCol(0));
      }

      (void) addFile3D(window, filename);
    }
    else if (! function.empty())
      (void) addFunction3D(window, function, style);
    else
      break;

    //---

    // move to next plot if any
    if (! line.isChar(','))
      break;

    line.skipChar();

    line.skipSpace();
  }

  window->setTitle(title ());

  window->setCamera(camera());

  window->setHidden3D(hidden3D());
  window->setSurface3D(surface3D());
  window->setContour3D(contour3D());
  window->setPm3D(pm3D());

  //---

  window->stateChanged(ChangeState::PLOT_ADDED);
}

// set ...
void
CGnuPlot::
setCmd(const std::string &args)
{
  CParseLine line(args);

  std::string option = readNonSpace(line);

  //---

  if (option.empty()) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  VariableName var;

  if (! CGnuPlotUtil::stringToValue(option, var))
    var = VariableName::NONE;

  // --- Configuration ---

  // set datafile separator ["<chars>"|whitespace]
  // set datafile commentschar ["<chars>"]
  // set datafile missing ["<chars>"]
  if      (var == VariableName::DATAFILE) {
    std::string varStr = readNonSpace(line);

    DataFileVar var1;

    if (! CGnuPlotUtil::stringToValueWithErr(varStr, var1, "data file type"))
      return;

    line.skipSpace();

    // set datafile separator ["<chars>"|whitespace]
    if      (var1 == DataFileVar::SEPARATOR) {
      if (line.isChar('"')) {
        std::string chars;

        if (! parseString(line, chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        if (chars.size() != 1) {
          std::cerr << "Only one character allowed for separator" << std::endl;
          return;
        }

        setSeparator(Separator(chars[0]));
      }
      else {
        std::string sepStr = readNonSpace(line);

        if (sepStr == "whitespace")
          setSeparator(Separator());
        else
          std::cerr << "Invalid separator type" << std::endl;
      }
    }
    // set datafile commentschar ["<chars>"]
    else if (var1 == DataFileVar::COMMENTS_CHAR) {
      if (line.isValid()) {
        std::string chars;

        if (! parseString(line, chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        setCommentChars(chars);
      }
      else
        setCommentChars("#");
    }
    // set datafile missing ["<chars>"]
    else if (var1 == DataFileVar::MISSING) {
      if (line.isValid()) {
        std::string chars;

        if (! parseString(line, chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        setMissingStr(chars);
      }
      else
        setMissingStr("");
    }
  }
  // set decimalsign [ "<char>" | locale [ "<locale>" ] ]
  else if (var == VariableName::DECIMALSIGN) {
  }
  // set encoding [ default | iso_8859_1 | iso_8859_2 | iso_8859_15 |
  //                koi8r | koi8u | cp437 | cp850 | cp852 | cp1250 ]
  else if (var == VariableName::ENCODING) {
  }
  // set fit [ logfile "<filename>" ] [ [no]errorvariables ]
  else if (var == VariableName::FIT) {
  }
  // set fontpath [ "<path>" ]
  else if (var == VariableName::FONTPATH) {
  }
  // set historysize <size:i>
  else if (var == VariableName::HISTORYSIZE) {
  }
  // set loadpath [ "<path>" ]
  else if (var == VariableName::LOADPATH) {
  }
  // set macros
  else if (var == VariableName::MACROS) {
  }
  // set mouse
  else if (var == VariableName::MOUSE) {
  }

  // --- Math ---

  // set angles [ radians | degrees ]
  else if (var == VariableName::ANGLES) {
    std::string type = readNonSpace(line);

    AngleType angleType;

    if (CGnuPlotUtil::stringToValueWithErr(type, angleType, "angle type"))
      setAngleType(angleType);
  }
  // set dummy <var1> [, <var2>]
  else if (var == VariableName::DUMMY) {
    std::string var1, var2;

    getDummyVars(var1, var2);

    if (line.isValid()) {
      var1 = readNonSpaceNonComma(line);

      if (line.isChar(',')) {
        line.skipChar();

        line.skipSpace();

        if (line.isValid()) {
          var2 = line.readNonSpace();
        }
      }
    }

    setDummyVars(var1, var2);
  }
  // set mapping [ cartesian | cylindrical | spherical ]
  else if (var == VariableName::MAPPING) {
  }
  // set parametric
  else if (var == VariableName::PARAMETRIC) {
  }
  // set polar
  else if (var == VariableName::POLAR) {
  }
  // set samples {samples1:i} [, {samples2:i} ]
  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    long n;

    std::string samples1, samples2;

    if (line.isValid()) {
      samples1 = readNonSpaceNonComma(line);

      if (CStrUtil::toInteger(samples1, &n))
        isamples1 = n;

      if (line.isChar(',')) {
        line.skipChar();

        line.skipSpace();

        if (line.isValid()) {
          samples2 = line.readNonSpace();

          if (CStrUtil::toInteger(samples2, &n))
            isamples2 = n;
        }
      }
    }

    setSamples(isamples1, isamples2);
  }
  // set zero {expression}
  else if (var == VariableName::ZERO) {
  }

  // set border {<integer>} {front | back} {linewidth | lw <line_width>}
  //            {{linestyle | ls <line_style>} | {linetype | lt <line_type>}}
  else if (var == VariableName::BORDER) {
    axesData().borders = 0xFF;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "front" || arg == "back") {
      }
      else if (arg == "linewidth" || arg == "lw") {
        double r;

        if (! parseReal(line, r))
          axesData().borderWidth = r;
      }
      else if (arg == "linestyle" || arg == "ls") {
        int i;

        if (! parseInteger(line, i))
          axesData().borderStyle = i;
      }
      else if (arg == "linetype" || arg == "lt") {
        int i;

        if (! parseInteger(line, i))
          axesData().borderType = i;
      }

      arg = readNonSpace(line);
    }
  }
  // set bmargin <val>
  else if (var == VariableName::BMARGIN) {
    double r = -1;

    (void) parseReal(line, r);

    margin_.setBottom(r);
  }
  // set lmargin <val>
  else if (var == VariableName::LMARGIN) {
    double r = -1;

    (void) parseReal(line, r);

    margin_.setLeft(r);
  }
  // set rmargin <val>
  else if (var == VariableName::RMARGIN) {
    double r = -1;

    (void) parseReal(line, r);

    margin_.setRight(r);
  }
  // set tmargin <val>
  else if (var == VariableName::TMARGIN) {
    double r = -1;

    (void) parseReal(line, r);

    margin_.setTop(r);
  }
  // set multiplot { title <page title> {font <fontspec>} {enhanced|noenhanced} }
  //               { layout <rows>,<cols> {rowsfirst|columnsfirst} {downwards|upwards}
  //                 {scale <xscale>{,<yscale>}} {offset <xoff>{,<yoff>}} }
  else if (var == VariableName::MULTIPLOT) {
    multiplot().enabled = true;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "title") {
        std::string titleStr; parseString(line, titleStr);
      }
      else if (arg == "font") {
        (void) readNonSpace(line);
      }
      else if (arg == "enhanced") {
      }
      else if (arg == "noenhanced") {
      }
      else if (arg == "layout") {
        std::string rowStr = readNonSpaceNonComma(line);

        CStrUtil::toInteger(rowStr, &multiplot().rows);

        line.skipSpace();

        if (line.isChar(',')) {
          line.skipChar();

          std::string colStr = readNonSpace(line);

          CStrUtil::toInteger(colStr, &multiplot().cols);
        }
      }
      else if (arg == "rowsfirst") {
      }
      else if (arg == "columnsfirst") {
      }
      else if (arg == "downwards") {
      }
      else if (arg == "upwards") {
      }
      else if (arg == "scale") {
        std::string xStr = readNonSpaceNonComma(line);
        std::string yStr = readNonSpace(line);
      }
      else if (arg == "offset") {
        std::string xStr = readNonSpaceNonComma(line);
        std::string yStr = readNonSpace(line);
      }

      arg = readNonSpace(line);
    }
  }

  // set logscale <axes> {<base>}
  else if (var == VariableName::LOGSCALE) {
    if (line.isValid()) {
      std::string axesStr = readNonSpace(line);
      line.skipSpace();

      if (line.isValid()) {
        std::string baseStr = readNonSpace(line);
        line.skipSpace();
      }
    }

    setLogScale(LogScale::X , 10);
    setLogScale(LogScale::Y , 10);
    setLogScale(LogScale::Z , 10);
    setLogScale(LogScale::X2, 10);
    setLogScale(LogScale::Y2, 10);
    setLogScale(LogScale::CB, 10);
  }

  // set origin <x-origin>,<y-origin>
  else if (var == VariableName::ORIGIN) {
    double r;

    if (parseReal(line, r))
      plotSize_.x = r;

    if (line.isChar(',')) {
      line.skipChar();

      if (parseReal(line, r))
        plotSize_.y = r;
    }
  }
  // set size {{no}square | ratio <r> | noratio} {<xscale>,<yscale>}
  else if (var == VariableName::SIZE) {
    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "square" || arg == "nosquare") {
        plotSize_.square = (arg == "square");
      }
      else if (arg == "ratio") {
        double r;

        if (parseReal(line, r))
          plotSize_.ratio = r;
      }
      else if (arg == "noratio") {
        plotSize_.ratio.setInvalid();
      }
      else if (CStrUtil::isReal(arg)) {
        double r;

        if (CStrUtil::toReal(arg, &r))
          plotSize_.xsize = r;

        line.skipSpace();

        if (line.isChar(',')) {
          line.skipChar();

          if (parseReal(line, r))
            plotSize_.ysize = r;
        }
        else
          plotSize_.ysize = r;
      }
      else {
        std::cerr << "Bad arg \'" << arg << "\'" << std::endl;
        break;
      }

      arg = readNonSpaceNonComma(line);
    }
  }

  // set arrow {<tag>} {from <position>} {to|rto <position>}
  //                   { {arrowstyle | as <arrow_style>} |
  //                     { {nohead | head | backhead | heads}
  //                       {size <length>,<angle>{,<backangle>}}
  //                       {filled | empty | nofilled}
  //                       {front | back}
  //                       { {linestyle | ls <line_style>} | {linetype | lt <line_type>}
  //                         {linewidth | lw <line_width} } } }
  else if (var == VariableName::ARROW) {
    int arrowId = -1;

    std::string arg = readNonSpace(line);

    if (CStrUtil::isInteger(arg)) {
      (void) CStrUtil::toInteger(arg, &arrowId);

      arg = readNonSpace(line);
    }

    Arrow arrow;

    if (arg == "from") {
      if (! parsePosition(line, arrow.from))
        return;

      arg = readNonSpace(line);

      if (arg == "to" || arg == "rto") {
        if (arg == "rto")
          arrow.relative = true;

        if (! parsePosition(line, arrow.to))
          return;

        arg = readNonSpace(line);
      }
    }

    line.skipSpace();

    while (arg != "") {
      if       (arg == "arrowstyle" || arg == "as") {
        // arrow style
        arg = readNonSpace(line);
      }
      else if  (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
        if      (arg == "nohead"  ) { arrow.fhead = false; arrow.thead = false; }
        else if (arg == "head"    ) { arrow.fhead = false; arrow.thead = true ; }
        else if (arg == "backhead") { arrow.fhead = true ; arrow.thead = false; }
        else if (arg == "heads"   ) { arrow.fhead = true ; arrow.thead = true ; }
      }
      else if (arg == "size") {
        arrow.length = -1;

        (void) parseReal(line, arrow.length);

        if (line.isChar(',')) {
          line.skipChar();

          arrow.angle = -1;

          (void) parseReal(line, arrow.angle);

          if (line.isChar(',')) {
            arrow.backAngle = -1;

            (void) parseReal(line, arrow.backAngle);
          }
        }
      }
      else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
        arrow.filled = (arg == "filled");
        arrow.empty  = (arg == "empty" );
      }
      else if (arg == "front" || arg == "back") {
        arrow.front = (arg == "front");
      }
      else if (arg == "linetype" || arg == "lt") {
        arg = readNonSpace(line);

        (void) CStrUtil::toInteger(arg, &arrow.lineType);
      }
      else if (arg == "linewidth" || arg == "lw") {
        (void) parseReal(line, arrow.lineWidth);
      }
      else if (arg == "linestyle" || arg == "ls") {
        arg = readNonSpace(line);

        int linestyle = -1;

        if (! CStrUtil::toInteger(arg, &linestyle))
          linestyle = -1;
      }
      else {
        std::cerr << "Invalid arg '" << arg << "'" << std::endl;
        break;
      }

      arg = readNonSpace(line);
    }

    if (arrowId == -1)
      arrow.ind = arrows_.size() + 1;

    arrows_.push_back(arrow);
  }
  // set key {on|off} {default}
  //         {{inside | outside} | {lmargin | rmargin | tmargin | bmargin} | {at <position>}}
  //         {left | right | center} {top | bottom | center}
  //         {vertical | horizontal} {Left | Right}
  //         {{no}opaque}
  //         {{no}reverse} {{no}invert}
  //         {samplen <sample_length>} {spacing <vertical_spacing>}
  //         {width <width_increment>}
  //         {height <height_increment>}
  //         {{no}autotitle {columnheader}}
  //         {title "<text>"} {{no}enhanced}
  //         {font "<face>,<size>"} {textcolor <colorspec>}
  //         {{no}box { {linestyle | ls <line_style>}
  else if (var == VariableName::KEY) {
    bool hSet = false;

    while (line.isValid()) {
      std::string arg = readNonSpace(line);

      if      (arg == "on"     ) { keyData_.displayed = true ; }
      else if (arg == "off"    ) { keyData_.displayed = false; }
      else if (arg == "default") { }
      else if (arg == "inside" ) { }
      else if (arg == "outside") { }
      else if (arg == "lmargin") { }
      else if (arg == "rmargin") { }
      else if (arg == "tmargin") { }
      else if (arg == "bmargin") { }
      else if (arg == "at"     ) {
        line.skipSpace(); std::string posStr = readNonSpace(line);
      }
      else if (arg == "left"  ) { keyData_.halign = CHALIGN_TYPE_LEFT  ; hSet = true; }
      else if (arg == "right" ) { keyData_.halign = CHALIGN_TYPE_RIGHT ; hSet = true; }
      else if (arg == "top"   ) { keyData_.valign = CVALIGN_TYPE_TOP   ; }
      else if (arg == "bottom") { keyData_.valign = CVALIGN_TYPE_BOTTOM; }

      else if (arg == "center") { if (hSet)
                                    keyData_.halign = CHALIGN_TYPE_CENTER;
                                  else
                                    keyData_.valign = CVALIGN_TYPE_CENTER;
                                }

      else if (arg == "vertical"  ) { }
      else if (arg == "horizontal") { }

      else if (arg == "Left" ) { }
      else if (arg == "Right") { }

      else if (arg == "opaque"   ) { }
      else if (arg == "noopaque" ) { }
      else if (arg == "reverse"  ) { keyData_.reverse = true ; }
      else if (arg == "noreverse") { keyData_.reverse = false; }
      else if (arg == "invert"   ) { keyData_.invert  = true ; }
      else if (arg == "noinvert" ) { keyData_.invert  = false; }

      else if (arg == "samplen") {
        double r;

        if (parseReal(line, r))
          keyData_.sampLen = r;
      }
      else if (arg == "spacing") {
        double r;

        if (parseReal(line, r))
          keyData_.spacing = r;
      }
      else if (arg == "width") {
        std::string incStr = readNonSpace(line);
      }
      else if (arg == "height") {
        std::string incStr = readNonSpace(line);
      }
      else if (arg == "autotitle") {
        std::string arg1 = readNonSpace(line);

        if (arg1 == "columnhead" || arg1 == "columnheader")
          keyData_.columnhead = true;
      }
      else if (arg == "noautotitle") {
        std::string headerStr = readNonSpace(line);
      }
      else if (arg == "title") {
        std::string titleStr; parseString(line, titleStr);
      }
      else if (arg == "enhanced") { }
      else if (arg == "noenhanced") { }
      else if (arg == "font") {
        std::string fontStr; parseString(line, fontStr);
      }
      else if (arg == "textcolor") {
        std::string colorStr = readNonSpace(line);
      }
      else if (arg == "box") {
        keyData_.box = true;

        line.skipSpace();

        if (line.isString("linestyle") || line.isString("ls")) {
          line.skipNonSpace();
          std::string styleStr = readNonSpace(line);
        }
      }
      else if (arg == "nobox") {
        keyData_.box = false;

        if (line.isString("linestyle") || line.isString("ls")) {
          line.skipNonSpace();
          std::string styleStr = readNonSpace(line);
        }
      }
      else {
        std::cerr << "Invalid key arg \"" << arg << "\"" << std::endl;
      }

      line.skipSpace();
    }
  }
  // set label {<tag>} {"<label text>"} {at <position>}
  //           {left | center | right}
  //           {norotate | rotate {by <degrees>}}
  //           {font "<name>{,<size>}"}
  //           {noenhanced}
  //           {front | back}
  //           {textcolor <colorspec>}
  //           {point <pointstyle> | nopoint}
  //           {offset <offset>}
  else if (var == VariableName::LABEL) {
    int labelId = -1;

    line.skipSpace();

    if (isdigit(line.lookChar())) {
      std::string arg = readNonSpace(line);

      (void) CStrUtil::toInteger(arg, &labelId);

      line.skipSpace();
    }

    Label label;

    if (line.isChar('"') || line.isChar('\''))
      (void) parseString(line, label.text);

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "at") {
        if (! parsePosition(line, label.pos))
          return;
      }
      else if (arg == "left" || "center" || arg == "right") {
        label.align = (arg != "left" ?
          (arg != "center" ? CHALIGN_TYPE_RIGHT : CHALIGN_TYPE_CENTER) : CHALIGN_TYPE_LEFT);
      }
      else if (arg == "rotate") {
        line.skipSpace();

        if (line.isString("by")) {
          arg = readNonSpace(line);

          arg = readNonSpace(line);

          (void) CStrUtil::toReal(arg, &label.angle);
        }
        else
          label.angle = 90;
      }
      else if (arg == "norotate") {
        label.angle = 0;
      }
      else if (arg == "font") {
        (void) parseString(line, label.font);
      }
      else if (arg == "noenhanced") {
      }
      else if (arg == "front" || arg == "back") {
        label.front = (arg == "front");
      }
      else if (arg == "textcolor") {
        arg = readNonSpace(line);
      }
      else if (arg == "point") {
        arg = readNonSpace(line);
      }
      else if (arg == "nopoint") {
      }
      else if (arg == "offset") {
        (void) parseReal(line, label.offset);
      }
      else {
        std::cerr << "Invalid arg '" << arg << "'" << std::endl;
        break;
      }

      arg = readNonSpace(line);
    }

    if (labelId == -1)
      label.ind = labels_.size() + 1;

    labels_.push_back(label);
  }
  // set object <index> <object-type> <object-properties>
  //            {front|back|behind} {fc|fillcolor <colorspec>} {fs <fillstyle>}
  //            {default} {lw|linewidth <width>}
  else if (var == VariableName::OBJECT) {
    int ind = -1;

    (void) parseInteger(line, ind);

    std::string arg = readNonSpace(line);

    ObjectType type;

    if (CStrUniqueMatch<ObjectType>({{"circle"   , ObjectType::CIRCLE   },
                                     {"ellipse"  , ObjectType::ELLIPSE  },
                                     {"polygon"  , ObjectType::POLYGON  },
                                     {"rectangle", ObjectType::RECTANGLE}}).match(arg, type)) {
      // set object <index> circle {at|center} <position> size <radius>
      //            {arc [<begin>:<end>]}
      //            {<other-object-properties>}
      if      (type == ObjectType::CIRCLE) {
        Ellipse ellipse;
        CSize2D size(1,1);

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "at" || arg == "circle") {
            if (! parsePosition(line, ellipse.p))
              continue;
          }
          else if (arg == "size") {
            arg = readNonSpace(line);

            double s;

            if (CStrUtil::toReal(arg, &s)) {
              ellipse.rx = s;
              ellipse.ry = s;
            }
          }
          else if (arg == "arc") {
            arg = readNonSpace(line);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            (void) parseColorSpec(line, ellipse.fillColor);
          }

          arg = readNonSpace(line);
        }

        ellipses_.push_back(ellipse);
      }
      // set object <index> ellipse {at|center} <position> size <w>,<h>
      //            {angle <orientation>} {units xy|xx|yy}
      //            {<other-object-properties>}
      else if (type == ObjectType::ELLIPSE) {
        Ellipse ellipse;
        CSize2D size(1,1);

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "at" || arg == "center") {
            if (! parsePosition(line, ellipse.p))
              return;
          }
          else if (arg == "size") {
            if (! parseSize(line, size))
              continue;

            ellipse.rx = size.width /2.0;
            ellipse.ry = size.height/2.0;
          }
          else if (arg == "angle") {
            arg = readNonSpace(line);
          }
          else if (arg == "units") {
            arg = readNonSpace(line);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            (void) parseColorSpec(line, ellipse.fillColor);
          }

          arg = readNonSpace(line);
        }

        ellipses_.push_back(ellipse);
      }
      // set object <index> polygon
      //            from <position> to <position> ... {to <position>} |
      //            from <position> rto <position> ... {rto <position>}
      else if (type == ObjectType::POLYGON) {
        Polygon poly;

        Position from, to;

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "from") {
            if (! parsePosition(line, from))
              return;

            poly.points.push_back(from.p);
          }
          else if (arg == "to") {
            if (! parsePosition(line, to))
              return;

            poly.points.push_back(to.p);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            (void) parseColorSpec(line, poly.fillColor);
          }

          arg = readNonSpace(line);
        }

        polygons_.push_back(poly);
      }
      // set object <index> rectangle
      //            {from <position> {to|rto} <position> |
      //            center <position> size <w>,<h> |
      //            at <position> size <w>,<h>}
      else if (type == ObjectType::RECTANGLE) {
        Rectangle rect;

        COptValT<Position> from, to, rto;
        COptValT<Position> center;
        COptValT<CSize2D>  size;

        Position p;

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "from" || arg == "at") {
            if (parsePosition(line, p))
              from = p;
          }
          else if (arg == "to") {
            if (parsePosition(line, p))
              to = p;
          }
          else if (arg == "rto") {
            if (parsePosition(line, p))
              rto = p;
          }
          else if (arg == "center") {
            if (parsePosition(line, p))
              center = p;
          }
          else if (arg == "size") {
            CSize2D size1;

            if (parseSize(line, size1))
              size = size1;
          }
          else if (arg == "fillcolor" || arg == "fc") {
            (void) parseColorSpec(line, rect.fillColor);
          }

          arg = readNonSpace(line);
        }

        CBBox2D bbox(0,0,1,1);

        if      (from.isValid()) {
          if      (to.isValid())
            bbox = CBBox2D(from.getValue().p, to.getValue().p);
          else if (rto.isValid())
            bbox = CBBox2D(from.getValue().p, from.getValue().p + rto.getValue().p);
          else if (size.isValid()) {
            CPoint2D s(size.getValue().width, size.getValue().height);

            bbox = CBBox2D(from.getValue().p, from.getValue().p + s);
          }
        }
        else if (center.isValid()) {
          if (size.isValid()) {
            CPoint2D s(size.getValue().width, size.getValue().height);

            bbox = CBBox2D(center.getValue().p - s/2, center.getValue().p + s/2);
          }
        }

        rect.from = bbox.getMin();
        rect.to   = bbox.getMax();

        rects_.push_back(rect);
      }
    }
    else
      std::cerr << "Invalid object type" << std::endl;
  }
  // set title "<title>"
  else if (var == VariableName::TITLE) {
    std::string titleStr;

    if (parseString(line, titleStr))
      title().str = titleStr;
    else
      std::cerr << "Invalid title string" << std::endl;
  }

  // set style [data|function|increment] <args>
  else if (var == VariableName::STYLE) {
    std::string var1Str = readNonSpace(line);

    StyleVar var1;

    if (! CGnuPlotUtil::stringToValueWithErr(var1Str, var1, "style type"))
      return;

    line.skipSpace();

    // set style data {style}
    if      (var1 == StyleVar::DATA) {
      std::string styleStr = readNonSpace(line);

      PlotStyle style(PlotStyle::NONE);

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, style, "plot style"))
        setDataStyle(style);
    }
    // set style function {style}
    else if (var1 == StyleVar::FUNCTION) {
      std::string styleStr = readNonSpace(line);

      PlotStyle style(PlotStyle::NONE);

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, style, "plot style"))
        setFunctionStyle(style);
    }
    // set style increment { user |  default }
    else if (var1 == StyleVar::INCREMENT) {
      std::string styleStr = readNonSpace(line);

      if (styleStr == "user") {
        setStyleIncrementType(StyleIncrementType::USER);

        setLineStyleNum(COptInt(1));
      }
      else {
        setStyleIncrementType(StyleIncrementType::DEFAULT);

        resetLineStyleNum();
      }
    }
    // set style arrow {index:i} default
    // set style arrow {index:i} [ nohead | head | backhead | heads ]
    //                           [ size {length:f} [,{angle:f}] [,{backangle:f}] ]
    //                           [ filled | empty | nofilled ]
    //                           [ front | back ]
    //                           [ [ linetype | lt {type:i} ]
    //                             [ linewidth | lw {type:i} ]
    //                             [ [ linestyle | ls {style:i} ] ]
    else if (var1 == StyleVar::ARROW) {
    }
    // set style fill [ empty | solid [{density:r}] | pattern [{id:i}]]
    //                [ border [ {linetype:i} ] | noborder ]
    else if (var1 == StyleVar::FILL) {
      std::string styleStr = readNonSpace(line);

      FillType fillType;

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, fillType, "fill type"))
        fillStyle().setStyle(fillType);

      line.skipSpace();

      if      (fillType == FillType::SOLID) {
        if (! line.isString("border") && ! line.isString("noborder")) {
          double density = 1.0;

          std::string arg1 = readNonSpace(line);

          double r;

          if (CStrUtil::toReal(arg1, &r))
            density = r;

          fillStyle().setDensity(density);

          line.skipSpace();
        }
      }
      else if (fillType == FillType::PATTERN) {
        if (! line.isString("border") && ! line.isString("noborder")) {
          int patternNum = 0;

          std::string arg1 = readNonSpace(line);

          long l;

          if (CStrUtil::toInteger(arg1, &l))
            patternNum = l;

          fillStyle().setPattern((PatternType) patternNum);

          line.skipSpace();
        }
      }

      if (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "border") {
          int fillBorderLine = -1;

          line.skipSpace();

          if (line.isValid()) {
            std::string arg1 = readNonSpace(line);

            long i;

            if (CStrUtil::toInteger(arg1, &i))
              fillBorderLine = i;
          }

          fillStyle().setBorder(true);
          fillStyle().setBorderLine(fillBorderLine);
        }
        else if (arg == "noborder")
          fillStyle().setBorder(false);
        else
          std::cerr << "Invalid fill border." << std::endl;
      }
    }
    // set style histogram [ clustered [ gap {gap:f} ] |
    //                       errorbars [ gap {gap:f} ] [ linewidth | lw {width:i} ] |
    //                       rowstacked |
    //                       columnstacked
    else if (var1 == StyleVar::HISTOGRAM) {
      std::string styleStr = readNonSpace(line);

      HistogramStyle histogramStyle;

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, histogramStyle, "histogram style"))
        setHistogramStyle(histogramStyle);

      if (histogramStyle == HistogramStyle::CLUSTERED ||
          histogramStyle == HistogramStyle::ERRORBARS) {
        std::string gapStr = readNonSpace(line);

        if (gapStr == "gap") {
          std::string gapValue = readNonSpace(line);

          double r;

          if (! CStrUtil::toReal(gapValue, &r))
            r = 0.0;

          std::string lwStr = readNonSpace(line);

          if (lwStr == "linewidth" || lwStr == "lw") {
            std::string widthValue = readNonSpace(line);

            long i;

            if (! CStrUtil::toInteger(widthValue, &i))
              i = 0;
          }
        }
      }
    }
    // set style line {index:i} [ [ linetype  | lt ] {linetype:i} ]
    //                            [ linewidth | lw ] {linewidth:f} ]
    //                            [ pointtype | pt ] {pointtype:i} ]
    //                            [ pointsize | ps ] {pointsize:f} ]
    //                            [ linecolor | lc ] {colorspace:c} ]
    else if (var1 == StyleVar::LINE) {
      std::string indStr = readNonSpace(line);

      long l;

      int ind = 0;

      if (CStrUtil::toInteger(indStr, &l))
        ind = l;

      line.skipSpace();

      LineStyle  lineStyle;
      PointStyle pointStyle;
      double     r;

      while (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "linetype" || arg == "lt") {
          std::string typeStr = readNonSpace(line);

          if (CStrUtil::toInteger(typeStr, &l))
            lineStyle.setType(l);
        }
        else if (arg == "linewidth" || arg == "lw") {
          if (parseReal(line, r))
            lineStyle.setWidth(r);
        }
        else if (arg == "pointtype" || arg == "pt") {
          std::string typeStr = readNonSpace(line);

          if (CStrUtil::toInteger(typeStr, &l))
            pointStyle.setType(static_cast<SymbolType>(l));
        }
        else if (arg == "pointsize" || arg == "ps") {
          if (parseReal(line, r))
            pointStyle.setSize(r);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CRGBA c;

          if (parseColorSpec(line, c))
            lineStyle.setColor(c);
        }
        else {
          std::cerr << "Invalid line modifier '" << arg << "'" << std::endl;
          break;
        }

        line.skipSpace();
      }

      lineStyles_[ind] = lineStyle;

      line.skipSpace();
    }
    // set style rectangle
    else if (var1 == StyleVar::RECTANGLE) {
    }
    else if (var1 == StyleVar::CIRCLE) {
    }
    else if (var1 == StyleVar::ELLIPSE) {
    }
  }

  // set bars [ small | large | fullwidth | {mult:f} ]
  else if (var == VariableName::BARS) {
    std::string sizeStr = readNonSpace(line);

    double size;

    if      (sizeStr == "small"    ) size = -3.0;
    else if (sizeStr == "large"    ) size = -2.0;
    else if (sizeStr == "fullwidth") size = -1.0;

    double r = 1.0;

    if (CStrUtil::toReal(sizeStr, &r))
      size = (r > 0 ? r : 1.0);

    std::cerr << "setBarSize(" << size << ");" << std::endl;
  }
  // boxwidth [ {size:r} ] [ absolute | relative ]
  else if (var == VariableName::BOXWIDTH) {
    BoxWidthType boxWidthType = BoxWidthType::AUTO;
    double       boxWidth     = getBoxWidth();

    if (line.isValid()) {
      std::string sizeStr = readNonSpace(line);

      double r;

      if (CStrUtil::toReal(sizeStr, &r)) {
        boxWidth = (r > 0 ? r : 1.0);

        line.skipSpace();

        if (line.isValid()) {
          std::string arg = readNonSpace(line);

          if      (arg == "absolute")
            boxWidthType = BoxWidthType::ABSOLUTE;
          else if (arg == "relative")
            boxWidthType = BoxWidthType::RELATIVE;
        }
      }
    }

    setBoxWidth(boxWidth, boxWidthType);
  }
  // set pointsize {mult:r}
  else if (var == VariableName::POINTSIZE) {
    std::string sizeStr = readNonSpace(line);

    double r = 1.0;

    if (CStrUtil::toReal(sizeStr, &r))
      pointStyle().setSize(r > 0 ? r : 1.0);
  }

  // set grid {{no}{m}xtics} {{no}{m}ytics} {{no}{m}ztics}
  //          {{no}{m}x2tics} {{no}{m}y2tics}
  //          {{no}{m}cbtics}
  //          {polar {<angle>}}
  //          {layerdefault | front | back}
  //          { {linestyle <major_linestyle>} | {linetype | lt <major_linetype>}
  //            {linewidth | lw <major_linewidth>}
  //            { , {linestyle | ls <minor_linestyle>} | {linetype | lt <minor_linetype>}
  //            {linewidth | lw <minor_linewidth>} } }
  else if (var == VariableName::GRID) {
    axesData().xaxis.grid = true;
    axesData().yaxis.grid = true;
  }
  // set xlabel "<label>"
  else if (var == VariableName::XLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr))
      axesData().xaxis.str = labelStr;
    else
      std::cerr << "Invalid xlabel string" << std::endl;
  }
  // set ylabel "<label>"
  else if (var == VariableName::YLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr))
      axesData().yaxis.str = labelStr;
    else
      std::cerr << "Invalid ylabel string" << std::endl;
  }
  // set xrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} } | restore
  else if (var == VariableName::XRANGE) {
    line.skipSpace();

    if (line.isChar('[')) {
      line.skipChar();

      std::string rangeStr;

      while (line.isValid() && ! line.isChar(']'))
        rangeStr += line.getChar();

      if (line.isValid())
        line.skipChar();

      std::vector<std::string> fields;

      CStrUtil::addFields(rangeStr, fields, ":");

      if (fields.size() == 2) {
        double xmin = -10, xmax = 10;
        (void) CStrUtil::toReal(fields[0], &xmin);
        (void) CStrUtil::toReal(fields[1], &xmax);
        axesData().xaxis.min = xmin;
        axesData().xaxis.max = xmax;
      }
    }
  }
  // set yrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} } | restore
  else if (var == VariableName::YRANGE) {
    line.skipSpace();

    if (line.isChar('[')) {
      line.skipChar();

      std::string rangeStr;

      while (line.isValid() && ! line.isChar(']'))
        rangeStr += line.getChar();

      if (line.isValid())
        line.skipChar();

      std::vector<std::string> fields;

      CStrUtil::addFields(rangeStr, fields, ":");

      if (fields.size() == 2) {
        double ymin = -1, ymax = 1;
        (void) CStrUtil::toReal(fields[0], &ymin);
        (void) CStrUtil::toReal(fields[1], &ymax);
        axesData().yaxis.min = ymin;
        axesData().yaxis.max = ymax;
      }
    }
  }
  // set tics {axis | border} {{no}mirror}
  //          {in | out} {scale {default | <major> {,<minor>}}}
  //          {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //          {left | right | center | autojustify}
  //          { format "formatstring" } { font "name{,<size>}" }
  //          { textcolor <colorspec> }
  else if (var == VariableName::TICS) {
  }
  // set xtics {axis | border} {{no}mirror}
  //           {in | out} {scale {default | <major> {,<minor>}}}
  //           {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //           {left | right | center | autojustify}
  //           {add}
  //           { autofreq | <incr> | <start>, <incr> {,<end>} |
  //             ({"<label>"} <pos> {<level>} {,{"<label>"}...) }
  //           { format "formatstring" } { font "name{,<size>}" }
  //           { rangelimited }
  //           { textcolor <colorspec> }
  else if (var == VariableName::XTICS) {
    axesData().xaxis.showTics = true;
  }
  else if (var == VariableName::YTICS) {
    axesData().yaxis.showTics = true;
  }

  // set hidden3d {defaults} |
  //              { {front | back}
  //                {{offset <offset>} | {nooffset}}
  //                {trianglepattern <bitpattern>}
  //                {{undefined <level>} | {noundefined}}
  //                {{no}altdiagonal}
  //                {{no}bentover} }
  else if (var == VariableName::HIDDEN3D) {
    std::string arg = readNonSpace(line);

    if (arg == "trianglepattern") {
      arg = readNonSpace(line);

      int n;

      if (CStrUtil::toInteger(arg, &n))
        setTrianglePattern3D(n);
    }
    else
      setHidden3D(true);
  }
  // set isosamples <iso_1> {,<iso_2>}
  else if (var == VariableName::ISOSAMPLES) {
    int isamples1, isamples2;

    getIsoSamples(isamples1, isamples2);

    long n;

    std::string samples1, samples2;

    if (line.isValid()) {
      samples1 = readNonSpaceNonComma(line);

      if (CStrUtil::toInteger(samples1, &n)) {
        isamples1 = n;
        isamples2 = n;
      }

      if (line.isChar(',')) {
        line.skipChar(); line.skipSpace();

        if (line.isValid()) {
          samples2 = line.readNonSpace();

          if (CStrUtil::toInteger(samples2, &n))
            isamples2 = n;
        }
      }
    }

    setIsoSamples(isamples1, isamples2);
  }
  // set surface
  else if (var == VariableName::SURFACE) {
    setSurface3D(true);
  }
  // set view <rot_x>{,{<rot_z>}{,{<scale>}{,<scale_z>}}}
  // set view map
  // set view {no}equal {xy|xyz}
  else if (var == VariableName::VIEW) {
    std::string arg = readNonSpaceNonComma(line);

    if (arg == "map") {
      camera_.enabled = false;

      camera_.rotateX = 0.0;
      camera_.rotateY = 0.0;
      camera_.rotateZ = 0.0;

      camera_.init();
    }
  }
  // set xyplane at <zvalue>
  // set xyplane relative <frac>
  else if (var == VariableName::XYPLANE) {
  }

  // set cntrparam { { linear | cubicspline | bspline | points <n> | order <n> |
  //                   levels { auto {<n>} | <n> | discrete <z1> {,<z2>{,<z3>...}} |
  //                            incremental <start>, <incr> {,<end>} } } }
  else if (var == VariableName::CNTRPARAM) {
  }
  // set contour {base | surface | both}
  else if (var == VariableName::CONTOUR) {
    setContour3D(true);
  }

  // set palette { { gray | color }
  //               { gamma <gamma> }
  //               { rgbformulae <r>,<g>,<b> |
  //                 defined { ( <gray1> <color1> {, <grayN> <colorN>}... ) } |
  //                 file '<filename>' {datafile-modifiers} |
  //                 functions <R>,<G>,<B> }
  //               { cubehelix {start <val>} {cycles <val>} {saturation <val>} }
  //               { model { RGB | HSV | CMY | YIQ | XYZ } }
  //               { positive | negative }
  //               { nops_allcF | ps_allcF }
  //               { maxcolors <maxcolors> } }
  else if (var == VariableName::PALETTE) {
    palette_.colorModel = ColorModel::RGB;

    palette_.colors.clear();

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if (arg == "model") {
        arg = readNonSpace(line);

        if      (arg == "RGB") palette_.colorModel = ColorModel::RGB;
        else if (arg == "HSV") palette_.colorModel = ColorModel::HSV;
        else if (arg == "CMY") palette_.colorModel = ColorModel::CMY;
        else if (arg == "YIQ") palette_.colorModel = ColorModel::YIQ;
        else if (arg == "XYZ") palette_.colorModel = ColorModel::XYZ;
      }
      else if (arg == "defined") {
        line.skipSpace();

        if (line.isChar('(')) {
          line.skipChar();

          line.skipSpace();

          while (line.isValid() && ! line.isChar(')')) {
            arg = readNonSpace(line);

            double gray = 0.0;

            (void) CStrUtil::toReal(arg, &gray);

            CRGBA c;

            parseColor(line, c);

            palette_.addColor(gray, c);

            line.skipSpace();

            if (line.isChar(','))
              line.skipChar();
          }
        }
      }

      arg = readNonSpace(line);
    }
  }
  // set pm3d { { at <position> }
  //            { interpolate <steps/points in scan, between scans> }
  //            { scansautomatic | scansforward | scansbackward | depthorder }
  //            { flush { begin | center | end } }
  //            { ftriangles | noftriangles }
  //            { clip1in | clip4in }
  //            { corners2color { mean|geomean|median|min|max|c1|c2|c3|c4 } }
  //            { hidden3d {<linestyle>} | nohidden3d }
  //            { implicit | explicit }
  //            { map } }
  else if (var == VariableName::PM3D) {
    setPm3D(true);
  }

  // set output ["<filename>"]
  else if (var == VariableName::OUTPUT) {
    std::string filename;

    if (! parseString(line, filename)) {
      std::cerr << "Invalid filename" << std::endl;
      return;
    }

    setOutputFile(filename);
  }
  // set print ["<filename>"] [append]
  else if (var == VariableName::PRINT) {
    std::string filename;

    if (! parseString(line, filename)) {
      std::cerr << "Invalid filename" << std::endl;
      return;
    }

    setPrintFile(filename);

    std::string arg = readNonSpace(line);

    if (arg == "append")
      setPrintAppend(true);
  }
  // set table ["<filename>"]
  else if (var == VariableName::TABLE) {
    std::string filename;

    if (! parseString(line, filename)) {
      std::cerr << "Invalid filename" << std::endl;
      return;
    }

    setTableFile(filename);
  }
  // set terminal <type> [<args>]
  else if (var == VariableName::TERMINAL) {
    std::cerr << "Unhandled option '" << option << "'" << std::endl;
  }
  // set termoption [no]enhanced
  // set termoption font "<name> [, <size:i> ]"
  else if (var == VariableName::TERMOPTION) {
    std::cerr << "Unhandled option '" << option << "'" << std::endl;
  }
  else if (var == VariableName::DEBUG) {
    setDebug(true);
  }
  else if (var == VariableName::EDEBUG) {
    setExprDebug(true);
  }
  else {
    std::cerr << "Unhandled option '" << option << "'" << std::endl;
  }
}

// show ...
// show plot
// show [ variables [all] | functions ]
// show [ all | version ] [ long ]
void
CGnuPlot::
showCmd(const std::string &args)
{
  CParseLine line(args);

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  if (args1.size() == 0) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  //---

  VariableName var;

  if (! CGnuPlotUtil::stringToValue(args1[0], var))
    var = VariableName::NONE;

  if      (var == VariableName::VARIABLES) {
    std::vector<std::string> names;

    CExprInst->getVariableNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprVariablePtr var = CExprInst->getVariable(names[i]);

      std::cout << var->name() << " = " << var->value() << std::endl;
    }
  }
  else if (var == VariableName::FUNCTIONS) {
    std::vector<std::string> names;

    CExprInst->getFunctionNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprFunctionPtr function = CExprInst->getFunction(names[i]);

      if (! function->isBuiltin())
        std::cout << function << std::endl;
    }
  }
  else if (var == VariableName::VERSION) {
    // TODO: long
    std::cout << "CGnuPlot Version 0.1" << std::endl;
  }
  else if (var == VariableName::DATAFILE) {
    std::map<std::string,bool> show;

    if (args1.size() > 1)
      show[args1[1]] = true;

    if (show.empty() || show.find("missing") != show.end()) {
      if (getMissingStr().empty())
        std::cerr << "No missing data string set for datafile" << std::endl;
      else
        std::cerr << "\"" << getMissingStr() <<
                     "\" in datafile is interpreted as missing value" << std::endl;
    }
    if (show.empty() || show.find("separator") != show.end()) {
      if (getSeparator().type == SeparatorType::WHITESPACE)
        std::cerr << "datafile fields separated by whitespace" << std::endl;
     else
        std::cerr << "datafile fields separated by \"" << getSeparator().c << "\"" << std::endl;
    }
    if (show.empty() || show.find("commentschar") != show.end()) {
      std::cerr << "Comments chars are \"" << getCommentChars() << "\"" << std::endl;
    }
  }
  else if (var == VariableName::TITLE) {
    std::cerr << "title is \"" << title_.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  else if (var == VariableName::ARROW) {
    for (const auto &arrow : arrows_) {
       std::cerr << "arrow " << arrow.ind << "," <<
                    (arrow.fhead ? (arrow.thead ? " heads" : " backhead") :
                                   (arrow.thead ? " head"  : " nohead"  )) <<
                    (! arrow.filled ? (! arrow.empty ? " nofilled" : " empty") : " filled") <<
                    (arrow.front ? " front" : " back") <<
                    " linetype " << arrow.lineType <<
                    " linewidth " << arrow.lineWidth <<
                    " from " << arrow.from.p <<
                    (arrow.relative ? " rto " : " to ") << arrow.to.p;

       if (arrow.length > 0.0 || arrow.angle >= 0.0 || arrow.backAngle >= 0.0) {
         std::cerr << " arrow head:";
         if (arrow.length    >  0.0) std::cerr << " length " << arrow.length << " deg";
         if (arrow.angle     >= 0.0) std::cerr << " angle " << arrow.angle << " deg";
         if (arrow.backAngle >= 0.0) std::cerr << " backangle " << arrow.backAngle << " deg";
       }

       std::cerr << std::endl;
    }
  }
  else if (var == VariableName::LABEL) {
    for (const auto &label : labels_) {
       std::cerr << "label " << label.ind << " \"" << label.text << "\"" <<
                    " at " << label.pos.p;

       std::cerr << std::endl;
    }
  }

  else if (var == VariableName::STYLE) {
    std::map<std::string,bool> show;

    if (args1.size() > 1)
      show[args1[1]] = true;

    if (show.empty() || show.find("data") != show.end()) {
      std::cerr << "Data are plotted with " <<
        CGnuPlotUtil::valueToString<PlotStyle>(getDataStyle()) << std::endl;
    }
  }

  else if (var == VariableName::XLABEL) {
    std::cerr << "xlabel is \"" << axesData().xaxis.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  else if (var == VariableName::YLABEL) {
    std::cerr << "ylabel is \"" << axesData().yaxis.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }

  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    std::cerr << "sampling rate is " << isamples1 << ", " << isamples2 << std::endl;
  }
  else if (var == VariableName::ANGLES) {
    std::cerr << "Angles are in " <<
      CGnuPlotUtil::valueToString<AngleType>(getAngleType()) << std::endl;
  }
  else if (var == VariableName::POINTSIZE) {
    std::cerr << "pointsize is " << pointStyle().size() << std::endl;
  }
  else if (var == VariableName::OUTPUT) {
    std::cerr << "output is sent to " <<
      (! getOutputFile().empty() ? ("'" + getOutputFile() + "'"): "STDOUT") << std::endl;
  }
  else if (var == VariableName::PRINT) {
    std::cerr << "print output is set to '" <<
      (! getPrintFile().empty() ? getPrintFile() : "<stderr>") << "'" << std::endl;
  }
  else {
    std::cerr << "Unhandled option name '" << args1[0] << "'" << std::endl;
  }
}

// reset
void
CGnuPlot::
resetCmd(const std::string &)
{
  // TODO: do not reset terminal and output
}

// unset ...
void
CGnuPlot::
unsetCmd(const std::string &args)
{
  CParseLine line(args);

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  if (args1.size() == 0) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  //---

  VariableName var;

  if (! CGnuPlotUtil::stringToValue(args1[0], var))
    var = VariableName::NONE;

  if      (var == VariableName::MULTIPLOT)
    multiplot().enabled = false;
  else if (var == VariableName::BORDER)
    axesData().borders = 0;
  else if (var == VariableName::ARROW)
    arrows_.clear();
  else if (var == VariableName::KEY)
    keyData_.displayed = false;
  else if (var == VariableName::TITLE)
    title().str = "";
  else if (var == VariableName::LABEL)
    labels_.clear();
  else if (var == VariableName::GRID) {
    axesData().xaxis.grid = false;
    axesData().yaxis.grid = false;
  }
  else if (var == VariableName::XLABEL)
    axesData().xaxis.str = "";
  else if (var == VariableName::YLABEL)
    axesData().yaxis.str = "";
  else if (var == VariableName::TICS) {
  }
  else if (var == VariableName::XTICS)
    axesData().xaxis.showTics = false;
  else if (var == VariableName::YTICS)
    axesData().yaxis.showTics = false;
  else if (var == VariableName::HIDDEN3D)
    setHidden3D(false);
  else if (var == VariableName::SURFACE)
    setSurface3D(false);
  else if (var == VariableName::CONTOUR)
    setContour3D(false);
  else if (var == VariableName::PM3D)
    setPm3D(false);
  else if (var == VariableName::TABLE)
    setTableFile("");
  else if (var == VariableName::DEBUG)
    setDebug(false);
  else if (var == VariableName::EDEBUG)
    CExprInst->setDebug(false);
  else
    std::cerr << "Unhandled option name '" << args1[0] << "'" << std::endl;
}

// shell
void
CGnuPlot::
shellCmd(const std::string &)
{
}

// system
void
CGnuPlot::
systemCmd(const std::string &args)
{
  CParseLine line(args);

  std::string str;

  if (parseString(line, str))
    system(str.c_str());
}

// clear
void
CGnuPlot::
clearCmd(const std::string &)
{
  double x = plotSize_.x    .getValue(0.0);
  double y = plotSize_.y    .getValue(0.0);
  double w = plotSize_.xsize.getValue(1.0);
  double h = plotSize_.ysize.getValue(1.0);

  clearRect_ = CBBox2D(x, y, x + w, y + h);
}

// lower {winid:i}
void
CGnuPlot::
lowerCmd(const std::string &)
{
}

// raise {winid:i}
void
CGnuPlot::
raiseCmd(const std::string &)
{
}

// test
// test palette
void
CGnuPlot::
testCmd(const std::string &args)
{
  CParseLine line(args);

  std::string arg = readNonSpace(line);

  CGnuPlotWindow *window = createWindow();

  windows_.push_back(window);

  CGnuPlotPlot *plot = createPlot(window);

  if (arg == "palette") {
    plot->setStyle(PlotStyle::TEST_PALETTE);

    plot->setMargin(CRange2D(5, 10, 5, 10));
    plot->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }
  else {
    plot->setStyle(PlotStyle::TEST_TERMINAL);

    plot->setMargin(CRange2D(0, 0, 0, 0));
    plot->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }

  plot->setPalette(palette());

  window->addPlot(plot);

  window->stateChanged(ChangeState::PLOT_ADDED);
}

// fit [ {ranges} ]
//     {function} "<datafile>"
//                [ index {start:i}[:{end:i}][:{step:i}] ]
//                [ every {step:i}[::{start:i}[::{end:i}]] ]
//                [ using {columnspec} ]
//                via [ {var1}, {var2}, ... | "<parameterfile>" ]
void
CGnuPlot::
fitCmd(const std::string &)
{
}

// update "<filename>"
void
CGnuPlot::
updateCmd(const std::string &)
{
}

// bind
void
CGnuPlot::
bindCmd(const std::string &)
{
}

// if ({condition}) {command}
//    [; else if ( {condition} ) {command} ]
//    [; else {command} ]
void
CGnuPlot::
ifCmd(const std::string &)
{
}

// pause {seconds:i} [ "<message>" ]
// pause mouse [ {eventmask} ] [ "<message>" ]
void
CGnuPlot::
pauseCmd(const std::string &)
{
}

// reread
void
CGnuPlot::
rereadCmd(const std::string &)
{
}

CGnuPlotWindow *
CGnuPlot::
createWindow()
{
  return new CGnuPlotWindow(this);
}

CGnuPlotPlot *
CGnuPlot::
createPlot(CGnuPlotWindow *window)
{
  return new CGnuPlotPlot(window);
}

CGnuPlotPlot *
CGnuPlot::
addFunction2D(CGnuPlotWindow *window, const std::string &function, const CBBox2D &region,
              PlotStyle style)
{
  CParseLine line(function);

  // get first space separated word
  line.skipSpace();

  std::string identifier;

  while (line.isValid()) {
    char c = line.lookChar();

    if (identifier.empty()) {
      if (! isalpha(c))
        break;
    }
    else {
      if (! isalnum(c) && c != '_')
        break;
    }

    identifier += line.getChar();
  }

  line.skipSpace();

  // variable assignment
  if (line.isChar('=')) {
    line.skipChar();

    line.skipSpace();

    CExprValuePtr value = CExprInst->evaluateExpression(line.substr());

    if (value.isValid())
      CExprInst->createVariable(identifier, value);

    // TODO always need function
    return 0;
  }

  //---

  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    plot = createPlot(window);

    plot->set3D(false);

    if (clearRect_.isValid()) {
      plot->setClearRect(clearRect_.getValue());

      clearRect_.setInvalid();
    }

    if (keyData_.title == "")
      keyData_.title = function;

    plot->setRegion(region);

    plot->setStyle(style);

    plot->init();

    keyData_.title = "";

    incLineStyle();
  }

  //---

  double xmin, ymin, xmax, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  //---

  int nx, ny;

  getSamples(nx, ny);

  std::string varName1("x"), varName2("t");

  getDummyVars(varName1, varName2);

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

  double x = xmin;
  double d = (xmax - xmin)/nx;

  for (int i = 0; i <= nx; ++i, x += d) {
    xvar->setRealValue(x);

    // TODO: only execute in loop
    CExprPTokenStack pstack = CExprInst->parseLine(function);
    CExprITokenPtr   itoken = CExprInst->interpPTokenStack(pstack);
    CExprCTokenStack cstack = CExprInst->compileIToken(itoken);
    CExprValuePtr    value  = CExprInst->executeCTokenStack(cstack);

    double y = 0.0;

    if (value.isValid() && value->getRealValue(y)) {
      if (plot) {
        plot->addPoint2D(x, y);
      }
      else {
        // TODO: inside/outside test
        std::cerr << x << " " << y << " i" << std::endl;
      }
    }
  }

  //---

  return plot;
}

CGnuPlotPlot *
CGnuPlot::
addFunction3D(CGnuPlotWindow *window, const std::string &function, PlotStyle style)
{
  CParseLine line(function);

  // get first space separated word
  line.skipSpace();

  std::string identifier;

  while (line.isValid()) {
    char c = line.lookChar();

    if (identifier.empty()) {
      if (! isalpha(c))
        break;
    }
    else {
      if (! isalnum(c) && c != '_')
        break;
    }

    identifier += line.getChar();
  }

  line.skipSpace();

  // variable assignment
  if (line.isChar('=')) {
    line.skipChar();

    line.skipSpace();

    CExprValuePtr value = CExprInst->evaluateExpression(line.substr());

    if (value.isValid())
      CExprInst->createVariable(identifier, value);

    // TODO always need function
    return 0;
  }

  //---

  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    plot = createPlot(window);

    plot->set3D(true);

    plot->setStyle(style);

    plot->init();

    incLineStyle();

    window->addPlot(plot);
  }

  //---

  double xmin, ymin, xmax, ymax, zmin, zmax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);
  getZRange(&zmin, &zmax);

  //---

  int nx, ny;

  getIsoSamples(nx, ny);

  std::string varName1("x"), varName2("y");

  getDummyVars(varName1, varName2);

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);
  CExprVariablePtr yvar = CExprInst->createRealVariable(varName2, 0.0);

  double dx = (xmax - xmin)/nx;
  double dy = (ymax - ymin)/ny;

  double y = ymin;

  for (int iy = 0; iy <= ny; ++iy, y += dy) {
    yvar->setRealValue(y);

    double x = xmin;

    for (int ix = 0; ix <= nx; ++ix, x += dx) {
      xvar->setRealValue(x);

      CExprValuePtr value = CExprInst->evaluateExpression(function);

      double z = 0.0;

      if (! value.isValid() || ! value->getRealValue(z))
        x = 0.0;

      if (plot) {
        plot->addPoint3D(iy, x, y, z);
      }
      else {
        // TODO: inside/outside test
        std::cerr << x << " " << y << " " << z << " i" << std::endl;
      }
    }
  }

  //---

  return plot;
}

CGnuPlot::Plots
CGnuPlot::
addFile2D(CGnuPlotWindow *window, const std::string &filename, const CBBox2D &region,
          PlotStyle style, const UsingCols &usingCols, const Index &index, const Every &every)
{
  Plots plots;

  //---

  // open file
  CUnixFile file(filename);

  if (! file.open())
    return plots;

  //---

  int           bline         = 0;
  int           setNum        = 1;
  int           pointNum      = 0;
  int           pointNum1     = every.start;
  bool          discontinuity = false;
  CGnuPlotPlot *plot          = 0;
  bool          firstLine    = true;

  //---

  // process each line in file
  std::string line;

  while (file.readLine(line)) {
    if (line.empty()) {
      ++bline;

      if      (bline == 1)
        discontinuity = true;
      else if (bline == 2) {
        setNum   += index.step;
        pointNum  = 0;

        plot = 0;
      }

      continue;
    }

    //---

    auto hp = line.find(getCommentChars());

    if (hp != std::string::npos) {
      line = CStrUtil::stripSpaces(line.substr(0, hp));

      if (line.empty())
        continue;
    }

    //---

    // get x, y value from fields on line
    fields_.clear();

    if (getSeparator().type == SeparatorType::WHITESPACE)
      CStrUtil::addWords(line, fields_);
    else
      CStrUtil::addFields(line, fields_, std::string(&getSeparator().c, 1), /*skipEmpty*/true);

    //---

    if (firstLine) {
      firstLine = false;

      if (keyData_.columnhead) {
        keyData_.columns = fields_;
        continue;
      }
    }

    //---

    if (setNum < index.start || setNum > index.end)
      continue;

    //---

    bool skip = false;

    const std::string &missing = getMissingStr();

    std::vector<CExprValuePtr> values;

    for (uint i = 0; i < usingCols.size(); ++i) {
      CExprValuePtr value =
        CGnuPlotUtil::decodeUsingCol(usingCols[i], setNum, pointNum,
                                     fields_, missing, skip, isDebug());
      if (skip) continue;

      values.push_back(value);
    }

    //---

    if (! plot) {
      plot = createPlot(window);

      plot->set3D(false);

      if (clearRect_.isValid()) {
        plot->setClearRect(clearRect_.getValue());

        clearRect_.setInvalid();
      }

      if (keyData_.title == "") {
        if (keyData_.columnhead && usingCols.size() > 1 && usingCols[1].isInt &&
            usingCols[1].ival >= 1 && usingCols[1].ival <= int(keyData_.columns.size()))
          keyData_.title = keyData_.columns[usingCols[1].ival - 1];
        else if (keyData_.columnNum.isValid() && keyData_.columnNum.getValue() >= 1 &&
                 keyData_.columnNum.getValue() <= int(keyData_.columns.size()))
          keyData_.title = keyData_.columns[keyData_.columnNum.getValue()];
        else
          keyData_.title = "\"" + filename + "\"";
      }

      plot->setRegion(region);

      plot->setStyle(style);

      plot->init();

      keyData_.title = "";

      incLineStyle();

      plots.push_back(plot);
    }

    if (pointNum == pointNum1) {
      plot->addPoint2D(values, discontinuity);

      if (pointNum1 < INT_MAX)
        pointNum1 += every.step;

      if (pointNum1 > every.end)
        pointNum1 = INT_MAX;
    }

    ++pointNum;

    //---

    bline         = 0;
    discontinuity = false;
  }

  return plots;
}

CGnuPlotPlot *
CGnuPlot::
addFile3D(CGnuPlotWindow *window, const std::string &filename)
{
  CGnuPlotPlot *plot = 0;

  //---

  // open file
  CUnixFile file(filename);
  if (! file.open()) return 0;

  //---

  typedef std::map<double,double> XZMap;
  typedef std::map<double,XZMap>  YXZMap;

  YXZMap values;
  bool   firstLine = true;

  // process each line in file
  std::string line;

  while (file.readLine(line)) {
    auto hp = line.find(getCommentChars());

    if (hp != std::string::npos) {
      line = CStrUtil::stripSpaces(line.substr(0, hp));

      if (line.empty())
        continue;
    }

    //---

    // get values from fields on line
    std::vector<std::string> fields;

    fields.clear();

    if (getSeparator().type == SeparatorType::WHITESPACE)
      CStrUtil::addWords(line, fields);
    else
      CStrUtil::addFields(line, fields, std::string(&getSeparator().c, 1), /*skipEmpty*/true);

    //---

    if (firstLine) {
      firstLine = false;

      if (keyData_.columnhead) {
        keyData_.columns = fields;
        continue;
      }
    }

    //---

    int nf = fields.size();

    if (nf < 3) continue;

    double x = 0.0, y = 0.0, z = 0.0;

    if (! CStrUtil::toReal(fields[0], &x)) x = 0.0;
    if (! CStrUtil::toReal(fields[1], &y)) y = 0.0;
    if (! CStrUtil::toReal(fields[2], &z)) z = 0.0;

    values[y][x] = z;
  }

  if (values.empty())
    return 0;

  //---

  plot = createPlot(window);

  plot->set3D(true);

  plot->init();

  int iy = 0;

  for (const auto &value : values) {
    double       y       = value.first;
    const XZMap &values1 = value.second;

    for (const auto &value1 : values1) {
      double x = value1.first;
      double z = value1.second;

      plot->addPoint3D(iy, x, y, z);
    }

    ++iy;
  }

  incLineStyle();

  window->addPlot(plot);

  //---

  return plot;
}

void
CGnuPlot::
incLineStyle()
{
  ++styleIncrement_.colorInd;

  lineStyle().setColor(CGnuPlotStyle::instance()->indexColor(styleIncrement_.colorInd));

  if (fillStyle().style() == FillType::PATTERN)
    fillStyle().setPattern(
      static_cast<CGnuPlot::PatternType>(static_cast<int>(fillStyle().pattern()) + 1));

  if (lineStyleNum().isValid())
    setLineStyleNum(COptInt(lineStyleNum().getValue(1) + 1));
}

void
CGnuPlot::
drawWindows()
{
  for (const auto &window : windows_)
    window->draw();
}

void
CGnuPlot::
setAngleType(AngleType type)
{
  angleType_ = type;

  CExprInst->setDegrees(angleType_ == AngleType::DEGREES);
}

bool
CGnuPlot::
parseColorSpec(CParseLine &line, CRGBA &c)
{
  std::string colorTypeStr = readNonSpaceNonComma(line);

  if (colorTypeStr == "rgbcolor" || colorTypeStr == "rgb") {
    std::string colorValueStr;

    if (! parseString(line, colorValueStr))
      std::cerr << "Invalid color string" << std::endl;

    double r, g, b;

    if (! CRGBName::lookup(colorValueStr, &r, &g, &b)) {
      std::cerr << "Invalid color string" << std::endl;
      return false;
    }

    c = CRGBA(r, g, b);
  }
  else {
    std::cerr << "Invalid color spec" << std::endl;
    return false;
  }

  return true;
}

bool
CGnuPlot::
parseColor(CParseLine &line, CRGBA &c)
{
  line.skipSpace();

  if (line.isChar('"') || line.isChar('\'')) {
    std::string name;

    if (! parseString(line, name))
      return false;

    c = CRGBName::toRGBA(name);

    return true;
  }
  else
    return false;
}

bool
CGnuPlot::
parseInteger(CParseLine &line, int &i)
{
  line.skipSpace();

  int pos = line.pos();

  std::string str;

  if (line.isOneOf("+-"))
    str += line.getChar();

  if (! line.isDigit()) {
    line.setPos(pos);
    return false;
  }

  while (line.isDigit())
    str += line.getChar();

  if (! CStrUtil::toInteger(str, &i)) {
    line.setPos(pos);
    return false;
  }

  line.skipSpace();

  return true;
}

bool
CGnuPlot::
parseReal(CParseLine &line, double &r)
{
  line.skipSpace();

  int pos = line.pos();

  std::string str;

  //------

  if (line.isOneOf("+-"))
    str += line.getChar();

  //------

  while (line.isDigit())
    str += line.getChar();

  //------

  if (line.isChar('.')) {
    str += line.getChar();

    while (line.isDigit())
      str += line.getChar();
  }

  //------

  if (line.isOneOf("eE")) {
    str += line.getChar();

    if (line.isOneOf("+-"))
      str += line.getChar();

    if (line.isDigit()) {
      line.setPos(pos);
      return false;
    }

    while (line.isDigit())
      str += line.getChar();
  }

  //------

  if (str == "")
    return false;

  if (! CStrUtil::toReal(str, &r)) {
    line.setPos(pos);
    return false;
  }

  //------

  line.skipSpace();

  return true;
}

bool
CGnuPlot::
parseString(CParseLine &line, std::string &str)
{
  line.skipSpace();

  if      (line.isChar('"')) {
    line.skipChar();

    while (line.isValid() && ! line.isChar('"')) {
      char c = line.getChar();

      if (c == '\\') {
        if (line.isValid()) {
          c = line.getChar();

          switch (c) {
            case 't' : str += '\t'; break;
            case 'n' : str += '\n'; break;
            default  : str += '?' ; break;
          }
        }
        else
          str += c;
      }
      else
        str += c;
    }

    if (line.isChar('"'))
      line.skipChar();
  }
  else if (line.isChar('\'')) {
    line.skipChar();

    while (line.isValid() && ! line.isChar('\''))
      str += line.getChar();

    if (line.isChar('\''))
      line.skipChar();
  }
  else {
    return false;
  }

  line.skipSpace();

  return true;
}

bool
CGnuPlot::
parsePosition(CParseLine &line, Position &pos)
{
  line.skipSpace();

  if (line.isString("first") || line.isString("second") || line.isString("graph") ||
      line.isString("screen") || line.isString("character")) {
    std::string typeStr = readNonSpaceNonComma(line);
  }

  double x = 0.0, y = 0.0;

  double r;

  std::string arg = readNonSpaceNonComma(line);

  if (CStrUtil::toReal(arg, &r))
    x = r;
  else
    return false;

  line.skipSpace();

  if (! line.isChar(','))
    return false;

  line.skipChar();

  line.skipSpace();

  arg = readNonSpace(line);

  if (CStrUtil::toReal(arg, &r))
    y = r;
  else
    return false;

  pos.p = CPoint2D(x, y);

  return true;
}

bool
CGnuPlot::
parseSize(CParseLine &line, CSize2D &size)
{
  line.skipSpace();

  double w = 1.0, h = 1.0;

  std::string arg = readNonSpaceNonComma(line);

  if (! CStrUtil::toReal(arg, &w))
    return false;

  line.skipSpace();

  if (! line.isChar(','))
    return false;

  line.skipChar();

  line.skipSpace();

  arg = readNonSpace(line);

  if (! CStrUtil::toReal(arg, &h))
    return false;

  size = CSize2D(w, h);

  return true;
}

bool
CGnuPlot::
parseFunction(CParseLine &line, std::string &str)
{
  line.skipSpace();

  if (! line.isValid())
    return false;

  int brackets = 0;

  while (line.isValid()) {
    if      (line.isChar('('))
      ++brackets;
    else if (line.isChar(')'))
      --brackets;
    else if (line.isSpace() || line.isChar(',')) {
      if (brackets == 0)
        break;
    }

    str += line.getChar();
  }

  return true;
}

std::string
CGnuPlot::
readIdentifier(CParseLine &line)
{
  line.skipSpace();

  if (! line.isValid())
    return "";

  char c = line.lookChar();

  if (! isalpha(c))
    return "";

  std::string str;

  str += line.getChar();

  while (line.isValid()) {
    char c = line.lookChar();

    if (! isalnum(c) && c != '_')
      str += line.getChar();
  }

  return str;
}

std::string
CGnuPlot::
readNonSpace(CParseLine &line)
{
  line.skipSpace();

  return line.readNonSpace();
}

std::string
CGnuPlot::
readNonSpaceNonComma(CParseLine &line)
{
  line.skipSpace();

  std::string str;

  while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
    str += line.getChar();

  return str;
}

//------

CGnuPlotPlot::Point::
Point() :
 values(), discontinuity(false)
{
}

CGnuPlotPlot::Point::
Point(const Values &values, bool discontinuity) :
 values(values), discontinuity(discontinuity)
{
}

CGnuPlotPlot::Point::
~Point()
{
}

bool
CGnuPlotPlot::Point::
getX(double &x) const
{
  x = 0.0;

  if (values.size() < 1 || ! values[0].isValid())
    return false;

  return values[0]->getRealValue(x);
}

bool
CGnuPlotPlot::Point::
getY(double &y) const
{
  y = 0.0;

  if (values.size() < 2 || ! values[1].isValid())
    return false;

  return values[1]->getRealValue(y);
}

bool
CGnuPlotPlot::Point::
getZ(double &z) const
{
  z = 0.0;

  if (values.size() < 3 || ! values[2].isValid())
    return false;

  return values[2]->getRealValue(z);
}

double
CGnuPlotPlot::Point::
getX() const
{
  double x = 0.0;

  getX(x);

  return x;
}

double
CGnuPlotPlot::Point::
getY() const
{
  double y = 0.0;

  getY(y);

  return y;
}

double
CGnuPlotPlot::Point::
getZ() const
{
  double z = 0.0;

  getZ(z);

  return z;
}

bool
CGnuPlotPlot::Point::
getValue(int n, double &r) const
{
  r = 0.0;

  if (int(values.size()) < n || ! values[n - 1].isValid())
    return false;

  return values[n - 1]->getRealValue(r);
}

bool
CGnuPlotPlot::Point::
getValue(int n, std::string &str) const
{
  str = "";

  if (int(values.size()) < n || ! values[n - 1].isValid())
    return false;

  return values[n - 1]->getStringValue(str);
}

int
CGnuPlotPlot::Point::
cmp(const Point &p) const
{
  double x1, y1, x2, y2;

  (void) getX(x1);
  (void) getY(y1);

  (void) p.getX(x2);
  (void) p.getY(y2);

  if (x1 < x2) return -1;
  if (x1 > x2) return  1;
  if (y1 < y2) return -1;
  if (y1 > y2) return  1;

  return 0;
}

void
CGnuPlotPlot::Point::
print(std::ostream &os) const
{
  os << "(";

  for (uint i = 0; i < values.size(); ++i) {
    if (i > 0) os << ",";

    os << values[i];
  }

  os << ")";
}
