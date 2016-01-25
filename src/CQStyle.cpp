#include <CQStyle.h>

#include <QGroupBox>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <qdrawutil.h>

CQStyle::
CQStyle() :
 QProxyStyle()
{
}

void
CQStyle::
drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                   QPainter *painter, const QWidget *widget) const
{
  switch (control) {
    case CC_GroupBox: {
      if (const QStyleOptionGroupBox *groupBox =
            qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
        // Draw frame
        QRect textRect = proxy()->subControlRect(CC_GroupBox, option, SC_GroupBoxLabel, widget);
        QRect checkBoxRect =
          proxy()->subControlRect(CC_GroupBox, option, SC_GroupBoxCheckBox, widget);
        if (groupBox->subControls & QStyle::SC_GroupBoxFrame) {
          QStyleOptionFrame frame;
          frame.QStyleOption::operator=(*groupBox);
          frame.features = groupBox->features;
          frame.lineWidth = groupBox->lineWidth;
          frame.midLineWidth = groupBox->midLineWidth;
          frame.rect = proxy()->subControlRect(CC_GroupBox, option, SC_GroupBoxFrame, widget);
          painter->save();
          QRegion region(groupBox->rect);
          if (!groupBox->text.isEmpty()) {
            bool ltr = groupBox->direction == Qt::LeftToRight;
            QRect finalRect;
            if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
              finalRect = checkBoxRect.united(textRect);
              finalRect.adjust(ltr ? -4 : 0, 0, ltr ? 0 : 4, 0);
            }
            else {
              finalRect = textRect;
            }
            region -= finalRect;
          }
          painter->setClipRegion(region);
          proxy()->drawPrimitive(PE_FrameGroupBox, &frame, painter, widget);
          painter->restore();
        }

        // Draw title
        if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
          QColor textColor = groupBox->textColor;
          if (textColor.isValid())
            painter->setPen(textColor);
          int alignment = int(groupBox->textAlignment);
          if (!proxy()->styleHint(QStyle::SH_UnderlineShortcut, option, widget))
            alignment |= Qt::TextHideMnemonic;

          proxy()->drawItemText(painter, textRect,
                                Qt::TextShowMnemonic | Qt::AlignHCenter | alignment,
                                groupBox->palette, groupBox->state & State_Enabled, groupBox->text,
                                textColor.isValid() ? QPalette::NoRole : QPalette::WindowText);

          if (groupBox->state & State_HasFocus) {
            QStyleOptionFocusRect fropt;
            fropt.QStyleOption::operator=(*groupBox);
            fropt.rect = textRect;
            proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
          }
        }

        // Draw checkbox
        if (groupBox->subControls & SC_GroupBoxCheckBox) {
          QStyleOptionButton box;
          box.QStyleOption::operator=(*groupBox);
          box.rect = checkBoxRect;
          proxy()->drawPrimitive(PE_IndicatorCheckBox, &box, painter, widget);
        }
      }
      break;
    }
    default: {
      return QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
  }
}

#if 0
void
CQStyle::
drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
            const QWidget *widget) const
{
  return QProxyStyle::drawControl(element, option, painter, widget);
}

void
CQStyle::
drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const
{
  return QProxyStyle::drawItemPixmap(painter, rect, alignment, pixmap);
}

void
CQStyle::
drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled,
             const QString &text, QPalette::ColorRole textRole) const
{
  return QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}
#endif

