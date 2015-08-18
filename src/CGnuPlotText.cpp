#include <CGnuPlotText.h>
#include <CGnuPlotRenderer.h>
#include <CParseLine.h>
#include <CMathGeom2D.h>
#include <CUtf8.h>

CGnuPlotText::
CGnuPlotText(const std::string &str) :
 str_(str)
{
  init();
}

void
CGnuPlotText::
setText(const std::string &str)
{
  str_  = str;
  estr_ = "";

  parts_.clear();

  init();
}

void
CGnuPlotText::
init()
{
  CParseLine line(str_);

  while (line.isValid()) {
    char c = line.getChar();

    if      (c == '^') {
      CGnuPlotText text;

      (void) readPartText(line, text);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::SUPERSCRIPT, text));
    }
    else if (c == '_') {
      CGnuPlotText text;

      (void) readPartText(line, text);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::SUBSCRIPT, text));
    }
    else if (c == '@') {
      CGnuPlotTextPart *part1;
      CGnuPlotTextPart *part2;

      (void) readPart(line, &part1);
      (void) readPart(line, &part2);

      CGnuPlotText text;

      if (part1) text.addPart(part1);
      if (part2) text.addPart(part2);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::BOX, text));
    }
    else if (c == '&') {
      CGnuPlotText text;

      (void) readPartText(line, text);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::SPACE, text));
    }
    else if (c == '~') {
      CGnuPlotText text, text1;

      (void) readPartText(line, text);
      (void) readPartText(line, text1);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::OVERPRINT, text, text1));
    }
    else if (c == '{') {
      std::string tstr;

      if (! readBraceString(line, tstr))
        continue;

      CGnuPlotText text(tstr);

      addPart(new CGnuPlotTextPart(CGnuPlotCharType::TEXT, text));
    }
    else if (c == '}') {
      break;
    }
    else if (c == '\\') {
      char c1 = '0', c2 = '0', c3 = '0';

      c1 = line.lookChar();

      if      (isdigit(c1)) {
        line.skipChar();

        c2 = line.lookChar();

        if (isdigit(c2)) {
          line.skipChar();

          c3 = line.lookChar();

          if (isdigit(c3))
            line.skipChar();
        }

        int i1 = c1 - '0';
        int i2 = c2 - '0';
        int i3 = c3 - '0';

        int i = i1*64 + i2*8 + i3;

        estr_.push_back(char(i & 0xff));
      }
      else if (c1 != '\0') {
        line.skipChar();

        estr_.push_back(c1);
      }
    }
    else if (c == '\n') {
      addPart(new CGnuPlotTextPart(CGnuPlotCharType::NEWLINE));
    }
    else {
      line.ungetChar();

      int pos = line.pos();

      (void) CUtf8::readNextChar(line.str(), pos);

      int n = pos - line.pos();

      for (int i = 0; i < n; ++i)
        estr_.push_back(line.getChar());
    }
  }
}

bool
CGnuPlotText::
readPart(CParseLine &line, CGnuPlotTextPart **part)
{
  *part = 0;

  if (! line.isValid())
    return false;

  char c = line.getChar();

  if      (c == '^') {
    CGnuPlotText text;

    (void) readPartText(line, text);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::SUPERSCRIPT, text);
  }
  else if (c == '_') {
    CGnuPlotText text;

    (void) readPartText(line, text);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::SUBSCRIPT, text);
  }
  else if (c == '@') {
    //CGnuPlotTextPart *part1, *part2;

    //(void) readPart(line, &part1);
    //(void) readPart(line, &part2);

    return false;
  }
  else if (c == '&') {
    CGnuPlotText text;

    (void) readPartText(line, text);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::SPACE, text);
  }
  else if (c == '~') {
    CGnuPlotText text, text1;

    (void) readPartText(line, text);
    (void) readPartText(line, text1);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::OVERPRINT, text, text1);
  }
  else if (c == '{') {
    std::string tstr;

    if (! readBraceString(line, tstr))
      return false;

    CGnuPlotText text(tstr);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::TEXT, text);
  }
  else {
    std::string tstr;

    tstr.push_back(c);

    CGnuPlotText text(tstr);

    *part = new CGnuPlotTextPart(CGnuPlotCharType::TEXT, text);
  }

  return true;
}

