/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongooe
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <math.h>
#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "ActionManager.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "Material.h"
#include "Printer.h"
#include "Plugin.h"
#include "PluginABI.h"
#include "VertexABI.h"
#include "MaterialABI.h"
#include "SkeletonABI.h"
#include "BoneABI.h"
#include "MeshABI.h"
#include "freyja.h"

#ifdef WIN32
#   undef MODEL_PLUGINS
#endif

#ifdef MODEL_PLUGINS
#   include <dlfcn.h> 
#endif

#ifndef FIXME
#   undef MODEL_PLUGINS
#endif


using namespace freyja;


Printer *gPrinter = 0x0;


byte freyjaIsTexCoordAllocated(index_t texcoordIndex)
{
	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

	return 0;
}


byte freyjaIsPolygonAllocated(index_t polygonIndex)
{
	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

	return 0;
}




//////////////////////////////////////////////////////////


int32 freyjaCheckModel(const char *filename)
{
	FreyjaFileReader r;
	freyja_file_header_t header;
	int32 offset;


	if (!r.openFile(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.littleEndianMode();

	/* Read header */
	offset = r.getFileOffset();
	r.readCharString(16, header.magic);
	r.closeFile();

	if (!strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
	{
		return 0;
	}

	return -1;
}


int32 freyjaLoadMeshChunkV1(FreyjaFileReader &r)
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
	index_t face;


	/* Read from diskfile */
	meshFlags = r.readLong();
	vertexCount = r.readLong();
	vertexWeightCount = r.readLong();
	vertexFrameCount = r.readLong();
	texCoordCount = r.readLong();
	polygonCount = r.readLong();
	vertexGroupCount = r.readLong();

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);
	
	/* Vertices */
	for (i = 0; i < vertexCount; ++i)
	{
		flags = r.readLong();

		for (j = 0; j < 3; ++j)
			xyz[j] = r.readFloat32();

		idx = freyjaVertexCreate3fv(xyz);

		for (j = 0; j < 3; ++j)
			xyz[j] = r.readFloat32();

		freyjaVertexNormal3fv(idx, xyz);

		for (j = 0; j < 2; ++j)
			uv[j] = r.readFloat32();
		
		freyjaVertexTexCoord2fv(idx, uv);

		verticesMap.pushBack(idx);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP

	/* VertexWeights */
	for (i = 0; i < vertexWeightCount; ++i)
	{
		idx = r.readLong();
		bone = r.readLong();
		weight = r.readFloat32();
		
		freyjaVertexAddWeight(verticesMap[idx], weight, bone);
	}

	/* VertexFrames */
	for (i = 0; i < vertexFrameCount; ++i)
	{
		idx = r.readLong();
		frame = r.readLong(); // Reserved use
		for (j = 0; j < 3; ++j)
			xyz[j] = r.readFloat32();
		
		freyjaVertexFrame3f(verticesMap[idx], xyz[0], xyz[1], xyz[2]);
	}

	/* TexCoords */
	for (i = 0; i < texCoordCount; ++i)
	{
		for (j = 0; j < 2; ++j)
			uv[j] = r.readFloat32();

		idx = freyjaTexCoordCreate2fv(uv);

		texcoordsMap.pushBack(idx);
	}

	/* Polygons */
	for (i = 0; i < polygonCount; ++i)
	{
		face = freyjaPolygonCreate();
		//freyjaBegin(FREYJA_POLYGON);

		flags = r.readLong();
		material = r.readLong();
		freyjaPolygonMaterial(face, material);

		count = r.readLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.readLong();
			freyjaPolygonAddVertex(face, verticesMap[idx]);

			freyjaGetVertexPosition3fv(verticesMap[idx], xyz);
		}

		count = r.readLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.readLong();
			freyjaPolygonAddTexCoord(face, texcoordsMap[idx]);
		}

		//FIXME freyjaMeshAddPolygon(mesh, face); 
		//freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	return 0;
}


/* This is to fix all the problems with the backend delete corruption
 * It filters and realigns indices to only save valid data, not
 * the entire junky backend cruft like undo data which remains
 *
 * Vector maps are very memory wasteful */
int32 freyjaSaveMeshChunkV1(FreyjaFileWriter &w, int32 meshIndex)
{
	const int32 version = 1;
	Vector<long> polygons, vertices, texcoords;
	Vector<long> verticesMap, texcoordsMap;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	int32 flags, material; // bone, frame;
	int32 i, j, k, count, idx, vertex, texcoord;
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
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
	byteSize += 4; // vertexGroupCount wasn't used in v9 anyway, treat as reserved

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


	/* Write to diskfile */
	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = byteSize;
	chunk.flags = 0x0;
	chunk.version = version;

	w.writeLong(chunk.type);
	w.writeLong(chunk.size);
	w.writeLong(chunk.flags);
	w.writeLong(chunk.version);

	w.writeLong(meshFlags);
	w.writeLong(vertexCount);
	w.writeLong(vertexWeightCount);
	w.writeLong(vertexFrameCount);
	w.writeLong(texCoordCount);
	w.writeLong(polygonCount);
	w.writeLong(0); // vertexGroupCount wasn't used in v9 anyway

	/* Vertices */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];

		flags = freyjaGetVertexFlags(vertex);
		w.writeLong(flags);

		freyjaGetVertexPosition3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexNormal3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexTexCoord2fv(vertex, uv);
		for (j = 0; j < 2; ++j)
			w.writeFloat32(uv[j]);
	}

	/* VertexWeights */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		count = freyjaGetVertexWeightCount(vertex);

		for (j = 0; j < count; ++j)
		{
			index_t boneIndex;
			//FIXME -1 <=> INVALID_INDEX

			freyjaGetVertexWeight(vertex, j, &boneIndex, &weight);
			w.writeLong(verticesMap[vertex]);
			w.writeLong(boneIndex); // boneIndex -- FIXME: Should be translated
			w.writeFloat32(weight);
		}
	}

	/* VertexFrames */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		count = freyjaGetVertexFrameCount(vertex);

		for (j = 0; j < count; ++j)
		{
			index_t frameIndex;
			//FIXME -1 <=> INVALID_INDEX

			freyjaGetVertexFrame(vertex, j, &frameIndex, xyz);
			w.writeLong(verticesMap[vertex]);
			w.writeLong(frameIndex); // frameIndex -- FIXME: Should be translated
			for (k = 0; k < 3; ++k)
				w.writeFloat32(xyz[k]);
		}
	}

	/* TexCoords */
	for (i = texcoords.begin(); i < (long)texcoords.end(); ++i)
	{
		texcoord = texcoords[i];

		freyjaGetTexCoord2fv(texcoord, uv);

		for (j = 0; j < 2; ++j)
			w.writeFloat32(uv[j]);
	}

	/* Polygons */
	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];

		flags = freyjaGetPolygonFlags(idx);
		w.writeLong(flags);

		material= freyjaGetPolygonMaterial(idx);
		w.writeLong(material);

		count = freyjaGetPolygonVertexCount(idx);
		w.writeLong(count);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetPolygonVertexIndex(idx, j);
			w.writeLong(verticesMap[vertex]);
		}

		count = freyjaGetPolygonTexCoordCount(idx);
		w.writeLong(count);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetPolygonTexCoordIndex(idx, j);
			w.writeLong(texcoordsMap[texcoord]);
		}
	}

	return 0;
}


