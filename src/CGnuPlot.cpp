#include <CGnuPlot.h>
#include <CGnuPlotWindow.h>
#include <CGnuPlotGroup.h>
#include <CGnuPlotPlot.h>
#include <CGnuPlotDevice.h>
#include <CGnuPlotContour.h>
#include <CGnuPlotStyle.h>
#include <CGnuPlotSVGDevice.h>
#include <CGnuPlotLogDevice.h>
#include <CGnuPlotUtil.h>
#include <CGnuPlotNameValues.h>
#include <CGnuPlotFunctions.h>
#include <CGnuPlotUnixFile.h>

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
#include <CGnuPlotStyleErrorBars.h>
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

#include <CUtf8.h>
#include <RangeItr.h>
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
#include <CAutoFree.h>

#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>
#include <sstream>

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

  void addSpaceSep(std::string &str, const std::string &text) {
    if (str.size() > 0 && ! isspace(str[str.size() - 1]))
      str += " ";

    str += text;
  }
}

//------

CGnuPlot::
CGnuPlot() :
 printFile_(this, true), tableFile_(this, false)
{
  varPrefs_[VariableName::ANGLES] =
    new CGnuPlotPrefValue<AngleType>("Angle Type", "angles", AngleType::RADIANS);

  CExprInst->createRealVariable("pi" , M_PI);
  CExprInst->createRealVariable("NaN", CMathGen::getNaN());

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

  CGnuPlotFunctions::init(this);

  svgDevice_ = new CGnuPlotSVGDevice();
  logDevice_ = new CGnuPlotLogDevice();

  addDevice("svg", svgDevice_);
  addDevice("log", logDevice_);

  for (const auto &i : IRangeItr(0, 8))
    paxis(i).unset();

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
  addPlotStyle(PlotStyle::ERRORBARS     , new CGnuPlotStyleErrorBars     );
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
  addPlotStyle(PlotStyle::XERRORLINES   , new CGnuPlotStyleXErrorLines   );
  addPlotStyle(PlotStyle::XYERRORBARS   , new CGnuPlotStyleXYErrorBars   );
  addPlotStyle(PlotStyle::XYERRORLINES  , new CGnuPlotStyleXYErrorLines  );
  addPlotStyle(PlotStyle::YERRORBARS    , new CGnuPlotStyleYErrorBars    );
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
      device_ = d.second;

      CExprInst->createStringVariable("GPVAL_TERM", device_->name());

      return true;
    }
  }

  return false;
}

void
CGnuPlot::
addWindow(CGnuPlotWindowP window)
{
  device_->addWindow(window);
}

void
CGnuPlot::
removeWindow(CGnuPlotWindow *window)
{
  device_->removeWindow(window);
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

    for (const auto &i : IRangeItr::range(0, 9)) {
      std::string argName = CStrUtil::strprintf("ARG%d", i + 1);

      if (i < int(args.size()))
        CExprInst->createStringVariable(argName, args[i]);
      else
        CExprInst->createStringVariable(argName, "");
    }
  }
  else {
    CExprInst->createIntegerVariable("ARGC", 0);

    for (const auto &i : IRangeItr::rangeClosed(1, 9)) {
      std::string argName = CStrUtil::strprintf("ARG%d", i);

      CExprInst->removeVariable(argName);
    }
  }

  //---

  // open file
  CUnixFile *file = new CUnixFile(filename);

  CAutoFree<CUnixFile> freeFile(file);

  if (! file->open())
    return false;

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
initReadLine() const
{
  if (! readLine_.isValid()) {
    CGnuPlot *th = const_cast<CGnuPlot *>(this);

    readLine_ = new CGnuPlotReadLine(th);

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

void
CGnuPlot::
waitForMouse(int mask, const std::string &msg)
{
  initReadLine();

  readLine_->setPrompt(msg);

  interruptMask_ = mask;

  (void) readLine_->readLineInterruptable();
}

void
CGnuPlot::
mousePress(const CGnuPlotMouseEvent &mouseEvent)
{
  if (readLine_.isValid() && ! readLine_->isInterruptable())
    return;

  bool interrupt = false;

  if ((interruptMask_ & int(CGnuPlotTypes::PauseMask::BUTTON1)) && mouseEvent.button() == 1)
    interrupt = true;

  if ((interruptMask_ & int(CGnuPlotTypes::PauseMask::BUTTON2)) && mouseEvent.button() == 2)
    interrupt = true;

  if ((interruptMask_ & int(CGnuPlotTypes::PauseMask::BUTTON3)) && mouseEvent.button() == 3)
    interrupt = true;

  if (! interrupt)
    return;

  if (readLine_.isValid())
    readLine_->doInterrupt();
}

void
CGnuPlot::
keyPress(const CGnuPlotKeyEvent &)
{
  if (readLine_.isValid() && ! readLine_->isInterruptable())
    return;

  bool interrupt = false;

  if (interruptMask_ & int(CGnuPlotTypes::PauseMask::KEY))
    interrupt = true;

  if (! interrupt)
    return;

  if (readLine_.isValid())
    readLine_->doInterrupt();
}

bool
CGnuPlot::
parseLine(const std::string &str)
{
  CParseLine line(str);

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

  line1.setPos(0);

  Statements statements;

  getStatements(line1, statements);

  //----

  for (int i = 0; i < int(statements.size()); ++i) {
    // NOTE: will update i if skip statements (e.g. if)
    if (! parseStatement(i, statements))
      return false;
  }

  return true;
}

void
CGnuPlot::
getStatements(CParseLine &line, Statements &statements) const
{
  int braces = 0;

  std::string str2;

  while (line.isValid()) {
    if      (line.isChar('\"') || line.isChar('\'')) {
      int pos = line.pos();

      skipString(line);

      str2 +=  line.substr(pos, line.pos() - pos);
    }
    else if (line.isChar(';')) {
      if (braces == 0) {
        line.skipChar();

        if (str2 != "")
          statements.push_back(str2);

        str2 = "";
      }
      else
        str2 += line.getChar();
    }
    else if (line.isChar('{')) {
      str2 += line.getChar();

      ++braces;
    }
    else if (line.isChar('}')) {
      str2 += line.getChar();

      --braces;
    }
    else
      str2 += line.getChar();
  }

  if (str2 != "")
    statements.push_back(str2);
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
      line.ungetChar();

      std::string str2;

      (void) replaceEmbeddedCmd(line, str2);

      str1 += str2;
    }
    else if (c == '\'') {
      line.ungetChar();

      std::string str2;

      replaceEmbeddedString(line, str2);

      str1 += "\'";
      //for (const auto &c : str2) { if (c == '\'') str1 += "\\"; str1 += c; }
      str1 += str2;
      str1 += "\'";
    }
    else if (c == '\"') {
      line.ungetChar();

      std::string str2;

      replaceEmbeddedString(line, str2);

      str1 += "\"";
      //for (const auto &c : str2) { if (c == '\"') str1 += "\\"; str1 += c; }
      str1 += str2;
      str1 += "\"";
    }
    else if (c == '@') {
      std::string str2;

      while (line.isValid() && (line.isAlNum() || line.isChar('_'))) {
        char c1 = line.getChar();

        str2 += c1;
      }

      std::string str3;

      if (isMacros() && getStringVariable(str2, str3))
        str1 += replaceEmbedded(str3);
      else
        str1 = "@" + str2;
    }
    else if (c == '#') {
      // TODO: custom comment char
      break;
    }
    else
      str1 += c;
  }

  return str1;
}

void
CGnuPlot::
replaceEmbeddedString(CParseLine &line, std::string &str) const
{
  str = "";

  if      (line.skipSpaceAndChar('"')) {
    while (line.isValid() && ! line.isChar('"')) {
      char c = line.getChar();

      if      (c == '\\') {
        line.ungetChar();

        std::string str1;

        if (replaceEscapeChar(line, str1))
          str += str1;
        else
          str += line.getChar();
      }
      else if (c == '`') {
        line.ungetChar();

        std::string str1;

        (void) replaceEmbeddedCmd(line, str1);

        str += str1;
      }
      else
        str += c;
    }

    if (line.isChar('"'))
      line.skipChar();
  }
  else if (line.skipSpaceAndChar('\'')) {
    while (line.isValid() && ! line.isChar('\'')) {
      char c = line.getChar();

      str += c;
    }

    if (line.isChar('\''))
      line.skipChar();
  }
}

bool
CGnuPlot::
replaceEscapeChar(CParseLine &line, std::string &str) const
{
  if (! line.isValid())
    return false;

  if (! line.isChar('\\'))
    return false;

  int pos = line.pos();

  line.skipChar();

  if (! line.isValid()) {
    line.setPos(pos);
    return false;
  }

  char c = line.getChar();

  switch (c) {
    case 't'  : str += '\t'  ; break;
    case 'n'  : str += '\n'  ; break;
    case '\"' : str += "\\\""; break;
    case '\'' : str += '\''  ; break;
    case '`'  : str += '`'   ; break;

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7': {
      line.ungetChar();

      int i = 0;

      while (line.isODigit()) {
        c = line.getChar();

        i = i*8 + (c - '0');
      }

      char c1 = char(i);

      if (c1 == '\\' || c1 == '\"')
        str += "\\";

      str += c1;

      break;
    }
    default: {
      str += '\\';
      str += c;
      break;
    }
  }

  return true;
}

bool
CGnuPlot::
replaceEmbeddedCmd(CParseLine &line, std::string &str) const
{
  line.skipChar(); // `

  std::string str1;

  while (line.isValid()) {
    char c1 = line.getChar();

    if (c1 == '`')
      break;

    str1 += c1;
  }

  StringArray lines;

  bool rc = execCmd(str1, lines);

  for (const auto &line : lines)
    str += line;

  return rc;
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
  line.skipSpace();

  int pos = line.pos();

  std::string identifier;

  (void) readIdentifier(line, identifier);

  // empty line is ok
  if (identifier.empty()) {
    if (line.substr() == "")
      return true;

    if (line.skipSpaceAndChar('$')) {
      std::string name = readNonSpace(line);

      if (name == "") {
        errorMsg("Missing block name");
        return false;
      }

      line.skipSpace();

      if (! line.isChars("<<")) {
        errorMsg("Missing block << for '" + name + "'");
        return false;
      }

      line.skipChars(2);

      line.skipSpace();

      std::string eofStr = line.substr();

      if (! readNamedBlock(name, eofStr)) {
        errorMsg("Failed to read block '" + name + "'");
        return false;
      }
    }
    else {
      line.setPos(pos);
      errorMsg("Invalid statement '" + line.substr());
      return false;
    }

    return true;
  }

  bool keyword = (identifier == "if" || identifier == "while");

  //---

  if (! keyword) {
    int pos = line.pos();

    // function definition
    if      (line.skipSpaceAndChar('(')) {
      if (parseFunctionDef(identifier, line))
        return true;

      line.setPos(pos);
    }
    // variable assignment
    else if (line.skipSpaceAndChar('=')) {
      if (parseVariableDef(identifier, line))
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

    case CommandName::DATA    : dataCmd   (args); break;
    case CommandName::WINDOW  : windowCmd (args); break;
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

bool
CGnuPlot::
parseFunctionDef(const std::string &identifier, CParseLine &line)
{
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

  std::string expr = replaceSumInExpr(line.substr());

  expr = CStrUtil::stripSpaces(expr);

  CExprInst->addFunction(identifier, args, expr);

  std::string gvar = "GPFUN_" + identifier;

  std::string gargs = "";

  for (const auto &arg : args) {
    if (gargs != "") gargs += ",";
    gargs + arg;
  }

  std::string gfunc = identifier + "(" + gargs + ") = " + expr;

  CExprInst->createStringVariable(gvar, gfunc);

  return true;
}

bool
CGnuPlot::
parseVariableDef(const std::string &identifier, CParseLine &line)
{
  auto expr = line.substr();

#if 0
  // TODO: comment should already be stripped
  auto p = expr.find('#');

  if (p != std::string::npos)
    expr = expr.substr(0, p);
#endif

  CExprValuePtr value;

  if (! evaluateExpression(expr, value))
    value = CExprValuePtr();

  if (! value.isValid())
    return false;

  CExprInst->createVariable(identifier, value);

  return true;
}

// exit
void
CGnuPlot::
exitCmd(const std::string &args)
{
  if (isDebug()) debugMsg("exit " + args);

  exit(0);
}

// help [{topic}]
void
CGnuPlot::
helpCmd(const std::string &args)
{
  if (isDebug()) debugMsg("help " + args);

  CParseLine line(args);

  line.skipSpace();

  std::string arg = readNonSpace(line);

  if      (arg == "commands") {
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
  else if (arg == "variables") {
    StringArray names;

    CExprInst->getVariableNames(names);

    std::string str;

    for (const auto &name : names) {
      if (str != "") str += " ";

      str += name;
    }

    std::cout << str << std::endl;
  }
  else if (arg == "functions") {
    StringArray names;

    CExprInst->getFunctionNames(names);

    std::string str;

    for (const auto &name : names) {
      if (str != "") str += " ";

      str += name;
    }

    std::cout << str << std::endl;
  }
}

// history [quiet] [{max:i}] ["<filename>" [append]]
// history ?"<cmd>"
// history !"<cmd>"
void
CGnuPlot::
historyCmd(const std::string &args)
{
  if (isDebug()) debugMsg("history " + args);

  // TODO: process args
  if (! readLine_.isValid())
    return;

  CReadLine::HistoryEntries entries;

  readLine_->getHistoryEntries(entries);

  for (const auto &e : entries)
    std::cout << e.line_num << ": " << e.line << std::endl;
}

// print {expression} [, {expression} ]
void
CGnuPlot::
printCmd(const std::string &args)
{
  if (isDebug()) debugMsg("print " + args);

  CParseLine line(args);

  line.skipSpace();

  if (line.isChar('$')) {
    line.skipChar();

    std::string blockName = readNonSpace(line);

    CGnuPlotBlock *block = getBlock(blockName);

    if (block)
      printFile_.print(block->str());
    else
      errorMsg("invalid block '" + blockName + "'");
  }
  else {
    std::ostringstream ss;

    CExprValueType lastType = CEXPR_VALUE_STRING;

    while (line.isValid()) {
      CExprValuePtr value;

      if (! parseValue(line, value, "Invalid Value"))
        return;

      CExprValueType thisType = value->getType();

      if (lastType != CEXPR_VALUE_STRING && thisType != CEXPR_VALUE_STRING)
        ss << " ";

      ss << value;

      lastType = thisType;

      if (! line.skipSpaceAndChar(','))
        break;
    }

    if (line.substr() != "")
      errorMsg("extra characters after print expression '" + line.substr() + "'");

    ss << std::endl;

    printFile_.print(ss.str());
  }
}

// printf <format> {expression} [, {expression} ]
void
CGnuPlot::
printfCmd(const std::string &args)
{
  if (isDebug()) debugMsg("printf " + args);

  CParseLine line(args);

  std::string fmt;

  if (! parseString(line, fmt)) {
    errorMsg("Invalid format");
    return;
  }

  Values values;

  if (line.skipSpaceAndChar(',')) {
    std::string expr = line.substr();

    if (! evaluateExpression(expr, values)) {
      errorMsg("Invalid expression");
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
  if (isDebug()) debugMsg("quit " + args);

  exit(0);
}

// cd "<path"
void
CGnuPlot::
cdCmd(const std::string &args)
{
  if (isDebug()) debugMsg("cd " + args);

  CParseLine line(args);

  std::string str;

  if (! parseString(line, str)) {
    errorMsg("Invalid directory name");
    return;
  }

  str = CStrUtil::stripSpaces(str);

  if (str.empty()) {
    errorMsg("Missing directory name");
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
  if (isDebug()) debugMsg("pwd " + args);

  std::cout << CDir::getCurrent() << std::endl;
}

// call "<filename>" [ {val}, ... ]
void
CGnuPlot::
callCmd(const std::string &args)
{
  if (isDebug()) debugMsg("call " + args);

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
  if (isDebug()) debugMsg("load " + args);

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
  if (isDebug()) debugMsg("save " + args);

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

  axesData_.saveBorder(os);

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

  xaxis(1).showFormat(os, "x");
  yaxis(1).showFormat(os, "y");
  xaxis(2).showFormat(os, "x2");
  yaxis(2).showFormat(os, "y2");
  zaxis(1).showFormat(os, "z");

  colorBox().axis().showFormat(os, "cb");

  raxis().showFormat(os, "r");

  if (timeFmt().isValid())
    os << "set timefmt \"" << timeFmt().getValue() << "\"" << std::endl;

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

void
CGnuPlot::
dataCmd(const std::string &args)
{
  if (isDebug()) debugMsg("data " + args);

  CParseLine line(args);

  std::string filename;

  if (! parseString(line, filename)) {
    errorMsg("Invalid filename");
    return;
  }

  if (filename == "") {
    errorMsg("Invalid filename");
    return;
  }

  bool columnheaders = false;
  bool binary        = false;
  bool matrix        = false;

  Sizes                sizes;
  CGnuPlotBinaryFormat binaryFormat;

  line.skipSpace();

  int pos = line.pos();

  std::string arg = readNonSpace(line);

  while (arg != "") {
    if      (arg == "column" || arg == "columnhead" ||
             arg == "columnheader" || arg == "columnheaders") {
      columnheaders = true;
    }
    else if (arg == "matrix") {
      matrix = true;
    }
    else if (arg == "binary") {
      binary = true;
    }
    else {
      line.setPos(pos);

      if      (line.isChars("array=")) {
        line.skipChars(6);

        parseArrayValues(line, sizes);
      }
      // format=
      else if (line.isChars("format=")) {
        line.skipChars(7);

        std::string str;

        (void) parseString(line, str, "Invalid format");

        binaryFormat.setFormat(str);
      }
      else
        warnMsg("Invalid arg '" + line.substr() + "'");
    }

    line.skipSpace();

    pos = line.pos();

    arg = readNonSpace(line);
  }

  dataFile_.reset();

  dataFile_.setColumnHeaders(columnheaders);

  dataFile_.setBinary(binary);
  dataFile_.setMatrix(matrix);
  dataFile_.setBinaryFormat(binaryFormat);

  if (! sizes.empty())
    dataFile_.setBinarySize(sizes[0]);

  if (! readDataFile(filename, dataFile_))
    return;

  dataFile_.processFile();

  setLastFilename(filename);
}

void
CGnuPlot::
windowCmd(const std::string &)
{
  createNewWindow();
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
//      {smooth [ unique | frequency | bezier | sbezier | csplines | acsplines | cumulative] }
//      {volatile} {noautoscale}
//      {axes [x1y1 | x1y2 | x2y1 | x2y2 ] }
//      {title [ "{str:explanation}" | {int:col} ] | notitle }
//      { with {stylespec} }
//      [, ...]
void
CGnuPlot::
plotCmd(const std::string &args)
{
  if (isDebug()) debugMsg("plot " + args);

  lastPlotCmd_  = args;
  lastSPlotCmd_ = "";

  setLastFilename("");

  xind_ = 1;
  yind_ = 1;
  zind_ = 1;

  saveAxisData();

  //---

  resetLineStyle();

  COptBBox2D clearRect = clearRect_;

  clearRect_.setInvalid();

  clearNewHistogramDatas();

  setHistogramPointOffset(0);

  //---

  // TODO: update local line style copy

  CGnuPlotWindowP window;
  bool            newWindow = false;

  if (multiplot_.isValid() && multiplot_->isEnabled()) {
    // set multi-window if doesn't exist
    if (! device_->multiWindow()) {
      // create or reuse existing window
      if (! device_->windows().empty()) {
        for (auto &w : device_->windows()) {
          w->clear();

          window = w;
        }
      }
      else {
        window    = CGnuPlotWindowP(createWindow());
        newWindow = true;
      }

      device_->setMultiWindow(window);

      device_->multiWindow()->setBackgroundColor(backgroundColor());
      device_->multiWindow()->set3D(false);
    }
    else
      window = device_->multiWindow();

    // set region (default to multiplot grid cell)
    int n = window->numGroups();

    double dx = multiplot_->dx();
    double dy = multiplot_->dy();

    CIPoint2D pos = multiplot_->pos(n);

    double x1 = plotSize_.xo.getValue(pos.x*dx);
    double y1 = plotSize_.yo.getValue(pos.y*dy);
    double x2 = x1 + plotSize_.xsize.getValue(dx);
    double y2 = y1 + plotSize_.ysize.getValue(dy);

    region_ = CBBox2D(x1, y1, x2, y2);
  }
  else {
    // create or reuse existing window
    if (! device_->windows().empty()) {
      for (auto &w : device_->windows()) {
        w->clear();

        window = w;
      }
    }
    else {
      window    = CGnuPlotWindowP(createWindow());
      newWindow = true;
    }

    window->setBackgroundColor(backgroundColor());
    window->set3D(false);

    // set region (default to full screen)
    double x1 = plotSize_.xo.getValue(0.0);
    double y1 = plotSize_.yo.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region_ = CBBox2D(x1, y1, x2, y2);
  }

  if (newWindow)
    addWindow(window);

  //----

  StringArray cmds;

  splitPlotCmd(args, cmds, false);

  //---

  CGnuPlotGroupP group(createGroup(window.get()));

  group->set3D(false);
  group->setPolar(isPolar());

  Plots plots;
  bool  sample = false, first = true;

  for (const auto &cmd : cmds) {
    plotCmd1(cmd, group, plots, sample, first);

    first = false;
  }

  //---

  if (! plots.empty()) {
    group->addObjects();

    group->init();
    group->set3D(false);

    group->setClearRect(clearRect);

    group->setTitleData(title());

    group->addSubPlots(plots);

    window->addGroup(group);

    for (auto plot : plots)
      plot->smooth();

    group->fit();

    for (auto plot : plots) {
      if (plot->tableFile().isEnabled())
        plot->printValues();
    }

    //---

    group->saveRange();

    clearTicLabels();

    //---

    if (device())
      stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
  }

  //---

  restoreAxisData();
}

void
CGnuPlot::
saveAxisData()
{
  saveAxesData_   = axesData_;
  saveSampleVars_ = sampleVars_;
}

void
CGnuPlot::
restoreAxisData()
{
  axesData_   = saveAxesData_;
  sampleVars_ = saveSampleVars_;
}

void
CGnuPlot::
plotCmd1(const std::string &args, CGnuPlotGroupP &group, Plots &plots,
         bool &sample, bool &first)
{
  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return;

    plotForCmd(forCmd, cmd, group, plots);

    return;
  }

  //----

  bool first1 = false;

  std::swap(first, first1);

  // newhistogram {"<title>"} {lt <linetype>} {fs <fillstyle>} {at <x-coord>}
  if (line.isString("newhistogram")) {
    CGnuPlotNewHistogramData newHistogramData;

    line.skipNonSpace();

    std::string histTitle;

    if (parseString(line, histTitle))
      newHistogramData.setTitle(histTitle);

    line.skipSpace();

    if      (line.isString("lt")) {
      int lt;

      if (parseLineType(line, lt))
        newHistogramData.setLineType(lt);
    }
    else if (line.isString("fs")) {
      CGnuPlotFillStyle fillStyle;

      if (parseFillStyle(line, fillStyle))
        newHistogramData.setFillStyle(fillStyle);
    }
    else if (line.isString("at")) {
      double x;

      if (parseReal(line, x))
        newHistogramData.setX(x);
    }

    newHistogramData.setInd(newHistogramDatas_.size());

    newHistogramDatas_.push_back(newHistogramData);

    //---

    int histId = newHistogramId();

    if (histId > 0) {
      std::map<int,int> histNumPoints;

      for (const auto &plot : plots)
        histNumPoints[plot->newHistogramId()] = plot->numPoints2D();

      int np = 0;

      for (const auto &hnp : histNumPoints) {
        if (hnp.first < histId)
          np += hnp.second;
      }

      setHistogramPointOffset(np);
    }

    return;
  }

  //----

  incLineStyle();

  //----

  // get local copy of next line style
  CGnuPlotLineStyle lineStyle = *this->lineStyle();
  CGnuPlotFillStyle fillStyle = this->fillStyle();
  CGnuPlotStyleData styleData = this->styleData();

  //----

  // Get global range (all plots)
  if (first1) {
    if      (isParametric()) {
      // Get Range
      //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX]
      parseAxisRange(line, taxis(1    ), false);
      parseAxisRange(line, xaxis(xind_), false);
      parseAxisRange(line, yaxis(yind_), false);
    }
    else if (isPolar()) {
      // Get Range
      //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX]
      parseAxisRange(line, taxis(1    ), false);
      parseAxisRange(line, xaxis(xind_), false);
      parseAxisRange(line, yaxis(yind_), false);
    }
    else {
      // Get Range
      //  [XMIN:XMAX][YMIN:YMAX]
      parseAxisRange(line, xaxis(xind_), false);
      parseAxisRange(line, yaxis(yind_), false);
    }

    //----

    // if sample specified then check for per-plot range
    if (line.isString("sample")) {
      line.skipNonSpace();

      sample = true;
    }
  }

  //----

  if (! first1 || sample) {
    CGnuPlotAxisData sampleXAxis;

    if (parseAxisRange(line, sampleXAxis, false)) {
      sampleVars_.x.var = sampleXAxis.getDummyVar();
      sampleVars_.x.min = sampleXAxis.min();
      sampleVars_.x.max = sampleXAxis.max();
    }

    CGnuPlotAxisData sampleYAxis;

    if (parseAxisRange(line, sampleYAxis, false)) {
      sampleVars_.y.var = sampleYAxis.getDummyVar();
      sampleVars_.y.min = sampleYAxis.min();
      sampleVars_.y.max = sampleYAxis.max();
    }
  }

  //----

  CGnuPlotKeyTitle keyTitle;

  fieldValues_.clear();

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
      filename = lastFilename();
    }
    // special filename '-' (read lines from stdin)
    else if (filename == "-") {
    }
    // special filename '+' (expression file)
    else if (filename == "+") {
    }
    // special filename '++' (expression file)
    else if (filename == "++") {
    }

    if (isDebug())
      debugMsg("Filename: " + filename);

    setLastFilename(filename);

    style = dataStyle();
  }
  else if (line.isChar('$')) {
    line.skipChar();

    std::string name = readNonSpace(line);

    filename = "$" + name;

    setLastFilename(filename);

    style = dataStyle();
  }
  else {
    // any number of assigns followed by single function
    FunctionData functionData;

    if (! parseFunction(line, functionData)) {
      errorMsg("missing function");
      return;
    }

    while (functionData.isAssign) {
      processAssignFunction(functionData.assign);

      if (! line.skipSpaceAndChar(',')) {
        errorMsg("missing comma after assign and before function");
        return;
      }

      if (! parseFunction(line, functionData)) {
        errorMsg("missing function");
        return;
      }
    }

    functions.push_back(functionData.function);

    if (isParametric()) {
      if (line.skipSpaceAndChar(',')) {
        FunctionData functionData1;

        if (! parseFunction(line, functionData1)) {
          errorMsg("missing function");
          return;
        }

        if (! functionData1.isAssign)
          functions.push_back(functionData1.function);
        else
          processAssignFunction(functionData1.assign);
      }
    }

    if (isDebug()) {
      for (const auto &f : functions)
        debugMsg("Function: " + f);
    }

    style = functionStyle();
  }

  line.skipSpace();

  //---

  CGnuPlotUsingCols usingCols(this);

  dataFile_.reset();

  imageStyle_  .reset();
  binaryFormat_.clear();

  setSmooth(Smooth::NONE);

  setBinary(false);

  matrixData().reset();

  // Read individual plot:
  //   <plot_data> [, <plot_data> ...]
  // TODO: using must come before title and with
  while (line.isValid() && ! line.isChar(',')) {
    int pos = line.pos();

    PlotVar plotVar;

    if (! parseOptionValue(this, line, plotVar)) {
      if (! parseModifiers2D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;

      continue;
    }

    // TODO: hack - fix me
    if (plotVar == PlotVar::DASHTYPE && line.isChar('=')) {
      line.setPos(pos);

      if (! parseModifiers2D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;

      continue;
    }

    line.skipSpace();

    // Read using columns
    // using <col1>[:<col2>[...]] <"format">
    if      (plotVar == PlotVar::USING) {
      std::string usingStr = parseUsingStr(line);

      usingCols.init(usingStr);

      if (isDebug())
        usingCols.print(std::cerr);

      std::string format;

      if (parseString(line, format))
        usingCols.setFormat(format);
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
          debugMsg("with " + CStrUniqueMatch::valueToString(style));

        if (style == CGnuPlot::PlotStyle::FILLEDCURVES) {
          // re-init style for filled curves
          if (fillStyle.style() == CGnuPlotTypes::FillType::EMPTY) {
            fillStyle.setStyle (CGnuPlotTypes::FillType::SOLID);
            fillStyle.setBorder(false);
          }

          parseFilledCurve(line, filledCurve_);
        }
      }

      parseModifiers2D(style, line, lineStyle, fillStyle, styleData, keyTitle);
    }
    // title string
    else if (plotVar == PlotVar::TITLE) {
      std::string titleStr;
      int         columnNum;

      parsePlotTitle_ = true;

      COptInt col;

      if      (isColumnHeader(line, col)) {
    //if      (line.isOneOf({"col", "columnhead", "columnheader", "columnheaders"}))
        setKeyColumnHeadNum(col.getValue(0));

        if      (line.isChars("at beginning")) {
          line.skipChars(12);

          line.skipSpace();

          keyData_.setHeadPosition(CGnuPlotKeyData::HeadPosition::AT_BEGINNING);
        }
        else if (line.isChars("at end")) {
          line.skipChars(6);

          line.skipSpace();

          keyData_.setHeadPosition(CGnuPlotKeyData::HeadPosition::AT_END);
        }

        //(void) readNonSpaceNonComma(line);
      }
      else if (parseString(line, titleStr)) {
        if (isDebug())
          debugMsg("title " + titleStr);

        if (titleStr != "")
          keyTitle.str = titleStr;
      }
      else if (parseInteger(line, columnNum)) {
        setKeyColumnHeadNum(columnNum);
      }

      parsePlotTitle_ = false;

      line.skipSpace();
    }
    // disable title
    else if (plotVar == PlotVar::NOTITLE) {
      keyTitle.enabled = false;
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

      if (parseLineType(line, lt))
        lineStyle.setLineType(lt);
    }
    // linewidth <lw>
    else if (plotVar == PlotVar::LINEWIDTH) {
      double lw;

      if (parseReal(line, lw))
        lineStyle.setLineWidth(lw);
    }
    // fillstyle <fs>
    else if (plotVar == PlotVar::FILLSTYLE) {
      CGnuPlotFillStyle fillStyle1 = fillStyle;

      if (parseFillStyle(line, fillStyle1))
        fillStyle = fillStyle1;
    }
    // dashtype <dt>
    else if (plotVar == PlotVar::DASHTYPE) {
      CGnuPlotDash dash;

      if (parseDash(line, dash))
        lineStyle.setLineDash(dash);
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
        styleData.arrow = this->arrowStyle(as);
    }
    // heads
    else if (plotVar == PlotVar::HEADS) {
      styleData.arrow.setHeads(true, true);
    }
    // binary
    else if (plotVar == PlotVar::BINARY) {
      setBinary(true);
    }
    // transpose
    else if (plotVar == PlotVar::TRANSPOSE) {
      // TODO
      //setTranspose(true);
    }
    // matrix
    else if (plotVar == PlotVar::MATRIX) {
      matrixData().setMatrix(true);
    }
    // pixels
    else if (plotVar == PlotVar::PIXELS) {
      matrixData().setPixels(true);
    }
    // rowheaders
    else if (plotVar == PlotVar::ROWHEADERS) {
      matrixData().setRowHeaders(true);
    }
    // columnheaders
    else if (plotVar == PlotVar::COLUMNHEADERS) {
      matrixData().setColumnHeaders(true);
    }
    // flipy
    else if (plotVar == PlotVar::FLIPY) {
      imageStyle_.setFlipY(true);
    }
    // whiskerbars [<fraction>]
    else if (plotVar == PlotVar::WHISKERBARS) {
      double s = 1;

      (void) parseReal(line, s);

      setWhiskerBars(s);
    }
    else if (plotVar == PlotVar::POINTINTERVAL) {
      line.setPos(pos);

      if (! parseModifiers2D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;
    }
    else if (plotVar == PlotVar::VOLATILE) {
      // TODO
    }
    else {
      errorMsg("Unhandled plot var '" + CStrUniqueMatch::valueToString(plotVar) + "'");

      line.setPos(pos);

      if (! parseModifiers2D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;
    }
  }

  //---

  if      (! filename.empty()) {
    if (isBinary()) {
      if (isImageStyle(style)) {
        CGnuPlotPlotP plot = addImage2D(group, filename, style, usingCols);

        if (plot)
          addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
      }
      else {
        CGnuPlotPlotP plot = addBinary2D(group, filename, style, usingCols);

        if (plot)
          addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
      }
    }
    else {
      addFile2D(plots, group, filename, style, usingCols,
                lineStyle, fillStyle, styleData, keyTitle);
    }
  }
  else if (! functions.empty()) {
    CGnuPlotPlotP plot = addFunction2D(group, functions, style);

    if (plot)
      addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
  }

  //---

  while (line.isValid()) {
    line.skipSpace();

    std::string arg = readNonSpaceNonComma(line);

    if (arg != "")
      errorMsg("Invalid arg '" + arg + "'");

    if (line.isChar(',')) {
      errorMsg("Invalid comma '" + line.substr() + "'");
      break;
    }
  }
}

void
CGnuPlot::
addPlotWithStyle(CGnuPlotPlotP &plot, Plots &plots, const CGnuPlotLineStyle &lineStyle,
                 const CGnuPlotFillStyle &fillStyle, const CGnuPlotStyleData &styleData,
                 const CGnuPlotKeyTitle &keyTitle)
{
  plot->setKeyTitleEnabled(keyTitle.enabled);

  if (keyTitle.str != "")
    plot->setKeyTitleString(keyTitle.str);

  plot->setLineStyle(lineStyle);
  plot->setFillStyle(fillStyle);
  plot->setStyleData(styleData);

  plots.push_back(plot);
}

void
CGnuPlot::
splitPlotCmd(const std::string &cmd, StringArray &cmds, bool is3D)
{
  int nf = (isParametric() ? (is3D ? 3 : 2) : 1);

  bool sample = false;
  bool first  = true;

  CParseLine line(cmd);

  while (line.isValid()) {
    std::string cmd1;

    // skip for
    if (line.isString("for")) {
      line.skipNonSpace();
      line.skipSpace();

      cmd1 += "for";

      if (line.skipSpaceAndChar('[')) {
        line.setPos(line.pos() - 1); // back to '['

        int pos = line.pos();

        skipSquareBracketedString(line);

        addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));
      }
    }

    bool first1 = false;

    std::swap(first, first1);

    // skip newhistogram
    if (line.isString("newhistogram")) {
      line.skipNonSpace();
      line.skipSpace();

      addSpaceSep(cmd1, "newhistogram");
    }

    if (first1) {
      // skip ranges
      while (line.skipSpaceAndChar('[')) {
        line.setPos(line.pos() - 1); // back to '['

        int pos = line.pos();

        skipSquareBracketedString(line);

        addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));
      }

      // skip sample
      if (line.isString("sample")) {
        line.skipNonSpace();
        line.skipSpace();

        addSpaceSep(cmd1, "sample");

        sample = true;
      }
    }

    //---

    if (! first1 && sample) {
      // skip sample ranges
      while (line.skipSpaceAndChar('[')) {
        line.setPos(line.pos() - 1); // back to '['

        int pos = line.pos();

        skipSquareBracketedString(line);

        addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));
      }
    }

    //---

    // skip filename
    if (line.isChar('\"') || line.isChar('\'')) {
      int pos = line.pos();

      skipString(line);

      addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));
    }
    // skip expressions (up to max of nf)
    else {
      int nf1 = nf;

      while (nf1 > 0) {
        int pos = line.pos();

        FunctionData functionData;

        if (! parseFunction(line, functionData)) {
          line.setPos(pos);
          break;
        }

        if (! functionData.isAssign)
          --nf1;

        addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));

        pos = line.pos();

        line.skipSpace();

        if (! line.isChar(',')) {
          line.setPos(pos);
          break;
        }

        addSpaceSep(cmd1, line.substr(pos, line.pos() - pos));

        if (nf1 > 0) {
          line.skipChar();

          addSpaceSep(cmd1, ",");
        }
      }
    }

    // skip to ',' or end of line
    while (line.isValid()) {
      if (line.isSpace()) {
        while (line.isSpace())
          line.skipChar();

        cmd1 += " ";
      }

      // skip string
      if      (line.isChar('\"') || line.isChar('\'')) {
        int pos = line.pos();

        skipString(line);

        cmd1 += line.substr(pos, line.pos() - pos);
      }
      // skip bracked expression
      else if (line.isChar('(')) {
        int pos = line.pos();

        skipRoundBracketedString(line);

        cmd1 += line.substr(pos, line.pos() - pos);
      }
      else if (line.isChar(',')) {
        line.skipChar();

        break;
      }
      else if (line.isString("with")) {
        line.skipNonSpace();

        cmd1 += "with ";

        if (line.isSpace()) {
          while (line.isSpace())
            line.skipChar();

          cmd1 += " ";
        }

        int pos = line.pos();

        PlotStyle style;

        if (parseOptionValue(this, line, style, "plot style")) {
          cmd1 += line.str().substr(pos, line.pos() - pos);

          if (style == CGnuPlot::PlotStyle::FILLEDCURVES) {
            int pos = line.pos();

            CGnuPlotFilledCurve filledCurve;

            parseFilledCurve(line, filledCurve);

            cmd1 += line.str().substr(pos, line.pos() - pos);
          }
        }
      }
      // handle options with comma values (position, offset, point, size)
      else if (line.isString("offset")) {
        cmd1 += line.getChars(6) + " ";

        line.skipSpace();

        CPoint2D o(0, 0);

        int pos = line.pos();

        (void) parseOffset(line, o);

        cmd1 += line.str().substr(pos, line.pos() - pos);
      }
      else {
        if (line.isValid())
          cmd1 += line.getChar();
      }
    }

    if (cmd1 != "") {
      if (isDebug())
        debugMsg("Cmd: " + cmd1);

      cmds.push_back(cmd1);
    }
  }
}

