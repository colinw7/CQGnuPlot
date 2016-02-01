#include <CGnuPlotBinaryFormat.h>
#include <CParseLine.h>
#include <CUnixFile.h>
#include <iostream>

CGnuPlotBinaryFormat::
CGnuPlotBinaryFormat(const std::string &fmt) :
 fmt_(fmt)
{
  if (fmt != "")
    init(fmt);
}

void
CGnuPlotBinaryFormat::
setFormat(const std::string &fmt)
{
  if (fmt != "")
    init(fmt);
}

bool
CGnuPlotBinaryFormat::
init(const std::string &fmt)
{
  fmt_ = fmt;

  clear();

  CParseLine line(fmt);

  while (line.isValid()) {
    line.skipSpace();

    if (! line.isChar('%'))
      break;

    line.skipChar();

    bool skip = false;

    if (line.isChar('*')) {
      skip = true;
      line.skipChar();
    }

    std::string is;

    while (line.isValid() && line.isDigit())
      is += line.getChar();

    int n = (is != "" ? stoi(is) : 1);

    std::string s;

    while (line.isValid() && ! line.isChar('%'))
      s += line.getChar();

    //std::cerr << skip << ":" << n << ":" << s << std::endl;

    CGnuPlotBinaryFormat::Format fmt = CGnuPlotBinaryFormat::stringToFormat(s);

    if (fmt != Format::INVALID)
      addFormat(skip, n, fmt);
    else
      std::cerr << "Unsupported format '" << s << "'" << std::endl;
  }

  return true;
}

void
CGnuPlotBinaryFormat::
clear()
{
  formats_.clear();
}

void
CGnuPlotBinaryFormat::
addFormat(bool skip, int n, Format fmt)
{
  formats_.push_back(FmtData(skip, n, fmt));
}

int
CGnuPlotBinaryFormat::
numFormatValues() const
{
  int n = 0;

  for (const auto &fmt : formats_) {
    if (fmt.skip)
      continue;

    n += fmt.n;
  }

  return n;
}

bool
CGnuPlotBinaryFormat::
readValues(CUnixFile &file, std::vector<double> &values, int n)
{
  if (formats_.empty()) {
    double r;

    while (int(values.size()) < n && readValue(file, Format::FLOAT, r))
      values.push_back(r);
  }
  else {
    int  j     = 0;
    bool valid = true;

    while (valid) {
      if (j >= n)
        break;

      for (const auto &fmt : formats_) {
        if (j >= n)
          break;

        double r;

        for (auto i = 0; i < fmt.n; ++i) {
          if (j >= n)
            break;

          if (! readValue(file, fmt.fmt, r)) {
            valid = false;
            break;
          }

          if (! fmt.skip)
            values.push_back(r);

          ++j;
        }

        if (! valid)
          break;
      }
    }
  }

  return true;
}

bool
CGnuPlotBinaryFormat::
readValue(CUnixFile &file, Format fmt, double &r)
{
  if      (fmt == Format::CHAR) {
    char v;

    if (! file.readData<char>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::UCHAR) {
    uchar v;

    if (! file.readData<uchar>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::SHORT) {
    short v;

    if (! file.readData<short>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::USHORT) {
    ushort v;

    if (! file.readData<ushort>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::INT) {
    int v;

    if (! file.readData<int>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::UINT) {
    uint v;

    if (! file.readData<uint>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::LONG) {
    long v;

    if (! file.readData<long>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::ULONG) {
    ulong v;

    if (! file.readData<ulong>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::FLOAT) {
    float v;

    if (! file.readData<float>(v))
      return false;

    r = v;
  }
  else if (fmt == Format::DOUBLE) {
    double v;

    if (! file.readData<double>(v))
      return false;

    r = v;
  }
  else {
    return false;
  }

  return true;
}

CGnuPlotBinaryFormat::FmtData
CGnuPlotBinaryFormat::
indexFormat(int i) const
{
  if (i < 0 || formats_.empty())
    return FmtData();

  int i1 = (i % formats_.size());

  return formats_[i1];
}

CGnuPlotBinaryFormat::Format
CGnuPlotBinaryFormat::
stringToFormat(const std::string &str)
{
  if (str == "char"   || str == "schar" || str == "c"              ) return Format::CHAR;
  if (str == "uchar"                                               ) return Format::UCHAR;
  if (str == "short"                                               ) return Format::SHORT;
  if (str == "ushort"                                              ) return Format::USHORT;
  if (str == "int"    || str == "sint"  || str == "i" || str == "d") return Format::INT;
  if (str == "uint"   || str == "u"                                ) return Format::UINT;
  if (str == "long"   || str == "ld"                               ) return Format::LONG;
  if (str == "ulong"  || str == "lu"                               ) return Format::ULONG;
  if (str == "float"  || str == "f"                                ) return Format::FLOAT;
  if (str == "double" || str == "lf"                               ) return Format::DOUBLE;

  if (str == "int8"   || str == "byte" ) return Format::CHAR;
  if (str == "uint8"  || str == "ubyte") return Format::UCHAR;
  if (str == "int16"  || str == "word" ) return Format::SHORT;
  if (str == "uint16" || str == "uword") return Format::USHORT;
  if (str == "int32"                   ) return Format::INT;
  if (str == "uint32"                  ) return Format::UINT;
  if (str == "int64"                   ) return Format::LONG;
  if (str == "uint64"                  ) return Format::ULONG;
  if (str == "float32"                 ) return Format::FLOAT;
  if (str == "float64"                 ) return Format::DOUBLE;

  return Format::INVALID;
}
