#ifndef CQPropertyEditor_H
#define CQPropertyEditor_H

#include <QString>
#include <QVariant>
#include <map>

class QWidget;
class QObject;

//------

class CQPropertyEditorFactory {
 public:
  CQPropertyEditorFactory() { }

  virtual ~CQPropertyEditorFactory() { }

  virtual QWidget *createEdit(QWidget *parent=0) = 0;

  virtual void connect(QWidget *w, QObject *obj, const char *method) = 0;

  virtual QVariant getValue(QWidget *w) = 0;

  virtual void setValue(QWidget *w, const QString &str) = 0;
};

//------

class CQPropertyIntegerEditor : public CQPropertyEditorFactory {
 public:
  CQPropertyIntegerEditor(int min=0, int max=INT_MAX, int step=1);

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QString &str);

 private:
  int min_, max_;
  int step_;
};

//------

class CQPropertyRealEditor : public CQPropertyEditorFactory {
 public:
  CQPropertyRealEditor(double min=0.0, double max=99.9, double step=1.0);

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QString &str);

 private:
  double min_, max_;
  double step_;
};

//------

class CQPropertyColorEditor : public CQPropertyEditorFactory {
 public:
  CQPropertyColorEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QString &str);
};

//------

class CQPropertyFontEditor : public CQPropertyEditorFactory {
 public:
  CQPropertyFontEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QString &str);
};

//------

class CQPropertyPaletteEditor : public CQPropertyEditorFactory {
 public:
  CQPropertyPaletteEditor();

  QWidget *createEdit(QWidget *parent);

  void connect(QWidget *w, QObject *obj, const char *method);

  QVariant getValue(QWidget *w);

  void setValue(QWidget *w, const QString &str);
};

//------

class CQPropertyEditorMgr {
 public:
  static CQPropertyEditorMgr *instance();

  CQPropertyEditorMgr();

  void setEditor(const QString &typeName, CQPropertyEditorFactory *editor);

  CQPropertyEditorFactory *getEditor(const QString &typeName) const;

 private:
  typedef std::map<QString, CQPropertyEditorFactory *> Editors;

  Editors editors_;
};

#endif
