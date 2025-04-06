#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
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
    matrix inverse();
    void print();
    int get_rows();
    int get_cols();
    std::vector<std::vector<double>> get_data();
    matrix add(matrix& mat);
    matrix subtract(matrix& mat);
    matrix add_leading_col_ones();

   private:
    std::vector<std::vector<double>> data;
    int n, m;
    void forward_elim(std::vector<std::vector<double>>& matrix);
    void forward_elim();
    matrix arithmetic_helper(matrix& mat, bool sub);
};

#endif