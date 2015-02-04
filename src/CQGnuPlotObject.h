#ifndef CQGnuPlotObject_H
#define CQGnuPlotObject_H

#include <QObject>
#include <QColor>
#include <QPointF>

#include <CGnuPlotObject.h>

class CQGnuPlot;

class CQGnuPlotAnnotation : public QObject {
  Q_OBJECT

  Q_PROPERTY(QColor strokeColor READ getStrokeColor WRITE setStrokeColor)
  Q_PROPERTY(QColor fillColor   READ getFillColor   WRITE setFillColor  )
  Q_PROPERTY(bool   selected    READ isSelected     WRITE setSelected   )

 public:
  CQGnuPlotAnnotation(CGnuPlotAnnotation *obj);

  QColor getStrokeColor() const;
  void setStrokeColor(const QColor &c);

  QColor getFillColor() const;
  void setFillColor(const QColor &c);

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

 private:
  CGnuPlotAnnotation *obj_;
  bool                selected_;
};

//---

class CQGnuPlotArrow : public CQGnuPlotAnnotation, public CGnuPlotArrow {
  Q_OBJECT

  Q_PROPERTY(QPointF from      READ getFrom        WRITE setFrom       )
  Q_PROPERTY(QPointF to        READ getTo          WRITE setTo         )
  Q_PROPERTY(bool    relative  READ getRelative    WRITE setRelative   )
  Q_PROPERTY(double  length    READ getLengthValue WRITE setLengthValue)
  Q_PROPERTY(double  angle     READ getAngle       WRITE setAngle      )
  Q_PROPERTY(double  backAngle READ getBackAngle   WRITE setBackAngle  )
  Q_PROPERTY(bool    fhead     READ getFHead       WRITE setFHead      )
  Q_PROPERTY(bool    thead     READ getTHead       WRITE setTHead      )
  Q_PROPERTY(bool    filled    READ getFilled      WRITE setFilled     )
  Q_PROPERTY(bool    empty     READ getEmpty       WRITE setEmpty      )
  Q_PROPERTY(bool    front     READ getFront       WRITE setFront      )
  Q_PROPERTY(int     lineType  READ getLineStyle   WRITE setLineStyle  )
  Q_PROPERTY(double  lineWidth READ getLineWidth   WRITE setLineWidth  )

 public:
  CQGnuPlotArrow(CQGnuPlot *plot);

  QPointF getFrom() const;
  void setFrom(const QPointF &p);

  QPointF getTo() const;
  void setTo(const QPointF &p);
};

//---

class CQGnuPlotLabel : public CQGnuPlotAnnotation, public CGnuPlotLabel {
  Q_OBJECT

  Q_PROPERTY(QString text   READ getText   WRITE setText  )
  Q_PROPERTY(QPointF pos    READ getPos    WRITE setPos   )
  Q_PROPERTY(QString font   READ getFont   WRITE setFont  )
  Q_PROPERTY(double  angle  READ getAngle  WRITE setAngle )
  Q_PROPERTY(bool    front  READ getFront  WRITE setFront )
  Q_PROPERTY(double  offset READ getOffset WRITE setOffset)

 public:
  CQGnuPlotLabel(CQGnuPlot *plot);

  QString getText() const;
  void setText(const QString &s);

  QPointF getPos() const;
  void setPos(const QPointF &p);

  QString getFont() const;
  void setFont(const QString &s);
};

//---

class CQGnuPlotEllipse : public CQGnuPlotAnnotation, public CGnuPlotEllipse {
  Q_OBJECT

  Q_PROPERTY(QPointF center READ getCenter WRITE setCenter)
  Q_PROPERTY(double  rx     READ getRX     WRITE setRX    )
  Q_PROPERTY(double  ry     READ getRY     WRITE setRY    )

 public:
  CQGnuPlotEllipse(CQGnuPlot *plot);

  QPointF getCenter() const;
  void setCenter(const QPointF &p);
};

//---

class CQGnuPlotPolygon : public CQGnuPlotAnnotation, public CGnuPlotPolygon {
  Q_OBJECT

 public:
  CQGnuPlotPolygon(CQGnuPlot *plot);
};

//---

class CQGnuPlotRectangle : public CQGnuPlotAnnotation, public CGnuPlotRectangle {
  Q_OBJECT

  Q_PROPERTY(QPointF from      READ getFrom      WRITE setFrom     )
  Q_PROPERTY(QPointF to        READ getTo        WRITE setTo       )
  Q_PROPERTY(int     fillStyle READ getFillStyle WRITE setFillStyle)
  Q_PROPERTY(double  lineWidth READ getLineWidth WRITE setLineWidth)

 public:
  CQGnuPlotRectangle(CQGnuPlot *plot);

  QPointF getFrom() const;
  void setFrom(const QPointF &p);

  QPointF getTo() const;
  void setTo(const QPointF &p);

  void draw(CGnuPlotRenderer *renderer) const override;
};

#endif