int32 freyjaLoadModel(const char *filename)
{
	FileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords, bones;
	freyja_file_chunk_t chunk;
	int32 offset, i, j, index, flags;
	vec4_t wxyz;
	vec3_t xyz;
	char buffer[64];


	if (freyjaCheckModel((char *)filename) != 0)
		return -1;

	if (!r.openFile(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.littleEndianMode();

	/* Read header */
	offset = r.getFileOffset();
	r.readCharString(8, header.magic);
	header.version = r.readLong();

	if (header.version != 1)
	{
		r.setFileOffset(offset);
		r.readCharString(16, header.magic);
		header.version = r.readLong();
	}

	header.flags = r.readLong();
	header.reserved = r.readLong();
	r.readCharString(64, header.comment);	

	if (strncmp(header.magic, FREYJA_API_VERSION, 7))
	{
		return -1;
	}

	while (!r.endOfFile())
	{
		chunk.type = r.readLong();
		chunk.size = r.readLong();
		chunk.flags = r.readLong();
		chunk.version = r.readLong();
		offset = r.getFileOffset() + chunk.size;

		switch (chunk.type)
		{
		case FREYJA_CHUNK_BONE:
			//freyjaBegin(FREYJA_BONE);
			//index = freyjaGetCurrent(FREYJA_BONE);
			index = freyjaBoneCreate(0); // only 1 skeleton in v9
			bones.pushBack(index);

			memset(buffer, 0, 64);
			r.readCharString(64, buffer);
			freyjaBoneName(index, buffer);
			freyjaBoneFlags(index, 0x0);
			freyjaBoneParent(index, r.readLong());
			flags = r.readLong();
 
			for (j = 0; j < 3; ++j)
				xyz[j] = r.readFloat32();

			freyjaBoneTranslate3fv(index, xyz);
			
			if (flags & 32)
			{
				r.readLong();

				for (j = 0; j < 3; ++j)
					xyz[j] = HEL_DEG_TO_RAD(r.readFloat32());

				freyjaBoneRotateEuler3fv(index, xyz);
			}
			else
			{
				for (j = 0; j < 4; ++j)
					wxyz[j] = r.readFloat32();

				freyjaBoneRotateQuat4fv(index, wxyz);
			}

			//freyjaEnd(); // FREYJA_BONE

			if ((long)r.getFileOffset() != offset)
				printf("BONE @ %i not %i!\n", r.getFileOffset(), offset);
			break;


		/* Materials */
		case FREYJA_CHUNK_MATERIAL:
			{
				FreyjaMaterial *mat;

				index = freyjaMaterialCreate();
				mat = Material::getMaterial(index);

				if (mat)
					mat->serialize(r);		
			}
			break;


		case FREYJA_CHUNK_MESH:
			freyjaLoadMeshChunkV1(r);

			if ((long)r.getFileOffset() != offset)
				printf("MESH @ %i not %i!\n", r.getFileOffset(), offset);
			break;


		default:
			continue;
		}

		r.setFileOffset(offset);
	}


	if (!bones.empty())
	{
		freyjaBegin(FREYJA_SKELETON);

		for (i = bones.begin(); i < (long)bones.end(); ++i)
		{
			for (j = bones.begin(); j < (long)bones.end(); ++j)
			{
				if (bones[i] == (int)freyjaGetBoneParent(bones[j]))
				{
					freyjaBoneAddChild(bones[i], bones[j]);
				}
			}
		} 

		freyjaEnd(); // FREYJA_SKELETON
	}

	r.closeFile();

	return 0;
}


int32 freyjaSaveModel(const char *filename)
{
	Vector<long> vertices, texcoords;
	FreyjaFileWriter w;
	freyja_file_header_t header;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	//vec4_t wxyz;
	char buffer[64];
	int32 i, j, index, idx, count, meshIndex;


	if (!w.openFile(filename))
		return -1;

	memset(header.magic, 0, 16);
	memset(header.comment, 0, 64);
	strncpy(header.magic, FREYJA_API_VERSION, 12);
	header.version = 2;
	header.flags = 0x0;
	header.reserved = 0x0;
	strcpy(header.comment, "Freyja 3d: http://icculus.org/freyja");

	/* Data is LITTLE_ENDIAN */
	w.littleEndianMode();

	/* Write header */
	w.writeCharString(16, header.magic);
	w.writeLong(header.version);
	w.writeLong(header.flags);
	w.writeLong(header.reserved);
	w.writeCharString(64, header.comment);


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

			w.writeLong(chunk.type);
			w.writeLong(chunk.size);
			w.writeLong(chunk.flags);
			w.writeLong(chunk.version);
			w.writeCharString(64, buffer);
			w.writeLong(idx);
#ifdef QUAT_BACKEND
			w.writeLong(0x0);
#else
			w.writeLong(32); // Flag 32 - Using euler angles
#endif

			freyjaGetBoneTranslation3fv(index, xyz);

			for (j = 0; j < 3; ++j)
				w.writeFloat32(xyz[j]);

#ifdef QUAT_BACKEND
			freyjaGetBoneRotation4fv(index, wxyz);
			
			for (j = 0; j < 4; ++j)
				w.writeFloat32(wxyz[j]);
#else
			freyjaGetBoneRotation3fv(index, xyz);
			w.writeLong(0x0); // pad out
			for (j = 0; j < 3; ++j)
				w.writeFloat32(xyz[j]);
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
			FreyjaMaterial *mat = Material::getMaterial(i);

			if (mat)
			{
				chunk.type = FREYJA_CHUNK_MATERIAL;
				chunk.size = mat->getSerializeSize();
				chunk.flags = 0x0;
				chunk.version = FreyjaMaterial::mVersion;

				w.writeLong(chunk.type);
				w.writeLong(chunk.size);
				w.writeLong(chunk.flags);
				w.writeLong(chunk.version);

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
	w.closeFile();

	return 0;
}


// TODO parent plugin class


/* Mesh generation utils */
void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	int32 index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 1.0));

	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON


	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	int32 index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index =freyjaVertexCreate3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, -0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertexCreate3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, -0.33, 0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoordCreate2f(0.0, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.0, 0.25));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.25, 0.5));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.5));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.75));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 1.0));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.0));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.25));

	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoordCreate2f(1.0, 0.50));
	texcoords.pushBack(freyjaTexCoordCreate2f(0.75, 0.50));


	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[4]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[5]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonTexCoord1i(texcoords[6]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[7]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[8]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[9]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[10]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[11]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[12]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[13]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[14]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[15]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[16]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[17]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[18]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[19]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[20]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[21]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[22]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[23]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateCircleMesh(vec3_t origin, int32 count)
{
	Vector<long> vertices, texcoords;
	int32 i, index, center, centerUV;
	vec_t x, z, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.5, 0.5);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		freyjaTexCoordCreate2f(x, z);
		texcoords.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateConeMesh(vec3_t origin, vec_t height, int32 count)
{
	Vector<long> vertices, texcoords, texcoords2;
	int32 i, index, point, center, pointUV, centerUV;
	vec_t x, z, u, v, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoordCreate2f(0.75, 0.25);
	point = index;

	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateFunctionalSphereMesh(vec3_t origin, vec_t radius, 
										int32 count, int32 segments, 
										float(*gen)(int32 i, int32 j, int32 seg))
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	height = radius * 2.0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			r = (*gen)(i, j, segments);
			r *= radius;

			index = freyjaVertexCreate3f(origin[0]+x*r, origin[1]+y*r, origin[2]+z*r);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 1; i < segments-1; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  int32 count, int32 segments)
{
	Vector<long> segVert, segTex, topTex, bottomTex;
	int32 i, j, index, index2, top, bottom, bt, tt;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	height = radius * 2.0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
		y = height * ((float)i/(float)segments);
		v = 0.5 * ((float)i/(float)segments) + 0.5;

		r = sin(helDegToRad(180 * ((float)i/(float)segments)));
		(i > segments/2) ? y = height*(1.0-r)+y*r : y *= r;

		if (i == 0)
		{
			bottom = freyjaVertexCreate3f(origin[0], origin[1]+y, origin[2]);
			freyjaVertexNormal3f(bottom, 0.0, -1.0, 0.0);

			/* Padding, if iterator needs alignment */
			for (j = 0; j < count; ++j)
			{
				segVert.pushBack(-1);
				segTex.pushBack(-1);
			}
			continue;
		}
		else if (i == segments-1)
		{
			top = freyjaVertexCreate3f(origin[0], origin[1]+height, origin[2]);
			freyjaVertexNormal3f(top, 0.0, 1.0, 0.0);
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));

			u = 1.0 * ((float)j/(float)count);

			nx = x * r;
			ny = (1.0 - r) * (i < segments/2) ? -0.3 : 0.3; // soften the halves
			nz = z * r;

			x *= r * radius;
			z *= r * radius;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y, origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		
		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		index = freyjaTexCoordCreate2f(u, v);
		topTex.pushBack(index);
		index = freyjaTexCoordCreate2f(u+0.5, v);
		bottomTex.pushBack(index);
	}

	bt = freyjaTexCoordCreate2f(0.75, 0.25);
	tt = freyjaTexCoordCreate2f(0.25, 0.25);

	/* Tube */
	for (i = 0; i < segments-1; ++i)
	{
		if (i == 0)
		{
			index = count * 1;

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(bottomTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(bottomTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}

			continue;
		}
		else if (i == segments-2)
		{
			index = count * (i + 1);

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(topTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(topTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}
		}

		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								int32 count, int32 segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2, point, center, pointUV, centerUV;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertexCreate3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoordCreate2f(0.75, 0.25);
	point = index;

	index = freyjaVertexCreate3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoordCreate2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertexCreate3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		/* Reuse top vertices for segment-th ring */
		if (i == segments)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices2[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices2[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices2[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices2[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices2[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							int32 count, int32 segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	int32 i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertexCreate3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoordCreate2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertexCreate3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoordCreate2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		/* Reuse top vertices for segment-th ring */
		if (i == segments)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices2[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	int32 i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

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

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(vec3_t bbox[2],
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
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

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


void freyjaPrintMessage(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);	

	if (gPrinter)
	{
		gPrinter->messageArgs(format, &args);
	}
	else
	{
		vfprintf(stdout, format, args);
		printf("\n");
	}

	va_end(args);
}


void freyjaPrintError(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);	

	if (gPrinter)
	{
		gPrinter->errorArgs(format, &args);
	}
	else
	{
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n");
	}

	va_end(args);
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


void freyjaGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, vertexCount, faceCount;
	int32 v0, v1, v2, index;
	index_t polygon;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");

	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		polygon = freyjaGetCurrent(FREYJA_POLYGON);

		v0 = freyjaGetPolygonVertexIndex(polygon, 0);
		v1 = freyjaGetPolygonVertexIndex(polygon, 1);
		v2 = freyjaGetPolygonVertexIndex(polygon, 2);

		freyjaPrintMessage("<%d %d %d>", v0, v1, v2);
		freyjaGetVertexPosition3fv(v0, a.mVec);
		freyjaGetVertexPosition3fv(v1, b.mVec);
		freyjaGetVertexPosition3fv(v2, c.mVec);

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

		uint32 refCount = freyjaGetVertexPolygonRefCount(freyjaGetCurrent(FREYJA_VERTEX));

		for (j = 0; j < refCount; ++j)
		{
			uint32 idx = freyjaGetVertexPolygonRefIndex(freyjaGetCurrent(FREYJA_VERTEX), j);

			if (idx != INDEX_INVALID)
				normal += *faceNormals[idx];
		}

		normal.normalize();

		freyjaVertexNormal3fv(index,normal.mVec);

		freyjaPrintMessage("%d :: %d faces :: %f %f %f", index,
						   ref.size(),
						   normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
    }

	faceNormals.erase();
	
	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBoundingVolume(BoundingVolume &vol)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

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

		freyjaGetVertexPosition3fv(index, xyz);
		
		if (vol.isVertexInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesInBox(vec3_t bbox[2],
											  Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_CURRENT);
		freyjaGetVertexPosition3fv(index, xyz);
		
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

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}



////////////////////////////////////////////////////////////////////

	

// General /////////

void freyjaBegin(freyja_object_t type)
{
	freyjaPrintMessage("freyjaBegin> Call is obsolete %s:%i",
					   __FILE__, __LINE__);
}


void freyjaEnd()
{
	freyjaPrintMessage("freyjaEnd> Call is obsolete %s:%i",
					   __FILE__, __LINE__);
}


void freyja__PolygonReplaceReference(int32 polygonIndex, 
									 int32 vertexA, int32 vertexB)
{
#ifdef FIXME
	Vector<unsigned int> ref;
	Vertex *a = 0x0, *b = 0x0;
	egg_polygon_t *polygon = 0x0;
	unsigned int i;


	if (EggPlugin::mEggPlugin)
	{
		a = Vertex::getVertex(vertexA);
		b = Vertex::getVertex(vertexB);
		polygon = 0x0; // FIXME EggPlugin::mEggPlugin->getPolygon(polygonIndex);
	}
	
	if (!a || !b || !polygon)
	{
		return;
	}

	/* Replace A with B to match sorted list ids to form same edge */
	polygon->vertices.Replace(vertexA, vertexB);
 	polygon->r_vertex.clear();

 	for (i = polygon->vertex.begin(); i < polygon->vertex.end(); ++i)
 	{
 		polygon->r_vertex.pushBack(EggPlugin::mEggPlugin->getVertex(polygon->vertex[i]));
 	}

	// Add polygonIndex to B's reference list
	b->ref.pushBack(polygonIndex);

	// Remove polygonIndex from A's reference list
	for (i = a->ref.begin(); i < a->ref.end(); ++i)
	{
		if ((int)a->ref[i] != polygonIndex)
		{
			ref.pushBack(a->ref[i]);
		}
	}

	a->ref.clear();
	a->ref.copy(ref);
#endif
}


void freyja__GetCommonPolygonReferences(index_t vertexA, index_t vertexB,
										Vector<index_t> &common)
{
	unsigned i, j, countA, countB;
	index_t polyA, polyB;

	common.clear();

	countA = freyjaGetVertexPolygonRefCount(vertexA);
	countB = freyjaGetVertexPolygonRefCount(vertexB);

	for (i = 0; i < countA; ++i)
	{
		polyA = freyjaGetVertexPolygonRefIndex(vertexA, i);

		for (j = 0; j < countB; ++j)
		{
			polyB = freyjaGetVertexPolygonRefIndex(vertexB, j);

			if (polyA == polyB)
				common.pushBack(polyA);
		}
	}
}


void freyja__GetDifferenceOfPolygonReferences(int32 vertexA, int32 vertexB,
											  Vector<unsigned int> &diffA)
{
	uint32 i, j, countA, countB;
	index_t polyA, polyB;	
	bool failed;

	diffA.clear();
	
	countA = freyjaGetVertexPolygonRefCount(vertexA);
	countB = freyjaGetVertexPolygonRefCount(vertexB);

	for (i = 0; i < countA; ++i)
	{
		failed = false;
		polyA = freyjaGetVertexPolygonRefIndex(vertexA, i);

		for (j = 0; j < countB; ++j)
		{
			polyB = freyjaGetVertexPolygonRefIndex(vertexB, j);

			if (polyA == polyB)
			{
				failed = true;
				break;
			}
		}
		
		if (!failed)
		{
			diffA.pushBack(polyA);
		}
	}
}


// NOTE meshIndex isn't used with Egg backend!!!

void freyjaPolygonSplitTexCoords(uint32 meshIndex, uint32 polygonIndex)
{	
	int32 polygonCount, vertexCount, vertexIndex, texcoordCount, texcoordIndex;
	int32 i;
	vec2_t uv;


	polygonCount = freyjaGetPolygonVertexCount(polygonIndex);
	

	if (polygonCount == 0)
		return;

	vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
	texcoordCount = freyjaGetPolygonTexCoordCount(polygonIndex);

	if (texcoordCount < vertexCount)
	{
		freyjaPolygonTexCoordPurge(polygonIndex);

		for (i = 0; i < vertexCount; ++i) 
		{
			vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, i);
			freyjaGetVertexTexCoord2fv(vertexIndex, uv);
			texcoordIndex = freyjaTexCoordCreate2fv(uv);
			freyjaPolygonAddTexCoord(polygonIndex, texcoordIndex);
		}
	}
	else
	{
		Vector<int32> texcoords;

		
		for (i = 0; i < texcoordCount; ++i) 
		{
			texcoordIndex = freyjaGetPolygonTexCoordIndex(polygonIndex, i);
			freyjaGetTexCoord2fv(texcoordIndex, uv);

			//printf("%i. %f %f\n", texcoordIndex, uv[0], uv[1]);
			texcoords.pushBack(texcoordIndex);
		}

		freyjaPolygonTexCoordPurge(polygonIndex);

		for (i = 0; i < texcoordCount; ++i) 
		{
			texcoordIndex = texcoords[i];
			freyjaGetTexCoord2fv(texcoordIndex, uv);

			//printf("%i. %f %f\n", texcoordIndex, uv[0], uv[1]);

			texcoordIndex = freyjaTexCoordCreate2fv(uv);
			freyjaPolygonAddTexCoord(polygonIndex, texcoordIndex);
		}
		
	}
}






void freyjaPolygonSplit(int32 meshIndex, int32 polygonIndex)
{
	Vector<unsigned int> common, face;
	Vector<long> ref;
	Vector3d a, b, c, d, m1, m2, n1, n2, n;
	vec2_t uv1, uv2, uv;
	int32 A, B, C, D, M1, M2, material, count;


	material = freyjaGetPolygonMaterial(polygonIndex);
	count = freyjaGetPolygonVertexCount(polygonIndex);

	if (!count)
		return;

	switch (count)
	{
	case 4:

		// 1. Generate midpoint vertices
		A = freyjaGetPolygonVertexIndex(polygonIndex, 0);
		freyjaGetVertexPosition3fv(A, a.mVec);
		B = freyjaGetPolygonVertexIndex(polygonIndex, 1);
		freyjaGetVertexPosition3fv(B, b.mVec);
		C = freyjaGetPolygonVertexIndex(polygonIndex, 2);
		freyjaGetVertexPosition3fv(C, c.mVec);
		D = freyjaGetPolygonVertexIndex(polygonIndex, 3);
		freyjaGetVertexPosition3fv(D, d.mVec);

		helMidpoint3v(a.mVec, b.mVec, m1.mVec);
		M1 = freyjaVertexCreate3fv(m1.mVec);
		freyjaGetVertexTexCoord2fv(A, uv1);
		freyjaGetVertexTexCoord2fv(B, uv2);
		uv[0] = (uv1[0] + uv2[0]) / 2;
		uv[1] = (uv1[1] + uv2[1]) / 2;
		freyjaVertexTexCoord2fv(M1, uv);
		freyjaGetVertexNormal3fv(A, n1.mVec);
		freyjaGetVertexNormal3fv(B, n2.mVec);
		n = n1 + n2;
		n.normalize();
		freyjaVertexNormal3fv(M1, n.mVec);

		helMidpoint3v(c.mVec, d.mVec, m2.mVec);
		M2 = freyjaVertexCreate3fv(m2.mVec);
		freyjaGetVertexTexCoord2fv(C, uv1);
		freyjaGetVertexTexCoord2fv(D, uv2);
		uv[0] = (uv1[0] + uv2[0]) / 2;
		uv[1] = (uv1[1] + uv2[1]) / 2;
		freyjaVertexTexCoord2fv(M2, uv);
		freyjaGetVertexNormal3fv(C, n1.mVec);
		freyjaGetVertexNormal3fv(D, n2.mVec);
		n = n1 + n2;
		n.normalize();
		freyjaVertexNormal3fv(M2, n.mVec);

		// 2. Generate the 2 new quad faces
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(A);
		freyjaPolygonVertex1i(M1);
		freyjaPolygonVertex1i(M2);
		freyjaPolygonVertex1i(D);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.5));
		}

		freyjaEnd();

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(B);
		freyjaPolygonVertex1i(M1);
		freyjaPolygonVertex1i(M2);
		freyjaPolygonVertex1i(C);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.5));
		}

		freyjaEnd();

		// 3. Remove old face we split
		freyjaMeshRemovePolygon(meshIndex, polygonIndex);
		break;

	default:
		;
	}
}


int freyjaPolygonExtrudeQuad1f(int32 polygonIndex, vec_t dist)
{
	Vector3d faceNormal, a, b, c;
	int32 i, v, count;

	if (freyjaIterator(FREYJA_POLYGON, polygonIndex) == FREYJA_ERROR)
		return -1;

	// 1. Get face normal
	faceNormal.zero();

	/* Generated face normal, which won't match preclac vertex normalsx */
	//freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);
	//freyjaGetVertexPosition3fv(v0, a.mVec);
	//freyjaGetVertexPosition3fv(v1, b.mVec);
	//freyjaGetVertexPosition3fv(v2, c.mVec);
	//faceNormal = Vector3d::cross(a - b, c - b);
	
	/* Use existing vertex normals ( perfered, also like n-patch ) */
	count = freyjaGetPolygonVertexCount(polygonIndex);
	for (i = 0; i < count; ++i)
	{
		v = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexNormal3fv(v, a.mVec);
		faceNormal += a;
	}


	// 2. Scale face normal by dist
	faceNormal.normalize();
	faceNormal *= dist;

	freyjaPolygonExtrudeQuad(polygonIndex, faceNormal.mVec);

	return 0;
}


void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t normal)
{
	Vector<unsigned int> common, face;
	Vector<long> ref;
	vec3_t xyz;
	vec2_t uv;
	unsigned int j;
	int32 A, B, C, D, i, material, count;


	material = freyjaGetPolygonMaterial(polygonIndex);
	count = freyjaGetPolygonVertexCount(polygonIndex);

	if (!count)
		return;

	for (i = 0; i < count; ++i)
	{
		/* 1. Make duplicate vertices with same wind for 'face' */
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexPosition3fv(A, xyz);
		B = freyjaVertexCreate3fv(xyz);
		freyjaGetVertexTexCoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv);
		freyjaGetVertexNormal3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz);
		
		face.pushBack(B);


		/* 2. Replace all references to A with B ( dupe of A ), 
		 * except polygonIndex */
		uint32 refCount = freyjaGetVertexPolygonRefCount(A);

		for (j = ref.begin(); j < refCount; ++j)
		{
			index_t refIndex = freyjaGetVertexPolygonRefIndex(A, j);

			if (refIndex != polygonIndex)
			{
				freyja__PolygonReplaceReference(refIndex, A, B);
			}
		}
	}

	for (i = 0; i < count; ++i)
	{
		// 3. Generate new quad ABCD connecting 'face' and ploygonIndex vertices
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		B = face[i];

		if (i+1 < count)
		{
			C = freyjaGetPolygonVertexIndex(polygonIndex, i+1);
			D = face[i+1];
		}
		else
		{
			C = freyjaGetPolygonVertexIndex(polygonIndex, 0);
			D = face[0];
		}

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(A);
		freyjaPolygonVertex1i(C);
		freyjaPolygonVertex1i(D);
		freyjaPolygonVertex1i(B);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.5));
		}

		freyjaEnd();
	}

	// FIXME: Should recompute normals for 'face' vertices here


	// Move polygonIndex vertices by 'normal'
	for (i = 0; i < count; ++i)
	{
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexPosition3fv(A, xyz);
		xyz[0] += normal[0];
		xyz[1] += normal[1];
		xyz[2] += normal[2];
		freyjaVertexPosition3fv(A, xyz);
	}
}


