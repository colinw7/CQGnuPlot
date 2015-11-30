#ifndef CGnuPlotSurface_H
#define CGnuPlotSurface_H

#include <CPoint2D.h>
#include <CRGBA.h>
#include <COptVal.h>
#include <map>
#include <vector>

class CGnuPlotRenderer;
class CGnuPlotPlot;

class CGnuPlotSurface {
 public:
  typedef std::vector<CPoint2D>            Points;
  typedef std::map<int,Points>             IPoints;
  typedef std::map<int,IPoints>            IJPoints;
  typedef std::pair<Points,int>            PointsInd;
  typedef std::pair<PointsInd,CRGBA>       PointsIndColor;
  typedef std::pair<double,PointsIndColor> ZPoints;
  typedef std::vector<ZPoints>             ZPointsArray;
  typedef std::map<double,ZPointsArray>    ZPolygons;

 public:
  CGnuPlotSurface(CGnuPlotPlot *plot);

  void draw(CGnuPlotRenderer *renderer);

  bool isCalcColor() const { return calcColor_; }
  void setCalcColor(bool b) { calcColor_ = b; }

  bool isDataColor() const { return dataColor_; }
  void setDataColor(bool b) { dataColor_ = b; }

  void setData(const COptReal &zmin, const COptReal &zmax, const ZPolygons &zpolygons,
               const IJPoints &ijpoints);

 private:
  CGnuPlotPlot *plot_;
  COptReal      zmin_, zmax_;
  ZPolygons     zpolygons_;
  IJPoints      ijpoints_;
  bool          calcColor_ { false };
  bool          dataColor_ { false };
};

#endif
