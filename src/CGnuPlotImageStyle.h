#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

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

  const std::optional<int> &width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  const std::optional<int> &height() const { return height_; }
  void setHeight(int h) { height_ = h; }

  const Points &originArray() const { return originArray_; }
  void setOriginArray(const Points &o) { originArray_ = o; }

  const std::optional<CPoint3D> &center() const { return center_; }
  void setCenter(const CPoint3D &c) { center_ = c; }

  const std::optional<CPoint3D> &perpendicular() const { return perp_; }
  void setPerpendicular(const CPoint3D &c) { perp_ = c; }

  const Reals &skip() const { return skip_; }
  void setSkip(const Reals &v) { skip_ = v; }

  const std::optional<double> &dx() const { return dx_; }
  void setDX(double r) { dx_ = r; }

  const std::optional<double> &dy() const { return dy_; }
  void setDY(double r) { dy_ = r; }

  const std::optional<double> &dt() const { return dt_; }
  void setDT(double r) { dt_ = r; }

  const std::optional<double> &angle() const { return angle_; }
  void setAngle(double a) { angle_ = a; }

  bool isFlipY() const { return flipY_; }
  void setFlipY(bool b) { flipY_ = b; }

  const CGnuPlotUsingCols &usingCols() const { return usingCols_; }
  void setUsingCols(const CGnuPlotUsingCols &v) { usingCols_ = v; }

  const ImageType &fileType() const { return fileType_; }
  void setFileType(const ImageType &v) { fileType_ = v; }

  const std::string &filename() const { return filename_; }
  void setFilename(const std::string &v) { filename_ = v; }

  std::optional<CPoint3D> origin() const {
    std::optional<CPoint3D> o;

    if (! originArray_.empty())
      o = originArray_[0];

    return o;
  }

  void reset() {
    width_ .reset();
    height_.reset();
    center_.reset();
    perp_  .reset();
    dx_    .reset();
    dy_    .reset();
    dy_    .reset();
    angle_ .reset();

    originArray_.clear();

    flipY_     = false;
    usingCols_ = CGnuPlotUsingCols(plot_);
    fileType_  = CGnuPlotTypes::ImageType::NONE;

    filename_ = "";
  }

 private:
  CGnuPlot*               plot_ { nullptr };
  std::optional<int>      width_;
  std::optional<int>      height_;
  Points                  originArray_;
  std::optional<CPoint3D> center_;
  std::optional<CPoint3D> perp_;
  Reals                   skip_;
  std::optional<double>   dx_, dy_, dt_;
  std::optional<double>   angle_;
  bool                    flipY_ { false };
  CGnuPlotUsingCols       usingCols_;
  ImageType               fileType_ { CGnuPlotTypes::ImageType::NONE };
  std::string             filename_;
};

#endif
