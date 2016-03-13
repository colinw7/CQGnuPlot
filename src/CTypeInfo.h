#ifndef CTYPEINFO_H
#define CTYPEINFO_H

#include <typeinfo>
#include <cassert>

class CTypeInfo {
 public:
  // Constructors
  CTypeInfo(); // needed for containers
  CTypeInfo(const std::type_info &ti); // non-explicit

  // Access for the wrapped std::type_info
  const std::type_info& get() const;

  // Compatibility functions
  bool before(const CTypeInfo &rhs) const;

  const char* name() const;

 private:
  const std::type_info *pInfo_;
};

// Implementation
// inline means create single instance for linker

inline
CTypeInfo::
CTypeInfo() {
  class Nil {};

  pInfo_ = &typeid(Nil);

  assert(pInfo_);
}

inline
CTypeInfo::
CTypeInfo(const std::type_info &ti) :
 pInfo_(&ti) {
  assert(pInfo_);
}

inline bool
CTypeInfo::
before(const CTypeInfo& rhs) const {
  assert(pInfo_);

  // type_info::before return type is int in some VC libraries
  return pInfo_->before(*rhs.pInfo_) != 0;
}

inline const std::type_info &
CTypeInfo::
get() const {
  assert(pInfo_);

  return *pInfo_;
}

inline const char*
CTypeInfo::
name() const {
  assert(pInfo_);

  return pInfo_->name();
}

// Comparison operators

inline bool
operator==(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return (lhs.get() == rhs.get()) != 0;
}

inline bool
operator<(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return lhs.before(rhs);
}

inline bool
operator!=(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return !(lhs == rhs);
}

inline bool
operator>(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return rhs < lhs;
}

inline bool
operator<=(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return !(lhs > rhs);
}

inline bool
operator>=(const CTypeInfo& lhs, const CTypeInfo& rhs) {
  return !(lhs < rhs);
}

#endif
