/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Model
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is the model container class, which mainly seperates data structures to allow multiple editing contexts in one system.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.08.02:
 * Mongoose - Created
 ==========================================================================*/

#include "Mesh.h"
#include "Model.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Model::Model() :
	mFlags(0),
	mSkeleton(0),
	mMeshes()
{
}


Model::~Model()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Model::Intersect(hel::Ray &r, vec_t &t)
{
	if (GetFlags() & fHidden)
		return false;

	bool hit = false;
	vec_t ttmp;

	for (uint32 i = 0, n = mMeshes.size(); i < n; ++i)
	{
		Mesh *m = Mesh::GetMesh(mMeshes[i]);
		if (m)
		{
			if (m->Intersect(r, ttmp))
				hit = true;
		}
	}	

	t = ttmp;
	return hit;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Model::AddMesh(index_t mesh) 
{ 
	bool found = false;

	for (uint32 i = 0, n = mMeshes.size(); i < n; ++i)
	{
		if (mMeshes[i] == mesh)
		{
			found = true;
			break;
		}
		else if (mMeshes[i] == INDEX_INVALID)
		{
			mMeshes[i] = mesh;
			found = true;
		}
	}
	
	if (!found)
		mMeshes.push_back(mesh); 
}


void Model::RemoveMesh(index_t mesh) 
{
	for (uint32 i = 0, n = mMeshes.size(); i < n; ++i)
	{
		if (mMeshes[i] == mesh)
			mMeshes[i] = INDEX_INVALID; 
	}
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

#ifdef UNIT_TEST_MODEL
int runModelUnitTest(int argc, char *argv[])
{
	Model test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Model class test]\n");

	return runModelUnitTest(argc, argv);
}
#endif
