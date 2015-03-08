#include <CGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotSVGDevice.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotNameValues.h>
#include <CGnuPlotPrefValue.h>

#include <CUnixFile.h>
#include <CParseLine.h>
#include <CDir.h>
#include <CReadLine.h>
#include <CExpr.h>
#include <CRGBName.h>
#include <CMathGeom2D.h>
#include <CMathGen.h>
#include <CFontMgr.h>

#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>

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

class CGnuPlotFn : public CExprFunctionObj {
 public:
  CGnuPlotFn(CGnuPlot *plot) : plot_(plot) { }

 protected:
  CGnuPlot *plot_;
};

class CGnuPlotAssertFn : public CGnuPlotFn {
 public:
  CGnuPlotAssertFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const std::vector<CExprValueP> &values) {
    long value = 0;

    if (! values[0]->getIntegerValue(value))
      assert(false);

    assert(value);

    return CExprValueP();
  }
};

class CGnuPlotColumnFn : public CGnuPlotFn {
 public:
  CGnuPlotColumnFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const std::vector<CExprValueP> &values) {
    long col = 0;

    if (! values[0]->getIntegerValue(col))
      return CExprInst->createRealValue(0.0);

    bool skip;

    return plot_->getFieldValue(plot_->pointNum(), col, 0, plot_->pointNum(), skip);
  }
};

class CGnuPlotExistsFn : public CGnuPlotFn {
 public:
  CGnuPlotExistsFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const std::vector<CExprValueP> &values) {
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

  CExprValueP operator()(const std::vector<CExprValueP> &values) {
    assert(values.size() == 1);

    std::string str;

    long col = 0;

    if (values[0]->getIntegerValue(col))
      str = plot_->getField(col);

    return CExprInst->createStringValue(str);
  }
};

class CGnuPlotStringValidFn : public CGnuPlotFn {
 public:
  CGnuPlotStringValidFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const std::vector<CExprValueP> &) {
    // TODO
    assert(plot_);

    return CExprInst->createRealValue(0.0);
  }
};

class CGnuPlotValueFn : public CGnuPlotFn {
 public:
  CGnuPlotValueFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const std::vector<CExprValueP> &values) {
    std::string name;

    if (! values[0]->getStringValue(name))
      return CExprValueP();

    CExprVariablePtr var = CExprInst->getVariable(name);

    if (! var.isValid())
      return CExprValueP();

    return var->getValue();
  }
};

//------

namespace {
  bool readOptionString(CParseLine &line, std::string &id) {
    line.skipSpace();

    if (! line.isValid())
      return false;

    char c = line.lookChar();

    if (! isalpha(c))
      return false;

    id += line.getChar();

    while (line.isValid()) {
      char c = line.lookChar();

      if (! isalnum(c) && c != '_')
        break;

      id += line.getChar();
    }

    return true;
  }

  template<typename T>
  bool parseOptionValue(CGnuPlot *plot, CParseLine &line, T &value, const std::string &msg="") {
    int pos = line.pos();

    std::string option;

    if (! readOptionString(line, option)) {
      line.setPos(pos);
      return false;
    }

    T value1;

    if (! CStrUniqueMatch::stringToValue(option, value1)) {
      if (msg != "")
        plot->errorMsg("Invalid " + msg + " '" + option + "'");
      line.setPos(pos);
      return false;
    }

    value = value1;

    return true;
  }
}

//------

CGnuPlot::
CGnuPlot()
{
  varPrefs_[VariableName::ANGLES] =
    new CGnuPlotPrefValue<AngleType>("Angle Type", AngleType::RADIANS);

  CExprInst->createRealVariable("pi", M_PI);

  CExprInst->addFunction("assert", "i", new CGnuPlotAssertFn(this));

  CExprInst->addFunction("column"      , "i", new CGnuPlotColumnFn(this));
  CExprInst->addFunction("exists"      , "s", new CGnuPlotExistsFn(this));
  CExprInst->addFunction("stringcolumn", "i", new CGnuPlotStringColumnFn(this));
  CExprInst->addFunction("valid"       , "i", new CGnuPlotStringValidFn(this));
  CExprInst->addFunction("value"       , "s", new CGnuPlotValueFn(this));
  // timecolumn

  CExprInst->createStringVariable("GPVAL_ENCODING", "default");

  svgDevice_ = new CGnuPlotSVGDevice();

  addDevice("SVG", svgDevice_);
}

CGnuPlot::
~CGnuPlot()
{
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
addDevice(const std::string &name, CGnuPlotDevice *device)
{
  device->setApp(this);

  devices_[name] = device;
}

bool
CGnuPlot::
setDevice(const std::string &name)
{
  auto name1 = CStrUtil::toLower(name);

  for (const auto &d : devices_) {
    auto name2 = CStrUtil::toLower(d.second->name());

    if (name1 == name2) {
      if (device_ && CStrUtil::toLower(device_->name()) == name1)
        return true;

      windows_.clear();

      multiWindow_ = 0;

      device_ = d.second;

      return true;
    }
  }

  return false;
}

bool
CGnuPlot::
load(const std::string &filename)
{
  // open file
  CUnixFile *file = new CUnixFile(filename);

  if (! file->open()) {
    delete file;
    return false;
  }

  fileDataArray_.push_back(fileData_);

  fileData_.file    = file;
  fileData_.lineNum = 0;

  fileData_.bufferLines.clear();

  // process each line in file
  std::string line;

  while (fileReadLine(line)) {
    if (line.empty()) continue;

    while (line[line.size() - 1] == '\\') {
      std::string line1;

      if (! fileReadLine(line1))
        line1 = "";

      line = line.substr(0, line.size() - 1) + line1;
    }

    parseLine(line);
  }

  delete fileData_.file;

  fileData_.file = 0;

  fileData_ = fileDataArray_.back();

  fileDataArray_.pop_back();

  return true;
}

bool
CGnuPlot::
exec(const std::string &cmd)
{
  return parseLine(cmd);
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

    auto line = readLine_->readLine();

    while (line[line.size() - 1] == '\\') {
      readLine_->setPrompt("+> ");

      auto line1 = readLine_->readLine();

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
  CParseLine line(str);

  line.skipSpace();

  if      (line.isChar('!')) {
    line.skipChar();

    auto cmd = line.substr();

    FILE *fp = popen(cmd.c_str(), "r");

    std::string line;

    int c;

    while ((c = fgetc(fp)) != EOF) {
      if (c == '\n') {
        std::cout << line << std::endl;

        line = "";
      }
      else
        line += char(c);
    }

    if (line != "")
      std::cout << line << std::endl;

    pclose(fp);
  }
  else if (line.isChar('#')) { // comments char ?
    return true;
  }

  //----

  Statements statements;

  auto str1 = str;

  auto p = str1.find(';');

  while (p != std::string::npos) {
    auto str2 = str1.substr(0, p);

    statements.push_back(str2);

    str1 = str1.substr(p + 1);

    p = str1.find(';');
  }

  statements.push_back(str1);

  for (int i = 0; i < int(statements.size()); ++i) {
    if (! parseStatement(i, statements))
      return false;
  }

  return true;
}

bool
CGnuPlot::
parseStatement(int &i, const Statements &statements)
{
  CParseLine line(statements[i]);

  //---

  // get command name or variable name
  int pos = line.pos();

  std::string identifier;

  (void) readIdentifier(line, identifier);

  // empty line is ok
  if (identifier.empty())
    return true;

  bool keyword = (identifier == "if" || identifier == "while");

  //---

  if (! keyword) {
    // function definition
    if      (line.skipSpaceAndChar('(')) {
      std::vector<std::string> args;

      std::string arg;

      if (! readIdentifier(line, arg))
        return false;

      args.push_back(arg);

      while (line.skipSpaceAndChar(',')) {
        std::string arg1;

        if (! readIdentifier(line, arg1))
          return false;

        args.push_back(arg1);
      }

      if (! line.skipSpaceAndChar(')'))
        return false;

      if (! line.skipSpaceAndChar('='))
        return false;

      CExprInst->addFunction(identifier, args, line.substr());

      return true;
    }
    // variable assignment
    else if (line.skipSpaceAndChar('=')) {
      auto expr = line.substr();

      auto p = expr.find('#');

      if (p != std::string::npos)
        expr = expr.substr(0, p);

      CExprValueP value;

      if (! evaluateExpression(expr, value))
        value = CExprValueP();

      if (! value.isValid())
        return false;

      CExprInst->createVariable(identifier, value);

      return true;
    }
  }

  //---

  line.setPos(pos);

  CommandName commandName = CommandName::NONE;

  if (! parseOptionValue(this, line, commandName, "command name"))
    return false;

  auto args = line.substr();

  // process command
  switch (commandName) {
    case CommandName::EXIT    : exitCmd   (args); break;
    case CommandName::HELP    : helpCmd   (args); break;
    case CommandName::HISTORY : historyCmd(args); break;
    case CommandName::PRINT   : printCmd  (args); break;
    case CommandName::PRINTF  : printfCmd (args); break;
    case CommandName::QUIT    : quitCmd   (args); break;
    case CommandName::CD      : cdCmd     (args); break;
    case CommandName::PWD     : pwdCmd    (args); break;

    case CommandName::CALL    : callCmd  (args); break;
    case CommandName::LOAD    : loadCmd  (args); break;
    case CommandName::SAVE    : saveCmd  (args); break;

    case CommandName::PLOT    : plotCmd   (args); break;
    case CommandName::REPLOT  : replotCmd (args); break;
    case CommandName::REFRESH : refreshCmd(args); break;
    case CommandName::SPLOT   : splotCmd  (args); break;

    case CommandName::SET     : setCmd   (args); break;
    case CommandName::SHOW    : showCmd  (args); break;
    case CommandName::RESET   : resetCmd (args); break;
    case CommandName::UNDEFINE: undefCmd (args); break;
    case CommandName::UNSET   : unsetCmd (args); break;

    case CommandName::SHELL   : shellCmd (args); break;
    case CommandName::SYSTEM  : systemCmd(args); break;
    case CommandName::STATS   : statsCmd (args); break;

    case CommandName::CLEAR   : clearCmd (args); break;
    case CommandName::LOWER   : lowerCmd (args); break;
    case CommandName::RAISE   : raiseCmd (args); break;
    case CommandName::TEST    : testCmd  (args); break;

    case CommandName::FIT     : fitCmd   (args); break;
    case CommandName::UPDATE  : updateCmd(args); break;

    case CommandName::BIND    : bindCmd    (args); break;
    case CommandName::IF      : ifCmd      (i, statements); break;
    case CommandName::DO      : doCmd      (args); break;
    case CommandName::WHILE   : whileCmd   (args); break;
    case CommandName::EVALUATE: evaluateCmd(args); break;
    case CommandName::PAUSE   : pauseCmd   (args); break;
    case CommandName::REREAD  : rereadCmd  (args); break;
    default                   : break;
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
    std::cout << entries[i].line_num << ": " << entries[i].line << std::endl;
  }
}

// print {expression} [, {expression} ]
void
CGnuPlot::
printCmd(const std::string &args)
{
  std::vector<CExprValueP> values;

  if (! CExprInst->evaluateExpression(args, values)) {
    errorMsg("Invalid expression '" + args + "'");
    return;
  }

  for (uint i = 0; i < values.size(); ++i) {
    if (i > 0) std::cerr << " ";

    std::cerr << values[i];
  }

  std::cerr << std::endl;
}

// printf <format> {expression} [, {expression} ]
void
CGnuPlot::
printfCmd(const std::string &args)
{
  CParseLine line(args);

  std::string fmt;

  if (! parseString(line, fmt)) {
    std::cerr << "Invalid format" << std::endl;
    return;
  }

  std::vector<CExprValueP> values;

  if (line.skipSpaceAndChar(',')) {
    std::string expr = line.substr();

    if (! CExprInst->evaluateExpression(expr, values)) {
      std::cerr << "Invalid expression" << std::endl;
      return;
    }
  }

  std::string str = CExprInst->printf(fmt, values);

  std::cout << str;
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

  if (! parseString(line, str)) {
    std::cerr << "Invalid directory name" << std::endl;
    return;
  }

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

  std::string filename;

  if (! parseString(line, filename))
    return;

  CExprInst->createStringVariable("ARG0", filename);

  std::vector<std::string> args1;

  while (line.isValid()) {
    auto arg = readNonSpace(line);

    args1.push_back(arg);
  }

  for (uint i = 0; i < args1.size(); ++i) {
    std::string argName = CStrUtil::strprintf("ARG%d", i + 1);

    CExprInst->createStringVariable(argName, args1[i]);
  }

  load(filename);
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
//      {binary <binary args>}
//      {{nonuniform} matrix}
//      {index <index_list> | index "<name>"}
//      {every <every_list>}
//      {thru <thru_expression>}
//      {skip <number-of-lines>}
//      {using <using_list>}
//      {smooth [ unique | frequency | bezier | sbezier | csplines | acsplines ] }
//      {volatile} {noautoscale}
//      {axes [x1y1 | x1y2 | x2y1 | x2y2 ] }
//      {title [ "{str:explanation}" | {int:col} ] | notitle }
//      { with {stylespec} }
//      [, ...]
void
CGnuPlot::
plotCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "plot " << args << std::endl;

  resetLineStyle();

  CGnuPlotGroup *group = 0;

  lastPlotCmd_ = args;

  // TODO: update local line style copy

  typedef std::vector<CGnuPlotPlot *> Plots;

  Plots plots;

  CGnuPlotWindowP window;

  if (multiplot().enabled) {
    if (! multiWindow_) {
      multiWindow_ = CGnuPlotWindowP(createWindow());

      multiWindow_->set3D(false);
      multiWindow_->setBackgroundColor(backgroundColor());

      windows_.push_back(multiWindow_);
    }

    window = multiWindow_;

    int n = window->numGroups();

    double dx = multiplot().dx();
    double dy = multiplot().dy();

    CIPoint2D pos = multiplot().pos(n);

    double x1 = plotSize_.x.getValue(pos.x*dx);
    double y1 = plotSize_.y.getValue(pos.y*dy);
    double x2 = x1 + plotSize_.xsize.getValue(dx);
    double y2 = y1 + plotSize_.ysize.getValue(dy);

    region_ = CBBox2D(x1, y1, x2, y2);
  }
  else {
    if (! windows_.empty()) {
      for (auto &w : windows_) {
        w->clear();

        window = w;
      }
    }
    else {
      window = CGnuPlotWindowP(createWindow());

      window->setBackgroundColor(backgroundColor());
    }

    window->set3D(false);

    double x1 = plotSize_.x.getValue(0.0);
    double y1 = plotSize_.y.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region_ = CBBox2D(x1, y1, x2, y2);

    windows_.push_back(window);
  }

  //----

  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    std::string var, start, end, inc, lcmd, rcmd;

    if (! parseFor(line, var, start, end, inc, lcmd, rcmd))
      return;

    plotForCmd(var, start, end, inc, lcmd);

    if (rcmd != "")
      plotCmd(rcmd);

    return;
  }

  //bool newhistogram = false;

  std::string lastFilename;

  while (true) {
    if (line.isString("newhistogram")) {
      line.skipNonSpace();

      std::string histTitle;

      if (! parseString(line, histTitle))
        histTitle = "";

      histogramData_.addNewTitle(plots.size(), histTitle);

      (void) line.skipSpaceAndChar(',');
    }

    //----

    // get local copy of next line style
    incLineStyle();

    CGnuPlotLineStyle lineStyle = *this->lineStyle();
    CGnuPlotFillStyle fillStyle = this->fillStyle();

    //----

    COptString keyTitle;

    if (parametric_) {
      // Get Range
      //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX]
      parseAxisRange(line, axesData_.taxis[1    ], false);
      parseAxisRange(line, axesData_.xaxis[xind_], false);
      parseAxisRange(line, axesData_.yaxis[yind_], false);
    }
    else {
      // Get Range
      //  [XMIN:XMAX][YMIN:YMAX]
      parseAxisRange(line, axesData_.xaxis[xind_], false);
      parseAxisRange(line, axesData_.yaxis[yind_], false);
    }

    //----

    // Get filename or expression
    //  "<filename>" | <expression>
    PlotStyle style(PlotStyle::POINTS);

    std::string filename;

    std::vector<std::string> functions;

    line.skipSpace();

    if (line.isChar('\"') || line.isChar('\'')) {
      (void) parseString(line, filename, "Invalid filename");

      // special filename '' (use previous file)
      if      (filename == "")
        filename = lastFilename;
      // special filename '-' (read lines from stdin)
      else if (filename == "-") {
        CGnuPlotFile::Lines lines;

        std::string line;

        while (fileReadLine(line)) {
          if (line == "e") break;

          lines.push_back(line);
        }

        dataFile_.setLines(lines);
      }
      // special filename '+' (expression file)
      else if (filename == "+") {
      }
      // special filename '++' (expression file)
      else if (filename == "++") {
      }

      if (isDebug())
        std::cerr << "Filename: " << filename << std::endl;

      lastFilename = filename;

      style = getDataStyle();
    }
    else {
      // function
      FunctionData functionData;

      if (! parseFunction(line, functionData))
        return;

      if (! functionData.isAssign)
        functions.push_back(functionData.function);
      else
        processAssignFunction(functionData.function, functionData.assign);

      if (parametric_) {
        if (line.skipSpaceAndChar(',')) {
          FunctionData functionData1;

          if (! parseFunction(line, functionData1))
            return;

          if (! functionData1.isAssign)
            functions.push_back(functionData1.function);
          else
            processAssignFunction(functionData1.function, functionData1.assign);
        }
      }

      if (isDebug()) {
        for (const auto &f : functions)
          std::cerr << "Function: " << f << std::endl;
      }

      style = getFunctionStyle();
    }

    line.skipSpace();

    //---

    CGnuPlotUsingCols usingCols;

    dataFile_.resetIndices();
    dataFile_.resetEvery  ();

    setSmooth(Smooth::NONE);

    setBinary(false);
    setMatrix(false);

    // Read individual plot:
    //   <plot_data> [, <plot_data> ...]
    // TODO: using must come before title and with
    while (line.isValid() && ! line.isChar(',')) {
      PlotVar plotVar;

      if (! parseOptionValue(this, line, plotVar)) {
        parseModifiers2D(line, lineStyle, fillStyle);
        break;
      }

      line.skipSpace();

      // Read using columns
      // using <col1>[:<col2>[...]]
      if      (plotVar == PlotVar::USING) {
        parseUsing(line, usingCols);
      }
      // read index range
      // index <start>[:<end>[:<step>]]
      else if (plotVar == PlotVar::INDEX) {
        int indexStart, indexEnd, indexStep;

        parseIndex(line, indexStart, indexEnd, indexStep);

        dataFile_.setIndices(indexStart, indexEnd, indexStep);
      }
      // every <step>[:<start>[:<end>]]
      // TODO (6 values [ipoint:iblock:spoint:epoint:eblock])
      else if (plotVar == PlotVar::EVERY) {
        int everyStart, everyEnd, everyStep;

        parseEvery(line, everyStart, everyEnd, everyStep);

        dataFile_.setEvery(everyStart, everyEnd, everyStep);
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
        if (parseOptionValue(this, line, style, "plot style")) {
          if (isDebug())
            std::cerr << "with " << CStrUniqueMatch::valueToString(style) << std::endl;

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

              if (line.skipSpaceAndChar(','))
                rhs1 = readNonSpaceNonComma(line);

              if (CStrUtil::toReal(rhs, &x) && CStrUtil::toReal(rhs1, &y))
                filledCurve_.xyval = CPoint2D(x, y);
            }
            else {
              line.setPos(pos);
            }
          }
        }

        parseModifiers2D(line, lineStyle, fillStyle);
      }
      // title string
      else if (plotVar == PlotVar::TITLE) {
        std::string titleStr;
        int         columnNum;

        if      (line.isString("col") || line.isString("columnhead") ||
                 line.isString("columnheader")) {
          keyData_.setColumnHead(true);
          keyData_.setColumnNum (0);

          (void) readNonSpaceNonComma(line);
        }
        else if (parseString(line, titleStr)) {
          if (isDebug())
            std::cerr << "title " << titleStr << std::endl;

          keyTitle = titleStr;
        }
        else if (parseInteger(line, columnNum)) {
          keyData_.setColumnHead(true);
          keyData_.setColumnNum (columnNum);
        }

        line.skipSpace();
      }
      // disable title
      else if (plotVar == PlotVar::NOTITLE) {
      }
      // smooth type
      else if (plotVar == PlotVar::SMOOTH) {
        Smooth smooth;

        if (parseOptionValue(this, line, smooth, "smooth type"))
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
      else if (plotVar == PlotVar::LINETYPE) {
        int lt;

        if (parseInteger(line, lt))
          lineStyle.setType(lt);
      }
      // linewidth <lw>
      else if (plotVar == PlotVar::LINEWIDTH) {
        double lw;

        if (parseReal(line, lw))
          lineStyle.setWidth(lw);
      }
      // fillstyle <fs>
      else if (plotVar == PlotVar::FILLSTYLE) {
        int fs;

        if (parseInteger(line, fs))
          fs = 0; // TODO
      }
      // dashtype <dt>
      else if (plotVar == PlotVar::DASHTYPE) {
        int dt;

        if (parseInteger(line, dt))
          lineStyle.setDash(lineDashes_[dt]);
      }
      // pointsize <size>
      else if (plotVar == PlotVar::POINTSIZE) {
        double r = 1.0;

        if (parseReal(line, r))
          lineStyle.setPointSize(r > 0 ? r : 1.0);
      }
      // arrowstyle <as>
      else if (plotVar == PlotVar::ARROWSTYLE) {
        int as;

        if (parseInteger(line, as))
          arrowStyle_ = arrowStyles_[as];
      }
      // heads
      else if (plotVar == PlotVar::HEADS) {
        arrowStyle_.setHeads(true, true);
      }
      // binary
      else if (plotVar == PlotVar::BINARY) {
        setBinary(true);
      }
      // matrix
      else if (plotVar == PlotVar::MATRIX) {
        setMatrix(true);
      }
      // array
      else if (plotVar == PlotVar::ARRAY) {
        // TODO () optional, : for multiple values
        if (line.skipSpaceAndChar('=')) {
          if (line.skipSpaceAndChar('(')) {
            (void) parseInteger(line, imageStyle_.w);

            if (line.skipSpaceAndChar(','))
              (void) parseInteger(line, imageStyle_.h);

            (void) line.skipSpaceAndChar(')');
          }
        }
      }
      // flip
      else if (plotVar == PlotVar::FLIP) {
        std::string arg = line.readNonSpace();

        if (arg == "=y")
          imageStyle_.flipy = true;
      }
      // flipy
      else if (plotVar == PlotVar::FLIPY) {
        imageStyle_.flipy = true;
      }
      // format
      else if (plotVar == PlotVar::FORMAT) {
        if (line.skipSpaceAndChar('=')) {
          std::string str;

          (void) parseString(line, str, "Invalid image format");

          imageStyle_.format = str;
        }
      }
      // origin
      else if (plotVar == PlotVar::ORIGIN) {
        if (line.skipSpaceAndChar('=')) {
          if (line.skipSpaceAndChar('(')) {
            CPoint2D o;

            if (parsePoint(line, o))
              imageStyle_.o = o;

            (void) line.skipSpaceAndChar(')');
          }
        }
      }
      // center
      else if (plotVar == PlotVar::CENTER) {
        if (line.skipSpaceAndChar('=')) {
          if (line.skipSpaceAndChar('(')) {
            CPoint2D c;

            if (parsePoint(line, c))
              imageStyle_.c = c;

            (void) line.skipSpaceAndChar(')');
          }
        }
      }
      // dx, dy
      else if (plotVar == PlotVar::DX || plotVar == PlotVar::DY) {
        if (line.skipSpaceAndChar('=')) {
          double r;

          if (parseReal(line, r)) {
            if (plotVar == PlotVar::DX) {
              imageStyle_.dx = r;
              imageStyle_.dy = r;
            }
            else
              imageStyle_.dy = r;
          }
        }
      }
      // rotation
      else if (plotVar == PlotVar::ROTATION) {
        if (line.skipSpaceAndChar('=')) {
          double a;

          if (parseReal(line, a)) {
            std::string arg = (! line.isSpace() ? line.readNonSpace() : "");

            if      (arg == "deg" || arg == "d")
              imageStyle_.a = a;
            else if (arg == "pi")
              imageStyle_.a = Rad2Deg(a*M_PI);
            else
              imageStyle_.a = Rad2Deg(a);
          }
        }
      }
      // whiskerbars [<fraction>]?
      else if (plotVar == PlotVar::WHISKERBARS) {
        setWhiskerBars(1);
      }
    }

    //---

    if (! group) {
      group = createGroup(window.get());

      group->set3D(false);
    }

    Plots plots1;

    if      (! filename.empty()) {
      if (isBinary()) {
        if (style == PlotStyle::IMAGE || style == PlotStyle::RGBIMAGE ||
            style == PlotStyle::RGBALPHA) {
          CGnuPlotPlot *plot = addImage2D(group, filename, style, usingCols);

          if (plot)
            plots1.push_back(plot);
        }
        else {
          std::cerr << "Unhandled binary plot" << std::endl;
        }
      }
      else
        plots1 = addFile2D(group, filename, style, usingCols);
    }
    else if (! functions.empty()) {
      CGnuPlotPlot *plot = addFunction2D(group, functions, style);

      if (plot)
        plots1.push_back(plot);
    }

    for (const auto &plot1 : plots1) {
      if (keyTitle.isValid())
        plot1->setKeyTitle(keyTitle.getValue());

      plot1->setLineStyle(lineStyle);
      plot1->setFillStyle(fillStyle);

      plots.push_back(plot1);
    }

    //---

    // move to next plot if any
    if (! line.isChar(','))
      break;

    line.skipChar();

    line.skipSpace();
  }

  //---

  if (plots.empty())
    return;

  //---

  group->addObjects();

  group->init();

  group->setTitleData(title_);

  group->addSubPlots(plots);

  group->fit();

  window->addGroup(group);

  for (auto plot : plots)
    plot->smooth();

  //---

  if (device_)
    stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
}

