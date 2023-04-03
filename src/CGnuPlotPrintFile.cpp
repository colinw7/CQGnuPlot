#include <CGnuPlotPrintFile.h>
#include <CGnuPlot.h>

CGnuPlotPrintFile::
CGnuPlotPrintFile(CGnuPlot *plot, bool enabled) :
 plot_(plot), enabled_(enabled)
{
}

CGnuPlotPrintFile::
~CGnuPlotPrintFile()
{
  close();
}

void
CGnuPlotPrintFile::
unset()
{
  setStdErr();

  append_ = false;
}

void
CGnuPlotPrintFile::
setStdOut()
{
  close();

  filename_ = "";
  isError_  = false;
  isBlock_  = false;
}

void
CGnuPlotPrintFile::
setStdErr()
{
  close();

  filename_ = "";
  isError_  = true;
  isBlock_  = false;
}

void
CGnuPlotPrintFile::
setFile(const std::string &file)
{
  close();

  filename_ = file;
  isError_  = false;
  isBlock_  = false;
}

void
CGnuPlotPrintFile::
setDataBlock(const std::string &name)
{
  close();

  filename_ = name;
  isError_  = false;
  isBlock_  = true;
}

void
CGnuPlotPrintFile::
print(const std::string &str) const
{
  if      (! isBlock_ && filename_ != "") {
    if (! fp_) {
      CGnuPlotPrintFile *th = const_cast<CGnuPlotPrintFile *>(this);

      th->valid_ = th->open();
    }

    if (fp_)
      fprintf(fp_, "%s", str.c_str());
  }
  else if (isBlock_ && filename_ != "") {
    plot_->getBlock(filename_)->addString(str);
  }
  else {
    if (isError_)
      std::cerr << str;
    else
      std::cout << str;
  }
}

bool
CGnuPlotPrintFile::
open()
{
  if (! fp_ && ! isBlock_ && filename_ != "") {
    if (valid_) {
      if (append_)
        fp_ = fopen(filename_.c_str(), "a");
      else
        fp_ = fopen(filename_.c_str(), "w");
    }

    return (fp_ != nullptr);
  }
  else if (isBlock_ && filename_ != "") {
    plot_->getBlock(filename_)->clear();
  }

  return true;
}

void
CGnuPlotPrintFile::
close()
{
  if (fp_) {
    fclose(fp_);

    fp_ = nullptr;
  }

  valid_ = true;
}

void
CGnuPlotPrintFile::
show(std::ostream &os) const
{
  if (isBlock_) {
    os << "print output is saved to datablock $" << filename_ << std::endl;
  }
  else {
    os << "print output is sent to ";

    if (filename_ != "")
      os << "'" << filename_ << "'";
    else if (isError_)
      os << "<stderr>";
    else
      os << "<stdout>";

    os << std::endl;
  }
}
