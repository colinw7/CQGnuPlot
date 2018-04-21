#include <CQGnuPlotPaletteDialog.h>
#include <CQGnuPlotWindow.h>
#include <CQGradientPalettePlot.h>
#include <CQGradientPaletteIFace.h>
#include <CQGnuPlotGroup.h>
#include <QHBoxLayout>

CQGnuPlotPaletteDialog::
CQGnuPlotPaletteDialog(CQGnuPlotMainWindow *window) :
 CQDialog(window), window_(window)
{
  setWindowTitle("Edit Palette");

  pal_ = new CGradientPalette(window->app()->expr());
}

CQGnuPlotPaletteDialog::
~CQGnuPlotPaletteDialog()
{
  delete pal_;
}

void
CQGnuPlotPaletteDialog::
createWidgets(QWidget *)
{
  CExpr *expr = window_->app()->expr();

  QFrame *frame = new QFrame;

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->setMargin(2); layout->setSpacing(2);

  palette_ = new CQGradientPalettePlot(expr);
  control_ = new CQGradientPaletteIFace(palette_);

  layout->addWidget(palette_);
  layout->addWidget(control_);

  connect(control_, SIGNAL(stateChanged()), this, SIGNAL(paletteChanged()));

  //---

  CQGnuPlotGroup *group = window_->currentGroup();

  if (group)
    *pal_ = *group->palette()->gradientPalette();
  else
    *pal_ = *window_->app()->palette().gradientPalette();

  palette_->setGradientPalette(pal_);

  //---

  addField("", frame, /*stretch*/false);
}
