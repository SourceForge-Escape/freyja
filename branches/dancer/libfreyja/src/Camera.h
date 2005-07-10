/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaCamera
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

/* FIXME: Remove vestiges of OpenGL vectors, and 
 *        replace with pure quaternion use internally */

#ifndef GUARD__FREYJA_MONGOOSE_CAMERA_H_
#define GUARD__FREYJA_MONGOOSE_CAMERA_H_

#include <hel/math.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>

#define FreyjaCamera Camera

namespace freyja {

class Camera
{
 public:

	typedef enum {
		eFlyByMode = 1,
		eStationary,
		eTrack

	} mode_t;


	class FlyByNode
	{
	public:
		Quaternion orientation;
		vec3_t position;
		vec3_t direction;
		vec_t speed;
		vec_t time;
	};

	class FlyByPath
	{ 
	public:
		Vector <FlyByNode *> path;  /* Flyby path */
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaCamera();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaCamera
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaCamera();
	/*------------------------------------------------------
	 * Pre  : Camera object is allocated
	 * Post : Deconstructs an object of FreyjaCamera
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getId();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns this camera's id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getPosition(vec3_t pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns current position
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getUp(vec3_t up);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns up vector
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getTarget(vec3_t target);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns target ( look at pos )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	vec_t getYaw();
	/*------------------------------------------------------
	 * Pre  : Get current yaw in degrees
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	double getRadianYaw();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns theta angle/yaw of camera
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	vec_t getPitch();
	/*------------------------------------------------------
	 * Pre  : Get current pitch in degrees
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	double getRadianPitch();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns phi angle/pitch of camera
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	bool isBehind(int x, int z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if (x, z) is behind camera eye
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void rotate(vec_t angle, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : x,y,z axis; angle in radians
	 * Post : Rotates camera
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.06.04: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void translate(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera position is set to x,y,z
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Camera is set to inital state
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

   void setSpeed(vec_t speed);
   /*------------------------------------------------------
    * Pre  : 
    * Post : Sets 'speed'
    *
    *-- History ------------------------------------------
    *
    * 2002.01.02: 
    * Mongoose - Created
    ------------------------------------------------------*/

	void update();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates view target
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setPosition(vec3_t pos);
	/*------------------------------------------------------
	 * Pre  : Set current position
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setUp(vec3_t up);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets up vector
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setTarget(vec3_t target);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets target ( look at pos )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	unsigned int mId;           /* Unquie id */

	mode_t mMode;               /* Camera mode */

	FlyByPath mPath;            /* Camera flyby path */

	Quaternion mQ;              /* Quaternion for rotation */

	vec_t mSpeed;               /* Movement speed for this camera */

	double mPos[4];             /* Location in 3 space (aka eye) */
	
	double mTarget[4];          /* Postition we're looking at  */
	
	double mUp[4];              /* Up vector  */
	
	double mSide[4];            /* Side vector  */
	
	double mViewDistance;       /* Distance from target */
	
	double mTheta;              /* View angle  Y */
	
	double mTheta2;             /* View angle  Z */
	
	bool mUpdate;               /* Check to see if view needs updating */

	static unsigned int mCounter;   /* Id system use */
};

}
#endif
