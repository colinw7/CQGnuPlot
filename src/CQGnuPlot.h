#ifndef CQGnuPlot_H
#define CQGnuPlot_H

#include <CGnuPlot.h>
#include <CQGnuPlotEnum.h>

#include <QObject>
#include <QString>
#include <QRect>

class CQGnuPlotDevice;
class CQGnuPlotPNGDevice;
class CQGnuPlotRenderer;

class CQGnuPlot : public QObject, public CGnuPlot {
  Q_OBJECT

 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CQGnuPlotDevice *qdevice() const { return device_; }

  CQGnuPlotRenderer *qrenderer() const;

  const std::string &usingString() const { return usingString_; }
  void setUsingString(const std::string &v) { usingString_ = v; }

  const CGnuPlot::PlotStyle &plotStyle() const { return plotStyle_; }
  void setPlotStyle(const CGnuPlot::PlotStyle &v) { plotStyle_ = v; }

 private:
  CQGnuPlotDevice    *device_;
  CQGnuPlotPNGDevice *pngDevice_;
  std::string         usingString_;
  CGnuPlot::PlotStyle plotStyle_ { CGnuPlot::PlotStyle::LINES_POINTS };
};

#endif