bool
CGnuPlot::
parseModifiers2D(CParseLine &line, CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle)
{
  bool modifiers = true;

  while (modifiers) {
    line.skipSpace();

    if      (line.isString("linestyle") || line.isString("ls")) {
      line.skipNonSpace();

      int i;

      if (parseInteger(line, i))
        setLineStyleInd(i);
    }
    else if (line.isString("linetype") || line.isString("lt")) {
      line.skipNonSpace();

      int i;

      if (parseInteger(line, i))
        lineStyle.setType(i);
    }
    else if (line.isString("linewidth") || line.isString("lw")) {
      line.skipNonSpace();

      double lw;

      if (parseReal(line, lw))
        lineStyle.setWidth(lw);
    }
    else if (line.isString("linecolor") || line.isString("lc")) {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle.setColorSpec(c);
    }
    else if (line.isString("textcolor") || line.isString("tc")) {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle.setColorSpec(c);
    }
    else if (line.isString("pointtype") || line.isString("pt")) {
      line.skipNonSpace();

      std::string typeStr = readNonSpaceNonComma(line);

      int i;

      if (parseInteger(line, i))
        lineStyle.setPointType(static_cast<CGnuPlotTypes::SymbolType>(i));
    }
    else if (line.isString("pointsize") || line.isString("ps")) {
      line.skipNonSpace();

      std::string style = readNonSpaceNonComma(line);

      double s;

      if      (style == "variable" || style == "var")
        pointStyle_.setVarSize(true);
      else if (CStrUtil::toReal(style, &s))
        lineStyle.setPointSize(s);
    }
    else if (line.isString("fill") || line.isString("fs")) {
      line.skipNonSpace();

      CGnuPlotFillStyle fillStyle1;

      if (parseFillStyle(line, fillStyle1))
        fillStyle = fillStyle1;
    }
    else if (line.isString("arrowstyle") || line.isString("as")) {
      line.skipNonSpace();

      std::string style = readNonSpaceNonComma(line);

      int as;

      if      (style == "variable" || style == "var")
        arrowStyle_.setVariable(true);
      else if (CStrUtil::toInteger(style, &as))
        arrowStyle_ = arrowStyles_[as];
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
  }

  return true;
}

void
CGnuPlot::
plotForCmd(const std::string &var, const std::string &start, const std::string &end,
           const std::string &inc, const std::string &args)
{
  CExprValuePtr value1;

  if (! evaluateExpression(start, value1))
    return;

  CExprValuePtr value2;

  if (! evaluateExpression(end, value2))
    return;

  CExprValuePtr value3;

  if (inc != "") {
    if (! evaluateExpression(inc, value3))
      return;
  }

  long i1 = 0, i2 = 1, i3 = 1;

  if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
    return;

  if (value3.isValid() && ! value3->getIntegerValue(i3))
    return;

  while (i1 <= i2) {
    CExprInst->createIntegerVariable(var, i1);

    plotCmd(args);

    i1 += i3;
  }
}

bool
CGnuPlot::
parseFor(CParseLine &line, std::string &var, std::string &start, std::string &end,
         std::string &inc, std::string &lcmd, std::string &rcmd)
{
  line.skipNonSpace();

  if (! line.skipSpaceAndChar('['))
    return false;

  if (! readIdentifier(line, var))
    return false;

  // for [ivar = start:end:incr]
  if (line.skipSpaceAndChar('=')) {
    line.skipSpace();

    while (line.isValid() && ! line.isChar(':'))
      start += line.getChar();

    if (! line.isChar(':'))
      return false;

    line.skipChar();
    line.skipSpace();

    while (line.isValid() && ! line.isChar(']') && ! line.isChar(':'))
      end += line.getChar();

    if (line.isChar(':')) {
      line.skipSpace();

      while (line.isValid() && ! line.isChar(']'))
        inc += line.getChar();
    }

    if (! line.isChar(']'))
      return false;

    line.skipChar();
    line.skipSpace();
  }
  // for [svar in "<words>"]
  else if (line.isString("in")) {
    line.skipNonSpace();

    if (! parseString(line, start, "Invalid for in"))
      return false;

    if (! line.skipSpaceAndChar(']'))
      return false;
  }

  while (line.isValid() && ! line.isChar(','))
    lcmd += line.getChar();

  if (line.isChar(',')) {
    line.skipChar();
    line.skipSpace();

    rcmd = (line.substr());
  }

  return true;
}

// {empty |
//  {transparent} solid {density} |
//  {transparent} pattern {id}}}
//  {border {lt} {lc <colorspec>} | noborder}
bool
CGnuPlot::
parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle)
{
  int pos = line.pos();

  std::string arg = readNonSpaceNonComma(line);

  if (arg == "empty") {
    fillStyle.setStyle(CGnuPlotTypes::FillType::EMPTY);

    arg = readNonSpaceNonComma(line);

    pos = line.pos();
  }

  if (arg == "transparent") {
    fillStyle.setTransparent(true);

    pos = line.pos();

    arg = readNonSpaceNonComma(line);
  }

  if      (arg == "solid") {
    fillStyle.setStyle(CGnuPlotTypes::FillType::SOLID);

    line.skipSpace();

    if (isdigit(line.lookChar())) {
      double density = 0.0;

      if (parseReal(line, density))
        fillStyle.setDensity(density);
    }

    pos = line.pos();

    arg = readNonSpaceNonComma(line);
  }
  else if (arg == "pattern") {
    fillStyle.setStyle(CGnuPlotTypes::FillType::PATTERN);

    line.skipSpace();

    if (isdigit(line.lookChar())) {
      int pattern = 0;

      if (parseInteger(line, pattern))
        fillStyle.setPattern(static_cast<CGnuPlotTypes::FillPattern>(pattern));
    }

    pos = line.pos();

    arg = readNonSpaceNonComma(line);
  }

  if      (arg == "border" || arg == "bo") {
    fillStyle.setBorder(true);

    int lt;

    line.skipSpace();

    if      (line.isString("lt")) {
      fillStyle.setBorderLineType(lineStyle()->type());
      fillStyle.setBorderColor(CRGBA());
    }
    else if (line.isString("lc")) {
      line.skipNonSpace();
      line.skipSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c)) {
        fillStyle.setBorderLineType(-1);
        fillStyle.setBorderColor(c.color());
      }
    }
    else if (parseInteger(line, lt)) {
      fillStyle.setBorderLineType(lt);
      fillStyle.setBorderColor(CRGBA());
    }

    pos = line.pos();

    arg = readNonSpaceNonComma(line);
  }
  else if (arg == "noborder") {
    fillStyle.setBorder(false);

    pos = line.pos();

    arg = readNonSpaceNonComma(line);
  }

  if (arg == "rgbcolor" || arg == "rgb") {
    std::string colorValueStr;

    if (parseString(line, colorValueStr, "Invalid color string")) {
      CRGBA c;

      double r, g, b;

      if (CRGBName::lookup(colorValueStr, &r, &g, &b))
        c = CRGBA(r, g, b);
      else
        std::cerr << "Invalid color string" << std::endl;
    }
  }
  else
    line.setPos(pos);

  return true;
}

// [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
bool
CGnuPlot::
parseAxisRange(CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs)
{
  line.skipSpace();

  if (line.isChar('[')) {
    std::vector<std::string> fields;

    if (! parseRange(line, fields))
      return false;

    if (! decodeRange(fields, axis))
      return false;
  }

  if (hasArgs) {
    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "reverse" || arg == "noreverse") {
        axis.setReverse(arg == "reverse");
      }
      else if (arg == "writeback" || arg == "nowriteback") {
        axis.setWriteBack(arg == "writeback");
      }
      else if (arg == "extend" || arg == "noextend") {
        axis.setExtend(arg == "extend");
      }
      else if (arg == "restore") {
      }
      else
        return false;

      arg = readNonSpace(line);
    }
  }

  return true;
}

// [{<dummary-var>=}{min}:{max}]
bool
CGnuPlot::
parseRange(CParseLine &line, std::vector<std::string> &fields)
{
  if (! line.skipSpaceAndChar('['))
    return false;

  std::string rangeStr;

  while (line.isValid() && ! line.isChar(']'))
    rangeStr += line.getChar();

  if (line.isChar(']'))
    line.skipChar();

  CStrUtil::addFields(rangeStr, fields, ":");

  return true;
}

