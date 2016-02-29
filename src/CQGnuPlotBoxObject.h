#ifndef CQGnuPlotBoxObject_H
#define CQGnuPlotBoxObject_H

#include <CGnuPlotBoxObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;
class CQGnuPlotMark;

class CQGnuPlotBoxObject : public QObject, public CGnuPlotBoxObject {
  Q_OBJECT

  Q_PROPERTY(bool displayed   READ isDisplayed   WRITE setDisplayed  )
  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double x READ x)
  Q_PROPERTY(double y READ y)

  Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth   )
  Q_PROPERTY(double boxWidth  READ boxWidth  WRITE setBoxWidth    )

  Q_PROPERTY(bool showOutliers READ isShowOutliers WRITE setShowOutliers)

  Q_PROPERTY(CQGnuPlotEnum::BoxType   boxType   READ boxType   WRITE setBoxType  )
  Q_PROPERTY(CQGnuPlotEnum::BoxLabels boxLabels READ boxLabels WRITE setBoxLabels)

  Q_PROPERTY(double range    READ range    WRITE setRange   )
  Q_PROPERTY(double fraction READ fraction WRITE setFraction)

  Q_PROPERTY(double minValue    READ minValue)
  Q_PROPERTY(double maxValue    READ maxValue)
  Q_PROPERTY(double medianValue READ medianValue)
  Q_PROPERTY(double lowerValue  READ lowerValue)
  Q_PROPERTY(double upperValue  READ upperValue)

 public:
  CQGnuPlotBoxObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBoxObject();

  CQGnuPlotEnum::BoxType boxType();
  void setBoxType(CQGnuPlotEnum::BoxType type);

  CQGnuPlotEnum::BoxLabels boxLabels();
  void setBoxLabels(CQGnuPlotEnum::BoxLabels labels);

  CQGnuPlotFill   *fill       () const;
  CQGnuPlotStroke *stroke     () const;
  CQGnuPlotMark   *outlierMark() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
