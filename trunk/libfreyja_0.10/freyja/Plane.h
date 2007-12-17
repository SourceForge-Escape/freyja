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

#ifndef GUARD__FREYJA_PLANE_H_
#define GUARD__FREYJA_PLANE_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include "Face.h"

#include "freyja.h"


namespace freyja {

class Plane
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Plane(Face *f, const vec_t a, const vec_t b, const vec_t c, const vec_t d) :
		mFace(f),
		mVisible(0x0),
		mA(0.0f), mB(0.0f), mC(0.0f), mD(0.0f), mNext(NULL) {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Plane() : mFace(NULL), mVisible(0x0),
			  mA(0.0f), mB(0.0f), mC(0.0f), mD(0.0f), mNext(NULL) {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~Plane()
	{
		Plane *cur = mNext, *tmp;
		while (cur) { tmp = cur; cur = cur->mNext; delete tmp; }
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void Calculate(const vec3_t a, const vec3_t b, const vec3_t c)
	{
		mA = ( a[2] * (b[2] - c[2]) + b[1] * (c[2] - a[2]) + 
			   c[1] * (a[2] - b[2]) );

		mB = ( a[2] * (b[0] - c[0]) + b[2] * (c[0] - a[0]) + 
			   c[2] * (a[0] - b[0]) );

		mC = ( a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + 
			   c[0] * (a[1] - b[1]) );

		mD = -( a[0] * ( b[1]*c[2] - c[1]*b[2] ) +
				b[0] * (c[1]*a[2] - a[1]*c[2]) +
				c[0] * (a[1]*b[2] - b[1]*a[2]) );
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void Append(const vec3_t a, const vec3_t b, const vec3_t c)
	{
		Plane *ins = new Plane();
		ins->Calculate(a, b, c);

		if (!mNext)
		{
			mNext = ins;
		}
		else
		{
			Plane *cur = mNext;

			while (cur)
			{
				if (!cur->mNext)
				{
					cur = cur->mNext = ins;
				}

				cur = cur->mNext;
			}
		}

		//return ins;
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	bool PointTest(uint32 flag, vec3_t p)
	{
		Plane *cur = this;
		mVisible &= ~flag;

		while (cur)
		{
			vec_t side = ( cur->mA * p[0] + cur->mB * p[1] + 
						   cur->mC * p[2] + cur->mD );

			if (side > 0)
			{
				mVisible |= flag;
				break;
			}

			cur = cur->mNext;
		}

		return ( mVisible & flag );
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void SetFaceRef(Face *face) { mFace = face; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Face *mFace;
	byte mVisible; // Lit 2^(light) bitflag
	vec_t mA, mB, mC, mD;
	Plane *mNext; 		// In case we have quad or polygon
};


} // namespace freyja

#endif // GUARD__FREYJA_PLANE_H_
