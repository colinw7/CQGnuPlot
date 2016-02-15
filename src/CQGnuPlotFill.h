#ifndef CQGnuPlotFill_H
#define CQGnuPlotFill_H

#include <CGnuPlotFill.h>
#include <CQGnuPlotEnum.h>
#include <QObject>
#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotFill : public QObject, public CGnuPlotFill {
  Q_OBJECT

  Q_PROPERTY(QColor                     color      READ color      WRITE setColor     )
  Q_PROPERTY(QColor                     background READ background WRITE setBackground)
  Q_PROPERTY(CQGnuPlotEnum::FillType    type       READ type       WRITE setType      )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern pattern    READ pattern    WRITE setPattern   )

 public:
  CQGnuPlotFill(CQGnuPlotPlot *plot);

  CQGnuPlotFill *dup() const override;

  void setValues(const CQGnuPlotFill &fill);

  QColor color() const;
  void setColor(const QColor &c);

  QColor background() const;
  void setBackground(const QColor &c);

  CQGnuPlotEnum::FillType type() const;
  void setType(const CQGnuPlotEnum::FillType &type);

  CQGnuPlotEnum::FillPattern pattern() const;
  void setPattern(const CQGnuPlotEnum::FillPattern &f);

 signals:
  void changed();
};

#endif
