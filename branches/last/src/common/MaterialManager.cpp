/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : MaterialManager
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the all singing all dancing interface to
 *           Material objects
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.03.25:
 * Mongoose - Created
 =================================================================*/

#ifdef HAVE_FREYJA_IMAGE
#   include <freyja_image/MtkImage.h>
#endif

#include "utils.h" // Only provides extcheck()

#include "MaterialManager.h"


MaterialManager *MaterialManager::mInstance = 0x0;
MaterialManager *MaterialManager::mSingleton = 0x0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MaterialManager::MaterialManager()
{
	unsigned char rgba[4] = {255, 255, 255, 255};


	/* Start off with a material in the collection */
	mMaterials.pushBack(new Material);
	mCurrent = 0;

	glEnable(GL_TEXTURE_2D);
	mTexture.reset();
	mTexture.setMaxTextureCount(64);
	mTexture.setFlag(Texture::fUseMipmaps);
	mTexture.loadColorTexture(rgba, 32, 32);

	mTextureId = 1;
	mFlags = 0;

	mPalette = 0x0;

	mSingleton = this;
}


void MaterialManager::DestroyInstance()
{
	if (mInstance)
		delete mInstance;
}


MaterialManager *MaterialManager::Instance()
{
	if (!mInstance)
		mInstance = new MaterialManager();

	return mInstance;
}


MaterialManager::~MaterialManager()
{
	mMaterials.erase();

	if (mPalette)
		delete [] mPalette;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void MaterialManager::applyEffectGL()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
		mat->applyEffectGL();
}

void MaterialManager::applyEffectGL(unsigned int index)
{
	Material *mat = mMaterials[index];
	
	if (mat)
		mat->applyEffectGL();
}

unsigned int MaterialManager::getBlendSourceIndex()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->getBlendIndex(mat->blend_src);
	}
	
	return 0;
}

unsigned int MaterialManager::getBlendDestIndex()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->getBlendIndex(mat->blend_dest);
	}

	return 0;
}

unsigned int MaterialManager::getCurrent()
{
	return mCurrent;
}


void MaterialManager::getColor(eColorTypes type, vec4_t color)
{
	Material *mat = mMaterials[mCurrent];
	
	if (!mat)
		return;
	  
	switch (type)
	{
	case eAmbient:
		color[0] = mat->ambient[0];	
		color[1] = mat->ambient[1];	
		color[2] = mat->ambient[2];	
		color[3] = mat->ambient[3];	
		break;
	case eDiffuse:
		color[0] = mat->diffuse[0];
		color[1] = mat->diffuse[1];
		color[2] = mat->diffuse[2];
		color[3] = mat->diffuse[3];
		break;
	case eSpecular:
		color[0] = mat->specular[0];
		color[1] = mat->specular[1];
		color[2] = mat->specular[2];
		color[3] = mat->specular[3];
		break;
	case eEmissive:
		color[0] = mat->emissive[0];
		color[1] = mat->emissive[1];
		color[2] = mat->emissive[2];
		color[3] = mat->emissive[3];
		break;
	}	
}

unsigned int MaterialManager::getFlags()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->getFlags();
	}
		
	return 0;
}

unsigned int MaterialManager::getGeneralFlags()
{
	return mFlags;
}

const char *MaterialManager::getName()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->getName();
	}
		
	return 0x0;
}

vec_t MaterialManager::getShininess()
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->shininess;
	}
		
	return 0;
}

int MaterialManager::save(const char *filename)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		return mat->saveFile(filename);
	}
		
	return -1;
}


void MaterialManager::takeScreenshot(const char *filename, 
									 unsigned int width, unsigned int height)
{
	mTexture.glScreenShot(filename, width, height);
}


unsigned int MaterialManager::getTexture(unsigned int layer)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		switch (layer)
		{
		case 0:
			return mat->texture;
			break;
		case 1:
			return mat->texture2;
			break;
		}
	}

	return 0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void MaterialManager::clearFlag(Material::MaterialFlags_t flag)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
		mat->clearFlag(flag);
}

void MaterialManager::clearGeneralFlag(MaterialManager::fOptionFlags flag)
{
	mFlags ^= flag;
}

int MaterialManager::load(const char *filename)
{
	Material *mat = mMaterials[mCurrent];
	int ret = -1;
	
	if (mat)
	{
		ret = mat->loadFile(filename);
		
		extern void refresh_material_interface();
		refresh_material_interface();
	}
	
	return ret;
}


int MaterialManager::loadTexturePalette(const char *filename)
{
#ifdef HAVE_FREYJA_IMAGE
	MtkImage img;
#endif
	int loadError = -1;
	

	if (mPalette)
		delete [] mPalette;
	
#ifdef HAVE_FREYJA_IMAGE
	loadError = img.PaletteLoad((char *)filename);
	img.Palette(&mPalette);
#else
#   warning MaterialManager::loadTexturePalette() Will be disabled
#endif

	return loadError;
}


