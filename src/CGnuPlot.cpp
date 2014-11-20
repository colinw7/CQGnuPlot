#include <CGnuPlot.h>
#include <CUnixFile.h>
#include <CParseLine.h>
#include <CAxis2D.h>
#include <CDrawAxis2D.h>
#include <CDir.h>
#include <CReadLine.h>
#include <CStrUniqueMatch.h>
#include <CExpr.h>

#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>

//------

namespace CGnuPlotUtil {
  void initNameValues(CStrUniqueMatch<CGnuPlot::CommandName> &nameValues) {
    nameValues.addValue("bind"   , CGnuPlot::BIND_CMD);
    nameValues.addValue("call"   , CGnuPlot::CALL_CMD);
    nameValues.addValue("cd"     , CGnuPlot::CD_CMD);
    nameValues.addValue("clear"  , CGnuPlot::CLEAR_CMD);
    nameValues.addValue("exit"   , CGnuPlot::EXIT_CMD);
    nameValues.addValue("fit"    , CGnuPlot::FIT_CMD);
    nameValues.addValue("help"   , CGnuPlot::HELP_CMD);
    nameValues.addValue("history", CGnuPlot::HISTORY_CMD);
    nameValues.addValue("if"     , CGnuPlot::IF_CMD);
    nameValues.addValue("load"   , CGnuPlot::LOAD_CMD);
    nameValues.addValue("lower"  , CGnuPlot::LOWER_CMD);
    nameValues.addValue("pause"  , CGnuPlot::PAUSE_CMD);
    nameValues.addValue("plot"   , CGnuPlot::PLOT_CMD);
    nameValues.addValue("print"  , CGnuPlot::PRINT_CMD);
    nameValues.addValue("pwd"    , CGnuPlot::PWD_CMD);
    nameValues.addValue("quit"   , CGnuPlot::QUIT_CMD);
    nameValues.addValue("raise"  , CGnuPlot::RAISE_CMD);
    nameValues.addValue("replot" , CGnuPlot::REPLOT_CMD);
    nameValues.addValue("reread" , CGnuPlot::REREAD_CMD);
    nameValues.addValue("reset"  , CGnuPlot::RESET_CMD);
    nameValues.addValue("save"   , CGnuPlot::SAVE_CMD);
    nameValues.addValue("set"    , CGnuPlot::SET_CMD);
    nameValues.addValue("shell"  , CGnuPlot::SHELL_CMD);
    nameValues.addValue("show"   , CGnuPlot::SHOW_CMD);
    nameValues.addValue("splot"  , CGnuPlot::SPLOT_CMD);
    nameValues.addValue("system" , CGnuPlot::SYSTEM_CMD);
    nameValues.addValue("test"   , CGnuPlot::TEST_CMD);
    nameValues.addValue("unset"  , CGnuPlot::UNSET_CMD);
    nameValues.addValue("update" , CGnuPlot::UPDATE_CMD);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::VariableName> &nameValues) {
    nameValues.addValue("datafile"   , CGnuPlot::DATAFILE_VAR);
    nameValues.addValue("decimalsign", CGnuPlot::DECIMALSIGN_VAR);
    nameValues.addValue("encoding"   , CGnuPlot::ENCODING_VAR);
    nameValues.addValue("fit"        , CGnuPlot::FIT_VAR);
    nameValues.addValue("fontpath"   , CGnuPlot::FONTPATH_VAR);
    nameValues.addValue("historysize", CGnuPlot::HISTORYSIZE_VAR);
    nameValues.addValue("loadpath"   , CGnuPlot::LOADPATH_VAR);
    nameValues.addValue("macros"     , CGnuPlot::MACROS_VAR);
    nameValues.addValue("mouse"      , CGnuPlot::MOUSE_VAR);

    nameValues.addValue("angles"     , CGnuPlot::ANGLES_VAR);
    nameValues.addValue("dummy"      , CGnuPlot::DUMMY_VAR);
    nameValues.addValue("mapping"    , CGnuPlot::MAPPING_VAR);
    nameValues.addValue("parametric" , CGnuPlot::PARAMETRIC_VAR);
    nameValues.addValue("polar"      , CGnuPlot::POLAR_VAR);
    nameValues.addValue("samples"    , CGnuPlot::SAMPLES_VAR);
    nameValues.addValue("zero"       , CGnuPlot::ZERO_VAR);

    nameValues.addValue("style"      , CGnuPlot::STYLE_VAR);
    nameValues.addValue("pointsize"  , CGnuPlot::POINT_SIZE_VAR);
    nameValues.addValue("table"      , CGnuPlot::TABLE_VAR);
    nameValues.addValue("boxwidth"   , CGnuPlot::BOX_WIDTH_VAR);
    nameValues.addValue("variables"  , CGnuPlot::VARIABLES_VAR);
    nameValues.addValue("functions"  , CGnuPlot::FUNCTIONS_VAR);
    nameValues.addValue("version"    , CGnuPlot::VERSION_VAR);

    nameValues.addValue("debug"      , CGnuPlot::DEBUG_VAR);
    nameValues.addValue("edebug"     , CGnuPlot::EDEBUG_VAR);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::DataFileVar> &nameValues) {
    nameValues.addValue("separator"   , CGnuPlot::DATAFILE_SEPARATOR_VAR);
    nameValues.addValue("commentschar", CGnuPlot::DATAFILE_COMMENTS_CHAR_VAR);
    nameValues.addValue("missing"     , CGnuPlot::DATAFILE_MISSING_VAR);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::StyleVar> &nameValues) {
    nameValues.addValue("data"     , CGnuPlot::STYLE_DATA_VAR);
    nameValues.addValue("function" , CGnuPlot::STYLE_FUNCTION_VAR);
    nameValues.addValue("line"     , CGnuPlot::STYLE_LINE_VAR);
    nameValues.addValue("fill"     , CGnuPlot::STYLE_FILL_VAR);
    nameValues.addValue("histogram", CGnuPlot::STYLE_HISTOGRAM_VAR);
    nameValues.addValue("rectangle", CGnuPlot::STYLE_RECTANGLE_VAR);
    nameValues.addValue("circle"   , CGnuPlot::STYLE_CIRCLE_VAR);
    nameValues.addValue("ellipse"  , CGnuPlot::STYLE_ELLIPSE_VAR);
    nameValues.addValue("arrow"    , CGnuPlot::STYLE_ARROW_VAR);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotStyle> &nameValues) {
    nameValues.addValue("lines"      , CGnuPlot::LINES_STYLE);
    nameValues.addValue("points"     , CGnuPlot::POINTS_STYLE);
    nameValues.addValue("linespoints", CGnuPlot::LINES_POINTS_STYLE);
    nameValues.addValue("dots"       , CGnuPlot::DOTS_STYLE);
    nameValues.addValue("labels"     , CGnuPlot::LABELS_STYLE);
    nameValues.addValue("histograms" , CGnuPlot::HISTOGRAMS_STYLE);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::Smooth> &nameValues) {
    nameValues.addValue("unique"   , CGnuPlot::SMOOTH_UNIQUE);
    nameValues.addValue("frequency", CGnuPlot::SMOOTH_FREQUENCY);
    nameValues.addValue("bezier"   , CGnuPlot::SMOOTH_BEZIER);
    nameValues.addValue("sbezier"  , CGnuPlot::SMOOTH_SBEZIER);
    nameValues.addValue("csplines" , CGnuPlot::SMOOTH_CSPLINES);
    nameValues.addValue("acsplines", CGnuPlot::SMOOTH_ACSPLINES);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::FillStyle> &nameValues) {
    nameValues.addValue("empty"  , CGnuPlot::FILL_EMPTY);
    nameValues.addValue("solid"  , CGnuPlot::FILL_SOLID);
    nameValues.addValue("pattern", CGnuPlot::FILL_PATTERN);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::HistogramStyle> &nameValues) {
    nameValues.addValue("clustered"    , CGnuPlot::HISTOGRAM_STYLE_CLUSTERED);
    nameValues.addValue("errorbars"    , CGnuPlot::HISTOGRAM_STYLE_ERRORBARS);
    nameValues.addValue("rowstacked"   , CGnuPlot::HISTOGRAM_STYLE_ROWSTACKED);
    nameValues.addValue("columnstacked", CGnuPlot::HISTOGRAM_STYLE_COLUMNSTACKED);
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
  std::string valueToString(const T &value) {
    return getNameValues<T>().lookup(value);
  }

  template<typename T>
  std::string valueStrings() {
    std::vector<std::string> strs;

    getNameValues<T>().values(strs);

    std::string str;

    for (uint i = 0; i < strs.size(); ++i) {
      if (i > 0) str += ", ";

      str += "'" + strs[i] + "'";
    }

    return str;
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

        if (fields[col.ival - 1] == missing) {
          skip = true;
          return CExprValuePtr();
        }

        if (CStrUtil::toReal(fields[col.ival - 1], &val))
          return CExprInst->createRealValue(val);
      }

      return CExprValuePtr();
    }
    else {
      std::string expr = col.str;

      // TODO: easier to define $1 variables
      std::string::size_type pos = expr.find('$');

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

          if (fields[icol - 1] != missing && CStrUtil::toReal(fields[icol - 1], &x2))
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

class CGnuPlotDrawAxis : public CDrawAxis2D {
 public:
  CGnuPlotDrawAxis(CGnuPlotRenderer *renderer) :
   renderer_(renderer) {
  }

  void getFont(CFontPtr &font) { font = renderer_->getFont(); }
  void setFont(CFontPtr font) { renderer_->setFont(font); }

  double pixelWidthToWindowWidth(double w) {
    return renderer_->pixelWidthToWindowWidth(w);
  }

  double pixelHeightToWindowHeight(double h) {
    return renderer_->pixelHeightToWindowHeight(h);
  }

  void setLineDash(const CLineDash &) { }

  void drawLine(const CPoint2D &p1, const CPoint2D &p2) {
    renderer_->drawLine(p1, p2);
  }

  void drawRect(const CBBox2D &rect) {
    renderer_->drawRect(rect);
  }

  void fillText(const CPoint2D &p, const std::string &text) {
    renderer_->drawText(p, text);
  }

 private:
  CGnuPlotRenderer *renderer_;
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
CGnuPlot() :
 debug_(false), separator_(), commentChars_("#"),
 dataStyle_(POINTS_STYLE), functionStyle_(LINES_STYLE), fillStyle_(FILL_EMPTY), fillBorder_(true),
 smooth_(SMOOTH_NONE), fitX_(true), fitY_(true), isamples1_(100), isamples2_(100)
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

void
CGnuPlot::
setRenderer(CGnuPlotRenderer *renderer)
{
  renderer_ = renderer;
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
    commandName = NO_CMD;

  // process command
  switch (commandName) {
    case EXIT_CMD   : exitCmd   (args); break;
    case HELP_CMD   : helpCmd   (args); break;
    case HISTORY_CMD: historyCmd(args); break;
    case PRINT_CMD  : printCmd  (args); break;
    case QUIT_CMD   : quitCmd   (args); break;
    case CD_CMD     : cdCmd     (args); break;
    case PWD_CMD    : pwdCmd    (args); break;

    case CALL_CMD   : callCmd(args); break;
    case LOAD_CMD   : loadCmd(args); break;
    case SAVE_CMD   : saveCmd(args); break;

    case PLOT_CMD   : plotCmd  (args); break;
    case REPLOT_CMD : replotCmd(args); break;
    case SPLOT_CMD  : splotCmd (args); break;

    case SET_CMD    : setCmd(args); break;
    case SHOW_CMD   : showCmd(args); break;
    case RESET_CMD  : resetCmd(args); break;
    case UNSET_CMD  : unsetCmd(args); break;

    case SHELL_CMD  : shellCmd(args); break;
    case SYSTEM_CMD : systemCmd(args); break;

    case CLEAR_CMD  : clearCmd(args); break;
    case LOWER_CMD  : lowerCmd(args); break;
    case RAISE_CMD  : raiseCmd(args); break;
    case TEST_CMD   : testCmd (args); break;

    case FIT_CMD    : fitCmd   (args); break;
    case UPDATE_CMD : updateCmd(args); break;

    case BIND_CMD   : bindCmd( args); break;
    case IF_CMD     : ifCmd    (args); break;
    case PAUSE_CMD  : pauseCmd (args); break;
    case REREAD_CMD : rereadCmd(args); break;
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
  std::string str;

  parseString(args, str);

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

  while (line.isValid() && ! line.isSpace())
    file += line.getChar();

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg;

    while (line.isValid() && ! line.isSpace())
      arg += line.getChar();

    args1.push_back(arg);
  }
}

// load "<filename>"
void
CGnuPlot::
loadCmd(const std::string &)
{
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
bool
CGnuPlot::
plotCmd(const std::string &args)
{
  CParseLine line(args);

  std::string lastFilename;

  setFitX(true);
  setFitY(true);

  while (true) {
    if (line.isChar('[')) {
      // ranges
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
        setXRange(xmin, xmax);
        setFitX(false);
      }

      if (yfields.size() == 2) {
        double ymin = -1, ymax = 1;
        (void) CStrUtil::toReal(yfields[0], &ymin);
        (void) CStrUtil::toReal(yfields[1], &ymax);
        if (isDebug())
          std::cerr << "YRange=(" << ymin << "," << ymax << ")" << std::endl;
        setYRange(ymin, ymax);
        setFitY(false);
      }

      line.skipSpace();
    }

    //----

    PlotStyle style(POINTS_STYLE);

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
      while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
        function += line.getChar();

      if (isDebug())
        std::cerr << "Function: " << function << std::endl;

      style = getFunctionStyle();
    }

    line.skipSpace();

    //---

    UsingCol col1(1), col2(2), col3(3);

    int indexStart = 1, indexEnd = 1      , indexStep = 1;
    int everyStart = 0, everyEnd = INT_MAX, everyStep = 1;

    setSmooth(SMOOTH_NONE);

    // TODO: using must come before title and with
    // can use any unique abbreviation for command w, wi, wit, with

    while (line.isValid() && ! line.isChar(',')) {
      if      (line.isString("using ") || line.isString("u ")) {
        if (line.isString("using "))
          line.incPos(6);
        else
          line.incPos(2);

        line.skipSpace();

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

        if      (usingStrs.size() > 2) {
          col1 = UsingCol(usingStrs[0]);
          col2 = UsingCol(usingStrs[1]);
          col3 = UsingCol(usingStrs[2]);
        }
        else if (usingStrs.size() > 1) {
          col1 = UsingCol(usingStrs[0]);
          col2 = UsingCol(usingStrs[1]);
        }
        else if (usingStrs.size() > 0) {
          col1 = UsingCol(0);
          col2 = UsingCol(usingStrs[0]);
        }
      }
      else if (line.isString("index ") || line.isString("i ")) {
        if (line.isString("index "))
          line.incPos(6);
        else
          line.incPos(2);

        line.skipSpace();

        std::string indexStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          indexStr += line.getChar();

        if (isDebug())
          std::cerr << "index " << indexStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(indexStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &indexStart);
          (void) CStrUtil::toInteger(inds[1], &indexEnd);
          (void) CStrUtil::toInteger(inds[2], &indexStep);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &indexStart);
          (void) CStrUtil::toInteger(inds[1], &indexEnd);
          indexStep = 1;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &indexStart);
          indexEnd  = indexStart;
          indexStep = 1;
        }
      }
      else if (line.isString("every ") || line.isString("e ")) {
        if (line.isString("every "))
          line.incPos(6);
        else
          line.incPos(2);

        line.skipSpace();

        std::string everyStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          everyStr += line.getChar();

        if (isDebug())
          std::cerr << "every " << everyStr << std::endl;

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(everyStr, inds, ":", /*skipEmpty*/true);

        if      (inds.size() > 2) {
          (void) CStrUtil::toInteger(inds[0], &everyStep);
          (void) CStrUtil::toInteger(inds[1], &everyStart);
          (void) CStrUtil::toInteger(inds[2], &everyEnd);
        }
        else if (inds.size() > 1) {
          (void) CStrUtil::toInteger(inds[0], &everyStep);
          (void) CStrUtil::toInteger(inds[1], &everyStart);
          everyEnd = INT_MAX;
        }
        else if (inds.size() > 0) {
          (void) CStrUtil::toInteger(inds[0], &everyStep);
          everyStart = 0;
          everyEnd   = INT_MAX;
        }
      }
      else if (line.isString("with ") || line.isString("w ")) {
        if (line.isString("with "))
          line.incPos(5);
        else
          line.incPos(2);

        line.skipSpace();

        std::string withStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          withStr += line.getChar();

        if (isDebug())
          std::cerr << "with " << withStr << std::endl;

        line.skipSpace();

        std::vector<std::string> styles;

        CStrUtil::addFields(withStr, styles, ":", /*skipEmpty*/true);

        for (uint i = 0; i < styles.size(); ++i) {
          if (! CGnuPlotUtil::stringToValue(styles[i], style))
            std::cerr << "Invalid plot style '" << styles[i] << "'" << std::endl;
        }
      }
      else if (line.isString("title ") || line.isString("t ")) {
        if (line.isString("title "))
          line.incPos(6);
        else
          line.incPos(2);

        line.skipSpace();

        std::string titleStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          titleStr += line.getChar();

        if (isDebug())
          std::cerr << "title " << titleStr << std::endl;

        line.skipSpace();
      }
      else if (line.isString("smooth ") || line.isString("s ")) {
        if (line.isString("smooth "))
          line.incPos(7);
        else
          line.incPos(2);

        line.skipSpace();

        std::string smoothStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          smoothStr += line.getChar();

        if (isDebug())
          std::cerr << "smooth " << smoothStr << std::endl;

        Smooth smooth;

        if (CGnuPlotUtil::stringToValue(smoothStr, smooth))
          setSmooth(smooth);
        else
          std::cerr << "Invalid smooth type" << std::endl;

        line.skipSpace();
      }
      else {
        std::cerr << "Invalid character " << line.lookChar() << std::endl;
        break;
      }
    }

