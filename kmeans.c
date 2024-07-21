#define err(x) { printf(x); free_memory(size, k, c_sizes, points, new_centroids, centroids, clusters); exit(1); }

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kmeanslib.h"

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

void free_memory(int size, int k, int *c_sizes, double **points, double **new_centroids, double **centroids, double ***clusters) {
    int i;
    if (points != NULL) {
        for (i = 0; i < size; i++) if (points[i] != NULL) free(points[i]);
        free(points);
    }

    if (new_centroids != NULL) {
        for (i = 0; i < k; i++) if (new_centroids[i] != NULL) free(new_centroids[i]);
        free(new_centroids);
    }

    if (clusters != NULL) {
        for (i = 0; i < k; i++) if (clusters[i] != NULL) free(clusters[i]);
        free(clusters);
    }

    if (centroids != NULL) {
        for (i = 0; i < k; i++) if (centroids[i] != NULL) free(centroids[i]);
        free(centroids);
    }

    if (c_sizes != NULL) free(c_sizes);
}

int kmeans(double **points, int size, double **centroids, int k, int iter, double eps, int dimension) {
    double max_cent_dist;
    int i, j;
    int iter_i;

    double **new_centroids = NULL;
    double ***clusters = NULL;
    int *c_sizes = NULL;

    new_centroids = calloc(k, sizeof(double *));
    clusters = calloc(k, sizeof(double **)); /* 2d array of pointers to elements in points */
    c_sizes = calloc(k, sizeof(int)); /* storing cluster lengths for appending new elements */
    if (new_centroids == NULL || clusters == NULL || c_sizes == NULL) err("An Error Has Occurred\n")

    for (i = 0; i < k; ++i) {
        new_centroids[i] = calloc(dimension, sizeof(double));
        clusters[i] = calloc(size, sizeof(double *));
        if (new_centroids[i] == NULL || clusters[i] == NULL) err("An Error Has Occurred\n")
    }


    for (iter_i = 0; iter_i < iter; iter_i++) {
        for (i = 0; i < k; ++i) c_sizes[i] = 0;

        /* assign points to clusters */
        for (i = 0; i < size; i++) {
            /* calculate distance from point i to the first centroid and then calculate the rest */
            double min_dist = euclidean_distance(points[i], centroids[0], dimension);
            int closest = 0;
            for (j = 1; j < k; j++) {
                double curr_dist = euclidean_distance(points[i], centroids[j], dimension);
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
            curr_dist = euclidean_distance(centroids[i], new_centroids[i], dimension);
            if (max_cent_dist < curr_dist) max_cent_dist = curr_dist;
            for (j = 0; j < dimension; j++) centroids[i][j] = new_centroids[i][j];
        }

        if (max_cent_dist < eps) {
            break;
        }
    }

    /* printing the centroids */
    for (i = 0; i < k; i++) {
        for (j = 0; j < dimension; j++) {
            printf("%.4f", centroids[i][j]);
            if (j != dimension - 1) printf(",");
        }
        printf("\n");
    }

    /* memory de-allocation */
    free_memory(size, k, c_sizes, points, new_centroids, centroids, clusters);

    return 0;
}
