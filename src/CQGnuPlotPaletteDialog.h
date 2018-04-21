#ifndef CQGnuPlotPaletteDialog_H
#define CQGnuPlotPaletteDialog_H

#include <CQDialog.h>

class CQGnuPlotMainWindow;
class CQGradientPalettePlot;
class CQGradientPaletteIFace;
class CGradientPalette;

class CQGnuPlotPaletteDialog : public CQDialog {
  Q_OBJECT

 public:
  CQGnuPlotPaletteDialog(CQGnuPlotMainWindow *window);
 ~CQGnuPlotPaletteDialog();

  CQGnuPlotMainWindow *window() const { return window_; }

  void createWidgets(QWidget *frame) override;

  CGradientPalette *palette() { return pal_; }

 signals:
  void paletteChanged();

 private:
  CQGnuPlotMainWindow*    window_;
  CGradientPalette*       pal_;
  CQGradientPalettePlot*  palette_;
  CQGradientPaletteIFace* control_;
};

#endif
