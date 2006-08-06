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


#ifndef GUARD__FREYJA_MONGOOSE_FREYJA3DCURSOR_H_
#define GUARD__FREYJA_MONGOOSE_FREYJA3DCURSOR_H_


#include <hel/math.h>
#include <hel/Ray.h>
#include <mstl/stack.h>

#include "freyja_events.h"
#include "FreyjaState.h"
#include "FreyjaOpenGL.h"


class Freyja3dCursor
{
 public:

	typedef enum {
		Rotation,
		Translation,
		Scale,
		Invisible
	} Flags;


	typedef enum {

		eNone = 0,
		eX,
		eY,
		eZ,
		eAll

	} axis_t;


	const static vec_t min = 0.5f, mid = 2.4f, max = 1.744f;


	Freyja3dCursor()
	{
		Reset();
	}

	~Freyja3dCursor() { }

	bool CheckForRayCollision(Ray &r)
	{
		bool ret = false;

		switch (GetMode())
		{
		case Freyja3dCursor::Scale:
			break;


		case Freyja3dCursor::Rotation:
			break;

		case Freyja3dCursor::Translation:
			break;

		default:
			ret = false;
		}

		return ret;
	}

	void Display()
	{
		glPushMatrix();
		glTranslatef(mPos.mVec[0], mPos.mVec[1], mPos.mVec[2]);

		switch (GetMode())
		{
		case Freyja3dCursor::Scale:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);
			Draw3dCursorScale(min, mid, max);
			glPopAttrib();
			break;


		case Freyja3dCursor::Rotation:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);
			Draw3dCursorRot(min, mid, max);
			glPopAttrib();
			break;

		case Freyja3dCursor::Translation:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);
			Draw3dCursorLoc(min, mid, max);
			glPopAttrib();
			break;

		default:
		case Freyja3dCursor::Invisible:
			break;
		}

		glPopMatrix();
	}


	void SetMode(Flags n)
	{
		mMode = n;
	}

	// When seperating events from cursor remember to restore mode here
	void ForceChangeState(FreyjaState *state, Flags mode)
	{
		if (!state)
			return;

		freyja_print("! force pushed undo frame");
		mStack.push(state);
		mLastState = *state;
		mMode = mode;
	}

	void ChangeState(FreyjaState *state, Flags mode)
	{
		if (!state)
			return;

		if ( *state != mLastState )
		{
			freyja_print("! pushed undo frame");
			mStack.push(state);
			mLastState = *state;
		}
		
		mMode = mode;
	}

	Flags GetMode() { return mMode; }

	void Push()
	{
		// FIXME: Not implemented yet
	}

	FreyjaState *Pop()
	{
		FreyjaState *state = NULL;
		mLastState.SetEvent(-1);

		if (!mStack.empty())
		{
			state = mStack.pop();
			state->Undo();
			return state;
		}

		return state;
	}

	void Reset()
	{
		mSelected = false;
		mAxis = eNone;
		mMode = Invisible;
		mPos = Vector3d(0.0f, 0.0f, 0.0f);
		mScale = Vector3d(1.0f, 1.0f, 1.0f);
		mRot = Quaternion();
	}

	bool mSelected;

	axis_t mAxis;

	Vector3d mPos;

	Vector3d mScale;

	Quaternion mRot;


 private:

	void Draw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max)
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

	void Draw3dCursorRot(const vec_t min, const vec_t mid, const vec_t max)
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


	void Draw3dCursorScale(const vec_t min, const vec_t mid, const vec_t max)
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


	FreyjaState mLastState;

	mstl::stack<FreyjaState *> mStack;

	Flags mMode;
};


#endif // GUARD__FREYJA_MONGOOSE_FREYJA3DCURSOR_H_
