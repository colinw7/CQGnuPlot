#include <CGnuPlot.h>
#include <CUnixFile.h>
#include <CParseLine.h>
#include <CAxis2D.h>
#include <CDrawAxis2D.h>
#include <CDir.h>
#include <CReadLine.h>
#include <CExpr.h>

#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>

namespace CGnuPlotUtil {
  bool nameToPlotStyle(const std::string &name, CGnuPlot::PlotStyle &style) {
    if      (name == "lines")
      style = CGnuPlot::LINES_STYLE;
    else if (name == "points")
      style = CGnuPlot::POINTS_STYLE;
    else if (name == "linespoints")
      style = CGnuPlot::LINES_POINTS_STYLE;
    else
      return false;

    return true;
  }
}

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
    renderer_->drawLine(CGnuPlot::Point(p1.x, p1.y), CGnuPlot::Point(p2.x, p2.y));
  }

  void fillText(const CPoint2D &p, const std::string &text) {
    renderer_->drawText(CGnuPlot::Point(p.x, p.y), text);
  }

 private:
  CGnuPlotRenderer *renderer_;
};

CGnuPlot::
CGnuPlot() :
 debug_(false), xmin_(-10), xmax_(10), ymin_(-1), ymax_(1),
 dataStyle_(POINTS_STYLE), functionStyle_(LINES_STYLE), smooth_(SMOOTH_NONE), fit_(true)
{
  CExprInst->createRealVariable("pi", M_PI);
}

void
CGnuPlot::
setDebug(bool b)
{
  debug_ = b;

  CExprInst->setDebug(debug_);
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
    if (line == "") continue;

    parseLine(line);
  }

  return true;
}

