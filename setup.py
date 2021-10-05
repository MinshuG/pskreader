# https://github.com/pybind/python_example/blob/master/setup.py
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.0.1"

from setuptools import setup

ext_modules = [
    Pybind11Extension("psk",
                      ["binding.cpp", "Psk.cpp"],
                      define_macros=[('VERSION_INFO', __version__)],
                      ),
]

setup(
    name="pskreader",
    version=__version__,
    author="Minshu Gurjar",
    url="https://github.com/MinshuG/pskreader",
    description="Python binding for pskreader",
    long_description="",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
