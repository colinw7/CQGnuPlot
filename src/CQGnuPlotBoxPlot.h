#ifndef CQGnuPlotBoxPlot_H
#define CQGnuPlotBoxPlot_H

#include <QObject>
#include <CGnuPlotBoxPlot.h>
#include <CQGnuPlotEnum.h>

class CQGnuPlotPlot;

class CQGnuPlotBoxPlot : public QObject, public CGnuPlotBoxPlot {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::BoxType   boxType    READ boxType    WRITE setBoxType   )
  Q_PROPERTY(double                   range      READ range      WRITE setRange     )
  Q_PROPERTY(double                   fraction   READ fraction   WRITE setFraction  )
  Q_PROPERTY(bool                     outliers   READ outliers   WRITE setOutliers  )
  Q_PROPERTY(double                   separation READ separation WRITE setSeparation)
  Q_PROPERTY(int                      pointType  READ pointType  WRITE setPointType )
  Q_PROPERTY(CQGnuPlotEnum::BoxLabels boxLabels  READ boxLabels  WRITE setBoxLabels )
  Q_PROPERTY(bool                     sorted     READ sorted     WRITE setSorted    )

 public:
  CQGnuPlotBoxPlot(CQGnuPlotPlot *plot);
 ~CQGnuPlotBoxPlot();

  CQGnuPlotEnum::BoxType boxType() const;
  void setBoxType(const CQGnuPlotEnum::BoxType &v);

  double range() const { return CGnuPlotBoxPlot::range().getValue(1.5); }
  void setRange(double r);

  double fraction() const { return CGnuPlotBoxPlot::fraction().getValue(0.95); }
  void setFraction(double r);

  void setOutliers(bool b);

  double separation() const { return CGnuPlotBoxPlot::separation().getValue(1.0); }
  void setSeparation(double r);

  int pointType() const { return CGnuPlotBoxPlot::pointType().getValue(7); }
  void setPointType(int t);

  CQGnuPlotEnum::BoxLabels boxLabels() const;
  void setBoxLabels(const CQGnuPlotEnum::BoxLabels &v);

  void setSorted(bool b);
};

#endif
