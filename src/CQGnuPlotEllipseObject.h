#ifndef CQGnuPlotEllipseObject_H
#define CQGnuPlotEllipseObject_H

#include <CGnuPlotEllipseObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotFill;
class CQGnuPlotStroke;

class CQGnuPlotEllipseObject : public QObject, public CGnuPlotEllipseObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QString text READ getText WRITE setText)

 public:
  CQGnuPlotEllipseObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotEllipseObject();

  QString getText() const;
  void setText(const QString &str);

  CQGnuPlotFill   *fill  () const;
  CQGnuPlotStroke *stroke() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
