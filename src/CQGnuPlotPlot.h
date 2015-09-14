#ifndef CQGnuPlotPlot_H
#define CQGnuPlotPlot_H

#include <CQGnuPlotEnum.h>
#include <CQGnuPlot.h>
#include <CGnuPlotPlot.h>
#include <CQGnuPlotObject.h>
#include <CGnuPlotTipData.h>

#include <QColor>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotGroup;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;
class CQGnuPlotPlotBarObjects;
class CQGnuPlotPlotPieObjects;

class CQGnuPlotPlot : public CQGnuPlotObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_PROPERTY(int  id   READ id)
  Q_PROPERTY(int  xind READ xind)
  Q_PROPERTY(int  yind READ yind)

  Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed)

  Q_PROPERTY(bool binary READ isBinary)
  Q_PROPERTY(bool matrix READ isMatrix)

  Q_PROPERTY(bool enhanced READ isEnhanced WRITE setEnhanced)

  Q_PROPERTY(double xmin READ getXMin)
  Q_PROPERTY(double ymin READ getYMin)
  Q_PROPERTY(double xmax READ getXMax)
  Q_PROPERTY(double ymax READ getYMax)

  Q_PROPERTY(CQGnuPlotEnum::PlotStyle plotStyle READ plotStyle WRITE setPlotStyle)

  Q_PROPERTY(int    lineStyleId READ lineStyleId WRITE setLineStyleId)
  Q_PROPERTY(double lineWidth   READ lineWidth   WRITE setLineWidth  )
  Q_PROPERTY(QColor lineColor   READ lineColor   WRITE setLineColor  )
  Q_PROPERTY(double pointSize   READ pointSize   WRITE setPointSize  )

  Q_PROPERTY(CQGnuPlotEnum::SymbolType pointType    READ pointType   WRITE setPointType  )
  Q_PROPERTY(CQGnuPlotEnum::FillType fillType       READ fillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern fillPattern READ fillPattern WRITE setFillPattern)

  Q_PROPERTY(double barsSize  READ barsSize  WRITE setBarsSize )
  Q_PROPERTY(bool   barsFront READ barsFront WRITE setBarsFront)

  Q_PROPERTY(double boxWidthValue READ getBoxWidthValue WRITE setBoxWidthValue)
  Q_PROPERTY(bool   boxWidthCalc  READ getBoxWidthCalc  WRITE setBoxWidthCalc )

  Q_PROPERTY(CQGnuPlotEnum::BoxWidthType boxWidthType READ getBoxWidthType WRITE setBoxWidthType)

  Q_PROPERTY(bool surfaceEnabled READ isSurfaceEnabled WRITE setSurfaceEnabled)

  Q_PROPERTY(bool contourEnabled READ isContourEnabled WRITE setContourEnabled)
  Q_PROPERTY(int  contourLevels  READ getContourLevels WRITE setContourLevels)

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

  Q_PROPERTY(double imageAngle READ imageAngle WRITE setImageAngle)

 public:
  CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style);

 ~CQGnuPlotPlot() override;

  CQGnuPlotGroup *qgroup() const { return group_; }

  CQGnuPlotWindow *qwindow() const;

  int lineStyleId() const { return CGnuPlotPlot::lineStyleId().getValue(-1); }

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  CQGnuPlotEnum::PlotStyle plotStyle() const;
  void setPlotStyle(const CQGnuPlotEnum::PlotStyle &s);

  CQGnuPlotEnum::FillType fillType() const;
  void setFillType(const CQGnuPlotEnum::FillType &type);

  CQGnuPlotEnum::FillPattern fillPattern() const;
  void setFillPattern(const CQGnuPlotEnum::FillPattern &f);

  CQGnuPlotEnum::SymbolType pointType() const;
  void setPointType(const CQGnuPlotEnum::SymbolType &type);

  CQGnuPlotEnum::BoxWidthType getBoxWidthType() const;
  void setBoxWidthType(const CQGnuPlotEnum::BoxWidthType &type);

  CQGnuPlotPlotBarObjects *barObjectsObj() const { return barObjects_; }
  CQGnuPlotPlotPieObjects *pieObjectsObj() const { return pieObjects_; }

  void draw();

  void mousePress(const CPoint2D &p, std::vector<CQGnuPlotObject *> &objects);
  void mouseMove (const CPoint2D &p);
  bool mouseTip  (const CPoint2D &p, CGnuPlotTipData &tip);

 public:
  CQGnuPlotGroup*          group_;
  COptValT<CPoint2D>       selectedPos_;
  CQGnuPlotPlotBarObjects* barObjects_;
  CQGnuPlotPlotPieObjects* pieObjects_;
};

//------

class CQGnuPlotPlotBarObjects : public QObject {
  Q_OBJECT

  Q_PROPERTY(QColor fillColor   READ fillColor WRITE setFillColor)
  Q_PROPERTY(bool   border      READ hasBorder WRITE setBorder   )
  Q_PROPERTY(QColor lineColor   READ lineColor WRITE setLineColor)

  Q_PROPERTY(CQGnuPlotEnum::FillType    fillType    READ getFillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern fillPattern READ getFillPattern WRITE setFillPattern)

 public:
  CQGnuPlotPlotBarObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

  QColor fillColor() const;
  void setFillColor(const QColor &c);

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  bool hasBorder() const;
  void setBorder(bool b);

  CQGnuPlotEnum::FillType getFillType() const;
  void setFillType(const CQGnuPlotEnum::FillType &t);

  CQGnuPlotEnum::FillPattern getFillPattern() const;
  void setFillPattern(const CQGnuPlotEnum::FillPattern &p);

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotPieObjects : public QObject {
  Q_OBJECT

  Q_PROPERTY(double innerRadius READ innerRadius WRITE setInnerRadius)
  Q_PROPERTY(double labelRadius READ labelRadius WRITE setLabelRadius)

 public:
  CQGnuPlotPlotPieObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

  double innerRadius() const;
  void   setInnerRadius(double r);

  double labelRadius() const;
  void   setLabelRadius(double r);

 private:
  CQGnuPlotPlot *plot_;
};

#endif
