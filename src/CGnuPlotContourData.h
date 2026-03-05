#ifndef CGnuPlotContourData_H
#define CGnuPlotContourData_H

#include <CGnuPlotUtil.h>
#include <CFont.h>
#include <iostream>
#include <optional>

class CGnuPlotContourData {
 public:
  enum class DrawPos {
    BASE,
    SURFACE,
    BOTH
  };

  enum class Style {
    LINEAR,
    CUBICSPLINE,
    BSPLINE
  };

  typedef std::vector<double> Levels;

  struct Range {
    bool                  set   { false };
    double                start { 0 };
    double                incr  { 1 };
    std::optional<double> end;

    Range() :
     set(false) {
    }

    Range(double start1, double incr1) :
     set(true), start(start1), incr(incr1) {
       end = std::optional<double>();
    }

    Range(double start1, double incr1, double end1) :
     set(true), start(start1), incr(incr1), end(end1) {
    }

    void reset() {
      set = false;
    }
  };

 public:
  CGnuPlotContourData() { }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  DrawPos pos() const { return pos_; }
  void setPos(DrawPos pos) { pos_ = pos; }

  const Style &style() const { return style_; }
  void setStyle(const Style &v) { style_ = v; }

  int points() const { return points_; }
  void setPoints(int i) { points_ = i; }

  int order() const { return order_; }
  void setOrder(int i) { order_ = i; }

  const std::optional<int> &levels() const { return levels_; }
  void setLevels(int i) {
    autoLevels_ = std::optional<int>(); discreteLevels_.clear(); levels_ = i; }

  const std::optional<int> &autoLevels() const { return autoLevels_; }
  void setAutoLevels(int i) {
    levels_ = std::optional<int>(); discreteLevels_.clear(); autoLevels_ = i; }

  const Levels &discreteLevels() const { return discreteLevels_; }
  void setDiscreteLevels(const Levels &v) { incrementalRange_.reset(); discreteLevels_ = v; }

  const Range &incrementalRange() const { return incrementalRange_; }
  void setIncrementalRange(const Range &r) { discreteLevels_.clear(), incrementalRange_ = r; }

  const std::string &labelFormat() const { return labelFormat_; }
  void setLabelFormat(const std::string &s) { labelFormat_ = s; }

  const CFontPtr &labelFont() const { return labelFont_; }
  void setLabelFont(const CFontPtr &f) { labelFont_ = f; }

  const std::optional<int> &labelStart() const { return labelStart_; }
  void setLabelStart(int i) { labelStart_ = i; }

  const std::optional<int> &labelInterval() const { return labelInterval_; }
  void setLabelInterval(int i) { labelInterval_ = i; }

  void resetLevels() {
    levels_     = std::optional<int>();
    autoLevels_ = std::optional<int>();

    discreteLevels_  .clear();
    incrementalRange_.reset();
  }

  std::vector<double> getLevelValues(double min, double max) const {
    if      (incrementalRange_.set) {
      if      (levels_)
        return getIncrementalValues(levels_.value());
      else if (autoLevels_)
        return getIncrementalValues(autoLevels_.value());
      else
        return getIncrementalValues(10);
    }
    else if (! discreteLevels_.empty())
      return discreteLevels_;
    else if (levels_)
      return getLevelValues(levels_.value(), min, max);
    else if (autoLevels_)
      return getLevelValues(autoLevels_.value(), min, max);
    else
      return getLevelValues(10, min, max);
  }

  std::vector<double> getLevelValues(int n, double min, double max) const {
    std::vector<double> levels;

    levels.resize(n);

    for (int i = 0; i < n; ++i)
      levels[i] = CGnuPlotUtil::map(i, 0, n - 1, min, max);

    return levels;
  }

  std::vector<double> getIncrementalValues(int n) const {
    if (incrementalRange_.end)
      n = int((incrementalRange_.end.value() - incrementalRange_.start)/
              incrementalRange_.incr + 0.5);

    std::vector<double> levels;

    levels.resize(n);

    for (int i = 0; i < n; ++i)
      levels[i] = incrementalRange_.start + i*incrementalRange_.incr;

    return levels;
  }

  void show(std::ostream &os) const {
    if (! enabled_)
      os << "contour for surfaces are not drawn\n";
    else {
      os << "contour for surfaces are drawn on ";

      if      (pos_ == DrawPos::BASE   )
        os << "base";
      else if (pos_ == DrawPos::SURFACE)
        os << "surface";
      else if (pos_ == DrawPos::BOTH   )
        os << "base and surface";

      os << "\n";
    }
  }

 private:
  bool               enabled_          { false };
  DrawPos            pos_              { DrawPos::BASE };
  Style              style_            { Style::LINEAR };
  int                points_           { 10 }; // number of points to approx curve
  int                order_            { 2 }; // bspline order
  std::optional<int> levels_           { };
  std::optional<int> autoLevels_       { };
  Levels             discreteLevels_   { };
  Range              incrementalRange_ { };
  std::string        labelFormat_;
  CFontPtr           labelFont_;
  std::optional<int> labelStart_;
  std::optional<int> labelInterval_;
};

#endif
