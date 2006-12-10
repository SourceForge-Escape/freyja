/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Material
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
#include <mstl/Vector.h>
#include "MaterialABI.h"

#include "Material.h"

using namespace mstl;
using namespace freyja;

int (*Material::mLoadTextureFunc)(const char *filename) = NULL;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Material::Material() :
	mId(-1),
	mFlags(0),
	mParent(-1),
	mShininess(0.0f),
	mTransparency(1.0f),
	mBlendSrc(0),
	mBlendDest(0),
	mTexture(0),
	mHasAlphaChannel(false),
	mBlendSrcString("GL_ONE"),
	mBlendDestString("GL_ONE"),
	mTextureFilename()
{
	mName[0] = '\0';

	mAmbient[0]  = mAmbient[1]  = mAmbient[2]  = 0.2;
	mAmbient[3]  = 1.0;

	mDiffuse[0]  = mDiffuse[1]  = mDiffuse[2]  = 0.9;
	mDiffuse[3]  = 1.0;

	mSpecular[0] = mSpecular[1] = mSpecular[2] = 0.0;
	mSpecular[3] = 1.0;

	mEmissive[0] = mEmissive[1] = mEmissive[2] = 0.0;
	mEmissive[3] = 1.0;

	mColor[0] = mColor[1] = mColor[2] = mColor[3] = 1.0f;
}


Material::~Material()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

const char *Material::getTextureName()
{
	return mTextureFilename.c_str();
}


uint32 Material::getSerializeSize()
{
	uint32 length = 0;

	if (mTextureFilename.c_str() != 0x0)
	{
		length = mTextureFilename.length();	
	}

	return (4 + 64 + 4 + 4 + 4 +
			4 +	length +
			4 + 
			16 + 16 + 16 + 16);
}


bool Material::Serialize(SystemIO::TextFileWriter &w)
{
	w.Print("[Material]\n");
	w.Print("\tmVersion %u\n", mVersion);
	w.Print("\tmId %u\n", mId);
	w.Print("\tmName \"%s\"\n", mName);
	w.Print("\tmFlags %u\n", mFlags);
	w.Print("\tmBlendSrc %u\n", mBlendSrc);
	w.Print("\tmBlendDest %u\n", mBlendDest);
	w.Print("\tmTextureName \"%s\"\n", 
			mTextureFilename.c_str() ? mTextureFilename.c_str() : "");

	w.Print("\tmShininess %f\n", mShininess);

	w.Print("\tmAmbient %f %f %f %f\n", 
			mAmbient[0], mAmbient[1], mAmbient[2], mAmbient[3]);

	w.Print("\tmDiffuse %f %f %f %f\n", 
			mDiffuse[0], mDiffuse[1], mDiffuse[2], mDiffuse[3]);

	w.Print("\tmSpecular %f %f %f %f\n", 
			mSpecular[0], mSpecular[1], mSpecular[2], mSpecular[3]);

	w.Print("\tmEmissive %f %f %f %f\n", 
			mEmissive[0], mEmissive[1], mEmissive[2], mEmissive[3]);

	w.Print("END\n");

	return true;
}


