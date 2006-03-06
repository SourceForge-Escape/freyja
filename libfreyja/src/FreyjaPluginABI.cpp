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
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "ActionManager.h"
#include "CopyModel.h"
#include "FreyjaCamera.h"
#include "FreyjaFSM.h"
#include "FreyjaFileWriter.h"
#include "FreyjaImage.h"
#include "FreyjaLight.h"
#include "FreyjaMaterial.h"
#include "FreyjaScene.h"
#include "FreyjaTexture.h"

#include "FreyjaPluginABI.h"

#include "Egg.h" // Left over from 9.1 API, but 9.5 it's gone


/* Internal / hidden API methods not exported by header */
Egg *freyja__getEggBackend();


/* Until Freyja replaces Egg backend, let these vector pools float here */
Vector<FreyjaMetaData *> gFreyjaMetaData; 
Vector<FreyjaCamera *>  gFreyjaCameras;
Vector<FreyjaLight *>  gFreyjaLights;
Vector<CopyModel *>  gCopyModels;
Vector<FreyjaPluginDesc *> gFreyjaPlugins;
Vector<char *> gPluginDirectories;

FreyjaPrinter *gPrinter = 0x0;
Egg *gEgg = 0x0;
int32 gCurrentFreyjaPlugin = -1;
long FreyjaPluginDesc::mNextId = 1;
index_t gFreyjaCurrentVertex = INDEX_INVALID;
index_t gFreyjaCurrentMesh = INDEX_INVALID;


// OBSOLETE
uint32 freyjaGetAnimationCount()
{
	return 0;
}


// OBSOLETE
uint32 freyjaGetAnimationFrameCount(index_t animationIndex)
{	
	return 0;
}


index_t freyjaGetCurrentModelIndex()
{
	return 0; // Egg is single model
}


index_t freyjaGetCurrentVertex()
{
	return gFreyjaCurrentVertex;
}


void freyjaCurrentVertex(index_t vertexIndex)
{
	gFreyjaCurrentVertex = vertexIndex;
}


char freyjaIsTexCoordAllocated(index_t texcoordIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_texel_t *texcoord;
	
	if (!egg)
		return 0;

	texcoord = egg->getTexel(texcoordIndex);

	return (texcoord != 0x0);
}


char freyjaIsPolygonAllocated(index_t polygonIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg && egg->getPolygon(polygonIndex))
		return 1;

	return 0;
}


char freyjaIsVertexAllocated(index_t vertexIndex)
{
	egg_vertex_t *vertex;
	

	if (!gEgg || vertexIndex == INDEX_INVALID)
		return 0;

	vertex = gEgg->getVertex(vertexIndex);

	return (vertex != 0x0);
}


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


index_t freyjaMeshCreate()
{
	Egg *egg = freyja__getEggBackend();
	egg_mesh_t *mesh;
	egg_group_t *grp;

	if (egg)
	{
		mesh = egg->newMesh();
		egg->addMesh(mesh);

		grp = egg->newGroup();
		egg->addGroup(grp);
  
		mesh->group.pushBack(grp->id);
		
		freyjaPrintMessage("Mesh[%i] made with group %i", mesh->id, grp->id);
		
		return mesh->id;
	}

	return INDEX_INVALID;
}


index_t freyjaGetCurrentMesh()
{
	return gFreyjaCurrentMesh;
}


void freyjaMeshDelete(index_t meshIndex)
{
	gEgg->delMesh(meshIndex);
}


void freyjaMeshTransform(index_t meshIndex, uint32 frame,
							freyja_transform_action_t action, 
							vec_t x, vec_t y, vec_t z)
{
	enum Egg::egg_transform type;
	Egg *egg = freyja__getEggBackend();

	// FIXME: Only one model -- ignore model index for now

	if (!egg)
		return;

	switch (action)
	{
	case fTranslate:
		type = Egg::TRANSLATE;
		break;

	case fRotate:
		type = Egg::ROTATE;
		break;

	case fScale:
		type = Egg::SCALE;
		break;

	case fScaleAboutPoint:
		freyjaPrintMessage("freyjaModelTransform> No appropreiate transform, file bug with %", EMAIL_ADDRESS);
		return;
		break;

	case fRotateAboutPoint:
		type = Egg::ROTATE_ABOUT_CENTER;
		break;
	}

	egg->Transform(egg->getMesh(meshIndex), type, x, y, z);

	egg_group_t *group = egg->getGroup(frame);
	egg_mesh_t *mesh = egg->getMesh(meshIndex);

	if (group && mesh)
		mesh->position = Vector3d(group->center);	
}


void freyjaMeshFrameTransform(index_t meshIndex, uint32 frame,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z)
{
	enum Egg::egg_transform type;
	Egg *egg = freyja__getEggBackend();

	// FIXME: Only one model -- ignore model index for now

	if (!egg)
		return;

	switch (action)
	{
	case fTranslate:
		type = Egg::TRANSLATE;
		break;

	case fRotate:
		type = Egg::ROTATE;
		break;

	case fScale:
		type = Egg::SCALE;
		break;

	case fScaleAboutPoint:
		freyjaPrintMessage("freyjaModelTransform> No appropreiate transform, file bug with %", EMAIL_ADDRESS);
		return;
		break;

	case fRotateAboutPoint:
		type = Egg::ROTATE_ABOUT_CENTER;
		break;
	}

	egg->Transform(egg->getGroup(frame), type, x, y, z);
}





void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (!egg)
		return;

	egg_mesh_t *mesh = egg->getMesh(meshIndex);
	egg_polygon_t *polygon = egg->getPolygon(polygonIndex);

	if (mesh && polygon)
	{
		mesh->polygon.pushBack(polygon->id);
		mesh->r_polygon.pushBack(polygon);
	}
}


void freyjaModelTransform(index_t modelIndex,
							freyja_transform_action_t action, 
							vec_t x, vec_t y, vec_t z)
{
	enum Egg::egg_transform type;
	Egg *egg = 0x0;

	// FIXME: Only one model -- ignore model index for now

	egg = freyja__getEggBackend();

	if (!egg)
		return;

	switch (action)
	{
	case fTranslate:
		type = Egg::TRANSLATE;
		break;

	case fRotate:
		type = Egg::ROTATE;
		break;

	case fScale:
		type = Egg::SCALE;
		break;

	case fScaleAboutPoint:
		freyjaPrintMessage("freyjaModelTransform> No appropreiate transform, file bug with %", EMAIL_ADDRESS);
		return;
		break;

	case fRotateAboutPoint:
		type = Egg::ROTATE_ABOUT_CENTER;
		break;
	}

	egg->Transform(type, x, y, z);
}


