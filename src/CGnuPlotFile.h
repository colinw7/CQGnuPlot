#ifndef CGnuPlotFile_H
#define CGnuPlotFile_H

#include <COptVal.h>

#include <string>
#include <vector>
#include <map>
#include <limits>

class CGnuPlotFile {
 public:
  typedef std::vector<std::string>   Lines;
  typedef std::vector<std::string>   Fields;
  typedef std::map<int,std::string>  LineString;
  typedef std::map<int,LineString>   SubSetString;
  typedef std::map<int,SubSetString> SetString;

  //---

  struct Indices {
    Indices() { }

    int start { 0 };
    int end   { std::numeric_limits<int>::max() };
    int step  { 1 };

    bool validIndex(int i) const { return (i >= start && i <= end && ((i - start) % step) == 0); }
  };

  //---

  struct Every {
    Every() { }

    int pointStart { 0 };
    int pointEnd   { std::numeric_limits<int>::max() };
    int pointStep  { 1 };

    int blockStart { 0 };
    int blockEnd   { std::numeric_limits<int>::max() };
    int blockStep  { 1 };

    bool validPointIndex(int i) const {
      return (i >= pointStart && i <= pointEnd && ((i - pointStart) % pointStep) == 0);
    }

    bool validBlockIndex(int i) const {
      return (i >= blockStart && i <= blockEnd && ((i - blockStart) % blockStep) == 0);
    }
  };

  struct SubSetLine {
    int    num { 0 };
    Fields fields;
  };

  typedef std::vector<SubSetLine> SubSetLines;

  struct SubSet {
    int         num { 0 };
    SubSetLines lines;
  };

  typedef std::vector<SubSet> SubSets;

  struct Set {
    int     num { 0 };
    SubSets subSets;
  };

  typedef std::vector<Set> Sets;

 public:
  CGnuPlotFile();

  const std::string &fileName() const { return filename_; }
  void setFileName(const std::string &v) { filename_ = v; }

  bool hasCommentChars() const { return commentChars_.isValid(); }
  std::string commentChars() const { return commentChars_.getValue("#"); }
  void setCommentChars(const std::string &chars) { commentChars_ = chars; }

  void setMissingStr(const std::string &chars) { missingStr_ = chars; }
  const std::string &getMissingStr() const { return missingStr_; }

  char getSeparator() const { return separator_; }
  void setSeparator(char c) { separator_ = c; }
  void resetSeparator() { separator_ = '\0'; }

  void getIndices(int &indexStart, int &indexEnd, int &indexStep);
  void setIndices(int indexStart, int indexEnd, int indexStep);
  void resetIndices() { setIndices(0, std::numeric_limits<int>::max(), 1); }

  const Every &every() const { return every_; }

  void getEvery(int &everyPointStart, int &everyPointEnd, int &everyPointStep,
                int &everyBlockStart, int &everyBlockEnd, int &everyBlockStep);

  void setEvery(int everyPointStart, int everyPointEnd, int everyPointStep,
                int everyBlockStart, int everyBlockEnd, int everyBlockStep);

  void resetEvery() { setEvery(0, std::numeric_limits<int>::max(), 1,
                               0, std::numeric_limits<int>::max(), 1); }

  int setBlankLines() const { return setBlankLines_; }
  void setSetBlankLines(int i) { setBlankLines_ = i; }

  int subSetBlankLines() const { return subSetBlankLines_; }
  void setSubSetBlankLines(int i) { subSetBlankLines_ = i; }

  void resetBlankLines() { setBlankLines_ = 2; subSetBlankLines_ = 1; }

  const Lines &lines() const { return lines_; }
  void setLines(const Lines &lines);
  void clearLines();

  bool loadFile(const std::string &filename);

  void processLines();

  int numSets() const { return sets_.size(); }

  int numSubSets(int setNum=0) const {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    return sets_[setNum].subSets.size();
  }

  int numLines(int subSetNum=0) const {
    return numLines(0, subSetNum);
  }

  int numLines(int setNum, int subSetNum) const {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return 0;

    return sets_[setNum].subSets[subSetNum].lines.size();
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

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return noFields;

    if (lineNum < 0 || lineNum >= numLines(setNum, subSetNum))
      return noFields;

    return sets_[setNum].subSets[subSetNum].lines[lineNum].fields;
  }

  int numFields(int setNum, int subSetNum, int lineNum) const {
    return fields(setNum, subSetNum, lineNum).size();
  }

  const std::string field(int setNum, int subSetNum, int lineNum, int fieldNum) const {
    static std::string noField;

    const Fields &fields = this->fields(setNum, subSetNum, lineNum);

    if (fieldNum < 0 || fieldNum >= int(fields.size()))
      return noField;

    return fields[fieldNum];
  }

  int maxNumFields() const { return maxNumFields_; }

  bool isFortran() const { return fortran_; }
  void setFortran(bool b) { fortran_ = b; }

  bool isFpeTrap() const { return fpeTrap_; }
  void setFpeTrap(bool b) { fpeTrap_ = b; }

  bool isBinary() const { return binary_; }
  void setBinary(bool b) { binary_ = b; }

  bool isCsv() const { return csv_; }
  void setCsv(bool b) { csv_ = b; }

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

  void show(std::ostream &os, std::map<std::string,bool> &show, bool verbose);

  void save(std::ostream &os);

 private:
  bool addFileLines();

  void parseFileLine(const std::string &str, Fields &fields);

 private:
  typedef std::vector<Fields>          FieldsArray;
  typedef std::map<int,FieldsArray>    SubFieldsArray;
  typedef std::map<int,SubFieldsArray> SetSubFieldsArray;

  std::string filename_     { "" };
  COptString  commentChars_;
  std::string missingStr_;
  char        separator_    { '\0' };
  bool        parseStrings_ { true };
  Indices     indices_;
  Every       every_;
  Lines       lines_;
  Sets        sets_;
  SetString   commentStrs_;
  int         maxNumFields_     { 0 };
  int         setBlankLines_    { 2 };
  int         subSetBlankLines_ { 1 };
  bool        fortran_          { false };
  bool        fpeTrap_          { true };
  bool        binary_           { false };
  bool        csv_              { false };
};

#endif
