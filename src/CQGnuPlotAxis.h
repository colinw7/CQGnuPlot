#ifndef CQGnuPlotAxis_H
#define CQGnuPlotAxis_H

#include <CQGnuPlotObject.h>
#include <CQGnuPlot.h>
#include <CGnuPlotAxis.h>
#include <QColor>
#include <QFont>

class CQGnuPlotGroup;

class CQGnuPlotAxis : public CQGnuPlotObject, public CGnuPlotAxis {
  Q_OBJECT

  Q_PROPERTY(bool     displayed          READ isDisplayed         WRITE setDisplayed)
  Q_PROPERTY(double   start              READ getStart)
  Q_PROPERTY(double   end                READ getEnd)
  Q_PROPERTY(double   start1             READ getStart1           WRITE setStart1)
  Q_PROPERTY(double   end1               READ getEnd1             WRITE setEnd1)
//Q_PROPERTY(CPoint3D position           READ position            WRITE setPosition)
//Q_PROPERTY(CPoint3D position1          READ position1           WRITE setPosition1)
  Q_PROPERTY(bool     log                READ isLog               WRITE setLog)
  Q_PROPERTY(int      logBase            READ logBase             WRITE setLogBase)
  Q_PROPERTY(int      majorTics          READ getNumMajorTicks    WRITE setNumMajorTicks)
  Q_PROPERTY(int      minorTics          READ getNumMinorTicks    WRITE setNumMinorTicks)
  Q_PROPERTY(double   majorIncrement     READ getMajorIncrement   WRITE setMajorIncrement)
  Q_PROPERTY(double   minorIncrement     READ getMinorIncrement)
  Q_PROPERTY(int      tickIncrement      READ getTickIncrement    WRITE setTickIncrement)
  Q_PROPERTY(double   majorScale         READ getTicMajorScale    WRITE setTicMajorScale)
  Q_PROPERTY(double   minorScale         READ getTicMinorScale    WRITE setTicMinorScale)
  Q_PROPERTY(double   ticsRotate         READ ticsRotate          WRITE setTicsRotate)
  Q_PROPERTY(double   labelRotate        READ labelRotate         WRITE setLabelRotate)
  Q_PROPERTY(bool     tickInside         READ isTickInside        WRITE setTickInside)
  Q_PROPERTY(bool     labelInside        READ isLabelInside       WRITE setLabelInside)
  Q_PROPERTY(QString  label              READ getLabel            WRITE setLabel)
  Q_PROPERTY(QString  timeFormat         READ getTimeFormat       WRITE setTimeFormat)
  Q_PROPERTY(QFont    ticFont            READ getTicFont          WRITE setTicFont)
  Q_PROPERTY(QFont    labelFont          READ getLabelFont        WRITE setLabelFont)
  Q_PROPERTY(QColor   ticColor           READ getTicColor         WRITE setTicColor)
  Q_PROPERTY(QColor   labelColor         READ getLabelColor       WRITE setLabelColor)
  Q_PROPERTY(bool     parallel           READ isParallel          WRITE setParallel)
  Q_PROPERTY(bool     zeroAxisDisplayed  READ isZeroAxisDisplayed WRITE setZeroAxisDisplayed)
  Q_PROPERTY(double   zeroAxisLineWidth  READ zeroAxisLineWidth   WRITE setZeroAxisLineWidth)
  Q_PROPERTY(bool     borderTics         READ isBorderTics        WRITE setBorderTics)
  Q_PROPERTY(bool     grid               READ hasGrid             WRITE setGrid)
  Q_PROPERTY(bool     gridMajor          READ hasGridMajor        WRITE setGridMajor)
  Q_PROPERTY(bool     gridMinor          READ hasGridMinor        WRITE setGridMinor)
  Q_PROPERTY(double   gridPolarAngle     READ gridPolarAngle      WRITE setGridPolarAngle)
  Q_PROPERTY(int      gridMajorLineStyle READ gridMajorLineStyle  WRITE setGridMajorLineStyle)
  Q_PROPERTY(int      gridMinorLineStyle READ gridMinorLineStyle  WRITE setGridMinorLineStyle)
  Q_PROPERTY(int      gridMajorLineType  READ gridMajorLineType   WRITE setGridMajorLineType)
  Q_PROPERTY(int      gridMinorLineType  READ gridMinorLineType   WRITE setGridMinorLineType)
  Q_PROPERTY(double   gridMajorLineWidth READ gridMajorLineWidth  WRITE setGridMajorLineWidth)
  Q_PROPERTY(double   gridMinorLineWidth READ gridMinorLineWidth  WRITE setGridMinorLineWidth)
  Q_PROPERTY(bool     enhanced           READ isEnhanced          WRITE setEnhanced)
  Q_PROPERTY(bool     drawTickMark       READ isDrawTickMark      WRITE setDrawTickMark)
  Q_PROPERTY(bool     mirror             READ isMirror            WRITE setMirror)
  Q_PROPERTY(bool     drawTickLabel      READ isDrawTickLabel     WRITE setDrawTickLabel)
  Q_PROPERTY(bool     drawLabel          READ isDrawLabel         WRITE setDrawLabel)
  Q_PROPERTY(bool     drawMinorTickMark  READ isDrawMinorTickMark WRITE setDrawMinorTickMark)

  Q_PROPERTY(CQGnuPlotEnum::DrawLayerType gridLayer READ getGridLayer WRITE setGridLayer)

 public:
  typedef CQGnuPlotEnum::DrawLayerType DrawLayerType;

  CQGnuPlotAxis(CQGnuPlotGroup *group, const CGnuPlotAxisData &data,
                double start=0.0, double end=1.0);
 ~CQGnuPlotAxis();

  QString getLabel() const;
  void setLabel(const QString &str);

  QString getTimeFormat() const;
  void setTimeFormat(const QString &str);

  void setStart1(double r);
  void setEnd1  (double r);

  bool isLog() const { return CGnuPlotAxis::logBase().isValid(); }

  void setLog(bool b) {
    if (b)
      CGnuPlotAxis::setLogBase(10);
    else
      CGnuPlotAxis::resetLogBase();
  }

  int logBase() const { return CGnuPlotAxis::logBase().getValue(-1); }

  void setLogBase(int b) { CGnuPlotAxis::setLogBase(b); }

  DrawLayerType getGridLayer() const;
  void setGridLayer(const DrawLayerType &layer);

  QFont getTicFont() const;
  void setTicFont(const QFont &f);

  QFont getLabelFont() const;
  void setLabelFont(const QFont &f);

  QColor getTicColor() const;
  void setTicColor(const QColor &c);

  QColor getLabelColor() const;
  void setLabelColor(const QColor &c);

  void drawAxes(CGnuPlotRenderer *renderer, bool drawOther) override;
};

#endif
