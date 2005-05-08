/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : CopyModel
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This holds all the 'copy and paste' classes
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.05.06:
 * Mongoose - Created, Moved from FreyjaModel for better abstraction
 ==========================================================================*/

#include "FreyjaPluginABI.h"

#include "CopyModel.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

CopyModel::CopyModel(Egg *egg)
{
	mEgg = egg;
	mAppendMode = false;
}


CopyModel::~CopyModel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool CopyModel::getAppendMeshMode()
{
	return mAppendMode;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool CopyModel::copyMesh(int currentMesh, int currentGroup)
{
	CopyVertex *v;
	CopyTexCoord *t;
	CopyPolygon *p;
	egg_mesh_t *mesh; 
	egg_group_t *group;
	egg_polygon_t *polygon;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	unsigned int i, j, index;


	if (!mEgg)
		return false;

	mesh = mEgg->getMesh(currentMesh);
	group = mEgg->getGroup(currentGroup);

	if (!mesh || !group)
		return false;

	// FIXME: You can add a back buffer stack/list here
	if (!mAppendMode)
		mCopyMesh.erase();

	mCopyMesh.flags = mesh->flags;
	mCopyMesh.center[0] = group->center[0];
	mCopyMesh.center[1] = group->center[1];
	mCopyMesh.center[2] = group->center[2];

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		index = mesh->polygon[i];
		polygon = mEgg->getPolygon(index);

		if (!polygon)
			continue;

		p = new CopyPolygon();
		p->material = polygon->shader;

		for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
		{
			index = polygon->vertex[j];
			vertex = mEgg->getVertex(index);

			v = new CopyVertex();

			if (vertex)  // This is to handle a whacky uv alignment issue
			{
				v->pos[0] = vertex->pos[0];
				v->pos[1] = vertex->pos[1];
				v->pos[2] = vertex->pos[2];
				v->uv[0] = vertex->uv[0];
				v->uv[1] = vertex->uv[1];
				v->norm[0] = vertex->norm[0];
				v->norm[1] = vertex->norm[1];
				v->norm[2] = vertex->norm[2];
			}
			else
			{
				freyjaPrintMessage("Selected mesh to copy has invalid vertex %i", 
							 index);
			}

			//FIXME: Add weight copy here, but this build handles
			//       weights so poorly no need until they're fixed

			mCopyMesh.vertices.pushBack(v);
			p->vertices.pushBack(mCopyMesh.vertices.end()-1);
		}

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			index = polygon->texel[j];
			texel = mEgg->getTexel(index);

			t = new CopyTexCoord();
			
			if (texel)
			{
				t->uv[0] = texel->st[0];
				t->uv[1] = texel->st[1];
			}
			else
			{
				freyjaPrintMessage("Selected mesh to copy has invalid texcoord %i", 
							 index);
			}

			mCopyMesh.texcoords.pushBack(t);
			p->texcoords.pushBack(mCopyMesh.texcoords.end()-1);	
		}

		mCopyMesh.polygons.pushBack(p);
	}	


	/* FIXME: Drop Animation frames for now ( duping them could waste memory,
	   and isn't likely to be expected by user ) */
	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		// FIXME: Remember if this is implemented, you have to skip the
		//        current group and reorder since its vertices are duped
	}	

	freyjaPrintMessage("Mesh[%i] copied...", mesh->id);

	return true;
}


