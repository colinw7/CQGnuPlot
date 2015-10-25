#ifndef CQGnuPlotPm3D_H
#define CQGnuPlotPm3D_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPm3D.h>

class CQGnuPlotGroup;

class CQGnuPlotPm3D : public CQGnuPlotObject, public CGnuPlotPm3D {
  Q_OBJECT

  Q_PROPERTY(bool enabled    READ isEnabled  WRITE setEnabled   )
  Q_PROPERTY(bool ftriangles READ ftriangles WRITE setFTriangles)
  Q_PROPERTY(bool clipIn     READ isClipIn   WRITE setClipIn    )
  Q_PROPERTY(int  lineType   READ lineType   WRITE setLineType  )
  Q_PROPERTY(int  implicit   READ isImplicit WRITE setImplicit  )

 public:
  CQGnuPlotPm3D(CQGnuPlotGroup *group);
};

#endif
