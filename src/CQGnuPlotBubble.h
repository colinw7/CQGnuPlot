#ifndef CQGnuPlotBubble_H
#define CQGnuPlotBubble_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotBubble.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotBubble : public CQGnuPlotObject, public CGnuPlotBubble {
  Q_OBJECT

  Q_PROPERTY(QString name  READ getName  WRITE setName )
  Q_PROPERTY(QColor  color READ getColor WRITE setColor)

 public:
  CQGnuPlotBubble(CQGnuPlotPlot *plot);
 ~CQGnuPlotBubble();

  QString getName() const;
  void setName(const QString &str);

  QColor getColor() const;
  void setColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
