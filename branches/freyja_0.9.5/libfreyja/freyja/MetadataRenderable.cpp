/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Metadata
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: The user given metadata will be made into an XML structure.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#include "Mesh.h"

#include "MetadataRenderable.h"

using namespace freyja;

mstl::Vector<MetadataRenderable*> MetadataRenderable::mGobalPool;

MetadataRenderable* MetadataRenderable::mMRU = NULL;


MetadataRenderable* metadatarenderable_convert_mesh_to_renderable(Mesh* mesh)
{
	MetadataRenderable* renderable = NULL;

	if ( mesh )
	{
	}

	return renderable;
}


MetadataRenderable* MetadataRenderable::FindInCache(const char* key)
{
	if ( mMRU && mMRU->mModel == key )
		return mMRU;

	unsigned int i;
	foreach (mGobalPool, i)
	{
		if ( mGobalPool[i]->mModel == key )
		{
			// Cache last hit to speed up most common pattern.
			mMRU = mGobalPool[i];
			return mGobalPool[i];
		}
	}

	return NULL;
}


MetadataRenderable* MetadataRenderable::ImportToCache(const char* filename)
{
	MetadataRenderable* renderable = FindInCache( filename );

	if ( renderable == NULL )
	{
		Mesh* mesh = new Mesh();

		if ( mesh->UnserializeXML( filename ) )
		{
			mesh->ApplyTrianglarTesselation();

			renderable = new MetadataRenderable();
			renderable->mModel = filename;			

			renderable->mFaceCount = mesh->GetFaceCount();
			renderable->mIndices = new uint16[ renderable->mFaceCount * 3 ];

			for (uint32 i = 0; i < renderable->mFaceCount; ++i)	
			{
				Face* face = mesh->GetFace( i );
					
				if ( face )
				{
					uint32 idx = ( i << 1 ) + i; // i * 3
					renderable->mIndices[idx  ] = face->mIndices[0];
					renderable->mIndices[idx+1] = face->mIndices[1];
					renderable->mIndices[idx+2] = face->mIndices[2];
				}
			}

			{
				uint32 count = mesh->GetVertexArrayCount();
				vec_t* vertices = mesh->GetVertexArray();	
				renderable->mVertices = new vec_t[ count * 3 ];
				memcpy( renderable->mVertices, vertices, sizeof(vec_t) * count * 3 );

				vec_t* normals = mesh->GetNormalArray();

				if ( normals )
				{
					renderable->mNormals = new vec_t[ count * 3 ];
					memcpy( renderable->mNormals, normals, sizeof(vec_t) * count * 3 );
				}
			}

			vec_t* texcoords = mesh->GetTexCoordArray();
	
			// NOTE: When default UV goes back to 2fv from 3fv this must change.
			if ( texcoords )
			{
				uint32 count = mesh->GetTexCoordCount();
				renderable->mTexcoords = new vec_t[ count * 3 ];
				memcpy( renderable->mTexcoords, texcoords, sizeof(vec_t) * count * 3 );
			}
		}

		delete mesh;
	}

	if ( renderable )
	{
		++(renderable->mRefCount);		
	}

	return renderable;
}



