/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : Freyja3dCursor
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Created, moved into it's own header from FreyjaModel 'test'
 ==========================================================================*/

#include <hel/math.h>
#include <hel/Ray.h>
#include <hel/Mat44.h>
#include <mstl/stack.h>

#include "freyja_events.h"
#include "FreyjaState.h"
#include "FreyjaOpenGL.h"
#include "Cursor.h"

namespace freyja3d {

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Cursor::Cursor()
{
	Reset();
}


Cursor::~Cursor() 
{ 
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Cursor::Display()
{
	glPushMatrix();
	glTranslatef(mPos.mVec[0], mPos.mVec[1], mPos.mVec[2]);

	switch (GetMode())
	{
	case freyja3d::Cursor::Scale:
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);
		Draw3dCursorScale(min, mid, max);
		glPopAttrib();
		break;


	case freyja3d::Cursor::Rotation:
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		if (GetMode() == freyja3d::Cursor::Rotation)
		{
			// Debug
			//Draw3dCursorRotHitBoxes();

			glRotatef(mRotate.mVec[0], 1, 0, 0);
			glRotatef(mRotate.mVec[1], 0, 1, 0);
			glRotatef(mRotate.mVec[2], 0, 0, 1);
		}
		Draw3dCursorRot(min, mid, max);
		glPopMatrix();
		glPopAttrib();
		break;

	case freyja3d::Cursor::Translation:
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);
		Draw3dCursorLoc(min, mid, max);
		glPopAttrib();
		break;

	default:
	case freyja3d::Cursor::Invisible:
		break;
	}

