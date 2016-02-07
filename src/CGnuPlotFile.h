#ifndef CGnuPlotFile_H
#define CGnuPlotFile_H

#include <CGnuPlotBinaryFormat.h>
#include <CGnuPlotEveryData.h>
#include <CGnuPlotIndexData.h>
#include <CSize2D.h>
#include <COptVal.h>

#include <string>
#include <vector>
#include <map>
#include <limits>
#include <iostream>

class CGnuPlotFile {
 public:
  typedef std::string                Line;
  typedef std::vector<Line>          Lines;
  typedef std::map<int,Line>         LineString;
  typedef std::map<int,LineString>   SubSetString;
  typedef std::map<int,SubSetString> SetString;

  //---

  class Field {
   public:
    Field(const std::string &str="") :
     str_(str) {
    }

    const std::string &str() const { return str_; }

    void print(std::ostream &os) const {
      os << str_;
    }

   private:
    std::string str_;
  };

  typedef std::vector<Field> Fields;

  struct SubSetLine {
    int    num { 0 };
    Fields fields;

    void print(std::ostream &os) const {
      for (const auto &f : fields) {
        os << " "; f.print(os);
      }
      os << std::endl;
    }
  };

  typedef std::vector<SubSetLine> SubSetLines;

  struct SubSet {
    int         num { 0 };
    SubSetLines lines;

    void print(std::ostream &os) const {
      for (const auto &l : lines)
        l.print(os);
    }
  };

  typedef std::vector<SubSet> SubSets;

  struct Set {
    int     num { 0 };
    SubSets subSets;

    void print(std::ostream &os) const {
      for (const auto &s : subSets)
        s.print(os);
    }
  };

  typedef std::vector<Set> Sets;

 public:
  CGnuPlotFile();

  const std::string &fileName() const { return filename_; }
  void setFileName(const std::string &v) { filename_ = v; }

  bool hasCommentChars() const { return commentChars_.isValid(); }
  std::string commentChars() const { return commentChars_.getValue("#"); }
  void setCommentChars(const std::string &chars) { commentChars_ = chars; }

  const std::string &missingStr() const { return missingStr_; }
  void setMissingStr(const std::string &chars) { missingStr_ = chars; }

  char separator() const { return separator_; }
  void setSeparator(char c) { separator_ = c; }
  void resetSeparator() { separator_ = '\0'; }

  bool isParseStrings() const { return parseStrings_; }
  void setParseStrings(bool b) { parseStrings_ = b; }

  bool isColumnHeaders() const { return columnHeaders_; }
  void setColumnHeaders(bool b) { columnHeaders_ = b; }

  const CGnuPlotIndexData &indices() const { return indices_; }
  void indices(CGnuPlotIndexData &index);
  void setIndices(const CGnuPlotIndexData &index);
  void resetIndices() { indices_.reset(); }

  const CGnuPlotEveryData &every() const { return every_; }
  void every(CGnuPlotEveryData &every);
  void setEvery(const CGnuPlotEveryData &every);
  void resetEvery() { every_.reset(); }

  int setBlankLines() const { return setBlankLines_; }
  void setSetBlankLines(int i) { setBlankLines_ = i; }

  int subSetBlankLines() const { return subSetBlankLines_; }
  void setSubSetBlankLines(int i) { subSetBlankLines_ = i; }

  void resetBlankLines() { setBlankLines_ = 2; subSetBlankLines_ = 1; }

  const Lines &lines() const { return lines_; }
  void setLines(const Lines &lines);
  void clearLines();

  bool loadFile(const std::string &filename);

  void reset();

  void processFile();

  void processBinaryFile();

  void processCsvFile();

  int numSets() const { return sets_.size(); }

  int numSubSets(int setNum=0) const {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    const Set &set = sets_[setNum];

    return set.subSets.size();
  }

  int numLines(int subSetNum=0) const {
    return numLines(0, subSetNum);
  }

  int numLines(int setNum, int subSetNum) const {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    const Set &set = sets_[setNum];

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return 0;

    const SubSet &subSet = set.subSets[subSetNum];

    return subSet.lines.size();
  }

