#ifndef CGnuPlotWindow_H
#define CGnuPlotWindow_H

#include <vector>

#include <CGnuPlot.h>

class CGnuPlotRenderer;

class CGnuPlotWindow {
 public:
  typedef std::vector<CGnuPlotPlot *> Plots;

 public:
  CGnuPlotWindow(CGnuPlot *plot);

  virtual ~CGnuPlotWindow();

  void clear();

  void set3D(bool b);
  bool is3D() const { return is3D_; }

  CGnuPlot *plot() const { return plot_; }

  void addPlot(CGnuPlotPlot *plot);

  const Plots &plots() const { return plots_; }
  uint numPlots() const { return plots_.size(); }

  void setRenderer(CGnuPlotRenderer *renderer);

  virtual void stateChanged(CGnuPlot::ChangeState) { }

  const CGnuPlot::Title &title() const { return title_; }
  CGnuPlot::Title &title() { return title_; }
  void setTitle(const CGnuPlot::Title &t) { title_ = t; }

  bool hidden3D() const { return hidden3D_; }
  void setHidden3D(bool b) { hidden3D_ = b; }

  bool surface3D() const { return surface3D_; }
  void setSurface3D(bool b) { surface3D_ = b; }

  bool contour3D() const { return contour3D_; }
  void setContour3D(bool b) { contour3D_ = b; }

  bool pm3D() const { return pm3D_; }
  void setPm3D(bool b) { pm3D_ = b; }

  const CGnuPlot::Camera &camera() const { return camera_; }
  void setCamera(const CGnuPlot::Camera &c) { camera_ = c; }

  void setCameraEnabled(bool b);

  void setCameraRotateX(double a);
  void setCameraRotateY(double a);
  void setCameraRotateZ(double a);

  void setCameraXMin(double x);
  void setCameraXMax(double x);
  void setCameraYMin(double y);
  void setCameraYMax(double y);
  void setCameraNear(double z);
  void setCameraFar (double z);

  void reset3D();

  void draw();

 private:
  void paintStart();
  void paintEnd();

 private:
  bool              is3D_;
  CGnuPlot*         plot_;
  CGnuPlotRenderer* renderer_;
  Plots             plots_;
  CGnuPlot::Title   title_;
  CGnuPlot::Camera  camera_;
  bool              hidden3D_;
  bool              surface3D_;
  bool              contour3D_;
  bool              pm3D_;
};

#endif
