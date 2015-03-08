#ifndef CQGnuPlotArrow_H
#define CQGnuPlotArrow_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotArrow.h>

#include <QPointF>

class CQGnuPlotGroup;

class CQGnuPlotArrow : public CQGnuPlotAnnotation, public CGnuPlotArrow {
  Q_OBJECT

  Q_PROPERTY(QPointF from      READ getFrom        WRITE setFrom       )
  Q_PROPERTY(QPointF to        READ getTo          WRITE setTo         )
  Q_PROPERTY(bool    relative  READ getRelative    WRITE setRelative   )
  Q_PROPERTY(double  length    READ getLengthValue WRITE setLengthValue)
  Q_PROPERTY(double  angle     READ getAngle       WRITE setAngle      )
  Q_PROPERTY(double  backAngle READ getBackAngle   WRITE setBackAngle  )
  Q_PROPERTY(bool    fhead     READ getFHead       WRITE setFHead      )
  Q_PROPERTY(bool    thead     READ getTHead       WRITE setTHead      )
  Q_PROPERTY(bool    filled    READ getFilled      WRITE setFilled     )
  Q_PROPERTY(bool    empty     READ getEmpty       WRITE setEmpty      )
  Q_PROPERTY(int     lineType  READ getLineStyle   WRITE setLineStyle  )
  Q_PROPERTY(double  lineWidth READ getLineWidth   WRITE setLineWidth  )

 public:
  CQGnuPlotArrow(CQGnuPlotGroup *group);

  QPointF getFrom() const;
  void setFrom(const QPointF &p);

  QPointF getTo() const;
  void setTo(const QPointF &p);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