void
CGnuPlot::
parseFilledCurve(CParseLine &line, CGnuPlotFilledCurve &filledCurve)
{
  int         pos = line.pos();
  std::string arg = readNonSpaceNonComma(line);

  filledCurve.closed = false;
  filledCurve.above  = false;
  filledCurve.below  = false;
  filledCurve.xaxis  = 0;
  filledCurve.yaxis  = 0;

  filledCurve.xval .setInvalid();
  filledCurve.yval .setInvalid();
  filledCurve.xyval.setInvalid();

  if (arg == "closed") {
    filledCurve.closed = true;

    pos = line.pos();
    arg = readNonSpaceNonComma(line);
  }

  if (arg == "above" || arg == "below") {
    filledCurve.above = (arg == "above");
    filledCurve.below = (arg == "below");

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
    filledCurve.xaxis = (lhs == "x1" ? 1 : 2);

    if (CStrUtil::toReal(rhs, &x))
      filledCurve.xval = x;
  }
  else if (lhs == "y1" || lhs == "y2") {
    filledCurve.yaxis = (lhs == "y1" ? 1 : 2);

    if (CStrUtil::toReal(rhs, &y))
      filledCurve.yval = y;
  }
  else if (lhs == "xy") {
    std::string rhs1;

    if (line.skipSpaceAndChar(','))
      rhs1 = readNonSpaceNonComma(line);

    if (CStrUtil::toReal(rhs, &x) && CStrUtil::toReal(rhs1, &y))
      filledCurve.xyval = CPoint2D(x, y);
  }
  else if (lhs == "r") {
    filledCurve.raxis = 1;

    if (CStrUtil::toReal(rhs, &x))
      filledCurve.rval = x;
  }
  else {
    line.setPos(pos);
  }
}

bool
CGnuPlot::
parseModifiers2D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &lineStyle,
                 CGnuPlotFillStyle &fillStyle, CGnuPlotStyleData &styleData,
                 CGnuPlotKeyTitle &keyTitle)
{
  bool modifiers = true;
  bool found     = false;

  while (modifiers) {
    line.skipSpace();

    if      (line.isOneOf({"linestyle", "ls"})) {
      line.skipNonSpace();

      int ls;

      if (parseInteger(line, ls))
        lineStyle = *getLineStyleInd(ls);

      found = true;
    }
    else if (line.isOneOf({"linetype", "lt"})) {
      line.skipNonSpace();

      int lt;

      if (parseLineType(line, lt))
        lineStyle.setLineType(lt);

      found = true;
    }
    else if (line.isOneOf({"linewidth", "lw"})) {
      line.skipNonSpace();

      double lw;

      if (parseReal(line, lw)) {
        if (style == CGnuPlotTypes::PlotStyle::LABELS)
          styleData.label.setLineWidth(lw);
        else
          lineStyle.setLineWidth(lw);
      }

      found = true;
    }
    else if (line.isOneOf({"linecolor", "lc"})) {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle.setLineColor(c);

      found = true;
    }
    else if (line.isOneOf({"pointtype", "pt"})) {
      line.skipNonSpace();

      int         pt = 1;
      std::string ptstr;

      if      (parseInteger(line, pt)) {
        if (style == CGnuPlotTypes::PlotStyle::LABELS)
          styleData.label.setPointType(pt);
        else
          lineStyle.setPointType(pt);
      }
      else if (parseString(line, ptstr))
        lineStyle.setPointTypeStr(ptstr);
      else
        errorMsg("Invalid pointtype");

      found = true;
    }
    else if (line.isOneOf({"pointsize", "ps"})) {
      line.skipNonSpace();

      std::string style = readNonSpaceNonComma(line);

      double s;

      if      (style == "variable" || style == "var")
        pointStyle_.setVarSize(true);
      else if (CStrUtil::toReal(style, &s))
        lineStyle.setPointSize(s);

      found = true;
    }
    else if (line.isOneOf({"pointinterval", "pi"})) {
      line.skipNonSpace();

      int pi;

      if (parseInteger(line, pi))
        lineStyle.setPointInterval(pi);

      found = true;
    }
    else if (line.isOneOf({"fill", "fs"})) {
      line.skipNonSpace();

      CGnuPlotFillStyle fillStyle1 = fillStyle;

      if (parseFillStyle(line, fillStyle1))
        fillStyle = fillStyle1;

      found = true;
    }
    else if (line.isOneOf({"arrowstyle", "as"})) {
      line.skipNonSpace();

      int pos = line.pos();

      std::string style = readNonSpaceNonComma(line);

      int as;

      if (style == "variable" || style == "var")
        styleData.arrow.setVariable(true);
      else {
        line.setPos(pos);

        if (parseInteger(line, as))
          styleData.arrow = this->arrowStyle(as);
      }

      found = true;
    }
    else if (line.isString("filled")) {
      styleData.arrow.setFilled(true);

      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("nohead")) {
      styleData.arrow.setHeads(false, false);

      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("notitle")) {
      line.skipNonSpace();

      keyTitle.enabled = false;

      found = true;
    }
    else if (line.isString("boxed")) {
      line.skipNonSpace();

      styleData.text.setBoxed(true);

      found = true;
    }
    else if (line.isString("nohidden3d")) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("nocontours")) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isOneOf({"nosurf", "nosurface"})) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isOneOf({"palette", "pal"})) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    // font
    else if (line.isString("font")) {
      line.skipNonSpace();

      CFontPtr font;

      if (! parseFont(line, font))
        errorMsg("Missing font");

      if (style == CGnuPlotTypes::PlotStyle::LABELS)
        styleData.label.setFont(font);

      found = true;
    }
    // format=
    else if (line.isChars("format=")) {
      line.skipChars(7);

      std::string str;

      (void) parseString(line, str, "Invalid format");

      setBinaryFormat(str);

      found = true;
    }
    // array=
    else if (line.isChars("array=")) {
      line.skipChars(6);

      Sizes sizes;

      parseArrayValues(line, sizes);

      if (! sizes.empty()) {
        imageStyle_.setWidth (sizes[0].getWidth ());
        imageStyle_.setHeight(sizes[0].getHeight());
      }

      found = true;
    }
    // record=
    else if (line.isChars("record=")) {
      line.skipChars(7);

      Integers values;

      parseArrayValues(line, values);

      setRecordValues(values);

      found = true;
    }
    // filetype=
    else if (line.isChars("filetype=")) {
      line.skipChars(9);

      std::string arg = readNonSpace(line);

      CGnuPlotTypes::ImageType imageType;

      if (CStrUniqueMatch::stringToValue(arg, imageType))
        imageStyle_.setFileType(imageType);

      found = true;
    }
    // origin=
    else if (line.isChars("origin=")) {
      line.skipChars(7);

      typedef std::vector<CPoint3D> Points3D;

      Points3D originArray;

      do {
        if (line.skipSpaceAndChar('(')) {
          CPoint2D origin;

          if (parsePoint(line, origin)) {
            CPoint3D origin1(origin.x, origin.y, 0);

            originArray.push_back(origin1);
          }

          (void) line.skipSpaceAndChar(')');
        }
      }
      while (line.skipSpaceAndChar(':'));

      imageStyle_.setOriginArray(originArray);

      found = true;
    }
    // center=
    else if (line.isChars("center")) {
      int pos = line.pos();

      line.skipChars(6);

      if      (line.skipSpaceAndChar('=')) {
        if (line.skipSpaceAndChar('(')) {
          CPoint2D c;

          if (parsePoint(line, c)) {
            CPoint3D c1(c.x, c.y, 0);

            imageStyle_.setCenter(c1);
          }

          (void) line.skipSpaceAndChar(')');
        }

        found = true;
      }
      else if (style == CGnuPlotTypes::PlotStyle::LABELS) {
        CHAlignType halign = CHALIGN_TYPE_CENTER;

        styleData.text .setHAlign(halign); // TODO: remove
        styleData.label.setAlign (halign);

        found = true;
      }
      else
        line.setPos(pos);
    }
    // skip=
    else if (line.isChars("skip=")) {
      int pos = line.pos();

      line.skipChars(4);

      if (line.skipSpaceAndChar('=')) {
        Reals reals;

        do {
          double r;

          if (parseReal(line, r))
            reals.push_back(r);
        }
        while (line.skipSpaceAndChar(':'));

        imageStyle_.setSkip(reals);

        found = true;
      }
      else
        line.setPos(pos);
    }
    // dx=, dy=
    else if (line.isChars("dx=") || line.isChars("dy=")) {
      bool isDX = line.isChars("dx=");

      line.skipChars(3);

      double r;

      if (parseReal(line, r)) {
        if (isDX) {
          imageStyle_.setDX(r);
          imageStyle_.setDY(r);
        }
        else
          imageStyle_.setDY(r);
      }

      found = true;
    }
    // dt=
    else if (line.isChars("dt=")) {
      line.skipChars(3);

      double r;

      if (parseReal(line, r))
        imageStyle_.setDT(r);

      found = true;
    }
    // rotation=
    else if (line.isChars("rot=") || line.isChars("rotate=") || line.isChars("rotation=")) {
      if (line.skipToChar('=')) line.skipChar();

      double a;

      if (parseReal(line, a)) {
        line.backSkipSpace();

        std::string arg = (! line.isSpace() ? line.readNonSpace() : "");

        if      (arg == "deg" || arg == "d")
          imageStyle_.setAngle(a);
        else if (arg == "pi")
          imageStyle_.setAngle(CAngle::Rad2Deg(a*M_PI));
        else
          imageStyle_.setAngle(CAngle::Rad2Deg(a));
      }

      found = true;
    }
    // endian=
    else if (line.isChars("endian=")) {
      line.skipChars(7);

      std::string arg = line.readNonSpace();

      if      (arg == "little")
        setEndian(Endian::LITTLE);
      else if (arg == "big")
        setEndian(Endian::BIG);
      else if (arg == "default")
        setEndian(Endian::DEFAULT);
      else if (arg == "swap")
        setEndian(Endian::SWAP);
      else
        errorMsg("Invalid endian \'" + arg + "'");

      line.readNonSpace();

      found = true;
    }
    // flip=
    else if (line.isChars("flip=")) {
      line.skipChars(5);

      std::string arg = line.readNonSpace();

      if (arg == "y")
        imageStyle_.setFlipY(true);

      found = true;
    }
    // scan=
    else if (line.isChars("scan=")) {
      line.skipChars(5);

      line.readNonSpace();

      //std::string arg = line.readNonSpace();

      //if      (arg == "xy") imageStyle_.setScanXY();
      //else if (arg == "yx") imageStyle_.setScanXY();
      //else if (arg == "xyz") imageStyle_.setScanXY();

      found = true;
    }
    else if (style == CGnuPlotTypes::PlotStyle::LABELS) {
      if      (line.isString("offset")) {
        line.skipNonSpace();

        CPoint2D o(0, 0);

        if (parseOffset(line, o)) {
          styleData.text .setOffset(o); // TODO: remove
          styleData.label.setOffset(o);
        }

        found = true;
      }
      else if (line.isString("point")) {
        line.skipNonSpace();

        styleData.label.setShowPoint(true);

        found = true;
      }
      else if (line.isOneOf({"left", "center", "right"})) {
        line.skipNonSpace();

        CHAlignType halign = CHALIGN_TYPE_LEFT;

        if      (line.isString("left"))
          halign = CHALIGN_TYPE_LEFT;
        else if (line.isString("center"))
          halign = CHALIGN_TYPE_CENTER;
        else
          halign = CHALIGN_TYPE_RIGHT;

        styleData.text .setHAlign(halign); // TODO: remove
        styleData.label.setAlign (halign);

        found = true;
      }
      else if (line.isOneOf({"textcolor", "tc"})) {
        line.skipNonSpace();

        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          styleData.label.setTextColor(c);

        found = true;
      }
      else if (line.isString("hypertext")) {
        line.skipNonSpace();

        styleData.label.setHypertext(true);
      }
      else
        modifiers = false;
    }
    else if (style == CGnuPlotTypes::PlotStyle::POINTS ||
             style == CGnuPlotTypes::PlotStyle::LINES_POINTS) {
      if (line.isOneOf({"textcolor", "tc"})) {
        line.skipNonSpace();

        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          styleData.label.setTextColor(c);

        found = true;
      }
      else
        modifiers = false;
    }
    else if (style == CGnuPlotTypes::PlotStyle::PIECHART) {
      if      (line.isString("startangle")) {
        line.skipNonSpace();

        double a;

        if (parseReal(line, a))
          styleData.pie.setStartAngle(a);

        found = true;
      }
      else if (line.isString("innerradius")) {
        line.skipNonSpace();

        double r;

        if (parseReal(line, r))
          styleData.pie.setInnerRadius(r);

        found = true;
      }
      else if (line.isString("labelradius")) {
        line.skipNonSpace();

        double r;

        if (parseReal(line, r))
          styleData.pie.setLabelRadius(r);

        found = true;
      }
      else
        modifiers = false;
    }
    else if (style == CGnuPlotTypes::PlotStyle::ELLIPSES) {
      if      (line.isString("units")) {
        line.skipNonSpace();

        line.skipSpace();

        // xx, xy, yy
        std::string arg = line.readNonSpace();

        if      (arg == "xx")
          styleData.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::XX);
        else if (arg == "xy")
          styleData.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::XY);
        else if (arg == "yy")
          styleData.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::YY);
        else
          errorMsg("Invalid ellipse style \'" + arg + "'");

        found = true;
      }
      else
        modifiers = false;
    }
    else if (parseAssignment(line)) {
      found = true;
    }
    else {
      modifiers = false;
    }
  }

  return found;
}

void
CGnuPlot::
plotForCmd(const ForCmd &forCmd, const std::string &args, CGnuPlotGroupP &group, Plots &plots)
{
  bool sample = false, first = true;

  if (forCmd.isIn) {
    StringArray words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      plotCmd1(args, group, plots, sample, first);
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

      plotCmd1(args, group, plots, sample, first);

      i1 += i3;
    }
  }
}

void
CGnuPlot::
splotForCmd(const ForCmd &forCmd, const std::string &args, CGnuPlotGroupP &group, Plots &plots)
{
  bool sample = false, first = true;

  if (forCmd.isIn) {
    StringArray words;

    CStrUtil::addWords(forCmd.start, words);

    for (const auto &w : words) {
      CExprInst->createStringVariable(forCmd.var, w);

      splotCmd1(args, group, plots, sample, first);
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

      splotCmd1(args, group, plots, sample, first);

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

// {empty}
// {{transparent} solid {density} | pattern {id}}
// {border {<linetype> | {lc <colorspec>}} | noborder}
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

    if (line.isDigit()) {
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

    if (line.isDigit()) {
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

    CGnuPlotColorSpec c;

    if      (line.isString("lt")) {
      if (lineStyle()->ind().isValid())
        fillStyle.setBorderLineType(lineStyle()->ind().getValue());

      fillStyle.unsetBorderColor();
    }
    else if (line.isString("lc")) {
      line.skipNonSpace();
      line.skipSpace();

      if (parseColorSpec(line, c)) {
        fillStyle.unsetBorderLineType();
        fillStyle.setBorderColor(c);
      }
    }
    else if (parseInteger(line, lt)) {
      fillStyle.setBorderLineType(lt);
      fillStyle.unsetBorderColor();
    }
    else if (parseColorSpec(line, c)) {
      fillStyle.unsetBorderLineType();
      fillStyle.setBorderColor(c);
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
        errorMsg("Invalid color string \'" + colorValueStr + "'");
    }
  }
  else
    line.setPos(pos);

  return true;
}

void
CGnuPlot::
parseLineProp(CParseLine &line, CGnuPlotLineProp &lineProp)
{
  int         pos = line.pos();
  std::string arg = readNonSpace(line);

  while (arg != "") {
    if      (arg == "linewidth" || arg == "lw") {
      double lw;

      if (parseReal(line, lw))
        lineProp.setLineWidth(lw);
    }
    else if (arg == "linestyle" || arg == "ls") {
      int ls;

      if (parseInteger(line, ls))
        lineProp.setLineStyle(ls);
    }
    else if (arg == "linetype" || arg == "lt") {
      int lt;

      if (parseLineType(line, lt))
        lineProp.setLineType(lt);
    }
    else if (arg == "linecolor" || arg == "lc") {
      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineProp.setLineColor(c);
    }
    else if (arg == "dashtype" || arg == "dt") {
      CGnuPlotDash dash;

      if (parseDash(line, dash))
        lineProp.setLineDash(dash);
    }
    else {
      line.setPos(pos);
      break;
    }

    pos = line.pos();
    arg = readNonSpace(line);
  }
}

// [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
bool
CGnuPlot::
parseAxisRange(CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs)
{
  line.skipSpace();

  if (line.isChar('[')) {
    StringArray fields;
    std::string dummyVar;

    if (! parseRange(line, fields, dummyVar)) {
      errorMsg("Failed to parse axis range '" + line.substr() + "'");
      return false;
    }

    if (dummyVar != "")
      axis.setDummyVar(dummyVar);

    if (! decodeRange(fields, axis)) {
      errorMsg("Failed to decode axis range");
      return false;
    }
  }
  else {
    if (! hasArgs)
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
        // TODO
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

  while (arg != "") {
    if      (arg == "offset") {
      CPoint3D o;

      if (parseOffset(line, o))
        axis.setLabelOffset(o);
    }
    else if (arg == "font") {
      CFontPtr font;

      if (parseFont(line, font))
        axis.setLabelFont(font);
    }
    else if (arg == "textcolor" || arg == "tc") {
      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        axis.setLabelColor(c);
    }
    else if (arg == "enhanced" || arg == "noenhanced") {
      axis.setEnhanced(arg == "enhanced");
    }
    else if (arg == "rotate") {
      int pos1 = line.pos();

      std::string arg1 = readNonSpace(line);

      if      (arg1 == "by") {
        double a;

        if (parseReal(line, a))
          axis.setLabelRotate(a);
      }
      else if (arg1 == "parallel") {
        axis.setLabelRotate(-1);
      }
      else {
        axis.setLabelRotate(90);

        line.setPos(pos1);
      }
    }
    else if (arg == "norotate") {
      axis.setLabelRotate(0);
    }
    else {
      line.setPos(pos);

      std::string labelStr;

      if (parseString(line, labelStr))
        axis.setText(labelStr);
      else {
        errorMsg("Invalid axes label arg '" + arg + "'");
        break;
      }
    }

    pos = line.pos();

    arg = readNonSpace(line);
  }

  return true;
}

// [{<dummy-var>=}{min}:{max}]
bool
CGnuPlot::
parseRange(CParseLine &line, StringArray &fields, std::string &dummyVar)
{
  if (! line.skipSpaceAndChar('['))
    return false;

  std::string rangeStr;

  while (line.isValid() && ! line.isChar(']'))
    rangeStr += line.getChar();

  if (line.isChar(']'))
    line.skipChar();

  CStrUtil::addFields(rangeStr, fields, ":");

  if (fields.size() == 2) {
    auto p = fields[0].find('=');

    if (p != std::string::npos) {
      std::string lhs = fields[0].substr(0, p);
      std::string rhs = fields[0].substr(p + 1);

      dummyVar = lhs;

      fields[0] = rhs;
    }
  }

  return true;
}

bool
CGnuPlot::
decodeRange(const StringArray &fields, CGnuPlotAxisData &axis)
{
  if (fields.empty())
    return true;

  typedef std::pair<COptReal,bool> OptRealBool;

  std::vector<OptRealBool> values;

  for (const auto &field : fields) {
    std::string field1 = CStrUtil::stripSpaces(field);

    struct tm tm1; memset(&tm1, 0, sizeof(tm));

    COptReal value;
    bool     force = false;

    if (axis.isTime()) {
      int len = field1.size();

      if (len > 0 && field1[0] == '"' && field1[len - 1] == '"') {
        double t;

        if (timeToValue(field1.substr(1, len - 2), t))
          value = t;
      }
    }
    else {
      auto pos = field1.find('*');

      if (pos == std::string::npos) {
        double r;

        if (field1 != "" && exprToReal(field1, r))
          value = r;
      }
      else {
        auto pos1 = field1.find('<');

        // handle range
        if      (pos1 != std::string::npos) {
          std::string lhs = field1.substr(0, pos1);
          std::string rhs = field1.substr(pos1 + 1);
          std::string mid;

          auto pos2 = rhs.find('<');

          if (pos2 != std::string::npos) {
            mid = rhs.substr(0, pos2);
            rhs = rhs.substr(pos2 + 1);
          }

          force = true;
        }
        else if (field1 == "*") {
          force = true;
        }
        else {
          double r;

          if (exprToReal(field1, r))
            value = r;
        }
      }
    }

    values.push_back(OptRealBool(value, force));
  }

  if      (values.size() >= 2) {
    COptReal &min = values[0].first;
    COptReal &max = values[1].first;

    if (isDebug())
      debugMsg("Range=(" + std::to_string(min.getValue(-1)) +
                     "," + std::to_string(max.getValue( 1)) + ")");

    if (min.isValid() && max.isValid()) {
      if (min.getValue() > max.getValue()) {
        axis.setReverse(true);

        axis.setMin(max.getValue());
        axis.setMax(min.getValue());
      }
      else {
        axis.setMin(min.getValue());
        axis.setMax(max.getValue());
      }
    }
    else {
      if      (min.isValid())
        axis.setMin(min.getValue());
      else if (values[0].second)
        axis.resetMin();

      if      (max.isValid())
        axis.setMax(max.getValue());
      else if (values[1].second)
        axis.resetMax();
    }
  }
  else if (values.size() == 1) {
    COptReal &min = values[0].first;

    if (isDebug())
      debugMsg("Range=(" + std::to_string(min.getValue(-1)) + ",*)");

    if      (min.isValid())
      axis.setMin(min.getValue());
    else if (values[0].second)
      axis.resetMin();
  }
  else
    return false;

  return true;
}

void
CGnuPlot::
parseArrayValues(CParseLine &line, Integers &values)
{
  std::string arg = line.readNonSpace();

  StringArray fields;

  CStrUtil::addFields(arg, fields, ":");

  for (const auto &f : fields) {
    int i;

    if (! CStrUtil::toInteger(f, &i))
      continue;

    values.push_back(i);
  }
}

void
CGnuPlot::
parseArrayValues(CParseLine &line, Sizes &sizes)
{
  if (line.skipSpaceAndChar('(')) {
    int w = 1, h = 1;

    if (! parseInteger(line, w))
      w = 1;

    if (line.skipSpaceAndChar(',')) {
      if (! parseInteger(line, h))
        h = 1;
    }

    (void) line.skipSpaceAndChar(')');

    sizes.push_back(CSize2D(w, h));
  }
  else {
    std::string arg = line.readNonSpace();

    StringArray fields;

    CStrUtil::addFields(arg, fields, ":");

    for (const auto &f : fields) {
      int w;

      if (! CStrUtil::toInteger(f, &w))
        continue;

      sizes.push_back(CSize2D(w, 1));
    }
  }
}

// replot
void
CGnuPlot::
replotCmd(const std::string &args)
{
  if (isDebug()) debugMsg("replot " + args);

  device_->redraw();

  CParseLine line(args);

  line.skipSpace();

  if (line.isValid()) {
    if      (lastPlotCmd_ != "")
      plotCmd(lastPlotCmd_ + ", " + line.str());
    else if (lastSPlotCmd_ != "")
      splotCmd(lastSPlotCmd_ + ", " + line.str());
  }
  else {
    if      (lastPlotCmd_ != "")
      plotCmd(lastPlotCmd_);
    else if (lastSPlotCmd_ != "")
      splotCmd(lastSPlotCmd_);
  }
}

// refresh
void
CGnuPlot::
refreshCmd(const std::string &args)
{
  if (isDebug()) debugMsg("refresh " + args);
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
  if (isDebug()) debugMsg("splot " + args);

  lastSPlotCmd_ = args;
  lastPlotCmd_  = "";

  setLastFilename("");

  saveAxisData();

  //----

  resetLineStyle();

  COptBBox2D clearRect = clearRect_;

  clearRect_.setInvalid();

  //----

  // TODO: update local line style copy

  CGnuPlotWindowP window;
  bool            newWindow = false;

  if (multiplot_.isValid() && multiplot_->isEnabled()) {
    // set multi-window if doesn't exist
    if (! device_->multiWindow()) {
      // create or reuse existing window
      if (! device_->windows().empty()) {
        for (auto &w : device_->windows()) {
          w->clear();

          window = w;
        }
      }
      else {
        window    = CGnuPlotWindowP(createWindow());
        newWindow = true;
      }

      device_->setMultiWindow(window);

      device_->multiWindow()->setBackgroundColor(backgroundColor());
      device_->multiWindow()->set3D(true);
    }
    else
      window = device_->multiWindow();

    // set region (default to multiplot grid cell)
    int n = window->numGroups();

    double dx = multiplot_->dx();
    double dy = multiplot_->dy();

    CIPoint2D pos = multiplot_->pos(n);

    double x1 = plotSize_.xo.getValue(pos.x*dx);
    double y1 = plotSize_.yo.getValue(pos.y*dy);
    double x2 = x1 + plotSize_.xsize.getValue(dx);
    double y2 = y1 + plotSize_.ysize.getValue(dy);

    region_ = CBBox2D(x1, y1, x2, y2);
  }
  else {
    // create or reuse existing window
    if (! device_->windows().empty()) {
      for (auto &w : device_->windows()) {
        w->clear();

        window = w;
      }
    }
    else {
      window    = CGnuPlotWindowP(createWindow());
      newWindow = true;
    }

    window->setBackgroundColor(backgroundColor());
    window->set3D(true);

    // set region (default to full screen)
    double x1 = plotSize_.xo.getValue(0.0);
    double y1 = plotSize_.yo.getValue(0.0);
    double x2 = x1 + plotSize_.xsize.getValue(1.0);
    double y2 = y1 + plotSize_.ysize.getValue(1.0);

    region_ = CBBox2D(x1, y1, x2, y2);
  }

  if (newWindow)
    addWindow(window);

  //---

  StringArray cmds;

  splitPlotCmd(args, cmds, true);

  //---

  CGnuPlotGroupP group(createGroup(window.get()));

  group->set3D(true);

  Plots plots;
  bool  sample = false, first = true;

  for (const auto &cmd : cmds) {
    splotCmd1(cmd, group, plots, sample, first);
  }

  //---

  if (! plots.empty()) {
    group->addObjects();

    group->init();
    group->set3D(true);

    group->setClearRect(clearRect);

    group->setTitleData(title());

    group->addSubPlots(plots);

    window->addGroup(group);

    for (auto plot : plots)
      plot->smooth();

    group->fit();

    for (auto plot : plots) {
      if (plot->tableFile().isEnabled())
        plot->printValues();
    }

    //---

    group->saveRange();

    clearTicLabels();

    //---

    if (device())
      stateChanged(window.get(), CGnuPlotTypes::ChangeState::PLOT_ADDED);
  }

  //---

  restoreAxisData();
}

void
CGnuPlot::
splotCmd1(const std::string &args, CGnuPlotGroupP &group, Plots &plots,
          bool &sample, bool &first)
{
  CParseLine line(args);

  line.skipSpace();

  if (line.isString("for")) {
    ForCmd      forCmd;
    std::string cmd;

    if (! parseFor(line, forCmd, cmd))
      return;

    splotForCmd(forCmd, cmd, group, plots);

    return;
  }

  //----

  bool first1 = false;

  std::swap(first, first1);

  //----

  incLineStyle();

  //----

  // get local copy of next line style
  CGnuPlotLineStyle lineStyle = *this->lineStyle();
  CGnuPlotFillStyle fillStyle = this->fillStyle();
  CGnuPlotStyleData styleData = this->styleData();

  //----

  // Get global range (all plots)
  if (first1) {
    if      (isParametric()) {
#if 0
      // Get Range
      //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]
      parseAxisRange(line, taxis(1), false);

      if (parseAxisRange(line, xaxis(xind_), false)) {
        sampleVars_.x.min = xaxis(xind_).min();
        sampleVars_.x.max = xaxis(xind_).max();
      }

      parseAxisRange(line, yaxis(yind_), false);
      parseAxisRange(line, zaxis(zind_), false);
#else
      parseAxisRange(line, uaxis(), false);
      parseAxisRange(line, vaxis(), false);
#endif
    }
    else if (isPolar()) {
#if 0
      // Get Range
      //  [TMIN:TMAX][XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]
      parseAxisRange(line, taxis(1), false);

      if (parseAxisRange(line, xaxis(xind_), false)) {
        sampleVars_.x.min = xaxis(xind_).min();
        sampleVars_.x.max = xaxis(xind_).max();
      }

      parseAxisRange(line, yaxis(yind_), false);
      parseAxisRange(line, zaxis(zind_), false);
#else
      parseAxisRange(line, uaxis(), false);
      parseAxisRange(line, vaxis(), false);
#endif
    }
    else {
      // Get Range
      //  [XMIN:XMAX][YMIN:YMAX][ZMIN:ZMAX]
      if (parseAxisRange(line, xaxis(xind_), false)) {
        if (sampleVars_.x.var != "")
          dummyVars_.setDummyVar("x", sampleVars_.x.var);

        sampleVars_.x.min = xaxis(xind_).min();
        sampleVars_.x.max = xaxis(xind_).max();
      }

      if (parseAxisRange(line, yaxis(yind_), false)) {
        if (sampleVars_.y.var != "")
          dummyVars_.setDummyVar("y", sampleVars_.y.var);

        sampleVars_.y.min = yaxis(yind_).min();
        sampleVars_.y.max = yaxis(yind_).max();
      }

      parseAxisRange(line, zaxis(zind_), false);
    }

    //----

    // if sample specified then check for per-plot range
    if (line.isString("sample")) {
      line.skipNonSpace();

      sample = true;
    }
  }

  //----

  if (! first1 || sample) {
    CGnuPlotAxisData sampleXAxis;

    if (parseAxisRange(line, sampleXAxis, false)) {
      sampleVars_.x.var = sampleXAxis.getDummyVar();
      sampleVars_.x.min = sampleXAxis.min();
      sampleVars_.x.max = sampleXAxis.max();
    }

    CGnuPlotAxisData sampleYAxis;

    if (parseAxisRange(line, sampleYAxis, false)) {
      sampleVars_.y.var = sampleYAxis.getDummyVar();
      sampleVars_.y.min = sampleYAxis.min();
      sampleVars_.y.max = sampleYAxis.max();
    }

    CGnuPlotAxisData sampleZAxis;

    if (parseAxisRange(line, sampleZAxis, false)) {
      sampleVars_.z.var = sampleZAxis.getDummyVar();
      sampleVars_.z.min = sampleZAxis.min();
      sampleVars_.z.max = sampleZAxis.max();
    }
  }

  //----

  CGnuPlotKeyTitle keyTitle;

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
      filename = lastFilename();
    }
    // special filename '-' (read lines from stdin)
    else if (filename == "-") {
    }
    // special filename '+' (expression file)
    else if (filename == "+") {
    }
    // special filename '++' (expression file)
    else if (filename == "++") {
    }

    if (isDebug())
      debugMsg("Filename: " + filename);

    setLastFilename(filename);

    style = dataStyle();
  }
  else if (line.isChar('$')) {
    line.skipChar();

    std::string name = readNonSpace(line);

    filename = "$" + name;

    setLastFilename(filename);

    style = dataStyle();
  }
  else {
    // function (comma separated list)
    while (true) {
      FunctionData functionData;

      if (! parseFunction(line, functionData))
        break;

      if (! functionData.isAssign)
        functions.push_back(functionData.function);
      else
        processAssignFunction(functionData.assign);

      if (! line.skipSpaceAndChar(','))
        break;
    }

    if (isDebug()) {
      for (const auto &f : functions)
        debugMsg("Function: " + f);
    }

    style = functionStyle();
  }

  line.skipSpace();

  //---

  CGnuPlotUsingCols usingCols(this);

  dataFile_.reset();

  imageStyle_  .reset();
  binaryFormat_.clear();

  setSmooth(Smooth::NONE);

  setBinary(false);

  matrixData().reset();

  // Read individual plot:
  //   <plot_data> [, <plot_data> ...]
  // TODO: using must come before title and with
  while (line.isValid() && ! line.isChar(',')) {
    int pos = line.pos();

    PlotVar plotVar;

    if (! parseOptionValue(this, line, plotVar)) {
      if (! parseModifiers3D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;

      continue;
    }

    line.skipSpace();

    // Read using columns
    // using <col1>[:<col2>[:<col3>[:...]]
    if      (plotVar == PlotVar::USING) {
      std::string usingStr = parseUsingStr(line);

      usingCols.init(usingStr);

      if (isDebug())
        usingCols.print(std::cerr);

      std::string format;

      if (parseString(line, format))
        usingCols.setFormat(format);
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
          debugMsg("with " + CStrUniqueMatch::valueToString(style));
      }

      parseModifiers3D(style, line, lineStyle, fillStyle, styleData, keyTitle);
    }
    // title string
    else if (plotVar == PlotVar::TITLE) {
      std::string titleStr;
      int         columnNum;

      parsePlotTitle_ = true;

      COptInt col;

      if      (isColumnHeader(line, col)) {
    //if      (line.isOneOf({"col", "columnhead", "columnheader", "columnheaders"}))
        setKeyColumnHeadNum(col.getValue(0));

        //(void) readNonSpaceNonComma(line);
      }
      else if (parseString(line, titleStr)) {
        if (isDebug())
          debugMsg("title " + titleStr);

        if (titleStr != "")
          keyTitle.str = titleStr;
      }
      else if (parseInteger(line, columnNum)) {
        setKeyColumnHeadNum(columnNum);
      }

      parsePlotTitle_ = false;

      line.skipSpace();
    }
    // disable title
    else if (plotVar == PlotVar::NOTITLE) {
      keyTitle.enabled = false;
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
      else {
        errorMsg("Invalid axes string '" + axesStr + "'");
      }
    }
    // linetype <lt>
    else if (plotVar == PlotVar::LINETYPE) {
      int lt;

      if (parseLineType(line, lt))
        lineStyle.setLineType(lt);
    }
    // linewidth <lw>
    else if (plotVar == PlotVar::LINEWIDTH) {
      double lw;

      if (parseReal(line, lw))
        lineStyle.setLineWidth(lw);
    }
    // fillstyle <fs>
    else if (plotVar == PlotVar::FILLSTYLE) {
      CGnuPlotFillStyle fillStyle1 = fillStyle;

      if (parseFillStyle(line, fillStyle1))
        fillStyle = fillStyle1; // TODO
    }
    // dashtype <dt>
    else if (plotVar == PlotVar::DASHTYPE) {
      CGnuPlotDash dash;

      if (parseDash(line, dash))
        lineStyle.setLineDash(dash);
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
        styleData.arrow = this->arrowStyle(as);
    }
    // heads
    else if (plotVar == PlotVar::HEADS) {
      styleData.arrow.setHeads(true, true);
    }
    // binary
    else if (plotVar == PlotVar::BINARY) {
      setBinary(true);
    }
    // matrix
    else if (plotVar == PlotVar::MATRIX) {
      matrixData().setMatrix(true);
    }
    // pixels
    else if (plotVar == PlotVar::PIXELS) {
      matrixData().setPixels(true);
    }
    // rowheaders
    else if (plotVar == PlotVar::ROWHEADERS) {
      matrixData().setRowHeaders(true);
    }
    // columnheaders
    else if (plotVar == PlotVar::COLUMNHEADERS) {
      matrixData().setColumnHeaders(true);
    }
    // flipy
    else if (plotVar == PlotVar::FLIPY) {
      imageStyle_.setFlipY(true);
    }
    else {
      errorMsg("Unhandled plot var '" + CStrUniqueMatch::valueToString(plotVar) + "'");

      line.setPos(pos);

      if (! parseModifiers3D(style, line, lineStyle, fillStyle, styleData, keyTitle))
        break;
    }
  }

  //---

  if      (! filename.empty()) {
    if (isBinary()) {
      if (isImageStyle(style)) {
        CGnuPlotPlotP plot = addImage3D(group, filename, style, usingCols);

        if (plot)
          addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
      }
      else {
        CGnuPlotPlotP plot = addBinary3D(group, filename, style, usingCols);

        if (plot)
          addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
      }
    }
    else {
      addFile3D(plots, group, filename, style, usingCols,
                lineStyle, fillStyle, styleData, keyTitle);
    }
  }
  else if (! functions.empty()) {
    CGnuPlotPlotP plot = addFunction3D(group, functions, style);

    if (plot)
      addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
  }

  //---

  while (line.isValid()) {
    line.skipSpace();

    std::string arg = readNonSpaceNonComma(line);

    if (arg != "")
      errorMsg("Invalid arg '" + arg + "'");

    if (line.isChar(',')) {
      errorMsg("Invalid comma '" + line.substr() + "'");
      break;
    }
  }
}

bool
CGnuPlot::
parseModifiers3D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &lineStyle,
                 CGnuPlotFillStyle &fillStyle, CGnuPlotStyleData &styleData,
                 CGnuPlotKeyTitle &keyTitle)
{
  bool modifiers = true;
  bool found     = false;

  while (modifiers) {
    line.skipSpace();

    if      (line.isOneOf({"linestyle", "ls"})) {
      line.skipNonSpace();

      int ls;

      if (parseInteger(line, ls))
        lineStyle = *getLineStyleInd(ls);

      found = true;
    }
    else if (line.isOneOf({"linetype", "lt"})) {
      line.skipNonSpace();

      int lt;

      if (parseLineType(line, lt))
        lineStyle.setLineType(lt);

      found = true;
    }
    else if (line.isOneOf({"linewidth", "lw"})) {
      line.skipNonSpace();

      double lw;

      if (parseReal(line, lw)) {
        if (style == CGnuPlotTypes::PlotStyle::LABELS)
          styleData.label.setLineWidth(lw);
        else
          lineStyle.setLineWidth(lw);
      }

      found = true;
    }
    else if (line.isOneOf({"linecolor", "lc"})) {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        lineStyle.setLineColor(c);

      found = true;
    }
    else if (line.isOneOf({"pointtype", "pt"})) {
      line.skipNonSpace();

      int         pt = 1;
      std::string ptstr;

      if      (parseInteger(line, pt)) {
        if (style == CGnuPlotTypes::PlotStyle::LABELS)
          styleData.label.setPointType(pt);
        else
          lineStyle.setPointType(pt);
      }
      else if (parseString(line, ptstr))
        lineStyle.setPointTypeStr(ptstr);
      else
        errorMsg("Invalid pointtype");

      found = true;
    }
    else if (line.isOneOf({"pointsize", "ps"})) {
      line.skipNonSpace();

      std::string style = readNonSpaceNonComma(line);

      double s;

      if      (style == "variable" || style == "var")
        pointStyle_.setVarSize(true);
      else if (CStrUtil::toReal(style, &s))
        lineStyle.setPointSize(s);

      found = true;
    }
    else if (line.isOneOf({"pointinterval", "pi"})) {
      line.skipNonSpace();

      int pi;

      if (parseInteger(line, pi))
        lineStyle.setPointInterval(pi);

      found = true;
    }
    else if (line.isOneOf({"fill", "fs"})) {
      line.skipNonSpace();

      CGnuPlotFillStyle fillStyle1 = fillStyle;

      if (parseFillStyle(line, fillStyle1))
        fillStyle = fillStyle1;

      found = true;
    }
    else if (line.isOneOf({"arrowstyle", "as"})) {
      line.skipNonSpace();

      int pos = line.pos();

      std::string style = readNonSpaceNonComma(line);

      int as;

      if (style == "variable" || style == "var")
        styleData.arrow.setVariable(true);
      else {
        line.setPos(pos);

        if (parseInteger(line, as))
          styleData.arrow = this->arrowStyle(as);
      }

      found = true;
    }
    else if (line.isString("filled")) {
      styleData.arrow.setFilled(true);

      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("nohead")) {
      styleData.arrow.setHeads(false, false);

      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("notitle")) {
      line.skipNonSpace();

      keyTitle.enabled = false;

      found = true;
    }
    else if (line.isString("boxed")) {
      line.skipNonSpace();

      styleData.text.setBoxed(true);

      found = true;
    }
    else if (line.isString("nohidden3d")) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isString("nocontours")) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isOneOf({"nosurf", "nosurface"})) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    else if (line.isOneOf({"palette", "pal"})) {
      // TODO
      line.skipNonSpace();

      found = true;
    }
    // font
    else if (line.isString("font")) {
      line.skipNonSpace();

      CFontPtr font;

      if (! parseFont(line, font))
        errorMsg("Missing font");

      if (style == CGnuPlotTypes::PlotStyle::LABELS)
        styleData.label.setFont(font);

      found = true;
    }
    // format=
    else if (line.isChars("format=")) {
      line.skipChars(7);

      std::string str;

      (void) parseString(line, str, "Invalid format");

      setBinaryFormat(str);

      found = true;
    }
    // array=
    else if (line.isChars("array=")) {
      line.skipChars(6);

      Sizes sizes;

      parseArrayValues(line, sizes);

      if (! sizes.empty()) {
        imageStyle_.setWidth (sizes[0].getWidth ());
        imageStyle_.setHeight(sizes[0].getHeight());
      }

      found = true;
    }
    // record=
    else if (line.isChars("record=")) {
      line.skipChars(7);

      Integers values;

      parseArrayValues(line, values);

      setRecordValues(values);

      found = true;
    }
    // filetype=
    else if (line.isChars("filetype=")) {
      line.skipChars(9);

      std::string arg = readNonSpace(line);

      CGnuPlotTypes::ImageType imageType;

      if (CStrUniqueMatch::stringToValue(arg, imageType))
        imageStyle_.setFileType(imageType);

      found = true;
    }
    // origin=
    else if (line.isChars("origin=")) {
      line.skipChars(7);

      typedef std::vector<CPoint3D> Points3D;

      Points3D originArray;

      do {
        if (line.skipSpaceAndChar('(')) {
          CPoint3D origin;

          if (parsePoint(line, origin)) {
            originArray.push_back(origin);
          }

          (void) line.skipSpaceAndChar(')');
        }
      }
      while (line.skipSpaceAndChar(':'));

      imageStyle_.setOriginArray(originArray);

      found = true;
    }
    // center=
    else if (line.isChars("center")) {
      int pos = line.pos();

      line.skipChars(6);

      if      (line.skipSpaceAndChar('=')) {
        if (line.skipSpaceAndChar('(')) {
          CPoint3D c;

          if (parsePoint(line, c))
            imageStyle_.setCenter(c);

          (void) line.skipSpaceAndChar(')');
        }

        found = true;
      }
      else if (style == CGnuPlotTypes::PlotStyle::LABELS) {
        CHAlignType halign = CHALIGN_TYPE_CENTER;

        styleData.text .setHAlign(halign); // TODO: remove
        styleData.label.setAlign (halign);

        found = true;
      }
      else
        line.setPos(pos);
    }
    // perp=
    else if (line.isChars("perp")) {
      int pos = line.pos();

      line.skipChars(4);

      if (line.skipSpaceAndChar('=')) {
        if (line.skipSpaceAndChar('(')) {
          CPoint3D p;

          if (parsePoint(line, p))
            imageStyle_.setPerpendicular(p);

          (void) line.skipSpaceAndChar(')');
        }
        else
          errorMsg("missing value for perp=");

        found = true;
      }
      else
        line.setPos(pos);
    }
    // skip=
    else if (line.isChars("skip=")) {
      int pos = line.pos();

      line.skipChars(4);

      if (line.skipSpaceAndChar('=')) {
        Reals reals;

        do {
          double r;

          if (parseReal(line, r))
            reals.push_back(r);
        }
        while (line.skipSpaceAndChar(':'));

        imageStyle_.setSkip(reals);

        found = true;
      }
      else
        line.setPos(pos);
    }
    // dx=, dy=
    else if (line.isChars("dx=") || line.isChars("dy=")) {
      bool isDX = line.isChars("dx=");

      line.skipChars(3);

      double r;

      if (parseReal(line, r)) {
        if (isDX) {
          imageStyle_.setDX(r);
          imageStyle_.setDY(r);
        }
        else
          imageStyle_.setDY(r);
      }

      found = true;
    }
    // dt=
    else if (line.isChars("dt=")) {
      line.skipChars(3);

      double r;

      if (parseReal(line, r))
        imageStyle_.setDT(r);

      found = true;
    }
    // rotation=
    else if (line.isChars("rot=") || line.isChars("rotate=") || line.isChars("rotation=")) {
      if (line.skipToChar('=')) line.skipChar();

      double a;

      if (parseReal(line, a)) {
        line.backSkipSpace();

        std::string arg = (! line.isSpace() ? line.readNonSpace() : "");

        if      (arg == "deg" || arg == "d")
          imageStyle_.setAngle(a);
        else if (arg == "pi")
          imageStyle_.setAngle(CAngle::Rad2Deg(a*M_PI));
        else
          imageStyle_.setAngle(CAngle::Rad2Deg(a));
      }

      found = true;
    }
    // endian=
    else if (line.isChars("endian=")) {
      line.skipChars(7);

      std::string arg = line.readNonSpace();

      if      (arg == "little")
        setEndian(Endian::LITTLE);
      else if (arg == "big")
        setEndian(Endian::BIG);
      else if (arg == "default")
        setEndian(Endian::DEFAULT);
      else if (arg == "swap")
        setEndian(Endian::SWAP);
      else
        errorMsg("Invalid endian \'" + arg + "'");

      line.readNonSpace();

      found = true;
    }
    // flip=
    else if (line.isChars("flip=")) {
      line.skipChars(5);

      std::string arg = line.readNonSpace();

      if (arg == "y")
        imageStyle_.setFlipY(true);

      found = true;
    }
    else if (style == CGnuPlotTypes::PlotStyle::LABELS) {
      if      (line.isString("offset")) {
        line.skipNonSpace();

        CPoint2D o(0, 0);

        if (parseOffset(line, o)) {
          styleData.text .setOffset(o); // TODO: remove
          styleData.label.setOffset(o);
        }

        found = true;
      }
      else if (line.isString("point")) {
        line.skipNonSpace();

        styleData.label.setShowPoint(true);

        found = true;
      }
      else if (line.isOneOf({"left", "center", "right"})) {
        line.skipNonSpace();

        CHAlignType halign = CHALIGN_TYPE_LEFT;

        if      (line.isString("left"))
          halign = CHALIGN_TYPE_LEFT;
        else if (line.isString("center"))
          halign = CHALIGN_TYPE_CENTER;
        else
          halign = CHALIGN_TYPE_RIGHT;

        styleData.text .setHAlign(halign); // TODO: remove
        styleData.label.setAlign (halign);

        found = true;
      }
      else if (line.isOneOf({"textcolor", "tc"})) {
        line.skipNonSpace();

        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          styleData.label.setTextColor(c);

        found = true;
      }
      else if (line.isString("hypertext")) {
        styleData.label.setHypertext(true);
      }
      else
        modifiers = false;
    }
    else
      modifiers = false;
  }

  return found;
}

