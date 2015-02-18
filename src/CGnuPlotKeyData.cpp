#include <CGnuPlotKeyData.h>
#include <CFontMgr.h>

CGnuPlotKeyData::
CGnuPlotKeyData()
{
  font_ = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, 12);
}
