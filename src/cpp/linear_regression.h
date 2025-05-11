#include "matrix.h"

enum REGRESSION_TYPE { LEAST_SQUARES = 0 };
enum SOLVE_METHOD { INVERSE = 0, FACTORIZATION = 1 };

class linear_regression {
   public:
    linear_regression(REGRESSION_TYPE tp, SOLVE_METHOD sm);
    void fit(matrix X, matrix y, double lambda);
    matrix predict(matrix X);

   private:
    matrix weights;
    REGRESSION_TYPE tp;
    SOLVE_METHOD sm;
};