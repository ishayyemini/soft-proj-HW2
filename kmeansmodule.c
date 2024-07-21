#define err(x) { printf(x); free_memory(points, size, centroids, k); exit(1); }
#define err_k(x) { printf(x); free_memory_k(new_centroids, clusters, k, c_sizes); exit(1); }

# define PY_SSIZE_T_CLEAN
#include "/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/Headers/Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double euclidean_distance(double *point1, double *point2, int dim) {
    double sum = 0;
    int i;
    for (i = 0; i < dim; ++i) sum += pow(point1[i] - point2[i], 2);
    return sqrt(sum);
}

void calculate_centroid(double *centroid, double **cluster, int c_size, int dim) {
    int i, j;
    for (j = 0; j < dim; j++) centroid[j] = 0.0;
    for (i = 0; i < c_size; i++)
        for (j = 0; j < dim; j++)
            centroid[j] += cluster[i][j];
    for (j = 0; j < dim; j++) centroid[j] /= c_size;
}

void free_memory(double **points, int size, double **centroids, int k) {
    int i;
    if (points != NULL) {
        for (i = 0; i < size; i++) if (points[i] != NULL) free(points[i]);
        free(points);
    }

    if (centroids != NULL) {
        for (i = 0; i < k; i++) if (centroids[i] != NULL) free(centroids[i]);
        free(centroids);
    }
}

void free_memory_k(double **new_centroids, double ***clusters, int k, int *c_sizes) {
    int i;
    if (new_centroids != NULL) {
        for (i = 0; i < k; i++) if (new_centroids[i] != NULL) free(new_centroids[i]);
        free(new_centroids);
    }

    if (clusters != NULL) {
        for (i = 0; i < k; i++) if (clusters[i] != NULL) free(clusters[i]);
        free(clusters);
    }

    if (c_sizes != NULL) free(c_sizes);
}

int kmeans(double **points, int size, double ***centroids, int k, int iter, double eps, int dimension) {
    double max_cent_dist;
    int i, j;
    int iter_i;

    double **new_centroids = NULL;
    double ***clusters = NULL;
    int *c_sizes = NULL;

    new_centroids = calloc(k, sizeof(double *));
    clusters = calloc(k, sizeof(double **)); /* 2d array of pointers to elements in points */
    c_sizes = calloc(k, sizeof(int)); /* storing cluster lengths for appending new elements */
    if (new_centroids == NULL || clusters == NULL || c_sizes == NULL) err_k("An Error Has Occurred\n")


    for (i = 0; i < k; ++i) {
        new_centroids[i] = calloc(dimension, sizeof(double));
        clusters[i] = calloc(size, sizeof(double *));
        if (new_centroids[i] == NULL || clusters[i] == NULL) err_k("An Error Has Occurred\n")
    }

    for (iter_i = 0; iter_i < iter; iter_i++) {
        for (i = 0; i < k; ++i) c_sizes[i] = 0;

        /* assign points to clusters */
        for (i = 0; i < size; i++) {
            /* calculate distance from point i to the first centroid and then calculate the rest */
            double min_dist = euclidean_distance(points[i], (*centroids)[0], dimension);
            int closest = 0;
            for (j = 1; j < k; j++) {
                double curr_dist = euclidean_distance(points[i], (*centroids)[j], dimension);
                if (curr_dist < min_dist) {
                    min_dist = curr_dist;
                    closest = j;
                }
            }
            clusters[closest][c_sizes[closest]++] = points[i];
        }


        /* calculate new centroids */
        max_cent_dist = 0;
        for (i = 0; i < k; i++) {
            double curr_dist;
            calculate_centroid(new_centroids[i], clusters[i], c_sizes[i], dimension);
            curr_dist = euclidean_distance((*centroids)[i], new_centroids[i], dimension);
            if (max_cent_dist < curr_dist) max_cent_dist = curr_dist;
            for (j = 0; j < dimension; j++) (*centroids)[i][j] = new_centroids[i][j];
        }

        if (max_cent_dist < eps) {
            break;
        }
    }

    /* printing the centroids */
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimension; j++) {
            printf("%.4f", (*centroids)[i][j]);
            if (j != dimension - 1) printf(",");
        }
        printf("\n");
    }

    /* memory de-allocation */
    free_memory_k(new_centroids, clusters, k, c_sizes);

    return 0;
}

static PyObject *KMeansLib_Fit(PyObject *self, PyObject *args) {
    PyObject *p_points;
    PyObject *p_centroids;
    PyObject *p_point;
    PyObject *p_coord;

    int iter;
    double eps;
    int size, k, dimension = 0;
    double **points = NULL;
    double **centroids = NULL;
    int i, j;

    if (!PyArg_ParseTuple(args, "OOid", &p_points, &p_centroids, &iter, &eps)) {
        return NULL;
    }

    size = PyObject_Length(p_points);
    if (size < 0) return NULL;

    k = PyObject_Length(p_centroids);
    if (k < 0) return NULL;

    points = calloc(size, sizeof(double *));
    if (points == NULL) err("An Error Has Occurred\n")
    for (i = 0; i < size; i++) {
        p_point = PyList_GetItem(p_points, i);
        if (dimension == 0) dimension = PyObject_Length(p_point);

        points[i] = calloc(dimension, sizeof(double));

        for (j = 0; j < dimension; j++) {
            p_coord = PyList_GetItem(p_point, j);
            points[i][j] = PyFloat_AsDouble(p_coord);
        }
    }

    centroids = calloc(k, sizeof(double *));
    if (centroids == NULL) err("An Error Has Occurred\n")
    for (i = 0; i < k; i++) {
        centroids[i] = calloc(dimension, sizeof(double));
        p_point = PyList_GetItem(p_centroids, i);
        for (j = 0; j < dimension; j++) {
            p_coord = PyList_GetItem(p_point, j);
            centroids[i][j] = PyFloat_AsDouble(p_coord);
        }
    }

    kmeans(points, size, &centroids, k, iter, eps, dimension);

    free_memory(points, size, centroids, k);

    return Py_BuildValue("i", 5);
}

static PyMethodDef KMeansLib_FunctionsTable[] = {
        {
                "fit", // name exposed to Python
                KMeansLib_Fit, // C wrapper function
                            METH_VARARGS, // received variable args (but really just 1)
                "Calculates the KMeans algorithm based on receiving the initial centroids" // documentation
        },
        {
                NULL, NULL, 0, NULL
        }
};

static struct PyModuleDef KMeans_Module = {
        PyModuleDef_HEAD_INIT,
        "kmeanssp",     // name of module exposed to Python
        "Integrate KMeans between Python and C", // module documentation
        -1,
        KMeansLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_kmeanssp(void) {
    return PyModule_Create(&KMeans_Module);
}
