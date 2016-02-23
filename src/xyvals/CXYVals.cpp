/* TODO: Reduce all incoming rectangular boundaries from
         5->4 points to allow quick bbox test for IsInside */

#include <CXYVals.h>
#include <CMathGeom2D.h>
#include <CMathGen.h>
#include <CAssert.h>

/*
 * Get unique X and Y Values for rectilinear polygon
 */

CXYVals::
CXYVals(const double *x, const double *y, uint num_xy)
{
  init(x, y, num_xy);
}

CXYVals::
CXYVals(const double *x1, const double *y1, uint num_xy1,
        const double *x2, const double *y2, uint num_xy2)
{
  init(x1, y1, num_xy1, x2, y2, num_xy2);
}

void
CXYVals::
init(const std::vector<double> &x, std::vector<double> &y)
{
  assert(x.size() == y.size());

  init(&x[0], &y[0], x.size());
}

void
CXYVals::
init(const std::vector<double> &x1, std::vector<double> &y1,
     const std::vector<double> &x2, std::vector<double> &y2)
{
  assert(x1.size() == y1.size());
  assert(x2.size() == y2.size());

  init(&x1[0], &y1[0], x1.size(), &x2[0], &y2[0], x2.size());
}

void
CXYVals::
init(const double *x, const double *y, uint num_xy)
{
  /* Allocate return value for maximum possible size */

  std::vector<double> xvals, yvals;

  xvals.reserve(num_xy);
  yvals.reserve(num_xy);

  /*------*/

  uint num_xvals = 0;
  uint num_yvals = 0;

  for (uint i = 0; i < num_xy; ++i) {
    insertValue(x[i], &xvals[0], num_xvals);
    insertValue(y[i], &yvals[0], num_yvals);
  }

  if (num_xy) {
    xvals_ = std::vector<double>(&xvals[0], &xvals[num_xvals]);
    yvals_ = std::vector<double>(&yvals[0], &yvals[num_yvals]);
  }
  else {
    xvals_.clear();
    yvals_.clear();
  }
}


void
CXYVals::
init(const double *x1, const double *y1, uint num_xy1,
     const double *x2, const double *y2, uint num_xy2)
{
  int num_xy = num_xy1 + num_xy2;

  std::vector<double> x, y;

  x.resize(num_xy);
  y.resize(num_xy);

  if (num_xy1) {
    memcpy(&x[0      ], x1, num_xy1*sizeof(double));
    memcpy(&y[0      ], y1, num_xy1*sizeof(double));
  }

  if (num_xy2) {
    memcpy(&x[num_xy1], x2, num_xy2*sizeof(double));
    memcpy(&y[num_xy1], y2, num_xy2*sizeof(double));
  }

  init(&x[0], &y[0], num_xy);
}
void
CXYVals::
insertValue(double v, double *vals, uint &num_vals)
{
  // handle empty list
  if (num_vals == 0) {
    vals[0] = v;

    ++num_vals;

    return;
  }

  // Find insertion position
  int l = 0;
  int h = num_vals - 1;

  if (CMathGen::realEq(v, vals[l]) || CMathGen::realEq(v, vals[h]))
    return;

  int pos;

  if      (v <= vals[l]) {
    pos = 0;
  }
  else if (v >= vals[h]) {
    pos = num_vals;
  }
  else {
    int m = (l + h)/2;

    while (l != m) {
      if (CMathGen::realEq(v, vals[m]))
        return;

      if (v < vals[m]) h = m;
      else             l = m;

      m = (l + h)/2;
    }

    pos = h;
  }

  // make room
  for (int i = (int) num_vals - 1; i >= pos; --i)
    vals[i + 1] = vals[i];

  // Add to array
  vals[pos] = v;

  ++num_vals;
}

/*------*/

CXYValsInside::
CXYValsInside(const double *x, const double *y, uint num_xy, bool init) :
 CXYVals(x, y, num_xy), num_xvals_(0), num_yvals_(0)
{
  if (init)
    initValues(x, y, num_xy);
}

CXYValsInside::
CXYValsInside(const double *x1, const double *y1, uint num_xy1,
              const double *x2, const double *y2, uint num_xy2, bool init) :
 CXYVals(x1, y1, num_xy1, x2, y2, num_xy2)
{
  if (init)
    initValues(x1, y1, num_xy1, x2, y2, num_xy2);
}