void freyjaVertexListTransform(Vector<uint32> &list,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z)
{
	enum Egg::egg_transform type;
	Egg *egg = 0x0;

	// FIXME: Only one model -- ignore model index for now

	egg = freyja__getEggBackend();

	if (!egg)
		return;

	switch (action)
	{
	case fTranslate:
		type = Egg::TRANSLATE;
		break;

	case fRotate:
		type = Egg::ROTATE;
		break;

	case fScale:
		type = Egg::SCALE;
		break;

	case fScaleAboutPoint:
		freyjaPrintMessage("freyjaModelTransform> No appropreiate transform, file bug with %", EMAIL_ADDRESS);
		return;
		break;

	case fRotateAboutPoint:
		type = Egg::ROTATE_ABOUT_CENTER;
		break;
	}


	if (list.empty())
		return;

	egg_vertex_t *vertex;
	Matrix m, inverse, normalTransform;
	unsigned int i;


	m.setIdentity();

	switch (type)
	{
	case Egg::SCALE:
		m.scale(x, y, z);
		break;

	case Egg::ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;

	case Egg::TRANSLATE:
		m.translate(x, y, z);
		break;

	default:
		return;
	}

	m.getInvert(inverse.mMatrix);
	inverse.getTransposeMatrix(normalTransform.mMatrix);
	normalTransform.setMatrix(inverse.mMatrix);

	for (i = list.begin(); i < list.end(); ++i)
	{
		vertex = egg->getVertex(list[i]);
				
		if (!vertex)
			continue;
				
		m.multiply3v(vertex->pos, vertex->pos);
		normalTransform.multiply3v(vertex->norm, vertex->norm);
	}
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

		idx = freyjaTexCoordCreate2fv(uv);
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
 * Vector maps are very memory wasteful */  // use a materialMap
int32 freyjaSaveMeshChunkV1(FreyjaFileWriter &w, index_t meshIndex)
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

		freyjaGetVertexTexcoord2fv(vertex, uv);
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
	Egg *egg = freyja__getEggBackend();
	FreyjaFileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords, bones;
	freyja_file_chunk_t chunk;
	int32 offset, i, j, index, flags, count;
	vec4_t wxyz;
	vec3_t xyz;
	char buffer[64];
	
	
	/* HACK - This is to load Egg inline like in current freyja,
	 *        until it's removed and made into purely a plugin */
	if (egg && egg->loadFile((char*)filename) == 0)
	{
		count = freyjaGetModelMeshCount(0); // egg is single model

		for (i = 0; i < count; ++i)
			freyjaMeshClampTexCoords(i);

		return 0;
	}

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

	if (strncmp(header.magic, FREYJA_API_VERSION, 7)) // 'Freyja '
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
					xyz[j] = r.readFloat32();

				freyjaBoneRotateEuler3fv(index, xyz);
			}
			else
			{
				for (j = 0; j < 4; ++j)
					wxyz[j] = r.readFloat32();

				freyjaBoneRotateQuat4fv(index, wxyz);
			}

			freyjaEnd(); // FREYJA_BONE

			if ((long)r.getFileOffset() != offset)
				printf("BONE @ %i not %i!\n", r.getFileOffset(), offset);
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

					if (FreyjaFileReader::doesFileExist(mat->getTextureName()))
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
			freyjaGetBoneRotationQuat4fv(index, wxyz);
			
			for (j = 0; j < 4; ++j)
				w.writeFloat32(wxyz[j]);
#else
			freyjaGetBoneRotationEuler3fv(index, xyz);
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
			FreyjaMaterial *mat = freyjaGetMaterialClass(i);

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
		
		if (vol.isVertexInside(xyz))
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


void freyjaGetVertexPolygonRef1i(index_t vertexIndex, Vector<long> &polygons)
{
	Egg *egg = freyja__getEggBackend();
	polygons.clear();

	if (egg)
	{
		egg_vertex_t *v = egg->getVertex(vertexIndex);
		unsigned int i;

		if (v && !v->ref.empty())
		{
			for (i = v->ref.begin(); i < v->ref.end(); ++i)
				polygons.pushBack((long)v->ref[i]);
		}
	}
}


