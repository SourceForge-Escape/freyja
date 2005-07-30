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
 *             compatiblity for binary C/C++ plugins.
 *             ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <math.h>

#ifdef WIN32
#   undef MODEL_PLUGINS
#endif

#ifdef MODEL_PLUGINS
#   include <dlfcn.h> 
#endif

#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "ActionManager.h"
#include "CopyModel.h"
#include "EggPlugin.h" // temp for testing
#include "FreyjaFileWriter.h"
#include "FreyjaPluginABI.h"
#include "Freyja.h"
#include "FreyjaCamera.h"
#include "FreyjaLight.h"
#include "FreyjaSkeleton.h"
#include "FreyjaMaterial.h"
#include "FreyjaTexture.h"
#include "FreyjaImage.h"


/* Internal / hidden API methods not exported by header */
Egg *freyja__getEggBackend();


/* Until Freyja replaces Egg backend, let these vector pools float here */
Vector<FreyjaSkeletalAnimation *> gFreyjaAnimations; 
Vector<FreyjaMetaData *> gFreyjaMetaData; 
Vector<FreyjaMaterial *> gFreyjaMaterials;

Vector<FreyjaTexture *> gFreyjaTextures;
uint32 gFreyjaTextureCount = 0;

Vector<FreyjaSkeleton *>  gFreyjaSkeletons;
Vector<FreyjaCamera *>  gFreyjaCameras;
Vector<FreyjaLight *>  gFreyjaLights;
Vector<CopyModel *>  gCopyModels;
Vector<FreyjaPluginDesc *> gFreyjaPlugins;
Vector<char *> gPluginDirectories;

FreyjaPrinter *gPrinter = 0x0;
Egg *gEgg = 0x0;
int32 gCurrentFreyjaPlugin = -1;
long FreyjaPluginDesc::mNextId = 1;
int32 gFreyjaCurrentVertex = -1;
int32 gFreyjaCurrentMesh = -1;


int32 freyjaGetCurrentVertex()
{
	return gFreyjaCurrentVertex;
}

void freyjaCurrentVertex(uint32 vertexIndex)
{
	gFreyjaCurrentVertex = vertexIndex;
}


char freyjaIsTexCoordAllocated(uint32 texcoordIndex)
{
	Egg *egg;
	egg_texel_t *texcoord;
	

	egg = freyja__getEggBackend();

	if (!egg)
		return 0;

	texcoord = egg->getTexel(texcoordIndex);

	return (texcoord != 0x0);
}


char freyjaIsPolygonAllocated(uint32 polygonIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg && egg->getPolygon(polygonIndex))
		return 1;

	return 0;
}


char freyjaIsVertexAllocated(uint32 vertexIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_vertex_t *vertex;
	

	if (!egg || vertexIndex == (uint32)-1) // cheap, but effective
		return 0;

	vertex = egg->getVertex(vertexIndex);

	//printf("%p <-- %u\n", vertex, vertexIndex);

	return (vertex != 0x0);
}


char freyjaIsBoneAllocated(uint32 boneIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
		return (egg->getTag(boneIndex) != 0x0);

	return 0;
}


FreyjaMaterial *freyjaGetMaterialClass(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex];
	}	

	return 0x0;
}


int32 freyjaMeshCreate()
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

	return -1;
}


int32 freyjaGetCurrentMesh()
{
	return gFreyjaCurrentMesh;
}


void freyjaMeshDelete(uint32 meshIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
		egg->delMesh(meshIndex);
}


void freyjaMeshTransform(uint32 meshIndex, uint32 frame,
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

	egg->Transform(egg->getMesh(meshIndex), type, x, y, z);

	egg_group_t *group = egg->getGroup(frame);
	egg_mesh_t *mesh = egg->getMesh(meshIndex);

	if (group && mesh)
		mesh->position = Vector3d(group->center);	
}


void freyjaMeshFrameTransform(uint32 meshIndex, uint32 frame,
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

	egg->Transform(egg->getGroup(frame), type, x, y, z);
}


void freyjaBoneTransform(uint32 boneIndex, 
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

	if (type != Egg::ROTATE)
	{
		egg->Transform(egg->getTag(boneIndex), type, x, y, z);
	}
	else
	{
		egg->TagRotateAbout(boneIndex, x, y, z);
	}
}


void freyjaMeshAddPolygon(int32 meshIndex, int32 polygonIndex)
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


void freyjaModelTransform(uint32 modelIndex,
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

	if (!strncmp(header.magic, FREYJA_PLUGIN_VERSION, 7)) // 'Freyja '
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
int32 freyjaSaveMeshChunkV1(FreyjaFileWriter &w, int32 meshIndex)
{
	const int32 version = 1;
	Vector<long> polygons, vertices, texcoords;
	Vector<long> verticesMap, texcoordsMap;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	int32 flags, bone, frame, material;
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


void freyjaMeshClampTexCoords(int32 meshIndex)
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
		
		freyjaTexCoordUV2fv(item, uv);
	}


	/* Clamp vertex UVs*/
	count = freyjaGetMeshVertexCount(meshIndex);

	for (i = 0; i < count; ++i)
	{
		item = freyjaGetMeshVertexIndex(meshIndex, i);
		freyjaGetVertexTexCoordUV2fv(item, uv);

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
				printf("BONE @ %i not %i!\n", r.getFileOffset(), offset);
			break;


		/* Materials */
		case FREYJA_CHUNK_MATERIAL:
			{
				FreyjaMaterial *mat;

				index = freyjaMaterialCreate();
				mat = freyjaGetMaterialClass(index);

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
	int32 index;


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


void freyjaGetVertexPolygonRef1i(int32 vertexIndex, Vector<long> &polygons)
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


int32 freyjaLightCreate()
{
	int32 lightIndex = gFreyjaLights.size();

	gFreyjaLights.pushBack(new FreyjaLight());
	// gFreyjaLights[lightIndex]->mId = lightIndex;

	return lightIndex;	
}

int32 gFreyjaLightIndex = -1;

int32 freyjaGetCurrentLight()
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
	

// General /////////

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


int32 freyjaTexCoord2fv(vec2_t uv)
{
	return freyjaTexCoord2f(uv[0], uv[1]);
}


int32 freyjaTexCoord2f(vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTexCoord2f(u, v);

	return FREYJA_PLUGIN_ERROR;
}

void freyjaVertexDelete(int32 vertexIndex)
{
	Egg *egg = freyja__getEggBackend();
	
	if (egg)
		egg->delVertex(vertexIndex);
}


int32 freyjaVertexCombine(int32 vertexIndexA, int32 vertexIndexB)
{
	Egg *egg = freyja__getEggBackend();
	
	if (egg)
	{
		egg->combineVertices(vertexIndexA, vertexIndexB);
		return vertexIndexA;
	}

	return -1;
}


int32 freyjaVertexCreate3fv(vec3_t xyz)
{
	return freyjaVertex3fv(xyz);
}


int32 freyjaVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(xyz[0], xyz[1], xyz[2]);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexFrame3f(int32 index, vec_t x, vec_t y, vec_t z)
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


int32 freyjaVertex3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexWeight(int32 index, vec_t weight, int32 bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexWeight(index, weight, bone);
}


void freyjaVertexTexCoord2fv(int32 vIndex, vec2_t uv)
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


void freyjaVertexTexCoord2f(int32 vIndex, vec_t u, vec_t v)
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


void freyjaVertexNormalFlip(int32 index)
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


void freyjaVertexNormal3fv(int32 vIndex, vec3_t nxyz)
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


void freyjaVertexNormal3f(int32 vIndex, vec_t x, vec_t y, vec_t z)
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


void freyja__PolygonReplaceReference(int32 polygonIndex, 
									 int32 vertexA, int32 vertexB)
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
		if ((int)a->ref[i] != polygonIndex)
		{
			ref.pushBack(a->ref[i]);
		}
	}

	a->ref.clear();
	a->ref.copy(ref);
}


void freyja__GetCommonPolygonReferences(int32 vertexA, int32 vertexB,
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


void freyja__GetDifferenceOfPolygonReferences(int32 vertexA, int32 vertexB,
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
			freyjaGetVertexTexCoordUV2fv(vertexIndex, uv);
			texcoordIndex = freyjaTexCoord2fv(uv);
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

			texcoordIndex = freyjaTexCoord2fv(uv);
			freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
		}
		
	}
}


