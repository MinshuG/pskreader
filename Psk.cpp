#include <cstdint>
#include <stdio.h>
#include "Psk.h"
#include <cassert>

PSK::PSK(std::string _filepath)
{
	Ar.open(_filepath, std::ios::binary);
	if (!Ar)
	{
		return;
	}
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
	Ar.read(reinterpret_cast<char *>(&Chunk), sizeof(VChunkHeader));
	if (Chunk.ChunkID != MAGIC)
	{
		std::cout << "Not a psk file";
		return;
	}

	bool read = !size < (int)Ar.tellg() + 32;
	while (true)
	{
		Ar.read(reinterpret_cast<char *>(&Chunk), sizeof(VChunkHeader));
		#define CHUNK(name) (strcmp(Chunk.ChunkID, name) == 0)
		if (CHUNK("PNTS0000"))
		{
			FVector vec;
			Verts.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char *>(&vec), sizeof(vec));
				Verts[i] = vec;
			}
		}
		else if (CHUNK("VTXW0000"))
		{
			VVertex vert;
			Wedges.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char *>(&vert), sizeof(vert));
				Wedges[i] = vert;
				if (Chunk.DataCount <= 65536)
					Wedges[i].PointIndex &= 0xFFFF;
			}
		}
		else if (CHUNK("FACE0000"))
		{
			Tris.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++) // my cpp skills at peak
			{
				Ar.read(reinterpret_cast<char *>(&Tris[i].WedgeIndex[0]), sizeof(short));
				Ar.read(reinterpret_cast<char *>(&Tris[i].WedgeIndex[1]), sizeof(short));
				Ar.read(reinterpret_cast<char *>(&Tris[i].WedgeIndex[2]), sizeof(short));

				Ar.read(reinterpret_cast<char *>(&Tris[i].MatIndex), sizeof(Tris[i].MatIndex));
				Ar.read(reinterpret_cast<char *>(&Tris[i].AuxMatIndex), sizeof(Tris[i].AuxMatIndex));
				Ar.read(reinterpret_cast<char *>(&Tris[i].SmoothingGroups), sizeof(Tris[i].SmoothingGroups));
			}
		}
		else if (CHUNK("FACE3200"))
		{
			Tris.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char *>(&Tris[i].WedgeIndex), sizeof(Tris[i].WedgeIndex));
				Ar.read(reinterpret_cast<char *>(&Tris[i].MatIndex), sizeof(Tris[i].MatIndex));
				Ar.read(reinterpret_cast<char *>(&Tris[i].AuxMatIndex), sizeof(Tris[i].AuxMatIndex));
				Ar.read(reinterpret_cast<char *>(&Tris[i].SmoothingGroups), sizeof(Tris[i].SmoothingGroups));
			}
		}
		else if (CHUNK("MATT0000"))
		{
			VMaterial mat;
			Materials.resize(Chunk.DataCount);
			for (int i = 0; i < Chunk.DataCount; i++)
			{
				Ar.read(reinterpret_cast<char *>(&mat), sizeof(mat));
				Materials[i] = mat;
			};
		}
		else // skel probably later
		{
			//assert(Chunk.DataCount > 0 && Chunk.DataSize > 0);
			Ar.seekg(Chunk.DataCount * Chunk.DataSize, 1);
		}
		if (size == (int)Ar.tellg())
			break;
	}
	return;
}