void freyjaVertexExtrude(index_t vertexIndex, vec_t midpointScale, vec3_t normal)
{
	Vector<long> faces, edges;
	Vector3d a, b, c, ab, ac;
	vec3_t xyz;
	vec2_t uv;
	index_t v, polygonIndex;
	int32 A, B, C, i, j, material, polygonCount, vertexCount, texcoordCount, last;


	uint32 refCount = freyjaGetVertexPolygonRefCount(vertexIndex);
	polygonCount = refCount;

	if (!polygonCount)
		return;

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetVertexPolygonRefIndex(vertexIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		texcoordCount = freyjaGetPolygonTexCoordCount(polygonIndex);
		material = freyjaGetPolygonMaterial(polygonIndex);


		// 1. Get edges containing vertexIndex in ith face
		edges.clear();
		
		for (j = 0, last = -1; j < vertexCount; ++j)
		{
			v = freyjaGetPolygonVertexIndex(polygonIndex, j);

			if (v == vertexIndex)
			{
				if (j == vertexCount - 1)
				{
					A = v;
					B = freyjaGetPolygonVertexIndex(polygonIndex, 0);
					C = last;
				}
				else
				{
					A = v;
					B = freyjaGetPolygonVertexIndex(polygonIndex, j+1);
					C = last;

					if (j == 0)
					{
						C = freyjaGetPolygonVertexIndex(polygonIndex, 
														vertexCount - 1);
					}
				}
				break;
			}

			last = v;
		}


		// 2. Compute midpoint of edges AB AC and scale by midpointScale
		freyjaGetVertexPosition3fv(A, a.mVec);
		freyjaGetVertexPosition3fv(B, b.mVec);
		freyjaGetVertexPosition3fv(C, c.mVec);
		ab = a + b;
		ab *= 0.5 * midpointScale;
		ac = a + c;
		ac *= 0.5 * midpointScale;


		// 3. Generate new vertices at scaled midpoints of edges
		B = freyjaVertexCreate3fv(ab.mVec);
		freyjaGetVertexTexCoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv); // FIXME
		freyjaGetVertexNormal3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz); // FIXME

		C = freyjaVertexCreate3fv(ac.mVec);
		freyjaGetVertexTexCoord2fv(A, uv);
		freyjaVertexTexCoord2fv(C, uv); // FIXME
		freyjaGetVertexNormal3fv(A, xyz);
		freyjaVertexNormal3fv(C, xyz); // FIXME


		// 4. Replace references to vertexIndex with each new vertex
		freyja__PolygonReplaceReference(polygonIndex, A, B);
		//freyja__PolygonInsertReferenceAfter(polygonIndex, B, C);

		// 5. Generate new a face ABC for new vertices and vertexIndex
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(material);
		freyjaPolygonVertex1i(A);
		freyjaPolygonVertex1i(B);
		freyjaPolygonVertex1i(C);

		// FIXME: Should be able to generate mixing both uvs
		if (freyjaGetPolygonTexCoordCount(polygonIndex))
		{
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(0.5, 0.5));
		}

		freyjaEnd();
	}


	// 6. Move vertexIndex vertex by 'normal'
	freyjaGetVertexPosition3fv(vertexIndex, xyz);
	xyz[0] += normal[0];
	xyz[1] += normal[1];
	xyz[2] += normal[2];
	freyjaVertexPosition3fv(vertexIndex, xyz);
}