void
CGnuPlot::
loop()
{
  CGnuPlotReadLine readline(this);

  readline.enableTimeoutHook(1);

  for (;;) {
    std::string line = readline.readLine();

    parseLine(line);
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
  if (identifier == "")
    return true;

  // variable assignment
  if      (line.isChar('=')) {
    line.skipChar();

    line.skipSpace();

    CExprValuePtr value = CExprInst->evaluateExpression(line.substr(line.pos()));

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

    if (! line.isChar('='))
      return false;

    line.skipChar();

    line.skipSpace();

    CExprInst->addFunction(identifier, args, line.substr(line.pos()));

    return true;
  }

  std::string args = line.substr(line.pos());

  // process command
  if      (identifier == "exit")
    exit(0);
  else if (identifier == "help")
    showHelp(args);
  else if (identifier == "history")
    showHistory(args);
  else if (identifier == "print")
    printExpression(args);
  else if (identifier == "quit")
    exit(0);
  else if (identifier == "cd")
    changeDir(args);
  else if (identifier == "pwd")
    printCurrenrDir();

  else if (identifier == "call")
    callCmd(args);
  else if (identifier == "load")
    loadCmd(args);
  else if (identifier == "save")
    saveCmd(args);

  else if (identifier == "plot")
    plotCmd(args);
  else if (identifier == "replot")
    replotCmd();
  else if (identifier == "splot")
    splotCmd(args);

  else if (identifier == "set")
    setCmd(args);
  else if (identifier == "show")
    showCmd(args);
  else if (identifier == "reset")
    resetCmd();
  else if (identifier == "unset")
    unsetCmd(args);

  else if (identifier == "shell")
    shellCmd(args);
  else if (identifier == "system")
    systemCmd(args);

  else if (identifier == "clear")
    clearDevice();
  else if (identifier == "lower")
    lowerWindow(args);
  else if (identifier == "raise")
    raiseWindow(args);
  else if (identifier == "test")
    testImage(args);

  else if (identifier == "fit")
    fitPoints(args);
  else if (identifier == "update")
    updateFit(args);

  else if (identifier == "bind")
    bindKeys(args);
  else if (identifier == "if")
    ifCmd(args);
  else if (identifier == "pause")
    pauseMsg(args);
  else if (identifier == "reread")
    rereadFile();

  else {
    std::cerr << "Invalid command: " << identifier << std::endl;
    return false;
  }

  return true;
}

void
CGnuPlot::
showHelp(const std::string &)
{
}

void
CGnuPlot::
showHistory(const std::string &)
{
}

void
CGnuPlot::
printExpression(const std::string &)
{
}

void
CGnuPlot::
changeDir(const std::string &)
{
}

void
CGnuPlot::
printCurrenrDir()
{
  std::cout << CDir::getCurrent() << std::endl;
}

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

void
CGnuPlot::
loadCmd(const std::string &)
{
}

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
  double xmin = -10, xmax = 10;
  double ymin = -1 , ymax = 1;

  CParseLine line(args);

  std::string lastFilename;

  setFit(true);

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

      std::vector<std::string> xfields, yfields;

      CStrUtil::addFields(xrange, xfields, ":", /*skipEmpty*/true);
      CStrUtil::addFields(yrange, yfields, ":", /*skipEmpty*/true);

      if (xfields.size() == 2) {
        double x1, x2;
        if (CStrUtil::toReal(xfields[0], &x1) && CStrUtil::toReal(xfields[1], &x2)) {
          xmin = x1; xmax = x2;
        }
      }

      if (yfields.size() == 2) {
        double y1, y2;
        if (CStrUtil::toReal(yfields[0], &y1) && CStrUtil::toReal(yfields[1], &y2)) {
          ymin = y1; ymax = y2;
        }
      }

      setXRange(xmin, xmax);
      setYRange(ymin, ymax);

      line.skipSpace();

      setFit(false);
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

    UsingCol col1(1), col2(2);

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

        if      (usingStrs.size() > 1) {
          col1 = UsingCol(usingStrs[0]);
          col2 = UsingCol(usingStrs[1]);
        }
        else if (usingStrs.size() > 0) {
          col1 = UsingCol(-1);
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
          if (! CGnuPlotUtil::nameToPlotStyle(styles[i], style))
            std::cerr << "Invalid plot style" << std::endl;
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

        if      (smoothStr == "unique")
          setSmooth(SMOOTH_UNIQUE);
        else if (smoothStr == "frequency")
          setSmooth(SMOOTH_FREQUENCY);
        else if (smoothStr == "bezier")
          setSmooth(SMOOTH_BEZIER);
        else if (smoothStr == "sbezier")
          setSmooth(SMOOTH_SBEZIER);
        else if (smoothStr == "csplines")
          setSmooth(SMOOTH_CSPLINES);
        else if (smoothStr == "acsplines")
          setSmooth(SMOOTH_ACSPLINES);

        line.skipSpace();
      }
      else {
        std::cerr << "Invalid character " << line.lookChar() << std::endl;
        break;
      }
    }

    //---

    if      (! filename.empty())
      (void) addFile(filename, style, col1, col2, indexStart, indexEnd, indexStep,
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

void
CGnuPlot::
replotCmd()
{
}

void
CGnuPlot::
splotCmd(const std::string &)
{
}

void
CGnuPlot::
setCmd(const std::string &args)
{
  CParseLine line(args);

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg;

    while (line.isValid() && ! line.isSpace())
      arg += line.getChar();

    line.skipSpace();
  }

  if (args1.size() == 0) {
    std::cerr << "Missing option name." << std::endl;
    return;
  }

  // set datafile commentschar ["<chars>"]
  // set datafile separator ["<chars>"|whitespace]
  // set datafile missing ["<chars>]
  if      (args1[0] == "datafile") {
  }
  // set dummy <var1> [, <var2>]
  else if (args1[0] == "dummy") {
  }
  // style [data|functiion] <style>
  else if (args1[0] == "style") {
    if (args1.size() == 1) {
      std::cerr << "Missing style type: 'data', 'function', 'line', 'fill', 'rectangle', "
                   "'circle', 'ellipse' or 'arrow'" << std::endl;
      return;
    }

    if      (args1[1] == "data") {
      if (args1.size() == 2) {
        std::cerr << "Missing plot style." << std::endl;
        return;
      }

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::nameToPlotStyle(args1[2], style))
        setDataStyle(style);
      else {
        std::cerr << "Invalid plot style." << std::endl;
        return;
      }
    }
    else if (args1[1] == "function") {
      if (args1.size() == 2) {
        std::cerr << "Missing plot style." << std::endl;
        return;
      }

      PlotStyle style(NO_STYLE);

      if (CGnuPlotUtil::nameToPlotStyle(args1[2], style))
        setFunctionStyle(style);
      else {
        std::cerr << "Invalid plot style." << std::endl;
        return;
      }
    }
    else if (args1[1] == "line") {
    }
    else if (args1[1] == "fill") {
    }
    else if (args1[1] == "rectangle") {
    }
    else if (args1[1] == "circle") {
    }
    else if (args1[1] == "ellipse") {
    }
    else if (args1[1] == "arrow") {
    }
  }
  else {
    if (args1.size() == 1) {
      std::cerr << "Missing option value." << std::endl;
      return;
    }

    // TODO: only certain names allowed
    setNameValue(args1[0], args1[1]);
  }
}

