#ifndef CQGnuPlotKey_H
#define CQGnuPlotKey_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotKey.h>
#include <CQGnuPlot.h>
#include <QFont>

class CQGnuPlotGroup;

class CQGnuPlotKey : public CQGnuPlotObject, public CGnuPlotKey {
  Q_OBJECT

  Q_PROPERTY(bool                  displayed READ isDisplayed WRITE setDisplayed)
  Q_PROPERTY(bool                  drawBox   READ getDrawBox  WRITE setDrawBox  )
  Q_PROPERTY(bool                  fillBox   READ getFillBox  WRITE setFillBox  )
  Q_PROPERTY(bool                  reverse   READ isReverse   WRITE setReverse  )
  Q_PROPERTY(bool                  outside   READ isOutside   WRITE setOutside  )
  Q_PROPERTY(CQGnuPlot::HAlignType halign    READ halign      WRITE setHAlign   )
  Q_PROPERTY(CQGnuPlot::VAlignType valign    READ valign      WRITE setVAlign   )
  Q_PROPERTY(QFont                 font      READ getFont     WRITE setFont     )

 public:
  CQGnuPlotKey(CQGnuPlotGroup *group);
 ~CQGnuPlotKey();

  CQGnuPlot::HAlignType halign() const;
  void setHAlign(const CQGnuPlot::HAlignType &a);

  CQGnuPlot::VAlignType valign() const;
  void setVAlign(const CQGnuPlot::VAlignType &a);

  QFont getFont() const;
  void setFont(const QFont &f);

  void draw(CGnuPlotRenderer *renderer) override;
};

#endif
