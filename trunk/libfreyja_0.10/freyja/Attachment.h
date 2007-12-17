/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Attachment
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Comments: 
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.05.01:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_ATTACHMENT_H_
#define GUARD__FREYJA_ATTACHMENT_H_

#include <hel/Vector3d.h>
#include <mstl/Vector.h>
#include <math.h>

#include "Bone.h"

namespace freyja {

class Attachment :
	public Bone
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Attachment() : Bone( ) {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~Attachment() {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};


} // namespace freyja

#endif // GUARD__FREYJA_ATTACHMENT_H_
