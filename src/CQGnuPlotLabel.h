#ifndef CQGnuPlotLabel_H
#define CQGnuPlotLabel_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotLabel.h>

#include <QPointF>
#include <QFont>

class CQGnuPlotLabel : public CQGnuPlotAnnotation, public CGnuPlotLabel {
  Q_OBJECT

  Q_PROPERTY(QString               text     READ getText    WRITE setText    )
  Q_PROPERTY(CQGnuPlot::HAlignType align    READ getAlign   WRITE setAlign   )
  Q_PROPERTY(QPointF               pos      READ getPos     WRITE setPos     )
  Q_PROPERTY(QFont                 font     READ getFont    WRITE setFont    )
  Q_PROPERTY(double                angle    READ getAngle   WRITE setAngle   )
  Q_PROPERTY(QPointF               offset   READ getOffset  WRITE setOffset  )
  Q_PROPERTY(bool                  enhanced READ isEnhanced WRITE setEnhanced)

 public:
  CQGnuPlotLabel(CQGnuPlotGroup *group);

  QString getText() const;
  void setText(const QString &s);

  CQGnuPlot::HAlignType getAlign() const;
  void setAlign(const CQGnuPlot::HAlignType &a);

  QPointF getPos() const;
  void setPos(const QPointF &p);

  QFont getFont() const;
  void setFont(const QFont &f);

  QPointF getOffset() const;
  void setOffset(const QPointF &p);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