bool CopyModel::copyVertexBuffer(Vector<unsigned int> &list, 
								 int currentMesh, int currentGroup)
{
	Vector<unsigned int> polygonList;
	CopyVertex *v;
	CopyTexCoord *t;
	CopyPolygon *p;
	egg_mesh_t *mesh;
	egg_group_t *group;
	egg_polygon_t *polygon;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	unsigned long i, j, k, r;
	long index, index2;
	Vector3d min, max, center;


	if (!mEgg)
		return false;

	mesh = mEgg->getMesh(currentMesh);
	group = mEgg->getGroup(currentGroup);

	if (!mesh || !group || list.empty())
		return false;

	// FIXME: You can add a back buffer stack/list here
	if (!mAppendMode)
		mCopyMesh.erase();

	min = Vector3d(999999.0f, 999999.0f, 999999.0f);
	max = Vector3d(-999999.0f, -999999.0f, -999999.0f);

	mCopyMesh.flags = mesh->flags;
	mCopyMesh.center[0] = group->center[0];
	mCopyMesh.center[1] = group->center[1];
	mCopyMesh.center[2] = group->center[2];

	for (i = list.begin(); i < list.end(); ++i)
	{
		index = list[i];
		vertex = mEgg->getVertex(index);

		if (!vertex)
			continue;

		for (k = 0; k < 3; ++k)
		{
			if (vertex->pos[k] < min.mVec[k])
				min.mVec[k] = vertex->pos[k];

			if (vertex->pos[k] > max.mVec[k])
				max.mVec[k] = vertex->pos[k];
		}

		for (r = vertex->ref.begin(); r < vertex->ref.end(); ++r)
		{
			index2 = vertex->ref[r];

			for (j = polygonList.begin(); j < polygonList.end(); ++j)
			{
				if (index2 == (int)polygonList[j])
				{
					index2 = -1;
					break;
				}
			}	
			
			if (index2 > -1)
			{
				polygonList.pushBack(index2);
			}
		}
	}

	center = (min + max) / 2;

	mCopyMesh.center[0] = center.mVec[0];
	mCopyMesh.center[1] = center.mVec[1];
	mCopyMesh.center[2] = center.mVec[2];

	for (i = polygonList.begin(); i < polygonList.end(); ++i)
	{
		index = polygonList[i];
		polygon = mEgg->getPolygon(index);

		if (!polygon)
			continue;

		p = new CopyPolygon();
		p->material = polygon->shader;

		for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
		{
			index = polygon->vertex[j];
			vertex = mEgg->getVertex(index);

			v = new CopyVertex();

			if (vertex)  // This is to handle a whacky uv alignment issue
			{
				v->pos[0] = vertex->pos[0];
				v->pos[1] = vertex->pos[1];
				v->pos[2] = vertex->pos[2];
				v->uv[0] = vertex->uv[0];
				v->uv[1] = vertex->uv[1];
				v->norm[0] = vertex->norm[0];
				v->norm[1] = vertex->norm[1];
				v->norm[2] = vertex->norm[2];
			}
			else
			{
				freyjaPrintMessage("Selected mesh to copy has invalid vertex %i", 
							 index);
			}

			//FIXME: Add weight copy here, but this build handles
			//       weights so poorly no need until they're fixed

			mCopyMesh.vertices.pushBack(v);
			p->vertices.pushBack(mCopyMesh.vertices.end()-1);
		}

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			index = polygon->texel[j];
			texel = mEgg->getTexel(index);

			t = new CopyTexCoord();
			
			if (texel)
			{
				t->uv[0] = texel->st[0];
				t->uv[1] = texel->st[1];
			}
			else
			{
				freyjaPrintMessage("Selected mesh to copy has invalid texcoord %i", 
							 index);
			}

			mCopyMesh.texcoords.pushBack(t);
			p->texcoords.pushBack(mCopyMesh.texcoords.end()-1);	
		}

		mCopyMesh.polygons.pushBack(p);
	}	

	freyjaPrintMessage("Vertex buffer copied...", mesh->id);
	return true;
}


bool CopyModel::pasteMesh()
{
	Vector<unsigned int> transV;
	Vector<unsigned int> transT;
	CopyVertex *v;
	CopyTexCoord *t;
	CopyPolygon *p;
	unsigned int i, j, index;


	if (mCopyMesh.vertices.empty() || mCopyMesh.polygons.empty())
		return false;

	freyjaBegin(FREYJA_MESH);
	freyjaMeshFlags1u(mCopyMesh.flags);
	freyjaMeshPosition(freyjaGetCurrent(FREYJA_MESH), mCopyMesh.center);

	freyjaBegin(FREYJA_VERTEX_GROUP);
	freyjaGroupCenter3f(mCopyMesh.center[0], 
						mCopyMesh.center[1],
						mCopyMesh.center[2]);

	for (i = mCopyMesh.vertices.begin(); i < mCopyMesh.vertices.end(); ++i)
	{
		v = mCopyMesh.vertices[i];
		//index = 0;

		if (v)
		{
			index = freyjaVertex3fv(v->pos);
			freyjaVertexNormal3fv(index, v->norm);
			freyjaVertexTexCoord2fv(index, v->uv);
		}

		transV.pushBack(index);
	}

	freyjaEnd(); // FREYJA_GROUP

	for (i = mCopyMesh.texcoords.begin(); i < mCopyMesh.texcoords.end(); ++i)
	{
		t = mCopyMesh.texcoords[i];
		//index = 0;

		if (t)
		{
			index = freyjaTexCoord2f(t->uv[0], t->uv[1]);
		}

		transT.pushBack(index);
	}

	for (i = mCopyMesh.polygons.begin(); i < mCopyMesh.polygons.end(); ++i)
	{
		p = mCopyMesh.polygons[i];

		if (!p)
			continue;

		freyjaBegin(FREYJA_POLYGON);

		freyjaPolygonMaterial1i(p->material);

		for (j = p->vertices.begin(); j < p->vertices.end(); ++j)
			freyjaPolygonVertex1i(transV[p->vertices[j]]);

		for (j = p->texcoords.begin(); j < p->texcoords.end(); ++j)
			freyjaPolygonTexCoord1i(transT[p->texcoords[j]]);

		freyjaEnd(); // FREYJA_POLYGON
	}
	

	freyjaEnd(); // FREYJA_MESH

	freyjaPrintMessage("Mesh pasted...");

	return true;
}


void CopyModel::setAppendMeshMode(bool on)
{
	mAppendMode = on;
}



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_COPYMODEL
int runCopyModelUnitTest(int argc, char *argv[])
{
	CopyModel test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[CopyModel class test]\n");

	return runCopyModelUnitTest(argc, argv);
}
#endif
