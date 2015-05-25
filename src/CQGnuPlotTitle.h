#ifndef CQGnuPlotTitle_H
#define CQGnuPlotTitle_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotTitle.h>
#include <CQGnuPlot.h>
#include <QColor>
#include <QFont>

class CQGnuPlotGroup;

class CQGnuPlotTitle : public CQGnuPlotObject, public CGnuPlotTitle {
  Q_OBJECT

  Q_PROPERTY(QString text     READ text       WRITE setText    )
  Q_PROPERTY(QPointF offset   READ offset     WRITE setOffset  )
  Q_PROPERTY(QFont   font     READ getFont    WRITE setFont    )
  Q_PROPERTY(QColor  color    READ color      WRITE setColor   )
  Q_PROPERTY(bool    enhanced READ isEnhanced WRITE setEnhanced)

 public:
  CQGnuPlotTitle(CQGnuPlotGroup *group);
 ~CQGnuPlotTitle();

  QString text() const;
  void setText(const QString &s);

  QPointF offset() const;
  void setOffset(const QPointF &p);

  QColor color() const;
  void setColor(const QColor &c);

  QFont getFont() const;
  void setFont(const QFont &f);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