// set ...
bool
CGnuPlot::
setCmd(const std::string &args)
{
  if (isDebug()) debugMsg("set " + args);

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
    defSystem_ = CoordSys::FIRST;

    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    bool created = false;

    auto arrow = lookupAnnotation<CGnuPlotArrow>(VariableName::ARROW, ind, created);
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
      if      (arg == "arrowstyle" || arg == "as") {
        int as;

        if (parseInteger(line, as)) {
          const CGnuPlotArrowStyle &as1 = arrowStyles_[as];

          arrow->setStyle(as1);
        }
      }
      else if (arg == "nohead" || arg == "head" || arg == "backhead" || arg == "heads") {
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

        if (parseInteger(line, ls))
          arrow->setLineStyle(ls);
      }
      else if (arg == "linetype" || arg == "lt") {
        int lt = -1;

        if (parseLineType(line, lt))
          arrow->setLineStyle(lt);
      }
      else if (arg == "linewidth" || arg == "lw") {
        double lw = 1.0;

        if (parseReal(line, lw))
          arrow->setLineWidth(lw);
      }
      else if (arg == "linecolor" || arg == "lc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          arrow->setLineColor(c);
      }
      else if (arg == "dashtype" || arg == "dt") {
        CGnuPlotDash dash;

        if (parseDash(line, dash))
          arrow->setLineDash(dash);
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
      for (const auto &i : IRangeItr::rangeClosed(1, 2)) {
        xaxis(i).setAutoScale(true); xaxis(i).unsetRange();
        yaxis(i).setAutoScale(true); yaxis(i).unsetRange();

        xaxis(i).setAutoScaleFix(false);
        yaxis(i).setAutoScaleFix(false);
      }

      zaxis(1).setAutoScale(true); zaxis(1).unsetRange();
      zaxis(1).setAutoScaleFix(false);
    }
    else if (arg == "noextend") {
      for (const auto &i : IRangeItr::rangeClosed(1, 2)) {
        xaxis(i).setAutoScaleFix(true);
        yaxis(i).setAutoScaleFix(true);
      }

      zaxis(1).setAutoScaleFix(false);
    }
    else if (arg == "x") {
      xaxis(1).setAutoScale(true); xaxis(1).unsetRange();
    }
    else if (arg == "y") {
      yaxis(1).setAutoScale(true); yaxis(1).unsetRange();
    }
    else if (arg == "x2") {
      xaxis(2).setAutoScale(true); xaxis(2).unsetRange();
    }
    else if (arg == "y2") {
      yaxis(2).setAutoScale(true); yaxis(2).unsetRange();
    }
    else if (arg == "z") {
      zaxis(1).setAutoScale(true); zaxis(1).unsetRange();
    }

    else if (arg == "xy") {
      xaxis(1).setAutoScale(true); xaxis(1).unsetRange();
      yaxis(1).setAutoScale(true); yaxis(1).unsetRange();
    }
    else if (arg == "xmin")
      xaxis(1).setAutoScaleMin(true);
    else if (arg == "ymin")
      yaxis(1).setAutoScaleMin(true);
    else if (arg == "xmax")
      xaxis(1).setAutoScaleMax(true);
    else if (arg == "ymax")
      yaxis(1).setAutoScaleMax(true);

    else if (arg == "fix") {
      xaxis(1).setAutoScaleFix(true);
      yaxis(1).setAutoScaleFix(true);
    }
    else if (arg == "xfix")
      xaxis(1).setAutoScaleFix(true);
    else if (arg == "yfix")
      yaxis(1).setAutoScaleFix(true);

    else if (arg == "xfixmin")
      xaxis(1).setAutoScaleFixMin(true);
    else if (arg == "yfixmin")
      yaxis(1).setAutoScaleFixMin(true);
    else if (arg == "xfixmax")
      xaxis(1).setAutoScaleFixMax(true);
    else if (arg == "yfixmax")
      yaxis(1).setAutoScaleFixMax(true);
    else
      errorMsg("Unhandled autoscale " + arg);
  }
  // set bars {small | large | fullwidth | size} ] {front | back}
  else if (var == VariableName::BARS) {
    line.skipSpace();

    double size  = 1.0;

    if (line.isOneOf({"small", "large", "fullwidth"})) {
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

    if      (line.isOneOf({"front", "back"})) {
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
    else
      margin_.resetBottom();
  }
  // set border {<integer>} {front | back} {linewidth | lw <line_width>}
  //            {{linestyle | ls <line_style>} | {linetype | lt <line_type>}}
  else if (var == VariableName::BORDER) {
    std::string arg = readNonSpace(line);

    if (arg == "")
      axesData_.resetBorderSides();
    else {
      while (arg != "") {
        if      (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) axesData_.setBorderLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) axesData_.setBorderLayer(DrawLayer::BACK);
          else if (arg == "behind") axesData_.setBorderLayer(DrawLayer::BEHIND);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            axesData_.setBorderWidth(lw);
        }
        else if (arg == "linestyle" || arg == "ls") {
          int ls;

          if (parseInteger(line, ls))
            axesData_.setBorderStyle(ls);
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt;

          if (parseLineType(line, lt))
            axesData_.setBorderType(lt);
        }
        else {
          int i;

          if      (getIntegerVariable(arg, i))
            axesData_.setBorderSides(i);
          else if (CStrUtil::toInteger(arg, &i))
            axesData_.setBorderSides(i);
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
          else
            errorMsg("Invalid boxwidth '" + arg + "'");
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
  // set cntrlabel onecolor
  else if (var == VariableName::CNTRLABEL) {
    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "format") {
        std::string format;

        if (parseString(line, format))
          contourData_.setLabelFormat(format);
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          contourData_.setLabelFont(font);
      }
      else if (arg == "start") {
        int n;

        if (parseInteger(line, n))
          contourData_.setLabelStart(n);
      }
      else if (arg == "interval") {
        int n;

        if (parseInteger(line, n))
          contourData_.setLabelInterval(n);
      }
      else if (arg == "onecolor") {
        // TODO
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
      }

      arg = readNonSpace(line);
    }
  }
  // set cntrparam { { linear | cubicspline | bspline | points <n> | order <n> |
  //                   levels { auto {<n>} | <n> | discrete <z1> {,<z2>{,<z3>...}} |
  //                            incremental <start>, <incr> {,<end>} } } }
  else if (var == VariableName::CNTRPARAM) {
    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "linear") {
        contourData_.setStyle(CGnuPlotContourData::Style::LINEAR);
      }
      else if (arg == "cubic" || arg == "cubicspline") {
        contourData_.setStyle(CGnuPlotContourData::Style::CUBICSPLINE);
      }
      else if (arg == "bspline") {
        contourData_.setStyle(CGnuPlotContourData::Style::BSPLINE);
      }
      else if (arg == "points") {
        int n;

        if (parseInteger(line, n))
          contourData_.setPoints(n);
      }
      else if (arg == "order") {
        int n;

        if (parseInteger(line, n))
          contourData_.setOrder(n);
      }
      else if (arg == "levels" || arg == "level") {
        int pos = line.pos();

        std::string arg1 = readNonSpace(line);

        while (arg1 != "") {
          if      (arg1 == "auto") {
            int n;

            if (parseInteger(line, n))
              contourData_.setAutoLevels(n);
          }
          else if (arg1 == "disc" || arg1 == "discrete") {
            Reals  z;
            double z1;

            if (parseReal(line, z1))
              z.push_back(z1);

            while (line.skipSpaceAndChar(',')) {
              if (parseReal(line, z1))
                z.push_back(z1);
            }

            contourData_.setDiscreteLevels(z);
          }
          else if (arg1 == "incr" || arg1 == "incremental") {
            double   start = 0, incr = 1;
            COptReal end;

            if (parseReal(line, start)) {
              if (line.skipSpaceAndChar(',')) {
                if (parseReal(line, incr)) {
                  if (line.skipSpaceAndChar(',')) {
                    double r;

                    if (parseReal(line, r))
                      end = r;
                  }

                  CGnuPlotContourData::Range range;

                  if (end.isValid())
                    range = CGnuPlotContourData::Range(start, incr, end.getValue());
                  else
                    range = CGnuPlotContourData::Range(start, incr);

                  contourData_.setIncrementalRange(range);
                }
              }
            }
          }
          else {
            line.setPos(pos);

            int n = 0;

            if (parseInteger(line, n)) {
              contourData_.setLevels(n);
            }
            else
              break;
          }

          pos = line.pos();

          arg1 = readNonSpace(line);
        }
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
      }

      arg = readNonSpace(line);
    }
  }
  // set colorbox
  // set colorbox { { vertical | horizontal }
  //                { default | user }
  //                { origin x, y }
  //                { size x, y }
  //                { front | back }
  //                { noborder | bdefault | border [line style] } }
  else if (var == VariableName::COLORBOX) {
    defSystem_ = CoordSys::FIRST;

    colorBox_.setEnabled(true);

    std::string arg = readNonSpaceNonComma(line);

    while (arg != "") {
      if      (arg == "vertical" || arg == "vert")
        colorBox_.setVertical(true);
      else if (arg == "horizontal" || arg == "horiz")
        colorBox_.setVertical(false);
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
      else {
        errorMsg("Invalid arg '" + arg + "'");
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
      contourData_.setPos(CGnuPlotContourData::DrawPos::BASE);
    else if (arg == "surface")
      contourData_.setPos(CGnuPlotContourData::DrawPos::SURFACE);
    else if (arg == "both")
      contourData_.setPos(CGnuPlotContourData::DrawPos::BOTH);

    contourData_.setEnabled(true);
  }
  // set dashtype
  else if (var == VariableName::DASHTYPE) {
    int type;

    if (! parseInteger(line, type))
      return false;

    CLineDash dash;

    if (! parseDash(line, dash))
      return false;

    lineDashes_[type] = dash;
  }
  // set datafile fortran
  // set datafile nofpe_trap
  // set datafile missing "<string>"
  // set datafile separator {whitespace | tab | comma | "<chars>"}
  // set datafile commentschar {"<string>"}
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
    // set datafile separator {whitespace | tab | comma | "<chars>"}
    else if (var1 == DataFileVar::SEPARATOR) {
      if (line.isChar('"') || line.isChar('\'')) {
        std::string chars;

        if (parseString(line, chars, "Invalid character string")) {
          CParseLine line(chars);

          if (line.isChar('\\')) {
            std::string str1;

            if (replaceEscapeChar(line, str1))
              chars = str1 + line.substr();
          }

          if (chars.size() != 1) {
            errorMsg("Only one character allowed for separator");
            return false;
          }

          dataFile_.setSeparator(chars[0]);
        }
      }
      else {
        std::string sepStr = readNonSpace(line);

        if      (sepStr == "whitespace")
          dataFile_.resetSeparator();
        else if (sepStr == "tab")
          dataFile_.setSeparator('\t');
        else if (sepStr == "comma")
          dataFile_.setSeparator(',');
        else if (sepStr == "")
          dataFile_.resetSeparator();
        else
          errorMsg("Invalid separator type '" + sepStr + "'");
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

    dummyVars().dummyVars(var1, var2, isParametric(), isPolar(), /*is3D*/ false);

    if (line.isValid()) {
      var1 = readNonSpaceNonComma(line);

      if (line.skipSpaceAndChar(',')) {
        if (line.isValid()) {
          var2 = line.readNonSpace();
        }
      }
    }

    dummyVars_.setDummyVars(var1, var2, isParametric(), isPolar());
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
  // set format {<axes>} {"<format-string>"} {numeric|timedate|geographic}
  else if (var == VariableName::FORMAT) {
    line.skipSpace();

    int pos = line.pos();

    std::string arg = readNonSpace(line);

    AxisTypeIdSet axisTypeIdSet;

    if (! stringToAxes(arg, axisTypeIdSet))
      line.setPos(pos);

    std::string formatStr;

    if (! parseString(line, formatStr))
      formatStr = "%g";

    if (! axisTypeIdSet.empty()) {
      for (int i = 1; i <= 2; ++i) {
        if (axisTypeIdSet.count(AxisTypeId(AxisType::X, i))) {
          if (xaxis(i).isTime())
            xaxis(i).setTimeFmt(formatStr);
          else
            xaxis(i).setFormat(formatStr);
        }

        if (axisTypeIdSet.count(AxisTypeId(AxisType::Y, i))) {
          if (yaxis(i).isTime())
            yaxis(i).setTimeFmt(formatStr);
          else
            yaxis(i).setFormat(formatStr);
        }
      }

      if (axisTypeIdSet.count(AxisTypeId(AxisType::Z, 1)))
        zaxis(1).setFormat(formatStr);

      if (axisTypeIdSet.count(AxisTypeId(AxisType::R, 1)))
        raxis().setFormat(formatStr);

      if (axisTypeIdSet.count(AxisTypeId(AxisType::CB, 1)))
        colorBox_.axis().setFormat(formatStr);
    }
    else {
      for (int i = 1; i <= 2; ++i) {
        xaxis(i).setFormat(formatStr);
        yaxis(i).setFormat(formatStr);
      }

      zaxis(1).setFormat(formatStr);
      raxis ().setFormat(formatStr);

      colorBox_.axis().setFormat(formatStr);
    }

    line.skipSpace();

    std::string arg1 = readNonSpace(line);

    if (arg1 == "numeric" || arg1 == "timedate" || arg1 == "geographic") {
      // TODO
      errorMsg(std::string("set format '") + arg1 + "' not handled");
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
    int ns = 0, nt = 0, nw = 0;

    std::string arg = readNonSpace(line);

    if (arg != "") {
      while (arg != "") {
        if      (arg == "xtics"     || arg == "x"   ) xaxis(1).setGridMajor(true);
        else if (arg == "ytics"     || arg == "y"   ) yaxis(1).setGridMajor(true);
        else if (arg == "ztics"     || arg == "z"   ) zaxis(1).setGridMajor(true);
        else if (arg == "noxtics"   || arg == "nox" ) xaxis(1).setGridMajor(false);
        else if (arg == "noytics"   || arg == "noy" ) yaxis(1).setGridMajor(false);
        else if (arg == "noztics"   || arg == "noz" ) zaxis(1).setGridMajor(false);

        else if (arg == "mxtics"    || arg == "mx"  ) xaxis(1).setGridMinor(true);
        else if (arg == "mytics"    || arg == "my"  ) yaxis(1).setGridMinor(true);
        else if (arg == "mztics"    || arg == "mz"  ) zaxis(1).setGridMinor(true);
        else if (arg == "nomxtics"  || arg == "nomx") xaxis(1).setGridMinor(false);
        else if (arg == "nomytics"  || arg == "nomy") yaxis(1).setGridMinor(false);
        else if (arg == "nomztics"  || arg == "nomz") zaxis(1).setGridMinor(false);

        else if (arg == "x2tics"    || arg == "x2"  ) xaxis(2).setGridMajor(true);
        else if (arg == "y2tics"    || arg == "y2"  ) yaxis(2).setGridMajor(true);
        else if (arg == "nox2tics"  || arg == "nox2") xaxis(2).setGridMajor(false);
        else if (arg == "noy2tics"  || arg == "noy2") yaxis(2).setGridMajor(false);

        else if (arg == "mx2tics"   || arg == "mx2"  ) xaxis(1).setGridMinor(true);
        else if (arg == "my2tics"   || arg == "my2"  ) yaxis(1).setGridMinor(true);
        else if (arg == "nomx2tics" || arg == "nomx2") xaxis(1).setGridMinor(false);
        else if (arg == "nomy2tics" || arg == "nomy2") yaxis(1).setGridMinor(false);

        else if (arg == "cbtics"    || arg == "cb"   ) colorBox_.axis().setGridMajor(true);
        else if (arg == "nocbtics"  || arg == "nocb" ) colorBox_.axis().setGridMajor(false);

        else if (arg == "mcbtics"   || arg == "mcb"  ) colorBox_.axis().setGridMinor(true);
        else if (arg == "nomcbtics" || arg == "nomcb") colorBox_.axis().setGridMinor(false);

        else if (arg == "polar") {
          raxis().setGridMajor(true);

          double a = 0;

          if (parseReal(line, a)) {
            double a1 = (! isAngleDegrees() ? CAngle::Rad2Deg(a) : a);

            axesData_.setGridPolarAngle(a1);
          }
        }
        else if (arg == "front" || arg == "back" || arg == "layerdefault") {
          if      (arg == "front"       ) axesData_.setGridLayer(DrawLayer::FRONT);
          else if (arg == "back"        ) axesData_.setGridLayer(DrawLayer::BACK);
          else if (arg == "layerdefault") axesData_.setGridLayer(DrawLayer::DEFAULT);
        }
        else if (arg == "linestyle" || arg == "ls") {
          int ls = -1;

          if (parseInteger(line, ls)) {
            if (ns == 0)
              axesData_.setGridMajorLineStyle(ls);
            else
              axesData_.setGridMinorLineStyle(ls);
          }

          ++ns;
        }
        else if (arg == "linetype" || arg == "lt") {
          int lt = 0;

          if (parseLineType(line, lt)) {
            if (nt == 0)
              axesData_.setGridMajorLineType(lt);
            else
              axesData_.setGridMinorLineType(lt);
          }

          ++nt;
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw)) {
            if (nw == 0)
              axesData_.setGridMajorLineWidth(lw);
            else
              axesData_.setGridMinorLineWidth(lw);
          }

          ++nw;
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    else {
      xaxis(1).setGridMajor(true);
      yaxis(1).setGridMajor(true);
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
      if      (arg == "front" || arg == "back") {
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
      else {
        errorMsg("Invalid arg '" + arg + "'");
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
      else {
        errorMsg("Invalid arg '" + arg + "'");
      }

      arg = readNonSpace(line);
    }
  }
  // set isosamples <iso_1> [,<iso_2>]
  else if (var == VariableName::ISOSAMPLES) {
    int isamples1, isamples2;

    isoSamples_.get(isamples1, isamples2);

    int i;

    if (parseInteger(line, i)) {
      isamples1 = i;
      isamples2 = i;
    }

    if (line.skipSpaceAndChar(',')) {
      if (parseInteger(line, i))
        isamples2 = i;
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
  //         {{no}box { {linestyle | ls <line_style>} } { {fillstyle | fc <fill_style} } }
  //         {maxcols { <n> | auto }}
  //         {maxrows { <n> | auto }}
  else if (var == VariableName::KEY) {
    defSystem_ = CoordSys::FIRST;

    keyData_.setDisplayed(true);

    bool hSet         = false;
    bool directionSet = false;

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
        // outside defaults to  top, right and vertical
        keyData_.setOutside(arg == "outside" || arg == "out");
      }
      else if (arg == "lmargin") {
        keyData_.setLMargin(true);
        keyData_.setRMargin(false);
      }
      else if (arg == "rmargin") {
        keyData_.setLMargin(false);
        keyData_.setRMargin(true);
      }
      else if (arg == "tmargin") {
        keyData_.setBMargin(false);
        keyData_.setTMargin(true);
      }
      else if (arg == "bmargin") {
        keyData_.setBMargin(true);
        keyData_.setTMargin(false);
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
        if (! hSet) {
          keyData_.setHAlign(CHALIGN_TYPE_CENTER);
          hSet = true;
        }
        else
          keyData_.setVAlign(CVALIGN_TYPE_CENTER);
      }
      else if (arg == "vertical"   || arg == "vert" ||
               arg == "horizontal" || arg == "horiz") {
        keyData_.setVertical(arg == "vertical" || arg == "vert");
        directionSet = true;
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
      else if (arg == "sample" || arg == "samplen") {
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

        int pos1 = line.pos();

        std::string arg1 = readNonSpace(line);

        if (arg1 == "column" || arg1 == "columnhead" ||
            arg1 == "columnheader" || arg1 == "columnheaders")
          setKeyAutoColumnHeadNum();
        else
          line.setPos(pos1);
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

        while (line.isValid()) {
          int pos1 = line.pos();

          std::string arg1 = readNonSpace(line);

          if      (arg1 == "linetype" || arg1 == "lt") {
            int lt;

            if (parseLineType(line, lt))
              keyData_.setBoxLineType(lt);
          }
          else if (arg1 == "linestyle" || arg1 == "ls") {
            int ls;

            if (parseInteger(line, ls))
              keyData_.setBoxLineStyle(ls);
          }
          else if (arg1 == "linewidth" || arg1 == "lw") {
            double lw = 1.0;

            if (parseReal(line, lw))
              keyData_.setBoxLineWidth(lw);
          }
          else if (arg1 == "fillstyle" || arg1 == "fs") {
            CGnuPlotFillStyle fs;

            if (parseFillStyle(line, fs))
              keyData_.setBoxFillStyle(fs);
          }
          else if (arg1 == "dashtype" || arg1 == "dt") {
            CGnuPlotDash dash;

            if (parseDash(line, dash))
              keyData_.setBoxLineDash(dash);
          }
          else {
            line.setPos(pos1);
            break;
          }
        }
      }
      else if (arg == "nobox") {
        keyData_.setBox(false);
      }
      else if (arg == "above" || arg == "over") {
        keyData_.setTMargin(true);
        keyData_.setBMargin(true);

        if (! hSet)
          keyData_.setHAlign(CHALIGN_TYPE_CENTER);

        if (! directionSet)
          keyData_.setVertical(false);
      }
      else if (arg == "below" || arg == "under") {
        keyData_.setTMargin(false);
        keyData_.setBMargin(true);

        if (! hSet)
          keyData_.setHAlign(CHALIGN_TYPE_CENTER);

        if (! directionSet)
          keyData_.setVertical(false);
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
        errorMsg("Invalid key arg \"" + arg + "\"");
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
    defSystem_ = CoordSys::FIRST;

    line.skipSpace();

    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    //---

    // if no tag then use lowest available value
    bool created = false;

    auto label = lookupAnnotation<CGnuPlotLabel>(VariableName::LABEL, ind, created);
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

          if (parseLineType(line, lt))
            label->setLineType(lt);
        }
        else if (arg1 == "pointtype" || arg1 == "pt") {
          int pt = 0;

          if (parseInteger(line, pt))
            label->setPointType(pt);
          else
            errorMsg("Invalid pointtype");
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

        if (parseOffset(line, o))
          label->setOffset(o);
      }
      else if (arg == "boxed") {
        label->boxStroke()->setEnabled(true);
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
  // set linetype {index} {{linecolor | lc } <colorspace>}
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

      CGnuPlotLineTypeP lineType = getLineTypeInd(ind);

      line.skipSpace();

      while (line.isValid()) {
        std::string arg = readNonSpace(line);

        if      (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec c;

          if (parseColorSpec(line, c))
            lineType->setLineColor(c);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            lineType->setLineWidth(lw);
        }
        else if (arg == "pointtype" || arg == "pt") {
          int         pt = 1;
          std::string ptstr;

          if      (parseInteger(line, pt))
            lineType->setPointType(pt);
          else if (parseString(line, ptstr))
            lineType->setPointType(ptstr);
          else
            errorMsg("Invalid pointtype");
        }
        else if (arg == "pointsize" || arg == "ps") {
          double ps;

          if (parseReal(line, ps))
            lineType->setPointSize(ps);
        }
        else if (arg == "pointinterval" || arg == "pi") {
          int pi;

          if (parseInteger(line, pi))
            lineType->setPointInterval(pi);
        }
        else if (arg == "dashtype" || arg == "dt") {
          CGnuPlotDash dash;

          if (parseDash(line, dash))
            lineType->setLineDash(dash);
        }
        else {
          errorMsg("Invalid line modifier '" + arg + "'");
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
    else
      margin_.resetLeft();
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

    if (axesStr != "") {
      AxisTypeIdSet axisTypeIdSet;

      if (! stringToAxes(axesStr, axisTypeIdSet))
        errorMsg("Invalid logscale axes '" + axesStr + "'");

      for (int i = 1; i <= 2; ++i) {
        if (axisTypeIdSet.count(AxisTypeId(AxisType::X, i))) xaxis(i).setLogBase(base);
        if (axisTypeIdSet.count(AxisTypeId(AxisType::Y, i))) yaxis(i).setLogBase(base);
      }

      if (axisTypeIdSet.count(AxisTypeId(AxisType::Z, 1))) zaxis(1).setLogBase(base);
      if (axisTypeIdSet.count(AxisTypeId(AxisType::R, 1))) raxis( ).setLogBase(base);

      if (axisTypeIdSet.count(AxisTypeId(AxisType::CB, 1))) colorBox_.axis().setLogBase(base);
    }
    else {
      for (int i = 1; i <= 2; ++i) {
        xaxis(i).setLogBase(base);
        yaxis(i).setLogBase(base);
      }

      zaxis(1).setLogBase(base);
      raxis( ).setLogBase(base);

      colorBox_.axis().setLogBase(base);
    }
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
        mouseData_.setPolarDistance(CGnuPlotMouseData::PolarDistanceType::DEG);
      }
      else if (arg == "polardistancetan") {
        mouseData_.setPolarDistance(CGnuPlotMouseData::PolarDistanceType::TAN);
      }
      else if (arg == "nopolardistance") {
        mouseData_.setPolarDistance(CGnuPlotMouseData::PolarDistanceType::NONE);
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
      else {
        errorMsg("Invalid arg '" + arg + "'");
      }

      arg = readNonSpace(line);
    }
  }
  // set multiplot { title <page title> {font <fontspec>} {enhanced|noenhanced} }
  //               { layout <rows>,<cols>
  //                 {rowsfirst|columnsfirst} {downwards|upwards}
  //                 {scale <xscale>{,<yscale>}} {offset <xoff>{,<yoff>}}
  //                 {margins <left>,<right>,<bottom>,<top>}
  //                 {spacing <xspacing>{,<yspacing>}}
  //               }
  else if (var == VariableName::MULTIPLOT) {
    if (! multiplot_.isValid())
      multiplot_ = createMultiplot();

    multiplot_->setEnabled(true);
    //multiplot_->setAutoFit(true);

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "title") {
        std::string titleStr;

        if (parseString(line, titleStr))
          multiplot_->setTitle(titleStr);
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          multiplot_->setTitleFont(font);
      }
      else if (arg == "enhanced" || arg == "noenhanced") {
        multiplot_->setEnhanced(arg == "enhanced");
      }
      else if (arg == "layout") {
        int i;

        if (parseInteger(line, i))
          multiplot_->setRows(i);

        if (line.skipSpaceAndChar(',')) {
          if (parseInteger(line, i))
            multiplot_->setCols(i);
        }

        multiplot_->setAutoFit(false);
      }
      else if (arg == "rowsfirst" || arg == "rows") {
        multiplot_->setRowsFirst(true);
      }
      else if (arg == "columnsfirst" || arg == "columns") {
        multiplot_->setRowsFirst(false);
      }
      else if (arg == "downwards" || arg == "down") {
        multiplot_->setDownward(true);
      }
      else if (arg == "upwards" || arg == "up") {
        multiplot_->setDownward(false);
      }
      else if (arg == "scale") {
        double r;

        if (parseReal(line, r))
          multiplot_->setXScale(r);

        if (line.skipSpaceAndChar(',')) {
          if (parseReal(line, r))
            multiplot_->setYScale(r);
        }
      }
      else if (arg == "offset") {
        CPoint2D o;

        if (parseOffset(line, o)) {
          multiplot_->setXOffset(o.x);
          multiplot_->setYOffset(o.y);
        }
      }
      else if (arg == "margin" || arg == "margins") {
        double l, r, t, b;

        multiplot_->getMargins(l, r, t, b);

        if (parseReal(line, l)) {
          if (line.skipSpaceAndChar(',') && parseReal(line, r)) {
            if (line.skipSpaceAndChar(',') && parseReal(line, t)) {
              if (line.skipSpaceAndChar(',') && parseReal(line, b)) {
                multiplot_->setMargins(l, r, t, b);
              }
            }
          }
        }
      }
      else if (arg == "spacing") {
        double x, y;

        multiplot_->getSpacing(x, y);

        if (parseReal(line, x)) {
          if (line.skipSpaceAndChar(',') && parseReal(line, y)) {
            multiplot_->setSpacing(x, y);
          }
        }
      }
      else if (arg == "autofit") {
        multiplot_->setAutoFit(true);
      }
      else if (arg == "next") {
        // TODO
      }
      else if (arg == "prev" || arg == "previous") {
        // TODO
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
    xaxis(2).setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      xaxis(2).resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        xaxis(2).setMinorTicsFreq(r);
    }
  }
  // set mxtics {<freq> | default}
  else if (var == VariableName::MXTICS) {
    xaxis(1).setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      xaxis(1).resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        xaxis(1).setMinorTicsFreq(r);
    }
  }
  // set my2tics {<freq> | default}
  else if (var == VariableName::MY2TICS) {
    yaxis(2).setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      yaxis(2).resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        yaxis(2).setMinorTicsFreq(r);
    }
  }
  // set mytics {<freq> | default}
  else if (var == VariableName::MYTICS) {
    yaxis(1).setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      yaxis(1).resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        yaxis(1).setMinorTicsFreq(r);
    }
  }
  // set mztics {<freq> | default}
  else if (var == VariableName::MZTICS) {
    zaxis(1).setMinorTicsDisplayed(true);

    line.skipSpace();

    if (line.isString("default"))
      zaxis(1).resetMinorTicsFreq();
    else {
      double r;

      if (parseReal(line, r))
        zaxis(1).setMinorTicsFreq(r);
    }
  }
  // set nox2tics
  else if (var == VariableName::NOX2TICS) {
    xaxis(2).setShowTics(false);
  }
  // set noxtics
  else if (var == VariableName::NOXTICS) {
    xaxis(1).setShowTics(false);
  }
  // set noy2tics
  else if (var == VariableName::NOY2TICS) {
    yaxis(2).setShowTics(false);
  }
  // set noytics
  else if (var == VariableName::NOYTICS) {
    yaxis(1).setShowTics(false);
  }
  // set object <index> <object-type> <object-properties>
  //            {front|back|behind} {fc|fillcolor <colorspec>} {fs <fillstyle>}
  //            {default} {lw|linewidth <width>}
  else if (var == VariableName::OBJECT) {
    int ind = -1;

    if (! parseInteger(line, ind))
      ind = -1;

    bool                       created    = false;
    CGnuPlotTypes::ObjectType  type       = CGnuPlotTypes::ObjectType::NONE;
    CGnuPlotGroupAnnotation   *annotation = 0;

    if (! parseOptionValue(this, line, type)) {
      auto annotation = getAnnotation<CGnuPlotGroupAnnotation>(VariableName::OBJECT, ind);

      if (annotation)
        type = annotation->type();
    }

    // set object <index> circle {at|center} <position> size <radius>
    //            {arc [<begin>:<end>]}
    //            {<other-object-properties>}
    if      (type == CGnuPlotTypes::ObjectType::CIRCLE) {
      defSystem_ = CoordSys::FIRST;

      auto circle = lookupAnnotation<CGnuPlotCircle>(VariableName::OBJECT, ind, created);
      if (! circle) return false;

      annotation = circle;

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
        else if (arg == "radius") {
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
        else if (arg == "fillstyle" || arg == "fs") {
          CGnuPlotFillStyle fs;

          if (parseFillStyle(line, fs))
            circle->setFillStyle(fs);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            circle->setLineWidth(lw);
        }
        else if (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) circle->setLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) circle->setLayer(DrawLayer::BACK);
          else if (arg == "behind") circle->setLayer(DrawLayer::BEHIND);
        }
        else if (arg == "clip" || arg == "noclip") {
          circle->setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid circle arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> ellipse {at|center} <position> size <w>,<h>
    //            {angle <orientation>} {units xy|xx|yy}
    //            {<other-object-properties>}
    else if (type == CGnuPlotTypes::ObjectType::ELLIPSE) {
      defSystem_ = CoordSys::FIRST;

      auto ellipse = lookupAnnotation<CGnuPlotEllipse>(VariableName::OBJECT, ind, created);
      if (! ellipse) return false;

      annotation = ellipse;

      //---

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "at" || arg == "center") {
          CGnuPlotPosition p;

          if (parsePosition(line, p))
            ellipse->setCenter(p);
        }
        else if (arg == "size") {
          CGnuPlotSize size;

          if (! parseSize(line, size))
            continue;

          ellipse->setSize(size);
        }
        else if (arg == "angle") {
          // always degrees
          double a;

          if (parseReal(line, a))
            ellipse->setAngle(a);
        }
        else if (arg == "units") {
          arg = readNonSpace(line);

          if      (arg == "xx")
            ellipse->setUnits(CGnuPlotTypes::EllipseUnits::XX);
          else if (arg == "xy")
            ellipse->setUnits(CGnuPlotTypes::EllipseUnits::XY);
          else if (arg == "yy")
            ellipse->setUnits(CGnuPlotTypes::EllipseUnits::YY);
          else
            errorMsg("Invalid units arg '" + arg + "'");
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
        else if (arg == "linewidth" || arg == "lw") {
          double lw = 1.0;

          if (parseReal(line, lw))
            ellipse->setLineWidth(lw);
        }
        else if (arg == "front" || arg == "back" || arg == "behind") {
          if      (arg == "front" ) ellipse->setLayer(DrawLayer::FRONT);
          else if (arg == "back"  ) ellipse->setLayer(DrawLayer::BACK);
          else if (arg == "behind") ellipse->setLayer(DrawLayer::BEHIND);
        }
        else if (arg == "clip" || arg == "noclip") {
          ellipse->setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid ellipse arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> polygon
    //            from <position> to <position> ... {to <position>} |
    //            from <position> rto <position> ... {rto <position>}
    else if (type == CGnuPlotTypes::ObjectType::POLYGON) {
      defSystem_ = CoordSys::FIRST;

      auto poly = lookupAnnotation<CGnuPlotPolygon>(VariableName::OBJECT, ind, created);
      if (! poly) return false;

      annotation = poly;

      //---

      CGnuPlotPosition from, to;

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "from") {
          if (parsePosition(line, from))
            poly->addPoint(from);
        }
        else if (arg == "to") {
          if (parsePosition(line, to))
            poly->addPoint(to);
        }
        else if (arg == "rto") {
          if (parsePosition(line, to))
            poly->addRPoint(to);
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

          if (parseLineType(line, lt))
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
        else if (arg == "clip" || arg == "noclip") {
          poly->setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid polygon arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set object <index> rectangle
    //            {from <position> {to|rto} <position> |
    //            center|at <position> size <w>,<h>}
    else if (type == CGnuPlotTypes::ObjectType::RECTANGLE) {
      defSystem_ = CoordSys::FIRST;

      auto rect = lookupAnnotation<CGnuPlotRectangle>(VariableName::OBJECT, ind, created);
      if (! rect) return false;

      annotation = rect;

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

          if (parseLineType(line, lt))
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
        else if (arg == "clip" || arg == "noclip") {
          rect->setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid rect arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    else {
      errorMsg("Invalid object type");
      return false;
    }

    if (created && annotation)
      annotation->initClip();
  }
  // set offsets <left>, <right>, <top>, <bottom>
  else if (var == VariableName::OFFSETS) {
    defSystem_ = CoordSys::FIRST;

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
    if (! isParametric()) {
      setParametric(true);

      dummyVars_.resetDummyVars();
    }

    // TODO: dummy variable is t for curves, u/v for surfaces
  }
  // set paxis <no> {range <range-options> | {tics <tic-options>}}
  else if (var == VariableName::PAXIS) {
    int i;

    if (parseInteger(line, i)) {
      paxis(i).setDisplayed(true);

      std::string arg = readNonSpace(line);

      if      (arg == "range")
        parseAxisRange(line, paxis(i));
      else if (arg == "tics")
        parseAxesTics(line, paxis(i));
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
        pm3D_.clearPos();

        std::string arg1 = readNonSpaceNonComma(line);

        for (const auto &a1 : arg1) {
          if      (a1 == 's') pm3D_.addPos(CGnuPlotPm3DData::PosType::SURFACE);
          else if (a1 == 't') pm3D_.addPos(CGnuPlotPm3DData::PosType::TOP);
          else if (a1 == 'b') pm3D_.addPos(CGnuPlotPm3DData::PosType::BOTTOM);
        }
      }
      else if (arg == "interpolate") {
        int steps1, steps2;

        if (parseInteger(line, steps1)) {
          if (line.skipSpaceAndChar(',')) {
            if (parseInteger(line, steps2)) {
              pm3D_.setSteps(steps1, steps2);
            }
          }
        }
      }
      else if (arg == "scansautomatic") {
        pm3D_.setScanType(CGnuPlotPm3DData::ScanType::AUTOMATIC);
      }
      else if (arg == "scansforward") {
        pm3D_.setScanType(CGnuPlotPm3DData::ScanType::FORWARD);
      }
      else if (arg == "scansbackward") {
        pm3D_.setScanType(CGnuPlotPm3DData::ScanType::BACKWARD);
      }
      else if (arg == "depthorder") {
        pm3D_.setScanType(CGnuPlotPm3DData::ScanType::DEPTH_ORDER);
      }
      else if (arg == "flush") {
        std::string arg1 = readNonSpaceNonComma(line);

        if      (arg1 == "begin" ) { pm3D_.setFlushType(CGnuPlotPm3DData::FlushType::BEGIN ); }
        else if (arg1 == "center") { pm3D_.setFlushType(CGnuPlotPm3DData::FlushType::CENTER); }
        else if (arg1 == "end"   ) { pm3D_.setFlushType(CGnuPlotPm3DData::FlushType::END   ); }
        else                       { errorMsg("Invalid arg '" + arg1 + "'"); }
      }
      else if (arg == "ftriangles" || arg == "noftriangles") {
        pm3D_.setFTriangles(arg == "ftriangles");
      }
      else if (arg == "clip1in" || arg == "clip4in") {
        pm3D_.setClipIn(arg == "clip1in" ? 1 : 4);
      }
      else if (arg == "corners2color") {
        std::string arg1 = readNonSpaceNonComma(line);

        if      (arg1 == "mean"   ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::MEAN   ); }
        else if (arg1 == "geomean") { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::GEOMEAN); }
        else if (arg1 == "harmean") { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::HARMEAN); }
        else if (arg1 == "rms"    ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::RMS    ); }
        else if (arg1 == "median" ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::MEDIAN ); }
        else if (arg1 == "min"    ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::MIN    ); }
        else if (arg1 == "max"    ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::MAX    ); }
        else if (arg1 == "c1"     ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::C1     ); }
        else if (arg1 == "c2"     ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::C2     ); }
        else if (arg1 == "c3"     ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::C3     ); }
        else if (arg1 == "c4"     ) { pm3D_.setCornerType(CGnuPlotPm3DData::CornerType::C4     ); }
        else                        { errorMsg("Invalid arg '" + arg1 + "'"); }
      }
      else if (arg == "hidden3d") {
        int lt;

        if (parseInteger(line, lt))
          pm3D_.setLineType(lt);
      }
      else if (arg == "nohidden3d") {
        pm3D_.unsetLineType();
      }
      else if (arg == "implicit" || arg == "explicit") {
        pm3D_.setImplicit(arg == "implicit");
      }
      else if (arg == "map") {
        //set pm3d at b; set view map scale 1.0; set style data pm3d; set style func pm3d;
        colorBox_.setEnabled(true);

        pm3D_.addPos(CGnuPlotPm3DData::PosType::BOTTOM);

        camera_.setMap();

        setDataStyle    (PlotStyle::PM3D);
        setFunctionStyle(PlotStyle::PM3D);

        //pm3D_.setMapPos();
      }
      else if (arg == "border") {
        CGnuPlotLineProp lineProp;

        parseLineProp(line, lineProp);

        pm3D_.setBorder(lineProp);
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
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
      else if (arg == "rgb" || arg == "rgbformulae" || arg == "rgbformula") {
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
          typedef std::pair<double,CColor> ValueColor;
          typedef std::vector<ValueColor>  ValueColors;

          ValueColors valueColors;

          while (line.isValid() && ! line.isChar(')')) {
            double gray = 0.0;

            if (! parseReal(line, gray))
              break;

            if      (palette_.colorModel() == CGnuPlotTypes::ColorModel::RGB) {
              CRGB   rgb;
              double a;

              parseRGBColor(line, rgb, a);

              CColor c(rgb, a);

              valueColors.push_back(ValueColor(gray, c));
            }
            else if (palette_.colorModel() == CGnuPlotTypes::ColorModel::HSV) {
              CHSV   hsv;
              double a;

              parseHSVColor(line, hsv, a);

              CColor c(hsv, a);

              valueColors.push_back(ValueColor(gray, c));
            }
            else
              errorMsg("unhandled color model in defined palette");

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
          palette_.readFile(this, filename);

        // TODO using ...
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
        }

        if (arg1 == "cycles" || arg1 == "cycle") {
          int i;

          if (parseInteger(line, i))
            cycles = i;

          pos = line.pos();

          arg1 = readNonSpace(line);
        }

        if (arg1 == "saturation") {
          double r;

          if (parseReal(line, r))
            saturation = r;

          pos = line.pos();

          arg1 = readNonSpace(line);
        }

        palette_.setCubeHelix(start, cycles, saturation);

        line.setPos(pos);
      }
      else if (arg == "model" || arg == "mode") {
        std::string arg1 = readNonSpace(line);

        if      (arg1 == "RGB") palette_.setColorModel(CGnuPlotTypes::ColorModel::RGB);
        else if (arg1 == "HSV") palette_.setColorModel(CGnuPlotTypes::ColorModel::HSV);
        else if (arg1 == "CMY") palette_.setColorModel(CGnuPlotTypes::ColorModel::CMY);
        else if (arg1 == "YIQ") palette_.setColorModel(CGnuPlotTypes::ColorModel::YIQ);
        else if (arg1 == "XYZ") palette_.setColorModel(CGnuPlotTypes::ColorModel::XYZ);
        else                    { errorMsg("Invalid arg '" + arg1 + "'"); }
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
    if (! isPolar()) {
      setPolar(true);

      dummyVars_.resetDummyVars();
    }

    // TODO: dummy variable is t for curves
  }
  // set print ["<filename>"] [append]
  else if (var == VariableName::PRINT) {
    std::string filename;

    line.skipSpace();

    if      (line.isChar('$')) {
      line.skipChar();

      std::string block = readNonSpace(line);

      printFile_.setDataBlock(block);
    }
    else if (parseString(line, filename, "Invalid filename")) {
      if (filename == "-")
        printFile_.setStdOut();
      else
        printFile_.setFile(filename);

      std::string arg = readNonSpace(line);

      if (arg == "append")
        printFile_.setAppend(true);
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
    raxis().setDisplayed(true);
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
    else
      margin_.resetRight();
  }
  // set rrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::RRANGE) {
    parseAxisRange(line, raxis());

    double s = raxis().max().getValue(100) - raxis().min().getValue(0);

    if (raxis().logBase().isValid())
      s = raxis().mapLogValue(s);

    xaxis(1).setMin(-s);
    xaxis(1).setMax( s);

    yaxis(1).setMin(-s);
    yaxis(1).setMax( s);
  }
  // set rtics
  else if (var == VariableName::RTICS) {
    parseAxesTics(line, raxis());
  }
  // set samples <samples_1> [,<samples_2> ]
  else if (var == VariableName::SAMPLES) {
    int isamples1, isamples2;

    samples_.get(isamples1, isamples2);

    int i;

    if (parseInteger(line, i)) {
      isamples1 = i;
      isamples2 = i;
    }

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

        if (multiplot_.isValid() && multiplot_->isEnabled())
          multiplot_->setAutoFit(false);
      }
      else {
        errorMsg("Bad arg \'" + arg + "\'");
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
      defSystem_ = CoordSys::FIRST;

      CGnuPlotArrowStyle *arrowStyle1 = &styleData_.arrow;

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

          if (parseLineType(line, lt))
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
        else if (arg == "fraction" || arg == "frac") {
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
          else
            errorMsg("Invalid pointtype");
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
          else                    errorMsg("Invalid arg '" + arg + "'");
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
    //                        {{linecolor | lc } <colorspec>}
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
          int               lt;
          CGnuPlotColorSpec lc;

          int pos = line.pos();

          if (parseLineType(line, lt))
            lineStyle->setLineType(lt);
          else {
            line.setPos(pos);

            if (parseColorSpec(line, lc))
              lineStyle->setLineColor(lc);
            else
              errorMsg("invalid line type");
          }
        }
        else if (arg == "linecolor" || arg == "lc") {
          CGnuPlotColorSpec lc;

          if (parseColorSpec(line, lc))
            lineStyle->setLineColor(lc);
        }
        else if (arg == "linewidth" || arg == "lw") {
          double lw;

          if (parseReal(line, lw))
            lineStyle->setLineWidth(lw);
        }
        else if (arg == "pointtype" || arg == "pt") {
          int         pt = 1;
          std::string ptstr;

          if      (parseInteger(line, pt))
            lineStyle->setPointType(pt);
          else if (parseString(line, ptstr))
            lineStyle->setPointTypeStr(ptstr);
          else
            errorMsg("Invalid pointtype");
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
        else if (arg == "palette" || arg == "pal") {
          lineStyle->setPalette(true);
        }
        else if (arg == "default") {
          resetLineStyleInd(ind);
        }
        else {
          errorMsg("Invalid line modifier '" + arg + "'");
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
      defSystem_ = CoordSys::FIRST;

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "radius") {
          for (const auto &i : IRangeItr::range(0, 3)) {
            CGnuPlotCoordValue c;

            if (! parseCoordValue(line, c))
              break;

            circleStyle_.setRadius(i, c);

            if (! line.skipSpaceAndChar(','))
              break;
          }
        }
        else if (arg == "wedge" || arg == "nowedge") {
          circleStyle_.setWedge(arg == "wedge");
        }
        else if (arg == "clip" || arg == "noclip") {
          circleStyle_.setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
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
        else if (arg == "clip" || arg == "noclip") {
          rectStyle_.setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
        }

        arg = readNonSpace(line);
      }
    }
    // set style ellipse {units xx|xy|yy}
    //                   {size {graph|screen} <a>, {{graph|screen} <b>}}
    //                   {angle <angle>}
    //                   {clip|noclip}
    else if (var1 == StyleVar::ELLIPSE) {
      defSystem_ = CoordSys::FIRST;

      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "units") {
          arg = readNonSpace(line);

          if      (arg == "xx")
            styleData_.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::XX);
          else if (arg == "xy")
            styleData_.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::XY);
          else if (arg == "yy")
            styleData_.ellipse.setUnits(CGnuPlotTypes::EllipseUnits::YY);
          else {
            errorMsg("Invalid arg '" + arg + "'");
          }
        }
        else if (arg == "size") {
          CGnuPlotCoordValue sac;

          if (parseCoordValue(line, sac))
            styleData_.ellipse.setSize(0, sac);

          if (line.skipSpaceAndChar(',')) {
            CGnuPlotCoordValue sbc;

            if (parseCoordValue(line, sbc))
              styleData_.ellipse.setSize(1, sbc);
          }
        }
        else if (arg == "angle") {
          double a;

          if (parseReal(line, a))
            styleData_.ellipse.setAngle(a);
        }
        else if (arg == "clip" || arg == "noclip") {
          styleData_.ellipse.setClip(arg == "clip");
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style textbox {opaque|transparent} {{no}border} {margins {x,y}}
    else if (var1 == StyleVar::TEXTBOX) {
      std::string arg = readNonSpace(line);

      while (arg != "") {
        if      (arg == "opaque" || arg == "transparent") {
          styleData_.textBox.setTransparent(arg == "transparent");
        }
        else if (arg == "border" || arg == "noborder") {
          styleData_.textBox.setBorder(arg == "border");
        }
        else if (arg == "margins") {
          double mx = 0.0, my = 0.0;

          if (parseReal(line, mx)) {
            if (line.skipSpaceAndChar(',')) {
              if (parseReal(line, my))
                styleData_.textBox.setMargins(CSize2D(mx, my));
            }
          }
        }
        else {
          errorMsg("Invalid arg '" + arg + "'");
          break;
        }

        arg = readNonSpace(line);
      }
    }
    // set style histogram [ clustered [ gap {gap} ] |
    //                       errorbars [ gap {gap} ] [ linewidth|lw {width} ] |
    //                       rowstacked |
    //                       columnstacked ]
    //                     horizontal
    else if (var1 == StyleVar::HISTOGRAM) {
      HistogramStyle style;

      if (parseOptionValue(this, line, style, ""))
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
        // TODO: standard modifiers ?
        else if (arg == "title") {
          // TODO
        }
        else if (arg == "offset") {
          CPoint2D o(0, 0);

          if (parseOffset(line, o))
            histogramData_.setTitleOffset(o);
        }
        else if (arg == "font") {
          CFontPtr font;

          if (parseFont(line, font))
            histogramData_.setTitleFont(font);
        }
        else if (arg == "boxed") {
          histogramData_.setBoxed(true);
        }
        else if (arg == "horizontal") {
          histogramData_.setHorizontal(true);
        }
        else
          errorMsg("Invalid histogram style '" + arg + "'");

        arg = readNonSpace(line);
      }
    }
  }
  // set surface
  else if (var == VariableName::SURFACE) {
    surfaceData_.setEnabled(true);
  }
  // set table ["<filename>"]
  else if (var == VariableName::TABLE) {
    std::string filename;

    line.skipSpace();

    if      (! line.isValid()) {
      setTableStdErr();
    }
    else if (line.isChar('$')) {
      line.skipChar();

      std::string block = readNonSpace(line);

      setTableDataBlock(block);
    }
    else if (parseString(line, filename, "Invalid filename")) {
      setTableFile(filename);
    }
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
        errorMsg("No device " + arg);

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
          int pos1 = line.pos();

          line.setPos(pos);

          if (! device()->parseArgs(line)) {
            errorMsg("Invalid arg '" + arg + "'");
            line.setPos(pos1);
          }
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
    else if (arg == "solid") {
      // TODO: reset dashes on default line types
      device()->setDashed(false);
    }
    else if (arg == "dashed" || arg == "dash") {
      // TODO: set default dashes on default line types
      device()->setDashed(true);
    }
    else if (arg == "linewidth" || arg == "lw") {
      double lw = 1.0;

      if (parseReal(line, lw))
        device()->setLineWidth(lw);
    }
    else {
      errorMsg("Invalid termoption '" + arg + "'");
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

    // TODO: copy values
    parseAxesTics(line, xaxis(1)); line.setPos(pos);
    parseAxesTics(line, xaxis(2)); line.setPos(pos);
    parseAxesTics(line, yaxis(1)); line.setPos(pos);
    parseAxesTics(line, yaxis(2)); line.setPos(pos);
    parseAxesTics(line, zaxis(1)); line.setPos(pos);

    parseAxesTics(line, colorBox_.axis()); line.setPos(pos);
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
    else
      timeStamp_.setFormat("%a %b %d %H:%M:%S %Y");

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "top" || arg == "bottom") {
        timeStamp_.setTop(arg == "top");
      }
      else if (arg == "norotate" || arg == "rotate") {
        timeStamp_.setRotated(arg == "rotate");
      }
      else if (arg == "offset") {
        CPoint2D o;

        if (parseOffset(line, o))
          timeStamp_.setOffset(o);
      }
      else if (arg == "font") {
        CFontPtr font;

        if (parseFont(line, font))
          timeStamp_.setFont(font);
      }
      else if (arg == "textcolor" || arg == "tc") {
        CGnuPlotColorSpec c;

        if (parseColorSpec(line, c))
          timeStamp_.setTextColor(c);
      }
      else {
        errorMsg("Invalid arg '" + arg + "'");
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

    if (parseString(line, fmt, "Invalid time format")) {
      setTimeFmt(fmt);

#if 0
      for (int i = 1; i <= 2; ++i) {
        xaxis(i).setTimeFmt(fmt);
        yaxis(i).setTimeFmt(fmt);
        zaxis(i).setTimeFmt(fmt);
      }

      colorBox_.axis().setTimeFmt(fmt);
#endif
    }
  }
  // set title {"<text>"} {offset <offset>} {font "<font>{,<size>}"}
  //           {{textcolor | tc} {<colorspec> | default}} {{no}enhanced}
  else if (var == VariableName::TITLE) {
    defSystem_ = CoordSys::FIRST;

    std::string s;

    if (parseString(line, s))
      title_.setText(s);

    std::string arg = readNonSpace(line);

    while (arg != "") {
      if      (arg == "offset") {
        CGnuPlotPosition o;

        if (parsePosition(line, o))
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
    else
      margin_.resetTop();
  }
  // set trange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::TRANGE) {
    parseAxisRange(line, taxis(1));
  }
  // set urange
  else if (var == VariableName::URANGE) {
    parseAxisRange(line, uaxis());
  }
  // set view <rot_x>{,{<rot_z>}{,{<scale>}{,<scale_z>}}}
  // set view map {scale <scale>}
  // set view {no}equal {xy|xyz}
  else if (var == VariableName::VIEW) {
    int pos = line.pos();

    std::string arg = readNonSpaceNonComma(line);

    if      (arg == "map") {
      camera_.setMap();

      std::string arg = readNonSpaceNonComma(line);

      if (arg == "scale") {
        double r;

        // TODO
        if (! parseReal(line, r))
          r = 0.0;
      }
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
      camera_.setEnabled(true);

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
    parseAxisRange(line, vaxis());
  }
  // set x2data {time}
  else if (var == VariableName::X2DATA) {
    xaxis(2).setDisplayed(true);

    std::string arg = readNonSpace(line);

    xaxis(2).setIsTime(arg == "time");
  }
  // set x2dtics
  else if (var == VariableName::X2DTICS) {
    xaxis(2).setDisplayed(true);

    xaxis(2).setIsDays(true);
  }
  // set x2label "<label>"
  else if (var == VariableName::X2LABEL) {
    xaxis(2).setDisplayed(true);

    parseAxisLabel(line, xaxis(2));
  }
  // set x2mtics
  else if (var == VariableName::X2MTICS) {
    xaxis(2).setDisplayed(true);

    xaxis(2).setIsMonths(true);
  }
  // set x2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::X2RANGE) {
    xaxis(2).setDisplayed(true);

    parseAxisRange(line, xaxis(2));
  }
  // set x2tics
  else if (var == VariableName::X2TICS) {
    xaxis(2).setDisplayed(true);

    parseAxesTics(line, xaxis(2));
  }
  // set x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    xaxis(2).setDisplayed(true);

    parseAxisZeroAxis(line, xaxis(2));
  }
  // set xdata {time}
  else if (var == VariableName::XDATA) {
    xaxis(1).setDisplayed(true);

    std::string arg = readNonSpace(line);

    xaxis(1).setIsTime(arg == "time");
  }
  // set xdtics
  else if (var == VariableName::XDTICS) {
    xaxis(1).setIsDays(true);
  }
  // set xlabel "<label>"
  else if (var == VariableName::XLABEL) {
    parseAxisLabel(line, xaxis(1));
  }
  // set xmtics
  else if (var == VariableName::XMTICS) {
    xaxis(1).setIsMonths(true);
  }
  // set xrange ...
  else if (var == VariableName::XRANGE) {
    parseAxisRange(line, xaxis(1));

    sampleVars_.x.min = xaxis(1).min();
    sampleVars_.x.max = xaxis(1).max();
  }
  // set xtics ...
  else if (var == VariableName::XTICS) {
    parseAxesTics(line, xaxis(1));
  }
  // set xyplane at <zvalue>
  // set xyplane relative <frac>
  else if (var == VariableName::XYPLANE) {
    std::string arg = readNonSpace(line);

    if      (arg == "at") {
      double z;

      if (parseReal(line, z))
        camera_.setXYPlane(CGnuPlotXYPlane(z, false));
    }
    else if (arg == "relative") {
      double z;

      if (parseReal(line, z))
        camera_.setXYPlane(CGnuPlotXYPlane(z, true));
    }
  }
  // set ticslevel <frac>
  else if (var == VariableName::TICSLEVEL) {
    double z;

    if (parseReal(line, z))
      camera_.setXYPlane(CGnuPlotXYPlane(z, true));
  }
  // set xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    parseAxisZeroAxis(line, xaxis(1));
  }
  // set y2data {time}
  else if (var == VariableName::Y2DATA) {
    yaxis(2).setDisplayed(true);

    std::string arg = readNonSpace(line);

    yaxis(2).setIsTime(arg == "time");
  }
  // set y2dtics
  else if (var == VariableName::Y2DTICS) {
    yaxis(2).setDisplayed(true);

    yaxis(2).setIsDays(true);
  }
  // set y2label "<label>"
  else if (var == VariableName::Y2LABEL) {
    yaxis(2).setDisplayed(true);

    parseAxisLabel(line, yaxis(2));
  }
  // set y2mtics
  else if (var == VariableName::Y2MTICS) {
    yaxis(2).setDisplayed(true);

    yaxis(2).setIsMonths(true);
  }
  // set y2range { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::Y2RANGE) {
    yaxis(2).setDisplayed(true);

    parseAxisRange(line, yaxis(2));
  }
  // set y2tics
  else if (var == VariableName::Y2TICS) {
    yaxis(2).setDisplayed(true);

    parseAxesTics(line, yaxis(2));
  }
  // set y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    yaxis(2).setDisplayed(true);

    parseAxisZeroAxis(line, yaxis(2));
  }
  // set ydata {time}
  else if (var == VariableName::YDATA) {
    yaxis(1).setDisplayed(true);

    std::string arg = readNonSpace(line);

    yaxis(1).setIsTime(arg == "time");
  }
  // set ydtics
  else if (var == VariableName::YDTICS) {
    yaxis(1).setIsDays(true);
  }
  // set ylabel "<label>"
  else if (var == VariableName::YLABEL) {
    parseAxisLabel(line, yaxis(1));
  }
  // set ymtics
  else if (var == VariableName::YMTICS) {
    yaxis(1).setIsMonths(true);
  }
  // set yrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::YRANGE) {
    parseAxisRange(line, yaxis(1));
  }
  // set ytics
  else if (var == VariableName::YTICS) {
    parseAxesTics(line, yaxis(1));
  }
  // set yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    parseAxisZeroAxis(line, yaxis(1));
  }
  // set zdata {time}
  else if (var == VariableName::ZDATA) {
    zaxis(1).setDisplayed(true);

    std::string arg = readNonSpace(line);

    zaxis(1).setIsTime(arg == "time");
  }
  // set zdtics
  else if (var == VariableName::ZDTICS) {
    zaxis(1).setIsDays(true);
  }
  // set zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    parseAxisZeroAxis(line, zaxis(1));
  }
  // set cbdata {time}
  else if (var == VariableName::CBDATA) {
    colorBox_.axis().setDisplayed(true);

    std::string arg = readNonSpace(line);

    colorBox_.axis().setIsTime(arg == "time");
  }
  // set cbdtics
  else if (var == VariableName::CBDTICS) {
    colorBox_.axis().setIsDays(true);
  }
  // set zero {expression}
  else if (var == VariableName::ZERO) {
    double r;

    if (parseReal(line, r))
      setZero(r);
  }
  // set zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    int pos = line.pos();

    parseAxisZeroAxis(line, xaxis(1)); line.setPos(pos);
    parseAxisZeroAxis(line, xaxis(2)); line.setPos(pos);
    parseAxisZeroAxis(line, yaxis(1)); line.setPos(pos);
    parseAxisZeroAxis(line, yaxis(2)); line.setPos(pos);
    parseAxisZeroAxis(line, zaxis(1)); line.setPos(pos);
    parseAxisZeroAxis(line, zaxis(2)); line.setPos(pos);
  }
  // set zlabel "<label>"
  else if (var == VariableName::ZLABEL) {
    parseAxisLabel(line, zaxis(1));
  }
  // set zmtics
  else if (var == VariableName::ZMTICS) {
    zaxis(1).setIsMonths(true);
  }
  // set zrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::ZRANGE) {
    parseAxisRange(line, zaxis(1));
  }
  // set ztics
  else if (var == VariableName::ZTICS) {
    parseAxesTics(line, zaxis(1));
  }
  // set cblabel "<label>"
  else if (var == VariableName::CBLABEL) {
    colorBox_.setEnabled(true);

    parseAxisLabel(line, colorBox_.axis());
  }
  // set cbmtics
  else if (var == VariableName::CBMTICS) {
    colorBox_.setEnabled(true);

    colorBox_.axis().setIsMonths(true);
  }
  // set cbrange { [{{<min>}:{<max>}}] {{no}reverse} {{no}writeback} {{no}extend} } | restore
  else if (var == VariableName::CBRANGE) {
    colorBox_.setEnabled(true);

    parseAxisRange(line, colorBox_.axis());
  }
  // set cbtics
  else if (var == VariableName::CBTICS) {
    colorBox_.setEnabled(true);

    parseAxesTics(line, colorBox_.axis());
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
    StringArray words;

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
  if (isDebug()) debugMsg("get " + args);

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
  if (isDebug()) debugMsg("show " + args);

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

    if      (xaxis(1).isAutoScaleMin() && xaxis(1).isAutoScaleMax())
      ostr << "ON, ";
    else if (xaxis(1).isAutoScaleMin())
      ostr << "ON (min), ";
    else if (xaxis(1).isAutoScaleMax())
      ostr << "ON (max), ";
    else
      ostr << "OFF, ";

    if (xaxis(1).isAutoScaleFixMin()) ostr << "(fixmin), ";
    if (xaxis(1).isAutoScaleFixMax()) ostr << "(fixmax), ";

    std::cout << ostr.str();

    for (int i = ostr.str().size(); i < 24; ++i)
      std::cout << " ";

    std::cout << "y: ";

    if      (yaxis(1).isAutoScaleMin() && yaxis(1).isAutoScaleMax())
      std::cout << "ON, ";
    else if (yaxis(1).isAutoScaleMin())
      std::cout << "ON (min), ";
    else if (yaxis(1).isAutoScaleMax())
      std::cout << "ON (max), ";
    else
      std::cout << "OFF, ";

    if (yaxis(1).isAutoScaleFixMin()) std::cout << "(fixmin), ";
    if (yaxis(1).isAutoScaleFixMax()) std::cout << "(fixmax), ";

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
    axesData_.showBorder(std::cout);
  }
  // show boxwidth
  else if (var == VariableName::BOXWIDTH) {
    boxWidth_.show(std::cout);
  }
  // show clip
  else if (var == VariableName::CLIP) {
    clip().show(std::cout);
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
    contourData_.show(std::cout);
  }
  // show dashtype
  else if (var == VariableName::DASHTYPE) {
    for (const auto &dt : lineDashes_) {
      std::cout << "dashtype " << dt.first << " (" << dt.second << ")" << std::endl;
    }
  }
  // show datablock
  else if (var == CGnuPlotTypes::VariableName::DATABLOCK) {
    for (const auto &b : blocks_) {
       std::cout << '$' << b.second->name() << std::endl;
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

    dummyVars().dummyVars(v1, v2, isParametric(), isPolar(), /*is3D*/ false);

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
    std::cout << " x-axis: \""  << xaxis(1).format() << std::endl;
    std::cout << " y-axis: \""  << yaxis(1).format() << std::endl;
    std::cout << " x2-axis: \"" << xaxis(2).format() << std::endl;
    std::cout << " y2-axis: \"" << yaxis(2).format() << std::endl;
    std::cout << " z-axis: \""  << zaxis(1).format() << std::endl;
    std::cout << " cb-axis: \"" << colorBox().axis().format() << std::endl;
    std::cout << " r-axis: \""  << raxis ().format() << std::endl;
  }
  // show functions
  else if (var == VariableName::FUNCTIONS) {
    showFunctions(std::cout);
  }
  // show grid
  else if (var == VariableName::GRID) {
    bool enabled = (xaxis(1).hasGrid() || yaxis(1).hasGrid());

    if (! enabled)
      std::cout << "grid is OFF" << std::endl;
    else {
      std::cout << "Polar grid drawn at r tics" << std::endl;

      std::cout << "Major grid drawn with linestyle " <<
                   axesData_.gridMajorLineStyle() << std::endl;
      std::cout << "Minor grid drawn with linestyle " <<
                   axesData_.gridMinorLineStyle() << std::endl;
      std::cout << "Major grid drawn with linetype " <<
                   axesData_.gridMajorLineType() << std::endl;
      std::cout << "Minor grid drawn with linetype " <<
                   axesData_.gridMinorLineType() << std::endl;

      std::cout << "Grid radii drawn every " <<
                   axesData_.gridPolarAngle() << " radians" << std::endl;

      std::cout << "Grid drawn at ";

      std::cout << CStrUniqueMatch::
        valueToString<CGnuPlotTypes::DrawLayer>(axesData_.getGridLayer()) << " layer";

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
    line.setPos(pos);

    int ind;

    if (parseInteger(line, ind)) {
      CGnuPlotLineTypeP lineType = getLineTypeInd(ind);

      lineType->show(std::cout);
    }
    else {
      // TODO: just defined ?
      for (const auto &i : IRangeItr::rangeClosed(1, 8)) {
        CGnuPlotLineTypeP lineType = getLineTypeInd(i);

        std::cout << "linetype " << i << ", ";

        lineType->show(std::cout);
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
    if (xaxis(1).logBase().isValid() || xaxis(2).logBase().isValid() ||
        yaxis(1).logBase().isValid() || yaxis(2).logBase().isValid() ||
        zaxis(1).logBase().isValid() || raxis( ).logBase().isValid()) {
      std::cout << "logscaling";

      auto print = [&](const std::string &id, const CGnuPlotAxisData &axis, bool &output) {
        if (! axis.logBase().isValid()) return;
        if (output) std::cout << " and";
        std::cout << " " << id << " (base " << axis.logBase().getValue() << ")";
        output = true;
      };

      bool output = false;

      print("x" , xaxis(1), output);
      print("x2", xaxis(2), output);
      print("y" , yaxis(1), output);
      print("y2", yaxis(2), output);
      print("z" , zaxis(1), output);
      print("r" , raxis( ), output);

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
    if (multiplot_.isValid() && multiplot_->isEnabled())
      std::cout << "multiplot mode is on" << std::endl;
    else
      std::cout << "multiplot mode is off" << std::endl;
  }
  // show mx2tics
  else if (var == VariableName::MX2TICS) {
    xaxis(2).showMinorTics(std::cout, "x2tics", "xtic");
  }
  // show mxtics
  else if (var == VariableName::MXTICS) {
    xaxis(1).showMinorTics(std::cout, "xtics", "xtic");
  }
  // show my2tics
  else if (var == VariableName::MY2TICS) {
    yaxis(2).showMinorTics(std::cout, "y2tics", "ytic");
  }
  // show mytics
  else if (var == VariableName::MYTICS) {
    yaxis(1).showMinorTics(std::cout, "ytics", "ytic");
  }
  // show mztics
  else if (var == VariableName::MZTICS) {
    zaxis(1).showMinorTics(std::cout, "ztics", "ztic");
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
    std::cout << "parametric is " << (isParametric() ? "ON" : "OFF") << std::endl;
  }
  // show paxis
  else if (var == VariableName::PAXIS) {
    line.setPos(pos);

    int i;

    if (parseInteger(line, i))
      paxis(i).show(std::cout, "p", 1);
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
    if      (arg == "palette" || arg == "pal") {
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
    else if (arg == "rgb" || arg == "rgbformulae") {
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
    std::cout << "polar is " << (isPolar() ? "ON" : "OFF") << std::endl;
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
    std::cout << "raxis is " << (raxis().isDisplayed() ? "drawn" : "not drawn") << std::endl;
  }
  // show rmargin
  else if (var == VariableName::RMARGIN) {
    margin_.showRight(std::cout);
  }
  // show rrange
  else if (var == VariableName::RRANGE) {
    raxis().showRange(std::cout, "rrange");
  }
  // show rtics
  else if (var == VariableName::RTICS) {
    raxis().showTics(std::cout, "r-axis");
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
        CStrUniqueMatch::valueToString<PlotStyle>(dataStyle()) << std::endl;
    }
    // show style fill
    else if (show.empty() || show.find("fill") != show.end()) {
      fillStyle_.show(std::cout);
    }
    // show style function
    else if (show.empty() || show.find("function") != show.end()) {
      std::cout << "Functions are plotted with " <<
        CStrUniqueMatch::valueToString<PlotStyle>(functionStyle()) << std::endl;
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
      for (const auto &lt : lineStyles()) {
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
      styleData_.ellipse.show(std::cout);
    }
    // show style textbox
    else if (show.empty() || show.find("textbox") != show.end()) {
      styleData_.textBox.show(std::cout);
    }
  }
  // show surface
  else if (var == VariableName::SURFACE) {
    std::cout << "surface is " << (surfaceData_.isEnabled() ? "drawn" : "not drawn") << std::endl;
  }
  // show table
  else if (var == VariableName::TABLE) {
    // needed ?
    if      (tableFile_.isFile())
      std::cout << "table file \"" << tableFile_.getFile() << "\"";
    else if (tableFile_.isDataBlock())
      std::cout << "table file $" << tableFile_.getDataBlock();
    else if (tableFile_.isStdErr())
      std::cout << "table file STDERR";
    else if (tableFile_.isStdOut())
      std::cout << "table file STDOUT";

    if (tableFile_.isAppend())
      std::cout << " (append)";

    std::cout << std::endl;
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
    for (int i = 1; i <= 2; ++i) {
      xaxis(i).showTics(std::cout, (i == 1 ? "x-tics" : "x2-tics"));
      yaxis(i).showTics(std::cout, (i == 1 ? "y-tics" : "x2-tics"));
    }

    zaxis(1).showTics(std::cout, "z-tics");

    colorBox().axis().showMinorTics(std::cout, "cbtics", "cbtic");
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
    taxis(1).showRange(std::cout, "trange");
  }
  // show urange
  else if (var == VariableName::URANGE) {
    uaxis().showRange(std::cout, "urange");
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
    vaxis().showRange(std::cout, "vrange");
  }
  // show x2data
  else if (var == VariableName::X2DATA) {
    std::cout << "x2 is set to " << (xaxis(2).isTime() ? "time" : "numerical") << std::endl;
  }
  // show x2dtics
  else if (var == VariableName::X2DTICS) {
    std::cout << "x2 is " << (xaxis(2).isDays() ? "days" : "not days") << std::endl;
  }
  // show x2label
  else if (var == VariableName::X2LABEL) {
    xaxis(2).printLabel(std::cout, "x2");
  }
  // show x2mtics
  else if (var == VariableName::X2MTICS) {
    std::cout << "x2 is " << (xaxis(2).isMonths() ? "months" : "not months") << std::endl;
  }
  // show x2range
  else if (var == VariableName::X2RANGE) {
    xaxis(2).showRange(std::cout, "x2range");
  }
  // show x2tics
  else if (var == VariableName::X2TICS) {
    xaxis(2).showTics(std::cout, "x2-tics");
  }
  // show x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    xaxis(2).showZeroAxis(std::cout, "x2");
  }
  // show xdata
  else if (var == VariableName::XDATA) {
    std::cout << "x is set to " <<
      (xaxis(1).isTime() ? "time" : "numerical") << std::endl;
  }
  // show xdtics
  else if (var == VariableName::XDTICS) {
    std::cout << "x is " << (xaxis(1).isDays() ? "days" : "not days") << std::endl;
  }
  // show xlabel
  else if (var == VariableName::XLABEL) {
    xaxis(1).printLabel(std::cout, "x");
  }
  // show xmtics
  else if (var == VariableName::XMTICS) {
    std::cout << "x is " << (xaxis(1).isMonths() ? "months" : "not months") << std::endl;
  }
  // show xrange
  else if (var == VariableName::XRANGE) {
    xaxis(1).showRange(std::cout, "xrange");
  }
  // show xtics
  else if (var == VariableName::XTICS) {
    xaxis(1).showTics(std::cout, "x-tics");
  }
  // show xyplane
  else if (var == VariableName::XYPLANE) {
    std::cout << "xyplane at " << camera_.xyPlane().z() <<
      (camera_.xyPlane().isRelative() ? " relative" : "") << std::endl;
  }
  // show xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    xaxis(1).showZeroAxis(std::cout, "x");
  }
  // show y2data
  else if (var == VariableName::Y2DATA) {
    std::cout << "y2 is set to " <<
      (yaxis(2).isTime() ? "time" : "numerical") << std::endl;
  }
  // show y2dtics
  else if (var == VariableName::Y2DTICS) {
    std::cout << "y2 is " << (yaxis(2).isDays() ? "days" : "not days") << std::endl;
  }
  // show y2label
  else if (var == VariableName::Y2LABEL) {
    yaxis(2).printLabel(std::cout, "y2");
  }
  // show y2mtics
  else if (var == VariableName::Y2MTICS) {
    std::cout << "y2 is " << (yaxis(2).isMonths() ? "months" : "not months") << std::endl;
  }
  // show y2range
  else if (var == VariableName::Y2RANGE) {
    yaxis(2).showRange(std::cout, "y2range");
  }
  // show y2tics
  else if (var == VariableName::Y2TICS) {
    yaxis(2).showTics(std::cout, "y2-tics");
  }
  // show y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    yaxis(2).showZeroAxis(std::cout, "y2");
  }
  // show ydata
  else if (var == VariableName::YDATA) {
    std::cout << "y is set to " <<
      (yaxis(1).isTime() ? "time" : "numerical") << std::endl;
  }
  // show ydtics
  else if (var == VariableName::YDTICS) {
    std::cout << "y is " << (yaxis(1).isDays() ? "days" : "not days") << std::endl;
  }
  // show ylabel
  else if (var == VariableName::YLABEL) {
    yaxis(1).printLabel(std::cout, "y");
  }
  // show ymtics
  else if (var == VariableName::YMTICS) {
    std::cout << "y is " << (yaxis(1).isMonths() ? "months" : "not months") << std::endl;
  }
  // show yrange
  else if (var == VariableName::YRANGE) {
    yaxis(1).showRange(std::cout, "yrange");
  }
  // show ytics
  else if (var == VariableName::YTICS) {
    yaxis(1).showTics(std::cout, "y-tics");
  }
  // show yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    yaxis(1).showZeroAxis(std::cout, "y");
  }
  // show zdata
  else if (var == VariableName::ZDATA) {
    std::cout << "z is set to " <<
      (zaxis(1).isTime() ? "time" : "numerical") << std::endl;
  }
  // show zdtics
  else if (var == VariableName::ZDTICS) {
    std::cout << "z is " << (zaxis(1).isDays() ? "days" : "not days") << std::endl;
  }
  // show zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    zaxis(1).showZeroAxis(std::cout, "z");
  }
  // show cbdata
  else if (var == VariableName::CBDATA) {
    std::cout << "cb is set to " <<
      (colorBox().axis().isTime() ? "time" : "numerical") << std::endl;
  }
  // show cbdtics
  else if (var == VariableName::CBDTICS) {
    std::cout << "cb is " <<
      (colorBox().axis().isDays() ? "days" : "not days") << std::endl;
  }
  // show zero
  else if (var == VariableName::ZERO) {
    std::cout << "zero is " << zero() << std::endl;
  }
  // show zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    xaxis(1).showZeroAxis(std::cout, "x");
    xaxis(2).showZeroAxis(std::cout, "x2");
    yaxis(1).showZeroAxis(std::cout, "y");
    yaxis(2).showZeroAxis(std::cout, "y2");
    zaxis(1).showZeroAxis(std::cout, "z");
    zaxis(2).showZeroAxis(std::cout, "z2");
  }
  // show zlabel
  else if (var == VariableName::ZLABEL) {
    zaxis(1).printLabel(std::cout, "z");
  }
  // show zmtics
  else if (var == VariableName::ZMTICS) {
    std::cout << "z is " << (zaxis(1).isMonths() ? "months" : "not months") << std::endl;
  }
  // show zrange
  else if (var == VariableName::ZRANGE) {
    zaxis(1).showRange(std::cout, "zrange");
  }
  // show ztics
  else if (var == VariableName::ZTICS) {
    zaxis(1).showTics(std::cout, "z-tics");
  }
  // show cblabel
  else if (var == VariableName::CBLABEL) {
    colorBox().axis().printLabel(std::cout, "cb");
  }
  // show cbmtics
  else if (var == VariableName::CBMTICS) {
    std::cout << "cb is " <<
      (colorBox().axis().isMonths() ? "months" : "not months") << std::endl;
  }
  // show cbrange
  else if (var == VariableName::CBRANGE) {
    colorBox().axis().showRange(std::cout, "cbrange");
  }
  // show cbtics
  else if (var == VariableName::CBTICS) {
    colorBox().axis().showMinorTics(std::cout, "cbtics", "cbtic");
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

  for (const auto &i : IRangeItr::range(0, n)) {
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

      CExprValuePtr value = var->value();

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

  for (const auto &n : names) {
    CExprFunctionPtr function = CExprInst->getFunction(n);

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
  if (isDebug()) debugMsg("reset " + args);

  // TODO: do not reset terminal and output
  resetAngleType();

  setParametric(false);
  setPolar     (false);

  dummyVars_.resetDummyVars();

  clearAnnotations<CGnuPlotGroupAnnotation>();

  fillStyle_.unset();

  margin_.reset();

  resetPlotData();

  keyData_.reset();

  palette_.unset();

  clearLineStyles();

  camera_.reset();

  surfaceData_.setEnabled(true);
  contourData_.setEnabled(false);

  multiplot_.reset();

  region_ = CBBox2D(0, 0, 1, 1);

  plotSize_.reset();
}

void
CGnuPlot::
resetPlotData()
{
  xind_ = 1;
  yind_ = 1;
  zind_ = 1;

  for (const auto &i : IRangeItr::rangeClosed(1, 2)) {
    xaxis(i).reset();
    yaxis(i).reset();
    zaxis(i).reset();
  }

  for (const auto &i : IRangeItr::range(0, 8))
    paxis(i).reset();

  taxis(1).reset();

  colorBox_.axis().reset();

  raxis().reset();
  uaxis().reset();
  vaxis().reset();

  axesData_.resetBorderSides();
}

// undefine ...
void
CGnuPlot::
undefineCmd(const std::string &args)
{
  if (isDebug()) debugMsg("undefine " + args);

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
  if (isDebug()) debugMsg("unset " + args);

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
      for (const auto &i : IRangeItr::rangeClosed(1, 2)) {
        xaxis(i).setAutoScale(false);
        yaxis(i).setAutoScale(false);
      }
    }
    else if (arg == "x")
      xaxis(1).setAutoScale(false);
    else if (arg == "y")
      yaxis(1).setAutoScale(false);
    else if (arg == "x2")
      xaxis(2).setAutoScale(false);
    else if (arg == "y2")
      yaxis(2).setAutoScale(false);
    else if (arg == "z")
      zaxis(1).setAutoScale(false);

    else if (arg == "xmin")
      xaxis(1).setAutoScaleMin(false);
    else if (arg == "xmax")
      xaxis(1).setAutoScaleMax(false);
    else if (arg == "ymin")
      yaxis(1).setAutoScaleMin(false);
    else if (arg == "ymax")
      yaxis(1).setAutoScaleMax(false);

    else if (arg == "xfixmin")
      xaxis(1).setAutoScaleFixMin(false);
    else if (arg == "xfixmax")
      xaxis(1).setAutoScaleFixMax(false);
    else if (arg == "yfixmin")
      yaxis(1).setAutoScaleFixMin(true);
    else if (arg == "yfixmax")
      yaxis(1).setAutoScaleFixMax(true);

    else if (arg == "xy") {
      xaxis(1).setAutoScale(false);
      yaxis(1).setAutoScale(false);
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
    axesData_.unsetBorder();
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
        else                      errorMsg("Invalid arg '" + arg + "'");

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
    contourData_.setEnabled(false);
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
    dummyVars_.resetDummyVars();
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
    for (int i = 1; i <= 2; ++i) {
      xaxis(i).unsetFormat();
      yaxis(i).unsetFormat();
    }

    zaxis(1).unsetFormat();

    colorBox_.axis().unsetFormat();

    raxis().unsetFormat();
  }
  // unset functions (invalid)
  else if (var == VariableName::FUNCTIONS) {
    errorMsg("Invalid command");
  }
  // unset grid
  else if (var == VariableName::GRID) {
    xaxis(1).setGridMajor(false);
    xaxis(1).setGridMinor(false);
    yaxis(1).setGridMajor(false);
    yaxis(1).setGridMinor(false);
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
    line.setPos(pos);

    int ind;

    if (parseInteger(line, ind)) {
      CGnuPlotLineTypeP lineType = getLineTypeInd(ind);

      lineType->unset();
    }
    else {
      for (const auto &i : IRangeItr::rangeClosed(1, 8)) {
        CGnuPlotLineTypeP lineType = getLineTypeInd(i);

        lineType->unset();
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
  // unset logscale <axes>
  else if (var == VariableName::LOGSCALE) {
    line.setPos(pos);

    line.skipSpace();

    std::string axesStr = line.substr();

    if (axesStr != "") {
      AxisTypeIdSet axisTypeIdSet;

      if (! stringToAxes(axesStr, axisTypeIdSet))
        errorMsg("Invalid logscale axes '" + axesStr + "'");

      for (int i = 1; i <= 2; ++i) {
        if (axisTypeIdSet.count(AxisTypeId(AxisType::X, i))) xaxis(i).resetLogBase();
        if (axisTypeIdSet.count(AxisTypeId(AxisType::Y, i))) yaxis(i).resetLogBase();
      }

      if (axisTypeIdSet.count(AxisTypeId(AxisType::Z, 1))) zaxis(1).resetLogBase();
      if (axisTypeIdSet.count(AxisTypeId(AxisType::R, 1))) raxis( ).resetLogBase();

      if (axisTypeIdSet.count(AxisTypeId(AxisType::CB, 1))) colorBox_.axis().resetLogBase();
    }
    else {
      for (int i = 1; i <= 2; ++i) {
        xaxis(i).resetLogBase();
        yaxis(i).resetLogBase();
      }

      zaxis(1).resetLogBase();
      raxis( ).resetLogBase();

      colorBox_.axis().resetLogBase();
    }
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
    if (device_->multiWindow() && multiplot_->isAutoFit())
      device_->multiWindow()->fitGroups();

    multiplot_->setEnabled(false);

    device_->resetMultiWindow();
  }
  // unset mx2tics
  else if (var == VariableName::MX2TICS) {
    xaxis(2).setMinorTicsDisplayed(false);
    xaxis(2).resetMinorTicsFreq();
  }
  // unset mxtics
  else if (var == VariableName::MXTICS) {
    xaxis(1).setMinorTicsDisplayed(false);
    xaxis(1).resetMinorTicsFreq();
  }
  // unset my2tics
  else if (var == VariableName::MY2TICS) {
    yaxis(2).setMinorTicsDisplayed(false);
    yaxis(2).resetMinorTicsFreq();
  }
  // unset mytics
  else if (var == VariableName::MYTICS) {
    yaxis(1).setMinorTicsDisplayed(false);
    yaxis(1).resetMinorTicsFreq();
  }
  // unset mztics
  else if (var == VariableName::MZTICS) {
    zaxis(1).setMinorTicsDisplayed(false);
    zaxis(1).resetMinorTicsFreq();
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
    if (isParametric()) {
      setParametric(false);

      dummyVars_.resetDummyVars();
    }

    // TODO: dummy variable is x for curves, x/y for surfaces
  }
  // unset paxis
  else if (var == VariableName::PAXIS) {
    line.setPos(pos);

    int i;

    if (parseInteger(line, i))
      paxis(i).unset();
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
    if (isPolar()) {
      setPolar(false);

      dummyVars_.resetDummyVars();
    }

    // TODO: dummy variable is x for curves
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
    raxis().setDisplayed(false);
  }
  // unset rmargin
  else if (var == VariableName::RMARGIN) {
    margin_.resetRight();
  }
  // unset rrange
  else if (var == VariableName::RRANGE) {
    raxis().unsetRange();
  }
  // unset rtics
  else if (var == VariableName::RTICS) {
    raxis().setShowTics(false);
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
      clearLineStyles();
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
      styleData_.ellipse.unset();
    }
    // unset style textbox
    else if (unset.empty() || unset.find("textbox") != unset.end()) {
      styleData_.textBox.unset();
    }
  }
  // unset surface
  else if (var == VariableName::SURFACE) {
    surfaceData_.setEnabled(false);
  }
  // unset table
  else if (var == VariableName::TABLE) {
    unsetTableFile();
  }
  // unset terminal
  else if (var == VariableName::TERMINAL) {
    setDevice("qt");
  }
  // unset tics
  else if (var == VariableName::TICS) {
    xaxis(1).setShowTics(false);
    xaxis(2).setShowTics(false);
    yaxis(1).setShowTics(false);
    yaxis(2).setShowTics(false);
    zaxis(1).setShowTics(false);

    colorBox_.axis().setShowTics(false);
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
    taxis(1).unset();
  }
  // unset urange
  else if (var == VariableName::URANGE) {
    uaxis().unset();
  }
  // unset view
  else if (var == VariableName::VIEW) {
    camera_.unsetView();
  }
  // unset vrange
  else if (var == VariableName::VRANGE) {
    vaxis().unset();
  }
  // unset x2data
  else if (var == VariableName::X2DATA) {
    xaxis(2).setIsTime(false);
  }
  // unset x2dtics
  else if (var == VariableName::X2DTICS) {
    xaxis(2).setIsDays(false);
  }
  // unset x2label
  else if (var == VariableName::X2LABEL) {
    xaxis(2).setText("");
  }
  // unset x2mtics
  else if (var == VariableName::X2MTICS) {
    xaxis(2).setIsMonths(false);
  }
  // unset x2range
  else if (var == VariableName::X2RANGE) {
    xaxis(2).unsetRange();
  }
  // unset x2tics
  else if (var == VariableName::X2TICS) {
    xaxis(2).setShowTics(false);
  }
  // unset x2zeroaxis
  else if (var == VariableName::X2ZEROAXIS) {
    xaxis(2).unsetZeroAxis();
  }
  // unset xdata
  else if (var == VariableName::XDATA) {
    xaxis(1).setIsTime(false);
  }
  // unset xdtics
  else if (var == VariableName::XDTICS) {
    xaxis(1).setIsDays(false);
  }
  // unset xlabel
  else if (var == VariableName::XLABEL) {
    xaxis(1).setText("");
  }
  // unset xmtics
  else if (var == VariableName::XMTICS) {
    xaxis(1).setIsMonths(false);
  }
  // unset xrange
  else if (var == VariableName::XRANGE) {
    xaxis(1).unsetRange();
  }
  // unset xtics
  else if (var == VariableName::XTICS) {
    xaxis(1).setShowTics(false);
  }
  // unset xyplane
  else if (var == VariableName::XYPLANE) {
    camera_.setXYPlane(CGnuPlotXYPlane(0, false));
  }
  // unset xzeroaxis
  else if (var == VariableName::XZEROAXIS) {
    xaxis(1).unsetZeroAxis();
  }
  // unset y2data
  else if (var == VariableName::Y2DATA) {
    yaxis(2).setIsTime(false);
  }
  // unset y2dtics
  else if (var == VariableName::Y2DTICS) {
    yaxis(2).setIsDays(false);
  }
  // unset y2label
  else if (var == VariableName::Y2LABEL) {
    yaxis(2).setText("");
  }
  // unset y2mtics
  else if (var == VariableName::Y2MTICS) {
    yaxis(2).setIsMonths(false);
  }
  // unset y2range
  else if (var == VariableName::Y2RANGE) {
    yaxis(2).unsetRange();
  }
  // unset y2tics
  else if (var == VariableName::Y2TICS) {
    yaxis(2).setShowTics(false);
  }
  // unset y2zeroaxis
  else if (var == VariableName::Y2ZEROAXIS) {
    yaxis(2).unsetZeroAxis();
  }
  // unset ydata
  else if (var == VariableName::YDATA) {
    yaxis(1).setIsTime(false);
  }
  // unset ydtics
  else if (var == VariableName::YDTICS) {
    yaxis(1).setIsDays(false);
  }
  // unset ylabel
  else if (var == VariableName::YLABEL) {
    yaxis(1).setText("");
  }
  // unset ymtics
  else if (var == VariableName::YMTICS) {
    yaxis(1).setIsMonths(false);
  }
  // unset yrange
  else if (var == VariableName::YRANGE) {
    yaxis(1).unsetRange();
  }
  // unset ytics
  else if (var == VariableName::YTICS) {
    yaxis(1).setShowTics(false);
  }
  // unset yzeroaxis
  else if (var == VariableName::YZEROAXIS) {
    yaxis(1).unsetZeroAxis();
  }
  // unset zdata
  else if (var == VariableName::ZDATA) {
    zaxis(1).setIsTime(false);
  }
  // unset zdtics
  else if (var == VariableName::ZDTICS) {
    zaxis(1).setIsDays(false);
  }
  // unset zzeroaxis
  else if (var == VariableName::ZZEROAXIS) {
    zaxis(1).unsetZeroAxis();
  }
  // unset cbdata
  else if (var == VariableName::CBDATA) {
    colorBox_.axis().setIsTime(false);
  }
  // unset cbdtics
  else if (var == VariableName::CBDTICS) {
    colorBox_.axis().setIsDays(false);
  }
  // unset zero
  else if (var == VariableName::ZERO) {
    setZero(1E-8);
  }
  // unset zeroaxis
  else if (var == VariableName::ZEROAXIS) {
    xaxis(1).unsetZeroAxis();
    xaxis(2).unsetZeroAxis();
    yaxis(1).unsetZeroAxis();
    yaxis(2).unsetZeroAxis();
    zaxis(1).unsetZeroAxis();
    zaxis(2).unsetZeroAxis();
  }
  // unset zlabel
  else if (var == VariableName::ZLABEL) {
    zaxis(1).setText("");
  }
  // unset zmtics
  else if (var == VariableName::ZMTICS) {
    zaxis(1).setIsMonths(false);
  }
  // unset zrange
  else if (var == VariableName::ZRANGE) {
    zaxis(1).unsetRange();
  }
  // unset ztics
  else if (var == VariableName::ZTICS) {
    zaxis(1).setShowTics(false);
  }
  // unset cblabel
  else if (var == VariableName::CBLABEL) {
    colorBox_.axis().setText("");
  }
  // unset cbmtics
  else if (var == VariableName::CBMTICS) {
    colorBox_.axis().setIsMonths(false);
  }
  // unset cbrange
  else if (var == VariableName::CBRANGE) {
    colorBox_.axis().unsetRange();
  }
  // unset cbtics
  else if (var == VariableName::CBTICS) {
    colorBox_.axis().setShowTics(false);
  }
  // unset ellipse
  else if (var == VariableName::ELLIPSE) {
    clearAnnotations<CGnuPlotEllipse>();
  }
  // unset circle
  else if (var == VariableName::CIRCLE) {
    clearAnnotations<CGnuPlotCircle>();
  }
  // unset polygon
  else if (var == VariableName::POLYGON) {
    clearAnnotations<CGnuPlotPolygon>();
  }
  // unset rectangle
  else if (var == VariableName::RECTANGLE) {
    clearAnnotations<CGnuPlotRectangle>();
  }
  // unset debug
  else if (var == VariableName::DEBUG) {
    setDebug(false);
  }
  // unset edebug
  else if (var == VariableName::EDEBUG) {
    CExprInst->setDebug(false);
  }

  return false;
}

void
CGnuPlot::
unsetForCmd(const ForCmd &forCmd, const std::string &args)
{
  if (forCmd.isIn) {
    StringArray words;

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
  if (isDebug()) debugMsg("shell " + args);

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
  if (isDebug()) debugMsg("system " + args);

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
  if (isDebug()) debugMsg("stats " + args);

  // TODO
}

// clear
void
CGnuPlot::
clearCmd(const std::string &args)
{
  if (isDebug()) debugMsg("clear " + args);

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
  if (isDebug()) debugMsg("lower " + args);
}

// raise {winid:i}
void
CGnuPlot::
raiseCmd(const std::string &args)
{
  if (isDebug()) debugMsg("raise " + args);
}

// test
// test palette
void
CGnuPlot::
testCmd(const std::string &args)
{
  if (isDebug()) debugMsg("test " + args);

  CParseLine line(args);

  std::string arg = readNonSpace(line);

  CGnuPlotWindowP window = CGnuPlotWindowP(createWindow());

  window->setBackgroundColor(backgroundColor());

  addWindow(window);

  CGnuPlotGroupP group(createGroup(window.get()));

  group->init();
  group->set3D(false);

  PlotStyle plotStyle = PlotStyle::TEST_TERMINAL;

  if (arg == "palette" || arg == "pal")
    plotStyle = PlotStyle::TEST_PALETTE;

  CGnuPlotPlotP plot(createPlot(group.get(), plotStyle));

  group->addSubPlots({plot});

  group->fit();

  if (plotStyle == PlotStyle::TEST_PALETTE) {
    group->setBorderSides(0);

    group->setMarginBottom(10);

    CGnuPlotColorBoxP cb = group->colorBox();

    cb->setEnabled (true);
    cb->setVertical(false);
    cb->setSize(CSize2D(1, 0.05));

    CGnuPlotAxis *xaxis = group->getPlotAxis(CGnuPlotTypes::AxisType::X, 1);
    CGnuPlotAxis *yaxis = group->getPlotAxis(CGnuPlotTypes::AxisType::Y, 1);

    if (xaxis) xaxis->setGridMajor(true);
    if (yaxis) yaxis->setGridMajor(true);

    CGnuPlotKeyP key = group->key();

    key->setTitle("R,G,B profiles of the current color palette");

    //group->setMargin(CGnuPlotMargin(2, 2, 2, 2));
  }
  else {
    //group->setMargin(CGnuPlotMargin(0, 0, 0, 0));
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
  if (isDebug()) debugMsg("fit " + args);

  // TODO
}

// update "<filename>"
void
CGnuPlot::
updateCmd(const std::string &args)
{
  if (isDebug()) debugMsg("update " + args);

  // TODO
}

// bind
void
CGnuPlot::
bindCmd(const std::string &args)
{
  if (isDebug()) debugMsg("bind " + args);
}

// if ({condition}) {command}
//    [; else if ( {condition} ) {command} ]
//    [; else {command} ]
void
CGnuPlot::
ifCmd(int &i, const Statements &statements)
{
  if (isDebug()) debugMsg("if");

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
    return styleData_.arrow;

  return (*p).second;
}

CLineDash
CGnuPlot::
getLineDash(int dt) const
{
  auto p = lineDashes_.find(dt);

  if (p != lineDashes_.end())
    return (*p).second;

  if (dt < 1)
    return CLineDash();

  int dt1 = ((dt - 1) % 35) + 1;

  switch (dt1) {
    case 1 : return CLineDash("1, 1");
    case 2 : return CLineDash("1, 2");
    case 3 : return CLineDash("1, 3");
    case 4 : return CLineDash("1, 4");
    case 5 : return CLineDash("1, 6");
    case 6 : return CLineDash("1, 8");
    case 7 : return CLineDash("1, 12");
    case 8 : return CLineDash("1, 24");
    case 9 : return CLineDash("1, 48");
    case 10: return CLineDash("2, 1");
    case 11: return CLineDash("3, 1");
    case 12: return CLineDash("4, 1");
    case 13: return CLineDash("6, 1");
    case 14: return CLineDash("8, 1");
    case 15: return CLineDash("12, 1");
    case 16: return CLineDash("24, 1");
    case 17: return CLineDash("2, 2");
    case 18: return CLineDash("3, 3");
    case 19: return CLineDash("4, 4");
    case 20: return CLineDash("6, 6");
    case 21: return CLineDash("8, 8");
    case 22: return CLineDash("12, 12");
    case 23: return CLineDash("24, 24");
    case 24: return CLineDash("2, 4");
    case 25: return CLineDash("4, 2");
    case 26: return CLineDash("2, 6");
    case 27: return CLineDash("6, 2");
    case 28: return CLineDash("4, 8");
    case 29: return CLineDash("8, 4");
    case 30: return CLineDash("2,1,0.5,1");
    case 31: return CLineDash("4,2,1,2");
    case 32: return CLineDash("8,2,1,2");
    case 33: return CLineDash("0.5,0.5");
    case 34: return CLineDash("0.25,0.25");
    case 35: return CLineDash("0.1,0.1");
  }

  return CLineDash();
}

void
CGnuPlot::
clearTicLabels()
{
  xaxis(1).clearTicLabels();
  xaxis(2).clearTicLabels();
  yaxis(1).clearTicLabels();
  yaxis(2).clearTicLabels();
  zaxis(1).clearTicLabels();
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

bool
CGnuPlot::
readNamedBlock(const std::string &name, const std::string &eofStr)
{
  CGnuPlotBlock *block = getBlock(name);

  if (fileData_.file) {
    std::string line;

    while (fileReadLine(line)) {
      if (isEOFStr(line, eofStr))
        return true;

      block->addString(line + "\n");
    }

    return false;
  }
  else {
    bool endFlag = false;

    initReadLine();

    readLine_->setPrompt("");

    for (;;) {
      auto line = readLine_->readLine();

      if (isEOFStr(line, eofStr)) {
        endFlag = true;
        break;
      }

      block->addString(line + "\n");
    }

    readLine_->setPrompt("> ");

    return endFlag;
  }
}

std::string
CGnuPlot::
parseUsingStr(CParseLine &line)
{
  std::string usingStr;

  int rbrackets = 0;

  while (line.isValid()) {
    if (rbrackets == 0) {
      int pos = line.pos();

      while (line.isSpace())
        line.skipChar();

      if (line.isChar(':')) {
        line.skipChar();

        while (line.isSpace())
          line.skipChar();

        usingStr += ':';
      }
      else
        line.setPos(pos);

      //---

      if (line.isSpace() || line.isChar(','))
        break;

      if (line.isChar('('))
        ++rbrackets;

      usingStr += line.getChar();
    }
    else {
      if      (line.isChar('('))
        ++rbrackets;
      else if (line.isChar(')'))
        --rbrackets;

      usingStr += line.getChar();
    }
  }

  return usingStr;
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
  parseInd(1, indexEnd  , indexStart /*std::numeric_limits<int>::max()*/);
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
      CExprValuePtr value;

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
  if (isDebug()) debugMsg("do " + args);

  CParseLine line(args);

  line.skipSpace();

  // check for 'for'
  if (! line.isString("for")) {
    errorMsg("Missing 'for' for do command");
    return;
  }

  (void) readNonSpace(line);

  // split for args into range or values
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

  //---

  Statements statements;

  getStatements(line, statements);

  for (const auto &statement : statements)
    fileData_.bufferLines.push_back(statement);

  //---

  // get block lines
  Statements  lines;
  std::string eline;

  readBlockLines(lines, eline, 1);

  //---

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

      for (const auto &l : lines)
        parseLine(l);

      i1 += i3;
    }
  }
  else {
    for (const auto &f : fields) {
      CExprInst->createStringVariable(var, f);

      for (const auto &l : lines)
        parseLine(l);
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
  if (isDebug()) debugMsg("while " + args);

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
  if (isDebug()) debugMsg("evaluate " + args);

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
  if (isDebug()) debugMsg("pause " + args);

  CParseLine line(args);

  line.skipSpace();

  if (line.isString("mouse")) {
    int mask = 0;

    while (true) {
      if (line.isString("key") || line.isString("keypress")) {
        if (line.isString("key")) line.skipChars(3);
        else                      line.skipChars(8);

        mask |= int(CGnuPlotTypes::PauseMask::KEY);
      }
      else if (line.isString("button1")) {
        line.skipChars(7);

        mask |= int(CGnuPlotTypes::PauseMask::BUTTON1);
      }
      else if (line.isString("button2")) {
        line.skipChars(7);

        mask |= int(CGnuPlotTypes::PauseMask::BUTTON2);
      }
      else if (line.isString("button3")) {
        line.skipChars(7);

        mask |= int(CGnuPlotTypes::PauseMask::BUTTON3);
      }
      else if (line.isString("close")) {
        line.skipChars(5);

        mask |= int(CGnuPlotTypes::PauseMask::CLOSE);
      }
      else if (line.isString("any")) {
        line.skipChars(3);

        mask = 0xFF;
      }
      else
        break;

      if (! line.skipSpaceAndChar(','))
        break;
    }

    if (mask == 0)
      mask = 0xFF;

    std::string msg;

    if (! parseString(line, msg))
      msg = "";

    if (! isAutoContinue())
      waitForMouse(mask, msg);
  }
  else {
    int secs;

    if (! parseInteger(line, secs))
      secs = -1;

    std::string msg;

    if (! parseString(line, msg))
      msg = "press return to continue";

    if (! isAutoContinue())
      prompt(msg);
  }
}

// reread
void
CGnuPlot::
rereadCmd(const std::string &args)
{
  if (isDebug()) debugMsg("reread " + args);

  fileData_.file->rewind();

  fileData_.lineNum = 0;

  fileData_.bufferLines.clear();
}

bool
CGnuPlot::
stringToAxes(const std::string &axesStr, AxisTypeIdSet &axisTypeIdSet) const
{
  CParseLine line(axesStr);

  while (line.isValid()) {
    if      (line.isChars("x2")) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::X , 2)); line.skipChars(2);
    }
    else if (line.isChars("y2")) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::Y , 2)); line.skipChars(2);
    }
    else if (line.isChars("x" )) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::X , 1)); line.skipChars(1);
    }
    else if (line.isChars("y" )) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::Y , 1)); line.skipChars(1);
    }
    else if (line.isChars("z" )) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::Z , 1)); line.skipChars(1);
    }
    else if (line.isChars("r" )) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::R , 1)); line.skipChars(1);
    }
    else if (line.isChars("cb")) {
      axisTypeIdSet.insert(AxisTypeId(AxisType::CB, 1)); line.skipChars(2);
    }
    else
      return false;
  }

  return true;
}