void freyjaGetVertexPolygonRef(Vector<long> &polygons)
{
	Egg *egg = freyja__getEggBackend();
	polygons.clear();

	if (egg)
	{
		egg_vertex_t *v = egg->getVertex(freyjaGetCurrent(FREYJA_VERTEX));
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

index_t gFreyjaLightIndex = INDEX_INVALID;


index_t freyjaLightCreate()
{
	index_t lightIndex = gFreyjaLights.size();

	gFreyjaLights.pushBack(new FreyjaLight());
	// gFreyjaLights[lightIndex]->mId = lightIndex;

	return lightIndex;	
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
	

index_t freyjaTexCoordCreate2f(vec_t u, vec_t v)
{
	return gEgg->addTexel(u, v);
}


index_t freyjaTexCoordCreate2fv(const vec2_t uv)
{
	return freyjaTexCoordCreate2f(uv[0], uv[1]);
}


void freyjaVertexWeight(index_t vertexIndex, vec_t weight, index_t bone)
{
	egg_vertex_t *vert = gEgg->getVertex(vertexIndex);
	egg_weight_t *vWeight;
	int emptySlot = -1;
	vec_t total = 0.0f;
	unsigned int i;


	if (!vert)
		return;

	for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
	{
		vWeight = vert->weights[i];

		if (vWeight)
		{
			if (weight <= 0.0) // Remove weight
			{
				delete vWeight;
				vert->weights.assign(i, 0x0);
				return;
			}

			if (vWeight->bone == bone) // Alter weight
			{
				vWeight->weight = weight;
				return;
			}

			total = vWeight->weight;
		}
		else
		{
			emptySlot = i;
		}
	}

	if (weight <= 0.0) // Don't add dead weight ( remove requested )
		return;

	if (total + weight > 1.0001)  // Just give a warning for now
		freyjaPrintError("WARNING: Weight overflow %.3f + %.3f > 1.0 not handled here %s:%d\n", 
					 total, weight, __FILE__, __LINE__);

	vWeight = new egg_weight_t;
	vWeight->weight = weight;
	vWeight->bone = bone;

	if (emptySlot > -1)
	{
		vert->weights.assign(emptySlot, vWeight);
	}
	else
	{
		vert->weights.pushBack(vWeight);
	}
}


void freyjaVertexPosition3fv(index_t vertexIndex, vec3_t xyz)
{
	Egg *egg = freyja__getEggBackend();
	egg_vertex_t *v;

	if (egg)
	{
		v = egg->getVertex(vertexIndex);

		if (v)
		{
			HEL_VEC3_COPY(xyz, v->pos);
		}		
	}
}


void freyjaVertexDelete(index_t vertexIndex)
{
	Egg *egg = freyja__getEggBackend();
	
	if (egg)
		egg->delVertex(vertexIndex);
}


index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB)
{
	Egg *egg = freyja__getEggBackend();
	
	if (egg)
	{
		egg->combineVertices(vertexIndexA, vertexIndexB);
		return vertexIndexA;
	}

	return INDEX_INVALID;
}


void freyjaVertexFrame3f(index_t index, vec_t x, vec_t y, vec_t z)
{
	if (gEgg)
	{
		egg_vertex_t *v = gEgg->getVertex(index);

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


void freyjaVertexTexCoord2fv(index_t vIndex, vec2_t uv)
{
	egg_vertex_t *vert;

	if (gEgg)
	{
		vert = gEgg->getVertex(vIndex);

		if (vert)
		{
			vert->uv[0] = uv[0];
			vert->uv[1] = uv[1];
		}
	}
}


void freyjaVertexTexcoord2f(index_t vIndex, vec_t u, vec_t v)
{
	egg_vertex_t *vert;

	if (gEgg)
	{
		vert = gEgg->getVertex(vIndex);

		if (vert)
		{
			vert->uv[0] = u;
			vert->uv[1] = v;
		}
	}
}


void freyjaVertexNormalFlip(index_t index)
{
	Vector3d n;

	if (gEgg)
	{
		freyjaGetVertexNormalXYZ3fv(index, n.mVec);
		n = -n;
		n.normalize();
		freyjaVertexNormal3fv(index, n.mVec);
	}
}


void freyjaVertexNormal3fv(index_t vIndex, vec3_t nxyz)
{
	egg_vertex_t *vert = 0x0;


	if (gEgg)
		vert = gEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->norm[0] = nxyz[0];
	vert->norm[1] = nxyz[1];
	vert->norm[2] = nxyz[2];
}


void freyjaVertexNormal3f(index_t vIndex, vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert = 0x0;

	if (gEgg)
		vert = gEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->norm[0] = x;
	vert->norm[1] = y;
	vert->norm[2] = z;
}


void freyja__PolygonReplaceReference(index_t polygonIndex, 
									 index_t vertexA, index_t vertexB)
{
	Vector<unsigned int> ref;
	egg_vertex_t *a = 0x0, *b = 0x0;
	egg_polygon_t *polygon = 0x0;
	unsigned int i;


	if (gEgg)
	{
		a = gEgg->getVertex(vertexA);
		b = gEgg->getVertex(vertexB);
		polygon = gEgg->getPolygon(polygonIndex);
	}
	
	if (!a || !b || !polygon)
	{
		return;
	}

	// Replace A with B to match sorted list ids to form same edge
	polygon->vertex.Replace(vertexA, vertexB);

	polygon->r_vertex.clear();

	for (i = polygon->vertex.begin(); i < polygon->vertex.end(); ++i)
	{
		polygon->r_vertex.pushBack(gEgg->getVertex(polygon->vertex[i]));
	}

	// Add polygonIndex to B's reference list
	b->ref.pushBack(polygonIndex);

	// Remove polygonIndex from A's reference list
	for (i = a->ref.begin(); i < a->ref.end(); ++i)
	{
		if (a->ref[i] != polygonIndex)
		{
			ref.pushBack(a->ref[i]);
		}
	}

	a->ref.clear();
	a->ref.copy(ref);
}



void freyja__GetCommonPolygonReferences(index_t vertexA, index_t vertexB,
										Vector<unsigned int> &common)
{
	egg_vertex_t *a, *b;
	unsigned i, j, countA, countB;

	common.clear();

	if (gEgg)
	{
		a = gEgg->getVertex(vertexA);
		b = gEgg->getVertex(vertexB);

		if (a && b)
		{
			countA = a->ref.end();
			countB = b->ref.end();

			for (i = a->ref.begin(); i < countA; ++i)
			{
				for (j = b->ref.begin(); j < countB; ++j)
				{
					if (a->ref[i] == b->ref[j])
						common.pushBack(a->ref[i]);
				}
			}
		}
	}
}


void freyja__GetDifferenceOfPolygonReferences(index_t vertexA, index_t vertexB,
											  Vector<unsigned int> &diffA)
{
	egg_vertex_t *a, *b;
	unsigned i, j, countA, countB;
	bool failed;

	diffA.clear();
	
	if (gEgg)
	{
		a = gEgg->getVertex(vertexA);
		b = gEgg->getVertex(vertexB);

		if (a && b)
		{
			countA = a->ref.end();
			countB = b->ref.end();

			for (i = a->ref.begin(); i < countA; ++i)
			{
				failed = false;

				for (j = b->ref.begin(); j < countB; ++j)
				{
					if (a->ref[i] == b->ref[j])
					{
						failed = true;
						break;
					}
				}

				if (!failed)
				{
					diffA.pushBack(a->ref[i]);
				}
			}
		}
	}
}


void freyjaMeshPolygonSplitTexCoords(index_t meshIndex, index_t polygonIndex)
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
			freyjaGetVertexTexcoord2fv(vertexIndex, uv);
			texcoordIndex = freyjaTexCoordCreate2fv(uv);
			freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
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
			freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
		}
		
	}
}


index_t freyjaPolygonCreate()
{
	egg_polygon_t *polygon;

	if (gEgg)
	{
		/* Spawns a degerate polygon */
		Vector<egg_polygon_t *> &polygons = gEgg->getPolygonVector();
		polygon = new egg_polygon_t;
		polygons.pushBack(polygon);
		polygon->id = polygons.size() - 1;

		return polygon->id;
	}

	return INDEX_INVALID;
}


void freyjaPolygonDelete(index_t polygonIndex)
{
	if (gEgg)
	{
		gEgg->delPolygon(polygonIndex);
	}
}


void freyjaPolygonFlagAlpha(index_t polygonIndex, char on)
{
	egg_polygon_t *polygon;

	if (gEgg)
	{
		polygon = gEgg->getPolygon(polygonIndex);

		if (polygon)
		{
			polygon->flags |= fPolygon_Alpha;

			if (!on)
				polygon->flags ^= fPolygon_Alpha;
		}
	}	
}


void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex)
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
		freyjaGetVertexXYZ3fv(A, a.mVec);
		B = freyjaGetPolygonVertexIndex(polygonIndex, 1);
		freyjaGetVertexXYZ3fv(B, b.mVec);
		C = freyjaGetPolygonVertexIndex(polygonIndex, 2);
		freyjaGetVertexXYZ3fv(C, c.mVec);
		D = freyjaGetPolygonVertexIndex(polygonIndex, 3);
		freyjaGetVertexXYZ3fv(D, d.mVec);

		helMidpoint3v(a.mVec, b.mVec, m1.mVec);
		M1 = freyjaVertexCreate3fv(m1.mVec);
		freyjaGetVertexTexcoord2fv(A, uv1);
		freyjaGetVertexTexcoord2fv(B, uv2);
		uv[0] = (uv1[0] + uv2[0]) / 2;
		uv[1] = (uv1[1] + uv2[1]) / 2;
		freyjaVertexTexCoord2fv(M1, uv);
		freyjaGetVertexNormalXYZ3fv(A, n1.mVec);
		freyjaGetVertexNormalXYZ3fv(B, n2.mVec);
		n = n1 + n2;
		n.normalize();
		freyjaVertexNormal3fv(M1, n.mVec);

		helMidpoint3v(c.mVec, d.mVec, m2.mVec);
		M2 = freyjaVertexCreate3fv(m2.mVec);
		freyjaGetVertexTexcoord2fv(C, uv1);
		freyjaGetVertexTexcoord2fv(D, uv2);
		uv[0] = (uv1[0] + uv2[0]) / 2;
		uv[1] = (uv1[1] + uv2[1]) / 2;
		freyjaVertexTexCoord2fv(M2, uv);
		freyjaGetVertexNormalXYZ3fv(C, n1.mVec);
		freyjaGetVertexNormalXYZ3fv(D, n2.mVec);
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


