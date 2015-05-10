#ifndef CGnuPlotNameValues_H
#define CGnuPlotNameValues_H

#include <CStrUniqueMatch.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlot.h>

#define CStrUniqueMatchInitT(T) \
template<> inline void \
initNameValues<T>(CStrUniqueMatchValues<T> &nameValues)

namespace CStrUniqueMatch {
  CStrUniqueMatchInitT(CGnuPlotTypes::CommandName) {
    nameValues.addValues({{"bind"    , CGnuPlotTypes::CommandName::BIND},
                          {"call"    , CGnuPlotTypes::CommandName::CALL},
                          {"cd"      , CGnuPlotTypes::CommandName::CD},
                          {"clear"   , CGnuPlotTypes::CommandName::CLEAR},
                          {"do"      , CGnuPlotTypes::CommandName::DO},
                          {"evaluate", CGnuPlotTypes::CommandName::EVALUATE},
                          {"exit"    , CGnuPlotTypes::CommandName::EXIT},
                          {"fit"     , CGnuPlotTypes::CommandName::FIT},
                          {"get"     , CGnuPlotTypes::CommandName::GET},
                          {"help"    , CGnuPlotTypes::CommandName::HELP},
                          {"history" , CGnuPlotTypes::CommandName::HISTORY},
                          {"if"      , CGnuPlotTypes::CommandName::IF},
                          {"load"    , CGnuPlotTypes::CommandName::LOAD},
                          {"lower"   , CGnuPlotTypes::CommandName::LOWER},
                          {"pause"   , CGnuPlotTypes::CommandName::PAUSE},
                          {"plot"    , CGnuPlotTypes::CommandName::PLOT},
                          {"print"   , CGnuPlotTypes::CommandName::PRINT},
                          {"printf"  , CGnuPlotTypes::CommandName::PRINTF},
                          {"pwd"     , CGnuPlotTypes::CommandName::PWD},
                          {"quit"    , CGnuPlotTypes::CommandName::QUIT},
                          {"raise"   , CGnuPlotTypes::CommandName::RAISE},
                          {"replot"  , CGnuPlotTypes::CommandName::REPLOT},
                          {"refresh" , CGnuPlotTypes::CommandName::REFRESH},
                          {"reread"  , CGnuPlotTypes::CommandName::REREAD},
                          {"reset"   , CGnuPlotTypes::CommandName::RESET},
                          {"save"    , CGnuPlotTypes::CommandName::SAVE},
                          {"set"     , CGnuPlotTypes::CommandName::SET},
                          {"shell"   , CGnuPlotTypes::CommandName::SHELL},
                          {"show"    , CGnuPlotTypes::CommandName::SHOW},
                          {"splot"   , CGnuPlotTypes::CommandName::SPLOT},
                          {"stats"   , CGnuPlotTypes::CommandName::STATS},
                          {"system"  , CGnuPlotTypes::CommandName::SYSTEM},
                          {"test"    , CGnuPlotTypes::CommandName::TEST},
                          {"undefine", CGnuPlotTypes::CommandName::UNDEFINE},
                          {"unset"   , CGnuPlotTypes::CommandName::UNSET},
                          {"update"  , CGnuPlotTypes::CommandName::UPDATE},
                          {"while"   , CGnuPlotTypes::CommandName::WHILE}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::VariableName) {
    nameValues.addValues({{"angles"          , CGnuPlotTypes::VariableName::ANGLES},
                          {"arrow"           , CGnuPlotTypes::VariableName::ARROW},
                          {"autoscale"       , CGnuPlotTypes::VariableName::AUTOSCALE},
                          {"bars"            , CGnuPlotTypes::VariableName::BARS},
                          {"bmargin"         , CGnuPlotTypes::VariableName::BMARGIN},
                          {"border"          , CGnuPlotTypes::VariableName::BORDER},
                          {"boxwidth"        , CGnuPlotTypes::VariableName::BOXWIDTH},
                          {"cbdata"          , CGnuPlotTypes::VariableName::CBDATA},
                          {"cbdtics"         , CGnuPlotTypes::VariableName::CBDTICS},
                          {"cblabel"         , CGnuPlotTypes::VariableName::CBLABEL},
                          {"cbmtics"         , CGnuPlotTypes::VariableName::CBMTICS},
                          {"cbrange"         , CGnuPlotTypes::VariableName::CBRANGE},
                          {"cbtics"          , CGnuPlotTypes::VariableName::CBTICS},
                          {"clip"            , CGnuPlotTypes::VariableName::CLIP},
                          {"cntrlabel"       , CGnuPlotTypes::VariableName::CNTRLABEL},
                          {"cntrparam"       , CGnuPlotTypes::VariableName::CNTRPARAM},
                          {"colorbox"        , CGnuPlotTypes::VariableName::COLORBOX},
                          {"colornames"      , CGnuPlotTypes::VariableName::COLORNAMES},
                          {"colorsequence"   , CGnuPlotTypes::VariableName::COLORSEQUENCE},
                          {"contour"         , CGnuPlotTypes::VariableName::CONTOUR},
                          {"cplane"          , CGnuPlotTypes::VariableName::CPLANE},
                          {"dashtype"        , CGnuPlotTypes::VariableName::DASHTYPE},
                          {"datafile"        , CGnuPlotTypes::VariableName::DATAFILE},
                          {"debug"           , CGnuPlotTypes::VariableName::DEBUG},
                          {"decimalsign"     , CGnuPlotTypes::VariableName::DECIMALSIGN},
                          {"dgrid3d"         , CGnuPlotTypes::VariableName::DGRID3D},
                          {"dt"              , CGnuPlotTypes::VariableName::DASHTYPE},
                          {"dummy"           , CGnuPlotTypes::VariableName::DUMMY},
                          {"edebug"          , CGnuPlotTypes::VariableName::EDEBUG},
                          {"encoding"        , CGnuPlotTypes::VariableName::ENCODING},
                          {"fit"             , CGnuPlotTypes::VariableName::FIT},
                          {"fontpath"        , CGnuPlotTypes::VariableName::FONTPATH},
                          {"format"          , CGnuPlotTypes::VariableName::FORMAT},
                          {"functions"       , CGnuPlotTypes::VariableName::FUNCTIONS},
                          {"grid"            , CGnuPlotTypes::VariableName::GRID},
                          {"hidden3d"        , CGnuPlotTypes::VariableName::HIDDEN3D},
                          {"history"         , CGnuPlotTypes::VariableName::HISTORY},
                          {"historysize"     , CGnuPlotTypes::VariableName::HISTORYSIZE},
                          {"isosamples"      , CGnuPlotTypes::VariableName::ISOSAMPLES},
                          {"key"             , CGnuPlotTypes::VariableName::KEY},
                          {"label"           , CGnuPlotTypes::VariableName::LABEL},
                          {"linetype"        , CGnuPlotTypes::VariableName::LINETYPE},
                          {"link"            , CGnuPlotTypes::VariableName::LINK},
                          {"lmargin"         , CGnuPlotTypes::VariableName::LMARGIN},
                          {"loadpath"        , CGnuPlotTypes::VariableName::LOADPATH},
                          {"locale"          , CGnuPlotTypes::VariableName::LOCALE},
                          {"logscale"        , CGnuPlotTypes::VariableName::LOGSCALE},
                          {"macros"          , CGnuPlotTypes::VariableName::MACROS},
                          {"mapping"         , CGnuPlotTypes::VariableName::MAPPING},
                          {"margins"         , CGnuPlotTypes::VariableName::MARGINS},
                          {"mouse"           , CGnuPlotTypes::VariableName::MOUSE},
                          {"multiplot"       , CGnuPlotTypes::VariableName::MULTIPLOT},
                          {"mx2tics"         , CGnuPlotTypes::VariableName::MX2TICS},
                          {"mxtics"          , CGnuPlotTypes::VariableName::MXTICS},
                          {"my2tics"         , CGnuPlotTypes::VariableName::MY2TICS},
                          {"mytics"          , CGnuPlotTypes::VariableName::MYTICS},
                          {"mztics"          , CGnuPlotTypes::VariableName::MZTICS},
                          {"object"          , CGnuPlotTypes::VariableName::OBJECT},
                          {"offsets"         , CGnuPlotTypes::VariableName::OFFSETS},
                          {"origin"          , CGnuPlotTypes::VariableName::ORIGIN},
                          {"output"          , CGnuPlotTypes::VariableName::OUTPUT},
                          {"palette"         , CGnuPlotTypes::VariableName::PALETTE},
                          {"parametric"      , CGnuPlotTypes::VariableName::PARAMETRIC},
                          {"paxis"           , CGnuPlotTypes::VariableName::PAXIS},
                          {"plot"            , CGnuPlotTypes::VariableName::PLOT},
                          {"pm3d"            , CGnuPlotTypes::VariableName::PM3D},
                          {"pointintervalbox", CGnuPlotTypes::VariableName::POINTINTERVALBOX},
                          {"pointsize"       , CGnuPlotTypes::VariableName::POINTSIZE},
                          {"polar"           , CGnuPlotTypes::VariableName::POLAR},
                          {"print"           , CGnuPlotTypes::VariableName::PRINT},
                          {"psdir"           , CGnuPlotTypes::VariableName::PSDIR},
                          {"raxis"           , CGnuPlotTypes::VariableName::RAXIS},
                          {"rmargin"         , CGnuPlotTypes::VariableName::RMARGIN},
                          {"rrange"          , CGnuPlotTypes::VariableName::RRANGE},
                          {"rtics"           , CGnuPlotTypes::VariableName::RTICS},
                          {"samples"         , CGnuPlotTypes::VariableName::SAMPLES},
                          {"size"            , CGnuPlotTypes::VariableName::SIZE},
                          {"style"           , CGnuPlotTypes::VariableName::STYLE},
                          {"surface"         , CGnuPlotTypes::VariableName::SURFACE},
                          {"table"           , CGnuPlotTypes::VariableName::TABLE},
                          {"terminal"        , CGnuPlotTypes::VariableName::TERMINAL},
                          {"termoption"      , CGnuPlotTypes::VariableName::TERMOPTION},
                          {"ticscale"        , CGnuPlotTypes::VariableName::TICSCALE},
                          {"ticslevel"       , CGnuPlotTypes::VariableName::TICSLEVEL},
                          {"tics"            , CGnuPlotTypes::VariableName::TICS},
                          {"time"            , CGnuPlotTypes::VariableName::TIMESTAMP},
                          {"timefmt"         , CGnuPlotTypes::VariableName::TIMEFMT},
                          {"timestamp"       , CGnuPlotTypes::VariableName::TIMESTAMP},
                          {"title"           , CGnuPlotTypes::VariableName::TITLE},
                          {"tmargin"         , CGnuPlotTypes::VariableName::TMARGIN},
                          {"trange"          , CGnuPlotTypes::VariableName::TRANGE},
                          {"urange"          , CGnuPlotTypes::VariableName::URANGE},
                          {"variables"       , CGnuPlotTypes::VariableName::VARIABLES},
                          {"version"         , CGnuPlotTypes::VariableName::VERSION},
                          {"view"            , CGnuPlotTypes::VariableName::VIEW},
                          {"vrange"          , CGnuPlotTypes::VariableName::VRANGE},
                          {"x2data"          , CGnuPlotTypes::VariableName::X2DATA},
                          {"x2dtics"         , CGnuPlotTypes::VariableName::X2DTICS},
                          {"x2label"         , CGnuPlotTypes::VariableName::X2LABEL},
                          {"x2mtics"         , CGnuPlotTypes::VariableName::X2MTICS},
                          {"x2range"         , CGnuPlotTypes::VariableName::X2RANGE},
                          {"x2tics"          , CGnuPlotTypes::VariableName::X2TICS},
                          {"x2zeroaxis"      , CGnuPlotTypes::VariableName::X2ZEROAXIS},
                          {"xdata"           , CGnuPlotTypes::VariableName::XDATA},
                          {"xdtics"          , CGnuPlotTypes::VariableName::XDTICS},
                          {"xlabel"          , CGnuPlotTypes::VariableName::XLABEL},
                          {"xmtics"          , CGnuPlotTypes::VariableName::XMTICS},
                          {"xrange"          , CGnuPlotTypes::VariableName::XRANGE},
                          {"xtics"           , CGnuPlotTypes::VariableName::XTICS},
                          {"xyplane"         , CGnuPlotTypes::VariableName::XYPLANE},
                          {"xzeroaxis"       , CGnuPlotTypes::VariableName::XZEROAXIS},
                          {"y2data"          , CGnuPlotTypes::VariableName::Y2DATA},
                          {"y2dtics"         , CGnuPlotTypes::VariableName::Y2DTICS},
                          {"y2label"         , CGnuPlotTypes::VariableName::Y2LABEL},
                          {"y2mtics"         , CGnuPlotTypes::VariableName::Y2MTICS},
                          {"y2range"         , CGnuPlotTypes::VariableName::Y2RANGE},
                          {"y2tics"          , CGnuPlotTypes::VariableName::Y2TICS},
                          {"y2zeroaxis"      , CGnuPlotTypes::VariableName::Y2ZEROAXIS},
                          {"ydata"           , CGnuPlotTypes::VariableName::YDATA},
                          {"ydtics"          , CGnuPlotTypes::VariableName::YDTICS},
                          {"ylabel"          , CGnuPlotTypes::VariableName::YLABEL},
                          {"ymtics"          , CGnuPlotTypes::VariableName::YMTICS},
                          {"yrange"          , CGnuPlotTypes::VariableName::YRANGE},
                          {"ytics"           , CGnuPlotTypes::VariableName::YTICS},
                          {"yzeroaxis"       , CGnuPlotTypes::VariableName::YZEROAXIS},
                          {"zdata"           , CGnuPlotTypes::VariableName::ZDATA},
                          {"zdtics"          , CGnuPlotTypes::VariableName::ZDTICS},
                          {"zeroaxis"        , CGnuPlotTypes::VariableName::ZEROAXIS},
                          {"zero"            , CGnuPlotTypes::VariableName::ZERO},
                          {"zlabel"          , CGnuPlotTypes::VariableName::ZLABEL},
                          {"zmtics"          , CGnuPlotTypes::VariableName::ZMTICS},
                          {"zrange"          , CGnuPlotTypes::VariableName::ZRANGE},
                          {"ztics"           , CGnuPlotTypes::VariableName::ZTICS},
                          {"zzeroaxis"       , CGnuPlotTypes::VariableName::ZZEROAXIS}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::DataFileVar) {
    nameValues.addValues({{"fortran"      , CGnuPlotTypes::DataFileVar::FORTRAN},
                          {"nofpe_trap"   , CGnuPlotTypes::DataFileVar::NOFPE_TRAP},
                          {"missing"      , CGnuPlotTypes::DataFileVar::MISSING},
                          {"separator"    , CGnuPlotTypes::DataFileVar::SEPARATOR},
                          {"commentschars", CGnuPlotTypes::DataFileVar::COMMENTS_CHARS},
                          {"binary"       , CGnuPlotTypes::DataFileVar::BINARY},
                          {"csv"          , CGnuPlotTypes::DataFileVar::CSV}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::StyleVar) {
    nameValues.addValues({{"arrow"    , CGnuPlotTypes::StyleVar::ARROW},
                          {"boxplot"  , CGnuPlotTypes::StyleVar::BOXPLOT},
                          {"circle"   , CGnuPlotTypes::StyleVar::CIRCLE},
                          {"data"     , CGnuPlotTypes::StyleVar::DATA},
                          {"ellipse"  , CGnuPlotTypes::StyleVar::ELLIPSE},
                          {"fill"     , CGnuPlotTypes::StyleVar::FILL},
                          {"function" , CGnuPlotTypes::StyleVar::FUNCTION},
                          {"histogram", CGnuPlotTypes::StyleVar::HISTOGRAM},
                          {"increment", CGnuPlotTypes::StyleVar::INCREMENT},
                          {"line"     , CGnuPlotTypes::StyleVar::LINE},
                          {"rectangle", CGnuPlotTypes::StyleVar::RECTANGLE},
                          {"textbox"  , CGnuPlotTypes::StyleVar::TEXTBOX}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::PlotVar) {
    nameValues.addValues({{"array"      , CGnuPlotTypes::PlotVar::ARRAY},
                          {"arrowstyle" , CGnuPlotTypes::PlotVar::ARROWSTYLE},
                          {"as"         , CGnuPlotTypes::PlotVar::ARROWSTYLE},
                          {"axes"       , CGnuPlotTypes::PlotVar::AXES},
                          {"binary"     , CGnuPlotTypes::PlotVar::BINARY},
                          {"center"     , CGnuPlotTypes::PlotVar::CENTER},
                          {"dashtype"   , CGnuPlotTypes::PlotVar::DASHTYPE},
                          {"dt"         , CGnuPlotTypes::PlotVar::DASHTYPE},
                          {"dx"         , CGnuPlotTypes::PlotVar::DX},
                          {"dy"         , CGnuPlotTypes::PlotVar::DY},
                          {"dz"         , CGnuPlotTypes::PlotVar::DZ},
                          {"endian"     , CGnuPlotTypes::PlotVar::ENDIAN},
                          {"every"      , CGnuPlotTypes::PlotVar::EVERY},
                          {"filetype"   , CGnuPlotTypes::PlotVar::FILETYPE},
                          {"fillstyle"  , CGnuPlotTypes::PlotVar::FILLSTYLE},
                          {"fs"         , CGnuPlotTypes::PlotVar::FILLSTYLE},
                          {"flip"       , CGnuPlotTypes::PlotVar::FLIP},
                          {"flipx"      , CGnuPlotTypes::PlotVar::FLIPX},
                          {"flipy"      , CGnuPlotTypes::PlotVar::FLIPY},
                          {"flipz"      , CGnuPlotTypes::PlotVar::FLIPZ},
                          {"format"     , CGnuPlotTypes::PlotVar::FORMAT},
                          {"heads"      , CGnuPlotTypes::PlotVar::HEADS},
                          {"index"      , CGnuPlotTypes::PlotVar::INDEX},
                          {"linetype"   , CGnuPlotTypes::PlotVar::LINETYPE},
                          {"lt"         , CGnuPlotTypes::PlotVar::LINETYPE},
                          {"linewidth"  , CGnuPlotTypes::PlotVar::LINEWIDTH},
                          {"lw"         , CGnuPlotTypes::PlotVar::LINEWIDTH},
                          {"matrix"     , CGnuPlotTypes::PlotVar::MATRIX},
                          {"notitle"    , CGnuPlotTypes::PlotVar::NOTITLE},
                          {"origin"     , CGnuPlotTypes::PlotVar::ORIGIN},
                          {"pointsize"  , CGnuPlotTypes::PlotVar::POINTSIZE},
                          {"ps"         , CGnuPlotTypes::PlotVar::POINTSIZE},
                          {"record"     , CGnuPlotTypes::PlotVar::RECORD},
                          {"rotate"     , CGnuPlotTypes::PlotVar::ROTATION},
                          {"rotation"   , CGnuPlotTypes::PlotVar::ROTATION},
                          {"scan"       , CGnuPlotTypes::PlotVar::SCAN},
                          {"skip"       , CGnuPlotTypes::PlotVar::SKIP},
                          {"smooth"     , CGnuPlotTypes::PlotVar::SMOOTH},
                          {"title"      , CGnuPlotTypes::PlotVar::TITLE},
                          {"t"          , CGnuPlotTypes::PlotVar::TITLE},
                          {"transpose"  , CGnuPlotTypes::PlotVar::TRANSPOSE},
                          {"using"      , CGnuPlotTypes::PlotVar::USING},
                          {"whiskerbars", CGnuPlotTypes::PlotVar::WHISKERBARS},
                          {"with"       , CGnuPlotTypes::PlotVar::WITH},
                          {"w"          , CGnuPlotTypes::PlotVar::WITH}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::PlotStyle) {
    nameValues.addValues({{"adjacency"     , CGnuPlotTypes::PlotStyle::ADJACENCY},
                          {"boxerrorbars"  , CGnuPlotTypes::PlotStyle::BOXERRORBARS},
                          {"boxes"         , CGnuPlotTypes::PlotStyle::BOXES},
                          {"boxplot"       , CGnuPlotTypes::PlotStyle::BOXPLOT},
                          {"boxxyerrorbars", CGnuPlotTypes::PlotStyle::BOXXYERRORBARS},
                          {"bubblechart"   , CGnuPlotTypes::PlotStyle::BUBBLEPLOT},
                          {"candlesticks"  , CGnuPlotTypes::PlotStyle::CANDLESTICKS},
                          {"chorddiagram"  , CGnuPlotTypes::PlotStyle::CHORDDIAGRAM},
                          {"circles"       , CGnuPlotTypes::PlotStyle::CIRCLES},
                          {"delaunay"      , CGnuPlotTypes::PlotStyle::DELAUNAY},
                          {"dots"          , CGnuPlotTypes::PlotStyle::DOTS},
                          {"ellipses"      , CGnuPlotTypes::PlotStyle::ELLIPSES},
                          {"errorbars"     , CGnuPlotTypes::PlotStyle::ERRORBARS},
                          {"errorlines"    , CGnuPlotTypes::PlotStyle::ERRORLINES},
                          {"filledcurves"  , CGnuPlotTypes::PlotStyle::FILLEDCURVES},
                          {"fillsteps"     , CGnuPlotTypes::PlotStyle::FILLSTEPS},
                          {"financebars"   , CGnuPlotTypes::PlotStyle::FINANCEBARS},
                          {"fsteps"        , CGnuPlotTypes::PlotStyle::FSTEPS},
                          {"histeps"       , CGnuPlotTypes::PlotStyle::HISTEPS},
                          {"histograms"    , CGnuPlotTypes::PlotStyle::HISTOGRAMS},
                          {"image"         , CGnuPlotTypes::PlotStyle::IMAGE},
                          {"impulses"      , CGnuPlotTypes::PlotStyle::IMPULSES},
                          {"labels"        , CGnuPlotTypes::PlotStyle::LABELS},
                          {"lines"         , CGnuPlotTypes::PlotStyle::LINES},
                          {"line"          , CGnuPlotTypes::PlotStyle::LINES},
                          {"l"             , CGnuPlotTypes::PlotStyle::LINES},
                          {"linespoints"   , CGnuPlotTypes::PlotStyle::LINES_POINTS},
                          {"parallelaxes"  , CGnuPlotTypes::PlotStyle::PARALLELAXES},
                          {"piechart"      , CGnuPlotTypes::PlotStyle::PIECHART},
                          {"pm3d"          , CGnuPlotTypes::PlotStyle::PM3D},
                          {"points"        , CGnuPlotTypes::PlotStyle::POINTS},
                          {"p"             , CGnuPlotTypes::PlotStyle::POINTS},
                          {"polygons"      , CGnuPlotTypes::PlotStyle::POLYGONS},
                          {"radar"         , CGnuPlotTypes::PlotStyle::RADAR},
                          {"rgbalpha"      , CGnuPlotTypes::PlotStyle::RGBALPHA},
                          {"rgbimage"      , CGnuPlotTypes::PlotStyle::RGBIMAGE},
                          {"steps"         , CGnuPlotTypes::PlotStyle::STEPS},
                          {"sunburst"      , CGnuPlotTypes::PlotStyle::SUNBURST},
                          {"surface"       , CGnuPlotTypes::PlotStyle::SURFACE},
                          {"treemap"       , CGnuPlotTypes::PlotStyle::TREEMAP},
                          {"vectors"       , CGnuPlotTypes::PlotStyle::VECTORS},
                          {"xerrorbars"    , CGnuPlotTypes::PlotStyle::XERRORBARS},
                          {"xerrorlines"   , CGnuPlotTypes::PlotStyle::XERRORLINES},
                          {"xyerrorbars"   , CGnuPlotTypes::PlotStyle::XYERRORBARS},
                          {"xyerrorlines"  , CGnuPlotTypes::PlotStyle::XYERRORLINES},
                          {"yerrorbars"    , CGnuPlotTypes::PlotStyle::YERRORBARS},
                          {"yerrorlines"   , CGnuPlotTypes::PlotStyle::YERRORLINES}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::Smooth) {
    nameValues.addValues({{"unique"   , CGnuPlotTypes::Smooth::UNIQUE},
                          {"frequency", CGnuPlotTypes::Smooth::FREQUENCY},
                          {"bezier"   , CGnuPlotTypes::Smooth::BEZIER},
                          {"sbezier"  , CGnuPlotTypes::Smooth::SBEZIER},
                          {"csplines" , CGnuPlotTypes::Smooth::CSPLINES},
                          {"acsplines", CGnuPlotTypes::Smooth::ACSPLINES}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::FillType) {
    nameValues.addValues({{"empty"  , CGnuPlotTypes::FillType::EMPTY},
                          {"solid"  , CGnuPlotTypes::FillType::SOLID},
                          {"pattern", CGnuPlotTypes::FillType::PATTERN}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::FillPattern) {
    nameValues.addValues({{"none"  , CGnuPlotTypes::FillPattern::NONE},
                          {"hatch" , CGnuPlotTypes::FillPattern::HATCH},
                          {"dense" , CGnuPlotTypes::FillPattern::DENSE},
                          {"fg"    , CGnuPlotTypes::FillPattern::FG},
                          {"fdiag" , CGnuPlotTypes::FillPattern::FDIAG},
                          {"bdiag" , CGnuPlotTypes::FillPattern::BDIAG},
                          {"fdiag1", CGnuPlotTypes::FillPattern::FDIAG1},
                          {"bdiag1", CGnuPlotTypes::FillPattern::BDIAG1},
                          {"bg"    , CGnuPlotTypes::FillPattern::BG}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::HistogramStyle) {
    nameValues.addValues({{"clustered"    , CGnuPlotTypes::HistogramStyle::CLUSTERED},
                          {"errorbars"    , CGnuPlotTypes::HistogramStyle::ERRORBARS},
                          {"rowstacked"   , CGnuPlotTypes::HistogramStyle::ROWSTACKED},
                          {"columnstacked", CGnuPlotTypes::HistogramStyle::COLUMNSTACKED}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::AngleType) {
    nameValues.addValues({{"radians", CGnuPlotTypes::AngleType::RADIANS},
                          {"degrees", CGnuPlotTypes::AngleType::DEGREES}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::CoordSys) {
    nameValues.addValues({{"first"    , CGnuPlotTypes::CoordSys::FIRST    },
                          {"second"   , CGnuPlotTypes::CoordSys::SECOND   },
                          {"graph"    , CGnuPlotTypes::CoordSys::GRAPH    },
                          {"screen"   , CGnuPlotTypes::CoordSys::SCREEN   },
                          {"character", CGnuPlotTypes::CoordSys::CHARACTER}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::ObjectType) {
    nameValues.addValues({{"circle"   , CGnuPlotTypes::ObjectType::CIRCLE   },
                          {"ellipse"  , CGnuPlotTypes::ObjectType::ELLIPSE  },
                          {"polygon"  , CGnuPlotTypes::ObjectType::POLYGON  },
                          {"rectangle", CGnuPlotTypes::ObjectType::RECTANGLE}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::ColorModel) {
    nameValues.addValues({{"RGB", CGnuPlotTypes::ColorModel::RGB},
                          {"HSV", CGnuPlotTypes::ColorModel::HSV},
                          {"CMY", CGnuPlotTypes::ColorModel::CMY},
                          {"YIQ", CGnuPlotTypes::ColorModel::YIQ},
                          {"XYZ", CGnuPlotTypes::ColorModel::XYZ}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::DrawLayer) {
    nameValues.addValues({{"front"  , CGnuPlotTypes::DrawLayer::FRONT  },
                          {"back"   , CGnuPlotTypes::DrawLayer::BACK   },
                          {"behind" , CGnuPlotTypes::DrawLayer::BEHIND },
                          {"default", CGnuPlotTypes::DrawLayer::DEFAULT}});
  }

  CStrUniqueMatchInitT(CGnuPlotTypes::ImageType) {
    nameValues.addValues({{"jpeg", CGnuPlotTypes::ImageType::JPEG},
                          {"png" , CGnuPlotTypes::ImageType::PNG },
                          {"gif" , CGnuPlotTypes::ImageType::GIF }});
  }
}

#endif