// Accesors /////////////////////////////////////

uint32 freyjaGetCount(freyja_object_t type)
{
	freyjaPrintMessage("freyjaGetCount> Call is obsolete %s:%i",
					   __FILE__, __LINE__);
	return 0;
}


index_t freyjaIterator(freyja_object_t type, int32 item)
{
	freyjaPrintMessage("freyjaIterator> Call is obsolete %s:%i",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

	return FREYJA_ERROR;
}


int32 freyjaGetModelFlags(int32 modelIndex)
{
	return 0x0;  // Not Implemented due to Egg backend use
}


int32 freyjaGetModelMeshIndex(int32 modelIndex, int32 element)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	Mesh *mesh = 0x0; // FIXME EggPlugin::mEggPlugin->getMesh(element);

	if (mesh)
	{
		return mesh->getUID();
	}

	return 0;
}


index_t freyjaGetCurrent(freyja_object_t type)
{

	freyjaPrintMessage("FIXME: %s:%i, file bug with %s",
					   __FILE__, __LINE__, EMAIL_ADDRESS);

	return FREYJA_ERROR;
}


//////////////////////////////////////////////////////
// Model
//
//////////////////////////////////////////////////////

// FIXME: Uses Egg
void freyjaModelMirrorTexCoord(uint32 modelIndex, uint32 texCoordIndex,
								Vector<int32> uvMap, bool x, bool y)
{
#ifdef FIXME
	Egg *egg;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !EggPlugin::mEggPlugin)
		return;

	egg = EggPlugin::mEggPlugin->getEgg();

	if (!egg)
		return;

	Vector<long> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel = egg->getTexel(texCoordIndex);
	egg_texel_t *tex;
	Vector3d u, trans, min;
	long i, j, k;

	if (texel && !uvMap.empty())
	{
		for (i = uvMap.begin(); i < (int)uvMap.end(); ++i)
		{
			poly = egg->getPolygon(uvMap[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < (int)poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->texel[j])
						{
							tex = 0x0;
							break;
						}
					}

					if (!tex) 
						continue;

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < trans.mVec[0])
						trans.mVec[0] = u.mVec[0];

					if (u.mVec[1] < trans.mVec[1])
						trans.mVec[1] = u.mVec[1];

					if (x) tex->st[0] = -tex->st[0];
					if (y) tex->st[1] = -tex->st[1];

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < min.mVec[0])
						min.mVec[0] = u.mVec[0];

					if (u.mVec[1] < min.mVec[1])
						min.mVec[1] = u.mVec[1];

					seen.pushBack(poly->texel[j]);
				}
			}
		}

		u = trans;
		trans = trans - min;
		//trans += Vector3d(u.mVec[0]/2, u.mVec[1]/2, 0);

		for (k = seen.begin(); k < (int)seen.end(); ++k)
		{
			tex = egg->getTexel(seen[k]);

			if (tex)
			{
				u = Vector3d(tex->st[0], tex->st[1], 0) + trans;

				tex->st[0] = u.mVec[0];
				tex->st[1] = u.mVec[1];
			}
		}

		return;
	}


	if (texel)
	{
		for (i = texel->ref.begin(); i < (int)texel->ref.end(); ++i)
		{
			poly = egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < (int)poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->texel[j])
						{
							tex = 0x0;
							break;
						}
					}

					if (!tex) 
						continue;

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < trans.mVec[0])
						trans.mVec[0] = u.mVec[0];

					if (u.mVec[1] < trans.mVec[1])
						trans.mVec[1] = u.mVec[1];

					if (x) tex->st[0] = -tex->st[0];
					if (y) tex->st[1] = -tex->st[1];

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < min.mVec[0])
						min.mVec[0] = u.mVec[0];

					if (u.mVec[1] < min.mVec[1])
						min.mVec[1] = u.mVec[1];

					seen.pushBack(poly->texel[j]);
				}
			}
		}

		u = trans;
		trans = trans - min;
		//trans += Vector3d(u.mVec[0]/2, u.mVec[1]/2, 0);

		for (k = seen.begin(); k < (int)seen.end(); ++k)
		{
			tex = egg->getTexel(seen[k]);

			if (tex)
			{
				u = Vector3d(tex->st[0], tex->st[1], 0) + trans;

				tex->st[0] = u.mVec[0];
				tex->st[1] = u.mVec[1];
			}
		}
	}
	else
	{
		egg_vertex_t *vertex = egg->getVertex(freyjaGetCurrentVertex());
		egg_vertex_t *vert;

		for (i = vertex->ref.begin(); i < (int)vertex->ref.end(); ++i)
		{
			poly = egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < (int)poly->vertex.end(); ++j)
				{
					vert = egg->getVertex(poly->vertex[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->vertex[j])
						{
							vert = 0x0;
							break;
						}
					}

					if (!vert) 
						continue;

					if (x) vert->uv[0] = -vert->uv[0];
					if (y) vert->uv[1] = -vert->uv[1];
					seen.pushBack(poly->vertex[j]);
				}
			}
		}
	}
