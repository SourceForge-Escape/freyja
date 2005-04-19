/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Material
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: GL Material
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.01.21:
 * Mongoose - Created
 =================================================================*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>

#   ifndef GL_COMBINE_ARB
#      define GL_COMBINE_ARB GL_COMBINE_EXT
#   endif

#   ifndef GL_RGB_SCALE_ARB
#      define GL_RGB_SCALE_ARB GL_RGB_SCALE_EXT
#   endif
#else
#   define GL_ZERO                      0x0
#   define GL_ONE                       0x1
#   define GL_SRC_COLOR                 0x0300
#   define GL_ONE_MINUS_SRC_COLOR       0x0301
#   define GL_DST_COLOR                 0x0306
#   define GL_ONE_MINUS_DST_COLOR       0x0307
#   define GL_SRC_ALPHA                 0x0302
#   define GL_ONE_MINUS_SRC_ALPHA       0x0303
#   define GL_DST_ALPHA                 0x0304
#   define GL_ONE_MINUS_DST_ALPHA       0x0305
#   define GL_SRC_ALPHA_SATURATE        0x0308
#   define GL_CONSTANT_COLOR            0x8001
#   define GL_ONE_MINUS_CONSTANT_COLOR  0x8002
#   define GL_CONSTANT_ALPHA            0x8003
#   define GL_ONE_MINUS_CONSTANT_ALPHA  0x8004
#endif

#ifdef USE_ARB_MULTITEXTURE
#   define glActiveTexture glActiveTextureARB
#   define glMultiTexCoord2f glMultiTexCoord2fARB
#endif

#include <mstl/String.h>

#include "Material.h"


unsigned int Material::m_counter = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Material::Material()
{
	ambient[0] = 0.904418; ambient[1] = 0.885908; ambient[2] = 0.885908;
	ambient[3]  = 1.0;

	diffuse[0] = 1.0; diffuse[1] = 1.0; diffuse[2] = 1.0;
	diffuse[3] = 1.0;

	specular[0] = specular[1] = specular[2] = 0.0;
	specular[3] = 1.0;

	emissive[0] = emissive[1] = emissive[2] = 0.0;
	emissive[3] = 1.0;

	shininess = 0.0;
	texture = 0;
	texture2 = 0;
	blend_src = 0;
	blend_dest = 0; 

	m_id = ++m_counter;
	m_flags = fEnable_Texture;
	m_name = String::strdup("Boring default");
	m_textureFilename = NULL;
}


Material::~Material()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int Material::getBlendIndex(int blend)
{
	int index = -1;


	switch (blend)
	{
	case GL_ZERO:
		index = 0;
		break;
	case GL_ONE:
		index = 1;
		break;
	case GL_SRC_COLOR:
		index = 2;
		break;
	case GL_ONE_MINUS_SRC_COLOR:
		index = 3;
		break;
	case GL_DST_COLOR:
		index = 4;
		break;
	case GL_ONE_MINUS_DST_COLOR:
		index = 5;
		break;
	case GL_SRC_ALPHA:
		index = 6;
		break;
	case GL_ONE_MINUS_SRC_ALPHA:
		index = 7;
		break;
	case GL_DST_ALPHA:
		index = 8;
		break;
	case GL_ONE_MINUS_DST_ALPHA:
		index = 9;
		break;
	case GL_SRC_ALPHA_SATURATE:
		index = 10;
		break;
	case GL_CONSTANT_COLOR:
		index = 11;
		break;
	case GL_ONE_MINUS_CONSTANT_COLOR:
		index = 12;
		break;
	case GL_CONSTANT_ALPHA:
		index = 13;
		break; 
	case GL_ONE_MINUS_CONSTANT_ALPHA:
		index = 14;
		break;
	}

	return index;
}


unsigned int Material::getCount()
{
	return m_counter;
}


unsigned int Material::getFlags()
{
	return m_flags;
}


unsigned int Material::getId()
{
	return m_id;
}


const char *Material::getName()
{
	return m_name;
}


const char *Material::getTextureName()
{
	return m_textureFilename;
}


const char *Material::getDetailTextureName()
{
	return m_textureDetailFilename;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Material::setFlag(MaterialFlags_t flag)
{
	m_flags |= flag;

#ifdef HAVE_OPENGL
	switch (flag)
	{
		//case fEnable_Blending:
		//	glEnable(GL_BLEND);
		//	break;
	case fEnable_Normalize:
		glEnable(GL_NORMALIZE);
		break;
	default:
		;
	}
#endif
}


void Material::clearFlag(MaterialFlags_t flag)
{
	m_flags |= flag;
	m_flags ^= flag;

#ifdef HAVE_OPENGL
	switch (flag)
	{
		//case fEnable_Blending:
		//	glDisable(GL_BLEND);
		//	break;
	case fEnable_Normalize:
		glDisable(GL_NORMALIZE);
		break;
	default:
		;
	}	
#endif
}


void Material::setName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (m_name)
	{
		delete [] m_name;
	}

	m_name = new char[len+1];
	strncpy(m_name, name, len);
	m_name[len] = 0;
}


void Material::setTextureName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (m_textureFilename)
	{
		delete [] m_textureFilename;
	}

	m_textureFilename = new char[len+1];
	strncpy(m_textureFilename, name, len);
	m_textureFilename[len] = 0;
}


void Material::setDetailTextureName(const char *name)
{
	int len;


	if (!name || !name[0])
	{
		return;
	}

	len = strlen(name);

	if (m_textureDetailFilename)
	{
		delete [] m_textureDetailFilename;
	}

	m_textureDetailFilename = new char[len+1];
	strncpy(m_textureDetailFilename, name, len);
	m_textureDetailFilename[len] = 0;
}


