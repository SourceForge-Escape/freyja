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
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "FreyjaFileWriter.h"
#include "EggPlugin.h" // temp for testing
#include "FreyjaPluginABI.h"
#include "Freyja.h"
#include "FreyjaCamera.h"
#include "FreyjaLight.h"
#include "FreyjaSkeleton.h"
#include "FreyjaMaterial.h"


/* Until Freyja replaces Egg backend, let these vectors float here */
Vector<FreyjaSkeletalAnimation *> gFreyjaAnimations; 
Vector<FreyjaMetaData *> gFreyjaMetaData; 
Vector<FreyjaMaterial *> gFreyjaMaterials;
Vector<FreyjaTexture *> gFreyjaTextures;
Vector<FreyjaSkeleton *>  gFreyjaSkeletons;
Vector<FreyjaCamera *>  gFreyjaCamera;
Vector<FreyjaLight *>  gFreyjaLight;


long freyjaCheckModel(const char *filename)
{
	FreyjaFileReader r;
	freyja_file_header_t header;
	long offset;


	if (!r.openFile(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.littleEndianMode();

	/* Read header */
	offset = r.getFileOffset();
	r.readCharString(16, header.magic);
	r.closeFile();

	if (!strncmp(header.magic, FREYJA_PLUGIN_VERSION, 7)) // 'Freyja '
	{
		return 0;
	}

	return -1;
}


long freyjaLoadMeshChunkV1(FreyjaFileReader &r)
{
	Vector<long> verticesMap, texcoordsMap;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	long bone, frame, material;
	long i, j, count, idx, flags;
	long polygonCount;
	long vertexGroupCount;
	long meshFlags;
	long vertexCount;
	long vertexWeightCount;
	long vertexFrameCount;
	long texCoordCount;


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

		idx = freyjaVertex3fv(xyz);

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
		
		freyjaVertexWeight(verticesMap[idx], weight, bone);
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

		idx = freyjaTexCoord2fv(uv);

		texcoordsMap.pushBack(idx);
	}

	/* Polygons */
	for (i = 0; i < polygonCount; ++i)
	{
		freyjaBegin(FREYJA_POLYGON);
		flags = r.readLong();
		material = r.readLong();
		freyjaPolygonMaterial1i(material);

		count = r.readLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.readLong();
			freyjaPolygonVertex1i(verticesMap[idx]);
		}

		count = r.readLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.readLong();
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
 * Vector maps are very memory wasteful */
long freyjaSaveMeshChunkV1(FreyjaFileWriter &w, long meshIndex)
{
	const long version = 1;
	Vector<long> polygons, vertices, texcoords;
	Vector<long> verticesMap, texcoordsMap;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	long flags, bone, frame, material;
	long i, j, k, count, idx, vertex, texcoord;
	long polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	long vertexGroupCount = freyjaGetMeshVertexGroupCount(meshIndex);
	long byteSize = 0;
	long meshFlags = freyjaGetMeshFlags(meshIndex);
	long vertexCount = 0;
	long vertexWeightCount = 0;
	long vertexFrameCount = 0;
	long texCoordCount = 0;


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
	w.writeLong(vertexGroupCount);

	/* Vertices */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];

		flags = freyjaGetVertexFlags(vertex);
		w.writeLong(flags);

		freyjaGetVertexXYZ3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexNormalXYZ3fv(vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexTexCoordUV2fv(vertex, uv);
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
			freyjaGetVertexWeight(vertex, j, &bone, &weight);
			w.writeLong(verticesMap[vertex]);
			w.writeLong(bone); // boneIndex -- FIXME: Should be translated
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
			freyjaGetVertexFrame(vertex, j, &frame, xyz);
			w.writeLong(verticesMap[vertex]);
			w.writeLong(frame); // frameIndex -- FIXME: Should be translated
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


long freyjaLoadModel(const char *filename)
{
	FreyjaFileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords, bones;
	freyja_file_chunk_t chunk;
	long offset, i, j, index, flags;
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

	if (strncmp(header.magic, FREYJA_PLUGIN_VERSION, 7))
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
			freyjaBegin(FREYJA_BONE);
			index = freyjaGetCurrent(FREYJA_BONE);
			bones.pushBack(index);

			memset(buffer, 0, 64);
			r.readCharString(64, buffer);
			freyjaBoneName1s(index, buffer);
			freyjaBoneFlags1i(index, 0x0);
			freyjaBoneParent1i(index, r.readLong());
			flags = r.readLong();
 
			for (j = 0; j < 3; ++j)
				xyz[j] = r.readFloat32();

			freyjaBoneTranslate3fv(index, xyz);
			
			if (flags & 32)
			{
				r.readLong();

				for (j = 0; j < 3; ++j)
					xyz[j] = r.readFloat32();

				freyjaBoneRotateEulerXYZ3fv(index, xyz);
			}
			else
			{
				for (j = 0; j < 4; ++j)
					wxyz[j] = r.readFloat32();

				freyjaBoneRotateQuatWXYZ4fv(index, wxyz);
			}

			freyjaEnd(); // FREYJA_BONE

			if ((long)r.getFileOffset() != offset)
				printf("BONE @ %i not %li!\n", r.getFileOffset(), offset);
			break;


		case FREYJA_CHUNK_MESH:
			freyjaLoadMeshChunkV1(r);

			if ((long)r.getFileOffset() != offset)
				printf("MESH @ %i not %li!\n", r.getFileOffset(), offset);
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
				if (bones[i] == freyjaGetBoneParent(bones[j]))
				{
					freyjaBoneAddChild1i(bones[i], bones[j]);
				}
			} 		
		} 

		freyjaEnd(); // FREYJA_SKELETON
	}

	r.closeFile();

	return 0;
}


