#include "dataset.h"

dataset::dataset(std::string filepath, double test_ratio, int label_ind,
                 bool min_max_scaling)
    : train_data(ZERO, 1),
      test_data(ZERO, 1),
      train_labels(ZERO, 1),
      test_labels(ZERO, 1),
      test_labels_unscaled(ZERO, 1) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "FAILED TO OPEN PASSED DATASET FILE\n";
        return;
    }

    std::string line;
    std::getline(file, line);  // first line is col names

    std::vector<std::string> cols;
    std::stringstream ss(line);
    std::string cell;

    int cnt = 0;
    std::string label_name;
    while (std::getline(ss, cell, ',')) {
        cell.erase(0, cell.find_first_not_of(" \t"));
        cell.erase(cell.find_last_not_of(" \t") + 1);
        if (cnt == label_ind) {
            label_name = cell;
        }
        cnt++;
        std::cerr << cell << " | ";
        cols.push_back(cell);
    }
    std::cerr << std::endl;
    this->cols = cols;
    std::cerr << " label ind: " << label_ind << std::endl;

    matrix data = matrix(filepath);
    std::cerr << "---------UNSCALED DATA HEAD (15 rows):---------" << std::endl;
    data.head(15);

    matrix labels(ZERO, 5);
    data = data.extract_col(labels, label_ind);
    int train_end = std::floor((1 - test_ratio) * data.get_rows());

    if (min_max_scaling) {
        data = data.min_max_scaled(train_end);
    }
    this->train_data = data.row_subset(0, train_end);
    this->test_data = data.row_subset(train_end + 1);
    this->label_col = label_col;
    this->train_labels = labels.row_subset(0, train_end);
    this->test_labels = labels.row_subset(train_end + 1);

    if (min_max_scaling) {
        this->y_min = train_labels.min_col(0);
        this->y_max = train_labels.max_col(0);
        double y_range = y_max - y_min;
        if (y_range == 0) y_range = 1.0;

        // Scale train labels
        auto train_label_data = train_labels.get_data();
        for (int i = 0; i < train_label_data.size(); i++) {
            double v = train_label_data[i][0];
            train_label_data[i][0] = (v - y_min) / y_range;
        }
        this->train_labels = matrix(train_label_data);
        this->test_labels_unscaled = test_labels;
        // Scale test labels
        auto test_label_data = test_labels.get_data();
        for (int i = 0; i < test_label_data.size(); i++) {
            double v = test_label_data[i][0];
            test_label_data[i][0] = (v - y_min) / y_range;
        }
        this->test_labels = matrix(test_label_data);
    }
}

matrix dataset::get_unscaled_test_labels() const {
    return this->test_labels_unscaled;
}

/*
To take scaled predictions back to unscaled predictions
*/
matrix dataset::unscale_labels(const matrix& scaled_preds) const {
    double y_range = y_max - y_min;
    if (y_range == 0) y_range = 1.0;

    auto pred_data = scaled_preds.get_data();  // assume column vector
    for (int i = 0; i < pred_data.size(); i++) {
        double scaled = pred_data[i][0];
        pred_data[i][0] = scaled * y_range + y_min;
    }

    return matrix(pred_data);
}

matrix dataset::get_train_data() const { return this->train_data; }
matrix dataset::get_test_data() const { return this->test_data; }
matrix dataset::get_train_labels() const { return this->train_labels; }
matrix dataset::get_test_labels() const { return this->test_labels; }

void dataset::print_cols() {
    for (std::string c_name : this->cols) {
        std::cerr << c_name << " ";
    }
    std::cerr << std::endl;
}

std::vector<std::string> dataset::get_cols() { return this->cols; }
