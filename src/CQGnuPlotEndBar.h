#ifndef CQGnuPlotEndBar_H
#define CQGnuPlotEndBar_H

#include <CGnuPlotBoxBarObject.h>
#include <CQGnuPlot.h>

class CQGnuPlotPlot;
class CQGnuPlotStroke;

class CQGnuPlotEndBar : public QObject, public CGnuPlotEndBar {
  Q_OBJECT

  Q_PROPERTY(bool   startLine READ isStartLine WRITE setStartLine)
  Q_PROPERTY(bool   endLine   READ isEndLine   WRITE setEndLine  )
  Q_PROPERTY(double endWidth  READ endWidth    WRITE setEndWidth )

 public:
  CQGnuPlotEndBar(CQGnuPlotPlot *plot);
 ~CQGnuPlotEndBar();

  CQGnuPlotStroke *stroke() const;
};

#endif
