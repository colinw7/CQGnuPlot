#ifndef CGnuPlotNameValues_H
#define CGnuPlotNameValues_H

#include <CStrUniqueMatch.h>

#define CStrUniqueMatchInitT(T) \
template<> inline void \
initNameValues<T>(CStrUniqueMatchValues<T> &nameValues)

namespace CStrUniqueMatch {
  CStrUniqueMatchInitT(CGnuPlot::CommandName) {
    nameValues.addValues({{"bind"    , CGnuPlot::CommandName::BIND},
                          {"call"    , CGnuPlot::CommandName::CALL},
                          {"cd"      , CGnuPlot::CommandName::CD},
                          {"clear"   , CGnuPlot::CommandName::CLEAR},
                          {"do"      , CGnuPlot::CommandName::DO},
                          {"evaluate", CGnuPlot::CommandName::EVALUATE},
                          {"exit"    , CGnuPlot::CommandName::EXIT},
                          {"fit"     , CGnuPlot::CommandName::FIT},
                          {"help"    , CGnuPlot::CommandName::HELP},
                          {"history" , CGnuPlot::CommandName::HISTORY},
                          {"if"      , CGnuPlot::CommandName::IF},
                          {"load"    , CGnuPlot::CommandName::LOAD},
                          {"lower"   , CGnuPlot::CommandName::LOWER},
                          {"pause"   , CGnuPlot::CommandName::PAUSE},
                          {"plot"    , CGnuPlot::CommandName::PLOT},
                          {"print"   , CGnuPlot::CommandName::PRINT},
                          {"pwd"     , CGnuPlot::CommandName::PWD},
                          {"quit"    , CGnuPlot::CommandName::QUIT},
                          {"raise"   , CGnuPlot::CommandName::RAISE},
                          {"replot"  , CGnuPlot::CommandName::REPLOT},
                          {"refresh" , CGnuPlot::CommandName::REFRESH},
                          {"reread"  , CGnuPlot::CommandName::REREAD},
                          {"reset"   , CGnuPlot::CommandName::RESET},
                          {"save"    , CGnuPlot::CommandName::SAVE},
                          {"set"     , CGnuPlot::CommandName::SET},
                          {"shell"   , CGnuPlot::CommandName::SHELL},
                          {"show"    , CGnuPlot::CommandName::SHOW},
                          {"splot"   , CGnuPlot::CommandName::SPLOT},
                          {"stats"   , CGnuPlot::CommandName::STATS},
                          {"system"  , CGnuPlot::CommandName::SYSTEM},
                          {"test"    , CGnuPlot::CommandName::TEST},
                          {"unset"   , CGnuPlot::CommandName::UNSET},
                          {"update"  , CGnuPlot::CommandName::UPDATE},
                          {"while"   , CGnuPlot::CommandName::WHILE}});
  }