void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist)
{
	Vector3d faceNormal, a, b, c;
	int32 i, v, count;

	if (freyjaIterator(FREYJA_POLYGON, polygonIndex) == INDEX_INVALID)
		return;

	// 1. Get face normal
	faceNormal.zero();

	/* Generated face normal, which won't match preclac vertex normalsx */
	//freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
	//freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);
	//freyjaGetVertexXYZ3fv(v0, a.mVec);
	//freyjaGetVertexXYZ3fv(v1, b.mVec);
	//freyjaGetVertexXYZ3fv(v2, c.mVec);
	//faceNormal = Vector3d::cross(a - b, c - b);
	
	/* Use existing vertex normals ( perfered, also like n-patch ) */
	count = freyjaGetPolygonVertexCount(polygonIndex);
	for (i = 0; i < count; ++i)
	{
		v = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexNormalXYZ3fv(v, a.mVec);
		faceNormal += a;
	}


	// 2. Scale face normal by dist
	faceNormal.normalize();
	faceNormal *= dist;

	freyjaPolygonExtrudeQuad(polygonIndex, faceNormal.mVec);
}


int32 freyjaVertexXYZ3fv(index_t vertexIndex, vec3_t xyz)
{
	egg_vertex_t *v;

	if (gEgg)
	{
		v = gEgg->getVertex(vertexIndex);

		if (v)
		{
			v->pos[0] = xyz[0];
			v->pos[1] = xyz[1];
			v->pos[2] = xyz[2];
			return 0;
		}
	}

	return -1;
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
		freyjaGetVertexXYZ3fv(A, xyz);
		B = freyjaVertexCreate3fv(xyz);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv);
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz);
		
		face.pushBack(B);


		/* 2. Replace all references to A with B ( dupe of A ), 
		 * except polygonIndex */
		freyjaGetVertexPolygonRef1i(A, ref);

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			if (ref[j] != (int32)polygonIndex)
			{
				freyja__PolygonReplaceReference(ref[j], A, B);
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
		freyjaGetVertexXYZ3fv(A, xyz);
		xyz[0] += normal[0];
		xyz[1] += normal[1];
		xyz[2] += normal[2];
		freyjaVertexXYZ3fv(A, xyz);
	}
}


void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex)
{
	if (gEgg)
	{
		egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

		if (polygon)
		{
			polygon->vertex.pushBack(vertexIndex);
			polygon->r_vertex.pushBack(gEgg->getVertex(vertexIndex));
		}
	}
}


void freyjaTexCoordCombine(uint32 A, uint32 B)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
		egg->combineTexels(A, B);
}


int freyjaVertexExtrude(index_t vertexIndex, vec_t midpointScale, vec3_t normal)
{
	Vector<long> faces, edges;
	Vector3d a, b, c, ab, ac;
	vec3_t xyz;
	vec2_t uv;
	uint32 A, B, C, i, j, material, polygonCount, polygonIndex, vertexCount, texcoordCount, v;
	int32 last;


	freyjaGetVertexPolygonRef1i(vertexIndex, faces);
	polygonCount = faces.end();

	if (!polygonCount)
		return -1;

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = faces[i];
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
		freyjaGetVertexXYZ3fv(A, a.mVec);
		freyjaGetVertexXYZ3fv(B, b.mVec);
		freyjaGetVertexXYZ3fv(C, c.mVec);
		ab = a + b;
		ab *= 0.5 * midpointScale;
		ac = a + c;
		ac *= 0.5 * midpointScale;


		// 3. Generate new vertices at scaled midpoints of edges
		B = freyjaVertexCreate3fv(ab.mVec);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv); // FIXME
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz); // FIXME

		C = freyjaVertexCreate3fv(ac.mVec);
		freyjaGetVertexTexcoord2fv(A, uv);
		freyjaVertexTexCoord2fv(C, uv); // FIXME
		freyjaGetVertexNormalXYZ3fv(A, xyz);
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
	freyjaGetVertexXYZ3fv(vertexIndex, xyz);
	xyz[0] += normal[0];
	xyz[1] += normal[1];
	xyz[2] += normal[2];
	freyjaVertexXYZ3fv(vertexIndex, xyz);

	return 0;
}


void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex)
{
	if (gEgg)
	{
		egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

		if (polygon)
		{
			polygon->texel.pushBack(texcoordIndex);
			polygon->r_texel.pushBack(gEgg->getTexel(texcoordIndex));
		}
	}
}


void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex)
{
	if (gEgg)
	{
		egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

		if (polygon)
			polygon->shader = materialIndex;
	}
}


const char *freyjaGetMeshNameString(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->name;
	}

	return 0x0;
}


void freyjaGetMeshName1s(index_t meshIndex, int32 lenght, char *name)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	name[0] = 0;

	if (mesh)
	{
		strncpy(name, mesh->name, lenght);
		name[lenght-1] = 0;
	}
}


void freyjaPolygonTexCoordPurge(index_t polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		polygon->texel.clear();
		polygon->r_texel.clear();
	}
}


void freyjaMeshUVMapPlanar(index_t meshIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_mesh_t *mesh = 0x0;
	egg_polygon_t *poly;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	unsigned int i, j;
	float u, v;


	if (egg)
		mesh = egg->getMesh(meshIndex);

	if (!mesh)
		return;

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{  
		poly = egg->getPolygon(mesh->polygon[i]);

		if (!poly)
			continue;

		// Generate texels and record their ids
		for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
		{
			vertex = egg->getVertex(poly->vertex[j]);

			if (!vertex)
				continue;

			// Mongoose 2002.01.18, Generate UV from vertex XYZ
			freyjaGenerateUVFromXYZ(vertex->pos, &u, &v);

#ifdef DEBUG_GEN_TEXEL
			freyja_print("FreyjaModel::generateUVMap> %f %f\n", u, v);
#endif

			if (poly->texel.empty())
			{
				texel = 0x0;
			}
			else
			{
				texel = egg->getTexel(poly->texel[j]);
			}
			
			if (!texel)
			{
				vertex->uv[0] = u;
				vertex->uv[1] = v;
			}
			else
			{
				texel->st[0] = u;
				texel->st[1] = v;
			}
		}
	}
}