void
CQStyle::
drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
              const QWidget *widget) const
{
  QRectF rect  = option->rect;
  int    state = option->state;

  QColor base          = option->palette.base().color();
  QColor button        = option->palette.button().color();
  QColor buttonShadow  = button.darker(120);
  QColor highlight     = option->palette.highlight().color();
  QColor darkHighlight = highlight.darker(110);

  switch (element) {
    case PE_FrameGroupBox: {
      if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
        if (frame->features & QStyleOptionFrame::Flat) {
          QRect fr = frame->rect;
          QPoint p1(fr.x(), fr.y() + 1);
          QPoint p2(fr.x() + fr.width(), p1.y());
          qDrawShadeLine(painter, p1, p2, frame->palette, true,
                         frame->lineWidth, frame->midLineWidth);
        }
        else {
          qDrawShadeRect(painter, frame->rect.x(), frame->rect.y(), frame->rect.width(),
                         frame->rect.height(), frame->palette, true,
                         frame->lineWidth, frame->midLineWidth);
        }
      }
      break;
    }
    case PE_IndicatorRadioButton: {
      double b = rect.width()*0.1;
      double w = rect.width()/3.0;
      //---
      QRectF oRect = rect.adjusted(b, b, -b, -b);
      QPainterPath opath;
      opath.addEllipse(oRect);
      //---
      QPointF c = rect.center();
      QPointF d = QPointF(w/2, w/2);
      QRectF iRect = QRectF(c - d, c + d);
      QPainterPath ipath;
      ipath.addEllipse(iRect);
      //---
      painter->save();
      painter->setRenderHint(QPainter::Antialiasing);
      if      (state & State_On) {
        painter->fillPath  (opath, QBrush(highlight    ));
        painter->strokePath(opath, QPen  (buttonShadow ));
        painter->fillPath  (ipath, QBrush(base         ));
        painter->strokePath(ipath, QPen  (darkHighlight));
      }
      else if (state & State_Sunken) {
        painter->fillPath  (opath, QBrush(button       ));
        painter->strokePath(opath, QPen  (buttonShadow ));
        painter->fillPath  (ipath, QBrush(base         ));
        painter->strokePath(ipath, QPen  (buttonShadow ));
      }
      else {
        painter->fillPath  (opath, QBrush(base        ));
        painter->strokePath(opath, QPen  (buttonShadow));
      }
      painter->restore();
      break;
    }
    case PE_IndicatorCheckBox: {
      double ob = rect.width()*0.1;
      double ib = rect.width()*0.25;
      //---
      QRectF oRect = rect.adjusted(ob, ob, -ob, -ob);
      QPainterPath opath;
      opath.addRect(oRect);
      //---
      QRectF iRect = rect.adjusted(ib, ib, -ib, -ib);
      //---
      painter->save();
      painter->setRenderHint(QPainter::Antialiasing);
      if      (state & State_On) {
        painter->fillPath  (opath, QBrush(highlight    ));
        painter->strokePath(opath, QPen  (buttonShadow ));
        drawCheck(painter, iRect, QBrush(base));
      }
      else if (state & State_Sunken) {
        painter->fillPath  (opath, QBrush(button       ));
        painter->strokePath(opath, QPen  (buttonShadow ));
        drawCheck(painter, iRect, QBrush(base));
      }
      else {
        painter->fillPath  (opath, QBrush(base        ));
        painter->strokePath(opath, QPen  (buttonShadow));
      }
      painter->restore();
      break;
    }
    default: {
      return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
  }
}

#if 0
QPixmap
CQStyle::
generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
  return QProxyStyle::generatedIconPixmap(iconMode, pixmap, opt);
}
#endif

QStyle::SubControl
CQStyle::
hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                      const QPoint &pos, const QWidget *widget) const
{
  SubControl sc = SC_None;

  switch (control) {
    case CC_GroupBox: {
      if (const QStyleOptionGroupBox *groupBox =
            qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
        QRect r;
        uint ctrl = SC_GroupBoxCheckBox;
        while (ctrl <= SC_GroupBoxFrame) {
          r = proxy()->subControlRect(control, groupBox, QStyle::SubControl(ctrl), widget);
          if (r.isValid() && r.contains(pos)) {
            sc = QStyle::SubControl(ctrl);
            break;
          }
          ctrl <<= 1;
        }
      }
      break;
    }
    default: {
      sc = QProxyStyle::hitTestComplexControl(control, option, pos, widget);
      break;
    }
  }

  return sc;
}

#if 0
QRect
CQStyle::
itemPixmapRect(const QRect &r, int flags, const QPixmap &pixmap) const
{
  return QProxyStyle::itemPixmapRect(r, flags, pixmap);
}

QRect
CQStyle::
itemTextRect(const QFontMetrics &fm, const QRect &r, int flags, bool enabled,
             const QString &text) const
{
  return QProxyStyle::itemTextRect(fm, r, flags, enabled, text);
}
#endif

int
CQStyle::
pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
  switch (metric) {
    case PM_SmallIconSize:
    case PM_ListViewIconSize:
    case PM_IconViewIconSize:
    case PM_TabBarIconSize:
    case PM_ButtonIconSize:
      return 24;
    case PM_LargeIconSize:
    case PM_ToolBarIconSize:
    case PM_MessageBoxIconSize:
      return 32;
    default:
      return QProxyStyle::pixelMetric(metric, option, widget);
  }
}

#if 0
void
CQStyle::
polish(QWidget *widget)
{
  return QProxyStyle::polish(widget);
}

void
CQStyle::
polish(QPalette &pal)
{
  return QProxyStyle::polish(pal);
}

void
CQStyle::
polish(QApplication *app)
{
  return QProxyStyle::polish(app);
}
#endif

QSize
CQStyle::
sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size,
                 const QWidget *widget) const
{
  QSize sz;

  switch (type) {
    case CT_GroupBox: {
      if (const QGroupBox *grb = static_cast<const QGroupBox *>(widget))
        sz += QSize(!grb->isFlat() ? 16 : 0, 0);
      break;
    }
    default: {
      sz = QProxyStyle::sizeFromContents(type, option, size, widget);
      break;
    }
  }

  return sz;
}

#if 0
QPalette
CQStyle::
standardPalette() const
{
  return QProxyStyle::standardPalette();
}

QPixmap
CQStyle::
standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget) const
{
  return QProxyStyle::standardPixmap(standardPixmap, opt, widget);
}
#endif

int
CQStyle::
styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
          QStyleHintReturn *returnData) const
{
  int ret = 0;

  switch (hint) {
    case SH_GroupBox_TextLabelVerticalAlignment:
      ret = Qt::AlignVCenter;
      break;
    case SH_GroupBox_TextLabelColor:
      ret = option ? int(option->palette.color(QPalette::Text).rgba()) : 0;
      break;
    default:
      ret = QProxyStyle::styleHint(hint, option, widget, returnData);
      break;
  }

  return ret;
}

