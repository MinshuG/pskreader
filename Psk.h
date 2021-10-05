#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

typedef unsigned char byte;
typedef unsigned short word;


class FVector
{
public:
	float X, Y, Z;
};

// Vertex with texturing info, akin to Hoppe's 'Wedge' concept - import only.
struct VVertex
{
	int PointIndex; // short, padded to int
	float U, V;
	byte MatIndex;
	byte Reserved;
	short Pad; // not used
};

struct VTriangle32 // same as 16
{
	int WedgeIndex[3];
	byte MatIndex;
	byte AuxMatIndex;
	unsigned SmoothingGroups;
};

struct VMaterial
{
	char MaterialName[64];
	int TextureIndex;
	unsigned PolyFlags;
	int AuxMaterial;
	unsigned AuxFlags;
	int LodBias;
	int LodStyle;
};

class PSK
{
public:
	PSK(std::string _filepath);
	void Deserialize();

	std::vector<FVector> Verts;
	std::vector<VVertex> Wedges;
	std::vector<VTriangle32> Tris;
	std::vector<VMaterial> Materials;

private:
	std::ifstream Ar;
};

struct VChunkHeader
{
	char ChunkID[20];
	int TypeFlag;
	int DataSize;
	int DataCount;

	void Deserialize(std::ifstream &Ar)
	{
		Ar >> ChunkID >> TypeFlag >> DataSize >> DataCount;
		return;
	}
};