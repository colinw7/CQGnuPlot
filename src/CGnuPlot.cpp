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
    nameValues.addValues({{"bind"   , CGnuPlot::CommandName::BIND},
                          {"call"   , CGnuPlot::CommandName::CALL},
                          {"cd"     , CGnuPlot::CommandName::CD},
                          {"clear"  , CGnuPlot::CommandName::CLEAR},
                          {"exit"   , CGnuPlot::CommandName::EXIT},
                          {"fit"    , CGnuPlot::CommandName::FIT},
                          {"help"   , CGnuPlot::CommandName::HELP},
                          {"history", CGnuPlot::CommandName::HISTORY},
                          {"if"     , CGnuPlot::CommandName::IF},
                          {"load"   , CGnuPlot::CommandName::LOAD},
                          {"lower"  , CGnuPlot::CommandName::LOWER},
                          {"pause"  , CGnuPlot::CommandName::PAUSE},
                          {"plot"   , CGnuPlot::CommandName::PLOT},
                          {"print"  , CGnuPlot::CommandName::PRINT},
                          {"pwd"    , CGnuPlot::CommandName::PWD},
                          {"quit"   , CGnuPlot::CommandName::QUIT},
                          {"raise"  , CGnuPlot::CommandName::RAISE},
                          {"replot" , CGnuPlot::CommandName::REPLOT},
                          {"reread" , CGnuPlot::CommandName::REREAD},
                          {"reset"  , CGnuPlot::CommandName::RESET},
                          {"save"   , CGnuPlot::CommandName::SAVE},
                          {"set"    , CGnuPlot::CommandName::SET},
                          {"shell"  , CGnuPlot::CommandName::SHELL},
                          {"show"   , CGnuPlot::CommandName::SHOW},
                          {"splot"  , CGnuPlot::CommandName::SPLOT},
                          {"system" , CGnuPlot::CommandName::SYSTEM},
                          {"test"   , CGnuPlot::CommandName::TEST},
                          {"unset"  , CGnuPlot::CommandName::UNSET},
                          {"update" , CGnuPlot::CommandName::UPDATE}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::VariableName> &nameValues) {
    nameValues.addValues({{"datafile"   , CGnuPlot::VariableName::DATAFILE},
                          {"decimalsign", CGnuPlot::VariableName::DECIMALSIGN},
                          {"encoding"   , CGnuPlot::VariableName::ENCODING},
                          {"fit"        , CGnuPlot::VariableName::FIT},
                          {"fontpath"   , CGnuPlot::VariableName::FONTPATH},
                          {"historysize", CGnuPlot::VariableName::HISTORYSIZE},
                          {"loadpath"   , CGnuPlot::VariableName::LOADPATH},
                          {"macros"     , CGnuPlot::VariableName::MACROS},
                          {"mouse"      , CGnuPlot::VariableName::MOUSE},

                          {"angles"     , CGnuPlot::VariableName::ANGLES},
                          {"dummy"      , CGnuPlot::VariableName::DUMMY},
                          {"mapping"    , CGnuPlot::VariableName::MAPPING},
                          {"parametric" , CGnuPlot::VariableName::PARAMETRIC},
                          {"polar"      , CGnuPlot::VariableName::POLAR},
                          {"samples"    , CGnuPlot::VariableName::SAMPLES},
                          {"zero"       , CGnuPlot::VariableName::ZERO},

                          {"border"     , CGnuPlot::VariableName::BORDER},
                          {"clip"       , CGnuPlot::VariableName::CLIP},
                          {"logscale"   , CGnuPlot::VariableName::LOGSCALE},
                          {"bmargin"    , CGnuPlot::VariableName::BMARGIN},
                          {"lmargin"    , CGnuPlot::VariableName::LMARGIN},
                          {"rmargin"    , CGnuPlot::VariableName::RMARGIN},
                          {"tmargin"    , CGnuPlot::VariableName::TMARGIN},
                          {"multiplot"  , CGnuPlot::VariableName::MULTIPLOT},
                          {"offsets"    , CGnuPlot::VariableName::OFFSETS},
                          {"origin"     , CGnuPlot::VariableName::ORIGIN},
                          {"size"       , CGnuPlot::VariableName::SIZE},

                          {"arrow"      , CGnuPlot::VariableName::ARROW},
                          {"key"        , CGnuPlot::VariableName::KEY},
                          {"label"      , CGnuPlot::VariableName::LABEL},
                          {"object"     , CGnuPlot::VariableName::OBJECT},
                          {"timestamp"  , CGnuPlot::VariableName::TIMESTAMP},
                          {"title"      , CGnuPlot::VariableName::TITLE},

                          {"style"      , CGnuPlot::VariableName::STYLE},

                          {"bars"       , CGnuPlot::VariableName::BARS},
                          {"boxwidth"   , CGnuPlot::VariableName::BOXWIDTH},
                          {"pointsize"  , CGnuPlot::VariableName::POINTSIZE},

                          {"autoscale"  , CGnuPlot::VariableName::AUTOSCALE},
                          {"format"     , CGnuPlot::VariableName::FORMAT},
                          {"grid"       , CGnuPlot::VariableName::GRID},
                          {"xlabel"     , CGnuPlot::VariableName::XLABEL}, // z, cb
                          {"ylabel"     , CGnuPlot::VariableName::YLABEL},
                          {"x2label"    , CGnuPlot::VariableName::X2LABEL},
                          {"y2label"    , CGnuPlot::VariableName::Y2LABEL},
                          {"xrange"     , CGnuPlot::VariableName::XRANGE}, // x2, y2, z, cb
                          {"yrange"     , CGnuPlot::VariableName::YRANGE}, // y, x2, y2, z, cb
                          {"tics"       , CGnuPlot::VariableName::TICS},
                          {"xtics"      , CGnuPlot::VariableName::XTICS}, // z, cb
                          {"ytics"      , CGnuPlot::VariableName::YTICS},
                          {"x2tics"     , CGnuPlot::VariableName::X2TICS},
                          {"y2tics"     , CGnuPlot::VariableName::Y2TICS},
                          {"mxtics"     , CGnuPlot::VariableName::MXTICS},
                          {"ticscale"   , CGnuPlot::VariableName::TICSCALE},
                          {"zeroaxis"   , CGnuPlot::VariableName::ZEROAXIS},

                          {"locale"     , CGnuPlot::VariableName::LOCALE},
                          {"timefmt"    , CGnuPlot::VariableName::TIMEFMT},
                          {"xdata"      , CGnuPlot::VariableName::XDATA},
                          {"ydata"      , CGnuPlot::VariableName::YDATA},
                          {"xdtics"     , CGnuPlot::VariableName::XDTICS},
                          {"xmtics"     , CGnuPlot::VariableName::XMTICS},

                          {"dgrid3d"    , CGnuPlot::VariableName::DGRID3D},
                          {"hidden3d"   , CGnuPlot::VariableName::HIDDEN3D},
                          {"isosamples" , CGnuPlot::VariableName::ISOSAMPLES},
                          {"surface"    , CGnuPlot::VariableName::SURFACE},
                          {"view"       , CGnuPlot::VariableName::VIEW},
                          {"xyplane"    , CGnuPlot::VariableName::XYPLANE},

                          {"cplane"     , CGnuPlot::VariableName::CPLANE},
                          {"cntrparam"  , CGnuPlot::VariableName::CNTRPARAM},
                          {"contour"    , CGnuPlot::VariableName::CONTOUR},

                          {"colorbox"   , CGnuPlot::VariableName::COLORBOX},
                          {"palette"    , CGnuPlot::VariableName::PALETTE},
                          {"pm3d"       , CGnuPlot::VariableName::PM3D},

                          {"output"     , CGnuPlot::VariableName::OUTPUT},
                          {"print"      , CGnuPlot::VariableName::PRINT},
                          {"table"      , CGnuPlot::VariableName::TABLE},
                          {"terminal"   , CGnuPlot::VariableName::TERMINAL},
                          {"termoption" , CGnuPlot::VariableName::TERMOPTION},

                          {"variables"  , CGnuPlot::VariableName::VARIABLES},
                          {"functions"  , CGnuPlot::VariableName::FUNCTIONS},
                          {"version"    , CGnuPlot::VariableName::VERSION},

                          {"debug"      , CGnuPlot::VariableName::DEBUG},
                          {"edebug"     , CGnuPlot::VariableName::EDEBUG}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::DataFileVar> &nameValues) {
    nameValues.addValues({{"separator"   , CGnuPlot::DataFileVar::SEPARATOR},
                          {"commentschar", CGnuPlot::DataFileVar::COMMENTS_CHAR},
                          {"missing"     , CGnuPlot::DataFileVar::MISSING}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::StyleVar> &nameValues) {
    nameValues.addValues({{"data"     , CGnuPlot::StyleVar::DATA},
                          {"function" , CGnuPlot::StyleVar::FUNCTION},
                          {"increment", CGnuPlot::StyleVar::INCREMENT},
                          {"line"     , CGnuPlot::StyleVar::LINE},
                          {"fill"     , CGnuPlot::StyleVar::FILL},
                          {"histogram", CGnuPlot::StyleVar::HISTOGRAM},
                          {"rectangle", CGnuPlot::StyleVar::RECTANGLE},
                          {"circle"   , CGnuPlot::StyleVar::CIRCLE},
                          {"ellipse"  , CGnuPlot::StyleVar::ELLIPSE},
                          {"arrow"    , CGnuPlot::StyleVar::ARROW}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotVar> &nameValues) {
    nameValues.addValues({{"index"      , CGnuPlot::PlotVar::INDEX},
                          {"every"      , CGnuPlot::PlotVar::EVERY},
                          {"using"      , CGnuPlot::PlotVar::USING},
                          {"axes"       , CGnuPlot::PlotVar::AXES},
                          {"title"      , CGnuPlot::PlotVar::TITLE},
                          {"notitle"    , CGnuPlot::PlotVar::NOTITLE},
                          {"smooth"     , CGnuPlot::PlotVar::SMOOTH},
                          {"with"       , CGnuPlot::PlotVar::WITH},
                          {"linetype"   , CGnuPlot::PlotVar::LINETYPE},
                          {"lt"         , CGnuPlot::PlotVar::LINETYPE},
                          {"linewidth"  , CGnuPlot::PlotVar::LINEWIDTH},
                          {"lw"         , CGnuPlot::PlotVar::LINEWIDTH},
                          {"fillstyle"  , CGnuPlot::PlotVar::FILLSTYLE},
                          {"fs"         , CGnuPlot::PlotVar::FILLSTYLE},
                          {"pointsize"  , CGnuPlot::PlotVar::POINTSIZE},
                          {"whiskerbars", CGnuPlot::PlotVar::WHISKERBARS}});
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotStyle> &nameValues) {
    nameValues.addValues({{"lines"         , CGnuPlot::PlotStyle::LINES},
                          {"l"             , CGnuPlot::PlotStyle::LINES},
                          {"points"        , CGnuPlot::PlotStyle::POINTS},
                          {"p"             , CGnuPlot::PlotStyle::POINTS},
                          {"linespoints"   , CGnuPlot::PlotStyle::LINES_POINTS},
                          {"dots"          , CGnuPlot::PlotStyle::DOTS},
                          {"impulses"      , CGnuPlot::PlotStyle::IMPULSES},
                          {"labels"        , CGnuPlot::PlotStyle::LABELS},
                          {"steps"         , CGnuPlot::PlotStyle::STEPS},
                          {"fsteps"        , CGnuPlot::PlotStyle::FSTEPS},
                          {"histeps"       , CGnuPlot::PlotStyle::HISTEPS},
                          {"errorbars"     , CGnuPlot::PlotStyle::ERRORBARS},
                          {"errorlines"    , CGnuPlot::PlotStyle::ERRORLINES},
                          {"financebars"   , CGnuPlot::PlotStyle::FINANCEBARS},
                          {"xerrorbars"    , CGnuPlot::PlotStyle::XERRORBARS},
                          {"xerrorlines"   , CGnuPlot::PlotStyle::XERRORLINES},
                          {"xyerrorbars"   , CGnuPlot::PlotStyle::XYERRORBARS},
                          {"xyerrorlines"  , CGnuPlot::PlotStyle::XYERRORLINES},
                          {"yerrorbars"    , CGnuPlot::PlotStyle::YERRORBARS},
                          {"yerrorlines"   , CGnuPlot::PlotStyle::YERRORLINES},
                          {"vectors"       , CGnuPlot::PlotStyle::VECTORS},
                          {"boxes"         , CGnuPlot::PlotStyle::BOXES},
                          {"boxerrorbars"  , CGnuPlot::PlotStyle::BOXERRORBARS},
                          {"boxxyerrorbars", CGnuPlot::PlotStyle::BOXXYERRORBARS},
                          {"boxplot"       , CGnuPlot::PlotStyle::BOXPLOT},
                          {"candlesticks"  , CGnuPlot::PlotStyle::CANDLESTICKS},
                          {"filledcurves"  , CGnuPlot::PlotStyle::FILLEDCURVES},
                          {"histograms"    , CGnuPlot::PlotStyle::HISTOGRAMS},
                          {"image"         , CGnuPlot::PlotStyle::IMAGE},
                          {"rgbimage"      , CGnuPlot::PlotStyle::RGBIMAGE},
                          {"rgbalpha"      , CGnuPlot::PlotStyle::RGBALPHA},
                          {"circles"       , CGnuPlot::PlotStyle::CIRCLES},
                          {"ellipses"      , CGnuPlot::PlotStyle::ELLIPSES},
                          {"pm3d"          , CGnuPlot::PlotStyle::PM3D}});
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

    while (line[line.size() - 1] == '\\') {
      std::string line1;

      if (! file.readLine(line1))
        line1 = "";

      line = line.substr(0, line.size() - 1) + line1;
    }

    parseLine(line);
  }

  return true;
}

void
CGnuPlot::
initReadLine()
{
  if (! readLine_.isValid()) {
    readLine_ = new CGnuPlotReadLine(this);

    readLine_->enableTimeoutHook(1);
  }
}

void
CGnuPlot::
loop()
{
  initReadLine();

  for (;;) {
    readLine_->setPrompt("> ");

    std::string line = readLine_->readLine();

    while (line[line.size() - 1] == '\\') {
      readLine_->setPrompt("+> ");

      std::string line1 = readLine_->readLine();

      line = line.substr(0, line.size() - 1) + line1;
    }

    parseLine(line);

    readLine_->addHistory(line);
  }
}

void
CGnuPlot::
prompt(const std::string &msg)
{
  initReadLine();

  readLine_->setPrompt(msg);

  (void) readLine_->readLine();

  readLine_->setPrompt("> ");
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

  // empty line is ok
  if (identifier.empty())
    return true;

  //---

  // function definition
  if (line.isChar('(')) {
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

  line.skipSpace();

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
  lastPlotCmd_ = args;

  // TODO: update local line style copy

  typedef std::vector<CGnuPlotPlot *> Plots;

  Plots plots;

  CBBox2D region;

  CGnuPlotWindow *window;

  if (multiplot().enabled) {
    if (! multiWindow_) {
      multiWindow_ = createWindow();

      multiWindow_->set3D(false);

      windows_.push_back(multiWindow_);
    }

    window = multiWindow_;

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
    if (! windows_.empty()) {
      for (auto &w : windows_) {
        w->clear();

        window = w;
      }
    }
    else
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
    line.skipSpace();

    // Get Range
    //  [XMIN:XMAX][YMIN:YMAX]

    if (line.isChar('[')) {
      line.skipChar();

      std::string xrange, yrange;

      while (line.isValid() && ! line.isChar(']'))
        xrange += line.getChar();

      if (line.isChar(']'))
        line.skipChar();

      line.skipSpace();

      if (line.isChar('[')) {
        line.skipChar();

        while (line.isValid() && ! line.isChar(']'))
          yrange += line.getChar();

        if (line.isChar(']'))
          line.skipChar();
      }

      if (isDebug())
        std::cerr << "Range [" << xrange << "][" << yrange << "]" << std::endl;

      std::vector<std::string> xfields, yfields;

      CStrUtil::addFields(xrange, xfields, ":");
      CStrUtil::addFields(yrange, yfields, ":");

      //---

      decodeXRange(xfields);
      decodeYRange(yfields);

      line.skipSpace();
    }

    //----

    // TODO: newhistogram <title> , ...

    //----

    // Get filename or expression
    //  "<filename>" | <expression>

    PlotStyle style(PlotStyle::POINTS);

    std::string filename, function;

    if (line.isChar('\"') || line.isChar('\'')) {
      (void) parseString(line, filename, "Invalid filename");

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
      std::string option = readNonSpaceNonComma(line);

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

              usingStr += line.getChar();
            }
          }
          else {
            if      (line.isChar('('))
              ++rbrackets;
            else if (line.isChar(')'))
              --rbrackets;

            usingStr += line.getChar();
          }
        }

        if (! usingStr.empty())
          usingStrs.push_back(usingStr);

        for (auto &str : usingStrs) {
          std::string str1 = CStrUtil::stripSpaces(str);

          if (str1.size() > 0 && str1[0] == '(' && str1[str1.size() - 1] == ')')
            str1 = CStrUtil::stripSpaces(str1.substr(1, str1.size() - 2));

          str = str1;
        }

        if (isDebug()) {
          std::cerr << "using ";

          for (uint i = 0; i < usingStrs.size(); ++i) {
            if (i > 0) std::cerr << ":";

            std::cerr << usingStrs[i];
          }

          std::cerr << std::endl;
        }

        line.skipSpace();

        for (const auto &str : usingStrs)
          usingCols.push_back(UsingCol(str));
      }
      // read index range
      // index <start>[:<end>[:<step>]]
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
      // every <step>[:<start>[:<end>]]
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

          if (style == CGnuPlot::PlotStyle::FILLEDCURVES) {
            int         pos = line.pos();
            std::string arg = readNonSpaceNonComma(line);

            filledCurve_.closed = false;
            filledCurve_.above  = false;
            filledCurve_.below  = false;
            filledCurve_.xaxis  = 0;
            filledCurve_.yaxis  = 0;

            filledCurve_.xval .setInvalid();
            filledCurve_.yval .setInvalid();
            filledCurve_.xyval.setInvalid();

            if (arg == "closed") {
              filledCurve_.closed = true;

              pos = line.pos();
              arg = readNonSpaceNonComma(line);
            }

            if (arg == "above" || arg == "below") {
              filledCurve_.above = (arg == "above");
              filledCurve_.below = (arg == "below");

              pos = line.pos();
              arg = readNonSpaceNonComma(line);
            }

            auto p = arg.find('=');

            std::string lhs, rhs;

            if (p != std::string::npos) {
              lhs = arg.substr(0, p);
              rhs = arg.substr(p + 1);
            }
            else
              lhs = arg;

            double x, y;

            if      (lhs == "x1" || lhs == "x2") {
              filledCurve_.xaxis = (lhs == "x1" ? 1 : 2);

              if (CStrUtil::toReal(rhs, &x))
                filledCurve_.xval = x;
            }
            else if (lhs == "y1" || lhs == "y2") {
              filledCurve_.yaxis = (lhs == "y1" ? 1 : 2);

              if (CStrUtil::toReal(rhs, &y))
                filledCurve_.yval = y;
            }
            else if (lhs == "xy") {
              std::string rhs1;

              if (line.isChar(',')) {
                line.skipChar();

                rhs1 = readNonSpaceNonComma(line);
              }

              if (CStrUtil::toReal(rhs, &x) && CStrUtil::toReal(rhs1, &y))
                filledCurve_.xyval = CPoint2D(x, y);
            }
            else {
              line.setPos(pos);
            }
          }
        }

        line.skipSpace();

        bool modifiers = true;

        while (modifiers) {
          if      (line.isString("linestyle") || line.isString("ls")) {
            line.skipNonSpace();

            std::string styleStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(styleStr, &l))
              setLineStyleInd(l);
          }
          else if (line.isString("linetype") || line.isString("lt")) {
            line.skipNonSpace();

            int i;

            if (parseInteger(line, i))
              lineStyle()->setType(i);
          }
          else if (line.isString("linewidth") || line.isString("lw")) {
            line.skipNonSpace();

            double lw;

            if (parseReal(line, lw))
              lineStyle()->setWidth(lw);
          }
          else if (line.isString("linecolor") || line.isString("lc")) {
            line.skipNonSpace();

            CRGBA c;

            if (parseColorSpec(line, c))
              lineStyle()->setColor(c);
          }
          else if (line.isString("pointtype") || line.isString("pt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              lineStyle()->setPointType(static_cast<CGnuPlotTypes::SymbolType>(l));
          }
          else if (line.isString("pointsize") || line.isString("ps")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);

            double s;

            if      (style == "variable" || style == "var")
              pointStyle().setVarSize(true);
            else if (CStrUtil::toReal(style, &s))
              lineStyle()->setPointSize(s);
          }
          else if (line.isString("fill") || line.isString("fs")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);

            if (style == "transparent")
              style = readNonSpaceNonComma(line);

            if (style == "solid") {
              line.skipSpace();

              if (isdigit(line.lookChar())) {
                double density = 0.0;

                if (! parseReal(line, density))
                  density = 0.0;
              }
            }
            else if (style == "pattern") {
              line.skipSpace();

              if (isdigit(line.lookChar())) {
                int pattern = 0;

                if (! parseInteger(line, pattern))
                  pattern = 0;
              }
            }
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
      // axes <axes>
      else if (plotVar == PlotVar::AXES) {
        std::string axesStr = readNonSpaceNonComma(line);

        if      (axesStr == "x1y1") {
          xind_ = 1; yind_ = 1;
        }
        else if (axesStr == "x1y2") {
          xind_ = 1; yind_ = 2;
        }
        else if (axesStr == "x2y1") {
          xind_ = 2; yind_ = 1;
        }
        else if (axesStr == "x2y2") {
          xind_ = 2; yind_ = 2;
        }
      }
      // linetype <lt>
      else if (plotVar == CGnuPlot::PlotVar::LINETYPE) {
        int lt;

        if (parseInteger(line, lt))
          lineStyle()->setType(lt);
      }
      // linewidth <lw>
      else if (plotVar == CGnuPlot::PlotVar::LINEWIDTH) {
        double lw;

        if (parseReal(line, lw))
          lineStyle()->setWidth(lw);
      }
      // fillstyle <fs>
      else if (plotVar == CGnuPlot::PlotVar::FILLSTYLE) {
        int fs;

        if (! parseInteger(line, fs))
          fs = 0;
      }
      // pointsize <size>
      else if (plotVar == PlotVar::POINTSIZE) {
        double r = 1.0;

        if (parseReal(line, r))
          lineStyle()->setPointSize(r > 0 ? r : 1.0);
      }
      // whiskerbars
      else if (plotVar == PlotVar::WHISKERBARS) {
      }
    }

    //---

    if      (! filename.empty()) {
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

    plot->addObjects();

    plot->setRegion(region);

    plot->setStyle   (plots[0]->getStyle());
    plot->setKeyData (plots[0]->keyData ());
    plot->setAxesData(plots[0]->axesData());

    plot->addSubPlots(plots);

    plot->setHistogramStyle(histogramStyle());

    plot->setFitX(true);
    plot->setFitY(true);

    plot->fit();

    window->addPlot(plot);
  }
  else {
    CGnuPlotPlot *plot = plots[0];

    plot->addObjects();

    window->addPlot(plot);
  }

  //---

  window->stateChanged(ChangeState::PLOT_ADDED);
}

void
CGnuPlot::
decodeXRange(const std::vector<std::string> &xfields)
{
  std::vector<COptReal> xvalues;

  for (const auto &xfield : xfields) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;

    if (axesData().xaxis.isTime) {
      int len = xfield.size();

      if (len > 0 && xfield[0] == '"' && xfield[len - 1] == '"') {
        strptime(xfield.substr(1, len - 2).c_str(), timeFmt_.c_str(), &tm1);

        value = mktime(&tm1);
      }
    }
    else {
      CExprValuePtr evalue = CExprInst->evaluateExpression(xfield);

      double r;

      if (evalue.isValid() && evalue->getRealValue(r))
        value = r;
    }

    xvalues.push_back(value);
  }

  if (xvalues.size() == 2) {
    COptReal &xmin = xvalues[0];
    COptReal &xmax = xvalues[1];

    if (isDebug())
      std::cerr << "XRange=(" << xmin.getValue(-10) << "," <<
                                 xmax.getValue( 10) << ")" << std::endl;

    if (xmin.isValid()) axesData().xaxis.min = xmin;
    if (xmax.isValid()) axesData().xaxis.max = xmax;

    setFitX(! xmin.isValid() || ! xmax.isValid());
  }
}

void
CGnuPlot::
decodeYRange(const std::vector<std::string> &yfields)
{
  std::vector<COptReal> yvalues;

  for (const auto &yfield : yfields) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;

    if (axesData().yaxis.isTime) {
      int len = yfield.size();

      if (len > 0 && yfield[0] == '"' && yfield[len - 1] == '"') {
        strptime(yfield.substr(1, len - 2).c_str(), timeFmt_.c_str(), &tm1);

        value = mktime(&tm1);
      }
    }
    else {
      CExprValuePtr evalue = CExprInst->evaluateExpression(yfield);

      double r;

      if (evalue.isValid() && evalue->getRealValue(r))
        value = r;
    }

    yvalues.push_back(value);
  }

  if (yvalues.size() == 2) {
    COptReal &ymin = yvalues[0];
    COptReal &ymax = yvalues[1];

    if (isDebug())
      std::cerr << "YRange=(" << ymin.getValue(-1) << "," <<
                                 ymax.getValue( 1) << ")" << std::endl;

    if (ymin.isValid()) axesData().yaxis.min = ymin;
    if (ymax.isValid()) axesData().yaxis.max = ymax;

    setFitY(! ymin.isValid() || ! ymax.isValid());
  }
}

void
CGnuPlot::
decodeZRange(const std::vector<std::string> &zfields)
{
  std::vector<COptReal> zvalues;

  for (const auto &zfield : zfields) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;

    if (axesData().zaxis.isTime) {
      int len = zfield.size();

      if (len > 0 && zfield[0] == '"' && zfield[len - 1] == '"') {
        strptime(zfield.substr(1, len - 2).c_str(), timeFmt_.c_str(), &tm1);

        value = mktime(&tm1);
      }
    }
    else {
      CExprValuePtr evalue = CExprInst->evaluateExpression(zfield);

      double r;

      if (evalue.isValid() && evalue->getRealValue(r))
        value = r;
    }

    zvalues.push_back(value);
  }

  if (zvalues.size() == 2) {
    COptReal &zmin = zvalues[0];
    COptReal &zmax = zvalues[1];

    if (isDebug())
      std::cerr << "ZRange=(" << zmin.getValue(-1) << "," <<
                                 zmax.getValue( 1) << ")" << std::endl;

    if (zmin.isValid()) axesData().zaxis.min = zmin;
    if (zmax.isValid()) axesData().zaxis.max = zmax;

    setFitZ(! zmin.isValid() || ! zmax.isValid());
  }
}

