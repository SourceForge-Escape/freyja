/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaSkeleton
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaSkeleton class and the classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJASKELETON - Builds FreyjaSkeleton class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJASKELETON_H_
#define GUARD__FREYJA_MONGOOSE_FREYJASKELETON_H_

#include <string.h>

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>


class FreyjaKeyFrame
{
public:

	FreyjaKeyFrame()
	{
		vec3_t xyz;

		xyz[0] = HEL_DEG_TO_RAD(30.0f);
		xyz[1] = HEL_DEG_TO_RAD(15.0f);
		xyz[2] = HEL_DEG_TO_RAD(7.5f);

		mOrientation.setByEulerAngles(xyz);
		printf("C 0. %f %f %f\n", xyz[0], xyz[1], xyz[2]);
		mOrientation.getEulerAngles(xyz);
		printf("C 1. %f %f %f\n", xyz[0], xyz[1], xyz[2]);
		printf("\n");
	}

	FreyjaKeyFrame(vec_t time, Vector3d position, Quaternion orientation)
	{
		mOrientation = orientation;
		mPosition = position;
		setTime(time);
	}

	FreyjaKeyFrame(vec_t time, const vec3_t xyz, const vec4_t wxyz)
	{
		setOrientationByQuaternion(wxyz);
		setPosition(xyz);
		setTime(time);
	}


	void getOrientationEuler(vec3_t xyz)
	{
		mOrientation.getEulerAngles(xyz+0, xyz+1, xyz+2);
	}

	Quaternion getOrientation()
	{
		Quaternion q = mOrientation;
		return q;
	}

	void getPosition(vec3_t xyz)
	{
		mPosition.getXYZ(xyz);
	}

	Vector3d getPosition()
	{
		Vector3d v = mPosition;
		return v;
	}

	vec_t getTime()
	{
		return mTime;
	}


	void setOrientationByEuler(const vec3_t xyz) // must be radians
	{
		mOrientation.setByEulerAngles(xyz);
	}

	void setOrientationByAxisAngles(const vec4_t axyz)
	{
		mOrientation.setByAxisAngles(axyz[0], axyz[1], axyz[2], axyz[3]);
	}

	void setOrientationByQuaternion(const vec4_t wxyz)
	{
		mOrientation = Quaternion(wxyz);
	}

	void setPosition(const vec3_t xyz)
	{
		mPosition = Vector3d(xyz);
	}

	void setTime(vec_t time)
	{
		mTime = time;
	}

	vec_t mTime;                /* Time to next keyframe */

	Vector3d mPosition;

	Quaternion mOrientation;
};


// This class handles unpadded keyframes, adds string checking, etc
class FreyjaSkeletalAnimation
{
public:

	class FreyjaBoneKeyFrame
	{
	public:
		FreyjaBoneKeyFrame(const char *name, long boneIndex)
		{
			setName(name);
			mBoneIndex = boneIndex;
		}

		long addKeyFrame(vec_t time, Vector3d position, Quaternion orientation)
		{
			long frame = mKeyFrames.size();

			mKeyFrames.pushBack(new FreyjaKeyFrame(time,position, orientation));

			return frame;
		}

		void setName(const char *name)
		{
			strncpy(mName, name, 64);
			mName[63] = 0;
		}

		void updateBoneIndex(long boneIndex)
		{
			mBoneIndex = boneIndex;
		}


		char *getName()
		{
			return mName;
		}

		long getBoneIndex()
		{
			return mBoneIndex;
		}

		FreyjaKeyFrame *getKeyFrame(long keyframeIndex)
		{
			if (keyframeIndex > -1 && keyframeIndex < (long)mKeyFrames.size())
			{
				return mKeyFrames[keyframeIndex];
			}

			return 0x0;
		}
		

		char mName[64];				/* Used to double check 
										skeleton <--> keyframe matches */
		long mBoneIndex;

		Vector<FreyjaKeyFrame *> mKeyFrames; 
	};


	long getBoneCount()
	{
		return mBoneKeyFrames.size();
	}

