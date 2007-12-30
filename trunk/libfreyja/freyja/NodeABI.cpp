/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the Node C ABI.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#include "Node.h"
#include "NodeABI.h"

using namespace freyja;


///////////////////////////////////////////////////////////////////////
// Node 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaNodeDuplicate( freyja_ptr node_ptr )
{
	freyja::Node* node = Node::Cast( node_ptr );
	return (( node_ptr ) ? ((freyja_ptr*)node->Duplicate()) : NULL);
}


byte freyjaNodeGetFlags( freyja_ptr node_ptr )
{
	return 0; // Node HAS a flags?  :3
}


const char* freyjaNodeGetName( freyja_ptr node_ptr )
{
	freyja::Node* node = Node::Cast( node_ptr );
	return ( node ) ? node->GetName() : NULL;
}


freyja_ptr freyjaNodeGetParent( freyja_ptr node_ptr )
{
	freyja::Node* node = Node::Cast( node_ptr );
	return ( node ) ? node->GetParent() : NULL;
}

void freyjaNodeGetPosition( freyja_ptr node_ptr, vec3_t xyz )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->GetPosition().Get( xyz );
}

byte freyjaNodeIsSelected( freyja_ptr node_ptr )
{
#warning FIXME This should be resolved by final design.
	return 0;
}



///////////////////////////////////////////////////////////////////////
// freyjaNodeSet
///////////////////////////////////////////////////////////////////////

void freyjaNodeSetFlags( freyja_ptr node_ptr, byte flags )
{
}


void freyjaNodeSetName( freyja_ptr node_ptr, const char* name )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->SetName( name );
}


void freyjaNodeSetParent( freyja_ptr node_ptr, freyja_ptr parent )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->SetParent( Node::Cast( parent ) );	
}


void freyjaNodeSetPosition3fv( freyja_ptr node_ptr, const vec3_t xyz )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->SetPosition( hel::Vec3(xyz) );
}


void freyjaNodeSetPosition3f( freyja_ptr node_ptr, vec_t x, vec_t y, vec_t z )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->SetPosition( hel::Vec3(x,y,z) );
}

void freyjaNodeSetOrientationQuat4f( freyja_ptr node_ptr,
                                     vec_t w, vec_t x, vec_t y, vec_t z )
{
	freyja::Node* node = Node::Cast( node_ptr );
	if ( node ) node->SetOrientation( hel::Quat(w,x,y,z) );
}