void freyjaMeshUVMapSpherical(index_t meshIndex)
{
	int32 i, vertexCount, vertexIndex;
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uv;


	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		
		freyjaGetVertexXYZ3fv(vertexIndex, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = latitude;

		freyjaVertexTexCoord2fv(vertexIndex, uv);
	}
}


// FIXME: Uses Egg
void freyjaMeshMaterial(index_t meshIndex, uint32 materialIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_mesh_t *mesh;
	egg_polygon_t *poly;
	uint32 i;
	
	if (!egg)
		return;

	mesh = egg->getMesh(meshIndex);

	if (!mesh)
	{
		freyjaPrintMessage("freyjaMeshMaterial> ERROR: Invalid mesh[%i]\n",
					 		meshIndex);
		return;
	}

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{  
		poly = egg->getPolygon(mesh->polygon[i]);

		if (!poly)
			continue;

		poly->shader = materialIndex;
	}	
}


// FIXME: Uses old egg gobal framing, etc
void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Egg *egg = freyja__getEggBackend();
	egg_group_t *grp;


	if (egg)
	{
		grp = egg->getGroup(frame);

		if (grp)
		{
			grp->center[0] = xyz[0];
			grp->center[1] = xyz[1];
			grp->center[2] = xyz[2];
		}
	}
}


// FIXME: Uses old egg gobal framing, etc
void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Egg *egg = freyja__getEggBackend();
	egg_group_t *grp;


	if (egg)
	{
		grp = egg->getGroup(frame);

		if (grp)
		{
			xyz[0] = grp->center[0];
			xyz[1] = grp->center[1];
			xyz[2] = grp->center[2];
		}
	}
}


void freyjaMeshUVMapCylindrical(index_t meshIndex)
{
	int32 i, j, vertexCount, vertexIndex;
	vec_t longitude, latitude, ysize;
	vec3_t xyz;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};
	vec2_t uv;
	

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexXYZ3fv(vertexIndex, xyz);

		for (j = 0; j < 3; ++j)
		{
			if (xyz[j] < min[j])
				min[j] = xyz[j];

			if (xyz[j] > max[j])
				max[j] = xyz[j];
		}
	}

	if (max[1] >= 0)
	{
		if (min[1] >= 0)
		{
			ysize = max[1] - min[1];
		}
		else
		{
			ysize = max[1] + -min[1];
		}
	}
	else
	{
		ysize = -max[1] + min[1];
	}

	if (ysize < 0.0001 && ysize > -0.0001)
		ysize = 1.0f;

    for (i = 0; i < vertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexXYZ3fv(vertexIndex, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = xyz[1] / ysize;

		freyjaVertexTexCoord2fv(vertexIndex, uv);
	}
}


void freyjaMeshPromoteTexcoordsToPloymapping(index_t meshIndex)
{
	vec2_t uv;
	int32 i, j, k, vertexCount, vertexIndex, texcoordIndex, polygonIndex, polygonCount;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

    for (i = 0; i < polygonCount; ++i)
    {
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexCount = freyjaGetPolygonVertexCount(polygonIndex);

			// NOTE: I just update all UV -> polymapp to avoid corrupt 
			//       'texture faces' eg not completely polymapped
			freyjaPolygonTexCoordPurge(polygonIndex);

			for (k = 0; k < vertexCount; ++k) 
			{
				vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, k);
				freyjaGetVertexTexcoord2fv(vertexIndex, uv);

				texcoordIndex = freyjaTexCoordCreate2fv(uv);
				freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
			}
		}
	}
}


void freyjaGetMeshFrameBoundingBox(index_t meshIndex, uint32 frame, 
									vec3_t min, vec3_t max)
{
	egg_group_t *grp;
	Egg *egg = freyja__getEggBackend();

	if (egg)
	{
		// Just use old unbound system for Egg 
		grp = egg->getGroup(frame);//freyjaGetMeshVertexGroupIndex(meshIndex, frame));

		if (grp)
		{
			min[0] = grp->bbox_min[0];
			min[1] = grp->bbox_min[1];
			min[2] = grp->bbox_min[2];

			max[0] = grp->bbox_max[0];
			max[1] = grp->bbox_max[1];
			max[2] = grp->bbox_max[2];
		}
	}
}


void freyjaMeshGenerateVertexNormals(index_t meshIndex)
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	int32 v0, v1, v2, i, j, vertexIndex, polygonIndex, meshVertexCount, polygonCount, vertexCount;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	meshVertexCount = freyjaGetMeshVertexCount(meshIndex);

	if (!meshVertexCount || !polygonCount)
		return;

    for (i = 0; i < polygonCount; ++i)
    {
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);
		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		
		/* Just need 3 sides -- assume all coplanar these are simple polygons,
		 * so just take first 3 vertices to make 2 edge vectors */
		v0 = freyjaGetPolygonVertexIndex(polygonIndex, 0);
		v1 = freyjaGetPolygonVertexIndex(polygonIndex, 1);
		v2 = freyjaGetPolygonVertexIndex(polygonIndex, 2);

		freyjaPrintMessage("%i <%d %d %d>", polygonIndex, v0, v1, v2);
		freyjaGetVertexXYZ3fv(v0, a.mVec);
		freyjaGetVertexXYZ3fv(v1, b.mVec);
		freyjaGetVertexXYZ3fv(v2, c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));
	}


	/* Compute vertex normals */
    for (i = 0; i < meshVertexCount; ++i)
    {
		vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, i);

		if (vertexIndex < 0)
		{
			freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		freyjaGetVertexPolygonRef1i(vertexIndex, ref);

		for (j = ref.begin(); j < (int)ref.end(); ++j)
		{
			if (ref[j] > polygonCount)
			{
				freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR bad face\n");
				continue;
			}

			normal += *faceNormals[ref[j]];
		}

		normal.normalize();

		freyjaVertexNormal3fv(vertexIndex, normal.mVec);
    }

	faceNormals.erase();
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


// FIXME: This is a horrible algorithm, but works with Vector for same ordering
void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex)
{
	Vector<index_t> keep;
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);
	int32 i, count;


	if (mesh)
	{
		count = mesh->polygon.end();

		for (i = mesh->polygon.begin(); i < count; ++i)
		{
			if (mesh->polygon[i] != polygonIndex)
			{
				keep.pushBack(mesh->polygon[i]);
			}
		}

		mesh->polygon.clear();
		mesh->r_polygon.clear();
		count = keep.end();

		for (i = keep.begin(); i < count; ++i)
		{
			mesh->polygon.pushBack(keep[i]);
			mesh->r_polygon.pushBack(gEgg->getPolygon(keep[i]));
		}
	}
}


