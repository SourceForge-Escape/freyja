/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Spring
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: These are the spring and spring system classes
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SPRING - Builds Spring class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.06.07:
 * Mongoose - Support for Hel CollisionObject
 *
 * 2003.06.06:
 * Mongoose - Created, based on:
 *            SheetSystem algorithms
 *            by Terry 'Mongoose' Hendrix II
 *
 *            Thread simulation algorithms, 
 *            ( Erkin's rope algorithm didn't handle sheer and bend )
 *            Rope simulator algorithm NeHe #40 
 *            by Erkin Tunca
 *
 *            Cloth simulation algorithms, sheer/bend spring basis:
 *            "Devil in blue faceted dress: Real-time cloth animation"
 *            by Jeff Lander
 ================================================================*/


#ifndef GUARD__HEL_MONGOOSE_SPRING_H_
#define GUARD__HEL_MONGOOSE_SPRING_H_

#include <hel/Simulation.h>

class Spring
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Spring(Mass *a, Mass *b, vec_t length, vec_t stiffness, vec_t friction);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Spring
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Spring();
	/*------------------------------------------------------
	 * Pre  : Spring object is allocated
	 * Post : Deconstructs an object of Spring
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual void solve();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Applies forces
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	/* Spring:  (A)------(B) */

	Mass *mMassA;					/* Mass on end of spring, A */

	Mass *mMassB;					/* Mass on end of spring, B */

	vec_t mSpringLength;			/* Length of spring at rest */

	vec_t mSpringConstant;		/* Spring stiffness */

	vec_t mFrictionConstant;	/* Inner spring friction */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};


class ThreadSpringSimulation : public Simulation
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ThreadSpringSimulation() : Simulation()
	{
		mGravitation = Vector3d(0.0f, -9.81f, 0.0f);
		mAirFrictionConstant = 0.02f;
	}


	~ThreadSpringSimulation()
	{
		clear();
	}


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual void generateMasses(unsigned int count, vec_t mass,
										 const Vector3d &position, 
										 const Vector3d &velocity, 
										 vec_t length, vec_t stiffness, vec_t friction)
	{
		unsigned int i, j;
		Mass *a, *b;


		mConnection.mPos = position;
		mConnection.mVelocity = velocity;
		mConnection.mMass = 0;

		/* 2003.06.08, Mongoose - Generate with 0 lengths */
		Simulation::generateMasses(count, mass, 
											Vector3d(0, 0, 0), 
											Vector3d(0, 0, 0));

		/* 2003.06.08, Mongoose - Setup lengths here ( Dependent on index ) */
		for (j = mMasses.begin(), i = 0; j < mMasses.end(); ++j, ++i)
		{
			b = mMasses[j];

			b->mPos.mVec[0] = length * i;

			/* 2003.06.08, Mongoose - Setup springs here too, faster */
			if (i > 0)
			{
				mSprings.pushBack(new Spring(a, b, length, stiffness, friction));
			}

			a = b;
		}

		a->mMass = 10; // Hack to make thread end fall down instead of bounce
	}


	void clear()
	{
		Simulation::clear();
		mSprings.erase();
	}


	void solve()
	{
		VectorIterator<Mass *> iter = VectorIterator<Mass *>(&mMasses);
		unsigned int i = 0;
		Mass *m, *last = 0x0;
		vec_t mass = 0.0;

		
		/* 2003.06.08, Mongoose - Apply spring forces */
		for (i = 0; i < mMassCount - 1; ++i)
		{
			(mSprings[i])->solve();
		}

		/* Apply common forces  */
		for (iter.finish(); iter.backward(); iter.prev())
		{
			m = iter.current();

			if (last)
			{
				m->applyForce(mGravitation * mass);
			}

			/* Apply gravity and air friction forces */
			m->applyForce(mGravitation * m->mMass);
			m->applyForce(-(m->mVelocity) * mAirFrictionConstant);

			/* No resolver support for now */
			if (collisionDetected(m))
			{
				Vector3d v = m->mVelocity;

				//v.mVelocity.mVec[1] = 0;
				m->applyForce(-v * 0.1); // Surface friction

				m->mVelocity.zero();
			}	

			mass += m->mMass;
			last = m;
		}
	}


	void simulate(float dt)
	{
		Simulation::simulate(dt);

		mConnection.mPos += mConnection.mVelocity * dt;

		if (collisionDetected(&mConnection))
		{
			mConnection.mVelocity.zero();
		}

		(mMasses[0])->mPos = mConnection.mPos;
		(mMasses[0])->mVelocity = mConnection.mVelocity;
	}


	void setVelocity(Vector3d connectionVelocity)
	{
		mConnection.mVelocity = connectionVelocity;
	}


	Vector<Spring *> mSprings;		/* Springs joining Masses, 
												Should be (mMassCount - 1) Springs */

	Mass mConnection;					/* Anchor location and velovity of 
												the system */

	Vector3d mGravitation;			/* Force of gravity on masses  */

	vec_t mAirFrictionConstant;	/* Friction of air on thread  */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};

#endif
