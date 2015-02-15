#ifndef CGnuPlotFile_H
#define CGnuPlotFile_H

#include <string>
#include <vector>
#include <map>
#include <limits>

class CGnuPlotFile {
 public:
  typedef std::vector<std::string> Lines;
  typedef std::vector<std::string> Fields;

  //---

  struct Indices {
    Indices() { }

    int start { 1 };
    int end   { std::numeric_limits<int>::max() };
    int step  { 1 };

    bool validIndex(int i) const { return (i >= start && i <= end && ((i - start) % step) == 0); }
  };

  //---

  struct Every {
    Every() { }

    int start { 0 };
    int end   { std::numeric_limits<int>::max() };
    int step  { 1 };

    bool validIndex(int i) const { return (i >= start && i <= end && ((i - start) % step) == 0); }
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

  const std::string &commentChars() const { return commentChars_; }
  void setCommentChars(const std::string &chars) { commentChars_ = chars; }

  char getSeparator() const { return separator_; }
  void setSeparator(char c) { separator_ = c; }
  void resetSeparator() { separator_ = '\0'; }

  void getIndices(int &indexStart, int &indexEnd, int &indexStep);
  void setIndices(int indexStart, int indexEnd, int indexStep);
  void resetIndices() { setIndices(0, std::numeric_limits<int>::max(), 1); }

  void getEvery(int &everyStart, int &everyEnd, int &everyStep);
  void setEvery(int everyStart, int everyEnd, int everyStep);
  void resetEvery() { setEvery(0, std::numeric_limits<int>::max(), 1); }

  const Lines &lines() const { return lines_; }
  void setLines(const Lines &line);

  bool loadFile(const std::string &filename);

  void processLines();

  int numSets() const { return sets_.size(); }

  int numSubSets(int setNum=0) const {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    return sets_[setNum].subSets.size();
  }

  int numLines(int subSetNum=0) {
    return numLines(0, subSetNum);
  }

  int numLines(int setNum, int subSetNum) {
    if (setNum < 0 || setNum >= numSets())
      return 0;

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return 0;

    return sets_[setNum].subSets[subSetNum].lines.size();
  }

  const Fields &fields(int lineNum) {
    return fields(0, 0, lineNum);
  }

  const Fields &fields(int subSetNum, int lineNum) {
    return fields(0, subSetNum, lineNum);
  }

  const Fields &fields(int setNum, int subSetNum, int lineNum) {
    static Fields noFields;

    if (setNum < 0 || setNum >= numSets())
      return noFields;

    if (subSetNum < 0 || subSetNum >= numSubSets(setNum))
      return noFields;

    if (lineNum < 0 || lineNum >= numLines(setNum, subSetNum))
      return noFields;

    return sets_[setNum].subSets[subSetNum].lines[lineNum].fields;
  }

  int maxNumFields() const { return maxNumFields_; }

 private:
  bool addFileLines();

  void parseFileLine(const std::string &str, Fields &fields);

 private:
  typedef std::vector<Fields>          FieldsArray;
  typedef std::map<int,FieldsArray>    SubFieldsArray;
  typedef std::map<int,SubFieldsArray> SetSubFieldsArray;

  std::string filename_     { "" };
  std::string commentChars_ { "#" };
  char        separator_    { '\0' };
  bool        parseStrings_ { true };
  Indices     indices_;
  Every       every_;
  Lines       lines_;
  Sets        sets_;
  int         maxNumFields_ { 0 };
};

#endif
