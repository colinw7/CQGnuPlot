#ifndef CQGnuPlotPie_H
#define CQGnuPlotPie_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPie.h>
#include <CQGnuPlot.h>

#include <QColor>

using std::string;

Q_DECLARE_METATYPE(string)

class CQGnuPlotPlot;

class CQGnuPlotPie : public CQGnuPlotObject, public CGnuPlotPie {
  Q_OBJECT

  Q_PROPERTY(QString name  READ getName  WRITE setName )
  Q_PROPERTY(QColor  color READ getColor WRITE setColor)
  Q_PROPERTY(string  name1 READ name)

 public:
  CQGnuPlotPie(CQGnuPlotPlot *plot);
 ~CQGnuPlotPie();

  QString getName() const;
  void setName(const QString &str);

  QColor getColor() const;
  void setColor(const QColor &c);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
