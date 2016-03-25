#ifndef CQGnuPlotLabelsStyleValue_H
#define CQGnuPlotLabelsStyleValue_H

#include <CGnuPlotLabelsStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotLabelsStyleValue : public QObject, public CGnuPlotLabelsStyleValue {
  Q_OBJECT

  Q_PROPERTY(bool   boxed       READ isBoxed       WRITE setBoxed      )
  Q_PROPERTY(bool   transparent READ isTransparent WRITE setTransparent)
  Q_PROPERTY(bool   showPoint   READ showPoint     WRITE setShowPoint  )
  Q_PROPERTY(int    pointType   READ pointType     WRITE setPointType  )
  Q_PROPERTY(int    pointSize   READ pointSize     WRITE setPointSize  )
  Q_PROPERTY(double lineWidth   READ lineWidth     WRITE setLineWidth  )

 public:
  CQGnuPlotLabelsStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotLabelsStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotLabelsStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  void setBoxed      (bool   b) { resetCache(); CGnuPlotLabelsStyleValue::setBoxed      (b); }
  void setTransparent(bool   b) { resetCache(); CGnuPlotLabelsStyleValue::setTransparent(b); }
  void setShowPoint  (bool   b) { resetCache(); CGnuPlotLabelsStyleValue::setShowPoint  (b); }
  void setPointType  (int    i) { resetCache(); CGnuPlotLabelsStyleValue::setPointType  (i); }
  void setPointSize  (int    i) { resetCache(); CGnuPlotLabelsStyleValue::setPointSize  (i); }
  void setLineWidth  (double r) { resetCache(); CGnuPlotLabelsStyleValue::setLineWidth  (r); }

 private:
  void resetCache() {
    qplot_->resetLabelCache();
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
