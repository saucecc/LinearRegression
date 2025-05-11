#include "matrix.h"

#include <cmath>

matrix::matrix(std::string filepath) : data(0) {
    std::ifstream file(filepath);

    std::string line;
    std::getline(file, line);  // first line is col names

    bool skip = false;
    int cnt = 0;
    while (std::getline(file, line)) {
        skip = false;
        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            if (cell.empty()) {
                skip = true;
                break;
            }
            row.push_back(std::stod(cell));
        }

        if (!row.empty() && !skip) {
            this->data.push_back(row);
        }
        cnt++;
    }

    this->m = data.size();
    this->n = m > 0 ? data.at(0).size() : 0;
}

matrix::matrix(int rows, int cols, double val) {
    this->m = rows;
    this->n = cols;
    this->data =
        std::vector<std::vector<double>>(rows, std::vector<double>(cols, val));
}

matrix::matrix(std::vector<std::vector<double>> in_data) {
    this->data = in_data;
    this->m = data.size();
    this->n = data.at(0).size();
    for (int r = 0; r < m; r++) {
        if (n != data.at(r).size()) {
            std::cerr << "ERR; MALFORMED, JAGGED MATRIX" << std::endl;
        }
    }
}
matrix::matrix(SPECIAL t, int n) {
    if (n <= 0) {
        std::cerr << "[FATAL] Cannot create special matrix with n = " << n
                  << "\n";
        std::exit(1);
    }
    this->m = n;
    this->n = n;
    this->data = std::vector<std::vector<double>>(n, std::vector<double>(n, 0));
    if (t == IDENT) {
        for (int i = 0; i < n; i++) {
            data[i][i] = 1.0;
        }
    }
}

matrix matrix::copy() const { return matrix(this->data); }

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
        largest_leading = std::make_pair(-1.0, d);
        for (int r = d; r < m; r++) {
            double r_leading_entry = this->data[r][d];
            // we use absolute value so that
            if (std::abs(r_leading_entry) > std::abs(largest_leading.first)) {
                largest_leading = std::make_pair(std::abs(r_leading_entry), r);
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
        for (int r = d + 1; r < m; r++) {
            factor = this->data[r][d] / pivot;
            for (int i = d; i < n; i++) {
                this->data[r][i] -= curr_row[i] * factor;
                do_same_to[r][i] -= curr_ident_row[i] * factor;
            }
        }
    }
}

std::vector<double> get_column(std::vector<std::vector<double>>& m_data,
                               int j) {
    int m = m_data.size();
    std::vector<double> res(m);
    for (int i = 0; i < m; i++) {
        res[i] = m_data[i][j];
    }
    return res;
}

double dot_prod(std::vector<double> a, std::vector<double> b) {
    double r = 0;
    for (size_t i = 0; i < a.size(); i++) {
        r += a[i] * b[i];
    }
    return r;
}

double vect_norm(std::vector<double> v) {
    double s = 0;
    for (auto& val : v) {
        s += (val * val);
    }
    return sqrt(s);
}

matrix matrix::back_substitute(const matrix& y) const {
    int n = this->get_rows();
    std::vector<std::vector<double>> result_data(n,
                                                 std::vector<double>(1, 0.0));

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += this->data[i][j] * result_data[j][0];
        }

        double diag = this->data[i][i];
        if (std::abs(diag) < 1e-12) {
            std::cerr << "[ERROR] Singular matrix: zero on diagonal during "
                         "back substitution\n";
            diag = 1e-12;
        }

        result_data[i][0] = (y.data[i][0] - sum) / diag;
    }

    return matrix(result_data);
}

void matrix::qr_factorize(matrix& Q, matrix& R) const {
    std::vector<std::vector<double>> a_data = this->get_data();
    std::vector<std::vector<double>> q_data(m, std::vector<double>(n, 0.0));
    std::vector<std::vector<double>> r_data(n, std::vector<double>(n, 0.0));

    for (int j = 0; j < n; j++) {
        std::vector<double> v = get_column(a_data, j);

        for (int i = 0; i < j; i++) {
            double dot = dot_prod(get_column(q_data, i), v);

            r_data[i][j] = dot;
            for (int k = 0; k < m; k++) {
                v[k] -= dot * q_data[k][i];
            }
        }

        double norm = vect_norm(v);
        r_data[j][j] = norm;

        for (int k = 0; k < m; k++) {
            q_data[k][j] = v[k] / norm;
        }
    }

    Q = matrix(q_data);
    R = matrix(r_data);
}

