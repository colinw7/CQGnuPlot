#ifndef CGnuPlotPm3DData_H
#define CGnuPlotPm3DData_H

class CGnuPlotPm3DData {
 public:
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

 public:
  CGnuPlotPm3DData() { }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  static std::string posTypeToStr(const PosType &p) {
    if      (p == PosType::SURFACE) return "SURFACE";
    else if (p == PosType::TOP    ) return "TOP";
    else if (p == PosType::BOTTOM ) return "BOTTOM";
    else                            return "???";
  }

  void clearPos() {
    pos_.clear();
  }

  void addPos(PosType pos) {
    pos_.push_back(pos);
  }

  void setMapPos() {
    pos_.clear();
    pos_.push_back(PosType::SURFACE);
  }

  void setSteps(int s1, int s2) {
    isteps1_ = s1;
    isteps2_ = s2;
  }

  void setScanType(ScanType type) {
    scanType_ = type;
  }

  void setFlushType(FlushType type) {
    flushType_ = type;
  }

  void setCornerType(CornerType type) {
    cornerType_ = type;
  }

  void setFTriangles(bool b) {
    ftriangles_ = b;
  }

  void setClipIn(bool b) {
    clipin_ = b;
  }

  void setLineType(int lt) {
    linetype_ = lt;
  }

  void unsetLineType() {
    linetype_ = -1;
  }

  void setImplicit(bool b) {
    implicit_ = b;
  }

  void show(std::ostream &os) const {
    os << "pm3d style is " << (implicit_ ? "implicit" : "explicit");
    os << " (draw pm3d surface according to style)" << std::endl;

    os << "pm3d plotted at";
    if (pos_.empty())
      os << " SURFACE";
    else {
      for (uint i = 0; i < pos_.size(); ++i) {
        if (i > 0) os << ", then";

        os << " " << posTypeToStr(pos_[i]);
      }
    }
    os << std::endl;

    if      (scanType_ == ScanType::AUTOMATIC)
      os << "taking scans direction automatically" << std::endl;
    else if (scanType_ == ScanType::FORWARD)
      os << "taking scans in FORWARD direction" << std::endl;
    else if (scanType_ == ScanType::BACKWARD)
      os << "taking scans in BACKWARD direction" << std::endl;
    else if (scanType_ == ScanType::DEPTH_ORDER)
      os << "true depth ordering" << std::endl;

    os << "subsequent scans with different nb of pts are flushed from";
    if      (flushType_ == FlushType::BEGIN ) os << "BEGIN"  << std::endl;
    else if (flushType_ == FlushType::CENTER) os << "CENTER" << std::endl;
    else if (flushType_ == FlushType::END   ) os << "END"    << std::endl;

    if (ftriangles_)
      os << "flushing triangles are drawn" << std::endl;
    else
      os << "flushing triangles are not drawn" << std::endl;

    if (clipin_ == 4)
      os << "clipping: all 4 points of the quadrangle in x,y ranges" << std::endl;
    else
      os << "clipping: at least 1 point of the quadrangle in x,y ranges" << std::endl;

    if (linetype_ != -1)
      os << "pm3d quadrangle borders will default to linetype " << linetype_ << std::endl;
    else
      os << "pm3d quadrangles will have no border" << std::endl;

    os << "steps for bilinear interpolation: " << isteps1_ << "," << isteps2_ << std::endl;

    if      (cornerType_ == CornerType::MEAN)
      os << "quadrangle color according to averaged 4 corners" << std::endl;
    else if (cornerType_ == CornerType::GEOMEAN)
      os << "quadrangle color according to geometrical mean of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::HARMEAN)
      os << "quadrangle color according to harmonic mean of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::RMS)
      os << "quadrangle color according to root mean square of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::MEDIAN)
      os << "quadrangle color according to median of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::MIN)
      os << "quadrangle color according to min of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::MAX)
      os << "quadrangle color according to max of 4 corners" << std::endl;
    else if (cornerType_ == CornerType::C1)
      os << "quadrangle color according to corner 1" << std::endl;
    else if (cornerType_ == CornerType::C2)
      os << "quadrangle color according to corner 2" << std::endl;
    else if (cornerType_ == CornerType::C3)
      os << "quadrangle color according to corner 3" << std::endl;
    else if (cornerType_ == CornerType::C4)
      os << "quadrangle color according to corner 4" << std::endl;
  }

  void unset() {
    enabled_  = false;
    implicit_ = false;
    linetype_ = -1;
  }

 private:
  typedef std::vector<PosType> PosTypeList;

  bool             enabled_    { false };
  PosTypeList      pos_;
  int              isteps1_    { -1 };
  int              isteps2_    { -1 };
  ScanType         scanType_   { ScanType::AUTOMATIC };
  FlushType        flushType_  { FlushType::BEGIN };
  bool             ftriangles_ { false };
  int              clipin_     { 4 };
  CornerType       cornerType_ { CornerType::MEAN };
  int              linetype_   { -1 };
  bool             implicit_   { false };
};

#endif