#endif
}


// FIXME: Uses Egg
void freyjaModelTransformTexCoord(uint32 modelIndex, uint32 texCoordIndex,
									freyja_transform_action_t action,
									vec_t x, vec_t y)
{
#ifdef FIXME
	Egg *egg;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !EggPlugin::mEggPlugin)
		return;

	egg = EggPlugin::mEggPlugin->getEgg();

	if (!egg)
		return;

	Vector<int32> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel = egg->getTexel(texCoordIndex);
	egg_texel_t *tex;
	Vector3d u, v, p;
	int32 i, j, k;
	Matrix m;
	vec_t z;


	if (texel)
	{
		p = Vector3d(texel->st[0], texel->st[1], 0);
	}
	else
	{
		egg_vertex_t *vertex = egg->getVertex(freyjaGetCurrentVertex());
		p = Vector3d(vertex->uv[0], vertex->uv[1], 0);
	}

	switch (action)
	{
	case fTranslate:
		m.translate(x, y, 0);
		break;

	case fRotateAboutPoint:
		m.translate(-p.mVec[0], -p.mVec[1], 0);
		x = helDegToRad(x);
		y = helDegToRad(0);
		z = helDegToRad(0);
		m.rotate(0, 0, helDegToRad(90));
		m.rotate(x, y, z);
		m.translate(p.mVec[0], p.mVec[1], 0);
		break;

	case fScaleAboutPoint:
		m.translate(-p.mVec[0], -p.mVec[1], 0);
		m.scale(x, y, 1);
		m.translate(p.mVec[0], p.mVec[1], 0);
		break;

	case fRotate:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(0);
		m.rotate(x, y, z);
		break;

	case fScale:
		m.scale(x, y, 1);
		break;

	default:
		freyjaPrintMessage("transformTexCoord: Unknown action");
		return;
	}


	if (texel)
	{
		v = Vector3d(texel->st[0], texel->st[1], 0);
		v = m * v;

		for (i = texel->ref.begin(); i < (int)texel->ref.end(); ++i)
		{
			poly = egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < (int)poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->texel[j])
						{
							tex = 0x0;
							break;
						}
					}

					if (!tex) 
						continue;

					u = Vector3d(tex->st[0], tex->st[1], 0);
					u = m * u;

					tex->st[0] = u.mVec[0];
					tex->st[1] = u.mVec[1];
					seen.pushBack(poly->texel[i]);
				}
			}
		}
	}
	else
	{
		egg_vertex_t *vertex = egg->getVertex(freyjaGetCurrentVertex());
		egg_vertex_t *vert;

		v = Vector3d(vertex->uv[0], vertex->uv[1], 0);
		v = m * v;

		for (i = vertex->ref.begin(); i < (int)vertex->ref.end(); ++i)
		{
			poly = egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < (int)poly->vertex.end(); ++j)
				{
					vert = egg->getVertex(poly->vertex[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->vertex[j])
						{
							vert = 0x0;
							break;
						}
					}

					if (!vert) 
						continue;

					u = Vector3d(vert->uv[0], vert->uv[1], 0);
					u = m * u;
					vert->uv[0] = u.mVec[0];
					vert->uv[1] = u.mVec[1];
					seen.pushBack(poly->vertex[j]);
				}
			}
		}
	}
