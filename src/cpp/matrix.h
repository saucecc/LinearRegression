#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum SPECIAL { ZERO = 0, IDENT = 1 };

class matrix {
   public:
    matrix(std::string csv);
    matrix(std::vector<std::vector<double>> data);
    matrix(SPECIAL t, int n);

    matrix copy();
    matrix transpose();
    matrix multiply(matrix& B);
    matrix multiply(double c);
    matrix inverse();
    void print() const;
    int get_rows() const;
    int get_cols() const;
    std::vector<std::vector<double>> get_data() const;
    matrix add(matrix& mat);
    matrix subtract(matrix& mat);
    matrix add_leading_col_ones();
    bool equals(const matrix& mat) const;
    matrix row_subset(int start_row, int end_row) const;
    matrix row_subset(int start_row) const;
    matrix min_max_scaled(int end_track_scale) const;
    matrix extract_col(matrix& labels, int ind);
    void head(int rows);
    matrix add_identity_scaled(double lambda) const;
    double at(int i, int j) const;
    double min_col(int col) const;
    double max_col(int col) const;

   private:
    std::vector<std::vector<double>> data;
    int n, m;
    void forward_elim(std::vector<std::vector<double>>& matrix);
    void forward_elim();
    matrix arithmetic_helper(matrix& mat, bool sub);
};

#endif