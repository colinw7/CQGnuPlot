#ifndef CGnuPlotTicLabel_H
#define CGnuPlotTicLabel_H

struct CGnuPlotTicLabel {
  bool        valid;
  char        id;
  int         ind;
  std::string str;

  CGnuPlotTicLabel() {
    valid = false;
    id    = 'x';
    ind   = 1;
    str   = "";
  }
};

#endif
