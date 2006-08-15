/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaState
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is a simple action event stack for undo/redo
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJASTATE - Builds FreyjaState class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.03.05:
 * Mongoose - Created, based on test
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJASTATE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJASTATE_H_

#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <freyja/freyja.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/MeshABI.h>
#include <freyja/Mesh.h>
#include <mstl/Action.h>


class FreyjaState
{
 public:

	typedef enum {

		eEvent = 0,
		eTransform = 1

	} state_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaState() :
		mState(eEvent),
		mEvent(-1),
		mMode(-1),
		mIndex(INDEX_INVALID)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaState
	 ------------------------------------------------------*/

	FreyjaState::FreyjaState(int event, int objIndex, int mode) :
		mEvent(event),
		mMode(mode),
		mIndex(objIndex)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaState
	 ------------------------------------------------------*/

	virtual ~FreyjaState() {}
	/*------------------------------------------------------
	 * Pre  : FreyjaState object is allocated
	 * Post : Deconstructs an object of FreyjaState
	 ------------------------------------------------------*/

	virtual bool Undo() { return false; }

	virtual bool Redo() { return false; }

	void FreyjaState::operator =(FreyjaState s) 
	{
		mEvent = s.mEvent; 
		mIndex = s.mIndex; 
		mMode = s.mMode; 
	}


	bool FreyjaState::operator ==(FreyjaState b) 
	{
		return (b.mEvent == mEvent && b.mIndex == mIndex && b.mMode == mMode); 
	}


	bool FreyjaState::operator !=(FreyjaState b) 
	{
		return (b.mEvent != mEvent || b.mIndex != mIndex || b.mMode != mMode); 
	}

	int GetEvent() { return mEvent; }

	void SetEvent(int i) { mEvent = i; }

	int GetIndex() { return mIndex; }

	int GetMode() { return mMode; }

	void SetMode(int i) { mMode = i; }

	state_t GetType() { return mState; }

	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool SerializeUndoHistory(FreyjaFileWriter &w) { return false; }

	virtual bool SerializeUndoHistory(FreyjaFileReader &r) { return false; }


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 protected:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	state_t mState;

	int mEvent;           /* Event to generate */

	int mMode;            /* Mode of the event */

	index_t mIndex;       /* Index of object on which to operate */
};


class ActionMeshTransformExt : public Action
{
 public:
	ActionMeshTransformExt(index_t mesh, freyja_transform_action_t a, vec3_t xyz, Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mMesh(mesh),
		mAction(a),
		mXYZ(xyz)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		freyjaModelMeshTransform3fv(0, mMesh, mAction, mXYZ.mVec);
		mCursorXYZ = mXYZ;
		return true;
	}

	Vec3 &mCursorXYZ;
	index_t mMesh;                      /* Which mesh? */
	freyja_transform_action_t mAction;  /* Type of transform */
	Vector3d mXYZ;                      /* Storage for 3d transform event */
};


class ActionMeshTransform : public Action
{
 public:
	ActionMeshTransform(index_t mesh, freyja_transform_action_t a, vec3_t xyz) :
		Action(),
		mMesh(mesh),
		mAction(a),
		mXYZ(xyz)
	{}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		freyjaModelMeshTransform3fv(0, mMesh, mAction, mXYZ.mVec);
		return true;
	}

	index_t mMesh;                      /* Which mesh? */
	freyja_transform_action_t mAction;  /* Type of transform */
	Vector3d mXYZ;                      /* Storage for 3d transform event */
};


class ActionTexCoordTransform : public Action
{
 public:
	ActionTexCoordTransform(index_t mesh, index_t texcoord, vec_t u, vec_t v) :
		Action(),
		mMesh(mesh),
		mTexCoordArrayIndex(texcoord),
		mU(u),
		mV(v)
	{}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		//DEBUG_MSG("$$$$$$$$$$$$ UNDO - %u %u %f %f\n", mMesh, mTexCoordArrayIndex, mU, mV);
		freyja::Mesh *m = freyjaModelGetMeshClass(0, mMesh);

		if (m)
		{
			vec3_t uvw = { mU, mV, 0.0f };
			m->SetTexCoord(mTexCoordArrayIndex, uvw);
			return true;
		}

		return false;
	}

	index_t mMesh, mTexCoordArrayIndex;
	vec_t mU, mV;
};


class ActionGenericTransformExt : public Action
{
 public:
	ActionGenericTransformExt(index_t idx, freyja_transform_t t, freyja_transform_action_t a, vec3_t xyz, Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mIndex(idx),
		mTransform(t),
		mAction(a),
		mXYZ(xyz)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		Vec3 xyz;

		freyjaGetGenericTransform3fv(mTransform, mAction, mIndex, xyz.mVec);
		
		switch (mAction)
		{
		case fTranslate:
			xyz = mXYZ - xyz; // translate from origin not current translation
			break;

		default:
			xyz = mXYZ;
		}

		freyjaPrintMessage("! FreyjaStateTransform::Undo() %i %i %i %f %f %f",
						   mTransform, mAction, mIndex,
						   xyz.mVec[0], xyz.mVec[1], xyz.mVec[2]);

		freyjaGenericTransform3fv(mTransform, mAction, mIndex, xyz.mVec);

		mCursorXYZ = mXYZ;
		return true;
	}

	Vec3 &mCursorXYZ;
	index_t mIndex;
	freyja_transform_t mTransform;      /* Object type */
	freyja_transform_action_t mAction;  /* Type of transform */
	Vector3d mXYZ;                      /* Storage for 3d transform event */
};


class ActionVertexTransformExt : public Action
{
 public:
	ActionVertexTransformExt(index_t mesh, index_t vertex, freyja_transform_action_t a, vec3_t xyz, Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mMesh(mesh),
		mVertex(vertex),
		mAction(a),
		mXYZ(xyz)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		if (mAction == fTranslate)
			freyjaMeshVertexTranslate3fv(mMesh, mVertex, mXYZ.mVec);
		mCursorXYZ = mXYZ;
		return true;
	}

	Vec3 &mCursorXYZ;
	index_t mMesh;
	index_t mVertex;
	freyja_transform_action_t mAction;  /* Type of transform */
	Vector3d mXYZ;                      /* Storage for 3d transform event */
};

#endif