void
CXYValsInside::
initValues(const std::vector<double> &x1, std::vector<double> &y1,
           const std::vector<double> &x2, std::vector<double> &y2)
{
  assert(x1.size() == y1.size());
  assert(x2.size() == y2.size());

  initValues(&x1[0], &y1[0], x1.size(), &x2[0], &y2[0], x2.size());
}

void
CXYValsInside::
initValues(const std::vector<double> &x, std::vector<double> &y)
{
  assert(x.size() == y.size());

  initValues(&x[0], &y[0], x.size());
}

void
CXYValsInside::
initValues(const CXYValsInside &xyvals, const std::vector<double> &x, std::vector<double> &y)
{
  initValues(xyvals, &x[0], &y[0], x.size());
}

void
CXYValsInside::
initValues(const CXYValsInside &xyvals, const double *x, const double *y, uint num_xy)
{
  std::vector<double> xo, yo;

  if (xyvals.getPoly(xo, yo))
    initValues(&xo[0], &yo[0], xo.size(), x, y, num_xy);
}

void
CXYValsInside::
initValues(const double *x, const double *y, uint num_xy)
{
  CXYVals::init(x, y, num_xy);

  initMem();

  if (num_xvals_ <= 0 || num_yvals_ <= 0)
    return;

  for (uint j = 0; j < num_yvals_ - 1; ++j) {
    double ym = CMathGen::avg(yvals_[j], yvals_[j + 1]);

    for (uint i = 0; i < num_xvals_ - 1; ++i) {
      double xm = CMathGen::avg(xvals_[i], xvals_[i + 1]);

      if (CMathGeom2D::PointInsideEvenOdd(xm, ym, x, y, num_xy))
        inside_[i][j] |= INSIDE1;
    }
  }
}

void
CXYValsInside::
initValues(const double *x1, const double *y1, uint num_xy1,
           const double *x2, const double *y2, uint num_xy2)
{
  CXYVals::init(x1, y1, num_xy1, x2, y2, num_xy2);

  initMem();

  if (num_xvals_ <= 0 || num_yvals_ <= 0)
    return;

  for (uint j = 0; j < num_yvals_ - 1; ++j) {
    double ym = CMathGen::avg(yvals_[j], yvals_[j + 1]);

    for (uint i = 0; i < num_xvals_ - 1; ++i) {
      double xm = CMathGen::avg(xvals_[i], xvals_[i + 1]);

      if (CMathGeom2D::PointInsideEvenOdd(xm, ym, x1, y1, num_xy1))
        inside_[i][j] |= INSIDE1;

      if (CMathGeom2D::PointInsideEvenOdd(xm, ym, x2, y2, num_xy2))
        inside_[i][j] |= INSIDE2;
    }
  }
}

void
CXYValsInside::
initMem()
{
  num_xvals_ = xvals_.size();
  num_yvals_ = yvals_.size();

  /* create inside flag grid */
  if (num_xvals_ > 0 && num_yvals_ > 0) {
    inside_.resize(num_xvals_ - 1);

    for (uint i = 0; i < num_xvals_ - 1; ++i)
      inside_[i].resize(num_yvals_ - 1);
  }
  else
    inside_.clear();
}

#define POLY_IS_INSIDE(i,j,v) \
  ((i) >= 0 && (i) < int(num_xvals_ - 1) && \
   (j) >= 0 && (j) < int(num_yvals_ - 1) && \
   (inside_[i][j] == (v)))

class CXYEdge {
 private:
  class Point {
   private:
    int i_, j_;

   public:
    Point(int i, int j) :
     i_(i), j_(j) {
    }

    friend bool operator==(const Point &p1, const Point &p2) {
      return (p1.i_ == p2.i_ && p1.j_ == p2.j_);
    }

    friend bool operator<(const Point &p1, const Point &p2) {
      return (p1.i_ < p2.i_ || (p1.i_ == p2.i_ && p1.j_ < p2.j_));
    }
  };

  Point p1_;
  Point p2_;

 public:
  CXYEdge(int i1, int j1, int i2, int j2) :
   p1_(i1, j1), p2_(i2, j2) {
    if (p2_ < p1_)
      std::swap(p1_, p2_);
  }

  friend bool operator==(const CXYEdge &e1, const CXYEdge &e2) {
    return (e1.p1_ == e2.p1_ && e1.p2_ == e2.p2_);
  }

  friend bool operator<(const CXYEdge &e1, const CXYEdge &e2) {
    return (e1.p1_ < e2.p1_ || (e1.p1_ == e2.p1_ && e1.p2_ < e2.p2_));
  }
};

class CXYEdgeList {
 private:
  typedef std::set<CXYEdge> EdgeSet;

