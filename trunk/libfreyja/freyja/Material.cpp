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
int (*Material::mLoadShaderFunc)(const char *filename) = NULL;


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
	mShaderId(0),
	mHasAlphaChannel(false),
	mName(),
	mFilename(),
	mMetadata(),
	mBlendSrcString("GL_ONE"),
	mBlendDestString("GL_ONE"),
	mTextureFilename(),
	mShaderFilename()
{
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

#if TINYXML_FOUND
bool Material::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;	
		
	TiXmlElement *mat = new TiXmlElement("material");

	mat->SetAttribute("version", 3);
	mat->SetAttribute("name", mName.c_str() );
	mat->SetAttribute("flags", mFlags);

	if (mTextureFilename.c_str() != 0x0)
	{
		TiXmlElement *element = new TiXmlElement("texture");
		element->SetAttribute("diffuse", mTextureFilename.c_str() );
		mat->LinkEndChild(element);
	}

	if (mShaderFilename.c_str() != 0x0)
	{
		TiXmlElement *element = new TiXmlElement("shader");
		element->SetAttribute("glsl", mShaderFilename.c_str() );
		mat->LinkEndChild(element);
	}

	{
		TiXmlElement *element = new TiXmlElement("blending");
		element->SetAttribute("blend-src", mBlendSrc);
		element->SetAttribute("blend-dest", mBlendDest);
		mat->LinkEndChild(element);
	}

	{
		TiXmlElement *element = new TiXmlElement("shininess");
		element->SetDoubleAttribute("exp", mShininess);
	}

	{
		char *rgba[] = {"r","g","b","a"};
		
		{
			TiXmlElement *color = new TiXmlElement("ambient");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mAmbient[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("diffuse");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mDiffuse[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("specular");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mSpecular[i]);
			}

			mat->LinkEndChild(color);
		}

		{
			TiXmlElement *color = new TiXmlElement("emissive");

			for (unsigned int i = 0; i < 4; ++i)
			{
				color->SetDoubleAttribute(rgba[i], mEmissive[i]);
			}

			mat->LinkEndChild(color);
		}
	}

	if (mMetadata.c_str() != NULL)
	{
		TiXmlElement *metadata = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( mMetadata.c_str() );
		metadata->LinkEndChild(text);
		mat->LinkEndChild(metadata);
	}

	container->LinkEndChild(mat);
	return true;
}


bool Material::Unserialize(TiXmlElement *mat)
{
	if (!mat)
		return false;	

	int attr;
	mat->QueryIntAttribute("version", &attr);

	if (attr != 3)
		return false;

	SetName( mat->Attribute("name") );

	mat->QueryIntAttribute("flags", &attr);
	mFlags = attr < 0 ? INDEX_INVALID : attr;

	TiXmlElement *child = mat->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "texture")
		{
			mTextureFilename = child->Attribute("diffuse");

			if (mLoadTextureFunc) // callback to load texture, get id
			{
				mTexture = mLoadTextureFunc( mTextureFilename.c_str() );
			} 
		}
		else if (s == "shader")
		{
			mShaderFilename = child->Attribute("glsl");

			if (mLoadShaderFunc) // callback to load shader, get id
			{
				mShaderId = mLoadShaderFunc( mShaderFilename.c_str() );
			} 
		}
		else if (s == "blending")
		{
			child->QueryIntAttribute("blend-src", &attr);
			mBlendSrc = attr;
			child->QueryIntAttribute("blend-dest", &attr);
			mBlendDest = attr;			
		}
		else if (s == "shininess")
		{
			child->QueryFloatAttribute("exp", &mShininess);
		}
		else if (s == "ambient")
		{
			child->QueryFloatAttribute("r", mAmbient+0);
			child->QueryFloatAttribute("g", mAmbient+1);
			child->QueryFloatAttribute("b", mAmbient+2);
			child->QueryFloatAttribute("a", mAmbient+3);
		}
		else if (s == "diffuse")
		{
			child->QueryFloatAttribute("r", mDiffuse+0);
			child->QueryFloatAttribute("g", mDiffuse+1);
			child->QueryFloatAttribute("b", mDiffuse+2);
			child->QueryFloatAttribute("a", mDiffuse+3);
		}
		else if (s == "specular")
		{
			child->QueryFloatAttribute("r", mSpecular+0);
			child->QueryFloatAttribute("g", mSpecular+1);
			child->QueryFloatAttribute("b", mSpecular+2);
			child->QueryFloatAttribute("a", mSpecular+3);
		}
		else if (s == "emissive")
		{
			child->QueryFloatAttribute("r", mEmissive+0);
			child->QueryFloatAttribute("g", mEmissive+1);
			child->QueryFloatAttribute("b", mEmissive+2);
			child->QueryFloatAttribute("a", mEmissive+3);
		}
		else if (s == "metadata")
		{
			mMetadata = child->GetText();
		}
	}

	return true;	
}

#endif


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Material::SetName(const char *name)
{
	if (!name || !name[0])
	{
		return;
	}

	mName = name;
}


void Material::SetTextureFilename(const char *name)
{
	if (!name || !name[0])
	{
		return;
	}

	mTextureFilename = name;
}


void Material::SetShaderFilename(const char *name)
{
	if (!name || !name[0])
	{
		return;
	}

	mShaderFilename = name;
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
