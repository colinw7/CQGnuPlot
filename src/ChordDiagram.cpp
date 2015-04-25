#include <ChordDiagram.h>

#if 0
namespace {
CRGBA colors[] = {
  CRGBA("#000000"), // black
  CRGBA("#FFDD89"), // blond
  CRGBA("#957244"), // brown
  CRGBA("#F26223"), // red
};

// cells are ribbons
const int num_sets = 4;

int set_data[num_sets][num_sets] = {
            /* black, blond, brown,  red */
  /* black */ {11975,  5871,  8916, 2868},
  /* blond */ { 1951, 10048,  2060, 6171},
  /* brown */ { 8010, 16145,  8090, 8045},
  /* red   */ { 1013,   990,   940, 6907}
};
#endif

class Axis {
 public:
  Axis(double start_value, double end_value, double minor_delta_value, double major_delta_value,
       double radius, double start_angle, double end_angle) :
   start_value_(start_value), end_value_(end_value), minor_delta_value_(minor_delta_value),
   major_delta_value_(major_delta_value), radius_(radius), start_angle_(start_angle),
   end_angle_(end_angle) {
  }

  void draw(ChordDiagramRenderer *renderer) {
    double dv = end_value_ - start_value_;
    double da = end_angle_ - start_angle_;

    double dangle1 = da*(major_delta_value_/dv);
    double dangle2 = da*(minor_delta_value_/dv);

    //---

    double dr = renderer->pixelLengthToWindowLength(4);

    //---

    double radius1 = radius_ + dr;

    for (double angle = start_angle_, value = start_value_;
           value <= end_value_; angle += dangle2, value += minor_delta_value_) {
      double ra = angle*M_PI/180.0;

      double c = cos(ra);
      double s = sin(ra);

      double x1 = radius_*c;
      double y1 = radius_*s;
      double x2 = radius1*c;
      double y2 = radius1*s;

      renderer->drawLine(x1, y1, x2, y2);
    }

    char buffer[32];

    double radius2 = radius_ + 2*dr;

    for (double angle = start_angle_, value = start_value_;
           value <= end_value_; angle += dangle1, value += major_delta_value_) {
      double ra = angle*M_PI/180.0;

      double c = cos(ra);
      double s = sin(ra);

      sprintf(buffer, "%dk", int(value/1000.0));

      double x1 = radius_*c;
      double y1 = radius_*s;
      double x2 = radius2*c;
      double y2 = radius2*s;

      renderer->drawLine(x1, y1, x2, y2);

      if (c >= 0)
        renderer->drawRotatedText(x2, y2, buffer, angle,
                                  CHALIGN_TYPE_LEFT, CVALIGN_TYPE_CENTER);
      else
        renderer->drawRotatedText(x2, y2, buffer, 180.0 + angle,
                                  CHALIGN_TYPE_RIGHT, CVALIGN_TYPE_CENTER);
    }
  }

 private:
  double start_value_, end_value_;
  double minor_delta_value_, major_delta_value_;
  double radius_, start_angle_, end_angle_;
};

ChordDiagram::
ChordDiagram()
{
}

void
ChordDiagram::
init()
{
  double total = 0.0;

  int n = valueSets_.size();

  for (int i = 0; i < n; ++i) {
    ValueSet &values = valueSets_[i];

    values.sortInds();

    total += values.total(n);
  }

  // 360 degree circle, minus 2 degree (gap) per set
  double drange = 360 - n*2;

  // divide remaining degrees by total to get value->degrees factor
  valueToDegrees_ = drange/total;
}

void
ChordDiagram::
setDimension(int n)
{
  valueSets_.resize(n);

  for (int i = 0; i < n; ++i)
    valueSets_[i].resize(n);
}

void
ChordDiagram::
setValue(int i, int j, double value)
{
  valueSets_[i].setValue(j, value);
}

void
ChordDiagram::
setName(int i, const std::string &name)
{
  valueSets_[i].setName(name);
}

void
ChordDiagram::
setColor(int i, const CRGBA &c)
{
  valueSets_[i].setColor(c);
}

void
ChordDiagram::
draw(ChordDiagramRenderer *renderer)
{
  //font.setPointSizeF(6.0);
  //p.setFont(font);

  //---

  double dr = renderer->pixelLengthToWindowLength(32);

  //---

  double xc = 0.0;
  double yc = 0.0;
  double r1 = 1.00 - dr;
  double r2 = r1*0.9;

  //---

  double angle1 = 90.0;

  int n = valueSets_.size();

  for (int i = 0; i < n; ++i) {
    ValueSet &values = valueSets_[i];

    double total = values.total(n);

    double dangle = -valueToDegrees_*total;

    double angle2 = angle1 + dangle;

    renderer->drawArc(CPoint2D(xc, yc), r1, r2, angle1, angle2, values.color());

    values.setAngles(angle1, dangle);
    values.setRadii (r2, r1);

    //------

    Axis axis(0, total, 1000, 5000, r1, angle1, angle2);

    axis.draw(renderer);

    //------

    angle1 = angle2 - 2;
  }

  // foreach value set
  for (int i = 0; i < n; ++i) {
    bool current = (currentInd_ == -1 || i == currentInd_);

    ValueSet &values1 = valueSets_[i];

    // foreach sub value of set (sorted)
    for (int j = 0; j < n; ++j) {
      int indj = values1.sortedInd(j);

      ValueSet &values2 = valueSets_[indj];

      int j1 = values2.sortedIndToInd(i);

#if 0
      CRGBA fc = (values1.sortedValue(j) < values2.sortedValue(j1) ?
                   values1.color() : values2.color());
#else
      CRGBA fc = (values1.sortedValue(j) > values2.sortedValue(j1) ?
                   values1.color() : values2.color());
#endif

      int alpha = (current ? 128 : 32);

      fc.setAlpha(alpha/255.0);

      // connect arc i, value set_data[i][j] to arc j value set_data[j][i];
      if (j == j1) {
        double angle1 = values1.angle1();

        double dangle1 = -values1.sortedTotal(j    )*valueToDegrees_;
        double dangle2 = -values1.sortedTotal(j + 1)*valueToDegrees_;

        double angle2 = angle1 + dangle1;
        double angle3 = angle1 + dangle2;

        renderer->drawChord(CPoint2D(xc, yc), r2, angle2, angle3, fc);
      }
      else {
        double angle1 = values1.angle1();
        double angle2 = values2.angle1();

        double dangle11 = -values1.sortedTotal(j     )*valueToDegrees_;
        double dangle12 = -values1.sortedTotal(j  + 1)*valueToDegrees_;
        double dangle21 = -values2.sortedTotal(j1    )*valueToDegrees_;
        double dangle22 = -values2.sortedTotal(j1 + 1)*valueToDegrees_;

        double angle11 = angle1 + dangle11;
        double angle12 = angle1 + dangle12;
        double angle21 = angle2 + dangle21;
        double angle22 = angle2 + dangle22;

        renderer->drawChord(CPoint2D(xc, yc), r2, angle11, angle12, angle21, angle22, fc);
      }
    }
  }
}
