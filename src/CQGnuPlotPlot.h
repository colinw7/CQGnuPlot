#ifndef CQGnuPlotPlot_H
#define CQGnuPlotPlot_H

#include <CQGnuPlot.h>
#include <CGnuPlotPlot.h>
#include <CQGnuPlotObject.h>

#include <QColor>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotGroup;
class CQGnuPlotCanvas;
class CQGnuPlotRenderer;

class CQGnuPlotPlot : public CQGnuPlotObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_PROPERTY(int  id   READ id)
  Q_PROPERTY(int  xind READ xind)
  Q_PROPERTY(int  yind READ yind)

  Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed)

  Q_PROPERTY(bool binary READ isBinary)
  Q_PROPERTY(bool matrix READ isMatrix)

  Q_PROPERTY(double xmin READ getXMin)
  Q_PROPERTY(double ymin READ getYMin)
  Q_PROPERTY(double xmax READ getXMax)
  Q_PROPERTY(double ymax READ getYMax)

  Q_PROPERTY(CQGnuPlot::PlotStyle plotStyle READ plotStyle WRITE setPlotStyle)

  Q_PROPERTY(int                     lineStyleId    READ lineStyleId      WRITE setLineStyleId  )
  Q_PROPERTY(CQGnuPlot::FillType     fillType       READ fillType         WRITE setFillType     )
  Q_PROPERTY(CQGnuPlot::FillPattern  fillPattern    READ fillPattern      WRITE setFillPattern  )
  Q_PROPERTY(QColor                  lineColor      READ lineColor        WRITE setLineColor    )
  Q_PROPERTY(double                  lineWidth      READ lineWidth        WRITE setLineWidth    )
  Q_PROPERTY(CQGnuPlot::SymbolType   pointType      READ pointType        WRITE setPointType    )
  Q_PROPERTY(double                  pointSize      READ pointSize        WRITE setPointSize    )

  Q_PROPERTY(double barsSize  READ barsSize  WRITE setBarsSize )
  Q_PROPERTY(bool   barsFront READ barsFront WRITE setBarsFront)

  Q_PROPERTY(double                  boxWidthValue  READ getBoxWidthValue WRITE setBoxWidthValue)
  Q_PROPERTY(CQGnuPlot::BoxWidthType boxWidthType   READ getBoxWidthType  WRITE setBoxWidthType )
  Q_PROPERTY(bool                    boxWidthCalc   READ getBoxWidthCalc  WRITE setBoxWidthCalc )

  Q_PROPERTY(bool surfaceEnabled READ isSurfaceEnabled WRITE setSurfaceEnabled)

  Q_PROPERTY(bool contourEnabled READ isContourEnabled WRITE setContourEnabled)
  Q_PROPERTY(int  contourLevels  READ getContourLevels WRITE setContourLevels)

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

  Q_PROPERTY(double imageAngle READ imageAngle WRITE setImageAngle)

  Q_PROPERTY(bool syncBars READ syncBars WRITE setSyncBars)

 public:
  CQGnuPlotPlot(CQGnuPlotGroup *group, CGnuPlotTypes::PlotStyle style);

 ~CQGnuPlotPlot() override;

  CQGnuPlotGroup *qgroup() const { return group_; }

  CQGnuPlotWindow *qwindow() const;
  CQGnuPlotCanvas *canvas() const;

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  CQGnuPlot::PlotStyle plotStyle() const;
  void setPlotStyle(const CQGnuPlot::PlotStyle &s);

  CQGnuPlot::FillType fillType() const;
  void setFillType(const CQGnuPlot::FillType &type);

  CQGnuPlot::FillPattern fillPattern() const;
  void setFillPattern(const CQGnuPlot::FillPattern &f);

  CQGnuPlot::SymbolType pointType() const;
  void setPointType(const CQGnuPlot::SymbolType &type);

  CQGnuPlot::BoxWidthType getBoxWidthType() const;
  void setBoxWidthType(const CQGnuPlot::BoxWidthType &type);

  bool syncBars() const { return syncBars_; }
  void setSyncBars(bool b) { syncBars_ = b; }

  void draw();

  void mousePress(const CPoint2D &p, std::vector<CQGnuPlotObject *> &objects);
  void mouseMove (const CPoint2D &p);
  bool mouseTip  (const CPoint2D &p, CQGnuPlot::TipRect &tip);

 public:
  CQGnuPlotGroup*    group_;
  COptValT<CPoint2D> selectedPos_;
  bool               syncBars_;
};

#endif