bool
CGnuPlotText::
readPartText(CParseLine &line, CGnuPlotText &text)
{
  if (! line.isValid())
    return false;

  std::string tstr;

  if (line.isChar('{')) {
    line.skipChar();

    if (! readBraceString(line, tstr))
      return false;
  }
  else {
    tstr += line.getChar();
  }

  text.setText(tstr);

  return true;
}

bool
CGnuPlotText::
readBraceString(CParseLine &line, std::string &tstr)
{
  tstr = "";

  int brackets = 1;

  while (line.isValid()) {
    if      (line.isChar('{')) {
      ++brackets;
    }
    else if (line.isChar('}')) {
      --brackets;

      if (brackets == 0)
        break;
    }

    tstr += line.getChar();
  }

  if (line.isChar('}'))
    line.skipChar();

  return true;
}

void
CGnuPlotText::
draw(CGnuPlotRenderer *renderer, const CBBox2D &bbox, CHAlignType halign,
     const CRGBA &c, double a) const
{
  //renderer->drawRotatedRect(bbox, a, CRGBA(1,0,0), 1);

  CFontPtr origFont = renderer->getFont();

  //---

  CGnuPlotTextState state(renderer);

  state.lines.push_back(CGnuPlotTextLine());

  placeChars(state, CGnuPlotCharType::TEXT);

  //---

  CPoint2D origin = bbox.getCenter();

  double dy = bbox.getTop();

  for (const auto &l : state.lines) {
    // calc line bbox
    CBBox2D lbbox;

    for (const auto &ch : l.chars) {
      double w  = renderer->pixelWidthToWindowWidth  (ch.font->getStringWidth(ch.str));
    //double fa = renderer->pixelHeightToWindowHeight(ch.font->getCharAscent ());
      double fa = l.ascent;
      double fd = renderer->pixelHeightToWindowHeight(ch.font->getCharDescent());

      CPoint2D p1(ch.pos.x    , ch.pos.y - fd);
      CPoint2D p2(ch.pos.x + w, ch.pos.y + fa);

      if (ch.type != CGnuPlotCharType::SUPERSCRIPT && ch.type != CGnuPlotCharType::SUBSCRIPT) {
        lbbox.add(p1);
        lbbox.add(p2);
      }
    }

    double dx;

    if      (halign == CHALIGN_TYPE_LEFT)
      dx = bbox.getXMin();
    else if (halign == CHALIGN_TYPE_CENTER)
      dx = bbox.getXMid() - lbbox.getWidth()/2;
    else if (halign == CHALIGN_TYPE_RIGHT)
      dx = bbox.getXMax() - lbbox.getWidth();

    //CBBox2D lbbox1 = lbbox.moveBy(CPoint2D(dx, dy));
    //renderer->drawRotatedRect(lbbox1, a, CRGBA(0,1,0), 1);

    //---

    // draw line
    for (const auto &ch : l.chars) {
      renderer->setFont(ch.font);

      CPoint2D p1 = ch.pos + CPoint2D(dx, dy);

      if (fabs(a) < 1E-6)
        renderer->drawText(p1, ch.str, c);
      else {
        CPoint2D pr1 = renderer->rotatePoint(p1, a, origin);

        renderer->drawRotatedText(pr1, ch.str, a, CHALIGN_TYPE_LEFT, CVALIGN_TYPE_BOTTOM, c);
      }
    }

    dy += lbbox.getYMin();
  }

  renderer->setFont(origFont);
}

