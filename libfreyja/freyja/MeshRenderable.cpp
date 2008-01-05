/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : MeshRenderable
 * Comments: 
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.25:
 * Mongoose - Created, expanded from Mesh.
 ==========================================================================*/

#include "Mesh.h"
#include "MeshRenderable.h"

using namespace freyja;


index_t MeshRenderable::ReserveIndexTriangle( index_t a, index_t b, index_t c )
{
	index_t off = mIndices.size();
	mIndices.push_back( a );
	mIndices.push_back( b );
	mIndices.push_back( c );

	MARK_MSGF( "%i <- %i %i %i", off, a, b, c );

	return off;
}


void MeshRenderable::Draw( freyja::RenderableStrategy* strategy )
{
	strategy->mTriangleCount = mIndices.size( ) / 3;
	strategy->mIndexArraySize = mIndices.size( );
	strategy->mIndexArrayPtr = mIndices.get_array( );

	strategy->mVertexWidth = 3;
	strategy->mVertexArraySize = mOwner->GetVertexArrayCount( );
	strategy->mVertexArrayPtr = (vec_t*)mOwner->GetVertexArray( );

	strategy->DrawTriangles( );
}


