#ifndef CGnuPlotUnixFile_H
#define CGnuPlotUnixFile_H

#include <CUnixFile.h>

class CGnuPlotUnixFile {
 public:
  CGnuPlotUnixFile(const std::string &filename) {
    init(filename);
  }

  CGnuPlotUnixFile(CUnixFile *file) {
    init(file);
  }

  CGnuPlotUnixFile(FILE *fp) {
    init(fp);
  }

  CGnuPlotUnixFile() { }

 ~CGnuPlotUnixFile() {
    if (owner_)
      delete file_;
  }

  void init(const std::string &filename) {
    if (owner_)
      delete file_;

    file_  = new CUnixFile(filename);
    valid_ = file_->open();
    owner_ = true;
  }

  void init(CUnixFile *file) {
    if (owner_)
      delete file_;

    file_  = file;
    valid_ = true;
    owner_ = false;
  }

  void init(FILE *fp) {
    if (owner_)
      delete file_;

    file_  = new CUnixFile(fp);
    valid_ = true;
    owner_ = true;
  }

  CUnixFile *file() const { return file_; }

  bool isOwner() const { return owner_; }

  bool isValid() const { return valid_; }

 private:
  CUnixFile *file_  { 0 };
  bool       owner_ { false };
  bool       valid_ { false };
};

#endif
