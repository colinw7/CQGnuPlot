#ifndef CQGnuPlotStroke_H
#define CQGnuPlotStroke_H

#include <QObject>
#include <CGnuPlotStroke.h>
#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotStroke : public QObject, public CGnuPlotStroke {
  Q_OBJECT

  Q_PROPERTY(bool      enabled    READ isEnabled  WRITE setEnabled   )
  Q_PROPERTY(QColor    color      READ color      WRITE setColor     )
  Q_PROPERTY(double    width      READ width      WRITE setWidth     )
  Q_PROPERTY(CLineDash lineDash   READ getDash    WRITE setDash      )
  Q_PROPERTY(double    mitreLimit READ mitreLimit WRITE setMitreLimit)

 public:
  CQGnuPlotStroke(CQGnuPlotPlot *plot);

  CQGnuPlotStroke *dup() const override;

  QColor color() const;
  void setColor(const QColor &c);

  CLineDash getDash() const;
  void setDash(const CLineDash &dash);
};

#endif