int32 freyjaPolygonCreate()
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

	return -1;
}


void freyjaPolygonDelete(int32 polygonIndex)
{
	if (gEgg)
	{
		gEgg->delPolygon(polygonIndex);
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
		freyjaGetVertexXYZ3fv(A, a.mVec);
		B = freyjaGetPolygonVertexIndex(polygonIndex, 1);
		freyjaGetVertexXYZ3fv(B, b.mVec);
		C = freyjaGetPolygonVertexIndex(polygonIndex, 2);
		freyjaGetVertexXYZ3fv(C, c.mVec);
		D = freyjaGetPolygonVertexIndex(polygonIndex, 3);
		freyjaGetVertexXYZ3fv(D, d.mVec);

		helMidpoint3v(a.mVec, b.mVec, m1.mVec);
		M1 = freyjaVertex3fv(m1.mVec);
		freyjaGetVertexTexCoordUV2fv(A, uv1);
		freyjaGetVertexTexCoordUV2fv(B, uv2);
		uv[0] = (uv1[0] + uv2[0]) / 2;
		uv[1] = (uv1[1] + uv2[1]) / 2;
		freyjaVertexTexCoord2fv(M1, uv);
		freyjaGetVertexNormalXYZ3fv(A, n1.mVec);
		freyjaGetVertexNormalXYZ3fv(B, n2.mVec);
		n = n1 + n2;
		n.normalize();
		freyjaVertexNormal3fv(M1, n.mVec);

		helMidpoint3v(c.mVec, d.mVec, m2.mVec);
		M2 = freyjaVertex3fv(m2.mVec);
		freyjaGetVertexTexCoordUV2fv(C, uv1);
		freyjaGetVertexTexCoordUV2fv(D, uv2);
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
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.5));
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
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.5));
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

	if (freyjaIterator(FREYJA_POLYGON, polygonIndex) == FREYJA_PLUGIN_ERROR)
		return -1;

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

	return 0;
}


int32 freyjaVertexXYZ3fv(int32 vertexIndex, vec3_t xyz)
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


int freyjaPolygonExtrudeQuad(int32 polygonIndex, vec3_t normal)
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
		return -1;

	for (i = 0; i < count; ++i)
	{
		/* 1. Make duplicate vertices with same wind for 'face' */
		A = freyjaGetPolygonVertexIndex(polygonIndex, i);
		freyjaGetVertexXYZ3fv(A, xyz);
		B = freyjaVertex3fv(xyz);
		freyjaGetVertexTexCoordUV2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv);
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz);
		
		face.pushBack(B);


		/* 2. Replace all references to A with B ( dupe of A ), 
		 * except polygonIndex */
		freyjaGetVertexPolygonRef1i(A, ref);

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			if (ref[j] != polygonIndex)
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
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.5));
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

	return 0;
}


void freyjaPolygonAddVertex1i(int32 polygonIndex, int32 vertexIndex)
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


int freyjaVertexExtrude(int32 vertexIndex, vec_t midpointScale, vec3_t normal)
{
	Vector<long> faces, edges;
	Vector3d a, b, c, ab, ac;
	vec3_t xyz;
	vec2_t uv;
	int32 A, B, C, i, j, material, polygonCount, polygonIndex, vertexCount, texcoordCount, v, last;


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
		B = freyjaVertex3fv(ab.mVec);
		freyjaGetVertexTexCoordUV2fv(A, uv);
		freyjaVertexTexCoord2fv(B, uv); // FIXME
		freyjaGetVertexNormalXYZ3fv(A, xyz);
		freyjaVertexNormal3fv(B, xyz); // FIXME

		C = freyjaVertex3fv(ac.mVec);
		freyjaGetVertexTexCoordUV2fv(A, uv);
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
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.25, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.25));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.5));
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


void freyjaPolygonVertex1i(int32 egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonVertex1i(egg_id);
}


void freyjaPolygonAddTexCoord1i(int32 polygonIndex, int32 texcoordIndex)
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


void freyjaPolygonTexCoord1i(int32 egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonTexCoord1i(egg_id);
}


void freyjaPolygonSetMaterial1i(int32 polygonIndex, int32 materialIndex)
{
	if (gEgg)
	{
		egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

		if (polygon)
			polygon->shader = materialIndex;
	}
}


void freyjaPolygonMaterial1i(int32 id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonMaterial1i(id);  
}


uint32 freyjaGetFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaFlags();

	return 0;
}


char *freyjaGetMeshNameString(int32 meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->name;
	}

	return 0x0;
}


int32 freyjaGetMeshName1s(int32 meshIndex, int32 lenght, char *name)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		strncpy(name, mesh->name, lenght);
		name[lenght-1] = 0;
		return 0;
	}

	return -1;
}


void freyjaPolygonTexCoordPurge(int32 polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		polygon->texel.clear();
		polygon->r_texel.clear();
	}
}


void freyjaMeshUVMapPlanar(int32 meshIndex)
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


void freyjaMeshUVMapSpherical(int32 meshIndex)
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
void freyjaMeshMaterial(uint32 meshIndex, uint32 materialIndex)
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


uint32 freyjaGetModelDebugLevel(uint32 model)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
		return egg->getDebugLevel();

	return 0;
}


void freyjaModelDebugLevel(uint32 model, uint32 debugLevel)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
		egg->setDebugLevel(debugLevel);
}