double matrix::at(int i, int j) const { return this->data[i][j]; }

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
void matrix::print() const {
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
    int q = mat.get_cols();
    if (n != mat.get_rows()) {
        std::cerr << "[MULT] ERR; MATRIX SIZES DO NOT MATCH" << std::endl;
        return matrix(ZERO, m);
    }
    std::vector<std::vector<double>> C(m, std::vector<double>(q, 0));
    std::vector<std::vector<double>> B = mat.get_data();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += this->data[i][k] * B[k][j];
            }
        }
    }
    return matrix(C);
}

/*
Multiply a matrix by a constant
*/
matrix matrix::multiply(double c) {
    std::vector<std::vector<double>> res(m, std::vector<double>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            res[i][j] = this->data[i][j] * c;
        }
    }
    return matrix(res);
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
std::vector<std::vector<double>> matrix::get_data() const { return this->data; }

int matrix::get_cols() const { return this->n; }

int matrix::get_rows() const { return this->m; }

bool matrix::equals(const matrix& mat) const {
    const double EPSILON = 1e-6;
    int p = mat.get_rows();
    int q = mat.get_cols();
    std::vector<std::vector<double>> mat_data = mat.get_data();

    if (m != p || n != q) {
        return false;
    }

    for (int x = 0; x < m; x++) {
        for (int y = 0; y < n; y++) {
            if (std::abs(mat_data[x][y] - this->data[x][y]) > EPSILON) {
                return false;
            }
        }
    }
    return true;
}

/*
Takes a subset of the rows of a matrix [start_row, end_row)
*/
matrix matrix::row_subset(int start_row, int end_row) const {
    std::vector<std::vector<double>> new_data;
    for (int r = start_row; r < end_row; r++) {
        new_data.push_back(this->data[r]);
    }
    return matrix(new_data);
}

matrix matrix::row_stack(const matrix& other) const {
    std::vector<std::vector<double>> new_data = this->data;
    for (const auto& row : other.get_data()) {
        new_data.push_back(row);
    }
    return matrix(new_data);
}

matrix matrix::row_subset(int start_row) const {
    return this->row_subset(start_row, m);
}

/*

Uses min-max scaling to make features be within range [0,1]
*/
matrix matrix::min_max_scaled(int end_track_scale) const {
    std::vector<std::vector<double>> scaled_data(m, std::vector<double>(n));
    std::cerr << "-----------SCALING DEBUG-----------" << std::endl;

    for (int c = 0; c < n; c++) {
        double col_min = this->data[0][c];
        double col_max = this->data[0][c];

        // so we dont use scaling from test data
        for (int r = 0; r < end_track_scale; r++) {
            double v = this->data[r][c];
            if (v < col_min) col_min = v;
            if (v > col_max) col_max = v;
        }

        double range = col_max - col_min;
        if (range == 0) range = 1.0;

        // apply scaling
        for (int r = 0; r < m; r++) {
            scaled_data[r][c] = (this->data[r][c] - col_min) / range;
        }

        std::cerr << "col ind: " << c << " min: " << col_min
                  << " | max: " << col_max << std::endl;
    }

    return matrix(scaled_data);
}

/*
This helper returns the matrix without the column indexed (skip)

It also returns the values of the column extracted (labels)

*/
matrix matrix::extract_col(matrix& labels, int skip) {
    std::vector<std::vector<double>> res(m, std::vector<double>(n - 1));
    std::vector<std::vector<double>> label_data(m, std::vector<double>(1));
    for (int r = 0; r < m; r++) {
        int res_col = 0;
        for (int c = 0; c < n; c++) {
            if (c == skip) {
                label_data[r][0] = this->data[r][c];
            } else {
                res[r][res_col] = this->data[r][c];
                res_col++;
            }
        }
    }

    labels = matrix(label_data);
    return matrix(res);
}

/*
Prints out the first (rows) of the matrix
*/
void matrix::head(int rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < n; c++) {
            std::cerr << this->data[r][c] << " ";
        }
        std::cerr << std::endl;
    }
}

/*
This is for ridge regression;
*/
matrix matrix::add_identity_scaled(double lambda) const {
    if (m != n) {
        std::cerr << "[ERROR] Can only add identity to square matrix\n";
        return *this;
    }

    std::vector<std::vector<double>> result = this->data;

    for (int i = 0; i < n; i++) {
        result[i][i] += lambda;
    }

    return matrix(result);
}

double matrix::min_col(int col) const {
    double min_val = data[0][col];
    for (const auto& row : data) {
        min_val = std::min(min_val, row[col]);
    }
    return min_val;
}

double matrix::max_col(int col) const {
    double max_val = data[0][col];
    for (const auto& row : data) {
        max_val = std::max(max_val, row[col]);
    }
    return max_val;
}

void matrix::write_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open file for writing: " << filename
                  << "\n";
        return;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            file << this->data[i][j];
            if (j < n - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
    std::cout << "[INFO] Matrix written to CSV: " << filename << "\n";
}