long freyjaSaveModel(const char *filename)
{
	Vector<long> vertices, texcoords;
	FreyjaFileWriter w;
	freyja_file_header_t header;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	//vec4_t wxyz;
	char buffer[64];
	long i, j, index, idx, count, meshIndex;


	if (!w.openFile(filename))
		return -1;

	memset(header.magic, 0, 16);
	memset(header.comment, 0, 64);
	strncpy(header.magic, FREYJA_PLUGIN_VERSION, 12);
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
		freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);
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
			freyjaGetBoneRotationWXYZ4fv(index, wxyz);
			
			for (j = 0; j < 4; ++j)
				w.writeFloat32(wxyz[j]);
#else
			freyjaGetBoneRotationXYZ3fv(index, xyz);
			w.writeLong(0x0); // pad out
			for (j = 0; j < 3; ++j)
				w.writeFloat32(xyz[j]);
#endif

			index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}


	/* Skeletons */


	/* Materials */


	/* Vertex groups */


	/* Meshes */
	count = freyjaGetModelMeshCount(0);
	printf("***count = %li\n", count);
	for (i = 0; i < count; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(0, i);

		printf("***%li / %li -> %li\n", i, count, meshIndex);
		freyjaSaveMeshChunkV1(w, meshIndex);
	}


	/* Metadata */


	/* That wasn't so bad, was it? */
	w.closeFile();

	return 0;
}


// TODO parent plugin class

/* UVMap generation utils */
void freyjaGenerateUVMapsFromMesh(long meshIndex)
{
#ifdef FIXME
	egg_texel_t *t = EggPlugin::mEggPlugin->getTexel(freyjaGetCurrent(FREYJA_VERTEX));
	Vector<long> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel = _egg->getTexel(texCoordIndex);
	egg_texel_t *tex;
	Vector3d u, trans, min;
	long i, j, k;


	if (texel)
	{
		for (i = texel->ref.begin(); i < (int)texel->ref.end(); ++i)
		{
			poly = _egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < (int)poly->texel.end(); ++j)
				{
					tex = _egg->getTexel(poly->texel[j]);

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
			tex = _egg->getTexel(seen[k]);

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
		egg_vertex_t *vertex = _egg->getVertex(getCurrentVertex());
		egg_vertex_t *vert;

		for (i = vertex->ref.begin(); i < (int)vertex->ref.end(); ++i)
		{
			poly = _egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < (int)poly->vertex.end(); ++j)
				{
					vert = _egg->getVertex(poly->vertex[j]);

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


/* Mesh generation utils */
void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	long index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 1.0));

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
	long index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index =freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, -0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, -0.33, 0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.5));

	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.75));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 1.0));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.50));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.50));


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


void freyjaGenerateCircleMesh(vec3_t origin, long count)
{
	Vector<long> vertices, texcoords;
	long i, index, center, centerUV;
	vec_t x, z, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.5, 0.5);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		freyjaTexCoord2f(x, z);
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


