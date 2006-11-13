/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and FreyjaFSM.
 *
 *             This common interface makes it possible to share ABI 
 *             compatiblity for binary C/C++ plugins.
 *             ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <math.h>

#if defined (FREYJA_PLUGINS) && !(WIN32)
#   include <dlfcn.h>
#endif

#ifdef WIN32
#   include <windows.h>
#endif

#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "FreyjaCamera.h"
#include "FreyjaFSM.h"
#include "FreyjaImage.h"
#include "FreyjaLight.h"
#include "FreyjaMaterial.h"
#include "FreyjaTexture.h"
#include "MetaData.h"

#include "FreyjaPluginABI.h"

#define QUAT_BACKEND 0


using namespace freyja;
using namespace mstl;


/* Until Freyja completely replaces Egg backend, 
 * let these vector pools float here.
 * These should be storged in the Scene and accessed from the
 * SceneManager singleton  */
Vector<MetaData *> gFreyjaMetaData; 
Vector<FreyjaCamera *>  gFreyjaCameras;
Vector<FreyjaLight *>  gFreyjaLights;
Vector<FreyjaPluginDesc *> gFreyjaPlugins;

Vector<char *> gPluginDirectories;
int32 gCurrentFreyjaPlugin = -1;




void freyjaGetGenericTransform3fv(freyja_transform_t transform,
								  freyja_transform_action_t action,
								  index_t id, vec3_t xyz)
{
	switch (transform)
	{
	case fTransformScene:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformModel:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformMesh:
		switch (action)
		{
		case fTranslate:
			freyjaGetMeshPosition(id, xyz);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertexFrame:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformSkeleton:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformBone:
		switch (action)
		{
		case fTranslate:
			freyjaGetBoneTranslation3fv(id, xyz);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformUVMap:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertexGroup:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertex:
		switch (action)
		{
		case fTranslate:
			freyjaGetVertexXYZ3fv(id, xyz);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformTexCoord:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;
	}
}

		
void freyjaGenericTransform3fv(freyja_transform_t transform,
							   freyja_transform_action_t action,
							   index_t id, vec3_t xyz)
{
	switch (transform)
	{
	case fTransformScene:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformModel:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformMesh:
		switch (action)
		{
		case fTranslate:
			//freyjaMeshPosition(id, xyz);
			freyjaMeshTransform(id, freyjaGetMeshVertexGroupIndex(id, 0),
								action, xyz[0], xyz[1], xyz[2]);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertexFrame:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformSkeleton:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformBone:
		switch (action)
		{
		case fTranslate:
			freyjaBoneTransform(id, action, xyz[0], xyz[1], xyz[2]);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformUVMap:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertexGroup:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformVertex:
		switch (action)
		{
		case fTranslate:
			freyjaVertexPosition3fv(id, xyz);
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;

	case fTransformTexCoord:
		switch (action)
		{
		case fTranslate:
			break;
		case fRotate:
			break;
		case fScale:
			break;
		case fRotateAboutPoint:
			break;
		case fScaleAboutPoint:
			break;
		}
		break;
	}
}


//////////////////////////////////////////////////////////


int32 freyjaCheckModel(const char *filename)
{
	SystemIO::FileReader r;
	freyja_file_header_t header;
	int32 offset;


	if (!r.Open(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.SetByteOrder(SystemIO::File::LITTLE);

	/* Read header */
	offset = r.GetOffset();
	r.ReadString(16, header.magic);
	r.Close();

	if (!strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
	{
		return 0;
	}

	return -1;
}


int32 freyjaLoadMeshChunkV1(SystemIO::FileReader &r)
{
	Vector<long> verticesMap, texcoordsMap;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	int32 bone, frame, material;
	int32 i, j, count, idx, flags;
	int32 polygonCount;
	int32 vertexGroupCount;
	int32 meshFlags;
	int32 vertexCount;
	int32 vertexWeightCount;
	int32 vertexFrameCount;
	int32 texCoordCount;


	/* Read from diskfile */
	meshFlags = r.ReadLong();
	vertexCount = r.ReadLong();
	vertexWeightCount = r.ReadLong();
	vertexFrameCount = r.ReadLong();
	texCoordCount = r.ReadLong();
	polygonCount = r.ReadLong();
	vertexGroupCount = r.ReadLong();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);
	
	/* Vertices */
	for (i = 0; i < vertexCount; ++i)
	{
		flags = r.ReadLong();

		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();

		idx = freyjaVertexCreate3fv(xyz);

		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();

		freyjaVertexNormal3fv(idx, xyz);

		MSTL_MSG("\nn[%i] <%f, %f, %f>", idx, xyz[0], xyz[1], xyz[2]);

		for (j = 0; j < 2; ++j)
			uv[j] = r.ReadFloat32();
		
		freyjaVertexTexCoord2fv(idx, uv);

		verticesMap.pushBack(idx);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP

	/* VertexWeights */
	for (i = 0; i < vertexWeightCount; ++i)
	{
		idx = r.ReadLong();
		bone = r.ReadLong();
		weight = r.ReadFloat32();
		
		freyjaVertexWeight(verticesMap[idx], weight, bone);
	}

	/* VertexFrames */
	for (i = 0; i < vertexFrameCount; ++i)
	{
		idx = r.ReadLong();
		frame = r.ReadLong(); // Reserved use
		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();
		
		freyjaVertexFrame3f(verticesMap[idx], xyz[0], xyz[1], xyz[2]);
	}

	/* TexCoords */
	for (i = 0; i < texCoordCount; ++i)
	{
		for (j = 0; j < 2; ++j)
			uv[j] = r.ReadFloat32();

		idx = freyjaTexCoordCreate2fv(uv);
		texcoordsMap.pushBack(idx);
	}

	/* Polygons */
	for (i = 0; i < polygonCount; ++i)
	{
		freyjaBegin(FREYJA_POLYGON);
		flags = r.ReadLong();
		material = r.ReadLong();
		freyjaPolygonMaterial1i(material);

		count = r.ReadLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.ReadLong();
			freyjaPolygonVertex1i(verticesMap[idx]);
		}

		count = r.ReadLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.ReadLong();
			freyjaPolygonTexCoord1i(texcoordsMap[idx]);
		}

		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	return 0;
}


/* This is to fix all the problems with the backend delete corruption
 * It filters and realigns indices to only save valid data, not
 * the entire junky backend cruft like undo data which remains
 *
 * Vector maps are very memory wasteful */  // use a materialMap
int32 freyjaSaveMeshChunkV1(SystemIO::FileWriter &w, index_t meshIndex)
{
	const int32 version = 1;
	Vector<long> polygons, vertices, texcoords;
	Vector<long> verticesMap, texcoordsMap;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	index_t bone, frame;
	int32 flags, material;
	int32 i, j, k, count, idx, vertex, texcoord;
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	int32 vertexGroupCount = freyjaGetMeshVertexGroupCount(meshIndex);
	int32 byteSize = 0;
	int32 meshFlags = freyjaGetMeshFlags(meshIndex);
	int32 vertexCount = 0;
	int32 vertexWeightCount = 0;
	int32 vertexFrameCount = 0;
	int32 texCoordCount = 0;


	if (polygonCount < 1)
		return -1;

	byteSize += 4; // meshFlags
	byteSize += 4; // vertexCount
	byteSize += 4; // vertexWeightCount
	byteSize += 4; // vertexFrameCount
	byteSize += 4; // texCoordCount
	byteSize += 4; // polygonCount
	byteSize += 4; // vertexGroupCount

	/* Polygons, filtered by mesh */
	for (i = 0, count = 0; i < polygonCount; ++i)
	{
		idx = freyjaGetMeshPolygonIndex(meshIndex, i);
		count = freyjaGetPolygonEdgeCount(idx);

		/* Weed out invalid polygons */
		if (count < 3) 
		{
			continue;
		}

		polygons.pushBack(idx);
	}

	polygonCount = polygons.end();


	/* Vertices and polymapped TexCoords, filtered by filtered polygons */
	verticesMap.reserve(freyjaGetCount(FREYJA_VERTEX) + 1);
	texcoordsMap.reserve(freyjaGetCount(FREYJA_TEXCOORD) + 1);

	count = freyjaGetCount(FREYJA_VERTEX);
	for (i = 0; i < count; ++i)
		verticesMap.pushBack(0);

	count = freyjaGetCount(FREYJA_TEXCOORD);
	for (i = 0; i < count; ++i)
		texcoordsMap.pushBack(0);

	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];
		count = freyjaGetPolygonVertexCount(idx);

		byteSize += 4 + 4 + 4 + 4; // flags vertCount texCount material
		byteSize += (freyjaGetPolygonVertexCount(idx) * 4 +
					 freyjaGetPolygonTexCoordCount(idx) * 4);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetPolygonVertexIndex(idx, j);

			for (k = vertices.begin(); k < (long)vertices.end(); ++k)
			{
				if (vertex == vertices[k])
				{
					vertex = -2;
					break;
				}
			}

			if (vertex > -2)
			{
				verticesMap.assign(vertex, vertexCount);
				++vertexCount;
				vertices.pushBack(vertex);
				byteSize += (4 + 12 + 12 + 8); // flags xyz nxyz uv

				vertexWeightCount += freyjaGetVertexWeightCount(vertex);
				byteSize += 12 * freyjaGetVertexWeightCount(vertex); // vidx weight bone
				vertexFrameCount += freyjaGetVertexFrameCount(vertex);
				byteSize += 20 * freyjaGetVertexFrameCount(vertex); // vidx frame xyz
			}
		}

		count = freyjaGetPolygonTexCoordCount(idx);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetPolygonTexCoordIndex(idx, j);

			for (k = texcoords.begin(); k < (long)texcoords.end(); ++k)
			{
				if (texcoord == texcoords[k])
				{
					texcoord = -2;
					break;
				}
			}

			if (texcoord > -2)
			{
				texcoordsMap.assign(texcoord, texCoordCount);
				++texCoordCount;
				texcoords.pushBack(texcoord);
				byteSize += 8; // uv
			}
		}
	}


	/* Groups WARNING currently not written to disk */
	for (i = 0; i < vertexGroupCount; ++i)
	{
		idx = freyjaGetMeshVertexGroupIndex(meshIndex, i);
	}

	vertexGroupCount = 0;

	/* Write to diskfile */
	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = byteSize;
	chunk.flags = 0x0;
	chunk.version = version;

	w.WriteLong(chunk.type);
	w.WriteLong(chunk.size);
	w.WriteLong(chunk.flags);
	w.WriteLong(chunk.version);

	w.WriteLong(meshFlags);
	w.WriteLong(vertexCount);
	w.WriteLong(vertexWeightCount);
	w.WriteLong(vertexFrameCount);
	w.WriteLong(texCoordCount);
	w.WriteLong(polygonCount);
	w.WriteLong(vertexGroupCount);

	/* Vertices */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];

		flags = freyjaGetVertexFlags(vertex);
		w.WriteLong(flags);

		freyjaGetVertexXYZ3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.WriteFloat32(xyz[j]);

		freyjaGetVertexNormalXYZ3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.WriteFloat32(xyz[j]);

		freyjaGetVertexTexcoord2fv(vertex, uv);
		for (j = 0; j < 2; ++j)
			w.WriteFloat32(uv[j]);
	}

	/* VertexWeights */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		count = freyjaGetVertexWeightCount(vertex);

		for (j = 0; j < count; ++j)
		{
			freyjaGetVertexWeight(vertex, j, &bone, &weight);
			w.WriteLong(verticesMap[vertex]);
			w.WriteLong(bone); // boneIndex -- FIXME: Should be translated
			w.WriteFloat32(weight);
		}
	}

	/* VertexFrames */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		count = freyjaGetVertexFrameCount(vertex);

		for (j = 0; j < count; ++j)
		{
			freyjaGetVertexFrame(vertex, j, &frame, xyz);
			w.WriteLong(verticesMap[vertex]);
			w.WriteLong(frame); // frameIndex -- FIXME: Should be translated
			for (k = 0; k < 3; ++k)
				w.WriteFloat32(xyz[k]);
		}
	}

	/* TexCoords */
	for (i = texcoords.begin(); i < (long)texcoords.end(); ++i)
	{
		texcoord = texcoords[i];

		freyjaGetTexCoord2fv(texcoord, uv);

		for (j = 0; j < 2; ++j)
			w.WriteFloat32(uv[j]);
	}

	/* Polygons */
	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];

		flags = freyjaGetPolygonFlags(idx);
		w.WriteLong(flags);

		material= freyjaGetPolygonMaterial(idx);
		w.WriteLong(material);

		count = freyjaGetPolygonVertexCount(idx);
		w.WriteLong(count);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetPolygonVertexIndex(idx, j);
			w.WriteLong(verticesMap[vertex]);
		}

		count = freyjaGetPolygonTexCoordCount(idx);
		w.WriteLong(count);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetPolygonTexCoordIndex(idx, j);
			w.WriteLong(texcoordsMap[texcoord]);
		}
	}

	return 0;
}