int Material::saveFile(const char *filename)
{
	FILE *f;

	
	if (!filename || !filename[0])
	{
		return -1;
	}


	f = fopen(filename, "w");

	if (!f)
	{
		perror("Material::saveFile> ");
		return -2;
	}

	fprintf(f, "[Material]\n");
	fprintf(f, "Name=%s\n", getName());

	if (getTextureName())
	{
		fprintf(f, "TextureName=%s\n", getTextureName());
	}

	//fprintf(f, "TextureName2=%s\n", TextureName());

	fprintf(f, "EnableBlending=%s\n", 
			  (m_flags & fEnable_Blending) ? "true" : "false");

	fprintf(f, "BlendSource=%s\n", 
			  (blend_src == GL_ZERO) ? "GL_ZERO" :
			  (blend_src == GL_ONE) ? "GL_ONE" :
			  (blend_src == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_src == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_src == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_src == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_src == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_src == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_src == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_src == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_src == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_src == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_src == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_src == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");


	fprintf(f, "BlendDest=%s\n", 
			  (blend_dest == GL_ZERO) ? "GL_ZERO" :
			  (blend_dest == GL_ONE) ? "GL_ONE" :
			  (blend_dest == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_dest == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_dest == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_dest == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_dest == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_dest == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_dest == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_dest == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_dest == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_dest == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");

	fprintf(f, "Ambient=%f,%f,%f,%f\n", 
			  ambient[0], ambient[1], ambient[2], ambient[3]);
	fprintf(f, "Diffuse=%f,%f,%f,%f\n", 
			  diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	fprintf(f, "Specular=%f,%f,%f,%f\n", 
			  specular[0], specular[1], specular[2], specular[3]);
	fprintf(f, "Emissive=%f,%f,%f,%f\n", 
			  emissive[0], emissive[1], emissive[2], emissive[3]);
	fprintf(f, "Shininess=%f\n", shininess);

	fclose(f);

	return 0;
}


int Material::loadFile(const char *filename)
{
	FILE *f;
	unsigned int i, j, k, l, mode;
	char buffer[128];
	char buf[64];
	bool line_comment;
	char c;

	
	if (!filename || !filename[0])
	{
		return -1;
	}


	f = fopen(filename, "r");

	if (!f)
	{
		perror("Material::loadFile> ");
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
					
					setTextureName(buf);
				}
				else if  (strncmp(buffer, "Name", 4) == 0)
				{
					for (j = 0, k = 5; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					setName(buf);
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
						m_flags |= fEnable_Blending;
					}
					else if (strncmp(buf, "false", 5) == 0)
					{
						m_flags |= fEnable_Blending;
						m_flags ^= fEnable_Blending;
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

					//printf("*** %s\n", buf);

					val = (strncmp(buf, "GL_ZERO", 11) == 0) ? GL_ZERO :
					(strncmp(buf, "GL_SRC_COLOR", 9) == 0) ? GL_SRC_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_SRC_COLOR", 22) == 0) ? GL_ONE_MINUS_SRC_COLOR :
					(strncmp(buf, "GL_DST_COLOR", 9) == 0) ? GL_DST_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_DST_COLOR", 22) == 0) ? GL_ONE_MINUS_DST_COLOR :
					(strncmp(buf, "GL_SRC_ALPHA", 9) == 0) ? GL_SRC_ALPHA :
					(strncmp(buf, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) ? GL_ONE_MINUS_SRC_ALPHA :
					(strncmp(buf, "GL_DST_ALPHA", 9) == 0) ? GL_DST_ALPHA :
					(strncmp(buf, "GL_ONE_MINUS_DST_ALPHA", 22) == 0) ? GL_ONE_MINUS_DST_ALPHA :
					(strncmp(buf, "GL_SRC_ALPHA_SATURATE", 21) == 0) ? GL_SRC_ALPHA_SATURATE :
					(strncmp(buf, "GL_CONSTANT_COLOR", 17) == 0) ? GL_CONSTANT_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_CONSTANT_COLOR", 27) == 0) ? GL_ONE_MINUS_CONSTANT_COLOR :
					(strncmp(buf, "GL_ONE", 6) == 0) ? GL_ONE :
					(strncmp(buf, "GL_CONSTANT_ALPHA", 17) == 0) ? GL_CONSTANT_ALPHA :					GL_ONE_MINUS_CONSTANT_ALPHA;

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

	return 0;
}


void Material::applyEffectGL()
{
#ifdef HAVE_OPENGL
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	glMaterialfv(GL_FRONT, GL_SHININESS, &(shininess));


	if (m_flags & fEnable_DetailTexture)
	{
#ifndef DISABLE_MULTITEXTURE
		glActiveTexture(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		// bump
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	
		glActiveTexture(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// bump
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);

		// Combine, gamma correct
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
#endif
	}

	if (m_flags & fEnable_Texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	if (m_flags & fEnable_Blending)
	{
		glBlendFunc(blend_src, blend_dest);
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
#endif
}



////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MATERIAL
int compare_array(float *a, float *b)
{
	if (a[0] > b[0] || a[0] < b[0])
		return 1;

	if (a[1] > b[1] || a[1] < b[1])
		return 2;

	if (a[2] > b[2] || a[2] < b[2])
		return 3;

	if (a[3] > b[3] || a[3] < b[3])
		return 4;

	return 0;
}


// Mongoose 2002.07.03, Unit tests are no longer members!
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
