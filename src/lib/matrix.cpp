#include "matrix.h"

matrix::matrix(std::string filepath) { std::ifstream file(filepath); }

matrix::matrix(std::vector<std::vector<double>> data) { this->data = data; }