// FIXME: Uses old egg gobal framing, etc
void freyjaMeshFrameCenter(uint32 meshIndex, uint32 frame, vec3_t xyz)
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
void freyjaGetMeshFrameCenter(uint32 meshIndex, uint32 frame, vec3_t xyz)
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


void freyjaMeshUVMapCylindrical(int32 meshIndex)
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


void freyjaMeshPromoteTexcoordsToPloymapping(int32 meshIndex)
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
				freyjaGetVertexTexCoordUV2fv(vertexIndex, uv);

				texcoordIndex = freyjaTexCoord2fv(uv);
				freyjaPolygonAddTexCoord1i(polygonIndex, texcoordIndex);
			}
		}
	}
}


int32 freyjaGetMeshFrameBoundingBox(int32 meshIndex, int32 frame, 
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

		return 1;
	}

	return 0;
}


void freyjaMeshGenerateVertexNormals(int32 meshIndex)
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


void freyjaMeshTesselateTriangles(int32 meshIndex)
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
int freyjaMeshRemovePolygon(int32 meshIndex, int32 polygonIndex)
{
	Vector<long> keep;
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);
	int32 i, count;


	if (mesh)
	{
		count = mesh->polygon.end();

		for (i = mesh->polygon.begin(); i < count; ++i)
		{
			if ((int)mesh->polygon[i] != polygonIndex)
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

		return 1;
	}

	return 0;
}


void freyjaMeshNormalFlip(int32 meshIndex)
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


void freyjaMeshName1s(int32 meshIndex, const char *name)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		strncpy(mesh->name, name, 64);
		mesh->name[63] = 0;
	}
}


int32 freyjaMeshPosition(int32 meshIndex, vec3_t xyz)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

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


void freyjaBoneAddVertex(int32 boneIndex, int32 vertexIndex)
{
	freyjaVertexWeight(vertexIndex, 1.0f, boneIndex);
}


void freyjaBoneRemoveVertex(int32 boneIndex, int32 vertexIndex)
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

			if (weight && (int32)weight->bone == boneIndex)
			{
				delete vert->weights[i];  /* Safe to null this out?  */
			}
		}
	}
}


void freyjaBoneFlags1i(int32 boneIndex, int32 flags)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		bone->flag = flags;
	}
}


void freyjaBoneParent1i(int32 boneIndex, int32 parentIndex)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		bone->parent = parentIndex;
	}
}


const char *freyjaGetBoneName1s(int32 boneIndex)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		return bone->name;
	}

	return 0x0;
}


void freyjaBoneName1s(int32 boneIndex, const char *name)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		strncpy(bone->name, name, 64);
	}
}


void freyjaBoneRemoveChild1i(int32 boneIndex, int32 childIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
	{
		egg->TagDisconnect(boneIndex, childIndex);
	}
}


void freyjaBoneAddChild1i(int32 boneIndex, int32 childIndex)
{
	//egg->connectTag(master, slave);
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		egg_tag_t *child =  gEgg->getTag(childIndex);
			
		if (child)
		{
			child->parent = bone->id;
		}
		
		// This is for plugin use -- will require child allocation in future
		bone->slave.pushBack(childIndex);
	}
}


void freyjaBoneRemoveMesh1i(int32 boneIndex, int32 meshIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
	{
		egg->TagDelMesh(egg->getTag(boneIndex), meshIndex);
	}
}


void freyjaBoneAddMesh1i(int32 boneIndex, int32 meshIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (egg)
	{
		egg->TagAddMesh(egg->getTag(boneIndex), meshIndex);
	}
}


void freyjaBoneTranslate3f(int32 boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		bone->center[0] = x;
		bone->center[1] = y;
		bone->center[2] = z;
	}
}


void freyjaBoneTranslate3fv(int32 boneIndex, vec3_t xyz)
{
	freyjaBoneTranslate3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateEulerXYZ3f(int32 boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		bone->rot[0] = x;
		bone->rot[1] = y;
		bone->rot[2] = z;
	}
}


void freyjaBoneRotateEulerXYZ3fv(int32 boneIndex, vec3_t xyz)
{
	freyjaBoneRotateEulerXYZ3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateQuatWXYZ4f(int32 boneIndex,vec_t w,vec_t x,vec_t y,vec_t z)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);

	if (bone)
	{
		vec_t heading, bank, attitude;
		Quaternion q = Quaternion(w, x, y, z);
		q.getEulerAngles(&heading, &bank, &attitude);

		bone->rot[0] = HEL_RAD_TO_DEG(attitude);
		bone->rot[1] = HEL_RAD_TO_DEG(bank);
		bone->rot[2] = HEL_RAD_TO_DEG(heading);
	}
}


void freyjaBoneRotateQuatWXYZ4fv(int32 boneIndex, vec4_t wxyz)
{
	freyjaBoneRotateQuatWXYZ4f(boneIndex, wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
}




void freyjaMeshTreeFrameAddBone(int32 tag)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshTreeFrameAddBone(tag);
}




// Accesors /////////////////////////////////////

uint32 freyjaGetCount(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCount(type);

	return 0; // 20050526, Was FREYJA_PLUGIN_ERROR;
}


int32 freyjaIterator(freyja_object_t type, int32 item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaIterator(type, item);

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaGetTexCoordPolygonRefIndex(int32 texcoordIndex, uint32 element)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);
		return t->ref[element];
	}

	return -1;
}


uint32 freyjaGetTexCoordPolygonRefCount(int32 texcoordIndex)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);
		return t->ref.size();
	}

	return 0;
}


void freyjaTexCoordUV2fv(int32 texcoordIndex, vec2_t uv)
{
	egg_texel_t *t;

	if (gEgg)
	{
		t = gEgg->getTexel(texcoordIndex);
		t->st[0] = uv[0];
		t->st[1] = uv[1];
	}
}


void freyjaGetTexCoord2fv(int32 tindex, vec2_t uv)
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


vec3_t *freyjaGetVertexXYZ(int32 vertexIndex)
{
	if (!gEgg)
		return 0x0;

	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);
			 
	if (vertex)
		return &(vertex->pos);

	return 0x0;
}


vec2_t *freyjaGetVertexUV(int32 vertexIndex)
{
	if (!gEgg)
		return 0x0;

	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);
			 
	if (vertex)
		return &(vertex->uv);
	
	return 0x0;
}


vec2_t *freyjaGetTexCoordUV(int32 texcoordIndex)
{
	if (!gEgg)
		return 0x0;

	egg_texel_t *texel = gEgg->getTexel(texcoordIndex);
			 
	if (texel)
		return &(texel->st); // really uv

	return 0x0;
}

int32 freyjaGetVertexPolygonRefIndex(int32 vertexIndex, uint32 element)
{
	egg_vertex_t *v;

	if (gEgg)
	{
		v = gEgg->getVertex(vertexIndex);
		return v->ref[element];
	}

	return -1;
}


