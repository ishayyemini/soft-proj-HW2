from setuptools import Extension, setup

module = Extension("mykmeanssp", sources=["kmeansmodule.c"])

setup(
    name="mykmeanssp",
    version="1.0",
    description="Integrate KMeans between Python and C",
    ext_modules=[module],
)
