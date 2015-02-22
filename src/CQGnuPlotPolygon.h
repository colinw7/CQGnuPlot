#ifndef CQGnuPlotPolygon_H
#define CQGnuPlotPolygon_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotPolygon.h>

class CQGnuPlotPolygon : public CQGnuPlotAnnotation, public CGnuPlotPolygon {
  Q_OBJECT

  Q_PROPERTY(double lineWidth READ getLineWidth WRITE setLineWidth)

 public:
  CQGnuPlotPolygon(CQGnuPlotGroup *group);

  double getLineWidth() const;

  void draw() const override;
};

#endif