void freyjaMeshClampTexCoords(index_t meshIndex)
{
	uint32 i, item, count;
	vec2_t uv;


	// FIXME: Might have to use gobal texcoords here?

	/* Clamp texcoords */
	count = freyjaGetMeshTexCoordCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		item = freyjaGetMeshTexCoordIndex(meshIndex, i);
		freyjaGetTexCoord2fv(item, uv);

		if (uv[0] < 0.0f)
			uv[0] = 0.0f;
		else if (uv[0] > 1.0f)
			uv[0] = 1.0f;

		if (uv[1] < 0.0f)
			uv[1] = 0.0f;
		else if (uv[1] > 1.0f)
			uv[1] = 1.0f;
		
		freyjaTexCoord2fv(item, uv);
	}


	/* Clamp vertex UVs*/
	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		item = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexTexcoord2fv(item, uv);

		if (uv[0] < 0.0f)
			uv[0] = 0.0f;
		else if (uv[0] > 1.0f)
			uv[0] = 1.0f;

		if (uv[1] < 0.0f)
			uv[1] = 0.0f;
		else if (uv[1] > 1.0f)
			uv[1] = 1.0f;
		
		freyjaVertexTexCoord2fv(item, uv);
	}
}


int32 freyjaLoadModel(const char *filename)
{
	SystemIO::FileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords, bones;
	freyja_file_chunk_t chunk;
	int32 offset, i, j, index, flags;
	vec4_t wxyz;
	vec3_t xyz;
	char buffer[64];
	
	
	if (freyjaCheckModel((char *)filename) != 0)
		return -1;

	if (!r.Open(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.SetByteOrder(SystemIO::File::LITTLE);

	/* Read header */
	offset = r.GetOffset();
	r.ReadString(8, header.magic);
	header.version = r.ReadLong();

	if (header.version != 1)
	{
		r.SetOffset(offset);
		r.ReadString(16, header.magic);
		header.version = r.ReadLong();
	}

	header.flags = r.ReadLong();
	header.reserved = r.ReadLong();
	r.ReadString(64, header.comment);	

	if (strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
	{
		return -1;
	}

	while (!r.IsEndOfFile())
	{
		chunk.type = r.ReadLong();
		chunk.size = r.ReadLong();
		chunk.flags = r.ReadLong();
		chunk.version = r.ReadLong();
		offset = r.GetOffset() + chunk.size;

		switch (chunk.type)
		{
		case FREYJA_CHUNK_BONE:
			freyjaBegin(FREYJA_BONE);
			index = freyjaGetCurrent(FREYJA_BONE);
			bones.pushBack(index);

			memset(buffer, 0, 64);
			r.ReadString(64, buffer);
			freyjaBoneName(index, buffer);
			freyjaBoneFlags(index, 0x0);
			freyjaBoneParent(index, r.ReadLong());
			flags = r.ReadLong();
 
			for (j = 0; j < 3; ++j)
				xyz[j] = r.ReadFloat32();

			freyjaBoneTranslate3fv(index, xyz);
			
			if (flags & 32)
			{
				r.ReadLong();

				for (j = 0; j < 3; ++j)
					xyz[j] = r.ReadFloat32();

				freyjaBoneRotateEuler3fv(index, xyz);
			}
			else
			{
				for (j = 0; j < 4; ++j)
					wxyz[j] = r.ReadFloat32();

				freyjaBoneRotateQuat4fv(index, wxyz);
			}

			freyjaEnd(); // FREYJA_BONE

			if ((long)r.GetOffset() != offset)
				printf("BONE @ %li not %i!\n", r.GetOffset(), offset);
			break;


		/* Materials */
		case FREYJA_CHUNK_MATERIAL:
			{
				FreyjaMaterial *mat;

				index = freyjaMaterialCreate();
				mat = freyjaGetMaterialClass(index);

				if (mat)
				{
					/* Read in new material */
					mat->serialize(r);

					// FIXME use string name matching like skeletalkeyframes
                    // to avoid dupe textures in the future

					if (SystemIO::File::DoesFileExist(mat->getTextureName()))
					{
						index_t textureIndex = freyjaTextureCreateFilename(mat->getTextureName());

						if (textureIndex != INDEX_INVALID)
						{
							mat->mTexture = textureIndex;
							mat->mFlags |= fFreyjaMaterial_Texture;
						}
					}
				}
			}
			break;


		case FREYJA_CHUNK_MESH:
			freyjaLoadMeshChunkV1(r);

			if ((long)r.GetOffset() != offset)
				printf("MESH @ %li not %i!\n", r.GetOffset(), offset);
			break;


		default:
			continue;
		}

		r.SetOffset(offset);
	}


	if (!bones.empty())
	{
		freyjaBegin(FREYJA_SKELETON);
		index_t skeleton = freyjaGetCurrent(FREYJA_SKELETON);

		for (i = bones.begin(); i < (long)bones.end(); ++i)
		{
			freyjaSkeletonAddBone(skeleton, bones[i]);

			for (j = bones.begin(); j < (long)bones.end(); ++j)
			{
				if (bones[i] == (long)freyjaGetBoneParent(bones[j]))
				{
					freyjaBoneAddChild(bones[i], bones[j]);
				}
			} 		
		} 

		freyjaEnd(); // FREYJA_SKELETON
	}

	r.Close();

	return 0;
}


int32 freyjaSaveModel(const char *filename)
{
	Vector<long> vertices, texcoords;
	SystemIO::FileWriter w;
	freyja_file_header_t header;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	//vec4_t wxyz;
	char buffer[64];
	int32 i, j, index, idx, count, meshIndex;


	if (!w.Open(filename))
		return -1;

	memset(header.magic, 0, 16);
	memset(header.comment, 0, 64);
	strncpy(header.magic, FREYJA_API_VERSION, 12);
	header.version = 2;
	header.flags = 0x0;
	header.reserved = 0x0;
	strcpy(header.comment, "Freyja 3d: http://icculus.org/freyja");

	/* Data is LITTLE_ENDIAN */
	w.SetByteOrder(SystemIO::File::LITTLE);

	/* Write header */
	w.WriteString(16, header.magic);
	w.WriteLong(header.version);
	w.WriteLong(header.flags);
	w.WriteLong(header.reserved);
	w.WriteString(64, header.comment);


	/* Write chunks... */


	/* Bones */
	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_RESET);
		index = freyjaIterator(FREYJA_BONE, FREYJA_RESET);
		count = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < count; ++i)
		{
			memset(buffer, 0, 64);
			freyjaGetBoneName(index, 64, buffer);
			idx = freyjaGetBoneParent(index);

			chunk.type = FREYJA_CHUNK_BONE;
			chunk.size = 64 + 4 + 4 + 12 + 16;
			chunk.flags = 0x0;
			chunk.version = 1;

			w.WriteLong(chunk.type);
			w.WriteLong(chunk.size);
			w.WriteLong(chunk.flags);
			w.WriteLong(chunk.version);
			w.WriteString(64, buffer);
			w.WriteLong(idx);
#if QUAT_BACKEND
			w.WriteLong(0x0);
#else
			w.WriteLong(32); // Flag 32 - Using euler angles
#endif

			freyjaGetBoneTranslation3fv(index, xyz);

			for (j = 0; j < 3; ++j)
				w.WriteFloat32(xyz[j]);

#if QUAT_BACKEND
			freyjaGetBoneRotationQuat4fv(index, wxyz);
			
			for (j = 0; j < 4; ++j)
				w.WriteFloat32(wxyz[j]);
#else
			freyjaGetBoneRotationEuler3fv(index, xyz);
			w.WriteLong(0x0); // pad out
			for (j = 0; j < 3; ++j)
				w.WriteFloat32(xyz[j]);
#endif

			index = freyjaIterator(FREYJA_BONE, FREYJA_NEXT);
		}
	}


	/* Skeletons */


	/* Materials */
	if (freyjaGetMaterialCount())
	{
		count = freyjaGetMaterialCount();

		for (i = 0; i < count; ++i)
		{
			FreyjaMaterial *mat = freyjaGetMaterialClass(i);

			if (mat)
			{
				chunk.type = FREYJA_CHUNK_MATERIAL;
				chunk.size = mat->getSerializeSize();
				chunk.flags = 0x0;
				chunk.version = FreyjaMaterial::mVersion;

				w.WriteLong(chunk.type);
				w.WriteLong(chunk.size);
				w.WriteLong(chunk.flags);
				w.WriteLong(chunk.version);

				mat->serialize(w);
			}
		}		
	}


	/* Vertex groups */


	/* Meshes */
	count = freyjaGetModelMeshCount(0);
	printf("***count = %i\n", count);
	for (i = 0; i < count; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(0, i);

		printf("***%i / %i -> %i\n", i, count, meshIndex);
		freyjaSaveMeshChunkV1(w, meshIndex);
	}


	/* Metadata */


	/* That wasn't so bad, was it? */
	w.Close();

	return 0;
}


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	int32 i, count;
	vec3_t xyz;


	freyjaCriticalSectionLock();

	count = freyjaGetCount(FREYJA_VERTEX); 

	list = new Vector<unsigned int>();

	if (count < 1)
	{
		/* Return empty list */
		return list;
	}


	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);
	
	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[2] >= bbox[0][2] && xyz[2] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_CURRENT));
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock();
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(vec3_t bbox[2],
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	freyjaCriticalSectionLock();

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (INDEX_INVALID == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[2] >= bbox[0][2] && xyz[2] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_CURRENT));
				}
			}
		}
	}

	freyjaCriticalSectionUnlock();
	
	return list;
}


