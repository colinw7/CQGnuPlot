#include <CGnuPlotFile.h>
#include <CUnixFile.h>
#include <CCsv.h>
#include <CParseLine.h>
#include <CStrUtil.h>

namespace {

struct Words {
  Words(std::vector<std::string> &fields) :
   fields_(fields) {
    fields_.clear();
  }

 ~Words() {
    flush();
  }

  void addChar(char c) {
    word_ += c;
  }

  void addWord(const std::string &word) {
    force_ = true;
    word_  = word;

    flush();
  }

  void flush(bool force=false) {
    if (force || force_ || word_ != "") {
      fields_.push_back(word_);

      force_ = false;
      word_  = "";
    }
  }

  std::vector<std::string> &fields_;
  std::string               word_;
  bool                      force_ { false };
};

}

//------

CGnuPlotFile::
CGnuPlotFile()
{
}

void
CGnuPlotFile::
indices(CGnuPlotIndexData &index)
{
  index = indices_;
}

void
CGnuPlotFile::
setIndices(const CGnuPlotIndexData &index)
{
  indices_ = index;
}

void
CGnuPlotFile::
every(CGnuPlotEveryData &every)
{
  every = every_;
}

void
CGnuPlotFile::
setEvery(const CGnuPlotEveryData &every)
{
  every_ = every;
}

bool
CGnuPlotFile::
loadFile(const std::string &filename)
{
  filename_ = filename;

  if (! addFileLines())
    return false;

  processFile();

  return true;
}

bool
CGnuPlotFile::
addFileLines()
{
  lines_.clear();

  // open file
  CUnixFile file(filename_);

  if (! file.open())
    return false;

  // read lines
  std::string line;

  while (file.readLine(line))
    lines_.push_back(line);

  return true;
}

void
CGnuPlotFile::
setLines(const Lines &lines)
{
  lines_ = lines;
}

void
CGnuPlotFile::
clearLines()
{
  lines_.clear();
}

void
CGnuPlotFile::
reset()
{
  resetIndices();
  resetEvery  ();

  resetBlankLines();

  columnHeaders_      = false;
  columnHeaderFields_ = Fields();
}

void
CGnuPlotFile::
processFile()
{
  if (isBinary())
    return processBinaryFile();

  if (isCsv())
    return processCsvFile();

  //---

  sets_.clear();

  maxNumFields_ = 0;

  // TODO: skip comments and blank lines at start of file

  // process each line in file
  bool columnHeaders = isColumnHeaders();

  Set        set;
  SubSet     subSet;
  SubSetLine subSetLine;

  int bline   = 0;

  int setNum    = 0;
  int subSetNum = 0;
  int lineNum   = 0;

  for (const auto &line : lines_) {
    if (columnHeaders && setNum == 0 && subSetNum == 0 && lineNum == 0) {
      if (line.empty())
        continue;

      Fields fields;

      parseFileLine(line, fields);

      columnHeaderFields_ = fields;

      columnHeaders = false;

      continue;
    }

    if (line.empty()) {
      ++bline;

      if      (bline == subSetBlankLines_) {
        lineNum = 0;

        ++subSetNum;

        // start new subset
        if (! subSet.lines.empty()) {
          subSet.num = subSetNum;

          set.subSets.push_back(subSet);

          subSet.lines.clear();
        }
      }
      else if (bline == setBlankLines_) {
        lineNum   = 0;
        subSetNum = 0;

        ++setNum;

        // start new set
        if (! set.subSets.empty()) {
          set.num = setNum;

          sets_.push_back(set);

          set.subSets.clear();
        }
      }

      continue;
    }

    //---

    bline = 0;

    //---

    auto comment      = commentChars_.getValue("#");
    bool commentFound = false;

    auto line1 = line;

    if (line1.find(comment) != std::string::npos) {
      CParseLine pline(line1);

      while (pline.isValid()) {
        if (isParseStrings() && pline.isChar('\"')) {
          pline.skipChar();

          while (pline.isValid() && ! pline.isChar('"')) {
            char c = pline.getChar();

            if (c == '\\') {
              if (pline.isValid())
                pline.skipChar();
            }
          }

          if (pline.isChar('"'))
            pline.skipChar();
        }
        else {
          if (pline.isChars(comment)) {
            line1 = CStrUtil::stripSpaces(pline.substr(0, pline.pos()));

            std::string cstr = CStrUtil::stripSpaces(pline.substr(pline.pos() + comment.length()));

            commentStrs_[setNum][subSetNum][lineNum] = cstr;

            commentFound = true;

            break;
          }
          else
            pline.skipChar();
        }
      }

      if (commentFound && line1.empty())
        continue;
    }

    //---

    bool setActive   = indices_.validIndex(setNum);
    bool blockActive = every_  .validBlockIndex(subSetNum);
    bool lineActive  = every_  .validPointIndex(lineNum);

    //---

    if (setActive && blockActive && lineActive) {
      // get fields from line
      parseFileLine(line1, subSetLine.fields);

      subSetLine.num = lineNum;

      subSet.lines.push_back(subSetLine);

      maxNumFields_ = std::max(maxNumFields_, int(subSetLine.fields.size()));

      //---

      subSetLine.fields.clear();
    }

    ++lineNum;
  }

  //---

  if (! subSet.lines.empty())
    set.subSets.push_back(subSet);

  if (! set.subSets.empty())
    sets_.push_back(set);
}