bool
CGnuPlot::
decodeRange(const std::vector<std::string> &fields, CGnuPlotAxisData &axis)
{
  std::vector<COptReal> values;

  for (const auto &field : fields) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;

    if (axis.isTime()) {
      int len = field.size();

      if (len > 0 && field[0] == '"' && field[len - 1] == '"') {
        strptime(field.substr(1, len - 2).c_str(), timeFmt_.c_str(), &tm1);

        value = mktime(&tm1);
      }
    }
    else {
      if (field != "*") {
        CExprValueP evalue;

        if (! evaluateExpression(field, evalue))
          evalue = CExprValueP();

        double r;

        if (evalue.isValid() && evalue->getRealValue(r))
          value = r;
      }
    }

    values.push_back(value);
  }

  if      (values.size() >= 2) {
    COptReal &min = values[0];
    COptReal &max = values[1];

    if (isDebug())
      std::cerr << "Range=(" << min.getValue(-1) << "," <<
                                max.getValue( 1) << ")" << std::endl;

    if (min.isValid() && max.isValid()) {
      if (min.getValue() > max.getValue()) {
        axis.setReverse(true);

        axis.setMin(max);
        axis.setMax(min);
      }
      else {
        axis.setMin(min);
        axis.setMax(max);
      }
    }
    else if (min.isValid())
      axis.setMin(min);
    else if (max.isValid())
      axis.setMax(max);
  }
  else if (values.size() == 1) {
    COptReal &min = values[0];

    if (isDebug())
      std::cerr << "Range=(" << min.getValue(-1) << ",*)" << std::endl;

    if (min.isValid())
      axis.setMin(min);
  }
  else
    return false;

  return true;
}

// replot
void
CGnuPlot::
replotCmd(const std::string &)
{
  if (lastPlotCmd_ != "")
    plotCmd(lastPlotCmd_);
}