void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		freyjaPrintMessage("freyjaGenerateUVFromXYZ> ERROR: Invalid UV");
		return;
	}

	*u = (xyz[0] > 0) ? xyz[0] : -xyz[0];
	s = 0.025;
  
	while (*u > 1.0)
	{
		*u *= s;
		s *= 0.01;
	}
  
	*v = (xyz[1] > 0) ? xyz[1] : -xyz[1];
	s = 0.025;
  
	while (*v > 1.0)
	{
		*v *= s;
		s *= 0.01;
	}
  
	*u = 1.0 - *u;
	*v = 1.0 - *v;
}


void freyjaModelGenerateVertexNormals(index_t modelIndex)
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, vertexCount, faceCount;
	int32 v0, v1, v2, index;
	index_t face;


	freyjaCriticalSectionLock();
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");

	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		face = freyjaGetCurrent(FREYJA_POLYGON);
		v0 = freyjaGetPolygonVertexIndex(face, 0);
		v1 = freyjaGetPolygonVertexIndex(face, 1);
		v2 = freyjaGetPolygonVertexIndex(face, 2);

		freyjaPrintMessage("<%d %d %d>", v0, v1, v2);
		freyjaGetVertexXYZ3fv(v0, a.mVec);
		freyjaGetVertexXYZ3fv(v1, b.mVec);
		freyjaGetVertexXYZ3fv(v2, c.mVec);

		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));

		freyjaIterator(FREYJA_POLYGON, FREYJA_NEXT);
	}

	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);

		if (index < 0)
		{
			freyjaPrintError("freyjaGenerateVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		freyjaGetVertexPolygonRef(ref);

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			normal += *faceNormals[ref[j]];
		}

		normal.normalize();

		freyjaVertexNormal3fv(index,normal.mVec);

		freyjaPrintMessage("%d :: %d faces :: %f %f %f", index,
						   ref.size(),
						   normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
    }

	faceNormals.erase();
	
	freyjaCriticalSectionUnlock();
}