	FreyjaBoneKeyFrame *getBone(long boneIndex)
	{
		if (boneIndex > -1 && boneIndex < (long)mBoneKeyFrames.size())
		{
			return mBoneKeyFrames[boneIndex];
		}

		return 0x0;
	}

	FreyjaKeyFrame *getBoneKeyFrame(long boneIndex, long keyframeIndex)
	{
		FreyjaBoneKeyFrame *bone = getBone(boneIndex);

		if (bone)
		{
			return bone->getKeyFrame(keyframeIndex);
		}

		return 0x0;
	}	

	long getKeyFrameCountForBone(long boneIndex)
	{
		FreyjaBoneKeyFrame *frame = mBoneKeyFrames[boneIndex];
		
		if (frame)
		{
			return frame->mKeyFrames.size();
		}

		return 0;
		
	}

	void setBoneName(long boneIndex, const char *name)
	{
		FreyjaBoneKeyFrame *frame = mBoneKeyFrames[boneIndex];
		
		if (frame)
		{
			return frame->setName(name);
		}
	}

	void setName(const char *name)
	{
		strncpy(mName, name, 64);
		mName[63] = 0;
	}
	
	long newKeyFrame(long boneKeyFrameIndex, vec_t time, 
					 Vector3d xyz, Quaternion wxyz)
	{
		FreyjaBoneKeyFrame *frame = mBoneKeyFrames[boneKeyFrameIndex];
		
		if (frame)
		{
			return frame->addKeyFrame(time, xyz, wxyz);
		}

		return -1;
	}

	long newKeyFrame(long boneKeyFrameIndex, vec_t time,vec3_t xyz, vec4_t wxyz)
	{
		FreyjaBoneKeyFrame *frame = mBoneKeyFrames[boneKeyFrameIndex];
		
		if (frame)
		{
			return frame->addKeyFrame(time, Vector3d(xyz), Quaternion(wxyz));
		}

		return -1;
	}

	long newBoneKeyFrame(const char *name, long boneIndex)
	{
		long frameIndex = mBoneKeyFrames.size();

		mBoneKeyFrames.pushBack(new FreyjaBoneKeyFrame(name, boneIndex));

		return frameIndex;
	}

	long getBoneKeyFrameCount(long boneIndex)
	{
		if (boneIndex > 0 && boneIndex < (int)mBoneKeyFrames.size())
			return mBoneKeyFrames[boneIndex]->mKeyFrames.size();
	}

	long mId;

	char mName[64];

	vec_t mFrameRate;

	vec_t mTime;

	long mStartBone;          /* For animation blending (subsets) use */

	long mBoneCount;

	long mCurrentFrame; // render use mostly

	long mLastFrame;    // render use mostly

	vec_t mLastTime;    // render use mostly

	Vector<FreyjaBoneKeyFrame *> mBoneKeyFrames; 
};


class FreyjaBone 
{
public:

	long mId;                        /* Unique identifier */

	unsigned long flags;             /* Options */

	char mName[64];                  /* Human readable identifier */

	long mParent;                    /* Parent of this bone */

	Vector <long> mChildren;         /* Children bones */

	Quaternion mRotate;              /* Cached for editing use */

	Matrix mWorld;                   /* Transform use */

	Vector3d mTranslate;
};



class FreyjaSkeleton
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaSkeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaSkeleton in Freyja
	 ------------------------------------------------------*/

	~FreyjaSkeleton();
	/*------------------------------------------------------
	 * Pre  : FreyjaSkeleton object is allocated
	 * Post : Deconstructs an object of FreyjaSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaSkeleton in Freyja
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getBoneCount() { return mBones.end(); }

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void connectBones(unsigned int parent, unsigned int child);

	void disconnectBones(unsigned int parent, unsigned int child);


	//private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	long mId;                         /* Unique identifier */

	char mName[64];                   /* Human readable identifier */

	Vector<FreyjaBone *> mBones;      /* Bones in this skeleton */

	long mRoot;                       /* Root bone index */


	// Remove this later

	Vector3d mPosition;               /* Position of skeleton */

	static unsigned int mCounter;     /* Used to generate ids */
};

#endif
