#ifndef CQGnuPlotAxis_H
#define CQGnuPlotAxis_H

#include <CQGnuPlotObject.h>
#include <CQGnuPlot.h>
#include <CGnuPlotAxis.h>

class CQGnuPlotGroup;

class CQGnuPlotAxis : public CQGnuPlotObject, public CGnuPlotAxis {
  Q_OBJECT

  Q_PROPERTY(bool     displayed       READ isDisplayed       WRITE setDisplayed)
  Q_PROPERTY(double   start           READ getStart          WRITE setStart)
  Q_PROPERTY(double   end             READ getEnd            WRITE setEnd)
//Q_PROPERTY(CPoint3D position        READ position          WRITE setPosition)
//Q_PROPERTY(CPoint3D position1       READ position1         WRITE setPosition1)
  Q_PROPERTY(bool     logarithmic     READ isLogarithmic     WRITE setLogarithmic)
  Q_PROPERTY(int      logarithmicBase READ logarithmicBase   WRITE setLogarithmicBase)
  Q_PROPERTY(double   majorIncrement  READ getMajorIncrement WRITE setMajorIncrement)
  Q_PROPERTY(int      majorTics       READ getNumMajorTicks  WRITE setNumMajorTicks)
  Q_PROPERTY(double   minorIncrement  READ getMinorIncrement)
  Q_PROPERTY(int      minorTics       READ getNumMinorTicks  WRITE setNumMinorTicks)
  Q_PROPERTY(int      tickIncrement   READ getTickIncrement  WRITE setTickIncrement)
  Q_PROPERTY(QString  label           READ getLabel          WRITE setLabel)
  Q_PROPERTY(bool     tickInside      READ isTickInside      WRITE setTickInside)
  Q_PROPERTY(bool     tickInside1     READ isTickInside1     WRITE setTickInside1)
  Q_PROPERTY(bool     drawTickMark    READ isDrawTickMark    WRITE setDrawTickMark)
  Q_PROPERTY(bool     drawTickMark1   READ isDrawTickMark1   WRITE setDrawTickMark1)
  Q_PROPERTY(bool     drawTickLabel   READ isDrawTickLabel   WRITE setDrawTickLabel)
  Q_PROPERTY(bool     drawTickLabel1  READ isDrawTickLabel1  WRITE setDrawTickLabel1)
  Q_PROPERTY(bool     labelInside     READ isLabelInside     WRITE setLabelInside)
  Q_PROPERTY(bool     labelInside1    READ isLabelInside1    WRITE setLabelInside1)
  Q_PROPERTY(bool     drawLabel       READ isDrawLabel       WRITE setDrawLabel)
  Q_PROPERTY(bool     drawLabel1      READ isDrawLabel1      WRITE setDrawLabel1)
  Q_PROPERTY(bool     enhanced        READ isEnhanced        WRITE setEnhanced)

  Q_PROPERTY(bool     grid      READ hasGrid      WRITE setGrid)
  Q_PROPERTY(bool     gridMajor READ hasGridMajor WRITE setGridMajor)
  Q_PROPERTY(bool     gridMinor READ hasGridMinor WRITE setGridMinor)

  Q_PROPERTY(CQGnuPlotEnum::DrawLayerType gridLayer READ getGridLayer WRITE setGridLayer)

 public:
  typedef CQGnuPlotEnum::DrawLayerType DrawLayerType;

  CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, CGnuPlotAxis::Direction dir,
                double start=0.0, double end=1.0);
 ~CQGnuPlotAxis();

  QString getLabel() const;
  void setLabel(const QString &str);

  void setStart(double r);
  void setEnd(double r);

  DrawLayerType getGridLayer() const;
  void setGridLayer(const DrawLayerType &layer);

  void drawAxis(CGnuPlotRenderer *renderer, bool first) override;
};

#endif