Vector<unsigned int> *freyjaFindVerticesByBoundingVolume(BoundingVolume &vol)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSectionLock();

	count = freyjaGetCount(FREYJA_VERTEX); 

	if (count < 1)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_RESET);
	
	for (i = 0; i < count; ++i)
	{
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);

		freyjaGetVertexXYZ3fv(index, xyz);
		
		if (vol.IsPointInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock();
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesInBox(vec3_t bbox[2],
											  Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSectionLock();

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (INDEX_INVALID == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);
		freyjaGetVertexXYZ3fv(index, xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(index);
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSectionUnlock();
	
	return list;
}


////////////////////////////////////////////////////////////////////

index_t gFreyjaLightIndex = INDEX_INVALID;


index_t freyjaLightCreate()
{
	index_t lightIndex = gFreyjaLights.size();

	gFreyjaLights.pushBack(new FreyjaLight());
	// gFreyjaLights[lightIndex]->mId = lightIndex;

	return lightIndex;	
}


uint32 freyjaGetLightCount()
{
	return gFreyjaLights.size();
}


index_t freyjaGetCurrentLight()
{
	return gFreyjaLightIndex;
}


void freyjaCurrentLight(uint32 lightIndex)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLightIndex = lightIndex;
	}
}


void freyjaLightDelete(int32 lightIndex)
{
}


