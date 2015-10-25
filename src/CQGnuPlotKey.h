#ifndef CQGnuPlotKey_H
#define CQGnuPlotKey_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotKey.h>
#include <CQGnuPlot.h>
#include <QFont>

class CQGnuPlotGroup;

class CQGnuPlotKey : public CQGnuPlotObject, public CGnuPlotKey {
  Q_OBJECT

  Q_PROPERTY(bool    displayed    READ isDisplayed  WRITE setDisplayed   )
  Q_PROPERTY(bool    drawBox      READ getDrawBox   WRITE setDrawBox     )
  Q_PROPERTY(bool    fillBox      READ getFillBox   WRITE setFillBox     )
  Q_PROPERTY(int     boxLineStyle READ boxLineStyle WRITE setBoxLineStyle)
  Q_PROPERTY(QString title        READ title        WRITE setTitle       )
  Q_PROPERTY(bool    reverse      READ isReverse    WRITE setReverse     )
  Q_PROPERTY(bool    outside      READ isOutside    WRITE setOutside     )
  Q_PROPERTY(bool    lmargin      READ isLMargin    WRITE setLMargin     )
  Q_PROPERTY(bool    rmargin      READ isRMargin    WRITE setRMargin     )
  Q_PROPERTY(bool    tmargin      READ isTMargin    WRITE setTMargin     )
  Q_PROPERTY(bool    bmargin      READ isBMargin    WRITE setBMargin     )
  Q_PROPERTY(QFont   font         READ getFont      WRITE setFont        )

  Q_PROPERTY(CQGnuPlotEnum::HAlignType halign READ halign WRITE setHAlign)
  Q_PROPERTY(CQGnuPlotEnum::VAlignType valign READ valign WRITE setVAlign)

 public:
  CQGnuPlotKey(CQGnuPlotGroup *group);
 ~CQGnuPlotKey();

  int boxLineStyle() const;

  QString title() const;
  void setTitle(const QString &s);

  CQGnuPlotEnum::HAlignType halign() const;
  void setHAlign(const CQGnuPlotEnum::HAlignType &a);

  CQGnuPlotEnum::VAlignType valign() const;
  void setVAlign(const CQGnuPlotEnum::VAlignType &a);

  QFont getFont() const;
  void setFont(const QFont &f);

  void draw(CGnuPlotRenderer *renderer) override;

  bool mousePress(const QPoint &qp);
};

#endif