// refresh
void
CGnuPlot::
refreshCmd(const std::string &)
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
  resetLineStyle();

  CGnuPlotWindowP window;

  if (multiplot().enabled && ! windows_.empty())
    window = windows_.back();
  else {
    window = CGnuPlotWindowP(createWindow());

    window->setBackgroundColor(backgroundColor());

    windows_.push_back(window);

    window->set3D(true);
  }

  //----

  CParseLine line(args);

  std::string lastFilename;

  while (true) {
    COptString keyTitle;

    // Get Range
    //  [XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]

    std::vector<std::string> xfields, yfields, zfields;

    parseAxisRange(line, axesData_.xaxis[xind_], false);
    parseAxisRange(line, axesData_.yaxis[yind_], false);
    parseAxisRange(line, axesData_.zaxis[zind_], false);

    //----

    // Get filename or expression
    //  "<filename>" | <expression>

    PlotStyle style(PlotStyle::POINTS);

    std::string filename, function;

    if (line.skipSpaceAndChar('\"')) {
      // filename
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
      FunctionData functionData;

      if (! parseFunction(line, functionData))
        return;

      if (! functionData.isAssign) {
        function = functionData.function;

        style = getFunctionStyle();

        if (isDebug())
          std::cerr << "Function: " << function << std::endl;
      }
      else
        processAssignFunction(functionData.function, functionData.assign);
    }

    line.skipSpace();

    //---

    CGnuPlotUsingCols usingCols;

    dataFile_.resetIndices();
    dataFile_.resetEvery  ();

    setSmooth(Smooth::NONE);

    // Read individual plot:
    //   <plot_data> [, <plot_data> ...]
    // TODO: using must come before title and with
    while (line.isValid() && ! line.isChar(',')) {
      PlotVar plotVar;

      if (! parseOptionValue(this, line, plotVar, "plot var"))
        break;

      line.skipSpace();

      // Read using columns
      // using <col1>[:<col2>[:<col3>[:...]]
      if      (plotVar == PlotVar::USING) {
        parseUsing(line, usingCols);
      }
      // read index range
      // index <start>[:<end>[:<step>]]
      else if (plotVar == PlotVar::INDEX) {
        int indexStart, indexEnd, indexStep;

        parseIndex(line, indexStart, indexEnd, indexStep);

        dataFile_.setIndices(indexStart, indexEnd, indexStep);
      }
      // every <step>[:<start>[:<end>]]
      // TODO (6 values [ipoint:iblock:spoint:epoint:eblock])
      else if (plotVar == PlotVar::EVERY) {
        int everyStart, everyEnd, everyStep;

        parseEvery(line, everyStart, everyEnd, everyStep);

        dataFile_.setEvery(everyStart, everyEnd, everyStep);
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
        if (parseOptionValue(this, line, style, "plot style")) {
          if (isDebug())
            std::cerr << "with " << CStrUniqueMatch::valueToString(style) << std::endl;
        }

        parseModifiers3D(line);
      }
      // title string
      else if (plotVar == PlotVar::TITLE) {
        std::string titleStr;
        int         columnNum;

        if      (parseString(line, titleStr)) {
          if (isDebug())
            std::cerr << "title " << titleStr << std::endl;

          keyTitle = titleStr;
        }
        else if (parseInteger(line, columnNum)) {
          keyData_.setColumnNum (columnNum);
          keyData_.setColumnHead(true);
        }

        line.skipSpace();
      }
      // disable title
      else if (plotVar == PlotVar::NOTITLE) {
      }
      // smooth type
      else if (plotVar == PlotVar::SMOOTH) {
        Smooth smooth;

        if (parseOptionValue(this, line, smooth, "smooth type"))
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

  window->setCamera(camera());

  window->setHidden3D(hidden3D());
  window->setSurface3D(surface3D());
  window->setContour3D(contour3D());
  window->setPm3D(pm3D());

  //---

  if (device_)
    stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
}

bool
CGnuPlot::
parseModifiers3D(CParseLine &line)
{
  bool modifiers = true;

  while (modifiers) {
    line.skipSpace();

    if      (line.isString("linestyle") || line.isString("ls")) {
      line.skipNonSpace();

      int i;

      if (parseInteger(line, i))
        setLineStyleInd(i);
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

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle()->setColorSpec(c);
    }
    else if (line.isString("pointtype") || line.isString("pt")) {
      line.skipNonSpace();

      int i;

      if (parseInteger(line, i))
        lineStyle()->setPointType(static_cast<CGnuPlotTypes::SymbolType>(i));
    }
    else if (line.isString("pointsize") || line.isString("ps")) {
      line.skipNonSpace();

      std::string style = readNonSpaceNonComma(line);

      double s;

      if      (style == "variable" || style == "var")
        pointStyle_.setVarSize(true);
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
  }

  return true;
}

// set ...
bool
CGnuPlot::
setCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "set " << args << std::endl;

  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    std::string var, start, end, inc, lcmd, rcmd;

    if (! parseFor(line, var, start, end, inc, lcmd, rcmd))
      return false;

    setForCmd(var, start, end, inc, lcmd);

    if (rcmd != "")
      setCmd(rcmd);

    return false;
  }

  //----

  VariableName var;

  if (! parseOptionValue(this, line, var, "option name"))
    return false;

  // --- Configuration ---

  // set datafile fortran
  // set datafile nofpe_trap
  // set datafile missing ["<chars>"]
  // set datafile separator ["<chars>"|whitespace]
  // set datafile commentschar ["<chars>"]
  // set datafile binary
  if      (var == VariableName::DATAFILE) {
    DataFileVar var1;

    if (! parseOptionValue(this, line, var1, "data file type"))
      return false;

    line.skipSpace();

    // set datafile separator ["<chars>"|whitespace]
    if      (var1 == DataFileVar::SEPARATOR) {
      if (line.isChar('"')) {
        std::string chars;

        if (parseString(line, chars, "Invalid character string")) {
          if (chars.size() != 1) {
            std::cerr << "Only one character allowed for separator" << std::endl;
            return false;
          }

          dataFile_.setSeparator(chars[0]);
        }
      }
      else {
        std::string sepStr = readNonSpace(line);

        if (sepStr == "whitespace")
          dataFile_.resetSeparator();
        else
          std::cerr << "Invalid separator type" << std::endl;
      }
    }
    // set datafile commentschar ["<chars>"]
    else if (var1 == DataFileVar::COMMENTS_CHAR) {
      if (line.isValid()) {
        std::string chars;

        if (! parseString(line, chars, "Invalid character string"))
          dataFile_.setCommentChars(chars);
      }
      else
        dataFile_.setCommentChars("#");
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
  // set history size <size:i>
  else if (var == VariableName::HISTORY) {
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
    std::string arg = readNonSpace(line);

    if (arg != "")
      varPrefs_[VariableName::ANGLES]->set(arg);
    else
      varPrefs_[VariableName::ANGLES]->reset();
  }
  // set dummy <var1> [, <var2>]
  else if (var == VariableName::DUMMY) {
    std::string var1, var2;

    getDummyVars(var1, var2);

    if (line.isValid()) {
      var1 = readNonSpaceNonComma(line);

      if (line.skipSpaceAndChar(',')) {
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
    parametric_ = true;
  }
  // set polar
  else if (var == VariableName::POLAR) {
    polar_ = true;
  }
  // set samples {samples1:i} [, {samples2:i} ]
  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    int i;

    if (parseInteger(line, i))
      isamples1 = i;

    if (line.skipSpaceAndChar(',')) {
      if (parseInteger(line, i))
        isamples2 = i;
    }

    setSamples(isamples1, isamples2);
  }
  // set zero {expression}
  else if (var == VariableName::ZERO) {
  }

  // set border {<integer>} {front | back} {linewidth | lw <line_width>}
  //            {{linestyle | ls <line_style>} | {linetype | lt <line_type>}}
  else if (var == VariableName::BORDER) {
    axesData_.borders = 0xFF;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "front" || arg == "back") {
      }
      else if (arg == "linewidth" || arg == "lw") {
        double lw;

        if (! parseReal(line, lw))
          axesData_.borderWidth = lw;
      }
      else if (arg == "linestyle" || arg == "ls") {
        int i;

        if (! parseInteger(line, i))
          axesData_.borderStyle = i;
      }
      else if (arg == "linetype" || arg == "lt") {
        int i;

        if (! parseInteger(line, i))
          axesData_.borderType = i;
      }
      else {
        int i;

        if (getIntegerVariable(arg, i))
          axesData_.borders = i;
      }

      arg = readNonSpace(line);
    }
  }
  // set clip
  else if (var == VariableName::CLIP) {
    clip_ = true;
  }
  // set bmargin {at screen} <val>
  else if (var == VariableName::BMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isString("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setBottom(r, screen);
  }
  // set lmargin {at screen} <val>
  else if (var == VariableName::LMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isString("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setLeft(r, screen);
  }
  // set rmargin {at screen} <val>
  else if (var == VariableName::RMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isString("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setRight(r, screen);
  }
  // set tmargin {at screen} <val>
  else if (var == VariableName::TMARGIN) {
    bool screen = false;

    double r = -1;

    if (line.isString("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    if (parseReal(line, r))
      margin_.setTop(r, screen);
  }
  // set margins <left>, <right>, <top>, <bottom>
  else if (var == VariableName::MARGINS) {
    double l = -1, r = -1, t = -1, b = -1;

    (void) parseReal(line, l);

    margin_.setLeft(l);

    if (line.skipSpaceAndChar(',')) {
      (void) parseReal(line, r);

      margin_.setRight(r);

      if (line.skipSpaceAndChar(',')) {
        (void) parseReal(line, t);

        margin_.setTop(t);

        if (line.skipSpaceAndChar(',')) {
          (void) parseReal(line, b);

          margin_.setBottom(b);
        }
      }
    }
  }
  // set multiplot { title <page title> {font <fontspec>} {enhanced|noenhanced} }
  //               { layout <rows>,<cols> {rowsfirst|columnsfirst} {downwards|upwards}
  //                 {scale <xscale>{,<yscale>}} {offset <xoff>{,<yoff>}} }
  else if (var == VariableName::MULTIPLOT) {
    multiplot().enabled = true;
    multiplot().autoFit = true;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "title") {
        std::string titleStr;

        parseString(line, titleStr);
      }
      else if (arg == "font") {
        CFontPtr font;

        (void) parseFont(line, font);
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
      }
      else if (arg == "layout") {
        int i;

        if (parseInteger(line, i))
          multiplot().rows = i;

        if (line.skipSpaceAndChar(',')) {
          if (parseInteger(line, i))
            multiplot().cols = i;
        }

        multiplot().autoFit = false;
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
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
  }
  // set offsets <left>, <right>, <top>, <bottom>
  else if (var == VariableName::OFFSETS) {
  }

  // set logscale <axes> {<base>}
  else if (var == VariableName::LOGSCALE) {
    std::string axesStr;
    int         base = 10;

    if (line.isValid()) {
      axesStr = readNonSpace(line);

      if (! parseInteger(line, base))
        base = 10;
    }

    if (axesStr == "" || axesStr == "x" ) setLogScale(LogScale::X , base);
    if (axesStr == "" || axesStr == "y" ) setLogScale(LogScale::Y , base);
    if (axesStr == "" || axesStr == "z" ) setLogScale(LogScale::Z , base);
    if (axesStr == "" || axesStr == "x2") setLogScale(LogScale::X2, base);
    if (axesStr == "" || axesStr == "y2") setLogScale(LogScale::Y2, base);
    if (axesStr == "" || axesStr == "cb") setLogScale(LogScale::CB, base);
  }

  // set origin <x-origin>,<y-origin>
  else if (var == VariableName::ORIGIN) {
    double r;

    if (parseReal(line, r))
      plotSize_.x = r;

    if (line.skipSpaceAndChar(',')) {
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

        if (line.skipSpaceAndChar(',')) {
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
    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    auto arrow = lookupAnnotation<CGnuPlotArrow>(ind);
    if (! arrow) return false;

    std::string arg = readNonSpace(line);

    if (arg == "from") {
      CGnuPlotPosition p;

      if (parsePosition(line, p))
        arrow->setFrom(p.point());

      arg = readNonSpace(line);

      if (arg == "to" || arg == "rto") {
        if (arg == "rto")
          arrow->setRelative(true);

        if (parsePosition(line, p))
          arrow->setTo(p.point());

        arg = readNonSpace(line);
      }
    }

    line.skipSpace();

    while (arg != "") {
      if       (arg == "arrowstyle" || arg == "as") {
        int as;

        if (parseInteger(line, as)) {
          const CGnuPlotArrowStyle &as1 = arrowStyles_[as];

          arrow->setStyle(as1);
        }
      }
      else if  (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
        if      (arg == "nohead"  ) { arrow->setFHead(false); arrow->setTHead(false); }
        else if (arg == "head"    ) { arrow->setFHead(false); arrow->setTHead(true ); }
        else if (arg == "backhead") { arrow->setFHead(true ); arrow->setTHead(false); }
        else if (arg == "heads"   ) { arrow->setFHead(true ); arrow->setTHead(true ); }
      }
      else if (arg == "size") {
        CGnuPlotCoordValue v;

        if (parseCoordValue(line, v))
          arrow->setLength(v);

        if (line.skipSpaceAndChar(',')) {
          double r;

          if (parseReal(line, r))
            arrow->setAngle(r);

          if (line.skipSpaceAndChar(',')) {
            if (parseReal(line, r))
              arrow->setBackAngle(r);
          }
        }
      }
      else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
        arrow->setFilled(arg == "filled" || arg == "empty");
        arrow->setEmpty (arg == "empty" );
      }
      else if (arg == "front" || arg == "back" || arg == "behind") {
        if      (arg == "front" ) arrow->setLayer(CGnuPlotLayer::FRONT);
        else if (arg == "back"  ) arrow->setLayer(CGnuPlotLayer::BACK);
        else if (arg == "behind") arrow->setLayer(CGnuPlotLayer::BEHIND);
      }
      else if (arg == "linetype" || arg == "lt") {
        int lt = -1;

        if (parseInteger(line, lt))
          arrow->setLineStyle(lt);
      }
      else if (arg == "linewidth" || arg == "lw") {
        double lw = 1.0;

        if (parseReal(line, lw))
          arrow->setLineWidth(lw);
      }
      else if (arg == "linestyle" || arg == "ls") {
        int ls = -1;

        if (! parseInteger(line, ls))
          arrow->setLineStyle(ls);
      }
      else if (arg == "dashtype" || arg == "dt") {
        int dt = -1;

        if (! parseInteger(line, dt))
          arrow->setDashType(dt);
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
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
  //         {{no}box { {linestyle | ls <line_style>} } }
  else if (var == VariableName::KEY) {
    bool hSet = false;

    while (line.isValid()) {
      std::string arg = readNonSpace(line);

      if      (arg == "on" || arg == "off")
        keyData_.setDisplayed(arg == "on");
      else if (arg == "default") { }
      else if (arg == "inside" || arg == "outside")
        keyData_.setOutside(arg == "outside");
      else if (arg == "lmargin") { }
      else if (arg == "rmargin") { }
      else if (arg == "tmargin") { }
      else if (arg == "bmargin") { }
      else if (arg == "at"     ) {
        line.skipSpace(); std::string posStr = readNonSpace(line);
      }
      else if (arg == "left"  ) { keyData_.setHAlign(CHALIGN_TYPE_LEFT  ); hSet = true; }
      else if (arg == "right" ) { keyData_.setHAlign(CHALIGN_TYPE_RIGHT ); hSet = true; }
      else if (arg == "top"   ) { keyData_.setVAlign(CVALIGN_TYPE_TOP   ); }
      else if (arg == "bottom") { keyData_.setVAlign(CVALIGN_TYPE_BOTTOM); }

      else if (arg == "center") { if (hSet)
                                    keyData_.setHAlign(CHALIGN_TYPE_CENTER);
                                  else
                                    keyData_.setVAlign(CVALIGN_TYPE_CENTER);
                                }

      else if (arg == "vertical"  ) { }
      else if (arg == "horizontal") { }
      else if (arg == "under") { }

      else if (arg == "Left" ) { }
      else if (arg == "Right") { }

      else if (arg == "opaque" || arg == "noopaque")
        keyData_.setOpaque(arg == "opaque");
      else if (arg == "reverse" || arg == "noreverse")
        keyData_.setReverse(arg == "reverse");
      else if (arg == "invert" || arg == "noinvert")
        keyData_.setInvert(arg == "invert");

      else if (arg == "samplen") {
        double r;

        if (parseReal(line, r))
          keyData_.setSampLen(r);
      }
      else if (arg == "spacing") {
        double r;

        if (parseReal(line, r))
          keyData_.setSpacing(r);
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
          keyData_.setColumnHead(true);
      }
      else if (arg == "noautotitle") {
        std::string headerStr = readNonSpace(line);
      }
      else if (arg == "title") {
        std::string titleStr;

        if (parseString(line, titleStr))
          keyData_.setTitle(titleStr);
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        keyData_.setEnhanced(arg == "enhanced");
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          keyData_.setFont(font);
      }
      else if (arg == "textcolor" || arg == "tc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          keyData_.setTextColor(c);
      }
      else if (arg == "linetype" || arg == "lt") {
        int lt;

        if (parseInteger(line, lt))
          keyData_.setLineType(lt);
      }
      else if (arg == "linestyle" || arg == "ls") {
        std::string styleStr = readNonSpace(line);
      }
      else if (arg == "box") {
        keyData_.setBox(true);
      }
      else if (arg == "nobox") {
        keyData_.setBox(false);

        if (line.isString("linestyle") || line.isString("ls")) {
          line.skipNonSpace();
          std::string styleStr = readNonSpace(line);
        }
      }
      else if (arg == "above" || arg == "below") {
        keyData_.setBelow(arg == "below");
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
  //           {hypertext}
  else if (var == VariableName::LABEL) {
    line.skipSpace();

    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    //---

    // if no tag then use lowest available value
    auto label = lookupAnnotation<CGnuPlotLabel>(ind);
    if (! label) return false;

    //---

    std::string              arg;
    bool                     isStr = false;
    std::vector<std::string> extraArgs;

    line.skipSpace();

    if (line.isChar('"') || line.isChar('\'')) {
      isStr = true;

      (void) parseString(line, arg);
    }
    else {
      isStr = false;
      arg   = readNonSpace(line);
    }

    while (arg != "") {
      if      (isStr) {
        label->setText(arg);
      }
      else if (arg == "at") {
        CGnuPlotPosition p;

        if (parsePosition(line, p))
          label->setPos(p.point());
      }
      else if (arg == "left" || arg == "center" || arg == "right") {
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
        CFontPtr font;

        if (parseFont(line, font))
          label->setFont(font);
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        label->setEnhanced(arg == "enhanced");
      }
      else if (arg == "front" || arg == "back" || arg == "behind") {
        if      (arg == "front" ) label->setLayer(CGnuPlotLayer::FRONT);
        else if (arg == "back"  ) label->setLayer(CGnuPlotLayer::BACK);
        else if (arg == "behind") label->setLayer(CGnuPlotLayer::BEHIND);
      }
      else if (arg == "textcolor" || arg == "tc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          label->setStrokeColor(c.color());
      }
      else if (arg == "point") {
        int ind;

        if (parseInteger(line, ind))
          label->setPointStyle(ind);
      }
      else if (arg == "nopoint") {
        label->setPointStyle(-1);
      }
      else if (arg == "offset") {
        CPoint2D o(0, 0);

        if (parsePoint(line, o))
          label->setOffset(o);
      }
      else
        extraArgs.push_back(arg);

      line.skipSpace();

      if (line.isChar('"') || line.isChar('\'')) {
        isStr = true;
        (void) parseString(line, arg);
      }
      else {
        isStr = false;
        arg = readNonSpace(line);
      }
    }

    if (! extraArgs.empty()) {
      std::string arg = extraArgs[0];

      std::string str;

      if (getStringVariable(arg, str))
        label->setText(str);
    }
  }
  // set object <index> <object-type> <object-properties>
  //            {front|back|behind} {fc|fillcolor <colorspec>} {fs <fillstyle>}
  //            {default} {lw|linewidth <width>}
  else if (var == VariableName::OBJECT) {
    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    CGnuPlotTypes::ObjectType type = CGnuPlotTypes::ObjectType::NONE;

    if (! parseOptionValue(this, line, type)) {
      auto annotation = getAnnotation<CGnuPlotGroupAnnotation>(ind);

      if (annotation)
        type = annotation->type();
    }

    // set object <index> circle {at|center} <position> size <radius>
    //            {arc [<begin>:<end>]}
    //            {<other-object-properties>}
    if      (type == CGnuPlotTypes::ObjectType::CIRCLE) {
      auto ellipse = lookupAnnotation<CGnuPlotEllipse>(ind);
      if (! ellipse) return false;

      //---

      CSize2D size(1,1);

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "at" || arg == "circle") {
          CGnuPlotPosition p;

          if (parsePosition(line, p))
            ellipse->setCenter(p.point());
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
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            ellipse->setFillColor(c);
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> ellipse {at|center} <position> size <w>,<h>
    //            {angle <orientation>} {units xy|xx|yy}
    //            {<other-object-properties>}
    else if (type == CGnuPlotTypes::ObjectType::ELLIPSE) {
      auto ellipse = lookupAnnotation<CGnuPlotEllipse>(ind);
      if (! ellipse) return false;

      //---

      CSize2D size(1,1);

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "at" || arg == "center") {
          CGnuPlotPosition p;

          if (parsePosition(line, p))
            ellipse->setCenter(p.point());
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
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            ellipse->setFillColor(c);
        }
        else if (arg == "fillstyle" || arg == "fs") {
          CGnuPlotFillStyle fs;

          if (parseFillStyle(line, fs))
            ellipse->setFillStyle(fs);
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> polygon
    //            from <position> to <position> ... {to <position>} |
    //            from <position> rto <position> ... {rto <position>}
    else if (type == CGnuPlotTypes::ObjectType::POLYGON) {
      auto poly = lookupAnnotation<CGnuPlotPolygon>(ind);
      if (! poly) return false;

      //---

      CGnuPlotPosition from, to;

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "from") {
          if (! parsePosition(line, from))
            return false;

          poly->addPoint(from.point());
        }
        else if (arg == "to") {
          if (! parsePosition(line, to))
            return false;

          poly->addPoint(to.point());
        }
        else if (arg == "fillcolor" || arg == "fc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            poly->setFillColor(c);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            poly->setLineWidth(lw);
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> rectangle
    //            {from <position> {to|rto} <position> |
    //            center|at <position> size <w>,<h>}
    else if (type == CGnuPlotTypes::ObjectType::RECTANGLE) {
      auto rect = lookupAnnotation<CGnuPlotRectangle>(ind);
      if (! rect) return false;

      //---

      CGnuPlotPosition pos;
      CGnuPlotSize     size;

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "from") {
          if (parsePosition(line, pos))
            rect->setFrom(pos);
        }
        else if (arg == "to") {
          if (parsePosition(line, pos))
            rect->setTo(pos);
        }
        else if (arg == "rto") {
          if (parsePosition(line, pos))
            rect->setRTo(pos);
        }
        else if (arg == "center" || arg == "at") {
          if (parsePosition(line, pos))
            rect->setCenter(pos);
        }
        else if (arg == "size") {
          if (parseSize(line, size))
            rect->setSize(size);
        }
        else if (arg == "fillcolor" || arg == "fc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            rect->setFillColor(c);
        }
        else if (arg == "fillstyle" || arg == "fs") {
          CGnuPlotFillStyle fs;

          if (parseFillStyle(line, fs))
            rect->setFillStyle(fs);
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            rect->setLineType(lt);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            rect->setLineWidth(lw);
        }
        else if (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) rect->setLayer(CGnuPlotLayer::FRONT);
          else if (arg == "back"  ) rect->setLayer(CGnuPlotLayer::BACK);
          else if (arg == "behind") rect->setLayer(CGnuPlotLayer::BEHIND);
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
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
      title_.setText(s);

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "offset") {
        CPoint2D o(0, 0);

        if (parsePoint(line, o))
          title_.setOffset(o);
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          title_.setFont(font);
      }
      else if (arg == "textcolor" || arg == "tc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          title_.setColor(c);
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        title_.setEnhanced(arg == "enhanced");
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
  }

  // set style [data|function|increment] <args>
  else if (var == VariableName::STYLE) {
    StyleVar var1;

    if (! parseOptionValue(this, line, var1, "style type"))
      return false;

    line.skipSpace();

    // set style data {style}
    if      (var1 == StyleVar::DATA) {
      PlotStyle style(PlotStyle::NONE);

      if (parseOptionValue(this, line, style, "plot style"))
        setDataStyle(style);
    }
    // set style function {style}
    else if (var1 == StyleVar::FUNCTION) {
      PlotStyle style(PlotStyle::NONE);

      if (parseOptionValue(this, line, style, "plot style"))
        setFunctionStyle(style);
    }
    // set style increment { user | default }
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
      CGnuPlotArrowStyle *arrowStyle1 = &arrowStyle_;

      int arrowId = -1;

      if (parseInteger(line, arrowId)) {
        auto p = arrowStyles_.find(arrowId);

        if (p == arrowStyles_.end())
          p = arrowStyles_.insert(p, ArrowStyles::value_type(arrowId, CGnuPlotArrowStyle()));

        arrowStyle1 = &(*p).second;
      }

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "default") {
          *arrowStyle1 = CGnuPlotArrowStyle();
        }
        else if (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
          if      (arg == "nohead"  ) { arrowStyle1->setHeads(false, false); }
          else if (arg == "head"    ) { arrowStyle1->setHeads(false, true ); }
          else if (arg == "backhead") { arrowStyle1->setHeads(true , false); }
          else if (arg == "heads"   ) { arrowStyle1->setHeads(true , true ); }
        }
        else if (arg == "size") {
          CGnuPlotCoordValue v;

          if (parseCoordValue(line, v))
            arrowStyle1->setLength(v);

          if (line.skipSpaceAndChar(',')) {
            double r;

            if (parseReal(line, r))
              arrowStyle1->setAngle(r);

            if (line.skipSpaceAndChar(',')) {
              if (parseReal(line, r))
                arrowStyle1->setBackAngle(r);
            }
          }
        }
        else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
          arrowStyle1->setFilled(arg == "filled" || arg == "empty");
          arrowStyle1->setEmpty (arg == "empty" );
        }
        else if (arg == "front" || arg == "back") {
          arrowStyle1->setFront(arg == "front");
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            arrowStyle1->setLineStyle(lt);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            arrowStyle1->setLineWidth(lw);
        }
        else if (arg == "linestyle" || arg == "ls") {
          int ls = -1;

          if (parseInteger(line, ls))
            arrowStyle1->setLineStyle(ls);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            arrowStyle1->setLineColor(c.color());
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style fill {empty |
    //                 {transparent} solid {density} |
    //                 {transparent} pattern {id}}
    //                {border {lt} {lc <colorspec>} | noborder}
    else if (var1 == StyleVar::FILL) {
      CGnuPlotTypes::FillType fillType;

      if (parseOptionValue(this, line, fillType, "fill type"))
        fillStyle_.setStyle(fillType);

      line.skipSpace();

      if      (fillType == CGnuPlotTypes::FillType::SOLID) {
        double density = 1.0;

        if (parseReal(line, density))
          fillStyle_.setDensity(density);

        line.skipSpace();
      }
      else if (fillType == CGnuPlotTypes::FillType::PATTERN) {
        int patternNum = 0;

        if (parseInteger(line, patternNum))
          fillStyle_.setPattern(static_cast<CGnuPlotTypes::FillPattern>(patternNum));

        line.skipSpace();
      }

      if (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "border" || arg == "bo") {
          int fillBorderLine = -1;

          line.skipSpace();

          if (line.isValid()) {
            int i;

            if (parseInteger(line, i))
              fillBorderLine = i;
          }

          fillStyle_.setBorder(true);
          fillStyle_.setBorderLineType(fillBorderLine);
        }
        else if (arg == "noborder")
          fillStyle_.setBorder(false);
        else
          errorMsg("Invalid fill border '" + arg + "'");
      }
    }
    // set style histogram [ clustered [ gap {gap} ] |
    //                       errorbars [ gap {gap} ] [ linewidth|lw {width} ] |
    //                       rowstacked |
    //                       columnstacked
    else if (var1 == StyleVar::HISTOGRAM) {
      HistogramStyle style;

      if (parseOptionValue(this, line, style, "histogram style"))
        histogramData_.setStyle(style);

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "gap") {
          double gap;

          if (parseReal(line, gap))
            histogramData_.setGap(gap);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            histogramData_.setLineWidth(lw);
        }
        else if (arg == "title") {
        }
        else if (arg == "offset") {
          CPoint2D o(0, 0);

          if (parsePoint(line, o))
            histogramData_.setTitleOffset(o);
        }
        else if (arg == "font") {
          CFontPtr font;

          if (parseFont(line, font))
            histogramData_.setTitleFont(font);
        }
        else if (arg == "boxed") {
        }
        else
          errorMsg("Invalid histogram style '" + arg + "'");

        arg = readNonSpace(line);
      }
    }
    // set style line {index:i} [ [ linetype  | lt ] {linetype:i} ]
    //                            [ linewidth | lw ] {linewidth:f} ]
    //                            [ pointtype | pt ] {pointtype:i} ]
    //                            [ pointsize | ps ] {pointsize:f} ]
    //                            [ linecolor | lc ] {colorspace:c} ]
    else if (var1 == StyleVar::LINE) {
      std::string indStr = readNonSpace(line);

      int i;

      int ind = 0;

      if (parseInteger(line, i))
        ind = i;

      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      line.skipSpace();

      CGnuPlotPointStyle pointStyle;
      double             r;

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
          int i;

          if (parseInteger(line, i))
            lineStyle->setPointType(static_cast<CGnuPlotTypes::SymbolType>(i));
        }
        else if (arg == "pointsize" || arg == "ps") {
          if (parseReal(line, r))
            lineStyle->setPointSize(r);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            lineStyle->setColorSpec(c);
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
    // set style circle
    else if (var1 == StyleVar::CIRCLE) {
    }
    // set style ellipse {units xx|xy|yy}
    //                   {size {graph|screen} <a>, {{graph|screen} <b>}}
    //                   {angle <angle>}
    else if (var1 == StyleVar::ELLIPSE) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "units") {
          arg = readNonSpace(line);
        }
        else if (arg == "size") {
          line.skipSpace();

          if (line.isString("graph") || line.isString("screen"))
            arg = readNonSpace(line);

          double sa;
          parseReal(line, sa);

          if (line.skipSpaceAndChar(',')) {
            if (line.isString("graph") || line.isString("screen"))
              arg = readNonSpace(line);

            double sb;
            parseReal(line, sb);
          }
        }
        else if (arg == "angle") {
          double a;
          parseReal(line, a);
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style textbox
    else if (var1 == StyleVar::TEXTBOX) {
      // TODO:
    }

    // set style boxplot {range <r> | fraction <f>}
    //                   {{no}outliers} {pointtype <p>}
    //                   {candlesticks | financebars}
    //                   {separation <x>}
    //                   {labels off | auto | x | x2}
    //                   {sorted | unsorted}
    else if (var1 == StyleVar::BOXPLOT) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "range") {
          double r;
          (void) parseReal(line, r);
        }
        else if (arg == "fraction") {
          double r;
          (void) parseReal(line, r);
        }
        else if (arg == "outliers" || arg == "nooutliers") {
          boxPlot_.outliers = (arg == "outliers");
        }
        else if (arg == "pointtype") {
          int i;

          if (parseInteger(line, i))
            boxPlot_.pointtype = i;
        }
        else if (arg == "candlesticks") {
        }
        else if (arg == "financebars") {
        }
        else if (arg == "separation") {
          double r;
          (void) parseReal(line, r);
        }
        else if (arg == "labels") {
          arg = readNonSpace(line);

          if      (arg == "off") {
          }
          else if (arg == "auto") {
          }
          else if (arg == "x") {
          }
          else if (arg == "x2") {
          }
        }
        else if (arg == "sorted" || arg == "unsorted") {
          boxPlot_.sorted = (arg == "sorted");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
  }
  // set linetype <n> <styles...>
  else if (var == VariableName::LINETYPE) {
  }
  // set dashtype
  else if (var == VariableName::DASHTYPE) {
    int type;

    if (! parseInteger(line, type))
      return false;

    std::vector<double> dashes;

    if (line.skipSpaceAndChar('(')) {
      double l;

      if (! parseReal(line, l))
        return false;

      dashes.push_back(l);

      while (line.skipSpaceAndChar(',')) {
        if (! parseReal(line, l))
          break;

        dashes.push_back(l);
      }

      if (! line.skipSpaceAndChar(')'))
        return false;
    }

    lineDashes_[type] = CLineDash(dashes);
  }
  else if (var == VariableName::POINTINTERVALBOX) {
  }

  // set bars {small | large | fullwidth | size} ] {front | back}
  else if (var == VariableName::BARS) {
    line.skipSpace();

    double size  = 1.0;
    bool   front = true;

    if      (line.isString("small") || line.isString("large") || line.isString("fullwidth")) {
      std::string str = readNonSpace(line);

      if      (str == "small"    ) size = 0.0;
      else if (str == "large"    ) size = 1.0;
      else if (str == "fullwidth") size = -2.0;
    }
    else if (parseReal(line, size)) {
    }

    line.skipSpace();

    if      (line.isString("front") || line.isString("back")) {
      std::string str = readNonSpace(line);

      if      (str == "front") front = true;
      else if (str == "back" ) front = false;
    }

    setBarSize(BarSize(size, front));
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

          if      (arg == "abs" || arg == "absolute")
            boxWidthType = BoxWidthType::ABSOLUTE;
          else if (arg == "rel" || arg == "relative")
            boxWidthType = BoxWidthType::RELATIVE;
        }
        else
          boxWidthType = BoxWidthType::ABSOLUTE;
      }
    }

    setBoxWidth(BoxWidth(boxWidth, boxWidthType));
  }
  // set pointintervalbox

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
      axesData_.xaxis[1].resetMin();
      axesData_.xaxis[1].resetMax();
    }
    else if (arg == "y") {
      axesData_.yaxis[1].resetMin();
      axesData_.yaxis[1].resetMax();
    }
  }

  // set format {<axes>} {"<format-string>"}
  else if (var == VariableName::FORMAT) {
    std::string arg = readNonSpace(line);

    std::string formatStr;

    if (parseString(line, formatStr, "Invalid format string")) {
      if      (arg == "x") axesData_.xaxis[1].setFormat(formatStr);
      else if (arg == "y") axesData_.yaxis[1].setFormat(formatStr);
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
    axesData_.xaxis[1].setGrid(true);
    axesData_.yaxis[1].setGrid(true);
  }
  // set xlabel "<label>"
  else if (var == VariableName::XLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid xlabel string"))
      axesData_.xaxis[1].setText(labelStr);
  }
  // set ylabel "<label>"
  else if (var == VariableName::YLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.yaxis[1].setText(labelStr);
  }
  // set x2label "<label>"
  else if (var == VariableName::X2LABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid x2label string"))
      axesData_.xaxis[2].setText(labelStr);
  }
  // set y2label "<label>"
  else if (var == VariableName::Y2LABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.yaxis[2].setText(labelStr);
  }
  // set cblabel "<label>"
  else if (var == VariableName::CBLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.cbaxis.setText(labelStr);
  }
  // set xrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::XRANGE) {
    parseAxisRange(line, axesData_.xaxis[1]);
  }
  // set yrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::YRANGE) {
    parseAxisRange(line, axesData_.yaxis[1]);
  }
  // set x2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::X2RANGE) {
    parseAxisRange(line, axesData_.xaxis[2]);
  }
  // set y2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::Y2RANGE) {
    parseAxisRange(line, axesData_.yaxis[2]);
  }
  // set zrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::ZRANGE) {
    parseAxisRange(line, axesData_.zaxis[1]);
  }
  // set trange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::TRANGE) {
    parseAxisRange(line, axesData_.taxis[1]);
  }
  // set rrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::RRANGE) {
    int pos = line.pos();

    parseAxisRange(line, axesData_.xaxis[1]);

    line.setPos(pos);

    parseAxisRange(line, axesData_.yaxis[1]);
  }
  // set cbrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::CBRANGE) {
    parseAxisRange(line, axesData_.cbaxis);
  }
  // set urange
  // set vrange
  // set tics {axis | border} {{no}mirror}
  //          {in | out} {scale {default | <major> {,<minor>}}}
  //          {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //          {left | right | center | autojustify}
  //          { format "formatstring" } { font "name{,<size>}" }
  //          { textcolor <colorspec> }
  else if (var == VariableName::TICS) {
    int pos = line.pos();

    parseAxesTics(line, axesData_.xaxis[1]); line.setPos(pos);
    parseAxesTics(line, axesData_.yaxis[1]); line.setPos(pos);
  }
  // set xtics {axis | border} {{no}mirror}
  //           {in | out} {scale {default | <major> {,<minor>}}}
  //           {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //           {left | right | center | autojustify}
  //           {add}
  //           { autofreq | <incr> | <start>, <incr> {,<end>} |
  //             ({"<label>"} <pos> {<level>} {,{"<label>"}...) }}
  //           { format "formatstring" } { font "name{,<size>}" }
  //           { rangelimited }
  //           { textcolor <colorspec> }
  else if (var == VariableName::XTICS) {
    parseAxesTics(line, axesData_.xaxis[1]);
  }
  else if (var == VariableName::YTICS) {
    parseAxesTics(line, axesData_.yaxis[1]);
  }
  else if (var == VariableName::X2TICS) {
    parseAxesTics(line, axesData_.xaxis[2]);
  }
  else if (var == VariableName::Y2TICS) {
    parseAxesTics(line, axesData_.yaxis[2]);
  }
  else if (var == VariableName::ZTICS) {
    parseAxesTics(line, axesData_.zaxis[1]);
  }
  else if (var == VariableName::RTICS) {
  }
  else if (var == VariableName::CBTICS) {
    parseAxesTics(line, axesData_.cbaxis);
  }
  // set zeroaxis
  else if (var == VariableName::ZEROAXIS) {
  }
  // set paxis <no> {range <range-options> | {tics <tic-options>}}
  else if (var == VariableName::PAXIS) {
    int i;

    if (! parseInteger(line, i))
      return false;

    std::string arg = readNonSpace(line);

    if      (arg == "range")
      parseAxisRange(line, axesData_.paxis[i]);
    else if (arg == "tics")
      parseAxesTics(line, axesData_.paxis[i]);
  }
  // set link
  else if (var == VariableName::LINK) {
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
      axesData_.xaxis[1].setIsTime(true);
  }
  // set ydata {time}
  else if (var == VariableName::YDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.yaxis[1].setIsTime(true);
  }

  // set dgrid3d
  else if (var == VariableName::DGRID3D) {
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
      int i;

      if (parseInteger(line, i))
        setTrianglePattern3D(i);
    }
    else
      setHidden3D(true);
  }
  // set isosamples <iso_1> {,<iso_2>}
  else if (var == VariableName::ISOSAMPLES) {
    int isamples1, isamples2;

    getIsoSamples(isamples1, isamples2);

    if (line.isValid()) {
      int i;

      if (parseInteger(line, i)) {
        isamples1 = i;
        isamples2 = i;
      }

      if (line.skipSpaceAndChar(',')) {
        if (parseInteger(line, i))
          isamples2 = i;
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
      camera_.setMap();
    }
  }
  // set xyplane at <zvalue>
  // set xyplane relative <frac>
  else if (var == VariableName::XYPLANE) {
  }

  // set cntrlabel

  // set cntrparam { { linear | cubicspline | bspline | points <n> | order <n> |
  //                   levels { auto {<n>} | <n> | discrete <z1> {,<z2>{,<z3>...}} |
  //                            incremental <start>, <incr> {,<end>} } } }
  else if (var == VariableName::CNTRPARAM) {
  }
  // set contour {base | surface | both}
  else if (var == VariableName::CONTOUR) {
    setContour3D(true);
  }

  // set colorbox
  // set colorbox { { vertical | horizontal }
  //                { default | user }
  //                { origin x, y }
  //                { size x, y }
  //                { front | back }
  //                { noborder | bdefault | border [line style] } }
  else if (var == VariableName::COLORBOX) {
    colorBox_.setEnabled(true);

    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "vertical" || arg == "horizontal")
        colorBox_.setVertical(arg == "vertical");
      else if (arg == "default" || arg == "user") {
        colorBox_.setUser(arg == "user");
      }
      else if (arg == "origin") {
        CPoint2D o(0, 0);

        if (parsePoint(line, o))
          colorBox_.setOrigin(o);
      }
      else if (arg == "size") {
        CSize2D size(1, 1);

        if (parseSize(line, size))
          colorBox_.setSize(size);
      }
      else if (arg == "front" || arg == "back") {
        colorBox_.setFront(arg == "front");
      }
      else if (arg == "noborder") {
        colorBox_.setBorder(false);
      }
      else if (arg == "bdefault") {
        colorBox_.setBorder(true);
        colorBox_.setBorderStyle(-1);
      }
      else if (arg == "border") {
        colorBox_.setBorder(true);

        int i;

        if (parseInteger(line, i))
          colorBox_.setBorderStyle(i);
      }

      arg = readNonSpaceNonComma(line);
    }
  }
  // set colornames

  // set colorsequence {default|classic|podo}
  else if (var == VariableName::COLORSEQUENCE) {
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
    palette_.setColorModel(CGnuPlotTypes::ColorModel::RGB);

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if (arg == "gray" || arg == "color") {
        palette_.setGray(arg == "gray");
      }
      else if (arg == "gamma") {
        double g;

        if (parseReal(line, g))
          palette_.setGamma(g);
      }
      else if (arg == "rgbformulae") {
        int r = 1, g = 1, b = 1;

        if (parseInteger(line, r)) {
          if (line.skipSpaceAndChar(',')) {
            if (parseInteger(line, g)) {
              if (line.skipSpaceAndChar(',')) {
                if (parseInteger(line, b)) {
                  palette_.setColorType(CGnuPlotPalette::ColorType::MODEL);

                  palette_.setRgbModel(r, g, b);
                }
              }
            }
          }
        }
      }
      else if (arg == "defined") {
        if (line.skipSpaceAndChar('(')) {
          typedef std::pair<double,CRGBA> ValueColor;
          typedef std::vector<ValueColor> ValueColors;

          ValueColors valueColors;

          while (line.isValid() && ! line.isChar(')')) {
            double gray = 0.0;

            if (! parseReal(line, gray))
              break;

            CRGBA c;

            parseColor(line, c);

            valueColors.push_back(ValueColor(gray, c));

            line.skipSpace();

            if (line.isChar(','))
              line.skipChar();
          }

          if (line.isChar(')'))
            line.skipChar();

          if (! valueColors.empty()) {
            palette_.setColorType(CGnuPlotPalette::ColorType::DEFINED);

            palette_.resetDefinedColors();

            for (const auto &c : valueColors)
              palette_.addDefinedColor(c.first, c.second);
          }
        }
      }
      else if (arg == "file") {
        // TODO
      }
      else if (arg == "functions") {
        // TODO
      }
      else if (arg == "model" || arg == "mode") {
        arg = readNonSpace(line);

        if      (arg == "RGB") palette_.setColorModel(CGnuPlotTypes::ColorModel::RGB);
        else if (arg == "HSV") palette_.setColorModel(CGnuPlotTypes::ColorModel::HSV);
        else if (arg == "CMY") palette_.setColorModel(CGnuPlotTypes::ColorModel::CMY);
        else if (arg == "YIQ") palette_.setColorModel(CGnuPlotTypes::ColorModel::YIQ);
        else if (arg == "XYZ") palette_.setColorModel(CGnuPlotTypes::ColorModel::XYZ);
      }
      else if (arg == "positive" || arg == "negative") {
        palette_.setNegative(arg == "negative");
      }
      else if (arg == "maxcolors") {
        // TODO
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
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
  // set psdir <dir>
  // set table ["<filename>"]
  else if (var == VariableName::TABLE) {
    std::string filename;

    if (parseString(line, filename, "Invalid filename"))
      setTableFile(filename);
  }
  // set terminal <type> [<args>]
  else if (var == VariableName::TERMINAL) {
    std::string arg = readNonSpace(line);

    if (! setDevice(arg))
      std::cerr << "No device " << arg << std::endl;

    arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "size") {
        int w, h;

        if (parseInteger(line, w)) {
          if (line.skipSpaceAndChar(',')) {
            if (parseInteger(line, h))
              setTerminalSize(CISize2D(w, h));
          }
        }
      }
      else if (arg == "background") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          setBackgroundColor(c.color());
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        setEnhanced(arg == "enhanced");
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
  }
  // set termoption [no]enhanced
  // set termoption font "<name> [, <size:i> ]"
  else if (var == VariableName::TERMOPTION) {
    std::string arg = readNonSpace(line);

    if (arg == "enhanced" || arg == "noenhanced") {
      setEnhanced(arg == "enhanced");
    }
  }
  else if (var == VariableName::DEBUG) {
    setDebug(true);
  }
  else if (var == VariableName::EDEBUG) {
    setExprDebug(true);
  }

  return true;
}

void
CGnuPlot::
setForCmd(const std::string &var, const std::string &start, const std::string &end,
          const std::string &inc, const std::string &args)
{
  CExprValuePtr value1;

  if (! evaluateExpression(start, value1))
    return;

  CExprValuePtr value2;

  if (! evaluateExpression(end, value2))
    return;

  CExprValuePtr value3;

  if (inc != "") {
    if (! evaluateExpression(inc, value3))
      value3 = CExprValuePtr();
  }

  long i1 = 0, i2 = 1, i3 = 1;

  if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
    return;

  if (value3.isValid() && ! value3->getIntegerValue(i3))
    return;

  while (i1 <= i2) {
    CExprInst->createIntegerVariable(var, i1);

    setCmd(args);

    i1 += i3;
  }
}

// show ...
// show plot
// show [ variables [all] | functions ]
// show [ all | version ] [ long ]
bool
CGnuPlot::
showCmd(const std::string &args)
{
  CParseLine line(args);

  VariableName var;

  if (! parseOptionValue(this, line, var)) {
    errorMsg("Missing option name");
    return false;
  }

  //---

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  //---

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

    if (args1.size() > 0)
      show[args1[0]] = true;

    if (show.empty() || show.find("missing") != show.end()) {
      if (getMissingStr().empty())
        std::cerr << "No missing data string set for datafile" << std::endl;
      else
        std::cerr << "\"" << getMissingStr() <<
                     "\" in datafile is interpreted as missing value" << std::endl;
    }
    if (show.empty() || show.find("separator") != show.end()) {
      std::cerr << "datafile fields separated by ";

      if (! dataFile_.getSeparator())
        std::cerr << "whitespace";
     else
        std::cerr << "\"" << dataFile_.getSeparator() << "\"";

      std::cerr << std::endl;
    }
    if (show.empty() || show.find("commentschar") != show.end()) {
      std::cerr << "Comments chars are \"" << dataFile_.commentChars() << "\"" << std::endl;
    }
  }
  // show title
  else if (var == VariableName::TITLE) {
    std::cerr << "title is \"" << title_.text() << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  // show arrow
  else if (var == VariableName::ARROW) {
    showAnnotations<CGnuPlotArrow>();
  }
  // show ellipse
  else if (var == VariableName::ELLIPSE) {
    showAnnotations<CGnuPlotEllipse>();
  }
  // show label
  else if (var == VariableName::LABEL) {
    showAnnotations<CGnuPlotLabel>();
  }
  // show polygon
  else if (var == VariableName::POLYGON) {
    showAnnotations<CGnuPlotPolygon>();
  }
  // show rectangle
  else if (var == VariableName::RECTANGLE) {
    showAnnotations<CGnuPlotRectangle>();
  }
  // show object
  else if (var == VariableName::OBJECT) {
    showAnnotations<CGnuPlotGroupAnnotation>();
  }

  else if (var == VariableName::STYLE) {
    std::map<std::string,bool> show;

    if (args1.size() > 0)
      show[args1[0]] = true;

    if      (show.empty() || show.find("data") != show.end()) {
      std::cerr << "Data are plotted with " <<
        CStrUniqueMatch::valueToString<PlotStyle>(getDataStyle()) << std::endl;
    }
    else if (show.find("arrow") != show.end()) {
      for (const auto &as : arrowStyles_) {
        std::cout << "arrowstyle " << as.first << ", ";
        as.second.print(this, std::cout);
        std::cout << std::endl;
      }
    }
    else if (show.find("line") != show.end()) {
      for (const auto &lt : lineStyles_) {
        std::cout << "linestyle " << lt.first << ", ";
        lt.second->print(std::cout);
        std::cout << std::endl;
      }
    }
  }

  else if (var == VariableName::XLABEL) {
    std::cerr << "xlabel is \"" << axesData_.xaxis[1].text() << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }
  else if (var == VariableName::YLABEL) {
    std::cerr << "ylabel is \"" << axesData_.yaxis[1].text() << "\", " <<
                 "offset at ((character units) 0, 0, 0)" << std::endl;
  }

  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    getSamples(isamples1, isamples2);

    std::cerr << "sampling rate is " << isamples1 << ", " << isamples2 << std::endl;
  }
  else if (var == VariableName::BORDER) {
  }
  else if (var == VariableName::ANGLES) {
    varPrefs_[VariableName::ANGLES]->print(std::cerr);
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

  return true;
}

// reset
void
CGnuPlot::
resetCmd(const std::string &)
{
  // TODO: do not reset terminal and output
}

// undefine ...
void
CGnuPlot::
undefCmd(const std::string &)
{
  // TODO
}

// unset ...
bool
CGnuPlot::
unsetCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "unset " << args << std::endl;

  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    std::string var, start, end, inc, lcmd, rcmd;

    if (! parseFor(line, var, start, end, inc, lcmd, rcmd))
      return false;

    unsetForCmd(var, start, end, inc, lcmd);

    if (rcmd != "")
      unsetCmd(rcmd);

    return false;
  }

  //---

  VariableName var;

  if (! parseOptionValue(this, line, var, "option name"))
    return false;

  //----

  std::vector<std::string> args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  //---

  if      (var == VariableName::MULTIPLOT) {
    if (multiWindow_ && multiplot().autoFit)
      multiWindow_->fitGroups();

    multiplot().enabled = false;

    multiWindow_ = 0;
  }
  else if (var == VariableName::BORDER)
    axesData_.borders = 0;
  else if (var == VariableName::ARROW) {
    clearAnnotations<CGnuPlotArrow>();
  }
  else if (var == VariableName::ELLIPSE) {
    clearAnnotations<CGnuPlotEllipse>();
  }
  else if (var == VariableName::LABEL) {
    clearAnnotations<CGnuPlotLabel>();
  }
  else if (var == VariableName::POLYGON) {
    clearAnnotations<CGnuPlotPolygon>();
  }
  else if (var == VariableName::RECTANGLE) {
    clearAnnotations<CGnuPlotRectangle>();
  }
  else if (var == VariableName::KEY)
    keyData_.setDisplayed(false);
  else if (var == VariableName::TITLE)
    title_.setText("");
  else if (var == VariableName::GRID) {
    axesData_.xaxis[1].setGrid(false);
    axesData_.yaxis[1].setGrid(false);
  }
  else if (var == VariableName::XLABEL)
    axesData_.xaxis[1].setText("");
  else if (var == VariableName::YLABEL)
    axesData_.yaxis[1].setText("");
  else if (var == VariableName::TICS) {
  }
  // unset xtics
  else if (var == VariableName::XTICS) {
    axesData_.xaxis[1].setShowTics(false);
  }
  else if (var == VariableName::YTICS) {
    axesData_.yaxis[1].setShowTics(false);
  }
  else if (var == VariableName::X2TICS) {
    axesData_.xaxis[2].setShowTics(false);
  }
  else if (var == VariableName::Y2TICS) {
    axesData_.yaxis[2].setShowTics(false);
  }
  else if (var == VariableName::ZTICS) {
    axesData_.zaxis[1].setShowTics(false);
  }
  else if (var == VariableName::RTICS) {
    axesData_.xaxis[1].setShowTics(false);
    axesData_.yaxis[1].setShowTics(false);
  }
  else if (var == VariableName::CBTICS) {
    axesData_.cbaxis.setShowTics(false);
  }
  else if (var == VariableName::HIDDEN3D)
    setHidden3D(false);
  else if (var == VariableName::SURFACE)
    setSurface3D(false);
  else if (var == VariableName::CONTOUR)
    setContour3D(false);
  else if (var == VariableName::COLORBOX) {
    colorBox_.setEnabled(false);
  }
  else if (var == VariableName::PM3D)
    setPm3D(false);
  else if (var == VariableName::TABLE)
    setTableFile("");
  else if (var == VariableName::ANGLES) {
    varPrefs_[VariableName::ANGLES]->reset();
  }
  // unset parametric
  else if (var == VariableName::PARAMETRIC)
    parametric_ = false;
  // unset polar
  else if (var == VariableName::POLAR)
    polar_ = false;
  else if (var == VariableName::ZEROAXIS) {
  }
  else if (var == VariableName::DEBUG)
    setDebug(false);
  else if (var == VariableName::EDEBUG)
    CExprInst->setDebug(false);

  return false;
}