void freyjaGenerateConeMesh(vec3_t origin, vec_t height, long count)
{
	Vector<long> vertices, texcoords, texcoords2;
	long i, index, point, center, pointUV, centerUV;
	vec_t x, z, u, v, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoord2f(0.75, 0.25);
	point = index;

	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.25, 0.25);
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

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
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
										long count, long segments, 
										float(*gen)(long i, long j, long seg))
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2;
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

			index = freyjaVertex3f(origin[0]+x*r, origin[1]+y*r, origin[2]+z*r);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
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
							  long count, long segments)
{
	Vector<long> segVert, segTex, topTex, bottomTex;
	long i, j, index, index2, top, bottom, bt, tt;
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
			bottom = freyjaVertex3f(origin[0], origin[1]+y, origin[2]);
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
			top = freyjaVertex3f(origin[0], origin[1]+height, origin[2]);
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

			index = freyjaVertex3f(origin[0]+x, origin[1]+y, origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
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

		index = freyjaTexCoord2f(u, v);
		topTex.pushBack(index);
		index = freyjaTexCoord2f(u+0.5, v);
		bottomTex.pushBack(index);
	}

	bt = freyjaTexCoord2f(0.75, 0.25);
	tt = freyjaTexCoord2f(0.25, 0.25);

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
								long count, long segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2, point, center, pointUV, centerUV;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoord2f(0.75, 0.25);
	point = index;

	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.25, 0.25);
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

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertex3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
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

				index = freyjaTexCoord2f(u, v);
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

				index = freyjaTexCoord2f(u, v);
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

			index = freyjaVertex3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
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
							long count, long segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2;
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

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertex3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
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

				index = freyjaTexCoord2f(u, v);
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

				index = freyjaTexCoord2f(u, v);
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

			index = freyjaVertex3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
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
	long i, count;
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
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
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
												  FREYJA_LIST_CURRENT));
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
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
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[2] >= bbox[0][2] && xyz[2] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_LIST_CURRENT));
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
	EggPlugin::mEggPlugin->freyjaPrintMessage(format, &args);
	va_end(args);
}


void freyjaPrintError(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->freyjaPrintError(format, &args);
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
	long v0, v1, v2, index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");

	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);

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

		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

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

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
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
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

		freyjaGetVertexXYZ3fv(index, xyz);
		
		if (vol.isVertexInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
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
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
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

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


void freyjaGetVertexPolygonRef(Vector<long> &polygons)
{
	polygons.clear();

	if (EggPlugin::mEggPlugin)
	{
		egg_vertex_t *v = EggPlugin::mEggPlugin->getVertex(freyjaGetCurrent(FREYJA_VERTEX));
		unsigned int i;

		if (v && !v->ref.empty())
		{
			// if longs ploygons.copy(v->ref);

			for (i = v->ref.begin(); i < v->ref.end(); ++i)
				polygons.pushBack((long)v->ref[i]);
		}
	}
}


////////////////////////////////////////////////////////////////////


void freyjaBegin(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBegin(type);
}


void freyjaEnd()
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaEnd();
}


long freyjaTexCoord2fv(vec2_t uv)
{
	return freyjaTexCoord2f(uv[0], uv[1]);
}


long freyjaTexCoord2f(vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTexCoord2f(u, v);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(xyz[0], xyz[1], xyz[2]);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexFrame3f(long index, vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_vertex_t *v = EggPlugin::mEggPlugin->getVertex(index);

		if (v)
		{
			vec_t *xyz = new vec3_t;
			xyz[0] = x;
			xyz[1] = y;
			xyz[2] = z;
			v->frameId.pushBack(freyjaGetCurrent(FREYJA_VERTEX_FRAME));
			v->frames.pushBack((vec3_t *)xyz);
		}
	}
}


long freyjaVertex3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexWeight(long index, vec_t weight, long bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexWeight(index, weight, bone);
}


void freyjaVertexTexCoord2fv(long vIndex, vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexTexCoord2f(vIndex, uv[0], uv[1]);
}


void freyjaVertexTexCoord2f(long vIndex, vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexTexCoord2f(vIndex, u, v);
}


void freyjaVertexNormal3fv(long vIndex, vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexNormal3f(vIndex, 
													nxyz[0], nxyz[1], nxyz[2]);
}


void freyjaVertexNormal3f(long vIndex, vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexNormal3f(vIndex, x, y, z);
}


void freyjaPolygonAddVertex1i(long polygonIndex, long vertexIndex)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_polygon_t *polygon = EggPlugin::mEggPlugin->getPolygon(polygonIndex);

		if (polygon)
			polygon->vertex.pushBack(vertexIndex);
	}
}


void freyjaPolygonVertex1i(long egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonVertex1i(egg_id);
}


void freyjaPolygonAddTexCoord1i(long polygonIndex, long texcoordIndex)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_polygon_t *polygon = EggPlugin::mEggPlugin->getPolygon(polygonIndex);

		if (polygon)
			polygon->texel.pushBack(texcoordIndex);
	}
}


