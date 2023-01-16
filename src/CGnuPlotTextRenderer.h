#ifndef CGnuPlotTextRenderer_H
#define CGnuPlotTextRenderer_H

#include <CFont.h>
#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CRGBA.h>
#include <CAlignType.h>

class CGnuPlotTextRenderer {
 public:
  typedef std::pair<CHAlignType,double> HAlignPos;
  typedef std::pair<CVAlignType,double> VAlignPos;

 public:
  CGnuPlotTextRenderer();

  virtual ~CGnuPlotTextRenderer() { }

  virtual CFontPtr getFont() const { return font_; }
  virtual void setFont(const CFontPtr &font) { font_ = font; }

  virtual bool isPseudo() const { return false; }

  double pixelWidthToWindowWidth  (double pw);
  double pixelHeightToWindowHeight(double ph);

  double windowWidthToPixelWidth  (double ww);
  double windowHeightToPixelHeight(double wh);

  void pixelToWindow(const CPoint2D &p, CPoint2D &w);
  void windowToPixel(const CPoint2D &w, CPoint2D &p);

  virtual void pixelToWindow(double px, double py, double *wx, double *wy) = 0;
  virtual void windowToPixel(double wx, double wy, double *px, double *py) = 0;

  virtual void drawText(const CPoint2D &p, const std::string &str, const CRGBA &c) = 0;

  virtual void drawRotatedText(const CPoint2D &p, const std::string &str, double a,
                               const HAlignPos &halignPos, const VAlignPos &valignPos,
                               const CRGBA &c) = 0;

  CPoint2D rotatePoint(const CPoint2D &p, double a, const CPoint2D &o);

 protected:
  CFontPtr font_;
};

class CGnuPlotBBoxTextRenderer : public CGnuPlotTextRenderer {
 public:
  CGnuPlotBBoxTextRenderer(CGnuPlotTextRenderer *renderer);

  bool isPseudo() const override { return true; }

  CGnuPlotTextRenderer *renderer() const { return renderer_; }

  const CBBox2D &bbox() const { return bbox_; }

  void setFont(const CFontPtr &font) override;

  void pixelToWindow(double px, double py, double *wx, double *wy) override;
  void windowToPixel(double wx, double wy, double *px, double *py) override;

  void drawText(const CPoint2D &p, const std::string &str, const CRGBA &c) override;

  void drawRotatedText(const CPoint2D &p, const std::string &str, double a,
                       const HAlignPos &halignPos, const VAlignPos &valignPos,
                       const CRGBA &c) override;

 protected:
  CGnuPlotTextRenderer *renderer_;
  CBBox2D               bbox_;
};

#endif
