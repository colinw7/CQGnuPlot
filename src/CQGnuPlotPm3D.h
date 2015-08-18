#ifndef CQGnuPlotPm3D_H
#define CQGnuPlotPm3D_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPm3D.h>

class CQGnuPlotGroup;

class CQGnuPlotPm3D : public CQGnuPlotObject, public CGnuPlotPm3D {
  Q_OBJECT

  Q_PROPERTY(bool enabled    READ isEnabled  WRITE setEnabled   )
  Q_PROPERTY(bool ftriangles READ ftriangles WRITE setFTriangles)
  Q_PROPERTY(bool clipin     READ isClipIn   WRITE setClipIn    )

 public:
  CQGnuPlotPm3D(CQGnuPlotGroup *group);
};

#endif
