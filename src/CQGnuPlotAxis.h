#ifndef CQGnuPlotAxis_H
#define CQGnuPlotAxis_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotAxis.h>

class CQGnuPlotGroup;

class CQGnuPlotAxis : public CQGnuPlotObject, public CGnuPlotAxis {
  Q_OBJECT

  Q_PROPERTY(bool    displayed READ isDisplayed WRITE setDisplayed)
  Q_PROPERTY(double  start     READ getStart)
  Q_PROPERTY(double  end       READ getEnd)
  Q_PROPERTY(QString label     READ getLabel    WRITE setLabel)
  Q_PROPERTY(bool    grid      READ hasGrid     WRITE setGrid)

 public:
  CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, COrientation dir,
                double start=0.0, double end=1.0);
 ~CQGnuPlotAxis();

  QString getLabel() const;
  void setLabel(const QString &str);

  void drawAxis(double pos) override;
};

#endif
