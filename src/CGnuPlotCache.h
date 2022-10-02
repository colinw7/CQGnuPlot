#ifndef CGnuPlotCache_H
#define CGnuPlotCache_H

#include <vector>

template<typename T>
class CGnuPlotCacheFactory {
 public:
  T *make() { return new T; }
};

template<typename T>
class CGnuPlotCache {
 public:
  typedef std::vector<T *>        Objects;
  typedef CGnuPlotCacheFactory<T> Factory;

 public:
  CGnuPlotCache(const Factory &factory=Factory()) :
   factory_(factory) {
  }

  const Objects &objects() const { return objects_; }

  void updateSize(int n) {
    int no = int(objects_.size());

    if (no > n) {
      for (int i = n; i < no; ++i)
        delete objects_[i];

      objects_.resize(n);

      no = n;
    }

    while (no < n) {
      objects_.push_back(factory_.make());

      ++no;
    }
  }

  void clear() {
    updateSize(0);
  }

  void reset() {
    for (auto &o : objects_)
      o->setUsed(false);
  }

 private:
  Factory factory_;
  Objects objects_;
};

#endif
