from setuptools import Extension, setup

module = Extension("kmeanssp", sources=["kmeansmodule.c"])

setup(
    name="kmeanssp",
    version="1.0",
    description="Integrate KMeans between Python and C",
    ext_modules=[module],
)
