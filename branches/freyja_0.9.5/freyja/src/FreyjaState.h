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


class FreyjaStateTransform : public FreyjaState
{
 public:

	FreyjaStateTransform(freyja_transform_t transform,
						 freyja_transform_action_t action,
						 index_t id, vec3_t xyz) : FreyjaState() 
	{
		mState = eTransform;
		mEvent = transform;
		mMode = action;
		mIndex = id;
		mTransform = transform;
		mAction = action;
		mXYZ.mVec[0] = xyz[0];
		mXYZ.mVec[1] = xyz[1];
		mXYZ.mVec[2] = xyz[2];		
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaStateTransform
	 ------------------------------------------------------*/

	virtual ~FreyjaStateTransform() {}
	/*------------------------------------------------------
	 * Pre  : FreyjaStateTransform object is allocated
	 * Post : Deconstructs an object of FreyjaStateTransform
	 ------------------------------------------------------*/

	virtual bool Undo()
	{	
		if (mTransform == fTransformMesh)
		{
			freyjaModelMeshTransform3fv(0, mIndex, fTranslate, mXYZ.mVec);
			return true;
		}


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
		
		return true;
	}	

	virtual bool Redo()
	{
		return false;
	}

	virtual bool SerializeUndoHistory(FreyjaFileWriter &w) 
	{
		w.writeInt32U(32); // Byte size 		
		w.writeInt32(mEvent);
		w.writeInt32(mMode);
		w.writeInt32U(mIndex);
		//w.writeInt32U(mObject);
		w.writeInt32U(mTransform);
		w.writeInt32U(mAction);
		w.writeFloat32(mXYZ.mVec[0]);
		w.writeFloat32(mXYZ.mVec[1]);
		w.writeFloat32(mXYZ.mVec[2]);
		return true; 
	}

	virtual bool SerializeUndoHistory(FreyjaFileReader &r) 
	{
		unsigned int byteSize = r.readInt32U(); // Byte size 

		if ( byteSize != 32 )
		{
			freyjaPrintError("! FreyjaStateTransform::SerializeUndoHistory error");
		}

		mEvent = r.readInt32();
		mMode = r.readInt32();
		mIndex = r.readInt32U();
		//mObject = (freyja_object_t)r.readInt32U();
		mTransform = (freyja_transform_t)r.readInt32U();
		mAction = (freyja_transform_action_t)r.readInt32U();
		mXYZ.mVec[0] = r.readFloat32();
		mXYZ.mVec[1] = r.readFloat32();
		mXYZ.mVec[2] = r.readFloat32();
		return true;
	}

	virtual void operator =(const FreyjaStateTransform &s)
    { 
		mEvent = s.mEvent; 
		mIndex = s.mIndex; 
		mMode = s.mMode; 

		//mObject = s.mObject;
		mTransform = s.mTransform;
		mAction = s.mAction;
		mXYZ.mVec[0] = s.mXYZ.mVec[0];
		mXYZ.mVec[1] = s.mXYZ.mVec[1];
		mXYZ.mVec[2] = s.mXYZ.mVec[2];	
	}

	Vector3d GetXYZ() { return mXYZ; }

	void SetXYZ(Vector3d &u) { mXYZ = u; }


 private:

	freyja_transform_t mTransform;      /* Object type */

	freyja_transform_action_t mAction;  /* Type of transform */

	Vector3d mXYZ;                      /* Storage for 3d transform event */
};

#endif
