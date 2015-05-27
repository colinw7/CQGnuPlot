#ifndef CGnuPlot_H
#define CGnuPlot_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <climits>
#include <memory>
#include <sys/types.h>

#include <CExpr.h>
#include <CFont.h>
#include <CAutoPtr.h>
#include <CStrUtil.h>
#include <COptVal.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CRange2D.h>
#include <CLineDash.h>
#include <COrientation.h>
#include <CRGBA.h>
#include <CAlignType.h>
#include <CAngle.h>
#include <NaN.h>

class CGnuPlotAxis;
class CGnuPlotDevice;
class CGnuPlotGroup;
class CGnuPlotKey;
class CGnuPlotColorBox;
class CGnuPlotPlot;
class CGnuPlotReadLine;
class CGnuPlotSVGDevice;
class CGnuPlotWindow;
class CGnuPlotStyleBase;

class CUnixFile;
class CParseLine;

typedef CRefPtr<CExprValue>             CExprValueP;
typedef std::shared_ptr<CGnuPlotWindow> CGnuPlotWindowP;

//------

#include <CGnuPlotColorSpec.h>
#include <CGnuPlotLineStyle.h>
#include <CGnuPlotFillStyle.h>
#include <CGnuPlotPointStyle.h>
#include <CGnuPlotCoordValue.h>
#include <CGnuPlotKeyData.h>
#include <CGnuPlotPosition.h>
#include <CGnuPlotSize.h>
#include <CGnuPlotAxisData.h>
#include <CGnuPlotTitle.h>
#include <CGnuPlotObject.h>
#include <CGnuPlotArrow.h>
#include <CGnuPlotCircle.h>
#include <CGnuPlotEllipse.h>
#include <CGnuPlotLabel.h>
#include <CGnuPlotPolygon.h>
#include <CGnuPlotRectangle.h>
#include <CGnuPlotFile.h>
#include <CGnuPlotCamera.h>
#include <CGnuPlotPalette.h>
#include <CGnuPlotColorBox.h>
#include <CGnuPlotTicLabel.h>
#include <CGnuPlotUsingCols.h>
#include <CGnuPlotImageStyle.h>
#include <CGnuPlotHistogramData.h>
#include <CGnuPlotMultiplot.h>
#include <CGnuPlotPrefValue.h>
#include <CGnuPlotBoxWidth.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlotBoxPlot.h>
#include <CGnuPlotCircleStyle.h>
#include <CGnuPlotEllipseStyle.h>
#include <CGnuPlotRectStyle.h>
#include <CGnuPlotTextBoxStyle.h>
#include <CGnuPlotTimeStampData.h>
#include <CGnuPlotMouseData.h>
#include <CGnuPlotClip.h>
#include <CGnuPlotBinaryFormat.h>

//------

class CGnuPlot {
 public:
  typedef CGnuPlotTypes::CommandName             CommandName;
  typedef CGnuPlotTypes::VariableName            VariableName;
  typedef CGnuPlotTypes::PlotStyle               PlotStyle;
  typedef CGnuPlotTypes::DataFileVar             DataFileVar;
  typedef CGnuPlotTypes::StyleVar                StyleVar;
  typedef CGnuPlotTypes::PlotVar                 PlotVar;
  typedef CGnuPlotTypes::StyleIncrementType      StyleIncrementType;
  typedef CGnuPlotTypes::Smooth                  Smooth;
  typedef CGnuPlotTypes::HistogramStyle          HistogramStyle;
  typedef CGnuPlotTypes::LogScale                LogScale;
  typedef CGnuPlotTypes::AngleType               AngleType;
  typedef CGnuPlotTypes::DrawLayer               DrawLayer;

  typedef std::map<PlotStyle,CGnuPlotStyleBase*> PlotStyles;
  typedef std::map<LogScale,int>                 LogScaleMap;
  typedef std::map<int,CGnuPlotLineStyleP>       LineStyles;
  typedef std::vector<CExprValueP>               Values;
  typedef std::map<std::string,std::string>      Params;

  //---

  class Bars {
   public:
    Bars(double s=1, bool f=true) :
     size_(s), front_(f) {
    }

    double size() const { return size_; }
    void setSize(double r) { size_ = r; }

    bool isFront() const { return front_; }
    void setFront(bool b) { front_ = b; }

    void show(std::ostream &os) const {
      if (size_ <= 0.0)
        os << "errors are plotted without bars" << std::endl;
      else
        os << "errorbars are plotted in " << (front_ ? "front" : "back") <<
              " with bars of size " << size_ << std::endl;
    }

    void save(std::ostream &os) const {
      os << "set bar " << size_ << " " << (front_ ? "front" : "back") << std::endl;
    }

   private:
    double size_ { 1 };
    bool   front_ { true };
  };

  //---

  typedef std::map<int,CGnuPlotAxisData> IAxisMap;

  struct BorderData {
    int       sides     { 0xFF };
    DrawLayer layer     { DrawLayer::FRONT };
    double    lineWidth { 1.0 };
    COptInt   lineStyle;
    COptInt   lineType;

    void unset() {
      sides = 0;
    }

    void show(std::ostream &os) const;
    void save(std::ostream &os) const;
  };

  struct GridData {
    bool      enabled        { false };
    DrawLayer layer          { DrawLayer::DEFAULT };
    double    polarAngle     { 0 };
    int       majorLineStyle { -1 };
    int       minorLineStyle { -1 };
    int       majorLineType  { -1 };
    int       minorLineType  { -1 };
    double    majorLineWidth { 1.0 };
    double    minorLineWidth { 1.0 };
  };

  struct AxesData {
    AxesData() { }

    GridData         grid;
    IAxisMap         xaxis;
    IAxisMap         yaxis;
    IAxisMap         zaxis;
    IAxisMap         paxis;
    IAxisMap         taxis;
    CGnuPlotAxisData cbaxis;
    CGnuPlotAxisData raxis;
    CGnuPlotAxisData uaxis;
    CGnuPlotAxisData vaxis;
    BorderData       border;
  };

