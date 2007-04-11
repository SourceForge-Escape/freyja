/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Simulation
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This class simulates mass physics
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2003.06.02:
 * Mongoose - Created, based on algorithms from Erkin Tunca
 =================================================================*/

#include "Simulation.h"

using namespace hel;

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Simulation::Simulation()
{
	mMassCount = 0;
}


Simulation::~Simulation()
{
	clear();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

Mass *Simulation::getMass(unsigned int i)
{
	if (i < mMasses.size())
		return mMasses[i];

	return NULL;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Simulation::attachCollisionObject(CollisionObject *obj)
{
	mObjects.pushBack(obj);
}


void Simulation::clear()
{
	mMasses.erase();
}


bool Simulation::collisionDetected(Mass *mass)
{
	unsigned int i;
	bool detected = false;


	for (i = mObjects.begin(); i < mObjects.end(); ++i)
	{
		if ((mObjects[i])->intersectPoint(mass->mPos))
		{
			detected = true; 
			resolveCollision(mass, mObjects[i]);
		}
	}

	return detected;
}


void Simulation::generateMasses(unsigned int count, vec_t mass)
{
	generateMasses(count, mass, Vec3(0, 0, 0), Vec3(0, 0, 0));
}


void Simulation::setMass(unsigned int idx, vec_t mass,
						 const Vec3 &position, const Vec3 &velocity)
{
	Mass *m = getMass(idx);

	if (m)
	{
		m->mMass = mass;
		m->mPos = position;
		m->mVelocity = velocity;
	}
}


void Simulation::generateMasses(unsigned int count, vec_t mass,
								const Vec3 &position, const Vec3 &velocity)
{
	unsigned int i;


	mMassCount = count;
	//mMasses.reserve(mMasses.size() + count);

	for (i = 0; i < count; ++i)
	{
		mMasses.pushBack(new Mass(mass, position, velocity));
	}
}


void Simulation::init()
{
	unsigned int i;
	Mass *m;

	for (i = mMasses.begin(); i < mMasses.end(); ++i)
	{
		m = mMasses[i];

		if (!m)
			continue;

		m->rest();
	}
}


/* FIXME: Add some fucking springs to push out of intercections here */
void Simulation::resolveCollision(Mass *mass, CollisionObject *obj)
{
	mass->mPos = obj->mIntersection;
}


void Simulation::run(vec_t timeDelta)
{
	// Reset forces
	init();

	// Apply external forces
	solve();

	// Update change in time
	simulate(timeDelta);
}


void Simulation::simulate(vec_t timeDelta)
{
	unsigned int i;
	Mass *m;

	for (i = mMasses.begin(); i < mMasses.end(); ++i)
	{
		m = mMasses[i];

		if (!m)
			continue;

		m->simulate(timeDelta);
	}
}


void Simulation::solve()
{
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

#ifdef UNIT_TEST_SIMULATION
#   include "SDLUnitTest.h"

//#   define ENABLE_TEX_PARTICLES

Simulation *gEmitter;
GravitySimulation gGravityEmitter;
SpringSimulation gSpringEmitter;
SpringSimulation gSpringEmitter2;

SDLUnitTest ut;

bool gOn = false;
float gSpeed = 0.1f;
char gTitle[128];
char gMessage[256];
float gYaw = 0.0f;
bool gYawOn = true;
float gPitch = -20.0f;
float gScale = 1.75f;
float gLightPos[] = { 0.0, 512.0, 1024.0, 0.0 };
unsigned int gWidth = 640, gHeight = 460;
bool gLighting = false;
bool gWireframe = false;


void displayFrame()
{
	Mass *m;
	Vector3d last;
	vec_t x, y, z;
	static float lastTime = 0.0f;
	float time;

	glClearColor(0.3, 0.3, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0,    0.0, -256.0, 
			  0.0,    8.0,    0.0, 
			  0.0,    1.0,    0.0);

	time = getTicks() * 0.1f;

	if (time - lastTime > 5.0f)
	{
		lastTime = time;

		if (gYawOn)
			++gYaw;
	}

	/* Don't use texture for colored lines */
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

#ifdef DRAW_ACTUAL_ORIGIN_AXIS_WITH_ROTATION
	glDrawAxis();
#endif

	// Draw light symbol
	glPushMatrix();
	glTranslatef(gLightPos[0], gLightPos[1], gLightPos[2]);
	glDrawLight(5.0);
	glPopMatrix();

	glTranslatef(0.0f, -128.0f, 512.0f);
	glRotatef(gPitch, 1, 0, 0);
	glRotatef(gYaw,   0, 1, 0);

	// Draw transformed origin axis
	glDrawAxis3f(30.0, 26.25, 3.75);

	// Draw grid
	glDrawGrid(500.0, 50.0);

	glPopAttrib(); /* End solid color rendering */

	if (gLighting)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, gLightPos);
		glEnable(GL_LIGHTING);
	}

	// Draw model
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, 8.0f, 0.0f);
	glScalef(gScale, gScale, gScale);
	glBindTexture(GL_TEXTURE_2D, 1);

	if (gOn)
		gEmitter->run((time - lastTime) * 0.1 * gSpeed); // slow it down

#ifdef ENABLE_TEX_PARTICLES
	// Alpha is determined by life, which is affected by blend amount
	//glColor4f(mColor, g, b, life);

	// Render tristrip quad
	for (gEmitter->mMasses.start(); gEmitter->mMasses.forward(); 
		  gEmitter->mMasses.next())
	{
		m = gEmitter->mMasses.current();

		if (!m)
			continue;

		if (gEmitter == &gSpringEmitter)
		{
			if (gEmitter->mMasses.getCurrentIndex() == 0)
			{
				glDisable(GL_TEXTURE_2D);

				glColor4f(0.0, 1.0, 0.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3fv(gSpringEmitter.connectionPos.mVec);
				glEnd();
			}

			glColor4f(gEmitter->mMasses.getCurrentIndex()*0.33, 0.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			glVertex3fv(m->mPos.mVec);
			glEnd();
			glBegin(GL_LINES);
			glVertex3fv(m->mPos.mVec);
			glVertex3fv(gSpringEmitter.connectionPos.mVec);
			glEnd();
		}
		else
		{
			x = m->mPos.mVec[0];
			y = m->mPos.mVec[1];
			z = m->mPos.mVec[2];

			glBegin(GL_TRIANGLE_STRIP);
			
			glTexCoord2d(1.0, 1.0);
			glVertex3f(x + 25, y + 25, z);
			
			glTexCoord2d(0.0, 1.0);
			glVertex3f(x - 25, y + 25, z);
			
			glTexCoord2d(1.0, 0.0);
			glVertex3f(x + 25, y - 25, z);
			
			glTexCoord2d(0.0, 0.0);
			glVertex3f(x - 25, y - 25, z);
			
			glEnd();
		}
	}

#else
	glBegin(GL_POINTS);

	for (gEmitter->mMasses.start(); gEmitter->mMasses.forward(); 
		  gEmitter->mMasses.next())
	{
		m = gEmitter->mMasses.current();

		if (!m)
			continue;

		glColor4f(0.0, 1.0, 1.0, 1.0);
		glVertex3fv(m->mPos.mVec);
	}

	glEnd();
#endif

	glPopMatrix();

#ifdef HAVE_SDL_TTF
	if (gWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Render 2d text */
	glEnterMode2d(ut.getWidth(), ut.getHeight());

	ut.drawWidgets();

	glExitMode2d();

	if (gWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glFlush();
	ut.swapBuffersGL();
}


void handleKeyPressEvent(unsigned int btn, unsigned int mod)
{
}


void handleKeyReleaseEvent(unsigned int btn, unsigned int mod)
{
	handleEventCode(btn);
}


void gameFrame()
{
}


void handleEventCode(unsigned int key)
{
	Mass *m;

	switch (key)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;
	case SDLK_F9:
		ut.resize(640, 480);
		break;
	case SDLK_F10:
		ut.resize(800, 600);
		break;
	case SDLK_F11:
		ut.resize(1024, 768);
		break;
	case SDLK_F12:
		ut.resize(1280, 1024);
		break;
	case ' ':
		m = gGravityEmitter.getMass(0);

		if (m)
		{
			m->rest();
			m->mPos += Vector3d(0, 2, 0);
			m->mVelocity = Vector3d(0, 10, 10);
			m->applyForce(Vector3d(0.0f, 120.0f, 0.0f));
			printf("Setting mass 0\n");
		}

		m = gGravityEmitter.getMass(1);

		if (m)
		{
			m->mVelocity = Vector3d(0, 100, 0);
			m->applyForce(Vector3d(0.0f, 120.0f, 0.0f));
			printf("Setting mass 1\n");
		}

		m = gGravityEmitter.getMass(2);

		if (m)
		{
			m->rest();
			m->mPos += Vector3d(0, 8, 0);
			m->mVelocity = Vector3d(0, 100, 0);
			//m->applyForce(Vector3d(0.0f, 120.0f, 0.0f));
			printf("Setting mass 2\n");
		}

		break;
	case 'r':
		gGravityEmitter.clear();

		gGravityEmitter.generateMasses(1, 1.0f, 
									   Vector3d(0.0f, 0.0f, 0.0f),
									   Vector3d(0.0f, 96.0f, -64.0f));
		gGravityEmitter.generateMasses(1, 1.0f, 
									   Vector3d(-128.0f, 0.0f, 0.0f),
									   Vector3d(32.0f, 96.0f, 0.0f));
		gGravityEmitter.generateMasses(1, 1.0f,
									   Vector3d(128.0f, 0.0f, 0.0f),
									   Vector3d(-32.0f, 96.0f, 0.0f));
		break;
	case 's':
		gEmitter = &gSpringEmitter;
		break;
	case 'g':
		gEmitter = &gGravityEmitter;
		break;
	case 'x':
		gGravityEmitter.gravitation *= 10.0;
		bufferedPrintf(gMessage, 256, "G is now %f\n", gGravityEmitter.gravitation.mVec[1]);
		break;
	case 'y':
		if (fabs(gGravityEmitter.gravitation.mVec[1] * 0.1) > 0.0000001)
			gGravityEmitter.gravitation *= 0.1;

		bufferedPrintf(gMessage, 256, "G is now %f\n", gGravityEmitter.gravitation.mVec[1]);
		break;
	case 't':
		gOn = !gOn;
		break;
	case '[':
		gSpringEmitter.springConstant += 0.5f;
		bufferedPrintf(gMessage, 256, "Spring constant = %f\n", gSpringEmitter.springConstant);
		break;
	case ']':
		if (gSpringEmitter.springConstant - 0.5 > 0.0f)
			gSpringEmitter.springConstant -= 0.5f;
		bufferedPrintf(gMessage, 256, "Spring constant = %f\n", gSpringEmitter.springConstant);
		break;
	case '.':
		gSpeed *= 10.0;
		bufferedPrintf(gMessage, 256, "Time is at %f speed\n", gSpeed);
		break;
	case ',':
		if (gSpeed * 0.1 > 0.0000001)
			gSpeed *= 0.1;
		bufferedPrintf(gMessage, 256, "Time is at %f speed\n", gSpeed);
		break;
	case 'h':
		printf("\n");
		printf("----------------------------------\n");
		printf("t -       Toggle emitter\n");
		printf("r -       Reset gravity emitter test\n");
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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);    // Q3A uses front face culling
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	glPointSize(5.0f);
	gYawOn = false;
	gScale = 0.3f;

	// Texture setup
	ut.mTexture.reset();
	ut.mTexture.setFlag(Texture::fUseMipmaps);
	ut.mTexture.setMaxTextureCount(64);
	printf("Loading textures: ");
	ut.mTexture.loadTGA("data/snow.tga");
	ut.mTexture.loadTGA("data/snow2.tga");
	printf(" done\n\n");

#ifdef HAVE_SDL_TTF
	ut.mTexture.loadFontTTF("data/Fonts/Playstation.ttf", 32, 126 - 32);
	bufferedPrintf(gMessage, 256, "SDL Unit Test");
#endif
}


int runSimulationUnitTest(int argc, char *argv[])
{
	gEmitter = &gGravityEmitter;


	ut.mResource.registerSymbolValue("EVENT_SHOWBONES",    (int)'0');
	ut.mResource.registerSymbolValue("EVENT_WIREFRAME",    (int)'w');
	ut.mResource.registerSymbolValue("EVENT_QUIT",         SDLK_ESCAPE);
	ut.mResource.registerSymbolValue("EVENT_SCREENSHOT",   SDLK_F1);
	ut.mResource.registerSymbolValue("EVENT_FULLSCREEN",   SDLK_F2);
	ut.mResource.registerSymbolValue("EVENT_RES640x480",   SDLK_F9);
	ut.mResource.registerSymbolValue("EVENT_RES800x600",   SDLK_F10);
	ut.mResource.registerSymbolValue("EVENT_RES1024x768",  SDLK_F11);
	ut.mResource.registerSymbolValue("EVENT_RES1280x1024", SDLK_F12);
	ut.mResource.evalFile("data/Scripts/SimulationUnitTest.lsp");

	// Masses created above the origin, traveling down on Y axis
	// as if it where in a vacuum pulled by gravity
	gGravityEmitter.generateMasses(1, 1.0f, 
								   Vector3d(0.0f, 0.0f, 0.0f),
								   Vector3d(0.0f, 96.0f, 0.0f));
	gGravityEmitter.generateMasses(1, 1.0f, 
								   Vector3d(-64.0f, 1424.0f, 0.0f),
								   Vector3d(-9.8f, 0.0f, 0.0f));
	gGravityEmitter.generateMasses(1, 1.0f,
								   Vector3d(64.0f, 1424.0f, 0.0f),
								   Vector3d(9.8f, 0.0f, 0.0f));

	//InternalBBoxCollisionObject *obj = 
	//new InternalBBoxCollisionObject(-9999, 0, -9999, 
	//										 9999, 9999, 9999);

	HeightCollisionObject *obj = new HeightCollisionObject(0, 9999);

	/* Don't fall out ot the box world  =) */
	gGravityEmitter.attachCollisionObject(obj);
	gSpringEmitter.attachCollisionObject(obj);

	// Spring
	gSpringEmitter.connectionPos = Vector3d(0.0f, 200.0f, 0.0f);
	gSpringEmitter.springConstant = 0.3f;
	gSpringEmitter.generateMasses(1, 0.5f, 
											Vector3d(0.0f, -512.0f, 0.0f),
											Vector3d(3.0f, 0.0f, 0.0f));
	gSpringEmitter.generateMasses(1, 0.5f, 
											Vector3d(-512.0f, -128.0f, 0.0f),
											Vector3d(0.0f, 3.0f, 0.0f));
	gSpringEmitter.generateMasses(1, 0.5f, 
											Vector3d(512.0f, -128.0f, 0.0f),
											Vector3d(0.0f, 0.0f, 0.3f));

	ut.setWindowTitle("[Simulation particle class test]");
	ut.initVideo(800, 600, false);
	initScene(argc, argv);
	ut.runGame();

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Simulation class test]\n");
	runSimulationUnitTest(argc, argv);

	return 0;
}
#endif
