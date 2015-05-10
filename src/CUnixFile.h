#ifndef CUNIX_FILE_H
#define CUNIX_FILE_H

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>

typedef unsigned char uchar;

class CUnixFile {
 public:
  explicit CUnixFile(const char *filename="") :
   filename_(filename), fp_(0) {
  }

  explicit CUnixFile(const std::string &filename) :
   filename_(filename), fp_(0) {
  }

 ~CUnixFile() {
    close();
  }

  bool isValid() const { return (fp_ != 0); }

  bool open() {
    close();

    fp_ = fopen(filename_.c_str(), "r");

    if (! fp_) {
      std::cerr << "Failed to open '" << filename_ << "'" << std::endl;
      return false;
    }

    return true;
  }

  void close() {
    if (! fp_) return;

    fclose(fp_);

    fp_ = 0;
  }

  bool readLine(std::string &line) {
    if (! fp_ || feof(fp_))
      return false;

    line = "";

    int c;

    while ((c = fgetc(fp_)) != EOF) {
      if (c == '\n') break;

      line += uchar(c);
    }

    return true;
  }

  bool readChar(uchar &c) {
    if (! fp_ || feof(fp_))
      return false;

    int c1 = fgetc(fp_);

    if (c1 == EOF)
      return false;

    c = uchar(c1);

    return true;
  }

  template<typename T>
  bool readData(T &data) {
    std::vector<uchar> buffer;

    size_t n = sizeof(T);

    buffer.resize(n);

    for (size_t i = 0; i < n; ++i) {
      if (! readChar(buffer[i]))
        return false;
    }

    memcpy((void *) &data, (void *) &buffer[0], n);

    return true;
  }

  void rewind() {
    if (! fp_) return;

    (void) fseek(fp_, 0L, SEEK_SET);
  }

 private:
  std::string  filename_;
  FILE        *fp_;
};

#endif
