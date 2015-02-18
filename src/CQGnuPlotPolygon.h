#ifndef CQGnuPlotPolygon_H
#define CQGnuPlotPolygon_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotPolygon.h>

class CQGnuPlotPolygon : public CQGnuPlotAnnotation, public CGnuPlotPolygon {
  Q_OBJECT

 public:
  CQGnuPlotPolygon(CQGnuPlotGroup *group);

  void draw() const override;
};

#endif
