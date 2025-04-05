#include "matrix.h"

matrix::matrix(std::string filepath) { std::ifstream file(filepath); }

matrix::matrix(std::vector<std::vector<double>> data) { this->data = data; }

matrix matrix::copy() { return matrix(this->data); }

/*
Transpose a matrix; 
*/
matrix matrix::transpose() {
    int m = this->data.size();
    int n = this->data.at(0).size();

    // if matrix was n x m becomes m x n
    std::vector<std::vector<double>> result(n);
    for (int x = 0; x < n; x++) {
        result[x].resize(m);
    }

    // actually transpose the matrix
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[j][i] = this->data[i][j];
        }
    }
    return result;
}

/*
Invert a matrix

implemented via gaussian elimination

*/
matrix matrix::inverse() {}