void freyjaPolygonTexCoord1i(long egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonTexCoord1i(egg_id);
}


void freyjaPolygonSetMaterial1i(long polygonIndex, long materialIndex)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_polygon_t *polygon = EggPlugin::mEggPlugin->getPolygon(polygonIndex);

		if (polygon)
			polygon->shader = materialIndex;
	}
}


void freyjaPolygonMaterial1i(long id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonMaterial1i(id);  
}


unsigned long freyjaGetFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaFlags();

	return 0;
}


char *freyjaGetMeshNameString(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->name;
	}

	return 0x0;
}


long freyjaGetMeshName1s(long meshIndex, long lenght, char *name)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		strncpy(name, mesh->name, lenght);
		name[lenght-1] = 0;
		return 0;
	}

	return -1;
}


void freyjaMeshName1s(long meshIndex, const char *name)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		strncpy(mesh->name, name, 64);
		mesh->name[63] = 0;
	}
}


long freyjaMeshPosition(long meshIndex, vec3_t xyz)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		mesh->position = Vector3d(xyz);
	}

	return 0;
}


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGroupCenter(x, y, z);
}


void freyjaBoneFlags1i(long boneIndex, long flags)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->flag = flags;
	}
}


void freyjaBoneParent1i(long boneIndex, long parentIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->parent = parentIndex;
	}
}


void freyjaBoneName1s(long boneIndex, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		strncpy(bone->name, name, 64);
	}
}


void freyjaBoneAddChild1i(long boneIndex, long childIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		egg_tag_t *child =  EggPlugin::mEggPlugin->getBone(childIndex);
			
		if (child)
			child->parent = bone->id;

		/* If it fails here it's got to be picked up in SKELETON */
		bone->slave.pushBack(childIndex);
	}
}


void freyjaBoneAddMesh1i(long boneIndex, long meshIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		// No check was old policy
		bone->mesh.pushBack(meshIndex);
	}
}


void freyjaBoneTranslate3f(long boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->center[0] = x;
		bone->center[1] = y;
		bone->center[2] = z;
	}
}


void freyjaBoneTranslate3fv(long boneIndex, vec3_t xyz)
{
	freyjaBoneTranslate3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateEulerXYZ3f(long boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->rot[0] = x;
		bone->rot[1] = y;
		bone->rot[2] = z;
	}
}


void freyjaBoneRotateEulerXYZ3fv(long boneIndex, vec3_t xyz)
{
	freyjaBoneRotateEulerXYZ3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateQuatWXYZ4f(long boneIndex,vec_t w,vec_t x,vec_t y,vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		vec_t heading, bank, attitude;
		Quaternion q = Quaternion(w, x, y, z);

		q.getEulerAngles(&heading, &bank, &attitude);

		heading *= 57.295779513082323;
		bank *= 57.295779513082323;
		attitude *= 57.295779513082323;

		EggPlugin::mEggPlugin->freyjaBoneRotate(heading, bank, attitude);
	}
}


void freyjaBoneRotateQuatWXYZ4fv(long boneIndex, vec4_t wxyz)
{
	freyjaBoneRotateQuatWXYZ4f(boneIndex, wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
}




void freyjaMeshTreeFrameAddBone(long tag)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshTreeFrameAddBone(tag);
}


int freyjaGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;

	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureFilename(index, 
															   filename);
	return FREYJA_PLUGIN_ERROR;	
}


int freyjaGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureImage(index, w, h, 
														 depth, type, image);
	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureFilename1s(const char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreFilename(filename);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureStoreBuffer(unsigned char *image, unsigned int depth,
							  unsigned int width, unsigned int height,
							  freyja_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreBuffer(image, depth,
															width, height,
															type);

	return FREYJA_PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

long freyjaGetCount(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCount(type);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaIterator(freyja_object_t type, long item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaIterator(type, item);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaGetTexCoord2fv(long index, vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetTexCoord(index, uv);
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertex(xyz);
}


void freyjaGetVertexTexCoord2fv(vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(uv);
}


void freyjaGetVertexNormal3fv(vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(nxyz);
}


long freyjaGetPolygonVertexCount(long polygonIndex)
{
	egg_polygon_t *polygon = EggPlugin::mEggPlugin->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->vertex.end();
	}

	return 0;
}


long freyjaGetPolygonTexCoordCount(long polygonIndex)
{
	egg_polygon_t *polygon = EggPlugin::mEggPlugin->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->texel.end();
	}

	return 0;
}


long freyjaGetVertexTexCoordUV2fv(long vertexIndex, vec2_t uv)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		uv[0] = vertex->uv[0];
		uv[1] = vertex->uv[1];
		return 0;
	}

	return -1;
}


