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
class CQGnuPlotStroke;
class CQGnuPlotFill;

class CQGnuPlotPlotArrowObjects;
class CQGnuPlotPlotBoxBarObjects;
class CQGnuPlotPlotBoxObjects;
class CQGnuPlotPlotBubbleObjects;
class CQGnuPlotPlotEllipseObjects;
class CQGnuPlotPlotErrorBarObjects;
class CQGnuPlotPlotFinanceBarObjects;
class CQGnuPlotPlotImageObjects;
class CQGnuPlotPlotLabelObjects;
class CQGnuPlotPlotPathObjects;
class CQGnuPlotPlotPieObjects;
class CQGnuPlotPlotPointObjects;
class CQGnuPlotPlotPolygonObjects;
class CQGnuPlotPlotRectObjects;

class CQGnuPlotBoxBarObject;
class CQGnuPlotPieObject;
class CQGnuPlotPointObject;

class QTimer;

class CQGnuPlotPlot : public CQGnuPlotObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_PROPERTY(int  id   READ id)
  Q_PROPERTY(int  xind READ xind)
  Q_PROPERTY(int  yind READ yind)

  Q_PROPERTY(int  setNum READ setNum)

  Q_PROPERTY(QString usingCols READ usingColsStr WRITE setUsingColsStr)

  Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed)

  Q_PROPERTY(CQGnuPlotEnum::PlotStyle plotStyle READ plotStyle WRITE setPlotStyle)

  Q_PROPERTY(bool binary     READ isBinary     WRITE setBinary    )
  Q_PROPERTY(bool matrix     READ isMatrix)
  Q_PROPERTY(bool polar      READ isPolar      WRITE setPolar     )
  Q_PROPERTY(bool parametric READ isParametric WRITE setParametric)

  Q_PROPERTY(bool enhanced READ isEnhanced WRITE setEnhanced)

  Q_PROPERTY(int samplesNX READ samplesNX WRITE setSampleNX)
  Q_PROPERTY(int samplesNY READ samplesNY WRITE setSampleNY)

  Q_PROPERTY(double xrangeMin READ xrangeMin WRITE setXRangeMin)
  Q_PROPERTY(double xrangeMax READ xrangeMax WRITE setXRangeMax)
  Q_PROPERTY(double yrangeMin READ yrangeMin WRITE setYRangeMin)
  Q_PROPERTY(double yrangeMax READ yrangeMax WRITE setYRangeMax)

  Q_PROPERTY(double xmin  READ getXMin)
  Q_PROPERTY(double xmax  READ getXMax)
  Q_PROPERTY(double ymin  READ getYMin)
  Q_PROPERTY(double ymax  READ getYMax)
  Q_PROPERTY(double zmin  READ getZMin)
  Q_PROPERTY(double zmax  READ getZMax)
  Q_PROPERTY(double bymin READ getBYMin)
  Q_PROPERTY(double bymax READ getBYMax)

  Q_PROPERTY(QString functions READ functions WRITE setFunctions)

  Q_PROPERTY(QString keyTitle READ keyTitle WRITE setKeyTitle)

  Q_PROPERTY(int    lineStyleId READ lineStyleId WRITE setLineStyleId)
  Q_PROPERTY(double lineWidth   READ lineWidth   WRITE setLineWidth  )
  Q_PROPERTY(QColor lineColor   READ lineColor   WRITE setLineColor  )
  Q_PROPERTY(double pointSize   READ pointSize   WRITE setPointSize  )

  Q_PROPERTY(CQGnuPlotEnum::SymbolType  pointType   READ pointType   WRITE setPointType  )
  Q_PROPERTY(CQGnuPlotEnum::FillType    fillType    READ fillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern fillPattern READ fillPattern WRITE setFillPattern)

  Q_PROPERTY(double barsSize  READ barsSize  WRITE setBarsSize )
  Q_PROPERTY(bool   barsFront READ barsFront WRITE setBarsFront)

  Q_PROPERTY(bool   surfaceEnabled READ isSurfaceEnabled WRITE setSurfaceEnabled)
  Q_PROPERTY(QColor surfaceColor   READ surfaceColor     WRITE setSurfaceColor  )

  Q_PROPERTY(bool contourEnabled READ isContourEnabled WRITE setContourEnabled)
  Q_PROPERTY(int  contourLevels  READ getContourLevels WRITE setContourLevels)

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

  Q_PROPERTY(double imageAngle READ imageAngle WRITE setImageAngle)

 public:
  typedef std::vector<QObject *> Objects;

 public:
  CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style);

 ~CQGnuPlotPlot() override;

  CQGnuPlotGroup *qgroup() const { return group_; }

  CQGnuPlotWindow *qwindow() const;

  bool isMatrix() const { return CGnuPlotPlot::matrixData().isMatrix(); }

  int lineStyleId() const { return CGnuPlotPlot::lineStyleId().getValue(-1); }

  QString usingColsStr() const;
  void setUsingColsStr(const QString &str);

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  bool isSurfaceEnabled() const;
  void setSurfaceEnabled(bool b);

  QColor surfaceColor() const;
  void setSurfaceColor(const QColor &c);

  CQGnuPlotEnum::PlotStyle plotStyle() const;
  void setPlotStyle(const CQGnuPlotEnum::PlotStyle &s);

  double xrangeMin() const;
  void setXRangeMin(double x);
  double xrangeMax() const;
  void setXRangeMax(double x);

  double yrangeMin() const;
  void setYRangeMin(double x);
  double yrangeMax() const;
  void setYRangeMax(double x);

  QString functions() const;
  void setFunctions(const QString &s);

  QString keyTitle() const;
  void setKeyTitle(const QString &s);

  CQGnuPlotEnum::FillType fillType() const;
  void setFillType(const CQGnuPlotEnum::FillType &type);

  CQGnuPlotEnum::FillPattern fillPattern() const;
  void setFillPattern(const CQGnuPlotEnum::FillPattern &f);

  CQGnuPlotEnum::SymbolType pointType() const;
  void setPointType(const CQGnuPlotEnum::SymbolType &type);

  CQGnuPlotPlotArrowObjects*      arrowObjectsObj     () { return arrowObjects_     ; }
  CQGnuPlotPlotBoxBarObjects*     boxBarObjectsObj    () { return boxBarObjects_    ; }
  CQGnuPlotPlotBoxObjects*        boxObjectsObj       () { return boxObjects_       ; }
  CQGnuPlotPlotBubbleObjects*     bubbleObjectsObj    () { return bubbleObjects_    ; }
  CQGnuPlotPlotEllipseObjects*    ellipseObjectsObj   () { return ellipseObjects_   ; }
  CQGnuPlotPlotErrorBarObjects*   errorBarObjectsObj  () { return errorBarObjects_  ; }
  CQGnuPlotPlotFinanceBarObjects* financeBarObjectsObj() { return financeBarObjects_; }
  CQGnuPlotPlotImageObjects*      imageObjectsObj     () { return imageObjects_     ; }
  CQGnuPlotPlotLabelObjects*      labelObjectsObj     () { return labelObjects_     ; }
  CQGnuPlotPlotPathObjects*       pathObjectsObj      () { return pathObjects_      ; }
  CQGnuPlotPlotPieObjects*        pieObjectsObj       () { return pieObjects_       ; }
  CQGnuPlotPlotPointObjects*      pointObjectsObj     () { return pointObjects_     ; }
  CQGnuPlotPlotPolygonObjects*    polygonObjectsObj   () { return polygonObjects_   ; }
  CQGnuPlotPlotRectObjects*       rectObjectsObj      () { return rectObjects_      ; }

  void draw() override;

  void mousePress  (const CGnuPlotMouseEvent &mouseEvent) override;
  void mouseMove   (const CGnuPlotMouseEvent &mouseEvent, bool pressed) override;
  void mouseRelease(const CGnuPlotMouseEvent &mouseEvent) override;

  bool mouseTip(const CGnuPlotMouseEvent &mouseEvent, CGnuPlotTipData &tip) override;

  void mouseObjects(const CGnuPlotMouseEvent &mouseEvent, Objects &objects);

  void moveObjects(int key);

 private:
  void initTimer();

 private slots:
  void animateSlot();

 public:
  CQGnuPlotGroup*                 group_ { 0 };
  COptValT<CPoint2D>              selectedPos_;
  CQGnuPlotPlotArrowObjects*      arrowObjects_;
  CQGnuPlotPlotBoxBarObjects*     boxBarObjects_;
  CQGnuPlotPlotBoxObjects*        boxObjects_;
  CQGnuPlotPlotBubbleObjects*     bubbleObjects_;
  CQGnuPlotPlotEllipseObjects*    ellipseObjects_;
  CQGnuPlotPlotErrorBarObjects*   errorBarObjects_;
  CQGnuPlotPlotFinanceBarObjects* financeBarObjects_;
  CQGnuPlotPlotImageObjects*      imageObjects_;
  CQGnuPlotPlotLabelObjects*      labelObjects_;
  CQGnuPlotPlotPathObjects*       pathObjects_;
  CQGnuPlotPlotPieObjects*        pieObjects_;
  CQGnuPlotPlotPointObjects*      pointObjects_;
  CQGnuPlotPlotPolygonObjects*    polygonObjects_;
  CQGnuPlotPlotRectObjects*       rectObjects_;
  QTimer*                         timer_ { 0 };
};

