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
 *-- History ------------------------------------------------- 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#include "RigidBody.h"

using namespace freyja;

uint32 ParticleSystem::mUIDCount = 0;


ParticleSystem::ParticleSystem() :
	mParticleCount(0),
	mX(0x0),
	mOldX(0x0),
	mA(0x0),
	mIsMassUsed(false),
	mMass(0x0),
	mGravity(0.0f, /*-9.8f*/0.0f, 0.0f),  // Y-up can't be expected
	mTimeStep(1.0f),
	mUID(mUIDCount++)
{
}


ParticleSystem::ParticleSystem(const ParticleSystem &sys) :
	mParticleCount(sys.mParticleCount),
	mX(sys.mX),
	mOldX(sys.mOldX),
	mA(sys.mA),
	mIsMassUsed(sys.mIsMassUsed),
	mMass(sys.mMass),
	mGravity(sys.mGravity), 
	mTimeStep(sys.mTimeStep),
	mUID(mUIDCount++)
{
}


ParticleSystem &ParticleSystem::operator=(const ParticleSystem &sys) 
{
	mParticleCount = sys.mParticleCount;
	mX = sys.mX;
	mOldX = sys.mOldX;
	mA = sys.mA;
	mIsMassUsed = sys.mIsMassUsed;
	mMass = sys.mMass;
	mGravity = sys.mGravity;
	mTimeStep = sys.mTimeStep;
	

	return *this;
}


ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::Init(uint32 numberOfParticles)
{
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

RigidBody::RigidBody(uint32 particleCount) :
	mSystem()
{
	mSystem.Init(particleCount);
}


RigidBody::~RigidBody()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_RIGIDBODY
int runRigidBodyUnitTest(int argc, char *argv[])
{
	RigidBody test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[RigidBody class test]\n");

	return runRigidBodyUnitTest(argc, argv);
}
#endif
