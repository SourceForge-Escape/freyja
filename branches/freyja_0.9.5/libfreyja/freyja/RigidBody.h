/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : RigidBody
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Rigid body subsystem
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_RIGIDBODY - Builds RigidBody class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created, based on Varlet integration from the gamasutra article:
 *            'Advanced Character Physics - Thomas Jakobsen', however clearly
 *            the idea here is editing as well as rendering.
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_RIGIDBODY_H_
#define GUARD__FREYJA_MONGOOSE_RIGIDBODY_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>


namespace freyja {


class Constraint
{
public:

	Constraint();

	~Constraint();

	
	uint32 mParticleA;

	uint32 mParticleB;

	vec_t mRestLength; 

	// vec_t mMinAngle;

	// vec_t coeffientA, coeffientB;  /* Must sum to 1.0 */


private:
	
	uint32 mUID;                 /* Gobal UID for this class */

	static uint32 mUIDCount;
};



class ParticleSystem
{
public:

	ParticleSystem();

	ParticleSystem(uint32 numberOfParticles);

	ParticleSystem(const ParticleSystem &sys);

	ParticleSystem &operator=(const ParticleSystem &sys);

	~ParticleSystem();

	void Init(uint32 numberOfParticles);

	void TimeStep();


	uint32 mParticleCount;

	hel::Vec3 *mX;

	hel::Vec3 *mOldX;

	hel::Vec3 *mA;

	bool mIsMassUsed;

	vec_t *mMass;

	hel::Vec3 mGravity;

	vec_t mTimeStep;


private:

	void Verlet();

	void SatisfyConstraints();

	void AccumulateForces();

	
	uint32 mUID;

	static uint32 mUIDCount;
};



class RigidBody
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	RigidBody(uint32 particleCount);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of RigidBody
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.06.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~RigidBody();
	/*------------------------------------------------------
	 * Pre  : RigidBody object is allocated
	 * Post : Deconstructs an object of RigidBody
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.06.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	ParticleSystem mSystem;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};


}

#endif
