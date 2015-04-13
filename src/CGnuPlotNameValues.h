#ifndef CGnuPlotNameValues_H
#define CGnuPlotNameValues_H

#include <CStrUniqueMatch.h>
#include <CGnuPlotTypes.h>
#include <CGnuPlot.h>

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
                          {"get"     , CGnuPlot::CommandName::GET},
                          {"help"    , CGnuPlot::CommandName::HELP},
                          {"history" , CGnuPlot::CommandName::HISTORY},
                          {"if"      , CGnuPlot::CommandName::IF},
                          {"load"    , CGnuPlot::CommandName::LOAD},
                          {"lower"   , CGnuPlot::CommandName::LOWER},
                          {"pause"   , CGnuPlot::CommandName::PAUSE},
                          {"plot"    , CGnuPlot::CommandName::PLOT},
                          {"print"   , CGnuPlot::CommandName::PRINT},
                          {"printf"  , CGnuPlot::CommandName::PRINTF},
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
                          {"undefine", CGnuPlot::CommandName::UNDEFINE},
                          {"unset"   , CGnuPlot::CommandName::UNSET},
                          {"update"  , CGnuPlot::CommandName::UPDATE},
                          {"while"   , CGnuPlot::CommandName::WHILE}});
  }

  CStrUniqueMatchInitT(CGnuPlot::VariableName) {
    nameValues.addValues({{"angles"          , CGnuPlot::VariableName::ANGLES},
                          {"arrow"           , CGnuPlot::VariableName::ARROW},
                          {"autoscale"       , CGnuPlot::VariableName::AUTOSCALE},
                          {"bars"            , CGnuPlot::VariableName::BARS},
                          {"bmargin"         , CGnuPlot::VariableName::BMARGIN},
                          {"border"          , CGnuPlot::VariableName::BORDER},
                          {"boxwidth"        , CGnuPlot::VariableName::BOXWIDTH},
                          {"cbdata"          , CGnuPlot::VariableName::CBDATA},
                          {"cbdtics"         , CGnuPlot::VariableName::CBDTICS},
                          {"cblabel"         , CGnuPlot::VariableName::CBLABEL},
                          {"cbmtics"         , CGnuPlot::VariableName::CBMTICS},
                          {"cbrange"         , CGnuPlot::VariableName::CBRANGE},
                          {"cbtics"          , CGnuPlot::VariableName::CBTICS},
                          {"clip"            , CGnuPlot::VariableName::CLIP},
                          {"cntrlabel"       , CGnuPlot::VariableName::CNTRLABEL},
                          {"cntrparam"       , CGnuPlot::VariableName::CNTRPARAM},
                          {"colorbox"        , CGnuPlot::VariableName::COLORBOX},
                          {"colornames"      , CGnuPlot::VariableName::COLORNAMES},
                          {"colorsequence"   , CGnuPlot::VariableName::COLORSEQUENCE},
                          {"contour"         , CGnuPlot::VariableName::CONTOUR},
                          {"cplane"          , CGnuPlot::VariableName::CPLANE},
                          {"dashtype"        , CGnuPlot::VariableName::DASHTYPE},
                          {"datafile"        , CGnuPlot::VariableName::DATAFILE},
                          {"debug"           , CGnuPlot::VariableName::DEBUG},
                          {"decimalsign"     , CGnuPlot::VariableName::DECIMALSIGN},
                          {"dgrid3d"         , CGnuPlot::VariableName::DGRID3D},
                          {"dt"              , CGnuPlot::VariableName::DASHTYPE},
                          {"dummy"           , CGnuPlot::VariableName::DUMMY},
                          {"edebug"          , CGnuPlot::VariableName::EDEBUG},
                          {"encoding"        , CGnuPlot::VariableName::ENCODING},
                          {"fit"             , CGnuPlot::VariableName::FIT},
                          {"fontpath"        , CGnuPlot::VariableName::FONTPATH},
                          {"format"          , CGnuPlot::VariableName::FORMAT},
                          {"functions"       , CGnuPlot::VariableName::FUNCTIONS},
                          {"grid"            , CGnuPlot::VariableName::GRID},
                          {"hidden3d"        , CGnuPlot::VariableName::HIDDEN3D},
                          {"history"         , CGnuPlot::VariableName::HISTORY},
                          {"historysize"     , CGnuPlot::VariableName::HISTORYSIZE},
                          {"isosamples"      , CGnuPlot::VariableName::ISOSAMPLES},
                          {"key"             , CGnuPlot::VariableName::KEY},
                          {"label"           , CGnuPlot::VariableName::LABEL},
                          {"linetype"        , CGnuPlot::VariableName::LINETYPE},
                          {"link"            , CGnuPlot::VariableName::LINK},
                          {"lmargin"         , CGnuPlot::VariableName::LMARGIN},
                          {"loadpath"        , CGnuPlot::VariableName::LOADPATH},
                          {"locale"          , CGnuPlot::VariableName::LOCALE},
                          {"logscale"        , CGnuPlot::VariableName::LOGSCALE},
                          {"macros"          , CGnuPlot::VariableName::MACROS},
                          {"mapping"         , CGnuPlot::VariableName::MAPPING},
                          {"margins"         , CGnuPlot::VariableName::MARGINS},
                          {"mouse"           , CGnuPlot::VariableName::MOUSE},
                          {"multiplot"       , CGnuPlot::VariableName::MULTIPLOT},
                          {"mx2tics"         , CGnuPlot::VariableName::MX2TICS},
                          {"mxtics"          , CGnuPlot::VariableName::MXTICS},
                          {"my2tics"         , CGnuPlot::VariableName::MY2TICS},
                          {"mytics"          , CGnuPlot::VariableName::MYTICS},
                          {"mztics"          , CGnuPlot::VariableName::MZTICS},
                          {"object"          , CGnuPlot::VariableName::OBJECT},
                          {"offsets"         , CGnuPlot::VariableName::OFFSETS},
                          {"origin"          , CGnuPlot::VariableName::ORIGIN},
                          {"output"          , CGnuPlot::VariableName::OUTPUT},
                          {"palette"         , CGnuPlot::VariableName::PALETTE},
                          {"parametric"      , CGnuPlot::VariableName::PARAMETRIC},
                          {"paxis"           , CGnuPlot::VariableName::PAXIS},
                          {"plot"            , CGnuPlot::VariableName::PLOT},
                          {"pm3d"            , CGnuPlot::VariableName::PM3D},
                          {"pointintervalbox", CGnuPlot::VariableName::POINTINTERVALBOX},
                          {"pointsize"       , CGnuPlot::VariableName::POINTSIZE},
                          {"polar"           , CGnuPlot::VariableName::POLAR},
                          {"print"           , CGnuPlot::VariableName::PRINT},
                          {"psdir"           , CGnuPlot::VariableName::PSDIR},
                          {"raxis"           , CGnuPlot::VariableName::RAXIS},
                          {"rmargin"         , CGnuPlot::VariableName::RMARGIN},
                          {"rrange"          , CGnuPlot::VariableName::RRANGE},
                          {"rtics"           , CGnuPlot::VariableName::RTICS},
                          {"samples"         , CGnuPlot::VariableName::SAMPLES},
                          {"size"            , CGnuPlot::VariableName::SIZE},
                          {"style"           , CGnuPlot::VariableName::STYLE},
                          {"surface"         , CGnuPlot::VariableName::SURFACE},
                          {"table"           , CGnuPlot::VariableName::TABLE},
                          {"terminal"        , CGnuPlot::VariableName::TERMINAL},
                          {"termoption"      , CGnuPlot::VariableName::TERMOPTION},
                          {"ticscale"        , CGnuPlot::VariableName::TICSCALE},
                          {"tics"            , CGnuPlot::VariableName::TICS},
                          {"time"            , CGnuPlot::VariableName::TIMESTAMP},
                          {"timefmt"         , CGnuPlot::VariableName::TIMEFMT},
                          {"timestamp"       , CGnuPlot::VariableName::TIMESTAMP},
                          {"title"           , CGnuPlot::VariableName::TITLE},
                          {"tmargin"         , CGnuPlot::VariableName::TMARGIN},
                          {"trange"          , CGnuPlot::VariableName::TRANGE},
                          {"urange"          , CGnuPlot::VariableName::URANGE},
                          {"variables"       , CGnuPlot::VariableName::VARIABLES},
                          {"version"         , CGnuPlot::VariableName::VERSION},
                          {"view"            , CGnuPlot::VariableName::VIEW},
                          {"vrange"          , CGnuPlot::VariableName::VRANGE},
                          {"x2data"          , CGnuPlot::VariableName::X2DATA},
                          {"x2dtics"         , CGnuPlot::VariableName::X2DTICS},
                          {"x2label"         , CGnuPlot::VariableName::X2LABEL},
                          {"x2mtics"         , CGnuPlot::VariableName::X2MTICS},
                          {"x2range"         , CGnuPlot::VariableName::X2RANGE},
                          {"x2tics"          , CGnuPlot::VariableName::X2TICS},
                          {"x2zeroaxis"      , CGnuPlot::VariableName::X2ZEROAXIS},
                          {"xdata"           , CGnuPlot::VariableName::XDATA},
                          {"xdtics"          , CGnuPlot::VariableName::XDTICS},
                          {"xlabel"          , CGnuPlot::VariableName::XLABEL},
                          {"xmtics"          , CGnuPlot::VariableName::XMTICS},
                          {"xrange"          , CGnuPlot::VariableName::XRANGE},
                          {"xtics"           , CGnuPlot::VariableName::XTICS},
                          {"xyplane"         , CGnuPlot::VariableName::XYPLANE},
                          {"xzeroaxis"       , CGnuPlot::VariableName::XZEROAXIS},
                          {"y2data"          , CGnuPlot::VariableName::Y2DATA},
                          {"y2dtics"         , CGnuPlot::VariableName::Y2DTICS},
                          {"y2label"         , CGnuPlot::VariableName::Y2LABEL},
                          {"y2mtics"         , CGnuPlot::VariableName::Y2MTICS},
                          {"y2range"         , CGnuPlot::VariableName::Y2RANGE},
                          {"y2tics"          , CGnuPlot::VariableName::Y2TICS},
                          {"y2zeroaxis"      , CGnuPlot::VariableName::Y2ZEROAXIS},
                          {"ydata"           , CGnuPlot::VariableName::YDATA},
                          {"ydtics"          , CGnuPlot::VariableName::YDTICS},
                          {"ylabel"          , CGnuPlot::VariableName::YLABEL},
                          {"ymtics"          , CGnuPlot::VariableName::YMTICS},
                          {"yrange"          , CGnuPlot::VariableName::YRANGE},
                          {"ytics"           , CGnuPlot::VariableName::YTICS},
                          {"yzeroaxis"       , CGnuPlot::VariableName::YZEROAXIS},
                          {"zdata"           , CGnuPlot::VariableName::ZDATA},
                          {"zdtics"          , CGnuPlot::VariableName::ZDTICS},
                          {"zeroaxis"        , CGnuPlot::VariableName::ZEROAXIS},
                          {"zero"            , CGnuPlot::VariableName::ZERO},
                          {"zlabel"          , CGnuPlot::VariableName::ZLABEL},
                          {"zmtics"          , CGnuPlot::VariableName::ZMTICS},
                          {"zrange"          , CGnuPlot::VariableName::ZRANGE},
                          {"ztics"           , CGnuPlot::VariableName::ZTICS},
                          {"zzeroaxis"       , CGnuPlot::VariableName::ZZEROAXIS}});
  }

  CStrUniqueMatchInitT(CGnuPlot::DataFileVar) {
    nameValues.addValues({{"fortran"      , CGnuPlot::DataFileVar::FORTRAN},
                          {"nofpe_trap"   , CGnuPlot::DataFileVar::NOFPE_TRAP},
                          {"missing"      , CGnuPlot::DataFileVar::MISSING},
                          {"separator"    , CGnuPlot::DataFileVar::SEPARATOR},
                          {"commentschars", CGnuPlot::DataFileVar::COMMENTS_CHARS},
                          {"binary"       , CGnuPlot::DataFileVar::BINARY}});
  }

  CStrUniqueMatchInitT(CGnuPlot::StyleVar) {
    nameValues.addValues({{"data"     , CGnuPlot::StyleVar::DATA},
                          {"function" , CGnuPlot::StyleVar::FUNCTION},
                          {"increment", CGnuPlot::StyleVar::INCREMENT},
                          {"line"     , CGnuPlot::StyleVar::LINE},
                          {"fill"     , CGnuPlot::StyleVar::FILL},
                          {"boxplot"  , CGnuPlot::StyleVar::BOXPLOT},
                          {"textbox"  , CGnuPlot::StyleVar::TEXTBOX},
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
}

#endif
