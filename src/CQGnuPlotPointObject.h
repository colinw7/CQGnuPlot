#ifndef CQGnuPlotPointObject_H
#define CQGnuPlotPointObject_H

#include <CGnuPlotPointObject.h>
#include <CQGnuPlot.h>

#include <QColor>

using std::string;

class CQGnuPlotPlot;

class CQGnuPlotPointObject : public QObject, public CGnuPlotPointObject {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(QPointF                   point       READ getPoint       WRITE setPoint      )
  Q_PROPERTY(CQGnuPlotEnum::SymbolType pointType   READ pointType      WRITE setPointType  )
  Q_PROPERTY(double                    size        READ getSize        WRITE setSize       )
  Q_PROPERTY(QColor                    color       READ getColor       WRITE setColor      )
  Q_PROPERTY(double                    lineWidth   READ lineWidth      WRITE setLineWidth  )
  Q_PROPERTY(QString                   pointString READ getPointString WRITE setPointString)
  Q_PROPERTY(bool                      erasePoint  READ isErasePoint   WRITE setErasePoint )
  Q_PROPERTY(bool                      visible     READ isVisible      WRITE setVisible    )

  Q_PROPERTY(CQGnuPlotEnum::SymbolType pointType READ pointType WRITE setPointType)

 public:
  CQGnuPlotPointObject(CQGnuPlotPlot *plot);
 ~CQGnuPlotPointObject();

  QPointF getPoint() const;
  void setPoint(const QPointF &p);

  CQGnuPlotEnum::SymbolType pointType() const;
  void setPointType(const CQGnuPlotEnum::SymbolType &type);

  double getSize() const;
  void setSize(double s);

  QColor getColor() const;
  void setColor(const QColor &c);

  QString getPointString() const;
  void setPointString(const QString &str);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
