#include <CQGnuPlot.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotPNGDevice.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotAnnotation.h>
#include <CQPropertyEditor.h>

#include <CQApp.h>
#include <CQUtil.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool debug        = false;
  bool edebug       = false;
  bool svg          = false;
  bool png          = false;
  bool autoContinue = false;
  bool autoExit     = false;
  bool mainLoop     = false;

  std::string ofile = "";

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
      else if (arg == "qt")
        mainLoop = true;
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  plot.setDebug(debug);
  plot.setExprDebug(edebug);
  plot.setAutoContinue(autoContinue);

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

  if      (mainLoop)
    app.exec();
  else if (! autoExit)
    plot.loop();

  return 0;
}

CQGnuPlot::
CQGnuPlot()
{
  CQUtil::initProperties();

  CQPropertyEditorMgrInst->setEditor("double", new CQPropertyRealEditor);

  device_ = new CQGnuPlotDevice;

  addDevice("Qt", device_);

  pngDevice_ = new CQGnuPlotPNGDevice;

  addDevice("PNG", pngDevice_);

  setDevice("Qt");
}

CQGnuPlot::
~CQGnuPlot()
{
}

CQGnuPlotRenderer *
CQGnuPlot::
qrenderer() const
{
  return qdevice()->qrenderer();
}
