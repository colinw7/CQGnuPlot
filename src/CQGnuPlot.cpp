#include <CQGnuPlot.h>
#include <CQGnuPlotWindow.h>
#include <CQGnuPlotPlot.h>

#include <CQApp.h>
#include <CQUtil.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool debug  = false;
  bool edebug = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (argv[i][1] == 'd')
        debug = true;
      else if (argv[i][1] == 'D') {
        debug  = true;
        edebug = true;
      }
    }
    else
      files.push_back(argv[i]);
  }

  CQGnuPlot plot;

  plot.setDebug(debug);
  plot.setExprDebug(edebug);

  uint num_files = files.size();

  for (uint i = 0; i < num_files; i++)
    plot.load(files[i]);

  plot.loop();

  return 0;
}

//------

CQGnuPlot::
CQGnuPlot()
{
}

CQGnuPlot::
~CQGnuPlot()
{
}

CGnuPlotWindow *
CQGnuPlot::
createWindow()
{
  CQGnuPlotWindow *window = new CQGnuPlotWindow(this);

  window->resize(1000, 800);

  window->show();

  return window;
}

CGnuPlotPlot *
CQGnuPlot::
createPlot(CGnuPlotWindow *window)
{
  CQGnuPlotWindow *qwindow = static_cast<CQGnuPlotWindow *>(window);

  CQGnuPlotPlot *plot = new CQGnuPlotPlot(qwindow);

  return plot;
}

void
CQGnuPlot::
timeout()
{
  qApp->processEvents();
}

void
CQGnuPlot::
load(const std::string &filename)
{
  CGnuPlot::load(filename);
}