// Light mutators /////////////////

void freyjaLightPosition4v(uint32 lightIndex, vec4_t position)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->setPosition(position);
	}
}


void freyjaLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mAmbient[0] = ambient[0];
		gFreyjaLights[lightIndex]->mAmbient[1] = ambient[1];
		gFreyjaLights[lightIndex]->mAmbient[2] = ambient[2];
		gFreyjaLights[lightIndex]->mAmbient[3] = ambient[3];
	}
}


void freyjaLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mDiffuse[0] = diffuse[0];
		gFreyjaLights[lightIndex]->mDiffuse[1] = diffuse[1];
		gFreyjaLights[lightIndex]->mDiffuse[2] = diffuse[2];
		gFreyjaLights[lightIndex]->mDiffuse[3] = diffuse[3];
	}
}


void freyjaLightSpecular(uint32 lightIndex, vec4_t specular)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mSpecular[0] = specular[0];
		gFreyjaLights[lightIndex]->mSpecular[1] = specular[1];
		gFreyjaLights[lightIndex]->mSpecular[2] = specular[2];
		gFreyjaLights[lightIndex]->mSpecular[3] = specular[3];
	}
}


// Light accessors /////////////////

void freyjaGetLightPosition4v(uint32 lightIndex, vec4_t position)
{
	if (lightIndex < gFreyjaLights.size())
	{
		position[0] = gFreyjaLights[lightIndex]->mPos[0];
		position[1] = gFreyjaLights[lightIndex]->mPos[1];
		position[2] = gFreyjaLights[lightIndex]->mPos[2];
		position[3] = gFreyjaLights[lightIndex]->mPos[3];
	}
}


void freyjaGetLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	if (lightIndex < gFreyjaLights.size())
	{
		ambient[0] = gFreyjaLights[lightIndex]->mAmbient[0];
		ambient[1] = gFreyjaLights[lightIndex]->mAmbient[1];
		ambient[2] = gFreyjaLights[lightIndex]->mAmbient[2];
		ambient[3] = gFreyjaLights[lightIndex]->mAmbient[3];
	}
}


void freyjaGetLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	if (lightIndex < gFreyjaLights.size())
	{
		diffuse[0] = gFreyjaLights[lightIndex]->mDiffuse[0];
		diffuse[1] = gFreyjaLights[lightIndex]->mDiffuse[1];
		diffuse[2] = gFreyjaLights[lightIndex]->mDiffuse[2];
		diffuse[3] = gFreyjaLights[lightIndex]->mDiffuse[3];
	}
}


void freyjaGetLightSpecular(uint32 lightIndex, vec4_t specular)
{
	if (lightIndex < gFreyjaLights.size())
	{
		specular[0] = gFreyjaLights[lightIndex]->mSpecular[0];
		specular[1] = gFreyjaLights[lightIndex]->mSpecular[1];
		specular[2] = gFreyjaLights[lightIndex]->mSpecular[2];
		specular[3] = gFreyjaLights[lightIndex]->mSpecular[3];
	}
}



void freyjaMeshTesselateTriangles(index_t meshIndex)
{
	Vector<long> purge;
	int32 i, j, polygonCount, polygonIndex, vertexCount, vertexIndex;
	int32 a, b, c, d, ta, tb, tc, td, material;
	unsigned int ii;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		material = freyjaGetPolygonMaterial(polygonIndex);

		if (polygonIndex > -1)
		{
			vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			if (vertexCount < 4)
				continue;
			
			if (vertexCount == 4)
			{
				/* 1. Get ABCD quad vertices */
				a = freyjaGetPolygonVertexIndex(polygonIndex, 0);
				b = freyjaGetPolygonVertexIndex(polygonIndex, 1);
				c = freyjaGetPolygonVertexIndex(polygonIndex, 2);
				d = freyjaGetPolygonVertexIndex(polygonIndex, 3);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					ta = freyjaGetPolygonTexCoordIndex(polygonIndex, 0);
					tb = freyjaGetPolygonTexCoordIndex(polygonIndex, 1);
					tc = freyjaGetPolygonTexCoordIndex(polygonIndex, 2);
					td = freyjaGetPolygonTexCoordIndex(polygonIndex, 3);
				}


				/* 2. Make ABC ACD triangles */
				freyjaBegin(FREYJA_POLYGON);
				freyjaPolygonMaterial1i(material);
				freyjaPolygonVertex1i(a);
				freyjaPolygonVertex1i(b);
				freyjaPolygonVertex1i(c);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					freyjaPolygonTexCoord1i(ta);
					freyjaPolygonTexCoord1i(tb);
					freyjaPolygonTexCoord1i(tc);
				}

				freyjaEnd();

				freyjaBegin(FREYJA_POLYGON);
				freyjaPolygonMaterial1i(material);
				freyjaPolygonVertex1i(a);
				freyjaPolygonVertex1i(c);
				freyjaPolygonVertex1i(d);

				if (freyjaGetPolygonTexCoordCount(polygonIndex))
				{
					freyjaPolygonTexCoord1i(ta); // should dupe a?
					freyjaPolygonTexCoord1i(tc);
					freyjaPolygonTexCoord1i(td);
				}

				freyjaEnd();


				/* 3. Prepare to remove ABCD polygon and update references */
				purge.pushBack(polygonIndex);
			}
			else  // Hhhhmm... can of worms...  doesn't touch polygons atm
			{
				for (j = 0; j < vertexCount; ++j)
				{
					// 0 1 2, 0 2 3, ..
					freyjaPrintError("freyjaMeshTesselateTriangles> No Implementation due to lack of constraints on 5+ edges");
					vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, j);
				}
			}
		}
	}

	for (ii = purge.begin(); ii < purge.end(); ++ii)
	{
		freyjaMeshRemovePolygon(meshIndex, purge[ii]);
	}
}


void freyjaBoneRemoveMesh(index_t boneIndex, index_t meshIndex)
{
	uint32 i, count;

	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		freyjaBoneRemoveVertex(boneIndex, freyjaGetMeshVertexIndex(meshIndex, i));
	}
}


void freyjaBoneAddMesh(index_t boneIndex, index_t meshIndex)
{
	uint32 i, count;

	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		freyjaBoneAddVertex(boneIndex, freyjaGetMeshVertexIndex(meshIndex, i));
	}
}


index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex)
{
	return boneIndex; // Assumes single skeleton model, FIXME when Skeleton added to backend API
}


