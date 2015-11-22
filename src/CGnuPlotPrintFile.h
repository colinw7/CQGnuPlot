#ifndef CGnuPlotPrintFile_H
#define CGnuPlotPrintFile_H

#include <string>
#include <cassert>

class CGnuPlot;

class CGnuPlotPrintFile {
 public:
  CGnuPlotPrintFile(CGnuPlot *plot, bool enabled=true);
 ~CGnuPlotPrintFile();

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  bool isFile() const { return ! isBlock_ && filename_ != ""; }
  const std::string &getFile() const { assert(isFile()); return filename_; }
  void setFile(const std::string &file);

  bool isAppend() const { return append_; }
  void setAppend(bool b) { append_ = b; }

  bool isStdOut() const { return (filename_ == ""  && ! isError_ && ! isBlock_); }
  void setStdOut();

  bool isStdErr() const { return (filename_ == ""  && isError_ && ! isBlock_); }
  void setStdErr();

  bool isDataBlock() const { return isBlock_; }
  const std::string &getDataBlock() const { assert(isDataBlock()); return filename_; }
  void setDataBlock(const std::string &name);

  void unset();

  void print(const std::string &str) const;

  void show(std::ostream &os) const;

 private:
  bool open();

  void close();

 private:
  CGnuPlot*   plot_    { 0 };
  bool        enabled_ { true };
  std::string filename_;
  bool        isError_ { true };
  bool        isBlock_ { false };
  bool        append_  { false };
  FILE*       fp_      { 0 };
  bool        valid_   { true };
};

#endif
