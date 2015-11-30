#ifndef CQGnuPlotCamera_H
#define CQGnuPlotCamera_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotCamera.h>

class CQGnuPlotGroup;

class CQGnuPlotCamera : public CQGnuPlotObject, public CGnuPlotCamera {
  Q_OBJECT

  Q_PROPERTY(bool   enabled  READ isEnabled WRITE setEnabled)
  Q_PROPERTY(double rotateX  READ rotateX   WRITE setRotateX)
  Q_PROPERTY(double rotateY  READ rotateY   WRITE setRotateY)
  Q_PROPERTY(double rotateZ  READ rotateZ   WRITE setRotateZ)

  Q_PROPERTY(double xmin READ xmin WRITE setXMin)
  Q_PROPERTY(double xmax READ xmax WRITE setXMax)
  Q_PROPERTY(double ymin READ ymin WRITE setYMin)
  Q_PROPERTY(double ymax READ ymax WRITE setYMax)
  Q_PROPERTY(double near READ near WRITE setNear)
  Q_PROPERTY(double far  READ far  WRITE setFar )

  Q_PROPERTY(double planeZ        READ planeZ          WRITE setPlaneZ       )
  Q_PROPERTY(bool   planeRelative READ isPlaneRelative WRITE setPlaneRelative)

 public:
  CQGnuPlotCamera(CQGnuPlotGroup *group);
};

#endif
