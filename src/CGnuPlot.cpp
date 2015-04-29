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

#include <CGnuPlotStyleAdjacency.h>
#include <CGnuPlotStyleBoxErrorBars.h>
#include <CGnuPlotStyleBoxes.h>
#include <CGnuPlotStyleBoxPlot.h>
#include <CGnuPlotStyleBoxXYErrorBars.h>
#include <CGnuPlotStyleBubblePlot.h>
#include <CGnuPlotStyleCandlesticks.h>
#include <CGnuPlotStyleChordDiagram.h>
#include <CGnuPlotStyleCircles.h>
#include <CGnuPlotStyleDelaunay.h>
#include <CGnuPlotStyleDots.h>
#include <CGnuPlotStyleEllipses.h>
#include <CGnuPlotStyleFilledCurves.h>
#include <CGnuPlotStyleFinanceBars.h>
#include <CGnuPlotStyleImage.h>
#include <CGnuPlotStyleImpulses.h>
#include <CGnuPlotStyleLabels.h>
#include <CGnuPlotStyleLines.h>
#include <CGnuPlotStyleLinesPoints.h>
#include <CGnuPlotStyleParallelAxes.h>
#include <CGnuPlotStylePieChart.h>
#include <CGnuPlotStylePolygons.h>
#include <CGnuPlotStylePoints.h>
#include <CGnuPlotStyleRadar.h>
#include <CGnuPlotStyleSteps.h>
#include <CGnuPlotStyleSunburst.h>
#include <CGnuPlotStyleTestPalette.h>
#include <CGnuPlotStyleTestTerminal.h>
#include <CGnuPlotStyleTreeMap.h>
#include <CGnuPlotStyleVectors.h>
#include <CGnuPlotStyleXErrorBars.h>
#include <CGnuPlotStyleXErrorLines.h>
#include <CGnuPlotStyleXYErrorBars.h>
#include <CGnuPlotStyleXYErrorLines.h>
#include <CGnuPlotStyleYErrorBars.h>
#include <CGnuPlotStyleYErrorLines.h>

#include <CImageLib.h>
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
  typedef std::vector<CExprValuePtr> Values;

 public:
  CGnuPlotFn(CGnuPlot *plot) : plot_(plot) { }

 protected:
  CGnuPlot *plot_;
};

class CGnuPlotAssertFn : public CGnuPlotFn {
 public:
  CGnuPlotAssertFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const Values &values) {
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

  CExprValueP operator()(const Values &values) {
    long col = 0;

    if      (values[0]->isIntegerValue()) {
      (void) values[0]->getIntegerValue(col);
    }
    else if (values[0]->isStringValue()) {
      std::string str;

      (void) values[0]->getStringValue(str);

      col = plot_->getColumnIndex(str);
    }
    else
      return CExprInst->createRealValue(0.0);

    bool skip;

    return plot_->getFieldValue(plot_->pointNum(), col, 0, plot_->pointNum(), skip);
  }
};

class CGnuPlotExistsFn : public CGnuPlotFn {
 public:
  CGnuPlotExistsFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const Values &values) {
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

  CExprValueP operator()(const Values &values) {
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

  CExprValueP operator()(const Values &values) {
    assert(values.size() == 1);

    std::string str;

    long col = 0;

    if (values[0]->getIntegerValue(col))
      str = plot_->getField(col);

    return CExprInst->createIntegerValue(str == "" ? 0 : 1);
  }
};

class CGnuPlotValueFn : public CGnuPlotFn {
 public:
  CGnuPlotValueFn(CGnuPlot *plot) : CGnuPlotFn(plot) { }

  CExprValueP operator()(const Values &values) {
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
      if (msg != "")
        plot->errorMsg("Missing " + msg + " '" + line.substr() + "'");
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
    new CGnuPlotPrefValue<AngleType>("Angle Type", "angles", AngleType::RADIANS);

  CExprInst->createRealVariable("pi", M_PI);

  CExprInst->addFunction("assert", "i", new CGnuPlotAssertFn(this));

  CExprInst->addFunction("column"      , "is", new CGnuPlotColumnFn(this));
  CExprInst->addFunction("exists"      , "s" , new CGnuPlotExistsFn(this));
  CExprInst->addFunction("stringcolumn", "i" , new CGnuPlotStringColumnFn(this));
  CExprInst->addFunction("valid"       , "i" , new CGnuPlotStringValidFn(this));
  CExprInst->addFunction("value"       , "s" , new CGnuPlotValueFn(this));
  // timecolumn

  CExprInst->createIntegerVariable("ARGC", 0);
  CExprInst->createStringVariable ("ARG0", "");

  CExprInst->createStringVariable("GNUTERM", "qt");

  CExprInst->createStringVariable("GPVAL_ENCODING" , "default");
  CExprInst->createStringVariable("GPVAL_ERRMSG"   , "");
  CExprInst->createStringVariable("GPVAL_OUTPUT"   , "");
  CExprInst->createStringVariable("GPVAL_PWD"      , CDir::getCurrent());
  CExprInst->createStringVariable("GPVAL_TERM"     , "qt");
  CExprInst->createStringVariable("GPVAL_TERMINALS", "qt svg");
  CExprInst->createRealVariable  ("GPVAL_pi"       , M_PI);
  CExprInst->createRealVariable  ("GPVAL_NaN"      , CMathGen::getNaN());

  CExprInst->createStringVariable("GPVAL_COMPILE_OPTIONS", "+GD_PNG");

  svgDevice_ = new CGnuPlotSVGDevice();

  addDevice("svg", svgDevice_);

  for (int i = 0; i < 8; ++i)
    axesData_.paxis[i].unset();

  addPlotStyles();
}

CGnuPlot::
~CGnuPlot()
{
}

void
CGnuPlot::
addPlotStyles()
{
  addPlotStyle(PlotStyle::ADJACENCY     , new CGnuPlotStyleAdjacency     );
  addPlotStyle(PlotStyle::BOXERRORBARS  , new CGnuPlotStyleBoxErrorBars  );
  addPlotStyle(PlotStyle::BOXES         , new CGnuPlotStyleBoxes         );
  addPlotStyle(PlotStyle::BOXPLOT       , new CGnuPlotStyleBoxPlot       );
  addPlotStyle(PlotStyle::BOXXYERRORBARS, new CGnuPlotStyleBoxXYErrorBars);
  addPlotStyle(PlotStyle::BUBBLEPLOT    , new CGnuPlotStyleBubblePlot    );
  addPlotStyle(PlotStyle::CANDLESTICKS  , new CGnuPlotStyleCandlesticks  );
  addPlotStyle(PlotStyle::CHORDDIAGRAM  , new CGnuPlotStyleChordDiagram  );
  addPlotStyle(PlotStyle::CIRCLES       , new CGnuPlotStyleCircles       );
  addPlotStyle(PlotStyle::DELAUNAY      , new CGnuPlotStyleDelaunay      );
  addPlotStyle(PlotStyle::DOTS          , new CGnuPlotStyleDots          );
  addPlotStyle(PlotStyle::ELLIPSES      , new CGnuPlotStyleEllipses      );
  addPlotStyle(PlotStyle::FILLEDCURVES  , new CGnuPlotStyleFilledCurves  );
  addPlotStyle(PlotStyle::FILLSTEPS     , new CGnuPlotStyleFillSteps     );
  addPlotStyle(PlotStyle::FINANCEBARS   , new CGnuPlotStyleFinanceBars   );
  addPlotStyle(PlotStyle::FSTEPS        , new CGnuPlotStyleFSteps        );
  addPlotStyle(PlotStyle::HISTEPS       , new CGnuPlotStyleHiSteps       );
  addPlotStyle(PlotStyle::IMAGE         , new CGnuPlotStyleImage         );
  addPlotStyle(PlotStyle::IMPULSES      , new CGnuPlotStyleImpulses      );
  addPlotStyle(PlotStyle::LABELS        , new CGnuPlotStyleLabels        );
  addPlotStyle(PlotStyle::LINES         , new CGnuPlotStyleLines         );
  addPlotStyle(PlotStyle::LINES_POINTS  , new CGnuPlotStyleLinesPoints   );
  addPlotStyle(PlotStyle::PARALLELAXES  , new CGnuPlotStyleParallelAxes  );
  addPlotStyle(PlotStyle::PIECHART      , new CGnuPlotStylePieChart      );
  addPlotStyle(PlotStyle::POINTS        , new CGnuPlotStylePoints        );
  addPlotStyle(PlotStyle::POLYGONS      , new CGnuPlotStylePolygons      );
  addPlotStyle(PlotStyle::RADAR         , new CGnuPlotStyleRadar         );
  addPlotStyle(PlotStyle::RGBIMAGE      , new CGnuPlotStyleRGBImage      );
  addPlotStyle(PlotStyle::RGBALPHA      , new CGnuPlotStyleRGBAlpha      );
  addPlotStyle(PlotStyle::STEPS         , new CGnuPlotStyleSteps         );
  addPlotStyle(PlotStyle::SUNBURST      , new CGnuPlotStyleSunburst      );
  addPlotStyle(PlotStyle::TEST_PALETTE  , new CGnuPlotStyleTestPalette   );
  addPlotStyle(PlotStyle::TEST_TERMINAL , new CGnuPlotStyleTestTerminal  );
  addPlotStyle(PlotStyle::TREEMAP       , new CGnuPlotStyleTreeMap       );
  addPlotStyle(PlotStyle::VECTORS       , new CGnuPlotStyleVectors       );
  addPlotStyle(PlotStyle::XERRORBARS    , new CGnuPlotStyleXErrorBars    );
  addPlotStyle(PlotStyle::XYERRORBARS   , new CGnuPlotStyleXYErrorBars   );
  addPlotStyle(PlotStyle::YERRORBARS    , new CGnuPlotStyleYErrorBars    );
  addPlotStyle(PlotStyle::XERRORLINES   , new CGnuPlotStyleXErrorLines   );
  addPlotStyle(PlotStyle::XYERRORLINES  , new CGnuPlotStyleXYErrorLines  );
  addPlotStyle(PlotStyle::YERRORLINES   , new CGnuPlotStyleYErrorLines   );
}

void
CGnuPlot::
addPlotStyle(PlotStyle plotStyle, CGnuPlotStyleBase *style)
{
  assert(plotStyle == style->style());

  plotStyles_[plotStyle] = style;
}

CGnuPlotStyleBase *
CGnuPlot::
getPlotStyle(PlotStyle plotStyle) const
{
  auto p = plotStyles_.find(plotStyle);

  return (p == plotStyles_.end() ? 0 : (*p).second);
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
  auto name1 = CStrUtil::toLower(name);

  device->setApp(this);

  devices_[name1] = device;
}

bool
CGnuPlot::
setDevice(const std::string &name)
{
  auto name1 = CStrUtil::toLower(name);

  if (device_ && CStrUtil::toLower(device_->name()) == name1)
    return true;

  for (const auto &d : devices_) {
    auto name2 = CStrUtil::toLower(d.second->name());

    if (name1 == name2) {
      windows_.clear();

      multiWindow_ = 0;

      device_ = d.second;

      CExprInst->createStringVariable("GPVAL_TERM", device_->name());

      return true;
    }
  }

  return false;
}

void
CGnuPlot::
pushDevice()
{
  if (! device_) return;

  deviceStack_.push_back(device_);
}

void
CGnuPlot::
popDevice()
{
  if (deviceStack_.empty()) return;

  device_ = deviceStack_.back();

  deviceStack_.pop_back();
}

void
CGnuPlot::
setOutputFile(const std::string &file)
{
  outputFile_ = file;

  CExprInst->createStringVariable("GPVAL_OUTPUT", outputFile_);
}

bool
CGnuPlot::
load(const std::string &filename, const StringArray &args)
{
  CExprInst->createStringVariable ("ARG0", filename);

  if (! args.empty()) {
    CExprInst->createIntegerVariable("ARGC", args.size());

    for (uint i = 0; i < 9; ++i) {
      std::string argName = CStrUtil::strprintf("ARG%d", i + 1);

      if (i < args.size())
        CExprInst->createStringVariable(argName, args[i]);
      else
        CExprInst->createStringVariable(argName, "");
    }
  }
  else {
    CExprInst->createIntegerVariable("ARGC", 0);

    for (uint i = 1; i <= 9; ++i) {
      std::string argName = CStrUtil::strprintf("ARG%d", i);

      CExprInst->removeVariable(argName);
    }
  }

  //---

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

  CExprInst->createIntegerVariable("GPVAL_LINENO", fileData_.lineNum);

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

  if (line.isChar('#')) { // comments char ?
    return true;
  }

  //----

  std::string str1 = replaceEmbedded(str);

  //----

  CParseLine line1(str1);

  line1.skipSpace();

  if (line1.isChar('!')) {
    line1.skipChar();

    StringArray lines;

    if (! execCmd(line1.substr(), lines))
      return false;

    for (const auto &l : lines)
      std::cout << l << std::endl;

    return true;
  }

  //----

  Statements statements;

  auto str2 = str1;

  auto p = str2.find(';');

  while (p != std::string::npos) {
    auto str3 = str2.substr(0, p);

    statements.push_back(str3);

    str2 = str2.substr(p + 1);

    p = str2.find(';');
  }

  statements.push_back(str2);

  for (int i = 0; i < int(statements.size()); ++i) {
    if (! parseStatement(i, statements))
      return false;
  }

  return true;
}

std::string
CGnuPlot::
replaceEmbedded(const std::string &str) const
{
  CParseLine line(str);

  std::string str1;

  while (line.isValid()) {
    char c = line.getChar();

    if      (c == '`') {
      std::string str2;

      while (line.isValid()) {
        char c1 = line.getChar();

        if (c1 == '`')
          break;

        str2 += c1;
      }

      StringArray lines;

      (void) execCmd(str2, lines);

      for (const auto &line : lines)
        str1 += line;
    }
    else if (c == '\'') {
      line.ungetChar();

      std::string str2;

      (void) parseString(line, str2);

      str1 += "'" + str2 + "'";
    }
    else if (c == '\"') {
      line.ungetChar();

      std::string str2;

      (void) parseString(line, str2);

      CParseLine line1(str2);

      str1 += "\"" + str2 + "\"";
    }
    else if (c == '@') {
      std::string str2;

      while (line.isValid() && ! line.isSpace()) {
        char c1 = line.getChar();

        str2 += c1;
      }

      std::string str3;

      if (isMacros() && getStringVariable(str2, str3))
        str1 += str3;
      else
        str1 = "@" + str2;
    }
    else
      str1 += c;
  }

  return str1;
}

bool
CGnuPlot::
execCmd(const std::string &cmd, StringArray &lines) const
{
  FILE *fp = popen(cmd.c_str(), "r");
  if (! fp) return false;

  std::string line;

  int c;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      lines.push_back(line);

      line = "";
    }
    else
      line += char(c);
  }

  if (line != "")
    lines.push_back(line);

  pclose(fp);

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
  if (identifier.empty()) {
    if (line.substr() == "")
      return true;
  }

  bool keyword = (identifier == "if" || identifier == "while");

  //---

  if (! keyword) {
    // function definition
    if      (line.skipSpaceAndChar('(')) {
      StringArray args;

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

    case CommandName::SET     : setCmd     (args); break;
    case CommandName::GET     : getCmd     (args); break;
    case CommandName::SHOW    : showCmd    (args); break;
    case CommandName::RESET   : resetCmd   (args); break;
    case CommandName::UNDEFINE: undefineCmd(args); break;
    case CommandName::UNSET   : unsetCmd   (args); break;

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
exitCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "exit " << args << std::endl;

  exit(0);
}

// help [{topic}]
void
CGnuPlot::
helpCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "help " << args << std::endl;

  CParseLine line(args);

  line.skipSpace();

  std::string arg = readNonSpace(line);

  if (arg == "commands") {
    StringArray names;

    CStrUniqueMatch::values<CGnuPlot::CommandName>(names);

    COptInt len;

    for (const auto &name : names)
      len.updateMax(name.size());

    int cols = 100/(len.getValue(0) + 1);

    int col = 0;

    for (const auto &name : names) {
      std::string fmt = CStrUtil::strprintf("%%%ds", len.getValue(0));

      std::cout << CStrUtil::strprintf(fmt.c_str(), name.c_str()) << " ";

      ++col;

      if (col >= cols) {
        std::cout << std::endl;

        col = 0;
      }
    }

    if (col > 0)
      std::cout << std::endl;
  }
}

// history [quiet] [{max:i}] ["<filename>" [append]]
// history ?"<cmd>"
// history !"<cmd>"
void
CGnuPlot::
historyCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "history " << args << std::endl;

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
  if (isDebug()) std::cerr << "print " << args << std::endl;

  Values values;

  if (! CExprInst->evaluateExpression(args, values)) {
    errorMsg("Invalid expression '" + args + "'");
    return;
  }

  CExprValueType lastType = CEXPR_VALUE_STRING;

  for (const auto &value : values) {
    CExprValueType thisType = value->getType();

    if (lastType != CEXPR_VALUE_STRING && thisType != CEXPR_VALUE_STRING)
      std::cerr << " ";

    std::cerr << value;

    lastType = thisType;
  }

  std::cerr << std::endl;
}

// printf <format> {expression} [, {expression} ]
void
CGnuPlot::
printfCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "printf " << args << std::endl;

  CParseLine line(args);

  std::string fmt;

  if (! parseString(line, fmt)) {
    std::cerr << "Invalid format" << std::endl;
    return;
  }

  Values values;

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
quitCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "quit " << args << std::endl;

  exit(0);
}

// cd "<path"
void
CGnuPlot::
cdCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "cd " << args << std::endl;

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

  CExprInst->createStringVariable("GPVAL_PWD", CDir::getCurrent());
}

// pwd
void
CGnuPlot::
pwdCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "pwd " << args << std::endl;

  std::cout << CDir::getCurrent() << std::endl;
}

// call "<filename>" [ {val}, ... ]
void
CGnuPlot::
callCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "call " << args << std::endl;

  CParseLine line(args);

  std::string filename;

  if (! parseString(line, filename))
    return;

  StringArray args1;

  while (line.isValid()) {
    line.skipSpace();

    std::string arg;

    if (line.isChar('"')) {
      if (! parseString(line, arg))
        arg = "";
    }
    else {
      arg = readNonSpace(line);

      if (arg.size() > 2 && arg[0] == '(' && arg[arg.size() - 1] == ')') {
        CExprValuePtr value;
        std::string   str;

        if (evaluateExpression(arg.substr(1, arg.size() - 2), value) &&
            value.isValid() && value->getStringValue(str))
          arg = str;
      }
      else {
        CExprVariablePtr var = CExprInst->getVariable(arg);

        CExprValuePtr value = (var.isValid() ? var->getValue() : CExprValuePtr());
        std::string   str;

        if (value.isValid() && value->isStringValue() && value->getStringValue(str))
          arg = str;
      }
    }

    args1.push_back(arg);
  }

  bool macros = isMacros();

  setMacros(true);

  load(filename, args1);

  setMacros(macros);
}

// load "<filename>"
void
CGnuPlot::
loadCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "load " << args << std::endl;

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
saveCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "save " << args << std::endl;

  CParseLine line(args);

  std::string filename;

  if (! parseString(line, filename))
    return;

  std::ofstream os(filename.c_str());

  os << "# CGnuPlot Version 0.1" << std::endl;

  /*
  unset clip points
  set clip one
  unset clip two
  */

  bars_.save(os);

  axesData_.border.save(os);

  /*
  set zdata
  set ydata
  set xdata
  set y2data
  set x2data
  */

  boxWidth_.save(os);

  /*
  set style fill empty border
  set style rectangle back fc bgnd fillstyle solid 1.00 border lt -1
  set style circle radius graph 0.02, first 0.00000, 0.00000
  set style ellipse size graph 0.05, 0.03, first 0.00000 angle 0 units xy

  set dummy x, y
  */

  axesData_.xaxis[1].showFormat(os, "x");
  axesData_.yaxis[1].showFormat(os, "y");
  axesData_.xaxis[2].showFormat(os, "x2");
  axesData_.yaxis[2].showFormat(os, "y2");
  axesData_.zaxis[1].showFormat(os, "z");
  axesData_.cbaxis  .showFormat(os, "cb");
  axesData_.raxis   .showFormat(os, "r");

  os << "set timefmt \"" << timeFmt() << "\"" << std::endl;

  showAngleType(os);

  /*
  unset grid
  set raxis
  set style parallel front lt black linewidth 2.000 dashtype solid
  set key title ""
  set key inside right top vertical Right noreverse enhanced autotitle nobox
  set key noinvert samplen 4 spacing 1 width 0 height 0
  set key maxcolumns 0 maxrows 0
  set key noopaque
  unset label
  unset arrow
  set style increment default
  unset style line
  unset style arrow
  set style histogram clustered gap 2 title textcolor lt -1
  unset object
  set style textbox transparent margins 1.0, 1.0 border
  unset logscale
  set offsets 0, 0, 0, 0
  set pointsize 1
  set pointintervalbox 1
  set encoding default
  unset polar
  unset parametric
  unset decimalsign
  set view 60, 30, 1, 1
  */

  samples_   .save(os);
  isoSamples_.save(os);

  /*
  set surface
  unset contour
  set cntrlabel format '%8.3g' font '' start 5 interval 20
  set mapping cartesian
  */

  dataFile_.save(os);

  /*
  unset hidden3d
  set cntrparam order 4
  set cntrparam linear
  set cntrparam levels auto 5
  set cntrparam points 5
  set size ratio 0 1,1
  set origin 0,0
  set style data points
  set style function lines

  unset xzeroaxis
  unset yzeroaxis
  unset zzeroaxis
  unset x2zeroaxis
  unset y2zeroaxis

  set ticslevel 0.5
  set tics scale 1, 0.5, 1, 1, 1

  set mxtics default
  set mytics default
  set mztics default
  set mx2tics default
  set my2tics default
  set mcbtics default
  set mrtics default

  set xtics border in scale 1,0.5 mirror norotate autojustify
  set xtics autofreq norangelimit
  set ytics border in scale 1,0.5 mirror norotate autojustify
  set ytics autofreq norangelimit
  set ztics border in scale 1,0.5 nomirror norotate autojustify
  set ztics autofreq norangelimit

  unset x2tics
  unset y2tics

  set cbtics border in scale 1,0.5 mirror norotate autojustify
  set cbtics autofreq  norangelimit
  set rtics axis in scale 1,0.5 nomirror norotate autojustify
  set rtics autofreq  norangelimit

  unset paxis 1 tics
  unset paxis 2 tics
  unset paxis 3 tics
  unset paxis 4 tics
  unset paxis 5 tics
  unset paxis 6 tics
  unset paxis 7 tics
  */

  title_.save(os);

  /*
  set timestamp bottom
  set timestamp ""
  set timestamp font "" norotate

  set rrange [ * : * ] noreverse nowriteback
  set trange [ * : * ] noreverse nowriteback
  set urange [ * : * ] noreverse nowriteback
  set vrange [ * : * ] noreverse nowriteback

  set xlabel ""
  set xlabel font "" textcolor lt -1 norotate
  set x2label ""
  set x2label font "" textcolor lt -1 norotate

  set xrange [ * : * ] noreverse nowriteback
  set x2range [ * : * ] noreverse nowriteback

  set ylabel ""
  set ylabel font "" textcolor lt -1 rotate by -270
  set y2label ""
  set y2label font "" textcolor lt -1 rotate by -270

  set yrange [ * : * ] noreverse nowriteback
  set y2range [ * : * ] noreverse nowriteback

  set zlabel ""
  set zlabel font "" textcolor lt -1 norotate
  set zrange [ * : * ] noreverse nowriteback

  set cblabel ""
  set cblabel font "" textcolor lt -1 rotate by -270
  set cbrange [ * : * ] noreverse nowriteback

  set paxis 1 range [ * : * ] noreverse nowriteback
  set paxis 2 range [ * : * ] noreverse nowriteback
  set paxis 3 range [ * : * ] noreverse nowriteback
  set paxis 4 range [ * : * ] noreverse nowriteback
  set paxis 5 range [ * : * ] noreverse nowriteback
  set paxis 6 range [ * : * ] noreverse nowriteback
  set paxis 7 range [ * : * ] noreverse nowriteback

  set zero 1e-08

  set lmargin -1
  set bmargin -1
  set rmargin -1
  set tmargin -1

  set locale "en_US.UTF-8"

  set pm3d explicit at s
  set pm3d scansautomatic
  set pm3d interpolate 1,1 flush begin noftriangles noborder corners2color mean

  set palette positive nops_allcF maxcolors 0 gamma 1.5 color model RGB
  set palette rgbformulae 7, 5, 15
*/

  colorBox_.save(os);

/*
  set style boxplot candles range 1.50 outliers pt 7 separation 1 labels auto unsorted

  set loadpath
  set fontpath
  set psdir
  set fit brief errorvariables nocovariancevariables errorscaling prescale nowrap v5
*/
  showVariables(os);
}

// plot [ {ranges} ]
//      [ (function) | "{str:filename}" ]
//      {csv}
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

  lastPlotCmd_  = args;
  lastSPlotCmd_ = "";
  lastFilename_ = "";

  //---

  resetLineStyle();

  COptValT<CBBox2D> clearRect = clearRect_;

  clearRect_.setInvalid();

  //---

  // TODO: update local line style copy

  CGnuPlotWindowP window;

  if (multiplot_.isEnabled()) {
    if (! multiWindow_) {
      multiWindow_ = CGnuPlotWindowP(createWindow());

      multiWindow_->set3D(false);
      multiWindow_->setBackgroundColor(backgroundColor());

      windows_.push_back(multiWindow_);
    }

    window = multiWindow_;

    int n = window->numGroups();

    double dx = multiplot_.dx();
    double dy = multiplot_.dy();

    CIPoint2D pos = multiplot_.pos(n);

    double x1 = plotSize_.xo.getValue(pos.x*dx);
    double y1 = plotSize_.yo.getValue(pos.y*dy);
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

    double x1 = plotSize_.xo.getValue(0.0);
    double y1 = plotSize_.yo.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region_ = CBBox2D(x1, y1, x2, y2);

    windows_.push_back(window);
  }

  //----

  std::vector<std::string> cmds;

  splitPlotCmd(args, cmds);

  //---

  CGnuPlotGroup *group = createGroup(window.get());

  group->set3D(false);

  Plots plots;

  for (const auto &cmd : cmds) {
    incLineStyle();

    plotCmd1(cmd, group, plots);
  }

  //---

  if (plots.empty())
    return;

  //---

  group->addObjects();

  group->init();
  group->set3D(false);

  group->setClearRect(clearRect);

  group->setTitleData(title_);

  group->addSubPlots(plots);

  group->fit();

  window->addGroup(group);

  for (auto plot : plots)
    plot->smooth();

  //---

  if (device())
    stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
}

