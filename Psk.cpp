#include <cstdint>
#include <stdio.h>
#include "Psk.h"
#include <cassert>

#define CHUNK(name) (strncmp(Chunk.ChunkID, name, strlen(name)) == 0)

PSK::PSK(std::string _filepath)
{
	Ar.open(_filepath, std::ios::binary);
	if (!Ar)
	{
		return;
	}
}

std::vector<FVector> PSK::GetScaledVerts(float scale)
{
	std::vector<FVector> scaledVerts;
	scaledVerts.reserve(Verts.size());
	for (auto &vert : Verts)
	{
		scaledVerts.push_back(vert * scale);
	}
	return scaledVerts;
}

void PSK::Deserialize()
{
	std::string MAGIC = "ACTRHEAD";
	Ar.seekg(0, 0);
	long size;
	Ar.seekg(0, 2);
	size = Ar.tellg();
	Ar.seekg(0, 0);

	VChunkHeader Chunk;
	Ar.read(reinterpret_cast<char*>(&Chunk), sizeof(VChunkHeader));
	if (Chunk.ChunkID != MAGIC)
	{
		std::cout << "Not a psk file";
		return;
	}

	bool read = !size < (int)Ar.tellg() + 32;
	while (true)
	{
		Ar.read(reinterpret_cast<char*>(&Chunk), sizeof(VChunkHeader));
		if (CHUNK("PNTS0000"))
		{
			Verts.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Verts[i]), sizeof(FVector));
			}
		}
		else if (CHUNK("VTXW0000"))
		{
			Wedges.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Wedges[i]), sizeof(VVertex));
				if (Chunk.DataCount <= 65536)
					Wedges[i].PointIndex &= 0xFFFF;
			}
		}
		else if (CHUNK("FACE0000"))
		{
			Tris.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++) // my cpp skills at peak
			{
				Tris[i].WedgeIndex.resize(3);
				Ar.read(reinterpret_cast<char*>(&Tris[i].WedgeIndex[0]), sizeof(short));
				Ar.read(reinterpret_cast<char*>(&Tris[i].WedgeIndex[1]), sizeof(short));
				Ar.read(reinterpret_cast<char *>(&Tris[i].WedgeIndex[2]), sizeof(short));

				Ar.read(reinterpret_cast<char*>(&Tris[i].MatIndex), sizeof(Tris[i].MatIndex));
				Ar.read(reinterpret_cast<char*>(&Tris[i].AuxMatIndex), sizeof(Tris[i].AuxMatIndex));
				Ar.read(reinterpret_cast<char*>(&Tris[i].SmoothingGroups), sizeof(Tris[i].SmoothingGroups));
			}
		}
		else if (CHUNK("FACE3200"))
		{
			Tris.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Tris[i].WedgeIndex), sizeof(Tris[i].WedgeIndex)); // will this work?
				Ar.read(reinterpret_cast<char*>(&Tris[i].MatIndex), sizeof(Tris[i].MatIndex));
				Ar.read(reinterpret_cast<char*>(&Tris[i].AuxMatIndex), sizeof(Tris[i].AuxMatIndex));
				Ar.read(reinterpret_cast<char*>(&Tris[i].SmoothingGroups), sizeof(Tris[i].SmoothingGroups));
			}
		}
		else if (CHUNK("MATT0000"))
		{
			Materials.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Materials[i]), sizeof(VMaterial));
			};
		}
		else if (CHUNK("VERTEXCOLOR"))
		{
			VertexColors.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&VertexColors[i]), sizeof(VertexColor));
			}
		}
		else if (CHUNK("EXTRAUVS"))
		{
			ExtraUVs.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&ExtraUVs[i]), sizeof(FVector2D));
			}
		}
		else if (CHUNK("RAWWEIGHTS") || CHUNK("RAWW0000"))
		{
			Weights.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Weights[i]), sizeof(VRawBoneInfluence));
			}
		}
		else if (CHUNK("REFSKELT") || CHUNK("REFSKEL0"))
		{
			Bones.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Bones[i]), sizeof(FNamedBoneBinary));
			}
		}
		else if (CHUNK("VTXNORMS"))
		{
			Normals.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char*>(&Normals[i]), sizeof(FVector));
			}
		}
		else
		{
			std::cout << "[PSK] Unknown chunk: " << Chunk.ChunkID << std::endl;
			Ar.seekg(Chunk.DataCount * Chunk.DataSize, 1);
		}
		if (size == (int)Ar.tellg())
			break;
	}
	return;
}