uint32 freyjaGetVertexPolygonRefCount(int32 vertexIndex)
{
	egg_vertex_t *v;

	if (gEgg)
	{
		v = gEgg->getVertex(vertexIndex);
		return v->ref.size();
	}

	return 0;
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertex(xyz);
}


void freyjaGetVertexTexCoord2fv(vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexTexCoord(uv);
}


void freyjaGetVertexNormal3fv(vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(nxyz);
}


uint32 freyjaGetPolygonVertexCount(int32 polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->vertex.end();
	}

	return 0;
}


uint32 freyjaGetPolygonTexCoordCount(int32 polygonIndex)
{
	egg_polygon_t *polygon = gEgg->getPolygon(polygonIndex);

	if (polygon)
	{
		return polygon->texel.end();
	}

	return 0;
}


int32 freyjaGetVertexTexCoordUV2fv(int32 vertexIndex, vec2_t uv)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		uv[0] = vertex->uv[0];
		uv[1] = vertex->uv[1];
		return 0;
	}

	return -1;
}


int32 freyjaGetVertexNormalXYZ3fv(int32 vertexIndex, vec3_t nxyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		nxyz[0] = vertex->norm[0];
		nxyz[1] = vertex->norm[1];
		nxyz[2] = vertex->norm[2];
		return 0;
	}

	return -1;
}


int32 freyjaGetVertexXYZ3fv(int32 vertexIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		xyz[0] = vertex->pos[0];
		xyz[1] = vertex->pos[1];
		xyz[2] = vertex->pos[2];
		return 0;
	}

	return -1;
}


int32 freyjaGetVertexFrame(int32 vertexIndex, int32 element,
						  int32 *frameIndex, vec3_t xyz)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);


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


int32 freyjaGetVertexFrameCount(int32 vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->frames.end();
	}

	return 0;
}


int32 freyjaGetVertexWeight(int32 vertexIndex, int32 element,
						   int32 *bone, vec_t *weight)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex && element > -1 && element < (int)vertex->weights.end())
	{
		*bone = vertex->weights[element]->bone;
		*weight = vertex->weights[element]->weight;
		return 0;
	}

	return -1;
}


int32 freyjaGetVertexWeightCount(int32 vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->weights.end();
	}

	return 0;
}


int32 freyjaGetVertexFlags(int32 vertexIndex)
{
	egg_vertex_t *vertex = gEgg->getVertex(vertexIndex);

	if (vertex)
	{
		return vertex->flags;
	}

	return 0;
}


int32 freyjaGetModelFlags(int32 modelIndex)
{
	return 0x0;  // Not Implemented due to Egg backend use
}


int32 freyjaGetModelMeshIndex(int32 modelIndex, int32 element)
{
	// Not Implemented properly due to Egg backend use ( not scene based )
	egg_mesh_t *mesh = gEgg->getMesh(element);

	if (mesh)
	{
		return mesh->id;
	}

	return 0;
}


int32 freyjaGetModelMeshCount(int32 modelIndex)
{
	// Not Implemented properly due to Egg backend use ( not scene based )

	if (modelIndex == 0)
		return freyjaGetCount(FREYJA_MESH);
	
	return 0;
}


char freyjaIsMeshAllocated(int32 meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
		return 1;
	
	return 0;
}


int32 freyjaGetMeshPosition(int32 meshIndex, vec3_t xyz)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		xyz[0] = mesh->position.mVec[0];
		xyz[1] = mesh->position.mVec[1];
		xyz[2] = mesh->position.mVec[2];
		return mesh->id;
	}

	return 0;
}


int32 freyjaGetMeshFlags(int32 meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->flags;
	}

	return 0;
}


int32 freyjaGetMeshVertexFrameIndex(int32 meshIndex, int32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		// Not Implemented
	}

	return 0;
}


int32 freyjaGetMeshVertexFrameCount(int32 meshIndex)
{
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	int32 i, j, frames, maxFrames = 0, polygonIndex;
	int32 vertexIndex, vertexCount;

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


int32 freyjaGetMeshPolygonCount(int32 meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->polygon.end();
	}

	return 0;
}


int32 freyjaGetMeshPolygonVertexIndex(int32 meshIndex, int32 faceVertexIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);


	if (mesh && 
		faceVertexIndex > -1 && faceVertexIndex < (long)mesh->verticesMap.size())
	{
		return mesh->verticesMap[faceVertexIndex];
	}

	return -1;
}


int32 freyjaGetMeshVertexIndex(int32 meshIndex, int32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);


	if (mesh &&
		element > -1 && element < (long)mesh->vertices.size())
	{
			return mesh->vertices[element];
	}

	return -1;
}


int32 freyjaGetMeshVertexCount(int32 meshIndex)
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


int32 freyjaGetMeshTexCoordIndex(int32 meshIndex, int32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);


	if (mesh &&
		element > -1 && element < (long)mesh->texcoordsMap.size())
	{
			return mesh->texcoordsMap[element];
	}

	return -1;
}


uint32 freyjaGetMeshTexCoordCount(int32 meshIndex)
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


void freyjaMeshVertexGroupAppendGobalVertex(int32 meshIndex, int32 element, 
											int32 vertexIndex)
{
	int32 groupIndex = freyjaGetMeshVertexGroupIndex(meshIndex, element);
	Egg *egg = freyja__getEggBackend();

	if (egg && groupIndex > -1 && vertexIndex > -1)
	{
		egg_group_t *grp = egg->getGroup(groupIndex);
		grp->vertex.add(vertexIndex); //pushBack(vertexIndex);
	}
}


int32 freyjaGetMeshVertexGroupVertexCount(int32 meshIndex, int32 element)
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


int32 freyjaGetMeshVertexGroupVertexIndex(int32 meshIndex, int32 element,
										  int32 vertexElement)
{
	int32 groupIndex = freyjaGetMeshVertexGroupIndex(meshIndex, element);
	Egg *egg = freyja__getEggBackend();


	if (egg && groupIndex > -1)
	{
		egg_group_t *grp = egg->getGroup(groupIndex);

		if (vertexElement < (int32)grp->vertex.size() && vertexElement > -1)
			return grp->vertex[vertexElement];
	}

	return -1;
}


int32 freyjaGetMeshVertexGroupCount(int32 meshIndex)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->group.end();
	}

	return 0;
}


int32 freyjaGetMeshPolygonIndex(int32 meshIndex, int32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh && element > -1 && element < (int)mesh->polygon.end())
	{
		return mesh->polygon[element];
	}

	return -1;
}


int32 freyjaGetMeshVertexGroupIndex(int32 meshIndex, int32 element)
{
	egg_mesh_t *mesh = gEgg->getMesh(meshIndex);

	if (mesh && element > -1 && element < (int)mesh->group.end())
	{
		return mesh->group[element];
	}

	return -1;
}


