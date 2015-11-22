#ifndef CGnuPlotBinaryFormat_H
#define CGnuPlotBinaryFormat_H

#include <string>
#include <vector>
#include <limits>

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

    FmtData() { }

    FmtData(bool skip1, int n1, Format fmt1) {
      skip = skip1; n = n1; fmt = fmt1;
    }

    bool isInteger() const {
      return (fmt == Format::CHAR  || fmt == Format::UCHAR  ||
              fmt == Format::SHORT || fmt == Format::USHORT ||
              fmt == Format::INT   || fmt == Format::UINT   ||
              fmt == Format::LONG  || fmt == Format::ULONG);
    }

    bool isReal() const {
      return (fmt == Format::FLOAT || fmt == Format::DOUBLE);
    }
  };

 public:
  CGnuPlotBinaryFormat(const std::string &str="");

  const std::string &fmt() const { return fmt_; }

  bool init(const std::string &str);

  void clear();

  int numFormats() const { return formats_.size(); }

  const FmtData &format(int i) const { return formats_[i]; }

  int numFormatValues() const;

  bool readValues(CUnixFile &file, std::vector<double> &values,
                  int n=std::numeric_limits<int>::max());

  FmtData indexFormat(int i) const;

 private:
  void addFormat(bool skip, int n, Format fmt);

  bool readValue(CUnixFile &file, Format fmt, double &r);

  static Format stringToFormat(const std::string &str);

 private:
  typedef std::vector<FmtData> Formats;

  std::string fmt_;
  Formats     formats_;
};

#endif