void
CGnuPlot::
plotCmd1(const std::string &args, CGnuPlotGroup *group, Plots &plots)
{
  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    resetLineStyle();

    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return;

    plotForCmd(forCmd, cmd, group, plots);

    return;
  }

  //----

  //bool newhistogram = false;

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
  CGnuPlotLineStyle  lineStyle  = *this->lineStyle();
  CGnuPlotFillStyle  fillStyle  = this->fillStyle();
  CGnuPlotArrowStyle arrowStyle = this->arrowStyle();

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

  StringArray functions;

  line.skipSpace();

  if (line.isChar('\"') || line.isChar('\'')) {
    (void) parseString(line, filename, "Invalid filename");

    // special filename '' (use previous file)
    if      (filename == "") {
      filename = lastFilename_;
    }
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

    lastFilename_ = filename;

    style = getDataStyle();
  }
  else {
    // function
    FunctionData functionData;

    if (! parseFunction(line, functionData))
      return;

    if (! functionData.isAssign) {
      functions.push_back(functionData.function);
    }
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
      parseModifiers2D(style, line, lineStyle, fillStyle, arrowStyle);
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
      int everyPointStart, everyPointEnd, everyPointStep;
      int everyBlockStart, everyBlockEnd, everyBlockStep;

      parseEvery(line, everyPointStart, everyPointEnd, everyPointStep,
                 everyBlockStart, everyBlockEnd, everyBlockStep);

      dataFile_.setEvery(everyPointStart, everyPointEnd, everyPointStep,
                         everyBlockStart, everyBlockEnd, everyBlockStep);
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

      parseModifiers2D(style, line, lineStyle, fillStyle, arrowStyle);
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
        keyData_.setColumnNum (columnNum);
        keyData_.setColumnHead(true);
      }

      line.skipSpace();
    }
    // disable title
    else if (plotVar == PlotVar::NOTITLE) {
      // TODO
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
        arrowStyle = this->arrowStyle(as);
    }
    // heads
    else if (plotVar == PlotVar::HEADS) {
      arrowStyle.setHeads(true, true);
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
          int w;

          if (parseInteger(line, w))
            imageStyle_.w = w;

          if (line.skipSpaceAndChar(',')) {
            int h;

            if (parseInteger(line, h))
              imageStyle_.h = h;
          }

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
    // filetype
    else if (plotVar == PlotVar::FILETYPE) {
      if (line.skipSpaceAndChar('=')) {
        std::string arg = readNonSpace(line);

        CGnuPlotTypes::ImageType imageType;

        if (CStrUniqueMatch::stringToValue(arg, imageType))
          imageStyle_.fileType = imageType;
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
            imageStyle_.a = CAngle::Rad2Deg(a*M_PI);
          else
            imageStyle_.a = CAngle::Rad2Deg(a);
        }
      }
    }
    // whiskerbars [<fraction>]?
    else if (plotVar == PlotVar::WHISKERBARS) {
      setWhiskerBars(1);
    }
  }

  //---

  Plots plots1;

  if      (! filename.empty()) {
    if (isBinary()) {
      if (isImageStyle(style)) {
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

    plot1->setLineStyle (lineStyle);
    plot1->setFillStyle (fillStyle);
    plot1->setArrowStyle(arrowStyle);

    plots.push_back(plot1);
  }

  //---

  while (line.isValid()) {
    line.skipSpace();

    std::string arg = readNonSpaceNonComma(line);

    if (arg != "")
      errorMsg("Invalid arg '" + arg + "'");
  }
}

void
CGnuPlot::
splitPlotCmd(const std::string &cmd, std::vector<std::string> &cmds)
{
  CParseLine line(cmd);

  std::string cmd1;

  while (line.isValid()) {
    if (line.isChar(',')) {
      if (cmd1 != "")
        cmds.push_back(cmd1);

      cmd1 = "";

      line.skipChar();
    }
    else if (line.isChar('\"') || line.isChar('\'')) {
      int pos = line.pos();

      skipString(line);

      cmd1 += line.substr(pos, line.pos() - pos);
    }
    else if (line.isChar('(')) {
      int pos = line.pos();

      skipBracketedString(line);

      cmd1 += line.substr(pos, line.pos() - pos);
    }
    else
      cmd1 += line.getChar();
  }

  if (cmd1 != "")
    cmds.push_back(cmd1);
}

bool
CGnuPlot::
parseModifiers2D(PlotStyle /*style*/, CParseLine &line, CGnuPlotLineStyle &lineStyle,
                 CGnuPlotFillStyle &fillStyle, CGnuPlotArrowStyle &arrowStyle)
{
  bool modifiers = true;

  while (modifiers) {
    line.skipSpace();

    if      (line.isString("linestyle") || line.isString("ls")) {
      line.skipNonSpace();

      int ls;

      if (parseInteger(line, ls))
        setLineStyleInd(ls);
    }
    else if (line.isString("linetype") || line.isString("lt")) {
      line.skipNonSpace();

      int lt;

      if (parseInteger(line, lt))
        lineStyle.setType(lt);
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
        lineStyle.setColor(c);
    }
    else if (line.isString("textcolor") || line.isString("tc")) {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle.setColor(c);
    }
    else if (line.isString("pointtype") || line.isString("pt")) {
      line.skipNonSpace();

      std::string typeStr = readNonSpaceNonComma(line);

      int pt;

      if (parseInteger(line, pt))
        lineStyle.setPointType(static_cast<CGnuPlotTypes::SymbolType>(pt));
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

      int pos = line.pos();

      std::string style = readNonSpaceNonComma(line);

      int as;

      if (style == "variable" || style == "var")
        arrowStyle.setVariable(true);
      else {
        line.setPos(pos);

        if (parseInteger(line, as))
          arrowStyle = this->arrowStyle(as);
      }
    }
    else if (line.isString("filled")) {
      arrowStyle.setFilled(true);

      line.skipNonSpace();
    }
    else if (line.isString("nohead")) {
      arrowStyle.setHeads(false, false);

      line.skipNonSpace();
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
plotForCmd(const ForCmd &forCmd, const std::string &args, CGnuPlotGroup *group, Plots &plots)
{
  if (forCmd.isIn) {
    std::vector<std::string> words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      plotCmd1(args, group, plots);
    }
  }
  else {
    CExprValuePtr value1;

    if (! evaluateExpression(forCmd.start, value1) || ! value1.isValid())
      return;

    CExprValuePtr value2;

    if (! evaluateExpression(forCmd.end, value2) || ! value2.isValid())
      return;

    CExprValuePtr value3;

    if (forCmd.inc != "") {
      if (! evaluateExpression(forCmd.inc, value3) || ! value3.isValid())
        value3 = CExprValuePtr();
    }

    long i1 = 0, i2 = 1, i3 = 1;

    if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
      return;

    if (value3.isValid() && ! value3->getIntegerValue(i3))
      return;

    while (i1 <= i2) {
      CExprInst->createIntegerVariable(forCmd.var, i1);

      plotCmd1(args, group, plots);

      i1 += i3;
    }
  }
}

void
CGnuPlot::
splotForCmd(const ForCmd &forCmd, const std::string &args, CGnuPlotGroup *group, Plots &plots)
{
  if (forCmd.isIn) {
    std::vector<std::string> words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      splotCmd1(args, group, plots);
    }
  }
  else {
    CExprValuePtr value1;

    if (! evaluateExpression(forCmd.start, value1) || ! value1.isValid())
      return;

    CExprValuePtr value2;

    if (! evaluateExpression(forCmd.end, value2) || ! value2.isValid())
      return;

    CExprValuePtr value3;

    if (forCmd.inc != "") {
      if (! evaluateExpression(forCmd.inc, value3) || ! value3.isValid())
        value3 = CExprValuePtr();
    }

    long i1 = 0, i2 = 1, i3 = 1;

    if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
      return;

    if (value3.isValid() && ! value3->getIntegerValue(i3))
      return;

    while (i1 <= i2) {
      CExprInst->createIntegerVariable(forCmd.var, i1);

      splotCmd1(args, group, plots);

      i1 += i3;
    }
  }
}

bool
CGnuPlot::
parseFor(CParseLine &line, ForCmd &forCmd, std::string &cmd)
{
  line.skipNonSpace();

  if (! line.skipSpaceAndChar('['))
    return false;

  if (! readIdentifier(line, forCmd.var))
    return false;

  // for [ivar = start:end:incr]
  if (line.skipSpaceAndChar('=')) {
    forCmd.isIn = false;

    line.skipSpace();

    forCmd.start = "";

    while (line.isValid() && ! line.isChar(':'))
      forCmd.start += line.getChar();

    if (! line.isChar(':'))
      return false;

    line.skipChar();
    line.skipSpace();

    forCmd.end = "";

    while (line.isValid() && ! line.isChar(']') && ! line.isChar(':'))
      forCmd.end += line.getChar();

    if (line.isChar(':')) {
      line.skipSpace();

      forCmd.inc = "";

      while (line.isValid() && ! line.isChar(']'))
        forCmd.inc += line.getChar();
    }

    if (! line.isChar(']'))
      return false;

    line.skipChar();
    line.skipSpace();
  }
  // for [svar in "<words>"]
  else if (line.isString("in")) {
    forCmd.isIn = true;

    line.skipNonSpace();

    if (! parseString(line, forCmd.start, "Invalid for in"))
      return false;

    if (! line.skipSpaceAndChar(']'))
      return false;
  }

  cmd = line.substr();

  return true;
}

// {empty |
//  {transparent} solid {density} |
//  {transparent} pattern {id}}}
//  {border {<linetype> | {lc <colorspec>} | noborder}
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
      fillStyle.setBorderLineType(lineStyle()->ind());
      fillStyle.unsetBorderColor();
    }
    else if (line.isString("lc")) {
      line.skipNonSpace();
      line.skipSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c)) {
        fillStyle.unsetBorderLineType();
        fillStyle.setBorderColor(c);
      }
    }
    else if (parseInteger(line, lt)) {
      fillStyle.setBorderLineType(lt);
      fillStyle.unsetBorderColor();
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
    StringArray fields;

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

// set xlabel {"<label>"} {offset <offset>} {font "<font>{,<size>}"}
//            {textcolor <colorspec>} {{no}enhanced}
//            {rotate by <degrees> | rotate parallel | norotate}
bool
CGnuPlot::
parseAxisLabel(CParseLine &line, CGnuPlotAxisData &axis)
{
  int pos = line.pos();

  std::string arg = readNonSpace(line);

  if      (arg == "offset") {
    double o;

    if (parseReal(line, o))
      axis.setOffset(o);
  }
  else if (arg == "font") {
    CFontPtr font;

    if (parseFont(line, font))
      axis.setFont(font);
  }
  else if (arg == "textcolor") {
    CGnuPlotColorSpec c;

    if (parseColorSpec(line, c))
      axis.setTextColor(c);
  }
  else if (arg == "enhanced" || arg == "noenhanced") {
    axis.setEnhanced(arg == "enhanced");
  }
  else if (arg == "rotate") {
    std::string arg1 = readNonSpace(line);

    if      (arg1 == "by") {
      double a;

      if (parseReal(line, a))
        axis.setRotate(a);
    }
    else if (arg1 == "parallel") {
      axis.setRotate(-1);
    }
  }
  else if (arg == "norotate") {
    axis.setRotate(0);
  }
  else {
    line.setPos(pos);

    std::string labelStr;

    if (parseString(line, labelStr, "Invalid label string"))
      axis.setText(labelStr);
  }

  return true;
}

// [{<dummary-var>=}{min}:{max}]
bool
CGnuPlot::
parseRange(CParseLine &line, StringArray &fields)
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
decodeRange(const StringArray &fields, CGnuPlotAxisData &axis)
{
  typedef std::pair<COptReal,bool> OptRealBool;

  std::vector<OptRealBool> values;

  for (const auto &field : fields) {
    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;
    bool     force = false;

    if (axis.isTime()) {
      int len = field.size();

      if (len > 0 && field[0] == '"' && field[len - 1] == '"') {
        strptime(field.substr(1, len - 2).c_str(), timeFmt().c_str(), &tm1);

        value = mktime(&tm1);
      }
    }
    else {
      auto pos = field.find('*');

      if (pos == std::string::npos) {
        CExprValueP evalue;

        if (! evaluateExpression(field, evalue))
          evalue = CExprValueP();

        double r;

        if (evalue.isValid() && evalue->getRealValue(r))
          value = r;
      }
      else {
        auto pos1 = field.find('<');

        if (pos1 != std::string::npos) {
          std::string lhs = field.substr(0, pos1);
          std::string rhs = field.substr(pos1 + 1);
          std::string mid;

          auto pos2 = rhs.find('<');

          if (pos2 != std::string::npos) {
            mid = rhs.substr(0, pos2);
            rhs = rhs.substr(pos2 + 1);
          }
        }

        force = true;
      }
    }

    values.push_back(OptRealBool(value, force));
  }

  if      (values.size() >= 2) {
    COptReal &min = values[0].first;
    COptReal &max = values[1].first;

    if (isDebug())
      std::cerr << "Range=(" << min.getValue(-1) << "," <<
                                max.getValue( 1) << ")" << std::endl;

    if (values[0].second) axis.setMin(min);
    if (values[1].second) axis.setMax(max);

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
    COptReal &min = values[0].first;

    if (isDebug())
      std::cerr << "Range=(" << min.getValue(-1) << ",*)" << std::endl;

    if (values[0].second) axis.setMin(min);

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
replotCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "replot " << args << std::endl;

  if      (lastPlotCmd_ != "")
    plotCmd(lastPlotCmd_);
  else if (lastSPlotCmd_ != "")
    splotCmd(lastSPlotCmd_);
}

// refresh
void
CGnuPlot::
refreshCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "refresh " << args << std::endl;
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
  if (isDebug()) std::cerr << "splot " << args << std::endl;

  lastSPlotCmd_ = args;
  lastPlotCmd_  = "";
  lastFilename_ = "";

  //----

  resetLineStyle();

  COptValT<CBBox2D> clearRect = clearRect_;

  clearRect_.setInvalid();

  //----

  // TODO: update local line style copy

  CGnuPlotWindowP window;

  if (multiplot_.isEnabled()) {
    if (! multiWindow_) {
      multiWindow_ = CGnuPlotWindowP(createWindow());

      multiWindow_->set3D(true);
      multiWindow_->setBackgroundColor(backgroundColor());

      windows_.push_back(multiWindow_);
    }

    window = multiWindow_;

    int n = window->numGroups();

    double dx = multiplot_.dx();
    double dy = multiplot_.dy();

    CIPoint2D pos = multiplot_.pos(n);

    double x1 = plotSize_.xo.getValue(pos.x*dx);
    double y1 = plotSize_.yo.getValue(pos.y*dy);
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

    window->set3D(true);

    double x1 = plotSize_.xo.getValue(0.0);
    double y1 = plotSize_.yo.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region_ = CBBox2D(x1, y1, x2, y2);

    windows_.push_back(window);
  }

  //---

  std::vector<std::string> cmds;

  splitPlotCmd(args, cmds);

  //---

  CGnuPlotGroup *group = createGroup(window.get());

  group->set3D(true);

  Plots plots;

  for (const auto &cmd : cmds) {
    incLineStyle();

    splotCmd1(cmd, group, plots);
  }

  //---

  if (plots.empty())
    return;

  //---

  group->addObjects();

  group->init();
  group->set3D(true);

  group->setClearRect(clearRect);

  group->setTitleData(title_);

  group->addSubPlots(plots);

  group->fit();

  window->addGroup(group);

  for (auto plot : plots)
    plot->smooth();

  //---

  window->setHidden3D(hidden3D_.enabled);
  window->setSurface3D(isSurface3D());
  window->setContour3D(isContour3D());
  window->setPm3D(pm3D());

  //----

  if (device())
    stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
}

void
CGnuPlot::
splotCmd1(const std::string &args, CGnuPlotGroup *group, Plots &plots)
{
  //----

  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    resetLineStyle();

    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return;

    splotForCmd(forCmd, cmd, group, plots);

    return;
  }

  //----

  // get local copy of next line style
  incLineStyle();

  CGnuPlotLineStyle  lineStyle  = *this->lineStyle();
  CGnuPlotFillStyle  fillStyle  = this->fillStyle();
  CGnuPlotArrowStyle arrowStyle = this->arrowStyle();

  //----

  COptString keyTitle;

  if (parametric_) {
    // Get Range
    //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]
    parseAxisRange(line, axesData_.taxis[1    ], false);
    parseAxisRange(line, axesData_.xaxis[xind_], false);
    parseAxisRange(line, axesData_.yaxis[yind_], false);
    parseAxisRange(line, axesData_.zaxis[zind_], false);
  }
  else {
    // Get Range
    //  [XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]
    parseAxisRange(line, axesData_.xaxis[xind_], false);
    parseAxisRange(line, axesData_.yaxis[yind_], false);
    parseAxisRange(line, axesData_.zaxis[zind_], false);
  }

  //----

  // Get filename or expression
  //  "<filename>" | <expression>
  PlotStyle style(PlotStyle::POINTS);

  std::string filename;

  StringArray functions;

  line.skipSpace();

  if (line.isChar('\"') || line.isChar('\'')) {
    (void) parseString(line, filename, "Invalid filename");

    // special filename '' (use previous file)
    if      (filename == "") {
      filename = lastFilename_;
    }
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

    lastFilename_ = filename;

    style = getDataStyle();
  }
  else {
    // function
    FunctionData functionData;

    if (! parseFunction(line, functionData))
      return;

    if (! functionData.isAssign) {
      functions.push_back(functionData.function);
    }
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

    if (! parseOptionValue(this, line, plotVar, "plot var")) {
      parseModifiers3D(style, line, lineStyle, fillStyle, arrowStyle);
      break;
    }

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
      int everyPointStart, everyPointEnd, everyPointStep;
      int everyBlockStart, everyBlockEnd, everyBlockStep;

      parseEvery(line, everyPointStart, everyPointEnd, everyPointStep,
                 everyBlockStart, everyBlockEnd, everyBlockStep);

      dataFile_.setEvery(everyPointStart, everyPointEnd, everyPointStep,
                         everyBlockStart, everyBlockEnd, everyBlockStep);
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

      parseModifiers3D(style, line, lineStyle, fillStyle, arrowStyle);
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
        keyData_.setColumnNum (columnNum);
        keyData_.setColumnHead(true);
      }

      line.skipSpace();
    }
    // disable title
    else if (plotVar == PlotVar::NOTITLE) {
      // TODO
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
        arrowStyle = this->arrowStyle(as);
    }
    // heads
    else if (plotVar == PlotVar::HEADS) {
      arrowStyle.setHeads(true, true);
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
          int w;

          if (parseInteger(line, w))
            imageStyle_.w = w;

          if (line.skipSpaceAndChar(',')) {
            int h;

            if (parseInteger(line, h))
              imageStyle_.h = h;
          }

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
    // filetype
    else if (plotVar == PlotVar::FILETYPE) {
      if (line.skipSpaceAndChar('=')) {
        std::string arg = readNonSpace(line);

        CGnuPlotTypes::ImageType imageType;

        if (CStrUniqueMatch::stringToValue(arg, imageType))
          imageStyle_.fileType = imageType;
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
            imageStyle_.a = CAngle::Rad2Deg(a*M_PI);
          else
            imageStyle_.a = CAngle::Rad2Deg(a);
        }
      }
    }
  }

  //---

  Plots plots1;

  if      (! filename.empty()) {
    if (isBinary()) {
      std::cerr << "Unhandled binary plot" << std::endl;
    }
    else
      plots1 = addFile3D(group, filename, style, usingCols);
  }
  else if (! functions.empty()) {
    CGnuPlotPlot *plot = addFunction3D(group, functions, style);

    if (plot)
      plots1.push_back(plot);
  }

  for (const auto &plot1 : plots1) {
    if (keyTitle.isValid())
      plot1->setKeyTitle(keyTitle.getValue());

    plot1->setLineStyle (lineStyle);
    plot1->setFillStyle (fillStyle);
    plot1->setArrowStyle(arrowStyle);

    plots.push_back(plot1);
  }

  //---

  while (line.isValid()) {
    line.skipSpace();

    std::string arg = readNonSpaceNonComma(line);

    if (arg != "")
      errorMsg("Invalid arg '" + arg + "'");
  }
}

