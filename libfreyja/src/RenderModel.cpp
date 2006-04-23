/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : RenderModel
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the runtime data structure translation class, which
 *           allows freyja to swap backend structures but export a
 *           standard structure for rendering
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.05.07:
 * Mongoose - Created
 ==========================================================================*/

#include "RenderModel.h"


// uint32 gRenderVertexCount = 0;  // Set to zero on update to avoid use in
                                   // renderer, don't worry about threads here

//Vector<Vector3d *> gRenderVertices;
//	Vector3d vertices[6];
//	Vector3d texcoords[6];
//	Vector3d normals[6];
//	vec4_t colors[6];

//	index_t *vindices


bool RenderMesh::createRenderPolygon(RenderPolygon &face,
									 egg_polygon_t &polygon, int32 frame)
{
	static vec_t *mpos;
	static Vector3d u, v, w;
	static unsigned int i, j, iend, jend;
	static egg_vertex_t *vertex;
	static egg_texel_t *texel;
	static egg_texel_t *texel2;
	static bool external_texel;


	face.count = 0;
	face.material = polygon.shader;
	face.id = polygon.id;
	face.flags = polygon.flags;

	external_texel = !polygon.r_texel.empty();

	if (polygon.r_vertex.empty() ||
		(!external_texel && (polygon.shader == COLORED_POLYGON)))
	{
		//freyjaPrintMessage("!FreyjaRender::DrawPolygon> Assertion failure, polygon %i malformed %s\n", polygon.id, (polygon.r_vertex.empty()) ? ": empty" : "");
		return false;
	}


	/* Mongoose 2004.12.23, 
	 * Setup vertex morphing for egg backend polygon */
	for (i = polygon.r_vertex.begin(), iend = polygon.r_vertex.end(); i < iend; ++i)
	{
		vertex = polygon.r_vertex[i];

		if (!vertex)
			continue;

		mpos = vertex->pos;

		if (frame > -1)
		{
			for (j = vertex->frameId.begin(), jend = vertex->frameId.end(); j < jend; ++j)
			{
				if (vertex->frameId[j] == frame)
				{
					mpos = *(vertex->frames[j]);
					break;
				}
			}

		}

		face.vertices[i] = Vector3d(mpos);
		face.normals[i] = Vector3d(vertex->norm);

		if (!external_texel)
		{
			face.texcoords[i] = Vector3d(vertex->uv[0], vertex->uv[1], 0);
		}
		else
		{
			if (polygon.shader == COLORED_POLYGON)
			{
				texel = polygon.r_texel[i*2];
				texel2 = polygon.r_texel[i*2+1];
	
				if (texel && texel2)
				{
					face.colors[i][0] = texel->st[0];
					face.colors[i][1] = texel->st[1];
					face.colors[i][2] = texel2->st[0];
					face.colors[i][3] = texel2->st[1];
				}
				else
				{
					face.colors[i][0] = face.colors[i][1] = face.colors[i][2] = 1.0;
					face.colors[i][3] = 0.75;
				}
			}
			else
			{
				texel = polygon.r_texel[i];
				face.texcoords[i] = Vector3d(texel->st[0], texel->st[1], 0);
			}
		}

		++face.count;
	}

	return true;
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

RenderModel::RenderModel()
{
}


RenderModel::~RenderModel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

RenderSkeleton &RenderModel::getSkeleton()
{
	mSkeleton.set(freyjaGetCurrentSkeleton());

	return mSkeleton;
}


unsigned int RenderModel::getMeshCount()
{
	if (mMeshlist->empty())
		return 0;
	
	return mMeshlist->end();
}


bool RenderModel::getMesh(int32 index, RenderMesh &mesh, int32 frame)
{
	return getRenderMesh(index, mesh, frame);
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool RenderModel::getRenderPolygon(unsigned int index, RenderPolygon &face)
{
	egg_polygon_t *poly = mEgg->getPolygon(index);

	if (poly)
	{
		return RenderMesh::createRenderPolygon(face, *poly, 0);
	}

	return false;
}


bool RenderModel::getRenderMesh(uint32 meshIndex, RenderMesh &rmesh,
								int32 frame)
{
	egg_mesh_t *mesh = mEgg->getMesh(meshIndex);

	if (mesh)
	{
		createRenderMesh(rmesh, *mesh, frame);
		return true;
	}

	return false;
}


void RenderModel::createRenderMesh(RenderMesh &rmesh, egg_mesh_t &mesh, 
								   int32 frameIndex)
{
	egg_polygon_t *polygon;
	egg_group_t *grp;
	unsigned int i;
	long frame = -1;


	/* Vertex morph frame fu */
	grp = mEgg->getGroup(frameIndex);
	if (grp && grp->flags == 0xBADA55)
		frame = grp->id;


	/* Mongoose 2004.03.26, 
	 * This was here for vertex morph frames, still used for edit updates */
	if (mesh.r_polygon.size() != mesh.polygon.size())
	{
		//freyjaPrintMessage("createRenderMesh> mesh[%i]: %i polygons, %i cached...", mesh.id, mesh.polygon.size(), mesh.r_polygon.size());

		for (i = mesh.polygon.begin(); i < mesh.polygon.end(); ++i)
		{
			polygon = mEgg->getPolygon(mesh.polygon[i]);
			
			if (polygon)
			{
				mesh.r_polygon.pushBack(polygon);
			}
		}
	}

	rmesh.setEgg(mEgg, &mesh, &mesh.r_polygon);
	rmesh.gbegin = mesh.group.begin(); 
	rmesh.gend = mesh.group.end();
	rmesh.id = mesh.id;
	rmesh.frame = frame;
}


void RenderModel::setEgg(Egg *egg)
{
	mEgg = egg;
	mMeshlist = egg->MeshList();
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

#ifdef UNIT_TEST_RENDERMODEL
int runRenderModelUnitTest(int argc, char *argv[])
{
	RenderModel test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[RenderModel class test]\n");

	return runRenderModelUnitTest(argc, argv);
}
#endif



Vector<RenderModel *> gRenderModels;


bool freyjaGetRenderModelPolygon(uint32 modelIndex, uint32 polygonIndex, 
								 RenderPolygon &face)
{
	if (modelIndex < gRenderModels.size())
	{
		return gRenderModels[modelIndex]->getRenderPolygon(polygonIndex, face);
	}

	return false;
}


bool freyjaGetRenderModelMesh(uint32 modelIndex, uint32 meshIndex, uint32 frame,
							  RenderMesh &mesh)
{
	if (modelIndex < gRenderModels.size())
	{
		return gRenderModels[modelIndex]->getRenderMesh(meshIndex, mesh, frame);
	}

	return false;
}



#define USING_EGG
#include "Egg.h"

uint32 freyjaGetRenderModelCount()
{
#ifdef USING_EGG 
	return 1;
#else
	return gRenderModels.size();
#endif
}


bool freyjaGetRenderModel(uint32 modelIndex, RenderModel &model)
{
#ifdef USING_EGG 
	extern Egg *freyja__getEggBackend();
	model.setEgg(freyja__getEggBackend());
	model.mIndex = 0;
	return true;
#else
	if (modelIndex < gRenderModels.size())
	{
		//model = &(gRenderModels[modelIndex]);
		model.mIndex = gRenderModels[modelIndex]->mIndex;
		model.mEgg = gRenderModels[modelIndex]->mEgg;
		model.mMeshlist = gRenderModels[modelIndex]->mMeshlist;

		return true;
	}
#endif

	return false;
}