void
CGnuPlotText::
placeChars(CGnuPlotTextState &state, CGnuPlotCharType type) const
{
  CGnuPlotRenderer *renderer = state.renderer;

  CFontPtr origFont = renderer->getFont();

  state.font = origFont;

  uint i = 0;

  if (estr_[0] == '/') {
    ++i;

    std::string family, style, multiplier, size;

    std::vector<std::string *> fstrs = {{ &family, &style, &multiplier, &size }};

    int fi = 0;

    // /<family>:<style>=<size>
    for ( ; i < estr_.size() && ! isspace(estr_[i]); ++i) {
      if      (estr_[i] == ':')
        fi = 1;
      else if (estr_[i] == '*')
        fi = 2;
      else if (estr_[i] == '=')
        fi = 3;
      else
        *(fstrs[fi]) += estr_[i];
    }

    while (i < estr_.size() && isspace(estr_[i]))
      ++i;

    if (family     == "") family     = state.font->getFamily();
    if (style      == "") style      = CFont::fontStyleToString(state.font->getStyle());
    if (multiplier == "") multiplier = "1";
    if (size       == "") size       = CStrUtil::toString(state.font->getSize());

    double fs = std::max(0.0, CStrUtil::toReal(size)*CStrUtil::toReal(multiplier));

    CFontPtr font1 = state.font->dup(family, CFont::stringToFontStyle(style), fs);

    state.font = font1;
  }

  if (type == CGnuPlotCharType::SUBSCRIPT || type == CGnuPlotCharType::SUPERSCRIPT) {
    std::string family = state.font->getFamily();
    CFontStyle  style  = state.font->getStyle();
    double      size   = state.font->getSize();

    CFontPtr font1 = state.font->dup(family, style, size*0.8);

    state.font = font1;
  }

  double dy = 0.0;

  if (type == CGnuPlotCharType::OVERPRINT) {
    double s = 0.0;

    if (estr_[0] == '.' || isdigit(estr_[0]))
      CStrUtil::readReal(estr_, &i, &s);

    //double o = state.font->getCharAscent()*s;
    CGnuPlotTextLine &line = state.lines.back();

    double o = line.ascent*s;

    dy = renderer->pixelHeightToWindowHeight(o);
  }

  while (i < estr_.size()) {
    state.pos += CPoint2D(0, dy);

    placePartChars(state, i, type);

    state.pos -= CPoint2D(0, dy);

    CGnuPlotTextLine &line = state.lines.back();

    if (line.ascent <= 0) {
      line.ascent  = renderer->pixelHeightToWindowHeight(state.font->getCharAscent ());
      line.descent = renderer->pixelHeightToWindowHeight(state.font->getCharDescent());
    }

    double fa = line.ascent;

    int i1 = i;

    (void) CUtf8::readNextChar(estr_, i1);

    std::string cs(&estr_[i], i1 - i);

    i = i1;

    CPoint2D pos = state.pos - CPoint2D(0, fa);

    CGnuPlotTextChar c(pos, state.font, cs, type);

    state.lines.back().chars.push_back(c);

    double w = renderer->pixelWidthToWindowWidth(state.font->getStringWidth(cs));

    state.pos += CPoint2D(w, 0);
  }

  state.pos += CPoint2D(0, dy);

  placePartChars(state, estr_.size(), type);

  renderer->setFont(origFont);
}

void
CGnuPlotText::
placePartChars(CGnuPlotTextState &state, int i, CGnuPlotCharType type) const
{
  auto p = parts_.find(i);
  if (p == parts_.end()) return;

  for (const auto &part : (*p).second) {
    CPoint2D pos1 = state.pos;

    part->placeChars(state);

    if (type == CGnuPlotCharType::BOX)
      state.pos = pos1;
  }
}

CBBox2D
CGnuPlotText::
calcBBox(CGnuPlotRenderer *renderer) const
{
  CFontPtr origFont = renderer->getFont();

  //---

  CGnuPlotTextState state(renderer);

  state.lines.push_back(CGnuPlotTextLine());

  placeChars(state, CGnuPlotCharType::TEXT);

  //---

  CPoint2D origin(0,0);

  CBBox2D bbox;

  CPoint2D pos(0,0);

  for (const auto &l : state.lines) {
    // calc line bbox
    CBBox2D lbbox;

    for (const auto &ch : l.chars) {
      double w  = renderer->pixelWidthToWindowWidth  (ch.font->getStringWidth(ch.str));
    //double fa = renderer->pixelHeightToWindowHeight(ch.font->getCharAscent ());
      double fa = l.ascent;
      double fd = renderer->pixelHeightToWindowHeight(ch.font->getCharDescent());

      CPoint2D p1(ch.pos.x    , ch.pos.y - fd);
      CPoint2D p2(ch.pos.x + w, ch.pos.y + fa);

      if (ch.type != CGnuPlotCharType::SUPERSCRIPT && ch.type != CGnuPlotCharType::SUBSCRIPT) {
        lbbox.add(p1);
        lbbox.add(p2);
      }
    }

    double y = lbbox.getYMin();

    lbbox.moveBy(CPoint2D(0, pos.y));

    bbox.add(lbbox);

    pos.y += y;
  }

  renderer->setFont(origFont);

  return bbox;
}