  EdgeSet edgeSet_;

 public:
  CXYEdgeList() { }

  void addEdge(int i1, int j1, int i2, int j2) {
    edgeSet_.insert(CXYEdge(i1, j1, i2, j2));
  }

  bool hasEdge(int i1, int j1, int i2, int j2) {
    return (edgeSet_.find(CXYEdge(i1, j1, i2, j2)) != edgeSet_.end());
  }
};

void
CXYValsInside::
setInside(int val)
{
  if (num_xvals_ <= 0 || num_yvals_ <= 0)
    return;

  for (uint j = 0; j < num_yvals_ - 1; ++j) {
    for (uint i = 0; i < num_xvals_ - 1; ++i) {
      inside_[i][j] = val;
    }
  }
}

void
CXYValsInside::
combineInside(int val)
{
  if (num_xvals_ <= 0 || num_yvals_ <= 0)
    return;

  for (uint j = 0; j < num_yvals_ - 1; ++j) {
    for (uint i = 0; i < num_xvals_ - 1; ++i) {
      if ((inside_[i][j] & INSIDE1) || (inside_[i][j] & INSIDE2))
        inside_[i][j] = val;
    }
  }
}

bool
CXYValsInside::
getPoly(double **xo, double **yo, uint *num_xyo, bool check_consistent) const
{
  return getPoly(INSIDE1, xo, yo, num_xyo, check_consistent);
}

bool
CXYValsInside::
getPoly(int inside_val, double **xo, double **yo, uint *num_xyo, bool check_consistent) const
{
  static std::vector<double> rect_temp_x, rect_temp_y;

  bool rc = getPoly(inside_val, rect_temp_x, rect_temp_y, check_consistent);

  if (rc) {
    if (xo     ) *xo      = &rect_temp_x[0];
    if (yo     ) *yo      = &rect_temp_y[0];
    if (num_xyo) *num_xyo = rect_temp_x.size();
  }
  else {
    if (xo     ) *xo      = 0;
    if (yo     ) *yo      = 0;
    if (num_xyo) *num_xyo = 0;
  }

  return rc;
}

bool
CXYValsInside::
getPoly(std::vector<double> &x, std::vector<double> &y, bool check_consistent) const
{
  return getPoly(INSIDE1, x, y, check_consistent);
}

