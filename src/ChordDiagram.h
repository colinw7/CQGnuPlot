#ifndef ChordDiagram_H
#define ChordDiagram_H

#include <CRGBA.h>
#include <CAlignType.h>
#include <CBBox2D.h>
#include <string>
#include <vector>
#include <algorithm>

class ChordDiagramRenderer;

class ChordDiagram {
 public:
  ChordDiagram();

  void init();

  void setDimension(int n);

  void setValue(int i, int j, double value);

  void setName (int i, const std::string &name);
  void setColor(int i, const CRGBA &c);

  void draw(ChordDiagramRenderer *);

 public:
  class ValueSet {
   public:
    ValueSet() : name_(), color_(0.5,0.5,0.5), r1_(1), r2_(1), angle_(0), dangle_(90) { }

    void resize(int n) {
      values_.resize(n);
    }

    void setValue(int i, double value) {
      values_[i] = value;
    }

    double value(int i) const {
      return values_[i];
    }

    double sortedValue(int i) const {
      return values_[sortedInds_[i]];
    }

    void setName(const std::string &name) { name_ = name; }
    const std::string &name() const { return name_; }

    const CRGBA &color() const { return color_; }
    void setColor(const CRGBA &color) { color_ = color; }

    double total(int n) const {
      double total = 0.0;

      for (int i = 0; i < n; ++i)
        total += value(i);

      return total;
    }

    double sortedTotal(int n) const {
      double total = 0.0;

      for (int i = 0; i < n; ++i)
        total += sortedValue(i);

      return total;
    }

    void setRadii(double r1, double r2) {
      assert(r1 <= r2);

      r1_ = r1;
      r2_ = r2;
    }

    void setAngles(double angle, double dangle) {
      angle_  = angle;
      dangle_ = dangle;
    }

    double angle1() const { return angle_; }
    double angle2() const { return angle_ + dangle_; }

    static double normalizeAngle(double a) {
      while (a <    0.0) a += 360.0;
      while (a >= 360.0) a -= 360.0;

      return a;
    }

    void sortInds() {
      sortedInds_.resize(values_.size());

      for (uint i = 0; i < values_.size(); ++i)
        sortedInds_[i] = i;

      std::sort(sortedInds_.begin(), sortedInds_.end(), IndCmp(*this));
    }

    int sortedInd(int i) {
      return sortedInds_[i];
    }

    int sortedIndToInd(int ind) {
      for (uint i = 0; i < values_.size(); ++i)
        if (sortedInds_[i] == ind)
          return i;

      return -1;
    }

    bool pointInside(double x, double y) {
      double r = sqrt(x*x + y*y);

      if (r < r1_ || r > r2_) return false;

      double a = normalizeAngle(180.0*atan2(y, x)/M_PI);

      double a1 = normalizeAngle(angle_);
      double a2 = a1 + dangle_;

      if (a2 > a1) {
        if (a2 >= 360.0) {
          double da = a2 - 360.0; a -= da; a1 -= -da; a2 = 360.0;
          a = normalizeAngle(a);
        }

        if (a < a1 || a > a2)
          return false;
      }
      else {
        if (a2 < 0.0) {
          double da = -a2; a += da; a1 += da; a2 = 0.0;

          a = normalizeAngle(a);
        }

        if (a < a2 || a > a1)
          return false;
      }

      return true;
    }

   private:
    struct IndCmp {
      IndCmp(ValueSet &v) : values(v) { }

      bool operator()(int i1, int i2) {
        return values.value(i1) > values.value(i2);
      }

      ValueSet &values;
    };

   private:
    typedef std::vector<int> Inds;

    std::vector<double> values_;
    std::string         name_;
    CRGBA              color_;
    Inds                sortedInds_;
    double              r1_, r2_;
    double              angle_, dangle_;
  };

  typedef std::vector<ValueSet> ValueSets;

 private:
  typedef std::vector<std::string> Names;

  Names     names_;
  ValueSets valueSets_;
  double    valueToDegrees_ = 1.0;
  int       currentInd_ = -1;
};

class ChordDiagramRenderer {
 public:
  ChordDiagramRenderer() { }

  virtual ~ChordDiagramRenderer() { }

  virtual double pixelLengthToWindowLength(double l) = 0;

  virtual void drawLine(double x1, double y1, double x2, double y2) = 0;

  virtual void drawRotatedText(double x, double y, const std::string &text, double a,
                               CHAlignType halign, CVAlignType valign) = 0;

  virtual void drawArc(const CPoint2D &p, double r1, double r2, double a1, double a2,
                       const CRGBA &c) = 0;

  virtual void drawChord(const CPoint2D &p, double r, double a1, double a2,
                         const CRGBA &c) = 0;

  virtual void drawComplexChord(const CPoint2D &p, double r, double a11, double a12,
                                double a21, double a22, const CRGBA &c) = 0;
};

#endif
