/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMaterial
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Material class for libfreyja, yadda yadda
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.01.08:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>

#include "FreyjaMaterial.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaMaterial::FreyjaMaterial()
{
	mId = -1;

	mName[0] = 0;

	mFlags = 0;

	mParent = -1;

	mTexture = -1;

	mAmbient[0]  = mAmbient[1]  = mAmbient[2]  = 0.2;
	mAmbient[3]  = 1.0;

	mDiffuse[0]  = mDiffuse[1]  = mDiffuse[2]  = 0.9;
	mDiffuse[3]  = 1.0;

	mSpecular[0] = mSpecular[1] = mSpecular[2] = 0.0;
	mSpecular[3] = 1.0;

	mEmissive[0] = mEmissive[1] = mEmissive[2] = 0.0;
	mEmissive[3] = 1.0;

	mShininess = 0.0;

	mBlendSrc = 0;
	mBlendDest = 0; 
}


FreyjaMaterial::~FreyjaMaterial()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool FreyjaMaterial::serialize(FreyjaFileWriter &w)
{
	w.writeLongU(mVersion);
	w.writeCharString(64, mName);
	w.writeLongU(mFlags);
	w.writeLongU(mBlendSrc);
	w.writeLongU(mBlendDest);

	w.writeFloat32(mShininess);

	w.writeFloat32(mAmbient[0]);
	w.writeFloat32(mAmbient[1]);
	w.writeFloat32(mAmbient[2]);
	w.writeFloat32(mAmbient[3]);

	w.writeFloat32(mDiffuse[0]);
	w.writeFloat32(mDiffuse[1]);
	w.writeFloat32(mDiffuse[2]);
	w.writeFloat32(mDiffuse[3]);

	w.writeFloat32(mSpecular[0]);
	w.writeFloat32(mSpecular[1]);
	w.writeFloat32(mSpecular[2]);
	w.writeFloat32(mSpecular[3]);

	w.writeFloat32(mEmissive[0]);
	w.writeFloat32(mEmissive[1]);
	w.writeFloat32(mEmissive[2]);
	w.writeFloat32(mEmissive[3]);

	return true;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool FreyjaMaterial::serialize(FreyjaFileReader &r)
{
	uint32 version = r.readLongU();

	if (version != mVersion)
		return false;

	r.readCharString(64, mName);
	mFlags = r.readLongU();
	mBlendSrc = r.readLongU();
	mBlendDest = r.readLongU();

	mShininess = r.readFloat32();

	mAmbient[0] = r.readFloat32();
	mAmbient[1] = r.readFloat32();
	mAmbient[2] = r.readFloat32();
	mAmbient[3] = r.readFloat32();

	mDiffuse[0] = r.readFloat32();
	mDiffuse[1] = r.readFloat32();
	mDiffuse[2] = r.readFloat32();
	mDiffuse[3] = r.readFloat32();

	mSpecular[0] = r.readFloat32();
	mSpecular[1] = r.readFloat32();
	mSpecular[2] = r.readFloat32();
	mSpecular[3] = r.readFloat32();

	mEmissive[0] = r.readFloat32();
	mEmissive[1] = r.readFloat32();
	mEmissive[2] = r.readFloat32();
	mEmissive[3] = r.readFloat32();

	return true;
}


void FreyjaMaterial::setFlag(Flags flag)
{
	mFlags |= flag;
}


void FreyjaMaterial::clearFlag(Flags flag)
{
	mFlags |= flag;
	mFlags ^= flag;
}


void FreyjaMaterial::setName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (len > 63)
		len = 63;

	//if (mName)
	//{
	//	delete [] mName;
	//}

	//mName = new char[len+1];
	strncpy(mName, name, len);
	mName[len] = 0;
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

#ifdef UNIT_TEST_FREYJAMATERIAL
int runFreyjaMaterialUnitTest(int argc, char *argv[])
{
	FreyjaMaterial test1, test2, test3;


	test1.setName("Glass");
	test2.setName("Wood");
	test3.setName("Metal");

	test1.setTextureName("r_glass.tga");
	test2.setTextureName("r_wood_grain.tga");
	test3.setTextureName("r_polished_metal.tga");

	printf("{%s %i %s}\n", 
			 test1.getName(), test1.getId(), test1.getTextureName());
	printf("{%s %i %s}\n", 
			 test2.getName(), test2.getId(), test2.getTextureName());
	printf("{%s %i %s}\n", 
			 test3.getName(), test3.getId(), test3.getTextureName());

	printf("Count should be 3 = %i\n", Material::getCount());

	// Mongoose 2002.02.14, Configure test1 material with nondefault values
	test1.blend_src = GL_ONE;
	test1.blend_dest = GL_ONE_MINUS_DST_COLOR;
	test1.setFlag(fEnable_Blending);
	test1.diffuse[0] = test1.specular[0] = test1.emissive[0] = test1.ambient[0] = 0.01;
	test1.diffuse[1] = test1.specular[1] = test1.emissive[1] = test1.ambient[1] = 0.02;
	test1.diffuse[2] = test1.specular[2] = test1.emissive[2] = test1.ambient[2] = 0.03;
	test1.diffuse[3] = test1.specular[3] = test1.emissive[3] = test1.ambient[3] = 0.04;
	test1.shininess = 0.67;

	printf("Starting file I/O tests, when finished\n");
	printf("'/tmp/material.mat' and '/tmp/material2.mat' should match...\n");

	test1.saveFile("/tmp/material.mat");

	printf("\nLoad test errors:\n");
	test2.loadFile("/tmp/material.mat");

	// Mongoose 2002.02.14, Compare test1 and test2 values 
	//   here to see if they match
	if (compare_array(test1.ambient, test2.ambient))
	{
		printf("ERROR> ambient file I/O corrupt!\n");
	}

	if (compare_array(test1.diffuse, test2.diffuse))
	{
		printf("ERROR> diffuse file I/O corrupt!\n");
	}

	if (compare_array(test1.specular, test2.specular))
	{
		printf("ERROR> specular file I/O corrupt!\n");
	}

	if (compare_array(test1.emissive, test2.emissive))
	{
		printf("ERROR> emissive file I/O corrupt!\n");
	}

	if (test1.shininess > test2.shininess || test1.shininess < test2.shininess)
	{
		printf("ERROR> shininess file I/O corrupt!\n");
	}

	if (test1.blend_src != test2.blend_src)
	{
		printf("ERROR> blend_src file I/O corrupt!\n");
	}

	if (test1.blend_dest != test2.blend_dest)
	{
		printf("ERROR> blend_dest file I/O corrupt!\n");
	}

	if (test1.getFlags() & fEnable_Blending != 
		 test2.getFlags() & fEnable_Blending)
	{
		printf("ERROR> blending file I/O corrupt!\n");
	}

	printf("\nSave test errors:\n");
	test2.saveFile("/tmp/material2.mat");


	// Mongoose 2002.02.14, Compare test1 and test2 values 
	//   here to see if they match
	if (compare_array(test1.ambient, test2.ambient))
	{
		printf("ERROR> ambient file I/O corrupt!\n");
	}

	if (compare_array(test1.diffuse, test2.diffuse))
	{
		printf("ERROR> diffuse file I/O corrupt!\n");
	}

	if (compare_array(test1.specular, test2.specular))
	{
		printf("ERROR> specular file I/O corrupt!\n");
	}

	if (compare_array(test1.emissive, test2.emissive))
	{
		printf("ERROR> emissive file I/O corrupt!\n");
	}

	if (test1.shininess > test2.shininess || test1.shininess < test2.shininess)
	{
		printf("ERROR> shininess file I/O corrupt!\n");
	}

	if (test1.blend_src != test2.blend_src)
	{
		printf("ERROR> blend_src file I/O corrupt!\n");
	}

	if (test1.blend_dest != test2.blend_dest)
	{
		printf("ERROR> blend_dest file I/O corrupt!\n");
	}

	if (test1.getFlags() & fEnable_Blending != 
		 test2.getFlags() & fEnable_Blending)
	{
		printf("ERROR> blending file I/O corrupt!\n");
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaMaterial class test]\n");

	return runFreyjaMaterialUnitTest(argc, argv);
}
#endif
