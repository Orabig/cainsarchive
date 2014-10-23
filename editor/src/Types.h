#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef _WIN32

	//Signed types
	typedef __int8 int8;
	typedef __int16 int16;
	typedef __int32 int32;
	typedef __int64 int64;

	//Unsigned types
	typedef unsigned __int8 uint8;
	typedef unsigned __int16 uint16;
	typedef unsigned __int32 uint32;
	typedef unsigned __int64 uint64;

	struct Vector2D
	{
		float x, y;
	};

	struct Vector3D
	{
		float x, y, z;
	};

	struct Vector4D
	{
		float x, y, z, w;
	};

	struct cPackedNormal
	{
		unsigned char x, y, z, w;
	};

	struct sPackedUV
	{
		unsigned short u, v;
	};

	struct Quaternion
	{
		float w;
		Vector3D p;
	};

	struct cRGBAColor
	{
		unsigned char r, g, b, a;
	};

	struct fRGBAColor
	{
		float r, g, b, a;
	};

	struct uRGBAColor
	{
		uint32 r, g, b, a;
	};

	struct BoundingBox
	{
		Vector3D v3Corner00;
		Vector3D v3Corner01;
	};

	struct BoundingSphere
	{
		Vector3D v3Position;
		float fRadius;
	};

	struct PRSTransform
	{
		Quaternion q;
		Vector3D v;
		float s;
	};

	struct PRTransform
	{
		Quaternion q;
		Vector3D v;
	};

	struct Chunkheader
	{
		uint32 Unknown[4];
	};

#endif //#ifdef _WIN32

#endif //#ifndef _BOXEL_TYPES_H_
