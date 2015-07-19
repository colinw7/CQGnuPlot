#ifndef CQGnuPlotTimeStamp_H
#define CQGnuPlotTimeStamp_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotTimeStamp.h>

class CQGnuPlotGroup;

class CQGnuPlotTimeStamp : public CQGnuPlotObject, public CGnuPlotTimeStamp {
  Q_OBJECT

  Q_PROPERTY(bool top READ isTop WRITE setTop)

 public:
  CQGnuPlotTimeStamp(CQGnuPlotGroup *group);
 ~CQGnuPlotTimeStamp();
};

#endif
