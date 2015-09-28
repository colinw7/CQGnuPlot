#ifndef CQGnuPlotBubbleObject_H
#define CQGnuPlotBubbleObject_H

#include <CGnuPlotBubbleObject.h>
#include <CQGnuPlot.h>

#include <QColor>

class CQGnuPlotPlot;

class CQGnuPlotBubbleObject : public QObject, public CGnuPlotBubbleObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString name  READ getName  WRITE setName )
  Q_PROPERTY(QColor  color READ getColor WRITE setColor)

 public:
  CQGnuPlotBubbleObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotBubbleObject();

  QString getName() const;
  void setName(const QString &str);

  QColor getColor() const;
  void setColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