//----------

class CQGnuPlotPlotArrowObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotArrowObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotBoxBarObjects : public QObject {
  Q_OBJECT

  Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor)
  Q_PROPERTY(bool   border    READ hasBorder WRITE setBorder   )
  Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
  Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)

  Q_PROPERTY(CQGnuPlotEnum::FillType    fillType    READ getFillType    WRITE setFillType   )
  Q_PROPERTY(CQGnuPlotEnum::FillPattern fillPattern READ getFillPattern WRITE setFillPattern)

 public:
  CQGnuPlotPlotBoxBarObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

  QColor fillColor() const;
  void setFillColor(const QColor &c);

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  double lineWidth() const;
  void setLineWidth(double r);

  bool hasBorder() const;
  void setBorder(bool b);

  CQGnuPlotEnum::FillType getFillType() const;
  void setFillType(const CQGnuPlotEnum::FillType &t);

  CQGnuPlotEnum::FillPattern getFillPattern() const;
  void setFillPattern(const CQGnuPlotEnum::FillPattern &p);

 private:
  CQGnuPlotBoxBarObject *firstBar() const;

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotBoxObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotBoxObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotBubbleObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotBubbleObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotEllipseObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotEllipseObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotErrorBarObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotErrorBarObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotFinanceBarObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotFinanceBarObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotImageObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotImageObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotLabelObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotLabelObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotPathObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotPathObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

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
  CQGnuPlotPieObject *firstPie() const;

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotPointObjects : public QObject {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlotEnum::SymbolType pointType READ pointType WRITE setPointType)
  Q_PROPERTY(double                    size      READ size      WRITE setSize     )
  Q_PROPERTY(QColor                    color     READ color     WRITE setColor    )

 public:
  CQGnuPlotPlotPointObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CQGnuPlotEnum::SymbolType pointType() const;
  void setPointType(const CQGnuPlotEnum::SymbolType &type);

  double size() const;
  void setSize(double s);

  QColor color() const;
  void setColor(const QColor &c);

 private:
  CQGnuPlotPointObject *firstPoint() const;

 private:
  CQGnuPlotPlot *plot_;
};

//------

class CQGnuPlotPlotPolygonObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotPolygonObjects(CQGnuPlotPlot *plot);

  CQGnuPlotStroke *stroke() const { return stroke_; }
  CQGnuPlotFill   *fill  () const { return fill_  ; }

 private slots:
  void updateStroke();
  void updateFill();

 private:
  CQGnuPlotPlot   *plot_;
  CQGnuPlotStroke *stroke_;
  CQGnuPlotFill   *fill_;
};

//------

class CQGnuPlotPlotRectObjects : public QObject {
  Q_OBJECT

 public:
  CQGnuPlotPlotRectObjects(CQGnuPlotPlot *plot) :
   plot_(plot) {
  }

 private:
  CQGnuPlotPlot *plot_;
};

//------

#endif