void
CGnuPlot::
unsetForCmd(const std::string &var, const std::string &start, const std::string &end,
            const std::string &inc, const std::string &args)
{
  CExprValuePtr value1;

  if (! evaluateExpression(start, value1))
    return;

  CExprValuePtr value2;

  if (evaluateExpression(end, value2))
    return;

  CExprValuePtr value3;

  if (inc != "") {
    if (! evaluateExpression(inc, value3))
      return;
  }

  long i1 = 0, i2 = 1, i3 = 1;

  if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
    return;

  if (value3.isValid() && ! value3->getIntegerValue(i3))
    return;

  while (i1 <= i2) {
    CExprInst->createIntegerVariable(var, i1);

    unsetCmd(args);

    i1 += i3;
  }
}

// shell
void
CGnuPlot::
shellCmd(const std::string &)
{
  const char *shell = getenv("SHELL");

  if (! shell)
    shell = "/bin/sh";

  system(shell);
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

// stats
void
CGnuPlot::
statsCmd(const std::string &)
{
  // TODO
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

  CGnuPlotWindowP window = CGnuPlotWindowP(createWindow());

  window->setBackgroundColor(backgroundColor());

  windows_.push_back(window);

  CGnuPlotGroup *group = createGroup(window.get());

  group->init();
  group->set3D(false);

  CGnuPlotPlot *plot = createPlot(group);

  group->addSubPlots({plot});

  if (arg == "palette") {
    plot->setStyle(PlotStyle::TEST_PALETTE);

    group->setMargin(Margin(5, 10, 5, 10));
    group->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }
  else {
    plot->setStyle(PlotStyle::TEST_TERMINAL);

    group->setMargin(Margin(0, 0, 0, 0));
    group->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }

  window->addGroup(group);

  if (device_)
    stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
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
ifCmd(int &i, const Statements &statements)
{
  CParseLine line(statements[i]);

  std::string identifier;

  (void) readIdentifier(line, identifier);

  assert(identifier == "if");

  bool processing = false;

  if (! parseBoolExpression(line, processing))
    return;

  Statements lines;

  if (line.skipSpaceAndChar('{')) {
    line.skipSpace();

    if (processing) {
      if (line.substr() != "")
        lines.push_back(line.substr());
    }

    // read if block
    Statements  lines1;
    std::string eline1;

    readBlockLines(lines1, eline1, 1);

    if (processing) {
      for (const auto &line1 : lines1)
        lines.push_back(line1);
    }

    // check for else block
    CParseLine line1(eline1);

    std::string identifier1;

    (void) readIdentifier(line1, identifier1);

    if (identifier1 == "else") {
      processing = ! processing;

      if (! line1.skipSpaceAndChar('{'))
        return;

      line1.skipSpace();

      if (processing) {
        if (line1.substr() != "")
          lines.push_back(line1.substr());
      }

      // read else block
      Statements  lines2;
      std::string eline2;

      readBlockLines(lines2, eline2, 1);

      if (processing) {
        for (const auto &line2 : lines2)
          lines.push_back(line2);
      }
    }
  }
  else {
    line.skipSpace();

    if (processing)
      lines.push_back(line.substr());

    while (i < int(statements.size()) - 1) {
      ++i;

      CParseLine line1(statements[i]);

      std::string identifier1;

      (void) readIdentifier(line1, identifier1);

      if (identifier1 == "else") {
        int pos1 = line1.pos();

        std::string identifier2;

        (void) readIdentifier(line1, identifier2);

        if (identifier2 == "if") {
          bool processing1;

          if (! parseBoolExpression(line1, processing1))
            return;

          if (! processing) {
            processing = processing1;

            if (processing)
              lines.push_back(line1.substr());
          }
        }
        else {
          line1.setPos(pos1);

          if (processing) {
            i = statements.size() - 1;
            break;
          }
          else {
            processing = ! processing;

            lines.push_back(line1.substr());
          }
        }
      }
      else {
        if (processing)
          lines.push_back(statements[i]);
      }
    }
  }

  for (const auto &line : lines)
    fileData_.bufferLines.push_back(line);
}

void
CGnuPlot::
readBlockLines(Statements &lines, std::string &eline, int depth)
{
  std::string str;

  while (fileReadLine(str)) {
    CParseLine line(str);

    if (line.skipSpaceAndChar('}')) {
      if (depth > 1)
        lines.push_back(str);

      line.skipSpace();

      eline = line.substr();

      break;
    }
    else {
      lines.push_back(str);

      std::string identifier;

      (void) readIdentifier(line, identifier);

      // TODO: other block commands
      if      (identifier == "if") {
        line.skipSpace();

        if (line.isChar('(')) {
          std::string expr;

          if (! parseExpression(line, expr))
            return;

          if (line.skipSpaceAndChar('{')) {
            std::string eline1;

            readBlockLines(lines, eline1, depth + 1);

            // check for else block
            CParseLine line1(eline1);

            std::string identifier1;

            (void) readIdentifier(line1, identifier1);

            if (identifier1 == "else") {
              if (! line1.skipSpaceAndChar('{'))
                return;

              // read else block
              std::string eline2;

              readBlockLines(lines, eline2, depth + 1);
            }
          }
        }
      }
      else if (identifier == "do") {
        std::string eline1;

        readBlockLines(lines, eline1, depth + 1);
      }
    }
  }
}

void
CGnuPlot::
parseUsing(CParseLine &line, CGnuPlotUsingCols &usingCols)
{
  std::string              usingStr;
  std::vector<std::string> usingStrs;
  bool                     isExpr = false;

  int rbrackets = 0;

  while (line.isValid()) {
    if (rbrackets == 0) {
      if (line.isSpace() || line.isChar(','))
        break;

      if (line.isChar(':')) {
        line.skipChar();

        usingStr = CStrUtil::stripSpaces(usingStr);

        if (! usingStr.empty()) {
          if (isExpr)
            usingStr = "(" + usingStr + ")";

          usingStrs.push_back(usingStr);
        }

        usingStr = "";
        isExpr   = false;
      }
      else {
        if (line.isChar('(')) {
          ++rbrackets;

          if (usingStr == "") {
            line.skipChar();

            isExpr = true;
          }
          else
            usingStr += line.getChar();
        }
        else
          usingStr += line.getChar();
      }
    }
    else {
      if      (line.isChar('('))
        ++rbrackets;
      else if (line.isChar(')'))
        --rbrackets;

      if (rbrackets == 0 && isExpr)
        line.skipChar();
      else
        usingStr += line.getChar();
    }
  }

  if (! usingStr.empty()) {
    if (isExpr)
      usingStr = "(" + usingStr + ")";

    usingStrs.push_back(usingStr);
  }

  for (auto &str : usingStrs)
    str = CStrUtil::stripSpaces(str);

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
    usingCols.push_back(CGnuPlotUsingCol(str));
}

void
CGnuPlot::
parseIndex(CParseLine &line, int &indexStart, int &indexEnd, int &indexStep)
{
  std::string indexStr = readNonSpaceNonComma(line);

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

void
CGnuPlot::
parseEvery(CParseLine &line, int &everyStart, int &everyEnd, int &everyStep)
{
  std::string everyStr = readNonSpaceNonComma(line);

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
    everyEnd = std::numeric_limits<int>::max();
  }
  else if (inds.size() > 0) {
    (void) CStrUtil::toInteger(inds[0], &everyStep);
    everyStart = 0;
    everyEnd   = std::numeric_limits<int>::max();
  }
}

bool
CGnuPlot::
parseFont(CParseLine &line, CFontPtr &font)
{
  std::string str;

  if (! parseString(line, str))
    return false;

  CParseLine line1(str);

  std::string family = readNonSpaceNonComma(line1);

  if (family != "") {
    if (! font.isValid())
      font = CFontMgrInst->lookupFont(family, CFONT_STYLE_NORMAL, 12);
    else
      font = font->dup(family, font->getStyle(), font->getSize());
  }

  if (line1.skipSpaceAndChar(',')) {
    double size;

    if (! parseReal(line1, size))
      return false;

    if (! font.isValid())
      font = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, size);
    else
      font = font->dup(font->getFamily(), font->getStyle(), size);
  }

  return true;
}

bool
CGnuPlot::
parseBoolExpression(CParseLine &line, bool &res)
{
  line.skipSpace();

  if (! line.isChar('('))
    return false;

  std::string expr;

  if (! parseExpression(line, expr))
    return false;

  CExprValuePtr value;

  if (! evaluateExpression(expr, value))
    return false;

  long l;

  if (! value.isValid() || ! value->getIntegerValue(l))
    return false;

  res = (l != 0);

  return true;
}

bool
CGnuPlot::
parseExpression(CParseLine &line, std::string &expr)
{
  if (! line.isChar('('))
    return false;

  int rbrackets = 0;

  while (line.isValid()) {
    if (rbrackets == 0) {
      if (line.isChar('('))
        ++rbrackets;

      expr += line.getChar();
    }
    else {
      if      (line.isChar('('))
        ++rbrackets;
      else if (line.isChar(')'))
        --rbrackets;

      expr += line.getChar();

      if (rbrackets == 0)
        break;
    }
  }

  return true;
}

// do for <iteration-spec> {
//  ...
// }
void
CGnuPlot::
doCmd(const std::string &args)
{
  CParseLine line(args);

  line.skipSpace();

  if (! line.isString("for"))
    return;

  (void) readNonSpace(line);

  if (! line.skipSpaceAndChar('['))
    return;

  std::string var;

  if (! readIdentifier(line, var))
    return;

  std::string              arg1, arg2, arg3;
  std::vector<std::string> fields;

  if      (line.skipSpaceAndChar('=')) {
    arg1 = readNonSpaceNonChar(line, ":]");

    if (line.skipSpaceAndChar(':')) {
      arg2 = readNonSpaceNonChar(line, ":]");

      if (line.skipSpaceAndChar(':')) {
        arg3 = readNonSpaceNonChar(line, ":]");
      }
    }
  }
  else if (line.isString("in")) {
    std::string str;

    if (! parseString(line, str))
      return;

    CStrUtil::addWords(str, fields);
  }

  if (! line.skipSpaceAndChar(']'))
    return;

  if (! line.skipSpaceAndChar('{'))
    return;

  std::vector<std::string> lines;

  std::string line1;

  while (fileReadLine(line1)) {
    if (line1 == "}") break;

    lines.push_back(line1);
  }

  if (arg1 != "") {
    CExprValuePtr value1;

    if (! evaluateExpression(arg1, value1))
      value1 = CExprValuePtr();

    CExprValuePtr value2;

    if (! evaluateExpression(arg2, value2))
      value2 = CExprValuePtr();

    CExprValuePtr value3;

    if (arg3 != "") {
      if (! evaluateExpression(arg3, value3))
        value3 = CExprValuePtr();
    }

    long i1 = 0, i2 = 1, i3 = 1;

    if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
      return;

    if (value3.isValid() && ! value3->getIntegerValue(i3))
      return;

    while (i1 <= i2) {
      CExprInst->createIntegerVariable(var, i1);

      for (uint i = 0; i < lines.size(); ++i) {
        parseLine(lines[i]);
      }

      i1 += i3;
    }
  }
  else {
    for (const auto &f : fields) {
      CExprInst->createStringVariable(var, f);

      for (uint i = 0; i < lines.size(); ++i) {
        parseLine(lines[i]);
      }
    }
  }
}

// while ({condition}) {
//   <commands>
// }
void
CGnuPlot::
whileCmd(const std::string &args)
{
  CParseLine line(args);

  line.skipSpace();

  if (! line.isChar('('))
    return;

  std::string expr;

  if (! parseExpression(line, expr))
    return;

  if (! line.skipSpaceAndChar('{'))
    return;

  line.skipSpace();

  // read block
  Statements  lines;
  std::string eline;

  readBlockLines(lines, eline, 1);

  while (true) {
    CExprValuePtr value;

    if (! evaluateExpression(expr, value))
      break;

    long l;

    if (! value.isValid() || ! value->getIntegerValue(l))
      break;

    bool res = (l != 0);

    if (! res)
      break;

    for (const auto &line : lines)
      parseLine(line);
  }
}

void
CGnuPlot::
evaluateCmd(const std::string &args)
{
  CExprValuePtr value;

  if (! evaluateExpression(args, value))
    return;

  std::string str;

  if (! value.isValid() || ! value->getStringValue(str))
    return;

  parseLine(str);
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

  if (! parseString(line, msg))
    msg = "press return to continue";

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
  return (device_ ? device_->createWindow() : new CGnuPlotWindow(this));
}

CGnuPlotGroup *
CGnuPlot::
createGroup(CGnuPlotWindow *window)
{
  return (device_ ? device_->createGroup(window) : new CGnuPlotGroup(window));
}

CGnuPlotPlot *
CGnuPlot::
createPlot(CGnuPlotGroup *group)
{
  return (device_ ? device_->createPlot(group) : new CGnuPlotPlot(group));
}

CGnuPlotLineStyle *
CGnuPlot::
createLineStyle()
{
  return (device_ ? device_->createLineStyle() : new CGnuPlotLineStyle);
}

CGnuPlotAxis *
CGnuPlot::
createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir)
{
  return (device_ ? device_->createAxis(group, id, dir) : new CGnuPlotAxis(group, id, dir));
}

