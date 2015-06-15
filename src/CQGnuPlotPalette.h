#ifndef CQGnuPlotPalette_H
#define CQGnuPlotPalette_H

#include <CQGnuPlotObject.h>
#include <CGnuPlotPalette.h>
#include <CQGnuPlot.h>

class CQGnuPlotGroup;

class CQGnuPlotPalette : public CQGnuPlotObject, public CGnuPlotPalette {
  Q_OBJECT

  Q_PROPERTY(double gamma      READ gamma      WRITE setGamma     )
  Q_PROPERTY(bool   gray       READ isGray     WRITE setGray      )
  Q_PROPERTY(bool   negative   READ isNegative WRITE setNegative  )
  Q_PROPERTY(int    redModel   READ redModel   WRITE setRedModel  )
  Q_PROPERTY(int    greenModel READ greenModel WRITE setGreenModel)
  Q_PROPERTY(int    blueModel  READ blueModel  WRITE setBlueModel )

  Q_PROPERTY(CQGnuPlotEnum::PaletteColorType colorType  READ colorType  WRITE setColorType )
  Q_PROPERTY(CQGnuPlotEnum::ColorModelType   colorModel READ colorModel WRITE setColorModel)

 public:
  CQGnuPlotPalette(CQGnuPlotGroup *group);
 ~CQGnuPlotPalette();

  CQGnuPlotEnum::PaletteColorType colorType() const;
  void setColorType(CQGnuPlotEnum::PaletteColorType c);

  CQGnuPlotEnum::ColorModelType colorModel() const;
  void setColorModel(CQGnuPlotEnum::ColorModelType c);
};

#endif
