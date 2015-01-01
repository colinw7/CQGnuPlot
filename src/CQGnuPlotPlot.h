#ifndef CQGnuPlotPlot_H
#define CQGnuPlotPlot_H

#include <CQGnuPlot.h>
#include <CGnuPlotPlot.h>

#include <QObject>
#include <QColor>
#include <QRect>

class CQGnuPlotWindow;
class CQGnuPlotRenderer;
class CQGnuPlotCanvas;
class QPainter;

class CQGnuPlotPlot : public QObject, public CGnuPlotPlot {
  Q_OBJECT

  Q_ENUMS(CQHAlignType)
  Q_ENUMS(CQVAlignType)
  Q_ENUMS(CQFillType)
  Q_ENUMS(CQFillPattern)

  Q_PROPERTY(double regionLeft   READ getRegionLeft   WRITE setRegionLeft  )
  Q_PROPERTY(double regionRight  READ getRegionRight  WRITE setRegionRight )
  Q_PROPERTY(double regionTop    READ getRegionTop    WRITE setRegionTop   )
  Q_PROPERTY(double regionBottom READ getRegionBottom WRITE setRegionBottom)

  Q_PROPERTY(double marginLeft   READ marginLeft   WRITE setMarginLeft  )
  Q_PROPERTY(double marginRight  READ marginRight  WRITE setMarginRight )
  Q_PROPERTY(double marginTop    READ marginTop    WRITE setMarginTop   )
  Q_PROPERTY(double marginBottom READ marginBottom WRITE setMarginBottom)

  Q_PROPERTY(double xmin READ getXMin WRITE setXMin)
  Q_PROPERTY(double ymin READ getYMin WRITE setYMin)
  Q_PROPERTY(double xmax READ getXMax WRITE setXMax)
  Q_PROPERTY(double ymax READ getYMax WRITE setYMax)

  Q_PROPERTY(double ratio READ getRatio WRITE setRatio)

  Q_PROPERTY(QString xlabel      READ getXLabel      WRITE setXLabel)
  Q_PROPERTY(QString ylabel      READ getYLabel      WRITE setYLabel)
  Q_PROPERTY(bool    xtics       READ getXTics       WRITE setXTics)
  Q_PROPERTY(bool    ytics       READ getYTics       WRITE setYTics)
  Q_PROPERTY(bool    xticsMirror READ getXTicsMirror WRITE setXTicsMirror)
  Q_PROPERTY(bool    yticsMirror READ getYTicsMirror WRITE setYTicsMirror)
  Q_PROPERTY(bool    xgrid       READ getXGrid       WRITE setXGrid)
  Q_PROPERTY(bool    ygrid       READ getYGrid       WRITE setYGrid)

  Q_PROPERTY(int     borders     READ getBorders     WRITE setBorders    )
  Q_PROPERTY(double  borderWidth READ getBorderWidth WRITE setBorderWidth)

  Q_PROPERTY(bool         keyDisplayed READ getKeyDisplayed WRITE setKeyDisplayed)
  Q_PROPERTY(CQHAlignType keyHAlign    READ getKeyHAlign    WRITE setKeyHAlign   )
  Q_PROPERTY(CQVAlignType keyVAlign    READ getKeyVAlign    WRITE setKeyVAlign   )
  Q_PROPERTY(bool         keyBox       READ getKeyBox       WRITE setKeyBox      )
  Q_PROPERTY(bool         keyReverse   READ getKeyReverse   WRITE setKeyReverse  )

  Q_PROPERTY(int                   lineStyleId READ lineStyleId WRITE setLineStyleId)
  Q_PROPERTY(CQFillType            fillType    READ fillType    WRITE setFillType   )
  Q_PROPERTY(CQFillPattern         fillPattern READ fillPattern WRITE setFillPattern)
  Q_PROPERTY(QColor                lineColor   READ lineColor   WRITE setLineColor  )
  Q_PROPERTY(double                lineWidth   READ lineWidth   WRITE setLineWidth  )
  Q_PROPERTY(CQGnuPlot::SymbolType pointType   READ pointType   WRITE setPointType  )
  Q_PROPERTY(double        pointSize   READ pointSize   WRITE setPointSize  )

  Q_PROPERTY(int trianglePattern3D READ trianglePattern3D WRITE setTrianglePattern3D)

 public:
  enum CQHAlignType {
    AlignLeft,
    AlignRight,
    AlignHCenter
  };

  enum CQVAlignType {
    AlignTop,
    AlignBottom,
    AlignVCenter
  };

  enum CQFillType {
    FillNone,
    FillSolid,
    FillPatterned
  };

  enum CQFillPattern {
    PatternNone,
    PatternHatch,
    PatternDense,
    PatternFg,
    PatternFDiag,
    PatternBDiag,
    PatternFDiag1,
    PatternBDiag1,
    PatternBg
  };

 public:
  CQGnuPlotPlot(CQGnuPlotWindow *window);

 ~CQGnuPlotPlot() override;

  CQGnuPlotCanvas *canvas() const;

  void setPainter(QPainter *p);

  QString getXLabel() const { return CGnuPlotPlot::getXLabel().c_str(); }
  void setXLabel(const QString &s) { CGnuPlotPlot::setXLabel(s.toStdString()); }
  QString getYLabel() const { return CGnuPlotPlot::getYLabel().c_str(); }
  void setYLabel(const QString &s) { CGnuPlotPlot::setYLabel(s.toStdString()); }

  QColor lineColor() const;
  void setLineColor(const QColor &c);

  CQHAlignType getKeyHAlign() const;
  void setKeyHAlign(const CQHAlignType &a);

  CQVAlignType getKeyVAlign() const;
  void setKeyVAlign(const CQVAlignType &a);

  CQFillType fillType() const;
  void setFillType(const CQFillType &type);

  CQFillPattern fillPattern() const;
  void setFillPattern(const CQFillPattern &f);

  CQGnuPlot::SymbolType pointType() const;
  void setPointType(const CQGnuPlot::SymbolType &type);

  void stateChanged(CGnuPlot::ChangeState) override { }

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

  CQGnuPlotWindow*   window_;
  CQGnuPlotRenderer* renderer_;
  COptValT<CPoint2D> selectedPos_;
  Bars               bars_;
};

#endif