	glPopMatrix();
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Cursor::CheckForRayCollision(hel::Ray &r)
{
	bool ret = false;

	switch (GetMode())
	{
	case freyja3d::Cursor::Rotation:
		{
			mSelected = false;

			// If this returns true mark mToken in Control
			// Also this has to match the rendered cursor *exactly to work

			// Ray test to pick rings of rotation
			// 1. Take the ray and generate a rect prism
			// 2. Use line segment / bbox detection to find
			//    nearest hit
			// 3. Return true if hit found, and set selected axis
			   

			hel::Mat44 rot;
			rot.SetRotation(helDegToRad(mRotate.mX), 
							helDegToRad(mRotate.mY), 
							helDegToRad(mRotate.mZ));

			// mPos <- center
			vec_t radius = max * 4.0f;
			const uint32 count = 64;

			vec_t x, z, i;
			Vector<vec_t> xs, zs;
			xs.reserve(count);
			zs.reserve(count);

			for (i = 0.0f; i < count; ++i)
			{
				helSinCosf(helDegToRad(360.0f * (i / count)), &x, &z);
					
				x *= radius;
				z *= radius;

				xs.push_back(x);
				zs.push_back(z);
			}

			// Pick the closest 'hit' from each 'axis' ring
			hel::Vec3 min, max;
			vec_t dist, best = 999.9f;
			bool hit;

			// 64 x 3 hit box tests could be optimized  =/
			for (uint32  j = 0; j < (count-1); ++j)
			{
				for (uint32  k = 0; k < 3; ++k)
				{
					hel::Vec3 a, b;

					switch (k)
					{
					case 0:
						a = mPos + hel::Vec3(0.0f, xs[j  ], zs[j  ]);
						b = mPos + hel::Vec3(0.0f, xs[j+1], zs[j+1]);
						break;

					case 1:
						a = mPos + hel::Vec3(xs[j  ], 0.0f, zs[j  ]);
						b = mPos + hel::Vec3(xs[j+1], 0.0f, zs[j+1]);
						break;

					case 2:
						a = mPos + hel::Vec3(xs[j  ], zs[j  ], 0.0f);
						b = mPos + hel::Vec3(xs[j+1], zs[j+1], 0.0f);
						break;
					}

					hel::Vec3 side = hel::Vec3::Cross(a, b);
					side.Norm();
					side *= 0.25f;
					min = a + side; 
					max = b - side;

					min = rot * min;
					max = rot * max;

					hit = r.IntersectBox(min.mVec, max.mVec, dist);
						
					if (hit && dist < best)
					{
						switch (k)
						{
						case 0:
							mAxis = eX;
							break;

						case 1:
							mAxis = eY;
							break;

						case 2:
							mAxis = eZ;
							break;
						}

						mSelected = true;
						best = dist;
					}
				}
			}

			ret = mSelected;
		}
		break;


	case freyja3d::Cursor::Scale:
	case freyja3d::Cursor::Translation:
		{
			vec_t z = 1.0f;
			vec_t t, dist = 9999.0f;
			hel::Vec3 o = hel::Vec3(z*freyja3d::Cursor::mid,0,0) + mPos;

			if (r.IntersectSphere(mPos.mVec, 1.0f, t))
			{
				mAxis = Cursor::eAll;
				mSelected = true;
				ret = true;
				dist = t;
			}

			if (r.IntersectSphere(o.mVec, Cursor::min*2, t) && t < dist)
			{
				mAxis = Cursor::eX;
				mSelected = true;
				ret = true;
				dist = t;
			}

			o = hel::Vec3(0, z*Cursor::mid,0) + mPos;

			if (!ret && r.IntersectSphere(o.mVec, Cursor::min*2, t) && t < dist)
			{
				mAxis = Cursor::eY;
				mSelected = true;
				ret = true;
				dist = t;
			}

			o = hel::Vec3(0, 0, z*Cursor::mid) + mPos;
				
			if (!ret && r.IntersectSphere(o.mVec, Cursor::min*2, t) && t < dist)
			{
				mAxis = Cursor::eZ;
				mSelected = true;
				ret = true;
				dist = t;
			}
		}
		break;

	default:
		ret = false;
	}

	return ret;
}


void Cursor::Reset()
{
	mSelected = false;
	mAxis = eNone;
	mMode = Invisible;
	mPos = hel::Vec3(0.0f, 0.0f, 0.0f);
	mScale = hel::Vec3(1.0f, 1.0f, 1.0f);
	mRotate = hel::Vec3(0.0f, 0.0f, 0.0f);//Quaternion();
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void Cursor::Draw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max)
{
	static int drawList = -1;

	if (drawList == -1)
	{
		drawList = glGenLists(1);
		glNewList(drawList, GL_COMPILE);
		mglDrawAxisWithCones(min, mid, max, 8);
		glEndList();
	}
	else
	{
		glCallList(drawList);
	}
}

void Cursor::Draw3dCursorRot(const vec_t min, const vec_t mid, const vec_t max)
{
	vec3_t center = {0.0f, 0.0f, 0.0f};
	vec_t radius = max * 4.0f;// / 2.0f;
	const uint32 count = 64;

	// red
	glColor3fv(RED);
	mglDraw3dCircle(center, radius, count, 1, true);

	// green
	glColor3fv(GREEN);
	mglDraw3dCircle(center, radius, count, 2, true);

	// blue
	glColor3fv(BLUE);
	mglDraw3dCircle(center, radius, count, 0, true);
}


void Cursor::Draw3dCursorScale(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(0.0,  0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);

	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  0.0, 0.0);	
      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0,  0.0);
	glEnd();


	glBegin(GL_QUADS);
      
	// X Axis, Red box
	glColor3fv(RED);
	glVertex3f(mid+min,  -min, min);
	glVertex3f(mid+min,  min, min);
	glVertex3f(mid-min,  min, min);
	glVertex3f(mid-min,  -min, min);

	glVertex3f(mid+min,  -min, -min);
	glVertex3f(mid+min,  min, -min);
	glVertex3f(mid-min,  min, -min);
	glVertex3f(mid-min,  -min, -min);

	glVertex3f(mid+min,  min, -min);
	glVertex3f(mid+min,  min, min);
	glVertex3f(mid-min,  min, min);
	glVertex3f(mid-min,  min, -min);

	glVertex3f(mid+min,  -min, -min);
	glVertex3f(mid+min,  -min, min);
	glVertex3f(mid-min,  -min, min);
	glVertex3f(mid-min,  -min, -min);

	glVertex3f(mid+min,  min, -min);
	glVertex3f(mid+min,  min, min);
	glVertex3f(mid+min,  -min, min);
	glVertex3f(mid+min,  -min, -min);


	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(-min, mid+min,  min);
	glVertex3f(min,mid+min,   min);
	glVertex3f(min,mid-min,   min);
	glVertex3f(-min,mid-min,   min);

	glVertex3f(-min,mid+min,   -min);
	glVertex3f(min,mid+min,   -min);
	glVertex3f( min,mid-min,  -min);
	glVertex3f(-min,mid-min,   -min);

	glVertex3f( min,mid+min,  -min);
	glVertex3f( min,mid+min,  min);
	glVertex3f(min,mid-min,   min);
	glVertex3f( min,mid-min,  -min);

	glVertex3f( -min,mid+min,  -min);
	glVertex3f( -min,mid+min,  min);
	glVertex3f(-min,mid-min,   min);
	glVertex3f(-min,mid-min,   -min);

	glVertex3f(min,mid+min,   -min);
	glVertex3f(min,mid+min,   min);
	glVertex3f( -min,mid+min,  min);
	glVertex3f(-min,mid+min,   -min);

      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(-min, min, mid+min);
	glVertex3f(min,   min, mid+min);
	glVertex3f(min,   min, mid-min);
	glVertex3f(-min,   min, mid-min);

	glVertex3f(-min,    -min, mid+min);
	glVertex3f(min,   -min, mid+min);
	glVertex3f( min,  -min, mid-min);
	glVertex3f(-min,   -min, mid-min);

	glVertex3f( min,  -min, mid+min);
	glVertex3f( min,  min, mid+min);
	glVertex3f(min,   min, mid-min);
	glVertex3f( min,  -min, mid-min);

	glVertex3f( -min,  -min, mid+min);
	glVertex3f( -min,  min, mid+min);
	glVertex3f(-min,   min, mid-min);
	glVertex3f(-min,   -min, mid-min);

	glVertex3f(min,   -min, mid+min);
	glVertex3f(min,   min, mid+min);
	glVertex3f( -min,  min, mid+min);
	glVertex3f(-min,   -min, mid+min);


	glEnd();
}


