#ifndef CGnuPlotImageStyle_H
#define CGnuPlotImageStyle_H

#include <COptVal.h>
#include <CGnuPlotTypes.h>

class CGnuPlotImageStyle {
 public:
  typedef CGnuPlotTypes::ImageType ImageType;

 public:
  CGnuPlotImageStyle() { }

  const COptInt &width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  const COptInt &height() const { return height_; }
  void setHeight(int h) { height_ = h; }

  const COptPoint2D &origin() const { return origin_; }
  void setOrigin(const CPoint2D &o) { origin_ = o; }

  const COptPoint2D &center() const { return center_; }
  void setCenter(const CPoint2D &c) { center_ = c; }

  const COptReal &dx() const { return dx_; }
  void setDX(double r) { dx_ = r; }

  const COptReal &dy() const { return dy_; }
  void setDY(double r) { dy_ = r; }

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

  void reset() {
    width_ .setInvalid();
    height_.setInvalid();
    origin_.setInvalid();
    center_.setInvalid();
    dx_    .setInvalid();
    dy_    .setInvalid();
    angle_ .setInvalid();

    flipY_     = false;
    usingCols_ = CGnuPlotUsingCols();
    fileType_  = CGnuPlotTypes::ImageType::NONE;

    filename_ = "";
  }

 private:
  COptInt           width_;
  COptInt           height_;
  COptPoint2D       origin_;
  COptPoint2D       center_;
  COptReal          dx_, dy_;
  COptReal          angle_;
  bool              flipY_ { false };
  CGnuPlotUsingCols usingCols_;
  ImageType         fileType_ { CGnuPlotTypes::ImageType::NONE };
  std::string       filename_;
};

#endif
