#ifndef CQGnuPlotLabel_H
#define CQGnuPlotLabel_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotLabel.h>

#include <QPointF>
#include <QFont>

class CQGnuPlotLabel : public CQGnuPlotAnnotation, public CGnuPlotLabel {
  Q_OBJECT

  Q_PROPERTY(QString text   READ getText   WRITE setText  )
  Q_PROPERTY(QPointF pos    READ getPos    WRITE setPos   )
  Q_PROPERTY(QFont   font   READ getFont   WRITE setFont  )
  Q_PROPERTY(double  angle  READ getAngle  WRITE setAngle )

 public:
  CQGnuPlotLabel(CQGnuPlotGroup *group);

  QString getText() const;
  void setText(const QString &s);

  QPointF getPos() const;
  void setPos(const QPointF &p);

  QFont getFont() const;
  void setFont(const QFont &f);

  void draw() const override;
};

#endif
