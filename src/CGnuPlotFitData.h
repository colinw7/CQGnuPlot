#ifndef CGnuPlotFitData_H
#define CGnuPlotFitData_H

struct CGnuPlotFitData {
  std::string logfile { false };
  bool        quiet { false };
  bool        results { false };
  bool        brief { false };
  bool        verbose { false };
  bool        errorvariables { false };
  bool        covariancevariables { false };
  bool        errorscaling { false };
  bool        prescale { false };
  int         maxiter { -1 };
  double      limit { 0 };
  double      limit_abs { 0 };
  double      start_lambda { 0 };
  double      lambda_factor { 0 };
  std::string script;
  int         version { 5 };
};

#endif
