#ifndef CQGnuPlotArrow_H
#define CQGnuPlotArrow_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotArrow.h>

#include <QPointF>

class CQGnuPlotGroup;

class CQGnuPlotArrow : public CQGnuPlotAnnotation, public CGnuPlotArrow {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::ArrowCoordType coordType READ getCoordType WRITE setCoordType)

  Q_PROPERTY(QPointF from          READ getFrom          WRITE setFrom         )
  Q_PROPERTY(QPointF to            READ getTo            WRITE setTo           )
  Q_PROPERTY(double  length        READ getLength        WRITE setLength       )
  Q_PROPERTY(CAngle  angle         READ getAngle         WRITE setAngle        )
  Q_PROPERTY(double  headLength    READ getHeadLength    WRITE setHeadLength   )
  Q_PROPERTY(double  headAngle     READ getHeadAngle     WRITE setHeadAngle    )
  Q_PROPERTY(double  headBackAngle READ getHeadBackAngle WRITE setHeadBackAngle)
  Q_PROPERTY(bool    fhead         READ getFHead         WRITE setFHead        )
  Q_PROPERTY(bool    thead         READ getTHead         WRITE setTHead        )
  Q_PROPERTY(bool    headFilled    READ getHeadFilled    WRITE setHeadFilled   )
  Q_PROPERTY(bool    headEmpty     READ getHeadEmpty     WRITE setHeadEmpty    )
  Q_PROPERTY(int     lineType      READ getLineStyle     WRITE setLineStyle    )
  Q_PROPERTY(double  lineWidth     READ getLineWidth     WRITE setLineWidth    )
  Q_PROPERTY(int     lineDash      READ getDash          WRITE setDash         )

 public:
  CQGnuPlotArrow(CQGnuPlotGroup *group);

  CQGnuPlotEnum::ArrowCoordType getCoordType() const;
  void setCoordType(CQGnuPlotEnum::ArrowCoordType type);

  QPointF getFrom() const;
  void setFrom(const QPointF &p);

  QPointF getTo() const;
  void setTo(const QPointF &p);

  double getLength() const;
  void setLength(double l);

  double getHeadLength() const;
  void setHeadLength(double l);

  void draw(CGnuPlotRenderer *renderer) const override;

 private:
  QPointF qfrom_, qto_;
};

#endif