void
CGnuPlotText::
print(std::ostream &os) const
{
  for (uint i = 0; i < estr_.size(); ++i) {
    printParts(os, i);

    os << estr_[i];
  }

  printParts(os, estr_.size());
}

void
CGnuPlotText::
printParts(std::ostream &os, int i) const
{
  auto p = parts_.find(i);
  if (p == parts_.end()) return;

  for (const auto &part : (*p).second)
    part->print(os);
}

//------

void
CGnuPlotTextPart::
placeChars(CGnuPlotTextState &state) const
{
  CGnuPlotRenderer *renderer = state.renderer;

  state.font = renderer->getFont();

  if      (type_ == CGnuPlotCharType::SUPERSCRIPT) {
    double h = renderer->pixelHeightToWindowHeight(state.font->getCharHeight()/2);

    state.pos += CPoint2D(0, h);

    text_.placeChars(state, type_);

    state.pos -= CPoint2D(0, h);
  }
  else if (type_ == CGnuPlotCharType::SUBSCRIPT) {
    double h = renderer->pixelHeightToWindowHeight(state.font->getCharHeight()/2);

    state.pos -= CPoint2D(0, h);

    text_.placeChars(state, type_);

    state.pos += CPoint2D(0, h);
  }
  else if (type_ == CGnuPlotCharType::OVERPRINT) {
    CPoint2D pos1 = state.pos;

    text_.placeChars(state, CGnuPlotCharType::TEXT);

    state.pos = pos1;

    text1_.placeChars(state, CGnuPlotCharType::OVERPRINT);
  }
  else if (type_ == CGnuPlotCharType::BOX) {
    text_.placeChars(state, CGnuPlotCharType::BOX);
  }
  else if (type_ == CGnuPlotCharType::SPACE) {
    double w = renderer->pixelWidthToWindowWidth(state.font->getStringWidth(text_.etext()));

    state.pos += CPoint2D(w, 0);
  }
  else if (type_ == CGnuPlotCharType::NEWLINE) {
    state.pos = CPoint2D(0, 0);

    state.lines.push_back(CGnuPlotTextLine());
  }
  else if (type_ == CGnuPlotCharType::TEXT) {
    text_.placeChars(state, CGnuPlotCharType::TEXT);
  }
}

std::string
CGnuPlotTextPart::
typeStr() const
{
  switch (type_) {
    case CGnuPlotCharType::SUPERSCRIPT: return "^";
    case CGnuPlotCharType::SUBSCRIPT  : return "_";
    case CGnuPlotCharType::BOX        : return "@";
    case CGnuPlotCharType::SPACE      : return "&";
    case CGnuPlotCharType::OVERPRINT  : return "~";
    case CGnuPlotCharType::FONT       : return "/";
    case CGnuPlotCharType::TEXT       : return "";
    default                           : return "";
  }
}

void
CGnuPlotTextPart::
print(std::ostream &os) const
{
  if      (type_ == CGnuPlotCharType::OVERPRINT) {
    os << "{~"; text_ .print(os); os << "}";
    os << "{" ; text1_.print(os); os << "}";
  }
  else if (type_ == CGnuPlotCharType::BOX) {
    os << "{@"; text_ .print(os); os << "}";
    os << "{" ; text1_.print(os); os << "}";
  }
  else if (type_ == CGnuPlotCharType::FONT) {
    os << "{/"; text_ .print(os);
    os << " " ; text1_.print(os); os << "}";
  }
  else {
    os << typeStr();
    os << "{"; text_.print(os); os << "}";
  }
}
