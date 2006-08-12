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

#include "FreyjaPluginABI.h"

#include "FreyjaMaterial.h"


using namespace mstl;

Vector<FreyjaMaterial *> gFreyjaMaterials;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaMaterial::FreyjaMaterial() :
	mId(-1),
	mFlags(0),
	mParent(-1),
	mShininess(0.0f),
	mTransparency(1.0f),
	mBlendSrc(0),
	mBlendDest(0),
	mTexture(0),
	mHasAlphaChannel(false),
	mTextureName(NULL)
{
	mName[0] = '\0';
	//setTextureName("No shader");

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


FreyjaMaterial::~FreyjaMaterial()
{
	if (mTextureName)
		delete [] mTextureName;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


const char *FreyjaMaterial::getTextureName()
{
	return mTextureName;
}


uint32 FreyjaMaterial::getSerializeSize()
{
	uint32 length = 0;


	if (mTextureName != 0x0)
	{
		length = strlen(mTextureName);	
	}

	return (4 + 64 + 4 + 4 + 4 +
			4 +	length +
			4 + 
			16 + 16 + 16 + 16);
}


bool FreyjaMaterial::serialize(FreyjaFileWriter &w)
{
	uint32 length = 0;


	w.writeInt32U(mVersion);
	w.writeCharString(64, mName);
	w.writeInt32U(mFlags);
	w.writeInt32U(mBlendSrc);
	w.writeInt32U(mBlendDest);

	if (mTextureName != 0x0)
	{
		length = strlen(mTextureName);
		w.writeInt32U(length);
		w.writeCharString(length, mTextureName);
	}
	else
	{
		w.writeInt32U(0); // length
	}

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
	uint32 version = r.readInt32U();
	uint32 length = 0;


	if (version != mVersion)
		return false;

	r.readCharString(64, mName);
	mFlags = r.readInt32U();
	mBlendSrc = r.readInt32U();
	mBlendDest = r.readInt32U();

	length = r.readInt32U();

	if (length > 0)
	{
		char name[length+1];

		r.readCharString(length, name);
		name[length] = 0;		
		//printf("'%s'\n", name);
		setTextureName(name);
	}

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


void FreyjaMaterial::setTextureName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (mTextureName)
	{
		delete [] mTextureName;
	}

	mTextureName = new char[len+1];
	strncpy(mTextureName, name, len);
	mTextureName[len] = 0;
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


///////////////////////////////////////////////////////////////////////
// Material ABI ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

index_t gCurrentMaterial = INDEX_INVALID;

index_t freyjaGetCurrentMaterial()
{
	return gCurrentMaterial;
}


void freyjaCurrentMaterial(index_t materialIndex)
{
	gCurrentMaterial = materialIndex;
}


index_t freyjaMaterialCreate()
{
	index_t materialIndex = gFreyjaMaterials.size();

	gFreyjaMaterials.pushBack(new FreyjaMaterial());
	gFreyjaMaterials[materialIndex]->mId = materialIndex;

	return materialIndex;
}


int32 freyjaLoadMaterialASCII(index_t materialIndex, const char *filename)
{
	FILE *f;
	unsigned int i, j, k, l, mode;
	char buffer[128];
	char buf[64];
	bool line_comment;
	char c;
	vec4_t ambient;
	vec4_t diffuse;
	vec4_t specular;
	vec4_t emissive;
	vec_t shininess;
	unsigned int texture;
	unsigned int blend_src;
	unsigned int blend_dest;
	int32 mIndex = materialIndex;
	//int32 mIndex = freyjaMaterialCreate();

	
	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "r");

	if (!f)
	{
		perror("freyjaLoadMaterialASCII> ");
		return -2;
	}

	i = 0;
	buffer[0] = 0;
	line_comment = false;
	mode = 0;

	// Strip out whitespace and comments
	while (fscanf(f, "%c", &c) != EOF)
	{
		if (line_comment && c != '\n')
			continue;

		if (i > 126)
		{
			printf("Material::loadFile> Overflow handled\n");
			i = 126;
		}
		
		switch (c)
		{
		case ' ':
		case '\v':
		case '\t':
			break;
		case '#':
			buffer[i++] = 0;
			line_comment = true;
			break;
		case '\n':
			if (line_comment)
			{
				line_comment = false;
				i = 0;
				buffer[0] = 0;		 
				continue;
			}
			else if (buffer[0] == 0)
			{
				i = 0;
				continue;
			}

			buffer[i] = 0;

			if (buffer[0] == '[')
			{
				if (strncmp(buffer, "[Material]", 10) == 0)
				{
					mode = 1;
				}
				else
				{
					mode = 0;
				}
			}
			else if (mode == 1)
			{
				if (strncmp(buffer, "Shininess", 9) == 0)
				{
					for (j = 0, k = 10; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					shininess = atof(buf);
				}
				else if  (strncmp(buffer, "TextureName", 11) == 0)
				{
					for (j = 0, k = 12; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					//setTextureName(buf);
				}
				else if  (strncmp(buffer, "Name", 4) == 0)
				{
					for (j = 0, k = 5; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					freyjaMaterialName(mIndex, buf);

					//setName(buf);
				}
				else if  (strncmp(buffer, "EnableBlending", 14) == 0)
				{
					for (j = 0, k = 15; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					if (strncmp(buf, "true", 4) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
					}
					else if (strncmp(buf, "false", 5) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
						//FIXME m_flags ^= Material::fEnable_Blending;
					}
				}
				else if (strncmp(buffer, "Blend", 5) == 0)
				{
					bool is_src = false;
					int val;


					if (strncmp(buffer, "BlendSource", 11) == 0)
					{
						is_src = true;
						k = 12;
					}
					else
					{
						k = 10;
					}

					for (j = 0; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					val = (strncmp(buf, "GL_ZERO", 11) == 0) ? 0x0 :
					(strncmp(buf, "GL_SRC_COLOR", 9) == 0) ? 0x0300 :
					(strncmp(buf, "GL_ONE_MINUS_SRC_COLOR", 22) == 0) ? 0x0301 :
					(strncmp(buf, "GL_DST_COLOR", 9) == 0) ? 0x0306 :
					(strncmp(buf, "GL_ONE_MINUS_DST_COLOR", 22) == 0) ? 0x0307 :
					(strncmp(buf, "GL_SRC_ALPHA", 9) == 0) ? 0x0302 :
					(strncmp(buf, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) ? 0x0303 :
					(strncmp(buf, "GL_DST_ALPHA", 9) == 0) ? 0x0304 :
					(strncmp(buf, "GL_ONE_MINUS_DST_ALPHA", 22) == 0) ? 0x0305 :
					(strncmp(buf, "GL_SRC_ALPHA_SATURATE", 21) == 0) ? 0x0308 :
					(strncmp(buf, "GL_CONSTANT_COLOR", 17) == 0) ? 0x8001 :
					(strncmp(buf, "GL_ONE_MINUS_CONSTANT_COLOR", 27) == 0) ? 0x8002 :
					(strncmp(buf, "GL_ONE", 6) == 0) ? 0x1 :
					(strncmp(buf, "GL_CONSTANT_ALPHA", 17) == 0) ? 0x8003 :					0x8004;

					if (is_src)
					{
						blend_src = val;
					}
					else
					{
						blend_dest = val;
					}
				}
				else if (strncmp(buffer, "Ambient", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							ambient[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					ambient[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Diffuse", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							diffuse[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					diffuse[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Specular", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							specular[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					specular[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Emissive", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							emissive[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					emissive[l++] = atof(buf);
				}
			}

			i = 0;
			buffer[0] = 0;
			break;
		default:
			buffer[i++] = c;
		}
	}

	fclose(f);

	freyjaMaterialAmbient(mIndex, ambient);
	freyjaMaterialDiffuse(mIndex, diffuse);
	freyjaMaterialSpecular(mIndex, specular);
	freyjaMaterialEmissive(mIndex, emissive);
	freyjaMaterialShininess(mIndex, shininess);
	freyjaMaterialTexture(mIndex, texture);
	freyjaMaterialBlendDestination(mIndex, blend_dest);
	freyjaMaterialBlendSource(mIndex, blend_src);

	return 0;
}


/* Material Accessors */

index_t freyjaGetMaterialCount()
{
	return gFreyjaMaterials.size();
}


index_t freyjaGetMaterialIndex(index_t materialIndex, int32 element)
{
	// This is mainly reserved for future use

	if (element > -1 && element < (long)gFreyjaMaterials.size())
	{
		return element;
	}

	return INDEX_INVALID;
}


const char *freyjaGetMaterialName(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mName;
	}	

	return 0x0;
}


index_t freyjaGetMaterialFlags(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mFlags;
	}	

	return INDEX_INVALID;
}


index_t freyjaGetMaterialTexture(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTexture;
	}	

	return INDEX_INVALID;
}


const char *freyjaGetMaterialTextureName(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->getTextureName();
	}

	return 0x0;
}


void freyjaGetMaterialAmbient(index_t materialIndex, vec4_t ambient)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			ambient[i] = gFreyjaMaterials[materialIndex]->mAmbient[i];
	}
}


void freyjaGetMaterialDiffuse(index_t materialIndex, vec4_t diffuse)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			diffuse[i] = gFreyjaMaterials[materialIndex]->mDiffuse[i];
	}
}


void freyjaGetMaterialSpecular(index_t materialIndex, vec4_t specular)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			specular[i] = gFreyjaMaterials[materialIndex]->mSpecular[i];
	}
}


void freyjaGetMaterialEmissive(index_t materialIndex, vec4_t emissive)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			emissive[i] = gFreyjaMaterials[materialIndex]->mEmissive[i];
	}
}


vec_t freyjaGetMaterialShininess(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mShininess;
	}	

	return -1.0f;
}