void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex)
{
	freyjaVertexWeight(vertexIndex, 1.0f, boneIndex);
}


void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v)
{
	vec2_t uv = {u, v};

	freyjaTexCoord2fv(texcoordIndex, uv);
}


uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex)
{
	uint32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	uint32 i, j, frames, maxFrames = 0;
	index_t vertexIndex, vertexCount, polygonIndex;

	/* Mongoose 2005.01.01, 
	 * Vertex frames are stored in the vertices themselves, find the 
	 * max frame count -- indexing greater than frame count will
	 * return frame[0] position in vertices with less than maxFrames
	 * frame, so it's safe and does what one would expect asymmetrically */
	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, j);
			frames = freyjaGetVertexFrameCount(vertexIndex);

			if (frames > maxFrames)
				maxFrames = frames;
		}
	}

	return maxFrames;
}


index_t freyjaCriticalSectionLock()
{
	return INDEX_INVALID;
}


void freyjaCriticalSectionUnlock()
{
}


void freyjaCriticalSectionUnLock(index_t lock)
{
}


//////////////////////////////////////////////////////
// Model
//
//////////////////////////////////////////////////////

uint32 freyjaGetModelCount()
{
	return 1;  // Egg backend is single model
}




char freyjaGetModelAppendMeshMode(index_t modelIndex)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->getAppendMeshMode();
	//}

	return -1;
}


void freyjaModelAppendMeshMode(index_t modelIndex, char on)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	gCopyModels[modelIndex]->setAppendMeshMode(on);
	//}
}


char freyjaModelCopyMesh(index_t modelIndex, index_t mesh, index_t frame)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->copyMesh(mesh, frame);
	//}

	return -1;
}


char freyjaModelPasteMesh(index_t modelIndex)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->pasteMesh();
	//}

	return -1;
}


char freyjaModelCopyVertexList(index_t modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame)
{
	//if (modelIndex < gCopyModels.size())
	//{
	//	return gCopyModels[modelIndex]->copyVertexBuffer(list, mesh, frame);
	//}

	return -1;
}



//////////////////////////////////////////////////////////////////////
// Plugin subsystem
//////////////////////////////////////////////////////////////////////

// TODO Replace FreyjaFSM plugin control


void freyjaPluginDirectoriesInit()
{
#ifdef WIN32
	freyjaPluginAddDirectory("modules/model");
#else
   	freyjaPluginAddDirectory("/usr/lib/freyja/modules/model");
   	freyjaPluginAddDirectory("/usr/local/lib/freyja/modules/model");
	freyjaPluginAddDirectory("/usr/share/freyja/modules/model");
#endif
}


void freyjaPluginAddDirectory(const char *dir)
{
	unsigned int l;
	char *dir2;


	if (!dir || !dir[0] || !SystemIO::File::IsDirectory(dir))
		return;

	l = strlen(dir);

	dir2 = new char[l+1];
	strncpy(dir2, dir, l);
	dir2[l] = 0;

	gPluginDirectories.pushBack(dir2);
}


void freyjaPluginFilename1s(const char *filename)
{

	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->setFilename(filename);
}


int qSort_FreyjaPluginDesc(const void *a, const void *b)
{
	FreyjaPluginDesc *ca = (FreyjaPluginDesc *)a;
	FreyjaPluginDesc *cb = (FreyjaPluginDesc *)b;

#if 0
	if (!ca || !cb || !ca->mExtention || !cb->mExtention)
		return 0;

	return strcmp(ca->mExtention, cb->mExtention);
#else
	if (!ca || !cb || !ca->mDescription || !cb->mDescription)
		return 0;
	return strcmp(ca->mDescription, cb->mDescription);
#endif
}


void freyjaPluginsInit()
{
#ifdef FREYJA_PLUGINS
	SystemIO::FileReader reader;
	FreyjaPluginDesc plugin;
	const char *module_filename;
	void (*import)();
	void *handle;
	unsigned int i;


	gFreyjaPlugins.erase();


	freyjaPluginBegin();
	freyjaPluginDescription1s("All files (*.*)");
	freyjaPluginAddExtention1s("*.*");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	freyjaPluginBegin();
	freyjaPluginDescription1s("Freyja Model (*.ja)");
	freyjaPluginAddExtention1s("*.ja");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while ((module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = freyjaModuleLoad(module_filename)))
//dlopen(module_filename, RTLD_NOW)))
			{
				//freyjaPrintError("In module '%s'.", module_filename);
				
				//if ((error = dlerror()) != NULL)  
				//{
				//	freyjaPrintError("%s", error);
				//}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				import = (void (*)())freyjaModuleImportFunction(handle, "freyja_init");
				//import = (void (*)())dlsym(handle, "freyja_init");

				if (!import)
				//if ((error = dlerror()) != NULL)  
				{
					freyjaModuleUnload(handle);
					//freyjaPrintError("%s", error);
					//dlclose(handle);
					continue;
				}

				freyjaPluginBegin();
				freyjaPluginFilename1s(module_filename);
				(*import)();
				freyjaPluginEnd();

				freyjaModuleUnload(handle);
			}
		}

		reader.Close();
	}

#   if 0   // useless really  =)
	/* Sort the plugins for interface usage */
	gFreyjaPlugins.qSort(qSort_FreyjaPluginDesc);

	foreach (gFreyjaPlugins, i)
	{
		DEBUG_MSG("\t+ '%s', '%s'\n", 
				  gFreyjaPlugins[i]->mDescription,
				  gFreyjaPlugins[i]->mExtention);
	}

#   endif

	gCurrentFreyjaPlugin = -1;
#endif
}