  CStrUniqueMatchInitT(CGnuPlot::VariableName) {
    nameValues.addValues({{"datafile"   , CGnuPlot::VariableName::DATAFILE},
                          {"decimalsign", CGnuPlot::VariableName::DECIMALSIGN},
                          {"encoding"   , CGnuPlot::VariableName::ENCODING},
                          {"fit"        , CGnuPlot::VariableName::FIT},
                          {"fontpath"   , CGnuPlot::VariableName::FONTPATH},
                          {"history"    , CGnuPlot::VariableName::HISTORY},
                          {"historysize", CGnuPlot::VariableName::HISTORYSIZE},
                          {"loadpath"   , CGnuPlot::VariableName::LOADPATH},
                          {"macros"     , CGnuPlot::VariableName::MACROS},
                          {"mouse"      , CGnuPlot::VariableName::MOUSE},

                          {"angles"     , CGnuPlot::VariableName::ANGLES},
                          {"dummy"      , CGnuPlot::VariableName::DUMMY},
                          {"mapping"    , CGnuPlot::VariableName::MAPPING},
                          {"parametric" , CGnuPlot::VariableName::PARAMETRIC},
                          {"polar"      , CGnuPlot::VariableName::POLAR},
                          {"samples"    , CGnuPlot::VariableName::SAMPLES},
                          {"zero"       , CGnuPlot::VariableName::ZERO},

                          {"border"     , CGnuPlot::VariableName::BORDER},
                          {"clip"       , CGnuPlot::VariableName::CLIP},
                          {"logscale"   , CGnuPlot::VariableName::LOGSCALE},
                          {"bmargin"    , CGnuPlot::VariableName::BMARGIN},
                          {"lmargin"    , CGnuPlot::VariableName::LMARGIN},
                          {"rmargin"    , CGnuPlot::VariableName::RMARGIN},
                          {"tmargin"    , CGnuPlot::VariableName::TMARGIN},
                          {"margins"    , CGnuPlot::VariableName::MARGINS},
                          {"multiplot"  , CGnuPlot::VariableName::MULTIPLOT},
                          {"offsets"    , CGnuPlot::VariableName::OFFSETS},
                          {"origin"     , CGnuPlot::VariableName::ORIGIN},
                          {"size"       , CGnuPlot::VariableName::SIZE},

                          {"arrow"      , CGnuPlot::VariableName::ARROW},
                          {"key"        , CGnuPlot::VariableName::KEY},
                          {"label"      , CGnuPlot::VariableName::LABEL},
                          {"object"     , CGnuPlot::VariableName::OBJECT},
                          {"timestamp"  , CGnuPlot::VariableName::TIMESTAMP},
                          {"title"      , CGnuPlot::VariableName::TITLE},

                          {"style"      , CGnuPlot::VariableName::STYLE},
                          {"dashtype"   , CGnuPlot::VariableName::DASHTYPE},

                          {"bars"       , CGnuPlot::VariableName::BARS},
                          {"boxwidth"   , CGnuPlot::VariableName::BOXWIDTH},
                          {"pointsize"  , CGnuPlot::VariableName::POINTSIZE},

                          {"autoscale"  , CGnuPlot::VariableName::AUTOSCALE},
                          {"format"     , CGnuPlot::VariableName::FORMAT},
                          {"grid"       , CGnuPlot::VariableName::GRID},
                          {"xlabel"     , CGnuPlot::VariableName::XLABEL}, // z, cb
                          {"ylabel"     , CGnuPlot::VariableName::YLABEL},
                          {"x2label"    , CGnuPlot::VariableName::X2LABEL},
                          {"y2label"    , CGnuPlot::VariableName::Y2LABEL},
                          {"xrange"     , CGnuPlot::VariableName::XRANGE},
                          {"x2range"    , CGnuPlot::VariableName::X2RANGE},
                          {"yrange"     , CGnuPlot::VariableName::YRANGE},
                          {"y2range"    , CGnuPlot::VariableName::Y2RANGE},
                          {"zrange"     , CGnuPlot::VariableName::ZRANGE},
                          {"cbrange"    , CGnuPlot::VariableName::CBRANGE},
                          {"rrange"     , CGnuPlot::VariableName::RRANGE},
                          {"trange"     , CGnuPlot::VariableName::TRANGE},
                          {"tics"       , CGnuPlot::VariableName::TICS},
                          {"xtics"      , CGnuPlot::VariableName::XTICS},
                          {"ytics"      , CGnuPlot::VariableName::YTICS},
                          {"x2tics"     , CGnuPlot::VariableName::X2TICS},
                          {"y2tics"     , CGnuPlot::VariableName::Y2TICS},
                          {"ztics"      , CGnuPlot::VariableName::ZTICS},
                          {"cbtics"     , CGnuPlot::VariableName::CBTICS},
                          {"rtics"      , CGnuPlot::VariableName::RTICS},
                          {"mxtics"     , CGnuPlot::VariableName::MXTICS},
                          {"mytics"     , CGnuPlot::VariableName::MYTICS},
                          {"mztics"     , CGnuPlot::VariableName::MZTICS},
                          {"mx2tics"    , CGnuPlot::VariableName::MX2TICS},
                          {"my2tics"    , CGnuPlot::VariableName::MY2TICS},
                          {"ticscale"   , CGnuPlot::VariableName::TICSCALE},
                          {"zeroaxis"   , CGnuPlot::VariableName::ZEROAXIS},
                          {"raxis"      , CGnuPlot::VariableName::RAXIS},
                          {"paxis"      , CGnuPlot::VariableName::PAXIS},
                          {"link"       , CGnuPlot::VariableName::LINK},

                          {"locale"     , CGnuPlot::VariableName::LOCALE},
                          {"timefmt"    , CGnuPlot::VariableName::TIMEFMT},
                          {"xdata"      , CGnuPlot::VariableName::XDATA},
                          {"ydata"      , CGnuPlot::VariableName::YDATA},
                          {"x2data"     , CGnuPlot::VariableName::X2DATA},
                          {"y2data"     , CGnuPlot::VariableName::Y2DATA},
                          {"xdtics"     , CGnuPlot::VariableName::XDTICS},
                          {"xmtics"     , CGnuPlot::VariableName::XMTICS},

                          {"dgrid3d"    , CGnuPlot::VariableName::DGRID3D},
                          {"hidden3d"   , CGnuPlot::VariableName::HIDDEN3D},
                          {"isosamples" , CGnuPlot::VariableName::ISOSAMPLES},
                          {"surface"    , CGnuPlot::VariableName::SURFACE},
                          {"view"       , CGnuPlot::VariableName::VIEW},
                          {"xyplane"    , CGnuPlot::VariableName::XYPLANE},

                          {"cplane"     , CGnuPlot::VariableName::CPLANE},
                          {"cntrparam"  , CGnuPlot::VariableName::CNTRPARAM},
                          {"contour"    , CGnuPlot::VariableName::CONTOUR},

                          {"colorbox"   , CGnuPlot::VariableName::COLORBOX},
                          {"palette"    , CGnuPlot::VariableName::PALETTE},
                          {"pm3d"       , CGnuPlot::VariableName::PM3D},

                          {"output"     , CGnuPlot::VariableName::OUTPUT},
                          {"print"      , CGnuPlot::VariableName::PRINT},
                          {"table"      , CGnuPlot::VariableName::TABLE},
                          {"terminal"   , CGnuPlot::VariableName::TERMINAL},
                          {"termoption" , CGnuPlot::VariableName::TERMOPTION},

                          {"variables"  , CGnuPlot::VariableName::VARIABLES},
                          {"functions"  , CGnuPlot::VariableName::FUNCTIONS},
                          {"version"    , CGnuPlot::VariableName::VERSION},

                          {"debug"      , CGnuPlot::VariableName::DEBUG},
                          {"edebug"     , CGnuPlot::VariableName::EDEBUG}});
  }