bool Material::serialize(SystemIO::FileWriter &w)
{
	uint32 length = 0;

	w.WriteInt32U(mVersion);
	w.WriteString(64, mName);
	w.WriteInt32U(mFlags);
	w.WriteInt32U(mBlendSrc);
	w.WriteInt32U(mBlendDest);

	if (mTextureFilename.c_str() != 0x0)
	{
		length = mTextureFilename.length();
		w.WriteInt32U(length);
		w.WriteString(length, mTextureFilename.c_str());
	}
	else
	{
		w.WriteInt32U(0); // length
	}

	w.WriteFloat32(mShininess);

	w.WriteFloat32(mAmbient[0]);
	w.WriteFloat32(mAmbient[1]);
	w.WriteFloat32(mAmbient[2]);
	w.WriteFloat32(mAmbient[3]);

	w.WriteFloat32(mDiffuse[0]);
	w.WriteFloat32(mDiffuse[1]);
	w.WriteFloat32(mDiffuse[2]);
	w.WriteFloat32(mDiffuse[3]);

	w.WriteFloat32(mSpecular[0]);
	w.WriteFloat32(mSpecular[1]);
	w.WriteFloat32(mSpecular[2]);
	w.WriteFloat32(mSpecular[3]);

	w.WriteFloat32(mEmissive[0]);
	w.WriteFloat32(mEmissive[1]);
	w.WriteFloat32(mEmissive[2]);
	w.WriteFloat32(mEmissive[3]);

	return true;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Material::Serialize(SystemIO::TextFileReader &r)
{
	if (strcmp(r.ParseSymbol(), "[Material]"))
		return false;

	const char *symbol;
	while ((symbol = r.ParseSymbol()))
	{
		if (strcmp(symbol, "END") == 0)
		{
			break;
		}
		else if (strcmp(symbol, "mName") == 0)
		{
			const char *s = r.ParseStringLiteral();
			if (s)
			{
				setName(s);
				delete [] s;
			}
		}
		else if (strcmp(symbol, "mVersion") == 0)
		{
			r.ParseInteger(); // Idealy this should == mVersion (const)
		}
		else if (strcmp(symbol, "mId") == 0)
		{
			r.ParseInteger(); // Old mId
		}
		else if (strcmp(symbol, "mFlags") == 0)
		{
			mFlags = r.ParseInteger();
		}
		else if (strcmp(symbol, "mBlendSrc") == 0)
		{
			mBlendSrc = r.ParseInteger();
		}
		else if (strcmp(symbol, "mBlendDest") == 0)
		{
			mBlendDest = r.ParseInteger();
		}
		else if (strcmp(symbol, "mTextureName") == 0)
		{
			const char *s = r.ParseStringLiteral();
			if (s)
			{
				setTextureName(s);
				if (mLoadTextureFunc)
				{
					mTexture = mLoadTextureFunc(s);
				} 
				delete [] s;
			}
		}
		else if (strcmp(symbol, "mShininess") == 0)
		{
			mShininess = r.ParseFloat();
		}
		else if (strcmp(symbol, "mAmbient") == 0)
		{
			mAmbient[0] = r.ParseFloat();
			mAmbient[1] = r.ParseFloat();
			mAmbient[2] = r.ParseFloat();
			mAmbient[3] = r.ParseFloat();
		}
		else if (strcmp(symbol, "mDiffuse") == 0)
		{
			mDiffuse[0] = r.ParseFloat();
			mDiffuse[1] = r.ParseFloat();
			mDiffuse[2] = r.ParseFloat();
			mDiffuse[3] = r.ParseFloat();
		}
		else if (strcmp(symbol, "mSpecular") == 0)
		{
			mSpecular[0] = r.ParseFloat();
			mSpecular[1] = r.ParseFloat();
			mSpecular[2] = r.ParseFloat();
			mSpecular[3] = r.ParseFloat();
		}
		else if (strcmp(symbol, "mEmissive") == 0)
		{
			mEmissive[0] = r.ParseFloat(); 
			mEmissive[1] = r.ParseFloat();
			mEmissive[2] = r.ParseFloat();
			mEmissive[3] = r.ParseFloat();
		}
	}

	return false;
}


bool Material::serialize(SystemIO::FileReader &r)
{
	uint32 version = r.ReadInt32U();
	uint32 length = 0;


	if (version != mVersion)
		return false;

	r.ReadString(64, mName);
	mFlags = r.ReadInt32U();
	mBlendSrc = r.ReadInt32U();
	mBlendDest = r.ReadInt32U();

	length = r.ReadInt32U();

	if (length > 0)
	{
		char name[length+1];

		r.ReadString(length, name);
		name[length] = 0;		
		//printf("'%s'\n", name);
		setTextureName(name);
	}

	mShininess = r.ReadFloat32();

	mAmbient[0] = r.ReadFloat32();
	mAmbient[1] = r.ReadFloat32();
	mAmbient[2] = r.ReadFloat32();
	mAmbient[3] = r.ReadFloat32();

	mDiffuse[0] = r.ReadFloat32();
	mDiffuse[1] = r.ReadFloat32();
	mDiffuse[2] = r.ReadFloat32();
	mDiffuse[3] = r.ReadFloat32();

	mSpecular[0] = r.ReadFloat32();
	mSpecular[1] = r.ReadFloat32();
	mSpecular[2] = r.ReadFloat32();
	mSpecular[3] = r.ReadFloat32();

	mEmissive[0] = r.ReadFloat32();
	mEmissive[1] = r.ReadFloat32();
	mEmissive[2] = r.ReadFloat32();
	mEmissive[3] = r.ReadFloat32();

	return true;
}


void Material::setFlag(Flags flag)
{
	mFlags |= flag;
}


void Material::clearFlag(Flags flag)
{
	mFlags |= flag;
	mFlags ^= flag;
}


void Material::setName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (len > 63)
		len = 63;

	strncpy(mName, name, len);
	mName[len] = 0;
}


void Material::setTextureName(const char *name)
{
	if (!name || !name[0])
	{
		return;
	}

	mTextureFilename = name;
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
int runMaterialUnitTest(int argc, char *argv[])
{
	Material test1, test2, test3;


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
	printf("[Material class test]\n");

	return runMaterialUnitTest(argc, argv);
}
#endif
