/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Track
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: Keyframe animation track
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.11.10:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_ANIMATIONTRACK_H_
#define GUARD__FREYJA_ANIMATIONTRACK_H_

#include "freyja.h"


namespace freyja {

class Keyframe;

class AnimationTrack
{
public:
	typedef mstl::avl_tree<freyja_id, freyja::Keyframe*> KeyframeList;

	AnimationTrack( freyja::Animation* animation, const char* name ) : 
		mAnimation(animation),
		mName(name), 
		mStart(0.0f), 
		mRate(30.0f), 
		mDuration(10.0f),
		mFlags(0x0)
	{ }

	virtual ~AnimationTrack() 
	{ }

	void RemoveKeyframe( freyja_id key );

	virtual KeyFrame *NewTrackKeyFrame(vec_t time) = 0;
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 *        No generics, we want specialized Track interfaces 
	 * Post : Returns keyframe type specific to Track type
	 ------------------------------------------------------*/

	virtual uint32 GetNextKeyframe(freyja_id frame);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual KeyFrame *GetPrevKey(vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual uint32 GetPrevKeyframe(freyja_id keyframe);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	freyja::Keyframe* CreateKeyframe(vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual KeyFrame *GetKeyframe(freyja_id idx) ;
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 * Post : Returns keyframe if it exists or NULL
	 ------------------------------------------------------*/

	freyja_id GetKeyfameIndexFromTime(vec_t time);
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 * Post : Returns claculated keyframe
	 ------------------------------------------------------*/

	uint16 GetKeyframeCount() 
	{ return mKeyframes.size(); } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns max number of keyframes for duration
	 ------------------------------------------------------*/

	vec_t GetDuration() 
	{ return mDuration; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets track duration
	 ------------------------------------------------------*/

	void SetDuration(vec_t lenght) 
	{ mDuration = lenght; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates animation duration and adjusts keyframe list 
	 ------------------------------------------------------*/

	vec_t GetRate() 
	{ return mRate; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates track rate
	 ------------------------------------------------------*/

	void SetRate(vec_t fps);
	/*------------------------------------------------------
	 * Pre  : <fps? must be at least 1 frame / second
	 * Post : Updates track rate and adjusts keyframe list 
	 ------------------------------------------------------*/

	mstl::list<freyja::Keyframe*> mKeyFrames;  /* Keyframe data. */

	freyja::Animation* mAnimation;   /* The animation of which this track belongs. */

	String mName;		             /* Human readable name for this track. */

	vec_t mStart;                    /* Whence track starts ( playback only ). */

	vec_t mRate;                     /* Playback rate. */

	vec_t mDuration;		         /* How long is this track. */	

	byte mFlags;                     /* Control flags. */
};


//////////////////////////////////////////////////////////////////////////
// Track 0.9.6 API
//////////////////////////////////////////////////////////////////////////

template <typename T>
class Track
{
public:

	Track();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~Track();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	Keyframe<T> *CreateKeyFrame(const T& data, vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns newly created keyframe.
	 *
	 ------------------------------------------------------*/

	void DeleteKeyFrame(index_t idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	index_t FindKeyframeIndex(vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns keyframe index if it exists or INDEX_INVALID.
	 *
	 ------------------------------------------------------*/

	Keyframe<T>* GetNextKeyframe(index_t idx);
	Keyframe<T>* GetPrevKeyframe(index_t idx);
	Keyframe<T>* GetKeyframe(index_t idx);
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 * Post : Returns keyframe if it exists or NULL.
	 *
	 ------------------------------------------------------*/

	uint32 GetKeyframeCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns keyframe count for this track.
	 *
	 ------------------------------------------------------*/

	vec_t GetDuration() { return mDuration; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets track duration
	 ------------------------------------------------------*/

	void SetDuration(vec_t lenght) { mDuration = lenght; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates animation duration and adjusts keyframe list 
	 ------------------------------------------------------*/

	vec_t GetRate() { return mRate; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get track rate.
	 *
	 ------------------------------------------------------*/

	void SetRate(vec_t fps) 
	{ if ( fps > 1.0f ) { mRate = fps; UpdateKeyframes(); } }
	/*------------------------------------------------------
	 * Pre  : <fps? must be at least 1 frame / second
	 * Post : Updates track rate and adjusts keyframe list 
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	bool Serialize(TiXmlElement *container);
	bool Unserialize(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif

	void UpdateTLB();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Force updating entire TLB.
	 *
	 ------------------------------------------------------*/

	mstl::list< Keyframe<T> *> mKeyframes;   /* Keyframe data */

	// Might want an iterator:index TLB for the list here.
	mstl::Vector<mstl::list< Keyframe<T> *>::iterator> mTLB;

	String mName;		             /* Human readable name for this track */

	vec_t mStart;                    /* Whence track starts. */

	vec_t mRate;                     /* Playback rate in frames per second. */

	vec_t mDuration;		         /* How many seconds this track lasts. */	

	byte mFlags;                     /* Control flags. */	
};


////////////////////////////////////////////////////////////
// Generic methods.
////////////////////////////////////////////////////////////

template<typename T>
Track<T>::Track()
	: mKeyframes(),
	  mName(" "), 
	  mStart(0.0f), 
	  mRate(30.0f), 
	  mDuration(10.0f),
	  mFlags(0x0)
{}


template<typename T>
Track<T>::~Track()
{
	mKeyframes.clear();
}


template<typename T>
Keyframe<T> *Track<T>::CreateKeyFrame(const T& data, vec_t time)
{
	Keyframe<T> *keyframe = new Keyframe<T>(data, time);

	if ( mKeyframes.empty() )
	{
		mKeyframes.push_back( keyframe );
		mTLB.push_back( mKeyframes.begin() );
		return keyframe;
	}

	mstl::list< Keyframe<T>* >::iterator it = mKeyframes.begin();
	for (; it < mKeyframes.end(); ++it)
	{
		if ( *it->GetTime() > time )
		{
			it = insert(it, keyframe);

			// You could rework the TLB here and save time in general case.
			break;
		}
	}

	UpdateTLB(); // for now just be sure, optimize later
	return keyframe;
}


template<typename T>
void Track<T>::DeleteKeyFrame(index_t idx)
{
	if ( idx < mTLB.size() )
	{
		mKeyframes.erase( mTLB[idx] );
		UpdateTLB(); // for now just be sure, optimize later
	}
}


template<typename T>
index_t Track<T>::FindKeyframeIndex(vec_t time)
{
	mstl::list< Keyframe<T>* >::iterator it = mKeyframes.begin();
	for (; it < mKeyframes.end(); ++it)
	{
		if ( *it->GetTime() > time )
		{
			it = insert(it, keyframe);

			// You could rework the TLB here and save time in general case.
			break;
		}
	}
}


template<typename T>
	Keyframe<T> *Track<T>::GetNextKeyframe(index_t idx)
{
}


template<typename T>
	Keyframe<T> *Track<T>::GetPrevKeyframe(index_t idx)
{
}


template<typename T>
Keyframe<T> *Track<T>::GetKeyframe(index_t idx)
{
}


template<typename T>
uint32 Track<T>::GetKeyframeCount()
{
	return mTLB.size(); // yes, you will keep TLB/list synced
}


template<typename T>
vec_t Track<T>::GetDuration() 
{ 
	return mDuration; 
}


template<typename T>
void Track<T>::SetDuration(vec_t lenght)
{
	mDuration = lenght; 
}


template<typename T>
vec_t Track<T>::GetRate() 
{
	return mRate; 
}


template<typename T>
void Track<T>::SetRate(vec_t fps) 
{
	mRate = fps; 
}


template<typename T>
void Track<T>::UpdateTLB()
{
	/* Flush TLB */
	mTLB.clear();

	/* Update TLB */
	{
		mstl::list< Keyframe<T>* >::iterator it = mKeyframes.begin();
		for (; it < mKeyframes.end(); ++it)
		{
			mTLB.push_back(it);
		}
	}
}


} // namespace freyja

#endif // GUARD__FREYJA_ANIMATIONTRACK_H_