long freyjaGetVertexNormalXYZ3fv(long vertexIndex, vec3_t nxyz)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		nxyz[0] = vertex->norm[0];
		nxyz[1] = vertex->norm[1];
		nxyz[2] = vertex->norm[2];
		return 0;
	}

	return -1;
}


long freyjaGetVertexXYZ3fv(long vertexIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		xyz[0] = vertex->pos[0];
		xyz[1] = vertex->pos[1];
		xyz[2] = vertex->pos[2];
		return 0;
	}

	return -1;
}


long freyjaGetVertexFrame(long vertexIndex, long element,
						  long *frameIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);


	if (vertex)
	{
		if (element > -1 && element < (int)vertex->frames.end())
		{
			vec_t *v = *(vertex->frames[element]);

			if (element < (int)vertex->frameId.end())
				*frameIndex = vertex->frameId[element];

			xyz[0] = v[0];
			xyz[1] = v[1];
			xyz[2] = v[2];

			return 0;
		}

		/* Invalid indices return orignal position, so you don't get holes */
		xyz[0] = vertex->pos[0];
		xyz[1] = vertex->pos[1];
		xyz[2] = vertex->pos[2];

		return 0;
	}

	return -1;
}


long freyjaGetVertexFrameCount(long vertexIndex)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->frames.end();
	}

	return 0;
}


long freyjaGetVertexWeight(long vertexIndex, long element,
						   long *bone, vec_t *weight)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex && element > -1 && element < (int)vertex->weights.end())
	{
		*bone = vertex->weights[element]->bone;
		*weight = vertex->weights[element]->weight;
		return 0;
	}

	return -1;
}


long freyjaGetVertexWeightCount(long vertexIndex)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->weights.end();
	}

	return 0;
}


long freyjaGetVertexFlags(long vertexIndex)
{
	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->flags;
	}

	return 0;
}


long freyjaGetModelFlags(long modelIndex)
{
	return 0x0;  // Not Implemented due to Egg backend use
}


long freyjaGetModelMeshIndex(long modelIndex, long element)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(element);

	if (mesh)
	{
		return mesh->id;
	}

	return 0;
}


long freyjaGetModelMeshCount(long modelIndex)
{
	// Not Implemented properly due to Egg backend use ( not scene based )

	if (modelIndex == 0)
		return freyjaGetCount(FREYJA_MESH);
	
	return 0;
}


long freyjaGetMeshPosition(long meshIndex, vec3_t xyz)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		xyz[0] = mesh->position.mVec[0];
		xyz[1] = mesh->position.mVec[1];
		xyz[2] = mesh->position.mVec[2];
		return mesh->id;
	}

	return 0;
}


long freyjaGetMeshFlags(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->flags;
	}

	return 0;
}


long freyjaGetMeshVertexFrameIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		// Not Implemented
	}

	return 0;
}


long freyjaGetMeshVertexFrameCount(long meshIndex)
{
	long polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	long i, j, frames, maxFrames = 0, polygonIndex;
	long vertexIndex, vertexCount;

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


long freyjaGetMeshPolygonCount(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->polygon.end();
	}

	return 0;
}


long freyjaGetMeshPolygonVertexIndex(long meshIndex, long faceVertexIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);


	if (mesh && 
		faceVertexIndex > -1 && faceVertexIndex < (long)mesh->verticesMap.size())
	{
		return mesh->verticesMap[faceVertexIndex];
	}

	return -1;
}


long freyjaGetMeshVertexIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);


	if (mesh && 
		element > -1 && element < (long)mesh->vertices.size())
	{
		return mesh->vertices[element];
	}

	return -1;
}


long freyjaGetMeshVertexCount(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		freyja__MeshUpdateMappings(meshIndex); // Setup Egg mesh for export
		return mesh->vertices.size();
	}

	return 0;
}


long freyjaGetMeshVertexGroupCount(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->group.end();
	}

	return 0;
}


long freyjaGetMeshPolygonIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh && element > -1 && element < (int)mesh->polygon.end())
	{
		return mesh->polygon[element];
	}

	return -1;
}

long freyjaGetMeshVertexGroupIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh && element > -1 && element < (int)mesh->group.end())
	{
		return mesh->group[element];
	}

	return -1;
}


long freyjaGetPolygonVertexIndex(long polygonIndex, long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonVertexIndex(polygonIndex, element);

	return -1;
}