  struct StyleIncrement {
    StyleIncrement() { }

    StyleIncrementType type      { StyleIncrementType::DEFAULT };
    int                styleInd  { 0 };
    int                increment { 8 };
  };

  //---

  struct FilledCurve {
    bool        closed { true };
    bool        above  { false  };
    bool        below  { false  };
    int         xaxis  { 0 };
    int         yaxis  { 0 };
    COptReal    xval;
    COptReal    yval;
    COptPoint2D xyval;
  };

  //---

  struct PlotSize {
    PlotSize() { }

    void unsetSize() {
      square = false;

      xsize.setInvalid();
      ysize.setInvalid();

      xratio.setInvalid();
      yratio.setInvalid();
    }

    void unsetOrigin() {
      xo.setInvalid();
      yo.setInvalid();
    }

    void showSize(std::ostream &os) {
      if (square)
        os << "size is scaled by 1,1" << std::endl;
      else
        os << "size is scaled by " << xsize.getValue(1) << "," << ysize.getValue(1) << std::endl;

      if (! xratio.isValid() && ! yratio.isValid())
        os << "No attempt to control aspect ratio" << std::endl;
      else
        os << "Try to set aspect ratio to " <<
              xratio.getValue(1) << ":" << yratio.getValue(1) << std::endl;
    }

    void showOrigin(std::ostream &os) {
      os << "origin is set to " << xo.getValue(0) << ", " << yo.getValue(0) << std::endl;
    }

    bool     square { false };
    COptReal xo;
    COptReal yo;
    COptReal xsize;
    COptReal ysize;
    COptReal xratio;
    COptReal yratio;
  };

  //---

  typedef std::map<int,CGnuPlotArrowStyle>      ArrowStyles;
  typedef std::map<int,CLineDash>               LineDashes;
  typedef std::vector<CGnuPlotGroupAnnotationP> Annotations;

  //---

  struct MarginValue {
    COptReal value;
    bool     screen { false };

    MarginValue() { }

    MarginValue(double v, bool s=false) { value = v; screen = s; }

    void setValue(double v, bool b) { value = v; screen = b; }

    void resetValue() { value.setInvalid(); screen = false; }

    void show(std::ostream &os, const std::string &id) {
      os << id << " is ";

      if (! value.isValid())
        os << "computed automatically" << std::endl;
      else {
        os << "set to";

        if (screen)
          os << " screen";

        os << " " << value.getValue() << std::endl;
      }
    }
  };

  struct Margin {
    Margin() { }

    Margin(double l, double b, double r, double t) :
     lmargin(l), bmargin(b), rmargin(r), tmargin(t) {
    }

    const COptReal &left  () const { return lmargin.value; }
    const COptReal &bottom() const { return bmargin.value; }
    const COptReal &right () const { return rmargin.value; }
    const COptReal &top   () const { return tmargin.value; }

    void setLeft  (double l, bool s=false) { lmargin.setValue(l, s); }
    void setBottom(double b, bool s=false) { bmargin.setValue(b, s); }
    void setRight (double r, bool s=false) { rmargin.setValue(r, s); }
    void setTop   (double t, bool s=false) { tmargin.setValue(t, s); }

    void resetLeft  () { lmargin.resetValue(); }
    void resetBottom() { bmargin.resetValue(); }
    void resetRight () { rmargin.resetValue(); }
    void resetTop   () { tmargin.resetValue(); }

    CRange2D range() const {
      return CRange2D(left ().getValue(10), bottom().getValue(10),
                      right().getValue(10), top   ().getValue(10));
    }

    void showLeft  (std::ostream &os) { lmargin.show(os, "lmargin"); }
    void showBottom(std::ostream &os) { bmargin.show(os, "bmargin"); }
    void showRight (std::ostream &os) { rmargin.show(os, "rmargin"); }
    void showTop   (std::ostream &os) { tmargin.show(os, "tmargin"); }

    MarginValue lmargin;
    MarginValue bmargin;
    MarginValue rmargin;
    MarginValue tmargin;
  };

  //---

  typedef std::deque<std::string> BufferLines;

  struct FileData {
    CUnixFile*  file { 0 };
    int         lineNum { 0 };
    BufferLines bufferLines;
  };

  struct FunctionData {
    std::string function;
    bool        isAssign = false;
    std::string assign;
  };

  struct HistoryData {
    int  size    { 500 };
    bool numbers { true };
    bool trim    { true };
  };

  class Samples {
   public:
    void get(int &s1, int &s2) const { s1 = samples1_; s2 = samples2_; }
    void set(int s1, int s2) { samples1_ = s1; samples2_ = s2; }

    void unset() { set(100, 100); }

    void save(std::ostream &os) const {
      os << "set samples " << samples1_ << ", " << samples2_ << std::endl;
    }

    void show(std::ostream &os) {
      os << "sampling rate is " << samples1_ << ", " << samples2_ << std::endl;
    }

   private:
    int samples1_ { 100 };
    int samples2_ { 100 };
  };

  struct LinkAxisData {
    bool        enabled { false };
    std::string expr;
    std::string iexpr;
  };

  struct LinkData {
    LinkAxisData linkX2;
    LinkAxisData linkY2;
  };

  class ISOSamples {
   public:
    ISOSamples() { }

    void get(int &s1, int &s2) const { s1 = samples1_; s2 = samples2_; }
    void set(int s1, int s2) { samples1_ = s1; samples2_ = s2; }

    void unset() { set(10, 10); }

    void save(std::ostream &os) const {
      os << "set isosamples " << samples1_ << ", " << samples2_ << std::endl;
    }

    void show(std::ostream &os) const {
      os << "iso sampling rate is " << samples1_ << ", " << samples2_ << std::endl;
    }

   private:
    int samples1_ { 10 };
    int samples2_ { 10 };
  };

  struct DecimalSign {
    void setChar(char c1) { c = c1; }

    void setLocale(const std::string &str) { locale = str; }

    char        c { '\0' };
    std::string locale;
  };

