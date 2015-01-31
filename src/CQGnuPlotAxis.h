#ifndef CQGnuPlotAxis_H
#define CQGnuPlotAxis_H

#include <QObject>
#include <CGnuPlotAxis.h>

class CQGnuPlotGroup;

class CQGnuPlotAxis : public QObject, public CGnuPlotAxis {
  Q_OBJECT

  Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed)

 public:
  CQGnuPlotAxis(CQGnuPlotGroup *group, const std::string &id, COrientation dir,
                double start=0.0, double end=1.0);
 ~CQGnuPlotAxis();

  bool isDisplayed() const;
  void setDisplayed(bool b);

 private:
  bool displayed_;
};

#endif
