#include <CQGnuPlot.h>
#include <CQApp.h>
#include <CQStyle.h>
#include <CBool.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  CBool               debug;
  CBool               edebug;
  CBool               fast;
  CBool               svg;
  CBool               png;
  CBool               autoContinue;
  CBool               autoExit;
  CBool               mainLoop;
  CBool               window;
  std::string         dataFile;
  std::string         function;
  std::string         ofile;
  std::string         usingStr;
  CGnuPlot::PlotStyle plotStyle = CGnuPlot::PlotStyle::LINES_POINTS;

  CGnuPlot::LoadDataParams     loadParams;
  CGnuPlot::FunctionDataParams functionParams;

  std::vector<std::string> files;
  std::vector<std::string> execs;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      std::string arg = (argv[i][1] == '-' ? &argv[i][1] : &argv[i][1]);

      if      (arg == "d")
        debug = true;
      else if (arg == "D") {
        debug  = true;
        edebug = true;
      }
      else if (arg == "e") {
        ++i;

        if (i < argc)
          execs.push_back(argv[i]);
      }
      else if (arg == "f")
        fast = true;
      else if (arg == "o") {
        ++i;

        if (i < argc)
          ofile = argv[i];
      }
      else if (arg == "svg") {
        svg   = true;
        ofile = "temp.svg";
      }
      else if (arg == "png") {
        png   = true;
        ofile = "temp_#.png";
      }
      else if (arg == "c")
        autoContinue = true;
      else if (arg == "x")
        autoExit = true;
      else if (arg == "window")
        window = true;
      else if (arg == "data") {
        ++i;

        if (i < argc)
          dataFile = argv[i];
      }
      else if (arg == "function") {
        ++i;

        if (i < argc)
          function = argv[i];
      }
      else if (arg == "csv")
        loadParams.csv = true;
      else if (arg == "header")
        loadParams.header = true;
      else if (arg == "separator") {
        ++i;

        if (i < argc) {
          arg = argv[i];

          char c = '\0';

          if      (arg == "whitespace")
            c = ' ';
          else if (arg == "tab" || arg == "\\t")
            c = '\t';
          else if (arg == "comma")
            c = ',';
          else
            c = arg[0];

          loadParams.separator = c;
        }
      }
      else if (arg == "using") {
        ++i;

        if (i < argc)
          usingStr = argv[i];
      }
      else if (arg == "style" || arg == "plot_style") {
        ++i;

        if (i < argc)
          plotStyle = CGnuPlot::stringToPlotStyle(argv[i]);
      }
      else if (arg == "qt")
        mainLoop = true;
      else if (arg == "3d")
        functionParams.is3D = true;
      else if (arg == "xmin") {
        ++i;

        if (i < argc)
          functionParams.xmin = CStrUtil::toReal(argv[i]);
      }
      else if (arg == "xmax") {
        ++i;

        if (i < argc)
          functionParams.xmax = CStrUtil::toReal(argv[i]);
      }
      else if (arg == "ymin") {
        ++i;

        if (i < argc)
          functionParams.ymin = CStrUtil::toReal(argv[i]);
      }
      else if (arg == "ymax") {
        ++i;

        if (i < argc)
          functionParams.ymax = CStrUtil::toReal(argv[i]);
      }
      else
        std::cerr << "Invalid arg '" << arg << "'" << std::endl;
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  if (! fast)
    plot.loadStatup();

  plot.setDebug(debug);
  plot.setExprDebug(edebug);
  plot.setAutoContinue(autoContinue);

  plot.setUsingString(usingStr);
  plot.setPlotStyle  (plotStyle);

  if      (svg) {
    plot.setDevice("SVG");

    if (ofile != "")
      plot.setOutputFile(ofile);
  }
  else if (png) {
    plot.setDevice("PNG");

    if (ofile != "")
      plot.setOutputFile(ofile);
  }

  for (const auto &exec : execs)
    plot.exec(exec);

  for (const auto &file : files)
    plot.load(file);

  if (window && ! plot.numWindows())
    plot.createNewWindow();

  if (dataFile != "")
    plot.loadData(dataFile, loadParams);

  if (function != "")
    plot.loadFunction(function, functionParams);

  if      (mainLoop)
    app.exec();
  else if (! autoExit)
    plot.loop();

  return 0;
}
