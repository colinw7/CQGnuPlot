#include <CGnuPlotText.h>
#include <CGnuPlotRenderer.h>
#include <CParseLine.h>
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
draw(CGnuPlotRenderer *renderer, const CBBox2D &bbox, CHAlignType halign, const CRGBA &c) const
{
  CFontPtr origFont = renderer->getFont();

  //---

  CGnuPlotTextState state(renderer);

  state.lines.push_back(CGnuPlotTextLine());

  placeChars(state, CGnuPlotCharType::TEXT);

  //---

  double dy = bbox.getTop();

  for (const auto &l : state.lines) {
    // calc line bbox
    CBBox2D lbbox;

    for (const auto &ch : l.chars) {
      double w  = renderer->pixelWidthToWindowWidth  (ch.font->getStringWidth(ch.str));
    //double fa = renderer->pixelHeightToWindowHeight(ch.font->getCharAscent ());
      double fa = l.ascent;
      double fd = renderer->pixelHeightToWindowHeight(ch.font->getCharDescent());

      if (ch.type != CGnuPlotCharType::SUPERSCRIPT && ch.type != CGnuPlotCharType::SUBSCRIPT) {
        lbbox.add(CPoint2D(ch.pos.x    , ch.pos.y - fd));
        lbbox.add(CPoint2D(ch.pos.x + w, ch.pos.y + fa));
      }
    }

    // draw line
    double dx;

    if      (halign == CHALIGN_TYPE_LEFT)
      dx = bbox.getXMin();
    else if (halign == CHALIGN_TYPE_CENTER)
      dx = bbox.getXMid() - lbbox.getWidth()/2;
    else if (halign == CHALIGN_TYPE_RIGHT)
      dx = bbox.getXMax() - lbbox.getWidth();

    for (const auto &ch : l.chars) {
      renderer->setFont(ch.font);

      renderer->drawText(ch.pos + CPoint2D(dx, dy), ch.str, c);
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
  CFontPtr font     = origFont;

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

    if (family     == "") family     = font->getFamily();
    if (style      == "") style      = CFont::fontStyleToString(font->getStyle());
    if (multiplier == "") multiplier = "1";
    if (size       == "") size       = CStrUtil::toString(font->getSize());

    double fs = std::max(0.0, CStrUtil::toReal(size)*CStrUtil::toReal(multiplier));

    CFontPtr font1 = font->dup(family, CFont::stringToFontStyle(style), fs);

    renderer->setFont(font1);

    font = renderer->getFont();
  }

  if (type == CGnuPlotCharType::SUBSCRIPT || type == CGnuPlotCharType::SUPERSCRIPT) {
    std::string family = font->getFamily();
    CFontStyle  style  = font->getStyle();
    double      size   = font->getSize();

    CFontPtr font1 = font->dup(family, style, size*0.8);

    renderer->setFont(font1);

    font = renderer->getFont();
  }

  double dy = 0.0;

  if (type == CGnuPlotCharType::OVERPRINT) {
    double s = 0.0;

    if (estr_[0] == '.' || isdigit(estr_[0]))
      CStrUtil::readReal(estr_, &i, &s);

    //double o = renderer->getFont()->getCharAscent()*s;
    CGnuPlotTextLine &line = state.lines.back();

    double o = line.ascent*s;

    dy = renderer->pixelHeightToWindowHeight(o);
  }

  while (i < estr_.size()) {
    state.pos += CPoint2D(0, dy);

    placePartChars(state, i, type);

    state.pos -= CPoint2D(0, dy);

    CGnuPlotTextLine &line = state.lines.back();

    if (line.ascent <= 0)
      line.ascent = renderer->pixelHeightToWindowHeight(renderer->getFont()->getCharAscent());

    double fa = line.ascent;

    int i1 = i;

    (void) CUtf8::readNextChar(estr_, i1);

    std::string cs(&estr_[i], i1 - i);

    i = i1;

    CPoint2D pos = state.pos - CPoint2D(0, fa);

    CGnuPlotTextChar c(pos, renderer->getFont(), cs, type);

    state.lines.back().chars.push_back(c);

    double w = renderer->pixelWidthToWindowWidth(renderer->getFont()->getStringWidth(cs));

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

      if (ch.type != CGnuPlotCharType::SUPERSCRIPT && ch.type != CGnuPlotCharType::SUBSCRIPT) {
        lbbox.add(CPoint2D(ch.pos.x    , ch.pos.y - fd));
        lbbox.add(CPoint2D(ch.pos.x + w, ch.pos.y + fa));
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

  CFontPtr font = renderer->getFont();

  if      (type_ == CGnuPlotCharType::SUPERSCRIPT) {
    double h = renderer->pixelHeightToWindowHeight(font->getCharHeight()/2);

    state.pos += CPoint2D(0, h);

    text_.placeChars(state, type_);

    state.pos -= CPoint2D(0, h);
  }
  else if (type_ == CGnuPlotCharType::SUBSCRIPT) {
    double h = renderer->pixelHeightToWindowHeight(font->getCharHeight()/2);

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
    double w = renderer->pixelWidthToWindowWidth(font->getStringWidth(text_.etext()));

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
