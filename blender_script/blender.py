from typing import List
import sys
import os
import time

sys.path.append(os.path.realpath(".."))
# try:
from psk import PSK, FVector, VTriangle32, VVertex, VertexColor
# except:
#     from psk import PSK, VTriangle32, VVertex

def color_linear_to_srgb(c):
    """
    Convert from linear to sRGB color space.
    Source: Cycles addon implementation, node_color.h.
    """
    if c < 0.0031308:
        return 0.0 if c < 0.0 else c * 12.92
    else:
        return 1.055 * pow(c, 1.0 / 2.4) - 0.055

class Loader(PSK):
    # Verts: List[FVector]
    # Wedges: List[VVertex]
    # Tris: List[VTriangle32]
    # Materials: List[VMaterial]
    # VertexColors: List[VertexColor]
    # ExtraUVs: List[FVector2D]
    # Weights: List[VRawBoneInfluence]
    # Bones: List[FNamedBoneBinary]

    UV_by_face: list
    Faces: list
    WedgeIdx_by_faceIdx: list

    def parse(self):
        self.parse_faces()

    def parse_faces(self):
        faces = self.Tris
        wedges = self.Wedges

        # Faces, UV_by_face, WedgeIdx_by_faceIdx
        self.UV_by_face = []
        self.Faces = []
        self.WedgeIdx_by_faceIdx = []

        for face in faces:
            wedge0 = wedges[face.WedgeIndex[0]]
            wedge1 = wedges[face.WedgeIndex[1]]
            wedge2 = wedges[face.WedgeIndex[2]]

            self.Faces.append((wedge1.PointIndex,
                               wedge0.PointIndex, wedge2.PointIndex))
            uv = ((wedge1.U, 1.0 - wedge1.V), (wedge0.U,
                                               1.0 - wedge0.V), (wedge2.U, 1.0 - wedge2.V))

            # Mapping: FaceIndex <=> UV data <=> FaceMatIndex
            self.UV_by_face.append((uv, face.MatIndex, (wedge2.MatIndex, wedge1.MatIndex, wedge0.MatIndex)))

            # We need this for EXTRA UVs
            self.WedgeIdx_by_faceIdx.append(face.WedgeIndex)

def util_select_all(select):
    import bpy
    if select:
        actionString = 'SELECT'
    else:
        actionString = 'DESELECT'

    if bpy.ops.object.select_all.poll():
        bpy.ops.object.select_all(action=actionString)

    if bpy.ops.mesh.select_all.poll():
        bpy.ops.mesh.select_all(action=actionString)

    if bpy.ops.pose.select_all.poll():
        bpy.ops.pose.select_all(action=actionString)


def import_mesh(f: Loader, mesh_name: str):
    print(f"Importing {mesh_name}")
    import bpy
    uv_mat_ids = {}

    gen_names = {
        'armature_object': mesh_name + '.ao',
        'armature_data': mesh_name + '.ad',
        'mesh_object': mesh_name + '.mo',
        'mesh_data': mesh_name + '.md'
    }

    mesh_data = bpy.data.meshes.new(gen_names['mesh_data'])
    mesh_obj = bpy.data.objects.new(gen_names['mesh_object'], mesh_data)

    for wedges in f.Wedges:
        material_index = wedges.MatIndex
        if not (material_index in uv_mat_ids):
            uv_mat_ids[material_index] = 1
        else:
            uv_mat_ids[material_index] += 1

    blen_materials = []
    for material in f.Materials:
        materialname = material.MaterialName
        matdata = bpy.data.materials.get(materialname)

        if matdata is None:
            matdata = bpy.data.materials.new(materialname)

        blen_materials.append(matdata)
        mesh_data.materials.append(matdata)

    for i in range(len(uv_mat_ids)):
        mesh_data.uv_layers.new(name="UV" + str(i))

    # for counter, uv_data in enumerate(f.ExtraUVs): # not implemented
    #     if len(mesh_data.uv_layers) < MAX_UVS:
    #         mesh_data.uv_layers.new(name = "EXTRAUVS"+str(counter))
    #     else:
    #         Extrauvs.remove(uv_data)
    #         print('Extra UV layer %s is ignored. Re-import without "Split UV data".' % counter)

    mesh_data.from_pydata(list((x.X, x.Y, x.Z) for x in f.GetScaledVerts(0.01)), [], f.Faces)

    uv_layers = mesh_data.uv_layers
    for face, (faceUVs, _, WedgeMatIds) in zip(mesh_data.polygons, f.UV_by_face):
        face.material_index = f.UV_by_face[face.index][1]
        # per vertex        
        uv_layers[WedgeMatIds[0]].data[face.index * 3 + 0].uv = faceUVs[0]
        uv_layers[WedgeMatIds[1]].data[face.index * 3 + 1].uv = faceUVs[1]
        uv_layers[WedgeMatIds[2]].data[face.index * 3 + 2].uv = faceUVs[2]

    # Vertex colors
    if len(f.VertexColors) > 0:
            vtx_color_layer = mesh_data.vertex_colors.new(name = "PSKVTXCOL_0", do_init = False)
            pervertex = f.GetPerVertexColor()

            indices = [loop.vertex_index for loop in list(mesh_data.loops)]
            for counter, vertex_index in enumerate(indices):
                color = pervertex[vertex_index]
                color = color.to_tuple()

                vtx_color_layer.data[counter].color = (
                    color[0] / 255,
                    color[1] / 255,
                    color[2] / 255,
                    color[3] / 255
                )
                # if color is None:
                #     vtx_color_layer.data[counter].color = (1.,1.,1.,1.)
                # else:
                    # if bToSRGB:
                    #     vtx_color_layer.data[ counter ].color = (
                    #         color_linear_to_srgb(color[0] / 255),
                    #         color_linear_to_srgb(color[1] / 255),
                    #         color_linear_to_srgb(color[2] / 255),
                    #         color[3] / 255
                    #     )
                    # else:
                
    else:
        print('No vertex colors')

    bpy.context.collection.objects.link(mesh_obj)
    util_select_all(False)

def main():
    start = time.time()
    # if __name__ == "__main__":
    psk = Loader(r"C:\Users\Minshu\Desktop\BlenderUmap\run\Game\Athena\Aircraft\PartyBus_RT\Meshes\PartyBus_RT_Ground.pskx")
    psk.deserialize()
    psk.close()
    print(f"Parsing took: {time.time() - start}")
    # start = time.time()
    psk.parse()
    import_mesh(psk, "test")
    print(f"Importing took: {time.time() - start}")

if __name__ == "__main__":
    main()

