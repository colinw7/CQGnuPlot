#ifndef CQGnuPlotPolygon_H
#define CQGnuPlotPolygon_H

#include <CQGnuPlotAnnotation.h>
#include <CGnuPlotPolygon.h>

class CQGnuPlotPolygon : public CQGnuPlotAnnotation, public CGnuPlotPolygon {
  Q_OBJECT

  Q_PROPERTY(bool selected    READ isSelected    WRITE setSelected   )
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)

  Q_PROPERTY(double lineWidth READ getLineWidth WRITE setLineWidth)

 public:
  CQGnuPlotPolygon(CQGnuPlotGroup *group);

  double getLineWidth() const;

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
