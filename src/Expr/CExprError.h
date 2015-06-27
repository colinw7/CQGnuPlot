#ifndef CExprError_H
#define CExprError_H

#define CEXPR_ERROR_ZERO_TO_NEG_POWER_UNDEF       1
#define CEXPR_ERROR_POWER_FAILED                  2
#define CEXPR_ERROR_REAL_TOO_BIG_FOR_INTEGER      3
#define CEXPR_ERROR_NAN_OPERATION                 4
#define CEXPR_ERROR_NON_INTEGER_POWER_OF_NEGATIVE 5
#define CEXPR_ERROR_DIVIDE_BY_ZERO                6

class CExprErrorData {
 public:
  CExprErrorData() :
   hasError_(false) {
  }

  void setLastError(const std::string &message) {
    if (message == "") {
      lastError_ = "";

      hasError_ = false;

      return;
    }

    if (hasError_)
      return;

    lastError_ = message;

    hasError_ = true;
  }

  bool isError() const {
    return hasError_;
  }

  const std::string &getLastError() const {
    return lastError_;
  }

 private:
  bool        hasError_;
  std::string lastError_;
};

#endif
