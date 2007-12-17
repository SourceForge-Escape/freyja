/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: 
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_EDGE_H_
#define GUARD__FREYJA_EDGE_H_

#include <hel/math.h>

#include "freyja.h"


namespace freyja {

	// No use serializing edges, as they can be generated.

class Vertex;
class Face;

class Edge
{
public:

	// Maybe break up subdiv / geometry flags.
	typedef enum {

		fUnknown = 0, /* The state or marking of this edge is unknown. */
		fNormal,      /* Implies shared edge between facets. */
		fDegenerate,  /* Both vertices are the same, or one is non existant. */
		fBoundary,
		fDart,
		fCorner,
		fCrease

	} Flags;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	static Edge* CreateEdge( freyja::Vertex* a, freyja::Vertex* b );
	/*------------------------------------------------------
	 * Pre  : <a> and <b> must be allocated and <a> != <b>.
	 *
	 * Post : Returns new Edge or NULL if precondition fails.
	 *
	 ------------------------------------------------------*/

	~Edge( );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Destructor. 
	 *
	 ------------------------------------------------------*/

	bool operator ==( const Edge& edge ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void WeldVertices( Vertex* replace, Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : The <vertex> is neither <mA> or <mB>.
	 * Post : Welding utility interface. 
	 *
	 ------------------------------------------------------*/

protected:

	Edge( freyja::Vertex* a, freyja::Vertex* b );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Constructor. 
	 *
	 ------------------------------------------------------*/

	byte mFlags; /* This edge is a boundry, dart, etc */

	freyja::Vertex* mA;  /* Vertices composing this edge. */

	freyja::Vertex* mB;

	mstl::list<freyja::Face*> mFaceRefs; /* Faces containing this edge. */
};


inline
Edge::Edge( freyja::Vertex* a, freyja::Vertex* b ) : 
	mFlags(fUnknown), 
	mA(a), 
	mB(b), 
	mFaceRefs() 
{
	// FIXME: Attach to a / b / faces as observer either in this class or above.

	// FIXME: Determine if this is a valid edge by checking faces.
}


inline
Edge* Edge::CreateEdge( freyja::Vertex* a, freyja::Vertex* b )
{
	return ( ( a == NULL || b == NULL || a == b )
			 ? NULL : new Edge( a, b ) );
}


inline
Edge::~Edge( ) 
{
	// NotifyDelete( ) vertices and faces?

	mA = mB = NULL;
	mFaceRefs.clear();
}


inline
bool Edge::operator ==( const Edge& edge ) const 
{ 
	return ( (edge.mA == mA && edge.mB == mB) || 
			 (edge.mA == mB && edge.mB == mA) ); 
}


inline
void Edge::WeldVertices( Vertex* replace, Vertex* vertex )
{
	if ( mA == replace ) 
	{
		mA = vertex;
	}
	else if ( mB == replace ) 
	{
		mB = vertex;
	}
}


} // namespace freyja

#endif // GUARD__FREYJA_EDGE_H_