CGnuPlotWindowP
CGnuPlot::
createNewWindow()
{
  CGnuPlotWindowP window(createWindow());

  addWindow(window);

  return window;
}

CGnuPlotWindow *
CGnuPlot::
createWindow()
{
  return (device() ? device()->createWindow() : new CGnuPlotWindow(this));
}

CGnuPlotMultiplot *
CGnuPlot::
createMultiplot()
{
  return (device() ? device()->createMultiplot() : new CGnuPlotMultiplot(this));
}

CGnuPlotGroup *
CGnuPlot::
createGroup(CGnuPlotWindow *window)
{
  CGnuPlotGroup *group = (device() ? device()->createGroup(window) : new CGnuPlotGroup(window));

  group->initObjects();

  return group;
}

CGnuPlotPlot *
CGnuPlot::
createPlot(CGnuPlotGroup *group, PlotStyle style)
{
  return (device() ? device()->createPlot(group, style) : new CGnuPlotPlot(group, style));
}

CGnuPlotLineStyle *
CGnuPlot::
createLineStyle(CGnuPlot *plot)
{
  return (device() ? device()->createLineStyle(plot) : new CGnuPlotLineStyle(plot));
}

CGnuPlotLineType *
CGnuPlot::
createLineType()
{
  return new CGnuPlotLineType;
}

