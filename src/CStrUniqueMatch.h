#ifndef CStrUniqueMatch_H
#define CStrUniqueMatch_H

template<typename T>
class CStrUniqueMatch {
 public:
  typedef std::pair<std::string, T> NameValue;

 public:
  CStrUniqueMatch() { }

  CStrUniqueMatch(std::initializer_list<NameValue> values) {
    addValues(values);
  }

  void addValues(std::initializer_list<NameValue> values) {
    for (const auto &v : values)
      addValue(v);
  }

  void addValue(const NameValue &nameValue) {
    addValue(nameValue.first, nameValue.second);
  }

  void addValue(const std::string &str, const T &value) {
    // name must match unique value but value can have many names
    strValues_[str] = value;

    valueStrs_[value].push_back(str);
  }

  int numValues() const { return strValues_.size(); }

  bool match(const std::string &str, T &value) {
    std::set<T> partialValues;

    for (auto &v : strValues_) {
      if (v.first == str) {
        value = v.second;
        return true;
      }

      if (v.first.find(str) == 0)
        partialValues.insert(v.second);
    }

    if (partialValues.size() != 1)
      return false;

    value = *partialValues.begin();

    return true;
  }

  const std::string &lookup(const T &value) {
    return *valueStrs_[value].begin();
  }

  void values(std::vector<std::string> &values) {
    for (auto &v : strValues_)
      values.push_back(v.first);
  }

 private:
  typedef std::map<std::string,T>  StringValueMap;
  typedef std::vector<std::string> Strings;
  typedef std::map<T,Strings>      ValueStringsMap;

  StringValueMap  strValues_;
  ValueStringsMap valueStrs_;
};

#endif
