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
  struct TipRect {
    QString str;
    QRectF  rect;
  };

 public:
  CQGnuPlot();
 ~CQGnuPlot() override;

  CQGnuPlotDevice *qdevice() const { return device_; }

  CQGnuPlotRenderer *qrenderer() const;

 private:
  CQGnuPlotDevice    *device_;
  CQGnuPlotPNGDevice *pngDevice_;
};

#endif
