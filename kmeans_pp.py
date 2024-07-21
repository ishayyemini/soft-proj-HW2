import sys

import numpy as np
import pandas as pd


def parse_int(num):
    try:
        return int(float(num)) if int(float(num)) == float(num) else 0
    except:
        return 0


def parse_float(num):
    try:
        return float(num)
    except:
        return -1


np.random.seed(1234)

error_msg = "An Error Has Occurred"

if len(sys.argv) < 5:
    print(error_msg)
    sys.exit(1)

try:
    k = parse_int(sys.argv[1])
    iter_num = parse_int(sys.argv[2]) if len(sys.argv) >= 6 else 300
    eps = parse_float(sys.argv[3 if len(sys.argv) >= 6 else 2])
    path_to_first_file = sys.argv[4 if len(sys.argv) >= 6 else 3]
    path_to_second_file = sys.argv[5 if len(sys.argv) >= 6 else 4]

    if not (1 < iter_num < 1000):
        error_msg = "Invalid maximum iteration!"
        sys.exit(1)

    if not (eps >= 0):
        error_msg = "Invalid epsilon!"
        sys.exit(1)

    df1 = pd.read_csv(path_to_first_file, header=None)
    df2 = pd.read_csv(path_to_second_file, header=None)

    if len(df1) != len(df2):
        sys.exit(1)

    if not (1 < k < len(df1)):
        error_msg = "Invalid number of clusters!"
        sys.exit(1)

    points = df1.merge(df2, on=0, how="inner")
    points.sort_values(by=0, inplace=True)
    points.columns = ["key"] + [i for i in range(len(points.columns) - 1)]
    points["key"] = pd.to_numeric(points["key"], downcast="integer")
    points.set_index("key", inplace=True)

    c_indices = np.random.choice(len(points), size=1).tolist()
    points["D"] = [0.0 for _ in range(len(points))]

    for i in range(1, k):
        points["D"] = points.apply(
            lambda x: min(
                np.linalg.norm(x[:-1] - points.iloc[i][:-1]) for i in c_indices
            ),
            axis=1,
        )
        next_point = np.random.choice(
            len(points),
            size=1,
            p=points["D"] / sum(points["D"]),
        ).tolist()[0]
        c_indices.append(next_point)

    centroids = [points.iloc[i].tolist()[:-1] for i in c_indices]

    print(centroids)

    # for iter_i in range(int(iter_num)):
    #     # clear previous clusters
    #     clusters = [[] for _ in range(k)]
    #     # reset pointer for file reading
    #
    #     for point in points.iloc:
    #         point = point.to_numpy()[:-1]
    #         distances = [np.linalg.norm(point - centroids[i]) for i in range(k)]
    #         closest = min(range(k), key=lambda x: distances[x])
    #         clusters[closest].append(point)
    #
    #     # calculating new centroids for checking convergence
    #     new_centroids = [
    #         np.array([sum(coord) / len(cluster) for coord in zip(*cluster)])
    #         for cluster in clusters
    #     ]
    #
    #     # if max convergence for all new centroids is less than epsilon then the condition applies for all
    #     if max(np.linalg.norm(centroids[i] - new_centroids[i]) for i in range(k)) < eps:
    #         centroids = new_centroids
    #         break
    #
    #     centroids = new_centroids
    #
    # print(",".join([str(i) for i in c_indices]))
    # for centroid in centroids:
    #     print(",".join(["%.4f" % coord for coord in centroid]))

except:
    print(error_msg)
    sys.exit(1)