int32 freyjaImportModel(const char *filename)
{
#ifdef FREYJA_PLUGINS
	SystemIO::FileReader reader;
	bool loaded = false, done = false;
	const char *module_filename;
	int (*import)(char *filename);
	void *handle;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]");

	if (!reader.DoesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* Check for native freyja JA format */
	if (freyjaCheckModel(filename) == 0)
	{
		if (freyjaLoadModel(filename) == 0)
			return 0;
		
		return -1;
	}


	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = freyjaModuleLoad(module_filename)))
			{
				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

#ifdef NO_DUPE_DL_SYM_HACK
				/* Mongoose 2004.11.01, 
				 * temp fix? */
				unsigned int l = strlen(module_filename);
				char tmp[128];
				module_filename[l-3] = 0;
				snprintf(tmp, 64, "%s_import_model", basename(module_filename));
				freyjaPrintMessage("Symbol '%s' import...", tmp);
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, tmp);
#else
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, "import_model");
#endif

				if (!import)  
				{
					freyjaModuleUnload(handle);
					continue;
				}


				FreyjaPluginDesc *plug = freyjaGetPluginClassByName(module_filename); 

				if (plug)
					gCurrentFreyjaPlugin = plug->getId(); 

				done = !(*import)((char*)filename);

				gCurrentFreyjaPlugin = -1;

				if (done)
				{
					loaded = true;
					freyjaPrintMessage("Module '%s' success.", module_filename);
				}

				freyjaModuleUnload(handle);
			}
		}

		reader.CloseDir();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#else
	SystemIO::FileReader reader;

	if (!reader.doesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* Check for native freyja JA format */
	if (freyjaCheckModel(filename) == 0)
	{
		if (freyjaLoadModel(filename) == 0)
			return 0;
		
		return -1;
	}

	/* Check for native egg format */
	if (freyja__getEggBackend())
	{
		if (Egg::checkFile(filename) == 0)
		{
			if (freyja__getEggBackend()->loadFile(filename) == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
	}
#endif
	return -1;
}


int32 freyjaExportModel(const char *filename, const char *type)
{
#ifdef FREYJA_PLUGINS
	SystemIO::FileReader reader;
	bool saved = false;
	char module_filename[256];
	char module_export[128];
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	unsigned long i;


	if (!type || !filename)
		return -100;

	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return freyjaSaveModel(filename); // FIXME: true or false needed?
	}

	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");

	name = (char*)type;

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

#ifdef WIN32
		snprintf(module_filename, 255, "%s/%s.dll", gPluginDirectories[i], name);
#else
		snprintf(module_filename, 255, "%s/%s.so", gPluginDirectories[i], name);
#endif
		module_filename[255] = 0;
		snprintf(module_export, 127, "freyja_model__%s_export", name);  // use 'model_export'?
		module_export[127] = 0;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			export_mdl = (int (*)(char * filename))freyjaModuleImportFunction(handle, module_export);

			FreyjaPluginDesc *plug = freyjaGetPluginClassByName(module_filename); 

			if (plug)
				gCurrentFreyjaPlugin = plug->getId(); 

			if (export_mdl)
				saved = (!(*export_mdl)((char*)filename));

			gCurrentFreyjaPlugin = -1;

			freyjaModuleUnload(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // success
#else
	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return freyjaSaveModel(filename); // FIXME: true or false needed?
	}
	else if (strcmp(type, "egg") == 0)
	{
		if (freyja__getEggBackend())
		{
			return freyja__getEggBackend()->saveFile(filename);
		}
	}
#endif
	return -1;
}


void freyjaPluginDescription(uint32 pluginIndex, const char *info_line)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->setDescription(info_line);
	}
}


void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mImportFlags = flags;
	}
}


void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mExportFlags = flags;
	}
}

void freyjaPluginExtention(uint32 pluginIndex, const char *ext)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->setExtention(ext);
	}
}


uint32 freyjaGetPluginCount()
{
	return gFreyjaPlugins.end();
}


FreyjaPluginDesc *freyjaGetPluginClassByName(const char *name)
{
	long i, l;

	if (!name || !name[0])
		return 0x0;

	l = strnlen(name, 8192);

	for (i = gFreyjaPlugins.begin(); i < (long)gFreyjaPlugins.end(); ++i)
	{
		if (gFreyjaPlugins[i] && 
			gFreyjaPlugins[i]->mFilename && gFreyjaPlugins[i]->mFilename[0])
		{
			if (!strncmp(gFreyjaPlugins[i]->mFilename, name, l))
			{
				return gFreyjaPlugins[i];
			}
		}
	}

	return 0x0;
}


FreyjaPluginDesc *freyjaGetPluginClassByIndex(long pluginIndex)
{
	if (pluginIndex > 0 && pluginIndex < (long)gFreyjaPlugins.end())
	{
		return gFreyjaPlugins[pluginIndex];
	}

	return 0x0;
}


void freyjaPluginBegin()
{
	FreyjaPluginDesc *plugin = new FreyjaPluginDesc();
	plugin->setId(gFreyjaPlugins.size());
	gFreyjaPlugins.pushBack(plugin);
	
	gCurrentFreyjaPlugin = plugin->getId();
}


void freyjaPluginDescription1s(const char *info_line)
{
	freyjaPluginDescription(gCurrentFreyjaPlugin, info_line);
}


void freyjaPluginAddExtention1s(const char *ext)
{
	freyjaPluginExtention(gCurrentFreyjaPlugin, ext);
}


void freyjaPluginImport1i(int32 flags)
{
	freyjaPluginImportFlags(gCurrentFreyjaPlugin, flags);
	//freyjaPrintMessage("\tImport: %s%s%s",
	//				   (flags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
	//				   (flags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
	//				   (flags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
}


void freyjaPluginExport1i(int32 flags)
{
	freyjaPluginExportFlags(gCurrentFreyjaPlugin,flags);
	//freyjaPrintMessage("\tExport: %s%s%s",
	//				   (flags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
	//				   (flags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
	//				   (flags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
}

//FIXME these need direct index functions too
void freyjaPluginArg1i(const char *name, int32 defaults)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->addIntArg(name, defaults);
}


void freyjaPluginArg1f(const char *name, float defaults)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->addFloatArg(name, defaults);
}


void freyjaPluginArg1s(const char *name, const char *defaults)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->addStringArg(name, defaults);
}


void freyjaPluginEnd()
{
	// ATM this does nothing, just here for reserved use
}


int32 freyjaGetPluginId()
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		return plugin->getId();

	return -1;
}


int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->getFloatArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->getIntArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->getStringArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArgString(int32 pluginId, const char *name, 
							 int32 len, char *arg)
{
	FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);
	char *s = 0x0;

	if (plugin)
		s = plugin->getStringArg(name);

	if (!s || !s[0])
		return -1;

	strncpy(arg, s, len);

	return 0;
}


///////////////////////////////////////////////////////////////////////
//  Pak VFS 
///////////////////////////////////////////////////////////////////////

#include "FreyjaPakReader.h"

Vector<FreyjaPakReader*> gFreyjaPaks;
uint32 gFreyjaPakCount = 0;


FreyjaPakReader *freyjaGetPakReader(index_t uid)
{
	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		return gFreyjaPaks[uid];
	}

	return 0x0;
}


