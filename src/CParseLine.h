#ifndef CPARSE_LINE_H
#define CPARSE_LINE_H

#include <cassert>
#include <cstring>

class CParseLine {
 public:
  CParseLine(const std::string &line="") :
   str_(line), pos_(0), len_(str_.size()) {
  }

  void clear() {
    str_ = "";
    pos_ = 0;
    len_ = 0;
  }

  const std::string &str() const { return str_; }

  std::string substr() const { return substr(pos_); }

  std::string substr(int pos) const {
    assert(pos >= 0 && pos <= len_);

    return str_.substr(pos);
  }

  std::string substr(int pos, int len) const {
    assert(pos >= 0 && len >= 0 && pos + len <= len_);

    return str_.substr(pos, len);
  }

  int pos() const { return pos_; }

  void setPos(int pos) { pos_ = pos; }

  void incPos(int n) { pos_ += n; }

  void addChar(char c) {
    str_ += c;

    ++len_;
  }

  char lookChar() const { return str_[pos_]; }

  char getChar() { return str_[pos_++]; }

  char lookNextChar(int offset=1) {
    if (pos_ + offset <= len_)
      return str_[pos_ + offset];
    else
      return '\0';
  }

  void skipChar() { ++pos_; }

  bool isValid() const { return pos_ < len_; }

  void skipSpace() {
    while (pos_ < len_ && isSpace())
      ++pos_;
  }

  void skipNonSpace() {
    while (pos_ < len_ && ! isSpace())
      ++pos_;
  }

  std::string readNonSpace() {
    std::string str;

    while (pos_ < len_ && ! isSpace())
      str += str_[pos_++];

    return str;
  }

  bool isSpace() const {
    return isspace(str_[pos_]);
  }

  bool isDigit() const {
    return isdigit(str_[pos_]);
  }

  bool isAlpha() const {
    return isalpha(str_[pos_]);
  }

  bool isAlNum() const {
    return isalnum(str_[pos_]);
  }

  bool isChar(char c) const {
    return str_[pos_] == c;
  }

  bool isOneOf(const std::string &chars) const {
    return std::strchr(chars.c_str(), str_[pos_]) != 0;
  }

  bool isString(const std::string &str) const {
    int n = str.size();

    if (pos_ + n > len_) return false;

    return (strncmp(&str_[pos_], &str[0], n) == 0);
  }

  void insert(const std::string &str) {
    str_ = str_.substr(0, pos_) + str + str_.substr(pos_);

    len_ += str.size();
  }

 private:
  std::string str_;
  int         pos_;
  int         len_;
};

#endif
