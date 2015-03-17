#include <CQPropertyEditor.h>
#include <CQColorChooser.h>
#include <CQFontChooser.h>
#include <CQPaletteChooser.h>
#include <CQPoint2DEdit.h>
#include <CQLineDash.h>
#include <CQAngleSpinBox.h>
#include <CQUtil.h>
#include <QSpinBox>
#include <cassert>

CQPropertyEditorMgr *
CQPropertyEditorMgr::
instance()
{
  static CQPropertyEditorMgr *instance;

  if (! instance)
    instance = new CQPropertyEditorMgr;

  return instance;
}

CQPropertyEditorMgr::
CQPropertyEditorMgr()
{
  setEditor("QPointF"  , new CQPropertyPointEditor   );
  setEditor("QColor"   , new CQPropertyColorEditor   );
  setEditor("QFont"    , new CQPropertyFontEditor    );
  setEditor("QPalette" , new CQPropertyPaletteEditor );
  setEditor("CLineDash", new CQPropertyLineDashEditor);
  setEditor("CAngle"   , new CQPropertyAngleEditor);
}

void
CQPropertyEditorMgr::
setEditor(const QString &typeName, CQPropertyEditorFactory *editor)
{
  Editors::iterator p = editors_.find(typeName);

  if (p != editors_.end()) {
    std::swap((*p).second, editor);

    delete editor;
  }
  else
    (void) editors_.insert(p, Editors::value_type(typeName, editor));
}

CQPropertyEditorFactory *
CQPropertyEditorMgr::
getEditor(const QString &typeName) const
{
  Editors::const_iterator p = editors_.find(typeName);

  if (p != editors_.end())
    return (*p).second;
  else
    return 0;
}

//------

CQPropertyIntegerEditor::
CQPropertyIntegerEditor(int min, int max, int step) :
 min_(min), max_(max), step_(step)
{
}

QWidget *
CQPropertyIntegerEditor::
createEdit(QWidget *parent)
{
  QSpinBox *spin = new QSpinBox(parent);

  spin->setRange(min_, max_);
  spin->setSingleStep(step_);

  return spin;
}

void
CQPropertyIntegerEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  QSpinBox *spin = qobject_cast<QSpinBox *>(w);
  assert(spin);

  QObject::connect(spin, SIGNAL(valueChanged(int)), obj, method);
}

QVariant
CQPropertyIntegerEditor::
getValue(QWidget *w)
{
  QSpinBox *spin = qobject_cast<QSpinBox *>(w);
  assert(spin);

  return QVariant(spin->value());
}

void
CQPropertyIntegerEditor::
setValue(QWidget *w, const QVariant &var)
{
  QSpinBox *spin = qobject_cast<QSpinBox *>(w);
  assert(spin);

  int i = var.toInt();

  spin->setValue(i);
}

//------

CQPropertyRealEditor::
CQPropertyRealEditor(double min, double max, double step) :
 min_(min), max_(max), step_(step)
{
}

QWidget *
CQPropertyRealEditor::
createEdit(QWidget *parent)
{
  QDoubleSpinBox *spin = new QDoubleSpinBox(parent);

  spin->setRange(min_, max_);
  spin->setSingleStep(step_);
  spin->setDecimals(6);

  return spin;
}

void
CQPropertyRealEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox *>(w);
  assert(spin);

  QObject::connect(spin, SIGNAL(valueChanged(double)), obj, method);
}

QVariant
CQPropertyRealEditor::
getValue(QWidget *w)
{
  QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox *>(w);
  assert(spin);

  return QVariant(spin->value());
}

void
CQPropertyRealEditor::
setValue(QWidget *w, const QVariant &var)
{
  QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox *>(w);
  assert(spin);

  double r = var.toDouble();

  spin->setValue(r);
}

//------

CQPropertyPointEditor::
CQPropertyPointEditor()
{
}

QWidget *
CQPropertyPointEditor::
createEdit(QWidget *parent)
{
  CQPoint2DEdit *edit = new CQPoint2DEdit(parent);

  edit->setSpin(true);

  return edit;
}

void
CQPropertyPointEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQPoint2DEdit *edit = qobject_cast<CQPoint2DEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(valueChanged()), obj, method);
}

QVariant
CQPropertyPointEditor::
getValue(QWidget *w)
{
  CQPoint2DEdit *edit = qobject_cast<CQPoint2DEdit *>(w);
  assert(edit);

  return edit->getQValue();
}

void
CQPropertyPointEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQPoint2DEdit *edit = qobject_cast<CQPoint2DEdit *>(w);
  assert(edit);

  QPointF p = var.toPointF();

  edit->setValue(p);
}

//------

CQPropertyColorEditor::
CQPropertyColorEditor()
{
}