bool
CGnuPlot::
parseModifiers3D(PlotStyle /*style*/, CParseLine &line, CGnuPlotLineStyle &lineStyle,
                 CGnuPlotFillStyle &fillStyle, CGnuPlotArrowStyle &arrowStyle)
{
  bool modifiers = true;

  while (modifiers) {
    line.skipSpace();

    if      (line.isString("linestyle") || line.isString("ls")) {
      line.skipNonSpace();

      int ls;

      if (parseInteger(line, ls))
        setLineStyleInd(ls);
    }
    else if (line.isString("linetype") || line.isString("lt")) {
      line.skipNonSpace();

      int lt;

      if (parseInteger(line, lt))
        lineStyle.setType(lt);
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
        lineStyle.setColor(c);
    }
    else if (line.isString("pointtype") || line.isString("pt")) {
      line.skipNonSpace();

      int pt;

      if (parseInteger(line, pt))
        lineStyle.setPointType(static_cast<CGnuPlotTypes::SymbolType>(pt));
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
    else if (line.isString("filled")) {
      arrowStyle.setFilled(true);

      line.skipNonSpace();
    }
    else if (line.isString("nohead")) {
      arrowStyle.setHeads(false, false);

      line.skipNonSpace();
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
    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return false;

    setForCmd(forCmd, cmd);

    return false;
  }

  //----

  VariableName var;

  if (! parseOptionValue(this, line, var, "option name"))
    return false;

  // set angles [ radians | degrees ]
  if      (var == VariableName::ANGLES) {
    std::string arg = readNonSpace(line);

    if (arg != "") {
      CGnuPlotTypes::AngleType angleType;

      if (CStrUniqueMatch::stringToValue(arg, angleType))
        setAngleType(angleType);
    }
    else
      resetAngleType();
  }
  // set arrow {<tag>} {from <position> to  <position>}
  //                   {from <position> rto <position>}
  //                   {from <position> length <coord> angle <angle>}
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

    //---

    // parse from->to
    std::string arg = readNonSpace(line);

    if (arg == "from") {
      CGnuPlotPosition from, to;

      if (! parsePosition(line, from))
        return false;

      arg = readNonSpace(line);

      if (arg == "to" || arg == "rto") {
        if (! parsePosition(line, to))
          return false;

        if (arg == "to")
          arrow->setFromTo(from, to);
        else
          arrow->setFromRTo(from, to);

        arg = readNonSpace(line);
      }
      else if (arg == "length") {
        CGnuPlotCoordValue l;

        if (! parseCoordValue(line, l))
          return false;

        arg = readNonSpace(line);

        double r = 0.0;

        if (arg == "angle") {
          if (! parseReal(line, r))
            r = 0.0;

          arg = readNonSpace(line);
        }

        arrow->setFromAngle(from, l, CAngle::makeDegrees(r));
      }
      else
        arrow->setFromTo(from, to);
    }

    line.skipSpace();

    //---

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
      // size <headlength>,<headangke>{,<backangle>}
      else if (arg == "size") {
        CGnuPlotCoordValue headlength;

        if (parseCoordValue(line, headlength))
          arrow->setHeadLength(headlength);

        if (line.skipSpaceAndChar(',')) {
          double headangke;

          if (parseReal(line, headangke))
            arrow->setHeadAngle(headangke);

          if (line.skipSpaceAndChar(',')) {
            double backangle;

            if (parseReal(line, backangle))
              arrow->setHeadBackAngle(backangle);
          }
        }
      }
      else if (arg == "filled" || arg == "empty" || arg == "nofilled") {
        arrow->setHeadFilled(arg == "filled" || arg == "empty");
        arrow->setHeadEmpty (arg == "empty" );
      }
      else if (arg == "front" || arg == "back" || arg == "behind") {
        if      (arg == "front" ) arrow->setLayer(DrawLayer::FRONT);
        else if (arg == "back"  ) arrow->setLayer(DrawLayer::BACK);
        else if (arg == "behind") arrow->setLayer(DrawLayer::BEHIND);
      }
      else if (arg == "linestyle" || arg == "ls") {
        int ls = -1;

        if (! parseInteger(line, ls))
          arrow->setLineStyle(ls);
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
      else if (arg == "dashtype" || arg == "dt") {
        int dt = -1;

        if (! parseInteger(line, dt))
          arrow->setDash(lineDashes_[dt]);
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
  }
  // set autoscale {<axes>{|min|max|fixmin|fixmax|fix} | fix | keepfix}
  // set autoscale {noextend}
  else if (var == VariableName::AUTOSCALE) {
    std::string arg = readNonSpace(line);

    if      (arg == "") {
      for (int i = 1; i <= 2; ++i) {
        axesData_.xaxis[i].setAutoScaleMin(true);
        axesData_.xaxis[i].setAutoScaleMax(true);
        axesData_.yaxis[i].setAutoScaleMin(true);
        axesData_.yaxis[i].setAutoScaleMax(true);

        axesData_.xaxis[i].setAutoScaleFixMin(false);
        axesData_.xaxis[i].setAutoScaleFixMax(false);
        axesData_.yaxis[i].setAutoScaleFixMin(false);
        axesData_.yaxis[i].setAutoScaleFixMax(false);
      }
    }
    else if (arg == "noextend") {
      for (int i = 1; i <= 2; ++i) {
        axesData_.xaxis[i].setAutoScaleFixMin(true);
        axesData_.xaxis[i].setAutoScaleFixMax(true);
        axesData_.yaxis[i].setAutoScaleFixMin(true);
        axesData_.yaxis[i].setAutoScaleFixMax(true);
      }
    }
    else if (arg == "x") {
      axesData_.xaxis[1].setAutoScaleMin(true);
      axesData_.xaxis[1].setAutoScaleMax(true);
    }
    else if (arg == "y") {
      axesData_.yaxis[1].setAutoScaleMin(true);
      axesData_.yaxis[1].setAutoScaleMax(true);
    }
    else if (arg == "xy") {
      axesData_.xaxis[1].setAutoScaleMin(true);
      axesData_.xaxis[1].setAutoScaleMin(true);
      axesData_.yaxis[1].setAutoScaleMin(true);
      axesData_.yaxis[1].setAutoScaleMin(true);
    }
    else if (arg == "xmin")
      axesData_.xaxis[1].setAutoScaleMin(true);
    else if (arg == "ymin")
      axesData_.yaxis[1].setAutoScaleMin(true);
    else if (arg == "xmax")
      axesData_.xaxis[1].setAutoScaleMax(true);
    else if (arg == "ymax")
      axesData_.yaxis[1].setAutoScaleMax(true);
    else if (arg == "fix") {
      axesData_.xaxis[1].setAutoScaleFixMin(true);
      axesData_.xaxis[1].setAutoScaleFixMax(true);
      axesData_.yaxis[1].setAutoScaleFixMin(true);
      axesData_.yaxis[1].setAutoScaleFixMax(true);
    }
    else if (arg == "xfix") {
      axesData_.xaxis[1].setAutoScaleFixMin(true);
      axesData_.xaxis[1].setAutoScaleFixMax(true);
    }
    else if (arg == "yfix") {
      axesData_.yaxis[1].setAutoScaleFixMin(true);
      axesData_.yaxis[1].setAutoScaleFixMax(true);
    }
    else if (arg == "xfixmin")
      axesData_.xaxis[1].setAutoScaleFixMin(true);
    else if (arg == "yfixmin")
      axesData_.yaxis[1].setAutoScaleFixMin(true);
    else if (arg == "xfixmax")
      axesData_.xaxis[1].setAutoScaleFixMax(true);
    else if (arg == "yfixmax")
      axesData_.yaxis[1].setAutoScaleFixMax(true);
    else
      errorMsg("Unhandled autoscale " + arg);
  }
  // set bars {small | large | fullwidth | size} ] {front | back}
  else if (var == VariableName::BARS) {
    line.skipSpace();

    double size  = 1.0;

    if (line.isString("small") || line.isString("large") || line.isString("fullwidth")) {
      std::string str = readNonSpace(line);

      if      (str == "small"    ) size = 0.0;
      else if (str == "large"    ) size = 1.0;
      else if (str == "fullwidth") size = -2.0;

      setBarsSize(size);
    }
    else if (parseReal(line, size)) {
      setBarsSize(size);
    }

    line.skipSpace();

    if      (line.isString("front") || line.isString("back")) {
      std::string str = readNonSpace(line);

      setBarsFront(str == "front" ? true : false);
    }

  }
  // set bind (not supported)
  else if (var == VariableName::BIND) {
  }
  // set bmargin {at screen} <val>
  else if (var == VariableName::BMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isChars("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setBottom(r, screen);
  }
  // set border {<integer>} {front | back} {linewidth | lw <line_width>}
  //            {{linestyle | ls <line_style>} | {linetype | lt <line_type>}}
  else if (var == VariableName::BORDER) {
    std::string arg = readNonSpace(line);

    if (arg == "")
      axesData_.border.sides = 0xFF;
    else {
      while (arg != "") {
        if      (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) axesData_.border.layer = DrawLayer::FRONT;
          else if (arg == "back"  ) axesData_.border.layer = DrawLayer::BACK;
          else if (arg == "behind") axesData_.border.layer = DrawLayer::BEHIND;
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (! parseReal(line, lw))
            axesData_.border.lineWidth = lw;
        }
        else if (arg == "linestyle" || arg == "ls") {
          int i;

          if (! parseInteger(line, i))
            axesData_.border.lineStyle = i;
        }
        else if (arg == "linetype" || arg == "lt") {
          int i;

          if (! parseInteger(line, i))
            axesData_.border.lineType = i;
        }
        else {
          int i;

          if (getIntegerVariable(arg, i))
            axesData_.border.sides = i;
        }

        arg = readNonSpace(line);
      }
    }
  }
  // set boxwidth [ {size:r} ] [ absolute | relative ]
  else if (var == VariableName::BOXWIDTH) {
    CGnuPlotTypes::BoxWidthType boxWidthType = CGnuPlotTypes::BoxWidthType::AUTO;
    double                      boxWidth     = boxWidth_.width();

    if (line.isValid()) {
      std::string sizeStr = readNonSpace(line);

      double r;

      if (CStrUtil::toReal(sizeStr, &r)) {
        boxWidth = (r > 0 ? r : 1.0);

        line.skipSpace();

        if (line.isValid()) {
          std::string arg = readNonSpace(line);

          if      (arg == "abs" || arg == "absolute")
            boxWidthType = CGnuPlotTypes::BoxWidthType::ABSOLUTE;
          else if (arg == "rel" || arg == "relative")
            boxWidthType = CGnuPlotTypes::BoxWidthType::RELATIVE;
        }
        else
          boxWidthType = CGnuPlotTypes::BoxWidthType::ABSOLUTE;
      }
    }

    setBoxWidth(CGnuPlotBoxWidth(boxWidth, boxWidthType));
  }
  // set clip <clip-type>
  else if (var == VariableName::CLIP) {
    std::string arg = readNonSpace(line);

    if (arg != "") {
      while (arg != "") {
        if      (arg == "points") clip_.setPoints(true);
        else if (arg == "one"   ) clip_.setOne   (true);
        else if (arg == "two"   ) clip_.setTwo   (true);

        arg = readNonSpace(line);
      }
    }
    else
      clip_.reset();
  }
  // set cntrlabel {format "format"} {font "font"}
  // set cntrlabel {start <int>} {interval <int>}
  else if (var == VariableName::CNTRLABEL) {
  }
  // set cntrparam { { linear | cubicspline | bspline | points <n> | order <n> |
  //                   levels { auto {<n>} | <n> | discrete <z1> {,<z2>{,<z3>...}} |
  //                            incremental <start>, <incr> {,<end>} } } }
  else if (var == VariableName::CNTRPARAM) {
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
        CGnuPlotPosition p;

        if (parsePosition(line, p))
          colorBox_.setOrigin(p);
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
  // set colornames (invalid)
  else if (var == VariableName::COLORNAMES) {
  }
  // set colorsequence {default|classic|podo}
  else if (var == VariableName::COLORSEQUENCE) {
    std::string arg = readNonSpace(line);

    if (arg != "" && CGnuPlotStyleInst->isNamedColor(arg))
      colorSequence_ = arg;
    else
      colorSequence_ = "default";

    CGnuPlotStyleInst->setDefName(colorSequence_);
  }
  // set contour {base | surface | both}
  else if (var == VariableName::CONTOUR) {
    std::string arg = readNonSpace(line);

    if      (arg == "base")
      contour3D_.pos = Contour3DData::DrawPos::DRAW_BASE;
    else if (arg == "surface")
      contour3D_.pos = Contour3DData::DrawPos::DRAW_SURFACE;
    else if (arg == "both")
      contour3D_.pos = Contour3DData::DrawPos::DRAW_BOTH;

    contour3D_.enabled = true;
  }
  // set dashtype
  else if (var == VariableName::DASHTYPE) {
    int type;

    if (! parseInteger(line, type))
      return false;

    std::vector<double> dashes;
    std::string         str;

    if      (line.skipSpaceAndChar('(')) {
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
    else if (parseString(line, str)) {
      uint i = 0;

      while (i < str.size()) {
        int n1 = 0, n2 = 0;

        if (str[i] == '.') { while (i < str.size() && str[i] == '.') { ++i; ++n1; } }
        if (str[i] == ' ') { while (i < str.size() && str[i] == ' ') { ++i; ++n1; } }

        if (str[i] == '_') { while (i < str.size() && str[i] == '_') { ++i; ++n2; } }
        if (str[i] == '-') { while (i < str.size() && str[i] == '-') { ++i; ++n2; } }

        if (n1 == 0 && n2 == 0) ++i;

        if (n1 > 0) dashes.push_back(n1);
        if (n2 > 0) dashes.push_back(n2);
      }
    }

    lineDashes_[type] = CLineDash(dashes);
  }
  // set datafile fortran
  // set datafile nofpe_trap
  // set datafile missing ["<chars>"]
  // set datafile separator ["<chars>"|whitespace]
  // set datafile commentschar ["<chars>"]
  // set datafile binary
  // set datafile csv
  else if (var == VariableName::DATAFILE) {
    DataFileVar var1;

    if (! parseOptionValue(this, line, var1, "data file type"))
      return false;

    line.skipSpace();

    // set datafile fortran
    if      (var1 == DataFileVar::FORTRAN) {
       dataFile_.setFortran(true);
    }
    // set datafile nofpe_trap
    else if (var1 == DataFileVar::NOFPE_TRAP) {
       dataFile_.setFpeTrap(false);
    }
    // set datafile missing ["<chars>"]
    else if (var1 == DataFileVar::MISSING) {
      if (line.isValid()) {
        std::string chars;

        if (parseString(line, chars, "Invalid character string"))
          dataFile_.setMissingStr(chars);
      }
      else
        dataFile_.setMissingStr("");
    }
    // set datafile separator ["<chars>"|whitespace]
    else if (var1 == DataFileVar::SEPARATOR) {
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
    else if (var1 == DataFileVar::COMMENTS_CHARS) {
      if (line.isValid()) {
        std::string chars;

        if (! parseString(line, chars, "Invalid character string"))
          dataFile_.setCommentChars(chars);
      }
      else
        dataFile_.setCommentChars("#");
    }
    // set datafile binary
    else if (var1 == DataFileVar::BINARY) {
       dataFile_.setBinary(true);
    }
    // set datafile csv
    else if (var1 == DataFileVar::CSV) {
       dataFile_.setCsv(true);
    }
  }
  // set decimalsign [ "<char>" | locale [ "<locale>" ] ]
  else if (var == VariableName::DECIMALSIGN) {
    std::string str;

    line.skipSpace();

    if      (line.isString("locale")) {
      // todo get data from locale
      str = readNonSpace(line);

      if (parseString(line, str))
        decimalSign_.setLocale(str);
    }
    else if (parseString(line, str)) {
      if (str.size())
        decimalSign_.setChar(str[0]);
    }
  }
  // set dgrid3d {<rows>} {,{<cols>}}
  //             { splines | qnorm {<norm>} | (gauss | cauchy | exp | box | hann)
  //               {kdensity} {<dx>} {,<dy>} }
  else if (var == VariableName::DGRID3D) {
    // TODO
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
  // set encoding [ default | iso_8859_1 | iso_8859_2 | iso_8859_15 |
  //                koi8r | koi8u | cp437 | cp850 | cp852 | cp1250 ]
  // set encoding locale
  else if (var == VariableName::ENCODING) {
    std::string arg = readNonSpace(line);

    if (arg == "locale")
      encoding_ = "utf8"; // TODO: get from locale
    else
      encoding_ = arg;
  }
  // set fit {logfile {"<filename>"|default}}
  //         {{no}quiet|results|brief|verbose}
  //         {{no}errorvariables}
  //         {{no}covariancevariables}
  //         {{no}errorscaling}
  //         {{no}prescale}
  //         {maxiter <value>|default}
  //         {limit <epsilon>|default}
  //         {limit_abs <epsilon_abs>}
  //         {start-lambda <value>|default}
  //         {lambda-factor <value>|default}
  //         {script {"<command>"|default}}
  //         {v4 | v5}
  else if (var == VariableName::FIT) {
    // TODO
  }
  // set fontpath [ "<path>" ]
  else if (var == VariableName::FONTPATH) {
    fontPath_.clear();

    std::string path;

    while (parseString(line, path))
      fontPath_.push_back(path);
  }
  // set format {<axes>} {"<format-string>"}
  else if (var == VariableName::FORMAT) {
    std::string arg = readNonSpace(line);

    std::string formatStr;

    if (parseString(line, formatStr, "Invalid format string")) {
      if      (arg == "x" ) axesData_.xaxis[1].setFormat(formatStr);
      else if (arg == "y" ) axesData_.yaxis[1].setFormat(formatStr);
      else if (arg == "x2") axesData_.xaxis[2].setFormat(formatStr);
      else if (arg == "y2") axesData_.yaxis[2].setFormat(formatStr);
      else if (arg == "z" ) axesData_.zaxis[1].setFormat(formatStr);
      else if (arg == "cb") axesData_.cbaxis  .setFormat(formatStr);
      else if (arg == "r" ) axesData_.raxis   .setFormat(formatStr);
    }
  }
  // set functions (invalid)
  else if (var == VariableName::FUNCTIONS) {
    errorMsg("Invalid command");
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
    axesData_.grid.enabled = true;

    axesData_.xaxis[1].setGrid(true);
    axesData_.yaxis[1].setGrid(true);

    int ns = 0, nt = 0, nw = 0;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "noxtics" || arg == "xtics")
        axesData_.xaxis[1].setGridTics(arg == "xtics");
      else if (arg == "nomxtics" || arg == "mxtics")
        axesData_.xaxis[1].setGridMinorTics(arg == "mxtics");
      else if (arg == "noytics" || arg == "ytics")
        axesData_.yaxis[1].setGridTics(arg == "ytics");
      else if (arg == "nomytics" || arg == "mytics")
        axesData_.yaxis[1].setGridMinorTics(arg == "mytics");
      else if (arg == "noztics" || arg == "ztics")
        axesData_.zaxis[1].setGridTics(arg == "ztics");
      else if (arg == "nomztics" || arg == "mztics")
        axesData_.zaxis[1].setGridMinorTics(arg == "mztics");
      else if (arg == "nox2tics" || arg == "x2tics")
        axesData_.xaxis[2].setGridTics(arg == "x2tics");
      else if (arg == "nomx2tics" || arg == "mx2tics")
        axesData_.xaxis[2].setGridMinorTics(arg == "mx2tics");
      else if (arg == "noy2tics" || arg == "y2tics")
        axesData_.yaxis[2].setGridTics(arg == "y2tics");
      else if (arg == "nomy2tics" || arg == "my2tics")
        axesData_.yaxis[2].setGridMinorTics(arg == "my2tics");
      else if (arg == "nocbtics" || arg == "cbtics")
        axesData_.cbaxis.setGridTics(arg == "cbtics");
      else if (arg == "nomcbtics" || arg == "mcbtics")
        axesData_.cbaxis.setGridMinorTics(arg == "mcbtics");
      else if (arg == "polar") {
        double r = 0;

        if (parseReal(line, r))
          axesData_.grid.polarAngle = r;
      }
      else if (arg == "front" || arg == "back" || arg == "layerdefault") {
        if      (arg == "front"  ) axesData_.grid.layer = DrawLayer::FRONT;
        else if (arg == "back"   ) axesData_.grid.layer = DrawLayer::BACK;
        else if (arg == "default") axesData_.grid.layer = DrawLayer::DEFAULT;
      }
      else if (arg == "linestyle" || arg == "ls") {
        int ls = -1;

        if (parseInteger(line, ls)) {
          if (ns == 0)
            axesData_.grid.majorLineStyle = ls;
          else
            axesData_.grid.minorLineStyle = ls;
        }

        ++ns;
      }
      else if (arg == "linetype" || arg == "lt") {
        int lt = 0;

        if (parseInteger(line, lt)) {
          if (nt == 0)
            axesData_.grid.majorLineType = lt;
          else
            axesData_.grid.minorLineType = lt;
        }

        ++nt;
      }
      else if (arg == "linewidth" || arg == "lw") {
        double lw = 1.0;

        if (parseReal(line, lw)) {
          if (nw == 0)
            axesData_.grid.majorLineWidth = lw;
          else
            axesData_.grid.minorLineWidth = lw;
        }

        ++nw;
      }

      arg = readNonSpace(line);
    }
  }
  // set hidden3d {defaults} |
  //              { {front | back}
  //                {{offset <offset>} | {nooffset}}
  //                {trianglepattern <bitpattern>}
  //                {{undefined <level>} | {noundefined}}
  //                {{no}altdiagonal}
  //                {{no}bentover} }
  else if (var == VariableName::HIDDEN3D) {
    hidden3D_.enabled = true;

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "front" && arg == "back") {
        if      (arg == "front" ) hidden3D_.layer = DrawLayer::FRONT;
        else if (arg == "back"  ) hidden3D_.layer = DrawLayer::BACK;
      }
      else if (arg == "offset") {
        double r;

        if (parseReal(line, r))
          hidden3D_.offset = r;
      }
      else if (arg == "nooffset") {
        hidden3D_.offset = COptReal();
      }
      else if (arg == "trianglepattern") {
        int i;

        if (parseInteger(line, i))
          hidden3D_.trianglePattern = i;
      }
      else if (arg == "undefined") {
        int i;

        if (parseInteger(line, i))
          hidden3D_.undefined = std::min(std::max(i, 1), 3);
      }
      else if (arg == "noundefined") {
        hidden3D_.undefined = COptInt();
      }
      else if (arg == "noaltdiagonal" || arg == "altdiagonal") {
        hidden3D_.altdiagonal = (arg == "altdiagonal");
      }
      else if (arg == "nobentover" || arg == "bentover") {
        hidden3D_.bentover = (arg == "bentover");
      }

      arg = readNonSpace(line);
    }
  }
  // set historysize <size:i>
  else if (var == VariableName::HISTORYSIZE) {
    int size;

    if (! parseInteger(line, size))
      historyData_.size = size;
  }
  // set history {size <N>} {quiet|numbers} {full|trim} {default}
  else if (var == VariableName::HISTORY) {
    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "size") {
        int size;

        if (! parseInteger(line, size))
          historyData_.size = size;
      }
      else if (arg == "quiet" || arg == "numbers") {
        historyData_.numbers = (arg == "numbers");
      }
      else if (arg == "full" || arg == "trim") {
        historyData_.trim = (arg == "trim");
      }
      else if (arg == "default") {
        historyData_.size    = 500;
        historyData_.numbers = true;
        historyData_.trim    = true;
      }

      arg = readNonSpace(line);
    }
  }
  // set isosamples <iso_1> {,<iso_2>}
  else if (var == VariableName::ISOSAMPLES) {
    int isamples1, isamples2;

    isoSamples_.get(isamples1, isamples2);

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

    isoSamples_.set(isamples1, isamples2);
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
  //         {maxcols { <n> | auto }
  //         {maxrows { <n> | auto }
  else if (var == VariableName::KEY) {
    keyData_.setDisplayed(true);

    bool hSet = false;

    while (line.isValid()) {
      std::string arg = readNonSpace(line);

      if      (arg == "on" || arg == "off") {
        keyData_.setDisplayed(arg == "on");
      }
      else if (arg == "default") {
        keyData_.reset();
      }
      else if (arg == "inside"  || arg == "ins" ||
               arg == "outside" || arg == "out") {
        keyData_.setOutside(arg == "outside" || arg == "out");
      }
      else if (arg == "lmargin") {
        keyData_.setLMargin(true);
      }
      else if (arg == "rmargin") {
        keyData_.setRMargin(true);
      }
      else if (arg == "tmargin") {
        keyData_.setTMargin(true);
      }
      else if (arg == "bmargin") {
        keyData_.setBMargin(true);
      }
      else if (arg == "at") {
        CGnuPlotPosition p;

        if (parsePosition(line, p))
          keyData_.setPosition(p);
      }
      else if (arg == "left") {
        keyData_.setHAlign(CHALIGN_TYPE_LEFT);
        hSet = true;
      }
      else if (arg == "right" ) {
        keyData_.setHAlign(CHALIGN_TYPE_RIGHT);
        hSet = true;
      }
      else if (arg == "top") {
        keyData_.setVAlign(CVALIGN_TYPE_TOP);
      }
      else if (arg == "bottom" || arg == "bot") {
        keyData_.setVAlign(CVALIGN_TYPE_BOTTOM);
      }
      else if (arg == "center" || arg == "cent") {
        if (hSet)
          keyData_.setHAlign(CHALIGN_TYPE_CENTER);
        else
          keyData_.setVAlign(CVALIGN_TYPE_CENTER);
      }
      else if (arg == "vertical"   || arg == "vert" ||
               arg == "horizontal" || arg == "horiz") {
        keyData_.setVertical(arg == "vertical" || arg == "vert");
      }
      else if (arg == "under") {
        keyData_.setUnder(true);
      }
      else if (arg == "Left" || arg == "Right") {
        keyData_.setJustify(arg == "Left" ? CHALIGN_TYPE_LEFT : CHALIGN_TYPE_RIGHT);
      }
      else if (arg == "opaque" || arg == "noopaque") {
        keyData_.setOpaque(arg == "opaque");
      }
      else if (arg == "reverse" || arg == "noreverse") {
        keyData_.setReverse(arg == "reverse");
      }
      else if (arg == "invert" || arg == "noinvert") {
        keyData_.setInvert(arg == "invert");
      }
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
        int i;

        if (parseInteger(line, i))
          keyData_.setWidthIncrement(i);
      }
      else if (arg == "height") {
        int i;

        if (parseInteger(line, i))
          keyData_.setHeightIncrement(i);
      }
      else if (arg == "autotitle") {
        keyData_.setAutotitle(true);

        std::string arg1 = readNonSpace(line);

        if (arg1 == "columnhead" || arg1 == "columnheader")
          keyData_.setColumnHead(true);
      }
      else if (arg == "noautotitle") {
        keyData_.setAutotitle(false);

        //std::string headerStr = readNonSpace(line);

        keyData_.setColumnHead(false);
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
      else if (arg == "box") {
        keyData_.setBox(true);

        int pos = line.pos();

        std::string arg = readNonSpace(line);

        if      (arg == "linetype" || arg == "lt") {
          int lt;

          if (parseInteger(line, lt))
            keyData_.setBoxLineType(lt);
        }
        else if (arg == "linestyle" || arg == "ls") {
          int ls;

          if (parseInteger(line, ls))
            keyData_.setBoxLineStyle(ls);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            keyData_.setBoxLineWidth(lw);
        }
        else
          line.setPos(pos);
      }
      else if (arg == "nobox") {
        keyData_.setBox(false);
      }
      else if (arg == "above" || arg == "below") {
        keyData_.setBelow(arg == "below");
      }
      else if (arg == "maxcols") {
        int n = 0;

        line.skipSpace();

        if      (line.isString("auto")) {
          line.skipNonSpace();

          keyData_.resetMaxCols();
        }
        else if (parseInteger(line, n))
          keyData_.setMaxCols(n);
      }
      else if (arg == "maxrows") {
        int n = 0;

        line.skipSpace();

        if      (line.isString("auto")) {
          line.skipNonSpace();

          keyData_.resetMaxRows();
        }
        else if (parseInteger(line, n))
          keyData_.setMaxRows(n);
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
  //           {boxed}
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
    StringArray extraArgs;

    line.skipSpace();

    if (line.isChar('"') || line.isChar('\'')) {
      isStr = parseString(line, arg);
    }
    else {
      int pos = line.pos();

      arg   = readNonSpace(line);
      isStr = false;

      auto p = arg.find('(');

      if (p != std::string::npos) {
        line.setPos(pos);

        isStr = parseString(line, arg);
      }
    }

    while (arg != "") {
      if      (isStr) {
        label->setText(arg);
      }
      else if (arg == "at") {
        CGnuPlotPosition p;

        if (parsePosition(line, p))
          label->setPos(p);
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
        if      (arg == "front" ) label->setLayer(DrawLayer::FRONT);
        else if (arg == "back"  ) label->setLayer(DrawLayer::BACK);
        else if (arg == "behind") label->setLayer(DrawLayer::BEHIND);
      }
      else if (arg == "textcolor" || arg == "tc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          label->setTextColor(c);
      }
      else if (arg == "point") {
        label->setShowPoint(true);

        int pos = line.pos();

        std::string arg1 = readNonSpace(line);

        if      (arg1 == "linetype" || arg1 == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            label->setLineType(lt);
        }
        else if (arg1 == "pointtype" || arg1 == "pt") {
          int pt = 0;

          if (parseInteger(line, pt))
            label->setPointType(pt);
        }
        else if (arg1 == "pointsize" || arg1 == "ps") {
          double ps = 0;

          if (parseReal(line, ps))
            label->setPointSize(ps);
        }
        else {
          line.setPos(pos);
        }
      }
      else if (arg == "nopoint") {
        label->setShowPoint(false);
      }
      else if (arg == "offset") {
        CPoint2D o(0, 0);

        if (parsePoint(line, o))
          label->setOffset(o);
      }
      else if (arg == "boxed") {
        label->setBox(true);
      }
      else if (arg == "hypertext") {
        label->setHypertext(true);
      }
      else
        extraArgs.push_back(arg);

      line.skipSpace();

      if (line.isChar('"') || line.isChar('\'')) {
        isStr = parseString(line, arg);
      }
      else {
        int pos = line.pos();

        arg   = readNonSpace(line);
        isStr = false;

        auto p = arg.find('(');

        if (p != std::string::npos) {
          line.setPos(pos);

          isStr = parseString(line, arg);
        }
      }
    }

    if (! extraArgs.empty()) {
      std::string arg = extraArgs[0];

      std::string str;

      if (getStringVariable(arg, str))
        label->setText(str);
    }
  }
  // set linetype {index} {{linetype  | lt ] <linetype> | <colorspec>}
  //                      {{linecolor | lc } <colorspace>}
  //                      {{linewidth | lw } <linewidth>}
  //                      {{pointtype | pt } <pointtype>}
  //                      {{pointsize | ps } <pointsize>}
  //                      {{pointinterval | pi } <interval>}
  // set linetype cycle <n>
  else if (var == VariableName::LINETYPE) {
    line.skipSpace();

    if (line.isString("cycle")) {
      line.skipNonSpace();

      int n = 0;

      if (parseInteger(line, n))
        styleIncrement_.increment = n;
    }
    else {
      int ind = 0;

      if (! parseInteger(line, ind))
        ind = 0;

      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      line.skipSpace();

      while (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "linetype" || arg == "lt") {
          int lt;

          if (parseInteger(line, lt))
            lineStyle->setType(lt);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            lineStyle->setColor(c);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            lineStyle->setWidth(lw);
        }
        else if (arg == "pointtype" || arg == "pt") {
          int pt;

          if (parseInteger(line, pt))
            lineStyle->setPointType(static_cast<CGnuPlotTypes::SymbolType>(pt));
        }
        else if (arg == "pointsize" || arg == "ps") {
          double ps;

          if (parseReal(line, ps))
            lineStyle->setPointSize(ps);
        }
        else if (arg == "pointinterval" || arg == "pi") {
          int pi;

          if (parseInteger(line, pi))
            lineStyle->setPointInterval(pi);
        }
        else {
          std::cerr << "Invalid line modifier '" << arg << "'" << std::endl;
          break;
        }

        line.skipSpace();
      }

      line.skipSpace();
    }
  }
  // set link {x2 | y2} {via <expression1> inverse <expression2>}
  else if (var == VariableName::LINK) {
    std::string arg = readNonSpace(line);

    if (arg == "x2" || arg == "y2") {
      std::string expr, iexpr;

      std::string arg1 = readNonSpace(line);

      if (arg1 == "via") {
        expr = readNonSpace(line);

        std::string arg2 = readNonSpace(line);

        if (arg2 == "inverse") {
          iexpr = readNonSpace(line);
        }
      }

      if (arg == "x2") {
        linkData_.linkX2.enabled = true;
        linkData_.linkX2.expr    = expr;
        linkData_.linkX2.iexpr   = iexpr;
      }
      else {
        linkData_.linkY2.enabled = true;
        linkData_.linkY2.expr    = expr;
        linkData_.linkY2.iexpr   = iexpr;
      }
    }
  }
  // set lmargin {at screen} <val>
  else if (var == VariableName::LMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isChars("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setLeft(r, screen);
  }
  // set loadpath {"<path>"}
  else if (var == VariableName::LOADPATH) {
    std::string path;

    while (parseString(line, path))
      loadPaths_.push_back(path);
  }
  // set locale {"<locale>"}
  else if (var == VariableName::LOCALE) {
    std::string str;

    if (parseString(line, str))
      locale_ = str;
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
  // set macros
  else if (var == VariableName::MACROS) {
    setMacros(true);
  }
  // set mapping [ cartesian | cylindrical | spherical ]
  else if (var == VariableName::MAPPING) {
    std::string arg = readNonSpace(line);

    if      (arg == "cartesian")
      mapping_ = CGnuPlotTypes::Mapping::CARTESIAN_MAPPING;
    else if (arg == "cylindrical")
      mapping_ = CGnuPlotTypes::Mapping::CYLINDRICAL_MAPPING;
    else if (arg == "spherical")
      mapping_ = CGnuPlotTypes::Mapping::SPHERICAL_MAPPING;
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
  // set mouse {doubleclick <ms>} {nodoubleclick}
  //           {{no}zoomcoordinates}
  //           {zoomfactors <xmultiplier>, <ymultiplier>}
  //           {noruler | ruler {at x,y}}
  //           {polardistance{deg|tan} | nopolardistance}
  //           {format <string>}
  //           {mouseformat <int>/<string>}
  //           {{no}labels {"labeloptions"}}
  //           {{no}zoomjump} {{no}verbose}
  else if (var == VariableName::MOUSE) {
    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "doubleclick") {
        int ms;

        if (parseInteger(line, ms))
          mouseData_.setDoubleClick(ms);
      }
      else if (arg == "nodoubleclick") {
        mouseData_.resetDoubleClick();
      }
      else if (arg == "zoomcoordinates" || arg == "nozoomcoordinates") {
        mouseData_.setZoomCoordinates(arg == "zoomcoordinates");
      }
      else if (arg == "zoomfactors") {
        double x, y;

        if (parseReal(line, x)) {
          if (line.skipSpaceAndChar(',')) {
            if (parseReal(line, y))
              mouseData_.setZoomFactors(x, y);
          }
        }
      }
      else if (arg == "ruler") {
        int pos = line.pos();

        arg = readNonSpace(line);

        if (arg == "at") {
          double x, y;

          if (parseReal(line, x) && parseReal(line, y))
            mouseData_.setRulerPos(CPoint2D(x, y));
        }
        else
          line.setPos(pos);
      }
      else if (arg == "noruler") {
        mouseData_.resetRulerPos();
      }
      else if (arg == "polardistancedeg") {
        mouseData_.setPolarDisance(CGnuPlotMouseData::PolarDistanceType::DEG);
      }
      else if (arg == "polardistancetan") {
        mouseData_.setPolarDisance(CGnuPlotMouseData::PolarDistanceType::TAN);
      }
      else if (arg == "nopolardistance") {
        mouseData_.setPolarDisance(CGnuPlotMouseData::PolarDistanceType::NONE);
      }
      else if (arg == "format") {
        std::string str;

        if (parseString(line, str))
          mouseData_.setFormat(str);
      }
      else if (arg == "mouseformat") {
        line.skipSpace();

        if (line.isChar('"')) {
          std::string str;

          if (parseString(line, str))
            mouseData_.setMouseFormat(str);
        }
        else {
          int i;

          if (parseInteger(line, i))
            mouseData_.setMouseFormat(i);
        }
      }
      else if (arg == "labels") {
        std::string str;

        if (parseString(line, str))
          mouseData_.setLabels(str);
      }
      else if (arg == "nolabels") {
        mouseData_.resetLabels();
      }
      else if (arg == "zoomjump" || arg == "nozoomjump") {
        mouseData_.setZoomJump(arg == "zoomjump");
      }
      else if (arg == "verbose" || arg == "noverbose") {
        mouseData_.setVerbose(arg == "verbose");
      }

      arg = readNonSpace(line);
    }
  }
  // set multiplot { title <page title> {font <fontspec>} {enhanced|noenhanced} }
  //               { layout <rows>,<cols> {rowsfirst|columnsfirst} {downwards|upwards}
  //                 {scale <xscale>{,<yscale>}} {offset <xoff>{,<yoff>}} }
  else if (var == VariableName::MULTIPLOT) {
    multiplot_.setEnabled(true);
    multiplot_.setAutoFit(true);

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
        multiplot_.setEnhanced(arg == "enhanced");
      }
      else if (arg == "layout") {
        int i;

        if (parseInteger(line, i))
          multiplot_.setRows(i);

        if (line.skipSpaceAndChar(',')) {
          if (parseInteger(line, i))
            multiplot_.setCols(i);
        }

        multiplot_.setAutoFit(false);
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
  // set mx2tics {<freq> | default}
  else if (var == VariableName::MX2TICS) {
    axesData_.xaxis[2].setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      axesData_.xaxis[2].resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        axesData_.xaxis[2].setMinorTicsFreq(r);
    }
  }
  // set mxtics {<freq> | default}
  else if (var == VariableName::MXTICS) {
    axesData_.xaxis[1].setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      axesData_.xaxis[1].resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        axesData_.xaxis[1].setMinorTicsFreq(r);
    }
  }
  // set my2tics {<freq> | default}
  else if (var == VariableName::MY2TICS) {
    axesData_.yaxis[2].setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      axesData_.yaxis[2].resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        axesData_.yaxis[2].setMinorTicsFreq(r);
    }
  }
  // set mytics {<freq> | default}
  else if (var == VariableName::MYTICS) {
    axesData_.yaxis[1].setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      axesData_.yaxis[1].resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        axesData_.yaxis[1].setMinorTicsFreq(r);
    }
  }
  // set mztics {<freq> | default}
  else if (var == VariableName::MZTICS) {
    axesData_.zaxis[1].setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      axesData_.zaxis[1].resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        axesData_.zaxis[1].setMinorTicsFreq(r);
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
      auto circle = lookupAnnotation<CGnuPlotCircle>(ind);
      if (! circle) return false;

      //---

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "at" || arg == "center") {
          CGnuPlotPosition p;

          if (parsePosition(line, p))
            circle->setCenter(p);
        }
        else if (arg == "size") {
          CGnuPlotCoordValue r;

          if (parseCoordValue(line, r))
            circle->setRadius(r);
        }
        else if (arg == "arc") {
          if (line.skipSpaceAndChar('[')) {
            std::string arg;

            while (line.isValid() && ! line.isChar(']'))
              arg += line.getChar();

            if (line.isChar(']'))
              line.skipChar();

            StringArray strs;

            CStrUtil::addFields(arg, strs, ":");

            double r;

            if (strs.size() > 0 && CStrUtil::toReal(strs[0], &r))
              circle->setArcStart(r);

            if (strs.size() > 1 && CStrUtil::toReal(strs[1], &r))
              circle->setArcEnd(r);
          }
        }
        else if (arg == "fillcolor" || arg == "fc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            circle->setFillColor(c);
        }
        else if (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) circle->setLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) circle->setLayer(DrawLayer::BACK);
          else if (arg == "behind") circle->setLayer(DrawLayer::BEHIND);
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

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "at" || arg == "center") {
          CGnuPlotPosition p;

          if (parsePosition(line, p))
            ellipse->setCenter(p);
        }
        else if (arg == "size") {
          CSize2D size(1,1);

          if (! parseSize(line, size))
            continue;

          ellipse->setRX(size.width /2.0);
          ellipse->setRY(size.height/2.0);
        }
        else if (arg == "angle") {
          double a;

          if (parseReal(line, a))
            ellipse->setAngle(a);
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
          if (parsePosition(line, from))
            poly->addPoint(from.point());
        }
        else if (arg == "to") {
          if (parsePosition(line, to))
            poly->addPoint(to.point());
        }
        else if (arg == "fillcolor" || arg == "fc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            poly->setFillColor(c);
        }
        else if (arg == "fillstyle" || arg == "fs") {
          CGnuPlotFillStyle fs;

          if (parseFillStyle(line, fs))
            poly->setFillStyle(fs);
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            poly->setLineType(lt);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            poly->setLineWidth(lw);
        }
        else if (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) poly->setLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) poly->setLayer(DrawLayer::BACK);
          else if (arg == "behind") poly->setLayer(DrawLayer::BEHIND);
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
          CGnuPlotSize size;

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
          if      (arg == "front" ) rect->setLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) rect->setLayer(DrawLayer::BACK);
          else if (arg == "behind") rect->setLayer(DrawLayer::BEHIND);
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
  // set offsets <left>, <right>, <top>, <bottom>
  else if (var == VariableName::OFFSETS) {
    CGnuPlotCoordValue l, r, t, b;

    if (parseCoordValue(line, l)) {
      if (line.skipSpaceAndChar(',') && parseCoordValue(line, r)) {
        if (line.skipSpaceAndChar(',') && parseCoordValue(line, t)) {
          if (line.skipSpaceAndChar(',') && parseCoordValue(line, b)) {
            offsets_.set(l, r, t, b);
          }
        }
      }
    }
  }
  // set origin <x-origin>,<y-origin>
  else if (var == VariableName::ORIGIN) {
    double r;

    if (parseReal(line, r))
      plotSize_.xo = r;

    if (line.skipSpaceAndChar(',')) {
      if (parseReal(line, r))
        plotSize_.yo = r;
    }
  }
  // set output ["<filename>"]
  else if (var == VariableName::OUTPUT) {
    std::string filename;

    if (parseString(line, filename, "Invalid filename"))
      setOutputFile(filename);
  }
  // set parametric
  else if (var == VariableName::PARAMETRIC) {
    parametric_ = true;
  }
  // set paxis <no> {range <range-options> | {tics <tic-options>}}
  else if (var == VariableName::PAXIS) {
    int i;

    if (parseInteger(line, i)) {
      axesData_.paxis[i].setDisplayed(true);

      std::string arg = readNonSpace(line);

      if      (arg == "range")
        parseAxisRange(line, axesData_.paxis[i]);
      else if (arg == "tics")
        parseAxesTics(line, axesData_.paxis[i]);
    }
  }
  // set plot (invalid)
  else if (var == VariableName::PLOT) {
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
    pm3D_.setEnabled(true);

    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "at") {
        pm3D_.pos.clear();

        std::string arg1 = readNonSpaceNonComma(line);

        for (uint i = 0; i < arg1.size(); ++i) {
          if      (arg1[i] == 's') pm3D_.pos.push_back(Pm3DData::PosType::SURFACE);
          else if (arg1[i] == 't') pm3D_.pos.push_back(Pm3DData::PosType::TOP);
          else if (arg1[i] == 'b') pm3D_.pos.push_back(Pm3DData::PosType::BOTTOM);
        }
      }
      else if (arg == "interpolate") {
        int steps1, steps2;

        if (parseInteger(line, steps1)) {
          if (line.skipSpaceAndChar(',')) {
            if (parseInteger(line, steps2)) {
              pm3D_.isteps1 = steps1;
              pm3D_.isteps2 = steps2;
            }
          }
        }
      }
      else if (arg == "scansautomatic") {
        pm3D_.scanType = Pm3DData::ScanType::AUTOMATIC;
      }
      else if (arg == "scansforward") {
        pm3D_.scanType = Pm3DData::ScanType::FORWARD;
      }
      else if (arg == "scansbackward") {
        pm3D_.scanType = Pm3DData::ScanType::BACKWARD;
      }
      else if (arg == "depthorder") {
        pm3D_.scanType = Pm3DData::ScanType::DEPTH_ORDER;
      }
      else if (arg == "flush") {
        std::string arg1 = readNonSpaceNonComma(line);

        if      (arg1 == "begin" ) { pm3D_.flushType = Pm3DData::FlushType::BEGIN; }
        else if (arg1 == "center") { pm3D_.flushType = Pm3DData::FlushType::CENTER; }
        else if (arg1 == "end"   ) { pm3D_.flushType = Pm3DData::FlushType::END; }
      }
      else if (arg == "ftriangles" || arg == "noftriangles") {
        pm3D_.ftriangles = (arg == "ftriangles");
      }
      else if (arg == "clip1in" || arg == "clip4in") {
        pm3D_.clipin = (arg == "clip1in" ? 1 : 4);
      }
      else if (arg == "corners2color") {
        std::string arg1 = readNonSpaceNonComma(line);

        if      (arg1 == "mean"   ) { pm3D_.cornerType = Pm3DData::CornerType::MEAN; }
        else if (arg1 == "geomean") { pm3D_.cornerType = Pm3DData::CornerType::GEOMEAN; }
        else if (arg1 == "harmean") { pm3D_.cornerType = Pm3DData::CornerType::HARMEAN; }
        else if (arg1 == "rms"    ) { pm3D_.cornerType = Pm3DData::CornerType::RMS; }
        else if (arg1 == "median" ) { pm3D_.cornerType = Pm3DData::CornerType::MEDIAN; }
        else if (arg1 == "min"    ) { pm3D_.cornerType = Pm3DData::CornerType::MIN; }
        else if (arg1 == "max"    ) { pm3D_.cornerType = Pm3DData::CornerType::MAX; }
        else if (arg1 == "c1"     ) { pm3D_.cornerType = Pm3DData::CornerType::C1; }
        else if (arg1 == "c2"     ) { pm3D_.cornerType = Pm3DData::CornerType::C2; }
        else if (arg1 == "c3"     ) { pm3D_.cornerType = Pm3DData::CornerType::C3; }
        else if (arg1 == "c4"     ) { pm3D_.cornerType = Pm3DData::CornerType::C4; }
      }
      else if (arg == "hidden3d") {
        int lt;

        if (parseInteger(line, lt))
          pm3D_.linetype = lt;
      }
      else if (arg == "nohidden3d") {
        pm3D_.linetype = -1;
      }
      else if (arg == "implicit" || arg == "explicit") {
        pm3D_.implicit = (arg == "implicit");
      }
      else if (arg == "map") {
        //set pm3d at b; set view map scale 1.0; set style data pm3d; set style func pm3d;
        pm3D_.pos.clear();
        pm3D_.pos.push_back(Pm3DData::PosType::SURFACE);
      }

      arg = readNonSpaceNonComma(line);
    }
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
        palette_.setGray(false);

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
        palette_.setGray(false);

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
        palette_.setGray(false);

        std::string filename;

        if (parseString(line, filename))
          palette_.readFile(filename);
      }
      else if (arg == "functions" || arg == "func") {
        palette_.setGray(false);

        std::string rf, gf, bf;

        rf = readNonSpaceNonComma(line);

        if (line.skipSpaceAndChar(','))
          gf = readNonSpaceNonComma(line);

        if (line.skipSpaceAndChar(','))
          bf = readNonSpaceNonComma(line);

        palette_.setFunctions(rf, gf, bf);
      }
      else if (arg == "cubehelix") {
        int pos = line.pos();

        std::string arg1 = readNonSpace(line);

        int    start = 0, cycles = 1;
        double saturation = 1.0;

        if (arg1 == "start") {
          int i;

          if (parseInteger(line, i))
            start = i;

          pos = line.pos();

          arg1 = readNonSpace(line);

          if (arg1 == "cycles") {
            int i;

            if (parseInteger(line, i))
              cycles = i;

            pos = line.pos();

            arg1 = readNonSpace(line);

            if (arg1 == "saturation") {
              double r;

              if (parseReal(line, r))
                saturation = r;
            }
            else
              line.setPos(pos);
          }
          else
            line.setPos(pos);
        }
        else
          line.setPos(pos);

        palette_.setCubeHelix(start, cycles, saturation);
      }
      else if (arg == "model" || arg == "mode") {
        std::string arg1 = readNonSpace(line);

        if      (arg1 == "RGB") palette_.setColorModel(CGnuPlotTypes::ColorModel::RGB);
        else if (arg1 == "HSV") palette_.setColorModel(CGnuPlotTypes::ColorModel::HSV);
        else if (arg1 == "CMY") palette_.setColorModel(CGnuPlotTypes::ColorModel::CMY);
        else if (arg1 == "YIQ") palette_.setColorModel(CGnuPlotTypes::ColorModel::YIQ);
        else if (arg1 == "XYZ") palette_.setColorModel(CGnuPlotTypes::ColorModel::XYZ);
      }
      else if (arg == "positive" || arg == "negative") {
        palette_.setNegative(arg == "negative");
      }
      else if (arg == "nops_allcF" || arg == "ps_allcF") {
        palette_.setPSAllCF(arg == "ps_allcF");
      }
      else if (arg == "maxcolors") {
        int n;

        if (parseInteger(line, n))
          palette_.setMaxColors(n);
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
        break;
      }

      arg = readNonSpace(line);
    }
  }
  // set pointintervalbox <n>
  else if (var == VariableName::POINTINTERVALBOX) {
    double r;

    if (parseReal(line, r))
      pointIntervalBox_ = r;
  }
  // set pointsize <size>
  else if (var == VariableName::POINTSIZE) {
    double r = 1.0;

    if (parseReal(line, r))
      lineStyle()->setPointSize(r > 0 ? r : 1.0);
  }
  // set polar
  else if (var == VariableName::POLAR) {
    polar_ = true;
  }
  // set print ["<filename>"] [append]
  else if (var == VariableName::PRINT) {
    std::string filename;

    line.skipSpace();

    if      (line.isChar('$')) {
      line.skipChar();

      std::string block = readNonSpace(line);

      setPrintDataBlock(block);
    }
    else if (parseString(line, filename, "Invalid filename")) {
      if (filename == "-")
        setPrintStdOut();
      else
        setPrintFile(filename);

      std::string arg = readNonSpace(line);

      if (arg == "append")
        setPrintAppend(true);
    }
  }
  // set psdir <dir>
  else if (var == VariableName::PSDIR) {
    std::string dirname;

    if (parseString(line, dirname, "Invalid dirname"))
      psDir_ = dirname;
  }
  // set raxis
  else if (var == VariableName::RAXIS) {
    axesData_.raxis.setDisplayed(true);
  }
  // set rmargin {at screen} <val>
  else if (var == VariableName::RMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isChars("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setRight(r, screen);
  }
  // set rrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::RRANGE) {
    parseAxisRange(line, axesData_.raxis);
  }
  // set rtics
  else if (var == VariableName::RTICS) {
    parseAxesTics(line, axesData_.raxis);
  }
  // set samples {samples1:i} [, {samples2:i} ]
  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    samples_.get(isamples1, isamples2);

    int i;

    if (parseInteger(line, i))
      isamples1 = i;

    if (line.skipSpaceAndChar(',')) {
      if (parseInteger(line, i))
        isamples2 = i;
    }

    samples_.set(isamples1, isamples2);
  }
  // set size {{no}square | ratio <r> | noratio} {<xscale>,<yscale>}
  else if (var == VariableName::SIZE) {
    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "square" || arg == "nosquare") {
        plotSize_.square = (arg == "square");
      }
      else if (arg == "ratio") {
        plotSize_.square = false;

        double r;

        if (parseReal(line, r))
          plotSize_.xratio = r;

        if (parseReal(line, r))
          plotSize_.yratio = r;
      }
      else if (arg == "noratio") {
        plotSize_.square = false;

        plotSize_.xratio.setInvalid();
        plotSize_.yratio.setInvalid();
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

        if (multiplot_.isEnabled())
          multiplot_.setAutoFit(false);
      }
      else {
        std::cerr << "Bad arg \'" << arg << "\'" << std::endl;
        break;
      }

      arg = readNonSpaceNonComma(line);
    }
  }
  // set style [data|function|increment] <args>
  else if (var == VariableName::STYLE) {
    StyleVar var1;

    if (! parseOptionValue(this, line, var1, "style type"))
      return false;

    line.skipSpace();

    // set style arrow {index:i} default
    // set style arrow {index:i} {nohead | head | backhead | heads}
    //                           {size <length>,<angle> {,<backangle>} {fixed}}
    //                           {filled | empty | nofilled | noborder}
    //                           {front | back}
    //                           {{linetype|lt <type>} |
    //                            {linewidth|lw <type>} |
    //                            {linestyle|ls <style>}}
    if      (var1 == StyleVar::ARROW) {
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
        // default
        if      (arg == "default") {
          *arrowStyle1 = CGnuPlotArrowStyle();
        }
        // {nohead | head | backhead | heads}
        else if (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
          if      (arg == "nohead"  ) { arrowStyle1->setHeads(false, false); }
          else if (arg == "head"    ) { arrowStyle1->setHeads(false, true ); }
          else if (arg == "backhead") { arrowStyle1->setHeads(true , false); }
          else if (arg == "heads"   ) { arrowStyle1->setHeads(true , true ); }
        }
        // {size <length>,<angle> {,<backangle>} {fixed}}
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

          line.skipSpace();

          if (line.isString("fixed")) {
            line.skipNonSpace();

            arrowStyle1->setFixed(true);
          }
        }
        // filled | nofilled
        else if (arg == "filled" || arg == "nofilled") {
          arrowStyle1->setFilled(arg == "filled");
        }
        // empty
        else if (arg == "empty") {
          arrowStyle1->setEmpty(true);
        }
        // noborder
        else if (arg == "noborder") {
          arrowStyle1->setBorder(false);
        }
        // front | back
        else if (arg == "front" || arg == "back") {
          arrowStyle1->setFront(arg == "front");
        }
        // linetype|lt <type>
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseInteger(line, lt))
            arrowStyle1->setLineStyle(lt);
        }
        // linewidth|lw <type>
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            arrowStyle1->setLineWidth(lw);
        }
        // linestyle|ls <style>
        else if (arg == "linestyle" || arg == "ls") {
          int ls = -1;

          if (parseInteger(line, ls))
            arrowStyle1->setLineStyle(ls);
        }
        // linecolor|lc <color>
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
    // set style boxplot {range <r> | fraction <f>}
    //                   {{no}outliers} {pointtype <p>}
    //                   {candlesticks | financebars}
    //                   {separation <x>}
    //                   {labels off | auto | x | x2}
    //                   {sorted | unsorted}
    else if (var1 == StyleVar::BOXPLOT) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        // range <r>
        if      (arg == "range") {
          double r;

          if (parseReal(line, r))
            boxPlot_.setRange(r);
        }
        // fraction <f>
        else if (arg == "fraction") {
          double r;

          if (parseReal(line, r))
            boxPlot_.setFraction(r);
        }
        // {no}outliers}
        else if (arg == "outliers" || arg == "nooutliers") {
          boxPlot_.setOutliers(arg == "outliers");
        }
        // {pointtype <p>
        else if (arg == "pointtype") {
          int pt;

          if (parseInteger(line, pt))
            boxPlot_.setPointType(pt);
        }
        // candlesticks
        else if (arg == "candlesticks") {
          boxPlot_.setType(CGnuPlotBoxPlot::Type::CandleSticks);
        }
        // financebars
        else if (arg == "financebars") {
          boxPlot_.setType(CGnuPlotBoxPlot::Type::FinanceBars);
        }
        // separation <x>
        else if (arg == "separation") {
          double r;

          if (parseReal(line, r))
            boxPlot_.setSeparation(r);
        }
        // labels off | auto | x | x2
        else if (arg == "labels") {
          arg = readNonSpace(line);

          if      (arg == "off" ) boxPlot_.setLabels(CGnuPlotBoxPlot::Labels::Off);
          else if (arg == "auto") boxPlot_.setLabels(CGnuPlotBoxPlot::Labels::Auto);
          else if (arg == "x"   ) boxPlot_.setLabels(CGnuPlotBoxPlot::Labels::X);
          else if (arg == "x2"  ) boxPlot_.setLabels(CGnuPlotBoxPlot::Labels::X2);
        }
        // sorted | unsorted
        else if (arg == "sorted" || arg == "unsorted") {
          boxPlot_.setSorted(arg == "sorted");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style data {style}
    else if (var1 == StyleVar::DATA) {
      PlotStyle style(PlotStyle::NONE);

      if (parseOptionValue(this, line, style, "plot style"))
        setDataStyle(style);
    }
    // set style fill {empty |
    //                 {transparent} solid {density} |
    //                 {transparent} pattern {id}}
    //                {border {lt} {lc <colorspec>} | noborder}
    else if (var1 == StyleVar::FILL) {
      CGnuPlotFillStyle fillStyle = fillStyle_;

      if (parseFillStyle(line, fillStyle))
        fillStyle_ = fillStyle;
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

      if (styleStr == "user" || styleStr == "userstyles") {
        setStyleIncrementType(StyleIncrementType::USER);

        setLineStyleInd(1);
      }
      else {
        setStyleIncrementType(StyleIncrementType::DEFAULT);

        setLineStyleInd(0);
      }
    }
    // set style line {index} default
    // set style line {index} {{linetype  | lt } <linetype> | <colorspec>}
    //                        {{linecolor | lc } <colorspace>}
    //                        {{linewidth | lw } <linewidth>}
    //                        {{pointtype | pt } <pointtype>}
    //                        {{pointsize | ps } <pointsize>}
    //                        {{pointinterval | pi } <interval>}
    //                        {palette}
    else if (var1 == StyleVar::LINE) {
      int ind = 0;

      if (! parseInteger(line, ind))
        ind = 0;

      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      line.skipSpace();

      while (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "linetype" || arg == "lt") {
          int lt;

          if (parseInteger(line, lt))
            lineStyle->setType(lt);
        }
        else if (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            lineStyle->setColor(c);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            lineStyle->setWidth(lw);
        }
        else if (arg == "pointtype" || arg == "pt") {
          int pt;

          if (parseInteger(line, pt))
            lineStyle->setPointType(static_cast<CGnuPlotTypes::SymbolType>(pt));
        }
        else if (arg == "pointsize" || arg == "ps") {
          double ps;

          if (parseReal(line, ps))
            lineStyle->setPointSize(ps);
        }
        else if (arg == "pointinterval" || arg == "pi") {
          double pi;

          if (parseReal(line, pi))
            lineStyle->setPointInterval(pi);
        }
        else if (arg == "palette") {
          lineStyle->setPalette(true);
        }
        else if (arg == "default") {
          resetLineStyleInd(ind);
        }
        else {
          std::cerr << "Invalid line modifier '" << arg << "'" << std::endl;
          break;
        }

        line.skipSpace();
      }

      line.skipSpace();
    }
    // set style circle {radius {graph|screen} <R>}
    //                  {{no}wedge}
    //                  {clip|noclip}
    else if (var1 == StyleVar::CIRCLE) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "radius") {
          for (int i = 0; i < 3; ++i) {
            line.skipSpace();

            CGnuPlotTypes::CoordSys coordSys = CGnuPlotTypes::CoordSys::FIRST;

            if (line.isString("screen") || line.isString("graph")) {
              if      (line.isString("screen"))
                coordSys = CGnuPlotTypes::CoordSys::SCREEN;
              else if (line.isString("graph"))
                coordSys = CGnuPlotTypes::CoordSys::GRAPH;

              line.skipNonSpace();
            }

            double r;

            if (! parseReal(line, r))
              continue;

            CGnuPlotCoordValue c(r, coordSys);

            circleStyle_.setRadius(i, c);

            if (! line.skipSpaceAndChar(','))
              break;
          }
        }
        else if (arg == "wedge" || arg == "nowedge") {
          circleStyle_.setWedge(arg == "wedge");
        }
        else if (arg == "clip" || arg == "noclip") {
          circleStyle_.setClip(arg == "wedge");
        }

        arg = readNonSpace(line);
      }
    }
    // set style rectangle {front|back} {lw|linewidth <lw>}
    //                     {fillcolor <colorspec>} {fs <fillstyle>}
    else if (var1 == StyleVar::RECTANGLE) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        // front | back
        if (arg == "front" || arg == "back") {
          rectStyle_.setFront(arg == "front");
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            rectStyle_.setLineWidth(lw);
        }
        else if (arg == "fillcolor" || arg == "fc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            rectStyle_.setFillColor(c);
        }
        else if (arg == "fillstyle" || arg == "fs") {
          CGnuPlotFillStyle fs;

          if (parseFillStyle(line, fs))
            rectStyle_.setFillStyle(fs);
        }

        arg = readNonSpace(line);
      }
    }
    // set style ellipse {units xx|xy|yy}
    //                   {size {graph|screen} <a>, {{graph|screen} <b>}}
    //                   {angle <angle>}
    //                   {clip|noclip}
    else if (var1 == StyleVar::ELLIPSE) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "units") {
          arg = readNonSpace(line);

          if      (arg == "xx")
            ellipseStyle_.setUnits(CGnuPlotEllipseStyle::Units::XX);
          else if (arg == "xy")
            ellipseStyle_.setUnits(CGnuPlotEllipseStyle::Units::XY);
          else if (arg == "yy")
            ellipseStyle_.setUnits(CGnuPlotEllipseStyle::Units::YY);
        }
        else if (arg == "size") {
          CGnuPlotTypes::CoordSys coordSys = CGnuPlotTypes::CoordSys::FIRST;

          line.skipSpace();

          if (line.isString("graph") || line.isString("screen")) {
            if (line.isString("graph"))
              coordSys = CGnuPlotTypes::CoordSys::GRAPH;
            else
              coordSys = CGnuPlotTypes::CoordSys::SCREEN;

            arg = readNonSpace(line);
          }

          double sa;

          if (parseReal(line, sa)) {
            CGnuPlotCoordValue sac(sa, coordSys);

            ellipseStyle_.setSize(0, sac);
          }

          if (line.skipSpaceAndChar(',')) {
            CGnuPlotTypes::CoordSys coordSys = CGnuPlotTypes::CoordSys::FIRST;

            line.skipSpace();

            if (line.isString("graph") || line.isString("screen")) {
              if (line.isString("graph"))
                coordSys = CGnuPlotTypes::CoordSys::GRAPH;
              else
                coordSys = CGnuPlotTypes::CoordSys::SCREEN;

              arg = readNonSpace(line);
            }

            double sb;

            if (parseReal(line, sb)) {
              CGnuPlotCoordValue sbc(sb, coordSys);

              ellipseStyle_.setSize(1, sbc);
            }
          }
        }
        else if (arg == "angle") {
          double a;

          if (parseReal(line, a))
            ellipseStyle_.setAngle(a);
        }
        else if (arg == "clip" || arg == "noclip") {
          ellipseStyle_.setClip(arg == "wedge");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style textbox {opaque|transparent}{{no}border}
    else if (var1 == StyleVar::TEXTBOX) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "opaque" || arg == "transparent") {
          textBoxStyle_.setTransparent(arg == "transparent");
        }
        else if (arg == "border" || arg == "noborder") {
          textBoxStyle_.setBorder(arg == "border");
        }

        arg = readNonSpace(line);
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
  }
  // set surface
  else if (var == VariableName::SURFACE) {
    setSurface3D(true);
  }
  // set table ["<filename>"]
  else if (var == VariableName::TABLE) {
    std::string filename;

    if (parseString(line, filename, "Invalid filename"))
      setTableFile(filename);
  }
  // set terminal <type> [<args>]
  else if (var == VariableName::TERMINAL) {
    std::string arg = readNonSpace(line);

    if      (arg == "push")
      pushDevice();
    else if (arg == "pop")
      popDevice();
    else {
      if (! setDevice(arg))
        std::cerr << "No device " << arg << std::endl;

      if (! device())
        return false;

      int pos = line.pos();

      arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "size") {
          int w, h;

          if (parseInteger(line, w)) {
            if (line.skipSpaceAndChar(',')) {
              if (parseInteger(line, h))
                device()->setSize(CISize2D(w, h));
            }
          }
        }
        else if (arg == "background") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            setBackgroundColor(c.color());
        }
        else if (arg == "enhanced" || arg == "noenhanced") {
          device()->setEnhanced(arg == "enhanced");
        }
        else {
          line.setPos(pos);

          if (! device()->parseArgs(line))
            errorMsg("Invalid arg '" + arg + "'");
        }

        arg = readNonSpace(line);
      }
    }
  }
  // set termoption [no]enhanced
  // set termoption font "<name> [, <size:i> ]"
  // set termoption fontscale <scale>
  // set termoption {solid|dashed}
  // set termoption {linewidth <lw>}{lw <lw>}
  else if (var == VariableName::TERMOPTION) {
    if (! device())
      return false;

    std::string arg = readNonSpace(line);

    if      (arg == "enhanced" || arg == "noenhanced") {
      device()->setEnhanced(arg == "enhanced");
    }
    else if (arg == "font") {
      CFontPtr font;

      if (parseFont(line, font))
        device()->setFont(font);
    }
    else if (arg == "fontscale") {
      double s = 1.0;

      if (parseReal(line, s))
        device()->setFontScale(s);
    }
    else if (arg == "solid" || arg == "dashed") {
      device()->setDashed(arg == "dashed");
    }
    else if (arg == "linewidth" || arg == "lw") {
      double lw = 1.0;

      if (parseReal(line, lw))
        device()->setLineWidth(lw);
    }
  }
  // set tics {axis | border} {{no}mirror}
  //          {in | out} {scale {default | <major> {,<minor>}}}
  //          {{no}rotate {by <ang>}} {offset <offset> | nooffset}
  //          {left | right | center | autojustify}
  //          { format "formatstring" } { font "name{,<size>}" }
  //          { textcolor <colorspec> }
  else if (var == VariableName::TICS) {
    int pos = line.pos();

    parseAxesTics(line, axesData_.xaxis[1]); line.setPos(pos);
    parseAxesTics(line, axesData_.xaxis[2]); line.setPos(pos);
    parseAxesTics(line, axesData_.yaxis[1]); line.setPos(pos);
    parseAxesTics(line, axesData_.yaxis[2]); line.setPos(pos);
    parseAxesTics(line, axesData_.zaxis[1]); line.setPos(pos);
    parseAxesTics(line, axesData_.cbaxis  ); line.setPos(pos);
  }
  // set timestamp {"<format>"} {top|bottom} {{no}rotate}
  //               {offset <xoff>{,<yoff>}} {font "<fontspec>"}
  //               {textcolor <colorspec>}
  else if (var == VariableName::TIMESTAMP) {
    line.skipSpace();

    if (line.isChar('"')) {
      std::string fmt;

      if (parseString(line, fmt))
        timeStamp_.setFormat(fmt);
    }

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "top" || arg == "bottom") {
        timeStamp_.setTop(arg == "top");
      }
      else if (arg == "norotate" || arg == "rotate") {
        timeStamp_.setRotated(arg == "rotate");
      }
      else if (arg == "offset") {
        double xo = 0.0, yo = 0.0;

        if (parseReal(line, xo)) {
          if (line.skipSpaceAndChar(',')) {
            if (! parseReal(line, yo))
              yo = 0.0;
          }

          timeStamp_.setOffset(CPoint2D(xo, yo));
        }
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          timeStamp_.setFont(font);
      }
      else if (arg == "textcolor") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          timeStamp_.setTextColor(c);
      }

      arg = readNonSpace(line);
    }
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
      setTimeFmt(fmt);
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
  // set tmargin {at screen} <val>
  else if (var == VariableName::TMARGIN) {
    line.skipSpace();

    bool screen = false;

    if (line.isChars("at screen")) {
      line.skipChars(9);

      line.skipSpace();

      screen = true;
    }

    double r = -1;

    if (parseReal(line, r))
      margin_.setTop(r, screen);
  }
  // set trange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::TRANGE) {
    parseAxisRange(line, axesData_.taxis[1]);
  }
  // set urange
  else if (var == VariableName::URANGE) {
    parseAxisRange(line, axesData_.uaxis);
  }
  // set view <rot_x>{,{<rot_z>}{,{<scale>}{,<scale_z>}}}
  // set view map {scale <scale>}
  // set view {no}equal {xy|xyz}
  else if (var == VariableName::VIEW) {
    int pos = line.pos();

    std::string arg = readNonSpaceNonComma(line);

    if      (arg == "map") {
      camera_.setMap();
    }
    else if (arg == "noequal") {
      camera_.setAxesScale(CGnuPlotCamera::AxesScale::NONE);
    }
    else if (arg == "equal") {
      arg = readNonSpaceNonComma(line);

      if      (arg == "xy")
        camera_.setAxesScale(CGnuPlotCamera::AxesScale::XY);
      else if (arg == "xyz")
        camera_.setAxesScale(CGnuPlotCamera::AxesScale::XYZ);
      else
        camera_.setAxesScale(CGnuPlotCamera::AxesScale::XY);
    }
    else {
      line.setPos(pos);

      double rx = 0.0;

      if (parseReal(line, rx)) {
        camera_.setRotateX(rx);

        double rz = 0.0;

        if (line.skipSpaceAndChar(',') && parseReal(line, rz)) {
          camera_.setRotateZ(rz);

          double scale = 0.0;

          if (line.skipSpaceAndChar(',') && parseReal(line, scale)) {
            camera_.setScaleX(scale);
            camera_.setScaleY(scale);

            double scaleZ = 0.0;

            if (line.skipSpaceAndChar(',') && parseReal(line, scaleZ))
              camera_.setScaleZ(scaleZ);
          }
        }
      }
    }
  }
  // set vrange
  else if (var == VariableName::VRANGE) {
    parseAxisRange(line, axesData_.vaxis);
  }
  // set x2data {time}
  else if (var == VariableName::X2DATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.xaxis[2].setIsTime(true);
  }
  // set x2dtics
  else if (var == VariableName::X2DTICS) {
    axesData_.xaxis[2].setIsDays(true);
  }
  // set x2label "<label>"
  else if (var == VariableName::X2LABEL) {
    parseAxisLabel(line, axesData_.xaxis[2]);
  }
  // set x2mtics
  else if (var == VariableName::X2MTICS) {
    axesData_.xaxis[2].setIsMonths(true);
  }
  // set x2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::X2RANGE) {
    parseAxisRange(line, axesData_.xaxis[2]);
  }
  // set x2tics
  else if (var == VariableName::X2TICS) {
    parseAxesTics(line, axesData_.xaxis[2]);
  }
  // set x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    parseAxisZeroAxis(line, axesData_.xaxis[2]);
  }
  // set xdata {time}
  else if (var == VariableName::XDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.xaxis[1].setIsTime(true);
  }
  // set xdtics
  else if (var == VariableName::XDTICS) {
    axesData_.xaxis[1].setIsDays(true);
  }
  // set xlabel "<label>"
  else if (var == VariableName::XLABEL) {
    parseAxisLabel(line, axesData_.xaxis[1]);
  }
  // set xmtics
  else if (var == VariableName::XMTICS) {
    axesData_.xaxis[1].setIsMonths(true);
  }
  // set xrange ...
  else if (var == VariableName::XRANGE) {
    parseAxisRange(line, axesData_.xaxis[1]);
  }
  // set xtics ...
  else if (var == VariableName::XTICS) {
    parseAxesTics(line, axesData_.xaxis[1]);
  }
  // set xyplane at <zvalue>
  // set xyplane relative <frac>
  else if (var == VariableName::XYPLANE) {
    std::string arg = readNonSpace(line);

    if      (arg == "at") {
      double z;

      if (parseReal(line, z))
        xyPlane_.setZ(z, false);
    }
    else if (arg == "relative") {
      double z;

      if (parseReal(line, z))
        xyPlane_.setZ(z, true);
    }
  }
  // set xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    parseAxisZeroAxis(line, axesData_.xaxis[1]);
  }
  // set y2data {time}
  else if (var == VariableName::Y2DATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.yaxis[2].setIsTime(true);
  }
  // set y2dtics
  else if (var == VariableName::Y2DTICS) {
    axesData_.yaxis[2].setIsDays(true);
  }
  // set y2label "<label>"
  else if (var == VariableName::Y2LABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.yaxis[2].setText(labelStr);
  }
  // set y2mtics
  else if (var == VariableName::Y2MTICS) {
    axesData_.yaxis[2].setIsMonths(true);
  }
  // set y2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::Y2RANGE) {
    parseAxisRange(line, axesData_.yaxis[2]);
  }
  // set y2tics
  else if (var == VariableName::Y2TICS) {
    parseAxesTics(line, axesData_.yaxis[2]);
  }
  // set y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    parseAxisZeroAxis(line, axesData_.yaxis[2]);
  }
  // set ydata {time}
  else if (var == VariableName::YDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.yaxis[1].setIsTime(true);
  }
  // set ydtics
  else if (var == VariableName::YDTICS) {
    axesData_.yaxis[1].setIsDays(true);
  }
  // set ylabel "<label>"
  else if (var == VariableName::YLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.yaxis[1].setText(labelStr);
  }
  // set ymtics
  else if (var == VariableName::YMTICS) {
    axesData_.yaxis[1].setIsMonths(true);
  }
  // set yrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::YRANGE) {
    parseAxisRange(line, axesData_.yaxis[1]);
  }
  // set ytics
  else if (var == VariableName::YTICS) {
    parseAxesTics(line, axesData_.yaxis[1]);
  }
  // set yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    parseAxisZeroAxis(line, axesData_.yaxis[1]);
  }
  // set zdata {time}
  else if (var == VariableName::ZDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.zaxis[1].setIsTime(true);
  }
  // set zdtics
  else if (var == VariableName::ZDTICS) {
    axesData_.zaxis[1].setIsDays(true);
  }
  // set zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    parseAxisZeroAxis(line, axesData_.zaxis[1]);
  }
  // set cbdata {time}
  else if (var == VariableName::CBDATA) {
    std::string arg = readNonSpace(line);

    if (arg == "time")
      axesData_.cbaxis.setIsTime(true);
  }
  // set cbdtics
  else if (var == VariableName::CBDTICS) {
    axesData_.cbaxis.setIsDays(true);
  }
  // set zero {expression}
  else if (var == VariableName::ZERO) {
    double r;

    if (parseReal(line, r))
      zero_ = r;
  }
  // set zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    int pos = line.pos();

    parseAxisZeroAxis(line, axesData_.xaxis[1]); line.setPos(pos);
    parseAxisZeroAxis(line, axesData_.xaxis[2]); line.setPos(pos);
    parseAxisZeroAxis(line, axesData_.yaxis[1]); line.setPos(pos);
    parseAxisZeroAxis(line, axesData_.yaxis[2]); line.setPos(pos);
    parseAxisZeroAxis(line, axesData_.zaxis[1]); line.setPos(pos);
    parseAxisZeroAxis(line, axesData_.zaxis[2]); line.setPos(pos);
  }
  // set zlabel "<label>"
  else if (var == VariableName::ZLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid zlabel string"))
      axesData_.zaxis[1].setText(labelStr);
  }
  // set zmtics
  else if (var == VariableName::ZMTICS) {
    axesData_.zaxis[1].setIsMonths(true);
  }
  // set zrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::ZRANGE) {
    parseAxisRange(line, axesData_.zaxis[1]);
  }
  // set ztics
  else if (var == VariableName::ZTICS) {
    parseAxesTics(line, axesData_.zaxis[1]);
  }
  // set cblabel "<label>"
  else if (var == VariableName::CBLABEL) {
    std::string labelStr;

    if (parseString(line, labelStr, "Invalid ylabel string"))
      axesData_.cbaxis.setText(labelStr);
  }
  // set cbmtics
  else if (var == VariableName::CBMTICS) {
    axesData_.cbaxis.setIsMonths(true);
  }
  // set cbrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::CBRANGE) {
    parseAxisRange(line, axesData_.cbaxis);
  }
  // set cbtics
  else if (var == VariableName::CBTICS) {
    parseAxesTics(line, axesData_.cbaxis);
  }
  // set debug
  else if (var == VariableName::DEBUG) {
    setDebug(true);
  }
  // set edebug
  else if (var == VariableName::EDEBUG) {
    setExprDebug(true);
  }

  return true;
}