int32 freyjaGetPolygonVertexIndex(int32 polygonIndex, int32 element)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return -1;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon || element < 0 || element >= (int)polygon->vertex.end())
		return -1; 

	return polygon->vertex[element];
}


int32 freyjaGetPolygonTexCoordIndex(int32 polygonIndex, int32 element)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return -1;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon || element < 0 || element >= (int)polygon->texel.end())
		return -1; 

	return polygon->texel[element];
}


int32 freyjaGetPolygonMaterial(int32 polygonIndex)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return 0;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon)
		return 0; 

	return polygon->shader;
}


int32 freyjaGetPolygonFlags(int32 polygonIndex)
{
	egg_polygon_t *polygon;

	if (!gEgg)
		return 0;

	polygon = gEgg->getPolygon(polygonIndex);

	if (!polygon)
		return 0;

	return polygon->vertex.end();
}


int32 freyjaGetPolygonEdgeCount(int32 polygonIndex)
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


int32 freyjaGetPolygon1u(freyja_object_t type, int32 item, int32 *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaGetPolygon3f(freyja_object_t type, int32 item, vec_t *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaCriticalSection(freyja_lock_t request)
{
	//freyjaPrintMessage("freyjaCriticalSection> Not implemented, %s:%i\n", 
	//				   __FILE__, __LINE__);

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaGetSkeletonBoneCount(int32 skeletonIndex)
{
	return freyjaGetCount(FREYJA_BONE); // Atm all one big reference skeleton in Egg backend
}


int32 freyjaGetSkeletonBoneIndex(int32 skeletonIndex, int32 element)
{
	egg_tag_t *bone = gEgg->getTag(element);
	// Atm all one big reference skeleton in Egg backend

	if (bone)
	{
		return bone->id;
	}

	return -1;
}


uint32 freyjaGetSkeletonRootIndex(uint32 skeletonIndex)
{
	return 0; // egg can only hold one skeleton, and root is always moved to 0
}


int32 freyjaGetBoneName(int32 index, unsigned int size, char *name)
{
	egg_tag_t *bone = gEgg->getTag(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaBoneCreate(uint32 skeletonIndex)
{
	Egg *egg = freyja__getEggBackend();

	if (!egg)
		return -1;

	egg_tag_t *tag = egg->addTag(0, 0, 0, 0x0);

	return tag->id;
}


void freyjaBoneParent(int32 index)
{
	egg_tag_t *bone = gEgg->getTag(freyjaGetCurrent(FREYJA_BONE));

	if (bone)
	{
		bone->parent = index;
	}	
}


int32 freyjaGetBoneParent(int32 index)
{
	egg_tag_t *bone = gEgg->getTag(index);

	if (bone)
	{
		return bone->parent;
	}

	return -2;
}


int32 freyjaGetBoneRotationWXYZ4fv(int32 index, vec4_t wxyz)
{
	egg_tag_t *bone = gEgg->getTag(index);
	
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


uint32 freyjaGetBoneSkeletalBoneIndex(int32 boneIndex)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);
	
	if (bone)
	{
		return bone->id;
	}

	return 0;
}


uint32 freyjaGetBoneChild(int32 boneIndex, uint32 childIndex)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);
	
	if (bone)
	{
		return bone->slave[childIndex];
	}

	return 0;
}


uint32 freyjaGetBoneChildCount(int32 boneIndex)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);
	
	if (bone)
	{
		return bone->slave.size();
	}

	return 0;
}


int32 freyjaGetBoneRotationXYZ3fv(int32 boneIndex, vec3_t xyz)
{
	egg_tag_t *bone = gEgg->getTag(boneIndex);
	
	if (bone)
	{
		xyz[0] = bone->rot[0];
		xyz[1] = bone->rot[1];
		xyz[2] = bone->rot[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaGetBoneTranslation3fv(int32 boneIndex, vec3_t xyz)
{
	if (gEgg)
	{
		egg_tag_t *bone = gEgg->getTag(boneIndex);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


int32 freyjaGetCurrent(freyja_object_t type)
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

FreyjaSkeletalAnimation *freyjaGetAnimation(int32 animationIndex);


int32 freyjaAnimationCreate()
{
	int32 animationIndex = gFreyjaAnimations.size();

	gFreyjaAnimations.pushBack(new FreyjaSkeletalAnimation());
	gFreyjaAnimations[animationIndex]->mId = animationIndex;

	return animationIndex;
	
}


int32 freyjaAnimationBoneCreate(int32 animationIndex, 
							   const char *name, int32 boneIndex)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);


	if (anim)
	{
		return anim->newBoneKeyFrame(name, boneIndex);
	}	

	return -1;
}


int32 freyjaAnimationBoneKeyFrameCreate(int32 animationIndex, int32 boneIndex,
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

FreyjaSkeletalAnimation *freyjaGetAnimation(int32 animationIndex)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		return gFreyjaAnimations[animationIndex]; 
	}	

	return 0x0;
}


int32 freyjaGetAnimationCount()
{
	return gFreyjaAnimations.size();
}


int32 freyjaGetAnimationFrameCount(int32 animationIndex)
{
	Egg *egg = freyja__getEggBackend();
	egg_animation_t *animation_frame;


	if (!egg)
		return 0;

	animation_frame = egg->getAnimation(animationIndex);

	if (animation_frame && animation_frame->frame.size())
	{
		return animation_frame->frame.size();
	}
	
	return 0;
}


int32 freyjaGetAnimationBoneCount(int32 animationIndex)
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


int32 freyjaGetAnimationBoneKeyFrameCount(int32 animationIndex, int32 boneIndex)
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

void freyjaAnimationName(int32 animationIndex, const char *name)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->setName(name);
		}
	}
}


void freyjaAnimationFrameRate(int32 animationIndex, vec_t frameRate)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mFrameRate = frameRate;
		}
	}
}


void freyjaAnimationTime(int32 animationIndex, vec_t time)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mTime = time;
		}
	}
}


void freyjaAnimationSubsetRoot(int32 animationIndex, int32 startBone)
{
	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
	{
		if (gFreyjaAnimations[animationIndex])
		{
			gFreyjaAnimations[animationIndex]->mStartBone = startBone;
		}
	}
}


//void freyjaAnimationSubsetCount(int32 animationIndex, int32 boneCount)
//{
//	if (animationIndex > -1 && animationIndex < (long)gFreyjaAnimations.size())
//	{
//		if (gFreyjaAnimations[animationIndex])
//		{
//			gFreyjaAnimations[animationIndex]->mBoneCount = boneCount;
//		}
//	}
//}


void freyjaAnimationBoneName(int32 animationIndex, int32 boneIndex,
							 const char *name)
{
	FreyjaSkeletalAnimation *anim = freyjaGetAnimation(animationIndex);

	if (anim)
	{
		anim->setBoneName(boneIndex, name);
	}
	
}


