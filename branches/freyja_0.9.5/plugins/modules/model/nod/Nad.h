/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Lwo
 * License : GPL
 * Comments: NAD module for freyja
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.01.21:
 * Mongoose - Created, Nad format supplied by documentation. Wow, docs.
 * Some Nad comments are right from the doc.  No special types in header by
 * request.
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_NAD_H
#define GUARD__FREYJA_MONGOOSE_NAD_H


	// Lwalk, 0, "Left foot hits the ground in walk animation, triggers footstep sounds"
	// Rwalk, 1, "Right foot hits the ground, used to trigger footstep sounds"
	// Lrun, 2, "Left footfall in run, sound trigger"
	// Rrun, 3, "Right footfall run, sound trigger"
	// Fire, 4, "When to spawn projectile in a weapon firing animation"
	// Strike, 5, "When to compute hit effects in a melee weapon strike animation"
	// Cast, 6, "When to trigger the discipline cast during a casting animation"
	// Fall, 7, "Soundtrigger for sound of body hitting the ground during death animation"
	// Project, 8, "When to enable display of the projectile mesh in player’s hand during firing animation"
	// Flap, 9, "Sound trigger for flying actors"
	// Suck, 10, "When during a feeding animation to trigger suck sound effects"
	// Idle, 11, "Special sounds for idle animations (horse whinney, etc.)"
	// Idle2, 12, "Special sounds for idle animations"
	// Codex1-Codex3, 13-15, "Trigger a special script callback at this point in animation"
	// Repeat, 16, "At this keyframe, check for held-down attack key and jump to REPEATTO tag if present, for ‘strafing’ weapons"
	// RepeatTo, 17, "Jump to this keyframe during strafe attack animations"
	// Throwdeath, 18, "During a throw animation, check for victim death at this frame and halt their animation if dead"

class Nad
{
 public:

	class Keyframe
	{
	 public:

		float mFrame; // time ( 0.0 to NumFrames )
		float mFrameScale; // Scaling factor used to interpolate to next keyframe, computed as (1/nextFrame-thisFrame)
		float mValue[3]; // Euler angles or position in 3 space
		float mCFactor[3]; // The constant curve fitting factor for the Hermite curve interpolation
		float mBFactor[3]; // The linear curve fitting factor
		float mAFactor[3]; // The square curve fitting factor
	};

	class BoneTrack
	{
	 public:

		BoneTrack() :
			mNumKeys(0),
			mBoneNum(0),
			mTrackType(0),
			mKeys(NULL)
		{
		}

		~BoneTrack()
		{
			if (mKeys)
				delete [] mKeys;
		}

		unsigned int mNumKeys;
		unsigned int mBoneNum;
		unsigned int mTrackType; // 0 = rotation, 1 = translate, 2 = scale
		Keyframe *mKeys; // mNumKeys
	};

	class KeyframeTag
	{
	 public:
		float mFrameNum;
		unsigned int mTagType; // Index into desc table
	};


	Nad() :
		mVersion(3),
		mNumBoneTracks(0),
		mFlags(0),
		mDuration(0.0f),
		mBoneTracks(NULL),
		mNumTags(0),
		mTags(NULL)
	{
	}

	~Nad()
	{
		if (mBoneTracks)
			delete [] mBoneTracks;

		if (mTags)
			delete [] mTags;
	}
	
	bool Load(const char *filename);
	
	bool Save(const char *filename);

	void Print();


	unsigned int mVersion; // nod v1.0 -> 3

	unsigned int mNumBoneTracks;

	unsigned int mFlags;

	float mDuration;

	BoneTrack *mBoneTracks; // mNumBoneTracks

	unsigned int mNumTags;

	KeyframeTag *mTags; // mNumTags
};

#endif // GUARD__FREYJA_MONGOOSE_NAD_H
