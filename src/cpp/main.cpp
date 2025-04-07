#include <limits.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

#include "dataset.h"
#include "linear_regression.h"
#include "matrix.h"

void evaluate_predictions(const matrix& predicted, const matrix& actual,
                          std::string tag) {
    int n = predicted.get_rows();

    if (n != actual.get_rows()) {
        std::cerr << "[ERROR] Prediction and actual label matrices have "
                     "different sizes!\n";
        return;
    }

    double mae = 0.0;
    double mse = 0.0;
    double sum_actual = 0.0;
    double sum_sq_total = 0.0;

    // compute mean of actual values
    for (int i = 0; i < n; i++) {
        sum_actual += actual.at(i, 0);
    }
    double mean_actual = sum_actual / n;

    for (int i = 0; i < n; i++) {
        double y_pred = predicted.at(i, 0);
        double y_true = actual.at(i, 0);
        double diff = y_pred - y_true;

        mae += std::abs(diff);
        mse += diff * diff;
        sum_sq_total += (y_true - mean_actual) * (y_true - mean_actual);
    }

    mae /= n;
    mse /= n;
    double rmse = std::sqrt(mse);

    // R² = 1 - (SS_res / SS_tot)
    double r2 = 1.0 - (mse * n / sum_sq_total);

    std::cout << "\n" << tag << " Evaluation Metrics:\n";
    std::cout << " - MAE  = " << mae << "\n";
    std::cout << " - MSE  = " << mse << "\n";
    std::cout << " - RMSE = " << rmse << "\n";
}

void print_predictions_vs_actual(const matrix& predictions,
                                 const matrix& actual, int limit = 20) {
    int n = predictions.get_rows();

    if (n != actual.get_rows()) {
        std::cerr << "[ERROR] Prediction and actual label matrices have "
                     "different sizes!\n";
        return;
    }

    std::cout << "Row\tPredicted\tActual\t\t(Error)\n";
    std::cout << "----------------------------------------------\n";

    for (int i = 0; i < std::min(n, limit); i++) {
        double pred = predictions.at(i, 0);
        double act = actual.at(i, 0);
        double diff = std::abs(pred - act);

        std::cout << i << "\t" << std::fixed << std::setprecision(2) << pred
                  << "\t\t" << act << "\t\t" << diff << "\n";
    }
}

void salary_reg() {
    std::cerr << "SALARY DATA (SIMPLE) LINEAR REGRESSION (ONE VAR)";
    dataset salary_data =
        dataset("data/input/Salary_dataset2.csv", .30, 1, true);

    salary_data.print_cols();
    std::cerr << "------------TRAIN DATA HEAD (15 rows)------------\n";
    matrix X_train = salary_data.get_train_data();
    std::cerr << X_train.get_rows() << " rows\n";
    X_train.head(15);
    std::cerr << "------------TRAIN LABELS SCALED HEAD (15 rows)------------\n";
    matrix y_train = salary_data.get_train_labels();
    y_train.head(15);
    std::cerr << "------------TEST DATA HEAD (15 rows)------------\n";
    matrix X_test = salary_data.get_test_data();
    std::cerr << X_test.get_rows() << " rows\n";
    X_test.head(8);
    std::cerr << "------------TEST LABELS UNSCALED (NOT USED IN LR MODEL) "
                 "------------\n";
    matrix y_test = salary_data.get_unscaled_test_labels();
    y_test.head(8);

    linear_regression lr = linear_regression(LEAST_SQUARES);
    lr.train(X_train, y_train, 10);

    std::cerr << "------------PREDICTED LABELS HEAD (15 rows)------------\n";
    matrix y_pred = lr.predict(X_test);
    y_pred.head(8);

    std::cerr
        << "------------PREDICTED LABELS UNSCALED HEAD (15 rows)------------\n";
    matrix y_pred_unscaled = salary_data.unscale_labels(y_pred);
    // y_pred_unscaled.print();
    print_predictions_vs_actual(y_pred_unscaled, y_test, 20);
    evaluate_predictions(y_pred_unscaled, y_test, "SALARY_DATA");
    y_pred_unscaled.write_to_csv("data/output/salary-pred.csv");
    y_test.write_to_csv("data/output/salary-actual.csv");
}