void Cursor::Draw3dCursorRotHitBoxes()
{
	// mPos <- center
	vec_t radius = max * 4.0f;
	const uint32 count = 64;

	vec_t x, z, i;
	Vector<vec_t> xs, zs;

	xs.reserve(count);
	zs.reserve(count);

	for (i = 0.0f; i < count; ++i)
	{
		helSinCosf(helDegToRad(360.0f * (i / count)), &x, &z);
			
		x *= radius;
		z *= radius;

		xs.pushBack(x);
		zs.pushBack(z);
	}

	// Pick the closest 'hit' from each 'axis' ring
	hel::Vec3 min, max;
	hel::Mat44 rot;
	rot.SetRotation(helDegToRad(mRotate.mX), 
					helDegToRad(mRotate.mY), 
					helDegToRad(mRotate.mZ));

	for (uint32  j = 0; j < (count-1); ++j)
	{
		for (uint32  k = 0; k < 3; ++k)
		{
			hel::Vec3 a, b;
				
			switch (k)
			{
			case 0:
				a = mPos + hel::Vec3(0.0f, xs[j  ], zs[j  ]);
				b = mPos + hel::Vec3(0.0f, xs[j+1], zs[j+1]);
				break;
					
			case 1:
				a = mPos + hel::Vec3(xs[j  ], 0.0f, zs[j  ]);
				b = mPos + hel::Vec3(xs[j+1], 0.0f, zs[j+1]);
				break;
					
			case 2:
				a = mPos + hel::Vec3(xs[j  ], zs[j  ], 0.0f);
				b = mPos + hel::Vec3(xs[j+1], zs[j+1], 0.0f);
				break;
			}
				
			hel::Vec3 side = hel::Vec3::Cross(a, b);
			side.Norm();
			side *= 0.25f;
			min = a + side; 
			max = b - side;

			min = rot * min;
			max = rot * max;
				
			switch (k)
			{
			case 0:
				mglDrawBbox(min.mVec, max.mVec, DARK_RED, DARK_RED);
				break;
					
			case 1:
				mglDrawBbox(min.mVec, max.mVec, DARK_GREEN, DARK_GREEN);
				break;
						
			case 2:
				mglDrawBbox(min.mVec, max.mVec, DARK_BLUE, DARK_BLUE);
				break;
			}
		}
	}
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


} // namespace freyja3d