CGnuPlotAxis *
CGnuPlot::
createAxis(CGnuPlotGroup *group, const CGnuPlotAxisData &data)
{
  return (device() ? device()->createAxis(group, data) : new CGnuPlotAxis(group, data));
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

CGnuPlotPm3D *
CGnuPlot::
createPm3D(CGnuPlotGroup *group)
{
  return (device() ? device()->createPm3D(group) : new CGnuPlotPm3D(group));
}

CGnuPlotTimeStamp *
CGnuPlot::
createTimeStamp(CGnuPlotGroup *group)
{
  return (device() ? device()->createTimeStamp(group) : new CGnuPlotTimeStamp(group));
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
processAssignFunction(const std::string &str)
{
  if (isDebug())
    debugMsg("Assign fn = '" + str + "'");

  CParseLine line(str);

  std::string identifier;

  (void) readIdentifier(line, identifier);

  if (identifier == "")
    return false;

  // function definition
  if      (line.skipSpaceAndChar('(')) {
    if (! parseFunctionDef(identifier, line))
      return false;
  }
  // variable assignment
  else {
    if (! line.skipSpaceAndChar('='))
      return false;

    if (! parseVariableDef(identifier, line))
      return false;
  }

  return true;
}

CGnuPlotPlotP
CGnuPlot::
addFunction2D(CGnuPlotGroupP &group, const StringArray &functions, PlotStyle style)
{
  uint nf = 1;

  if (isParametric())
    nf = 2;

  if (functions.size() < nf) {
    errorMsg("Too few functions");
    return CGnuPlotPlotP();
  }

  StringArray functions1;

  for (uint i = 0; i < nf; ++i)
    functions1.push_back(functions[i]);

  //---

  //if (! tableFile_.isEnabled()) {

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title;

    for (const auto &f : functions1) {
      if (title != "") title += ",";

      title += f;
    }

    plot->setKeyTitleString(title);
  }

  plot->setSetNum(0);

  plot->init();

  plot->setFunctions(functions1);

  updateFunction2D(plot.get());

  return plot;
}

void
CGnuPlot::
updateFunction2D(CGnuPlotPlot *plot)
{
  plot->clearPoints();

  // don't set polar for plot (double conversion)
  // TODO: set polar and mark converted

  //---

  const CGnuPlotAxisData &xaxis = plot->xaxis(plot->xind());

  const SampleVars &sampleVars = plot->sampleVars();

  int nx = plot->samplesNX();

  if (! plot->isPolar() && ! plot->isParametric()) {
    double xmin, xmax;

    plot->getXRange(&xmin, &xmax);

    if (sampleVars.x.min.isValid()) xmin = sampleVars.x.min.getValue();
    if (sampleVars.x.max.isValid()) xmax = sampleVars.x.max.getValue();

    double xmin1 = xaxis.mapLogValue(xmin);
    double xmax1 = xaxis.mapLogValue(xmax);

    //---

    std::string varName1("x"), varName2("t");

    plot->dummyVars().dummyVars(varName1, varName2,
      plot->isParametric(), plot->isPolar(), /*is3D*/ false);

    if (xaxis.getDummyVar() != "")
      varName1 = xaxis.getDummyVar();

    if (sampleVars.x.var != "")
      varName1 = sampleVars.x.var;

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

    double dx1 = (nx > 1 ? (xmax1 - xmin1)/(nx - 1) : 0);

    if (plot->functions()[0] == "NaN") {
      for (auto x1 : RRangeItr::range(xmin1).step(dx1).limit(nx)) {
        double x = xaxis.unmapLogValue(x1);

        plot->addPoint2D(x, CExprValuePtr());
      }
    }
    else {
      CExprTokenStack cstack = plot->app()->compileExpression(plot->functions()[0]);

      for (auto x1 : RRangeItr::range(xmin1).step(dx1).limit(nx)) {
        double x = xaxis.unmapLogValue(x1);

        xvar->setRealValue(x);

        CExprValuePtr value;

        if (! CExprInst->executeCTokenStack(cstack, value)) {
          plot->app()->errorMsg("Failed to eval \'" + plot->functions()[0] + "\' for " +
                                varName1 + "=" + CStrUtil::toString(x));
          value = CExprValuePtr(); // TODO
        }

        double y = 0.0;

        if (value.isValid() && value->getRealValue(y)) {
          plot->addPoint2D(x, y);
        }
      }
    }
  }
  else if (plot->isPolar()) {
    double tmin, tmax;

    plot->getPolarTRange(&tmin, &tmax);

    //---

    std::string varName("t");

    //getDummyVar("t", varName);

    CExprVariablePtr tvar = CExprInst->createRealVariable(varName, 0.0);

    double da = (nx > 1 ? (tmax - tmin)/(nx - 1) : 0);

    if (plot->functions()[0] == "NaN") {
      for (auto a : RRangeItr::range(tmin).step(da).limit(nx))
        plot->addPoint2D(a, CExprValuePtr());
    }
    else {
      CExprTokenStack cstack = plot->app()->compileExpression(plot->functions()[0]);

      for (auto a : RRangeItr::range(tmin).step(da).limit(nx)) {
        //double c = cos(a);
        //double s = sin(a);

        tvar->setRealValue(a);

        CExprValuePtr value;

        if (! CExprInst->executeCTokenStack(cstack, value)) {
          plot->app()->errorMsg("Failed to eval " + plot->functions()[0] + " for " +
                                varName + "=" + CStrUtil::toString(a));
          value = CExprValuePtr();
        }

        double r = 0.0;

        if (value.isValid() && value->getRealValue(r)) {
          //double x = r*c;
          //double y = r*s;

          //plot->addPoint2D(x, y);
          plot->addPoint2D(a, r);
        }
      }
    }
  }
  else if (plot->isParametric()) {
    plot->setMapping(CGnuPlotTypes::Mapping::CARTESIAN_MAPPING);

    //------

    double tmin, tmax;

    plot->getParametricTRange(&tmin, &tmax);

    //---

    std::string varName1("t"), varName2("y");

    plot->dummyVars().dummyVars(varName1, varName2,
      plot->isParametric(), plot->isPolar(), /*is3D*/ false);

    CExprVariablePtr tvar = CExprInst->createRealVariable(varName1, 0.0);

    double dt = (nx > 1 ? (tmax - tmin)/(nx - 1) : 0);

    bool f1 = (plot->functions()[0] != "NaN");
    bool f2 = (plot->functions()[1] != "NaN");

    CExprTokenStack cstack1, cstack2;

    if (f1) cstack1 = plot->app()->compileExpression(plot->functions()[0]);
    if (f2) cstack2 = plot->app()->compileExpression(plot->functions()[1]);

    for (auto t : RRangeItr::range(tmin).step(dt).limit(nx)) {
      tvar->setRealValue(t);

      double x, y;

      //---

      if (f1) {
        CExprValuePtr value;

        if (! CExprInst->executeCTokenStack(cstack1, value)) {
          plot->app()->errorMsg("Failed to eval " + plot->functions()[0] + " for " +
                                varName1 + "=" + CStrUtil::toString(t));
          value = CExprValuePtr();
        }

        if (! value.isValid() || ! value->getRealValue(x))
          x = CMathGen::getNaN();
      }
      else
        x = CMathGen::getNaN();

      //---

      if (f2) {
        CExprValuePtr value;

        if (! CExprInst->executeCTokenStack(cstack2, value)) {
          plot->app()->errorMsg("Failed to eval " + plot->functions()[1] + " for " +
                                varName2 + "=" + CStrUtil::toString(t));
          value = CExprValuePtr();
        }

        if (! value.isValid() || ! value->getRealValue(y))
          y = CMathGen::getNaN();
      }
      else
        y = CMathGen::getNaN();

      //---

      plot->addPoint2D(x, y);
    }
  }
  else
    assert(false);
}

CGnuPlotPlotP
CGnuPlot::
addFunction3D(CGnuPlotGroupP &group, const StringArray &functions, PlotStyle style)
{
  //if (! tableFile_.isEnabled()) {

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title;

    for (const auto &f : functions) {
      if (title != "") title += ",";

      title += f;
    }

    plot->setKeyTitleString(title);
  }

  plot->setSetNum(0);

  plot->init();

  //---

  const SampleVars &sampleVars = plot->sampleVars();

  int nx, ny;

  //isoSamples_.get(nx, ny);
  plot->samples().get(nx, ny);

  if (! plot->isPolar() && ! plot->isParametric()) {
    double xmin, ymin, xmax, ymax;

    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);

    if (sampleVars.x.min.isValid()) xmin = sampleVars.x.min.getValue();
    if (sampleVars.x.max.isValid()) xmax = sampleVars.x.max.getValue();
    if (sampleVars.y.min.isValid()) ymin = sampleVars.y.min.getValue();
    if (sampleVars.y.max.isValid()) ymax = sampleVars.y.max.getValue();

    //---

    std::string varName1("x"), varName2("y");

    dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ true);

    if (xaxis(xind_).getDummyVar() != "")
      varName1 = xaxis(xind_).getDummyVar();

    if (sampleVars.x.var != "")
      varName1 = sampleVars.x.var;

    if (yaxis(yind_).getDummyVar() != "")
      varName2 = yaxis(yind_).getDummyVar();

    if (sampleVars.y.var != "")
      varName2 = sampleVars.y.var;

    CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);
    CExprVariablePtr yvar = CExprInst->createRealVariable(varName2, 0.0);

    double dx = (nx > 1 ? (xmax - xmin)/(nx - 1) : 0);
    double dy = (ny > 1 ? (ymax - ymin)/(ny - 1) : 0);

    if (functions[0] == "NaN") {
      int iy = 0;

      for (auto y : RRangeItr::range(ymin).step(dy).limit(ny + 1)) {
        for (auto x : RRangeItr::range(xmin).step(dx).limit(nx + 1)) {
          plot->addPoint3D(iy, x, y, CExprValuePtr());
        }

        ++iy;
      }
    }
    else {
      CExprTokenStack cstack = plot->app()->compileExpression(functions[0]);

      int iy = 0;

      for (auto y : RRangeItr::range(ymin).step(dy).limit(ny)) {
        yvar->setRealValue(y);

        for (auto x : RRangeItr::range(xmin).step(dx).limit(nx)) {
          xvar->setRealValue(x);

          CExprValuePtr value;

          if (! CExprInst->executeCTokenStack(cstack, value)) {
            errorMsg("Failed to eval " + functions[0] + " for " + varName1 + "=" +
                     CStrUtil::toString(x) + ", " + varName2 + "=" + CStrUtil::toString(y));
            value = CExprValuePtr(); // TODO
          }

          double z = 0.0;

          if (value.isValid() && value->getRealValue(z)) {
            //assert(! IsNaN(z));

            plot->addPoint3D(iy, x, y, z);
          }
        }

        ++iy;
      }
    }
  }
  else if (plot->isPolar()) {
    double umin, umax;
    double vmin, vmax;

    getURange(&umin, &umax);
    getVRange(&vmin, &vmax);

    //---

    std::string varName1("u"), varName2("v");

    //dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ true);

    CExprVariablePtr uvar = CExprInst->createRealVariable(varName1, 0.0);
    CExprVariablePtr vvar = CExprInst->createRealVariable(varName2, 0.0);

    double da1 = (nx > 0 ? (umax - umin)/nx : 0);
    double da2 = (ny > 0 ? (vmax - vmin)/ny : 0);

    bool f1 = (functions[0] != "NaN");
    bool f2 = (functions[1] != "NaN");

    CExprTokenStack cstack1, cstack2;

    if (f1) cstack1 = plot->app()->compileExpression(functions[0]);
    if (f2) cstack2 = plot->app()->compileExpression(functions[1]);

    double a2 = vmin;

    for (int j = 0; j <= ny; ++j, a2 += da2) {
      //double c2 = cos(a2), s2 = sin(a2);

      vvar->setRealValue(a2);

      double a1 = umin;

      for (int i = 0; i <= nx; ++i, a1 += da1) {
        double c1 = cos(a1), s1 = sin(a1);

        uvar->setRealValue(a1);

        //---

        double r1 = 0.0;

        if (f1) {
          CExprValuePtr value1;

          if (! CExprInst->executeCTokenStack(cstack1, value1)) {
            errorMsg("Failed to eval " + functions[0] + " for a1=" + CStrUtil::toString(a1) +
                     " a2=" + CStrUtil::toString(a2));
            value1 = CExprValuePtr();
          }

          if (! value1.isValid() || ! value1->getRealValue(r1))
            r1 = CMathGen::getNaN();
        }
        else
          r1 = CMathGen::getNaN();

        //---

        double r2 = 0.0;

        if (f2) {
          CExprValuePtr value2;

          if (! CExprInst->executeCTokenStack(cstack2, value2)) {
            errorMsg("Failed to eval " + functions[1] + " for a1=" + CStrUtil::toString(a1) +
                     " a2=" + CStrUtil::toString(a2));
            value2 = CExprValuePtr();
          }

          if (! value2.isValid() || ! value2->getRealValue(r2))
            r2 = CMathGen::getNaN();
        }
        else
          r2 = CMathGen::getNaN();

        //---

        double x = r1*c1;
        double y = r1*s1;
        double z = r2;

        plot->addPoint3D(j, x, y, z);
      }
    }
  }
  else if (plot->isParametric()) {
    plot->setMapping(CGnuPlotTypes::Mapping::CARTESIAN_MAPPING);

    if (functions.size() < 3) {
      errorMsg("Too few functions");
      return 0;
    }

    //------

    double umin, umax;
    double vmin, vmax;

    getURange(&umin, &umax);
    getVRange(&vmin, &vmax);

    //---

    std::string varName1("u"), varName2("v");

    dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ true);

    CExprVariablePtr uvar = CExprInst->createRealVariable(varName1, 0.0);
    CExprVariablePtr vvar = CExprInst->createRealVariable(varName2, 0.0);

    double du = (umax - umin)/nx;
    double dv = (vmax - vmin)/ny;

    bool f1 = (functions[0] != "NaN");
    bool f2 = (functions[1] != "NaN");
    bool f3 = (functions[2] != "NaN");

    CExprTokenStack cstack1, cstack2, cstack3;

    if (f1) cstack1 = compileExpression(functions[0]);
    if (f2) cstack2 = compileExpression(functions[1]);
    if (f3) cstack3 = compileExpression(functions[2]);

    double v = vmin;

    for (int j = 0; j <= ny; ++j, v += dv) {
      vvar->setRealValue(v);

      double u = umin;

      for (int i = 0; i <= nx; ++i, u += du) {
        uvar->setRealValue(u);

        double x, y, z;

        //---

        if (f1) {
          CExprValuePtr value;

          if (! CExprInst->executeCTokenStack(cstack1, value)) {
            errorMsg("Failed to eval " + functions[0] + " for " + varName1 + "=" +
                     CStrUtil::toString(u) + " " + varName2 + "=" + CStrUtil::toString(v));
            value = CExprValuePtr();
          }

          if (! value.isValid() || ! value->getRealValue(x))
            x = CMathGen::getNaN();
        }
        else
          x = CMathGen::getNaN();

        //---

        if (f2) {
          CExprValuePtr value;

          if (! CExprInst->executeCTokenStack(cstack2, value)) {
            errorMsg("Failed to eval " + functions[1] + " for " + varName1 + "=" +
                     CStrUtil::toString(u) + " " + varName2 + "=" + CStrUtil::toString(v));
            value = CExprValuePtr();
          }

          if (! value.isValid() || ! value->getRealValue(y))
            y = CMathGen::getNaN();
        }
        else
          y = CMathGen::getNaN();

        //---

        if (f3) {
          CExprValuePtr value;

          if (! CExprInst->executeCTokenStack(cstack3, value)) {
            errorMsg("Failed to eval " + functions[2] + " for " + varName1 + "=" +
                     CStrUtil::toString(u) + " " + varName2 + "=" + CStrUtil::toString(v));
            value = CExprValuePtr();
          }

          if (! value.isValid() || ! value->getRealValue(z))
            z = CMathGen::getNaN();
        }
        else
          z = CMathGen::getNaN();

        //---

        plot->addPoint3D(j, x, y, z);
      }
    }
  }
  else
    assert(false);

  //---

  return plot;
}