// replot
void
CGnuPlot::
replotCmd(const std::string &)
{
  if (lastPlotCmd_ != "")
    plotCmd(lastPlotCmd_);
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

      decodeXRange(xfields);
      decodeYRange(yfields);
      decodeZRange(zfields);

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
              setLineStyleInd(l);
          }
          else if (line.isString("linetype") || line.isString("lt")) {
            int i;

            if (parseInteger(line, i))
              lineStyle()->setType(i);
          }
          else if (line.isString("linewidth") || line.isString("lw")) {
            double lw;

            if (parseReal(line, lw))
              lineStyle()->setWidth(lw);
          }
          else if (line.isString("linecolor") || line.isString("lc")) {
            CRGBA c;

            if (parseColorSpec(line, c))
              lineStyle()->setColor(c);
          }
          else if (line.isString("pointtype") || line.isString("pt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              lineStyle()->setPointType(static_cast<CGnuPlotTypes::SymbolType>(l));
          }
          else if (line.isString("pointsize") || line.isString("ps")) {
            line.skipNonSpace();

            std::string style = readNonSpaceNonComma(line);

            double s;

            if      (style == "variable" || style == "var")
              pointStyle().setVarSize(true);
            else if (CStrUtil::toReal(style, &s))
              lineStyle()->setPointSize(s);
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
        double r = 1.0;

        if (parseReal(line, r))
          lineStyle()->setPointSize(r > 0 ? r : 1.0);
      }
    }

    //---

    if      (! filename.empty()) {
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

        if (parseString(line, chars, "Invalid character string")) {
          if (chars.size() != 1) {
            std::cerr << "Only one character allowed for separator" << std::endl;
            return;
          }

          setSeparator(Separator(chars[0]));
        }
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

        if (! parseString(line, chars, "Invalid character string"))
          setCommentChars(chars);
      }
      else
        setCommentChars("#");
    }
    // set datafile missing ["<chars>"]
    else if (var1 == DataFileVar::MISSING) {
      if (line.isValid()) {
        std::string chars;

        if (parseString(line, chars, "Invalid character string"))
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
        double lw;

        if (! parseReal(line, lw))
          axesData().borderWidth = lw;
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
        std::string titleStr;

        parseString(line, titleStr);
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
    CGnuPlotArrowP arrow(createArrow());

    std::string arg = readNonSpace(line);

    if (CStrUtil::isInteger(arg)) {
      int arrowId = -1;

      if (CStrUtil::toInteger(arg, &arrowId))
        arrow->setInd(arrowId);

      arg = readNonSpace(line);
    }

    if (arg == "from") {
      Position p;

      if (parsePosition(line, p))
        arrow->setFrom(p.p);

      arg = readNonSpace(line);

      if (arg == "to" || arg == "rto") {
        if (arg == "rto")
          arrow->setRelative(true);

        if (parsePosition(line, p))
          arrow->setTo(p.p);

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
        if      (arg == "nohead"  ) { arrow->setFHead(false); arrow->setTHead(false); }
        else if (arg == "head"    ) { arrow->setFHead(false); arrow->setTHead(true ); }
        else if (arg == "backhead") { arrow->setFHead(true ); arrow->setTHead(false); }
        else if (arg == "heads"   ) { arrow->setFHead(true ); arrow->setTHead(true ); }
      }
      else if (arg == "size") {
        double r = 1.0;

        if (parseReal(line, r))
          arrow->setLength(r);

        if (line.isChar(',')) {
          line.skipChar();

          if (parseReal(line, r))
            arrow->setAngle(r);

          if (line.isChar(',')) {
            line.skipChar();

            if (parseReal(line, r))
              arrow->setBackAngle(r);
          }
        }
      }
      else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
        arrow->setFilled(arg == "filled" || arg == "empty");
        arrow->setEmpty (arg == "empty" );
      }
      else if (arg == "front" || arg == "back") {
        arrow->setFront(arg == "front");
      }
      else if (arg == "linetype" || arg == "lt") {
        int lt = 0;

        if (parseInteger(line, lt))
          arrow->setLineType(lt);
      }
      else if (arg == "linewidth" || arg == "lw") {
        double lw = 1.0;

        if (parseReal(line, lw))
          arrow->setLineWidth(lw);
      }
      else if (arg == "linestyle" || arg == "ls") {
        int linestyle = -1;

        if (! parseInteger(line, linestyle))
          linestyle = -1;
      }
      else {
        std::cerr << "Invalid arg '" << arg << "'" << std::endl;
        break;
      }

      arg = readNonSpace(line);
    }

    if (arrow->getInd() == -1)
      arrow->setInd(arrows_.size() + 1);

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

      if      (arg == "on" || arg == "off") { keyData_.displayed = (arg == "on"); }
      else if (arg == "default") { }
      else if (arg == "inside" || arg == "outside") { keyData_.outside = (arg == "outside"); }
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
        std::string titleStr;

        parseString(line, titleStr);
      }
      else if (arg == "enhanced") { }
      else if (arg == "noenhanced") { }
      else if (arg == "font") {
        std::string fontStr;

        parseString(line, fontStr);
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

    CGnuPlotLabelP label(createLabel());

    if (line.isChar('"') || line.isChar('\'')) {
      std::string t;

      if (parseString(line, t))
        label->setText(t);
    }

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "at") {
        Position p;

        if (parsePosition(line, p))
          label->setPos(p.p);
      }
      else if (arg == "left" || "center" || arg == "right") {
        label->setAlign(arg != "left" ?
          (arg != "center" ? CHALIGN_TYPE_RIGHT : CHALIGN_TYPE_CENTER) : CHALIGN_TYPE_LEFT);
      }
      else if (arg == "rotate") {
        line.skipSpace();

        if (line.isString("by")) {
          arg = readNonSpace(line);

          arg = readNonSpace(line);

          double a;

          if (CStrUtil::toReal(arg, &a))
            label->setAngle(a);
        }
        else
          label->setAngle(90);
      }
      else if (arg == "norotate") {
        label->setAngle(0);
      }
      else if (arg == "font") {
        std::string s;

        if (parseString(line, s))
          label->setFont(s);
      }
      else if (arg == "noenhanced") {
      }
      else if (arg == "front" || arg == "back") {
        label->setFront(arg == "front");
      }
      else if (arg == "textcolor") {
        CRGBA c;

        if (parseColorSpec(line, c))
          label->setStrokeColor(c);
      }
      else if (arg == "point") {
        arg = readNonSpace(line);
      }
      else if (arg == "nopoint") {
      }
      else if (arg == "offset") {
        double r;

        if (parseReal(line, r))
          label->setOffset(r);
      }
      else {
        std::cerr << "Invalid arg '" << arg << "'" << std::endl;
        break;
      }

      arg = readNonSpace(line);
    }

    if (labelId == -1)
      label->setInd(labels_.size() + 1);

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
        CGnuPlotEllipseP ellipse = CGnuPlotEllipseP(createEllipse());

        CSize2D size(1,1);

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "at" || arg == "circle") {
            Position p;

            if (parsePosition(line, p))
              ellipse->setCenter(p.p);
          }
          else if (arg == "size") {
            arg = readNonSpace(line);

            double s;

            if (CStrUtil::toReal(arg, &s)) {
              ellipse->setRX(s);
              ellipse->setRY(s);
            }
          }
          else if (arg == "arc") {
            arg = readNonSpace(line);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            CRGBA c;

            if (parseColorSpec(line, c))
              ellipse->setFillColor(c);
          }

          arg = readNonSpace(line);
        }

        ellipses_.push_back(ellipse);
      }
      // set object <index> ellipse {at|center} <position> size <w>,<h>
      //            {angle <orientation>} {units xy|xx|yy}
      //            {<other-object-properties>}
      else if (type == ObjectType::ELLIPSE) {
        CGnuPlotEllipseP ellipse = CGnuPlotEllipseP(createEllipse());

        CSize2D size(1,1);

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "at" || arg == "center") {
            Position p;

            if (parsePosition(line, p))
              ellipse->setCenter(p.p);
          }
          else if (arg == "size") {
            if (! parseSize(line, size))
              continue;

            ellipse->setRX(size.width /2.0);
            ellipse->setRY(size.height/2.0);
          }
          else if (arg == "angle") {
            arg = readNonSpace(line);
          }
          else if (arg == "units") {
            arg = readNonSpace(line);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            CRGBA c;

            if (parseColorSpec(line, c))
              ellipse->setFillColor(c);
          }

          arg = readNonSpace(line);
        }

        ellipses_.push_back(ellipse);
      }
      // set object <index> polygon
      //            from <position> to <position> ... {to <position>} |
      //            from <position> rto <position> ... {rto <position>}
      else if (type == ObjectType::POLYGON) {
        CGnuPlotPolygonP poly = CGnuPlotPolygonP(createPolygon());

        Position from, to;

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "from") {
            if (! parsePosition(line, from))
              return;

            poly->addPoint(from.p);
          }
          else if (arg == "to") {
            if (! parsePosition(line, to))
              return;

            poly->addPoint(to.p);
          }
          else if (arg == "fillcolor" || arg == "fc") {
            CRGBA c;

            if (parseColorSpec(line, c))
              poly->setFillColor(c);
          }

          arg = readNonSpace(line);
        }

        polygons_.push_back(poly);
      }
      // set object <index> rectangle
      //            {from <position> {to|rto} <position> |
      //            center|at <position> size <w>,<h> |
      else if (type == ObjectType::RECTANGLE) {
        CGnuPlotRectangleP rect = CGnuPlotRectangleP(createRectangle());

        COptValT<Position> from, to, rto;
        COptValT<Position> center;
        COptValT<CSize2D>  size;

        Position p;

        arg = readNonSpace(line);

        while (arg != "") {
          if      (arg == "from") {
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
          else if (arg == "center" || arg == "at") {
            if (parsePosition(line, p))
              center = p;
          }
          else if (arg == "size") {
            CSize2D size1;

            if (parseSize(line, size1))
              size = size1;
          }
          else if (arg == "fillcolor" || arg == "fc") {
            CRGBA c;

            if (parseColorSpec(line, c))
              rect->setFillColor(c);
          }
          else if (arg == "fillstyle" || arg == "fs") {
            int i;

            if (parseInteger(line, i))
              rect->setFillStyle(i);
          }
          else if (arg == "linewidth" || arg == "lw") {
            double lw = 1.0;

            if (parseReal(line, lw))
              rect->setLineWidth(lw);
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

        rect->setFrom(bbox.getMin());
        rect->setTo  (bbox.getMax());

        rects_.push_back(rect);
      }
    }
    else
      std::cerr << "Invalid object type" << std::endl;
  }
  // set title {"<text>"} {offset <offset>} {font "<font>{,<size>}"}
  //           {{textcolor | tc} {<colorspec> | default}} {{no}enhanced}
  else if (var == VariableName::TITLE) {
    std::string s;

    if (parseString(line, s))
      title_.str = s;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "offset") {
        double r;

        if (parseReal(line, r))
          title_.offset = r;
      }
      else if (arg == "font") {
        if (parseString(line, s, "Invalid font string"))
          title_.font = s;
      }
      else if (arg == "textcolor" || arg == "tc") {
        CRGBA c;

        if (parseColorSpec(line, c))
          title_.color = c;
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        title_.enhanced = (arg == "enhanced");
      }
    }
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

        setLineStyleInd(1);
      }
      else {
        setStyleIncrementType(StyleIncrementType::DEFAULT);

        setLineStyleInd(0);
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
      CGnuPlotArrow::ArrowStyle arrowStyle;

      CGnuPlotArrow::ArrowStyle &arrowStyle1 = arrowStyle;

      std::string arg = readNonSpace(line);

      if (CStrUtil::isInteger(arg)) {
        int arrowId = -1;

        if (CStrUtil::toInteger(arg, &arrowId))
          arrowStyle1 = arrowStyles_[arrowId];

        arg = readNonSpace(line);
      }

      while (arg != "") {
        if      (arg == "default") {
          arrowStyle1 = CGnuPlotArrow::ArrowStyle();
        }
        else if (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
          if      (arg == "nohead"  ) { arrowStyle1.fhead = false; arrowStyle1.thead = false; }
          else if (arg == "head"    ) { arrowStyle1.fhead = false; arrowStyle1.thead = true ; }
          else if (arg == "backhead") { arrowStyle1.fhead = true ; arrowStyle1.thead = false; }
          else if (arg == "heads"   ) { arrowStyle1.fhead = true ; arrowStyle1.thead = true ; }
        }
        else if (arg == "size") {
          double r = 1.0;

          if (parseReal(line, r))
            arrowStyle1.length = r;

          if (line.isChar(',')) {
            line.skipChar();

            if (parseReal(line, r))
              arrowStyle1.angle = r;

            if (line.isChar(',')) {
              line.skipChar();

              if (parseReal(line, r))
                arrowStyle1.backAngle = r;
            }
          }
        }
        else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
          arrowStyle1.filled = (arg == "filled" || arg == "empty");
          arrowStyle1.empty  = (arg == "empty" );
        }
        else if (arg == "front" || arg == "back") {
          arrowStyle1.front = (arg == "front");
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            arrowStyle1.lineType = lt;
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            arrowStyle1.lineWidth = lw;
        }
        else if (arg == "linestyle" || arg == "ls") {
          int i = -1;

          if (! parseInteger(line, i))
            arrowStyle1.lineStyle = i;
        }
        else {
          std::cerr << "Invalid arg '" << arg << "'" << std::endl;
          break;
        }

        arg = readNonSpace(line);
      }
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

          if (parseInteger(line, patternNum))
            fillStyle().setPattern((FillPattern) patternNum);

          line.skipSpace();
        }
      }

      if (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "border") {
          int fillBorderLine = -1;

          line.skipSpace();

          if (line.isValid()) {
            int i;

            if (parseInteger(line, i))
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
            double lw;

            if (! parseReal(line, lw))
              lw = 0;
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

      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      line.skipSpace();

      PointStyle pointStyle;
      double     r;

      while (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "linetype" || arg == "lt") {
          int i;

          if (parseInteger(line, i))
            lineStyle->setType(i);
        }
        else if (arg == "linewidth" || arg == "lw") {
          if (parseReal(line, r))
            lineStyle->setWidth(r);
        }
        else if (arg == "pointtype" || arg == "pt") {
          std::string typeStr = readNonSpace(line);

          if (CStrUtil::toInteger(typeStr, &l))
            lineStyle->setPointType(static_cast<CGnuPlotTypes::SymbolType>(l));
        }
        else if (arg == "pointsize" || arg == "ps") {
          if (parseReal(line, r))
            lineStyle->setPointSize(r);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CRGBA c;

          if (parseColorSpec(line, c))
            lineStyle->setColor(c);
        }
        else {
          std::cerr << "Invalid line modifier '" << arg << "'" << std::endl;
          break;
        }

        line.skipSpace();
      }

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
  // set boxwidth [ {size:r} ] [ absolute | relative ]
  else if (var == VariableName::BOXWIDTH) {
    BoxWidthType boxWidthType = BoxWidthType::AUTO;
    double       boxWidth     = getBoxWidth().width;

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
        else
          boxWidthType = BoxWidthType::ABSOLUTE;
      }
    }

    setBoxWidth(BoxWidth(boxWidth, boxWidthType));
  }
  // set pointsize <size>
  else if (var == VariableName::POINTSIZE) {
    double r = 1.0;

    if (parseReal(line, r))
      lineStyle()->setPointSize(r > 0 ? r : 1.0);
  }
  // set autoscale {<axes>{|min|max|fixmin|fixmax|fix} | fix | keepfix}
  else if (var == VariableName::AUTOSCALE) {
    std::string arg = readNonSpace(line);

    if      (arg == "x") {
      axesData().xaxis.min.setInvalid();
      axesData().xaxis.max.setInvalid();
    }
    else if (arg == "y") {
      axesData().yaxis.min.setInvalid();
      axesData().yaxis.max.setInvalid();
    }
  }

  // set format {<axes>} {"<format-string>"}
  else if (var == VariableName::FORMAT) {
    std::string arg = readNonSpace(line);

    std::string formatStr;

    if (parseString(line, formatStr, "Invalid format string")) {
      if      (arg == "x") axesData().xaxis.format = formatStr;
      else if (arg == "y") axesData().yaxis.format = formatStr;
    }
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

    if (parseString(line, labelStr, "Invalid xlabel string"))
      axesData().xaxis.str = labelStr;
  }
  // set ylabel "<label>"
  else if (var == VariableName::YLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData().yaxis.str = labelStr;
  }
  // set x2label "<label>"
  else if (var == VariableName::X2LABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid x2label string"))
      axesData().x2axis.str = labelStr;
  }
  // set y2label "<label>"
  else if (var == VariableName::Y2LABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData().y2axis.str = labelStr;
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

      decodeXRange(fields);
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

      decodeYRange(fields);
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
    parseAxesTics(line, axesData().xaxis);
  }
  else if (var == VariableName::YTICS) {
    parseAxesTics(line, axesData().yaxis);
  }
  else if (var == VariableName::X2TICS) {
    parseAxesTics(line, axesData().x2axis);
  }
  else if (var == VariableName::Y2TICS) {
    parseAxesTics(line, axesData().y2axis);
  }

  // set timefmt "<format string>"
  //
  // Format  Description
  // ------  -----------
  // %d      day of the month, 1--31
  // %m      month of the year, 1--12
  // %y      year, 0--99
  // %Y      year, 4-digit
  // %j      day of the year, 1--365
  // %H      hour, 0--24
  // %M      minute, 0--60
  // %s      seconds since the Unix epoch (1970-01-01, 00:00 UTC)
  // %S      second, integer 0--60 on output, (double) on input
  // %b      three-character abbreviation of the name of the month
  // %B      name of the month
  else if (var == VariableName::TIMEFMT) {
    std::string fmt;

    if (parseString(line, fmt, "Invalid time format"))
      timeFmt_ = fmt;
  }
  // set xdata {time}
  else if (var == VariableName::XDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData().xaxis.isTime = true;
  }
  // set ydata {time}
  else if (var == VariableName::YDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData().yaxis.isTime = true;
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

    if (parseString(line, filename, "Invalid filename"))
      setOutputFile(filename);
  }
  // set print ["<filename>"] [append]
  else if (var == VariableName::PRINT) {
    std::string filename;

    if (parseString(line, filename, "Invalid filename")) {
      setPrintFile(filename);

      std::string arg = readNonSpace(line);

      if (arg == "append")
        setPrintAppend(true);
    }
  }
  // set table ["<filename>"]
  else if (var == VariableName::TABLE) {
    std::string filename;

    if (parseString(line, filename, "Invalid filename"))
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
       std::cerr << "arrow " << arrow->getInd() << "," <<
                    (arrow->getFHead() ? (arrow->getTHead() ? " heads" : " backhead") :
                                         (arrow->getTHead() ? " head"  : " nohead"  )) <<
                    (! arrow->getFilled() ? (! arrow->getEmpty() ? " nofilled" : " empty") :
                                             " filled") <<
                    (arrow->getFront() ? " front" : " back") <<
                    " linetype " << arrow->getLineType() <<
                    " linewidth " << arrow->getLineWidth() <<
                    " from " << arrow->getFrom() <<
                    (arrow->getRelative() ? " rto " : " to ") << arrow->getTo();

       if (arrow->getLength() > 0.0 || arrow->getAngle() >= 0.0 || arrow->getBackAngle() >= 0.0) {
         std::cerr << " arrow head:";

         if (arrow->getLength()    >  0.0)
           std::cerr << " length " << arrow->getLength() << " deg";
         if (arrow->getAngle()     >= 0.0)
           std::cerr << " angle " << arrow->getAngle() << " deg";
         if (arrow->getBackAngle() >= 0.0)
           std::cerr << " backangle " << arrow->getBackAngle() << " deg";
       }

       std::cerr << std::endl;
    }
  }
  else if (var == VariableName::LABEL) {
    for (const auto &label : labels_) {
       std::cerr << "label " << label->getInd() << " \"" << label->getText() << "\"" <<
                    " at " << label->getPos();

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
    std::cerr << "pointsize is " << lineStyle()->pointSize() << std::endl;
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

  if      (var == VariableName::MULTIPLOT) {
    multiplot().enabled = false;

    multiWindow_ = 0;
  }
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
pauseCmd(const std::string &args)
{
  CParseLine line(args);

  int secs;

  if (! parseInteger(line, secs))
    secs = -1;

  std::string msg;

  if (! parseString(line, msg, "Invalid message"))
    return;

  prompt(msg);
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

CGnuPlotLineStyle *
CGnuPlot::
createLineStyle()
{
  return new CGnuPlotLineStyle;
}

CGnuPlotArrow *
CGnuPlot::
createArrow()
{
  return new CGnuPlotArrow;
}

CGnuPlotLabel *
CGnuPlot::
createLabel()
{
  return new CGnuPlotLabel;
}

CGnuPlotEllipse *
CGnuPlot::
createEllipse()
{
  return new CGnuPlotEllipse;
}

CGnuPlotPolygon *
CGnuPlot::
createPolygon()
{
  return new CGnuPlotPolygon;
}

CGnuPlotRectangle *
CGnuPlot::
createRectangle()
{
  return new CGnuPlotRectangle;
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

  if (function == "NaN") {
    for (int i = 0; i <= nx; ++i, x += d)
      plot->addPoint2D(x, CExprValuePtr());
  }
  else {
    CExprPTokenStack pstack = CExprInst->parseLine(function);
    CExprITokenPtr   itoken = CExprInst->interpPTokenStack(pstack);
    CExprCTokenStack cstack = CExprInst->compileIToken(itoken);

    for (int i = 0; i <= nx; ++i, x += d) {
      xvar->setRealValue(x);

      CExprValuePtr value = CExprInst->executeCTokenStack(cstack);

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
  bool          firstLine     = true;

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

    std::vector<CExprValuePtr> values;

    for (uint i = 0; i < usingCols.size(); ++i) {
      CExprValuePtr value = decodeUsingCol(i, usingCols[i], setNum, pointNum, skip);

      if (skip) continue;

      if (value.isValid())
        values.push_back(value);
    }

    if (values.empty()) {
      CExprValuePtr value1 = getFieldValue(0, 1, setNum, pointNum, skip);

      if (value1.isValid())
        values.push_back(value1);

      CExprValuePtr value2 = getFieldValue(1, 2, setNum, pointNum, skip);

      if (value2.isValid())
        values.push_back(value2);
    }

    if (values.size() == 1) {
      CExprValuePtr value1 = getFieldValue(0, 0, setNum, pointNum, skip);

      if (value1.isValid()) {
        values.push_back(value1);

        std::swap(values[0], values[1]);
      }
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
        if      (keyData_.columnhead && usingCols.size() > 1 && usingCols[1].isInt &&
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

    //---

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

  for (auto plot : plots)
    plot->setAxesData(axesData());

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

CExprValuePtr
CGnuPlot::
decodeUsingCol(int i, const CGnuPlot::UsingCol &col, int setNum, int pointNum, bool &skip)
{
  CExprValuePtr value;

  skip = false;

  if (col.isInt)
    value = getFieldValue(i, col.ival, setNum, pointNum, skip);
  else {
    std::string expr = col.str;

    // replace $N variables
    // TODO: easier to define $1 variables
    auto pos = expr.find('$');

    while (pos != std::string::npos) {
      int pos1 = ++pos;

      while (isdigit(expr[pos1]))
        ++pos1;

      std::string numStr = expr.substr(pos, pos1 - pos);

      int icol = 0;

      (void) CStrUtil::toInteger(numStr, &icol);

      CExprValuePtr value1 = getFieldValue(i, icol, setNum, pointNum, skip);

      double x1;

      if (value1.isValid() && value1->getRealValue(x1))
        expr = expr.substr(0, pos - 1) + CStrUtil::toString(x1) + expr.substr(pos1);
      else
        expr = expr.substr(0, pos - 1) + "NaN" + expr.substr(pos1);

      pos = expr.find('$');
    }

    // check for axis label string
    pos = expr.find('(');

    if (pos != std::string::npos) {
      std::string name = expr.substr(0, pos);

      if (name == "xtic" || name == "xticlabels" || name == "ytic" || name == "yticlabels") {
        std::string name1 = expr.substr(pos + 1);

        auto p1 = name1.find(')');

        if (p1 != std::string::npos)
          name1 = name1.substr(0, p1);

        CExprValuePtr value = CExprInst->evaluateExpression(name1);

        long icol;

        if (value.isValid() && value->getIntegerValue(icol)) {
          CExprValuePtr value1 = getFieldValue(i, icol, setNum, pointNum, skip);

          std::string str1;

          if (value1.isValid() && value1->getStringValue(str1)) {
            if (name == "xtic" || name == "xticlabels")
              axesData().xaxis.ticlabel[pointNum] = str1;
            else
              axesData().yaxis.ticlabel[pointNum] = str1;
          }
        }

        expr = "";
      }
    }

    //----

    if (expr != "") {
      if (isDebug())
        std::cerr << "expr " << expr << std::endl;

      value = CExprInst->evaluateExpression(expr);
    }
  }

  return value;
}

CExprValuePtr
CGnuPlot::
getFieldValue(int i, int ival, int setNum, int pointNum, bool &skip)
{
  skip = false;

  CExprValuePtr value;

  int nf = fields_.size();

  if      (ival == 0)
    value = CExprInst->createRealValue(pointNum);
  else if (ival == -2)
    value = CExprInst->createRealValue(setNum);
  else if (ival > 0 && ival <= nf) {
    const std::string &missing = getMissingStr();

    const std::string &field = fields_[ival - 1];

    if (field == missing) {
      skip = true;
      return CExprValuePtr();
    }

    double val;

    if      ((i == 0 && axesData().xaxis.isTime) || (i == 1 && axesData().yaxis.isTime)) {
      struct tm tm1; memset(&tm1, 0, sizeof(tm));

      strptime(field.c_str(), timeFmt_.c_str(), &tm1);

      value = CExprInst->createRealValue(mktime(&tm1));
    }
    else if (CStrUtil::toReal(field, &val))
      value = CExprInst->createRealValue(val);
    else
      value = CExprInst->createStringValue(field);
  }

  return value;
}

CGnuPlotLineStyleP
CGnuPlot::
lineStyle()
{
  if (! lineStyle_)
    lineStyle_ = createLineStyle();

  return lineStyle_;
}

CGnuPlotLineStyleP
CGnuPlot::
getLineStyleInd(int ind)
{
  CGnuPlotLineStyleP lineStyle = this->lineStyle(ind);

  if (! lineStyle) {
    lineStyle = createLineStyle();

    lineStyle->setInd(ind);

    lineStyles_[ind] = lineStyle;
  }

  return lineStyle;
}

void
CGnuPlot::
setLineStyleInd(int ind)
{
  lineStyle_ = getLineStyleInd(ind);
}

CGnuPlotLineStyleP
CGnuPlot::
lineStyle(int i) const
{
  auto p = lineStyles_.find(i);

  return (p != lineStyles_.end() ? (*p).second : CGnuPlotLineStyleP());
}

void
CGnuPlot::
incLineStyle()
{
  ++styleIncrement_.colorInd;

  lineStyle()->setColor(CGnuPlotStyle::instance()->indexColor(styleIncrement_.colorInd));

  if (fillStyle().style() == FillType::PATTERN)
    fillStyle().setPattern(static_cast<FillPattern>(static_cast<int>(fillStyle().pattern()) + 1));

  setLineStyleInd(lineStyle()->ind() + 1);
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
bool
CGnuPlot::
parseAxesTics(CParseLine &line, AxisData& axis)
{
  axis.showTics = true;

  std::string arg = readNonSpace(line);

  while (arg != "") {
    if      (arg == "axis"  ) { }
    else if (arg == "border") { }
    else if (arg == "mirror" || arg == "nomirror") {
      axis.mirror = (arg == "mirror");
    }
    else if (arg == "in") { }
    else if (arg == "out") { }
    else if (arg == "scale") {
      std::string arg1 = readNonSpaceNonComma(line);

      line.skipSpace();

      if (line.isChar(','))
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "rotate") {
      std::string arg1 = readNonSpaceNonComma(line);

      if (arg1 == "by")
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "norotate") { }
    else if (arg == "offset") {
      std::string arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "nooffset") { }
    else if (arg == "left") { }
    else if (arg == "right") { }
    else if (arg == "center") { }
    else if (arg == "autojustify") { }
    else if (arg == "add") { }
    else if (arg == "autofreq") { }
    else if (arg == "format") {
      std::string formatStr;

      if (parseString(line, formatStr, "Invalid format")) {
      }
    }
    else if (arg == "font") {
      std::string fontStr;

      if (parseString(line, fontStr, "Invalid font")) {
      }
    }
    else if (arg == "rangelimited") { }
    else if (arg == "textcolor") {
      std::string arg1 = readNonSpaceNonComma(line);
    }

    arg = readNonSpace(line);
  }

  return true;
}

bool
CGnuPlot::
parseColorSpec(CParseLine &line, CRGBA &c)
{
  std::string colorTypeStr = readNonSpaceNonComma(line);

  if (colorTypeStr == "rgbcolor" || colorTypeStr == "rgb") {
    std::string colorValueStr;

    if (parseString(line, colorValueStr, "Invalid color string")) {
      double r, g, b;

      if (! CRGBName::lookup(colorValueStr, &r, &g, &b)) {
        std::cerr << "Invalid color string" << std::endl;
        return false;
      }

      c = CRGBA(r, g, b);
    }
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

    if (parseString(line, name, "Invalid color string")) {
      c = CRGBName::toRGBA(name);
      return true;
    }
  }

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
parseString(CParseLine &line, std::string &str, const std::string &msg)
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
    if (msg != "")
      std::cerr << msg << std::endl;

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
