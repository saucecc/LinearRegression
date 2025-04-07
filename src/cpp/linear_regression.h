#include "matrix.h"

enum REGRESSION_TYPE { LEAST_SQUARES = 0 };

class linear_regression {
   public:
    linear_regression(REGRESSION_TYPE tp);
    void train(matrix X, matrix y, double lambda);
    matrix predict(matrix X);

   private:
    matrix weights;
    REGRESSION_TYPE tp;
};