void freyjaMeshNormalFlip(index_t meshIndex)
{
	int32 i, n, v;

	for (i = 0, n = freyjaGetMeshVertexCount(meshIndex); i < n; ++i)
	{
		v = freyjaGetMeshVertexIndex(meshIndex, i);

		if (v > -1)
		{
			freyjaVertexNormalFlip(v);
		}
	}
}


void freyjaMeshName1s(index_t meshIndex, const char *name)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		strncpy(mesh->name, name, 64);
		mesh->name[63] = 0;
	}
}


void freyjaMeshPosition(index_t meshIndex, vec3_t xyz)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		mesh->position = Vector3d(xyz);
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


void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_vertex_t *vert = egg->getVertex(vertexIndex);
	egg_weight_t *weight;
	unsigned int i;

	if (!egg)
		return;

	vert = egg->getVertex(vertexIndex);

	if (vert)
	{
		for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
		{
			weight = vert->weights[i];

			if (weight && weight->bone == boneIndex)
			{
				delete vert->weights[i];  /* Safe to null this out?  */
			}
		}
	}
}


index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex, uint32 element)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);
		return t->ref[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);
		return t->ref.size();
	}

	return 0;
}


void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v)
{
	vec2_t uv = {u, v};

	freyjaTexCoord2fv(texcoordIndex, uv);
}


void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);

		if (t)
		{
			t->st[0] = uv[0];
			t->st[1] = uv[1];
		}
	}
}


void freyjaGetTexCoord2fv(index_t tindex, vec2_t uv)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(tindex);

		if (t)
		{
			uv[0] = t->st[0];
			uv[1] = t->st[1];
		}
	}
}


vec3_t *freyjaGetVertexXYZ(index_t vertexIndex)
{
	if (!gEgg)
		return 0x0;

	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);
			 
	if (vertex)
		return &(vertex->pos);

	return 0x0;
}


vec2_t *freyjaGetVertexUV(index_t vertexIndex)
{
	if (!gEgg)
		return 0x0;

	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);
			 
	if (vertex)
		return &(vertex->uv);
	
	return 0x0;
}


vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex)
{
	if (!gEgg)
		return 0x0;

	egg_texel_t *texel = gEgg->getTexel(texcoordIndex);
			 
	if (texel)
		return &(texel->st); // really uv

	return 0x0;
}


index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element)
{
	egg_vertex_t *v;

	if (gEgg)
	{
		v = gEgg->getVertex(vertexIndex);
		return v->ref[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex)
{
	egg_vertex_t *v;

	if (gEgg)
	{
		v = gEgg->getVertex(vertexIndex);
		return v->ref.size();
	}

	return 0;
}


index_t freyjaMeshVertexCreate3f(index_t meshIndex, index_t groupIndex,
								 vec_t x, vec_t y, vec_t z)
{
	index_t id = INDEX_INVALID;
	egg_vertex_t *vert = gEgg->addVertex(x, y, z);
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);
	egg_group_t *group = gEgg->getGroup(groupIndex);

	if (vert)
	{
		if (mesh)
		{
			mesh->vertices.pushBack(vert->id);
		}

		if (group)
		{
			group->vertex.pushBack(vert->id);
		}
		else
		{
#ifdef WARN_VERTEX_OUTSIDE_GROUP
			freyjaPrintError("freyjaVertex3f: WARNING Vertex[%i] outside GROUP!",
							 vert->id);
#endif
		}
		
		id = vert->id;
	}

	return id;
}


uint32 freyjaGetPolygonVertexCount(index_t polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->vertex.end();
	}

	return 0;
}


uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->texel.end();
	}

	return 0;
}


void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		uv[0] = vertex->uv[0];
		uv[1] = vertex->uv[1];
	}
}


void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		nxyz[0] = vertex->norm[0];
		nxyz[1] = vertex->norm[1];
		nxyz[2] = vertex->norm[2];
	}
}


void freyjaGetVertexXYZ3fv(index_t vertexIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		xyz[0] = vertex->pos[0];
		xyz[1] = vertex->pos[1];
		xyz[2] = vertex->pos[2];
	}
}


void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
						  index_t *frameIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);


	if (vertex)
	{
		if (element < vertex->frames.end())
		{
			vec_t *v = *(vertex->frames[element]);

			if (element < vertex->frameId.end())
				*frameIndex = vertex->frameId[element];

			xyz[0] = v[0];
			xyz[1] = v[1];
			xyz[2] = v[2];
		}

		/* Invalid indices return orignal position, so you don't get holes */
		xyz[0] = vertex->pos[0];
		xyz[1] = vertex->pos[1];
		xyz[2] = vertex->pos[2];
	}
}


uint32 freyjaGetVertexFrameCount(index_t vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->frames.end();
	}

	return 0;
}


void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
						   index_t *bone, vec_t *weight)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex && element < vertex->weights.end())
	{
		*bone = vertex->weights[element]->bone;
		*weight = vertex->weights[element]->weight;
	}
}


index_t freyjaGetVertexWeightCount(index_t vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->weights.end();
	}

	return 0;
}


index_t freyjaGetVertexFlags(index_t vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->flags;
	}

	return 0;
}


index_t freyjaGetModelFlags(index_t modelIndex)
{
	return 0x0;  // Not Implemented due to Egg backend use
}


index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = gEgg->getMesh(element);

	if (mesh)
	{
		return mesh->id;
	}

	return 0;
}


index_t freyjaGetModelMeshCount(index_t modelIndex)
{
	// Not Implemented properly due to Egg backend use ( not scene based )

	if (modelIndex == 0)
		return freyjaGetCount(FREYJA_MESH);
	
	return 0;
}


char freyjaIsMeshAllocated(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
		return 1;
	
	return 0;
}


void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		xyz[0] = mesh->position.mVec[0];
		xyz[1] = mesh->position.mVec[1];
		xyz[2] = mesh->position.mVec[2];
	}
}


uint32 freyjaGetMeshFlags(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->flags;
	}

	return 0;
}


index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		// Not Implemented
	}

	return 0;//INDEX_INVALID;
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


uint32 freyjaGetMeshPolygonCount(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->polygon.end();
	}

	return 0;
}


index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex, index_t faceVertexIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);


	if (mesh && faceVertexIndex < mesh->verticesMap.size())
	{
		return mesh->verticesMap[faceVertexIndex];
	}

	return INDEX_INVALID;
}