void
CGnuPlot::
addFile2D(Plots &plots, CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
          const CGnuPlotUsingCols &usingCols, CGnuPlotLineStyle &lineStyle,
          CGnuPlotFillStyle &fillStyle, CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  // gen file lines (one dimension)
  if      (filename == "+") {
    addGen1File2D(plots, group, style, usingCols, lineStyle, fillStyle, styleData, keyTitle);

    return;
  }
  // gen file lines (two dimensions)
  else if (filename == "++") {
    addGen2File2D(plots, group, style, usingCols, lineStyle, fillStyle, styleData, keyTitle);

    return;
  }

  //---

  // get block lines
  if      (filename[0] == '$') {
    std::string name = filename.substr(1);

    CGnuPlotFile::Lines lines;

    getBlockLines(name, lines);

    dataFile_.setFileName(filename);

    dataFile_.setLines(lines);
  }
  // get named file lines
  else if (filename != "-") {
    if (! readDataFile(filename, dataFile_))
      return;
  }
  // get input file lines
  else if (filename == "-") {
    dataFile_.setFileName("-");

    readDataFileLines();
  }

  //---

  dataFile_.processFile();

  if (dataFile_.numSets() == 0) {
    errorMsg("no data in file");
  }

  //---

  typedef std::map<int,CGnuPlotPlotP> SetPlots;

  SetPlots setPlots;

  // create plot per set
  for (int setNum = 0; setNum < dataFile_.numSets(); ++setNum) {
    CGnuPlotPlotP plot(createPlot(group.get(), style));

    setPlots[setNum] = plot;

    plot->setSetNum  (setNum);
    plot->setDataFile(dataFile_);
    plot->setKeyTitle(keyTitle);

    plot->setUsingCols(usingCols);

    plot->init();

    addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
  }

  //---

  // select values for plot
  for (int setNum = 0; setNum < dataFile_.numSets(); ++setNum) {
    CGnuPlotPlotP plot = setPlots[setNum];

    setPlotValues2D(plot.get());
  }
}

void
CGnuPlot::
setPlotValues2D(CGnuPlotPlot *plot)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  plot->clearPoints();

  //---

  const CGnuPlotFile      &dataFile  = plot->dataFile();
  const CGnuPlotUsingCols &usingCols = plot->usingCols();
  const CGnuPlotKeyTitle  &keyTitle  = plot->keyTitle();

  CGnuPlotLineStyle lineStyle = plot->lineStyle();
  CGnuPlotFillStyle fillStyle = plot->fillStyle();
  CGnuPlotStyleData styleData = plot->styleData();
  CGnuPlotKeyData   keyData   = plot->keyData  ();

  //---

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "" && ! keyData.columnHead()) {
    std::string title = "\"" + dataFile.fileName() + "\"";

    plot->setKeyTitleString(title);
  }

  //---

  pointNum_ = 0;

  // add fields (discontinuity per subset)
  int setNum = plot->setNum();

  setSetNum(setNum);

  int numSubSets = dataFile.numSubSets(setNum);

  for (int subSetNum = 0; subSetNum < numSubSets; ++subSetNum) {
    int lineNum = 0;

    std::string cstr;

    if (dataFile.commentStr(setNum, subSetNum, 0, cstr)) {
      CParseLine line1(cstr);

      if (line1.isString("--style--")) {
        line1.skipNonSpace();

        parseCommentStyle(line1, lineStyle, fillStyle, styleData, keyTitle);

        plot->setLineStyle(lineStyle);
        plot->setFillStyle(fillStyle);
        plot->setStyleData(styleData);
      }
    }

    // use first line for columnheaders if option enabled
    if (subSetNum == 0 && keyData.columnHead()) {
      const CGnuPlotFile::Fields &fields = dataFile.fields(setNum, subSetNum, lineNum);

      keyData.setColumns(fields);

      if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "" && keyData.columnHead()) {
        std::string title;

        if (keyData.isColumnNumValid())
          title = keyData.columnNumValue();
        else
          title = usingCols.columnTitle(keyData.columns());

        plot->setKeyTitleString(title);
      }

      ++lineNum;
    }

    int lineNum1 = lineNum;

    for ( ; lineNum < dataFile.numLines(setNum, subSetNum); ++lineNum, ++pointNum_) {
      // first point of each new set is a discontinuity
      bool discontinuity = (subSetNum > 0 && lineNum == lineNum1);

      //---

      // convert fields on line to values
      // (Note: save fields as used by columnhead function)
      // (TODO: save current file setNum, subSetNum and lineNum instead)
      const CGnuPlotFile::Fields &fields = dataFile.fields(setNum, subSetNum, lineNum);

      fieldValues_.clear();

      for (const auto &field : fields) {
        CExprValuePtr value = fieldToValue(fieldValues_.size(), field);

        fieldValues_.push_back(value);
      }

      bool bad = false;

      Values values;
      Params params;
      Matrix matrix;

      if (matrixData().isMatrix()) {
        if (matrixData().isColumnHeaders() && lineNum == 0) {
          StringArray strs;

          for (const auto &value : fieldValues_) {
            std::string str;

            if (! value.isValid() || ! value->getStringValue(str))
              str = "";

            strs.push_back(str);
          }

          for (uint i = 0; i < strs.size(); ++i) {
            if (i == 0 && matrixData().isRowHeaders())
              continue;

            int colNum = (matrixData().isRowHeaders() ? i - 1 : i);

            //matrixData().setColumnHeader(colNum, strs[i]);
            xaxis(1).setTicLabel(colNum, strs[i], 0);
          }
        }
        else {
          if (dataFile.numSets() == 1) {
            int i = 0;

            for (const auto &value : fieldValues_) {
              if (i == 0 && matrixData().isRowHeaders()) {
                int rowNum = (matrixData().isColumnHeaders() ? lineNum - 1 : lineNum);

                std::string str;

                if (! value.isValid() || ! value->getStringValue(str))
                  str = "";

                //matrixData().setRowHeader(rowNum, str);
                yaxis(1).setTicLabel(rowNum, str, 0);
              }
              else {
                int x = (matrixData().isColumnHeaders() ? i         - 1 : i        );
                int y = (matrixData().isRowHeaders   () ? pointNum_ - 1 : pointNum_);

                CExprValuePtr xvalue = CExprInst->createRealValue(x);
                CExprValuePtr yvalue = CExprInst->createRealValue(y);

                values.push_back(xvalue);
                values.push_back(yvalue);
                values.push_back(value);

                if (usingCols.numCols() > 0) {
                  Values values1;

                  bool   bad1;
                  Params params1;

                  (void) usingCols.decodeValues(th, 0, i, values, bad1, values1, params1);

                  // TODO: error handling
                  values = values1;
                }

                matrix.push_back(values);

                values.clear();
              }

              ++i;
            }
          }
          else {
            for (const auto &value : fieldValues_) {
              values.push_back(value);
            }
          }
        }
      }
      else {
        int nskip = 0;

        // no columns specified so use default number of columns for style
        if (usingCols.numCols() == 0) {
          int nv = fieldValues_.size();

          CGnuPlotStyleBase *plotStyle = getPlotStyle(plot->style());

          if (plotStyle)
            nv = std::min(nv, plotStyle->numUsing());

          for (const auto &i : IRangeItr::range(0, nv)) {
            if (! fieldValues_[i].isValid())
              bad = true;

            values.push_back(fieldValues_[i]);
          }
        }
        // extract specified columns
        else {
          nskip = usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

          // one value then auto add the point number
          if (values.size() == 1) {
            CExprValuePtr value1 = CExprInst->createRealValue(pointNum());

            values.push_back(value1);

            std::swap(values[0], values[1]);
          }
        }

        if      (nskip)
          discontinuity = true;
      //else if (bad)
      //  continue;
      }

      //---

      // add values
      if (matrixData().isMatrix()) {
        int i = 0;

        for (const auto &values : matrix) {
          bool discontinuity = (i == int(matrix.size() - 1));

          if (lineNum == dataFile.numLines(setNum, subSetNum) - 1)
            discontinuity = false;

          // add values
          /*int pointNum=*/ plot->addPoint2D(values, discontinuity, bad, params);

          //if (ticLabel_.valid) plot->setPoint2DLabel(pointNum, ticLabel_.str);

          ++i;
        }
      }
      else {
        for (uint i = 0; i < values.size(); ++i) {
          if ((i == 0 && xaxis(1).isTime()) ||
              (i == 1 && yaxis(1).isTime())) {
            std::string str;

            if (! values[i]->getStringValue(str))
              continue;

            double r;

            if (timeToValue(str, r))
              values[i] = CExprInst->createRealValue(r);
          }
        }

        // add values
        /*int pointNum=*/ plot->addPoint2D(values, discontinuity, bad, params);

        //if (ticLabel_.valid) plot->setPoint2DLabel(pointNum, ticLabel_.str);
      }
    }
  }
}

void
CGnuPlot::
addGen1File2D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
              const CGnuPlotUsingCols &usingCols,
              CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
              CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  // create plot
  setSetNum(0);

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);

  //---

  initGen1File2D(plot.get(), usingCols);
}

void
CGnuPlot::
initGen1File2D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  // gen file lines (one dimensions)
  int nx, ny;

  plot->samples().get(nx, ny);

  //---

  // get sample range
  const SampleVars &sampleVars = plot->sampleVars();

  double xmin, xmax;

  getXRange(&xmin, &xmax);

  if (sampleVars.x.min.isValid()) xmin = sampleVars.x.min.getValue();
  if (sampleVars.x.max.isValid()) xmax = sampleVars.x.max.getValue();

  //---

  std::string varName1("x"), varName2("t");

  dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ false);

  if (sampleVars.x.var != "")
    varName1 = sampleVars.x.var;

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

  double x  = xmin;
  double dx = (xmax - xmin)/nx;

  for (int ix = 0; ix <= nx; ++ix, x += dx) {
    CExprValuePtr xval = CExprInst->createRealValue(x);

    xvar->setValue(xval);

    fieldValues_.clear();

    fieldValues_.push_back(xval);

    //---

    pointNum_ = ix;

    bool   bad;
    Values values;
    Params params;

    usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

    plot->addPoint2D(values, false, bad, params);
  }
}

void
CGnuPlot::
addGen2File2D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
              const CGnuPlotUsingCols &usingCols,
              CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
              CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  setSetNum(0);

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);

  //---

  initGen2File2D(plot.get(), usingCols);
}

void
CGnuPlot::
initGen2File2D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  // gen file lines (two dimensions)
  int nx, ny;

  plot->samples().get(nx, ny);

  //---

  // get sample range
  double xmin, xmax;
  double ymin, ymax;

  getXRange(&xmin, &xmax);
  getYRange(&ymin, &ymax);

  //---

  std::string varName1("x"), varName2("t");

  dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ false);

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);
  CExprVariablePtr yvar = CExprInst->createRealVariable(varName2, 0.0);

  double y  = ymin;
  double dy = (ymax - ymin)/ny;

  for (int iy = 0; iy <= ny; ++iy, y += dy) {
    CExprValuePtr yval = CExprInst->createRealValue(y);

    yvar->setValue(yval);

    double x  = xmin;
    double dx = (xmax - xmin)/nx;

    for (int ix = 0; ix <= nx; ++ix, x += dx) {
      CExprValuePtr xval = CExprInst->createRealValue(x);

      xvar->setValue(xval);

      fieldValues_.clear();

      fieldValues_.push_back(xval);
      fieldValues_.push_back(yval);

      //---

      pointNum_ = ix;

      bool   bad;
      Values values;
      Params params;

      usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

      plot->addPoint2D(values, false, bad, params);
    }
  }
}

