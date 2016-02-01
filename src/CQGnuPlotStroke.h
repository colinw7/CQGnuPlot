#ifndef CQGnuPlotStroke_H
#define CQGnuPlotStroke_H

#include <QObject>
#include <CGnuPlotStroke.h>
#include <CQGnuPlotEnum.h>
#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotStroke : public QObject, public CGnuPlotStroke {
  Q_OBJECT

  Q_PROPERTY(bool      enabled    READ isEnabled  WRITE setEnabled   )
  Q_PROPERTY(QColor    color      READ color      WRITE setColor     )
  Q_PROPERTY(double    width      READ width      WRITE setWidth     )
  Q_PROPERTY(CLineDash lineDash   READ getDash    WRITE setDash      )
  Q_PROPERTY(double    mitreLimit READ mitreLimit WRITE setMitreLimit)

  Q_PROPERTY(CQGnuPlotEnum::LineCapType  lineCap  READ getLineCap  WRITE setLineCap )
  Q_PROPERTY(CQGnuPlotEnum::LineJoinType lineJoin READ getLineJoin WRITE setLineJoin)
 public:
  CQGnuPlotStroke(CQGnuPlotPlot *plot);

  CQGnuPlotStroke *dup() const override;

  QColor color() const;
  void setColor(const QColor &c);

  CLineDash getDash() const;
  void setDash(const CLineDash &dash);

  CQGnuPlotEnum::LineCapType getLineCap() const;
  void setLineCap(const CQGnuPlotEnum::LineCapType &a);

  CQGnuPlotEnum::LineJoinType getLineJoin() const;
  void setLineJoin(const CQGnuPlotEnum::LineJoinType &a);
};

#endif
