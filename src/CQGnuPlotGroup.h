#ifndef CQGnuPlotGroup_H
#define CQGnuPlotGroup_H

#include <CQGnuPlot.h>
#include <CGnuPlotGroup.h>
#include <CQGnuPlotObject.h>
#include <CGnuPlotTipData.h>
#include <CGnuPlotMouseEvent.h>

class CQGnuPlotWindow;
class QPainter;

class CQGnuPlotGroup : public CQGnuPlotObject, public CGnuPlotGroup {
  Q_OBJECT

  Q_PROPERTY(int    id           READ id)
  Q_PROPERTY(int    ind          READ ind             WRITE setInd)
  Q_PROPERTY(bool   is3D         READ is3D            WRITE set3D)

  Q_PROPERTY(double regionLeft   READ getRegionLeft   WRITE setRegionLeft  )
  Q_PROPERTY(double regionRight  READ getRegionRight  WRITE setRegionRight )
  Q_PROPERTY(double regionTop    READ getRegionTop    WRITE setRegionTop   )
  Q_PROPERTY(double regionBottom READ getRegionBottom WRITE setRegionBottom)

  Q_PROPERTY(double marginLeft   READ marginLeft      WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight     WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop       WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom    WRITE setMarginBottom)

  Q_PROPERTY(double rangeLeft   READ dataRangeLeft   WRITE setDataRangeLeft  )
  Q_PROPERTY(double rangeRight  READ dataRangeRight  WRITE setDataRangeRight )
  Q_PROPERTY(double rangeTop    READ dataRangeTop    WRITE setDataRangeTop   )
  Q_PROPERTY(double rangeBottom READ dataRangeBottom WRITE setDataRangeBottom)

  Q_PROPERTY(double ratio        READ getRatio        WRITE setRatio)

  Q_PROPERTY(double xmin         READ getXMin         WRITE setXMin)
  Q_PROPERTY(double xmax         READ getXMax         WRITE setXMax)
  Q_PROPERTY(double ymin         READ getYMin         WRITE setYMin)
  Q_PROPERTY(double ymax         READ getYMax         WRITE setYMax)

  Q_PROPERTY(int    borderSides  READ getBorderSides  WRITE setBorderSides)
  Q_PROPERTY(double borderWidth  READ getBorderWidth  WRITE setBorderWidth)
  Q_PROPERTY(double borderStyle  READ getBorderStyle  WRITE setBorderStyle)
  Q_PROPERTY(double borderType   READ getBorderType   WRITE setBorderType )

  Q_PROPERTY(CQGnuPlotEnum::DrawLayerType borderLayer READ getBorderLayer WRITE setBorderLayer)

  Q_PROPERTY(CQGnuPlotEnum::HistogramStyle histogramStyle
               READ histogramStyle WRITE setHistogramStyle)

  Q_PROPERTY(double histogramGap READ histogramGap WRITE setHistogramGap)

  Q_PROPERTY(bool hidden3D        READ isHidden3D        WRITE setHidden3D       )
  Q_PROPERTY(bool hiddenGrayScale READ isHiddenGrayScale WRITE setHiddenGrayScale)

 public:
  typedef CQGnuPlotEnum::DrawLayerType DrawLayerType;

 public:
  CQGnuPlotGroup(CQGnuPlotWindow *window);

 ~CQGnuPlotGroup() override;

  CQGnuPlotWindow *qwindow() const { return window_; }

  void setPainter(QPainter *p);

  double getRatio() const;
  void setRatio(double r);

  QString getXLabel() const { return CGnuPlotGroup::getXLabel().c_str(); }
  void setXLabel(const QString &s) { CGnuPlotGroup::setXLabel(s.toStdString()); }
  QString getYLabel() const { return CGnuPlotGroup::getYLabel().c_str(); }
  void setYLabel(const QString &s) { CGnuPlotGroup::setYLabel(s.toStdString()); }

  CQGnuPlotEnum::HistogramStyle histogramStyle() const;
  void setHistogramStyle(const CQGnuPlotEnum::HistogramStyle &s);

  DrawLayerType getBorderLayer() const;
  void setBorderLayer(const DrawLayerType &layer);

  void draw() override;

  void mousePress  (const CGnuPlotMouseEvent &mouseEvent) override;
  void mouseMove   (const CGnuPlotMouseEvent &mouseEvent);
  void mouseRelease(const CGnuPlotMouseEvent &mouseEvent);

  bool mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip);

  void keyPress(const CGnuPlotKeyEvent &keyEvent) override;

  void redraw();

  void moveObjects(int key);

  void pixelToWindow(const CPoint2D &p, CPoint2D &w);
  void windowToPixel(const CPoint2D &w, CPoint2D &p);

  bool inside(const CGnuPlotMouseEvent &mouseEvent) const;

 private:
  CQGnuPlotWindow *window_;
};

#endif