  const Fields &fields(int lineNum) const {
    return fields(0, 0, lineNum);
  }

  const Fields &fields(int subSetNum, int lineNum) const {
    return fields(0, subSetNum, lineNum);
  }

  const Fields &fields(int setNum, int subSetNum, int lineNum) const {
    static Fields noFields;

    if (setNum < 0 || setNum >= numSets())
      return noFields;

    const Set &set = sets_[setNum];

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return noFields;

    const SubSet &subSet = set.subSets[subSetNum];

    if (lineNum < 0 || lineNum >= numLines(setNum, subSetNum))
      return noFields;

    const SubSetLine &line = subSet.lines[lineNum];

    return line.fields;
  }

  int numFields(int setNum, int subSetNum, int lineNum) const {
    return fields(setNum, subSetNum, lineNum).size();
  }

  const Field field(int setNum, int subSetNum, int lineNum, int fieldNum) const {
    static std::string noField;

    const Fields &fields = this->fields(setNum, subSetNum, lineNum);

    if (fieldNum < 0 || fieldNum >= int(fields.size()))
      return noField;

    return fields[fieldNum];
  }

  int maxNumFields() const { return maxNumFields_; }

  int numColumnHeaders() const {
    return columnHeaderFields_.size();
  }

  std::string columnHeader(int fieldNum) const {
    if (fieldNum < 0 || fieldNum >= numColumnHeaders())
      return "";

    return columnHeaderFields_[fieldNum].str();
  }

  bool isFortran() const { return fortran_; }
  void setFortran(bool b) { fortran_ = b; }

  bool isFpeTrap() const { return fpeTrap_; }
  void setFpeTrap(bool b) { fpeTrap_ = b; }

  bool isCsv() const { return csv_; }
  void setCsv(bool b) { csv_ = b; }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool b) { matrix_ = b; }

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  const CGnuPlotBinaryFormat &binaryFormat() const { return binaryFormat_; }
  void setBinaryFormat(const CGnuPlotBinaryFormat &v) { binaryFormat_ = v; }

  const CSize2D &binarySize() const { return binarySize_; }
  void setBinarySize(const CSize2D &v) { binarySize_ = v; }

  bool commentStr(int setNum, int subSetNum, int lineNum, std::string &str) const {
    auto p1 = commentStrs_.find(setNum);
    if (p1 == commentStrs_.end()) return false;

    auto p2 = (*p1).second.find(subSetNum);
    if (p2 == (*p1).second.end()) return false;

    auto p3 = (*p2).second.find(lineNum);
    if (p3 == (*p2).second.end()) return false;

    str = (*p3).second;
    if (str == "") return false;

    return true;
  }

  void unset();

  void addSet(const Set &set);

  void show(std::ostream &os, std::map<std::string,bool> &show, bool verbose);

  void save(std::ostream &os);

  void print(std::ostream &os) const;

 private:
  bool addFileLines();

  void parseFileLine(const std::string &str, Fields &fields);

 private:
  typedef std::vector<Fields>          FieldsArray;
  typedef std::map<int,FieldsArray>    SubFieldsArray;
  typedef std::map<int,SubFieldsArray> SetSubFieldsArray;

  std::string          filename_     { "" };
  COptString           commentChars_;
  std::string          missingStr_;
  char                 separator_    { '\0' };
  bool                 parseStrings_ { true };
  bool                 columnHeaders_ { false };
  Fields               columnHeaderFields_;
  CGnuPlotIndexData    indices_;
  CGnuPlotEveryData    every_;
  Lines                lines_;
  Sets                 sets_;
  SetString            commentStrs_;
  int                  maxNumFields_     { 0 };
  int                  setBlankLines_    { 2 };
  int                  subSetBlankLines_ { 1 };
  bool                 fortran_          { false };
  bool                 fpeTrap_          { true };
  bool                 csv_              { false };
  bool                 matrix_           { false };
  bool                 binary_           { false };
  CGnuPlotBinaryFormat binaryFormat_;
  CSize2D              binarySize_;
};

#endif
