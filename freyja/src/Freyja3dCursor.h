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

const vec3_t gCursorDrawSz = { 0.5f, 2.4f, 1.744f };


class Freyja3dCursor
{
 public:

	typedef enum Freyja3dCursorFlags {
		Rotation,
		Translation,
		Scale,
		Invisible
	} Freyja3dCursorFlags_t;

	Freyja3dCursor()
	{
		Reset();
	}

	~Freyja3dCursor() { }

	void SetMode(Freyja3dCursorFlags_t n)
	{
		mMode = n;
	}

	void ForceChangeState(FreyjaState *state, Freyja3dCursorFlags_t mode)
	{
		if (!state)
			return;

		freyja_print("! force pushed undo frame");
		mStack.push(state);
		mLastState = *state;
		mMode = mode;
	}

	void ChangeState(FreyjaState *state, Freyja3dCursorFlags_t mode)
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

	Freyja3dCursorFlags_t GetMode() { return mMode; }

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
		mAxis = 0; // implies X, Y screen/free virtual plane
		mMode = Invisible;
		mPos = Vector3d(0.0f, 0.0f, 0.0f);
		mScale = Vector3d(1.0f, 1.0f, 1.0f);
		mRot = Quaternion();
	}

	uint16 mAxis;

	Vector3d mPos;

	Vector3d mScale;

	Quaternion mRot;

 private:

	FreyjaState mLastState;

	mstl::stack<FreyjaState *> mStack;

	mstl::stack<FreyjaState *> mRedoStack;

	Freyja3dCursorFlags_t mMode;
};


#endif // GUARD__FREYJA_MONGOOSE_FREYJA3DCURSOR_H_