void
CGnuPlot::
setForCmd(const ForCmd &forCmd, const std::string &args)
{
  if (forCmd.isIn) {
    std::vector<std::string> words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      setCmd(args);
    }
  }
  else {
    CExprValuePtr value1;

    if (! evaluateExpression(forCmd.start, value1) || ! value1.isValid())
      return;

    CExprValuePtr value2;

    if (! evaluateExpression(forCmd.end, value2) || ! value2.isValid())
      return;

    CExprValuePtr value3;

    if (forCmd.inc != "") {
      if (! evaluateExpression(forCmd.inc, value3) || ! value3.isValid())
        value3 = CExprValuePtr();
    }

    long i1 = 0, i2 = 1, i3 = 1;

    if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
      return;

    if (value3.isValid() && ! value3->getIntegerValue(i3))
      return;

    while (i1 <= i2) {
      CExprInst->createIntegerVariable(forCmd.var, i1);

      setCmd(args);

      i1 += i3;
    }
  }
}

// get ...
bool
CGnuPlot::
getCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "get " << args << std::endl;

  CParseLine line(args);

  VariableName var;

  if (! parseOptionValue(this, line, var, "option name"))
    return false;

  std::string identifier;

  if (! readIdentifier(line, identifier))
    identifier = "_" + CStrUniqueMatch::valueToString(var);

  std::stringstream ss;

  // get angles
  if      (var == VariableName::ANGLES) {
    CExprInst->createStringVariable(identifier, getAngleTypeString());
  }

  return true;
}

