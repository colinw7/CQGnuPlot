#ifndef CGnuPlotText_H
#define CGnuPlotText_H

#include <CBBox2D.h>
#include <CAlignType.h>
#include <CRGBA.h>
#include <CFont.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

class CParseLine;
class CGnuPlotTextPart;
class CGnuPlotRenderer;

enum class CGnuPlotCharType {
  SUPERSCRIPT,
  SUBSCRIPT,
  BOX,
  SPACE,
  OVERPRINT,
  FONT,
  NEWLINE,
  TEXT
};

struct CGnuPlotTextChar {
  CGnuPlotTextChar(CPoint2D &p, CFontPtr font1, const std::string &str1, CGnuPlotCharType type1) :
   pos(p), font(font1), str(str1), type(type1) {
  }

  CPoint2D         pos;
  CFontPtr         font;
  std::string      str;
  CGnuPlotCharType type;
};

struct CGnuPlotTextLine {
  typedef std::vector<CGnuPlotTextChar> Chars;

  Chars  chars;
  double ascent { 0 };
};

struct CGnuPlotTextState {
  typedef std::vector<CGnuPlotTextLine> Lines;

  CGnuPlotTextState(CGnuPlotRenderer *r) :
   renderer(r) {
  }

  CGnuPlotRenderer *renderer;
  CPoint2D          pos { 0, 0 };
  Lines             lines;
};

class CGnuPlotText {
 public:
  CGnuPlotText(const std::string &str="");

  void setText(const std::string &str);

  const std::string &text() const { return str_; }

  const std::string &etext() const { return estr_; }

  void draw(CGnuPlotRenderer *renderer, const CBBox2D &bbox, CHAlignType halign,
            const CRGBA &c=CRGBA(0,0,0)) const;

  CBBox2D calcBBox(CGnuPlotRenderer *renderer) const;

  void print(std::ostream &os=std::cout) const;

 private:
  friend class CGnuPlotTextPart;

  void init();

  void placeChars(CGnuPlotTextState &state, CGnuPlotCharType type) const;

  bool readPart(CParseLine &line, CGnuPlotTextPart **part);
  bool readPartText(CParseLine &line, CGnuPlotText &text);
  bool readBraceString(CParseLine &line, std::string &tstr);

  void addPart(CGnuPlotTextPart *part) { parts_[estr_.size()].push_back(part); }

  void placePartChars(CGnuPlotTextState &state, int i, CGnuPlotCharType type) const;

  void printParts(std::ostream &os, int i) const;

 private:
  typedef std::vector<CGnuPlotTextPart *> Parts;
  typedef std::map<int,Parts>             PosParts;

  std::string str_;
  std::string estr_;
  PosParts    parts_;
};

class CGnuPlotTextPart {
 public:
  CGnuPlotTextPart(CGnuPlotCharType type, const CGnuPlotText &text=CGnuPlotText(),
                   const CGnuPlotText &text1=CGnuPlotText()) :
   type_(type), text_(text), text1_(text1) {
  }

  void placeChars(CGnuPlotTextState &state) const;

  void print(std::ostream &os) const;

 private:
  std::string typeStr() const;

 private:
  CGnuPlotCharType type_;
  CGnuPlotText     text_;
  CGnuPlotText     text1_;
};

#endif