  CStrUniqueMatchInitT(CGnuPlot::DataFileVar) {
    nameValues.addValues({{"separator"   , CGnuPlot::DataFileVar::SEPARATOR},
                          {"commentschar", CGnuPlot::DataFileVar::COMMENTS_CHAR},
                          {"missing"     , CGnuPlot::DataFileVar::MISSING}});
  }

  CStrUniqueMatchInitT(CGnuPlot::StyleVar) {
    nameValues.addValues({{"data"     , CGnuPlot::StyleVar::DATA},
                          {"function" , CGnuPlot::StyleVar::FUNCTION},
                          {"increment", CGnuPlot::StyleVar::INCREMENT},
                          {"line"     , CGnuPlot::StyleVar::LINE},
                          {"fill"     , CGnuPlot::StyleVar::FILL},
                          {"boxplot"  , CGnuPlot::StyleVar::BOXPLOT},
                          {"histogram", CGnuPlot::StyleVar::HISTOGRAM},
                          {"rectangle", CGnuPlot::StyleVar::RECTANGLE},
                          {"circle"   , CGnuPlot::StyleVar::CIRCLE},
                          {"ellipse"  , CGnuPlot::StyleVar::ELLIPSE},
                          {"arrow"    , CGnuPlot::StyleVar::ARROW}});
  }

