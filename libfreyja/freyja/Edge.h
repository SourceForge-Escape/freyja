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

class Edge
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Edge(const index_t  a, const index_t b) : mA(a), mB(b), mFaceRefs() {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Edge() : mA(INDEX_INVALID), mB(INDEX_INVALID), mFaceRefs() {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~Edge() { }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	bool operator ==(const Edge &e) const 
	{ return (e.mA == mA && e.mB == mB); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	byte mFlags;

	index_t mA, mB;

	mstl::Vector<index_t> mFaceRefs;
};


} // namespace freyja

#endif // GUARD__FREYJA_EDGE_H_

