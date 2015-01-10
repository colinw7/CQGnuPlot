#ifndef CQGnuPlotPlot_H
#define CQGnuPlotPlot_H

#include <CQGnuPlot.h>
#include <CGnuPlotPlot.h>

#include <QObject>
#include <QColor>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotGroup;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;

class CQGnuPlotPlot : public QObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed)

  Q_PROPERTY(double xmin READ getXMin WRITE setXMin)
  Q_PROPERTY(double ymin READ getYMin WRITE setYMin)
  Q_PROPERTY(double xmax READ getXMax WRITE setXMax)
  Q_PROPERTY(double ymax READ getYMax WRITE setYMax)

  Q_PROPERTY(CQGnuPlot::CQPlotStyle plotStyle READ plotStyle WRITE setPlotStyle)

  Q_PROPERTY(int                       lineStyleId    READ lineStyleId      WRITE setLineStyleId  )
  Q_PROPERTY(CQGnuPlot::CQFillType     fillType       READ fillType         WRITE setFillType     )
  Q_PROPERTY(CQGnuPlot::CQFillPattern  fillPattern    READ fillPattern      WRITE setFillPattern  )
  Q_PROPERTY(QColor                    lineColor      READ lineColor        WRITE setLineColor    )
  Q_PROPERTY(double                    lineWidth      READ lineWidth        WRITE setLineWidth    )
  Q_PROPERTY(CQGnuPlot::CQSymbolType   pointType      READ pointType        WRITE setPointType    )
  Q_PROPERTY(double                    pointSize      READ pointSize        WRITE setPointSize    )
  Q_PROPERTY(double                    boxWidthValue  READ getBoxWidthValue WRITE setBoxWidthValue)
  Q_PROPERTY(CQGnuPlot::CQBoxWidthType boxWidthType   READ getBoxWidthType  WRITE setBoxWidthType )

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

 public:
  CQGnuPlotPlot(CQGnuPlotGroup *group);

 ~CQGnuPlotPlot() override;

  CQGnuPlotGroup *qgroup() const { return group_; }

  CQGnuPlotWindow *qwindow() const;
  CQGnuPlotCanvas *canvas() const;

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  CQGnuPlot::CQPlotStyle plotStyle() const;
  void setPlotStyle(const CQGnuPlot::CQPlotStyle &s);

  CQGnuPlot::CQFillType fillType() const;
  void setFillType(const CQGnuPlot::CQFillType &type);

  CQGnuPlot::CQFillPattern fillPattern() const;
  void setFillPattern(const CQGnuPlot::CQFillPattern &f);

  CQGnuPlot::CQSymbolType pointType() const;
  void setPointType(const CQGnuPlot::CQSymbolType &type);

  CQGnuPlot::CQBoxWidthType getBoxWidthType() const;
  void setBoxWidthType(const CQGnuPlot::CQBoxWidthType &type);

  void draw();

  void drawBar(double x, double y, const CBBox2D &bbox, FillType fillType,
               FillPattern fillPattern, const CRGBA &fillColor, const CRGBA &lineColor);

  void mouseMove(const CPoint2D &p);

  bool mouseTip(const CPoint2D &p, CQGnuPlot::TipRect &tip);

 private:
  struct Bar {
    Bar(double x1, double y1, const CBBox2D &bbox1) :
     x(x1), y(y1), bbox(bbox1) {
    }

    double  x;
    double  y;
    CBBox2D bbox;
  };

  typedef std::vector<Bar> Bars;

  CQGnuPlotGroup*    group_;
  COptValT<CPoint2D> selectedPos_;
  Bars               bars_;
};

#endif