  CStrUniqueMatchInitT(CGnuPlot::PlotVar) {
    nameValues.addValues({{"index"      , CGnuPlot::PlotVar::INDEX},
                          {"every"      , CGnuPlot::PlotVar::EVERY},
                          {"using"      , CGnuPlot::PlotVar::USING},
                          {"axes"       , CGnuPlot::PlotVar::AXES},
                          {"t"          , CGnuPlot::PlotVar::TITLE},
                          {"title"      , CGnuPlot::PlotVar::TITLE},
                          {"notitle"    , CGnuPlot::PlotVar::NOTITLE},
                          {"smooth"     , CGnuPlot::PlotVar::SMOOTH},
                          {"w"          , CGnuPlot::PlotVar::WITH},
                          {"with"       , CGnuPlot::PlotVar::WITH},
                          {"linetype"   , CGnuPlot::PlotVar::LINETYPE},
                          {"lt"         , CGnuPlot::PlotVar::LINETYPE},
                          {"linewidth"  , CGnuPlot::PlotVar::LINEWIDTH},
                          {"lw"         , CGnuPlot::PlotVar::LINEWIDTH},
                          {"fillstyle"  , CGnuPlot::PlotVar::FILLSTYLE},
                          {"fs"         , CGnuPlot::PlotVar::FILLSTYLE},
                          {"dashtype"   , CGnuPlot::PlotVar::DASHTYPE},
                          {"dt"         , CGnuPlot::PlotVar::DASHTYPE},
                          {"pointsize"  , CGnuPlot::PlotVar::POINTSIZE},
                          {"arrowstyle" , CGnuPlot::PlotVar::ARROWSTYLE},
                          {"as"         , CGnuPlot::PlotVar::ARROWSTYLE},
                          {"heads"      , CGnuPlot::PlotVar::HEADS},
                          {"binary"     , CGnuPlot::PlotVar::BINARY},
                          {"matrix"     , CGnuPlot::PlotVar::MATRIX},
                          {"array"      , CGnuPlot::PlotVar::ARRAY},
                          {"skip"       , CGnuPlot::PlotVar::SKIP},
                          {"flip"       , CGnuPlot::PlotVar::FLIP},
                          {"flipx"      , CGnuPlot::PlotVar::FLIPX},
                          {"flipy"      , CGnuPlot::PlotVar::FLIPY},
                          {"flipz"      , CGnuPlot::PlotVar::FLIPZ},
                          {"format"     , CGnuPlot::PlotVar::FORMAT},
                          {"endian"     , CGnuPlot::PlotVar::ENDIAN},
                          {"filetype"   , CGnuPlot::PlotVar::FILETYPE},
                          {"scan"       , CGnuPlot::PlotVar::SCAN},
                          {"transpose"  , CGnuPlot::PlotVar::TRANSPOSE},
                          {"origin"     , CGnuPlot::PlotVar::ORIGIN},
                          {"center"     , CGnuPlot::PlotVar::CENTER},
                          {"dx"         , CGnuPlot::PlotVar::DX},
                          {"dy"         , CGnuPlot::PlotVar::DY},
                          {"dz"         , CGnuPlot::PlotVar::DZ},
                          {"rotation"   , CGnuPlot::PlotVar::ROTATION},
                          {"rotate"     , CGnuPlot::PlotVar::ROTATION},
                          {"whiskerbars", CGnuPlot::PlotVar::WHISKERBARS}});
  }