// show ...
// show plot
// show [ variables [all] | functions ]
// show [ all | version ] [ long ]
bool
CGnuPlot::
showCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "show " << args << std::endl;

  CParseLine line(args);

  VariableName var;

  if (! parseOptionValue(this, line, var)) {
    errorMsg("Missing option name");
    return false;
  }

  //---

  int pos = line.pos();

  StringArray args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  //---

  // show angles
  if      (var == VariableName::ANGLES) {
    printAngleType(std::cout);
  }
  // show arrow {<tag>}
  else if (var == VariableName::ARROW) {
    line.setPos(pos);

    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    showAnnotations<CGnuPlotArrow>(std::cout, ind);
  }
  // show autoscale
  else if (var == VariableName::AUTOSCALE) {
    std::cout << "autoscaling is ";

    std::cout << "x: ";

    std::ostringstream ostr;

    if      (axesData_.xaxis[1].isAutoScaleMin() && axesData_.xaxis[1].isAutoScaleMax())
      ostr << "ON, ";
    else if (axesData_.xaxis[1].isAutoScaleMin())
      ostr << "ON (min), ";
    else if (axesData_.xaxis[1].isAutoScaleMax())
      ostr << "ON (max), ";
    else
      ostr << "OFF, ";

    if (axesData_.xaxis[1].isAutoScaleFixMin()) ostr << "(fixmin), ";
    if (axesData_.xaxis[1].isAutoScaleFixMax()) ostr << "(fixmax), ";

    std::cout << ostr.str();

    for (int i = ostr.str().size(); i < 24; ++i)
      std::cout << " ";

    std::cout << "y: ";

    if      (axesData_.yaxis[1].isAutoScaleMin() && axesData_.yaxis[1].isAutoScaleMax())
      std::cout << "ON, ";
    else if (axesData_.yaxis[1].isAutoScaleMin())
      std::cout << "ON (min), ";
    else if (axesData_.yaxis[1].isAutoScaleMax())
      std::cout << "ON (max), ";
    else
      std::cout << "OFF, ";

    if (axesData_.yaxis[1].isAutoScaleFixMin()) std::cout << "(fixmin), ";
    if (axesData_.yaxis[1].isAutoScaleFixMax()) std::cout << "(fixmax), ";

    std::cout << std::endl;
  }
  // show bars
  else if (var == VariableName::BARS) {
    bars_.show(std::cout);
  }
  // show bind
  else if (var == VariableName::BIND) {
    // TODO
  }
  // show bmargin
  else if (var == VariableName::BMARGIN) {
    margin_.showBottom(std::cout);
  }
  // show border
  else if (var == VariableName::BORDER) {
    axesData_.border.show(std::cout);
  }
  // show boxwidth
  else if (var == VariableName::BOXWIDTH) {
    boxWidth_.show(std::cout);
  }
  // show clip
  else if (var == VariableName::CLIP) {
    clip_.show(std::cout);
  }
  // show colorbox
  else if (var == VariableName::COLORBOX) {
    colorBox_.show(std::cout);
  }
  // show colornames
  else if (var == VariableName::COLORNAMES) {
    showColorNames();
  }
  // show colorsequence
  else if (var == VariableName::COLORSEQUENCE) {
    showColorNames();
  }
  // show contour
  else if (var == VariableName::CONTOUR) {
    if (! contour3D_.enabled)
      std::cout << "contour for surfaces are not drawn" << std::endl;
    else {
      std::cout << "contour for surfaces are drawn on ";

      if      (contour3D_.pos == Contour3DData::DrawPos::DRAW_BASE   )
        std::cout << "base";
      else if (contour3D_.pos == Contour3DData::DrawPos::DRAW_SURFACE)
        std::cout << "surface";
      else if (contour3D_.pos == Contour3DData::DrawPos::DRAW_BOTH   )
        std::cout << "base and surface";

      std::cout << std::endl;
    }
  }
  // show dashtype
  else if (var == VariableName::DASHTYPE) {
    for (const auto &dt : lineDashes_) {
      std::cout << "dashtype " << dt.first << " (" << dt.second << ")" << std::endl;
    }
  }
  // show datafile
  else if (var == VariableName::DATAFILE) {
    std::map<std::string,bool> show;

    if (args1.size() > 0)
      show[args1[0]] = true;

    dataFile_.show(std::cout, show, true);
  }
  // show decimalsign
  else if (var == VariableName::DECIMALSIGN) {
    std::cout << "decimalsign for input is  . " << std::endl;

    if (decimalSign_.c == '\0')
      std::cout << "decimalsign for output has default value (normally '.')" << std::endl;
    else
      std::cout << "decimalsign for output is " << decimalSign_.c << std::endl;

    std::cout << "degree sign for output is °" << std::endl;
  }
  // show dgrid3d
  else if (var == VariableName::DGRID3D) {
    // TODO
  }
  // show dummy
  else if (var == VariableName::DUMMY) {
    std::string v1, v2;

    getDummyVars(v1, v2);

    std::cout << "dummy variables are " << v1 << " " << v2 << std::endl;
  }
  // show encoding
  else if (var == VariableName::ENCODING) {
    if (encoding_ == "")
      std::cout << "nominal character encoding is default" << std::endl;
    else
      std::cout << "nominal character encoding is " << encoding_ << std::endl;
    // however LC_CTYPE in current locale is en_US.UTF-8
  }
  // show fit
  else if (var == VariableName::FIT) {
    // TODO
  }
  // show fontpath
  else if (var == VariableName::FONTPATH) {
    std::cout << "fontpath is";

    for (const auto &f : fontPath_)
      std::cout << " \"" << f << "\"";

    std::cout << std::endl;
  }
  // show format
  else if (var == VariableName::FORMAT) {
    std::cout << "tic format is:" << std::endl;
    std::cout << " x-axis: \""  << axesData_.xaxis[1].format() << std::endl;
    std::cout << " y-axis: \""  << axesData_.yaxis[1].format() << std::endl;
    std::cout << " x2-axis: \"" << axesData_.xaxis[2].format() << std::endl;
    std::cout << " y2-axis: \"" << axesData_.yaxis[2].format() << std::endl;
    std::cout << " z-axis: \""  << axesData_.zaxis[1].format() << std::endl;
    std::cout << " cb-axis: \"" << axesData_.cbaxis  .format() << std::endl;
    std::cout << " r-axis: \""  << axesData_.raxis   .format() << std::endl;
  }
  // show functions
  else if (var == VariableName::FUNCTIONS) {
    showFunctions(std::cout);
  }
  // show grid
  else if (var == VariableName::GRID) {
    if (! axesData_.grid.enabled)
      std::cout << "grid is OFF" << std::endl;
    else {
      std::cout << "Polar grid drawn at r tics" << std::endl;

      std::cout << "Major grid drawn with linestyle " <<
                   axesData_.grid.majorLineStyle << std::endl;
      std::cout << "Minor grid drawn with linestyle " <<
                   axesData_.grid.minorLineStyle << std::endl;
      std::cout << "Major grid drawn with linetype " <<
                   axesData_.grid.majorLineType << std::endl;
      std::cout << "Minor grid drawn with linetype " <<
                   axesData_.grid.minorLineType << std::endl;

      std::cout << "Grid radii drawn every " <<
                   axesData_.grid.polarAngle << " radians" << std::endl;

      std::cout << "Grid drawn at ";

      std::cout << CStrUniqueMatch::
        valueToString<CGnuPlotTypes::DrawLayer>(axesData_.grid.layer) << " layer";

      std::cout << std::endl;
    }
  }
  // show hidden3d
  else if (var == VariableName::HIDDEN3D) {
    if (hidden3D_.enabled)
      std::cout << "hidden surface is removed" << std::endl;
    else
      std::cout << "hidden surface is drawn" << std::endl;

    std::cout << "  Hidden3d elements will be drawn in ";

    if (hidden3D_.layer == DrawLayer::FRONT)
      std::cout << "back of non-hidden3d elements" << std::endl;
    else
      std::cout << "front of non-hidden3d elements" << std::endl;

    std::cout << "  Back side of surfaces has linestyle offset of " <<
                 hidden3D_.offset.getValue(0) << std::endl;

    std::cout << "  Bit-Mask of Lines to draw in each triangle is " <<
                 hidden3D_.trianglePattern << std::endl;

    int undefVal = hidden3D_.undefined.getValue(1);

    if      (undefVal == 1)
      std::cout << "  1: Outranged and undefined datapoints are omitted from the surface." <<
                   std::endl;
    else if (undefVal == 2)
      std::cout << "  2: Only undefined datapoints are omitted from the surface." << std::endl;
    else if (undefVal == 3)
      std::cout << "  3: Will not check for undefined datapoints (may cause crashes)." << std::endl;

    if (hidden3D_.altdiagonal)
      std::cout << "  Will use other diagonal if it gives a less jaggy outline" << std::endl;
    else
      std::cout << "  Will not use other diagonal if it gives a less jaggy outline" << std::endl;

    if (hidden3D_.bentover)
      std::cout << "  Will draw diagonal visibly if quadrangle is 'bent over'" << std::endl;
    else
      std::cout << "  Will not draw diagonal visibly if quadrangle is 'bent over'" << std::endl;
  }
  // show history
  else if (var == VariableName::HISTORY) {
     std::cout << "history size " << historyData_.size << ", " <<
                  (historyData_.numbers ? "numbers" : "quiet") << ", " <<
                  (historyData_.trim ? "suppress duplicates" : "full") << std::endl;
  }
  // show isosamples
  else if (var == VariableName::ISOSAMPLES) {
    isoSamples_.show(std::cout);
  }
  // show key
  else if (var == VariableName::KEY) {
    keyData_.show(std::cout);
  }
  // show label
  else if (var == VariableName::LABEL) {
    showAnnotations<CGnuPlotLabel>(std::cout);
  }
  // show linetype <ind>
  else if (var == VariableName::LINETYPE) {
    int ind;

    if (parseInteger(line, ind)) {
      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      lineStyle->show(std::cout);
    }
    else {
      for (int i = 1; i <= 8; ++i) {
        CGnuPlotLineStyleP lineStyle = getLineStyleInd(i);

        lineStyle->show(std::cout);
      }

      if (styleIncrement_.increment > 0)
        std::cout << "Linetypes repeat every " << styleIncrement_.increment <<
                     " unless explicitly defined" << std::endl;
    }
  }
  // show link
  else if (var == VariableName::LINK) {
    if (linkData_.linkX2.enabled) {
      std::cout << "set link x2";

      if (linkData_.linkX2.expr != "")
        std::cout << " via " << linkData_.linkX2.expr;

      if (linkData_.linkX2.iexpr != "")
        std::cout << " inverse " << linkData_.linkX2.iexpr;

      std::cout << std::endl;
    }

    if (linkData_.linkY2.enabled) {
      std::cout << "set link y2";

      if (linkData_.linkY2.expr != "")
        std::cout << " via " << linkData_.linkY2.expr;

      if (linkData_.linkY2.iexpr != "")
        std::cout << " inverse " << linkData_.linkY2.iexpr;

      std::cout << std::endl;
    }
  }
  // show lmargin
  else if (var == VariableName::LMARGIN) {
    margin_.showLeft(std::cout);
  }
  // show loadpath
  else if (var == VariableName::LOADPATH) {
    std::cout << "loadpath is";

    if (loadPaths_.empty())
      std::cout << " empty" << std::endl;
    else {
      for (const auto &p : loadPaths_)
        std::cout << " \"" << p << "\"";

      std::cout << std::endl;

      std::cout << "loadpath from GNUPLOT_LIB is ";

      if (getenv("GNUPLOT_LIB"))
        std::cout << getenv("GNUPLOT_LIB");

      std::cout << std::endl;
    }
  }
  // show locale
  else if (var == VariableName::LOCALE) {
    std::cout << "locale_ \"" << locale_ << "\"" << std::endl;
  }
  // show logscale
  else if (var == VariableName::LOGSCALE) {
    if (isLogScale(LogScale::X ) || isLogScale(LogScale::Y ) || isLogScale(LogScale::Z ) ||
        isLogScale(LogScale::X2) || isLogScale(LogScale::Y2) || isLogScale(LogScale::CB)) {
      std::cout << "logscaling";

      auto print = [&](const std::string &id, LogScale s, bool &output) {
        if (! isLogScale(s)) return;
        if (output) std::cout << " and";
        std::cout << " " << id << " (base " << getLogScale(s) << ")";
        output = true;
      };

      bool output = false;

      print("x" , LogScale::X , output);
      print("y" , LogScale::Y , output);
      print("z" , LogScale::Z , output);
      print("x2", LogScale::X2, output);
      print("y2", LogScale::Y2, output);
      print("cb", LogScale::CB, output);

      std::cout << std::endl;
    }
    else {
      std::cout << "no logscaling" << std::endl;
    }
  }
  // show macros
  else if (var == VariableName::MACROS) {
  }
  // show mapping
  else if (var == VariableName::MAPPING) {
    std::cout << "mapping for 3-d data is ";

    if      (mapping_ == CGnuPlotTypes::Mapping::CARTESIAN_MAPPING  ) std::cout << "cartesian";
    else if (mapping_ == CGnuPlotTypes::Mapping::CYLINDRICAL_MAPPING) std::cout << "cylindrical";
    else if (mapping_ == CGnuPlotTypes::Mapping::SPHERICAL_MAPPING  ) std::cout << "spherical";

    std::cout << std::endl;
  }
  // show margins
  else if (var == VariableName::MARGINS) {
    margin_.showLeft  (std::cout);
    margin_.showRight (std::cout);
    margin_.showTop   (std::cout);
    margin_.showBottom(std::cout);
  }
  // show mouse
  else if (var == VariableName::MOUSE) {
    mouseData_.show(std::cout);
  }
  // show multiplot
  else if (var == VariableName::MULTIPLOT) {
    if (multiplot_.isEnabled())
      std::cout << "multiplot mode is on" << std::endl;
    else
      std::cout << "multiplot mode is off" << std::endl;
  }
  // show mx2tics
  else if (var == VariableName::MX2TICS) {
    axesData_.xaxis[2].showMinorTics(std::cout, "x2tics", "xtic");
  }
  // show mxtics
  else if (var == VariableName::MXTICS) {
    axesData_.xaxis[1].showMinorTics(std::cout, "xtics", "xtic");
  }
  // show my2tics
  else if (var == VariableName::MY2TICS) {
    axesData_.yaxis[2].showMinorTics(std::cout, "y2tics", "ytic");
  }
  // show mytics
  else if (var == VariableName::MYTICS) {
    axesData_.yaxis[1].showMinorTics(std::cout, "ytics", "ytic");
  }
  // show mztics
  else if (var == VariableName::MZTICS) {
    axesData_.zaxis[1].showMinorTics(std::cout, "ztics", "ztic");
  }
  // show object
  else if (var == VariableName::OBJECT) {
    std::string arg = readNonSpace(line);

    CGnuPlotTypes::ObjectType type = CGnuPlotTypes::ObjectType::NONE;

    if (parseOptionValue(this, line, type)) {
      if      (type == CGnuPlotTypes::ObjectType::RECTANGLE)
        showAnnotations<CGnuPlotRectangle>(std::cout);
      else if (type == CGnuPlotTypes::ObjectType::ELLIPSE)
        showAnnotations<CGnuPlotEllipse>(std::cout);
      else if (type == CGnuPlotTypes::ObjectType::CIRCLE)
        showAnnotations<CGnuPlotCircle>(std::cout);
      else if (type == CGnuPlotTypes::ObjectType::POLYGON)
        showAnnotations<CGnuPlotPolygon>(std::cout);
    }
    else
      showAnnotations<CGnuPlotGroupAnnotation>(std::cout);
  }
  // show offsets
  else if (var == VariableName::OFFSETS) {
    offsets_.show(std::cout);
  }
  // show origin
  else if (var == VariableName::ORIGIN) {
    plotSize_.showOrigin(std::cout);
  }
  // show output
  else if (var == VariableName::OUTPUT) {
    std::cout << "output is sent to " <<
      (! getOutputFile().empty() ? ("'" + getOutputFile() + "'"): "STDOUT") << std::endl;
  }
  // show parametric
  else if (var == VariableName::PARAMETRIC) {
    std::cout << "parametric is " << (parametric_ ? "ON" : "OFF") << std::endl;
  }
  // show paxis
  else if (var == VariableName::PAXIS) {
    line.setPos(pos);

    int i;

    if (parseInteger(line, i))
      axesData_.paxis[i].show(std::cout, "p", 1);
  }
  // show plot
  else if (var == VariableName::PLOT) {
    std::cout << "last plot command was: ";

    if      (lastPlotCmd_ != "")
      std::cout << "plot " << lastPlotCmd_;
    else if (lastSPlotCmd_ != "")
      std::cout << "splot " << lastSPlotCmd_;

    std::cout << std::endl;
  }
  // show pm3d
  else if (var == VariableName::PM3D) {
    pm3D_.show(std::cout);
  }
  // show palette
  else if (var == VariableName::PALETTE) {
    line.setPos(pos);

    std::string arg = readNonSpace(line);

    // show palette palette <n> {{float | int}}
    if      (arg == "palette") {
      int n = 0;

      if (! parseInteger(line, n))
        n = 0;

      std::string arg1 = readNonSpace(line);

      bool is_float = (arg1 == "float");
      bool is_int   = (arg1 == "int");

      palette_.showPaletteValues(std::cout, n, is_float, is_int);
    }
    // show palette gradient
    else if (arg == "gradient") {
      palette_.showGradient(std::cout);
    }
    // show palette fit2rgbformulae
    else if (arg == "fit2rgbformulae") {
      palette_.showRGBFormulae(std::cout);
    }
    // show palette rgbformulae
    else if (arg == "rgbformulae") {
      showRGBFormulae(std::cout);
    }
    else
      palette_.show(std::cout);
  }
  // show pointintervalbox
  else if (var == VariableName::POINTINTERVALBOX) {
    std::cout << "pointintervalbox is " << pointIntervalBox_ << std::endl;
  }
  // show pointsize
  else if (var == VariableName::POINTSIZE) {
    std::cout << "pointsize is " << lineStyle()->pointSize() << std::endl;
  }
  // show polar
  else if (var == VariableName::POLAR) {
    std::cout << "polar is " << (polar_ ? "ON" : "OFF") << std::endl;
  }
  // show print
  else if (var == VariableName::PRINT) {
    printFile_.show(std::cout);
  }
  // show psdir
  else if (var == VariableName::PSDIR) {
    std::cout << "directory from 'set psdir': " << (psDir_ != "" ? psDir_ : "none") << std::endl;
  }
  // show raxis
  else if (var == VariableName::RAXIS) {
    std::cout << "raxis is " <<
     (axesData_.raxis.isDisplayed() ? "drawn" : "not drawn") << std::endl;
  }
  // show rmargin
  else if (var == VariableName::RMARGIN) {
    margin_.showRight(std::cout);
  }
  // show rrange
  else if (var == VariableName::RRANGE) {
    axesData_.raxis.showRange(std::cout, "rrange");
  }
  // show rtics
  else if (var == VariableName::RTICS) {
    axesData_.raxis.showTics(std::cout, "r-axis");
  }
  // show samples
  else if (var == VariableName::SAMPLES) {
    samples_.show(std::cout);
  }
  // show size
  else if (var == VariableName::SIZE) {
    plotSize_.showSize(std::cout);
  }
  // show style
  else if (var == VariableName::STYLE) {
    std::map<std::string,bool> show;

    if (args1.size() > 0)
      show[args1[0]] = true;

    // show style arrow
    if      (show.empty() || show.find("arrow") != show.end()) {
      for (const auto &as : arrowStyles_) {
        std::cout << "arrowstyle " << as.first << ", ";
        as.second.print(this, std::cout);
        std::cout << std::endl;
      }
    }
    // show style boxplot
    else if (show.empty() || show.find("boxplot") != show.end()) {
      boxPlot_.show(std::cout);
    }
    // show style data
    else if (show.empty() || show.find("data") != show.end()) {
      std::cout << "Data are plotted with " <<
        CStrUniqueMatch::valueToString<PlotStyle>(getDataStyle()) << std::endl;
    }
    // show style fill
    else if (show.empty() || show.find("fill") != show.end()) {
      fillStyle_.show(std::cout);
    }
    // show style function
    else if (show.empty() || show.find("function") != show.end()) {
      std::cout << "Functions are plotted with " <<
        CStrUniqueMatch::valueToString<PlotStyle>(getFunctionStyle()) << std::endl;
    }
    // show style increment
    else if (show.empty() || show.find("increment") != show.end()) {
      if (getStyleIncrementType() == StyleIncrementType::USER)
        std::cout << "Plot lines increment over user-defined line styles "
                     "rather than default line types" << std::endl;
      else
        std::cout << "Plot lines increment over default linetypes" << std::endl;
    }
    // show style line
    else if (show.empty() || show.find("line") != show.end()) {
      for (const auto &lt : lineStyles_) {
        std::cout << "linestyle " << lt.first << ", ";
        lt.second->show(std::cout);
      }
    }
    // show style circle
    else if (show.empty() || show.find("circle") != show.end()) {
      circleStyle_.show(std::cout);
    }
    // show style rectangle
    else if (show.empty() || show.find("rectangle") != show.end()) {
      rectStyle_.show(std::cout);
    }
    // show style ellipse
    else if (show.empty() || show.find("ellipse") != show.end()) {
      ellipseStyle_.show(std::cout);
    }
    // show style textbox
    else if (show.empty() || show.find("textbox") != show.end()) {
      textBoxStyle_.show(std::cout);
    }
  }
  // show surface
  else if (var == VariableName::SURFACE) {
    std::cout << "surface is " << (isSurface3D() ? "drawn" : "not drawn") << std::endl;
  }
  // show table
  else if (var == VariableName::TABLE) {
    // needed ?
    std::cout << "table file \"" << getTableFile() << "\"" << std::endl;
  }
  // show terminal
  else if (var == VariableName::TERMINAL) {
    std::cout << "terminal type is " << device()->name() << " ";

    if (device())
      device()->show(std::cout);

    std::cout <<  std::endl;
  }
  // show tics
  else if (var == VariableName::TICS) {
    axesData_.xaxis[1].showMinorTics(std::cout, "xtics", "xtic");
    axesData_.xaxis[2].showMinorTics(std::cout, "x2tics", "xtic");
    axesData_.yaxis[1].showMinorTics(std::cout, "ytics", "ytic");
    axesData_.yaxis[2].showMinorTics(std::cout, "y2tics", "ytic");
    axesData_.zaxis[1].showMinorTics(std::cout, "ztics", "ztic");
    axesData_.cbaxis  .showMinorTics(std::cout, "cbtics", "cbtic");
  }
  // show timestamp
  else if (var == VariableName::TIMESTAMP) {
    timeStamp_.show(std::cout);
  }
  // show title
  else if (var == VariableName::TITLE) {
    title_.show(std::cout);
  }
  // show tmargin
  else if (var == VariableName::TMARGIN) {
    margin_.showTop(std::cout);
  }
  // show trange
  else if (var == VariableName::TRANGE) {
    axesData_.taxis[1].showRange(std::cout, "trange");
  }
  // show urange
  else if (var == VariableName::URANGE) {
    axesData_.uaxis.showRange(std::cout, "urange");
  }
  // show variables
  else if (var == VariableName::VARIABLES) {
    showVariables(std::cout, args1);
  }
  // show version
  else if (var == VariableName::VERSION) {
    // TODO: long
    std::cout << "CGnuPlot Version 0.1" << std::endl;
  }
  // show view
  else if (var == VariableName::VIEW) {
    camera_.showView(std::cout);
  }
  // show vrange
  else if (var == VariableName::VRANGE) {
    axesData_.vaxis.showRange(std::cout, "vrange");
  }
  // show x2data
  else if (var == VariableName::X2DATA) {
    std::cout << "x2 is set to " <<
      (axesData_.xaxis[2].isTime() ? "time" : "numerical") << std::endl;
  }
  // show x2dtics
  else if (var == VariableName::X2DTICS) {
    std::cout << "x2 is " << (axesData_.xaxis[2].isDays() ? "days" : "not days") << std::endl;
  }
  // show x2label
  else if (var == VariableName::X2LABEL) {
    axesData_.xaxis[2].printLabel(std::cout, "x2");
  }
  // show x2mtics
  else if (var == VariableName::X2MTICS) {
    std::cout << "x2 is " << (axesData_.xaxis[2].isMonths() ? "months" : "not months") << std::endl;
  }
  // show x2range
  else if (var == VariableName::X2RANGE) {
    axesData_.xaxis[2].showRange(std::cout, "x2range");
  }
  // show x2tics
  else if (var == VariableName::X2TICS) {
    axesData_.xaxis[2].showMinorTics(std::cout, "x2tics", "xtic");
  }
  // show x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    axesData_.xaxis[2].showZeroAxis(std::cout, "x2");
  }
  // show xdata
  else if (var == VariableName::XDATA) {
    std::cout << "x is set to " <<
      (axesData_.xaxis[1].isTime() ? "time" : "numerical") << std::endl;
  }
  // show xdtics
  else if (var == VariableName::XDTICS) {
    std::cout << "x is " << (axesData_.xaxis[1].isDays() ? "days" : "not days") << std::endl;
  }
  // show xlabel
  else if (var == VariableName::XLABEL) {
    axesData_.xaxis[1].printLabel(std::cout, "x");
  }
  // show xmtics
  else if (var == VariableName::XMTICS) {
    std::cout << "x is " << (axesData_.xaxis[1].isMonths() ? "months" : "not months") << std::endl;
  }
  // show xrange
  else if (var == VariableName::XRANGE) {
    axesData_.xaxis[1].showRange(std::cout, "xrange");
  }
  // show xtics
  else if (var == VariableName::XTICS) {
    axesData_.xaxis[1].showMinorTics(std::cout, "xtics", "xtic");
  }
  // show xyplane
  else if (var == VariableName::XYPLANE) {
    std::cout << "xyplane at " << xyPlane_.z() <<
      (xyPlane_.isRelative() ? " relative" : "") << std::endl;
  }
  // show xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    axesData_.xaxis[1].showZeroAxis(std::cout, "x");
  }
  // show y2data
  else if (var == VariableName::Y2DATA) {
    std::cout << "y2 is set to " <<
      (axesData_.yaxis[2].isTime() ? "time" : "numerical") << std::endl;
  }
  // show y2dtics
  else if (var == VariableName::Y2DTICS) {
    std::cout << "y2 is " << (axesData_.yaxis[2].isDays() ? "days" : "not days") << std::endl;
  }
  // show y2label
  else if (var == VariableName::Y2LABEL) {
    axesData_.yaxis[2].printLabel(std::cout, "y2");
  }
  // show y2mtics
  else if (var == VariableName::Y2MTICS) {
    std::cout << "y2 is " << (axesData_.yaxis[2].isMonths() ? "months" : "not months") << std::endl;
  }
  // show y2range
  else if (var == VariableName::Y2RANGE) {
    axesData_.yaxis[2].showRange(std::cout, "y2range");
  }
  // show y2tics
  else if (var == VariableName::Y2TICS) {
    axesData_.yaxis[2].showMinorTics(std::cout, "y2tics", "ytic");
  }
  // show y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    axesData_.yaxis[2].showZeroAxis(std::cout, "y2");
  }
  // show ydata
  else if (var == VariableName::YDATA) {
    std::cout << "y is set to " <<
      (axesData_.yaxis[1].isTime() ? "time" : "numerical") << std::endl;
  }
  // show ydtics
  else if (var == VariableName::YDTICS) {
    std::cout << "y is " << (axesData_.yaxis[1].isDays() ? "days" : "not days") << std::endl;
  }
  // show ylabel
  else if (var == VariableName::YLABEL) {
    axesData_.yaxis[1].printLabel(std::cout, "y");
  }
  // show ymtics
  else if (var == VariableName::YMTICS) {
    std::cout << "y is " << (axesData_.yaxis[1].isMonths() ? "months" : "not months") << std::endl;
  }
  // show yrange
  else if (var == VariableName::YRANGE) {
    axesData_.yaxis[1].showRange(std::cout, "yrange");
  }
  // show ytics
  else if (var == VariableName::YTICS) {
    axesData_.yaxis[1].showMinorTics(std::cout, "ytics", "ytic");
  }
  // show yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    axesData_.yaxis[1].showZeroAxis(std::cout, "y");
  }
  // show zdata
  else if (var == VariableName::ZDATA) {
    std::cout << "z is set to " <<
      (axesData_.zaxis[1].isTime() ? "time" : "numerical") << std::endl;
  }
  // show zdtics
  else if (var == VariableName::ZDTICS) {
    std::cout << "z is " << (axesData_.zaxis[1].isDays() ? "days" : "not days") << std::endl;
  }
  // show zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    axesData_.zaxis[1].showZeroAxis(std::cout, "z");
  }
  // show cbdata
  else if (var == VariableName::CBDATA) {
    std::cout << "cb is set to " <<
      (axesData_.cbaxis.isTime() ? "time" : "numerical") << std::endl;
  }
  // show cbdtics
  else if (var == VariableName::CBDTICS) {
    std::cout << "cb is " << (axesData_.cbaxis.isDays() ? "days" : "not days") << std::endl;
  }
  // show zero
  else if (var == VariableName::ZERO) {
    std::cout << "zero is " << zero_ << std::endl;
  }
  // show zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    axesData_.xaxis[1].showZeroAxis(std::cout, "x");
    axesData_.xaxis[2].showZeroAxis(std::cout, "x2");
    axesData_.yaxis[1].showZeroAxis(std::cout, "y");
    axesData_.yaxis[2].showZeroAxis(std::cout, "y2");
    axesData_.zaxis[1].showZeroAxis(std::cout, "z");
    axesData_.zaxis[2].showZeroAxis(std::cout, "z2");
  }
  // show zlabel
  else if (var == VariableName::ZLABEL) {
    axesData_.zaxis[1].printLabel(std::cout, "z");
  }
  // show zmtics
  else if (var == VariableName::ZMTICS) {
    std::cout << "z is " << (axesData_.zaxis[1].isMonths() ? "months" : "not months") << std::endl;
  }
  // show zrange
  else if (var == VariableName::ZRANGE) {
    axesData_.zaxis[1].showRange(std::cout, "zrange");
  }
  // show ztics
  else if (var == VariableName::ZTICS) {
    axesData_.zaxis[1].showMinorTics(std::cout, "ztics", "ztic");
  }
  // show cblabel
  else if (var == VariableName::CBLABEL) {
    axesData_.cbaxis.printLabel(std::cout, "cb");
  }
  // show cbmtics
  else if (var == VariableName::CBMTICS) {
    std::cout << "cb is " << (axesData_.cbaxis.isMonths() ? "months" : "not months") << std::endl;
  }
  // show cbrange
  else if (var == VariableName::CBRANGE) {
    axesData_.cbaxis.showRange(std::cout, "cbrange");
  }
  // show cbtics
  else if (var == VariableName::CBTICS) {
    axesData_.cbaxis.showMinorTics(std::cout, "cbtics", "cbtic");
  }
  // show ellipse
  else if (var == VariableName::ELLIPSE) {
    showAnnotations<CGnuPlotEllipse>(std::cout);
  }
  // show circle
  else if (var == VariableName::CIRCLE) {
    showAnnotations<CGnuPlotCircle>(std::cout);
  }
  // show polygon
  else if (var == VariableName::POLYGON) {
    showAnnotations<CGnuPlotPolygon>(std::cout);
  }
  // show rectangle
  else if (var == VariableName::RECTANGLE) {
    showAnnotations<CGnuPlotRectangle>(std::cout);
  }

  return true;
}