long freyjaGetPolygonTexCoordIndex(long polygonIndex, long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonTexCoordIndex(polygonIndex, element);

	return -1;
}


long freyjaGetPolygonMaterial(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonMaterial(polygonIndex);

	return 0;
}


long freyjaGetPolygonFlags(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonFlags(polygonIndex);

	return 0;
}


long freyjaGetPolygonEdgeCount(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonEdgeCount(polygonIndex);

	return 0;
}


long freyjaGetPolygon1u(freyja_object_t type, long item, long *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetPolygon3f(freyja_object_t type, long item, vec_t *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaCriticalSection(freyja_lock_t request)
{
	//freyjaPrintMessage("freyjaCriticalSection> Not implemented, %s:%i\n", 
	//				   __FILE__, __LINE__);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetSkeletonBoneCount(long skeletonIndex)
{
	return freyjaGetCount(FREYJA_BONE); // Atm all one big reference skeleton in Egg backend
}


long freyjaGetSkeletonBoneIndex(long skeletonIndex, long element)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(element);
	// Atm all one big reference skeleton in Egg backend

	if (bone)
	{
		return bone->id;
	}

	return -1;
}


long freyjaGetBoneName(long index, unsigned int size, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


void freyjaBoneParent(long index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(freyjaGetCurrent(FREYJA_BONE));

	if (bone)
	{
		bone->parent = index;
	}	
}


long freyjaGetBoneParent(long index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);

	if (bone)
	{
		return bone->parent;
	}

	return -2;
}


long freyjaGetBoneRotationWXYZ4fv(long index, vec4_t wxyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		Quaternion q = Quaternion(helDegToRad(bone->rot[2]), // roll
								  helDegToRad(bone->rot[0]), // pitch
								  helDegToRad(bone->rot[1]));// yaw
		q.getQuaternion4fv(wxyz);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneRotationXYZ3fv(long index, vec3_t xyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		xyz[0] = bone->rot[0];
		xyz[1] = bone->rot[1];
		xyz[2] = bone->rot[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneTranslation3fv(long index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetCurrent(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCurrent(type);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshFlags1u(flags);	
}


///////////////////////////////////////////////////////////////////////
// Animation ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

FreyjaSkeletalAnimation *freyjaGetAnimation(long animationIndex);


long freyjaAnimationCreate()
{
	long animationIndex = gFreyjaAnimations.size();

	gFreyjaAnimations.pushBack(new FreyjaSkeletalAnimation());
	gFreyjaAnimations[animationIndex]->mId = animationIndex;

	return animationIndex;
	
}


long freyjaAnimationBoneCreate(long animationIndex, 
							   const char *name, long boneIndex)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);


	if (anim)
	{
		return anim->newBoneKeyFrame(name, boneIndex);
	}	

	return -1;
}


long freyjaAnimationBoneKeyFrameCreate(long animationIndex, long boneIndex,
									   vec_t time, vec3_t xyz, vec4_t wxyz)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);


	if (anim)
	{
		return anim->newKeyFrame(boneIndex, time, xyz, wxyz);
	}	

	return -1;
}



/* Animation Accessors */

FreyjaSkeletalAnimation *freyjaGetAnimation(long animationIndex)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		return gFreyjaAnimations[animationIndex]; 
	}	

	return 0x0;
}


long freyjaGetAnimationCount()
{
	return gFreyjaAnimations.size();
}


long freyjaGetAnimationBoneCount(long animationIndex)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		FreyjaSkeletalAnimation *anim = gFreyjaAnimations[animationIndex];

		if (anim)
		{
			return anim->getBoneCount();
		}
	}	

	return -1;	
}


long freyjaGetAnimationBoneKeyFrameCount(long animationIndex, long boneIndex)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		FreyjaSkeletalAnimation *anim = gFreyjaAnimations[animationIndex];

		if (anim)
		{
			return anim->getKeyFrameCountForBone(boneIndex);
		}
	}	

	return 0;	
}


/* Animation Mutators */

void freyjaAnimationName(long animationIndex, const char *name)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->setName(name);
		}
	}
}


void freyjaAnimationFrameRate(long animationIndex, vec_t frameRate)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mFrameRate = frameRate;
		}
	}
}


void freyjaAnimationTime(long animationIndex, vec_t time)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mTime = time;
		}
	}
}


void freyjaAnimationSubsetRoot(long animationIndex, long startBone)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mStartBone = startBone;
		}
	}
}


//void freyjaAnimationSubsetCount(long animationIndex, long boneCount)
//{
//	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
//	{
//		if (gFreyjaAnimations[animationIndex])
//		{
//			gFreyjaAnimations[animationIndex]->mBoneCount = boneCount;
//		}
//	}
//}


