#ifndef _APPEARANCEDEFINITIONS_H_
#define _APPEARANCEDEFINITIONS_H_
#include "Types.h"

struct AppearanceHeader
{
	uint32 Unknown00;
	uint32 uSNOType;
	uint32 Unknown01[2];
	uint32 uSNOIndex;
	uint32 Unknown02[2];
	uint32 dwFlags;
	uint32 Unknown03;

	//Bone Chunk
	uint32 uBoneChunkCount;
	uint32 uBoneChunkOffset;
	uint32 uBoneChunkSize;

	uint32 Unknown04[2];

	//Look Data
	uint32 Unknown05;
	char szLookName[64];
	float fAngles[4];
	float fAngularVelocity;
	float Unknown06;

	int nBonePulseCount;
	uint32 Unknown07[2];

	uint32 uBonePulseListOffset;
	uint32 uBonePulseListSize;

	//Geo Set 00 Chunk
	uint32 uGeoSet00ChunkCount;
	uint32 uGeoSet00ChunkOffset;
	uint32 uGeoSet00ChunkSize;

	uint32 Unknown08[3];

	//Geo Set 01 Chunk
	uint32 uGeoSet01ChunkCount;
	uint32 uGeoSet01ChunkOffset;
	uint32 uGeoSet01ChunkSize;

	uint32 Unknown09[3];

	//Bounding Sphere
	float v3BoundingSpherePosition[3];
	float fBoundingSphereRadius;

	//Collision Chunk
	uint32 uCollisionChunkCount;
	uint32 uCollisionChunkOffset;
	uint32 uCollisionChunkSize;

	uint32 Unknown10[3];

	//HardPoints Chunk
	uint32 uHardPointsChunkCount;
	uint32 uHardPointsChunkOffset;
	uint32 uHardPointsChunkSize;

	uint32 Unknown11[3];

	float v3Unknown00[3];

	uint32 Unknown12;

	//Octree
	uint32 uMaxNodes;
	uint32 uMaxLeaves;
	uint32 uMaxPrimitives;
	uint32 uNodeCount;
	uint32 uLeafCount;
	uint32 uPrimitiveCount;
	
	uint32 uNodeChunkOffset;
	uint32 uNodeChunkSize;

	uint32 Unknown13[2];

	uint32 uLeafChunkOffset;
	uint32 uLeafChunkSize;

	uint32 Unknown14[2];

	uint32 uPrimitivesChunkOffset;
	uint32 uPrimitivesChunkSize;

	uint32 Unknown15[2];

	//AABB Bounding Box
	float v3BoundingBoxPosition00[3];
	float v3BoundingBoxPosition01[3];

	//Loop Constraint Chunk
	uint32 uLoopConstraintChunkCount;
	uint32 uLoopConstraintChunkOffset;
	uint32 uLoopConstraintChunkSize;

	uint32 Unknown16[3];

	uint32 uRagdollDegrade;

	uint32 Unknown17[3];

	//XObject
	char szXObjectName[256];
	char szXObjectPathName[256];

	//XMesh
	char szXMeshName[256];
	char szXMeshPathName[256];

	uint32 Unknown18[4];

	uint32 uMaterialNameCount; //The Material Names chunk is 64 byte strings
	uint32 uMaterialReferenceCount;

	uint32 uMaterialsOffset;
	uint32 uMaterialsSize;

	uint32 Unknown19[2];

	uint32 uMaterialNameOffset;
	uint32 uMaterialNameSize;

	uint32 Unknown20[2];

	uint32 uStaticLightChunkCount;
	uint32 uStaticLightChunkOffset;
	uint32 uStaticLightChunkSize;

	uint32 Unknown21[4];

	uint32 uTimes[5];

	uint32 Unknown22[8];
};

struct BoneChunk
{
	char szName[64];
	int nParentIndex;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	//Transforms
	PRSTransform prsTransform00, prsTransform01, prsTransform02;