  CStrUniqueMatchInitT(CGnuPlot::PlotStyle) {
    nameValues.addValues({{"boxerrorbars"  , CGnuPlot::PlotStyle::BOXERRORBARS},
                          {"boxes"         , CGnuPlot::PlotStyle::BOXES},
                          {"boxplot"       , CGnuPlot::PlotStyle::BOXPLOT},
                          {"boxxyerrorbars", CGnuPlot::PlotStyle::BOXXYERRORBARS},
                          {"bubblechart"   , CGnuPlot::PlotStyle::BUBBLECHART},
                          {"candlesticks"  , CGnuPlot::PlotStyle::CANDLESTICKS},
                          {"circles"       , CGnuPlot::PlotStyle::CIRCLES},
                          {"dots"          , CGnuPlot::PlotStyle::DOTS},
                          {"ellipses"      , CGnuPlot::PlotStyle::ELLIPSES},
                          {"errorbars"     , CGnuPlot::PlotStyle::ERRORBARS},
                          {"errorlines"    , CGnuPlot::PlotStyle::ERRORLINES},
                          {"filledcurves"  , CGnuPlot::PlotStyle::FILLEDCURVES},
                          {"fillsteps"     , CGnuPlot::PlotStyle::FILLSTEPS},
                          {"financebars"   , CGnuPlot::PlotStyle::FINANCEBARS},
                          {"fsteps"        , CGnuPlot::PlotStyle::FSTEPS},
                          {"histeps"       , CGnuPlot::PlotStyle::HISTEPS},
                          {"histograms"    , CGnuPlot::PlotStyle::HISTOGRAMS},
                          {"image"         , CGnuPlot::PlotStyle::IMAGE},
                          {"impulses"      , CGnuPlot::PlotStyle::IMPULSES},
                          {"labels"        , CGnuPlot::PlotStyle::LABELS},
                          {"l"             , CGnuPlot::PlotStyle::LINES},
                          {"line"          , CGnuPlot::PlotStyle::LINES},
                          {"lines"         , CGnuPlot::PlotStyle::LINES},
                          {"linespoints"   , CGnuPlot::PlotStyle::LINES_POINTS},
                          {"parallelaxes"  , CGnuPlot::PlotStyle::PARALLELAXES},
                          {"p"             , CGnuPlot::PlotStyle::POINTS},
                          {"piechart"      , CGnuPlot::PlotStyle::PIECHART},
                          {"pm3d"          , CGnuPlot::PlotStyle::PM3D},
                          {"points"        , CGnuPlot::PlotStyle::POINTS},
                          {"rgbalpha"      , CGnuPlot::PlotStyle::RGBALPHA},
                          {"rgbimage"      , CGnuPlot::PlotStyle::RGBIMAGE},
                          {"steps"         , CGnuPlot::PlotStyle::STEPS},
                          {"vectors"       , CGnuPlot::PlotStyle::VECTORS},
                          {"xerrorbars"    , CGnuPlot::PlotStyle::XERRORBARS},
                          {"xerrorlines"   , CGnuPlot::PlotStyle::XERRORLINES},
                          {"xyerrorbars"   , CGnuPlot::PlotStyle::XYERRORBARS},
                          {"xyerrorlines"  , CGnuPlot::PlotStyle::XYERRORLINES},
                          {"yerrorbars"    , CGnuPlot::PlotStyle::YERRORBARS},
                          {"yerrorlines"   , CGnuPlot::PlotStyle::YERRORLINES}});
  }

  CStrUniqueMatchInitT(CGnuPlot::Smooth) {
    nameValues.addValues({{"unique"   , CGnuPlot::Smooth::UNIQUE},
                          {"frequency", CGnuPlot::Smooth::FREQUENCY},
                          {"bezier"   , CGnuPlot::Smooth::BEZIER},
                          {"sbezier"  , CGnuPlot::Smooth::SBEZIER},
                          {"csplines" , CGnuPlot::Smooth::CSPLINES},
                          {"acsplines", CGnuPlot::Smooth::ACSPLINES}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::FillType) {
    nameValues.addValues({{"empty"  , CGnuPlotTypes::FillType::EMPTY},
                          {"solid"  , CGnuPlotTypes::FillType::SOLID},
                          {"pattern", CGnuPlotTypes::FillType::PATTERN}});
  }

  CStrUniqueMatchInitT(CGnuPlot::HistogramStyle) {
    nameValues.addValues({{"clustered"    , CGnuPlot::HistogramStyle::CLUSTERED},
                          {"errorbars"    , CGnuPlot::HistogramStyle::ERRORBARS},
                          {"rowstacked"   , CGnuPlot::HistogramStyle::ROWSTACKED},
                          {"columnstacked", CGnuPlot::HistogramStyle::COLUMNSTACKED}});
  }

  CStrUniqueMatchInitT(CGnuPlot::AngleType) {
    nameValues.addValues({{"radians", CGnuPlot::AngleType::RADIANS},
                          {"degrees", CGnuPlot::AngleType::DEGREES}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::CoordSys) {
    nameValues.addValues({{"first"  , CGnuPlotTypes::CoordSys::FIRST    },
                          {"second" , CGnuPlotTypes::CoordSys::SECOND   },
                          {"graph"  , CGnuPlotTypes::CoordSys::GRAPH    },
                          {"screen" , CGnuPlotTypes::CoordSys::SCREEN   },
                          {"degrees", CGnuPlotTypes::CoordSys::CHARACTER}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::ObjectType) {
    nameValues.addValues({{"circle"   , CGnuPlotTypes::ObjectType::CIRCLE   },
                          {"ellipse"  , CGnuPlotTypes::ObjectType::ELLIPSE  },
                          {"polygon"  , CGnuPlotTypes::ObjectType::POLYGON  },
                          {"rectangle", CGnuPlotTypes::ObjectType::RECTANGLE}});
  }
}

#endif