void freyjaAnimationKeyFrameTime(int32 animationIndex, int32 boneIndex, 
								 int32 keyFrameIndex, vec_t time)
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


void freyjaAnimationKeyFramePosition(int32 animationIndex, int32 boneIndex, 
									 int32 keyFrameIndex, vec3_t position)
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


void freyjaAnimationKeyFrameOrientationXYZ(int32 animationIndex, int32 boneIndex, 
										   int32 keyFrameIndex, vec3_t xyz)
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


void freyjaAnimationKeyFrameOrientationWXYZ(int32 animationIndex,
											int32 boneIndex, 
											int32 keyFrameIndex,vec4_t wxyz)
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
// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

index_t freyjaTextureCreateFilename(const char *filename)
{	
	FreyjaImage image;
	FreyjaTexture texture;
	index_t uid;
	freyja_colormode_t colorMode;
	uint32 byteDepth;


	if (image.loadImage(filename) != 0)
		return INDEX_INVALID;

	image.getImage(&texture.mImage);
	texture.mWidth = image.getWidth();
	texture.mHeight = image.getHeight();

	switch (image.getColorMode())
	{
	case FreyjaImage::RGBA_32:
		byteDepth = 32;
		colorMode = RGBA_32;
		break;

	case FreyjaImage::RGB_24:
		byteDepth = 24;
		colorMode = RGB_24;
		break;

	case FreyjaImage::INDEXED_8:
		byteDepth = 8;
		colorMode = INDEXED_8;
		break;

	default:
		byteDepth = 0;	
	}

	uid = freyjaTextureCreateBuffer(texture.mImage, byteDepth, 
									texture.mWidth, texture.mHeight,
									colorMode);

	/* Texture will delete the image copy here on scope exit */
	return uid;
}


index_t freyjaTextureCreateBuffer(byte *image, uint32 byteDepth,
                                  uint32 width, uint32 height,
                                  freyja_colormode_t type)
{
	FreyjaTexture *texture = new FreyjaTexture();
	index_t uid;
	uint32 i, count, size =  width * height * byteDepth;
	bool found = false;


	if (image == 0x0 || size == 0)
	{
		return INDEX_INVALID;
	}

	/* Setup texture */
	texture->mImage = new byte[size];
	memcpy(texture->mImage, image, size);
	texture->mWidth = width;
	texture->mHeight = height;
	texture->mBitDepth = byteDepth * 8;


	/* Setup UID and class container reference */
	uid = count = gFreyjaTextures.size();

	for (i = 0; i < count; ++i)
	{
		if (gFreyjaTextures[i] == 0x0)
		{
			uid = i;
			gFreyjaTextures.assign(uid, texture);

			found = true;
			break;
		}	
	}

	if (!found)
	{
		gFreyjaTextures.pushBack(texture);
	}

	++gFreyjaTextureCount;

	texture->mUID = uid;

	return uid;
}


void freyjaTextureDelete(index_t textureIndex)
{
	FreyjaTexture *texture;

	if (textureIndex < gFreyjaTextures.size())
	{
		texture = gFreyjaTextures[textureIndex];

		if (texture != 0x0)
		{
			delete texture;

			gFreyjaTextures.assign(textureIndex, 0x0);
			--gFreyjaTextureCount;
		}
	}
}


void freyjaGetTextureImage(index_t textureIndex,
                           uint32 *w, uint32 *h, uint32 *bitDepth,  
                           uint32 *type, byte **image)
{
	FreyjaTexture *texture;

	/* Init */
	*image = 0x0;
	*bitDepth = 0;
	*type = 0;
	*w = 0;
	*h = 0;

	if (textureIndex < gFreyjaTextures.size())
	{
		texture = gFreyjaTextures[textureIndex];

		if (texture != 0x0)
		{
			*image = texture->mImage;
			*bitDepth = texture->mBitDepth;
			*w = texture->mWidth;
			*h = texture->mHeight;

			switch (texture->mPixelFormat)
			{
			case FreyjaTexture::RGBA32:
				*type = RGBA_32; 
				break;

			case FreyjaTexture::RGB24:
				*type = RGB_24; 
				break;

			case FreyjaTexture::Indexed8:
				*type = INDEXED_8; 
				break;
			}
		}
	}
}


uint32 freyjaGetTexturePoolCount()
{
	return gFreyjaTextures.size();
}


uint32 freyjaGetTextureCount()
{
	return gFreyjaTextureCount;
}



///////////////////////////////////////////////////////////////////////
// Material ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

int32 gCurrentMaterial = -1;


int32 freyjaMaterialCreate()
{
	int32 materialIndex = gFreyjaMaterials.size();

	gFreyjaMaterials.pushBack(new FreyjaMaterial());
	gFreyjaMaterials[materialIndex]->mId = materialIndex;

	return materialIndex;
}


