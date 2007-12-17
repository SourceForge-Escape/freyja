/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : VertexArray
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the VertexArray class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.10:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#include "VertexArray.h"

inline
void VertexArray::Repack( Observer* observer )
{
	if ( !mGaps.empty()	)
	{
		for (uint32 i = 0; i < mArray.size(); ++i )
		{
		// Precondition 1: Found a 'gap' vertex.
		if (mVertices[i] == NULL)
		{
			for (uint32 j = i; j < mVertices.size(); ++j)
			{
				// Precondition 2: Found a valid pointer later.
				if (mVertices[j])
				{
					// 1. Swap 'gap' pointer with valid vertex.
					mVertices[i] = mVertices[j];
					mVertices[j] = NULL;
						
					// 2. Now do a vertex weld to update references.
					WeldVertices(j, i);

					Vertex *v = mVertices[i];

					// Only Vertex objects reference vertex buffer directly, so
					// we can 'tear it up and rebuild' here.
					{
						Vec3 pos;
						GetTripleVec(mVertexPool, v->mVertexIndex, pos.mVec);

						v->mVertexIndex = i;
						//( !mFreedVertices.empty() ) ? mFreedVertices.pop() : i;

						SetTripleVec(mVertexPool, v->mVertexIndex, pos.mVec);
					}

					{
						Vec3 pos;
						GetTripleVec(mNormalPool, v->mNormalIndex, pos.mVec);

						v->mNormalIndex = i;
						//( !mFreedVertices.empty() ) ? mFreedVertices.pop() : i;

						SetTripleVec(mNormalPool, v->mNormalIndex, pos.mVec);
					}

					break;
				}
			}		
		}
	}

	/* Purge 'free' state.
	 * Only Vertex objects reference vertex buffer directly.
	 */
	while ( !mGaps.empty() )
	{
		mGaps.pop();
	}
}


inline
void Mesh::GetVec(Vector<vec_t> &v, uint32 n, index_t tIndex, vec_t *u)
{
	tIndex *= n;

	if (tIndex > v.end())
		return;

	vec_t *array = v.get_array();

	for ( uint32 i = 0; i < n; ++i)
	{
		u[i] = array[tIndex + i];
	}
}


inline
void SetVec(Vector<vec_t> &v, uint32 n, index_t tIndex, vec_t *u)
{
	tIndex *= n;

	if (tIndex > v.end())
		return;

	vec_t *array = v.get_array();

	for ( uint32 i = 0; i < n; ++i)
	{
		array[tIndex + i] = u[i];
	}
}


inline
index_t Mesh::AddTripleVec(Vector<vec_t> &v, mstl::stack<index_t> &f, vec3_t xyz)
{
	if (f.empty())
	{
		return AppendTripleVec(v, xyz);
	}

	index_t tIndex = f.pop();

	SetTripleVec(v, tIndex, xyz);

	return tIndex;
}


inline
index_t Mesh::AppendTripleVec(Vector<vec_t> &v, vec3_t xyz)
{
	v.push_back(xyz[0]);
	v.push_back(xyz[1]);
	v.push_back(xyz[2]);

	return (( v.end() / 3 ) - 1);
}


inline
void Mesh::TripleVec_Addition(Vector<vec_t> &v, const vec3_t xyz)
{
	uint32 i, size = v.size();
	vec_t *array = v.get_array();

	for ( i = 0; i < size; i += 3 )
	{
		array[i  ] += xyz[0];  
		array[i+1] += xyz[1];  
		array[i+2] += xyz[2];  
	}
}




inline
void Mesh::GetTripleVec(Vector<vec_t> &v, index_t tIndex, vec3_t xyz)
{
	tIndex *= 3;

	if (tIndex > v.end())
		return;

	vec_t *array = v.get_array();

	xyz[0] = array[tIndex    ];
	xyz[1] = array[tIndex + 1];
	xyz[2] = array[tIndex + 2];
}


inline
void Mesh::SetTripleVec(Vector<vec_t> &v, index_t tIndex, const vec3_t xyz)
{
	tIndex *= 3;

	if (tIndex > v.end())
		return;

	vec_t *array = v.get_array();

	array[tIndex    ] = xyz[0];
	array[tIndex + 1] = xyz[1];
	array[tIndex + 2] = xyz[2];
}


