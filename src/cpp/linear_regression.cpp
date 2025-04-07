#include "linear_regression.h"

linear_regression::linear_regression(REGRESSION_TYPE tp) : weights(ZERO, 5) {
    this->tp = tp;
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
void linear_regression::train(matrix X, matrix y, double lambda) {
    if (this->tp == LEAST_SQUARES) {
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
    }
}

/*
add the bias column and multiply by weights to get the y predictions
*/
matrix linear_regression::predict(matrix X) {
    matrix X_new = X.add_leading_col_ones();
    return X_new.multiply(this->weights);
}