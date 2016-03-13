#ifndef CGnuPlotPlotCacheFactoriesDcl_H
#define CGnuPlotPlotCacheFactoriesDcl_H

#include <CGnuPlotCache.h>

template<>
class CGnuPlotCacheFactory<CGnuPlotArrowObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotArrowObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBoxBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBoxBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBoxObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBoxObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotBubbleObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotBubbleObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotEllipseObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotEllipseObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotErrorBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotErrorBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotFinanceBarObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotFinanceBarObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotImageObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotImageObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotLabelObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotLabelObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPathObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPathObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPieObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPieObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPointObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPointObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotPolygonObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotPolygonObject *make();

 private:
  CGnuPlotPlot *plot_;
};

template<>
class CGnuPlotCacheFactory<CGnuPlotRectObject> {
 public:
  CGnuPlotCacheFactory(CGnuPlotPlot *plot) :
   plot_(plot) {
  }

  CGnuPlotRectObject *make();

 private:
  CGnuPlotPlot *plot_;
};

#endif
