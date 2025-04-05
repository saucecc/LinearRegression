import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../output/results.csv", header=None, names=["x", "y", "y_pred"])

plt.figure(figsize=(8, 5))
plt.scatter(df["x"], df["y"], label="Actual", color="blue")
plt.plot(df["x"], df["y_pred"], label="Prediction", color="red")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.title("Linear Regression Fit")
plt.grid(True)
plt.tight_layout()
plt.show()
plt.savefig("../output/plot.png") 
