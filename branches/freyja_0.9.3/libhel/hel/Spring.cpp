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
 *-- History ------------------------------------------------- 
 *
 * 2003.06.06:
 * Mongoose - Created
 =================================================================*/

#include "Spring.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Spring::Spring(Mass *a, Mass *b, vec_t len, vec_t stiffness, vec_t friction)
{
	mMassA = a;
	mMassB = b;
	mSpringLength = len;
	mSpringConstant = stiffness;
	mFrictionConstant = friction;
}


Spring::~Spring()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Spring::solve()
{
	Vector3d force;
	Vector3d v = mMassA->mPos - mMassB->mPos;  // Vector BA
	vec_t r = v.magnitude(); // Length of BA


	force.zero();

	// Add spring force to the force
	if (r != 0)
	{
		force += (v / r) * (r -  mSpringLength) * (-mSpringConstant);
	}
	
	// Friction is added to the force
	force += -(mMassA->mVelocity - mMassB->mVelocity) * mFrictionConstant;

	// Net force of the spring, apply force to each mass
	mMassA->applyForce(force);
	mMassB->applyForce(-force);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_SPRING

#define SDL_UNIT_TEST
#include "SDLUnitTest.cpp"

Simulation *gSim;
ThreadSpringSimulation gSpringSim;
bool gOn = false;
float gSpeed = 0.1f;


// Draw particles
#define ENABLE_TEX_PARTICLES
void renderScene(float time, float lastTime)
{
	Mass *a, *b;
	Vector3d last;
	vec_t c;

	/* 
	glPrint3d(128, 128, 0, 
				 0, 180, 180, 
				 1.25,
				 "This is a test of SDL_TTF subsystem");
	*/

	if (gOn)
	{
		gSim->run((time - lastTime) * (gSpeed * 0.01)); // slow it down
	}

	// Render spring chain
	a = 0x0;
	for (gSim->mMasses.start(); gSim->mMasses.forward(); gSim->mMasses.next())
	{
		b = gSim->mMasses.current();

		if (a)
		{
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3fv(a->mPos.mVec);
			glVertex3fv(b->mPos.mVec);
			glEnd();

			c = 1.0f - 0.5 * ((float)gSim->mMasses.getCurrentIndex() / 
									(float)gSim->mMasses.end());

			glColor3f(c, 0.0, c);
			glBegin(GL_POINTS);
			glVertex3fv(b->mPos.mVec);
			glEnd();
		}
		else
		{
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			glVertex3fv(b->mPos.mVec);
			glEnd();
		}
		
		a = b;
	}

	/* Render 2d text */
	glEnterMode2d(gWidth, gHeight);
	glColor3f(1.0, 1.0, 1.0);
	glPrint2d(0, 0, 0.90, "libHel Spring unit test");
	glExitMode2d();
}


void handleKey(int key)
{
	switch (key)
	{
	case 'v':
		gSpringSim.mConnection.mVelocity += Vector3d(0, 0, 10);
		break;
	case 'b':
		gSpringSim.mConnection.mVelocity -= Vector3d(0, 0, 10);
		break;
	case 'n':
		gSpringSim.mConnection.mVelocity += Vector3d(10, 0, 0);
		break;
	case 'm':
		gSpringSim.mConnection.mVelocity -= Vector3d(10, 0, 0);
		break;
	case 'r':
		gSpringSim.clear();
		gSpringSim.generateMasses(32, 0.05, 
										  Vector3d(0, 400, 0), Vector3d(0, 0, 0),
										  0.05, 10000, 0.2);
		break;
	case 'f':
		gSpringSim.mGravitation *= 10.0;
		printf("G is now %f\n", gSpringSim.mGravitation.mVec[1]);
		break;
	case 'g':
		if (fabs(gSpringSim.mGravitation.mVec[1] * 0.1) > 0.0000001)
			gSpringSim.mGravitation *= 0.1;

		printf("G is now %f\n", gSpringSim.mGravitation.mVec[1]);
		break;
	case 's':
		gOn = !gOn;
		break;
// 	case '[':
// 		gSpringEmitter.springConstant += 0.5f;
// 		printf("Spring constant = %f\n", gSpringEmitter.springConstant);
// 		break;
// 	case ']':
// 		if (gSpringEmitter.springConstant - 0.5 > 0.0f)
// 			gSpringEmitter.springConstant -= 0.5f;
// 		printf("Spring constant = %f\n", gSpringEmitter.springConstant);
// 		break;
	case '.':
		gSpeed *= 10.0;
		printf("Time is at %f speed\n", gSpeed);
		break;
	case ',':
		if (gSpeed * 0.1 > 0.0000001)
			gSpeed *= 0.1;
		printf("Time is at %f speed\n", gSpeed);
		break;
	case 'h':
		printf("\n");
		printf("----------------------------------\n");
		printf("r -       Reset test\n");
		printf("[ ] -     Adjust spring stiffness\n");
		printf(", . -     Adjust time delta\n");
		printf("g -       Scale gravity down\n");
		printf("f -       Scale gravity up\n");
		printf("s -       Start/stop simulation\n");
		break;
	}
}


void handleArgKey(char key)
{
	switch (key)
	{
	default:
		;
	}
}


void initScene(int argc, char *argv[])
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	// Texture setup
	gTexture.reset();
	gTexture.setMaxTextureCount(64);
	gTexture.setFlag(Texture::fUseMipmaps);
	printf("Loading textures: ");
	gTexture.loadTGA("data/snow.tga");
	gTexture.loadTGA("data/snow2.tga");
	gTexture.loadFontTTF("data/test.ttf", 32, 126 - 32);
	printf(" done\n\n");

	// App setup
	glPointSize(5.0f);
	gYawOn = false;
	gScale = 1.0f;
}


int runSpringUnitTest(int argc, char *argv[])
{
	HeightCollisionObject *obj = new HeightCollisionObject(0, 9999);

	gSpringSim.generateMasses(16, 0.05, 
									  Vector3d(0, 128, 0), Vector3d(0, -9.8, 0),
									  0.05, 100.0, 0.02);
	gSpringSim.attachCollisionObject(obj);

	gSim = &gSpringSim;
	

	updateWindowTitle("[Spring simulation class test]");
	runSDLUnitTest(argc, argv);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Spring class test]\n");
	runSpringUnitTest(argc, argv);

	return 0;
}
#endif
