#include <fstream>
#include <string>
#include <vector>

class matrix {
   public:
    matrix(std::string csv);
    matrix(std::vector<std::vector<double>> data);

   private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;
};