/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaLight
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the light data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created, based on Freyja's Light class from 2002.01.27
 ==========================================================================*/

#include "Light.h"
#include "LightABI.h"

using namespace freyja;

unsigned int Light::mType = 0x4c495445;
uint32 Light::mVersion = 1;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Light::Light()
{
	uint32 i, count;


	mName[0] = 0;
	mOptions = 0;
	mLightType = ePoint;

	mAmbient[0] = 0.1f; mAmbient[1] = 0.1f; mAmbient[2] = 0.1f;
	mAmbient[3] = 1.0f;

	mDiffuse[0] = 1.0f; mDiffuse[1] = 1.0f; mDiffuse[2] = 1.0f;
	mDiffuse[3] = 1.0f;

	mSpecular[0] = 1.0f; mSpecular[1] = 1.0f; mSpecular[2] = 1.0f;
	mSpecular[3] = 1.0f;

	mPos[0] = 0.0f; mPos[1] = 0.0f; mPos[2] = 0.0f; mPos[3] = 1.0f;
	mDir[0] = 0.0f; mDir[1] = 1.0f; mDir[2] = 0.0f; mDir[3] = 0.0f;

	mCutOff = 15.0f;
	mExponent = 0.0f;
	mAttenuation = 1.0f;

	/* Setup gobal pool UID reference */
	mOldUID = INDEX_INVALID;
	mUID = count = mGobalPool.size();

	for (i = 0; i < count; ++i)
	{
		if (mGobalPool[i] == 0x0)
		{
			mUID = i;
			break;
		}	
	}
}


Light::~Light()
{
	/* Mark NULL in pool, marking free slot */
	mGobalPool.assign(mUID, 0x0);
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 Light::getCount()
{
	return mGobalPool.size();
}


Light *Light::getLight(index_t uid)
{
	if (uid >= getCount() || uid == INDEX_INVALID)
		return 0x0;
	
	return mGobalPool[uid];
}


index_t Light::getUID()
{
	return mUID;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

index_t Light::createLight()
{
	Light *l = new Light();
	return l->getUID();
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// C ABI
////////////////////////////////////////////////////////////

//  START  Hmmm... this should be in editor
index_t gFreyjaLightIndex = INDEX_INVALID;

index_t freyjaGetCurrentLight()
{
	return gFreyjaLightIndex;
}

void freyjaCurrentLight(uint32 lightIndex)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		gFreyjaLightIndex = lightIndex;
	}
}
//  END  Hmmm... this should be in editor


index_t freyjaLightCreate()
{
	return Light::createLight();
}


void freyjaLightDelete(index_t lightIndex)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		delete l;
	}
}


// Light mutators /////////////////

void freyjaLightPosition4v(uint32 lightIndex, vec4_t position)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		l->setPosition(position);
	}
}


void freyjaLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		l->mAmbient[0] = ambient[0];
		l->mAmbient[1] = ambient[1];
		l->mAmbient[2] = ambient[2];
		l->mAmbient[3] = ambient[3];
	}
}


void freyjaLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		l->mDiffuse[0] = diffuse[0];
		l->mDiffuse[1] = diffuse[1];
		l->mDiffuse[2] = diffuse[2];
		l->mDiffuse[3] = diffuse[3];
	}
}


void freyjaLightSpecular(uint32 lightIndex, vec4_t specular)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		l->mSpecular[0] = specular[0];
		l->mSpecular[1] = specular[1];
		l->mSpecular[2] = specular[2];
		l->mSpecular[3] = specular[3];
	}
}


// Light accessors /////////////////

void freyjaGetLightPosition4v(uint32 lightIndex, vec4_t position)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		position[0] = l->mPos[0];
		position[1] = l->mPos[1];
		position[2] = l->mPos[2];
		position[3] = l->mPos[3];
	}
}


void freyjaGetLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		ambient[0] = l->mAmbient[0];
		ambient[1] = l->mAmbient[1];
		ambient[2] = l->mAmbient[2];
		ambient[3] = l->mAmbient[3];
	}
}


void freyjaGetLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		diffuse[0] = l->mDiffuse[0];
		diffuse[1] = l->mDiffuse[1];
		diffuse[2] = l->mDiffuse[2];
		diffuse[3] = l->mDiffuse[3];
	}
}


void freyjaGetLightSpecular(uint32 lightIndex, vec4_t specular)
{
	Light *l = Light::getLight(lightIndex);

	if (l)
	{
		specular[0] = l->mSpecular[0];
		specular[1] = l->mSpecular[1];
		specular[2] = l->mSpecular[2];
		specular[3] = l->mSpecular[3];
	}
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJALIGHT
int runLightUnitTest(int argc, char *argv[])
{
	Light test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Light class test]\n");

	return runLightUnitTest(argc, argv);
}
#endif