bool
CXYValsInside::
getPoly(int inside_val, std::vector<double> &x, std::vector<double> &y,
        bool check_consistent) const
{
  uint new_max_xy = num_xvals_*num_yvals_;

  x.reserve(new_max_xy);
  y.reserve(new_max_xy);

  CXYEdgeList edgeList;

  /*------*/

  /* Find Bottom Left of inside coords */

  bool found = false;

  uint i, j;

  for (j = 0; j < num_yvals_ - 1; ++j) {
    for (i = 0; i < num_xvals_ - 1; ++i) {
      if (inside_[i][j] == inside_val) {
        found = true;
        break;
      }
    }

    if (found)
      break;
  }

  if (! found)
    return false;

  int i1 = i;
  int j1 = j;

  /*------*/

  /* Walk alternate X and Y directions to trace boundary */

  int i2 = i1;
  int j2 = j1;

  uint num_xy = 0;

  x[num_xy] = xvals_[i2];
  y[num_xy] = yvals_[j2];

  ++num_xy;

  while (num_xy < new_max_xy) {
    uint save_num_xy = num_xy;

    /*------*/

    bool r_ok = (i2 < (int) num_xvals_ - 1 && ! edgeList.hasEdge(i2, j2, i2 + 1, j2));
    bool l_ok = (i2 >                    0 && ! edgeList.hasEdge(i2, j2, i2 - 1, j2));

    if (r_ok) {
      /* Walk left to right */

      int i = i2;

      for ( ; i < (int) num_xvals_ - 1; ++i)
        if (  POLY_IS_INSIDE(i, j2 - 1, inside_val) ||
            ! POLY_IS_INSIDE(i, j2    , inside_val))
          break;

      if (i > i2) {
        for ( ; i2 < i; ++i2)
          edgeList.addEdge(i2, j2, i2 + 1, j2);

        if (i1 == i2 && j1 == j2)
          break;

        x[num_xy] = xvals_[i2];
        y[num_xy] = yvals_[j2];

        ++num_xy;

        if (num_xy >= new_max_xy) break;

        l_ok = false;
      }
    }

    if (l_ok) {
      /* Walk right to left */

      int i = i2;

      for ( ; i >= 0; --i)
        if (! POLY_IS_INSIDE(i - 1, j2 - 1, inside_val) ||
              POLY_IS_INSIDE(i - 1, j2    , inside_val))
          break;

      if (i < i2) {
        for ( ; i2 > i; --i2)
          edgeList.addEdge(i2, j2, i2 - 1, j2);

        if (i1 == i2 && j1 == j2)
          break;

        x[num_xy] = xvals_[i2];
        y[num_xy] = yvals_[j2];

        ++num_xy;

        if (num_xy >= new_max_xy) break;
      }
    }

    /*------*/

    bool u_ok = (j2 < (int) num_yvals_ - 1 && ! edgeList.hasEdge(i2, j2, i2, j2 + 1));
    bool d_ok = (j2 >                    0 && ! edgeList.hasEdge(i2, j2, i2, j2 - 1));

    if (u_ok) {
      /* Walk bottom to top */

      int j = j2;

      for ( ; j < (int) num_yvals_ - 1; ++j)
        if (! POLY_IS_INSIDE(i2 - 1, j, inside_val) ||
              POLY_IS_INSIDE(i2    , j, inside_val))
          break;

      if (j > j2) {
        for ( ; j2 < j; ++j2)
          edgeList.addEdge(i2, j2, i2, j2 + 1);

        if (i1 == i2 && j1 == j2)
          break;

        x[num_xy] = xvals_[i2];
        y[num_xy] = yvals_[j2];

        ++num_xy;

        if (num_xy >= new_max_xy) break;

        d_ok = false;
      }
    }

    if (d_ok) {
      /* Walk top to bottom */

      int j = j2;

      for ( ; j >= 0; --j)
        if (  POLY_IS_INSIDE(i2 - 1, j - 1, inside_val) ||
            ! POLY_IS_INSIDE(i2    , j - 1, inside_val))
          break;

      if (j < j2) {
        for ( ; j2 > j; --j2)
          edgeList.addEdge(i2, j2, i2, j2 - 1);

        if (i1 == i2 && j1 == j2)
          break;

        x[num_xy] = xvals_[i2];
        y[num_xy] = yvals_[j2];

        ++num_xy;

        if (num_xy >= new_max_xy) break;
      }
    }

    /*------*/

    /* Avoid endless loop */

    if (num_xy == save_num_xy)
      break;
  }

  /*----*/

  /* Ensure new boundary is consistent with the inside flags */

  if (check_consistent) {
    for (j = 0; j < num_yvals_ - 1; ++j) {
      double ym = CMathGen::avg(yvals_[j], yvals_[j + 1]);

      for (i = 0; i < num_xvals_ - 1; ++i) {
        double xm = CMathGen::avg(xvals_[i], xvals_[i + 1]);

        int inside1 = (inside_[i][j] == inside_val);
        int inside2 = CMathGeom2D::PointInsideEvenOdd(xm, ym, &x[0], &y[0], num_xy);

        if (inside1 != inside2)
          return false;
      }
    }
  }

  /*----*/

  x = std::vector<double>(&x[0], &x[num_xy]);
  y = std::vector<double>(&y[0], &y[num_xy]);

  return true;
}

void
CXYValsInside::
print(std::ostream &os) const
{
  uint num_x = getNumXVals();
  uint num_y = getNumYVals();

  for (uint j = 0; j < num_y - 1; ++j) {
    for (uint i = 0; i < num_x - 1; ++i)
      os << inside_[i][j];

    os << std::endl;
  }
}

//---------

bool
CXYValsSelfTest(const char *)
{
  static double x[5] = { -0.1, 24.7, 50.3, 75.2, 100.5 };
  static double y[5] = {  0.1, 45.8, 45.8, 77.3,  99.9 };

  CXYValsInside xyvals(x, y, 5);

  CXYValsInside::InsideArray &inside = xyvals.getInside();

  uint num_x = xyvals.getNumXVals();
  uint num_y = xyvals.getNumYVals();

  CASSERT(num_x == 5, "num_x");
  CASSERT(num_y == 4, "num_y");

  for (uint j = 0; j < num_y - 1; ++j) {
    for (uint i = 0; i < num_x - 1; ++i) {
      inside[i][j] = (i + j) & 1;
    }
  }

  double *xo, *yo;
  uint    num_xyo;

  bool rc = xyvals.getPoly(CXYValsInside::INSIDE1, &xo, &yo, &num_xyo, false);

  CASSERT(rc, "getPoly");

  CASSERT(num_xyo == 20, "num_xyo");

  return true;
}