#endif
}


void freyjaModelClampTexCoords(uint32 modelIndex)
{
#ifdef FIXME
	uint32 i;

	for (i = 0; i < freyjaGetTexelCount(); ++i)
	{
		t = egg->getTexel(i);

		if (t)
		{
			if (t->st[0] < 0.0f)
				t->st[0] = 0.0f;
			else if (t->st[0] > 1.0f)
				t->st[0] = 1.0f;

			if (t->st[1] < 0.0f)
				t->st[1] = 0.0f;
			else if (t->st[1] > 1.0f)
				t->st[1] = 1.0f;
		}
	}

	for (i = 0; i < egg->getVertexCount(); ++i)
	{
		v = egg->getVertex(i);

		if (v)
		{
			if (v->uv[0] < 0.0f)
				v->uv[0] = 0.0f;
			else if (v->uv[0] > 1.0f)
				v->uv[0] = 1.0f;

			if (v->uv[1] < 0.0f)
				v->uv[1] = 0.0f;
			else if (v->uv[1] > 1.0f)
				v->uv[1] = 1.0f;
		}
	}
#endif
}


///////////////////////////////////////////////////////////////////////
//  Stubs to allow linking to tracer build of this library
///////////////////////////////////////////////////////////////////////

int32 freyjaGetTextureImage(index_t textureIndex,
								  uint32 *w, uint32 *h, 
								  uint32 *depth, uint32 *type, byte **image)
	{ return -1; }