  class Offsets {
   public:
    void set(const CGnuPlotCoordValue &l, const CGnuPlotCoordValue &r,
             const CGnuPlotCoordValue &t, const CGnuPlotCoordValue &b) {
      l_ = l; r_ = r;
      t_ = t; b_ = b;
    }

    void unset() {
      l_ = CGnuPlotCoordValue();
      r_ = CGnuPlotCoordValue();
      t_ = CGnuPlotCoordValue();
      b_ = CGnuPlotCoordValue();
    }

    void show(std::ostream &os) {
      os << "offsets are " << l_ << ", " << r_ << ", " << t_ << ", " << b_ << std::endl;
    }

   private:
    CGnuPlotCoordValue l_, r_, t_, b_;
  };

  struct FitData {
    std::string logfile { false };
    bool        quiet { false };
    bool        results { false };
    bool        brief { false };
    bool        verbose { false };
    bool        errorvariables { false };
    bool        covariancevariables { false };
    bool        errorscaling { false };
    bool        prescale { false };
    int         maxiter { -1 };
    double      limit { 0 };
    double      limit_abs { 0 };
    double      start_lambda { 0 };
    double      lambda_factor { 0 };
    std::string script;
    int         version { 5 };
  };

  struct PrintFile {
    std::string filename;
    bool        isError { true };
    bool        isBlock { false };
    bool        append  { false };

    void unset() {
      filename = "";
      isError  = true;
      isBlock  = false;
      append   = false;
    };

    void show(std::ostream &os) const {
      if (isBlock) {
        os << "print output is saved to datablock $" << filename << std::endl;
      }
      else {
        os << "print output is sent to ";

        if (filename != "")
          os << "'" << filename << "'";
        else if (isError)
          os << "<stderr>";
        else
          os << "<stdout>";

        os << std::endl;
      }
    }
  };

  //---

  struct Hidden3DData {
    bool      enabled         { false };
    DrawLayer layer           { DrawLayer::FRONT };
    COptReal  offset;
    int       trianglePattern { 3 };
    COptInt   undefined;
    bool      altdiagonal     { false };
    bool      bentover        { false };
  };

  struct Surface3DData {
    bool enabled { true };
  };

  struct Contour3DData {
    enum class DrawPos {
      DRAW_BASE,
      DRAW_SURFACE,
      DRAW_BOTH
    };

    bool    enabled { false };
    DrawPos pos     { DrawPos::DRAW_BASE };
  };

  struct Pm3DData {
    enum class PosType {
      SURFACE,
      TOP,
      BOTTOM
    };

    enum class ScanType {
      AUTOMATIC,
      FORWARD,
      BACKWARD,
      DEPTH_ORDER
    };

    enum class FlushType {
      BEGIN,
      CENTER,
      END
    };

    enum class CornerType {
      MEAN,
      GEOMEAN,
      HARMEAN,
      RMS,
      MEDIAN,
      MIN,
      MAX,
      C1,
      C2,
      C3,
      C4
    };

    bool isEnabled() const { return enabled; }
    void setEnabled(bool b) { enabled = b; }

    static std::string posTypeToStr(const PosType &p) {
      if      (p == PosType::SURFACE) return "SURFACE";
      else if (p == PosType::TOP    ) return "TOP";
      else if (p == PosType::BOTTOM ) return "BOTTOM";
      else                            return "???";
    }

    void show(std::ostream &os) const {
      os << "pm3d style is " << (implicit ? "implicit" : "explicit");
      os << " (draw pm3d surface according to style)" << std::endl;

      os << "pm3d plotted at";
      if (pos.empty())
        os << " SURFACE";
      else {
        for (uint i = 0; i < pos.size(); ++i) {
          if (i > 0) os << ", then";

          os << " " << posTypeToStr(pos[i]);
        }
      }
      os << std::endl;

      if      (scanType == ScanType::AUTOMATIC)
        os << "taking scans direction automatically" << std::endl;
      else if (scanType == ScanType::FORWARD)
        os << "taking scans in FORWARD direction" << std::endl;
      else if (scanType == ScanType::BACKWARD)
        os << "taking scans in BACKWARD direction" << std::endl;
      else if (scanType == ScanType::DEPTH_ORDER)
        os << "true depth ordering" << std::endl;

      os << "subsequent scans with different nb of pts are flushed from";
      if      (flushType == FlushType::BEGIN ) os << "BEGIN"  << std::endl;
      else if (flushType == FlushType::CENTER) os << "CENTER" << std::endl;
      else if (flushType == FlushType::END   ) os << "END"    << std::endl;

      if (ftriangles)
        os << "flushing triangles are drawn" << std::endl;
      else
        os << "flushing triangles are not drawn" << std::endl;

      if (clipin == 4)
        os << "clipping: all 4 points of the quadrangle in x,y ranges" << std::endl;
      else
        os << "clipping: at least 1 point of the quadrangle in x,y ranges" << std::endl;

      if (linetype != -1)
        os << "pm3d quadrangle borders will default to linetype " << linetype << std::endl;
      else
        os << "pm3d quadrangles will have no border" << std::endl;

      os << "steps for bilinear interpolation: " << isteps1 << "," << isteps2 << std::endl;

      if      (cornerType == CornerType::MEAN)
        os << "quadrangle color according to averaged 4 corners" << std::endl;
      else if (cornerType == CornerType::GEOMEAN)
        os << "quadrangle color according to geometrical mean of 4 corners" << std::endl;
      else if (cornerType == CornerType::HARMEAN)
        os << "quadrangle color according to harmonic mean of 4 corners" << std::endl;
      else if (cornerType == CornerType::RMS)
        os << "quadrangle color according to root mean square of 4 corners" << std::endl;
      else if (cornerType == CornerType::MEDIAN)
        os << "quadrangle color according to median of 4 corners" << std::endl;
      else if (cornerType == CornerType::MIN)
        os << "quadrangle color according to min of 4 corners" << std::endl;
      else if (cornerType == CornerType::MAX)
        os << "quadrangle color according to max of 4 corners" << std::endl;
      else if (cornerType == CornerType::C1)
        os << "quadrangle color according to corner 1" << std::endl;
      else if (cornerType == CornerType::C2)
        os << "quadrangle color according to corner 2" << std::endl;
      else if (cornerType == CornerType::C3)
        os << "quadrangle color according to corner 3" << std::endl;
      else if (cornerType == CornerType::C4)
        os << "quadrangle color according to corner 4" << std::endl;
    }

