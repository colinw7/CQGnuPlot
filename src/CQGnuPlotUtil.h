#ifndef CQGnuPlotUtil_H
#define CQGnuPlotUtil_H

#include <CQGnuPlot.h>
#include <CQGnuPlotPlot.h>
#include <CAlignType.h>
#include <QIcon>
#include <QPixmap>

namespace CQGnuPlotUtil {
  inline QIcon svgIcon(const uchar *data, int len) {
    QPixmap pixmap;

    pixmap.loadFromData(data, len);

    return QIcon(pixmap);
  }

  //---

  Qt::BrushStyle fillPatternQtConv(const CGnuPlotTypes::FillPattern &type);
  CGnuPlotTypes::FillPattern fillPatternQtConv(const Qt::BrushStyle &type);

  //---

  inline QColor toQColor(const CRGBA &c) {
    return QColor(c.getRedI(),c.getGreenI(),c.getBlueI(),c.getAlphaI());
  }

  inline CRGBA fromQColor(const QColor &c) {
    return CRGBA(c.redF(),c.greenF(),c.blueF(),c.alphaF());
  }
}

#endif