index_t freyjaIterator(freyja_object_t type, index_t item) {return INDEX_INVALID;}

//index_t freyjaTexCoord2fv(vec2_t uv) {return INDEX_INVALID;}
void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv) {}

void freyjaPolygonVertexDeleteHandler(Vector<unsigned>, unsigned) {}

index_t freyjaGetModelMeshIndex(index_t, uint32) { return INDEX_INVALID;}

uint32 freyjaGetModelMeshCount(index_t) { return 0;}

void freyjaVertexPosition3fv(index_t, const vec3_t) {}

void freyjaVertexFrame3f(index_t index, vec_t x, vec_t y, vec_t z) {}

uint32 freyjaGetPluginCount() {	return 0; }

byte freyjaGetPolygonFlags(index_t) { return 0; }

void freyjaPluginsInit() {}

void freyjaPluginDirectoriesInit() {}

index_t freyjaCriticalSection(freyja_lock_t) { return INDEX_INVALID; }

uint32 freyjaGetVertexFrameCount(index_t) { return 0; }

index_t freyjaTexCoordCreate2fv(vec2_t) { return INDEX_INVALID; }

void freyjaGetVertex3fv(vec3_t) { }

void freyjaGetTexCoord2fv(index_t, vec2_t) {}

index_t freyjaTexCoordCreate2f(vec_t u, vec_t v) { return INDEX_INVALID; }


uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex) { return 0; }

uint32 freyjaGetPolygonVertexCount(index_t polygonIndex) { return 0; }

void freyjaPolygonTexCoordPurge(index_t) {}

uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex) { return 0; }

index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element){ return INDEX_INVALID; }

index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element){ return INDEX_INVALID; }
index_t freyjaGetPolygonMaterial(index_t polygonIndex) { return INDEX_INVALID; }

void freyjaPolygonTexCoord1i(index_t texcoordIndex) {}
void freyjaPolygonVertex1i(index_t vertexIndex) {}
void freyjaPolygonMaterial1i(index_t materialIndex) {}





///////////////////////////////////////////////////////////////////////
//  Internal aka Managed API
///////////////////////////////////////////////////////////////////////

void freyja__MeshUpdateMappings(int32 meshIndex)
{
#ifdef FIXME
	Vector<long> polygons, texcoords;
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);
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
#endif
}


void freyja__setPrinter(Printer *printer, bool freyjaManaged)
{
	if (!printer)
		return;

	if (freyjaManaged)
	{
		if (gPrinter && gPrinter != printer)
		{
			delete gPrinter;
		}

		gPrinter = printer;
	}
}


///////////////////////////////////////////////////////////////////////
// Managed API 
///////////////////////////////////////////////////////////////////////

void freyjaSpawn()
{
	// FIXME
	if (!gPrinter)//EggPlugin::mEggPlugin)
	{
		//Egg *egg = new Egg();
		//EggPlugin *eggplugin = new EggPlugin(egg);
		//
		//eggplugin->getEgg();

		/* Setup basic default stdout printer */
		freyja__setPrinter(new Printer(), true);

		/* Spawn cut/copy/paste system */
		//freyja__spawnCopyModel(egg);

		/* Setup plugins */
		freyjaPluginDirectoriesInit();
		freyjaPluginsInit();

		freyjaPrintMessage("libfreyja invoked using freyjaSpawn()");
	}
}


void freyjaFree()
{
	freyjaPrintMessage("libfreyja stopping on freyjaFree()");

	if (gPrinter)
	{
		delete gPrinter;
	}

	//gPluginDirectories.erase();
	//gFreyjaPlugins.erase();
}