int32 freyjaLoadMaterialASCII(int32 materialIndex, const char *filename)
{
	FILE *f;
	unsigned int i, j, k, l, mode;
	char buffer[128];
	char buf[64];
	bool line_comment;
	char c;
	vec4_t ambient;
	vec4_t diffuse;
	vec4_t specular;
	vec4_t emissive;
	vec_t shininess;
	unsigned int texture;
	unsigned int blend_src;
	unsigned int blend_dest;
	int32 mIndex = materialIndex;
	//int32 mIndex = freyjaMaterialCreate();

	
	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "r");

	if (!f)
	{
		perror("freyjaLoadMaterialASCII> ");
		return -2;
	}

	i = 0;
	buffer[0] = 0;
	line_comment = false;
	mode = 0;

	// Strip out whitespace and comments
	while (fscanf(f, "%c", &c) != EOF)
	{
		if (line_comment && c != '\n')
			continue;

		if (i > 126)
		{
			printf("Material::loadFile> Overflow handled\n");
			i = 126;
		}
		
		switch (c)
		{
		case ' ':
		case '\v':
		case '\t':
			break;
		case '#':
			buffer[i++] = 0;
			line_comment = true;
			break;
		case '\n':
			if (line_comment)
			{
				line_comment = false;
				i = 0;
				buffer[0] = 0;		 
				continue;
			}
			else if (buffer[0] == 0)
			{
				i = 0;
				continue;
			}

			buffer[i] = 0;

			if (buffer[0] == '[')
			{
				if (strncmp(buffer, "[Material]", 10) == 0)
				{
					mode = 1;
				}
				else
				{
					mode = 0;
				}
			}
			else if (mode == 1)
			{
				if (strncmp(buffer, "Shininess", 9) == 0)
				{
					for (j = 0, k = 10; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					shininess = atof(buf);
				}
				else if  (strncmp(buffer, "TextureName", 11) == 0)
				{
					for (j = 0, k = 12; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					//setTextureName(buf);
				}
				else if  (strncmp(buffer, "Name", 4) == 0)
				{
					for (j = 0, k = 5; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					freyjaMaterialName(mIndex, buf);

					//setName(buf);
				}
				else if  (strncmp(buffer, "EnableBlending", 14) == 0)
				{
					for (j = 0, k = 15; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					if (strncmp(buf, "true", 4) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
					}
					else if (strncmp(buf, "false", 5) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
						//FIXME m_flags ^= Material::fEnable_Blending;
					}
				}
				else if (strncmp(buffer, "Blend", 5) == 0)
				{
					bool is_src = false;
					int val;


					if (strncmp(buffer, "BlendSource", 11) == 0)
					{
						is_src = true;
						k = 12;
					}
					else
					{
						k = 10;
					}

					for (j = 0; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					val = (strncmp(buf, "GL_ZERO", 11) == 0) ? 0x0 :
					(strncmp(buf, "GL_SRC_COLOR", 9) == 0) ? 0x0300 :
					(strncmp(buf, "GL_ONE_MINUS_SRC_COLOR", 22) == 0) ? 0x0301 :
					(strncmp(buf, "GL_DST_COLOR", 9) == 0) ? 0x0306 :
					(strncmp(buf, "GL_ONE_MINUS_DST_COLOR", 22) == 0) ? 0x0307 :
					(strncmp(buf, "GL_SRC_ALPHA", 9) == 0) ? 0x0302 :
					(strncmp(buf, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) ? 0x0303 :
					(strncmp(buf, "GL_DST_ALPHA", 9) == 0) ? 0x0304 :
					(strncmp(buf, "GL_ONE_MINUS_DST_ALPHA", 22) == 0) ? 0x0305 :
					(strncmp(buf, "GL_SRC_ALPHA_SATURATE", 21) == 0) ? 0x0308 :
					(strncmp(buf, "GL_CONSTANT_COLOR", 17) == 0) ? 0x8001 :
					(strncmp(buf, "GL_ONE_MINUS_CONSTANT_COLOR", 27) == 0) ? 0x8002 :
					(strncmp(buf, "GL_ONE", 6) == 0) ? 0x1 :
					(strncmp(buf, "GL_CONSTANT_ALPHA", 17) == 0) ? 0x8003 :					0x8004;

					if (is_src)
					{
						blend_src = val;
					}
					else
					{
						blend_dest = val;
					}
				}
				else if (strncmp(buffer, "Ambient", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							ambient[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					ambient[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Diffuse", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							diffuse[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					diffuse[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Specular", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							specular[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					specular[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Emissive", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							emissive[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					emissive[l++] = atof(buf);
				}
			}

			i = 0;
			buffer[0] = 0;
			break;
		default:
			buffer[i++] = c;
		}
	}

	fclose(f);

	freyjaMaterialAmbient(mIndex, ambient);
	freyjaMaterialDiffuse(mIndex, diffuse);
	freyjaMaterialSpecular(mIndex, specular);
	freyjaMaterialEmissive(mIndex, emissive);
	freyjaMaterialShininess(mIndex, shininess);
	freyjaMaterialTexture(mIndex, texture);
	freyjaMaterialBlendDestination(mIndex, blend_dest);
	freyjaMaterialBlendSource(mIndex, blend_src);

	return 0;
}



uint32 freyjaGetCurrentMaterial()
{
	return gCurrentMaterial;
}


void freyjaCurrentMaterial(uint32 materialIndex)
{
	gCurrentMaterial = materialIndex;
}


/* Material Accessors */

int32 freyjaGetMaterialCount()
{
	return gFreyjaMaterials.size();
}


int32 freyjaGetMaterialIndex(int32 materialIndex, int32 element)
{
	// This is mainly reserved for future use

	if (element > -1 && element < (long)gFreyjaMaterials.size())
	{
		return element;
	}

	return -1;
}


char *freyjaGetMaterialName(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mName;
	}	

	return 0x0;
}


int32 freyjaGetMaterialFlags(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mFlags;
	}	

	return -1;
}


int32 freyjaGetMaterialTexture(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTexture;
	}	

	return -1;
}


const char *freyjaGetMaterialTextureName(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->getTextureName();
	}

	return 0x0;
}


void freyjaGetMaterialAmbient(int32 materialIndex, vec4_t ambient)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			ambient[i] = gFreyjaMaterials[materialIndex]->mAmbient[i];
	}
}


void freyjaGetMaterialDiffuse(int32 materialIndex, vec4_t diffuse)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			diffuse[i] = gFreyjaMaterials[materialIndex]->mDiffuse[i];
	}
}


void freyjaGetMaterialSpecular(int32 materialIndex, vec4_t specular)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			specular[i] = gFreyjaMaterials[materialIndex]->mSpecular[i];
	}
}


void freyjaGetMaterialEmissive(int32 materialIndex, vec4_t emissive)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			emissive[i] = gFreyjaMaterials[materialIndex]->mEmissive[i];
	}
}


vec_t freyjaGetMaterialShininess(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mShininess;
	}	

	return -1.0f;
}


vec_t freyjaGetMaterialTransparency(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTransparency;
	}	

	return -1.0f;
}


int32 freyjaGetMaterialBlendSource(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendSrc;
	}	

	return -1;
}


int32 freyjaGetMaterialBlendDestination(int32 materialIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendDest;
	}	

	return -1;
}



/* Material Mutators */

void freyjaMaterialName(int32 materialIndex, const char *name)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		strncpy(gFreyjaMaterials[materialIndex]->mName, name, 64);
		gFreyjaMaterials[materialIndex]->mName[63] = 0;
	}	
}


void freyjaMaterialClearFlag(int32 materialIndex, int32 flag)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags |= flag;
		gFreyjaMaterials[materialIndex]->mFlags ^= flag;
	}
}


void freyjaMaterialSetFlag(int32 materialIndex, int32 flag)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags |= flag;
	}
}


void freyjaMaterialFlags(int32 materialIndex, int32 flags)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags = flags;
	}
}


void freyjaMaterialTexture(int32 materialIndex, int32 textureIndex)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTexture = textureIndex;
	}
}


void freyjaMaterialTextureName(int32 materialIndex, const char *name)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			gFreyjaMaterials[materialIndex]->setTextureName(name);
	}
}


void freyjaMaterialAmbient(int32 materialIndex, const vec4_t ambient)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mAmbient[i] = ambient[i];
	}
}


void freyjaMaterialDiffuse(int32 materialIndex, const vec4_t diffuse)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mDiffuse[i] = diffuse[i];
	}
}


void freyjaMaterialSpecular(int32 materialIndex, const vec4_t specular)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mSpecular[i] = specular[i];
	}
}


void freyjaMaterialEmissive(int32 materialIndex, const vec4_t emissive)
{
	int32 i;

	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mEmissive[i] = emissive[i];
	}
}


