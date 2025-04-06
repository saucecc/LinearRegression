#include "linear_regression.h"

/*
Least squares: want to minimize squared error between predicted and actual
output:
    - In class, this was presented as the element-wise minimum; in matrix form:
    - J(theta) = ||X(theta) - y||^2
    => theta = (X^TX)^-1 * X^Ty

*/
matrix linear_regession::train_least_squares_reg(matrix X, matrix y) {
    // least squares regression
    matrix Xt = X.transpose();
    matrix XtX = Xt.multiply(X);
    matrix XtX_inv = XtX.inverse();
    matrix Xty = Xt.multiply(y);
    matrix theta = XtX_inv.multiply(Xty);
}