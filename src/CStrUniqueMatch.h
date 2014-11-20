#ifndef CStrUniqueMatch_H
#define CStrUniqueMatch_H

template<typename T>
class CStrUniqueMatch {
 public:
  CStrUniqueMatch() { }

  void addValue(const std::string &str, const T &value) {
    strValues_[str  ] = value;
    valueStrs_[value] = str;
  }

  int numValues() const { return strValues_.size(); }

  bool match(const std::string &str, T &value) {
    std::vector<T> partialValues;

    for (auto &v : strValues_) {
      if (v.first == str) {
        value = v.second;
        return true;
      }

      if (v.first.find(str) == 0)
        partialValues.push_back(v.second);
    }

    if (partialValues.size() != 1)
      return false;

    value = partialValues[0];

    return true;
  }

  const std::string &lookup(const T &value) {
    return valueStrs_[value];
  }

  void values(std::vector<std::string> &values) {
    for (auto &v : strValues_)
      values.push_back(v.first);
  }

 private:
  typedef std::map<std::string,T> StringValueMap;
  typedef std::map<T,std::string> ValueStringMap;

  StringValueMap strValues_;
  ValueStringMap valueStrs_;
};

#endif
