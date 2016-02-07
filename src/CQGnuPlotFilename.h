#ifndef CQGnuPlotFilename_H
#define CQGnuPlotFilename_H

#include <QFrame>

class QLineEdit;
class QToolButton;

class CQGnuPlotFilename : public QFrame {
  Q_OBJECT

  Q_PROPERTY(bool    save    READ isSave  WRITE setSave   )
  Q_PROPERTY(QString pattern READ pattern WRITE setPattern)

 public:
  CQGnuPlotFilename(QWidget *parent=0);

  bool isSave() const { return save_; }
  void setSave(bool b) { save_ = b; }

  const QString &pattern() const { return pattern_; }
  void setPattern(const QString &v) { pattern_ = v; }

  QString name() const;
  void setName(const QString &name);

 private slots:
  void fileSlot();

 private:
  bool         save_;
  QString      pattern_;
  QLineEdit   *edit_;
  QToolButton *button_;
};

#endif