	uint32 uCollisionShapeChunkCount;
	uint32 uCollisionShapeChunkOffset;
	uint32 uCollisionShapeChunkSize;

	uint32 Unknown00; //Constraint count??

	uint32 uConstraintsChunkOffset;
	uint32 uConstraintsChunkSize;

	uint32 Unknown01;
	uint32 uSNOParticleSystem; //?
};

struct CollisionShapeChunk
{
	uint32 Unknown00[4];
	float Unknown01[3];
	uint32 uPolytopeChunkOffset;
	uint32 uPolytopeChunkSize;
	uint32 Unknown02;
	float Unknown03[7]; //2x vector3 + float?
};

struct ConstraintParameters
{
	char Unknown00[64];
	uint32 Unknown01[4];
	PRTransform prTransform00;
	Vector3D v300;
	PRTransform prTransform01, prTransform02;
	float Unknown02[8];
	char Unknown03[64];
};

struct PolytopeChunk
{
	uint32 Unknown00[8];
	float Unknown01[3];
	int nVerticies;
	int nPlanes;
	int nSubEdges;
	float Unknown02[2];

	uint32 uVertexChunkOffset;
	uint32 uVertexChunkSize;

	uint32 uPlaceChunkOffset;
	uint32 uPlaceChunkSize;

	uint32 uSubEdgesChunkOffset;
	uint32 uSubEdgesChunkSize;

	uint32 uFaceSubEdgesChunkOffset;
	uint32 uFaceSubEdgesChunkSize;
};

struct PlaneChunk
{
	Vector3D v;
	float f;
};

struct SubEdgeChunk
{
	char Unknown00[4];
};

struct ModelVertex
{
	Vector3D v3Position;
	cPackedNormal packedv4Normal;
	int32 Unknown00[2];
	sPackedUV packedv2UV;
	int32 Unknown01[4];
};

struct SubObject
{
	uint32 Unknown00;

	uint32 uVertexCount;

	uint32 uVertexListOffset;
	uint32 uVertexListSize;

	uint32 Unknown01;

	uint32 uInfluenceListOffset;
	uint32 uInfluenceListSize;

	uint32 Unknown02;

	uint32 uIndexCount;
	uint32 uIndexListOffset;
	uint32 uIndexListSize;

	uint32 Unknown03;

	uint32 uClothListOffset;
	uint32 uClothListSize;

	uint32 Unknown04;

	int nSNOSurface;

	uint32 Unknown05;
	float Unknown06;
	char szObjectName[128];
	char szMeshName[128];

	BoundingBox boundingBox;

	uint32 uCollisionShapeCount;
	uint32 uCollisionShapeListOffset;
	uint32 uCollisionShapeListSize;
	uint32 Unknown08[2];
};

struct MaterialReference
{
	char *szName;
	uint32 uMaterialOffset;
	uint32 uMaterialSize;
	uint32 Unknown;
};

struct MaterialTexture
{
	uint32 uTextureType; //is it?
	uint32 uSNOIndex;
	uint32 Unknown01[44]; //TODO: Tex Animation Parameters add / use them
};

struct MaterialHeader
{
	char szMaterialName[128];
	uint32 uMaterialsOffset;
	uint32 uMaterialsSize;
	uint32 Unknown00; //padding?
};

struct Material
{
	uint32 Unknown00;
	int32 nSNOCloth;
	uint32 uTagMapOffset;
	uint32 uTagMapSize;
	uint32 Unknown01[2];

	int32 nSNOShaderMap;

	fRGBAColor materialColors[4]; //?
	float Unknown02;
	int32 Unknown03;

	uint32 uTextureListOffset;
	uint32 uTextureListSize;

	int32 Unknown04[5];

	int32 nSNOMaterial;

	int32 Unknown05[29];
};


#endif //#ifndef _APPEARANCEDEFINITIONS_H_
