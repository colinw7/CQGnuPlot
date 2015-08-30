#include <CQGnuPlot.h>
#include <CQGnuPlotDevice.h>
#include <CQGnuPlotPNGDevice.h>
#include <CQPropertyEditor.h>

#include <CQUtil.h>

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