void
CGnuPlot::
showColorNames()
{
  std::set<std::string> names;

  int n = CRGBName::numColorNames();

  for (int i = 0; i < n; ++i) {
    std::string name = CRGBName::colorName(i);
    CRGB        rgb  = CRGBName::colorRGBA(i).getRGB();

    auto p = name.find("dark");

    if (p != std::string::npos) {
      if      (name[p + 4] == ' ')
        name[p + 4] = '-';
      else if (name[p + 4] != '-')
        name = name.substr(0, p + 4) + "-" + name.substr(p + 4);
    }

    p = name.find("light");

    if (p != std::string::npos) {
      if      (name[p + 5] == ' ')
        name[p + 5] = '-';
      else if (name[p + 5] != '-')
        name = name.substr(0, p + 5) + "-" + name.substr(p + 5);
    }

    p = name.find("medium");

    if (p != std::string::npos) {
      if      (name[p + 6] == ' ')
        name[p + 6] = '-';
      else if (name[p + 6] != '-')
        name = name.substr(0, p + 6) + "-" + name.substr(p + 6);
    }

    p = name.find("grey");

    if (p != std::string::npos)
      name[p + 2] = 'a';

    p = name.find(' ');

    while (p != std::string::npos) {
      name = name.substr(0, p) + name.substr(p + 1);

      p = name.find(' ');
    }

    if (names.find(name) != names.end())
      continue;

    names.insert(name);

    std::cout << name;

    int l = name.size();

    for (int j = l; j < 24; ++j)
      std::cout << " ";

    std::cout << rgb.stringEncode();

    int r = rgb.getRedI  ();
    int g = rgb.getGreenI();
    int b = rgb.getBlueI ();

    std::cout << " = ";

    if      (r <  10) std::cout << "  ";
    else if (r < 100) std::cout << " " ;
    std::cout << r << " ";

    if      (g <  10) std::cout << "  ";
    else if (g < 100) std::cout << " " ;
    std::cout << g << " ";

    if      (b <  10) std::cout << "  ";
    else if (b < 100) std::cout << " " ;
    std::cout << b << std::endl;
  }
}

void
CGnuPlot::
showVariables(std::ostream &os, const StringArray &args)
{
  StringArray names;

  CExprInst->getVariableNames(names);

  for (const auto &name : names) {
    bool match = (args.empty() ? true : false);

    for (const auto &arg1 : args) {
      if (name.size() > arg1.size() && arg1 == name.substr(0, arg1.size())) {
        match = true;
        break;
      }
    }

    if (match) {
      CExprVariablePtr var = CExprInst->getVariable(name);

      os << var->name() << " = ";

      CExprValueP value = var->value();

      CExprValueType type = (value.isValid() ? value->getType() : CEXPR_VALUE_NONE);

      if (type == CEXPR_VALUE_STRING)
        os << "\"" << value << "\"";
      else
        os << value;

      os << std::endl;
    }
  }
}

void
CGnuPlot::
showFunctions(std::ostream &os)
{
  StringArray names;

  CExprInst->getFunctionNames(names);

  for (uint i = 0; i < names.size(); ++i) {
    CExprFunctionPtr function = CExprInst->getFunction(names[i]);

    if (! function->isBuiltin())
      os << function << std::endl;
  }
}

void
CGnuPlot::
showRGBFormulae(std::ostream &os)
{
  os << "* there are 37 available rgb color mapping formulae:"                  << std::endl;
  os << "   0: 0               1: 0.5             2: 1"                         << std::endl;
  os << "   3: x               4: x^2             5: x^3"                       << std::endl;
  os << "   6: x^4             7: sqrt(x)         8: sqrt(sqrt(x))"             << std::endl;
  os << "   9: sin(90x)       10: cos(90x)       11: |x-0.5|"                   << std::endl;
  os << "  12: (2x-1)^2       13: sin(180x)      14: |cos(180x)|"               << std::endl;
  os << "  15: sin(360x)      16: cos(360x)      17: |sin(360x)|"               << std::endl;
  os << "  18: |cos(360x)|    19: |sin(720x)|    20: |cos(720x)|"               << std::endl;
  os << "  21: 3x             22: 3x-1           23: 3x-2"                      << std::endl;
  os << "  24: |3x-1|         25: |3x-2|         26: (3x-1)/2"                  << std::endl;
  os << "  27: (3x-2)/2       28: |(3x-1)/2|     29: |(3x-2)/2|"                << std::endl;
  os << "  30: x/0.32-0.78125 31: 2*x-0.84       32: 4x;1;-2x+1.84;x/0.08-11.5" << std::endl;
  os << "  33: |2*x - 0.5|    34: 2*x            35: 2*x - 0.5"                 << std::endl;
  os << "  36: 2*x - 1"                                                         << std::endl;
  os << "* negative numbers mean inverted=negative color component"             << std::endl;
  os << "* thus the ranges in `set pm3d rgbformulae' are -36..36"               << std::endl;
}

