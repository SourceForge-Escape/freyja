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

#include "FreyjaPluginABI.h"


/* Mesh generation utils */
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
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index =freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
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


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	long i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	count = freyjaGetCount(FREYJA_VERTEX); 

	list = new Vector<unsigned int>();

	if (count == FREYJA_PLUGIN_ERROR)
	{
		return list;
	}


	/* Using egg iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
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

		eggGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
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



void freyjaPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintMessage(format, &args);
	va_end(args);
}


void freyjaPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintError(format, &args);
	va_end(args);
}


int eggGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		freyjaPrintMessage("eggGenerateUVFromXYZ> ERROR: Passed invalid data");
		return -1;
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

	return 0;
}


void freyjaGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, v0, v1, v2, vertexCount, faceCount;
	long index;


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
		freyjaGetVertex3fv(v0, a.mVec);
		freyjaGetVertex3fv(v1, b.mVec);
		freyjaGetVertex3fv(v2, c.mVec);

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

		ref = freyjaVertexPolygonRef();

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


////////////////////////////////////////////////////////////////////


void freyjaBegin(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBegin(type);
}


void freyjaEnd()
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaEnd();
}


int eggTextureStore(EggTextureData *textureData)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStore(textureData);

	return -1;
}


unsigned int eggTexCoordStore2f(float s, float t)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTexCoordStore2f(s, t);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggVertexStore3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggVertexStore3f(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


void eggVertexWeightStore(unsigned int index, 
						  vec_t weight, unsigned int bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexWeightStore(index, weight, bone);
}


void eggVertexUVStore2f(unsigned int vIndex, float u, float v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexUVStore2f(vIndex, u, v);
}


void eggVertexNormalStore3f(unsigned int vIndex, float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexNormalStore3f(vIndex, x, y, z);
}


void eggVertex1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertex1i(egg_id);
}


void eggTexCoord1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexCoord1i(egg_id);
}


void eggTexture1i(int id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexture1i(id);  
}


unsigned int eggFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggFlags();

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGroupCenter3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGroupCenter(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggBoneName(char *name)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagName(name);

	return FREYJA_PLUGIN_ERROR;  
}

unsigned int eggBonePos3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagPos(x, y, z);

	return FREYJA_PLUGIN_ERROR;  
}


unsigned int eggBoneFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagFlags(flags);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggBoneAddMesh1u(unsigned int mesh)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddMesh(mesh);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggBoneAddSlave1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddSlave(tag);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggMeshTreeAddFrame3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeAddFrame(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggBoneRotate3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagRotate(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggBoneRotateQuaternion4f(vec_t w, vec_t x, vec_t y, vec_t z)
{
	vec_t heading, bank, attitude;
	Quaternion q = Quaternion(w, x, y, z);

	q.getEulerAngles(&heading, &bank, &attitude);

	// For now just switch to eular ( ew ) and do I have this right?
	if (EggPlugin::mEggPlugin) // h b a; a b h
		return EggPlugin::mEggPlugin->eggTagRotate(heading*57.295779513082323,
												   bank*57.295779513082323, 
												   attitude*57.295779513082323);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggMeshTreeBone1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeTag(tag);

	return FREYJA_PLUGIN_ERROR;
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


unsigned int eggTextureStoreFilename(char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreFilename(filename);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int eggTextureStoreBuffer(unsigned char *image, unsigned int depth,
								   unsigned int width, unsigned int height,
								   egg_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreBuffer(image, depth,
															width, height,
															type);

	return FREYJA_PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

unsigned int freyjaGetCount(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCount(type);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaIterator(egg_plugin_t type, int item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaIterator(type, item);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetVertex3f(float *xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetVertex(xyz);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetVertexNormal3f(float *xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetVertexNormal(xyz);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetPolygon1u(egg_plugin_t type, int item, 
							 unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetPolygon3f(egg_plugin_t type, int item, float *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}

unsigned int freyjaCriticalSection(egg_lock_t request)
{
	freyjaPrintMessage("freyjaCriticalSection> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return FREYJA_PLUGIN_ERROR;
}

unsigned int freyjaGetBoneName(unsigned int index, unsigned int size, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


void eggSetBoneParent(int index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);


	if (bone)
	{
		bone->parent = index;
	}	
}


int freyjaGetBoneParent(unsigned int index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);


	if (bone)
	{
		return bone->parent;
	}

	return -2;
}

unsigned int freyjaGetBoneRotationWXYZ4fv(unsigned int index, vec4_t wxyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);
	
	if (bone)
	{
		Quaternion q = Quaternion(bone->rot[0], bone->rot[1], bone->rot[2]);
		q.getQuaternion4fv(wxyz);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}

unsigned int freyjaGetBoneRotationXYZ3fv(unsigned int index, vec3_t xyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);
	
	if (bone)
	{
		xyz[0] = bone->rot[0];
		xyz[1] = bone->rot[1];
		xyz[2] = bone->rot[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}

unsigned int freyjaGetBoneTranslation3fv(unsigned int index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_tag_t *bone = EggPlugin::mEggPlugin->freyjaGetBone(index);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetBoneMesh1u(unsigned int item, unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTagMesh(item, value);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetCurrent(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCurrent(type);

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetBoneMeshNum()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTagMeshNum();

	return FREYJA_PLUGIN_ERROR;
}


unsigned int freyjaGetBoneRotate3f(float *x, float *y, float *z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTagRotate(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}

void eggMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggMeshFlags1u(flags);	
}



