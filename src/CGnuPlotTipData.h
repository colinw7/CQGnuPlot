#ifndef CGnuPlotTipData_H
#define CGnuPlotTipData_H

class CGnuPlotTipData {
 public:
  explicit CGnuPlotTipData(const std::string &xstr="", const CBBox2D &bbox=CBBox2D()) :
   xstr_(xstr), bbox_(bbox) {
  }

  const std::string &xstr() const { return xstr_; }
  void setXStr(const std::string &s) { xstr_ = s; }

  const std::string &ystr() const { return ystr_; }
  void setYStr(const std::string &s) { ystr_ = s; }

  const CRGBA &xcolor() const { return xcolor_; }
  void setXColor(const CRGBA &v) { xcolor_ = v; }

  const CRGBA &ycolor() const { return ycolor_; }
  void setYColor(const CRGBA &v) { ycolor_ = v; }

  const CRGBA &borderColor() const { return borderColor_; }
  void setBorderColor(const CRGBA &v) { borderColor_ = v; }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &r) { bbox_ = r; }

 private:
  std::string xstr_;
  std::string ystr_;
  CRGBA       xcolor_      { CRGBA(0,0,0) };
  CRGBA       ycolor_      { CRGBA(0,0,0) };
  CRGBA       borderColor_ { CRGBA(0,0,0) };
  CBBox2D     bbox_;
};

#endif
