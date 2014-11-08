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

  double pixelLengthToWindowLength(double p) {
    return renderer_->pixelLengthToWindowLength(p);
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
 debug_(false), xmin_(-10), xmax_(10), ymin_(-1), ymax_(1)
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

  int pos = line.pos();

  line.skipNonSpace();

  std::string keyword = line.substr(pos, line.pos() - pos);

  line.skipSpace();

  // empty line is ok
  if (keyword == "")
    return true;

  std::string args = line.substr(line.pos());

  // process command
  if      (keyword == "exit")
    exit(0);
  else if (keyword == "help")
    showHelp(args);
  else if (keyword == "history")
    showHistory(args);
  else if (keyword == "print")
    printExpression(args);
  else if (keyword == "quit")
    exit(0);
  else if (keyword == "cd")
    changeDir(args);
  else if (keyword == "pwd")
    printCurrenrDir();

  else if (keyword == "call")
    callCommand(args);
  else if (keyword == "load")
    loadCommand(args);
  else if (keyword == "save")
    saveSession(args);

  else if (keyword == "plot")
    plotCmd(args);
  else if (keyword == "replot")
    replot();
  else if (keyword == "splot")
    addSPlot(args);

  else if (keyword == "set")
    setOption(args);
  else if (keyword == "show")
    showInfo(args);
  else if (keyword == "reset")
    resetOptions();
  else if (keyword == "unset")
    unsetOption(args);

  else if (keyword == "shell")
    shellCmd(args);
  else if (keyword == "system")
    systemCmd(args);

  else if (keyword == "clear")
    clearDevice();
  else if (keyword == "lower")
    lowerWindow(args);
  else if (keyword == "raise")
    raiseWindow(args);
  else if (keyword == "test")
    testImage(args);

  else if (keyword == "fit")
    fitPoints(args);
  else if (keyword == "update")
    updateFit(args);

  else if (keyword == "bind")
    bindKeys(args);
  else if (keyword == "if")
    ifCmd(args);
  else if (keyword == "pause")
    pauseMsg(args);
  else if (keyword == "reread")
    rereadFile();

  else {
    std::cerr << "Invalid command: " << keyword << std::endl;
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
callCommand(const std::string &)
{
}

void
CGnuPlot::
loadCommand(const std::string &)
{
}

void
CGnuPlot::
saveSession(const std::string &)
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

      CStrUtil::addFields(xrange, xfields, ":");
      CStrUtil::addFields(yrange, yfields, ":");

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

      std::cerr << "Filename: " << filename << std::endl;
    }
    else {
      // function
      while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
        function += line.getChar();

      std::cerr << "Function: " << function << std::endl;
    }

    line.skipSpace();

    int col1 = 1, col2 = 2;

    if (line.isString("using ")) {
      line.incPos(6);

      line.skipSpace();

      std::string columnsStr;

      while (line.isValid() && ! line.isSpace() && ! line.isChar(','))
        columnsStr += line.getChar();

      std::vector<std::string> columns;

      CStrUtil::addFields(columnsStr, columns, ":");

      if (columns.size() > 0) (void) CStrUtil::toInteger(columns[0], &col1);
      if (columns.size() > 1) (void) CStrUtil::toInteger(columns[1], &col2);
    }

    if      (! filename.empty())
      (void) addFile(filename, col1, col2);
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
replot()
{
}

void
CGnuPlot::
addSPlot(const std::string &)
{
}

void
CGnuPlot::
setOption(const std::string &)
{
}

void
CGnuPlot::
showInfo(const std::string &)
{
}

void
CGnuPlot::
resetOptions()
{
}

void
CGnuPlot::
unsetOption(const std::string &)
{
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
  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  Plot *plot = new Plot;

  plot->setXRange(xmin, xmax);
  plot->setYRange(ymin, ymax);

  //---

  int nx;

  getNumX(&nx);

  CExprInst->setDebug(debug_);

  CExprVariablePtr xvar = CExprInst->createRealVariable("x", 0.0);

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
      plot->addPoint(x, y);

    //std::cerr << x << " " << y << std::endl;
  }

  //---

  plots_.push_back(plot);

  stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
}

CGnuPlot::Plot *
CGnuPlot::
addFile(const std::string &filename, int col1, int col2)
{
  double xmin, xmax, ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  Plot *plot = new Plot;

  plot->setXRange(xmin, xmax);
  plot->setYRange(ymin, ymax);

  //---

  // open file
  CUnixFile file(filename);

  if (file.open()) {
    // process each line in file
    std::string line;

    while (file.readLine(line)) {
      if (line.empty() || line[0] == '#') continue;

      std::vector<std::string> fields;

      CStrUtil::addFields(line, fields, " ");

      double x = 0.0, y = 0.0;

      int nf = fields.size();

      if (col1 > 0 && nf >= col1) (void) CStrUtil::toReal(fields[col1 - 1], &x);
      if (col2 > 0 && nf >= col2) (void) CStrUtil::toReal(fields[col2 - 1], &y);

      plot->addPoint(x, y);
    }

    plot->fit();
  }

  //---

  plots_.push_back(plot);

  stateChanged(CHANGE_STATE_PLOT_ADDED);

  return plot;
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

//------

CGnuPlot::Plot::
Plot() :
 xmin_(0), xmax_(100), ymin_(0), ymax_(100), xaxis_(0), yaxis_(0)
{
}

CGnuPlot::Plot::
~Plot()
{
  delete xaxis_;
  delete yaxis_;
}

void
CGnuPlot::Plot::
addPoint(double x, double y)
{
  points_.push_back(Point(x, y));
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

  Point point1, point2;

  for (uint i = 0; i < np; ++i) {
    point2 = getPoint(i);

    renderer->drawPoint(point2);

    if (i > 0)
      renderer->drawLine(point1, point2);

    point1 = point2;
  }

  CGnuPlotDrawAxis drawAxis(renderer);

  if (xaxis_)
    drawAxis.drawAxis(*xaxis_, ymin_, CORIENTATION_HORIZONTAL);

  if (yaxis_)
    drawAxis.drawAxis(*yaxis_, xmin_, CORIENTATION_VERTICAL);
}
