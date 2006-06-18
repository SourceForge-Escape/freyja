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

ParticleSystem::ParticleSystem()
{
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

RigidBody::RigidBody(uint32 particleCount)
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