int MaterialManager::loadTexture(const char *filename)
{
	MtkImage img;
	unsigned char *image;
	unsigned int w, h;


	// Mongoose 2002.01.10, Evil...
	if (extcheck(filename, ".lst") == 0)
	{
		FILE *f;
		const unsigned int bufferSize = 256;
		unsigned int i = 0;
		char buffer[bufferSize];
		char c;


		f = fopen(filename, "r");
		
		if (!f)
		{
			perror(filename);
			return -1;
		}

		while (fscanf(f, "%c", &c) != EOF)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
				break;
			case ';':
				printf("Loading texture from list '%s'\n", buffer);
				loadTexture(buffer);
				
				i = 0;
				buffer[0] = 0;
				break;
			default:
				if (i > bufferSize-1)
					i = bufferSize-1;
				
				buffer[i++] = c;
				buffer[i] = 0;
			}
		} 
		
		fclose(f);

		return 0;
	}
	
	printf("[MaterialManager::loadTexture]\n");
	printf(" Loading texture '%s'\n", filename);

	if (!img.Load((char *)filename))
	{
		img.Image(&image);
		w = img.Width();
		h = img.Height();


		switch (img.Mode())
		{
		case COLORMODE_RGBA:
			loadTextureBuffer(image, w, h, 32, Texture::RGBA);
			break;
		case COLORMODE_RGB:
			loadTextureBuffer(image, w, h, 24, Texture::RGB);
			break;
		default:
			printf("ERROR Loading Texture with MtkImage!\n");
			delete [] image;
			return -2;
		}
		
		delete [] image;
		
		printf("[Success]\n");

		return 0;
	}
	
	return -100;
}


int MaterialManager::loadTextureBuffer(unsigned char *image, 
										unsigned int width, 
										unsigned int height, 
										unsigned int bpp, 
										Texture::ColorMode type)
{
	int err = 0;


	if (mFlags & fLoadTextureInSlot)
	{
		err = mTexture.loadBufferSlot(image, width, height, type, bpp,
									  mTextureId);
	}
	else
	{
		mTextureId = mTexture.loadBuffer(image, width, height, type, bpp);

		printf("-- %i\n", mTextureId);
	}

	if (err < 0)
	{
		printf("MaterialManager::loadTextureBuffer> ERROR Loading buffer\n");
	}

	return err;
}


void MaterialManager::setBlendSource(unsigned int blend)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
		mat->blend_src = blend;
}
	
void MaterialManager::setBlendDest(unsigned int blend)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
		mat->blend_dest = blend;
}

void MaterialManager::setCurrent(unsigned int index)
{
	mCurrent = index;
}

void MaterialManager::setColor(eColorTypes type, vec4_t color)
{
	Material *mat = mMaterials[mCurrent];
	
	if (!mat)
		return;
	  
	switch (type)
	{
	case eAmbient:
		mat->ambient[0] = color[0];	
		mat->ambient[1] = color[1];	
		mat->ambient[2] = color[2];	
		mat->ambient[3] = color[3];	
		break;
	case eDiffuse:
		mat->diffuse[0] = color[0];
		mat->diffuse[1] = color[1];
		mat->diffuse[2] = color[2];
		mat->diffuse[3] = color[3];
		break;
	case eSpecular:
		mat->specular[0] = color[0];
		mat->specular[1] = color[1];
		mat->specular[2] = color[2];
		mat->specular[3] = color[3];
		break;
	case eEmissive:
		mat->emissive[0] = color[0];
		mat->emissive[1] = color[1];
		mat->emissive[2] = color[2];
		mat->emissive[3] = color[3];
		break;
	}	
}

void MaterialManager::setFlag(Material::MaterialFlags_t flag)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
		mat->setFlag(flag);
}

void MaterialManager::setGeneralFlag(MaterialManager::fOptionFlags flag)
{
	mFlags |= flag;
}

void MaterialManager::setName(const char *name)
{
	Material *mat = mMaterials[mCurrent];

	if (mat)
		mat->setName(name);
}

void MaterialManager::setShininess(vec_t shine)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		mat->shininess = shine;
	}
}

void MaterialManager::setTexture(unsigned int layer, unsigned int id)
{
	Material *mat = mMaterials[mCurrent];
	
	if (mat)
	{
		switch (layer)
		{
		case 0:
			mat->texture = id;
			break;
		case 1:
			mat->texture2 = id;
			break;
		}
	}
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

#ifdef UNIT_TEST_MATERIALMANAGER
int runMaterialManagerUnitTest(int argc, char *argv[])
{
	MaterialManager test;


	return 0;
}


int main(int argc, char *argv[])
{
	printf("[MaterialManager class test]\n");

	return runMaterialManagerUnitTest(argc, argv);
}
#endif
