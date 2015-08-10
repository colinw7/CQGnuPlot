#ifndef CGnuPlotBlock_H
#define CGnuPlotBlock_H

class CGnuPlotBlock {
 public:
  CGnuPlotBlock(const std::string &name) :
   name_(name) {
  }

  const std::string &name() { return name_; }

  const std::string &str() { return str_; }

  void clear() {
    str_ = "";
  }

  void addString(const std::string &str) {
    str_ += str;
  }

 private:
  std::string name_;
  std::string str_;
};

#endif
