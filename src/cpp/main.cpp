#include <iostream>

#include "matrix.h"

int main(int argc, char* argv[]) {
    std::cout << "hello world!" << std::endl;

    matrix X = matrix(IDENT, 5);  // m x (n - 1)
    // matrix y;                     // m x 1

    // least squares regression
    matrix Xt = X.transpose();
    matrix XtX = Xt.multiply(X);
    matrix XtX_inv = XtX.inverse();
    matrix Xty = Xt.multiply(y);
    matrix theta = XtX_inv.multiply(Xty);
}