QRect
CQStyle::
subControlRect(ComplexControl cc, const QStyleOptionComplex *option, SubControl sc,
               const QWidget *widget) const
{
  QRect ret;

  switch (cc) {
    case CC_GroupBox: {
      if (const QStyleOptionGroupBox *groupBox =
            qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
        switch (sc) {
          case SC_GroupBoxFrame:
            // FALL THROUGH
          case SC_GroupBoxContents: {
            int topMargin = 0;
            int topHeight = 0;
            int verticalAlignment =
              proxy()->styleHint(SH_GroupBox_TextLabelVerticalAlignment, groupBox, widget);
            if (groupBox->text.size() || (groupBox->subControls & QStyle::SC_GroupBoxCheckBox)) {
              topHeight = groupBox->fontMetrics.height();
              if (verticalAlignment & Qt::AlignVCenter)
                topMargin = topHeight / 2;
              else if (verticalAlignment & Qt::AlignTop)
                topMargin = topHeight;
            }

            QRect frameRect = groupBox->rect;
            frameRect.setTop(topMargin);

            if (sc == SC_GroupBoxFrame) {
              ret = frameRect;
              break;
            }

            int frameWidth = 0;
            if ((groupBox->features & QStyleOptionFrame::Flat) == 0)
              frameWidth = proxy()->pixelMetric(PM_DefaultFrameWidth, groupBox, widget);
            ret = frameRect.adjusted(frameWidth, frameWidth + topHeight - topMargin,
                                     -frameWidth, -frameWidth);
            break;
          }
          case SC_GroupBoxCheckBox:
            // FALL THROUGH
          case SC_GroupBoxLabel: {
            QFontMetrics fontMetrics = groupBox->fontMetrics;
            int h = fontMetrics.height();
            int tw = fontMetrics.size(Qt::TextShowMnemonic,
                       groupBox->text + QLatin1Char(' ')).width();
            int marg = (groupBox->features & QStyleOptionFrame::Flat) ? 0 : 8;
            ret = groupBox->rect.adjusted(marg, 0, -marg, 0);
            ret.setHeight(h);

            int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, option, widget);
            int indicatorSpace = proxy()->pixelMetric(PM_CheckBoxLabelSpacing, option, widget) - 1;
            bool hasCheckBox = groupBox->subControls & QStyle::SC_GroupBoxCheckBox;
            int checkBoxSize = hasCheckBox ? (indicatorWidth + indicatorSpace) : 0;

            // Adjusted rect for label + indicatorWidth + indicatorSpace
            QRect totalRect = alignedRect(groupBox->direction, groupBox->textAlignment,
                                          QSize(tw + checkBoxSize, h), ret);

            // Adjust totalRect if checkbox is set
            if (hasCheckBox) {
              bool ltr = groupBox->direction == Qt::LeftToRight;
              int left = 0;
              // Adjust for check box
              if (sc == SC_GroupBoxCheckBox) {
                int indicatorHeight = proxy()->pixelMetric(PM_IndicatorHeight, option, widget);
                left = ltr ? totalRect.left() : (totalRect.right() - indicatorWidth);
                int top = totalRect.top() + qMax(0, fontMetrics.height() - indicatorHeight) / 2;
                totalRect.setRect(left, top, indicatorWidth, indicatorHeight);
                // Adjust for label
              }
              else {
                left = ltr ? (totalRect.left() + checkBoxSize - 2) : totalRect.left();
                totalRect.setRect(left, totalRect.top(),
                                  totalRect.width() - checkBoxSize, totalRect.height());
              }
            }
            ret = totalRect;
            break;
          }
          default:
            break;
        }
      }
      break;
    }
    default: {
      ret = QProxyStyle::subControlRect(cc, option, sc, widget);
      break;
    }
  }

  return ret;
}

#if o
QRect
CQStyle::
subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
  return QProxyStyle::subElementRect(element, option, widget);
}

void
CQStyle::
unpolish(QWidget *widget)
{
  return QProxyStyle::unpolish(widget);
}

void
CQStyle::
unpolish(QApplication *app)
{
  return QProxyStyle::unpolish(app);
}
#endif

void
CQStyle::
drawCheck(QPainter *p, const QRectF &r, const QBrush &b) const
{
  double xo = r.left();
  double yo = r.top();
  double xs = r.width ()/100.0;
  double ys = r.height()/100.0;

  auto mapf = [&](double x, double y) -> QPointF {
    return QPointF(xo + x*xs, yo + (100 - y)*ys);
  };

  QPainterPath path;

  path.moveTo(mapf(  0,  32));
  path.lineTo(mapf( 36,   0));
  path.lineTo(mapf( 49,   0));
  path.lineTo(mapf(100,  92));
  path.lineTo(mapf( 92, 100));
  path.lineTo(mapf( 42,  37));
  path.lineTo(mapf( 26,  61));

  path.closeSubpath();

  p->fillPath(path, b);
}
