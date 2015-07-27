#ifndef CGnuPlotMouseData_H
#define CGnuPlotMouseData_H

class CGnuPlotMouseData {
 public:
  enum class PolarDistanceType {
    NONE,
    DEG,
    TAN
  };

 public:
  CGnuPlotMouseData() { }

  bool enabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  void setDoubleClick(double ms) { dclick_ = ms; }
  void resetDoubleClick() { dclick_.setInvalid(); }

  void setZoomCoordinates(bool b) { zoomCoords_ = b; }

  void setZoomFactors(double x, double y) { zoomX_ = x; zoomY_ = y; }

  void setRulerPos(const CPoint2D &p) { rulerPos_ = p; }
  void resetRulerPos() { rulerPos_.setInvalid(); }

  void setPolarDistance(PolarDistanceType type) { polarDistType_ = type; }

  void setFormat(const std::string &fmt) { format_ = fmt; }

  void setMouseFormat(const std::string &fmt) { mouseFormatStr_ = fmt; }
  void setMouseFormat(int i) { mouseFormatInt_ = i; }

  void setLabels(const std::string &str) { labels_ = str; }
  void resetLabels() { labels_ = ""; }

  bool zoomJump() const { return zoomJump_; }
  void setZoomJump(bool b) { zoomJump_ = b; }

  bool verbose() const { return verbose_; }
  void setVerbose(bool b) { verbose_ = b; }

  void show(std::ostream &os) {
    if (enabled_) {
      os << "mouse is on" << std::endl;

      if (zoomCoords_)
        os << "zoom coordinates will be drawn" << std::endl;
      else
        os << "no zoom coordinates will be drawn" << std::endl;

      if (polarDistType_ == PolarDistanceType::NONE)
        os << "no polar distance to ruler will be shown" << std::endl;
      else
        os << "distance to ruler will be show in polar coordinates" << std::endl;

      if (dclick_.isValid())
        os << "double click resolution is " << dclick_.getValue() << " ms" << std::endl;
      else
        os << "double click resolution is 300 ms" << std::endl;

      if (format_.isValid())
        os << "formatting numbers with \"" << format_.getValue() << "\"" << std::endl;
      else
        os << "formatting numbers with \"%g\"" << std::endl;

      if (mouseFormatInt_.isValid())
        os << "format for Button 2 is " << mouseFormatInt_.getValue() << std::endl;
      else
        os << "format for Button 2 is 0" << std::endl;

      if (mouseFormatStr_ != "")
        os << "Button 2 draws persistent labels with options \"" <<
              mouseFormatStr_ << "\"" << std::endl;

      if (! zoomX_.isValid() || ! zoomY_.isValid())
        os << "zoom factors are x: 1   y: 1" << std::endl;
      else
        os << "zoom factors are x: " << zoomX_.getValue() <<
              "   y: " << zoomY_.getValue() << std::endl;

      if (zoomJump_)
        os << "zoomjump is on" << std::endl;
      else
        os << "zoomjump is off" << std::endl;

      if (verbose_)
        os << "communication commands will be shown" << std::endl;
      else
        os << "communication commands will not be shown" << std::endl;
    }
    else
      os << "mouse is off" << std::endl;
  }

 private:
  bool              enabled_        { true };
  COptReal          dclick_;
  bool              zoomCoords_     { true };
  COptReal          zoomX_;
  COptReal          zoomY_;
  COptPoint2D       rulerPos_;
  PolarDistanceType polarDistType_  { PolarDistanceType::NONE };
  COptString        format_;
  std::string       mouseFormatStr_;
  COptInt           mouseFormatInt_;
  std::string       labels_;
  bool              zoomJump_       { false };
  bool              verbose_        { false };
};

#endif
