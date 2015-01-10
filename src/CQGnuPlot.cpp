#include <CQGnuPlot.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotGroup.h>
#include <CQGnuPlotPlot.h>
#include <CQGnuPlotLineStyle.h>
#include <CQGnuPlotObject.h>

#include <CQApp.h>
#include <CQUtil.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool debug  = false;
  bool edebug = false;
  bool svg    = false;
  bool exit   = false;

  std::string ofile = "";

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      std::string arg = (argv[i][1] == '-' ? &argv[i][1] : &argv[i][1]);

      if      (arg == "d")
        debug = true;
      else if (arg == "D") {
        debug  = true;
        edebug = true;
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
      else if (arg == "x")
        exit = true;
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  plot.setDebug(debug);
  plot.setExprDebug(edebug);

  if (svg) {
    plot.setDevice("SVG");

    if (ofile != "")
      plot.setOutputFile(ofile);
  }

  for (const auto &file : files)
    plot.load(file);

  if (! exit)
    plot.loop();

  return 0;
}

CQGnuPlot::
CQGnuPlot()
{
  device_ = new CQGnuPlotDevice;

  addDevice("Qt", device_);
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
