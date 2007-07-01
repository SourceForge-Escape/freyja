/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Camera
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: OpenGL camera class for Freyja
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Taken from old Hel and moved to Freyja
 *            API clean up
 *
 * 2002.12.16:
 * Mongoose - Removed perspective setting and OpenGL dependency
 *            API changes to reflect new direction of this object:
 *              Removing outdated algorithms and code 
 *              And refactoring the class in general
 *
 * 2001.06.06:
 * Mongoose - Moving GLU code into here to setup break up
 *            into Camera base class, DynamicCamera,
 *            and GLUCamera child classes
 *
 * 2001.06.04:
 * Mongoose - Quaternion based compile option
 *
 * 2001.05.18:
 * Mongoose - Created, based on my old GL camera code
 *            that has been used in GooseEgg since alpha
 *            and algorithms from Yuri Zhivago's trview
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_CAMERA_H_
#define GUARD__FREYJA_MONGOOSE_CAMERA_H_

#include <hel/math.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <hel/Vec3.h>
#include <hel/Ray.h>

#include <mstl/Vector.h>

namespace freyja
{

class Camera
{
	////////////////////////////////////////////////////////////
	// Public 
	////////////////////////////////////////////////////////////

 public:

	Camera();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~Camera();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	const char* GetName();
	void SetName(const char* name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera name position property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetDir();
	void SetDir(hel::Vec3& dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera dir position property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetPos();
	void SetPos(hel::Vec3& pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera position property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetUp();
	void SetUp(hel::Vec3& up);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera up vector property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetTarget();
	void SetTarget(hel::Vec3& target);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera target position property.
	 *
	 ------------------------------------------------------*/
	
	bool IsVisible(const hel::Vec3 &min, const hel::Vec3 &max);
	/*------------------------------------------------------
	 * Pre  : Given abstract bounding box.
	 * Post : Returns true if bounding volume is in the view volume.
	 *
	 ------------------------------------------------------*/

	bool IsVisible(const hel::Vec3 &pos, const vec_t radius);
	/*------------------------------------------------------
	 * Pre  : Given abstract bounding sphere.
	 * Post : Returns true if bounding volume is in the view volume.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Protected  
	////////////////////////////////////////////////////////////

 protected:

	void Update();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates view target
	 *
	 ------------------------------------------------------*/

	//unsigned int mId;           /* Unquie id */

	//mode_t mMode;               /* Camera mode */

	mstl::String mName;         /* Name of this camera */

	hel::Ray mView;             /* 'Eye' pos+dir */
	
	hel::Vec3 mTarget;          /* Postition we're looking at  */
	
	hel::Vec3 mUp;              /* Up vector  */
	
	hel::Vec3 mSide;            /* Side vector  */

	hel::Quat mRot;             /* Quaternion for rotation */
	
	//vec_t mViewDistance;        /* Distance from target */
	
	//bool mUpdate;               /* Check to see if view needs updating */

	//mstl::Vector<> mTracks;     /* Camera view animation tracks. */
	
	//mstl::Vector<> mUpTracks;   /* Camera up vector animation tracks. */
};

	////////////////////////////////////////////////////////////
	// Inline Methods. 
	////////////////////////////////////////////////////////////
	inline
	Camera::Camera()
		: mName("camera"),
		  mView(),
		  mTarget(),
		  mUp(0.0f, 1.0f, 0.0f),
		  mSide(0.0f, 0.0f, 1.0f),
		  mRot()
	{ }


	inline
	Camera::~Camera()
	{ }


	inline
	const char* Camera::GetName()
	{
		return this->mName.c_str();
	}


	inline
	void Camera::SetName(const char* name)
	{
		this->mName = name;
	}


	inline
	const hel::Vec3& Camera::GetDir()
	{
		return this->mView.mDir;
	}


	inline
	void Camera::SetDir(hel::Vec3& dir)
	{
		this->mView.mDir = dir;
	}


	inline
	const hel::Vec3& Camera::GetPos()
	{
		return this->mView.mOrigin;
	}


	inline
	void Camera::SetPos(hel::Vec3& pos)
	{
		this->mView.mOrigin = pos;
	}


	////////////////////////////////////////////////////////////
	// Camera path ( 0.9.3 API )
	////////////////////////////////////////////////////////////

	class CameraFlyByNode
	{
	public:
		CameraFlyByNode() 
			: orientation(), 
			  position(), 
			  direction(), 
			  speed(0.0f), 
			  time(0.0f) 
		{ }

		hel::Quat orientation;
		hel::Vec3 position;
		hel::Vec3 direction;
		vec_t speed;
		vec_t time;
	};


	class CameraFlyByPath
	{ 
	public:
		CameraFlyByPath() 
			: path() 
		{ }

		~CameraFlyByPath() 
		{ }

		mstl::Vector <CameraFlyByNode *> path;  /* Flyby path */
	};


} // namespace freyja


#endif
