#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

typedef unsigned char byte;
typedef unsigned short word;


struct FVector
{
	float X, Y, Z;

	FVector operator*(float scalar) const
	{
		return FVector{ X * scalar, Y * scalar, Z * scalar };
	}
};

struct FVector2D
{
	float X, Y;
};

struct FQuat
{
	float X, Y, Z, W;
};

struct VertexColor
{
	byte R, G, B, A;

	VertexColor()
	{
		R = G = B = A = 1;
	}
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
	std::vector<int> WedgeIndex;
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

struct VRawBoneInfluence
{
	float Weight;
	int PntIdx;
	int BoneIdx;
};

struct VJointPos
{
	FQuat Orientation;
	FVector Position;

	float Length;
	float XSize;
	float YSize;
	float ZSize;
};

struct FNamedBoneBinary
{
	char Name[64];
	int Flags;
	int NumChildren;
	int ParentIndex;
	VJointPos BonePos;
};

class PSK
{
public:
	PSK(std::string _filepath);
	void Deserialize();
	void close() { Ar.close(); };
	std::vector<FVector> GetScaledVerts(float scale);

	std::vector<FVector> Verts;
	std::vector<VVertex> Wedges;
	std::vector<VTriangle32> Tris;
	std::vector<FVector> Normals;
	std::vector<VMaterial> Materials;
	std::vector<VertexColor> VertexColors;
	std::vector<FVector2D> ExtraUVs;
	std::vector<VRawBoneInfluence> Weights;
	std::vector<FNamedBoneBinary> Bones;
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