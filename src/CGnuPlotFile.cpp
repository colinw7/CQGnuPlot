#include <CGnuPlotFile.h>
#include <CUnixFile.h>
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

  void flush() {
    if (force_ || word_ != "") {
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
getIndices(int &indexStart, int &indexEnd, int &indexStep)
{
  indexStart = indices_.start;
  indexEnd   = indices_.end;
  indexStep  = indices_.step;
}

void
CGnuPlotFile::
setIndices(int indexStart, int indexEnd, int indexStep)
{
  indices_.start = indexStart;
  indices_.end   = indexEnd;
  indices_.step  = indexStep;
}

void
CGnuPlotFile::
getEvery(int &everyStart, int &everyEnd, int &everyStep)
{
  everyStart = every_.start;
  everyEnd   = every_.end;
  everyStep  = every_.step;
}

void
CGnuPlotFile::
setEvery(int everyStart, int everyEnd, int everyStep)
{
  every_.start = everyStart;
  every_.end   = everyEnd;
  every_.step  = everyStep;
}

bool
CGnuPlotFile::
loadFile(const std::string &filename)
{
  filename_ = filename;

  if (! addFileLines())
    return false;

  processLines();

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
processLines()
{
  sets_.clear();

  maxNumFields_ = 0;

  // TODO: skip comments and blank lines at start of file

  // process each line in file
  Set        set;
  SubSet     subSet;
  SubSetLine subSetLine;

  int bline   = 0;

  int setNum    = 0;
  int subSetNum = 0;
  int lineNum   = 0;

  for (const auto &line : lines_) {
    if (line.empty()) {
      ++bline;

      if      (bline == 1) {
        lineNum = 0;

        ++subSetNum;

        // start new subset
        if (! subSet.lines.empty()) {
          subSet.num = subSetNum;

          set.subSets.push_back(subSet);

          subSet.lines.clear();
        }
      }
      else if (bline == 2) {
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

    std::string line1 = line;

    auto pos1 = line1.find(commentChars_.getValue("#"));

    if (pos1 != std::string::npos) {
      line1 = CStrUtil::stripSpaces(line1.substr(0, pos1));

      if (line1.empty()) {
        continue;
      }
    }

    //---

    bool setActive  = indices_.validIndex(setNum);
    bool lineActive = every_  .validIndex(lineNum);

    //---

    if (setActive && lineActive) {
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
  if (csv_) {
    (void) CStrUtil::addFields(str, fields, ",");
  }
  else {
    Words words(fields);

    CParseLine line(str);

    while (line.isValid()) {
      if (parseStrings_ && line.isChar('\"')) {
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

        words.addWord(word);
      }
      else if (separator_ == '\0' && line.isSpace()) {
        words.flush();

        line.skipSpace();
      }
      else if (line.isChar(separator_)) {
        words.flush();

        while (line.isValid() && line.isChar(separator_))
          line.skipChar();
      }
      else {
        words.addChar(line.getChar());
      }
    }
  }
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
show(std::ostream &os, std::map<std::string,bool> &show, bool verbose)
{
  if (show.empty() || show.find("missing") != show.end()) {
    if (verbose) {
      if (getMissingStr().empty())
        os << "No missing data string set for datafile";
      else
        os << "\"" << getMissingStr() << "\" in datafile is interpreted as missing value";

      os << std::endl;
    }
    else {
      if ( ! getMissingStr().empty())
        os << "set datafile missing \"" << getMissingStr() << "\"" << std::endl;
    }
  }

  if (show.empty() || show.find("separator") != show.end()) {
    if (verbose)
      os << "datafile fields separated by ";
    else
      os << "set datafile separator ";

    if (! getSeparator())
      os << "whitespace";
    else
      os << "\"" << getSeparator() << "\"";

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

  if (separator_ == '\0')
    os << "separator whitespace";
  else
    os << "separator \"" << getSeparator() << "\"";

  os << std::endl;
}
