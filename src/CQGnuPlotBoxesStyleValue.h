#ifndef CQGnuPlotBoxesStyleValue_H
#define CQGnuPlotBoxesStyleValue_H

#include <CGnuPlotBoxesStyleValue.h>
#include <CQGnuPlotPlot.h>
#include <QObject>

class CQGnuPlotPlot;

class CQGnuPlotBoxesStyleValue : public QObject, public CGnuPlotBoxesStyleValue {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::BoxWidthType type      READ type   WRITE setType)
  Q_PROPERTY(double                      width     READ width  WRITE setWidth)
  Q_PROPERTY(bool                        calc      READ isCalc WRITE setCalc)
  Q_PROPERTY(double                      spacing   READ getSpacing)
  Q_PROPERTY(bool                        autoWidth READ isAutoWidth)

 public:
  CQGnuPlotBoxesStyleValue(CQGnuPlotPlot *qplot) :
   CGnuPlotBoxesStyleValue(qplot), qplot_(qplot) {
  }

 ~CQGnuPlotBoxesStyleValue() { }

  CQGnuPlotPlot *qplot() const { return qplot_; }

  CQGnuPlotEnum::BoxWidthType type() const {
    return CQGnuPlotEnum::boxWidthTypeConv(CGnuPlotBoxesStyleValue::type());
  }

  void setType(const CQGnuPlotEnum::BoxWidthType &type) {
    if (type != this->type()) {
      CGnuPlotBoxesStyleValue::setType(CQGnuPlotEnum::boxWidthTypeConv(type));
    }
  }

 private:
  CQGnuPlotPlot *qplot_;
};

#endif
