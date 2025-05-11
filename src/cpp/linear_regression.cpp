#include "linear_regression.h"

#include <cmath>

linear_regression::linear_regression(REGRESSION_TYPE tp, SOLVE_METHOD sm)
    : weights(ZERO, 5) {
    this->tp = tp;
    this->sm = sm;
}

/*
Least squares regression: want to minimize squared error between predicted and
actual output: From README.md:
    - theta = (X^TX)^-1 * X^TY

X - data
y - labels

we add the bias column, then compute X^T, X^T * X, (X^T * X)^-1,
and finally X^Ty = theta

This uses ridge regression
*/
void linear_regression::fit(matrix X, matrix y, double lambda) {
    if (this->tp == LEAST_SQUARES) {
        if (this->sm == INVERSE) {
            matrix X_new = X.add_leading_col_ones();  // X w/ bias
            matrix Xt = X_new.transpose();
            matrix XtX = Xt.multiply(X_new);

            // ridge regression
            matrix XtX_reg = XtX.add_identity_scaled(lambda);

            matrix XtX_inv = XtX_reg.inverse();
            matrix Xty = Xt.multiply(y);
            matrix theta = XtX_inv.multiply(Xty);
            std::cerr << "-------WEIGHTS-------" << std::endl;
            theta.head(theta.get_rows());
            this->weights = theta;
        } else if (this->sm == FACTORIZATION) {
            matrix X_new = X.add_leading_col_ones();  // X w/ bias

            matrix I = matrix(IDENT, X_new.get_cols());

            matrix sqrt_lambda_I = I.multiply(sqrt(lambda));

            matrix X_aug = X_new.row_stack(sqrt_lambda_I);

            std::cerr << "RAHH" << X_new.get_cols() << std::endl;

            matrix y_zeros(X_new.get_cols(), 1, 0.0);  // zeros
            matrix y_aug = y.row_stack(y_zeros);

            matrix Q(ZERO, 1), R(ZERO, 1);

            X_aug.qr_factorize(Q, R);
            matrix Qt = Q.transpose();
            matrix Qt_y = Qt.multiply(y_aug);
            matrix beta = R.back_substitute(Qt_y);
            this->weights = beta;
        }
    }
}

/*
add the bias column and multiply by weights to get the y predictions
*/
matrix linear_regression::predict(matrix X) {
    matrix X_new = X.add_leading_col_ones();
    return X_new.multiply(this->weights);
}