void freyjaMaterialShininess(int32 materialIndex, vec_t exponent)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mShininess = exponent;
	}
}


void freyjaMaterialTransparency(int32 materialIndex, vec_t transparency)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTransparency = transparency;
	}
}


void freyjaMaterialBlendSource(int32 materialIndex, uint32 factor)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendSrc = factor;
	}
}


void freyjaMaterialBlendDestination(int32 materialIndex,	uint32 factor)
{
	if (materialIndex > -1 && materialIndex < (long)gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendDest = factor;
	}
}



//////////////////////////////////////////////////////
// Model
//
//////////////////////////////////////////////////////

// FIXME: Uses Egg
void freyjaModelMirrorTexCoord(uint32 modelIndex, uint32 texCoordIndex,
								Vector<int32> uvMap, bool x, bool y)
{
	Egg *egg = freyja__getEggBackend();
	Vector<long> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel;
	egg_texel_t *tex;
	Vector3d u, trans, min;
	long i, j, k;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	texel = egg->getTexel(texCoordIndex);

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
}


// FIXME: Uses Egg
void freyjaModelTransformTexCoord(uint32 modelIndex, uint32 texCoordIndex,
									freyja_transform_action_t action,
									vec_t x, vec_t y)
{
	Egg *egg = freyja__getEggBackend();
	Vector<int32> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel;
	egg_texel_t *tex;
	Vector3d u, v, p;
	int32 i, j, k;
	Matrix m;
	vec_t z;


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	texel = egg->getTexel(texCoordIndex);

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
}


// FIXME: Uses Egg
void freyjaModelClear(uint32 modelIndex)
{
	Egg *egg = freyja__getEggBackend();


	// egg backend only supports 1 model at a time
	if (modelIndex != 0 || !egg)
		return;

	egg->clear();	
}


// FIXME: Uses Egg
void freyjaModelClampTexCoords(uint32 modelIndex)
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


char freyjaGetModelAppendMeshMode(int32 modelIndex)
{
	if (modelIndex > -1 && modelIndex < (long)gCopyModels.size())
	{
		return gCopyModels[modelIndex]->getAppendMeshMode();
	}

	return -1;
}


void freyjaModelAppendMeshMode(int32 modelIndex, char on)
{
	if (modelIndex > -1 && modelIndex < (long)gCopyModels.size())
	{
		gCopyModels[modelIndex]->setAppendMeshMode(on);
	}
}


char freyjaModelCopyMesh(int32 modelIndex, int mesh, int frame)
{
	if (modelIndex > -1 && modelIndex < (long)gCopyModels.size())
	{
		return gCopyModels[modelIndex]->copyMesh(mesh, frame);
	}

	return -1;
}


char freyjaModelPasteMesh(int32 modelIndex)
{
	if (modelIndex > -1 && modelIndex < (long)gCopyModels.size())
	{
		return gCopyModels[modelIndex]->pasteMesh();
	}

	return -1;
}


char freyjaModelCopyVertexList(int32 modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame)
{
	if (modelIndex > -1 && modelIndex < (long)gCopyModels.size())
	{
		return gCopyModels[modelIndex]->copyVertexBuffer(list, mesh, frame);
	}

	return -1;
}



//////////////////////////////////////////////////////////////////////
// Plugin subsystem
//////////////////////////////////////////////////////////////////////

// TODO Replace EggPlugin plugin control


void freyjaPluginDirectoriesInit()
{
#ifdef unix
   	freyjaPluginAddDirectory("/usr/lib/freyja/modules/model");
   	freyjaPluginAddDirectory("/usr/local/lib/freyja/modules/model");
	freyjaPluginAddDirectory("/usr/share/freyja/modules/model");
#else
	freyjaPluginAddDirectory("./modules/model");
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
	char *error;
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

#ifdef MACOSX
#endif

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				freyjaPrintError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				//freyjaPrintMessage("Module '%s' opened.", module_filename);

				import = (void (*)())dlsym(handle, "freyja_init");

				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}

				freyjaPluginBegin();
				freyjaPluginFilename1s(module_filename);
				(*import)();
				freyjaPluginEnd();

				if ((error = dlerror()) != NULL) 
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
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
	char *error;
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

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				freyjaPrintError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
				}

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
				import = (int (*)(char *filename))dlsym(handle, tmp);
#else
				import = (int (*)(char *filename))dlsym(handle, "import_model");
#endif

				if ((error = dlerror()) != NULL)  
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
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

				if ((error = dlerror()) != NULL) 
				{
					freyjaPrintError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
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
	char *error;
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

		sprintf(module_filename, "%s/%s.so", gPluginDirectories[i], name);
		sprintf(module_export, "freyja_model__%s_export", name);  // use 'model_export'?

		if (!(handle = dlopen(module_filename, RTLD_NOW)))
		{
			freyjaPrintError("\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
			}
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			export_mdl = (int (*)(char * filename))dlsym(handle, module_export);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
				dlclose(handle);
			}

			FreyjaPluginDesc *plug = freyjaGetPluginClassByName(module_filename); 

			if (plug)
				gCurrentFreyjaPlugin = plug->getId(); 

			saved = (!(*export_mdl)((char*)filename));

			gCurrentFreyjaPlugin = -1;

			if ((error = dlerror()) != NULL) 
			{
				dlclose(handle);
			}

			dlclose(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // success
#else
	if (!type || !filename)
		return -100;

	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return !saveModel(filename);
	}
	else if (strcmp(type, "egg") == 0)
	{
		return mEgg->saveFile((char *)filename);
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


int32 freyjaGetPluginCount()
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

int32 freyjaPakBegin(const char *filename)
{
	// ATM this does nothing, just here for reserved use
	return -1;
}


int32 freyjaPakAddFullPathFile(int32 pakIndex,
							  const char *vfsFilename, int32 offset, int32 size)
{
	// ATM this does nothing, just here for reserved use
	return -1;
}


void freyjaPakEnd(int32 pakIndex)
{
	// ATM this does nothing, just here for reserved use
}


///////////////////////////////////////////////////////////////////////
//  Internal ABI calls 
///////////////////////////////////////////////////////////////////////

// Hidden API
void freyja__MeshUpdateMappings(int32 meshIndex)
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
	if (!EggPlugin::mEggPlugin)
	{
		gEgg = new Egg();
		EggPlugin *eggplugin = new EggPlugin(gEgg);


		/* Here just to avoid compiler warnings and removal by opt */
		eggplugin->freyjaGetCount(FREYJA_VERTEX);

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
	//Egg *egg = 0x0;
	EggPlugin *eggplugin = EggPlugin::mEggPlugin;
	
	freyjaPrintMessage("libfreyja stopped using freyjaFree()");

	if (eggplugin)
	{
		//egg = eggplugin->getEgg();
		delete eggplugin;
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




