#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

#include <COptVal.h>
#include <CGnuPlotTypes.h>

class CGnuPlotImageStyle {
 public:
  typedef CGnuPlotTypes::ImageType ImageType;
  typedef std::vector<double>      Reals;
  typedef std::vector<CPoint3D>    Points;

 public:
  CGnuPlotImageStyle(CGnuPlot *plot) :
   plot_(plot), usingCols_(plot_) {
  }

  const COptInt &width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  const COptInt &height() const { return height_; }
  void setHeight(int h) { height_ = h; }

  const Points &originArray() const { return originArray_; }
  void setOriginArray(const Points &o) { originArray_ = o; }

  const COptPoint3D &center() const { return center_; }
  void setCenter(const CPoint3D &c) { center_ = c; }

  const COptPoint3D &perpendicular() const { return perp_; }
  void setPerpendicular(const CPoint3D &c) { perp_ = c; }

  const Reals &skip() const { return skip_; }
  void setSkip(const Reals &v) { skip_ = v; }

  const COptReal &dx() const { return dx_; }
  void setDX(double r) { dx_ = r; }

  const COptReal &dy() const { return dy_; }
  void setDY(double r) { dy_ = r; }

  const COptReal &dt() const { return dt_; }
  void setDT(double r) { dt_ = r; }

  const COptReal &angle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  bool isFlipY() const { return flipY_; }
  void setFlipY(bool b) { flipY_ = b; }

  const CGnuPlotUsingCols &usingCols() const { return usingCols_; }
  void setUsingCols(const CGnuPlotUsingCols &v) { usingCols_ = v; }

  const ImageType &fileType() const { return fileType_; }
  void setFileType(const ImageType &v) { fileType_ = v; }

  const std::string &filename() const { return filename_; }
  void setFilename(const std::string &v) { filename_ = v; }

  COptPoint3D origin() const {
    COptPoint3D o;

    if (! originArray_.empty())
      o = COptPoint3D(originArray_[0]);

    return o;
  }

  void reset() {
    width_ .setInvalid();
    height_.setInvalid();
    center_.setInvalid();
    perp_  .setInvalid();
    dx_    .setInvalid();
    dy_    .setInvalid();
    dy_    .setInvalid();
    angle_ .setInvalid();

    originArray_.clear();

    flipY_     = false;
    usingCols_ = CGnuPlotUsingCols(plot_);
    fileType_  = CGnuPlotTypes::ImageType::NONE;

    filename_ = "";
  }

 private:
  CGnuPlot*         plot_ { 0 };
  COptInt           width_;
  COptInt           height_;
  Points            originArray_;
  COptPoint3D       center_;
  COptPoint3D       perp_;
  Reals             skip_;
  COptReal          dx_, dy_, dt_;
  COptReal          angle_;
  bool              flipY_ { false };
  CGnuPlotUsingCols usingCols_;
  ImageType         fileType_ { CGnuPlotTypes::ImageType::NONE };
  std::string       filename_;
};

#endif
