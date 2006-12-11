/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005,2006 Mongoose
 * Comments: This is the ABI to expose Mesh class and utils to C.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Mesh.cpp
 ==========================================================================*/

#include <math.h>
#include "Mesh.h"
#include "Model.h"
#include "MeshABI.h"
#include "ModelABI.h"

using namespace freyja;


////////////////////////////////////////////////////////////////////////
// 0.9.5 C++ ABI
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
// 0.9.5 ABI
////////////////////////////////////////////////////////////////////////

// ModelABI is currently an empty vessel as you can see

uint32 freyjaGetModelCount()
{
	return 1;
}


index_t freyjaModelCreate()
{
	return 0;
}


void freyjaModelAddSkeleton(index_t model, index_t skeleton)
{
	
}


void freyjaModelAddMesh(index_t model, index_t mesh)
{
	
}


index_t freyjaGetModelMeshCount(index_t modelIndex)
{
	return freyjaGetMeshCount();
}


index_t freyjaGetModelFlags(index_t model)
{
	return 0x0;
}


index_t freyjaGetModelMeshIndex(index_t model, uint32 element)
{
	return element;
}


index_t freyjaGetModelSkeleton(index_t model)
{
	return 0;
}


void freyjaModelTransform(index_t model,
						  freyja_transform_action_t action, 
						  vec_t x, vec_t y, vec_t z)
{
	Vec3 v(x, y, z), min, max, center;

	

	// FIXME: Scaling and Rotation should use model center
	for (uint32 i = 0, n = freyjaGetModelMeshCount(model); i < n; ++i)
	{
		index_t mesh = freyjaGetModelMeshIndex(model, i);

		switch (action)
		{
		case fRotate:
			{
				Mesh *m = Mesh::GetMesh(mesh);
				if (m)
				{
					m->RotateAboutPoint(center, v);
				}
			}
			break;

		case fScale:
			{
				Mesh *m = Mesh::GetMesh(mesh);
				if (m)
				{
					m->ScaleAboutPoint(center, v);
				}
			}
			break;

		default:
			freyjaMeshTransform3fv(mesh, action, v.mVec);
		}
	}

	// FIXME: No skeleton support yet
}

