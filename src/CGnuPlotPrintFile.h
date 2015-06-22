#ifndef CGnuPlotPrintFile_H
#define CGnuPlotPrintFile_H

#include <string>

class CGnuPlot;

class CGnuPlotPrintFile {
 public:
  CGnuPlotPrintFile(CGnuPlot *plot);
 ~CGnuPlotPrintFile();

  void setAppend(bool b) { append_ = b; }
  bool getAppend() { return append_; }

  void unset();

  void setStdOut();

  void setFile(const std::string &file);

  void setDataBlock(const std::string &name);

  void print(const std::string &str) const;

  void show(std::ostream &os) const;

 private:
  void open();

  void close();

 private:
  CGnuPlot*   plot_    { 0 };
  std::string filename_;
  bool        isError_ { true };
  bool        isBlock_ { false };
  bool        append_  { false };
  FILE*       fp_      { 0 };
};

#endif