void
CGnuPlot::
showCmd(const std::string &args)
{
  CParseLine line(args);

  std::string name;

  while (line.isValid() && ! line.isSpace())
    name += line.getChar();

  if      (name == "variables") {
    std::vector<std::string> names;

    CExprInst->getVariableNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprVariablePtr var = CExprInst->getVariable(names[i]);

      std::cout << var->name() << " = " << var->value() << std::endl;
    }
  }
  else if (name == "functions") {
    std::vector<std::string> names;

    CExprInst->getFunctionNames(names);

    for (uint i = 0; i < names.size(); ++i) {
      CExprFunctionPtr function = CExprInst->getFunction(names[i]);

      if (! function->isBuiltin())
        std::cout << function << std::endl;
    }
  }
  else if (name == "version") {
    std::cout << "CGnuPlot Version 0.1" << std::endl;
  }
  else {
    std::string value = getNameValue(name);

    std::cout << value << std::endl;
  }
}

void
CGnuPlot::
resetCmd()
{
  // TODO: do not reset terminal and output
  nameValues_.clear();
}

void
CGnuPlot::
unsetCmd(const std::string &args)
{
  CParseLine line(args);

  std::string name, value;

  while (line.isValid() && ! line.isSpace())
    name += line.getChar();

  while (line.isValid() && ! line.isSpace())
    value += line.getChar();

  nameValues_.erase(name);
}

void
CGnuPlot::
shellCmd(const std::string &)
{
}

void
CGnuPlot::
systemCmd(const std::string &)
{
}

void
CGnuPlot::
clearDevice()
{
}

void
CGnuPlot::
lowerWindow(const std::string &)
{
}

void
CGnuPlot::
raiseWindow(const std::string &)
{
}

void
CGnuPlot::
testImage(const std::string &)
{
}

void
CGnuPlot::
fitPoints(const std::string &)
{
}

void
CGnuPlot::
updateFit(const std::string &)
{
}

void
CGnuPlot::
bindKeys(const std::string &)
{
}

void
CGnuPlot::
ifCmd(const std::string &)
{
}

void
CGnuPlot::
pauseMsg(const std::string &)
{
}

void
CGnuPlot::
rereadFile()
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

    CExprValuePtr value = CExprInst->evaluateExpression(line.substr(line.pos()));

    if (value.isValid())
      CExprInst->createVariable(identifier, value);

    return 0;
  }

  //---

  Plot *plot = new Plot(this);

  plot->setStyle(style);

  addPlot(plot);

  //---

  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  //---

  int nx;

  getNumX(&nx);

  std::string varName = getNameValue("dummy");

  if (varName == "")
    varName = "x";

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName, 0.0);

  double x = xmin;
  double d = (xmax - xmin)/nx;

  for (int i = 0; i <= nx; ++i, x += d) {
    xvar->setRealValue(x);

    CExprPTokenStack pstack = CExprInst->parseLine(function);
    CExprITokenPtr   itoken = CExprInst->interpPTokenStack(pstack);
    CExprCTokenStack cstack = CExprInst->compileIToken(itoken);
    CExprValuePtr    value  = CExprInst->executeCTokenStack(cstack);

    double y = 0.0;

    if (value.isValid() && value->getRealValue(y))
      plot->addPoint(x, y, false);
  }

  //---

  stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