void bev_sales_reg() {
    std::cerr << "BEVERAGE SALES DATA LINEAR REGRESSION" << std::endl;
    dataset bev_sales = dataset("data/input/beverage_sales.csv", .30, 2, true);

    bev_sales.print_cols();
    std::cerr << "------------TRAIN DATA HEAD (15 rows)------------\n";
    matrix X_train = bev_sales.get_train_data();
    std::cerr << X_train.get_rows() << " rows\n";
    X_train.head(15);
    std::cerr << "------------TRAIN LABELS HEAD (15 rows)------------\n";
    matrix y_train = bev_sales.get_train_labels();
    y_train.head(15);
    std::cerr << "------------TEST DATA HEAD (15 rows)------------\n";
    matrix X_test = bev_sales.get_test_data();
    std::cerr << X_test.get_rows() << " rows\n";
    X_test.head(8);
    std::cerr
        << "------------TEST LABELS (NOT USED IN LR MODEL) ------------\n";
    matrix y_test = bev_sales.get_unscaled_test_labels();
    y_test.head(8);

    linear_regression lr = linear_regression(LEAST_SQUARES);
    lr.train(X_train, y_train, 1);

    std::cerr << "------------PREDICTED LABELS HEAD (15 rows)------------\n";
    matrix y_pred = lr.predict(X_test);
    y_pred.head(8);

    std::cerr
        << "------------PREDICTED LABELS UNSCALED HEAD (15 rows)------------\n";
    matrix y_pred_unscaled = bev_sales.unscale_labels(y_pred);
    y_pred_unscaled.head(15);
    print_predictions_vs_actual(y_pred_unscaled, y_test, 20);
    evaluate_predictions(y_pred_unscaled, y_test, "BEVERAGE_SALES");
    y_pred_unscaled.write_to_csv("data/output/bev-sales-pred.csv");
    y_test.write_to_csv("data/output/bev-sales-actual.csv");
}

void cali_housing_reg() {
    std::cerr << "Loading cali housing data..." << std::endl;
    dataset cali_housing_data =
        dataset("data/input/cali-housing-encoding2.csv", .20, 9, true);

    cali_housing_data.print_cols();
    std::cerr << "------------TRAIN DATA HEAD (15 rows)------------\n";
    matrix X_train = cali_housing_data.get_train_data();
    std::cerr << X_train.get_rows() << " rows\n";
    X_train.head(15);
    std::cerr << "------------TRAIN LABELS HEAD (15 rows)------------\n";
    matrix y_train = cali_housing_data.get_train_labels();
    y_train.head(15);
    std::cerr << "------------TEST DATA HEAD (15 rows)------------\n";
    matrix X_test = cali_housing_data.get_test_data();
    std::cerr << X_test.get_rows() << " rows\n";
    X_test.head(15);
    std::cerr
        << "------------TEST LABELS (NOT USED IN LR MODEL) ------------\n";
    matrix y_test = cali_housing_data.get_unscaled_test_labels();
    y_test.head(15);

    linear_regression lr = linear_regression(LEAST_SQUARES);
    lr.train(X_train, y_train, 100);

    std::cerr << "------------PREDICTED LABELS HEAD (15 rows)------------\n";
    matrix y_pred = lr.predict(X_test);
    y_pred.head(15);

    std::cerr
        << "------------PREDICTED LABELS UNSCALED HEAD (15 rows)------------\n";
    matrix y_pred_unscaled = cali_housing_data.unscale_labels(y_pred);
    y_pred_unscaled.head(15);
    evaluate_predictions(y_pred_unscaled, y_test, "CALI_HOUSING_PRICE");
    y_pred_unscaled.write_to_csv("data/output/cali-housing-pred.csv");
    y_test.write_to_csv("data/output/cali-housing-actual.csv");
}

int main(int argc, char* argv[]) {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    std::cout << "running linear regression on Salary" << std::endl;
    salary_reg();

    // std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "running linear regression on Beverage Sales Dataset"
              << std::endl;

    bev_sales_reg();

    // std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "running linear regression on California Housing Dataset"
              << std::endl;

    cali_housing_reg();
}