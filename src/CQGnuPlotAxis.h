#ifndef CQGnuPlotAxis_H
#define CQGnuPlotAxis_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotAxis.h>

class CQGnuPlotGroup;

class CQGnuPlotAxis : public CQGnuPlotObject, public CGnuPlotAxis {
  Q_OBJECT

  Q_PROPERTY(bool    displayed       READ isDisplayed       WRITE setDisplayed)
  Q_PROPERTY(double  start           READ getStart          WRITE setStart)
  Q_PROPERTY(double  end             READ getEnd            WRITE setEnd)
  Q_PROPERTY(bool    logarithmic     READ isLogarithmic     WRITE setLogarithmic)
  Q_PROPERTY(int     logarithmicBase READ logarithmicBase   WRITE setLogarithmicBase)
  Q_PROPERTY(double  majorIncrement  READ getMajorIncrement WRITE setMajorIncrement)
  Q_PROPERTY(int     majorTics       READ getNumMajorTicks  WRITE setNumMajorTicks)
  Q_PROPERTY(double  minorIncrement  READ getMinorIncrement)
  Q_PROPERTY(int     minorTics       READ getNumMinorTicks  WRITE setNumMinorTicks)
  Q_PROPERTY(int     tickIncrement   READ getTickIncrement  WRITE setTickIncrement)
  Q_PROPERTY(QString label           READ getLabel          WRITE setLabel)
  Q_PROPERTY(bool    grid            READ hasGrid           WRITE setGrid)
  Q_PROPERTY(bool    tickInside      READ isTickInside      WRITE setTickInside)

 public:
  CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, COrientation dir,
                double start=0.0, double end=1.0);
 ~CQGnuPlotAxis();

  QString getLabel() const;
  void setLabel(const QString &str);

  void setStart(double r);
  void setEnd(double r);

  void drawAxis(double pos) override;
};

#endif