CGnuPlot::Plot *
CGnuPlot::
addFile(const std::string &filename, PlotStyle style,
        const UsingCol &col1, const UsingCol &col2,
        int indexStart, int indexEnd, int indexStep,
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

      std::string::size_type hp = line.find('#');

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
      std::vector<std::string> fields;

      CStrUtil::addFields(line, fields, " ", /*skipEmpty*/true);

      int nf = fields.size();

      //---

      double x = pointNum;

      if (col1.isInt) {
        if      (col1.ival == 0)
          x = pointNum;
        else if (col1.ival == -2)
          x = setNum;
        else if (col1.ival > 0 && col1.ival <= nf)
          (void) CStrUtil::toReal(fields[col1.ival - 1], &x);
      }
      else {
        std::string expr = col1.str;

        // TODO: easier to define $1 variables
        std::string::size_type pos = expr.find('$');

        while (pos != std::string::npos) {
          int pos1 = ++pos;

          while (isdigit(expr[pos1]))
            ++pos1;

          std::string numStr = expr.substr(pos, pos1 - pos);

          int icol1 = 0;

          (void) CStrUtil::toInteger(numStr, &icol1);

          double x1 = 0.0;

          if (icol1 > 0 && icol1 <= nf)
            (void) CStrUtil::toReal(fields[icol1 - 1], &x1);

          expr = expr.substr(0, pos - 1) + CStrUtil::toString(x1) + expr.substr(pos1);

          pos = expr.find('$');
        }

        if (isDebug())
          std::cerr << "expr " << expr << std::endl;

        CExprValuePtr value = CExprInst->evaluateExpression(expr);

        if (value.isValid())
          (void) value->getRealValue(x);
      }

      //---

      double y = pointNum;

      if (col2.isInt) {
        if      (col2.ival == 0)
          y = pointNum;
        else if (col2.ival == -2)
          y = setNum;
        else if (col2.ival > 0 && col2.ival <= nf)
          (void) CStrUtil::toReal(fields[col2.ival - 1], &y);
      }
      else {
        std::string expr = col2.str;

        // TODO: easier to define $1 variables
        std::string::size_type pos = expr.find('$');

        while (pos != std::string::npos) {
          int pos1 = ++pos;

          while (isdigit(expr[pos1]))
            ++pos1;

          std::string numStr = expr.substr(pos, pos1 - pos);

          int icol2 = 0;

          (void) CStrUtil::toInteger(numStr, &icol2);

          double y1 = 0.0;

          if (icol2 > 0 && icol2 <= nf)
            (void) CStrUtil::toReal(fields[icol2 - 1], &y1);

          expr = expr.substr(0, pos - 1) + CStrUtil::toString(y1) + expr.substr(pos1);

          pos = expr.find('$');
        }

        if (isDebug())
          std::cerr << "expr " << expr << std::endl;

        CExprValuePtr value = CExprInst->evaluateExpression(expr);

        if (value.isValid())
          (void) value->getRealValue(y);
      }

      //---

      if (! plot) {
        plot = new Plot(this);

        plot->setStyle(style);

        addPlot(plot);
      }

      if (pointNum == pointNum1) {
        plot->addPoint(x, y, discontinuity);

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

  plots_.push_back(plot);
}

void
CGnuPlot::
getXRange(double *xmin, double *xmax) const
{
  *xmin = xmin_;
  *xmax = xmax_;
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
  *ymin = ymin_;
  *ymax = ymax_;
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
  std::string str = getNameValue("samples");

  if (! CStrUtil::toInteger(str, nx))
    *nx = 100;
}

void
CGnuPlot::
setNameValue(const std::string &name, const std::string &value)
{
  nameValues_[name] = value;
}

std::string
CGnuPlot::
getNameValue(const std::string &name) const
{
  NameValues::const_iterator p = nameValues_.find(name);

  if (p == nameValues_.end())
    return "";

  return (*p).second;
}

//------

CGnuPlot::Plot::
Plot(CGnuPlot *plot) :
 plot_(plot), xmin_(0), xmax_(100), ymin_(0), ymax_(100), xaxis_(0), yaxis_(0),
 style_(POINTS_STYLE), smooth_(SMOOTH_NONE), fit_(false)
{
  plot_->getXRange(&xmin_, &xmax_);
  plot_->getYRange(&ymin_, &ymax_);

  setSmooth(plot_->getSmooth());

  setFit(plot_->getFit());
}

CGnuPlot::Plot::
~Plot()
{
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
addPoint(double x, double y, bool discontinuity)
{
  if (plot_->isDebug())
    std::cerr << "Add Point [" << points_.size() << "] (" << x << "," << y << "]" << std::endl;

  points_.push_back(Point(x, y, discontinuity));
}

void
CGnuPlot::Plot::
setXRange(double xmin, double xmax)
{
  xmin_ = xmin;
  xmax_ = xmax;
}

void
CGnuPlot::Plot::
setYRange(double ymin, double ymax)
{
  ymin_ = ymin;
  ymax_ = ymax;
}

void
CGnuPlot::Plot::
fit()
{
  if (! getFit())
    return;

  uint np = numPoints();

  for (uint i = 0; i < np; ++i) {
    Point p = getPoint(i);

    if (i == 0) {
      xmin_ = p.x  ; ymin_ = p.y  ;
      xmax_ = xmin_; ymax_ = ymin_;
    }
    else {
      xmin_ = std::min(xmin_, p.x);
      ymin_ = std::min(ymin_, p.y);
      xmax_ = std::max(xmax_, p.x);
      ymax_ = std::max(ymax_, p.y);
    }
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

    for (uint i = 0; i < numPoints(); ++i)
      points[points_[i].x].push_back(points_[i].y);

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
showXAxis(bool show)
{
  delete xaxis_;

  if (show)
    xaxis_ = new CAxis2D;
  else
    xaxis_ = 0;
}

void
CGnuPlot::Plot::
showYAxis(bool show)
{
  delete yaxis_;

  if (show)
    yaxis_ = new CAxis2D;
  else
    yaxis_ = 0;
}

void
CGnuPlot::Plot::
draw(CGnuPlotRenderer *renderer)
{
  if (xaxis_) xaxis_->setRange(xmin_, xmax_);
  if (yaxis_) yaxis_->setRange(ymin_, ymax_);

  uint np = numPoints();

  if (getSmooth() == SMOOTH_BEZIER) {
    Point point1, point2, point3, point4;

    for (uint i1 = 0, i2 = 1, i3 = 2, i4 = 3; i4 < np;
           i1 = i4, i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1) {
      point1 = getPoint(i1);
      point2 = getPoint(i2);
      point3 = getPoint(i3);
      point4 = getPoint(i4);

      renderer->drawBezier(point1, point2, point3, point4);
    }
  }
  else {
    if (style_ & LINES_STYLE) {
      Point point1, point2;

      for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
        point1 = getPoint(i1);
        point2 = getPoint(i2);

        renderer->drawLine(point1, point2);
      }
    }

    if (style_ & POINTS_STYLE) {
      for (uint i = 0; i < np; ++i) {
        Point point = getPoint(i);

        renderer->drawPoint(point);
      }
    }
  }

  CGnuPlotDrawAxis drawAxis(renderer);

  if (xaxis_)
    drawAxis.drawAxis(*xaxis_, ymin_, CORIENTATION_HORIZONTAL);

  if (yaxis_)
    drawAxis.drawAxis(*yaxis_, xmin_, CORIENTATION_VERTICAL);
}