void
CGnuPlot::
parseCommentStyle(CParseLine &line, CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &,
                  CGnuPlotStyleData &, const CGnuPlotKeyTitle &)
{
//std::cerr << "parseCommentStyle: " << line.substr() << std::endl;
  while (line.isValid()) {
    std::string arg = readNonSpace(line);

    if      (arg == "linecolor" || arg == "lc") {
      CGnuPlotColorSpec lc;

      if (parseColorSpec(line, lc))
        lineStyle.setLineColor(lc);
    }
    else if (arg == "pointtype" || arg == "pt") {
      int         pt = 1;
      std::string ptstr;

      if      (parseInteger(line, pt))
        lineStyle.setPointType(pt);
      else if (parseString(line, ptstr))
        lineStyle.setPointTypeStr(ptstr);
      else
        errorMsg("Invalid pointtype");
    }
    else if (arg == "pointsize" || arg == "ps") {
      double s;

      if (parseReal(line, s))
        lineStyle.setPointSize(s);
      else
        errorMsg("Invalid pointsize");
    }
    else if (arg == "tippoints") {
      lineStyle.setTipPoints(true);
    }
    else if (arg == "notippoints") {
      lineStyle.setTipPoints(false);
    }
    else {
      errorMsg("Invalid arg : '" + arg + "'");
    }
  }
}

CGnuPlotPlotP
CGnuPlot::
addImage2D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
           const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  CGnuPlotImageStyle istyle = imageStyle();

  istyle.setFilename(filename);

  istyle.setUsingCols(usingCols);

  CGnuPlotPlot::YXColor   rowColors;
  CGnuPlotPlot::RowValues rowValues;

  COptReal xmin, ymin, xmax, ymax;

  if (istyle.fileType() == CGnuPlotTypes::ImageType::PNG) {
    if (filename == "-") {
      errorMsg("no \"-\" filename support for PNG");
      return 0;
    }

    CImageFileSrc src(filename);

    CImagePtr image = CImageMgrInst->createImage(src);
    if (! image.isValid()) return 0;

    istyle.setWidth (image->getWidth ());
    istyle.setHeight(image->getHeight());

    for (auto iy : IRangeItr::range(0, istyle.height().getValue())) {
      ymin.updateMin(iy); ymax.updateMax(iy);

      for (auto ix : IRangeItr::range(0, istyle.width().getValue())) {
        CRGBA rgba;

        image->getRGBAPixel(ix, iy, rgba);

        xmin.updateMin(ix); xmax.updateMax(ix);

        rowColors[iy][ix] = rgba;
      }
    }

    istyle.setFlipY(true);
  }
  else {
    CGnuPlotUnixFile unixFile;

    if (filename == "-") {
      if (fileData_.file)
        unixFile.init(fileData_.file);
      else
        unixFile.init(stdin);
    }
    else
      unixFile.init(filename);

    if (! unixFile.isValid())
      return 0;

    CUnixFile *file = unixFile.file();

    //---

    if (binaryFormat().format() == "") {
      int iw = -1, ih = -1;

      if (istyle.width().isValid() && istyle.height().isValid()) {
        iw = istyle.width ().getValue();
        ih = istyle.height().getValue();
      }

      if (style == PlotStyle::IMAGE) {
        std::vector<uchar> chars;

        uchar c;

        while (file->readChar(c))
          chars.push_back(c);

        // how set image size ???
        int nv = chars.size();

        iw = int(sqrt(nv));
        ih = nv/iw;

        for (int i = 0; i < nv; ++i) {
          int ix = i % iw;
          int iy = i / iw;

          xmin.updateMin(ix); xmax.updateMax(ix);
          ymin.updateMin(iy); ymax.updateMax(iy);

          rowValues[iy].push_back(chars[i]);
        }
      }
      else {
        uchar              c;
        std::vector<uchar> chars;

        int n = std::numeric_limits<int>::max();

        if (istyle.width().isValid() && istyle.height().isValid())
          n = 3*istyle.width().getValue()*istyle.height().getValue();

        // read from current input source
        while (int(chars.size()) < n && file->readChar(c))
          chars.push_back(c);

        if (iw < 0 || ih < 0) {
          int nv = chars.size();

          iw = int(sqrt(nv));
          ih = nv/iw;
        }

        //---

        std::vector<uchar> rgb;

        rgb.resize(3);

        for (uint i = 0, j = 0, k = 0; i < chars.size(); ++i) {
          rgb[j++] = chars[i];

          if (j == 3) {
            int ix = k % iw;
            int iy = k / iw;

            CRGBA rgba(rgb[0]/255.0, rgb[1]/255.0, rgb[2]/255.0);

            xmin.updateMin(ix); xmax.updateMax(ix);
            ymin.updateMin(iy); ymax.updateMax(iy);

            rowColors[iy][ix] = rgba;

            j = 0;

            ++k;
          }
        }
      }
    }
    else {
      //int nc = usingCols.numCols();

      typedef std::vector<Reals> RealsArray;

      RealsArray valuesArray;

      int iw, ih;

      if (istyle.width().isValid() && istyle.height().isValid()) {
        iw = istyle.width ().getValue();
        ih = istyle.height().getValue();

        int n = iw*ih;

        for (int i = 0; i < n; ++i) {
          Reals values;

          binaryFormat_.readValues(*file, values, 3);

          valuesArray.push_back(values);
        }
      }
      else {
        // read all values
        Reals values;

        binaryFormat_.readValues(*file, values);

        int nv = values.size()/3;

        valuesArray.resize(nv);

        for (int i = 0, j = 0; i < nv; ++i, j += 3)
          valuesArray[i] = Reals({values[j], values[j + 1], values[j + 2]});

        // how set image size ???
        iw = int(sqrt(nv));
        ih = nv/iw;
      }

      //---

      for (uint i = 0; i < valuesArray.size(); ++i) {
        int ix = (i % iw);
        int iy = (i / iw);

        if (! dataFile_.every().validPointIndex(ix) || ! dataFile_.every().validBlockIndex(iy))
          continue;

        xmin.updateMin(ix); xmax.updateMax(ix);
        ymin.updateMin(iy); ymax.updateMax(iy);

        const Reals &values = valuesArray[i];

        if (usingCols.numCols() > 0) {
          Values fieldValues;

          for (const auto &v : values) {
            CExprValuePtr val = CExprInst->createRealValue(v);

            fieldValues.push_back(val);
          }

          Params params;

          bool   bad;
          Values values;

          setSetNum(0);

          pointNum_ = i;

          usingCols.decodeValues(th, setNum_, pointNum_, fieldValues, bad, values, params);

          if (style == PlotStyle::IMAGE) { // palette
            double r = 0;

            if (values.size() > 0 && values[0].isValid()) values[0]->getRealValue(r);

            rowValues[iy].push_back(r);
          }
          else {
            double r = 0, g = 0, b = 0;

            if (values.size() > 0 && values[0].isValid()) {
              values[0]->getRealValue(r);
              if (! binaryFormat_.indexFormat(0).isReal())
                r = scaleColorComponent(r);
            }

            if (values.size() > 1 && values[1].isValid()) {
              values[1]->getRealValue(g);
              if (! binaryFormat_.indexFormat(1).isReal())
                g = scaleColorComponent(g);
            }

            if (values.size() > 2 && values[2].isValid()) {
              values[2]->getRealValue(b);
              if (! binaryFormat_.indexFormat(2).isReal())
                b = scaleColorComponent(b);
            }

            CRGBA rgba(r, g, b);

            rowColors[iy][ix] = rgba;
          }
        }
        else {
          int nv = values.size();

          if (style == PlotStyle::IMAGE) { // palette
            double r = (nv > 0 ? values[0] : 0);

            rowValues[iy].push_back(r);
          }
          else {
            double r = 0, g = 0, b = 0;

            r = (nv > 0 ? values[0] : 0);
            g = (nv > 1 ? values[1] : 0);
            b = (nv > 2 ? values[2] : 0);

            CRGBA rgba(r/255.0, g/255.0, b/255.0);

            rowColors[iy][ix] = rgba;
          }
        }
      }

      if (istyle.width().isValid() && istyle.height().isValid()) {
        if (style == PlotStyle::IMAGE) {
          int nx = 0;

          for (const auto &d : rowValues)
            nx = std::max(nx, int(d.second.size()));

          for (auto &d : rowValues) {
            while (int(d.second.size()) < nx)
              d.second.push_back(0);
          }
        }
      }
    }
  }

  //---

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title = "\"" + filename + "\"";

    plot->setKeyTitleString(title);
  }

  CBBox2D ibbox(xmin.getValue(0), ymin.getValue(0), xmax.getValue(1), ymax.getValue(1));

  plot->setImageRange(ibbox);

  if (style == PlotStyle::IMAGE)
    plot->setImageValues(rowValues);
  else
    plot->setImageColors(rowColors);

  plot->setImageStyle(istyle);

  return plot;
}

CGnuPlotPlotP
CGnuPlot::
addBinary2D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
            const CGnuPlotUsingCols &usingCols)
{
  CGnuPlotPlotP plot;

  // matrix format
  if (binaryFormat().format() == "") {
    plot = CGnuPlotPlotP(createPlot(group.get(), PlotStyle::SURFACE));

    plot->setSetNum(0);

    plot->init();

    readBinaryFile2D(filename, plot);
  }
  else {
    // init input file
    CGnuPlotUnixFile unixFile;

    if (filename == "-") {
      if (fileData_.file)
        unixFile.init(fileData_.file);
      else
        unixFile.init(stdin);
    }
    else
      unixFile.init(filename);

    if (! unixFile.isValid())
      return plot;

    CUnixFile *file = unixFile.file();

    plot = readBinaryFormatFile2D(file, group, style, usingCols);
  }

  //---

  return plot;
}

CGnuPlotPlotP
CGnuPlot::
readBinaryFormatFile2D(CUnixFile *file, CGnuPlotGroupP &group, PlotStyle style,
                       const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  CGnuPlotStyleBase *plotStyle = getPlotStyle(style);

  // get plots style expected number of values
  int nv = (plotStyle ? plotStyle->numUsing() : 2);

  // get max using column
  if      (usingCols.colMax().isValid())
    nv = usingCols.colMax().getValue();
  else if (binaryFormat_.numFormats() > 0)
    nv = binaryFormat_.numFormatValues();

  //---

  Reals vals;

  if (! recordValues().empty()) {
    for (const auto &n : recordValues()) {
      binaryFormat_.readValues(*file, vals, nv*n);
    }
  }
  else {
    binaryFormat_.readValues(*file, vals);
  }

  //---

  CGnuPlotPlotP plot = CGnuPlotPlotP(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  //---

  // add values
  int n = vals.size();

  if (n < nv) {
    errorMsg("Missing values for binary plot");
    return CGnuPlotPlotP();
  }

  for (const auto &i : IRangeItr::range(0, n, nv)) {
    fieldValues_.clear();

    for (const auto &j : IRangeItr::range(0, nv)) {
      CExprValuePtr val = CExprInst->createRealValue(vals[i + j]);

      fieldValues_.push_back(val);
    }

    Params params;

    if (usingCols.numCols()) {
      bool   bad;
      Values values;

      setSetNum(0);

      pointNum_ = i/nv;

      usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

      plot->addPoint2D(values, false, bad, params);
    }
    else
      plot->addPoint2D(fieldValues_, false, false, params);
  }

  return plot;
}

bool
CGnuPlot::
readBinaryFile2D(const std::string &filename, CGnuPlotPlotP &plot)
{
  // init input file
  CGnuPlotUnixFile unixFile;

  if (filename == "-") {
    if (fileData_.file)
      unixFile.init(fileData_.file);
    else
      unixFile.init(stdin);
  }
  else
    unixFile.init(filename);

  if (! unixFile.isValid())
    return false;

  CUnixFile *file = unixFile.file();

  return readBinaryFile2D(file, plot);
}

bool
CGnuPlot::
readBinaryFile2D(CUnixFile *file, CGnuPlotPlotP &plot)
{
  // read number of columns
  float fn;

  if (! file->readData<float>(fn)) {
    errorMsg("no data in file");
    return false;
  }

  int nx = fn;

  if (nx <= 0) {
    errorMsg("no data in file");
    return false;
  }

  // read x values
  Floats xvals;

  for (int i = 0; i < nx; ++i) {
    float x;

    if (file->readData<float>(x))
      xvals.push_back(x);
  }

  if (int(xvals.size()) != nx) {
    errorMsg("data size mismatch");
    return false;
  }

  Floats              yvals;
  std::vector<Floats> data;

  while (true) {
    float y;

    if (! file->readData<float>(y))
      break;

    yvals.push_back(y);

    Floats data1;

    for (int i = 0; i < nx; ++i) {
      float d;

      if (file->readData<float>(d))
        data1.push_back(d);
    }

    if (int(data1.size()) != nx) {
      errorMsg("data size mismatch");
      return false;
    }

    data.push_back(data1);
  }

  if (data.empty()) {
    errorMsg("no data in file");
    return false;
  }

  int ny = data[0].size();

  //---

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title = "\"" + file->filename() + "\"";

    plot->setKeyTitleString(title);
  }

  //---

  for (const auto &iy : IRangeItr::range(0, ny)) {
    float y = yvals[iy];

    for (const auto &ix : IRangeItr::range(0, nx)) {
      float x = xvals[ix];
      float z = data[iy][ix];

      Reals rvals = {{x, y, z}};

      plot->addPoint2D(rvals);
    }
  }

  return true;
}

void
CGnuPlot::
addFile3D(Plots &plots, CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
          const CGnuPlotUsingCols &usingCols, CGnuPlotLineStyle &lineStyle,
          CGnuPlotFillStyle &fillStyle, CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  // gen file lines (one dimension)
  if      (filename == "+") {
    addGen1File3D(plots, group, style, usingCols, lineStyle, fillStyle, styleData, keyTitle);

    return;
  }
  // gen file lines (two dimensions)
  else if (filename == "++") {
    addGen2File3D(plots, group, style, usingCols, lineStyle, fillStyle, styleData, keyTitle);

    return;
  }

  //---

  // get block lines
  if      (filename[0] == '$') {
    std::string name = filename.substr(1);

    CGnuPlotFile::Lines lines;

    getBlockLines(name, lines);

    dataFile_.setFileName(filename);

    dataFile_.setLines(lines);
  }
  else if (filename != "-") {
    if (! readDataFile(filename, dataFile_))
      return;
  }
  // get input file lines
  else if (filename == "-") {
    dataFile_.setFileName("-");

    readDataFileLines();
  }

  //---

  if (matrixData().isMatrix()) {
    dataFile_.setSetBlankLines(1);
    dataFile_.setSubSetBlankLines(-1); // disable
  }

  dataFile_.processFile();

  if (dataFile_.numSets() == 0) {
    errorMsg("no data in file");
  }

  //---

  typedef std::map<int,CGnuPlotPlotP> SetPlots;

  SetPlots setPlots;

  // create plot per set
  for (int setNum = 0; setNum < dataFile_.numSets(); ++setNum) {
    CGnuPlotPlotP plot(createPlot(group.get(), style));

    setPlots[setNum] = plot;

    plot->setSetNum  (setNum);
    plot->setDataFile(dataFile_);
    plot->setKeyTitle(keyTitle);

    plot->setUsingCols(usingCols);

    plot->init();

    addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);
  }

  //---

  // select values for plot
  for (int setNum = 0; setNum < dataFile_.numSets(); ++setNum) {
    CGnuPlotPlotP plot = setPlots[setNum];

    setPlotValues3D(plot.get());
  }
}

void
CGnuPlot::
setPlotValues3D(CGnuPlotPlot *plot)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  plot->clearPoints();

  //---

  const CGnuPlotFile      &dataFile  = plot->dataFile();
  const CGnuPlotUsingCols &usingCols = plot->usingCols();
  const CGnuPlotKeyTitle  &keyTitle  = plot->keyTitle();

  CGnuPlotLineStyle lineStyle = plot->lineStyle();
  CGnuPlotFillStyle fillStyle = plot->fillStyle();
  CGnuPlotStyleData styleData = plot->styleData();
  CGnuPlotKeyData   keyData   = plot->keyData();

  //---

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title;

    if      (keyData.columnHead() && usingCols.numCols() > 1 && usingCols.getCol(1).isInt() &&
             usingCols.getCol(1).ival() >= 1 &&
             usingCols.getCol(1).ival() <= int(keyData.columns().size()))
      title = keyData.columns()[usingCols.getCol(1).ival() - 1];
    else if (keyData.columnNum().isValid() && keyData.columnNum().getValue() >= 1 &&
             keyData.columnNum().getValue() <= int(keyData.columns().size()))
      title = keyData.columns()[keyData.columnNum().getValue()];
    else
      title = "\"" + dataFile.fileName() + "\"";

    plot->setKeyTitleString(title);
  }

  //---

  pointNum_ = 0;

  // add fields (discontinuity per subset)
  int setNum = plot->setNum();

  setSetNum(setNum);

  int numSubSets = dataFile.numSubSets(setNum);

  for (int subSetNum = 0; subSetNum < numSubSets; ++subSetNum) {
    int lineNum = 0;

    std::string cstr;

    if (dataFile.commentStr(setNum, subSetNum, 0, cstr)) {
      CParseLine line1(cstr);

      if (line1.isString("--style--")) {
        line1.skipNonSpace();

        parseCommentStyle(line1, lineStyle, fillStyle, styleData, keyTitle);

        plot->setLineStyle(lineStyle);
        plot->setFillStyle(fillStyle);
        plot->setStyleData(styleData);
      }
    }

    // use first line for columnheaders if option enabled
    if (subSetNum == 0 && keyData.columnHead()) {
      const CGnuPlotFile::Fields &fields = dataFile.fields(setNum, subSetNum, lineNum);

      keyData.setColumns(fields);

      ++lineNum;
    }

    int lineNum1 = lineNum;

    for ( ; lineNum < dataFile.numLines(setNum, subSetNum); ++lineNum, ++pointNum_) {
      // first point of each new set is a discontinuity
      bool discontinuity = (subSetNum > 0 && lineNum == lineNum1);

      //---

      // convert fields on line to values
      // (Note: save fields as used by columnhead function)
      // (TODO: save current file setNum, subSetNum and lineNum instead)
      const CGnuPlotFile::Fields &fields = dataFile.fields(setNum, subSetNum, lineNum);

      fieldValues_.clear();

      for (const auto &field : fields) {
        CExprValuePtr value = fieldToValue(fieldValues_.size(), field);

        fieldValues_.push_back(value);
      }

      bool bad = false;

      Values values;
      Params params;

      if (matrixData().isMatrix()) {
        for (const auto &value : fieldValues_) {
          values.push_back(value);
        }
      }
      else {
        int nskip = 0;

        // no columns specified so use all columns
        if (usingCols.numCols() == 0) {
          for (const auto &value : fieldValues_) {
            if (! value.isValid())
              bad = true;

            values.push_back(value);
          }
        }
        // extract specified columns
        else {
          usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

          // one value then auto add the point and set number
          if (values.size() == 1) {
            CExprValuePtr value1 = CExprInst->createRealValue(lineNum);
            CExprValuePtr value2 = CExprInst->createRealValue(subSetNum);

            if (value1.isValid() && value2.isValid()) {
              values.push_back(value1);
              values.push_back(value2);

              std::swap(values[0], values[1]);
              std::swap(values[1], values[2]);
            }
          }
          // Note: two or four values values then last value is color
        }

        if      (nskip)
          discontinuity = true;
      //else if (bad)
      //  continue;
      }

      //---

      // add values
      if (matrixData().isMatrix()) {
        int i = 0;

        for (const auto &value : fieldValues_) {
          Values values1;

          CExprValuePtr value1 = CExprInst->createIntegerValue(i);
          CExprValuePtr value2 = CExprInst->createIntegerValue(lineNum);

          values1.push_back(value1);
          values1.push_back(value2);
          values1.push_back(value);

          plot->addPoint3D(lineNum, values1, discontinuity, bad);

          ++i;
        }
      }
      else {
        plot->addPoint3D(subSetNum, values, discontinuity, bad);
      }
    }
  }
}

void
CGnuPlot::
addGen1File3D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
              const CGnuPlotUsingCols &usingCols,
              CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
              CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  // create plot
  setSetNum(0);

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);

  //---

  initGen1File3D(plot.get(), usingCols);
}

void
CGnuPlot::
initGen1File3D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  // gen file lines (one dimension)
  int nx, ny;

  //isoSamples_.get(nx, ny);
  plot->samples().get(nx, ny);

  //---

  // get sample range
  const SampleVars &sampleVars = plot->sampleVars();

  double xmin, xmax;

  getXRange(&xmin, &xmax);

  if (sampleVars.x.min.isValid()) xmin = sampleVars.x.min.getValue();
  if (sampleVars.x.max.isValid()) xmax = sampleVars.x.max.getValue();

  //---

  std::string varName1("x"), varName2("t");

  dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ false);

  if (sampleVars.x.var != "")
    varName1 = sampleVars.x.var;

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);

  double x  = xmin;
  double dx = (xmax - xmin)/nx;

  for (int ix = 0; ix <= nx; ++ix, x += dx) {
    CExprValuePtr xval = CExprInst->createRealValue(x);

    xvar->setValue(xval);

    fieldValues_.clear();

    fieldValues_.push_back(xval);

    //---

    pointNum_ = ix;

    bool   bad;
    Values values;
    Params params;

    usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

    plot->addPoint3D(0, values, false, bad);
  }

#if 0
  for (auto &plot : plots) {
    if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
      std::string title;

      if (sampleVars.x.var != "") {
        title += "[" + sampleVars.x.var + "=";

        if (sampleVars.x.min.isValid())
          title += CStrUtil::strprintf("%g", sampleVars.x.min.getValue());
        else
          title += "*";

        title += ":";

        if (sampleVars.x.max.isValid())
          title += CStrUtil::strprintf("%g", sampleVars.x.max.getValue());
        else
          title += "*";

        title += "]";
      }

      title += " \'+\' using " + usingCols.toString();

      plot->setKeyTitleString(title);
    }
  }
#endif
}

void
CGnuPlot::
addGen2File3D(Plots &plots, CGnuPlotGroupP &group, PlotStyle style,
              const CGnuPlotUsingCols &usingCols,
              CGnuPlotLineStyle &lineStyle, CGnuPlotFillStyle &fillStyle,
              CGnuPlotStyleData &styleData, CGnuPlotKeyTitle &keyTitle)
{
  setSetNum(0);

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  addPlotWithStyle(plot, plots, lineStyle, fillStyle, styleData, keyTitle);

  //---

  initGen2File3D(plot.get(), usingCols);
}

void
CGnuPlot::
initGen2File3D(CGnuPlotPlot *plot, const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  // gen file lines (two dimensions)
  int nx, ny;

  //isoSamples_.get(nx, ny);
  plot->samples().get(nx, ny);

  //---

  double xmin, xmax;
  double ymin, ymax;

  if (isParametric()) {
    getURange(&xmin, &xmax);
    getVRange(&ymin, &ymax);
  }
  else {
    getXRange(&xmin, &xmax);
    getYRange(&ymin, &ymax);
  }

  //---

  std::string varName1, varName2;

  if (plot->isParametric()) {
    varName1 = "u";
    varName2 = "v";
  }
  else {
    varName1 = "x";
    varName2 = "t";
  }

  dummyVars().dummyVars(varName1, varName2, isParametric(), isPolar(), /*is3D*/ true);

  CExprVariablePtr xvar = CExprInst->createRealVariable(varName1, 0.0);
  CExprVariablePtr yvar = CExprInst->createRealVariable(varName2, 0.0);

  double y  = ymin;
  double dy = (ymax - ymin)/ny;

  for (int iy = 0; iy <= ny; ++iy, y += dy) {
    CExprValuePtr yval = CExprInst->createRealValue(y);

    yvar->setValue(yval);

    double x  = xmin;
    double dx = (xmax - xmin)/nx;

    for (int ix = 0; ix <= nx; ++ix, x += dx) {
      CExprValuePtr xval = CExprInst->createRealValue(x);

      xvar->setValue(xval);

      fieldValues_.clear();

      fieldValues_.push_back(xval);
      fieldValues_.push_back(yval);

      //---

      pointNum_ = ix;

      bool   bad;
      Values values;
      Params params;

      usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

      plot->addPoint3D(iy, values, false, bad);
    }
  }
}

CGnuPlotPlotP
CGnuPlot::
addImage3D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
           const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  CGnuPlotImageStyle istyle = imageStyle();

  istyle.setFilename(filename);

  istyle.setUsingCols(usingCols);

  CGnuPlotPlot::YXColor   rowColors;
  CGnuPlotPlot::RowValues rowValues;

  COptReal xmin, ymin, xmax, ymax;

  if (istyle.fileType() == CGnuPlotTypes::ImageType::PNG) {
    if (filename == "-") {
      errorMsg("no \"-\" filename support for PNG");
      return 0;
    }

    CImageFileSrc src(filename);

    CImagePtr image = CImageMgrInst->createImage(src);
    if (! image.isValid()) return 0;

    istyle.setWidth (image->getWidth ());
    istyle.setHeight(image->getHeight());

    for (auto iy : IRangeItr::range(0, istyle.height().getValue())) {
      ymin.updateMin(iy); ymax.updateMax(iy);

      for (auto ix : IRangeItr::range(0, istyle.width().getValue())) {
        CRGBA rgba;

        image->getRGBAPixel(ix, iy, rgba);

        xmin.updateMin(ix); xmax.updateMax(ix);

        rowColors[iy][ix] = rgba;
      }
    }

    istyle.setFlipY(true);
  }
  else {
    CGnuPlotUnixFile unixFile;

    if (filename == "-") {
      if (fileData_.file)
        unixFile.init(fileData_.file);
      else
        unixFile.init(stdin);
    }
    else
      unixFile.init(filename);

    if (! unixFile.isValid())
      return 0;

    CUnixFile *file = unixFile.file();

    //---

    if (binaryFormat().format() == "") {
      int iw = -1, ih = -1;

      if (istyle.width().isValid() && istyle.height().isValid()) {
        iw = istyle.width ().getValue();
        ih = istyle.height().getValue();
      }

      if (style == PlotStyle::IMAGE) {
        std::vector<uchar> chars;

        uchar c;

        while (file->readChar(c))
          chars.push_back(c);

        // how set image size ???
        int nv = chars.size();

        iw = int(sqrt(nv));
        ih = nv/iw;

        for (int i = 0; i < nv; ++i) {
          int ix = i % iw;
          int iy = i / iw;

          xmin.updateMin(ix); xmax.updateMax(ix);
          ymin.updateMin(iy); ymax.updateMax(iy);

          rowValues[iy].push_back(chars[i]);
        }
      }
      else {
        uchar              c;
        std::vector<uchar> chars;

        int n = std::numeric_limits<int>::max();

        if (istyle.width().isValid() && istyle.height().isValid())
          n = 3*istyle.width().getValue()*istyle.height().getValue();

        // read from current input source
        while (int(chars.size()) < n && file->readChar(c))
          chars.push_back(c);

        if (iw < 0 || ih < 0) {
          int nv = chars.size();

          iw = int(sqrt(nv));
          ih = nv/iw;
        }

        //---

        std::vector<uchar> rgb;

        rgb.resize(3);

        for (uint i = 0, j = 0, k = 0; i < chars.size(); ++i) {
          rgb[j++] = chars[i];

          if (j == 3) {
            int ix = k % iw;
            int iy = k / iw;

            CRGBA rgba(rgb[0]/255.0, rgb[1]/255.0, rgb[2]/255.0);

            xmin.updateMin(ix); xmax.updateMax(ix);
            ymin.updateMin(iy); ymax.updateMax(iy);

            rowColors[iy][ix] = rgba;

            j = 0;

            ++k;
          }
        }
      }
    }
    else {
      //int nc = usingCols.numCols();

      typedef std::vector<Reals> RealsArray;

      RealsArray valuesArray;

      int iw, ih;

      if (istyle.width().isValid() && istyle.height().isValid()) {
        iw = istyle.width ().getValue();
        ih = istyle.height().getValue();

        int n = iw*ih;

        for (int i = 0; i < n; ++i) {
          Reals values;

          binaryFormat_.readValues(*file, values, 3);

          valuesArray.push_back(values);
        }
      }
      else {
        // read all values
        Reals values;

        binaryFormat_.readValues(*file, values);

        int nv = values.size()/3;

        valuesArray.resize(nv);

        for (int i = 0, j = 0; i < nv; ++i, j += 3)
          valuesArray[i] = Reals({values[j], values[j + 1], values[j + 2]});

        // how set image size ???
        iw = int(sqrt(nv));
        ih = nv/iw;
      }

      //---

      for (uint i = 0; i < valuesArray.size(); ++i) {
        int ix = (i % iw);
        int iy = (i / iw);

        if (! dataFile_.every().validPointIndex(ix) || ! dataFile_.every().validBlockIndex(iy))
          continue;

        xmin.updateMin(ix); xmax.updateMax(ix);
        ymin.updateMin(iy); ymax.updateMax(iy);

        const Reals &values = valuesArray[i];

        if (usingCols.numCols() > 0) {
          Values fieldValues;

          for (const auto &v : values) {
            CExprValuePtr val = CExprInst->createRealValue(v);

            fieldValues.push_back(val);
          }

          Params params;

          bool   bad;
          Values values;

          setSetNum(0);

          pointNum_ = i;

          usingCols.decodeValues(th, setNum_, pointNum_, fieldValues, bad, values, params);

          if (style == PlotStyle::IMAGE) { // palette
            double r = 0;

            if (values.size() > 0 && values[0].isValid()) values[0]->getRealValue(r);

            rowValues[iy].push_back(r);
          }
          else {
            double r = 0, g = 0, b = 0;

            if (values.size() > 0 && values[0].isValid()) {
              values[0]->getRealValue(r);
              if (! binaryFormat_.indexFormat(0).isReal())
                r = scaleColorComponent(r);
            }

            if (values.size() > 1 && values[1].isValid()) {
              values[1]->getRealValue(g);
              if (! binaryFormat_.indexFormat(1).isReal())
                g = scaleColorComponent(g);
            }

            if (values.size() > 2 && values[2].isValid()) {
              values[2]->getRealValue(b);
              if (! binaryFormat_.indexFormat(2).isReal())
                b = scaleColorComponent(b);
            }

            CRGBA rgba(r, g, b);

            rowColors[iy][ix] = rgba;
          }
        }
        else {
          int nv = values.size();

          if (style == PlotStyle::IMAGE) { // palette
            double r = (nv > 0 ? values[0] : 0);

            rowValues[iy].push_back(r);
          }
          else {
            double r = 0, g = 0, b = 0;

            r = (nv > 0 ? values[0] : 0);
            g = (nv > 1 ? values[1] : 0);
            b = (nv > 2 ? values[2] : 0);

            CRGBA rgba(r/255.0, g/255.0, b/255.0);

            rowColors[iy][ix] = rgba;
          }
        }
      }

      if (istyle.width().isValid() && istyle.height().isValid()) {
        if (style == PlotStyle::IMAGE) {
          int nx = 0;

          for (const auto &d : rowValues)
            nx = std::max(nx, int(d.second.size()));

          for (auto &d : rowValues) {
            while (int(d.second.size()) < nx)
              d.second.push_back(0);
          }
        }
      }
    }
  }

  //---

  CGnuPlotPlotP plot(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
    std::string title = "\"" + filename + "\"";

    plot->setKeyTitleString(title);
  }

  CBBox2D ibbox(xmin.getValue(0), ymin.getValue(0), xmax.getValue(1), ymax.getValue(1));

  plot->setImageRange(ibbox);

  if (style == PlotStyle::IMAGE)
    plot->setImageValues(rowValues);
  else
    plot->setImageColors(rowColors);

  plot->setImageStyle(istyle);

  return plot;
}

CGnuPlotPlotP
CGnuPlot::
addBinary3D(CGnuPlotGroupP &group, const std::string &filename, PlotStyle style,
            const CGnuPlotUsingCols &usingCols)
{
  CGnuPlotPlotP plot;

  // matrix format
  if (binaryFormat().format() == "") {
    plot = CGnuPlotPlotP(createPlot(group.get(), PlotStyle::SURFACE));

    plot->setSetNum(0);

    plot->init();

    readBinaryFile3D(filename, plot, usingCols);
  }
  else {
    // init input file
    CGnuPlotUnixFile unixFile;

    if (filename == "-") {
      if (fileData_.file)
        unixFile.init(fileData_.file);
      else
        unixFile.init(stdin);
    }
    else
      unixFile.init(filename);

    if (! unixFile.isValid())
      return plot;

    CUnixFile *file = unixFile.file();

    plot = readBinaryFormatFile3D(file, group, style, usingCols);
  }

  //---

  return plot;
}

CGnuPlotPlotP
CGnuPlot::
readBinaryFormatFile3D(CUnixFile *file, CGnuPlotGroupP &group, PlotStyle style,
                       const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  CGnuPlotStyleBase *plotStyle = getPlotStyle(style);

  // get plots style expected number of values
  int nv = (plotStyle ? plotStyle->numUsing() : 3);

  // get max using column
  if      (usingCols.colMax().isValid())
    nv = usingCols.colMax().getValue();
  else if (binaryFormat_.numFormats() > 0)
    nv = binaryFormat_.numFormatValues();

  //---

  std::vector<Reals> realsArray;

  if (! recordValues().empty()) {
    for (const auto &n : recordValues()) {
      Reals reals;

      binaryFormat_.readValues(*file, reals, nv*n);

      realsArray.push_back(reals);
    }
  }
  else {
    Reals reals;

    binaryFormat_.readValues(*file, reals);

    realsArray.push_back(reals);
  }

  //---

  CGnuPlotPlotP plot = CGnuPlotPlotP(createPlot(group.get(), style));

  plot->setSetNum(0);

  plot->init();

  //---

  // add values
  int iy = 0;

  for (const auto &reals : realsArray) {
    int n = reals.size();

    if (n < nv)
      warnMsg("Missing values for binary 3d plot");

    for (const auto &i : IRangeItr::range(0, n, nv)) {
      fieldValues_.clear();

      for (const auto &j : IRangeItr::range(0, nv)) {
        CExprValuePtr val = CExprInst->createRealValue(reals[i + j]);

        fieldValues_.push_back(val);
      }

      pointNum_ = i/nv;

      if (dataFile_.every().validPointIndex(pointNum_) &&
          dataFile_.every().validBlockIndex(iy)) {
        Params params;

        if (usingCols.numCols()) {
          bool   bad;
          Values values;

          setSetNum(0);

          usingCols.decodeValues(th, setNum_, pointNum_, fieldValues_, bad, values, params);

          plot->addPoint3D(iy, values, false, bad, params);
        }
        else
          plot->addPoint3D(iy, fieldValues_, false, false, params);
      }
    }

    ++iy;
  }

  return plot;
}

bool
CGnuPlot::
readBinaryFile3D(const std::string &filename, CGnuPlotPlotP &plot,
                 const CGnuPlotUsingCols &usingCols)
{
  // init input file
  CGnuPlotUnixFile unixFile;

  if (filename == "-") {
    if (fileData_.file)
      unixFile.init(fileData_.file);
    else
      unixFile.init(stdin);
  }
  else
    unixFile.init(filename);

  if (! unixFile.isValid())
    return false;

  CUnixFile *file = unixFile.file();

  return readBinaryFile3D(file, plot, usingCols);
}