QWidget *
CQPropertyColorEditor::
createEdit(QWidget *parent)
{
  CQColorChooser *chooser = new CQColorChooser(parent);

  chooser->setStyles(CQColorChooser::Text | CQColorChooser::ColorButton);

  return chooser;
}

void
CQPropertyColorEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQColorChooser *chooser = qobject_cast<CQColorChooser *>(w);
  assert(chooser);

  QObject::connect(chooser, SIGNAL(colorChanged(const QColor&)), obj, method);
}

QVariant
CQPropertyColorEditor::
getValue(QWidget *w)
{
  CQColorChooser *chooser = qobject_cast<CQColorChooser *>(w);
  assert(chooser);

  return chooser->colorName();
}

void
CQPropertyColorEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQColorChooser *chooser = qobject_cast<CQColorChooser *>(w);
  assert(chooser);

  QString str = var.toString();

  chooser->setColorName(str);
}

//------

CQPropertyFontEditor::
CQPropertyFontEditor()
{
}

QWidget *
CQPropertyFontEditor::
createEdit(QWidget *parent)
{
  CQFontChooser *chooser = new CQFontChooser(parent);

  return chooser;
}

void
CQPropertyFontEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQFontChooser *chooser = qobject_cast<CQFontChooser *>(w);
  assert(chooser);

  QObject::connect(chooser, SIGNAL(fontChanged(const QString&)), obj, method);
}

QVariant
CQPropertyFontEditor::
getValue(QWidget *w)
{
  CQFontChooser *chooser = qobject_cast<CQFontChooser *>(w);
  assert(chooser);

  return chooser->fontName();
}

void
CQPropertyFontEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQFontChooser *chooser = qobject_cast<CQFontChooser *>(w);
  assert(chooser);

  QString str = var.toString();

  chooser->setFontName(str);
}

//------

CQPropertyPaletteEditor::
CQPropertyPaletteEditor()
{
}

QWidget *
CQPropertyPaletteEditor::
createEdit(QWidget *parent)
{
  CQPaletteChooser *chooser = new CQPaletteChooser(parent);

  return chooser;
}

void
CQPropertyPaletteEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQPaletteChooser *chooser = qobject_cast<CQPaletteChooser *>(w);
  assert(chooser);

  QObject::connect(chooser, SIGNAL(paletteChanged(const QString&)), obj, method);
}

QVariant
CQPropertyPaletteEditor::
getValue(QWidget *w)
{
  CQPaletteChooser *chooser = qobject_cast<CQPaletteChooser *>(w);
  assert(chooser);

  return chooser->palette();
}

void
CQPropertyPaletteEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQPaletteChooser *chooser = qobject_cast<CQPaletteChooser *>(w);
  assert(chooser);

  QString str = var.toString();

  chooser->setPaletteDef(str);
}

//------

CQPropertyLineDashEditor::
CQPropertyLineDashEditor()
{
}

QWidget *
CQPropertyLineDashEditor::
createEdit(QWidget *parent)
{
  CQLineDash *edit = new CQLineDash(parent);

  return edit;
}

void
CQPropertyLineDashEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQLineDash *edit = qobject_cast<CQLineDash *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(valueChanged(const CLineDash &)), obj, method);
}

QVariant
CQPropertyLineDashEditor::
getValue(QWidget *w)
{
  CQLineDash *edit = qobject_cast<CQLineDash *>(w);
  assert(edit);

  return QVariant::fromValue(edit->getLineDash());
}

void
CQPropertyLineDashEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQLineDash *edit = qobject_cast<CQLineDash *>(w);
  assert(edit);

  CLineDash dash = var.value<CLineDash>();

  edit->setLineDash(dash);
}

//------

CQPropertyAngleEditor::
CQPropertyAngleEditor()
{
}

QWidget *
CQPropertyAngleEditor::
createEdit(QWidget *parent)
{
  CQAngleSpinBox *edit = new CQAngleSpinBox(parent);

  return edit;
}

void
CQPropertyAngleEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  CQAngleSpinBox *edit = qobject_cast<CQAngleSpinBox *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(angleChanged(const CAngle &)), obj, method);
}

QVariant
CQPropertyAngleEditor::
getValue(QWidget *w)
{
  CQAngleSpinBox *edit = qobject_cast<CQAngleSpinBox *>(w);
  assert(edit);

  return QVariant::fromValue(edit->getAngle());
}

void
CQPropertyAngleEditor::
setValue(QWidget *w, const QVariant &var)
{
  CQAngleSpinBox *edit = qobject_cast<CQAngleSpinBox *>(w);
  assert(edit);

  CAngle angle = var.value<CAngle>();

  edit->setAngle(angle);
}
