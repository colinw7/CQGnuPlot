#ifndef CGnuPlotBinaryFormat_H
#define CGnuPlotBinaryFormat_H

#include <string>
#include <vector>

class CUnixFile;

class CGnuPlotBinaryFormat {
 public:
  enum class Format {
    INVALID,
    CHAR,
    UCHAR,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
    FLOAT,
    DOUBLE
  };

 public:
  struct FmtData {
    bool   skip { false };
    int    n    { 1 };
    Format fmt  { Format::INVALID };

    FmtData(bool skip1, int n1, Format fmt1) {
      skip = skip1; n = n1; fmt = fmt1;
    }
  };

 public:
  CGnuPlotBinaryFormat(const std::string &str="");

  const std::string &fmt() const { return fmt_; }

  bool init(const std::string &str);

  void clear();

  void addFormat(bool skip, int n, Format fmt);

  bool readValues(CUnixFile &file, std::vector<double> &values);

  bool readValue(CUnixFile &file, Format fmt, double &r);

  static Format stringToFormat(const std::string &str);

 private:
  typedef std::vector<FmtData> Formats;

  std::string fmt_;
  Formats     formats_;
};

#endif
