#include "matrix.h"

matrix::matrix(std::string filepath) { std::ifstream file(filepath); }

matrix::matrix(std::vector<std::vector<double>> data) {
    this->data = data;
    this->m = data.size();
    this->n = data.at(0).size();
    for (int r = 0; r < m; r++) {
        if (n != data.at(r).size()) {
            std::cerr << "ERR; MALFORMED, JAGGED MATRIX" << std::endl;
        }
    }
}

matrix::matrix(SPECIAL t, int n) {
    this->data = std::vector<std::vector<double>>(n, std::vector<double>(n, 0));
    switch (t) {
        case ZERO:
            break;
        case IDENT:
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == j) {
                        this->data[i][j] = 1;
                    }
                }
            }
            break;
    }
}

matrix matrix::copy() { return matrix(this->data); }

/*
Transpose a matrix;

Time complexity: O(mn)
*/
matrix matrix::transpose() {
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
    return matrix(result);
}

/*
forward_elim: Helper for implementing forwards elimination w/ partial pivoting,
since this is used in multiple different operations

Note; we handle errors such as trying to do this on a singular matrix

Time Complexity:
    know: m == n
    => O(n^3)
*/
void matrix::forward_elim(std::vector<std::vector<double>>& do_same_to) {
    // keep track of the row with the largest leading entry for the current col
    std::pair<double, int> largest_leading;

    for (int d = 0; d < n; d++) {
        // swapping row for partial pivoting
        largest_leading = std::make_pair(std::abs(this->data[d][d]), d);
        for (int r = d; r < m; r++) {
            double r_leading_entry = this->data[r][d];
            // we use absolute value so that
            if (std::abs(r_leading_entry) > std::abs(largest_leading.first)) {
                largest_leading = std::make_pair(r_leading_entry, r);
            }
        }
        // exchange rows
        if (d != largest_leading.second) {
            std::swap(this->data[d], this->data[largest_leading.second]);
            std::swap(do_same_to[d], do_same_to[largest_leading.second]);
        }
        double pivot = this->data[d][d];
        double factor;

        if (std::abs(pivot) < 1e-12) {
            std::cerr
                << "[ERROR] Singular matrix: pivot is zero (or very close)\n";
        }

        std::vector<double>& curr_row = this->data[d];
        std::vector<double>& curr_ident_row = do_same_to[d];
        for (int r = d + 1; r < n; r++) {
            factor = this->data[r][d] / pivot;
            for (int i = d; i < n; i++) {
                this->data[r][i] -= curr_row[i] * factor;
                do_same_to[r][i] -= curr_ident_row[i] * factor;
            }
        }
    }
}

/*
For when we do not care about doing this to another matrix as well; just pass
garbage to implementation where we do care about duplicating the steps
*/
void matrix::forward_elim() {
    std::vector<std::vector<double>> garbage(m, std::vector<double>(n, 0));
    this->forward_elim(garbage);
}

/*
Invert a matrix

implemented via gaussian elimination w/ partial pivoting

step1: create the identity matrix
step2: forward elimination (logic within helper)
 - we do same steps to ident
step3: normalize pivots
step4: backward elimination
 - once again do same to ident

Note; we handle errors such as trying to do this on a singular matrix


Time Complexity: O(n^3); dominated by forward elimination
*/
matrix matrix::inverse() {
    if (m != n) {
        std::cerr << "[ERROR] Attempting to invert a non-square matrix\n";
        return matrix(IDENT, n);
    }

    // create ident matrix
    std::vector<std::vector<double>> ident(n, std::vector<double>(n, 0));
    for (int x = 0; x < n; x++) {
        ident[x][x] = 1;
    }

    // perform forward elimination on matrix, and ident
    this->forward_elim(ident);

    // normalizing the pivots (diag)
    for (int i = 0; i < n; i++) {
        double pivot = this->data[i][i];
        if (std::abs(pivot) < 1e-12) {
            std::cerr
                << "[ERROR] Singular matrix: pivot is zero (or very close)\n";
        }
        for (int j = 0; j < n; j++) {
            this->data[i][j] /= pivot;
            ident[i][j] /= pivot;
        }
    }

    // eliminate above the diagonal
    for (int d = n - 1; d >= 0; d--) {
        for (int r = d - 1; r >= 0; r--) {
            double factor = this->data[r][d];
            for (int c = 0; c < n; c++) {
                this->data[r][c] -= factor * this->data[d][c];
                ident[r][c] -= factor * ident[d][c];
            }
        }
    }
    return ident;  // ident now A^-1
}

/*
Printing out a matrix for debugging
*/
void matrix::print() {
    for (int x = 0; x < m; x++) {
        for (int y = 0; y < n; y++) {
            std::cout << this->data[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

/*
multiply: Multiplies this matrix (A) by another matrix (B)

Time Complexity: O(mnp)
*/
matrix matrix::multiply(matrix& mat) {
    int p = mat.get_cols();
    if (n != p) {
        std::cerr << "[MULT] ERR; MATRIX SIZES DO NOT MATCH" << std::endl;
        return matrix(ZERO, m);
    }
    std::vector<std::vector<double>> C(m, std::vector<double>(p, 0));
    std::vector<std::vector<double>> B = mat.get_data();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += this->data[i][k] * B[k][j];
            }
        }
    }
    return matrix(C);
}

/*
Helper so we do not duplicate code for addition and subtraction since they
are the same thing
*/
matrix matrix::arithmetic_helper(matrix& mat, bool sub) {
    int p = mat.get_rows();
    int q = mat.get_cols();
    if (m != p || n != q) {
        std::cerr << "[ADD] MATRIX DIMENSIONS DO NOT MATCH" << std::endl;
        return matrix(ZERO, p);
    }

    std::vector<std::vector<double>> res(m, std::vector<double>(n, 0));
    std::vector<std::vector<double>> B = mat.get_data();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double a_ij = this->data[i][j];
            res[i][j] = (sub) ? a_ij - B[i][j] : a_ij + B[i][j];
        }
    }
    return matrix(res);
}

matrix matrix::add_leading_col_ones() {
    std::vector<std::vector<double>> new_data(m,
                                              std::vector<double>(n + 1, 1.0));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            new_data[r][c + 1] = this->data[r][c];
        }
    }
    return matrix(new_data);
}

matrix matrix::add(matrix& mat) { return arithmetic_helper(mat, false); }

matrix matrix::subtract(matrix& mat) { return arithmetic_helper(mat, true); }

// returns a copy of a matrix's data
std::vector<std::vector<double>> matrix::get_data() { return this->data; }