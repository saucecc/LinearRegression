#include "matrix.h"

class linear_regession {
   public:
    matrix train_least_squares_reg(matrix X, matrix y);

   private:
    matrix weights;
};