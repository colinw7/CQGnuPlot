#ifndef CQGnuPlotVectorsStyleValue_H
#define CQGnuPlotVectorsStyleValue_H

#include <CGnuPlotVectorsStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotVectorsStyleValue : public QObject, public CGnuPlotVectorsStyleValue {
  Q_OBJECT

  Q_PROPERTY(bool    fhead     READ fhead     WRITE setFHead    )
  Q_PROPERTY(bool    thead     READ thead     WRITE setTHead    )
  Q_PROPERTY(double  angle     READ angle     WRITE setAngle    )
  Q_PROPERTY(double  backAngle READ backAngle WRITE setBackAngle)
  Q_PROPERTY(bool    filled    READ filled    WRITE setFilled   )
  Q_PROPERTY(bool    empty     READ empty     WRITE setEmpty    )
  Q_PROPERTY(bool    border    READ border    WRITE setBorder   )
  Q_PROPERTY(bool    front     READ front     WRITE setFront    )
  Q_PROPERTY(bool    fixed     READ fixed     WRITE setFixed    )

 public:
  CQGnuPlotVectorsStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotVectorsStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotVectorsStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  void setFHead(bool b) {
    qplot_->resetArrowCache();

    CGnuPlotVectorsStyleValue::setFHead(b);
  }

  void setTHead(bool b) {
    qplot_->resetArrowCache();

    CGnuPlotVectorsStyleValue::setTHead(b);
  }

  void setAngle(double a) {
    qplot_->resetArrowCache();

    CGnuPlotVectorsStyleValue::setAngle(a);
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
