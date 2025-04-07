import pandas as pd
import matplotlib.pyplot as plt
from sklearn.metrics import r2_score, mean_squared_error, mean_absolute_error
import subprocess

def plot_predictions_vs_actual(pred_path, actual_path, title="Predicted vs Actual", limit=None, k=1):
    # Load CSVs
    y_pred = pd.read_csv(pred_path, header=None)[0].values
    y_true = pd.read_csv(actual_path, header=None)[0].values

    # Optionally limit data
    if limit is not None:
        y_pred = y_pred[:limit]
        y_true = y_true[:limit]

    r2 = r2_score(y_true, y_pred)
    mse = mean_squared_error(y_true, y_pred)
    mae = mean_absolute_error(y_true, y_pred)

    plt.figure(figsize=(8, 6))
    plt.scatter(y_true, y_pred, alpha=0.5, label="Predictions")
    plt.plot([min(y_true), max(y_true)], [min(y_true), max(y_true)], 'r--', label="Ideal")
    plt.xlabel("Actual")
    plt.ylabel("Predicted")
    plt.title(f"{title} (R² = {r2:.2f})")
    plt.legend()
    plt.grid(True)

    textstr = f"MAE: {mae:.2f}\nMSE: {mse:.2f}\nR²: {r2:.2f}"
    plt.gcf().text(0.7, 0.2, textstr, fontsize=10, bbox=dict(facecolor='white', edgecolor='black'))

    plt.tight_layout()

    plt.show()
    plt.savefig("plot-" + str(k) + ".png")

print("runnning c++ tests from python")
result_t = subprocess.run(["./bin/test"], capture_output=True, text=True)
print(result_t.stdout)

print("runnning c++ main code from python")
result = subprocess.run(["./bin/main"], capture_output=True, text=True)
print(result.stdout)

plot_predictions_vs_actual("data/output/salary-pred.csv", "data/output/salary-actual.csv", "Salary Predicted vs Actual", limit = 8, k=1)
plot_predictions_vs_actual("data/output/bev-sales-pred.csv", "data/output/bev-sales-actual.csv", "Beverage Sales Predicted vs Actual", limit = 300, k=2)
plot_predictions_vs_actual("data/output/cali-housing-pred.csv", "data/output/cali-housing-actual.csv", "California Housing Block Prices Predicted vs Actual", limit = 1000, k=3)