    void unset() {
      enabled  = false;
      implicit = false;
      linetype = -1;
    }

    typedef std::vector<PosType> PosTypeList;

    bool             enabled    { false };
    PosTypeList      pos;
    int              isteps1    { -1 };
    int              isteps2    { -1 };
    ScanType         scanType   { ScanType::AUTOMATIC };
    FlushType        flushType  { FlushType::BEGIN };
    bool             ftriangles { false };
    int              clipin     { 4 };
    CornerType       cornerType { CornerType::MEAN };
    int              linetype   { -1 };
    bool             implicit   { false };
  };

  //---

  class XYPlane {
   public:
    XYPlane() { }

    double z() const { return z_; }

    bool isRelative() const { return relative_; }

    void setZ(double z, bool rel=false) {
      z_        = z;
      relative_ = rel;
    }

   private:
    double z_        { 0.0   };
    bool   relative_ { false };
  };

  //---

  struct ForCmd {
    bool        isIn { false };
    std::string var;
    std::string start;
    std::string end;
    std::string inc;
  };

  //---

  typedef std::vector<CGnuPlotPlot *> Plots;
  typedef StringArray                 Statements;

  typedef std::map<CGnuPlot::VariableName,CGnuPlotPrefValueBase *> VarPrefs;

 public:
  CGnuPlot();

  virtual ~CGnuPlot();

  //----

  void setDebug(bool b) { debug_ = b; }
  bool isDebug() const { return debug_; }

  void setExprDebug(bool b);
  bool isExprDebug() const { return edebug_; }

  //----

  CGnuPlotDevice *device() const { return device_; }

  void addDevice(const std::string &name, CGnuPlotDevice *device);
  bool setDevice(const std::string &name);

  void pushDevice();
  void popDevice();

  //----

  void addWindow(CGnuPlotWindowP window);
  void removeWindow(CGnuPlotWindow *window);

  //----

  const PlotStyles &plotStyles() const { return plotStyles_; }

  void addPlotStyle(PlotStyle plotStyle, CGnuPlotStyleBase *style);
  CGnuPlotStyleBase *getPlotStyle(PlotStyle plotStyle) const;

  //----

  PlotStyle getDataStyle() const { return dataStyle_; }
  void setDataStyle(PlotStyle style) { dataStyle_ = style; }

  PlotStyle getFunctionStyle() const { return functionStyle_; }
  void setFunctionStyle(PlotStyle style) { functionStyle_ = style; }

  const CGnuPlotFillStyle &fillStyle() const { return fillStyle_; }
  void setFillStyle(const CGnuPlotFillStyle &s) { fillStyle_ = s; }

  CGnuPlotLineStyleP lineStyle();
  void setLineStyle(CGnuPlotLineStyleP ls) { lineStyle_ = ls; }

  CGnuPlotLineStyleP getLineStyleInd(int ind);
  void setLineStyleInd(int ind);
  void resetLineStyleInd(int ind);

  const CGnuPlotPointStyle &pointStyle() const { return pointStyle_; }
  void setPointStyle(const CGnuPlotPointStyle &s) { pointStyle_ = s; }

  const LineStyles &lineStyles() const { return lineStyles_; }
  CGnuPlotLineStyleP lineStyle(int i) const;

  const CGnuPlotHistogramData &histogramData() { return histogramData_; }
  void setHistogramData(const CGnuPlotHistogramData &data) { histogramData_ = data; }

  const CGnuPlotArrowStyle &arrowStyle() const { return arrowStyle_; }
  void setArrowStyle(const CGnuPlotArrowStyle &as) { arrowStyle_ = as; }

  const CGnuPlotKeyData &keyData() const { return keyData_; }
  void setKeyData(const CGnuPlotKeyData &k) { keyData_ = k; }

  const CBBox2D &region() const { return region_; }
  void setRegion(const CBBox2D &r) { region_ = r; }

  const Margin &margin() const { return margin_; }
  void setMargin(const Margin &b) { margin_ = b; }

  const CRGBA &backgroundColor() const { return backgroundColor_; }
  void setBackgroundColor(const CRGBA &c) { backgroundColor_ = c; }

  int xind() const { return xind_; }
  int yind() const { return yind_; }
  int zind() const { return zind_; }

  CGnuPlotTypes::Mapping mapping() const { return mapping_; }
  void setMapping(CGnuPlotTypes::Mapping m) { mapping_ = m; }

  const PlotSize &plotSize() const { return plotSize_; }
  void setPlotSize(const PlotSize &s) { plotSize_ = s; }

  void setMacros(bool b) { macros_ = b; }
  bool isMacros() const { return macros_; }

  //---

  const COptBBox2D &clearRect() const { return clearRect_; }

  //---

  const AxesData &axesData() const { return axesData_; }
  void setAxesData(const AxesData &a) { axesData_ = a; }