CGnuPlotKey *
CGnuPlot::
createKey(CGnuPlotGroup *group)
{
  return (device_ ? device_->createKey(group) : new CGnuPlotKey(group));
}

CGnuPlotColorBox *
CGnuPlot::
createColorBox(CGnuPlotGroup *group)
{
  return (device_ ? device_->createColorBox(group) : new CGnuPlotColorBox(group));
}

CGnuPlotPalette *
CGnuPlot::
createPalette(CGnuPlotGroup *group)
{
  return (device_ ? device_->createPalette(group) : new CGnuPlotPalette(group));
}

CGnuPlotTitle *
CGnuPlot::
createTitle(CGnuPlotGroup *group)
{
  return (device_ ? device_->createTitle(group) : new CGnuPlotTitle(group));
}


CGnuPlotRenderer *
CGnuPlot::
renderer()
{
  return (device_ ? device_->renderer() : 0);
}

void
CGnuPlot::
timeout()
{
  if (device_)
    device_->timeout();
}

void
CGnuPlot::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (device_)
    device_->stateChanged(window, state);
}

bool
CGnuPlot::
processAssignFunction(const std::string &lhs, const std::string &rhs)
{
  if (isDebug())
    std::cerr << lhs << '=' << rhs << std::endl;

  CParseLine line(lhs);

  std::string identifier;

  (void) readIdentifier(line, identifier);

  if (identifier == "")
    return false;

  // function definition
  if      (line.skipSpaceAndChar('(')) {
    std::vector<std::string> args;

    std::string arg;

    if (! readIdentifier(line, arg))
      return false;

    args.push_back(arg);

    while (line.skipSpaceAndChar(',')) {
      std::string arg1;

      if (! readIdentifier(line, arg1))
        return false;

      args.push_back(arg1);
    }

    if (! line.skipSpaceAndChar(')'))
      return false;

    CExprInst->addFunction(identifier, args, rhs);
  }
  // variable assignment
  else {
    CExprValueP value;

    if (! evaluateExpression(rhs, value))
      value = CExprValueP();

    if (! value.isValid())
      return false;

    CExprInst->createVariable(lhs, value);
  }

  return true;
}

