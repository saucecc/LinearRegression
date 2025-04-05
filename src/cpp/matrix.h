#include <fstream>
#include <string>
#include <vector>

class matrix {
   public:
    matrix(std::string csv);
    matrix(std::vector<std::vector<double>> data);
    matrix copy();
    matrix transpose();
    matrix multiply(matrix B);
    matrix inverse();

   private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;
};