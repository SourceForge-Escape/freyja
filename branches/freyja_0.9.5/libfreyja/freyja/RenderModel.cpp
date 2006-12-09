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
#include "freyja.h"
#include <hel/math.h>
#include "Mesh.h"
#include "MeshABI.h"
#include "LegacyMeshABI.h"

using namespace freyja;

Vector<RenderModel *> gRenderModels;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

RenderModel::RenderModel() :
	mIndex(0),
	mSkeleton()
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
	return freyjaGetModelMeshCount(0);
}


bool RenderModel::getMesh(int32 index, RenderMesh &mesh, int32 frame)
{
	mIndex = index;
	return getRenderMesh(index, mesh, frame);
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool RenderModel::getRenderPolygon(unsigned int index, RenderPolygon &rface)
{
	MARK_MSGF("getRenderPolygon Not Implemented");
	freyja::Mesh *mesh = Mesh::GetMesh(mIndex);

	if (mesh == NULL)
		return false;

	Face *face = mesh->GetFace(index);
	
	if (face == NULL)
		return false;

	rface.count = face->mIndices.size();
	rface.material = face->mMaterial;
	rface.id = index;
	rface.flags = face->mFlags;

	for ( uint32 i = 0, idx; i < rface.count; ++i )
	{
		idx = face->mIndices[i];
		// FIXME: we should be using arrays later once the modeler is updated
		rface.vertices[i]  = mesh->GetVertexPosition(idx);
		rface.normals[i]   = mesh->GetVertexNormal(idx);
		rface.texcoords[i] = mesh->GetVertexTexCoord(idx);
		rface.colors[i][0] = rface.colors[i][1] = rface.colors[i][2] = 1.0;
		rface.colors[i][3] = 0.75;
	}

	return (rface.count > 0);
}


bool RenderMesh::getPolygon(unsigned int index, RenderPolygon &rface)
{
	freyja::Mesh *mesh = Mesh::GetMesh(id);

	if (mesh == NULL)
		return false;

	Face *face = mesh->GetFace(index);

	if (face == NULL)
		return false;

	rface.count = face->mIndices.size();
	rface.material = face->mMaterial;
	rface.id = index;
	rface.flags = face->mFlags;

	for ( uint32 i = 0, idx; i < rface.count; ++i )
	{
		idx = face->mIndices[i];
		// FIXME: we should be using arrays later once the modeler is updated
		rface.vertices[i]  = mesh->GetVertexPosition(idx);
		rface.normals[i]   = mesh->GetVertexNormal(idx);
		rface.texcoords[i] = mesh->GetVertexTexCoord(idx);
		rface.colors[i][0] = rface.colors[i][1] = rface.colors[i][2] = 1.0;
		rface.colors[i][3] = 0.75;
	}

	return true;//(rface.count > 0);
}


bool RenderModel::getRenderMesh(uint32 meshIndex, RenderMesh &rmesh,
								int32 frame)
{
	if (Mesh::GetCount() == 0)
		return false;

	// FIXME: Fix this when we have true models in later
	freyja::Mesh *mesh = Mesh::GetMesh(meshIndex);

	rmesh.count = 0;

	if (mesh == NULL)
		return false;

	/* Mongoose 2004.03.26, 
	 * This was here for vertex morph frames, still used for edit updates */
	if (mesh->GetFaceCount())
	{
		rmesh.count = mesh->GetFaceCount();
		rmesh.gbegin = 0; 
		rmesh.gend = 1;
		rmesh.id = meshIndex;
		rmesh.frame = frame;

		//BUG_ME("Not Implemented");

#ifdef STORE_ON_RENDER_IS_DUMB		
		for (uint32 i = 0; i < rmesh.count; ++i)
		{ 
			freyja::Face *face = mesh->GetFace();

			if (face == NULL)
				continue;

			( face->mIndices.size() < 6 ) ? face->mIndices.size() : 6;

			for (uint32 j = 0, countJ = face->mIndices.size(); j < countJ; ++j)
			{
				polygon = mEgg->getPolygon(mesh.polygon[i]);
				
				if (polygon)
				{
					mesh.r_polygon.pushBack(polygon);
				}
			}
		}
#endif
	}

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Exported gobals
////////////////////////////////////////////////////////////

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


uint32 freyjaGetRenderModelCount()
{
	// FIXME: Right now have no models, since we still use Egg conventions 
	return gRenderModels.size() + 1;
}


bool freyjaGetRenderModel(uint32 modelIndex, RenderModel &model)
{
	// FIXME: Right now have no models, since we still use Egg conventions 
	if ( modelIndex == 0 )  // FIXME
	{
		model.mIndex = 0;
		return true;
	}
	else if (modelIndex < gRenderModels.size())
	{
		model = *(gRenderModels[modelIndex]);
		//model.mIndex = gRenderModels[modelIndex]->mIndex;
		//model.mEgg = gRenderModels[modelIndex]->mEgg;
		//model.mMeshlist = gRenderModels[modelIndex]->mMeshlist;

		MARK_MSG("gRenderModels Usage Not Implemented");
		return true;
	}

	return false;
}


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