CGnuPlotPlot *
CGnuPlot::
addFunction2D(CGnuPlotGroup *group, const std::vector<std::string> &functions, PlotStyle style)
{
  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    plot = createPlot(group);

    if (clearRect_.isValid()) {
      plot->setClearRect(clearRect_.getValue());

      clearRect_.setInvalid();
    }

    if (plot->keyTitle() == "")
      plot->setKeyTitle(functions[0]);

    plot->setStyle(style);

    plot->init();
  }

  //---

  int nx, ny;

  getSamples(nx, ny);

  if (! polar_ && ! parametric_) {
    double xmin, ymin, xmax, ymax;

    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);

    //---

    std::string varName1("x"), varName2("t");

    getDummyVars(varName1, varName2);

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

    double x  = xmin;
    double dx = (xmax - xmin)/nx;

    if (functions[0] == "NaN") {
      for (int i = 0; i <= nx; ++i, x += dx)
        plot->addPoint2D(x, CExprValueP());
    }
    else {
      CExprCTokenStack cstack = compileExpression(functions[0]);

      for (int i = 0; i <= nx; ++i, x += dx) {
        xvar->setRealValue(x);

        CExprValueP value;

        if (! CExprInst->executeCTokenStack(cstack, value)) {
          errorMsg("Failed to eval " + functions[0] + " for x=" + CStrUtil::toString(x));
          value = CExprValueP(); // TODO
        }

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
  }
  else if (polar_) {
    double tmin, tmax;

    getTRange(&tmin, &tmax);

    //---

    std::string varName("t");

    //getDummyVar("t", varName);

    CExprVariablePtr tvar = CExprInst->createRealVariable(varName, 0.0);

    double a  = tmin;
    double da = (tmax - tmin)/nx;

    if (functions[0] == "NaN") {
      for (int i = 0; i <= nx; ++i, a += da)
        plot->addPoint2D(a, CExprValueP());
    }
    else {
      CExprCTokenStack cstack = compileExpression(functions[0]);

      for (int i = 0; i <= nx; ++i, a += da) {
        double c = cos(a);
        double s = sin(a);

        tvar->setRealValue(a);

        CExprValueP value;

        if (! CExprInst->executeCTokenStack(cstack, value)) {
          errorMsg("Failed to eval " + functions[0] + " for t=" + CStrUtil::toString(a));
          value = CExprValueP();
        }

        double r = 0.0;

        if (value.isValid() && value->getRealValue(r)) {
          double x = r*c;
          double y = r*s;

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
  }
  else if (parametric_) {
    if (functions.size() < 2)
      return 0;

    //------

    double tmin, tmax;

    getTRange(&tmin, &tmax);

    //---

    std::string varName("t");

    //getDummyVar("t", varName);

    CExprVariablePtr tvar = CExprInst->createRealVariable(varName, 0.0);

    double t  = tmin;
    double dt = (tmax - tmin)/nx;

    bool f1 = (functions[0] != "NaN");
    bool f2 = (functions[1] != "NaN");

    CExprCTokenStack cstack1, cstack2;

    if (f1)
      cstack1 = compileExpression(functions[0]);

    if (f2)
      cstack2 = compileExpression(functions[1]);

    for (int i = 0; i <= nx; ++i, t += dt) {
      tvar->setRealValue(t);

      double x, y;

      if (f1) {
        CExprValueP value;

        if (! CExprInst->executeCTokenStack(cstack1, value)) {
          errorMsg("Failed to eval " + functions[0] + " for t=" + CStrUtil::toString(t));
          value = CExprValueP();
        }

        if (! value.isValid() || ! value->getRealValue(x))
          x = CMathGen::getNaN();
      }
      else
        x = CMathGen::getNaN();

      if (f2) {
        CExprValueP value;

        if (! CExprInst->executeCTokenStack(cstack2, value)) {
          errorMsg("Failed to eval " + functions[1] + " for t=" + CStrUtil::toString(t));
          value = CExprValueP();
        }

        if (! value.isValid() || ! value->getRealValue(y))
          y = CMathGen::getNaN();
      }
      else
        y = CMathGen::getNaN();

      if (plot) {
        plot->addPoint2D(x, y);
      }
      else {
        // TODO: inside/outside test
        std::cerr << x << " " << y << " i" << std::endl;
      }
    }
  }
  else
    assert(false);

  //---

  return plot;
}

CGnuPlotPlot *
CGnuPlot::
addFunction3D(CGnuPlotWindowP window, const std::string &function, PlotStyle style)
{
  CParseLine line(function);

  // check for <var> = <expr>
  std::string identifier;

  (void) readIdentifier(line, identifier);

  // variable assignment
  if (line.skipSpaceAndChar('=')) {
    CExprValueP value;

    if (! evaluateExpression(line.substr(), value))
      value = CExprValueP();

    if (value.isValid())
      CExprInst->createVariable(identifier, value);

    // TODO always need function
    return 0;
  }

  //---

  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    CGnuPlotGroup *group = createGroup(window.get());

    group->init();
    group->set3D(true);

    group->setTitleData(title_);

    plot = createPlot(group);

    group->addSubPlots({plot});

    plot->setStyle(style);

    plot->init();

    window->addGroup(group);
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

      CExprValueP value;

      if (! evaluateExpression(function, value))
        value = CExprValueP();

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
addFile2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
          const CGnuPlotUsingCols &usingCols)
{
  Plots plots;

  //---

  // get file lines
  if (filename != "-") {
    // open file
    CUnixFile file(filename);

    if (! file.open())
      return plots;

    CGnuPlotFile::Lines lines;

    std::string line;

    while (file.readLine(line))
      lines.push_back(line);

    dataFile_.setLines(lines);
  }

  //---

  dataFile_.processLines();

  // plot per set
  for (int setNum = 0; setNum < dataFile_.numSets(); ++setNum) {
    // create plot
    CGnuPlotPlot *plot = createPlot(group);

    if (clearRect_.isValid()) {
      plot->setClearRect(clearRect_.getValue());

      clearRect_.setInvalid();
    }

    if (plot->keyTitle() == "") {
      std::string title;

      if      (keyData_.columnHead() && usingCols.size() > 1 && usingCols[1].isInt &&
               usingCols[1].ival >= 1 && usingCols[1].ival <= int(keyData_.columns().size()))
        title = keyData_.columns()[usingCols[1].ival - 1];
      else if (keyData_.columnNum().isValid() && keyData_.columnNum().getValue() >= 1 &&
               keyData_.columnNum().getValue() <= int(keyData_.columns().size()))
        title = keyData_.columns()[keyData_.columnNum().getValue()];
      else
        title = "\"" + filename + "\"";

      plot->setKeyTitle(title);
    }

    plot->setStyle(style);

    plot->init();

    plots.push_back(plot);

    pointNum_ = 0;

    // add fields (discontinuity per subset)
    for (int subSetNum = 0; subSetNum < dataFile_.numSubSets(setNum); ++subSetNum) {
      int lineNum = 0;

      // use first line for columnheaders if option enabled
      if (subSetNum == 0 && keyData_.columnHead()) {
        const CGnuPlotFile::Fields &fields = dataFile_.fields(setNum, subSetNum, lineNum);

        keyData_.setColumns(fields);

        ++lineNum;
      }

      int lineNum1 = lineNum;

      for ( ; lineNum < dataFile_.numLines(setNum, subSetNum); ++lineNum, ++pointNum_) {
        // first point of each new set is a discontinuity
        bool discontinuity = (subSetNum > 0 && lineNum == lineNum1);

        //---

        // convert fields on line to values
        // (None: save fields as used by columnhead function)
        // (TODO: save current file setNum, subSetNum and lineNum instead)
        fields_ = dataFile_.fields(setNum, subSetNum, lineNum);

        std::vector<CExprValueP> values;

        if (isMatrix()) {
          double r;

          for (const auto &field : fields_) {
            CExprValueP value;

            if (CStrUtil::toReal(field, &r))
              value = CExprInst->createRealValue(r);
            else
              value = CExprInst->createStringValue(field);

            values.push_back(value);
          }
        }
        else {
          bool skip = false;
          bool bad  = false;

          // no columns specified so use all columns
          if (usingCols.empty()) {
            for (uint i = 0; i < fields_.size(); ++i) {
              CExprValueP value = getFieldValue(i, i + 1, setNum, pointNum_, skip);

              if (! value.isValid())
                bad = true;

              values.push_back(value);
            }
          }
          // extract specified columns
          else {
            for (uint i = 0; i < usingCols.size(); ++i) {
              bool ignore = false;

              CExprValueP value = decodeUsingCol(i, usingCols[i], setNum, pointNum_, skip, ignore);

              if (! ignore) {
                if (! value.isValid())
                  bad = true;

                values.push_back(value);
              }
            }

#if 0
            // no values then use all values (TODO: remove)
            if      (values.empty()) {
              // all values
              for (uint i = 0; i < fields_.size(); ++i) {
                CExprValueP value = getFieldValue(i, i + 1, setNum, pointNum_, skip);

                if (value.isValid())
                  values.push_back(value);
              }
            }
#endif

            // one value then auto add the point number
            if (values.size() == 1) {
              CExprValueP value1 = getFieldValue(0, 0, setNum, pointNum_, skip);

              if (value1.isValid()) {
                values.push_back(value1);

                std::swap(values[0], values[1]);
              }
            }
          }

          if      (skip)
            discontinuity = true;
          else if (bad)
            continue;
        }

        //---

        // add values
        plot->addPoint2D(values, discontinuity);
      }
    }
  }

  return plots;
}

CGnuPlotPlot *
CGnuPlot::
addImage2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
           const CGnuPlotUsingCols &usingCols)
{
  // open file
  CUnixFile file(filename);
  if (! file.open()) return 0;

  std::vector<uchar> data;

  uchar c;

  while (file.readChar(c)) {
    data.push_back(c);
  }

  CGnuPlotPlot *plot = createPlot(group);

  plot->setStyle(style);

  plot->init();

  plot->setImageData(data);

  CGnuPlotImageStyle istyle = imageStyle();

  istyle.usingCols = usingCols;

  plot->setImageStyle(istyle);

  return plot;
}

CGnuPlotPlot *
CGnuPlot::
addFile3D(CGnuPlotWindowP window, const std::string &filename)
{
  // open file
  CUnixFile file(filename);
  if (! file.open()) return 0;

  //---

  CGnuPlotPlot *plot = 0;

  typedef std::map<double,double> XZMap;
  typedef std::map<double,XZMap>  YXZMap;

  YXZMap values;
  bool   firstLine = true;

  // process each line in file
  std::string line;

  while (file.readLine(line)) {
    auto hp = line.find(dataFile_.commentChars());

    if (hp != std::string::npos) {
      line = CStrUtil::stripSpaces(line.substr(0, hp));

      if (line.empty())
        continue;
    }

    //---

    // get values from fields on line
    std::vector<std::string> fields;

    fields.clear();

    if (! dataFile_.getSeparator())
      CStrUtil::addWords(line, fields);
    else {
      char c = dataFile_.getSeparator();

      CStrUtil::addFields(line, fields, std::string(&c, 1), /*skipEmpty*/true);
    }

    //---

    if (firstLine) {
      firstLine = false;

      if (keyData_.columnHead()) {
        keyData_.setColumns(fields);
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

  CGnuPlotGroup *group = createGroup(window.get());

  group->init();
  group->set3D(true);

  group->setTitleData(title_);

  plot = createPlot(group);

  group->addSubPlots({plot});

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

  window->addGroup(group);

  //---

  return plot;
}

CExprValueP
CGnuPlot::
decodeUsingCol(int i, const CGnuPlotUsingCol &col, int setNum, int pointNum,
               bool &skip, bool &ignore)
{
  CExprValueP value;

  skip   = false;
  ignore = false;

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

      bool skip1 = false;

      CExprValueP value1 = getFieldValue(i, icol, setNum, pointNum, skip1);

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

        CExprValueP value;

        if (! evaluateExpression(name1, value, true))
          value = CExprValueP();

        long icol;

        if (value.isValid() && value->getIntegerValue(icol)) {
          CExprValueP value1 = getFieldValue(i, icol, setNum, pointNum, skip);

          std::string str1;

          if (value1.isValid() && value1->getStringValue(str1)) {
            if (name == "xtic" || name == "xticlabels")
              axesData_.xaxis[1].setTicLabel(pointNum, str1);
            else
              axesData_.yaxis[1].setTicLabel(pointNum, str1);
          }
        }

        ignore = true;
        expr   = "";
      }
    }

    //----

    if (expr != "") {
      if (isDebug())
        std::cerr << "expr " << expr << std::endl;

      if (! evaluateExpression(expr, value, true))
        value = CExprValueP();
    }
  }

  return value;
}

bool
CGnuPlot::
evaluateExpression(const std::string &expr, CExprValueP &value, bool quiet) const
{
  if (! CExprInst->evaluateExpression(expr, value))
    value = CExprValueP();

  if (! value.isValid() && ! quiet)
    std::cerr << "Eval failed: " << expr << std::endl;

  return true;
}

CExprCTokenStack
CGnuPlot::
compileExpression(const std::string &expr) const
{
  CExprPTokenStack pstack = CExprInst->parseLine(expr);
  CExprITokenPtr   itoken = CExprInst->interpPTokenStack(pstack);

  CExprCTokenStack cstack;

  if (itoken.isValid())
    cstack = CExprInst->compileIToken(itoken);
  else
    std::cerr << "Eval failed: " << expr << std::endl;

  return cstack;
}

CExprValueP
CGnuPlot::
getFieldValue(int i, int ival, int setNum, int pointNum, bool &skip)
{
  skip = false;

  CExprValueP value;

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
      return CExprValueP();
    }

    double val;

    if      ((i == 0 && axesData_.xaxis[1].isTime()) ||
             (i == 1 && axesData_.yaxis[1].isTime())) {
      struct tm tm1; memset(&tm1, 0, sizeof(tm));

      strptime(field.c_str(), timeFmt_.c_str(), &tm1);

      value = CExprInst->createRealValue(mktime(&tm1));
    }
    else if (fieldToReal(field, val))
      value = CExprInst->createRealValue(val);
    else
      value = CExprInst->createStringValue(field);
  }

  return value;
}

bool
CGnuPlot::
fieldToReal(const std::string &str, double &r) const
{
  const char *c_str = str.c_str();

  int i = 0;

  while (c_str[i] != 0 && ::isspace(c_str[i]))
    ++i;

  if (c_str[i] == '\0')
    return false;

  const char *p1 = &c_str[i];
  const char *p2 = p1;

  errno = 0;

  r = strtod(&c_str[i], (char **) &p1);

  if (errno == ERANGE)
    return false;

  if (p1 == p2)
    return false;

  while (*p1 != 0 && ::isspace(*p1))
    ++p1;

  //if (*p1 != '\0') return false;

  return true;
}

CGnuPlotLineStyleP
CGnuPlot::
lineStyle()
{
  if (! lineStyle_) {
    CGnuPlotLineStyleP lineStyle = CGnuPlotLineStyleP(createLineStyle());

    lineStyles_[-1] = lineStyle;

    lineStyle_ = lineStyle;
  }

  return lineStyle_;
}

CGnuPlotLineStyleP
CGnuPlot::
getLineStyleInd(int ind)
{
  CGnuPlotLineStyleP lineStyle = this->lineStyle(ind);

  if (! lineStyle) {
    lineStyle = CGnuPlotLineStyleP(createLineStyle());

    lineStyle->setInd(ind);

    if (ind > 0)
      lineStyle->setColor(CGnuPlotStyleInst->indexColor(ind));

    lineStyles_[ind] = lineStyle;
  }

  return lineStyle;
}