// reset
void
CGnuPlot::
resetCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "reset " << args << std::endl;

  // TODO: do not reset terminal and output
  resetAngleType();

  clearAnnotations<CGnuPlotGroupAnnotation>();

  fillStyle_.unset();

  for (int i = 1; i <= 2; ++i) {
    axesData_.xaxis[i].unsetRange();
    axesData_.yaxis[i].unsetRange();
    axesData_.zaxis[i].unsetRange();
  }

  axesData_.raxis .unsetRange();
  axesData_.cbaxis.unsetRange();

  axesData_.border.unset();

  keyData_.reset();

  palette_.unset();
}

// undefine ...
void
CGnuPlot::
undefineCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "undefine " << args << std::endl;

  CParseLine line(args);

  StringArray args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  for (const auto &arg : args1) {
    CExprInst->removeVariable(arg);
  }
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
    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return false;

    unsetForCmd(forCmd, cmd);

    return false;
  }

  //---

  VariableName var;

  if (! parseOptionValue(this, line, var, "option name"))
    return false;

  //----

  int pos = line.pos();

  StringArray args1;

  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if (! arg.empty())
      args1.push_back(arg);

    line.skipSpace();
  }

  //---

  // unset angles
  if      (var == VariableName::ANGLES) {
    varPrefs_[VariableName::ANGLES]->reset();
  }
  // unset arrow
  else if (var == VariableName::ARROW) {
    line.setPos(pos);

    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    clearAnnotations<CGnuPlotArrow>(ind);
  }
  // unset autoscale {axes}
  else if (var == VariableName::AUTOSCALE) {
    line.setPos(pos);

    std::string arg = readNonSpace(line);

    if      (arg == "") {
      for (int i = 1; i <= 2; ++i) {
        axesData_.xaxis[i].setAutoScaleMin(false);
        axesData_.xaxis[i].setAutoScaleMax(false);
        axesData_.yaxis[i].setAutoScaleMin(false);
        axesData_.yaxis[i].setAutoScaleMax(false);
      }
    }
    else if (arg == "x") {
      axesData_.xaxis[1].setAutoScaleMin(false);
      axesData_.xaxis[1].setAutoScaleMax(false);
    }
    else if (arg == "xmin")
      axesData_.xaxis[1].setAutoScaleMin(false);
    else if (arg == "xmax")
      axesData_.xaxis[1].setAutoScaleMax(false);
    else if (arg == "xfixmin")
      axesData_.xaxis[1].setAutoScaleFixMin(false);
    else if (arg == "xfixmax")
      axesData_.xaxis[1].setAutoScaleFixMax(false);
    else if (arg == "y") {
      axesData_.yaxis[1].setAutoScaleMin(false);
      axesData_.yaxis[1].setAutoScaleMax(false);
    }
    else if (arg == "ymin")
      axesData_.yaxis[1].setAutoScaleMin(false);
    else if (arg == "ymax")
      axesData_.yaxis[1].setAutoScaleMax(false);
    else if (arg == "yfixmin")
      axesData_.yaxis[1].setAutoScaleFixMin(true);
    else if (arg == "yfixmax")
      axesData_.yaxis[1].setAutoScaleFixMax(true);
    else if (arg == "xy") {
      axesData_.xaxis[1].setAutoScaleMin(false);
      axesData_.xaxis[1].setAutoScaleMin(false);
      axesData_.yaxis[1].setAutoScaleMin(false);
      axesData_.yaxis[1].setAutoScaleMin(false);
    }
    else
      errorMsg("Unhandled autoscale " + arg);
  }
  // unset bars
  else if (var == VariableName::BARS) {
    setBarsSize (0.0);
    setBarsFront(true);
  }
  // unset bind ???
  else if (var == VariableName::BIND) {
  }
  // unset bmargin
  else if (var == VariableName::BMARGIN) {
    margin_.resetBottom();
  }
  // unset border
  else if (var == VariableName::BORDER) {
    axesData_.border.unset();
  }
  // unset boxwidth
  else if (var == VariableName::BOXWIDTH) {
    boxWidth_.unset();
  }
  // unset clip <clip-type>
  else if (var == VariableName::CLIP) {
    std::string arg = readNonSpace(line);

    if (arg != "") {
      while (arg != "") {
        if      (arg == "points") clip_.setPoints(false);
        else if (arg == "one"   ) clip_.setOne   (false);
        else if (arg == "two"   ) clip_.setTwo   (false);

        arg = readNonSpace(line);
      }
    }
    else
      clip_.unset();
  }
  // unset colorbox
  else if (var == VariableName::COLORBOX) {
    colorBox_.setEnabled(false);
  }
  // unset colorsequence (invalid ?)
  else if (var == VariableName::COLORSEQUENCE) {
    errorMsg("invalid command");
  }
  // unset colornames (invalid ?)
  else if (var == VariableName::COLORNAMES) {
    errorMsg("invalid command");
  }
  // unset contour
  else if (var == VariableName::CONTOUR) {
    contour3D_.enabled = false;
  }
  // unset dashtype
  else if (var == VariableName::DASHTYPE) {
    lineDashes_.clear();
  }
  // unset datafile
  else if (var == VariableName::DATAFILE) {
    dataFile_.unset();
  }
  // unset decimalsign
  else if (var == VariableName::DECIMALSIGN) {
    decimalSign_.c      = '\0';
    decimalSign_.locale = "";
  }
  // unset dgrid3d
  else if (var == VariableName::DGRID3D) {
    // TODO
  }
  // unset dummy
  else if (var == VariableName::DUMMY) {
    resetDummyVars();
  }
  // unset encoding
  else if (var == VariableName::ENCODING) {
    encoding_ = "";
  }
  // unset fit
  else if (var == VariableName::FIT) {
    // TODO
  }
  // unset fontpath
  else if (var == VariableName::FONTPATH) {
    fontPath_.clear();
  }
  // unset format
  else if (var == VariableName::FORMAT) {
    axesData_.xaxis[1].setFormat("%g");
    axesData_.yaxis[1].setFormat("%g");
    axesData_.xaxis[2].setFormat("%g");
    axesData_.yaxis[2].setFormat("%g");
    axesData_.zaxis[1].setFormat("%g");
    axesData_.cbaxis  .setFormat("%g");
    axesData_.raxis   .setFormat("%g");
  }
  // unset functions (invalid)
  else if (var == VariableName::FUNCTIONS) {
    errorMsg("Invalid command");
  }
  // unset grid
  else if (var == VariableName::GRID) {
    axesData_.grid.enabled = false;

    axesData_.xaxis[1].setGrid(false);
    axesData_.yaxis[1].setGrid(false);
  }
  // unset hidden3d
  else if (var == VariableName::HIDDEN3D) {
    hidden3D_.enabled = false;
  }
  // unset history
  else if (var == VariableName::HISTORY) {
    historyData_.size    = 100;
    historyData_.numbers = true;
    historyData_.trim    = true;
  }
  // unset isosamples
  else if (var == VariableName::ISOSAMPLES) {
    isoSamples_.unset();
  }
  // unset key
  else if (var == VariableName::KEY) {
    keyData_.setDisplayed(false);
  }
  // unset label
  else if (var == VariableName::LABEL) {
    clearAnnotations<CGnuPlotLabel>();
  }
  // unset linetype <ind>
  else if (var == VariableName::LINETYPE) {
    int ind;

    if (parseInteger(line, ind)) {
      CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

      lineStyle->unset();
    }
    else {
      for (int i = 1; i <= 8; ++i) {
        CGnuPlotLineStyleP lineStyle = getLineStyleInd(i);

        lineStyle->unset();
      }

      styleIncrement_.increment = -1;
    }
  }
  // unset link
  else if (var == VariableName::LINK) {
    linkData_.linkX2.enabled = false;
    linkData_.linkY2.enabled = false;
  }
  // unset lmargin
  else if (var == VariableName::LMARGIN) {
    margin_.resetLeft();
  }
  // unset loadpath
  else if (var == VariableName::LOADPATH) {
    loadPaths_.clear();
  }
  // unset locale
  else if (var == VariableName::LOCALE) {
    locale_ = "";
  }
  // unset logscale
  else if (var == VariableName::LOGSCALE) {
    resetLogScale();
  }
  // unset macros
  else if (var == VariableName::MACROS) {
    setMacros(false);
  }
  // unset mapping
  else if (var == VariableName::MAPPING) {
    mapping_ = CGnuPlotTypes::Mapping::CARTESIAN_MAPPING;
  }
  // unset margins
  else if (var == VariableName::MARGINS) {
    margin_.resetLeft  ();
    margin_.resetRight ();
    margin_.resetTop   ();
    margin_.resetBottom();
  }
  // unset mouse
  else if (var == VariableName::MOUSE) {
    mouseData_.setEnabled(false);
  }
  // unset multiplot
  else if (var == VariableName::MULTIPLOT) {
    if (multiWindow_ && multiplot_.isAutoFit())
      multiWindow_->fitGroups();

    multiplot_.setEnabled(false);

    multiWindow_ = 0;
  }
  // unset mx2tics
  else if (var == VariableName::MX2TICS) {
    axesData_.xaxis[2].setMinorTicsDisplayed(false);
    axesData_.xaxis[2].resetMinorTicsFreq();
  }
  // unset mxtics
  else if (var == VariableName::MXTICS) {
    axesData_.xaxis[1].setMinorTicsDisplayed(false);
    axesData_.xaxis[1].resetMinorTicsFreq();
  }
  // unset my2tics
  else if (var == VariableName::MY2TICS) {
    axesData_.yaxis[2].setMinorTicsDisplayed(false);
    axesData_.yaxis[2].resetMinorTicsFreq();
  }
  // unset mytics
  else if (var == VariableName::MYTICS) {
    axesData_.yaxis[1].setMinorTicsDisplayed(false);
    axesData_.yaxis[1].resetMinorTicsFreq();
  }
  // unset mztics
  else if (var == VariableName::MZTICS) {
    axesData_.zaxis[1].setMinorTicsDisplayed(false);
    axesData_.zaxis[1].resetMinorTicsFreq();
  }
  // unset object <index>
  else if (var == VariableName::OBJECT) {
    // TODO
  }
  // unset offsets
  else if (var == VariableName::OFFSETS) {
    offsets_.unset();
  }
  // unset origin
  else if (var == VariableName::ORIGIN) {
    plotSize_.unsetOrigin();
  }
  // unset output
  else if (var == VariableName::OUTPUT) {
    setOutputFile("");
  }
  // unset parametric
  else if (var == VariableName::PARAMETRIC) {
    parametric_ = false;
  }
  // unset paxis
  else if (var == VariableName::PAXIS) {
    line.setPos(pos);

    int i;

    if (parseInteger(line, i))
      axesData_.paxis[i].unset();
  }
  // unset plot (invalid)
  else if (var == VariableName::PLOT) {
  }
  // unset pm3d
  else if (var == VariableName::PM3D) {
    pm3D_.unset();
  }
  // unset palette
  else if (var == VariableName::PALETTE) {
    palette_.unset();
  }
  // unset pointintervalbox
  else if (var == VariableName::POINTINTERVALBOX) {
    pointIntervalBox_ = 1;
  }
  // unset pointsize
  else if (var == VariableName::POINTSIZE) {
    lineStyle()->resetPointSize();
  }
  // unset polar
  else if (var == VariableName::POLAR) {
    polar_ = false;
  }
  // unset print
  else if (var == VariableName::PRINT) {
    printFile_.unset();
  }
  // unset psdir
  else if (var == VariableName::PSDIR) {
    psDir_ = "";
  }
  // unset raxis
  else if (var == VariableName::RAXIS) {
    axesData_.raxis.setDisplayed(false);
  }
  // unset rmargin
  else if (var == VariableName::RMARGIN) {
    margin_.resetRight();
  }
  // unset rrange
  else if (var == VariableName::RRANGE) {
    axesData_.raxis.unsetRange();
  }
  // unset rtics
  else if (var == VariableName::RTICS) {
    axesData_.raxis.setShowTics(false);
  }
  // unset samples
  else if (var == VariableName::SAMPLES) {
    samples_.unset();
  }
  // unset size
  else if (var == VariableName::SIZE) {
    // TODO: don't reset ratio ?
    plotSize_.unsetSize();
  }
  // unset style
  else if (var == VariableName::STYLE) {
    std::map<std::string,bool> unset;

    if (args1.size() > 0)
      unset[args1[0]] = true;

    // unset style arrow
    if      (unset.empty() || unset.find("arrow") != unset.end()) {
      arrowStyles_.clear();
    }
    // unset style boxplot
    else if (unset.empty() || unset.find("boxplot") != unset.end()) {
      boxPlot_.unset();
    }
    // unset style data
    else if (unset.empty() || unset.find("data") != unset.end()) {
      setDataStyle(PlotStyle::POINTS);
    }
    // unset style fill
    else if (unset.empty() || unset.find("fill") != unset.end()) {
      fillStyle_.unset();
    }
    // unset style function
    else if (unset.empty() || unset.find("function") != unset.end()) {
      setFunctionStyle(PlotStyle::LINES);
    }
    // unset style increment
    else if (unset.empty() || unset.find("increment") != unset.end()) {
      setStyleIncrementType(StyleIncrementType::USER);
    }
    // unset style line
    else if (unset.empty() || unset.find("line") != unset.end()) {
      lineStyles_.clear();
    }
    // unset style circle
    else if (unset.empty() || unset.find("circle") != unset.end()) {
      circleStyle_.unset();
    }
    // unset style rectangle
    else if (unset.empty() || unset.find("rectangle") != unset.end()) {
      rectStyle_.unset();
    }
    // unset style ellipse
    else if (unset.empty() || unset.find("ellipse") != unset.end()) {
      ellipseStyle_.unset();
    }
    // unset style textbox
    else if (unset.empty() || unset.find("textbox") != unset.end()) {
      textBoxStyle_.unset();
    }
  }
  // unset surface
  else if (var == VariableName::SURFACE) {
    setSurface3D(false);
  }
  // unset table
  else if (var == VariableName::TABLE) {
    setTableFile("");
  }
  // unset terminal
  else if (var == VariableName::TERMINAL) {
    setDevice("qt");
  }
  // unset tics
  else if (var == VariableName::TICS) {
    axesData_.xaxis[1].setShowTics(false);
    axesData_.xaxis[2].setShowTics(false);
    axesData_.yaxis[1].setShowTics(false);
    axesData_.yaxis[2].setShowTics(false);
    axesData_.zaxis[1].setShowTics(false);
    axesData_.cbaxis  .setShowTics(false);
  }
  // unset timestamp
  else if (var == VariableName::TIMESTAMP) {
    timeStamp_.unset();
  }
  // unset title
  else if (var == VariableName::TITLE) {
    title_.unset();
  }
  // unset tmargin
  else if (var == VariableName::TMARGIN) {
    margin_.resetTop();
  }
  // unset trange
  else if (var == VariableName::TRANGE) {
    axesData_.taxis[1].unset();
  }
  // unset urange
  else if (var == VariableName::URANGE) {
    axesData_.uaxis.unset();
  }
  // unset view
  else if (var == VariableName::VIEW) {
    camera_.unsetView();
  }
  // unset vrange
  else if (var == VariableName::VRANGE) {
    axesData_.vaxis.unset();
  }
  // unset x2data
  else if (var == VariableName::X2DATA) {
    axesData_.xaxis[2].setIsTime(false);
  }
  // unset x2dtics
  else if (var == VariableName::X2DTICS) {
    axesData_.xaxis[2].setIsDays(false);
  }
  // unset x2label
  else if (var == VariableName::X2LABEL) {
    axesData_.xaxis[2].setText("");
  }
  // unset x2mtics
  else if (var == VariableName::X2MTICS) {
    axesData_.xaxis[2].setIsMonths(false);
  }
  // unset x2range
  else if (var == VariableName::X2RANGE) {
    axesData_.xaxis[2].unsetRange();
  }
  // unset x2tics
  else if (var == VariableName::X2TICS) {
    axesData_.xaxis[2].setShowTics(false);
  }
  // unset x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    axesData_.xaxis[2].unsetZeroAxis();
  }
  // unset xdata
  else if (var == VariableName::XDATA) {
    axesData_.xaxis[1].setIsTime(false);
  }
  // unset xdtics
  else if (var == VariableName::XDTICS) {
    axesData_.xaxis[1].setIsDays(false);
  }
  // unset xlabel
  else if (var == VariableName::XLABEL) {
    axesData_.xaxis[1].setText("");
  }
  // unset xmtics
  else if (var == VariableName::XMTICS) {
    axesData_.xaxis[1].setIsMonths(false);
  }
  // unset xrange
  else if (var == VariableName::XRANGE) {
    axesData_.xaxis[1].unsetRange();
  }
  // unset xtics
  else if (var == VariableName::XTICS) {
    axesData_.xaxis[1].setShowTics(false);
  }
  // unset xyplane
  else if (var == VariableName::XYPLANE) {
    xyPlane_.setZ(0, false);
  }
  // unset xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    axesData_.xaxis[1].unsetZeroAxis();
  }
  // unset y2data
  else if (var == VariableName::Y2DATA) {
    axesData_.yaxis[2].setIsTime(false);
  }
  // unset y2dtics
  else if (var == VariableName::Y2DTICS) {
    axesData_.yaxis[2].setIsDays(false);
  }
  // unset y2label
  else if (var == VariableName::Y2LABEL) {
    axesData_.yaxis[2].setText("");
  }
  // unset y2mtics
  else if (var == VariableName::Y2MTICS) {
    axesData_.yaxis[2].setIsMonths(false);
  }
  // unset y2range
  else if (var == VariableName::Y2RANGE) {
    axesData_.yaxis[2].unsetRange();
  }
  // unset y2tics
  else if (var == VariableName::Y2TICS) {
    axesData_.yaxis[2].setShowTics(false);
  }
  // unset y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    axesData_.yaxis[2].unsetZeroAxis();
  }
  // unset ydata
  else if (var == VariableName::YDATA) {
    axesData_.yaxis[1].setIsTime(false);
  }
  // unset ydtics
  else if (var == VariableName::YDTICS) {
    axesData_.yaxis[1].setIsDays(false);
  }
  // unset ylabel
  else if (var == VariableName::YLABEL) {
    axesData_.yaxis[1].setText("");
  }
  // unset ymtics
  else if (var == VariableName::YMTICS) {
    axesData_.yaxis[1].setIsMonths(false);
  }
  // unset yrange
  else if (var == VariableName::YRANGE) {
    axesData_.yaxis[1].unsetRange();
  }
  // unset ytics
  else if (var == VariableName::YTICS) {
    axesData_.yaxis[1].setShowTics(false);
  }
  // unset yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    axesData_.yaxis[1].unsetZeroAxis();
  }
  // unset zdata
  else if (var == VariableName::ZDATA) {
    axesData_.zaxis[1].setIsTime(false);
  }
  // unset zdtics
  else if (var == VariableName::ZDTICS) {
    axesData_.zaxis[1].setIsDays(false);
  }
  // unset zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    axesData_.zaxis[1].unsetZeroAxis();
  }
  // unset cbdata
  else if (var == VariableName::CBDATA) {
    axesData_.cbaxis.setIsTime(false);
  }
  // unset cbdtics
  else if (var == VariableName::CBDTICS) {
    axesData_.cbaxis.setIsDays(false);
  }
  // unset zero
  else if (var == VariableName::ZERO) {
    zero_ = 1E-8;
  }
  // unset zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    axesData_.xaxis[1].unsetZeroAxis();
    axesData_.xaxis[2].unsetZeroAxis();
    axesData_.yaxis[1].unsetZeroAxis();
    axesData_.yaxis[2].unsetZeroAxis();
    axesData_.zaxis[1].unsetZeroAxis();
    axesData_.zaxis[2].unsetZeroAxis();
  }
  // unset zlabel
  else if (var == VariableName::ZLABEL) {
    axesData_.zaxis[1].setText("");
  }
  // unset zmtics
  else if (var == VariableName::ZMTICS) {
    axesData_.zaxis[1].setIsMonths(false);
  }
  // unset zrange
  else if (var == VariableName::ZRANGE) {
    axesData_.zaxis[1].unsetRange();
  }
  // unset ztics
  else if (var == VariableName::ZTICS) {
    axesData_.zaxis[1].setShowTics(false);
  }
  // unset cblabel
  else if (var == VariableName::CBLABEL) {
    axesData_.cbaxis.setText("");
  }
  // unset cbmtics
  else if (var == VariableName::CBMTICS) {
    axesData_.cbaxis.setIsMonths(false);
  }
  // unset cbrange
  else if (var == VariableName::CBRANGE) {
    axesData_.cbaxis.unsetRange();
  }
  // unset cbtics
  else if (var == VariableName::CBTICS) {
    axesData_.cbaxis.setShowTics(false);
  }
  else if (var == VariableName::ELLIPSE) {
    clearAnnotations<CGnuPlotEllipse>();
  }
  else if (var == VariableName::CIRCLE) {
    clearAnnotations<CGnuPlotCircle>();
  }
  else if (var == VariableName::POLYGON) {
    clearAnnotations<CGnuPlotPolygon>();
  }
  else if (var == VariableName::RECTANGLE) {
    clearAnnotations<CGnuPlotRectangle>();
  }
  // unset debug
  else if (var == VariableName::DEBUG)
    setDebug(false);
  // unset edebug
  else if (var == VariableName::EDEBUG)
    CExprInst->setDebug(false);

  return false;
}

void
CGnuPlot::
unsetForCmd(const ForCmd &forCmd, const std::string &args)
{
  if (forCmd.isIn) {
    std::vector<std::string> words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      unsetCmd(args);
    }
  }
  else {
    CExprValuePtr value1;

    if (! evaluateExpression(forCmd.start, value1) || ! value1.isValid())
      return;

    CExprValuePtr value2;

    if (! evaluateExpression(forCmd.end, value2) || ! value2.isValid())
      return;

    CExprValuePtr value3;

    if (forCmd.inc != "") {
      if (! evaluateExpression(forCmd.inc, value3) || ! value3.isValid())
        value3 = CExprValuePtr();
    }

    long i1 = 0, i2 = 1, i3 = 1;

    if (! value1->getIntegerValue(i1) || ! value2->getIntegerValue(i2))
      return;

    if (value3.isValid() && ! value3->getIntegerValue(i3))
      return;

    while (i1 <= i2) {
      CExprInst->createIntegerVariable(forCmd.var, i1);

      unsetCmd(args);

      i1 += i3;
    }
  }
}

// shell
void
CGnuPlot::
shellCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "shell " << args << std::endl;

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
  if (isDebug()) std::cerr << "system " << args << std::endl;

  CParseLine line(args);

  std::string str;

  if (parseString(line, str))
    system(str.c_str());
}

// stats
void
CGnuPlot::
statsCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "stats " << args << std::endl;

  // TODO
}

// clear
void
CGnuPlot::
clearCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "clear " << args << std::endl;

  double x = plotSize_.xo   .getValue(0.0);
  double y = plotSize_.yo   .getValue(0.0);
  double w = plotSize_.xsize.getValue(1.0);
  double h = plotSize_.ysize.getValue(1.0);

  clearRect_ = CBBox2D(x, y, x + w, y + h);
}

// lower {winid:i}
void
CGnuPlot::
lowerCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "lower " << args << std::endl;
}

// raise {winid:i}
void
CGnuPlot::
raiseCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "upper " << args << std::endl;
}

// test
// test palette
void
CGnuPlot::
testCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "test " << args << std::endl;

  CParseLine line(args);

  std::string arg = readNonSpace(line);

  CGnuPlotWindowP window = CGnuPlotWindowP(createWindow());

  window->setBackgroundColor(backgroundColor());

  windows_.push_back(window);

  CGnuPlotGroup *group = createGroup(window.get());

  group->init();
  group->set3D(false);

  PlotStyle plotStyle = PlotStyle::TEST_TERMINAL;

  if (arg == "palette")
    plotStyle = PlotStyle::TEST_PALETTE;

  CGnuPlotPlot *plot = createPlot(group, plotStyle);

  group->addSubPlots({plot});

  if (arg == "palette") {
    group->setMargin(Margin(5, 10, 5, 10));
    group->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }
  else {
    group->setMargin(Margin(0, 0, 0, 0));
    group->setRange(CBBox2D(0.0, 0.0, 1.0, 1.0));
  }

  window->addGroup(group);

  if (device())
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
fitCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "fit " << args << std::endl;

  // TODO
}

// update "<filename>"
void
CGnuPlot::
updateCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "update " << args << std::endl;

  // TODO
}

// bind
void
CGnuPlot::
bindCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "bind " << args << std::endl;
}

