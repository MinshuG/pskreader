#include "pybind11/pybind11.h"
#include "Psk.h"

namespace py = pybind11;

PYBIND11_MODULE(psk, m) {
    py::class_<PSK>(m, "PSK")
        .def(py::init<const std::string &>())
        .def("deserialize", &PSK::Deserialize)
        .def("close", &PSK::close)
        .def_readwrite("Verts", &PSK::Verts)
        .def_readwrite("Wedges", &PSK::Wedges)
        .def_readwrite("Tris", &PSK::Tris);
}
