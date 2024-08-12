import numpy as np
from sklearn.datasets import load_iris
from sklearn.cluster import k_means
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

iris = load_iris()

inertia_arr = []

for k in range(1, 11):
    centroids, labels, _ = k_means(iris.data, k, random_state=0, init="k-means++")
    inertia = 0
    for i in range(len(iris.data)):
        inertia += np.power(np.linalg.norm(centroids[labels[i]] - iris.data[i]), 2)
    inertia_arr.append(inertia)

figure, axes = plt.subplots()
axes.plot(
    2.5,
    (inertia_arr[1] + inertia_arr[2]) / 2,
    marker="o",
    fillstyle="none",
    markersize=60,
)
plt.annotate(
    "Elbow Point",
    xy=(3.2, 185),
    xytext=(5, 400),
    arrowprops={"arrowstyle": "->"},
)

plt.plot(range(1, 11), inertia_arr)

plt.title("Elbow Method for selection of optimal K clusters")
plt.xlabel("K")
plt.ylabel("Inertia")

plt.savefig("elbow.png")