    //---

    if      (! filename.empty())
      (void) addFile(filename, style, col1, col2, col3, indexStart, indexEnd, indexStep,
                     everyStart, everyEnd, everyStep);
    else if (! function.empty())
      (void) addFunction(function, style);

    if (! line.isChar(','))
      break;

    line.skipChar();

    line.skipSpace();
  }

  return true;
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
splotCmd(const std::string &)
{
}

// set ...
void
CGnuPlot::
setCmd(const std::string &args)
{
  CParseLine line(args);

  line.skipSpace();

  std::string option;

  while (line.isValid() && ! line.isSpace())
    option += line.getChar();

  line.skipSpace();

  std::string optionStr = line.substr();

  //---

  std::vector<std::string> args1;

  args1.push_back(option);

  while (line.isValid()) {
    std::string arg;

    while (line.isValid() && ! line.isSpace())
      arg += line.getChar();

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  //---

  if (option.empty()) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  VariableName var;

  if (! CGnuPlotUtil::stringToValue(option, var))
    var = NO_VAR;

  // --- Configuration ---

  // set datafile commentschar ["<chars>"]
  // set datafile separator ["<chars>"|whitespace]
  // set datafile missing ["<chars>]
  if      (var == DATAFILE_VAR) {
    if (args1.size() == 1) {
      std::cerr << "Missing modifier: " << CGnuPlotUtil::valueStrings<DataFileVar>() << std::endl;
      return;
    }

    DataFileVar var1;

    if (! CGnuPlotUtil::stringToValue(args1[1], var1))
      var1 = DATAFILE_NO_VAR;

    if      (var1 == DATAFILE_SEPARATOR_VAR) {
      if (args1.size() > 3)
        std::cerr << "Ignoring extra arguments." << std::endl;

      if (args1.size() == 2 || args1[2] == "whitespace")
        setSeparator(Separator());
      else {
        std::string chars;

        if (! parseString(args1[2], chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        if (chars.size() != 1) {
          std::cerr << "Only one character allowed for separator" << std::endl;
          return;
        }

        setSeparator(Separator(chars[0]));
      }
    }
    else if (var1 == DATAFILE_COMMENTS_CHAR_VAR) {
      if (args1.size() > 3)
        std::cerr << "Ignoring extra arguments." << std::endl;

      if (args1.size() == 2)
        setCommentChars("#");
      else {
        std::string chars;

        if (! parseString(args1[2], chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        setCommentChars(chars);
      }
    }
    else if (var1 == DATAFILE_MISSING_VAR) {
      if (args1.size() > 3)
        std::cerr << "Ignoring extra arguments." << std::endl;

      if (args1.size() == 2)
        setMissingStr("");
      else {
        std::string chars;

        if (! parseString(args1[2], chars)) {
          std::cerr << "Invalid character string" << std::endl;
          return;
        }

        setMissingStr(chars);
      }
    }
  }
  // set decimalsign [ "<char>" | locale [ "<locale>" ] ]
  else if (var == DECIMALSIGN_VAR) {
  }
  // set encoding [ default | iso_8859_1 | iso_8859_2 | iso_8859_15 |
  //                koi8r | koi8u | cp437 | cp850 | cp852 | cp1250 ]
  else if (var == ENCODING_VAR) {
  }
  // set fit [ logfile "<filename>" ] [ [no]errorvariables ]
  else if (var == FIT_VAR) {
  }
  // set fontpath [ "<path>" ]
  else if (var == FONTPATH_VAR) {
  }
  // set historysize <size:i>
  else if (var == HISTORYSIZE_VAR) {
  }
  // set loadpath [ "<path>" ]
  else if (var == LOADPATH_VAR) {
  }
  // set macros
  else if (var == MACROS_VAR) {
  }
  // set mouse
  else if (var == MOUSE_VAR) {
  }

  // --- Math ---

  // set angles [ radians | degrees ]
  else if (var == ANGLES_VAR) {
  }
  // set dummy <var1> [, <var2>]
  else if (var == DUMMY_VAR) {
    CParseLine line1(optionStr);

    line1.skipSpace();

    std::string var1, var2;

    getDummyVars(var1, var2);

    if (line1.isValid()) {
      var1 = "";

      while (line1.isValid() && ! line1.isSpace() && ! line1.isChar(','))
        var1 += line1.getChar();

      if (line1.isChar(',')) {
        line1.skipChar();

        line1.skipSpace();

        if (line1.isValid()) {
          var2 = "";

          while (line1.isValid() && ! line1.isSpace())
            var2 += line1.getChar();
        }
      }
    }

    setDummyVars(var1, var2);
  }
  // set mapping [ cartesian | cylindrical | spherical ]
  else if (var == MAPPING_VAR) {
  }
  // set parametric
  else if (var == PARAMETRIC_VAR) {
  }
  // set polar
  else if (var == POLAR_VAR) {
  }
  // set samples {samples1:i} [, {samples2:i} ]
  else if (var == SAMPLES_VAR) {
    CParseLine line1(optionStr);

    line1.skipSpace();

    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    long n;

    std::string samples1, samples2;

    if (line1.isValid()) {
      samples1 = "";

      while (line1.isValid() && ! line1.isSpace() && ! line1.isChar(','))
        samples1 += line1.getChar();

      if (CStrUtil::toInteger(samples1, &n))
        isamples1 = n;

      if (line1.isChar(',')) {
        line1.skipChar();

        line1.skipSpace();

        if (line1.isValid()) {
          samples2 = "";

          while (line1.isValid() && ! line1.isSpace())
            samples2 += line1.getChar();

          if (CStrUtil::toInteger(samples2, &n))
            isamples2 = n;
        }
      }
    }

    setSamples(isamples1, isamples2);
  }
  // set zero {expression}
  else if (var == ZERO_VAR) {
  }

  // style [data|functiion] <style>
  else if (var == STYLE_VAR) {
    if (args1.size() == 1) {
      std::cerr << "Missing style type: " << CGnuPlotUtil::valueStrings<StyleVar>() << std::endl;
      return;
    }

    StyleVar var1;

    if (! CGnuPlotUtil::stringToValue(args1[1], var1))
      var1 = STYLE_NO_VAR;

    if      (var1 == STYLE_DATA_VAR) {
      if (args1.size() == 2) {
        std::cerr << "Missing plot style." << std::endl;
        return;
      }

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::stringToValue(args1[2], style))
        setDataStyle(style);
      else {
        std::cerr << "Invalid plot style." << std::endl;
        return;
      }
    }
    else if (var1 == STYLE_FUNCTION_VAR) {
      if (args1.size() == 2) {
        std::cerr << "Missing plot style." << std::endl;
        return;
      }

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::stringToValue(args1[2], style))
        setFunctionStyle(style);
      else {
        std::cerr << "Invalid plot style." << std::endl;
        return;
      }
    }
    else if (var1 == STYLE_LINE_VAR) {
    }
    // set style fill [ empty | solid [{desity:r}] | pattern [{id:i}]
    //                [border [linetype:i] | noborder]
    else if (var1 == STYLE_FILL_VAR) {
      if (args1.size() < 2) return;

      FillStyle fill;

      if (CGnuPlotUtil::stringToValue(args1[2], fill))
        setFillStyle(fill);
      else
        std::cerr << "Invalid fill style." << std::endl;

      if (args1.size() < 3) return;

      if      (args1[2] == "border")
        setFillBorder(true);
      else if (args1[2] == "noborder")
        setFillBorder(false);
      else
        std::cerr << "Invalid fill border." << std::endl;
    }
    // set style histogram [ clustered [ gap {gap:f} ] |
    //                       errorbars [ gap {gap:f} ] [ linewidth | lw {width:i} ] |
    //                       rowstacked |
    //                       columnstacked
    else if (var1 == STYLE_HISTOGRAM_VAR) {
      if (args1.size() < 2) return;

      HistogramStyle histogramStyle;

      if (CGnuPlotUtil::stringToValue(args1[2], histogramStyle))
        setHistogramStyle(histogramStyle);
    }
    else if (var1 == STYLE_RECTANGLE_VAR) {
    }
    else if (var1 == STYLE_CIRCLE_VAR) {
    }
    else if (var1 == STYLE_ELLIPSE_VAR) {
    }
    else if (var1 == STYLE_ARROW_VAR) {
    }
  }
  // set pointsize {mult:r}
  else if (var == POINT_SIZE_VAR) {
    if (args1.size() > 1) {
      double r = 1.0;

      if (CStrUtil::toReal(args1[1], &r)) {
        //setPointSize(r);
      }
    }
  }
  else if (var == TABLE_VAR) {
    if (args1.size() > 2)
      std::cerr << "Ignoring extra arguments." << std::endl;

    std::string filename;

    if (args1.size() == 2) {
      if (! parseString(args1[1], filename)) {
        std::cerr << "Invalid filename" << std::endl;
        return;
      }
    }

    if (filename.empty())
      filename = "STDOUT";

    setTableFile(filename);
  }
  // boxwidth [ {size:r} ] [ absolute | relative ]
  else if (var == BOX_WIDTH_VAR) {
    setDebug(true);
  }
  else if (var == DEBUG_VAR) {
    setDebug(true);
  }
  else if (var == EDEBUG_VAR) {
    setExprDebug(true);
  }
  else {
    std::cerr << "Unhandled option '" << args[0] << "'" << std::endl;

    if (args1.size() == 1) {
      std::cerr << "Missing option value." << std::endl;
      return;
    }
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
    std::string arg;

    while (line.isValid() && ! line.isSpace())
      arg += line.getChar();

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
    var = NO_VAR;

  if      (var == VARIABLES_VAR) {
    std::vector<std::string> names;

    CExprInst->getVariableNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprVariablePtr var = CExprInst->getVariable(names[i]);

      std::cout << var->name() << " = " << var->value() << std::endl;
    }
  }
  else if (var == FUNCTIONS_VAR) {
    std::vector<std::string> names;

    CExprInst->getFunctionNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprFunctionPtr function = CExprInst->getFunction(names[i]);

      if (! function->isBuiltin())
        std::cout << function << std::endl;
    }
  }
  else if (var == VERSION_VAR) {
    // TODO: long
    std::cout << "CGnuPlot Version 0.1" << std::endl;
  }
  else if (var == DATAFILE_VAR) {
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
      if (getSeparator().type == SEP_WHITESPACE)
        std::cerr << "datafile fields separated by whitespace" << std::endl;
     else
        std::cerr << "datafile fields separated by \"" << getSeparator().c << "\"" << std::endl;
    }
    if (show.empty() || show.find("commentschar") != show.end()) {
      std::cerr << "Comments chars are \"" << getCommentChars() << "\"" << std::endl;
    }
  }
  else if (var == STYLE_VAR) {
    std::map<std::string,bool> show;

    if (args1.size() > 1)
      show[args1[1]] = true;

    if (show.empty() || show.find("data") != show.end()) {
      std::cerr << "Data are plotted with " <<
        CGnuPlotUtil::valueToString<PlotStyle>(getDataStyle()) << std::endl;
    }
  }
  else if (var == SAMPLES_VAR) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    std::cerr << "sampling rate is " << isamples1 << ", " << isamples2 << std::endl;
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
    std::string arg;

    while (line.isValid() && ! line.isSpace())
      arg += line.getChar();

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
    var = NO_VAR;

  if      (var == TABLE_VAR)
    setTableFile("");
  else if (var == DEBUG_VAR)
    setDebug(false);
  else if (var == EDEBUG_VAR)
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
  std::string str;

  parseString(args, str);

  system(str.c_str());
}

// clear
void
CGnuPlot::
clearCmd(const std::string &)
{
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
testCmd(const std::string &)
{
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

CGnuPlot::Plot *
CGnuPlot::
addFunction(const std::string &function, PlotStyle style)
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

  Plot *plot = 0;

  if (tableFile.empty()) {
    plot = new Plot(this);

    plot->setStyle(style);

    addPlot(plot);
  }

  //---

  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  //---

  int nx;

  getNumX(&nx);

  std::string varName1("x"), varName2("t");

  getDummyVars(varName1, varName2);

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

  double x = xmin;
  double d = (xmax - xmin)/nx;

  for (int i = 0; i <= nx; ++i, x += d) {
    xvar->setRealValue(x);

    CExprPTokenStack pstack = CExprInst->parseLine(function);
    CExprITokenPtr   itoken = CExprInst->interpPTokenStack(pstack);
    CExprCTokenStack cstack = CExprInst->compileIToken(itoken);
    CExprValuePtr    value  = CExprInst->executeCTokenStack(cstack);

    double y = 0.0;

    if (value.isValid() && value->getRealValue(y)) {
      if (plot) {
        plot->addPoint(COptReal(x), COptReal(y));
      }
      else {
        // TODO: inside/outside test
        std::cerr << x << " " << y << " i" << std::endl;
      }
    }
  }

  //---

  if (plot)
    stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

CGnuPlot::Plot *
CGnuPlot::
addFile(const std::string &filename, PlotStyle style, const UsingCol &col1, const UsingCol &col2,
        const UsingCol &col3, int indexStart, int indexEnd, int indexStep,
        int everyStart, int everyEnd, int everyStep)
{
  Plot *plot = 0;

  //---

  int  bline         = 0;
  int  setNum        = 1;
  int  pointNum      = 0;
  bool discontinuity = false;

  int pointNum1 = everyStart;

  // open file
  CUnixFile file(filename);

  if (file.open()) {
    // process each line in file
    std::string line;

    while (file.readLine(line)) {
      if (line.empty()) {
        ++bline;

        if      (bline == 1)
          discontinuity = true;
        else if (bline == 2) {
          setNum   += indexStep;
          pointNum  = 0;

          if (plot) {
            plot->fit();

            plot->smooth();
          }

          plot = 0;
        }

        continue;
      }

      //---

      std::string::size_type hp = line.find(getCommentChars());

      if (hp != std::string::npos) {
        line = CStrUtil::stripSpaces(line.substr(0, hp));

        if (line.empty())
          continue;
      }

      //---

      if (setNum < indexStart || setNum > indexEnd)
        continue;

      //---

      // get x, y value from fields on line
      fields_.clear();

      if (getSeparator().type == SEP_WHITESPACE)
        CStrUtil::addWords(line, fields_);
      else
        CStrUtil::addFields(line, fields_, std::string(&getSeparator().c, 1), /*skipEmpty*/true);

      //---

      bool skip = false;

      const std::string &missing = getMissingStr();

      CExprValuePtr xValue =
        CGnuPlotUtil::decodeUsingCol(col1, setNum, pointNum, fields_, missing, skip, isDebug());
      if (skip) continue;

      CExprValuePtr yValue =
        CGnuPlotUtil::decodeUsingCol(col2, setNum, pointNum, fields_, missing, skip, isDebug());
      if (skip) continue;

      CExprValuePtr sValue =
        CGnuPlotUtil::decodeUsingCol(col3, setNum, pointNum, fields_, missing, skip, isDebug());
      if (skip) continue;

      //---

      if (! plot) {
        plot = new Plot(this);

        plot->setStyle(style);

        addPlot(plot);
      }

      if (pointNum == pointNum1) {
        COptReal x, y;

        if (xValue.isValid()) {
          double x1 = 0.0;

          if (xValue->getRealValue(x1))
            x = x1;
        }

        if (yValue.isValid()) {
          double y1 = 0.0;

          if (yValue->getRealValue(y1))
            y = y1;
        }

        COptString str;

        if (sValue.isValid()) {
          std::string str1;

          if (sValue->getStringValue(str1))
            str = str1;
        }

        plot->addPoint(x, y, str, discontinuity);

        if (pointNum1 < INT_MAX)
          pointNum1 += everyStep;

        if (pointNum1 > everyEnd)
          pointNum1 = INT_MAX;
      }

      ++pointNum;

      //---

      bline         = 0;
      discontinuity = false;
    }
  }

  //---

  if (plot) {
    plot->fit();

    plot->smooth();
  }

  stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

void
CGnuPlot::
addPlot(Plot *plot)
{
  if (isDebug())
    std::cerr << "Add Plot" << std::endl;

  plot->setInd(plots_.size() + 1);

  plots_.push_back(plot);
}

void
CGnuPlot::
showXAxis(bool show)
{
  if (show)
    xaxis_ = new CAxis2D;
  else
    xaxis_ = 0;
}

void
CGnuPlot::
showYAxis(bool show)
{
  if (show)
    yaxis_ = new CAxis2D;
  else
    yaxis_ = 0;
}

void
CGnuPlot::
paintStart()
{
  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  if (xaxis_) xaxis_->setRange(xmin, xmax);
  if (yaxis_) yaxis_->setRange(ymin, ymax);

  CGnuPlotDrawAxis drawAxis(renderer_);

  if (xaxis_)
    drawAxis.drawAxis(*xaxis_, ymin, CORIENTATION_HORIZONTAL);

  if (yaxis_)
    drawAxis.drawAxis(*yaxis_, xmin, CORIENTATION_VERTICAL);
}

void
CGnuPlot::
paintEnd()
{
}

void
CGnuPlot::
getXRange(double *xmin, double *xmax) const
{
  if (! plots_.empty()) {
    for (uint i = 0; i < plots_.size(); ++i) {
      Plot *plot = plots_[i];

      double xmin1, xmax1;

      plot->getXRange(&xmin1, &xmax1);

      if (i == 0) *xmin = xmin1; else *xmin = std::min(*xmin, xmin1);
      if (i == 0) *xmax = xmax1; else *xmax = std::max(*xmax, xmax1);
    }
  }
  else {
    double xmin1 = -10, xmax1 = 10;

    *xmin = xmin_.getValue(xmin1);
    *xmax = xmax_.getValue(xmax1);
  }
}

void
CGnuPlot::
setXRange(double xmin, double xmax)
{
  xmin_ = xmin;
  xmax_ = xmax;
}

void
CGnuPlot::
getYRange(double *ymin, double *ymax) const
{
  if (! plots_.empty()) {
    for (uint i = 0; i < plots_.size(); ++i) {
      Plot *plot = plots_[i];

      double ymin1, ymax1;

      plot->getYRange(&ymin1, &ymax1);

      if (i == 0) *ymin = ymin1; else *ymin = std::min(*ymin, ymin1);
      if (i == 0) *ymax = ymax1; else *ymax = std::max(*ymax, ymax1);
    }
  }
  else {
    double ymin1 = -1, ymax1 = 1;

    *ymin = ymin_.getValue(ymin1);
    *ymax = ymax_.getValue(ymax1);
  }
}

void
CGnuPlot::
setYRange(double ymin, double ymax)
{
  ymin_ = ymin;
  ymax_ = ymax;
}

void
CGnuPlot::
getNumX(int *nx) const
{
  int isamples1, isamples2;

  getSamples(isamples1, isamples2);

  *nx = isamples1;
}

bool
CGnuPlot::
parseString(const std::string &line, std::string &str)
{
  CParseLine line1(line);

  line1.skipSpace();

  if      (line1.isChar('"')) {
    line1.skipChar();

    while (line1.isValid() && ! line1.isChar('"'))
      str += line1.getChar();

    if (line1.isChar('"'))
      line1.skipChar();
  }
  else if (line1.isChar('\'')) {
    line1.skipChar();

    while (line1.isValid() && ! line1.isChar('\''))
      str += line1.getChar();

    if (line1.isChar('\''))
      line1.skipChar();
  }
  else {
    std::cerr << "Invalid string '" << line << "'" << std::endl;
    // TODO: variable ?
    return false;
  }

  return true;
}

//------

CGnuPlot::Plot::
Plot(CGnuPlot *plot) :
 plot_(plot), ind_(0), xmin_(0), xmax_(100), ymin_(0), ymax_(100),
 style_(POINTS_STYLE), smooth_(SMOOTH_NONE), fitX_(false), fitY_(false)
{
  plot_->getXRange(&xmin_, &xmax_);
  plot_->getYRange(&ymin_, &ymax_);

  setSmooth(plot_->getSmooth());

  setFitX(plot_->getFitX());
  setFitY(plot_->getFitY());
}

CGnuPlot::Plot::
~Plot()
{
}

void
CGnuPlot::Plot::
getXRange(double *xmin, double *xmax) const
{
  *xmin = xmin_;
  *xmax = xmax_;
}

void
CGnuPlot::Plot::
getYRange(double *ymin, double *ymax) const
{
  *ymin = ymin_;
  *ymax = ymax_;
}

void
CGnuPlot::Plot::
clearPoints()
{
  if (plot_->isDebug())
    std::cerr << "Clear Points" << std::endl;

  points_.clear();
}

void
CGnuPlot::Plot::
addPoint(const COptReal &x, const COptReal &y, const COptString &str, bool discontinuity)
{
  if (plot_->isDebug())
    std::cerr << "Add Point [" << points_.size() << "] " <<
                 "(" << x << "," << y << ") {" << str << "}" << std::endl;

  points_.push_back(Point(x, y, str, discontinuity));
}

void
CGnuPlot::Plot::
fit()
{
  if (! getFitX() && ! getFitY())
    return;

  bool firstX = true, firstY = true;

  double xmin = 0.0, ymin = 0.0, xmax = 0.0, ymax = 0.0;

  uint np = numPoints();

  for (uint i = 0; i < np; ++i) {
    Point p = getPoint(i);

    if (p.x.isValid()) {
      double x = p.x.getValue();

      if (firstX) {
        xmin = x;
        xmax = xmin;

        firstX = false;
      }
      else {
        xmin = std::min(xmin, x);
        xmax = std::max(xmax, x);
      }
    }

    if (p.y.isValid()) {
      double y = p.y.getValue();

      if (firstY) {
        ymin = y;
        ymax = ymin;

        firstY = false;
      }
      else {
        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);
      }
    }
  }

  if (getFitX()) {
    xmin_ = xmin;
    xmax_ = xmax;
  }

  if (getFitY()) {
    ymin_ = ymin;
    ymax_ = ymax;
  }
}

void
CGnuPlot::Plot::
smooth()
{
  if (smooth_ == SMOOTH_UNIQUE) {
    typedef std::vector<double>     Values;
    typedef std::map<double,Values> Points;

    Points points;

    for (uint i = 0; i < numPoints(); ++i) {
      double x = points_[i].x.getValue(0.0);
      double y = points_[i].y.getValue(0.0);

      points[x].push_back(y);
    }

    clearPoints();

    for (Points::iterator p = points.begin(); p != points.end(); ++p) {
      double x = (*p).first;
      double y = 0.0;

      int np = (*p).second.size();

      for (int i = 0; i < np; ++i)
        y += (*p).second[i];

      y /= np;

      addPoint(COptReal(x), COptReal(y));
    }
  }
}

void
CGnuPlot::Plot::
draw()
{
  CGnuPlotRenderer *renderer = plot_->renderer();
  if (! renderer) return;

  uint np = numPoints();

  if (getSmooth() == SMOOTH_BEZIER) {
    for (uint i1 = 0, i2 = 1, i3 = 2, i4 = 3; i4 < np;
           i1 = i4, i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1) {
      const Point &point1 = getPoint(i1);
      const Point &point2 = getPoint(i2);
      const Point &point3 = getPoint(i3);
      const Point &point4 = getPoint(i4);

      renderer->drawBezier(point1.getXY(), point2.getXY(), point3.getXY(), point4.getXY());
    }
  }
  else if (style_ == LABELS_STYLE) {
    for (uint i = 0; i < np; ++i) {
      const Point &point = getPoint(i);

      if (point.isXYValid() && point.str.isValid())
        renderer->drawText(point.getXY(), point.str.getValue());
    }
  }
  else if (style_ == HISTOGRAMS_STYLE) {
    double xmin, xmax, ymin, ymax;

    plot_->getXRange(&xmin, &xmax);
    plot_->getYRange(&ymin, &ymax);

    double w = 0.5*(xmax - xmin)/(np*plot_->numPlots());
    double d = (ind() - plot_->numPlots()/2.0)*w;

    for (uint i = 0; i < np; ++i) {
      const Point &point = getPoint(i);

      if (! point.isXYValid())
        continue;

      double x = point.getX();
      double y = point.getY();

      CBBox2D bbox(x + d, ymin, x + d + w, y);

      renderer->drawRect(bbox);
    }
  }
  else {
    if (style_ & LINES_STYLE) {
      for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
        const Point &point1 = getPoint(i1);
        const Point &point2 = getPoint(i2);

        if (point1.isXYValid() && point2.isXYValid())
          renderer->drawLine(point1.getXY(), point2.getXY());
      }
    }

    if (style_ & DOTS_STYLE) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint(i);

        if (point.isXYValid())
          renderer->drawPoint(point.getXY());
      }
    }

    if (style_ & POINTS_STYLE) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint(i);

        if (point.isXYValid())
          renderer->drawSymbol(point.getXY(), 0);
      }
    }
  }
}