index_t freyjaGetMeshVertexIndex(index_t meshIndex, uint32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh && element < mesh->vertices.size())
	{
			return mesh->vertices[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexCount(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		/* FIXME
		 * This should only be updated when mesh is MUTATED, and not everytime
		 * it's accessed in this way! */
		freyja__MeshUpdateMappings(meshIndex); // Setup Egg mesh for export
		return mesh->vertices.size();
	}

	return 0;
}


index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, uint32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);


	if (mesh && element < mesh->texcoordsMap.size())
	{
			return mesh->texcoordsMap[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshTexCoordCount(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		/* FIXME
		 * This should only be updated when mesh is MUTATED, and not everytime
		 * it's accessed in this way! */
		//freyja__MeshUpdateMappings(meshIndex); // Setup Egg mesh for export
		return mesh->texcoordsMap.size();
	}

	return 0;
}


void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
											index_t vertexIndex)
{
	int32 groupIndex = freyjaGetMeshVertexGroupIndex(meshIndex, element);
	Egg *egg = freyja__getEggBackend();

	if (egg && groupIndex > -1 && vertexIndex != INDEX_INVALID)
	{
		egg_group_t *grp = egg->getGroup(groupIndex);
		grp->vertex.add(vertexIndex); //pushBack(vertexIndex);
	}
}


uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element)
{
	int32 groupIndex = freyjaGetMeshVertexGroupIndex(meshIndex, element);
	Egg *egg = freyja__getEggBackend();

	if (egg && groupIndex > -1)
	{
		egg_group_t *grp = egg->getGroup(groupIndex);
		return grp->vertex.size();
	}

	return 0;
}


index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
										  uint32 vertexElement)
{
	int32 groupIndex = freyjaGetMeshVertexGroupIndex(meshIndex, element);
	Egg *egg = freyja__getEggBackend();


	if (egg && groupIndex > -1)
	{
		egg_group_t *grp = egg->getGroup(groupIndex);

		if (vertexElement < grp->vertex.size())
			return grp->vertex[vertexElement];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->group.end();
	}

	return 0;
}


index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh && element < mesh->polygon.end())
	{
		return mesh->polygon[element];
	}

	return INDEX_INVALID;
}


index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh && element < mesh->group.end())
	{
		return mesh->group[element];
	}

	return INDEX_INVALID;
}


index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return INDEX_INVALID;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon || element >= polygon->vertex.end())
		return INDEX_INVALID; 

	return polygon->vertex[element];
}


index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return INDEX_INVALID;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon || element >= polygon->texel.end())
		return INDEX_INVALID; 

	return polygon->texel[element];
}


index_t freyjaGetPolygonMaterial(index_t polygonIndex)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return 0;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon)
		return 0; 

	return polygon->shader;
}


index_t freyjaGetPolygonFlags(index_t polygonIndex)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return 0;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon)
		return 0;

	return polygon->vertex.end();//FIXME | polygon->flags;
}


uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex)
{
	egg_polygon_t *polygon;
	long flags = 0x0;


	if (!gEgg)
		return 0;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon)
		return 0; 

	// Currently using Egg backend -- it can't do more than one UV type
	// or coloring
	if (polygon->vertex.end() == polygon->texel.end())
	{
		flags |= fPolygon_PolyMapped;
	}
	else if (polygon->vertex.end() == polygon->texel.end()*2)
	{
		flags |= fPolygon_ColorMapped;
	}
	else
	{
		flags |= fPolygon_VertexUV;
	}

	return flags;
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