void
CGnuPlotFile::
parseFileLine(const std::string &str, Fields &fields)
{
  bool isSeparator = false;

  std::vector<std::string> strs;

  Words words(strs);

  CParseLine line(str);

  while (line.isValid()) {
    if (isParseStrings() && line.isChar('\"')) {
      line.skipChar();

      std::string word;

      while (line.isValid() && ! line.isChar('"')) {
        char c = line.getChar();

        if (c == '\\') {
          if (line.isValid()) {
            c = line.getChar();

            switch (c) {
              case 't' : word += '\t'; break;
              case 'n' : word += '\n'; break;
              default  : word += '?' ; break;
            }
          }
          else
            word += c;
        }
        else
          word += c;
      }

      if (line.isChar('"'))
        line.skipChar();

      words.addWord("\"" + word + "\"");

      isSeparator = false;
    }
    else if (separator() == '\0' && line.isSpace()) {
      words.flush(false);

      line.skipSpace();

      isSeparator = true;
    }
    else if (line.isChar(separator())) {
      words.flush(isSeparator);

      // TODO: skip all generic separators ?
      while (line.isValid() && line.isChar(separator()))
        line.skipChar();

      isSeparator = true;
    }
    else {
      words.addChar(line.getChar());

      isSeparator = false;
    }
  }

  words.flush(true);

  for (const auto &s : strs)
    fields.push_back(Field(s));
}

