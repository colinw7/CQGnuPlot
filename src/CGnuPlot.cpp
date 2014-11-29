#include <CGnuPlot.h>
#include <CUnixFile.h>
#include <CParseLine.h>
#include <CGnuPlotAxis.h>
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

namespace {
  double Deg2Rad(double d) { return M_PI*d/180.0; }
  double Rad2Deg(double d) { return 180.0*d/M_PI; }
}

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

    nameValues.addValue("border"     , CGnuPlot::BORDER_VAR);
    nameValues.addValue("clip"       , CGnuPlot::CLIP_VAR);
    nameValues.addValue("logscale"   , CGnuPlot::LOGSCALE_VAR);
    nameValues.addValue("bmargin"    , CGnuPlot::BMARGIN_VAR);
    nameValues.addValue("lmargin"    , CGnuPlot::LMARGIN_VAR);
    nameValues.addValue("rmargin"    , CGnuPlot::RMARGIN_VAR);
    nameValues.addValue("tmargin"    , CGnuPlot::TMARGIN_VAR);
    nameValues.addValue("multiplot"  , CGnuPlot::MULTIPLOT_VAR);
    nameValues.addValue("offsets"    , CGnuPlot::OFFSETS_VAR);
    nameValues.addValue("origin"     , CGnuPlot::ORIGIN_VAR);
    nameValues.addValue("size"       , CGnuPlot::SIZE_VAR);

    nameValues.addValue("arrow"      , CGnuPlot::ARROW_VAR);
    nameValues.addValue("key"        , CGnuPlot::KEY_VAR);
    nameValues.addValue("label"      , CGnuPlot::LABEL_VAR);
    nameValues.addValue("object"     , CGnuPlot::LABEL_VAR);
    nameValues.addValue("timestamp"  , CGnuPlot::TIMESTAMP_VAR);
    nameValues.addValue("title"      , CGnuPlot::TITLE_VAR);

    nameValues.addValue("style"      , CGnuPlot::STYLE_VAR);

    nameValues.addValue("bars"       , CGnuPlot::BARS_VAR);
    nameValues.addValue("boxwidth"   , CGnuPlot::BOXWIDTH_VAR);
    nameValues.addValue("pointsize"  , CGnuPlot::POINTSIZE_VAR);

    nameValues.addValue("autoscale"  , CGnuPlot::AUTOSCALE_VAR);
    nameValues.addValue("format"     , CGnuPlot::FORMAT_VAR);
    nameValues.addValue("grid"       , CGnuPlot::GRID_VAR);
    nameValues.addValue("xlabel"     , CGnuPlot::XLABEL_VAR); // x2, y2, z, cb
    nameValues.addValue("ylabel"     , CGnuPlot::YLABEL_VAR);
    nameValues.addValue("xrange"     , CGnuPlot::XRANGE_VAR); // x2, y2, z, cb
    nameValues.addValue("yrange"     , CGnuPlot::YRANGE_VAR); // y, x2, y2, z, cb
    nameValues.addValue("xtics"      , CGnuPlot::XTICS_VAR); // x2, y2, z, cb
    nameValues.addValue("ytics"      , CGnuPlot::YTICS_VAR);
    nameValues.addValue("mxtics"     , CGnuPlot::MXTICS_VAR);
    nameValues.addValue("ticscale"   , CGnuPlot::TICSCALE_VAR);
    nameValues.addValue("zeroaxis"   , CGnuPlot::ZEROAXIS_VAR);

    nameValues.addValue("locale"     , CGnuPlot::LOCALE_VAR);
    nameValues.addValue("timefmt"    , CGnuPlot::TIMEFMT_VAR);
    nameValues.addValue("xdata"      , CGnuPlot::XDATA_VAR);
    nameValues.addValue("xdtics"     , CGnuPlot::XDTICS_VAR);
    nameValues.addValue("xmtics"     , CGnuPlot::XMTICS_VAR);

    nameValues.addValue("dgrid3d"    , CGnuPlot::DGRID3D_VAR);
    nameValues.addValue("hidden3D"   , CGnuPlot::HIDDEN3D_VAR);
    nameValues.addValue("isosamples" , CGnuPlot::ISOSAMPLES_VAR);
    nameValues.addValue("surface"    , CGnuPlot::SURFACE_VAR);
    nameValues.addValue("view"       , CGnuPlot::VIEW_VAR);
    nameValues.addValue("xyplane"    , CGnuPlot::XYPLANE_VAR);

    nameValues.addValue("cplane"     , CGnuPlot::CPLANE_VAR);
    nameValues.addValue("cntrparam"  , CGnuPlot::CNTRPARAM_VAR);
    nameValues.addValue("contour"    , CGnuPlot::CONTOUR_VAR);

    nameValues.addValue("colorbox"   , CGnuPlot::COLORBOX_VAR);
    nameValues.addValue("palette"    , CGnuPlot::PALETTE_VAR);
    nameValues.addValue("pm3d"       , CGnuPlot::PM3D_VAR);

    nameValues.addValue("output"     , CGnuPlot::OUTPUT_VAR);
    nameValues.addValue("print"      , CGnuPlot::PRINT_VAR);
    nameValues.addValue("table"      , CGnuPlot::TABLE_VAR);
    nameValues.addValue("terminal"   , CGnuPlot::TERMINAL_VAR);
    nameValues.addValue("termoption" , CGnuPlot::TERMOPTION_VAR);

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
    nameValues.addValue("increment", CGnuPlot::STYLE_INCREMENT_VAR);
    nameValues.addValue("line"     , CGnuPlot::STYLE_LINE_VAR);
    nameValues.addValue("fill"     , CGnuPlot::STYLE_FILL_VAR);
    nameValues.addValue("histogram", CGnuPlot::STYLE_HISTOGRAM_VAR);
    nameValues.addValue("rectangle", CGnuPlot::STYLE_RECTANGLE_VAR);
    nameValues.addValue("circle"   , CGnuPlot::STYLE_CIRCLE_VAR);
    nameValues.addValue("ellipse"  , CGnuPlot::STYLE_ELLIPSE_VAR);
    nameValues.addValue("arrow"    , CGnuPlot::STYLE_ARROW_VAR);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotVar> &nameValues) {
    nameValues.addValue("using"    , CGnuPlot::PLOT_USING_VAR);
    nameValues.addValue("index"    , CGnuPlot::PLOT_INDEX_VAR);
    nameValues.addValue("every"    , CGnuPlot::PLOT_EVERY_VAR);
    nameValues.addValue("with"     , CGnuPlot::PLOT_WITH_VAR);
    nameValues.addValue("title"    , CGnuPlot::PLOT_TITLE_VAR);
    nameValues.addValue("notitle"  , CGnuPlot::PLOT_NOTITLE_VAR);
    nameValues.addValue("smooth"   , CGnuPlot::PLOT_SMOOTH_VAR);
    nameValues.addValue("pointsize", CGnuPlot::PLOT_POINTSIZE_VAR);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::PlotStyle> &nameValues) {
    nameValues.addValue("lines"       , CGnuPlot::LINES_STYLE);
    nameValues.addValue("l"           , CGnuPlot::LINES_STYLE);
    nameValues.addValue("points"      , CGnuPlot::POINTS_STYLE);
    nameValues.addValue("p"           , CGnuPlot::POINTS_STYLE);
    nameValues.addValue("linespoints" , CGnuPlot::LINES_POINTS_STYLE);
    nameValues.addValue("dots"        , CGnuPlot::DOTS_STYLE);
    nameValues.addValue("impulses"    , CGnuPlot::IMPULSES_STYLE);
    nameValues.addValue("labels"      , CGnuPlot::LABELS_STYLE);
    nameValues.addValue("steps"       , CGnuPlot::STEPS_STYLE);
    nameValues.addValue("fsteps"      , CGnuPlot::FSTEPS_STYLE);
    nameValues.addValue("histeps"     , CGnuPlot::HISTEPS_STYLE);
    nameValues.addValue("errorbars"   , CGnuPlot::ERRORBARS_STYLE);
    nameValues.addValue("errorlines"  , CGnuPlot::ERRORLINES_STYLE);
    nameValues.addValue("financebars" , CGnuPlot::FINANCEBARS_STYLE);
    nameValues.addValue("xerrorbars"  , CGnuPlot::XERRORBARS_STYLE);
    nameValues.addValue("xerrorlines" , CGnuPlot::XERRORLINES_STYLE);
    nameValues.addValue("xyerrorbars" , CGnuPlot::XYERRORBARS_STYLE);
    nameValues.addValue("xyerrorlines", CGnuPlot::XYERRORLINES_STYLE);
    nameValues.addValue("yerrorbars"  , CGnuPlot::YERRORBARS_STYLE);
    nameValues.addValue("yerrorlines" , CGnuPlot::YERRORLINES_STYLE);
    nameValues.addValue("vectors"     , CGnuPlot::VECTORS_STYLE);

    nameValues.addValue("boxes"         , CGnuPlot::BOXES_STYLE);
    nameValues.addValue("boxerrorbars"  , CGnuPlot::BOXERRORBARS_STYLE);
    nameValues.addValue("boxxyerrorbars", CGnuPlot::BOXXYERRORBARS_STYLE);
    nameValues.addValue("boxplot"       , CGnuPlot::BOXPLOT_STYLE);
    nameValues.addValue("candlesticks"  , CGnuPlot::CANDLESTICKS_STYLE);
    nameValues.addValue("filledcurves"  , CGnuPlot::FILLEDCURVES_STYLE);
    nameValues.addValue("histograms"    , CGnuPlot::HISTOGRAMS_STYLE);
    nameValues.addValue("image"         , CGnuPlot::IMAGE_STYLE);
    nameValues.addValue("rgbimage"      , CGnuPlot::RGBIMAGE_STYLE);
    nameValues.addValue("rgbalpha"      , CGnuPlot::RGBALPHA_STYLE);
    nameValues.addValue("circles"       , CGnuPlot::CIRCLES_STYLE);
    nameValues.addValue("ellipses"      , CGnuPlot::ELLIPSES_STYLE);
    nameValues.addValue("pm3d"          , CGnuPlot::PM3D_STYLE);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::Smooth> &nameValues) {
    nameValues.addValue("unique"   , CGnuPlot::SMOOTH_UNIQUE);
    nameValues.addValue("frequency", CGnuPlot::SMOOTH_FREQUENCY);
    nameValues.addValue("bezier"   , CGnuPlot::SMOOTH_BEZIER);
    nameValues.addValue("sbezier"  , CGnuPlot::SMOOTH_SBEZIER);
    nameValues.addValue("csplines" , CGnuPlot::SMOOTH_CSPLINES);
    nameValues.addValue("acsplines", CGnuPlot::SMOOTH_ACSPLINES);
  }

  void initNameValues(CStrUniqueMatch<CGnuPlot::FillType> &nameValues) {
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

  void initNameValues(CStrUniqueMatch<CGnuPlot::AngleType> &nameValues) {
    nameValues.addValue("radians", CGnuPlot::ANGLE_RADIANS);
    nameValues.addValue("degrees", CGnuPlot::ANGLE_DEGREES);
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
CGnuPlot() :
 debug_(false), edebug_(false), commentChars_("#"), printAppend_(false),
 dataStyle_(POINTS_STYLE), functionStyle_(LINES_STYLE), smooth_(SMOOTH_NONE),
 histogramStyle_(HISTOGRAM_STYLE_NONE), angleType_(ANGLE_RADIANS),
 fitX_(true), fitY_(true), isamples1_(100), isamples2_(100)
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

  if (! parseString(args, str))
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
bool
CGnuPlot::
plotCmd(const std::string &args)
{
  CGnuPlotWindow *window = createWindow();

  windows_.push_back(window);

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
      function = readNonSpaceNonComma(line);

      if (isDebug())
        std::cerr << "Function: " << function << std::endl;

      style = getFunctionStyle();
    }

    line.skipSpace();

    //---

    UsingCols usingCols;
    Index     index;
    Every     every;

    setSmooth(SMOOTH_NONE);

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
      if      (plotVar == PLOT_USING_VAR) {
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
      else if (plotVar == PLOT_INDEX_VAR) {
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
      else if (plotVar == PLOT_EVERY_VAR) {
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

      else if (plotVar == PLOT_WITH_VAR) {
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
            line.skipNonSpace();

            std::string widthStr = readNonSpaceNonComma(line);

            double r;

            if (CStrUtil::toReal(widthStr, &r))
              lineStyle().setWidth(r);
          }
          else if (line.isString("linecolor") || line.isString("lc")) {
            line.skipNonSpace();

            std::string colorTypeStr = readNonSpaceNonComma(line);

            if (colorTypeStr == "rgbcolor" || colorTypeStr == "rgb") {
              std::string colorValueStr;

              if (! parseString(line, colorValueStr))
                std::cerr << "Invalid color string" << std::endl;

              Color c;

              if (! CRGBName::lookup(colorValueStr, &c.r, &c.g, &c.b))
                std::cerr << "Invalid color string" << std::endl;

              lineStyle().setColor(c);
            }
            else
              std::cerr << "Invalid color spec" << std::endl;
          }
          else if (line.isString("pointtype") || line.isString("pt")) {
            line.skipNonSpace();

            std::string typeStr = readNonSpaceNonComma(line);

            long l;

            if (CStrUtil::toInteger(typeStr, &l))
              pointStyle().setType(l);
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
      else if (plotVar == PLOT_TITLE_VAR) {
        std::string titleStr;

        (void) parseString(line, titleStr);

        if (isDebug())
          std::cerr << "title " << titleStr << std::endl;

        line.skipSpace();
      }
      // disable title
      else if (plotVar == PLOT_NOTITLE_VAR) {
      }
      // smooth type
      else if (plotVar == PLOT_SMOOTH_VAR) {
        std::string smoothStr = readNonSpaceNonComma(line);

        if (isDebug())
          std::cerr << "smooth " << smoothStr << std::endl;

        Smooth smooth;

        if (CGnuPlotUtil::stringToValueWithErr(smoothStr, smooth, "smooth type"))
          setSmooth(smooth);

        line.skipSpace();
      }
      // pointsize <size>
      else if (plotVar == PLOT_POINTSIZE_VAR) {
        std::string sizeStr = readNonSpaceNonComma(line);

        // TODO: just for this plot ?
        double r;

        if (CStrUtil::toReal(sizeStr, &r))
          pointStyle().setSize(r > 0 ? r : 1.0);
      }
    }

    //---

    window->setStyle(style);
    window->setAxesData(axesData());

    if      (! filename.empty()) {
      if  (usingCols.empty())
        usingCols.push_back(UsingCol(1));

      if (usingCols.size() == 1) {
        usingCols.push_back(UsingCol(0));

        std::swap(usingCols[0], usingCols[1]);
      }

      (void) addFile(window, filename, style, usingCols, index, every);
    }
    else if (! function.empty())
      (void) addFunction(window, function, style);
    else
      break;

    //---

    // move to next plot if any
    if (! line.isChar(','))
      break;

    line.skipChar();

    line.skipSpace();
  }

  window->setTitle (title ());
  window->setArrows(arrows());
  window->setLabels(labels());

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

  std::string option = readNonSpace(line);

  //---

  if (option.empty()) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  VariableName var;

  if (! CGnuPlotUtil::stringToValue(option, var))
    var = NO_VAR;

  // --- Configuration ---

  // set datafile separator ["<chars>"|whitespace]
  // set datafile commentschar ["<chars>"]
  // set datafile missing ["<chars>"]
  if      (var == DATAFILE_VAR) {
    std::string varStr = readNonSpace(line);

    DataFileVar var1;

    if (! CGnuPlotUtil::stringToValueWithErr(varStr, var1, "data file type"))
      return;

    line.skipSpace();

    // set datafile separator ["<chars>"|whitespace]
    if      (var1 == DATAFILE_SEPARATOR_VAR) {
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
    else if (var1 == DATAFILE_COMMENTS_CHAR_VAR) {
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
    else if (var1 == DATAFILE_MISSING_VAR) {
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
    std::string type = readNonSpace(line);

    AngleType angleType;

    if (CGnuPlotUtil::stringToValueWithErr(type, angleType, "angle type"))
      setAngleType(angleType);
  }
  // set dummy <var1> [, <var2>]
  else if (var == DUMMY_VAR) {
    std::string var1, var2;

    getDummyVars(var1, var2);

    if (line.isValid()) {
      var1 = readNonSpaceNonComma(line);

      if (line.isChar(',')) {
        line.skipChar();

        line.skipSpace();

        if (line.isValid()) {
          var2 = "";

          while (line.isValid() && ! line.isSpace())
            var2 += line.getChar();
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
          samples2 = "";

          while (line.isValid() && ! line.isSpace())
            samples2 += line.getChar();

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

  // set border {<integer>} {front | back} {linewidth | lw <line_width>}
  //            {{linestyle | ls <line_style>} | {linetype | lt <line_type>}}
  else if (var == BORDER_VAR) {
  }

  // set logscale <axes> {<base>}
  else if (var == LOGSCALE_VAR) {
    if (line.isValid()) {
      std::string axesStr = readNonSpace(line);
      line.skipSpace();

      if (line.isValid()) {
        std::string baseStr = readNonSpace(line);
        line.skipSpace();
      }
    }

    setLogScale(X_LOGSCALE, 10);
    setLogScale(Y_LOGSCALE, 10);
    setLogScale(Z_LOGSCALE, 10);
    setLogScale(X2_LOGSCALE, 10);
    setLogScale(Y2_LOGSCALE, 10);
    setLogScale(CB_LOGSCALE, 10);
  }

  // set origin <x-origin>,<y-origin>
  else if (var == ORIGIN_VAR) {
    std::string arg = readNonSpaceNonComma(line);

    double x;

    CStrUtil::toReal(arg, &x);

    if (line.isChar(',')) {
      line.skipChar(); arg = readNonSpaceNonComma(line); line.skipSpace();

      double y;

      CStrUtil::toReal(arg, &y);
    }
  }
  // set size {{no}square | ratio <r> | noratio} {<xscale>,<yscale>}
  else if (var == SIZE_VAR) {
    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "square" || arg == "nosquare") {
      }
      else if (arg == "ratio") {
        arg = readNonSpaceNonComma(line);
      }
      else if (arg == "noratio") {
      }
      else if (CStrUtil::isReal(arg)) {
        double x;

        CStrUtil::toReal(arg, &x);

        line.skipSpace();

        if (line.isChar(',')) {
          line.skipChar(); arg = readNonSpaceNonComma(line); line.skipSpace();

          double y;

          CStrUtil::toReal(arg, &y);
        }
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
  else if (var == ARROW_VAR) {
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
        arg = readNonSpaceNonComma(line); line.skipSpace();

        arrow.length = -1;

        (void) CStrUtil::toReal(arg, &arrow.length);

        if (line.isChar(',')) {
          line.skipChar(); arg = readNonSpaceNonComma(line); line.skipSpace();

          arrow.angle = -1;

          (void) CStrUtil::toReal(arg, &arrow.angle);

          if (line.isChar(',')) {
            line.skipChar(); arg = readNonSpaceNonComma(line); line.skipSpace();

            arrow.backAngle = -1;

            (void) CStrUtil::toReal(arg, &arrow.backAngle);
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
        arg = readNonSpace(line);

        (void) CStrUtil::toReal(arg, &arrow.lineWidth);
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
  else if (var == KEY_VAR) {
    while (line.isValid()) {
      std::string arg = readNonSpace(line);

      if      (arg == "inside") { }
      else if (arg == "outside") { }
      else if (arg == "lmargin") { }
      else if (arg == "rmargin") { }
      else if (arg == "tmargin") { }
      else if (arg == "bmargin") { }
      else if (arg == "at") {
        line.skipSpace(); std::string posStr = readNonSpace(line);
      }
      else if (arg == "left") { }
      else if (arg == "right") { }
      else if (arg == "center") { }
      else if (arg == "top") { }
      else if (arg == "bottom") { }
      else if (arg == "center") { }
      else if (arg == "vertical") { }
      else if (arg == "horizontal") { }
      else if (arg == "Left") { }
      else if (arg == "Right") { }
      else if (arg == "opaque") { }
      else if (arg == "noopaque") { }
      else if (arg == "invert") { }
      else if (arg == "noinvert") { }
      else if (arg == "samplen") {
        std::string sampleStr = readNonSpace(line);
      }
      else if (arg == "spacing") {
        std::string spaceStr = readNonSpace(line);
      }
      else if (arg == "width") {
        std::string incStr = readNonSpace(line);
      }
      else if (arg == "height") {
        std::string incStr = readNonSpace(line);
      }
      else if (arg == "autotitle") {
        std::string headerStr = readNonSpace(line);
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
        line.skipSpace();
        if (line.isString("linestyle") || line.isString("ls")) {
          line.skipNonSpace();
          std::string styleStr = readNonSpace(line);
        }
      }
      else if (arg == "nobox") {
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
  else if (var == LABEL_VAR) {
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
        arg = readNonSpace(line);

        (void) CStrUtil::toReal(arg, &label.offset);
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
  // set title "<title>"
  else if (var == TITLE_VAR) {
    std::string titleStr;

    if (parseString(line, titleStr))
      title().str = titleStr;
    else
      std::cerr << "Invalid title string" << std::endl;
  }

  // set style [data|function|increment] <args>
  else if (var == STYLE_VAR) {
    std::string var1Str = readNonSpace(line);

    StyleVar var1;

    if (! CGnuPlotUtil::stringToValueWithErr(var1Str, var1, "style type"))
      return;

    line.skipSpace();

    // set style data {style}
    if      (var1 == STYLE_DATA_VAR) {
      std::string styleStr = readNonSpace(line);

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, style, "plot style"))
        setDataStyle(style);
    }
    // set style function {style}
    else if (var1 == STYLE_FUNCTION_VAR) {
      std::string styleStr = readNonSpace(line);

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, style, "plot style"))
        setFunctionStyle(style);
    }
    // set style increment { user |  default }
    else if (var1 == STYLE_INCREMENT_VAR) {
      std::string styleStr = readNonSpace(line);

      if (styleStr == "user") {
        setStyleIncrement(STYLE_INCREMENT_USER);

        setLineStyleNum(COptInt(1));
      }
      else {
        setStyleIncrement(STYLE_INCREMENT_DEFAULT);

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
    else if (var1 == STYLE_ARROW_VAR) {
    }
    // set style fill [ empty | solid [{density:r}] | pattern [{id:i}]]
    //                [ border [ {linetype:i} ] | noborder ]
    else if (var1 == STYLE_FILL_VAR) {
      std::string styleStr = readNonSpace(line);

      FillType fillType;

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, fillType, "fill type"))
        fillStyle().setStyle(fillType);

      line.skipSpace();

      if      (fillType == FILL_SOLID) {
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
      else if (fillType == FILL_PATTERN) {
        if (! line.isString("border") && ! line.isString("noborder")) {
          int patternNum = 0;

          std::string arg1 = readNonSpace(line);

          long l;

          if (CStrUtil::toInteger(arg1, &l))
            patternNum = l;

          fillStyle().setPattern(patternNum);

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
    else if (var1 == STYLE_HISTOGRAM_VAR) {
      std::string styleStr = readNonSpace(line);

      HistogramStyle histogramStyle;

      if (CGnuPlotUtil::stringToValueWithErr(styleStr, histogramStyle, "histogram style"))
        setHistogramStyle(histogramStyle);

      if (histogramStyle == HISTOGRAM_STYLE_CLUSTERED ||
          histogramStyle == HISTOGRAM_STYLE_ERRORBARS) {
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
    else if (var1 == STYLE_LINE_VAR) {
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
          std::string widthStr = readNonSpace(line);

          if (CStrUtil::toReal(widthStr, &r))
            lineStyle.setWidth(r);
        }
        else if (arg == "pointtype" || arg == "pt") {
          std::string typeStr = readNonSpace(line);

          if (CStrUtil::toInteger(typeStr, &l))
            pointStyle.setType(l);
        }
        else if (arg == "pointsize" || arg == "ps") {
          std::string sizeStr = readNonSpace(line);

          if (CStrUtil::toReal(sizeStr, &r))
            pointStyle.setSize(r);
        }
        else if (arg == "linecolor" || arg == "lc") {
          std::string colorTypeStr = readNonSpace(line);

          if (colorTypeStr == "rgbcolor" || colorTypeStr == "rgb") {
            std::string colorValueStr;

            if (! parseString(line, colorValueStr))
              std::cerr << "Invalid color string" << std::endl;

            Color c;

            if (! CRGBName::lookup(colorValueStr, &c.r, &c.g, &c.b))
              std::cerr << "Invalid color string" << std::endl;

            lineStyle.setColor(c);
          }
          else
            std::cerr << "Invalid color spec" << std::endl;
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
    else if (var1 == STYLE_RECTANGLE_VAR) {
    }
    else if (var1 == STYLE_CIRCLE_VAR) {
    }
    else if (var1 == STYLE_ELLIPSE_VAR) {
    }
  }

  // set bars [ small | large | fullwidth | {mult:f} ]
  else if (var == BARS_VAR) {
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
  else if (var == BOXWIDTH_VAR) {
    BoxWidthType boxWidthType = BOXWIDTH_AUTO;
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
            boxWidthType = BOXWIDTH_ABSOLUTE;
          else if (arg == "relative")
            boxWidthType = BOXWIDTH_RELATIVE;
        }
      }
    }

    setBoxWidth(boxWidth, boxWidthType);
  }
  // set pointsize {mult:r}
  else if (var == POINTSIZE_VAR) {
    std::string sizeStr = readNonSpace(line);

    double r = 1.0;

    if (CStrUtil::toReal(sizeStr, &r))
      pointStyle().setSize(r > 0 ? r : 1.0);
  }

  // set xlabel "<label>"
  else if (var == XLABEL_VAR) {
    std::string labelStr;

    if (parseString(line, labelStr))
      axesData().xaxis.str = labelStr;
    else
      std::cerr << "Invalid xlabel string" << std::endl;
  }
  // set ylabel "<label>"
  else if (var == YLABEL_VAR) {
    std::string labelStr;

    if (parseString(line, labelStr))
      axesData().yaxis.str = labelStr;
    else
      std::cerr << "Invalid ylabel string" << std::endl;
  }
  // set xrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} } | restore
  else if (var == XRANGE_VAR) {
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
  else if (var == XRANGE_VAR) {
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
  // set xtics {axis | border} {{no}mirror}
  //           {in | out} {scale {default | <major> {,<minor>}}}
  //           {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //           {left | right | center | autojustify}
  //           {add}
  //           {  autofreq | <incr> | <start>, <incr> {,<end>} |
  //             ({"<label>"} <pos> {<level>} {,{"<label>"}...) }
  //           { format "formatstring" } { font "name{,<size>}" }
  //           { rangelimited }
  //           { textcolor <colorspec> }
  else if (var == XTICS_VAR) {
    axesData().xaxis.showTics = true;
  }
  else if (var == YTICS_VAR) {
    axesData().yaxis.showTics = true;
  }

  // set output ["<filename>"]
  else if (var == OUTPUT_VAR) {
    std::string filename;

    if (! parseString(line, filename)) {
      std::cerr << "Invalid filename" << std::endl;
      return;
    }

    setOutputFile(filename);
  }
  // set print ["<filename>"] [append]
  else if (var == PRINT_VAR) {
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
  else if (var == TABLE_VAR) {
    std::string filename;

    if (! parseString(line, filename)) {
      std::cerr << "Invalid filename" << std::endl;
      return;
    }

    setTableFile(filename);
  }
  // set terminal <type> [<args>]
  else if (var == TERMINAL_VAR) {
    std::cerr << "Unhandled option '" << option << "'" << std::endl;
  }
  // set termoption [no]enhanced
  // set termoption font "<name> [, <size:i> ]"
  else if (var == TERMOPTION_VAR) {
    std::cerr << "Unhandled option '" << option << "'" << std::endl;
  }
  else if (var == DEBUG_VAR) {
    setDebug(true);
  }
  else if (var == EDEBUG_VAR) {
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
  else if (var == TITLE_VAR) {
    std::cerr << "title is \"" << title_.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  else if (var == ARROW_VAR) {
    for (auto arrow : arrows_) {
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
  else if (var == LABEL_VAR) {
    for (auto label : labels_) {
       std::cerr << "label " << label.ind << " \"" << label.text << "\"" <<
                    " at " << label.pos.p;

       std::cerr << std::endl;
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

  else if (var == XLABEL_VAR) {
    std::cerr << "xlable is \"" << axesData().xaxis.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  else if (var == YLABEL_VAR) {
    std::cerr << "ylabel is \"" << axesData().yaxis.str << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }

  else if (var == SAMPLES_VAR) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    std::cerr << "sampling rate is " << isamples1 << ", " << isamples2 << std::endl;
  }
  else if (var == ANGLES_VAR) {
    std::cerr << "Angles are in " <<
      CGnuPlotUtil::valueToString<AngleType>(getAngleType()) << std::endl;
  }
  else if (var == POINTSIZE_VAR) {
    std::cerr << "pointsize is " << pointStyle().size() << std::endl;
  }
  else if (var == OUTPUT_VAR) {
    std::cerr << "output is sent to " <<
      (! getOutputFile().empty() ? ("'" + getOutputFile() + "'"): "STDOUT") << std::endl;
  }
  else if (var == PRINT_VAR) {
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
    var = NO_VAR;

  if      (var == TABLE_VAR)
    setTableFile("");
  else if (var == TITLE_VAR)
    title().str = "";
  else if (var == ARROW_VAR)
    arrows_.clear();
  else if (var == LABEL_VAR)
    labels_.clear();
  else if (var == XLABEL_VAR)
    axesData().xaxis.str = "";
  else if (var == YLABEL_VAR)
    axesData().yaxis.str = "";
  else if (var == XTICS_VAR)
    axesData().xaxis.showTics = false;
  else if (var == YTICS_VAR)
    axesData().yaxis.showTics = false;
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

CGnuPlotWindow *
CGnuPlot::
createWindow()
{
  return new CGnuPlotWindow(this);
}

CGnuPlot::Plot *
CGnuPlot::
addFunction(CGnuPlotWindow *window, const std::string &function, PlotStyle style)
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
    plot = new Plot(window);

    plot->setStyle(style);

    window->addPlot(plot);
  }

  //---

  double xmin, ymin, xmax, ymax;

  window->getXRange(&xmin, &xmax);
  window->getYRange(&ymin, &ymax);

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
        plot->addPoint(x, y);
      }
      else {
        // TODO: inside/outside test
        std::cerr << x << " " << y << " i" << std::endl;
      }
    }
  }

  //---

  if (plot)
    window->stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

CGnuPlot::Plot *
CGnuPlot::
addFile(CGnuPlotWindow *window, const std::string &filename, PlotStyle style,
        const UsingCols &usingCols, const Index &index, const Every &every)
{
  Plot *plot = 0;

  //---

  int  bline         = 0;
  int  setNum        = 1;
  int  pointNum      = 0;
  bool discontinuity = false;

  int pointNum1 = every.start;

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
          setNum   += index.step;
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

      if (setNum < index.start || setNum > index.end)
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
        plot = new Plot(window);

        plot->setStyle(style);

        window->addPlot(plot);
      }

      if (pointNum == pointNum1) {
        plot->addPoint(values, discontinuity);

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
  }

  //---

  if (plot) {
    plot->fit();

    plot->smooth();
  }

  window->stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

void
CGnuPlot::
drawWindows()
{
  for (auto window : windows_)
    window->draw();
}

void
CGnuPlot::
getNumX(int *nx) const
{
  int isamples1, isamples2;

  getSamples(isamples1, isamples2);

  *nx = isamples1;
}

void
CGnuPlot::
setAngleType(AngleType type)
{
  angleType_ = type;

  CExprInst->setDegrees(angleType_ == ANGLE_DEGREES);
}

bool
CGnuPlot::
parseString(const std::string &line, std::string &str)
{
  CParseLine line1(line);

  return parseString(line1, str);
}

bool
CGnuPlot::
parseString(CParseLine &line, std::string &str)
{
  line.skipSpace();

  int i = line.pos();

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
  // TODO: variable ?
  else {
    line.setPos(i);
    std::cerr << "Invalid string '" << line.substr() << "'" << std::endl;
    return false;
  }

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

CGnuPlotWindow::
CGnuPlotWindow(CGnuPlot *plot) :
 plot_(plot), style_(CGnuPlot::POINTS_STYLE), renderer_(0)
{
  xaxis_ = new CGnuPlotAxis(this, CORIENTATION_HORIZONTAL);
  yaxis_ = new CGnuPlotAxis(this, CORIENTATION_VERTICAL);
}

CGnuPlotWindow::
~CGnuPlotWindow()
{
}

void
CGnuPlotWindow::
setRenderer(CGnuPlotRenderer *renderer)
{
  renderer_ = renderer;
}

void
CGnuPlotWindow::
showXAxis(bool show)
{
  axesData().xaxis.displayed = show;

  stateChanged(CGnuPlot::CHANGE_STATE_AXIS_DISPLAY);
}

void
CGnuPlotWindow::
showYAxis(bool show)
{
  axesData().yaxis.displayed = show;

  stateChanged(CGnuPlot::CHANGE_STATE_AXIS_DISPLAY);
}

void
CGnuPlotWindow::
paintStart()
{
  renderer_->setWindow(this);

  double xmin, ymin, xmax, ymax;

  //getDisplayRange(&xmin, &ymin, &xmax, &ymax);
  calcXRange(&xmin, &xmax);
  calcYRange(&ymin, &ymax);

  if (! title().str.empty()) {
    drawHAlignedText(CPoint2D((xmin + xmax)/2.0, ymax), CHALIGN_TYPE_CENTER, 0,
                     CVALIGN_TYPE_BOTTOM, -4, title().str);
  }

  if (axesData().xaxis.displayed) {
    xaxis_->setLabel(axesData().xaxis.str);
    xaxis_->setRange(xmin, xmax);

    if (plot()->getLogScale(CGnuPlot::X_LOGSCALE)) {
      xaxis_->setLogarithmic(plot()->getLogScale(CGnuPlot::X_LOGSCALE));
      xaxis_->setTickInside(true);
    }
    else {
      xaxis_->resetLogarithmic();
      xaxis_->setTickInside(false);
    }

    xaxis_->setFont(renderer_->getFont());
    xaxis_->drawAxis(ymin);
  }

  if (axesData().yaxis.displayed) {
    yaxis_->setLabel(axesData().yaxis.str);
    yaxis_->setRange(ymin, ymax);

    if (plot()->getLogScale(CGnuPlot::Y_LOGSCALE)) {
      yaxis_->setLogarithmic(plot()->getLogScale(CGnuPlot::Y_LOGSCALE));
      yaxis_->setTickInside(true);
    }
    else {
      yaxis_->resetLogarithmic();
      yaxis_->setTickInside(false);
    }

    yaxis_->setFont(renderer_->getFont());
    yaxis_->drawAxis(xmin);
  }
}

void
CGnuPlotWindow::
paintEnd()
{
}

void
CGnuPlotWindow::
addPlot(CGnuPlot::Plot *plot)
{
  if (plot_->isDebug())
    std::cerr << "Add Plot" << std::endl;

  plot->setInd(plots_.size() + 1);

  plot->setFillStyle   (plot_->fillStyle ());
  plot->setLineStyle   (plot_->lineStyle ());
  plot->setPointStyle  (plot_->pointStyle());
  plot->setLineStyleNum(plot_->lineStyleNum());

  if (plot_->fillStyle().style() == CGnuPlot::FILL_PATTERN)
    plot_->fillStyle().setPattern(plot_->fillStyle().pattern() + 1);

  plot_->setLineStyleNum(COptInt(plot_->lineStyleNum().getValue(1) + 1));

  plots_.push_back(plot);
}

void
CGnuPlotWindow::
getDisplayRange(double *xmin, double *ymin, double *xmax, double *ymax) const
{
  calcXRange(xmin, xmax);
  calcYRange(ymin, ymax);

  mapLogPoint(xmin, ymin);
  mapLogPoint(xmax, ymax);
}

void
CGnuPlotWindow::
mapLogPoint(double *x, double *y) const
{
  int xbase = plot()->getLogScale(CGnuPlot::X_LOGSCALE);
  int ybase = plot()->getLogScale(CGnuPlot::Y_LOGSCALE);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = log(std::max(*x, 0.5))/xlogscale;
  if (ybase > 1) *y = log(std::max(*y, 0.5))/ylogscale;
}

void
CGnuPlotWindow::
unmapLogPoint(double *x, double *y) const
{
  int xbase = plot()->getLogScale(CGnuPlot::X_LOGSCALE);
  int ybase = plot()->getLogScale(CGnuPlot::Y_LOGSCALE);

  double xlogscale = (xbase > 1 ? log(xbase) : 1);
  double ylogscale = (ybase > 1 ? log(ybase) : 1);

  if (xbase > 1) *x = exp((*x)*xlogscale);
  if (ybase > 1) *y = exp((*y)*ylogscale);
}

void
CGnuPlotWindow::
calcXRange(double *xmin, double *xmax) const
{
  if (axesData().xaxis.min.isValid() && axesData().xaxis.max.isValid()) {
    *xmin = axesData().xaxis.min.getValue();
    *xmax = axesData().xaxis.max.getValue();
    return;
  }

  if (! plots_.empty()) {
    for (uint i = 0; i < plots_.size(); ++i) {
      CGnuPlot::Plot *plot = plots_[i];

      plot->fit();

      double xmin1, xmax1;

      plot->getXRange(&xmin1, &xmax1);

      if (i == 0) *xmin = xmin1; else *xmin = std::min(*xmin, xmin1);
      if (i == 0) *xmax = xmax1; else *xmax = std::max(*xmax, xmax1);
    }
  }
  else {
    double xmin1 = -10, xmax1 = 10;

    *xmin = axesData().xaxis.min.getValue(xmin1);
    *xmax = axesData().yaxis.max.getValue(xmax1);
  }
}

void
CGnuPlotWindow::
calcYRange(double *ymin, double *ymax) const
{
  if (axesData().yaxis.min.isValid() && axesData().yaxis.max.isValid()) {
    *ymin = axesData().yaxis.min.getValue();
    *ymax = axesData().yaxis.max.getValue();
    return;
  }

  if (! plots_.empty()) {
    for (uint i = 0; i < plots_.size(); ++i) {
      CGnuPlot::Plot *plot = plots_[i];

      plot->fit();

      double ymin1, ymax1;

      plot->getYRange(&ymin1, &ymax1);

      if (i == 0) *ymin = ymin1; else *ymin = std::min(*ymin, ymin1);
      if (i == 0) *ymax = ymax1; else *ymax = std::max(*ymax, ymax1);
    }
  }
  else {
    double ymin1 = -1, ymax1 = 1;

    *ymin = axesData().yaxis.min.getValue(ymin1);
    *ymax = axesData().yaxis.max.getValue(ymax1);
  }
}

void
CGnuPlotWindow::
draw()
{
  paintStart();

  drawPlots();

  drawAnnotations();

  paintEnd();
}

void
CGnuPlotWindow::
drawPlots()
{
  if (getStyle() == CGnuPlot::HISTOGRAMS_STYLE) {
    if (! renderer_) return;

    //---

    double xmin, ymin, xmax, ymax;

    calcXRange(&xmin, &xmax);
    calcYRange(&ymin, &ymax);

    //---

    uint np = 0;

    for (auto plot : plots_)
      np = std::max(np, plot->numPoints());

    if (np == 0) return;

    //---

    if      (plot_->getHistogramStyle() == CGnuPlot::HISTOGRAM_STYLE_CLUSTERED) {
      axesData().yaxis.min = 0.0;
      axesData().yaxis.max = ymax;

      double w = 0.5*(xmax - xmin)/(np*numPlots());

      for (auto plot : plots_) {
        CGnuPlot::LineStyle lineStyle = plot->lineStyle();

        if (plot->lineStyleNum().isValid())
          lineStyle = this->plot()->lineStyles(plot->lineStyleNum().getValue());

        //---

        double d = (plot->ind() - 1 - numPlots()/2.0)*w;

        for (auto point : plot->getPoints()) {
          double x, y;

          if (! point.getX(x) || ! point.getY(y))
            continue;

          CBBox2D bbox(x + d, 0.0, x + d + w, y);

          if      (plot->fillStyle().style() == CGnuPlot::FILL_PATTERN)
            renderer_->patternRect(bbox, plot->fillStyle().pattern());
          else if (plot->fillStyle().style() == CGnuPlot::FILL_SOLID)
            renderer_->fillRect(bbox, lineStyle.color());

          renderer_->drawRect(bbox);
        }
      }
    }
    else if (plot_->getHistogramStyle() == CGnuPlot::HISTOGRAM_STYLE_ROWSTACKED) {
      ymin = 0;
      ymax = 0;

      for (auto plot : plots_) {
        double ymin1, ymax1;

        plot->getYRange(&ymin1, &ymax1);

        ymax += ymax1;
      }

      axesData().yaxis.min = ymin;
      axesData().yaxis.max = ymax;

      //---

      double w = (xmax - xmin)/(np - 1);
      double x = xmin - w/2.0;

      for (uint i = 0; i < np; ++i) {
        double h = 0;

        for (auto plot : plots_) {
          CGnuPlot::LineStyle lineStyle = plot->lineStyle();

          if (plot->lineStyleNum().isValid())
            lineStyle = this->plot()->lineStyles(plot->lineStyleNum().getValue());

          //---

          const CGnuPlot::Point &point = plot->getPoint(i);

          double y;

          if (! point.getY(y))
            continue;

          CBBox2D bbox(x, h, x + w, h + y);

          if      (plot->fillStyle().style() == CGnuPlot::FILL_PATTERN)
            renderer_->patternRect(bbox, plot->fillStyle().pattern());
          else if (plot->fillStyle().style() == CGnuPlot::FILL_SOLID)
            renderer_->fillRect(bbox, lineStyle.color());

          renderer_->drawRect(bbox);

          h += y;
        }

        x += w;
      }
    }
  }
  else {
    for (auto plot : plots_)
      plot->draw();
  }
}

void
CGnuPlotWindow::
drawAnnotations()
{
  for (auto arrow : arrows_)
    drawArrow(arrow);

  for (auto label : labels_)
    drawLabel(label);
}

void
CGnuPlotWindow::
drawArrow(const CGnuPlot::Arrow &arrow)
{
  CPoint2D from = arrow.from.p;
  CPoint2D to   = (arrow.relative ? arrow.from.p + arrow.to.p : arrow.to.p);

  double fx, fy, tx, ty;

  renderer_->windowToPixel(from.x, from.y, &fx, &fy);
  renderer_->windowToPixel(to  .x, to  .y, &tx, &ty);

  double w = (arrow.lineWidth > 0 ? arrow.lineWidth : 1);

  double a = atan2(ty - fy, tx - fx);

  double aa = Deg2Rad(arrow.angle > 0 ? arrow.angle : 30);

  double l  = (arrow.length > 0 ? renderer_->windowWidthToPixelWidth(arrow.length) : 16);
  double l1 = l*cos(aa);

  double c = cos(a), s = sin(a);

  double x1 = fx, y1 = fy;
  double x4 = tx, y4 = ty;

  double x2 = x1 + l1*c;
  double y2 = y1 + l1*s;
  double x3 = x4 - l1*c;
  double y3 = y4 - l1*s;

  double x11 = x1, y11 = y1;
  double x41 = x4, y41 = y4;

  if (arrow.fhead) {
    if (arrow.filled || arrow.empty) {
      x11 = x2;
      y11 = y2;
    }
    else {
      x11 = x1 + w*c;
      y11 = y1 + w*s;
    }
  }

  if (arrow.thead) {
    if (arrow.filled || arrow.empty) {
      x41 = x3;
      y41 = y3;
    }
    else {
      x41 = x4 - w*c;
      y41 = y4 - w*s;
    }
  }

  double ba = Deg2Rad(arrow.backAngle > 0 ? arrow.backAngle : 90);

  renderer_->setMapping(false);

  if (arrow.fhead) {
    double a1 = a + aa;
    double a2 = a - aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xf1 = x1 + l*c1;
    double yf1 = y1 + l*s1;
    double xf2 = x1 + l*c2;
    double yf2 = y1 + l*s2;

    double xf3 = x2, yf3 = y2;

    if (! arrow.filled && ! arrow.empty) {
      renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(xf1, yf1), w);
      renderer_->drawLine(CPoint2D(x1, y1), CPoint2D(xf2, yf2), w);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x1, y1, x2, y2, xf1, yf1, xf1 - 10*c3, yf1 - 10*s3, &xf3, &yf3);

        x11 = xf3;
        y11 = yf3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x1 , y1 ));
      points.push_back(CPoint2D(xf1, yf1));
      points.push_back(CPoint2D(xf3, yf3));
      points.push_back(CPoint2D(xf2, yf2));

      renderer_->drawPolygon(points, w, arrow.filled);
    }
  }

  if (arrow.thead) {
    double a1 = a + M_PI - aa;
    double a2 = a + M_PI + aa;

    double c1 = cos(a1), s1 = sin(a1);
    double c2 = cos(a2), s2 = sin(a2);

    double xt1 = x4 + l*c1;
    double yt1 = y4 + l*s1;
    double xt2 = x4 + l*c2;
    double yt2 = y4 + l*s2;

    double xt3 = x3, yt3 = y3;

    if (! arrow.filled && ! arrow.empty) {
      renderer_->drawLine(CPoint2D(x4, y4), CPoint2D(xt1, yt1), w);
      renderer_->drawLine(CPoint2D(x4, y4), CPoint2D(xt2, yt2), w);
    }
    else {
      if (ba > aa && ba < M_PI) {
        double a3 = a + M_PI - ba;

        double c3 = cos(a3), s3 = sin(a3);

        CMathGeom2D::IntersectLine(x3, y3, x4, y4, xt1, yt1, xt1 - 10*c3, yt1 - 10*s3, &xt3, &yt3);

        x41 = xt3;
        y41 = yt3;
      }

      std::vector<CPoint2D> points;

      points.push_back(CPoint2D(x4 , y4 ));
      points.push_back(CPoint2D(xt1, yt1));
      points.push_back(CPoint2D(xt3, yt3));
      points.push_back(CPoint2D(xt2, yt2));

      renderer_->drawPolygon(points, w, arrow.filled);
    }
  }

  renderer_->drawLine(CPoint2D(x11, y11), CPoint2D(x41, y41), w);

  renderer_->setMapping(true);
}

void
CGnuPlotWindow::
drawLabel(const CGnuPlot::Label &label)
{
  CVAlignType valign = (label.front ? CVALIGN_TYPE_TOP : CVALIGN_TYPE_CENTER);

  drawHAlignedText(label.pos.p, label.align, 0, valign, 0, label.text);
}

void
CGnuPlotWindow::
drawHAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str)
{
  CFontPtr font = renderer_->getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  double font_size = font->getCharAscent() + font->getCharDescent();

  double width1  = 0;
  double height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  double width2 = font->getStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1, y1;

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y + renderer_->pixelHeightToWindowHeight(height1/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y + renderer_->pixelHeightToWindowHeight(height1);

  y1 -= renderer_->pixelHeightToWindowHeight(font->getCharAscent());

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    double width2 = font->getStringWidth(str2);

    if      (halign == CHALIGN_TYPE_LEFT)
      x1 = pos.x;
    else if (halign == CHALIGN_TYPE_CENTER)
      x1 = pos.x - renderer_->pixelWidthToWindowWidth(width2/2);
    else if (halign == CHALIGN_TYPE_RIGHT)
      x1 = pos.x - renderer_->pixelWidthToWindowWidth(width2);

    CPoint2D w(x1 + renderer_->pixelWidthToWindowWidth  (x_offset),
               y1 - renderer_->pixelHeightToWindowHeight(y_offset));

    renderer_->drawText(w, str2);

    y1 -= renderer_->pixelHeightToWindowHeight(font_size);

    str1 = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getStringWidth(str1);

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - renderer_->pixelWidthToWindowWidth(width2/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - renderer_->pixelWidthToWindowWidth(width2);

  CPoint2D w(x1 + renderer_->pixelWidthToWindowWidth  (x_offset),
             y1 - renderer_->pixelHeightToWindowHeight(y_offset));

  renderer_->drawText(w, str1);
}

void
CGnuPlotWindow::
drawVAlignedText(const CPoint2D &pos, CHAlignType halign, int x_offset,
                 CVAlignType valign, int y_offset, const std::string &str)
{
  CFontPtr font = renderer_->getFont();

  /* Calculate width and height of string (max width and sum of heights of all lines) */
  int font_size = font->getISize();

  int width1  = 0;
  int height1 = 0;

  std::string str1 = str;

  std::string::size_type pstr1 = str1.find('\n');

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    width1   = std::max(width1, width2);
    height1 += font_size + 1;

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  int width2 = font->getIStringWidth(str1);

  width1   = std::max(width1, width2);
  height1 += font_size;

  /*-------*/

  /* Draw each line using X and Y alignment */

  str1 = str;

  pstr1 = str1.find('\n');

  double x1, y1;

  if      (halign == CHALIGN_TYPE_LEFT)
    x1 = pos.x;
  else if (halign == CHALIGN_TYPE_CENTER)
    x1 = pos.x - renderer_->pixelWidthToWindowWidth(height1/2);
  else if (halign == CHALIGN_TYPE_RIGHT)
    x1 = pos.x - renderer_->pixelWidthToWindowWidth(height1);

  while (pstr1 != std::string::npos) {
    std::string str2 = str1.substr(0, pstr1);

    int width2 = font->getIStringWidth(str2);

    if      (valign == CVALIGN_TYPE_TOP)
      y1 = pos.y;
    else if (valign == CVALIGN_TYPE_CENTER)
      y1 = pos.y - renderer_->pixelHeightToWindowHeight(width2/2);
    else if (valign == CVALIGN_TYPE_BOTTOM)
      y1 = pos.y - renderer_->pixelHeightToWindowHeight(width2);

    CPoint2D w(x1 + renderer_->pixelWidthToWindowWidth  (x_offset),
               y1 - renderer_->pixelHeightToWindowHeight(y_offset));

    renderer_->drawText(w, str2);

    x1 += renderer_->pixelWidthToWindowWidth(font_size + 1);

    str1  = str1.substr(pstr1 + 1);
    pstr1 = str1.find('\n');
  }

  width2 = font->getIStringWidth(str1);

  if      (valign == CVALIGN_TYPE_TOP)
    y1 = pos.y;
  else if (valign == CVALIGN_TYPE_CENTER)
    y1 = pos.y - renderer_->pixelHeightToWindowHeight(width2/2);
  else if (valign == CVALIGN_TYPE_BOTTOM)
    y1 = pos.y - renderer_->pixelHeightToWindowHeight(width2);

  CPoint2D w(x1 + renderer_->pixelWidthToWindowWidth  (x_offset),
             y1 - renderer_->pixelHeightToWindowHeight(y_offset));

  drawRotatedText(w, str1);
}

void
CGnuPlotWindow::
drawRotatedText(const CPoint2D &p, const std::string &text)
{
  CFontPtr font  = renderer_->getFont();
  CFontPtr rfont = font->rotated(90);

  renderer_->setFont(rfont);

  renderer_->drawText(p, text);

  renderer_->setFont(font);
}

//------

CGnuPlot::Plot::
Plot(CGnuPlotWindow *window) :
 window_(window), ind_(0), xmin_(0), xmax_(100), ymin_(0), ymax_(100),
 style_(POINTS_STYLE), smooth_(SMOOTH_NONE), fitX_(false), fitY_(false)
{
  setSmooth(plot()->getSmooth());

  setFitX(plot()->getFitX());
  setFitY(plot()->getFitY());
}

CGnuPlot::Plot::
~Plot()
{
}

CGnuPlot *
CGnuPlot::Plot::
plot() const
{
  return window_->plot();
}

void
CGnuPlot::Plot::
clearPoints()
{
  if (plot()->isDebug())
    std::cerr << "Clear Points" << std::endl;

  points_.clear();
}

void
CGnuPlot::Plot::
addPoint(double x, double y)
{
  std::vector<CExprValuePtr> values;

  values.push_back(CExprInst->createRealValue(x));
  values.push_back(CExprInst->createRealValue(y));

  addPoint(values, false);
}

void
CGnuPlot::Plot::
addPoint(const Values &values, bool discontinuity)
{
  if (plot()->isDebug()) {
    std::cerr << "Add Point [" << points_.size() << "] " << "(";

    for (uint i = 0; i < values.size(); ++i) {
      if (i > 0) std::cerr << ",";

      std::cerr << values[i];
    }

    std::cerr << ")" << std::endl;
  }

  points_.push_back(Point(values, discontinuity));
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

    double x, y;

    if (p.getX(x)) {
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

    if (p.getY(y)) {
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
      double x, y;

      points_[i].getX(x);
      points_[i].getY(y);

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

      addPoint(x, y);
    }
  }
}

void
CGnuPlot::Plot::
draw()
{
  CGnuPlotRenderer *renderer = window_->renderer();
  if (! renderer) return;

  LineStyle lineStyle = this->lineStyle();

  if (lineStyleNum().isValid())
    lineStyle = plot()->lineStyles(lineStyleNum().getValue());

  uint np = numPoints();

  if (getSmooth() == SMOOTH_BEZIER) {
    for (uint i1 = 0, i2 = 1, i3 = 2, i4 = 3; i4 < np;
           i1 = i4, i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1) {
      const Point &point1 = getPoint(i1);
      const Point &point2 = getPoint(i2);
      const Point &point3 = getPoint(i3);
      const Point &point4 = getPoint(i4);

      double x1, y1, x2, y2, x3, y3, x4, y4;

      if (! point1.getX(x1) || ! point1.getY(y1) || ! point2.getX(x2) || ! point2.getY(y2) ||
          ! point3.getX(x3) || ! point3.getY(y3) || ! point4.getX(x4) || ! point4.getY(y4))
        continue;

      renderer->drawBezier(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3), CPoint2D(x4, y4));
    }
  }
  else if (style_ == LABELS_STYLE) {
    for (uint i = 0; i < np; ++i) {
      const Point &point = getPoint(i);

      double x, y;

      if (! point.getX(x) || ! point.getY(y))
        continue;

      std::string str;

      if (! point.getValue(3, str))
        continue;

      renderer->drawText(CPoint2D(x, y), str);
    }
  }
  else {
    if (style_ == LINES_STYLE || style_ == LINES_POINTS_STYLE) {
      for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
        const Point &point1 = getPoint(i1);
        const Point &point2 = getPoint(i2);

        double x1, y1, x2, y2;

        if (! point1.getX(x1) || ! point1.getY(y1) || ! point2.getX(x2) || ! point2.getY(y2))
          continue;

        renderer->drawLine(CPoint2D(x1, y1), CPoint2D(x2, y2), lineStyle.width());
      }
    }

    if (style_ == DOTS_STYLE) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint(i);

        double x, y;

        if (! point.getX(x) || ! point.getY(y))
          continue;

        renderer->drawPoint(CPoint2D(x, y));
      }
    }

    if (style_ == POINTS_STYLE || style_ == LINES_POINTS_STYLE) {
      for (uint i = 0; i < np; ++i) {
        const Point &point = getPoint(i);

        double x, y;

        if (! point.getX(x) || ! point.getY(y))
          continue;

        double size = pointStyle().size();

        if (pointStyle().varSize()) {
          if (! point.getValue(3, size))
            size = 1;
        }

        window()->mapLogPoint(&x, &y);

        renderer->drawSymbol(CPoint2D(x, y), pointStyle().type(), size);
      }
    }
  }
}

//------

CGnuPlot::Point::
Point() :
 values(), discontinuity(false)
{
}

CGnuPlot::Point::
Point(const Values &values, bool discontinuity) :
 values(values), discontinuity(discontinuity)
{
}

CGnuPlot::Point::
~Point()
{
}

bool
CGnuPlot::Point::
getX(double &x) const
{
  x = 0.0;

  if (values.size() < 1 || ! values[0].isValid())
    return false;

  return values[0]->getRealValue(x);
}

bool
CGnuPlot::Point::
getY(double &y) const
{
  y = 0.0;

  if (values.size() < 2 || ! values[1].isValid())
    return false;

  return values[1]->getRealValue(y);
}

bool
CGnuPlot::Point::
getValue(int n, double &r) const
{
  r = 0.0;

  if (int(values.size()) < n || ! values[n - 1].isValid())
    return false;

  return values[n - 1]->getRealValue(r);
}

bool
CGnuPlot::Point::
getValue(int n, std::string &str) const
{
  str = "";

  if (int(values.size()) < n || ! values[n - 1].isValid())
    return false;

  return values[n - 1]->getStringValue(str);
}

int
CGnuPlot::Point::
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
CGnuPlot::Point::
print(std::ostream &os) const
{
  os << "(";

  for (uint i = 0; i < values.size(); ++i) {
    if (i > 0) os << ",";

    os << values[i];
  }

  os << ")";
}
