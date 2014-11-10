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
 lineJoin_(false), pointSymbol_(true), smooth_(SMOOTH_NONE), fit_(true)
{
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
    showInfo(args);
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

      std::cerr << "Filename: " << filename << std::endl;

      lastFilename = filename;

      setLineJoin   (false);
      setPointSymbol(true);
    }
    else {
      // function
      while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
        function += line.getChar();

      std::cerr << "Function: " << function << std::endl;

      setLineJoin   (true);
      setPointSymbol(true);
    }

    line.skipSpace();

    //---

    int col1 = 1, col2 = 2;
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

        std::string usingStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          usingStr += line.getChar();

        line.skipSpace();

        std::vector<std::string> columns;

        CStrUtil::addFields(usingStr, columns, ":", /*skipEmpty*/true);

        if      (columns.size() > 1) {
          (void) CStrUtil::toInteger(columns[0], &col1);
          (void) CStrUtil::toInteger(columns[1], &col2);
        }
        else if (columns.size() > 0) {
          col1 = -1;
          (void) CStrUtil::toInteger(columns[0], &col2);
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

        std::string indexStr;

        while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
          indexStr += line.getChar();

        line.skipSpace();

        std::vector<std::string> inds;

        CStrUtil::addFields(indexStr, inds, ":", /*skipEmpty*/true);

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

        line.skipSpace();

        std::vector<std::string> styles;

        CStrUtil::addFields(withStr, styles, ":", /*skipEmpty*/true);

        for (uint i = 0; i < styles.size(); ++i) {
          if      (styles[i] == "lines") {
            setLineJoin   (true);
            setPointSymbol(false);
          }
          else if (styles[i] == "points") {
            setLineJoin   (false);
            setPointSymbol(true);
          }
          else if (styles[i] == "linespoints") {
            setLineJoin   (true);
            setPointSymbol(true);
          }
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
      (void) addFile(filename, col1, col2, indexStart, indexEnd, indexStep,
                     everyStart, everyEnd, everyStep);
    else if (! function.empty())
      (void) addFunction(function);

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

  std::string name, value;

  while (line.isValid() && ! line.isSpace())
    name += line.getChar();

  while (line.isValid() && ! line.isSpace())
    value += line.getChar();

  setNameValue(name, value);
}

void
CGnuPlot::
showInfo(const std::string &)
{
}

void
CGnuPlot::
resetCmd()
{
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
addFunction(const std::string &function)
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

  addPlot(plot);

  //---

  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  //---

  int nx;

  getNumX(&nx);

  CExprInst->setDebug(debug_);

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
addFile(const std::string &filename, int col1, int col2,
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

      double x = 0.0, y = 0.0;

      int nf = fields.size();

      if (col1 > 0 && col1 < nf)
        (void) CStrUtil::toReal(fields[col1 - 1], &x);
      else
        x = pointNum;

      if (col2 > 0 && col2 < nf)
        (void) CStrUtil::toReal(fields[col2 - 1], &y);
      else
        y = pointNum;

      if (! plot) {
        plot = new Plot(this);

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
 lineJoin_(false), pointSymbol_(true)
{
  plot_->getXRange(&xmin_, &xmax_);
  plot_->getYRange(&ymin_, &ymax_);

  setLineJoin   (plot_->isLineJoin   ());
  setPointSymbol(plot_->isPointSymbol());

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
  std::cerr << "Clear Points" << std::endl;

  points_.clear();
}

void
CGnuPlot::Plot::
addPoint(double x, double y, bool discontinuity)
{
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
    if (lineJoin_) {
      Point point1, point2;

      for (uint i1 = 0, i2 = 1; i2 < np; i1 = i2++) {
        point1 = getPoint(i1);
        point2 = getPoint(i2);

        renderer->drawLine(point1, point2);
      }
    }

    if (pointSymbol_) {
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