void
CGnuPlotFile::
processBinaryFile()
{
  sets_.clear();

  CUnixFile file(filename_);

  if (! file.open())
    return;

  std::vector<double> values;

  binaryFormat_.readValues(file, values);

  if (values.empty())
    return;

  if (isMatrix()) {
    int nv = values.size();

    int nx = values[0];

    int i = 1;

    if (i + nx > nv)
      return;

    typedef std::map<int,double> IRMap;

    std::vector<double> xvals;
    std::vector<double> yvals;
    std::map<int,IRMap> zvals;

    for (int j = 0; j < nx; ++j)
      xvals.push_back(values[i++]);

    int ny = (nv - nx - 1)/(nx + 1);

    for (int k = 0; k < ny; ++k) {
      yvals.push_back(values[i++]);

      for (int j = 0; j < nx; ++j) {
        zvals[j][k] = values[i++];
      }
    }

    CGnuPlotFile::Set set;

    for (int k = 0; k < ny; ++k) {
      CGnuPlotFile::SubSet subSet;

      std::string ystr = std::to_string(yvals[k]);

      for (int j = 0; j < nx; ++j) {
        std::string xstr = std::to_string(xvals[j]);
        std::string zstr = std::to_string(zvals[j][k]);

        CGnuPlotFile::SubSetLine subSetLine;

        subSetLine.fields.push_back(xstr);
        subSetLine.fields.push_back(ystr);
        subSetLine.fields.push_back(zstr);

        subSet.lines.push_back(subSetLine);
      }

      set.subSets.push_back(subSet);
    }

    sets_.push_back(set);
  }
  else {
    int w = binarySize_.width;
    int h = binarySize_.height;

    if (w <= 1) w = 1;
    if (h <= 1) h = 1;

    int wh = w*h;
    int nd = values.size()/wh;

    if (nd < 0) {
      std::cerr << "Bad image data size" << std::endl;
      return;
    }

    int i = 0;

    CGnuPlotFile::Set set;

    for (int y = 0; y < h; ++y) {
      CGnuPlotFile::SubSet subSet;

      std::string ystr = std::to_string(y);

      for (int x = 0; x < w; ++x) {
        CGnuPlotFile::SubSetLine subSetLine;

        std::string xstr = std::to_string(x);

        subSetLine.fields.push_back(xstr);
        subSetLine.fields.push_back(ystr);

        for (int z = 0; z < nd; ++z) {
          std::string str = std::to_string(values[i++]);

          subSetLine.fields.push_back(str);
        }

        subSet.lines.push_back(subSetLine);
      }

      set.subSets.push_back(subSet);
    }

    sets_.push_back(set);
  }
}

void
CGnuPlotFile::
processCsvFile()
{
  bool columnHeaders = isColumnHeaders();

  sets_.clear();

  CGnuPlotFile::Set    set;
  CGnuPlotFile::SubSet subSet;

  CCsv csv(filename_);

  CCsv::FieldsArray fieldsArray;

  csv.getFields(fieldsArray);

  int i = 0;

  for (const auto &fields : fieldsArray) {
    if (columnHeaders) {
      for (const auto &f : fields)
        columnHeaderFields_.push_back(f);

      columnHeaders = false;

      continue;
    }

    CGnuPlotFile::SubSetLine subSetLine;

    subSetLine.num = i;

    for (const auto &f : fields)
      subSetLine.fields.push_back(f);

    subSet.lines.push_back(subSetLine);

    ++i;
  }

  set.subSets.push_back(subSet);

  sets_.push_back(set);
}

void
CGnuPlotFile::
unset()
{
  commentChars_ = COptString();
  missingStr_   = "";
  separator_    = '\0';
  fortran_      = false;
  fpeTrap_      = true;
  binary_       = false;
}

void
CGnuPlotFile::
addSet(const Set &set)
{
  sets_.push_back(set);
}

void
CGnuPlotFile::
show(std::ostream &os, std::map<std::string,bool> &show, bool verbose)
{
  if (show.empty() || show.find("missing") != show.end()) {
    if (verbose) {
      if (missingStr().empty())
        os << "No missing data string set for datafile";
      else
        os << "\"" << missingStr() << "\" in datafile is interpreted as missing value";

      os << std::endl;
    }
    else {
      if ( ! missingStr().empty())
        os << "set datafile missing \"" << missingStr() << "\"" << std::endl;
    }
  }

  if (show.empty() || show.find("separator") != show.end()) {
    if (verbose)
      os << "datafile fields separated by ";
    else
      os << "set datafile separator ";

    if (! separator())
      os << "whitespace";
    else
      os << "\"" << separator() << "\"";

    os << std::endl;
  }

  if (show.empty() || show.find("commentschar") != show.end()) {
    if (verbose) {
      os << "Comments chars are \"" << commentChars() << "\"" << std::endl;
    }
    else {
      if (hasCommentChars()) {
        os << "set datafile commentschar \"" << commentChars() << "\"" << std::endl;
      }
    }
  }
}

void
CGnuPlotFile::
save(std::ostream &os)
{
  os << "set datafile ";

  if (separator() == '\0')
    os << "separator whitespace";
  else
    os << "separator \"" << separator() << "\"";

  os << std::endl;
}

void
CGnuPlotFile::
print(std::ostream &os) const
{
  for (const auto &s : sets_)
    s.print(os);
}
