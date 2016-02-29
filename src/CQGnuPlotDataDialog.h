#ifndef CQGnuPlotDataDialog_H
#define CQGnuPlotDataDialog_H

#include <CGnuPlotFile.h>
#include <CGnuPlot.h>
#include <QFrame>
#include <CExpr.h>

class CQGnuPlotMainWindow;
class CGnuPlotFile;
class CQGnuPlotDataTree;
class CQGnuPlotFilename;
class CQIntegerSpin;
class CQGnuPlotEnum;
class QLineEdit;
class QCheckBox;
class QRadioButton;

class CQGnuPlotDataDialog : public QFrame {
  Q_OBJECT

 public:
  enum class Smooth {
    None,
    Average,
    Summed
  };

  struct ValuesData {
    ValuesData(const CGnuPlot::Values &values1=CGnuPlot::Values(), bool discontinuity1=false,
               bool bad1=false, const CGnuPlot::Params &params1=CGnuPlot::Params()) :
     values(values1), discontinuity(discontinuity1), bad(bad1), params(params1) {
    }

    CGnuPlot::Values values;
    bool             discontinuity { false };
    bool             bad { false };
    CGnuPlot::Params params;
  };

  typedef std::vector<ValuesData> ValuesDataList;

 public:
  CQGnuPlotDataDialog(CQGnuPlotMainWindow *window, const CGnuPlotFile &file);

  void doPlot(bool add);

  QSize sizeHint() const;

 private:
  void initPlotStyle(CGnuPlotPlotP plot);

  CExprValuePtr fieldToValue(const std::string &field, bool &missing) const;

  CGnuPlotFile::SubSetLines smoothLines(const CGnuPlotFile::SubSetLines &lines,
                                        Smooth smooth) const;

  ValuesDataList smoothValuesList(const ValuesDataList &valuesDataList, Smooth smooth) const;

  CGnuPlot::Matrix smoothValuesList(const CGnuPlot::Matrix &valuesList, Smooth smooth) const;

 private slots:
  void loadSlot();
  void generateSlot();
  void filterSlot();
  void overlayPlotSlot();
  void addPlotSlot();
  void closeSlot();

 private:
  CQGnuPlotMainWindow* window_           { 0 };
  CQGnuPlotDataTree*   tree_             { 0 };
  CQGnuPlotFilename*   fileEdit_         { 0 };
  CQIntegerSpin*       numberEdit_       { 0 };
  QCheckBox*           csvCheck_         { 0 };
  QCheckBox*           headerCheck_      { 0 };
  QLineEdit*           separatorEdit_    { 0 };
  QLineEdit*           missingEdit_      { 0 };
  QLineEdit*           usingEdit_        { 0 };
  QLineEdit*           indexEdit_        { 0 };
  QLineEdit*           everyEdit_        { 0 };
  QLineEdit*           whereEdit_        { 0 };
  QCheckBox*           averageCheck_     { 0 };
  QCheckBox*           summedCheck_      { 0 };
  QCheckBox*           uniqueCheck_      { 0 };
  CQGnuPlotEnum*       enum_             { 0 };
  QRadioButton*        dimension2DRadio_ { 0 };
  QRadioButton*        dimension3DRadio_ { 0 };
  CQIntegerSpin*       lineTypeEdit_    { 0 };
  std::string          missingStr_;
};

#endif