// if ({condition}) {command}
//    [; else if ( {condition} ) {command} ]
//    [; else {command} ]
void
CGnuPlot::
ifCmd(int &i, const Statements &statements)
{
  if (isDebug()) std::cerr << "if" << std::endl;

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

CGnuPlotArrowStyle
CGnuPlot::
arrowStyle(int id) const
{
  auto p = arrowStyles_.find(id);

  if (p == arrowStyles_.end())
    return arrowStyle_;

  return (*p).second;
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
  // split using string into integer range
  auto splitUsingFn = [] (const std::string &usingStr, StringArray &usingStrs) -> bool {
    auto p = usingStr.find('-');

    if (p == std::string::npos)
      return false;

    std::string lhs = usingStr.substr(0, p);
    std::string rhs = usingStr.substr(p + 1);

    int col1, col2;

    if (CStrUtil::toInteger(lhs, &col1) && CStrUtil::toInteger(rhs, &col2)) {
      for (int i = col1; i <= col2; ++i)
        usingStrs.push_back(CStrUtil::toString(i));

      return true;
    }

    return false;
  };

  //---

  std::string usingStr;
  StringArray usingStrs;
  bool        isExpr = false;

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
          else {
            if (splitUsingFn(usingStr, usingStrs))
              usingStr = "";
          }

          if (! usingStr.empty())
            usingStrs.push_back(usingStr);
        }

        usingStr = "";
        isExpr   = false;
      }
      else {
        if (line.isChar('(')) {
          ++rbrackets;

          if (usingStr.empty()) {
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
    else {
      if (splitUsingFn(usingStr, usingStrs))
        usingStr = "";
    }

    if (! usingStr.empty())
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

  StringArray inds;

  CStrUtil::addFields(indexStr, inds, ":");

  //---

  auto parseInd = [&](int i, int &var, int def) {
    var = def;

    if (int(inds.size()) > i) {
      if (! CStrUtil::toInteger(inds[i], &var))
        var = def;
    }
  };

  parseInd(0, indexStart, 0);
  parseInd(1, indexEnd  , std::numeric_limits<int>::max());
  parseInd(2, indexStep , 1);
}

void
CGnuPlot::
parseEvery(CParseLine &line, int &everyPointStart, int &everyPointEnd, int &everyPointStep,
           int &everyBlockStart, int &everyBlockEnd, int &everyBlockStep)
{
  std::string everyStr = readNonSpaceNonComma(line);

  line.skipSpace();

  StringArray inds;

  CStrUtil::addFields(everyStr, inds, ":");

  //---

  auto parseInd = [&](int i, int &var, int def) {
    var = def;

    if (int(inds.size()) > i) {
      CExprValueP value;

      var = def;

      if (inds[i] != "" && evaluateExpression(inds[i], value, true)) {
        long l;

        if (value.isValid() && value->getIntegerValue(l))
          var = l;
      }
    }
  };

  parseInd(0, everyPointStep , 1);
  parseInd(1, everyBlockStep , 1);
  parseInd(2, everyPointStart, 0);
  parseInd(3, everyBlockStart, 0);
  parseInd(4, everyPointEnd  , std::numeric_limits<int>::max());
  parseInd(5, everyBlockEnd  , std::numeric_limits<int>::max());
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
  if (isDebug()) std::cerr << "do " << args << std::endl;

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

  std::string arg1, arg2, arg3;
  StringArray fields;

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
    line.skipNonSpace();

    std::string str;

    if (! parseString(line, str))
      return;

    CStrUtil::addWords(str, fields);
  }

  if (! line.skipSpaceAndChar(']'))
    return;

  if (! line.skipSpaceAndChar('{'))
    return;

  StringArray lines;

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
  if (isDebug()) std::cerr << "while " << args << std::endl;

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
  if (isDebug()) std::cerr << "evaluate " << args << std::endl;

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
  if (isDebug()) std::cerr << "pause " << args << std::endl;

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
rereadCmd(const std::string &args)
{
  if (isDebug()) std::cerr << "reread " << args << std::endl;

  fileData_.file->rewind();

  fileData_.lineNum = 0;

  fileData_.bufferLines.clear();
}

CGnuPlotWindow *
CGnuPlot::
createWindow()
{
  return (device() ? device()->createWindow() : new CGnuPlotWindow(this));
}

CGnuPlotGroup *
CGnuPlot::
createGroup(CGnuPlotWindow *window)
{
  return (device() ? device()->createGroup(window) : new CGnuPlotGroup(window));
}

CGnuPlotPlot *
CGnuPlot::
createPlot(CGnuPlotGroup *group, PlotStyle style)
{
  return (device() ? device()->createPlot(group, style) : new CGnuPlotPlot(group, style));
}

CGnuPlotLineStyle *
CGnuPlot::
createLineStyle()
{
  return (device() ? device()->createLineStyle() : new CGnuPlotLineStyle);
}

CGnuPlotAxis *
CGnuPlot::
createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir)
{
  return (device() ? device()->createAxis(group, id, dir) : new CGnuPlotAxis(group, id, dir));
}

CGnuPlotKey *
CGnuPlot::
createKey(CGnuPlotGroup *group)
{
  return (device() ? device()->createKey(group) : new CGnuPlotKey(group));
}

CGnuPlotColorBox *
CGnuPlot::
createColorBox(CGnuPlotGroup *group)
{
  return (device() ? device()->createColorBox(group) : new CGnuPlotColorBox(group));
}

CGnuPlotPalette *
CGnuPlot::
createPalette(CGnuPlotGroup *group)
{
  return (device() ? device()->createPalette(group) : new CGnuPlotPalette(group));
}

CGnuPlotTitle *
CGnuPlot::
createTitle(CGnuPlotGroup *group)
{
  return (device() ? device()->createTitle(group) : new CGnuPlotTitle(group));
}

CGnuPlotCamera *
CGnuPlot::
createCamera(CGnuPlotGroup *group)
{
  return (device() ? device()->createCamera(group) : new CGnuPlotCamera(group));
}

CGnuPlotRenderer *
CGnuPlot::
renderer()
{
  return (device() ? device()->renderer() : 0);
}

void
CGnuPlot::
timeout()
{
  if (device()) device()->timeout();
}

void
CGnuPlot::
stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state)
{
  if (device()) device()->stateChanged(window, state);
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
    StringArray args;

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
addFunction2D(CGnuPlotGroup *group, const StringArray &functions, PlotStyle style)
{
  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    plot = createPlot(group, style);

    if (plot->keyTitle() == "")
      plot->setKeyTitle(functions[0]);

    plot->init();
  }

  //---

  int nx, ny;

  samples_.get(nx, ny);

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
          errorMsg("Failed to eval \'" + functions[0] + "\' for x=" + CStrUtil::toString(x));
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
addFunction3D(CGnuPlotGroup *group, const StringArray &functions, PlotStyle style)
{
  const std::string &tableFile = getTableFile();

  CGnuPlotPlot *plot = 0;

  if (tableFile.empty()) {
    plot = createPlot(group, style);

    if (plot->keyTitle() == "")
      plot->setKeyTitle(functions[0]);

    plot->init();
  }

  //---

  int nx, ny;

  isoSamples_.get(nx, ny);

  if (! polar_ && ! parametric_) {
    double xmin, ymin, xmax, ymax, zmin, zmax;

    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);
    getZRange(&zmin, &zmax);

    //---

    std::string varName1("x"), varName2("y");

    getDummyVars(varName1, varName2);

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);
    CExprVariablePtr yvar = CExprInst->createRealVariable(varName2, 0.0);

    double y  = ymin;
    double dx = (xmax - xmin)/nx;
    double dy = (ymax - ymin)/ny;

    if (functions[0] == "NaN") {
      for (int iy = 0; iy <= ny; ++iy, y += dy) {
        double x = xmin;

        for (int ix = 0; ix <= nx; ++ix, x += dx) {
          plot->addPoint3D(iy, x, y, CExprValueP());
        }
      }
    }
    else {
      CExprCTokenStack cstack = compileExpression(functions[0]);

      for (int iy = 0; iy <= ny; ++iy, y += dy) {
        yvar->setRealValue(y);

        double x = xmin;

        for (int ix = 0; ix <= nx; ++ix, x += dx) {
          xvar->setRealValue(x);

          CExprValueP value;

           if (! CExprInst->executeCTokenStack(cstack, value)) {
            errorMsg("Failed to eval " + functions[0] + " for x=" + CStrUtil::toString(x) +
                     ", y=" + CStrUtil::toString(y));
            value = CExprValueP(); // TODO
          }

          double z = 0.0;

          if (value.isValid() && value->getRealValue(z)) {
            if (plot) {
              plot->addPoint3D(iy, x, y, z);
            }
            else {
              // TODO: inside/outside test
              std::cerr << x << " " << y << " " << z << " i" << std::endl;
            }
          }
        }
      }
    }
  }
  else if (polar_) {
    // TODO
  }
  else if (parametric_) {
    // TODO
  }
  else
    assert(false);

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

  // gen file lines (one dimension)
  if      (filename == "+") {
    int nx, ny;

    samples_.get(nx, ny);

    //---

    double xmin, xmax;

    getXRange(&xmin, &xmax);

    //---

    CGnuPlotPlot *plot = createPlot(group, style);

    plot->init();

    plots.push_back(plot);

    //---

    std::string varName1("x"), varName2("t");

    getDummyVars(varName1, varName2);

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

    double x  = xmin;
    double dx = (xmax - xmin)/nx;

    for (int ix = 0; ix <= nx; ++ix, x += dx) {
      xvar->setRealValue(x);

      fields_.clear();

      fields_.push_back(CStrUtil::toString(x));

      //---

      TicLabel ticLabel;
      Values   values;
      Params   params;

      bool skip = false;

      for (uint i = 0; i < usingCols.size(); ++i) {
        bool ignore = false;

        const CGnuPlotUsingCol &col = usingCols[i];

        CExprValueP value = decodeUsingCol(i, col, 0, ix, skip, ignore, ticLabel, params);

        if (! ignore) {
          //if (! value.isValid()) bad = true;

          values.push_back(value);
        }
      }

      plot->addPoint2D(values, false, params);
    }

    return plots;
  }
  // gen file lines (two dimensions)
  else if (filename == "++") {
    int nx, ny;

    samples_.get(nx, ny);

    //---

    double xmin, xmax;
    double ymin, ymax;

    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);

    //---

    CGnuPlotPlot *plot = createPlot(group, style);

    plot->init();

    plots.push_back(plot);

    //---

    std::string varName1("x"), varName2("t");

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

        fields_.clear();

        fields_.push_back(CStrUtil::toString(x));
        fields_.push_back(CStrUtil::toString(y));

        //---

        TicLabel ticLabel;
        Values   values;
        Params   params;

        bool skip = false;

        for (uint i = 0; i < usingCols.size(); ++i) {
          bool ignore = false;

          const CGnuPlotUsingCol &col = usingCols[i];

          CExprValueP value = decodeUsingCol(i, col, 0, ix, skip, ignore, ticLabel, params);

          if (! ignore) {
            //if (! value.isValid()) bad = true;

            values.push_back(value);
          }
        }

        plot->addPoint2D(values, false, params);
      }
    }

    return plots;
  }

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
    CGnuPlotPlot *plot = createPlot(group, style);

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

    plot->init();

    plots.push_back(plot);

    //---

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

        TicLabel ticLabel;
        Values   values;
        Params   params;

        if (isMatrix()) {
          double r;

          for (const auto &field : fields_) {
            CExprValueP value;

            auto len = field.size();

            if      (field[0] == '\"' && field[len - 1] == '\"')
              value = CExprInst->createStringValue(field.substr(1, len - 2));
            else if (CStrUtil::toReal(field, &r))
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

              const CGnuPlotUsingCol &col = usingCols[i];

              CExprValueP value =
                decodeUsingCol(i, col, setNum, pointNum_, skip, ignore, ticLabel, params);

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
        int pointNum = plot->addPoint2D(values, discontinuity, params);

        if (ticLabel.valid)
          plot->setPoint2DLabel(pointNum, ticLabel.str);
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
  CGnuPlotImageStyle istyle = imageStyle();

  istyle.usingCols = usingCols;

  std::vector<CRGBA> data;

  if (imageStyle_.fileType == CGnuPlotTypes::ImageType::PNG) {
    CImageFileSrc src(filename);

    CImagePtr image = CImageMgrInst->createImage(src);
    if (! image.isValid()) return 0;

    istyle.w = image->getWidth ();
    istyle.h = image->getHeight();

    for (int y = 0; y < istyle.h; ++y) {
      for (int x = 0; x < istyle.w; ++x) {
        CRGBA rgba;

        image->getRGBAPixel(x, y, rgba);

        data.push_back(rgba);
      }
    }

    istyle.flipy = true;
  }
  else {
    // open file
    CUnixFile file(filename);
    if (! file.open()) return 0;

    uchar c;
    uchar rgb[3];
    int   i = 0;

    while (file.readChar(c)) {
      rgb[i++] = c;

      if (i == 3) {
        CRGBA rgba(rgb[0]/255.0, rgb[1]/255.0, rgb[2]/255.0);

        data.push_back(rgba);

        i = 0;
      }
    }
  }

  //---

  CGnuPlotPlot *plot = createPlot(group, style);

  plot->init();

  plot->setImageData(data);

  plot->setImageStyle(istyle);

  return plot;
}

CGnuPlot::Plots
CGnuPlot::
addFile3D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
          const CGnuPlotUsingCols &usingCols)
{
  Plots plots;

  //---

  // gen file lines (one dimension)
  if      (filename == "+") {
    int nx, ny;

    samples_.get(nx, ny);

    //---

    double xmin, xmax;

    getXRange(&xmin, &xmax);

    //---

    CGnuPlotPlot *plot = createPlot(group, style);

    plot->init();

    plots.push_back(plot);

    //---

    std::string varName1("x"), varName2("t");

    getDummyVars(varName1, varName2);

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

    double x  = xmin;
    double dx = (xmax - xmin)/nx;

    for (int ix = 0; ix <= nx; ++ix, x += dx) {
      xvar->setRealValue(x);

      fields_.clear();

      fields_.push_back(CStrUtil::toString(x));

      //---

      TicLabel ticLabel;
      Values   values;
      Params   params;

      bool skip = false;

      for (uint i = 0; i < usingCols.size(); ++i) {
        bool ignore = false;

        const CGnuPlotUsingCol &col = usingCols[i];

        CExprValueP value = decodeUsingCol(i, col, 0, ix, skip, ignore, ticLabel, params);

        if (! ignore) {
          //if (! value.isValid()) bad = true;

          values.push_back(value);
        }
      }

      plot->addPoint3D(0, values, false);
    }

    return plots;
  }
  // gen file lines (two dimensions)
  else if (filename == "++") {
    int nx, ny;

    samples_.get(nx, ny);

    //---

    double xmin, xmax;
    double ymin, ymax;

    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);

    //---

    CGnuPlotPlot *plot = createPlot(group, style);

    plot->init();

    plots.push_back(plot);

    //---

    std::string varName1("x"), varName2("t");

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

        fields_.clear();

        fields_.push_back(CStrUtil::toString(x));
        fields_.push_back(CStrUtil::toString(y));

        //---

        TicLabel ticLabel;
        Values   values;
        Params   params;

        bool skip = false;

        for (uint i = 0; i < usingCols.size(); ++i) {
          bool ignore = false;

          const CGnuPlotUsingCol &col = usingCols[i];

          CExprValueP value = decodeUsingCol(i, col, 0, ix, skip, ignore, ticLabel, params);

          if (! ignore) {
            //if (! value.isValid()) bad = true;

            values.push_back(value);
          }
        }

        plot->addPoint3D(0, values, false);
      }
    }

    return plots;
  }

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
    CGnuPlotPlot *plot = createPlot(group, style);

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

    plot->init();

    plots.push_back(plot);

    //---

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

        TicLabel ticLabel;
        Values   values;
        Params   params;

        if (isMatrix()) {
          double r;

          for (const auto &field : fields_) {
            CExprValueP value;

            auto len = field.size();

            if      (field[0] == '\"' && field[len - 1] == '\"')
              value = CExprInst->createStringValue(field.substr(1, len - 2));
            else if (CStrUtil::toReal(field, &r))
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

              const CGnuPlotUsingCol &col = usingCols[i];

              CExprValueP value =
                decodeUsingCol(i, col, setNum, pointNum_, skip, ignore, ticLabel, params);

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
              CExprValueP value1 = getFieldValue(0,  0, setNum, pointNum_, skip);
              CExprValueP value2 = getFieldValue(0, -2, setNum, pointNum_, skip);

              if (value1.isValid() && value2.isValid()) {
                values.push_back(value1);
                values.push_back(value2);

                std::swap(values[0], values[1]);
                std::swap(values[1], values[2]);
              }
            }
            // Note: two or four values values then last value is color
          }

          if      (skip)
            discontinuity = true;
          else if (bad)
            continue;
        }

        //---

        // add values
        plot->addPoint3D(0, values, discontinuity);
      }
    }
  }

  return plots;
}

int
CGnuPlot::
getColumnIndex(const std::string &str) const
{
  const CGnuPlotKeyData::Columns &columns = keyData_.columns();

  for (uint i = 0; i < columns.size(); ++i)
    if (columns[i] == str)
      return i;

  return -1;
}

CExprValueP
CGnuPlot::
decodeUsingCol(int i, const CGnuPlotUsingCol &col, int setNum, int pointNum,
               bool &skip, bool &ignore, TicLabel &ticLabel, Params &params)
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

    // check for parameter function
    pos = expr.find('(');

    if (pos > 0 && pos != std::string::npos) {
      bool valid = false;

      std::string name = expr.substr(0, pos);

      std::string name1 = expr.substr(pos + 1);

      auto p1 = name1.find(')');

      if (p1 != std::string::npos)
        name1 = name1.substr(0, p1);

      CExprValueP value;

      if (! evaluateExpression(name1, value, true))
        value = CExprValueP();

      long        icol;
      std::string str1;

      if (value.isValid() && value->getIntegerValue(icol)) {
        CExprValueP value1 = getFieldValue(i, icol, setNum, pointNum, skip);

        if (value1.isValid() && value1->getStringValue(str1))
          valid = true;
      }

      if (name == "xtic"  || name == "xticlabels"  || name == "ytic"  || name == "yticlabels" ||
          name == "x2tic" || name == "x2ticlabels" || name == "y2tic" || name == "y2ticlabels") {
        if (valid) {
          ticLabel.valid = true;
          ticLabel.str   = str1;

          if      (name == "xtic" || name == "xticlabels") {
            ticLabel.id = 'x'; ticLabel.ind = 1;
            axesData_.xaxis[ticLabel.ind].setTicLabel(pointNum, ticLabel.str);
          }
          else if (name == "x2tic"|| name == "x2ticlabels") {
            ticLabel.id = 'x'; ticLabel.ind = 2;
            axesData_.xaxis[ticLabel.ind].setTicLabel(pointNum, ticLabel.str);
          }
          else if (name == "ytic" || name == "yticlabels") {
            ticLabel.id = 'y'; ticLabel.ind = 1;
            axesData_.yaxis[ticLabel.ind].setTicLabel(pointNum, ticLabel.str);
          }
          else if (name == "y2tic"|| name == "y2ticlabels") {
            ticLabel.id = 'y'; ticLabel.ind = 2;
            axesData_.yaxis[ticLabel.ind].setTicLabel(pointNum, ticLabel.str);
          }
        }
      }
      else {
        if (valid)
          params[name] = str1;
      }

      ignore = true;
      expr   = "";
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
    const std::string &missing = dataFile_.getMissingStr();

    const std::string &field = fields_[ival - 1];

    auto len = field.size();

    if (field == missing) {
      skip = true;
      return CExprValueP();
    }

    double val;

    if      ((i == 0 && axesData_.xaxis[1].isTime()) ||
             (i == 1 && axesData_.yaxis[1].isTime())) {
      struct tm tm1; memset(&tm1, 0, sizeof(tm));

      strptime(field.c_str(), timeFmt().c_str(), &tm1);

      value = CExprInst->createRealValue(mktime(&tm1));
    }
    else if (field[0] == '\"' && field[len - 1] == '\"')
      value = CExprInst->createStringValue(field.substr(1, len - 2));
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

  // allow for extra chars after number

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
      lineStyle->init(ind);

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

void
CGnuPlot::
resetLineStyleInd(int ind)
{
  lineStyles_.erase(ind);
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

void
CGnuPlot::
setAngleType(AngleType type)
{
  setPrefValue<AngleType>(VariableName::ANGLES, type);

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

void
CGnuPlot::
resetDummyVars()
{
  dummyVars_.clear();
}

// set tics {axis | border} {{no}mirror}
//          {in | out} {front | back}
//          {scale {default | <major> {,<minor>}}}
//          {{no}rotate {by <ang>}} {offset <offset> | nooffset}
//          {left | right | center | autojustify}
//          {add}
//          {autofreq | <incr> | <start>, <incr> {,<end>} |
//           ({"<label>"} <pos> {<level>} {,{"<label>"}...)}}
//          {format "formatstring"}
//          {font "name{,<size>}"} {{no}enhanced}
//          {numeric | timedate | geographic}
//          {rangelimited}
//          {textcolor <colorspec>}
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
    if      (arg == "axis"  ) {
      // TODO
    }
    else if (arg == "border") {
      // TODO
    }
    else if (arg == "mirror" || arg == "nomirror") {
      axis.setMirror(arg == "mirror");
    }
    else if (arg == "in" || arg == "out") {
      // TODO
      //axis.setTicsIn(arg == "in");
    }
    else if (arg == "front" || arg == "back") {
      axis.setFront(arg == "front");
    }
    else if (arg == "scale") {
      // TODO
      std::string arg1 = readNonSpaceNonComma(line);

      if (line.skipSpaceAndChar(','))
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "rotate") {
      // TODO
      std::string arg1 = readNonSpaceNonComma(line);

      if (arg1 == "by")
        arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "norotate") {
      // TODO
    }
    else if (arg == "offset") {
      // TODO
      std::string arg1 = readNonSpaceNonComma(line);
    }
    else if (arg == "nooffset") {
      // TODO
    }
    else if (arg == "left" || arg == "right" ||  arg == "center" || arg == "autojustify") {
      // TODO
      //setTicJustify(arg != "left" ?
      //  (arg != "right" ? (arg != "center" ? AUTOJUSTIFY : CENTER) : RIGHT) : LEFT);
    }
    else if (arg == "add") {
      // TODO
    }
    else if (arg == "autofreq") {
      // TODO
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
    else if (arg == "enhanced" || arg == "noenhanced") {
      axis.setEnhanced(arg == "enhanced");
    }
    else if (arg == "rangelimited") {
      // TODO
    }
    else if (arg == "textcolor" || arg == "tc") {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        axis.setTextColor(c);
    }
    else if (arg == "linetype" || arg == "lt") {
      int lt;

      if (parseInteger(line, lt)) {
        axis.setLineType(lt);
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

// set {x|x2|y|y2|z}zeroaxis { {linestyle | ls <line_style>} |
//                             {linetype | lt <line_type>} { linewidth | lw <line_width>}}
void
CGnuPlot::
parseAxisZeroAxis(CParseLine &line, CGnuPlotAxisData &axis)
{
  axis.setZeroAxisDisplayed(true);

  std::string arg = readNonSpace(line);

  if      (arg == "linestyle" || arg == "ls") {
    int ls = -1;

    if (parseInteger(line, ls))
      axis.setZeroAxisLineStyle(ls);
  }
  else if (arg == "linetype" || arg == "lt") {
    int lt;

    if (parseInteger(line, lt))
      axis.setZeroAxisLineType(lt);
  }
  else if (arg == "linewidth" || arg == "lw") {
    double lw;

    if (parseReal(line, lw))
      axis.setZeroAxisLineWidth(lw);
  }
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
    int pos = line.pos();

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
    else {
      c.setPaletteVariable();

      line.setPos(pos);
    }
  }
  else if (arg == "variable" || arg == "var") {
    c.setVariable();
  }
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
parseInteger(CParseLine &line, int &i) const
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
getIntegerVariable(const std::string &name, int &value) const
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
getRealVariable(const std::string &name, double &value) const
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
parseReal(CParseLine &line, double &r) const
{
  line.skipSpace();

  int pos = line.pos();

  if (line.isChar('(')) {
    std::string expr;

    if (! parseBracketedString(line, expr)) {
      line.setPos(pos);
      return false;
    }

    CExprValueP value;

    if (! evaluateExpression(expr, value))
      value = CExprValueP();

    if (! value.isValid() || ! value->getRealValue(r)) {
      line.setPos(pos);
      return false;
    }

    return true;
  }
  else {
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
  }

  return true;
}

bool
CGnuPlot::
parseString(CParseLine &line, std::string &str, const std::string &msg) const
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

    if (! readIdentifier(line, id)) {
      line.setPos(pos);

      if (msg != "")
        errorMsg(msg);

      return false;
    }

    if (line.isChar('(')) {
      std::string str1;

      if (! parseBracketedString(line, str1)) {
        line.setPos(pos);
        return false;
      }

      std::string expr = id + str1;

      CExprValueP value;

      if (! evaluateExpression(expr, value))
        value = CExprValueP();

      if (! value.isValid() || ! value->getStringValue(str)) {
        line.setPos(pos);

        return false;
      }
    }
    else {
      if (! getStringVariable(id, str)) {
        line.setPos(pos);

        if (msg != "")
          errorMsg(msg);

        return false;
      }
    }
  }

  return true;
}

void
CGnuPlot::
skipString(CParseLine &line)
{
  if      (line.skipSpaceAndChar('"')) {
    while (line.isValid() && ! line.isChar('"')) {
      char c = line.getChar();

      if (c == '\\') {
        if (line.isValid())
          line.skipChar();
      }
    }

    if (line.isChar('"'))
      line.skipChar();
  }
  else if (line.skipSpaceAndChar('\'')) {
    while (line.isValid() && ! line.isChar('\''))
      line.skipChar();

    if (line.isChar('\''))
      line.skipChar();
  }
}

bool
CGnuPlot::
getStringVariable(const std::string &name, std::string &value) const
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

  if (parseOptionValue(this, line, system))
    pos.setSystemX(system);
  else
    pos.setSystemX(CGnuPlotTypes::CoordSys::FIRST);

  double x;

  if (! parseReal(line, x))
    return false;

  if (! line.skipSpaceAndChar(','))
    return false;

  if (parseOptionValue(this, line, system))
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

  if (parseOptionValue(this, line, system))
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

  if (parseOptionValue(this, line, system))
    size.setSystemX(system);
  else
    size.setSystemX(CGnuPlotTypes::CoordSys::FIRST);

  double x;

  if (! parseReal(line, x))
    return false;

  if (! line.skipSpaceAndChar(','))
    return false;

  if (parseOptionValue(this, line, system))
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
parseBracketedString(CParseLine &line, std::string &str) const
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
skipBracketedString(CParseLine &line) const
{
  if (! line.isChar('('))
    return false;

  line.skipChar();

  int brackets = 1;

  while (line.isValid()) {
    if      (line.isChar('('))
      ++brackets;
    else if (line.isChar(')'))
      --brackets;

    line.skipChar();

    if (brackets == 0)
      break;
  }

  return true;
}

bool
CGnuPlot::
readIdentifier(CParseLine &line, std::string &id) const
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

    CExprInst->createIntegerVariable("GPVAL_LINENO", fileData_.lineNum);
  }

  return true;
}

void
CGnuPlot::
errorMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;

  CExprInst->createStringVariable("GPVAL_ERRMSG" , msg);
}

CPoint3D
CGnuPlot::
sphericalMap(const CPoint2D &p) const
{
  static double rad = 6378137.0;          // Radius of the Earth (in meters)
  static double f   = 1.0/298.257223563;  // Flattening factor WGS84 Model

  double alt = 0.0; // altitude

  double cosLon = cos(angleToRad(p.x));
  double sinLon = sin(angleToRad(p.x));
  double cosLat = cos(angleToRad(p.y));
  double sinLat = sin(angleToRad(p.y));

  double ff = (1.0 - f)*(1.0 - f);
  double c  = 1.0/sqrt(cosLat*cosLat + ff*sinLat*sinLat);
  double s  = c*ff;

  double x = (rad*c + alt)*cosLat*cosLon;
  double y = (rad*c + alt)*cosLat*sinLon;
  double z = (rad*s + alt)*sinLat;

  return CPoint3D(x, y, z);
}

double
CGnuPlot::
angleToRad(double a) const
{
  return (getAngleType() == CGnuPlotTypes::AngleType::DEGREES ? CAngle::Deg2Rad(a) : a);
}

//-----

void
CGnuPlot::BorderData::
show(std::ostream &os) const
{
  if (! sides)
    os << "border is not drawn" << std::endl;
  else
    os << "border " << sides << " is drawn in" <<
          CStrUniqueMatch::valueToString<DrawLayer>(layer) << " layer with" <<
          " linecolor " << lineStyle <<
          " linewidth " << lineWidth <<
          " lineType " << lineType << std::endl;
}

void
CGnuPlot::BorderData::
save(std::ostream &os) const
{
  os << "set border " << sides << " " <<
        CStrUniqueMatch::valueToString<DrawLayer>(layer) <<
        " lt " << lineType <<
        " linewidth " << lineWidth <<
        " dashtype solid" << std::endl;
}