void
CGnuPlot::
setLineStyleInd(int ind)
{
  CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

  if (! lineStyle_ || lineStyle->ind() != lineStyle_->ind())
    lineStyle_ = lineStyle;
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
resetLineStyle()
{
  styleIncrement_.styleInd = 0;
}

void
CGnuPlot::
incLineStyle()
{
  // TODO: StyleIncrementType::USER, StyleIncrementType::DEFAULT
  ++styleIncrement_.styleInd;

#if 0
  CGnuPlotLineStyleP ls = lineStyle();

  ls->setColor(CGnuPlotStyleInst->indexColor(styleIncrement_.colorInd));

  if (fillStyle().style() == CGnuPlotTypes::FillType::PATTERN)
    fillStyle_.setPattern(static_cast<CGnuPlotTypes::FillPattern>(
      static_cast<int>(fillStyle().pattern()) + 1));
#endif

  setLineStyleInd(styleIncrement_.styleInd);
}

void
CGnuPlot::
drawWindows()
{
  for (const auto &window : windows_)
    window->draw();
}

CGnuPlot::AngleType
CGnuPlot::
getAngleType() const
{
  return static_cast<CGnuPlotPrefValue<AngleType> *>
   (const_cast<CGnuPlot *>(this)->varPrefs_[VariableName::ANGLES])->get();
}

void
CGnuPlot::
setAngleType(AngleType type)
{
  static_cast<CGnuPlotPrefValue<AngleType> *>(varPrefs_[VariableName::ANGLES])->set(type);

  CExprInst->setDegrees(type == AngleType::DEGREES);
}

void
CGnuPlot::
setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2)
{
  if      (parametric_) {
    if (dummyVar1 != "") dummyVars_["t"] = dummyVar1;
    if (dummyVar2 != "") dummyVars_["y"] = dummyVar2;
  }
  else if (polar_) {
    if (dummyVar1 != "") dummyVars_["t"] = dummyVar1;
    if (dummyVar2 != "") dummyVars_["y"] = dummyVar2;
  }
  else {
    if (dummyVar1 != "") dummyVars_["x"] = dummyVar1;
    if (dummyVar2 != "") dummyVars_["y"] = dummyVar2;
  }
}

void
CGnuPlot::
getDummyVars(std::string &dummyVar1, std::string &dummyVar2) const
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  if      (parametric_) {
    dummyVar1 = th->dummyVars_["t"]; if (dummyVar1 == "") dummyVar1 = "t";
    dummyVar2 = th->dummyVars_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
  }
  else if (polar_) {
    dummyVar1 = th->dummyVars_["t"]; if (dummyVar1 == "") dummyVar1 = "t";
    dummyVar2 = th->dummyVars_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
  }
  else {
    dummyVar1 = th->dummyVars_["x"]; if (dummyVar1 == "") dummyVar1 = "x";
    dummyVar2 = th->dummyVars_["y"]; if (dummyVar2 == "") dummyVar2 = "y";
  }
}

// set xtics {axis | border} {{no}mirror}
//           {in | out} {scale {default | <major> {,<minor>}}}
//           {{no}rotate {by <ang>}} {offset <offset> | nooffset}
//           {left | right | center | autojustify}
//           {add}
//           {autofreq |
//            <incr> | <start>, <incr> {,<end>} |
//            ({"<label>"} <pos> {<level>} {,{"<label>"}...)}}
//           {format "formatstring"}
//           {font "name{,<size>}"}
//           {numeric | timedate | geographic}
//           {rangelimited}
//           {textcolor <colorspec>}
bool
CGnuPlot::
parseAxesTics(CParseLine &line, CGnuPlotAxisData &axis)
{
  axis.setShowTics(true);

  std::string arg;

  if (! readIdentifier(line, arg)) {
    line.skipSpace();

    arg = (line.isValid() ? line.getChars(1) : "");
  }

  double r;

  while (arg != "") {
    if      (arg == "axis"  ) { }
    else if (arg == "border") { }
    else if (arg == "mirror" || arg == "nomirror") {
      axis.setMirror(arg == "mirror");
    }
    else if (arg == "in" || arg == "out") {
      //axis.setTicsIn(arg == "in");
    }
    else if (arg == "scale") {
      std::string arg1 = readNonSpaceNonComma(line);

      if (line.skipSpaceAndChar(','))
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "rotate") {
      std::string arg1 = readNonSpaceNonComma(line);

      if (arg1 == "by")
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "norotate") {
    }
    else if (arg == "offset") {
      std::string arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "nooffset") {
    }
    else if (arg == "left" || arg == "right" ||  arg == "center" || arg == "autojustify") {
      //setTicJustify(arg != "left" ?
      //  (arg != "right" ? (arg != "center" ? AUTOJUSTIFY : CENTER) : RIGHT) : LEFT);
    }
    else if (arg == "add") {
    }
    else if (arg == "autofreq") {
    }
    else if (arg == "format") {
      std::string formatStr;

      if (parseString(line, formatStr, "Invalid format")) {
        axis.setFormat(formatStr);
      }
    }
    else if (arg == "font") {
      CFontPtr font;

      if (parseFont(line, font))
        axis.setFont(font);
    }
    else if (arg == "rangelimited") {
    }
    else if (arg == "textcolor" || arg == "tc") {
      //std::string arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "linetype" || arg == "lt") {
      int lt;

      if (parseInteger(line, lt)) {
        //axis.setLineType(lt);
      }
    }
    // ({"<label>"} <pos> {<level>} {,{"<label>"}...)}
    else if (arg == "(") {
      bool first = true;

      while (line.isValid()) {
        if (! first) {
          if (! line.skipSpaceAndChar(','))
            break;
        }

        line.skipSpace();

        std::string label;

        if (line.isChar('"')) {
          if (! parseString(line, label))
            label = "";
        }

        double pos = 0.0;

        if (! parseReal(line, pos))
          pos = 0.0;

        double level = 0.0;

        if (! parseReal(line, level))
          level = 0.0;

        first = false;
      }

      line.skipSpaceAndChar(')');
    }
    // <incr> | <start>, <incr> {,<end>} |
    else if (CStrUtil::isReal(arg) && CStrUtil::toReal(arg, &r)) {
      double start = 0.0, incr = r, end = 0.0;

      if (line.skipSpaceAndChar(',')) {
        if (parseReal(line, r)) {
          start = incr;
          incr  = r;

          if (line.skipSpaceAndChar(',')) {
            if (parseReal(line, r))
              end = r;
          }
        }
      }

      std::cerr << "start=" << start << ", incr=" << incr << ", end=" << end << std::endl;
    }
    else {
      errorMsg("Invalid arg '" + arg + "'");
      break;
    }

    if (! readIdentifier(line, arg)) {
      line.skipSpace();

      arg = (line.isValid() ? line.getChars(1) : "");
    }
  }

  return true;
}

bool
CGnuPlot::
parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c)
{
  int pos = line.pos();

  std::string arg = readNonSpaceNonComma(line);

  if      (arg == "rgbcolor" || arg == "rgb") {
    line.skipSpace();

    if (line.isString("var") || line.isString("variable")) {
      arg = readNonSpaceNonComma(line);

      c.setRGBVariable();
    }
    else {
      std::string colorValueStr;

      if (parseString(line, colorValueStr, "Invalid color string")) {
        double r, g, b;

        if (! CRGBName::lookup(colorValueStr, &r, &g, &b)) {
          errorMsg("Invalid color string '" + colorValueStr + "'");
          return false;
        }

        c.setRGB(CRGBA(r, g, b));
      }
    }
  }
  else if (arg == "palette" || arg == "pal") {
    arg = readNonSpaceNonComma(line);

    if      (arg == "frac") {
      double r;

      if (! parseReal(line, r))
        return false;

      c.setPaletteFrac(r);
    }
    else if (arg == "cb") {
      double r;

      if (! parseReal(line, r))
        return false;

      c.setPaletteCB(r);
    }
    else if (arg == "z") {
      c.setPaletteZ();
    }
  }
  else if (arg == "variable" || arg == "var")
    c.setVariable();
  else if (arg == "linetype" || arg == "lt") {
    int lt;

    if (! parseInteger(line, lt)) {
      line.setPos(pos);
      return false;
    }

    c.setRGB(CGnuPlotStyleInst->indexColor(lt));
  }
  else if (arg == "bgnd") {
    c.setRGB(backgroundColor());
  }
  else if (arg == "black") {
    c.setRGB(CRGBA(0,0,0));
  }
  else {
    int ind;

    if (! getIntegerVariable(arg, ind)) {
      line.setPos(pos);
      return false;
    }

    c.setIndex(ind);
  }

  return true;
}

bool
CGnuPlot::
parseColor(CParseLine &line, CRGBA &c)
{
  // <color> :=  { <r> <g> <b> | '<color-name>' | '#rrggbb' }
  line.skipSpace();

  if (line.isChar('"') || line.isChar('\'')) {
    std::string name;

    if (! parseString(line, name, "Invalid color string"))
      return false;

    c = CRGBName::toRGBA(name);
  }
  else {
    double r = 0.0, g = 0.0, b = 0.0;

    if (! parseReal(line, r) || ! parseReal(line, g) || ! parseReal(line, b))
      return false;

    c = CRGBA(r, g, b);
  }

  return true;
}

bool
CGnuPlot::
parseInteger(CParseLine &line, int &i)
{
  int pos = line.pos();

  line.skipSpace();

  if (line.isAlpha()) {
    std::string id;

    if (! readIdentifier(line, id)) {
      line.setPos(pos);
      return false;
    }

    if (! getIntegerVariable(id, i)) {
      line.setPos(pos);
      return false;
    }
  }
  else {
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
  }

  return true;
}

bool
CGnuPlot::
getIntegerVariable(const std::string &name, int &value)
{
  CExprVariablePtr var = CExprInst->getVariable(name);

  if (! var.isValid())
    return false;

  CExprValuePtr evalue = var->value();

  long l = 0;

  if (! evalue->getIntegerValue(l))
    return false;

  value = l;

  return true;
}

bool
CGnuPlot::
getRealVariable(const std::string &name, double &value)
{
  CExprVariablePtr var = CExprInst->getVariable(name);

  if (! var.isValid())
    return false;

  CExprValuePtr evalue = var->value();

  double r = 0;

  if (! evalue->getRealValue(r))
    return false;

  value = r;

  return true;
}

bool
CGnuPlot::
parseReal(CParseLine &line, double &r)
{
  line.skipSpace();

  int pos = line.pos();

  int sign = 1;

  if (line.isOneOf("+-"))
    sign = (line.getChar() == '-' ? -1 : 1);

  if (line.isAlpha()) {
    std::string id;

    if (! readIdentifier(line, id)) {
      line.setPos(pos);
      return false;
    }

    if (line.isChar('(')) {
      std::string str;

      if (! parseBracketedString(line, str)) {
        line.setPos(pos);
        return false;
      }

      std::string expr = id + str;

      CExprValueP value;

      if (! evaluateExpression(expr, value))
        value = CExprValueP();

      if (! value.isValid() || ! value->getRealValue(r)) {
        line.setPos(pos);
        return false;
      }
    }
    else {
      if (! getRealVariable(id, r)) {
        line.setPos(pos);
        return false;
      }
    }
  }
  else {
    std::string str;

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

    if (str == "" || ! CStrUtil::toReal(str, &r)) {
      line.setPos(pos);
      return false;
    }
  }

  r = sign*r;

  return true;
}

bool
CGnuPlot::
parseString(CParseLine &line, std::string &str, const std::string &msg)
{
  str = "";

  if      (line.skipSpaceAndChar('"')) {
    while (line.isValid() && ! line.isChar('"')) {
      char c = line.getChar();

      if (c == '\\') {
        if (line.isValid()) {
          c = line.getChar();

          switch (c) {
            case 't'  : str += '\t'; break;
            case 'n'  : str += '\n'; break;
            case '\"' : str += '\"'; break;
            case '\'' : str += '\''; break;
            default   : str += '?' ; break;
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
  else if (line.skipSpaceAndChar('\'')) {
    while (line.isValid() && ! line.isChar('\''))
      str += line.getChar();

    if (line.isChar('\''))
      line.skipChar();
  }
  else {
    int pos = line.pos();

    std::string id;

    if (! readIdentifier(line, id) || ! getStringVariable(id, str)) {
      line.setPos(pos);

      if (msg != "")
        errorMsg(msg);

      return false;
    }
  }

  return true;
}

bool
CGnuPlot::
getStringVariable(const std::string &name, std::string &value)
{
  CExprVariablePtr var = CExprInst->getVariable(name);

  if (! var.isValid())
    return false;

  CExprValuePtr evalue = var->value();

  if (! evalue->getStringValue(value))
    return false;

  return true;
}

// <system> <x>, <system> <y>
bool
CGnuPlot::
parsePosition(CParseLine &line, CGnuPlotPosition &pos)
{
  CGnuPlotTypes::CoordSys system;

  if (parseOptionValue(this, line, system, "coord system"))
    pos.setSystemX(system);
  else
    pos.setSystemX(CGnuPlotTypes::CoordSys::FIRST);

  double x;

  if (! parseReal(line, x))
    return false;

  if (! line.skipSpaceAndChar(','))
    return false;

  if (parseOptionValue(this, line, system, "coord system"))
    pos.setSystemY(system);
  else
    pos.setSystemY(pos.systemX());

  double y;

  if (! parseReal(line, y))
    return false;

  pos.setPoint(CPoint2D(x, y));

  return true;
}

// <system> <r>
bool
CGnuPlot::
parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v)
{
  CGnuPlotTypes::CoordSys system;

  if (parseOptionValue(this, line, system, "coord system"))
    v.setSystem(system);
  else
    v.setSystem(CGnuPlotTypes::CoordSys::FIRST);

  double r;

  if (! parseReal(line, r))
    return false;

  v.setValue(r);

  return true;
}

bool
CGnuPlot::
parsePoint(CParseLine &line, CPoint2D &p)
{
  int pos = line.pos();

  double x = 0.0;

  if (parseReal(line, x)) {
    if (line.skipSpaceAndChar(',')) {
      double y = 0.0;

      if (parseReal(line, y)) {
        p = CPoint2D(x, y);
        return true;
      }
    }
  }

  line.setPos(pos);

  return false;
}

bool
CGnuPlot::
parseSize(CParseLine &line, CGnuPlotSize &size)
{
  CGnuPlotTypes::CoordSys system;

  if (parseOptionValue(this, line, system, "coord system"))
    size.setSystemX(system);
  else
    size.setSystemX(CGnuPlotTypes::CoordSys::FIRST);

  double x;

  if (! parseReal(line, x))
    return false;

  if (! line.skipSpaceAndChar(','))
    return false;

  if (parseOptionValue(this, line, system, "coord system"))
    size.setSystemY(system);
  else
    size.setSystemY(size.systemX());

  double y;

  if (! parseReal(line, y))
    return false;

  size.setSize(CSize2D(x, y));

  return true;
}

bool
CGnuPlot::
parseSize(CParseLine &line, CSize2D &size)
{
  // TODO: support coord types
  int pos = line.pos();

  double w = 1.0;

  if (parseReal(line, w)) {
    if (line.skipSpaceAndChar(',')) {
      double h = 1.0;

      if (parseReal(line, h)) {
        size = CSize2D(w, h);
        return true;
      }
    }
  }

  line.setPos(pos);

  return true;
}

bool
CGnuPlot::
parseFunction(CParseLine &line, FunctionData &functionData)
{
  functionData.isAssign = false;

  line.skipSpace();

  if (! line.isValid())
    return false;

  std::string str;

  int brackets = 0;

  while (line.isValid()) {
    if      (line.isChar('('))
      ++brackets;
    else if (line.isChar(')'))
      --brackets;
    else if (line.isSpace() || line.isChar(',') || line.isChar('=')) {
      if (brackets == 0)
        break;
    }

    str += line.getChar();
  }

  functionData.function = str;

  if (line.skipSpaceAndChar('=')) {
    functionData.isAssign = true;

    line.skipSpace();

    std::string str;

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

    functionData.assign = str;
  }

  return true;
}

bool
CGnuPlot::
parseBracketedString(CParseLine &line, std::string &str)
{
  if (! line.isChar('('))
    return false;

  str += line.getChar();

  int brackets = 1;

  while (line.isValid()) {
    if      (line.isChar('('))
      ++brackets;
    else if (line.isChar(')'))
      --brackets;

    str += line.getChar();

    if (brackets == 0)
      break;
  }

  return true;
}

bool
CGnuPlot::
readIdentifier(CParseLine &line, std::string &id)
{
  line.skipSpace();

  if (! line.isValid())
    return false;

  char c = line.lookChar();

  if (! isalpha(c))
    return false;

  id = line.getChar();

  while (line.isValid()) {
    char c = line.lookChar();

    if (! isalnum(c) && c != '_')
      break;

    id += line.getChar();
  }

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
  return readNonSpaceNonChar(line, ',');
}

std::string
CGnuPlot::
readNonSpaceNonChar(CParseLine &line, char c)
{
  line.skipSpace();

  std::string str;

  while (line.isValid() && ! line.isSpace() && ! line.isChar(c))
    str += line.getChar();

  return str;
}

std::string
CGnuPlot::
readNonSpaceNonChar(CParseLine &line, const std::string &c)
{
  line.skipSpace();

  std::string str;

  while (line.isValid() && ! line.isSpace() && ! line.isOneOf(c))
    str += line.getChar();

  return str;
}

std::string
CGnuPlot::
readName(CParseLine &line)
{
  line.skipSpace();

  std::string str;

  while (line.isValid() && ! line.isAlpha())
    str += line.getChar();

  return str;
}

bool
CGnuPlot::
fileReadLine(std::string &line)
{
  if (! fileData_.bufferLines.empty()) {
    line = fileData_.bufferLines.front();

    fileData_.bufferLines.pop_front();
//std::cerr << "buffer: " << line << std::endl;
  }
  else {
    if (! fileData_.file || ! fileData_.file->readLine(line))
      return false;

    ++fileData_.lineNum;
//std::cerr << fileData_.lineNum << ":" << line << std::endl;
  }

  return true;
}

void
CGnuPlot::
errorMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;
}