vec_t freyjaGetMaterialTransparency(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mTransparency;
	}	

	return -1.0f;
}


int32 freyjaGetMaterialBlendSource(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendSrc;
	}	

	return -1;
}


int32 freyjaGetMaterialBlendDestination(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mBlendDest;
	}	

	return -1;
}



/* Material Mutators */

void freyjaMaterialName(index_t materialIndex, const char *name)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		strncpy(gFreyjaMaterials[materialIndex]->mName, name, 64);
		gFreyjaMaterials[materialIndex]->mName[63] = 0;
	}	
}


void freyjaMaterialClearFlag(index_t materialIndex, uint32 flag)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags |= flag;
		gFreyjaMaterials[materialIndex]->mFlags ^= flag;
	}
}


void freyjaMaterialSetFlag(index_t materialIndex, uint32 flag)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags |= flag;
	}
}


void freyjaMaterialFlags(index_t materialIndex, uint32 flags)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mFlags = flags;
	}
}


void freyjaMaterialTexture(index_t materialIndex, index_t textureIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTexture = textureIndex;
	}
}


void freyjaMaterialTextureName(index_t materialIndex, const char *name)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			gFreyjaMaterials[materialIndex]->setTextureName(name);
	}
}


void freyjaMaterialAmbient(index_t materialIndex, const vec4_t ambient)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mAmbient[i] = ambient[i];
	}
}


void freyjaMaterialDiffuse(index_t materialIndex, const vec4_t diffuse)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mDiffuse[i] = diffuse[i];
	}
}


void freyjaMaterialSpecular(index_t materialIndex, const vec4_t specular)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mSpecular[i] = specular[i];
	}
}


void freyjaMaterialEmissive(index_t materialIndex, const vec4_t emissive)
{
	int32 i;

	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		for (i = 0; i < 4; ++i)
			gFreyjaMaterials[materialIndex]->mEmissive[i] = emissive[i];
	}
}


void freyjaMaterialShininess(index_t materialIndex, vec_t exponent)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mShininess = exponent;
	}
}


void freyjaMaterialTransparency(index_t materialIndex, vec_t transparency)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mTransparency = transparency;
	}
}


void freyjaMaterialBlendSource(index_t materialIndex, uint32 factor)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendSrc = factor;
	}
}


void freyjaMaterialBlendDestination(index_t materialIndex, uint32 factor)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[materialIndex])
			return;

		gFreyjaMaterials[materialIndex]->mBlendDest = factor;
	}
}


FreyjaMaterial *freyjaGetMaterialClass(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex];
	}	

	return 0x0;
}


