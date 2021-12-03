from typing import List

class FVector:
    X: float
    Y: float
    Z: float

class VVertex:
    PointIndex: int
    U: float
    V: float
    MatIndex: int

class VTriangle32:
    WedgeIndex: List[int]
    MatIndex: int
    AuxMatIndex: int
    SmoothingGroups: int

class VMaterial:
    MaterialName: str
    TextureIndex: int
    PolyFlags: int
    AuxMaterial: int
    AuxFlags: int
    LodBias: int
    LodStyle: int

class VRawBoneInfluence:
    Weight: float
    PntIdx: int
    BoneIdx: int

class VJointPos:
    Orientation: FQuat
    Position: FVector
    Length: float
    XSize: float
    YSize: float
    ZSize: float

class FNamedBoneBinary:
    Name: str
    Flags: int
    NumChildren: int
    ParentIndex: int
    BonePos: VJointPos

class FVector2D:
    X: float
    Y: float

class FQuat:
    X: float
    Y: float
    Z: float
    W: float

class VertexColor:
    R: int
    G: int
    B: int
    A: int

class PSK:
    Verts: List[FVector]
    Wedges: List[VVertex]
    Tris: List[VTriangle32]
    Materials: List[VMaterial]
    VertexColors: List[VertexColor]
    ExtraUVs: List[FVector2D]
    Weights: List[VRawBoneInfluence]
    Bones: List[FNamedBoneBinary]

    def __init__(self, filepath: str) -> None: ...
    def deserialize(): ...
    def close(): ...
    def GetPerVertexColor(self) -> List[VertexColor]: ...
    def GetScaledVerts(self, scale: float) -> List[FVector]: ...
