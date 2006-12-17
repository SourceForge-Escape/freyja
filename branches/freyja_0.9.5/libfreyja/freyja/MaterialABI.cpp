/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the material ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#include <mstl/Vector.h>
#include "Material.h"
#include "MaterialABI.h"

using namespace mstl;
using namespace freyja;

// FIXME: Update to class internal pool system

index_t gCurrentMaterial = INDEX_INVALID;
Vector<Material *> gFreyjaMaterials;

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

	gFreyjaMaterials.pushBack(new Material());
	gFreyjaMaterials[materialIndex]->mId = materialIndex;

	return materialIndex;
}


void freyjaMaterialClearAll()
{
	gFreyjaMaterials.erase();
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


// This is kind of worthless... ax it before release?
index_t freyjaGetMaterialIndex(index_t materialIndex, uint32 element)
{
	if (element < (long)gFreyjaMaterials.size())
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
			return gFreyjaMaterials[materialIndex]->GetName();
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


index_t freyjaGetMaterialShader(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->mShaderId;
	}	

	return 0; // Never pass shader system a huge number, use default 'off'
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


const char *freyjaGetMaterialShaderName(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->GetShaderFilename();
	}

	return 0x0;
}


const char *freyjaGetMaterialTextureName(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex]->GetTextureFilename();
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

		gFreyjaMaterials[materialIndex]->SetName(name);
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


void freyjaMaterialShader(index_t material, index_t shader)
{
	if (material < gFreyjaMaterials.size())
	{
		if (!gFreyjaMaterials[material])
			return;

		gFreyjaMaterials[material]->mShaderId = shader;
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
			gFreyjaMaterials[materialIndex]->SetTextureFilename(name);
	}
}


void freyjaMaterialShaderName(index_t materialIndex, const char *filename)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			gFreyjaMaterials[materialIndex]->SetShaderFilename(filename);
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


Material *freyjaGetMaterialClass(index_t materialIndex)
{
	if (materialIndex < gFreyjaMaterials.size())
	{
		if (gFreyjaMaterials[materialIndex])
			return gFreyjaMaterials[materialIndex];
	}	

	return 0x0;
}


bool freyjaMaterialLoadChunkTextJA(SystemIO::TextFileReader &r)
{
	index_t mat = freyjaMaterialCreate();
	freyjaPrintMessage("> Reading in material %i...", mat);
	Material *m = freyjaGetMaterialClass(mat);
	return m ? m->Serialize(r) : false;
}
