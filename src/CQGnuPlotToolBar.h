#ifndef CQGnuPlotToolBar_H
#define CQGnuPlotToolBar_H

#include <QToolBar>

class CQGnuPlotToolBar : public QToolBar {
   Q_OBJECT

 public:
  CQGnuPlotToolBar(const QString &text=QString());

  void contextMenuEvent(QContextMenuEvent *e);

 private slots:
  void sizeSlot(int);
};

#endif
