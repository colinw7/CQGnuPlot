#ifndef CGnuPlotPlotCacheFactoriesDef_H
#define CGnuPlotPlotCacheFactoriesDef_H

inline CGnuPlotArrowObject *
CGnuPlotCacheFactory<CGnuPlotArrowObject>::
make()
{
  return plot_->createArrowObject();
}

inline CGnuPlotBoxBarObject *
CGnuPlotCacheFactory<CGnuPlotBoxBarObject>::
make()
{
  return plot_->createBoxBarObject();
}

inline CGnuPlotBoxObject *
CGnuPlotCacheFactory<CGnuPlotBoxObject>::
make()
{
  return plot_->createBoxObject();
}

inline CGnuPlotBubbleObject *
CGnuPlotCacheFactory<CGnuPlotBubbleObject>::
make()
{
  return plot_->createBubbleObject();
}

inline CGnuPlotEllipseObject *
CGnuPlotCacheFactory<CGnuPlotEllipseObject>::
make()
{
  return plot_->createEllipseObject();
}

inline CGnuPlotErrorBarObject *
CGnuPlotCacheFactory<CGnuPlotErrorBarObject>::
make()
{
  return plot_->createErrorBarObject();
}

inline CGnuPlotFinanceBarObject *
CGnuPlotCacheFactory<CGnuPlotFinanceBarObject>::
make()
{
  return plot_->createFinanceBarObject();
}

inline CGnuPlotImageObject *
CGnuPlotCacheFactory<CGnuPlotImageObject>::
make()
{
  return plot_->createImageObject();
}

inline CGnuPlotLabelObject *
CGnuPlotCacheFactory<CGnuPlotLabelObject>::
make()
{
  return plot_->createLabelObject();
}

inline CGnuPlotPathObject *
CGnuPlotCacheFactory<CGnuPlotPathObject>::
make()
{
  return plot_->createPathObject();
}

inline CGnuPlotPieObject *
CGnuPlotCacheFactory<CGnuPlotPieObject>::
make()
{
  return plot_->createPieObject();
}

inline CGnuPlotPointObject *
CGnuPlotCacheFactory<CGnuPlotPointObject>::
make()
{
  return plot_->createPointObject();
}

inline CGnuPlotPolygonObject *
CGnuPlotCacheFactory<CGnuPlotPolygonObject>::
make()
{
  return plot_->createPolygonObject();
}

inline CGnuPlotRectObject *
CGnuPlotCacheFactory<CGnuPlotRectObject>::
make()
{
  return plot_->createRectObject();
}

#endif