void freyjaPakDelete(index_t uid)
{
	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		delete gFreyjaPaks[uid];
		gFreyjaPaks.assign(uid, 0x0);
		--gFreyjaPakCount;
	}
}


index_t freyjaPakBegin(const char *filename)
{
	FreyjaPakReader *pak = new FreyjaPakReader(filename);
	uint32 i, count;
	index_t uid;
	bool found = false;


	/* Setup UID and class container reference */
	uid = count = gFreyjaPaks.size();

	for (i = 0; i < count; ++i)
	{
		if (gFreyjaPaks[i] == 0x0)
		{
			uid = i;
			gFreyjaPaks.assign(uid, pak);

			found = true;
			break;
		}	
	}

	if (!found)
	{
		gFreyjaPaks.pushBack(pak);
	}

	++gFreyjaPakCount;

	pak->mUID = uid;

	return uid;
}


void freyjaPakAddFullPathFile(index_t pakIndex,
							  const char *vfsFilename, int32 offset, int32 size)
{
	index_t uid = pakIndex;

	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		gFreyjaPaks[uid]->addFullPathFileDesc(vfsFilename, offset, size);
	}
}


void freyjaPakDecryptBufferXOR1c(unsigned char *buffer, unsigned int size, unsigned char key)
{
	unsigned int i;


	for (i = 0; i < size; ++i)
	{
		buffer[i] ^= key;
	}
}


void freyjaPakAddFullPathFileXOR1c(index_t pakIndex, unsigned char key,
							  	const char *vfsFilename, int32 offset, int32 size)
{
	index_t uid = pakIndex;

	if (uid < gFreyjaPaks.size() && gFreyjaPaks[uid] != 0x0)
	{
		gFreyjaPaks[uid]->addFullPathFileDesc(vfsFilename, offset, size, key);
	}
}


void freyjaPakEnd(index_t pakIndex)
{
	// ATM this does nothing, just here for reserved use
}


///////////////////////////////////////////////////////////////////////
//  Internal ABI calls 
///////////////////////////////////////////////////////////////////////

// Hidden API
void freyja__MeshUpdateMappings(index_t meshIndex)
{
#ifdef USING_EGG
	Vector<long> polygons, texcoords;
	egg_mesh_t *mesh = freyja__getEggBackend()->getMesh(meshIndex);
	int32 i, j, k, count, idx, vertex, texcoord;
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	int32 vertexCount = 0;
	int32 texCoordCount = 0;


	if (!mesh || polygonCount < 1)
		return;

	/* Polygons, filtered by mesh */
	for (i = 0, count = 0; i < polygonCount; ++i)
	{
		idx = freyjaGetMeshPolygonIndex(meshIndex, i);
		count = freyjaGetPolygonEdgeCount(idx);

		/* Weed out invalid polygons */
		//if (count < 3) 
		//{
		//	continue;
		//}

		polygons.pushBack(idx);
	}

	polygonCount = polygons.end();

	/* Vertices and polymapped TexCoords, filtered by filtered polygons */
	mesh->vertices.clear();
	mesh->verticesMap.clear();
	mesh->texcoordsMap.clear();
	mesh->verticesMap.reserve(freyjaGetCount(FREYJA_VERTEX) + 1);
	mesh->texcoordsMap.reserve(freyjaGetCount(FREYJA_TEXCOORD) + 1);

	count = freyjaGetCount(FREYJA_VERTEX);
	for (i = 0; i < count; ++i)
		mesh->verticesMap.pushBack(-1);

	count = freyjaGetCount(FREYJA_TEXCOORD);
	for (i = 0; i < count; ++i)
		mesh->texcoordsMap.pushBack(-1);

	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];
		count = freyjaGetPolygonVertexCount(idx);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetPolygonVertexIndex(idx, j);

			for (k = mesh->vertices.begin(); k < (long)mesh->vertices.end(); ++k)
			{
				if (vertex == mesh->vertices[k])
				{
					vertex = -2;
					break;
				}
			}

			if (vertex > -2)
			{
				/* This generates a 2-way mapping to Egg and mesh ids */
				mesh->verticesMap.assign(vertex, mesh->vertices.size());
				mesh->vertices.pushBack(vertex);
				++vertexCount;
			}
		}


		count = freyjaGetPolygonTexCoordCount(idx);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetPolygonTexCoordIndex(idx, j);

			for (k = texcoords.begin(); k < (long)texcoords.end(); ++k)
			{
				if (texcoord == texcoords[k])
				{
					texcoord = -2;
					break;
				}
			}

			if (texcoord > -2)
			{
				mesh->texcoordsMap.assign(texcoord, texCoordCount);
				++texCoordCount;
				texcoords.pushBack(texcoord);
			}
		}
	}
#else
	BUG_ME("Not Implemented");
#endif
}




///////////////////////////////////////////////////////////////////////
// C++ API 
///////////////////////////////////////////////////////////////////////

int32 freyjaFindPolygonByVertices(Vector<uint32> vertices)
{
#ifdef USING_EGG
	if (freyja__getEggBackend())
		return freyja__getEggBackend()->selectPolygon(&vertices);
#else
	BUG_ME("Not Implemented");
#endif

	return -1;
}


const char *freyjaGetObjectName(freyja_object_t obj)
{
	switch (obj)
	{
	case FREYJA_MODEL:
		return "FREYJA_MODEL";
		break;

	case FREYJA_MESH:
		return "FREYJA_MESH";
		break;

	case FREYJA_POLYGON:
		return "FREYJA_POLYGON";
		break;

	case FREYJA_BONE:
		return "FREYJA_BONE";
		break;

	case FREYJA_SKELETON:
		return "FREYJA_SKELETON";
		break;

	case FREYJA_VERTEX_FRAME:
		return "FREYJA_VERTEX_FRAME";
		break;

	case FREYJA_VERTEX_GROUP:
		return "FREYJA_VERTEX_GROUP";
		break;

	case FREYJA_VERTEX:
		return "FREYJA_VERTEX";
		break;

	case FREYJA_TEXCOORD:
		return "FREYJA_TEXCOORD";
		break;

	case FREYJA_MATERIAL:
		return "FREYJA_MATERIAL";
		break;

	case FREYJA_TEXTURE:
		return "FREYJA_TEXTURE";
		break;

	case FREYJA_SKEL_ANIMATION:
		return "FREYJA_SKEL_ANIMATION";
		break;

	case FREYJA_SKEL_KEYFRAME:
		return "FREYJA_SKEL_KEYFRAME";
		break;
	}

	return "UNKNOWN";
}



