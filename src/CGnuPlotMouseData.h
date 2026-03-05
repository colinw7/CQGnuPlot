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
  void resetDoubleClick() { dclick_.reset(); }

  void setZoomCoordinates(bool b) { zoomCoords_ = b; }

  void setZoomFactors(double x, double y) { zoomX_ = x; zoomY_ = y; }

  void setRulerPos(const CPoint2D &p) { rulerPos_ = p; }
  void resetRulerPos() { rulerPos_.reset(); }

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
      os << "mouse is on\n";

      if (zoomCoords_)
        os << "zoom coordinates will be drawn\n";
      else
        os << "no zoom coordinates will be drawn\n";

      if (polarDistType_ == PolarDistanceType::NONE)
        os << "no polar distance to ruler will be shown\n";
      else
        os << "distance to ruler will be show in polar coordinates\n";

      if (dclick_)
        os << "double click resolution is " << dclick_.value() << " ms\n";
      else
        os << "double click resolution is 300 ms\n";

      if (format_)
        os << "formatting numbers with \"" << format_.value() << "\"\n";
      else
        os << "formatting numbers with \"%g\"\n";

      if (mouseFormatInt_)
        os << "format for Button 2 is " << mouseFormatInt_.value() << "\n";
      else
        os << "format for Button 2 is 0\n";

      if (mouseFormatStr_ != "")
        os << "Button 2 draws persistent labels with options \"" <<
              mouseFormatStr_ << "\"\n";

      if (! zoomX_ || ! zoomY_)
        os << "zoom factors are x: 1   y: 1\n";
      else
        os << "zoom factors are x: " << zoomX_.value() << "   y: " << zoomY_.value() << "\n";

      if (zoomJump_)
        os << "zoomjump is on\n";
      else
        os << "zoomjump is off\n";

      if (verbose_)
        os << "communication commands will be shown\n";
      else
        os << "communication commands will not be shown\n";
    }
    else
      os << "mouse is off\n";
  }

 private:
  bool                       enabled_        { true };
  std::optional<double>      dclick_;
  bool                       zoomCoords_     { true };
  std::optional<double>      zoomX_;
  std::optional<double>      zoomY_;
  std::optional<CPoint2D>    rulerPos_;
  PolarDistanceType          polarDistType_  { PolarDistanceType::NONE };
  std::optional<std::string> format_;
  std::string                mouseFormatStr_;
  std::optional<int>         mouseFormatInt_;
  std::string                labels_;
  bool                       zoomJump_       { false };
  bool                       verbose_        { false };
};

#endif