bool
CGnuPlot::
readBinaryFile3D(CUnixFile *file, CGnuPlotPlotP &plot, const CGnuPlotUsingCols &usingCols)
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  // matrix format
  if (recordValues_.empty()) {
    // read number of columns
    float fn;

    if (! file->readData<float>(fn)) {
      errorMsg("no data in file");
      return false;
    }

    int nx = fn;

    if (nx <= 0) {
      errorMsg("no data in file");
      return false;
    }

    // read x values
    Floats xvals;

    for (int i = 0; i < nx; ++i) {
      float x;

      if (file->readData<float>(x))
        xvals.push_back(x);
    }

    if (int(xvals.size()) != nx) {
      errorMsg("data size mismatch");
      return false;
    }

    Floats              yvals;
    std::vector<Floats> data;

    while (true) {
      float y;

      if (! file->readData<float>(y))
        break;

      yvals.push_back(y);

      Floats data1;

      for (int i = 0; i < nx; ++i) {
        float d;

        if (file->readData<float>(d))
          data1.push_back(d);
      }

      if (int(data1.size()) != nx) {
        errorMsg("data size mismatch");
        return false;
      }


      data.push_back(data1);
    }

    if (data.empty()) {
      errorMsg("no data in file");
      return false;
    }

    int ny = data[0].size();

    //---

    if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
      std::string title = "\"" + file->filename() + "\"";

      plot->setKeyTitleString(title);
    }

    //---

    int iy1 = 0;

    for (const auto &iy : IRangeItr::range(0, ny)) {
      float y = yvals[iy];

      bool found = false;

      for (const auto &ix : IRangeItr::range(0, nx)) {
        float x = xvals[ix];

        if (! dataFile_.every().validPointIndex(ix) || ! dataFile_.every().validBlockIndex(iy))
          continue;

        float z = data[iy][ix];

        plot->addPoint3D(iy1, x, y, z);

        found = true;
      }

      if (found)
        ++iy1;
    }
  }
  else {
    // read n points for each record
    std::vector<Points3D> data;

    for (const auto &n : recordValues_) {
      Points3D points;

      for (int i = 0; i < n; ++i) {
        float x, y, z;

        CPoint3D point;

        if (readEndianFloat(file, endian_, x) &&
            readEndianFloat(file, endian_, y) &&
            readEndianFloat(file, endian_, z))
          point = CPoint3D(x, y, z);

        if (usingCols.numCols() > 0) {
          Values values;

          values.push_back(CExprInst->createRealValue(point.x));
          values.push_back(CExprInst->createRealValue(point.y));
          values.push_back(CExprInst->createRealValue(point.z));

          bool   bad;
          Values values1;
          Params params;

          usingCols.decodeValues(th, 0, i, values, bad, values1, params);

          // one value then auto add the point and set number
          CPoint3D point1;

          if (values1.size() > 0) values1[0]->getRealValue(point1.x);
          if (values1.size() > 1) values1[1]->getRealValue(point1.y);
          if (values1.size() > 2) values1[2]->getRealValue(point1.z);

          point = point1;
        }

        points.push_back(point);
      }

      data.push_back(points);
    }

    //---

    if (plot->isKeyTitleEnabled() && plot->keyTitleString() == "") {
      std::string title = "\"" + file->filename() + "\"";

      plot->setKeyTitleString(title);
    }

    //---

    int iy = 0;

    for (const auto &points : data) {
      int ix = 0;

      for (const auto &p : points) {
        if (dataFile_.every().validPointIndex(ix) &&
            dataFile_.every().validBlockIndex(iy))
          plot->addPoint3D(iy, p);

        ++ix;
      }

      ++iy;
    }
  }

  return true;
}

bool
CGnuPlot::
readEndianFloat(CUnixFile *file, Endian endian, float &f)
{
  if (endian == Endian::DEFAULT || endian == Endian::SWAP) {
    if (! file->readData<float>(f))
      return false;

    if (endian == Endian::SWAP) {
      uchar *data = (uchar *) &f;

      // swap the bytes into a temporary buffer
      std::swap(data[0], data[3]);
      std::swap(data[1], data[2]);
    }
  }
  else {
    uchar buffer[sizeof(float)];

    for (size_t i = 0; i < sizeof(float); ++i) {
      if (! file->readChar(buffer[i]))
        return false;
    }

    uchar *data = (uchar *) &f;

    if (endian == Endian::LITTLE) {
      for (size_t i = 0; i < sizeof(float); ++i)
        data[i] = buffer[i];
    }
    else {
      for (size_t i = 0; i < sizeof(float); ++i)
        data[sizeof(float) - 1 - i] = buffer[i];
    }
  }

  return true;
}

int
CGnuPlot::
getColumnIndex(const std::string &str) const
{
  const CGnuPlotKeyData::Columns &columns = keyData_.columns();

  for (const auto &c : SRangeItr::ofPair(columns))
    if (c.second == str)
      return c.first;

  return -1;
}

CExprTokenStack
CGnuPlot::
compileExpression(const std::string &expr) const
{
  CExprTokenStack pstack = CExprInst->parseLine(expr);
  CExprITokenPtr  itoken = CExprInst->interpPTokenStack(pstack);

  CExprTokenStack cstack;

  if (itoken.isValid())
    cstack = CExprInst->compileIToken(itoken);
  else
    errorMsg("Eval failed: '" + expr + "'");

  return cstack;
}

bool
CGnuPlot::
timeToValue(const std::string &str, double &t)
{
  std::string fmt = timeFmt().getValue("%d/%m/%y,%H:%M");

  struct tm tm1; memset(&tm1, 0, sizeof(tm));

  char *p = strptime(str.c_str(), fmt.c_str(), &tm1);

  if (! p) return false;

  t = mktime(&tm1);

  return true;
}

CExprValuePtr
CGnuPlot::
fieldToValue(int /*nf*/, const std::string &field)
{
  CExprValuePtr value;

  auto len = field.size();

  double r;

  if      (field == dataFile_.getMissingStr())
    value = CExprValuePtr();
#if 0
  else if ((nf == 0 && xaxis(1).isTime()) ||
           (nf == 1 && yaxis(1).isTime())) {
    double r;

    if (timeToValue(field, r))
      value = CExprInst->createRealValue(r);
  }
#endif
  else if (field[0] == '\"' && field[len - 1] == '\"')
    value = CExprInst->createStringValue(field.substr(1, len - 2));
  else if (fieldToReal(field, r))
    value = CExprInst->createRealValue(r);
  else
    value = CExprInst->createStringValue(field);

  return value;
}

bool
CGnuPlot::
fieldToReal(const std::string &str, double &r)
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

  // check for utf spaces
  std::string cstr(p1);
  int         pos = 0;

  ulong c = CUtf8::readNextChar(cstr, pos);

  while (c) {
    if (! CUtf8::isSpace(c))
      return false;

    c = CUtf8::readNextChar(cstr, pos);
  }

#if 0
  // allow for extra chars after number (optional)
  // what characters allowed ?
  while (*p1 != 0 && ::isspace(*p1))
    ++p1;

  if (*p1 != '\0') return false;
#endif

  return true;
}

//------

CGnuPlotLineStyleP
CGnuPlot::
lineStyle()
{
  if (! lineStyle_.isValid())
    setLineStyle(getLineStyleInd(0));

  return lineStyle_;
}

void
CGnuPlot::
setLineStyle(CGnuPlotLineStyleP ls)
{
  lineStyle_ = ls->dup();

  lineStyle_->resetInd();
}

CGnuPlotLineStyleP
CGnuPlot::
getLineStyleInd(int ind)
{
  CGnuPlotLineStyleP lineStyle = this->lineStyle(ind);

  if (! lineStyle) {
    lineStyle = CGnuPlotLineStyleP(createLineStyle(this));

    lineStyle->setInd(ind);
    lineStyle->init  (ind);

    lineStyles_[ind] = lineStyle;
  }

  return lineStyle;
}

void
CGnuPlot::
setLineStyleInd(int ind)
{
  CGnuPlotLineStyleP lineStyle = getLineStyleInd(ind);

  setLineStyle(lineStyle);
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
clearLineStyles()
{
  lineStyles_.clear();
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

  if (! lineStyle_.isValid())
    setLineStyle(getLineStyleInd(0));

  lineStyle_->setLineType(styleIncrement_.styleInd);
}

//------

CGnuPlotLineTypeP
CGnuPlot::
getLineTypeInd(int ind)
{
  CGnuPlotLineTypeP lineType = this->lineType(ind);

  if (! lineType) {
    lineType = CGnuPlotLineTypeP(createLineType());

    lineType->setInd(ind);
    lineType->init  (ind);

    lineTypes_[ind] = lineType;
  }

  return lineType;
}

void
CGnuPlot::
resetLineTypeInd(int ind)
{
  lineTypes_.erase(ind);
}

CGnuPlotLineTypeP
CGnuPlot::
lineType(int i) const
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  if      (i == -1) {
    if (! blackLineType_.isValid()) {
      th->blackLineType_ = CGnuPlotLineTypeP(new CGnuPlotLineType);

      th->blackLineType_->setInd(-1);
      th->blackLineType_->init  (-1);
    }

    return blackLineType_;
  }
  else if (i == -2) {
    if (! nodrawLineType_.isValid()) {
      th->nodrawLineType_ = CGnuPlotLineTypeP(new CGnuPlotLineType);

      th->nodrawLineType_->setInd(-2);
      th->nodrawLineType_->init  (-2);
    }

    return nodrawLineType_;
  }
  else if (i == -3) {
    if (! bgndLineType_.isValid()) {
      th->bgndLineType_ = CGnuPlotLineTypeP(new CGnuPlotLineType);

      th->bgndLineType_->setInd(-3);
      th->bgndLineType_->init  (-3);
    }

    return bgndLineType_;
  }
  else if (i >= 0) {
    int i1 = i % 8;

    auto p = lineTypes_.find(i1);

    return (p != lineTypes_.end() ? (*p).second : CGnuPlotLineTypeP());
  }
  else
    return CGnuPlotLineTypeP();
}

//-----

int
CGnuPlot::
numWindows() const
{
  return device_->windows().size();
}

void
CGnuPlot::
drawWindows()
{
  for (const auto &window : device_->windows())
    window->draw();
}

void
CGnuPlot::
setLastFilename(const std::string &v)
{
  lastFilename_ = v;

  CExprInst->createStringVariable("GPVAL_FILENAME", lastFilename_);
}

void
CGnuPlot::
setAngleType(AngleType type)
{
  setPrefValue<AngleType>(VariableName::ANGLES, type);

  CExprInst->setDegrees(type == AngleType::DEGREES);
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

  int pos = line.pos();

  std::string arg;
  bool        add = false;

  if (! readIdentifier(line, arg)) {
    line.skipSpace();

    arg = (line.isValid() ? line.getChars(1) : "");
  }

  while (arg != "") {
    if      (arg == "axis") {
      axis.setBorderTics(false);
    }
    else if (arg == "border") {
      axis.setBorderTics(true);
    }
    else if (arg == "mirror" || arg == "nomirror") {
      axis.setMirror(arg == "mirror");
    }
    else if (arg == "in" || arg == "out") {
      axis.setInside(arg == "in");
    }
    else if (arg == "front" || arg == "back") {
      axis.setFront(arg == "front");
    }
    else if (arg == "scale") {
      double majorScale = 1.0;
      double minorScale = 0.5;

      if (line.isString("default")) {
        majorScale = 1.0;
        minorScale = 0.2;
      }
      else {
        double r;

        if (parseReal(line, r))
          majorScale = r;

        if (line.skipSpaceAndChar(',')) {
          if (parseReal(line, r))
            minorScale = r;
        }
      }

      axis.setTicScale(majorScale, minorScale);
    }
    else if (arg == "rotate") {
      std::string arg1 = readNonSpaceNonComma(line);

      if (arg1 == "by") {
        double r;

        if (parseReal(line, r))
          axis.setTicsRotate(r);
      }
    }
    else if (arg == "norotate") {
      axis.setTicsRotate(0);
    }
    else if (arg == "offset") {
      CPoint2D o(0, 0);

      if (parseOffset(line, o))
        axis.setTicOffset(o);
    }
    else if (arg == "nooffset") {
      CPoint2D o(0, 0);

      axis.setTicOffset(o);
    }
    else if (arg == "left" || arg == "right" ||  arg == "center" || arg == "autojustify") {
      axis.setTicJustify(arg != "left" ?  (arg != "right" ?  (arg != "center" ?
       CGnuPlotAxisData::Justify::AUTOJUSTIFY : CGnuPlotAxisData::Justify::CENTER) :
       CGnuPlotAxisData::Justify::RIGHT) : CGnuPlotAxisData::Justify::LEFT);
    }
    else if (arg == "add") {
      add = true; // TODO: don't clear
    }
    else if (arg == "autofreq" || arg == "auto") {
      axis.clearTicLabels();
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
        axis.setTicFont(font);
    }
    else if (arg == "enhanced" || arg == "noenhanced") {
      axis.setEnhanced(arg == "enhanced");
    }
    else if (arg == "numeric") {
      axis.setNumeric(true);
    }
    else if (arg == "timedate" || arg == "time") {
      axis.setTimeDate(true);
    }
    else if (arg == "geographic") {
      axis.setGeographic(true);
    }
    else if (arg == "rangelimited") {
      axis.setRangeLimited(true);
    }
    else if (arg == "textcolor" || arg == "tc") {
      line.skipNonSpace();

      CGnuPlotColorSpec c;

      if (parseColorSpec(line, c))
        axis.setTicColor(c);
    }
    else if (arg == "linetype" || arg == "lt") {
      int lt;

      if (parseLineType(line, lt)) {
        axis.setLineType(lt);
      }
    }
    // ({"<label>"} <pos> {<level>} {,{"<label>"}...)}
    else if (arg == "(") {
      if (! add)
        axis.clearTicLabels();

      line.setPos(line.pos() - 1);

      std::string str1;

      (void) parseRoundBracketedString(line, str1);

      CParseLine line1(str1.substr(1, str1.size() - 2));

      while (line1.isValid()) {
        line1.skipSpace();

        std::string label;

        if (! parseString(line1, label, /*msg*/"", /*conv*/false))
          label = "";

        double pos = 0.0; // pos

        if (! parseReal(line1, pos))
          pos = 0.0;

        double level = 0.0; // level (integer) 0: major, 1: minor

        if (! parseReal(line1, level))
          level = 0.0;

        if (label == "")
          label = CStrUtil::strprintf("%g", pos);

        axis.setTicLabel(pos, label, int(level));

        if (! line1.skipSpaceAndChar(','))
          break;
      }
    }
    // <incr> | <start>, <incr> {,<end>} |
    else {
      //line.ungetChar(1);
      line.setPos(pos);

      double r;

      if (! parseReal(line, r)) {
        errorMsg("Invalid axis range");
        break;
      }

      COptReal start, end, incr;

      if (line.skipSpaceAndChar(',')) {
        start = r;

        if (parseReal(line, r)) {
          incr = r;

          if (line.skipSpaceAndChar(',')) {
            if (parseReal(line, r))
              end = r;
            else {
              errorMsg("Invalid axis range");
              break;
            }
          }
        }
        else {
          errorMsg("Invalid axis range");
          break;
        }
      }
      else {
        incr = r;
      }

      if (isDebug())
        debugMsg("start="  + std::to_string(start.getValue(0)) +
                 ", incr=" + std::to_string(incr .getValue(0)) +
                 ", end=" + std::to_string(end.getValue(0)));

      axis.clearTicLabels();

      if (start.isValid()) axis.setTicStart(start.getValue());
      if (end  .isValid()) axis.setTicEnd  (end  .getValue());
      if (incr .isValid()) axis.setTicIncr (incr .getValue());
    }

    pos = line.pos();

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
  axis.resetZeroAxis();

  axis.setZeroAxisDisplayed(true);

  while (line.isValid()) {
    int pos = line.pos();

    std::string arg = readNonSpace(line);

    if      (arg == "linestyle" || arg == "ls") {
      int ls = -1;

      if (parseInteger(line, ls))
        axis.setZeroAxisLineStyle(ls);
    }
    else if (arg == "linetype" || arg == "lt") {
      int lt;

      if (parseLineType(line, lt))
        axis.setZeroAxisLineType(lt);
    }
    else if (arg == "linewidth" || arg == "lw") {
      double lw;

      if (parseReal(line, lw))
        axis.setZeroAxisLineWidth(lw);
    }
    else {
      line.setPos(pos);
      break;
    }
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

    if (line.isOneOf({"var", "variable"})) {
      arg = readNonSpaceNonComma(line);

      c.setRGBVariable();
    }
    else {
      std::string colorValueStr;

      if (parseString(line, colorValueStr, "Invalid color string")) {
        double r, g, b;
        double a = 1.0;

        if (! CRGBName::lookup(colorValueStr, &r, &g, &b)) {
          int rgba;

          if (! CStrUtil::toInteger(colorValueStr, &rgba)) {
            errorMsg("Invalid color string '" + colorValueStr + "'");
            return false;
          }

          a = ((rgba >> 24) & 0xFF)/255.0;
          r = ((rgba >> 16) & 0xFF)/255.0;
          g = ((rgba >> 8 ) & 0xFF)/255.0;
          b = ( rgba        & 0xFF)/255.0;
        }

        c.setRGB(CRGBA(r, g, b, a));
      }
    }
  }
  else if (arg == "palette" || arg == "pal") {
    int pos = line.pos();

    arg = readNonSpaceNonComma(line);

    if      (arg == "fraction" || arg == "frac") {
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

    if (! parseLineType(line, lt)) {
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

    if (getIntegerVariable(arg, ind))
      c.setIndex(ind);
    else {
      line.setPos(pos);

      if (parseInteger(line, ind))
        c.setIndex(ind);
      else
        return false;
    }
  }

  return true;
}

bool
CGnuPlot::
parseLineType(CParseLine &line, int &lt)
{
  line.skipSpace();

  if      (line.isString("black")) {
    line.skipNonSpace(); lt = -1;
  }
  else if (line.isString("nodraw")) {
    line.skipNonSpace(); lt = -2;
  }
  else if (line.isString("bgnd")) {
    line.skipNonSpace(); lt = -3;
  }
  else {
    if (! parseInteger(line, lt))
      return false;
  }

  return true;
}

bool
CGnuPlot::
parseRGBColor(CParseLine &line, CRGB &rgb, double &a)
{
  a = 1.0;

  // <color> :=  { <r> <g> <b> <a> | '<color-name>' | '#rrggbb' }
  line.skipSpace();

  if (line.isChar('"') || line.isChar('\'')) {
    std::string name;

    if (! parseString(line, name, "Invalid color string"))
      return false;

    rgb = CRGBName::toRGB(name);
  }
  else {
    double r = 0.0, g = 0.0, b = 0.0;

    if (! parseReal(line, r) || ! parseReal(line, g) || ! parseReal(line, b))
      return false;

    if (! parseReal(line, a))
      a = 1.0;

    rgb = CRGB(r, g, b);
  }

  return true;
}

bool
CGnuPlot::
parseHSVColor(CParseLine &line, CHSV &hsv, double &a)
{
  a = 1.0;

  // <color> :=  { <h> <s> <v> <a> }
  line.skipSpace();

  double h = 0.0, s = 0.0, v = 0.0;

  if (! parseReal(line, h) || ! parseReal(line, s) || ! parseReal(line, v))
    return false;

  if (! parseReal(line, a))
    a = 1.0;

  hsv = CHSV(h*360, s, v);

  return true;
}

bool
CGnuPlot::
parseDash(CParseLine &line, CGnuPlotDash &dash)
{
  line.skipSpace();

  int ind;

  if (parseInteger(line, ind)) {
    dash.setInd(ind);
    return true;
  }

  CLineDash dash1;

  if (! parseDash(line, dash1))
    return false;

  dash = CGnuPlotDash(dash1);

  return true;
}

bool
CGnuPlot::
parseDash(CParseLine &line, CLineDash &dash)
{
  if (line.isString("solid")) {
    line.skipNonSpace();
    return true;
  }

  //---

  Reals       dashes;
  std::string str;

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
  else
    return false;

  dash = CLineDash(dashes);

  return true;
}

bool
CGnuPlot::
parseAssignment(CParseLine &line)
{
  int pos = line.pos();

  std::string identifier;

  if (! readIdentifier(line, identifier)) {
    line.setPos(pos);
    return false;
  }

  if (line.skipSpaceAndChar('=')) {
    std::string expr;

    if (! skipExpression("parseAssignment", line, expr)) {
      line.setPos(pos);
      return false;
    }

    CExprValuePtr value;

    if (! evaluateExpression(expr, value)) {
      line.setPos(pos);
      return false;
    }

    if (! value.isValid()) {
      line.setPos(pos);
      return false;
    }

    CExprInst->createVariable(identifier, value);
  }
  else {
    line.setPos(pos);
    return false;
  }

  return true;
}

bool
CGnuPlot::
parseInteger(CParseLine &line, int &i) const
{
  int pos = line.pos();

  std::string expr;

  if (! skipExpression("parseInteger", line, expr))
    return false;

  if (! exprToInteger(expr, i, true)) {
    line.setPos(pos);
    return false;
  }

  return true;
}

bool
CGnuPlot::
parseReal(CParseLine &line, double &r) const
{
  int pos = line.pos();

  std::string expr;

  if (! skipExpression("parseReal", line, expr))
    return false;

  if (! exprToReal(expr, r, true)) {
    line.setPos(pos);
    return false;
  }

  return true;
}

bool
CGnuPlot::
parseString(CParseLine &line, std::string &str, const std::string &msg, bool conv) const
{
  int pos = line.pos();

  std::string expr;

  if (! skipExpression("parseString", line, expr)) {
    if (msg != "") errorMsg(msg);
    return false;
  }

  if (! exprToString(expr, str, true, conv)) {
    if (msg != "") errorMsg(msg);
    line.setPos(pos);
    return false;
  }

  // TODO: handle inline characters ??

  return true;
}

bool
CGnuPlot::
parseValue(CParseLine &line, CExprValuePtr &value, const std::string &msg) const
{
  int pos = line.pos();

  std::string expr;

  if (! skipExpression("parseString", line, expr)) {
    if (msg != "") errorMsg(msg);
    return false;
  }

  bool quiet = (msg == "");

  if (! evaluateExpression(expr, value, quiet) || ! value.isValid()) {
    if (msg != "") errorMsg(msg + ": '" + expr + "'");
    line.setPos(pos);
    return false;
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
skipExpression(const char *id, CParseLine &line, std::string &expr) const
{
  line.skipSpace();

  uint pos = line.pos();

  if (! CExprInst->skipExpression(line.str(), pos))
    return false;

  expr = line.substr(line.pos(), pos - line.pos());

  if (isDebug())
    debugMsg(std::string(id) + ": \'" + line.substr() + "\' -> \'" + expr + "\'");

  line.setPos(pos);

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
        if (line.isValid()) {
          if (line.isODigit()) {
            while (line.isODigit())
              line.skipChar();
          }
          else
            line.skipChar();
        }
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

bool
CGnuPlot::
isColumnHeader(CParseLine &line, COptInt &col)
{
  int pos = line.pos();

  std::string name = readName(line);

  bool found = false;

  for (const auto &n : {"col", "columnhead", "columnheader", "columnheaders"}) {
    if (n == name) {
      found = true;
      break;
    }
  }

  if (! found) {
    line.setPos(pos);
    return false;
  }

  line.skipSpace();

  if (line.isChar('(')) {
    line.skipChar();

    int pos1 = line.pos();

    while (line.isValid() && ! line.isChar(')'))
      line.skipChar();

    if (! line.isValid()) {
      line.setPos(pos);
      return false;
    }

    int pos2 = line.pos();

    std::string istr = line.substr(pos1, pos2 - pos1);

    line.skipChar();

    int col1;

    if (! exprToInteger(istr, col1, true)) {
      line.setPos(pos);
      return false;
    }

    col = col1;
  }

  return true;
}

// <system> <x>, <system> <y>
bool
CGnuPlot::
parsePosition(CParseLine &line, CGnuPlotPosition &pos)
{
  double x = 0, y = 0, z = 0;

  CGnuPlotTypes::CoordSys system;

  //---

  if (parseOptionValue(this, line, system)) {
    pos.setSystemX(system);

    defSystem_ = system;
  }
  else
    pos.setSystemX(defSystem_);

  if (! parseReal(line, x))
    return false;

  //---

  if (line.skipSpaceAndChar(',')) {
    if (parseOptionValue(this, line, system)) {
      pos.setSystemY(system);

      defSystem_ = system;
    }
    else
      pos.setSystemY(defSystem_);

    if (! parseReal(line, y))
      return false;

    //---

    if (line.skipSpaceAndChar(',')) {
      if (parseOptionValue(this, line, system)) {
        pos.setSystemZ(system);

        defSystem_ = system;
      }
      else
        pos.setSystemZ(defSystem_);

      if (! parseReal(line, z))
        return false;
    }
  }

  //---

  pos.setPoint(CPoint3D(x, y, z));

  return true;
}

// <system> <r>
bool
CGnuPlot::
parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v)
{
  CGnuPlotTypes::CoordSys system;

  if (parseOptionValue(this, line, system)) {
    v.setSystem(system);

    defSystem_ = system;
  }
  else
    v.setSystem(defSystem_);

  double r;

  if (! parseReal(line, r))
    return false;

  v.setValue(r);

  return true;
}

bool
CGnuPlot::
parseOffset(CParseLine &line, CPoint3D &p)
{
  double x = 0, y = 0, z = 0;

  if (! parseReal(line, x))
    return false;

  if (line.skipSpaceAndChar(',')) {
    if (! parseReal(line, y))
      return false;

    if (line.skipSpaceAndChar(',')) {
      if (! parseReal(line, z))
        return false;
    }
  }

  p = CPoint3D(x, y, z);

  return true;
}

bool
CGnuPlot::
parseOffset(CParseLine &line, CPoint2D &p)
{
  double x = 0, y = 0;

  if (! parseReal(line, x))
    return false;

  if (line.skipSpaceAndChar(',')) {
    if (! parseReal(line, y))
      return false;
  }

  p = CPoint2D(x, y);

  return true;
}

bool
CGnuPlot::
parsePoint(CParseLine &line, CPoint3D &p)
{
  int pos = line.pos();

  double x = 0.0, y = 0.0, z = 0.0;

  if (parseReal(line, x)) {
    if (line.skipSpaceAndChar(',')) {
      if (parseReal(line, y)) {
        if (line.skipSpaceAndChar(',')) {
          if (parseReal(line, z)) {
            p = CPoint3D(x, y, z);
            return true;
          }
        }
        else {
          p = CPoint3D(x, y, z);
          return true;
        }
      }
    }
  }

  line.setPos(pos);

  return false;
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

  if (parseOptionValue(this, line, system)) {
    size.setSystemX(system);

    defSystem_ = system;
  }
  else
    size.setSystemX(defSystem_);

  double x;

  if (! parseReal(line, x))
    return false;

  if (! line.skipSpaceAndChar(','))
    return false;

  if (parseOptionValue(this, line, system)) {
    size.setSystemY(system);

    defSystem_ = system;
  }
  else
    size.setSystemY(defSystem_);

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

  uint pos = line.pos();

  if (! CExprInst->skipExpression(line.str(), pos))
    return false;

  str = CStrUtil::stripSpaces(line.substr(line.pos(), pos - line.pos()));

  line.setPos(pos);

  auto p = str.find('=');

  if (p != std::string::npos && (p >= str.size() - 1 || str[p + 1] != '=')) {
    functionData.isAssign = true;

    functionData.assign = str;
  }
  else
    functionData.function = str;

  return true;
}

bool
CGnuPlot::
parseRoundBracketedString(CParseLine &line, std::string &str) const
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
    else if (line.isChar('"') || line.isChar('\'')) {
      int pos = line.pos();

      skipString(line);

      str += line.substr(pos, line.pos() - pos);

      continue;
    }

    str += line.getChar();

    if (brackets == 0)
      break;
  }

  return true;
}

bool
CGnuPlot::
skipRoundBracketedString(CParseLine &line) const
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
    else if (line.isChar('"') || line.isChar('\'')) {
      skipString(line);

      continue;
    }

    line.skipChar();

    if (brackets == 0)
      break;
  }

  return true;
}

bool
CGnuPlot::
parseSquareBracketedString(CParseLine &line, std::string &str) const
{
  if (! line.isChar('['))
    return false;

  str += line.getChar();

  int brackets = 1;

  while (line.isValid()) {
    if      (line.isChar('['))
      ++brackets;
    else if (line.isChar(']'))
      --brackets;

    str += line.getChar();

    if (brackets == 0)
      break;
  }

  return true;
}

bool
CGnuPlot::
skipSquareBracketedString(CParseLine &line) const
{
  if (! line.isChar('['))
    return false;

  line.skipChar();

  int brackets = 1;

  while (line.isValid()) {
    if      (line.isChar('['))
      ++brackets;
    else if (line.isChar(']'))
      --brackets;

    line.skipChar();

    if (brackets == 0)
      break;
  }

  return (brackets == 0);
}

bool
CGnuPlot::
readIdentifier(CParseLine &line, std::string &id) const
{
  line.skipSpace();

  if (! line.isValid())
    return false;

  int pos = line.pos();

  if (! line.skipCIdentifier()) {
    line.setPos(pos);
    return false;
  }

  id = line.substr(pos, line.pos() - pos);

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

  if (line.isValid() && line.isAlpha())
    str += line.getChar();

  while (line.isValid() && line.isAlNum())
    str += line.getChar();

  return str;
}

bool
CGnuPlot::
readDataFile(const std::string &filename, CGnuPlotFile &dataFile)
{
  // open file
  CUnixFile file(filename);

  if (! file.open())
    return false;

  CGnuPlotFile::Lines lines;

  fileReadLines(file, lines);

  dataFile.setFileName(filename);

  dataFile.setLines(lines);

  return true;
}

void
CGnuPlot::
readDataFileLines()
{
  // open file
  if (isBinary()) {
    // TODO: buffered lines ?

    std::vector<uchar> chars;

    if (fileData_.file) {
      uchar c;

      while (fileData_.file->readChar(c)) {
        chars.push_back(c);
      }
    }
    else {
      char c;

      while ((c = fgetc(stdin)) != EOF) {
        chars.push_back(c);
      }
    }
  }
  else {
    CGnuPlotFile::Lines lines;

    readFileLines(lines);

    dataFile_.setLines(lines);
  }
}

void
CGnuPlot::
readFileLines(CGnuPlotFile::Lines &lines) const
{
  bool endFlag = false;

  std::string line;

  while (fileReadLine(line)) {
    if (line == "e") {
      endFlag = true;
      break;
    }

    lines.push_back(line);
  }

  if (! endFlag) {
    initReadLine();

    readLine_->setPrompt("input data ('e' ends) > ");

    for (;;) {
      auto line = readLine_->readLine();

      if (isEOFStr(line, "e")) {
        endFlag = true;
        break;
      }

      lines.push_back(line);
    }

    readLine_->setPrompt("> ");
  }
}

void
CGnuPlot::
fileReadLines(CUnixFile &file, CGnuPlotFile::Lines &lines)
{
  std::string line;

  while (file.readLine(line))
    lines.push_back(line);
}

bool
CGnuPlot::
fileReadLine(std::string &line) const
{
  CGnuPlot *th = const_cast<CGnuPlot *>(this);

  if (! th->fileData_.bufferLines.empty()) {
    line = th->fileData_.bufferLines.front();

    th->fileData_.bufferLines.pop_front();
//std::cerr << "buffer: " << line << std::endl;
  }
  else {
    if (! th->fileData_.file || ! th->fileData_.file->readLine(line))
      return false;

    ++th->fileData_.lineNum;
//std::cerr << th->fileData_.lineNum << ":" << line << std::endl;

    CExprInst->createIntegerVariable("GPVAL_LINENO", th->fileData_.lineNum);
  }

  return true;
}

bool
CGnuPlot::
isEOFStr(const std::string &line, const std::string &eofStr) const
{
  int len1 = line  .size();
  int len2 = eofStr.size();

  if (len1 < len2) return false;

  int i = 0;

  for ( ; i < len2; ++i) {
    if (line[i] != eofStr[i])
      return false;
  }

  if (len1 == len2)
    return true;

  return isspace(line[i]);
}

void
CGnuPlot::
errorMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;

  CExprInst->createStringVariable("GPVAL_ERRMSG" , msg);
}

void
CGnuPlot::
warnMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;
}

void
CGnuPlot::
debugMsg(const std::string &msg) const
{
  std::cerr << msg << std::endl;
}

bool
CGnuPlot::
evaluateExpression(const std::string &expr, CExprValuePtr &value, bool quiet) const
{
  std::string expr1 = replaceSumInExpr(expr);

  bool oldQuiet = CExprInst->getQuiet();

  CExprInst->setQuiet(quiet);

  if (! CExprInst->evaluateExpression(expr1, value))
    value = CExprValuePtr();

  if (! value.isValid() && ! quiet)
    errorMsg("Eval failed: '" + expr1 + "'");

  CExprInst->setQuiet(oldQuiet);

  return true;
}

bool
CGnuPlot::
evaluateExpression(const std::string &expr, Values &values, bool quiet) const
{
  std::string expr1 = replaceSumInExpr(expr);

  bool oldQuiet = CExprInst->getQuiet();

  CExprInst->setQuiet(quiet);

  if (! CExprInst->evaluateExpression(expr1, values))
    errorMsg("Eval failed: '" + expr1 + "'");

  CExprInst->setQuiet(oldQuiet);

  return true;
}

std::string
CGnuPlot::
replaceSumInExpr(const std::string &expr) const
{
  int pos0 = 0, pos1 = 0, pos2 = 0, pos3 = 0;

  CParseLine line(expr);

  while (line.isValid()) {
    line.skipSpace();

    if      (line.isChar('\"') || line.isChar('\'')) {
      skipString(line);
    }
    else if (line.isString("sum")) {
      pos0 = line.pos();

      line.skipNonSpace();

      line.skipSpace();

      if (line.isChar('[')) {
        int pos11 = line.pos() + 1;

        if (skipSquareBracketedString(line)) {
          int pos21 = line.pos() - 1;

          line.skipSpace();

          pos1 = pos11;
          pos2 = pos21;
          pos3 = line.pos();

          break;
        }
      }
      else
        line.skipChar();
    }
    else
      line.skipChar();
  }

  if (pos1 <= 0)
    return expr;

  std::string loop  = line.substr(pos1, pos2 - pos1);
  std::string expr1 = line.substr(pos3);

  CParseLine line1(loop);

  line1.skipSpace();

  std::string var;

  while (line1.isValid() && ! line1.isChar('='))
    var += line1.getChar();

  if (! line1.isChar('='))
    return expr;

  line1.skipChar();
  line1.skipSpace();

  std::string start, end;

  while (line1.isValid() && ! line1.isChar(':'))
    start += line1.getChar();

  if (! line1.isChar(':'))
    return expr;

  line1.skipChar();

  while (line1.isValid())
    end += line1.getChar();

  std::stringstream ss;

  ss << line.substr(0, pos0) <<
    "sum_range(\"" << var << "\"," << start << "," << end << ",\"" << expr1 << "\")";

  return ss.str();
}

bool
CGnuPlot::
exprToInteger(const std::string &expr, int &i, bool quiet) const
{
  CExprValuePtr value;

  if (! evaluateExpression(expr, value, quiet))
    return false;

  long i1;

  if (! value.isValid() || ! value->getIntegerValue(i1))
    return false;

  i = i1;

  return true;
}

bool
CGnuPlot::
exprToReal(const std::string &expr, double &r, bool quiet) const
{
  CExprValuePtr value;

  if (! evaluateExpression(expr, value, quiet))
    return false;

  double r1;

  if (! value.isValid() || ! value->getRealValue(r1))
    return false;

  r = r1;

  return true;
}

bool
CGnuPlot::
exprToString(const std::string &expr, std::string &s, bool quiet, bool conv) const
{
  CExprValuePtr value;

  if (! evaluateExpression(expr, value, quiet))
    return false;

  std::string s1;

  if (conv) {
    if (! value.isValid() || ! value->getStringValue(s1))
      return false;
  }
  else {
    if (! value.isValid() || ! value->isStringValue())
     return false;

    (void) value->getStringValue(s1);
  }

  s = s1;

  return true;
}

void
CGnuPlot::
getBlockLines(const std::string &name, CGnuPlotFile::Lines &lines)
{
  CGnuPlotBlock *block = getBlock(name);

  const std::string &str = block->str();
  uint               len = str.size();

  std::string line;

  for (uint i = 0; i < len; ++i) {
    if (str[i] == '\n') {
      lines.push_back(line);

      line = "";
    }
    else
      line += str[i];
  }

  if (line != "")
    lines.push_back(line);
}

CGnuPlotBlock *
CGnuPlot::
getBlock(const std::string &name)
{
  auto p = blocks_.find(name);

  if (p == blocks_.end()) {
    CGnuPlotBlock *block = new CGnuPlotBlock(name);

    p = blocks_.insert(p, Blocks::value_type(name, block));
  }

  return (*p).second;
}

double
CGnuPlot::
scaleColorComponent(double z) const
{
  double cbmin = colorBox_.min().getValue(0);
  double cbmax = colorBox_.max().getValue(255);

  return CGnuPlotUtil::map(z, cbmin, cbmax, 0, 1);
}