  const CGnuPlotAxisData &xaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->xaxis(ind);
  }
  const CGnuPlotAxisData &yaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->yaxis(ind);
  }
  const CGnuPlotAxisData &zaxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->zaxis(ind);
  }
  const CGnuPlotAxisData &paxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->paxis(ind);
  }
  const CGnuPlotAxisData &taxis(int ind) const {
    return const_cast<CGnuPlot *>(this)->taxis(ind);
  }
  const CGnuPlotAxisData &cbaxis() const {
    return const_cast<CGnuPlot *>(this)->cbaxis();
  }

  CGnuPlotAxisData &xaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.xaxis.find(ind);

    if (p == axesData_.xaxis.end())
      p = axesData_.xaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &yaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.yaxis.find(ind);

    if (p == axesData_.yaxis.end())
      p = axesData_.yaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &zaxis(int ind) {
    CGnuPlot::IAxisMap::iterator p = axesData_.zaxis.find(ind);

    if (p == axesData_.zaxis.end())
      p = axesData_.zaxis.insert(p, CGnuPlot::IAxisMap::value_type(ind, CGnuPlotAxisData(ind)));

    return (*p).second;
  }

  CGnuPlotAxisData &paxis(int ind) { return this->axesData_.paxis[ind]; }
  CGnuPlotAxisData &taxis(int ind) { return this->axesData_.taxis[ind]; }

  CGnuPlotAxisData &cbaxis() { return this->axesData_.cbaxis; }
  CGnuPlotAxisData &raxis () { return this->axesData_.raxis ; }
  CGnuPlotAxisData &uaxis () { return this->axesData_.uaxis ; }
  CGnuPlotAxisData &vaxis () { return this->axesData_.vaxis ; }

  //---

  void getXRange(double *xmin, double *xmax) {
    *xmin = xaxis(1).min().getValue(-10);
    *xmax = xaxis(1).max().getValue( 10);
  }
  void getYRange(double *ymin, double *ymax) {
    *ymin = yaxis(1).min().getValue(-10);
    *ymax = yaxis(1).max().getValue( 10);
  }
  void getZRange(double *zmin, double *zmax) {
    *zmin = zaxis(1).min().getValue(-10);
    *zmax = zaxis(1).max().getValue( 10);
  }
  void getTRange(double *tmin, double *tmax) {
    *tmin = taxis(1).min().getValue(0);

    if (getAngleType() == CGnuPlotTypes::AngleType::DEGREES)
      *tmax = taxis(1).max().getValue(360.0);
    else
      *tmax = taxis(1).max().getValue(2*M_PI);
  }
  void getURange(double *umin, double *umax) {
    *umin = uaxis().min().getValue(-5);
    *umax = uaxis().max().getValue(5);
  }
  void getVRange(double *vmin, double *vmax) {
    *vmin = vaxis().min().getValue(-5);
    *vmax = vaxis().max().getValue(5);
  }

  //---

  const Bars &bars() const { return bars_; }
  void setBars(const Bars &s) { bars_ = s; }

  double barsSize() const { return bars_.size(); }
  void setBarsSize(double s) { bars_.setSize(s); }

  bool barsFront() const { return bars_.isFront(); }
  void setBarsFront(bool b) { bars_.setFront(b); }

  //---

  const CGnuPlotBoxWidth &boxWidth() const { return boxWidth_; }
  void setBoxWidth(const CGnuPlotBoxWidth &w) { boxWidth_ = w; }

  //---

  void setBoxPlot(const CGnuPlotBoxPlot &b) { boxPlot_ = b; }
  const CGnuPlotBoxPlot &getBoxPlot() const { return boxPlot_; }

  void setSmooth(Smooth s) { smooth_ = s; }
  Smooth getSmooth() const { return smooth_; }

  const CGnuPlotPalette &palette() const { return palette_; }
  void setPalette(const CGnuPlotPalette &p) { palette_ = p; }

  const CGnuPlotColorBox &colorBox() const { return colorBox_; }
  void setColorBox(const CGnuPlotColorBox &c) { colorBox_ = c; }

  const FilledCurve &filledCurve() const { return filledCurve_; }
  void setFilledCurve(const FilledCurve &c) { filledCurve_ = c; }

  const LogScaleMap &logScaleMap() const { return logScale_; }
  void setLogScale(LogScale scale, int base) { logScale_[scale] = base; }
  bool isLogScale(LogScale scale) const { return logScale_.find(scale) != logScale_.end(); }
  int getLogScale(LogScale scale) const { auto p = logScale_.find(scale); return (*p).second; }
  void resetLogScale() { logScale_.clear(); }

  double whiskerBars() const { return whiskerBars_; }
  void setWhiskerBars(double w) { whiskerBars_ = w; }

  const CGnuPlotCamera &camera() const { return camera_; }

  //---

  const Hidden3DData &hidden3D() const { return hidden3D_; }
  void setHidden3D(const Hidden3DData &h) { hidden3D_ = h; }

  //---

  bool isSurface3D() const { return surface3D_.enabled; }
  void setSurface3D(bool b) { surface3D_.enabled = b; }

  //---

  bool isContour3D() const { return contour3D_.enabled; }
  void setContour3D(bool b) { contour3D_.enabled = b; }

  //---

  const Pm3DData &pm3D() const { return pm3D_; }
  void setPm3D(const Pm3DData &p) { pm3D_ = p; }

  //------

  const CGnuPlotClip &clip() const { return clip_; }
  void setClip(const CGnuPlotClip &clip) { clip_ = clip; }

  //------

  bool isParametric() const { return parametric_; }
  void setParametric(bool b) { parametric_ = b; }

  bool isPolar() const { return polar_; }
  void setPolar(bool b) { polar_ = b; }

  //------

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  const CGnuPlotBinaryFormat &binaryFormat() const { return binaryFormat_; }
  void setBinaryFormat(const CGnuPlotBinaryFormat &fmt) { binaryFormat_ = fmt; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  const CGnuPlotImageStyle &imageStyle() const { return imageStyle_; }
  void setImageStyle(const CGnuPlotImageStyle &imageStyle) { imageStyle_ = imageStyle; }

  bool isImageStyle(PlotStyle style) const {
    return (style == PlotStyle::IMAGE || style == PlotStyle::RGBIMAGE ||
            style == PlotStyle::RGBALPHA);
  }

  void setTimeFmt(const std::string &f) { timeFmt_ = f; }
  const std::string &timeFmt() { return timeFmt_; }

  //------

  const Annotations &annotations() const { return annotations_; }
  void setAnnotations(const Annotations &annotations) { annotations_ = annotations; }

  //------

  void setOutputFile(const std::string &file);
  const std::string &getOutputFile() const { return outputFile_; }

  bool load(const std::string &filename, const StringArray &args=StringArray());

  bool exec(const std::string &cmd);

  void initReadLine();

  void loop();

  void prompt(const std::string &msg);

  CGnuPlotWindow *createWindow();

  CGnuPlotGroup *createGroup(CGnuPlotWindow *window);

  CGnuPlotPlot *createPlot(CGnuPlotGroup *group, PlotStyle plotStyle);

  CGnuPlotLineStyle *createLineStyle();

  CGnuPlotAxis *createAxis(CGnuPlotGroup *group, const std::string &id, COrientation dir);
  CGnuPlotKey  *createKey (CGnuPlotGroup *group);

  CGnuPlotColorBox *createColorBox(CGnuPlotGroup *group);

  CGnuPlotPalette *createPalette(CGnuPlotGroup *group);

  CGnuPlotTitle *createTitle(CGnuPlotGroup *group);

  CGnuPlotCamera *createCamera(CGnuPlotGroup *group);

  CGnuPlotRenderer *renderer();

  //----

  template<typename T>
  T *getAnnotation(int ind) const {
    for (auto &ann : annotations_)
      if (ann->getInd() == ind && (dynamic_cast<T *>(ann.get()) != 0))
        return dynamic_cast<T *>(ann.get());

    return 0;
  }

  template<typename T>
  T *lookupAnnotation(int ind) {
    T *annotation = getAnnotation<T>(ind);

    if (! annotation) {
      // TODO: unique ind ?
      if (ind <= 0)
        ind = annotations_.size() + 1;

      annotation = new T(0);

      annotation->setInd(ind);

      annotations_.push_back(CGnuPlotGroupAnnotationP(annotation));
    }

    return annotation;
  }

  template<typename T>
  void showAnnotations(std::ostream &os=std::cout, int ind=-1) {
    for (const auto &ann : annotations_) {
      const T *obj = dynamic_cast<const T *>(ann.get());
      if (! obj) continue;

      if (ind != -1 && obj->getInd() != ind) continue;

      os << T::getName() << " " << obj->getInd();
      obj->print(os);
      os << std::endl;
    }
  }

  template<typename T>
  void clearAnnotations(int ind=-1) {
    Annotations annotations;

    for (auto &ann : annotations_) {
      T *obj = dynamic_cast<T *>(ann.get());

      if (obj) {
        if (ind != -1 && obj->getInd() != ind)
          annotations.push_back(ann);
      }
      else
        annotations.push_back(ann);
    }

    annotations_ = annotations;
  }

  //----

  void timeout();

  void stateChanged(CGnuPlotWindow *window, CGnuPlotTypes::ChangeState state);

  int numFieldValues() const { return fieldValues_.size(); }

  CExprValueP fieldValue(int i) const {
    if (i < 1 || i > int(fieldValues_.size())) return CExprValueP();
    return fieldValues_[i - 1];
  }

  int setNum() const { return setNum_; }
  void setSetNum(int i) { setNum_ = i; }

  int pointNum() const { return pointNum_; }
  void setPointNum(int n) { pointNum_ = n; }

  int getColumnIndex(const std::string &str) const;

  CExprValueP fieldToValue(int nf, const std::string &field);

  bool timeToValue(const std::string &str, double &t);

  void resetLineStyle();

  void incLineStyle();

  void drawWindows();

  CGnuPlotArrowStyle arrowStyle(int id) const;

  void errorMsg(const std::string &msg) const;
  void debugMsg(const std::string &msg) const;

  CPoint3D sphericalMap(const CPoint2D &p) const;

  double angleToRad(double a) const;

 private:
  void addPlotStyles();

  bool parseLine(const std::string &str);

  std::string replaceEmbedded(const std::string &str) const;

  bool execCmd(const std::string &cmd, StringArray &lines) const;

  bool parseStatement(int &i, const Statements &statements);

  void exitCmd   (const std::string &args);
  void helpCmd   (const std::string &args);
  void historyCmd(const std::string &args);
  void printCmd  (const std::string &args);
  void printfCmd (const std::string &args);
  void quitCmd   (const std::string &args);
  void cdCmd     (const std::string &args);
  void pwdCmd    (const std::string &args);

  void callCmd(const std::string &args);
  void loadCmd(const std::string &args);
  void saveCmd(const std::string &args);

  void plotCmd   (const std::string &args);
  void plotCmd1  (const std::string &args, CGnuPlotGroup *group, Plots &plots);
  void replotCmd (const std::string &args);
  void refreshCmd(const std::string &args);
  void splotCmd  (const std::string &args);
  void splotCmd1 (const std::string &args, CGnuPlotGroup *group, Plots &plots);

  void plotForCmd (const ForCmd &forCmd, const std::string &args,
                   CGnuPlotGroup *group, Plots &plots);
  void splotForCmd(const ForCmd &forCmd, const std::string &args,
                   CGnuPlotGroup *group, Plots &plots);
  void setForCmd  (const ForCmd &forCmd, const std::string &args);
  void unsetForCmd(const ForCmd &forCmd, const std::string &args);

  void splitPlotCmd(const std::string &cmd, std::vector<std::string> &cmds, bool is3D);

  bool parseModifiers2D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &ls,
                        CGnuPlotFillStyle &fs, CGnuPlotArrowStyle &as);
  bool parseModifiers3D(PlotStyle style, CParseLine &line, CGnuPlotLineStyle &ls,
                        CGnuPlotFillStyle &fs, CGnuPlotArrowStyle &as);

  bool parseFor(CParseLine &line, ForCmd &forCmd, std::string &cmd);

  bool setCmd     (const std::string &args);
  bool getCmd     (const std::string &args);
  bool showCmd    (const std::string &args);
  void resetCmd   (const std::string &args);
  void undefineCmd(const std::string &args);
  bool unsetCmd   (const std::string &args);

  void shellCmd (const std::string &args);
  void systemCmd(const std::string &args);
  void statsCmd (const std::string &args);

  void clearCmd(const std::string &args);
  void lowerCmd(const std::string &args);
  void raiseCmd(const std::string &args);
  void testCmd (const std::string &args);

  void fitCmd   (const std::string &args);
  void updateCmd(const std::string &args);

  void bindCmd    (const std::string &args);
  void ifCmd      (int &i, const Statements &statements);
  void doCmd      (const std::string &args);
  void whileCmd   (const std::string &args);
  void evaluateCmd(const std::string &args);
  void pauseCmd   (const std::string &args);
  void rereadCmd  (const std::string &args);

  void showColorNames();
  void showVariables(std::ostream &os, const StringArray &args=StringArray());
  void showFunctions(std::ostream &os);
  void showRGBFormulae(std::ostream &os);

  void readBlockLines(Statements &lines, std::string &eline, int depth);

  std::string getUsingStr(CParseLine &line);

  void parseIndex(CParseLine &line, int &indexStart, int &indexEnd, int &indexStep);
  void parseEvery(CParseLine &line, int &everyPointStart, int &everyPointEnd, int &everyPointStep,
                  int &everyBlockStart, int &everyBlockEnd, int &everyBlockStep);

  bool parseFont(CParseLine &line, CFontPtr &font);

  bool parseBoolExpression(CParseLine &line, bool &res);

  bool parseExpression(CParseLine &line, std::string &expr);

  bool parseFillStyle(CParseLine &line, CGnuPlotFillStyle &fillStyle);

  CGnuPlotPlot *addFunction2D(CGnuPlotGroup *group, const StringArray &functions, PlotStyle style);
  CGnuPlotPlot *addFunction3D(CGnuPlotGroup *group, const StringArray &functions, PlotStyle style);

  Plots addFile2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                  const CGnuPlotUsingCols &usingCols);
  Plots addFile3D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                  const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addImage2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);
  CGnuPlotPlot *addImage3D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                           const CGnuPlotUsingCols &usingCols);

  CGnuPlotPlot *addBinary2D(CGnuPlotGroup *group, const std::string &filename, PlotStyle style,
                            const CGnuPlotUsingCols &usingCols);
  CGnuPlotPlot *addBinary3D(CGnuPlotGroup *group, const std::string &filename,
                            const CGnuPlotUsingCols &usingCols);

  bool parseAxisRange   (CParseLine &line, CGnuPlotAxisData &axis, bool hasArgs=true);
  bool parseAxisLabel   (CParseLine &line, CGnuPlotAxisData &axis);
  bool parseAxesTics    (CParseLine &line, CGnuPlotAxisData &axis);
  void parseAxisZeroAxis(CParseLine &line, CGnuPlotAxisData &axis);

  bool parseRange(CParseLine &line, StringArray &fields);

  bool decodeRange(const StringArray &xfields, CGnuPlotAxisData &axis);

  void parseArrayValues(CParseLine &line, std::vector<CSize2D> &sizes);

  void processParams(const Params &params);

  CExprCTokenStack compileExpression(const std::string &expr) const;

  //---

  void setPrintStdOut() {
    printFile_.filename = ""; printFile_.isError = false; printFile_.isBlock = false;
  }
  void setPrintFile(const std::string &file) {
    printFile_.filename = file; printFile_.isError = false; printFile_.isBlock = false;
  }
  void setPrintDataBlock(const std::string &name) {
    printFile_.filename = name; printFile_.isError = false; printFile_.isBlock = true;
  }

  void setPrintAppend(bool b) { printFile_.append = b; }
  bool getPrintAppend() { return printFile_.append; }

  //---

  void setTableFile(const std::string &file) { tableFile_ = file; }
  const std::string &getTableFile() const { return tableFile_; }

  //---

  void setAngleType(AngleType type);
  AngleType getAngleType() const { return getPrefValue<AngleType>(VariableName::ANGLES); }
  void resetAngleType() { resetPrefValue(VariableName::ANGLES); }
  void printAngleType(std::ostream &os) { printPrefValue(os, VariableName::ANGLES); }
  void showAngleType(std::ostream &os) { showPrefValue(os, VariableName::ANGLES); }
  std::string getAngleTypeString() const {
    return getPrefValueString<AngleType>(VariableName::ANGLES);
  }

  //---

  void setStyleIncrementType(StyleIncrementType s) { styleIncrement_.type = s; }
  StyleIncrementType getStyleIncrementType() { return styleIncrement_.type; }

  const CGnuPlotTitle &title() const { return title_; }
  void setTitle(const CGnuPlotTitle &t) { title_ = t; }

  void setDummyVars(const std::string &dummyVar1, const std::string &dummyVar2);
  void getDummyVars(std::string &dummyVar1, std::string &dummyVar2) const;
  void resetDummyVars();

  const Samples    &samples   () const { return samples_; }
  const ISOSamples &isoSamples() const { return isoSamples_; }

  bool parseColor(CParseLine &line, CRGBA &c);
  bool parseColorSpec(CParseLine &line, CGnuPlotColorSpec &c);

  bool parseInteger(CParseLine &line, int &i) const;
  bool parseReal(CParseLine &line, double &r) const;
  bool parseString(CParseLine &line, std::string &str, const std::string &msg="") const;

  void skipString(CParseLine &line) const;

  bool parseRoundBracketedString(CParseLine &line, std::string &str) const;
  bool skipRoundBracketedString(CParseLine &line) const;

  bool parseSquareBracketedString(CParseLine &line, std::string &str) const;
  bool skipSquareBracketedString(CParseLine &line) const;

  bool getIntegerVariable(const std::string &name, int &value) const;
  bool getRealVariable   (const std::string &name, double &value) const;
  bool getStringVariable (const std::string &name, std::string &value) const;

  bool parsePosition(CParseLine &line, CGnuPlotPosition &pos);
  bool parseCoordValue(CParseLine &line, CGnuPlotCoordValue &v);
  bool parsePoint(CParseLine &line, CPoint2D &point);
  bool parseSize(CParseLine &line, CGnuPlotSize &size);
  bool parseSize(CParseLine &line, CSize2D &size);

  const CGnuPlotMultiplot &multiplot() const { return multiplot_; }
  void setMultiplot(const CGnuPlotMultiplot &m) { multiplot_ = m; }

  //---

  bool parseFunction(CParseLine &line, FunctionData &functionData);

  bool processAssignFunction(const std::string &lhs, const std::string &rhs);

  bool readIdentifier(CParseLine &line, std::string &identifier) const;

  std::string readNonSpace(CParseLine &line);
  std::string readNonSpaceNonComma(CParseLine &line);
  std::string readNonSpaceNonChar(CParseLine &line, char c);
  std::string readNonSpaceNonChar(CParseLine &line, const std::string &c);
  std::string readName(CParseLine &line);

  bool fileReadLine(std::string &line);

  bool fieldToReal(const std::string &field, double &r) const;

  template<typename T>
  T getPrefValue(VariableName name) const {
    return static_cast<CGnuPlotPrefValue<T> *>
     (const_cast<CGnuPlot *>(this)->varPrefs_[name])->get();
  }

  template<typename T>
  void setPrefValue(VariableName name, const T &value) {
    static_cast<CGnuPlotPrefValue<T> *>(varPrefs_[name])->set(value);
  }

  void setPrefValue(VariableName name, const std::string &str) {
    varPrefs_[name]->set(str);
  }

  void resetPrefValue(VariableName name) {
    varPrefs_[name]->reset();
  }

  template<typename T>
  std::string getPrefValueString(VariableName name) const {
    return static_cast<CGnuPlotPrefValue<T> *>
     (const_cast<CGnuPlot *>(this)->varPrefs_[name])->toString();
  }

  void showPrefValue(std::ostream &os, VariableName name) {
    varPrefs_[name]->show(os);
  }

  void printPrefValue(std::ostream &os, VariableName name) {
    varPrefs_[name]->print(os);
  }

 private:
  typedef std::map<std::string,CGnuPlotDevice*>  Devices;
  typedef std::vector<CGnuPlotDevice*>           DeviceStack;
  typedef StringArray                            FileLines;
  typedef std::vector<FileData>                  FileDataArray;
  typedef CAutoPtr<CGnuPlotReadLine>             ReadLineP;
  typedef std::map<std::string,std::string>      DummyVarMap;
  typedef std::vector<std::string>               PathList;

  bool                   debug_  { false };
  bool                   edebug_ { false };
  CGnuPlotSVGDevice*     svgDevice_ { 0 };
  CGnuPlotDevice*        device_ { 0 };
  Devices                devices_;
  DeviceStack            deviceStack_;
  PlotStyles             plotStyles_;
  FileData               fileData_;
  FileDataArray          fileDataArray_;
  CGnuPlotFile           dataFile_;
  CGnuPlotBoxWidth       boxWidth_;
  Bars                   bars_;
  std::string            outputFile_;
  PrintFile              printFile_;
  std::string            lastPlotCmd_;
  std::string            lastSPlotCmd_;
  std::string            lastFilename_;
  std::string            tableFile_;
  Smooth                 smooth_ { Smooth::NONE };
  CGnuPlotHistogramData  histogramData_;
  CGnuPlotLineStyleP     lineStyle_;
  CGnuPlotPointStyle     pointStyle_;
  CGnuPlotTitle          title_;
  VarPrefs               varPrefs_;
  AxesData               axesData_;
  CGnuPlotKeyData        keyData_;
  CBBox2D                region_ { 0, 0, 1, 1 };
  Margin                 margin_;
  CRGBA                  backgroundColor_ { 1, 1, 1};
  std::string            colorSequence_ { "default" };
  int                    xind_ { 1 };
  int                    yind_ { 1 };
  int                    zind_ { 1 };

  // styles
  ArrowStyles            arrowStyles_;
  CGnuPlotArrowStyle     arrowStyle_;
  CGnuPlotBoxPlot        boxPlot_;
  PlotStyle              dataStyle_      { PlotStyle::POINTS };
  CGnuPlotFillStyle      fillStyle_;
  PlotStyle              functionStyle_  { PlotStyle::LINES };
  StyleIncrement         styleIncrement_;
  LineStyles             lineStyles_;
  CGnuPlotCircleStyle    circleStyle_;
  CGnuPlotRectStyle      rectStyle_;
  CGnuPlotEllipseStyle   ellipseStyle_;
  CGnuPlotTextBoxStyle   textBoxStyle_;

  LineDashes             lineDashes_;
  bool                   binary_ { false };
  CGnuPlotBinaryFormat   binaryFormat_;
  bool                   matrix_ { false };
  CGnuPlotClip           clip_;
  bool                   parametric_ { false };
  bool                   polar_ { false };
  bool                   macros_ { false };
  double                 zero_ { 1E-8 };
  HistoryData            historyData_;
  CGnuPlotImageStyle     imageStyle_;
  Annotations            annotations_;
  CGnuPlotCamera         camera_;
  CGnuPlotPalette        palette_;
  CGnuPlotColorBox       colorBox_;
  FilledCurve            filledCurve_;
  std::string            timeFmt_ { "%d/%m/%y,%H:%M" };
  LogScaleMap            logScale_;
  DummyVarMap            dummyVars_;
  Samples                samples_;
  LinkData               linkData_;
  ISOSamples             isoSamples_;
  CGnuPlotTypes::Mapping mapping_ { CGnuPlotTypes::Mapping::CARTESIAN_MAPPING };
  PlotSize               plotSize_;
  DecimalSign            decimalSign_;
  Offsets                offsets_;
  FitData                fitData_;
  PathList               loadPaths_;
  PathList               fontPath_;
  std::string            psDir_;
  std::string            encoding_;
  std::string            locale_;
  CGnuPlotMouseData      mouseData_;
  CGnuPlotMultiplot      multiplot_;
  CGnuPlotTimeStampData  timeStamp_;
  COptBBox2D             clearRect_;
  Hidden3DData           hidden3D_;
  Surface3DData          surface3D_;
  Contour3DData          contour3D_;
  XYPlane                xyPlane_;
  Pm3DData               pm3D_;
  double                 pointIntervalBox_ { 1 };
  double                 whiskerBars_ { 0 };
  ReadLineP              readLine_;

  mutable Values           fieldValues_;
  mutable int              setNum_;
  mutable int              pointNum_;
  mutable CGnuPlotTicLabel ticLabel_;
};

#endif
