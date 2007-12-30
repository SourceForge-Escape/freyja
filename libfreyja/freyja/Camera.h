/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
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

#ifndef GUARD__FREYJA_CAMERA_H_
#define GUARD__FREYJA_CAMERA_H_

#include <hel/math.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <hel/Vec3.h>
#include <hel/Ray.h>
#include "Node.h"

namespace freyja
{

class Camera :
		public Node
{
	////////////////////////////////////////////////////////////
	// Public 
	////////////////////////////////////////////////////////////

 public:

	Camera( const char* name );
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

	const char* GetMetadata();
	void SetMetadata(const char* name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera metadata position property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetDir();
	void SetDir(const hel::Vec3& dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera dir position property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetPos();
	void SetPos(const hel::Vec3& pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera position property.
	 *
	 ------------------------------------------------------*/

	const hel::Quat& GetRot();
	void SetRot(const hel::Quat& q);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera orientation property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetUp();
	void SetUp(const hel::Vec3& up);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera up vector property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetTarget();
	void SetTarget(const hel::Vec3& target);
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
	// Public interfaces.
	////////////////////////////////////////////////////////////

	FREYJA_NODE_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Node implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_RENDERABLE_INTERFACE
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renderable implementation.
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

	mstl::String mMetadata;     /* XML metadata. */

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
Camera::Camera( const char* name ) :
	Node( name ),
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
const hel::Vec3& Camera::GetDir()
{ return mView.GetDir(); }


inline
void Camera::SetDir(const hel::Vec3& dir)
{ mView.SetDir( dir ); }


inline
const hel::Vec3& Camera::GetPos()
{ return mView.GetOrigin(); }


inline
void Camera::SetPos(const hel::Vec3& pos)
{
	// Do we want to maintain same up vector or force other side to update?
	//mUp += pos - mView.mOrigin;
	//mUp.Norm();

	mView.SetOrigin( pos );
}

//////


inline
const hel::Quat& Camera::GetRot()
{ return mRot; }


inline
void Camera::SetRot(const hel::Quat& q)
{ mRot = q;	}


inline
const hel::Vec3& Camera::GetTarget()
{ return mTarget; }


inline
void Camera::SetTarget(const hel::Vec3& pos)
{ mTarget = pos; }


inline
const hel::Vec3& Camera::GetUp()
{ return mUp; }


inline
void Camera::SetUp(const hel::Vec3& v)
{ mUp = v; }


inline
const char* Camera::GetType() const
{ return "Camera"; }

 
inline
uint32 Camera::GetVersion() const
{ return 0; }


inline
freyja::Node* Camera::Duplicate() const
{ return new Camera(*this); }


inline
freyja::Material* Camera::GetMaterial() const
{ return NULL; }


inline
bool Camera::Serialize( XMLSerializerNode parent ) const
{
	return false;
}

inline
bool Camera::Unserialize( XMLSerializerNode node )
{
	return false;
}

inline
mstl::String Camera::GetInfo() const
{
	return mstl::String( "Camera" );
}

inline
const hel::Quat& Camera::GetWorldOrientation() const
{
	return mOrientation;
}

inline
const hel::Vec3& Camera::GetWorldPosition() const
{
	return mPosition;
}

inline
void Camera::DuplicateChildren( freyja::Node* parent, bool recurse )
{
}


} // namespace freyja


#endif // GUARD__FREYJA_CAMERA_H_