// FIXME: Uses Egg
void freyjaModelMirrorTexCoord(index_t modelIndex, index_t texcoordIndex,
								Vector<int32> uvMap, bool x, bool y)
{
	Egg *egg = freyja__getEggBackend();
	Vector<index_t> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel;
	egg_texel_t *tex;
	Vector3d u, trans, min;
	uint32 i, j, k;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	texel = egg->getTexel(texcoordIndex);

	if (texel && !uvMap.empty())
	{
		for (i = uvMap.begin(); i < uvMap.end(); ++i)
		{
			poly = egg->getPolygon(uvMap[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < seen.end(); ++k)
					{
						if (seen[k] == poly->texel[j])
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

		for (k = seen.begin(); k < seen.end(); ++k)
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
		for (i = texel->ref.begin(); i < texel->ref.end(); ++i)
		{
			poly = egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < seen.end(); ++k)
					{
						if (seen[k] == poly->texel[j])
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

		for (k = seen.begin(); k < seen.end(); ++k)
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

		for (i = vertex->ref.begin(); i < vertex->ref.end(); ++i)
		{
			poly = egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
				{
					vert = egg->getVertex(poly->vertex[j]);

					for (k = seen.begin(); k < seen.end(); ++k)
					{
						if (seen[k] == poly->vertex[j])
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
}


// FIXME: Uses Egg
void freyjaModelTransformTexCoord(index_t modelIndex, index_t texcoordIndex,
									freyja_transform_action_t action,
									vec_t x, vec_t y)
{
	Egg *egg = freyja__getEggBackend();
	Vector<index_t> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel;
	egg_texel_t *tex;
	Vector3d u, v, p;
	uint32 i, j, k;
	Matrix m;
	vec_t z;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	texel = egg->getTexel(texcoordIndex);

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

		for (i = texel->ref.begin(); i < texel->ref.end(); ++i)
		{
			poly = egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
				{
					tex = egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < seen.end(); ++k)
					{
						if (seen[k] == poly->texel[j])
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

		for (i = vertex->ref.begin(); i < vertex->ref.end(); ++i)
		{
			poly = egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
				{
					vert = egg->getVertex(poly->vertex[j]);

					for (k = seen.begin(); k < seen.end(); ++k)
					{
						if (seen[k] == poly->vertex[j])
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
}


// FIXME: Uses Egg
void freyjaModelClear(index_t modelIndex)
{
	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !gEgg)
		return;

	gEgg->clear();

	// Currently there is no multimodel while egg is being used
	freyjaSkeletonPoolClear();
	freyjaBonePoolClear();
}


// FIXME: Uses Egg
void freyjaModelClampTexCoords(index_t modelIndex)
{
	unsigned int i;
	egg_texel_t *t;
	egg_vertex_t *v;
	Egg *egg = freyja__getEggBackend();


	// Egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	for (i = 0; i < egg->getTexelCount(); ++i)
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
}


char freyjaGetModelAppendMeshMode(index_t modelIndex)
{
	if (modelIndex < gCopyModels.size())
	{
		return gCopyModels[modelIndex]->getAppendMeshMode();
	}

	return -1;
}


void freyjaModelAppendMeshMode(index_t modelIndex, char on)
{
	if (modelIndex < gCopyModels.size())
	{
		gCopyModels[modelIndex]->setAppendMeshMode(on);
	}
}


char freyjaModelCopyMesh(index_t modelIndex, index_t mesh, index_t frame)
{
	if (modelIndex < gCopyModels.size())
	{
		return gCopyModels[modelIndex]->copyMesh(mesh, frame);
	}

	return -1;
}


char freyjaModelPasteMesh(index_t modelIndex)
{
	if (modelIndex < gCopyModels.size())
	{
		return gCopyModels[modelIndex]->pasteMesh();
	}

	return -1;
}


char freyjaModelCopyVertexList(index_t modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame)
{
	if (modelIndex < gCopyModels.size())
	{
		return gCopyModels[modelIndex]->copyVertexBuffer(list, mesh, frame);
	}

	return -1;
}



//////////////////////////////////////////////////////////////////////
// Plugin subsystem
//////////////////////////////////////////////////////////////////////

// TODO Replace FreyjaFSM plugin control


void freyjaPluginDirectoriesInit()
{
#ifdef WIN32
	freyjaPluginAddDirectory("C:/freyja/modules/model");
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


	if (!dir || !dir[0] || !FreyjaFileReader::isDirectory(dir))
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


void freyjaPluginsInit()
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	FreyjaPluginDesc plugin;
	char *module_filename;
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


	freyjaPluginBegin();
	freyjaPluginDescription1s("GooseEgg 8 Model (*.egg)");
	freyjaPluginAddExtention1s("*.egg");
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
		if (!reader.openDirectory(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while ((module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
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

		reader.closeDirectory();
	}

	gCurrentFreyjaPlugin = -1;
#endif
}


int32 freyjaImportModel(const char *filename)
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool loaded = false, done = false;
	char *module_filename;
	int (*import)(char *filename);
	void *handle;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]");

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
	if (gEgg)
	{
		if (Egg::checkFile(filename) == 0)
		{
			if (gEgg->loadFile(filename) == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
	}

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
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

		reader.closeDirectory();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#else
	FreyjaFileReader reader;

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
	if (gEgg)
	{
		if (Egg::checkFile(filename) == 0)
		{
			if (gEgg->loadFile(filename) == 0)
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
	FreyjaFileReader reader;
	bool saved = false;
	char module_filename[128];
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
	else if (strcmp(type, "egg") == 0)
	{
		if (gEgg)
		{
			return gEgg->saveFile(filename);
		}
	}

	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");

	name = (char*)type;

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(gPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

#ifdef WIN32
		sprintf(module_filename, "%s/%s.dll", gPluginDirectories[i], name);
#else
		sprintf(module_filename, "%s/%s.so", gPluginDirectories[i], name);
#endif
		sprintf(module_export, "freyja_model__%s_export", name);  // use 'model_export'?

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
		if (gEgg)
		{
			return gEgg->saveFile(filename);
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
	Vector<long> polygons, texcoords;
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);
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
}




///////////////////////////////////////////////////////////////////////
// C++ API 
///////////////////////////////////////////////////////////////////////

int32 freyjaFindPolygonByVertices(Vector<uint32> vertices)
{
#ifdef UMBRA
	freyjaPrintMessage("findPolygonByVertices> Not Implemented for UMBRA");
	return -1;
#else // EGG
	Egg *model = freyja__getEggBackend();

	if (!model)
		return -1;

	return model->selectPolygon(&vertices);
#endif
}


///////////////////////////////////////////////////////////////////////
// Managed ABI 
///////////////////////////////////////////////////////////////////////

// Hidden API
Egg *freyja__getEggBackend()
{
	return gEgg;
}


// Hidden API
int freyja__spawnCopyModel(Egg *egg)
{
	if (!egg)
		return -1;

	freyjaPrintMessage("Spawned CopyModel");
	gCopyModels.pushBack(new CopyModel(egg));

	return gCopyModels.size();
}


// Hidden API
void freyja__setPrinter(FreyjaPrinter *printer, bool freyjaManaged)
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

	/* Hookup gobal classes using a Printer child class */
	gEgg->setPrinter(printer);
}


void freyjaSpawn()
{
	if (!FreyjaFSM::mFreyjaFSM)
	{
		gEgg = new Egg();
		FreyjaFSM *fsm = new FreyjaFSM();

		/* Here just to avoid compiler warnings and removal by opt */
		fsm->freyjaGetCount(FREYJA_VERTEX);

		/* Setup basic default stdout printer */
		freyja__setPrinter(new FreyjaPrinter(), true);

		/* Spawn cut/copy/paste system */
		freyja__spawnCopyModel(gEgg);

		/* Setup plugins */
		freyjaPluginDirectoriesInit();
		freyjaPluginsInit();

		freyjaPrintMessage("libfreyja invoked using freyjaSpawn()");
	}
}


void freyjaFree()
{
	FreyjaFSM *FreyjaFSM = FreyjaFSM::mFreyjaFSM;
	
	freyjaPrintMessage("libfreyja stopped using freyjaFree()");

	if (FreyjaFSM)
	{
		//egg = FreyjaFSM->getEgg();
		delete FreyjaFSM;
	}

	if (gEgg)
	{
		delete gEgg;
	}

	if (gPrinter)
	{
		delete gPrinter;
	}

	gEgg = 0x0;

	gPluginDirectories.erase();

	gFreyjaPlugins.erase();
}
 

/* Thanks to Sam for the WIN32 module loader example */
void *freyjaModuleImportFunction(void *handle, const char *name)
{
	char *loaderror = 0x0;
	void *symbol = NULL;

#ifdef WIN32
	char errbuf[512];

	symbol = (void *)GetProcAddress((HMODULE)handle, name);

	if (symbol == NULL)
	{
		FormatMessage((FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_FROM_SYSTEM),
				NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				errbuf, 512, NULL);
		loaderror = errbuf;
	}

#else // UNIX is always assumed here, everything else is special case

	symbol = dlsym(handle, name);

	if (symbol == NULL)
	{
		loaderror = (char *)dlerror();
	}

#endif

	if (symbol == NULL)
	{
		freyjaPrintError("Failed to import %s: %s", name, loaderror);
	}

	return symbol;
}


void *freyjaModuleLoad(const char *module)
{
	void *handle = NULL;
	char *loaderror;

#ifdef WIN32
	char errbuf[512];

	if (FreyjaFileReader::compareFilenameExtention(module, ".dll") != 0)
	{
		return NULL;
	}

	handle = (void *)LoadLibrary(module);

	/* Generate an error message if all loads failed */
	if (handle == NULL) 
	{
		FormatMessage((FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_FROM_SYSTEM),
				NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				errbuf, 512, NULL);
		loaderror = errbuf;
	}

#else

	if (FreyjaFileReader::compareFilenameExtention(module, ".so") != 0)
	{
		return NULL;
	}

	handle = dlopen(module, RTLD_NOW);
	loaderror = (char *)dlerror();

#endif

	if (handle == NULL)
	{
		freyjaPrintError("Failed to load %s: %s", module, loaderror);
	}

	return handle;
}


void freyjaModuleUnload(void *handle)
{
	if (handle == NULL)
	{
		return;
	}

#ifdef WIN32
	FreeLibrary((HMODULE)handle);
#else
	dlclose(handle);
#endif
}
