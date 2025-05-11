#include <cmath>
#include <iostream>

#include "linear_regression.h"
#include "matrix.h"

using matrix_data = std::vector<std::vector<double>>;

void header(std::string header) {
    std::cerr << "------------------" << header << "------------------"
              << std::endl;
}

bool compare_within_epsilon(const matrix& preds, const matrix& expected,
                            double epsilon = 1e-3) {
    int rows = preds.get_rows();
    int cols = preds.get_cols();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = std::abs(preds.at(i, j) - expected.at(i, j));
            if (diff > epsilon) {
                std::cerr << "Mismatch at (" << i << "," << j << "): "
                          << "predicted = " << preds.at(i, j)
                          << ", expected = " << expected.at(i, j)
                          << ", diff = " << diff << std::endl;
                return false;
            }
        }
    }
    return true;
}

void assert_equals(const matrix& mat1, const matrix& mat2, std::string tname) {
    if (!mat1.equals(mat2)) {
        if (!tname.empty()) {
            std::cerr << "FAILED " << tname << " TEST" << std::endl;
        }
        std::cerr << "EXPECTED: \n";
        mat1.print();
        std::cerr << "\nGOT: \n";
        mat2.print();
        std::cerr << std::endl;
        exit(1);
    }
    if (!tname.empty()) {
        std::cerr << "PASSED " << tname << " TEST" << std::endl;
    }
}

void test_simple_linear_regression(SOLVE_METHOD sm) {
    matrix_data X_data = {
        {1, 2},
        {2, 1},
        {3, 0},
        {0, 3},
    };

    matrix_data y_data = {
        {8},
        {7},
        {6},
        {9},
    };

    matrix X(X_data);
    matrix y(y_data);

    std::cerr << "Simple linear regression using "
              << ((sm == INVERSE) ? "inverse" : "LU factorization")
              << " to solve\n";

    linear_regression lr = linear_regression(LEAST_SQUARES, sm);
    lr.fit(X, y, 1e-3);

    matrix preds = lr.predict(X);
    std::cerr << "---- PREDICTIONS ----" << std::endl;
    preds.print();

    std::cerr << "---- EXPECTED ----" << std::endl;
    y.print();

    if (compare_within_epsilon(preds, y)) {
        std::cout << "PASSED: predictions within epsilon of expected"
                  << std::endl;
    } else {
        std::cout << "FAILED: predictions deviate beyond epsilon of expected"
                  << std::endl;
    }
}

void test_qr_factorization() {
    // Input matrix A (3x3)
    std::vector<std::vector<double>> qr_data{
        {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}};
    matrix A(qr_data);

    // Output placeholders
    matrix Q(IDENT, 1);  // will be resized
    matrix R(IDENT, 1);  // will be resized

    A.qr_factorize(Q, R);

    std::cerr << "Q:\n";
    Q.print();
    std::cerr << "\nR:\n";
    R.print();

    // Test 1: Q^T Q ≈ I
    matrix Qt = Q.transpose();
    matrix QtQ = Qt.multiply(Q);
    matrix I(QtQ.get_data());  // identity matrix

    if (!QtQ.equals(I)) {
        std::cerr << "FAILED: Q^T * Q != I (Q not orthonormal)\n";
        QtQ.print();
    } else {
        std::cerr << "PASSED: Q is orthonormal (Q^T Q ≈ I)\n";
    }

    // Test 2: Q * R ≈ A
    matrix QR = Q.multiply(R);
    if (!QR.equals(A)) {
        std::cerr << "FAILED: Q * R != A\n";
        QR.print();
    } else {
        std::cerr << "PASSED: Q * R ≈ A\n";
    }
}

int main() {
    matrix_data small_mat_data = {{2, 5}, {6, 9}};
    matrix_data small_mat_t_data = {{2, 6}, {5, 9}};
    matrix small_mat = matrix(small_mat_data);
    matrix small_mat_t = matrix(small_mat_t_data);

    matrix_data small_mat_inv_data = {{9.0, -5}, {-6, 2}};
    // manually calculated determinant
    matrix small_mat_inv = matrix(small_mat_inv_data).multiply((1.0 / -12.0));

    header("RUNNING MATRIX TESTS");
    matrix small_mat_tt = small_mat.transpose();
    assert_equals(small_mat_t, small_mat_tt, "SMALL TRANSPOSE");
    matrix small_inv_tt = small_mat.inverse();
    assert_equals(small_mat_inv, small_inv_tt, "SMALL INVERSE");

    // ADDITION TEST
    matrix_data A_data = {{1, 2}, {3, 4}};
    matrix_data B_data = {{5, 6}, {7, 8}};
    matrix_data sum_data = {{6, 8}, {10, 12}};

    matrix A(A_data);
    matrix B(B_data);
    matrix expected_sum(sum_data);

    assert_equals(A.add(B), expected_sum, "2x2 ADDITION");

    matrix_data diff_data = {{-4, -4}, {-4, -4}};
    matrix expected_diff(diff_data);

    // SUBTRACTION TEST
    assert_equals(A.subtract(B), expected_diff, "2x2 SUBTRACTION");

    // MULTIPLICATION TEST

    matrix_data product_data = {{19, 22}, {43, 50}};
    matrix expected_product(product_data);
    assert_equals(A.multiply(B), expected_product, "2x2 MULTIPLICATION");

    // rectangular
    matrix_data K_data = {
        {1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};

    matrix_data M_data = {
        {1, 0, 2}, {0, 1, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    matrix_data expected_KM_data = {
        {91, 104, 122}, {201, 229, 282}, {311, 354, 442}};

    matrix K(K_data);
    matrix M(M_data);
    matrix expected_KM(expected_KM_data);

    assert_equals(expected_KM, K.multiply(M),
                  "3x5 * 5x3 RECTANGULAR MULTIPLICATION");

    // ADDING LEADING COL OF ONES:
    matrix_data with_ones_data = {{1, 1, 2}, {1, 3, 4}};
    matrix expected_with_ones(with_ones_data);

    assert_equals(A.add_leading_col_ones(), expected_with_ones,
                  "ADD LEADING COL OF ONES");

    header("RUNNING SIMPLE LINEAR REGRESSION (INVERSE) TEST");
    test_simple_linear_regression(INVERSE);
    header("RUNNING SIMPLE LINEAR REGRESSION (LU FACTORIZATION) TEST");
    test_simple_linear_regression(FACTORIZATION);

    // testing QR factorization
    test_qr_factorization();
}