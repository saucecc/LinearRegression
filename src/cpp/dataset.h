#include <cmath>

#include "matrix.h"

class dataset {
   public:
    dataset(std::string filepath, double test_ratio, int label_col,
            bool min_max_scaling);
    matrix get_train_data() const;
    matrix get_test_data() const;
    std::vector<std::string> get_cols();
    void print_cols();
    matrix get_train_labels() const;
    matrix get_test_labels() const;
    matrix unscale_labels(const matrix& scaled_preds) const;
    matrix get_unscaled_test_labels() const;

   private:
    matrix train_data;
    matrix test_data;
    matrix train_labels;
    matrix test_labels;
    matrix test_labels_unscaled;
    std::vector<std::string> cols;
    double test_ratio;
    std::string label_col;
    double y_min;
    double y_max;
    std::vector<std::pair<double, double>> feature_min_max;
};