void freyjaAnimationBoneName(long animationIndex, long boneIndex,
							 const char *name)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		anim->setBoneName(boneIndex, name);
	}
	
}


void freyjaAnimationKeyFrameTime(long animationIndex, long boneIndex, 
								 long keyFrameIndex, vec_t time)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		FreyjaKeyFrame *keyframe = anim->getBoneKeyFrame(boneIndex, 
														 keyFrameIndex);

		if (keyframe)
			keyframe->setTime(time);
	}
}


void freyjaAnimationKeyFramePosition(long animationIndex, long boneIndex, 
									 long keyFrameIndex, vec3_t position)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		FreyjaKeyFrame *keyframe = anim->getBoneKeyFrame(boneIndex, 
														 keyFrameIndex);

		if (keyframe)
			keyframe->setPosition(position);
	}
}


void freyjaAnimationKeyFrameOrientationXYZ(long animationIndex, long boneIndex, 
										   long keyFrameIndex, vec3_t xyz)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		FreyjaKeyFrame *keyframe = anim->getBoneKeyFrame(boneIndex, 
														 keyFrameIndex);

		if (keyframe)
			keyframe->setOrientationByEuler(xyz);
	}
}


void freyjaAnimationKeyFrameOrientationWXYZ(long animationIndex,
											long boneIndex, 
											long keyFrameIndex,vec4_t wxyz)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		FreyjaKeyFrame *keyframe = anim->getBoneKeyFrame(boneIndex, 
														 keyFrameIndex);

		if (keyframe)
			keyframe->setOrientationByQuaternion(wxyz);
	}
}


///////////////////////////////////////////////////////////////////////
// Material ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

long freyjaMaterialCreate()
{
	long materialIndex = gFreyjaMaterials.size();

	gFreyjaMaterials.pushBack(new FreyjaMaterial());
	gFreyjaMaterials[materialIndex]->mId = materialIndex;

	return materialIndex;
}


/* Material Accessors */

long freyjaGetMaterialCount()
{
	return gFreyjaMaterials.size();
}


long freyjaGetMaterialIndex(long materialIndex, long element)
{
	// This is mainly reserved for future use

	if (element > -1 && element < (long)gFreyjaMaterials.size())
	{
		return element;
	}

	return -1;
}


char *freyjaGetMaterialName(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mName;
	}	

	return 0x0;
}


long freyjaGetMaterialFlags(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mFlags;
	}	

	return -1;
}


long freyjaGetMaterialTexture(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTexture;
	}	

	return -1;
}


void freyjaGetMaterialAmbient(long materialIndex, vec4_t ambient)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			ambient[i] = gFreyjaMaterials[materialIndex]->mAmbient[i];
	}
}


void freyjaGetMaterialDiffuse(long materialIndex, vec4_t diffuse)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			diffuse[i] = gFreyjaMaterials[materialIndex]->mDiffuse[i];
	}
}


void freyjaGetMaterialSpecular(long materialIndex, vec4_t specular)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			specular[i] = gFreyjaMaterials[materialIndex]->mSpecular[i];
	}
}


void freyjaGetMaterialEmissive(long materialIndex, vec4_t emissive)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			emissive[i] = gFreyjaMaterials[materialIndex]->mEmissive[i];
	}
}


vec_t freyjaGetMaterialShininess(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mShininess;
	}	

	return -1.0f;
}


vec_t freyjaGetMaterialTransparency(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTransparency;
	}	

	return -1.0f;
}


long freyjaGetMaterialBlendSource(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendSrc;
	}	

	return -1;
}


vec_t freyjaGetMaterialBlendDestination(long materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendDest;
	}	

	return -1;
}



/* Material Mutators */

void freyjaMaterialName(long materialIndex, const char *name)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		strncpy(gFreyjaMaterials[materialIndex]->mName, name, 64);
		gFreyjaMaterials[materialIndex]->mName[63] = 0;
	}	
}


void freyjaMaterialFlags(long materialIndex, long flags)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags = flags;
	}
}


void freyjaMaterialTexture(long materialIndex, long textureIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTexture = textureIndex;
	}
}


void freyjaMaterialAmbient(long materialIndex, const vec4_t ambient)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mAmbient[i] = ambient[i];
	}
}


void freyjaMaterialDiffuse(long materialIndex, const vec4_t diffuse)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mDiffuse[i] = diffuse[i];
	}
}


void freyjaMaterialSpecular(long materialIndex, const vec4_t specular)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mSpecular[i] = specular[i];
	}
}


