#include <CQPropertyEditor.h>
#include <CQColorChooser.h>
#include <CQFontChooser.h>
#include <CQPaletteChooser.h>
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
  setEditor("QColor"  , new CQPropertyColorEditor  );
  setEditor("QFont"   , new CQPropertyFontEditor   );
  setEditor("QPalette", new CQPropertyPaletteEditor);
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
CQPropertyIntegerEditor()
{
}

QWidget *
CQPropertyIntegerEditor::
createEdit(QWidget *parent)
{
  QSpinBox *spin = new QSpinBox(parent);
  assert(spin);

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
setValue(QWidget *w, const QString &str)
{
  QSpinBox *spin = qobject_cast<QSpinBox *>(w);
  assert(spin);

  int i = str.toInt();

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
  assert(spin);

  spin->setRange(min_, max_);
  spin->setSingleStep(step_);

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
setValue(QWidget *w, const QString &str)
{
  QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox *>(w);
  assert(spin);

  double r = str.toDouble();

  spin->setValue(r);
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
  assert(chooser);

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
setValue(QWidget *w, const QString &str)
{
  CQColorChooser *chooser = qobject_cast<CQColorChooser *>(w);
  assert(chooser);

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
  assert(chooser);

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
setValue(QWidget *w, const QString &str)
{
  CQFontChooser *chooser = qobject_cast<CQFontChooser *>(w);
  assert(chooser);

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
  assert(chooser);

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
setValue(QWidget *w, const QString &str)
{
  CQPaletteChooser *chooser = qobject_cast<CQPaletteChooser *>(w);
  assert(chooser);

  chooser->setPaletteDef(str);
}
