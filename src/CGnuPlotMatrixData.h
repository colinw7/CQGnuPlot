#ifndef CGnuPlotMatrixData_H
#define CGnuPlotMatrixData_H

class CGnuPlotMatrixData {
 public:
  CGnuPlotMatrixData() { }

  bool isMatrix() const { return matrix_; }
  void setMatrix(bool m) { matrix_ = m; }

  bool isRowHeaders() const { return rowheaders_; }
  void setRowHeaders(bool b) { rowheaders_ = b; }

  bool isColumnHeaders() const { return columnheaders_; }
  void setColumnHeaders(bool b) { columnheaders_ = b; }

  bool isPixels() const { return pixels_; }
  void setPixels(bool b) { pixels_ = b; }

  void reset() {
    matrix_        = false;
    rowheaders_    = false;
    columnheaders_ = false;
    pixels_        = false;
  }

 private:
  bool matrix_        { false };
  bool rowheaders_    { false };
  bool columnheaders_ { false };
  bool pixels_        { false };
};

#endif