void freyjaMaterialEmissive(long materialIndex, const vec4_t emissive)
{
	long i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mEmissive[i] = emissive[i];
	}
}


void freyjaMaterialShininess(long materialIndex, vec_t exponent)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mShininess = exponent;
	}
}


void freyjaMaterialTransparency(long materialIndex, vec_t transparency)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTransparency = transparency;
	}
}


void freyjaMaterialBlendSource(long materialIndex, unsigned long factor)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendSrc = factor;
	}
}


void freyjaMaterialBlendDestination(long materialIndex,	unsigned long factor)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendDest = factor;
	}
}



//////////////////////////////////////////////////////////////////////
// Plugin import / export nicities
//////////////////////////////////////////////////////////////////////

/* TODO hook up to EggPlugin */

void freyjaPluginBegin()
{
	// ATM this does nothing, just here for reserved use
}


void freyjaPluginDescription1s(const char *info_line)
{
	EggPlugin::mEggPlugin->setPluginDescription(info_line);
	//freyjaPrintMessage("\t%s", info_line);
}


void freyjaPluginAddExtention1s(const char *ext)
{
	EggPlugin::mEggPlugin->setPluginExtention(ext);
}


void freyjaPluginImport1i(long flags)
{
	EggPlugin::mEggPlugin->setPluginImportFlags(flags);
	//freyjaPrintMessage("\tImport: %s%s%s",
	//				   (flags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
	//				   (flags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
	//				   (flags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
}


void freyjaPluginExport1i(long flags)
{
	EggPlugin::mEggPlugin->setPluginExportFlags(flags);
	//freyjaPrintMessage("\tExport: %s%s%s",
	//				   (flags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
	//				   (flags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
	//				   (flags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
}


void freyjaPluginArg1i(const char *name, long defaults)
{
	EggPlugin::mEggPlugin->addPluginArgInt(name, defaults);
}


void freyjaPluginArg1f(const char *name, float defaults)
{
	EggPlugin::mEggPlugin->addPluginArgFloat(name, defaults);
}


void freyjaPluginArg1s(const char *name, const char *defaults)
{
	EggPlugin::mEggPlugin->addPluginArgString(name, defaults);
}


void freyjaPluginEnd()
{
	// ATM this does nothing, just here for reserved use
}


long freyjaGetPluginId()
{
	return EggPlugin::mEggPlugin->getPluginId();
}


int freyjaGetPluginArg1f(long pluginId, const char *name, float *arg)
{
	*arg = EggPlugin::mEggPlugin->getPluginArgFloat(pluginId, name);
	 return -1;
}


int freyjaGetPluginArg1i(long pluginId, const char *name, long *arg)
{
	*arg = EggPlugin::mEggPlugin->getPluginArgInt(pluginId, name);
	 return -1;
}


int freyjaGetPluginArg1s(long pluginId, const char *name, char **arg)
{
	*arg = EggPlugin::mEggPlugin->getPluginArgString(pluginId, name);
	return 0;
}


int freyjaGetPluginArgString(long pluginId, const char *name, 
							 long len, char *arg)
{
	char *s = EggPlugin::mEggPlugin->getPluginArgString(pluginId, name);

	if (!s || !s[0])
		return 0x0;

	strncpy(arg, s, len);

	return 0;
}


///////////////////////////////////////////////////////////////////////
//  Pak VFS 
///////////////////////////////////////////////////////////////////////

long freyjaPakBegin(const char *filename)
{
	// ATM this does nothing, just here for reserved use
	return -1;
}


long freyjaPakAddFullPathFile(long pakIndex,
							  const char *vfsFilename, long offset, long size)
{
	// ATM this does nothing, just here for reserved use
	return -1;
}


void freyjaPakEnd(long pakIndex)
{
	// ATM this does nothing, just here for reserved use
}


///////////////////////////////////////////////////////////////////////
//  Internal ABI calls 
///////////////////////////////////////////////////////////////////////

void freyja__MeshUpdateMappings(long meshIndex)
{
	Vector<long> polygons, texcoords;
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);
	long i, j, k, count, idx, vertex, texcoord;
	long polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	long vertexCount = 0;
	long texCoordCount = 0;


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
}



///////////////////////////////////////////////////////////////////////
//  Deprecated ABI 
///////////////////////////////////////////////////////////////////////

#ifdef DEPRECATED_FREYJA_PLUGIN_ABI
long freyjaGetBoneMeshCount()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshCount();

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneMeshIndex(long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshIndex(element);

	return FREYJA_PLUGIN_ERROR;
}
#endif
