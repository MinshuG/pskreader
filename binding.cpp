#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "Psk.h"

namespace py = pybind11;

PYBIND11_MODULE(psk, m) {
    py::class_<PSK>(m, "PSK")
        .def(py::init<const std::string &>())
        .def("deserialize", &PSK::Deserialize)
        .def("close", &PSK::close)
        .def_readonly("Verts", &PSK::Verts)
        .def_readonly("Wedges", &PSK::Wedges)
        .def_readonly("Tris", &PSK::Tris)
        .def_readonly("Materials", &PSK::Materials)
        .def_readonly("VertexColors", &PSK::VertexColors)
        .def_readonly("ExtraUVs", &PSK::ExtraUVs)
        .def_readonly("Weights", &PSK::Weights)
        .def_readonly("Bones", &PSK::Bones);

    py::class_<FNamedBoneBinary>(m, "FNamedBoneBinary")
        .def_readonly("Name", &FNamedBoneBinary::Name)
        .def_readonly("Flags", &FNamedBoneBinary::Flags)
        .def_readonly("NumChildren", &FNamedBoneBinary::NumChildren)
        .def_readonly("ParentIndex", &FNamedBoneBinary::ParentIndex)
        .def_readonly("BonePos", &FNamedBoneBinary::BonePos)
        .def("__repr__", [](const FNamedBoneBinary &b) {
            return py::str("FNamedBoneBinary(Name={}, Flags={}, NumChildren={}, ParentIndex={}, BonePos={})").format(b.Name, b.Flags, b.NumChildren, b.ParentIndex, b.BonePos);; 
        });

    py::class_<VJointPos>(m, "VJointPos")
        .def_readonly("Orientation", &VJointPos::Orientation)
        .def_readonly("Position", &VJointPos::Position)
        .def_readonly("Length", &VJointPos::Length)
        .def_readonly("XSize", &VJointPos::XSize)
        .def_readonly("YSize", &VJointPos::YSize)
        .def_readonly("ZSize", &VJointPos::ZSize)
        .def("__repr__", [](const VJointPos &b) {
            return py::str("VJointPos(Orientation={}, Position={}, Length={}, XSize={}, YSize={}, ZSize={})").format(b.Orientation, b.Position, b.Length, b.XSize, b.YSize, b.ZSize);; 
        });

    py::class_<VRawBoneInfluence>(m, "VRawBoneInfluence")
        .def_readonly("Weight", &VRawBoneInfluence::Weight)
        .def_readonly("PntIdx", &VRawBoneInfluence::PntIdx)
        .def_readonly("BoneIdx", &VRawBoneInfluence::BoneIdx)
        .def("__repr__", [](const VRawBoneInfluence &b) {
            return py::str("VRawBoneInfluence(Weight={}, PntIdx={}, BoneIdx={})").format(b.Weight, b.PntIdx, b.BoneIdx);; 
        });

    py::class_<FVector>(m, "FVector")
        .def(py::init<float, float, float>())
        .def_readonly("X", &FVector::X)
        .def_readonly("Y", &FVector::Y)
        .def_readonly("Z", &FVector::Z)
        .def("__repr__", [](const FVector &v) {
            return py::str("FVector(X={}, Y={}, Z={})").format(v.X, v.Y, v.Z);
        })
        .def("__iter__", [](const FVector &v) {
            std::vector<float> values = {v.X, v.Y, v.Z};
            return py::make_iterator(values);
        });

    py::class_<FQuat>(m, "FQuat")
        .def(py::init<float, float, float, float>())
        .def_readonly("X", &FQuat::X)
        .def_readonly("Y", &FQuat::Y)
        .def_readonly("Z", &FQuat::Z)
        .def_readonly("W", &FQuat::W)
        .def("__repr__", [](const FQuat &v) {
            return py::str("FQuat(X={}, Y={}, Z={}, W={})").format(v.X, v.Y, v.Z, v.W);
        })
        .def("__iter__", [](const FQuat &v) {
            std::vector<float> values = {v.X, v.Y, v.Z, v.W};
            return py::make_iterator(values);
        });

    py::class_<VTriangle32>(m, "VTriangle32")
        .def(py::init<>())
        .def_readonly("WedgeIndex", &VTriangle32::WedgeIndex)
        .def_readonly("MatIndex", &VTriangle32::MatIndex)
        .def_readonly("AuxMatIndex", &VTriangle32::AuxMatIndex)
        .def_readonly("SmoothingGroups", &VTriangle32::SmoothingGroups)
        .def("__repr__", [](const VTriangle32 &v) {
            return py::str("VTriangle32(WedgeIndex={}, MatIndex={}, AuxMatIndex={}, SmoothingGroups={})").format(v.WedgeIndex, v.MatIndex, v.AuxMatIndex, v.SmoothingGroups);
        });

    py::class_<VVertex>(m, "VVertex")
        .def(py::init<>())
        .def_readonly("PointIndex", &VVertex::PointIndex)
        .def_readonly("U", &VVertex::U)
        .def_readonly("V", &VVertex::V)
        .def_readonly("MatIndex", &VVertex::MatIndex)
        .def("__repr__", [](const VVertex &v) {
            return py::str("VVertex(PointIndex={}, U={}, V={}, MatIndex={})").format(v.PointIndex, v.U, v.V, v.MatIndex);
        });

    py::class_<VMaterial>(m, "VMaterial")
    .def(py::init<>())
    .def_readonly("MaterialName", &VMaterial::MaterialName)
    .def_readonly("TextureIndex", &VMaterial::TextureIndex)
    .def_readonly("PolyFlags", &VMaterial::PolyFlags)
    .def_readonly("AuxMaterial", &VMaterial::AuxMaterial)
    .def_readonly("AuxFlags", &VMaterial::AuxFlags)
    .def_readonly("LodBias", &VMaterial::LodBias)
    .def_readonly("LodStyle", &VMaterial::LodStyle);
}
