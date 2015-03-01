#ifndef CQGnuPlotColorBox_H
#define CQGnuPlotColorBox_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotColorBox.h>
#include <CQGnuPlot.h>
#include <QFont>

class CQGnuPlotGroup;

class CQGnuPlotColorBox : public CQGnuPlotObject, public CGnuPlotColorBox {
  Q_OBJECT

  Q_PROPERTY(bool    enabled     READ isEnabled   WRITE setEnabled    )
  Q_PROPERTY(bool    vertical    READ isVertical  WRITE setVertical   )
  Q_PROPERTY(bool    user        READ isUser      WRITE setUser       )
  Q_PROPERTY(bool    front       READ isFront     WRITE setFront      )
  Q_PROPERTY(bool    border      READ hasBorder   WRITE setBorder     )
  Q_PROPERTY(int     borderStyle READ borderStyle WRITE setBorderStyle)
  Q_PROPERTY(QPointF origin      READ origin      WRITE setOrigin     )
  Q_PROPERTY(QSizeF  size        READ size        WRITE setSize       )

 public:
  CQGnuPlotColorBox(CQGnuPlotGroup *group);
 ~CQGnuPlotColorBox();

  QPointF origin() const;
  void setOrigin(const QPointF &p);

  QSizeF size() const;
  void setSize(const QSizeF &s);

  void draw() override;
};

#endif
