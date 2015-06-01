#ifndef CQGnuPlotPalette_H
#define CQGnuPlotPalette_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPalette.h>
#include <CQGnuPlot.h>

class CQGnuPlotGroup;

class CQGnuPlotPalette : public CQGnuPlotObject, public CGnuPlotPalette {
  Q_OBJECT

  Q_PROPERTY(CQGnuPlot::PaletteColorType colorType  READ colorType  WRITE setColorType )
  Q_PROPERTY(CQGnuPlot::ColorModelType   colorModel READ colorModel WRITE setColorModel)
  Q_PROPERTY(double                      gamma      READ gamma      WRITE setGamma     )
  Q_PROPERTY(bool                        gray       READ isGray     WRITE setGray      )
  Q_PROPERTY(bool                        negative   READ isNegative WRITE setNegative  )
  Q_PROPERTY(int                         redModel   READ redModel   WRITE setRedModel  )
  Q_PROPERTY(int                         greenModel READ greenModel WRITE setGreenModel)
  Q_PROPERTY(int                         blueModel  READ blueModel  WRITE setBlueModel )

 public:
  CQGnuPlotPalette(CQGnuPlotGroup *group);
 ~CQGnuPlotPalette();

  CQGnuPlot::PaletteColorType colorType() const;
  void setColorType(CQGnuPlot::PaletteColorType c);

  CQGnuPlot::ColorModelType colorModel() const;
  void setColorModel(CQGnuPlot::ColorModelType c);
};

#endif
