#ifndef CStrUniqueMatch_H
#define CStrUniqueMatch_H

template<typename T>
class CStrUniqueMatchValues {
 public:
  typedef std::pair<std::string, T> NameValue;

 public:
  CStrUniqueMatchValues() { }

  CStrUniqueMatchValues(std::initializer_list<NameValue> values) {
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

namespace CStrUniqueMatch {
  template<typename T>
  inline void initNameValues(CStrUniqueMatchValues<T> &) { }

  template<typename T>
  inline CStrUniqueMatchValues<T> &getNameValues() {
    static CStrUniqueMatchValues<T> nameValues;

    if (! nameValues.numValues())
      initNameValues<T>(nameValues);

    return nameValues;
  }

  template<typename T>
  inline bool stringToValue(const std::string &str, T &value) {
    return getNameValues<T>().match(str, value);
  }

  template<typename T>
  inline bool stringToValueWithErr(const std::string &str, T &value, const std::string &err) {
    if (! stringToValue(str, value)) {
      std::cerr << "Invalid " << err << " '" << str << "'" << std::endl;
      return false;
    }
    return true;
  }

  template<typename T>
  inline std::string valueToString(const T &value) {
    return getNameValues<T>().lookup(value);
  }

  template<typename T>
  inline std::string valueStrings() {
    std::vector<std::string> strs;

    getNameValues<T>().values(strs);

    std::string cstr;

    for (const auto &str : strs) {
      if (! cstr.empty()) cstr += ", ";

      cstr += "'" + str + "'";
    }

    return cstr;
  }
}

#endif
