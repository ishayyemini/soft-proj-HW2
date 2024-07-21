# define PY_SSIZE_T_CLEAN
#include "/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/Headers/Python.h"
#include <stdio.h>

#include "kmeanslib.h"

int counter = 0;

static PyObject *KMeansLib_Fit(PyObject *self, PyObject *args) {
    PyObject *lst;
    PyObject *item;
    long num;
    if (!PyArg_ParseTuple(args, "O", &lst)) {
        return NULL;
    }

    int n = PyObject_Length(lst);
    if (n < 0) {
        return NULL;
    }

    long *nums = (long *)malloc(n * sizeof(long));
    if (nums == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return NULL;
    }
    int i;
    for (i = 0; i < n; i++) {
        item = PyList_GetItem(lst, i);
        num = PyLong_AsLong(item);
        nums[i] = num;
    }

    unsigned long fact_sum;
    fact_sum = kmeans(nums, n);
    free(nums);
    return Py_BuildValue("i", fact_sum);
}

static PyObject* GetList(PyObject* self, PyObject* args)
{
    int N,r;
    PyObject* python_val;
    PyObject* python_int;
    if (!PyArg_ParseTuple(args, "i", &N)) {
        return NULL;
    }
    python_val = PyList_New(N);
    for (int i = 0; i < N; ++i)
    {
        r = i;
        python_int = Py_BuildValue("i", r);
        PyList_SetItem(python_val, i, python_int);
    }
    return python_val;
}

// module's function table
static PyMethodDef KMeansLib_FunctionsTable[] = {
        {
                "fit", // name exposed to Python
                KMeansLib_Fit, // C wrapper function
                            METH_VARARGS, // received variable args (but really just 1)
                "Calculates the KMeans algorithm based on receiving the initial centroids" // documentation
        }, {
                NULL, NULL, 0, NULL
        }
};

// modules definition
static struct PyModuleDef KMeans_Module = {
        PyModuleDef_HEAD_INIT,
        "kmeansmodule",     // name of module exposed to Python
        "Integrate KMeans between Python and C", // module documentation
        -1,
        KMeansLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_demo(void) {
    return PyModule_Create(&KMeans_Module);
}
