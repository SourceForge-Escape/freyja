/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Md3
 * Comments: This is the MD3 model module.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 - 2000
 *
 *           MD3 file format specs provided by Mental Vortex,
 *           and that site says the specs may not be 100% correct
 *           so - since this class is desgined using their specs...
 *
 *           Of course I've touched up the specs as best I could, for as
 *           much as I use MD3 this works 100%.  Also added animation 
 *           support and other nice things.
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2002.10.19:
 * Mongoose - Interface update, Animation support starts
 *
 * 2000.10.07:
 * Mongoose - Interface update
 *
 * 2000.10.06:
 * Mongoose - The new code for the new interface
 *
 * 2000.06.03:
 * Mongoose - Those specs where FUBAR - but I figured out the ordering
 * Mongoose - Created from MD3 specs provided by Mental Vortex
 ==========================================================================*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#endif

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include "Md3.h"

using namespace mstl;


Md3::Md3()
{
	mFlags = fDecodeNormals;

	memset(m_filename, 0, 68);     
 
	m_id = 0;
	m_version = 0;           
	m_num_bones = 0;   
	m_num_tags = 0;          
	m_num_meshes = 0;        
	m_max_skins = 0;

	m_header_length = 0;     
	m_tag_start = 0;         
	m_surfaces_start = 0;          
	m_file_size = 0;        

	m_debug = 1;

	m_tags = NULL;     
	m_bones = NULL;
	m_meshes = NULL;
	slaveTest = NULL;
	texTest = NULL;
}


Md3::Md3(unsigned int num_meshes, unsigned int num_bones, unsigned int num_tags)
{	
	memset(m_filename, 0, 68);     
 
	m_id = 0;
	m_version = 0;
	m_max_skins = 0;

	m_header_length = 0;     
	m_tag_start = 0;         
	m_surfaces_start = 0;          
	m_file_size = 0;        

	m_debug = 1;

	m_tags = NULL;     
	m_bones = NULL;
	m_meshes = NULL;
	slaveTest = NULL;
	texTest = NULL;

	m_num_bones = num_bones;   
	m_num_tags = num_tags;          
	m_num_meshes = num_meshes;
	createMeshes(num_meshes);
	createTags(num_tags*num_bones);
	createBones(num_bones);
}


Md3::~Md3()
{
	reset();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned int Md3::getNumMeshes()
{
	return m_num_meshes;
}


unsigned int Md3::getNumTags()
{
	return m_num_tags;
}


unsigned int Md3::getNumBones()
{
	return m_num_bones;
}


md3_mesh_t *Md3::getMeshes()
{
	return m_meshes;
}


md3_tag_t *Md3::getTags()
{
	return m_tags;
}


md3_bone_t *Md3::getBones()
{
	return m_bones;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Md3::toggleFlag(Md3Flag flag)
{
	mFlags ^= flag;
}


void Md3::reset()
{
	int i;

	memset(m_filename, 0, 68);     

	m_id = 0;
	m_version = 0;           
	m_num_bones = 0;   
	m_num_tags = 0;          
	m_num_meshes = 0;        
	m_max_skins = 0;
	m_header_length = 0;     
	m_tag_start = 0;         
	m_surfaces_start = 0;          
	m_file_size = 0;  

	if (m_meshes)
	{
		for (i = 0; i < m_num_meshes; ++i)
		{
			if (m_meshes[i].skin)
			{
				delete [] m_meshes[i].skin;
			}

			if (m_meshes[i].tris)
			{
				delete [] m_meshes[i].tris;
			}

			if (m_meshes[i].texel)
			{
				delete [] m_meshes[i].texel;
			}

			if (m_meshes[i].vertex)
			{
				delete [] m_meshes[i].vertex;
			}
		}

		delete [] m_meshes;
	}

	if (m_tags)
	{
		delete [] m_tags;
	}

	if (m_bones)
	{
		delete [] m_bones;
	}

	if (slaveTest)
	{
		delete [] slaveTest;
	}

	if (texTest)
	{
		delete [] texTest;
	}
} 


void Md3::setDebug(unsigned char level)
{
	m_debug = level;
}


void Md3::setMeshes(md3_mesh_t *meshes, unsigned long count)
{
	long i;


	if (m_meshes)
	{
		for (i = 0; i < m_num_meshes; ++i)
		{
			if (m_meshes[i].skin)
			{
				delete [] m_meshes[i].skin;
			}

			if (m_meshes[i].tris)
			{
				delete [] m_meshes[i].tris;
			}

			if (m_meshes[i].texel)
			{
				delete [] m_meshes[i].texel;
			}

			if (m_meshes[i].vertex)
			{
				delete [] m_meshes[i].vertex;
			}
		}

		delete [] m_meshes;
	}

	m_meshes = meshes;
	m_num_meshes = count;
}


// FIXME: Only partial endian support
int Md3::load(const char *filename)
{
	FILE *f;
	int i, j, k, npherno_warn = 0;
	bool dump_skin;
	int16_t ss;
	int8_t sb;


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("Md3::load> \n");
		return -1;
	}

	// Start Header ////////////////

	fread(&m_id, 4, 1, f);
	printDebug("load", "id = 0x%x\n", m_id);
  
	if (m_id != MD3_IDALIASHEADER)
	{
		printError("load", "header not 0x%x\n", MD3_IDALIASHEADER);
		return -2;
	}
  
	fread(&m_version, 4, 1, f);
	printDebug("load", "version = %i\n", m_version);

	if (m_version != MD3_ALIAS_VERSION)
	{
		printError("load", "version not %i\n", MD3_ALIAS_VERSION);
		return -3;
	}
  
	fread(&m_filename, 68, 1, f);
	printDebug("load", "filename = '%s'\n", m_filename);

	fread(&m_num_bones, 4, 1, f);
	printDebug("load", "num_bone_frames = %i\n", m_num_bones);

	fread(&m_num_tags, 4, 1, f);
	printDebug("load", "num_tags = %i\n", m_num_tags);

	fread(&m_num_meshes, 4, 1, f);
	printDebug("load", "num_meshes = %i\n", m_num_meshes);

	fread(&m_max_skins, 4, 1, f);
	printDebug("load", "max_skins = %i\n", m_max_skins);

	fread(&m_header_length, 4, 1, f);
	printDebug("load", "header_length = %i\n", m_header_length);

	fread(&m_tag_start, 4, 1, f);
	printDebug("load", "tag_start = %i\n", m_tag_start);

	fread(&m_surfaces_start, 4, 1, f);
	printDebug("load", "surfaces_start = %i\n", m_surfaces_start);

	fread(&m_file_size, 4, 1, f);
	printDebug("load", "file_size = %i\n", m_file_size);
 
	// End Header //////////////////
	printDebug("load", "Loading %i bones\n", m_num_bones);
	createBones(m_num_bones);

	for (i = 0; i < m_num_bones; ++i)
	{
		//fread(&m_bones[i].mins, 12, 1, f);
		fread(&m_bones[i].mins[0], 4, 1, f);
		fread(&m_bones[i].mins[1], 4, 1, f);
		fread(&m_bones[i].mins[2], 4, 1, f);
		//fread(&m_bones[i].maxs, 12, 1, f);
		fread(&m_bones[i].maxs[0], 4, 1, f);
		fread(&m_bones[i].maxs[1], 4, 1, f);
		fread(&m_bones[i].maxs[2], 4, 1, f);
		//fread(&m_bones[i].center, 12, 1, f);
		fread(&m_bones[i].center[0], 4, 1, f);
		fread(&m_bones[i].center[1], 4, 1, f);
		fread(&m_bones[i].center[2], 4, 1, f);
		//fread(&m_bones[i].scale, 4, 1, f);
		fread(&m_bones[i].scale, 4, 1, f);
		fread(&m_bones[i].creator, 16, 1, f);

		if (!i && strncmp("NPherno->MD3", m_bones[i].creator, 16) == 0)
		{
			npherno_warn = 1;
		}

		printDebug("load", "bone[%i].mins = %f %f %f\n", i, 
				 m_bones[i].mins[0], m_bones[i].mins[1], m_bones[i].mins[2]);
		printDebug("load", "bone[%i].maxs = %f %f %f\n", i, 
				 m_bones[i].maxs[0], m_bones[i].maxs[1], m_bones[i].maxs[2]);
		printDebug("load", "bone[%i].center = %f %f %f\n", i, 
				 m_bones[i].center[0], m_bones[i].center[1], m_bones[i].center[2]);
		printDebug("load", "bone[%i].scale = %f\n", i, m_bones[i].scale);
		printDebug("load", "bone[%i].creator = '%s'\n", i, m_bones[i].creator);
	}


	if (m_tag_start != ftell(f))
	{
		printWarning("load", "tag_start %i != file pos %lu\n", 
						 m_tag_start,ftell(f));

		fseek(f, m_tag_start, SEEK_SET);
		printWarning("load", "File position set to %lu\n", ftell(f));
	}

	printDebug("load", "Loading %i tags\n", m_num_tags * m_num_bones);
	createTags(m_num_tags * m_num_bones);

	for (i = 0; i < m_num_tags * m_num_bones; ++i)
	{
		fread(&m_tags[i].name, 64, 1, f);
		//fread(&m_tags[i].center, 12, 1, f);
		fread(&m_tags[i].center[0], 4, 1, f);
		fread(&m_tags[i].center[1], 4, 1, f);
		fread(&m_tags[i].center[2], 4, 1, f);
		fread(&m_tags[i].rotation, 36, 1, f);

		printDebug("load", "tag[%i].name = '%s'\n", i, m_tags[i].name);
	}

	if (m_surfaces_start != ftell(f))
	{
		printWarning("load", "surfaces_start %i != file pos %lu\n", 
						 m_surfaces_start, ftell(f));

		fseek(f, m_surfaces_start, SEEK_SET);
		printWarning("load", "File position set to %lu\n", ftell(f));
	}
 
	printDebug("load", "Loading %i meshes\n", m_num_meshes);
	createMeshes(m_num_meshes);

	for(i = 0; i < m_num_meshes; ++i)
	{
		// Start Mesh Header /////////////////

		fread(&m_meshes[i].id, 4, 1, f);
		fread(&m_meshes[i].name, 64, 1, f);
		fread(&m_meshes[i].flags, 4, 1, f);
		fread(&m_meshes[i].num_frames, 4, 1, f);
    
		fread(&m_meshes[i].num_shaders, 4, 1, f);
		m_meshes[i].num_skins = m_meshes[i].num_shaders;
    
		fread(&m_meshes[i].num_vertices, 4, 1, f);
		fread(&m_meshes[i].num_triangles, 4, 1, f);
		fread(&m_meshes[i].tris_offset, 4, 1, f);
		fread(&m_meshes[i].header_size, 4, 1, f);   // offset from start of surface
		// to shaders
		fread(&m_meshes[i].texel_offset, 4, 1, f);
		fread(&m_meshes[i].vertex_offset, 4, 1, f); // offset to normals
		// num_verts * num_frames
		fread(&m_meshes[i].mesh_size, 4, 1, f);     // next surface

		printDebug("load", "mesh[%i].id = '%c%c%c%c'\n", i, 
				 m_meshes[i].id[0], m_meshes[i].id[1], m_meshes[i].id[2], m_meshes[i].id[3]);
		printDebug("load", "mesh[%i].name = '%s'\n", i, 
				 m_meshes[i].name);
		printDebug("load", "mesh[%i].flags = %i\n", i, 
				 m_meshes[i].flags);
		printDebug("load", "mesh[%i].num_frames = %i\n", i, 
				 m_meshes[i].num_frames);
		printDebug("load", "mesh[%i].num_shaders = %i\n", i, 
				 m_meshes[i].num_shaders);
		printDebug("load", "mesh[%i].num_vertices = %i\n", i, 
				 m_meshes[i].num_vertices);
		printDebug("load", "mesh[%i].num_triangles = %i\n", i, 
				 m_meshes[i].num_triangles);
		printDebug("load", "mesh[%i].tris_offset = %i\n", i, 
				 m_meshes[i].tris_offset+m_surfaces_start);
		printDebug("load", "mesh[%i].shader_offset = %i\n", i, 
				 m_meshes[i].header_size+m_surfaces_start);
		printDebug("load", "mesh[%i].texel_offset = %i\n", i, 
				 m_meshes[i].texel_offset+m_surfaces_start);
		printDebug("load", "mesh[%i].vertex_offset = %i\n", i, 
				 m_meshes[i].vertex_offset+m_surfaces_start);
		printDebug("load", "mesh[%i].mesh_end = %i\n", i, 
				 m_meshes[i].mesh_size+m_surfaces_start);

		// This will kind of handle bad mesh reads here
		if (*(int*)&m_meshes[i].id != MD3_IDALIASHEADER)
		{
			printWarning("load", "Bad/damaged mesh, handling...\n");
			printWarning("load", "HANDLE: Keeping valid meshes... %i/%i total\n",
							 i, m_num_meshes);

			m_num_meshes = i; // don't use more than prev valid meshes
			break;
		}

		// End Mesh Header ///////////////////

		m_meshes[i].skin = NULL;
		m_meshes[i].tris = NULL;
		m_meshes[i].texel = NULL;
		m_meshes[i].vertex = NULL;

		// Start shaders /////////////////////
		if (m_surfaces_start+m_meshes[i].header_size != ftell(f))
		{
			printWarning("load", "_mesh[%i] shader offset %i != file pos %lu\n",
							 i, m_surfaces_start+m_meshes[i].header_size, ftell(f));
      
			fseek(f, m_surfaces_start+m_meshes[i].header_size, SEEK_SET);
			printWarning("load", "HANDLE: File position set to %lu\n", ftell(f));
		}

		m_meshes[i].skin = new md3_skin_t[m_meshes[i].num_skins];

		dump_skin = false;

		for (j = 0; j < m_meshes[i].num_skins; ++j)
		{
			fread(&m_meshes[i].skin[j].name, 68, 1, f);

			if (!m_meshes[i].skin[j].name[0])
			{
				m_meshes[i].skin[j].name[0] = 'm';

				printWarning("load", "Error in skin name %i/%i.\n",
								 j+1, m_meshes[i].num_skins);

				dump_skin = true;
			}

#ifdef UNIT_TEST_MD3_LV2
			dump_skin = true;
#endif

			if (dump_skin)
			{
				printDebug("load", "Dumping skin buffer:\n'");

				for (k = 0; k < 68; ++k)
				{
					if (m_meshes[i].skin[j].name[k] > 32 && 
						 m_meshes[i].skin[j].name[k] < 127)
					{
						printf("%c", m_meshes[i].skin[j].name[k]);
					}
					else
					{
						printf("%c", 183); // print a dot for invalid text
					}
				}

				printf("'\n");
			}

			printDebug("load", "mesh[%i].skin[%i].name = '%s'\n", 
					 i, j, m_meshes[i].skin[j].name);
		}

		// Start triangles ////////////////////////
		if (m_surfaces_start+m_meshes[i].tris_offset != ftell(f))
		{
			printWarning("load", "mesh[%i] tris offset %i != file pos %lu\n", i, 
							 m_surfaces_start+m_meshes[i].tris_offset, ftell(f));
      
			fseek(f, m_surfaces_start+m_meshes[i].tris_offset, SEEK_SET);
			printWarning("load", "HANDLE: File position set to %lu\n", ftell(f));
		}

		m_meshes[i].tris = new md3_tri_index_t[m_meshes[i].num_triangles];

		for (j = 0; j < m_meshes[i].num_triangles; ++j)
		{
			fread(&m_meshes[i].tris[j].triangle, 12, 1, f);
		}

		// Start texels /////////////////////
		if (m_surfaces_start+m_meshes[i].texel_offset != ftell(f))
		{
			printWarning("load", "mesh[%i] texel offset %i != file pos %lu\n", i, 
							 m_surfaces_start+m_meshes[i].texel_offset, ftell(f));
      
			fseek(f, m_surfaces_start+m_meshes[i].texel_offset, SEEK_SET);
			printWarning("load", "HANDLE: File position set to %lu\n", ftell(f));
		}

		m_meshes[i].texel = new md3_texel_t[m_meshes[i].num_vertices];

		for (j = 0; j < m_meshes[i].num_vertices; j++)
		{
			fread(&m_meshes[i].texel[j].st[0], 4, 1, f);
			fread(&m_meshes[i].texel[j].st[1], 4, 1, f);
		}

		// Start vertices /////////////////////
		if (m_surfaces_start+m_meshes[i].vertex_offset != ftell(f))
		{
			printWarning("load", "mesh[%i] vertex offset %i != file pos %lu\n", 
							 i, m_surfaces_start+m_meshes[i].vertex_offset, ftell(f));
      
			fseek(f, m_surfaces_start+m_meshes[i].vertex_offset, SEEK_SET);
			printWarning("load", "HANDLE: File position set to %lu\n", ftell(f));
		}

		// Should be num_vertices * num_frames??
		m_meshes[i].vertex = 
      new md3_vertex_t[m_meshes[i].num_vertices * m_meshes[i].num_frames];

		for (j = 0; j < m_meshes[i].num_vertices * m_meshes[i].num_frames; ++j)
		{
			fread(&ss, 2, 1, f);
			m_meshes[i].vertex[j].pos[0] = ss;
			fread(&ss, 2, 1, f);
			m_meshes[i].vertex[j].pos[1] = ss;
			fread(&ss, 2, 1, f);
			m_meshes[i].vertex[j].pos[2] = ss;

			fread(&sb, 1, 1, f);
			m_meshes[i].vertex[j].st[0] = sb;

			fread(&sb, 1, 1, f);
			m_meshes[i].vertex[j].st[1] = sb;

			if (mFlags & fDecodeNormals)
			{
				//unsigned short normal;
				//unsigned char lat, lng;
				float lat, lng;

				// Method 1.
				//normal = (unsigned short)m_meshes[i].vertex[j].st;
				//lat = (unsigned char)((normal & 255);
				//lng = (unsigned char)(((normal >> 8) & 255);

				// Method 2.
				//lat = m_meshes[i].vertex[j].st[0];
				//lng = m_meshes[i].vertex[j].st[1];
				// lat *= (2 * M_PI) / 255.0
				// lag *= (2 * M_PI) / 255.0

				// Method 3.
				lat = (m_meshes[i].vertex[j].st[0] * 2 * M_PI) / 255.0;
				lng = (m_meshes[i].vertex[j].st[1] * 2 * M_PI) / 255.0;
				
				m_meshes[i].vertex[j].norm[0] = cos(lat) * sin(lng);
				m_meshes[i].vertex[j].norm[1] = sin(lat) * sin(lng);
				m_meshes[i].vertex[j].norm[2] = cos(lng);

				printDebug("load", "%f %f %f\n", 
							  m_meshes[i].vertex[j].norm[0],
							  m_meshes[i].vertex[j].norm[1],
							  m_meshes[i].vertex[j].norm[2]);
			}
		}

		// Start next surface/mesh /////////////////////
		if (m_surfaces_start+m_meshes[i].mesh_size != ftell(f))
		{
			printWarning("load", "_mesh[%i] next mesh offset %i != file pos %lu\n", 
						  i, m_surfaces_start+m_meshes[i].mesh_size, ftell(f));
      
			fseek(f, m_surfaces_start+m_meshes[i].mesh_size, SEEK_SET);
			printWarning("load", "HANDLE: File position set to %lu\n", ftell(f));
		}

		// Now acculate this mesh offset
		m_surfaces_start = ftell(f);
	}

	fclose(f);

	return 0;
}


int Md3::save(char *filename)
{
	FILE *f;
	int i, ii, tmp, hms;
	int32_t si;
  

	if ((!m_tags && m_num_tags) || (!m_bones && m_num_bones) || 
		(!m_meshes && m_num_meshes))
	{
		printError("save", "invalid md3\n");
		return -1;
	}

	f = fopen(filename, "wb");

	if (!f)
	{
		perror("Md3::Save> \n");
		return -1;
	}

	// Start Header ////////////////
	si = m_id = MD3_IDALIASHEADER;
	fwrite(&si, 4, 1, f);
	printDebug("save", "id = 0x%x\n", m_id);
  
	m_version = MD3_ALIAS_VERSION;

	fwrite(&m_version, 4, 1, f);
	printDebug("save", "version = %i\n", m_version);

	fwrite(&m_filename, 68, 1, f);
	printDebug("save", "filename = '%s'\n", m_filename);

	fwrite(&m_num_bones, 4, 1, f);
	printDebug("save", "num_bones = %i\n", m_num_bones);

	fwrite(&m_num_tags, 4, 1, f);
	printDebug("save", "num_tags = %i\n", m_num_tags);

	fwrite(&m_num_meshes, 4, 1, f);
	printDebug("save", "num_meshes = %i\n", m_num_meshes);

	fwrite(&m_max_skins, 4, 1, f);
	printDebug("save", "max_skins = %i\n", m_max_skins);

	// Seek back and write in actual value later
	// store file postion for now
	m_header_length = ftell(f);
	fwrite(&m_header_length, 4, 1, f);

	// Seek back and write in actual value later
	// store file postion for now
	m_tag_start = ftell(f);
	fwrite(&m_tag_start, 4, 1, f);

	// Seek back and write in actual value later
	// store file postion for now
	m_surfaces_start = ftell(f);
	fwrite(&m_surfaces_start, 4, 1, f);

	// Seek back and write in actual value later
	// store file postiion for now
	m_file_size = ftell(f);
	fwrite(&m_file_size, 4, 1, f);
 
	// End Header //////////////////
	tmp = m_header_length;
	m_header_length = ftell(f);
	fseek(f, tmp, SEEK_SET);
	fwrite(&m_header_length, 4, 1, f);
	printDebug("save", "header_length = %i\n", m_header_length);
	fseek(f, m_header_length, SEEK_SET);

	for (i = 0; i < m_num_bones; ++i)
	{
		fwrite(&m_bones[i].mins, sizeof(m_bones[i].mins), 1, f);
		fwrite(&m_bones[i].maxs, sizeof(m_bones[i].maxs), 1, f);
		fwrite(&m_bones[i].center, sizeof(m_bones[i].center), 1, f);
		fwrite(&m_bones[i].scale, sizeof(float), 1, f);
		fwrite(&m_bones[i].creator, 16, 1, f);

		printDebug("save", "bone[%i].creator = '%s'\n", i, m_bones[i].creator);
	}

	printDebug("save", "Saving %i tags\n", m_num_tags * m_num_bones);

  // Start Tags ////////////////
  tmp = m_tag_start;
  m_tag_start = ftell(f);
  fseek(f, tmp, SEEK_SET);
  fwrite(&m_tag_start, 4, 1, f);
  printDebug("save", "tag_start = %i\n", m_tag_start);
  fseek(f, m_tag_start, SEEK_SET);

  for (i = 0; i < m_num_tags * m_num_bones; ++i)
  {
    fwrite(&m_tags[i].name, 64, 1, f);
    fwrite(&m_tags[i].center, sizeof(m_tags[i].center), 1, f);
    fwrite(&m_tags[i].rotation, sizeof(m_tags[i].rotation), 1, f);

    printDebug("save", "tag[%i].name = '%s'\n", i, m_tags[i].name);
  }

  // Start Geometeric data ////////////////
  tmp = m_surfaces_start;
  m_surfaces_start = ftell(f);
  fseek(f, tmp, SEEK_SET);
  fwrite(&m_surfaces_start, 4, 1, f);
  printDebug("save", "surfaces_start = %i\n", m_surfaces_start);
  fseek(f, m_surfaces_start, SEEK_SET);

  for (i = 0; i < m_num_meshes; i++)
  {
    // Start Mesh Header /////////////////
    hms = ftell(f);

	strncpy(m_meshes[i].id, "IDP3", 4);

    fwrite(&m_meshes[i].id, 4, 1, f);
    fwrite(&m_meshes[i].name, 68, 1, f);
    fwrite(&m_meshes[i].num_frames, 4, 1, f);
    fwrite(&m_meshes[i].num_skins, 4, 1, f);
    fwrite(&m_meshes[i].num_vertices, 4, 1, f);
    fwrite(&m_meshes[i].num_triangles, 4, 1, f);

    m_meshes[i].tris_offset = ftell(f);
    fwrite(&m_meshes[i].tris_offset, 4, 1, f);

    m_meshes[i].header_size = ftell(f);
    fwrite(&m_meshes[i].header_size, 4, 1, f);

    m_meshes[i].texel_offset = ftell(f);
    fwrite(&m_meshes[i].texel_offset, 4, 1, f);

    m_meshes[i].vertex_offset = ftell(f);
    fwrite(&m_meshes[i].vertex_offset, 4, 1, f);

    m_meshes[i].mesh_size = ftell(f);
    fwrite(&m_meshes[i].mesh_size, 4, 1, f);

    printDebug("save", "_mesh[%i].name = '%s'\n", i, m_meshes[i].name);

    // End Mesh Header ///////////////////
    tmp = ftell(f);
    fseek(f, m_meshes[i].header_size, SEEK_SET);
    m_meshes[i].header_size = tmp - hms;
    fwrite(&m_meshes[i].header_size, 4, 1, f);
    printDebug("save", "_mesh[%i].header_size = %i\n",i,m_meshes[i].header_size);
    fseek(f, tmp, SEEK_SET);

    for(ii = 0; ii < m_meshes[i].num_skins; ii++)
    {
      fwrite(&m_meshes[i].skin[ii].name, 68, 1, f);

      if (!m_meshes[i].skin[ii].name[0])
	m_meshes[i].skin[ii].name[0] = 'm';

#ifdef TEST_MD3
      printf("Viewing full skin buffer:\n'");
		
      for (int foo = 0; foo < 68; ++foo)
	printf("%c", m_meshes[i].skin[ii].name[foo]);

      printf("'\n");
#endif

      printDebug("save", "_mesh[%i].skin[%i].name = '%s'\n", 
	     i, ii, m_meshes[i].skin[ii].name);
    }

    // Tris Start //////////////
    tmp = ftell(f);
    fseek(f, m_meshes[i].tris_offset, SEEK_SET);
    m_meshes[i].tris_offset = tmp - hms;
    fwrite(&m_meshes[i].tris_offset, 4, 1, f);
    printDebug("save", "_mesh[%i].tris_offset = %i\n",i,m_meshes[i].tris_offset);
    fseek(f, tmp, SEEK_SET);   

    for(ii = 0; ii < m_meshes[i].num_triangles; ++ii)
    {
      fwrite(&m_meshes[i].tris[ii].triangle, 12, 1, f);
    }

    // Texel Start //////////////
    tmp = ftell(f);
    fseek(f, m_meshes[i].texel_offset, SEEK_SET);
    m_meshes[i].texel_offset = tmp - hms;
    fwrite(&m_meshes[i].texel_offset, 4, 1, f);
    printDebug("save", "_mesh[%i].texel_offset = %i\n",
	   i,m_meshes[i].texel_offset);
    fseek(f, tmp, SEEK_SET);   

    for (ii = 0; ii < m_meshes[i].num_vertices; ++ii)
    {
		fwrite(&m_meshes[i].texel[ii].st, 8, 1, f);
    }

    // Vertex Start //////////////
    tmp = ftell(f);
    fseek(f, m_meshes[i].vertex_offset, SEEK_SET);
    m_meshes[i].vertex_offset = tmp - hms;
    fwrite(&m_meshes[i].vertex_offset, 4, 1, f);
    printDebug("save", "_mesh[%i]._vertexoffset = %i\n",
	   i,m_meshes[i].vertex_offset);
    fseek(f, tmp, SEEK_SET);  

    // Should be num_vertices * num_frames??
    for(ii = 0; ii < m_meshes[i].num_vertices * m_meshes[i].num_frames; ++ii)
    {
      fwrite(&m_meshes[i].vertex[ii].pos, sizeof(m_meshes[i].vertex[ii].pos), 1, f);
      fwrite(&m_meshes[i].vertex[ii].st, sizeof(m_meshes[i].vertex[ii].st), 1, f);
    }
    // End Mesh  ///////////////////
    tmp = ftell(f);
    fseek(f, m_meshes[i].mesh_size, SEEK_SET);
    m_meshes[i].mesh_size = tmp - hms;
    fwrite(&m_meshes[i].mesh_size, 4, 1, f);
    printDebug("save", "_mesh[%i].mesh_size = %i\n",i,m_meshes[i].mesh_size);
    fseek(f, tmp, SEEK_SET);

  }

  // End File /////////////
  tmp = m_file_size;
  m_file_size = ftell(f);
  fseek(f, tmp, SEEK_SET);
  fwrite(&m_file_size, 4, 1, f);
  printDebug("save", "file_size = %i\n", m_file_size);
  fseek(f, m_file_size, SEEK_SET);

  fclose(f);

  return 0;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void Md3::printDebug(char *method, char *s, ...)
{
	va_list args;


	if (m_debug < 2)
	{
		return;
	}

	va_start(args, s);
	fprintf(stdout, "Md3::%s> ", method);
	vfprintf(stdout, s, args);
	fprintf(stdout, "\n");
	va_end(args);
}


void Md3::printError(char *method, char *s, ...)
{
	va_list args;


	va_start(args, s);
	fprintf(stderr, "Md3::%s> ERROR: ", method);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


void Md3::printWarning(char *method, char *s, ...)
{
	va_list args;


	if (m_debug < 1)
	{
		return;
	}

	va_start(args, s);
	fprintf(stderr, "Md3::%s> WARNING: ", method);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


void Md3::createMeshes(unsigned int num)
{
	m_meshes = new md3_mesh_t[num];

	texTest = new int[num];
	memset(texTest, 0, num*4);	
}


void Md3::createTags(unsigned int num)
{
	m_tags = new md3_tag_t[num];

	slaveTest = new unsigned int[num];
	memset(slaveTest, 0, num*4);
}


void Md3::createBones(unsigned int num)
{
	m_bones = new md3_bone_t[num];
}



////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <string.h>
#include <stdio.h>
#include <hel/Matrix.h>
#include <freyja/PluginABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;

#include "Md3.h"



extern "C" {

	int freyja_model__md3_check(char *filename);
	int freyja_model__md3_import(char *filename);
	int freyja_model__md3_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("md3");
	freyjaPluginDescription1s("Quake3 Model (*.md3)");
	freyjaPluginAddExtention1s("*.md3");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_VERTEX_MORPHING);
}

int import_model(char *filename)
{
	if (!freyja_model__md3_check(filename))
		return freyja_model__md3_import(filename);

	return -1;
}


int freyja_model__md3_check(char *filename)
{
	FILE *f;
   char header[32];


   f = fopen(filename, "rb");

   if (!f)
   { 
	   //sys_errlist[errno]
	   perror("mdl_check> fopen failed");
	   return -1;
   }

   fread(&header, 32, 1, f);      
   fclose(f);

   header[4] = 0;

   // Quake3
   if (strcmp(header, "IDP3") == 0)
		return 0;

   return -2;
}


int freyja_model__md3_import(char *filename)
{
	const float scale = 0.01;
	unsigned int i, m;
	int v, p, f, off, num_texels;
	md3_mesh_t *md3_mesh;
	md3_bone_t *md3_bone;
	md3_tag_t *md3_tag;
	float s, t;
	//Matrix matrix;
	float pos[3];
	Md3 md3;
	Vector<long> transV;
	unsigned int vertex;


	if (md3.load(filename))
		return -1;

	md3_mesh = md3.getMeshes();
	md3_bone = md3.getBones();
	md3_tag = md3.getTags(); 

	for (m = 0; m < md3.getNumMeshes(); ++m)
	{
		freyjaBegin(FREYJA_MESH);

		/* 0.9.3 FREYJA_VERTEX_FRAME deprecated! */
		for (f = 0; f < 1/*md3_mesh[m].num_frames*/; ++f)
		{  
			freyjaPrintMessage("Importing mesh: %d, frame: %d of %d", 
							m, f, md3_mesh[m].num_frames);

			if (!f)
			{
				freyjaBegin(FREYJA_VERTEX_GROUP);  
			}
			else
			{
				freyjaBegin(FREYJA_VERTEX_FRAME);
			}

			off = f * md3_mesh[m].num_vertices;

			for (v = 0; v < md3_mesh[m].num_vertices; ++v)
			{
				pos[0] = md3_mesh[m].vertex[v+off].pos[0] * scale;
				pos[1] = md3_mesh[m].vertex[v+off].pos[1] * scale;
				pos[2] = md3_mesh[m].vertex[v+off].pos[2] * scale;

				if (!f)
				{
					vertex = freyjaVertexCreate3f(pos[1], pos[2], pos[0]); // XZY
					freyjaVertexNormal3f(vertex, 
										 md3_mesh[m].vertex[v].norm[1], 
										 md3_mesh[m].vertex[v].norm[2], 
										 md3_mesh[m].vertex[v].norm[0]);
					transV.pushBack(vertex);
				}
				else
				{
					//freyjaVertexFrame3f(transV[v],
					//					pos[1], pos[2], pos[0]);
				}
			}

			freyjaGroupCenter3f(md3_bone[f].center[0]*scale, 
								md3_bone[f].center[1]*scale, 
								md3_bone[f].center[2]*scale);

			freyjaEnd(); // End FREYJA_GROUP
		}

		/* 0.9.5 vertex morph keyframes */
		index_t mesh = freyjaGetFSMMeshIndex();
		index_t track = freyjaMeshVertexTrackNew(mesh, 
			(md3_mesh[m].num_frames+3)/15.0f, 15.0f);

		for (f = 1; f < md3_mesh[m].num_frames; ++f)
		{  
			//freyjaPrintMessage("Importing mesh: %d, frame: %d of %d", 
			//				m, f, md3_mesh[m].num_frames);
			index_t key = freyjaMeshVertexKeyFrameNew(mesh, 
													  track, (vec_t)f/15.0f);
			off = f * md3_mesh[m].num_vertices;

			for (v = 0; v < md3_mesh[m].num_vertices; ++v)
			{
				pos[0] = md3_mesh[m].vertex[v+off].pos[0] * scale;
				pos[1] = md3_mesh[m].vertex[v+off].pos[1] * scale;
				pos[2] = md3_mesh[m].vertex[v+off].pos[2] * scale;

				freyjaMeshVertexKeyFrame3f(mesh, track, key,
										   transV[v],
										   pos[1], pos[2], pos[0]);
			}
		}

		/* End 0.9.5 vertex morph keyframes */


		num_texels = md3_mesh[m].num_vertices * md3_mesh[m].num_frames;

		for (p = 0; p < md3_mesh[m].num_triangles; p++)
		{
			// Start a new polygon
			freyjaBegin(FREYJA_POLYGON);

			for (i = 0; i < 3; i++)
			{
				// Store vertices by true id, using translation table
				freyjaPolygonVertex1i(transV[md3_mesh[m].tris[p].triangle[i]]);
	
				if (md3_mesh[m].tris[p].triangle[i] > num_texels)
				{
					printf("FreyjaModel::Md3Import> triangle[%i] texel overflow\n", p);
					s = 0.5;
					t = 0.5;
				}
				else
				{
					s = md3_mesh[m].texel[md3_mesh[m].tris[p].triangle[i]].st[0];
					t = md3_mesh[m].texel[md3_mesh[m].tris[p].triangle[i]].st[1];
				}

				freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(s, t));
			}

#ifdef FIXME
			// FIXME: Handle texture/shader loading
			freyjaShaderStore(md3_mesh[m].skin[s].name, ...);
#endif

			freyjaPolygonMaterial1i(0);
      
			// End FREYJA_POLYGON
			freyjaEnd(); 
		}

		// End FREYJA_MESH
		freyjaEnd();

		transV.clear();
	}

	return 0;
}


int freyja_model__md3_export(char *filename)
{
#ifdef MD3_EXPORT_ENABLED
	const float scale = 100.0f;
	const int x = 2, y = 0, z = 1;
	Vector<long> transM, transV;
	md3_mesh_t *mesh;
	//md3_bone_t *bone;
	//md3_tag_t *tag;
	Md3 md3;
	int32 num_meshes, num_frames;
	int32 i, j, k, v, f, vertex, idx, texcoord;
	index_t frameIndex;
	vec3_t xyz;
	vec2_t uv;
	vec_t lat, lng;


	num_meshes = freyjaGetCount(FREYJA_MESH);

	if (!num_meshes)
	{
		return -1;
	}

	// Don't allow use of internal iterators or
	// changes of data by other processes
	freyjaCriticalSectionLock();

	mesh = new md3_mesh_t[num_meshes];
	memset(mesh, 0, sizeof(md3_mesh_t) * num_meshes);
	md3.setMeshes(mesh, num_meshes);

	// Using list interface, as opposed to array
	freyjaIterator(FREYJA_MESH, FREYJA_RESET);

	for (k = 0; k < num_meshes; k++)
	{
		transM.pushBack(freyjaIterator(FREYJA_MESH, FREYJA_CURRENT));
		freyjaIterator(FREYJA_MESH, FREYJA_NEXT);

		// Don't write edges > 3, tesselate to triangles
		//freyjaMeshTesselation(transM[k], FREYJA_TRIANGLES); //let user control

		strcpy(mesh[k].name, "Freyja-test");
		mesh[k].num_frames = 0; 
		mesh[k].num_vertices = 0;
		mesh[k].num_skins = 0;
		mesh[k].num_triangles = freyjaGetMeshPolygonCount(transM[k]);
    
		/* Add 1 for non morphed vertices ( Would be frame[0] )
		 * Note all this code assumes TRIANGLES -- THREE EDGES! 3! 3! 3! */
		num_frames = (freyjaGetMeshVertexFrameCount(transM[k]) + 1);
		freyjaPrintMessage("Exporting %i frames...", num_frames);
		mesh[k].num_frames = num_frames;
		mesh[k].num_vertices = mesh[k].num_triangles * 3;
		mesh[k].vertex = new md3_vertex_t[mesh[k].num_vertices * num_frames];
		mesh[k].texel = new md3_texel_t[mesh[k].num_vertices * num_frames];
		mesh[k].tris = new md3_tri_index_t[mesh[k].num_triangles];

		// frame[0], also sets up mesh faces
		for (i = 0, v = 0; i < mesh[k].num_triangles; ++i)
		{
			idx = freyjaGetMeshPolygonIndex(transM[k], i);

			for (j = 0; j < 3; ++j, ++v)
			{
				vertex = freyjaGetPolygonVertexIndex(idx, j);

				// FIXME: May need bounds ckecking/scaling for short
				freyjaGetVertexXYZ3fv(vertex, xyz);
				mesh[k].vertex[v].pos[0] = (short)(xyz[x]*scale);
				mesh[k].vertex[v].pos[1] = (short)(xyz[y]*scale);
				mesh[k].vertex[v].pos[2] = (short)(xyz[z]*scale);

				// Normal isn't actually stored in md3's vertex.norm
				// it must be encoded into the 'vertex.st' normal
				freyjaGetVertexNormalXYZ3fv(vertex, xyz);
				mesh[k].vertex[v].norm[0] = (short)xyz[x];
				mesh[k].vertex[v].norm[1] = (short)xyz[y];
				mesh[k].vertex[v].norm[2] = (short)xyz[z];
				lng = acos(xyz[z]);
				lat = acos(xyz[x]/sin(lng)); 
				lat = (lat * 255.0) / (2 * M_PI);
				lng = (lng * 255.0) / (2 * M_PI);
				mesh[k].vertex[v].st[0] = (unsigned char)lat;  
				mesh[k].vertex[v].st[1] = (unsigned char)lng;

				// Modeler must use VertexUV, not polymapping for best results
				if (freyjaGetPolygonFlags(idx) & fPolygon_VertexUV)
				{
					freyjaGetVertexTexcoord2fv(vertex, uv);
					mesh[k].texel[v].st[0] = uv[0];
					mesh[k].texel[v].st[1] = uv[1];
				}
				else if (freyjaGetPolygonFlags(idx) & fPolygon_PolyMappedUV)
				{
					// This will make 'interesting' UVMaps in some cases
					// You can't fit more UVs than Vertices in md3!
					texcoord = freyjaGetPolygonTexCoordIndex(idx, j);
					freyjaGetTexCoord2fv(texcoord, uv);
					mesh[k].texel[v].st[0] = uv[0];
					mesh[k].texel[v].st[1] = uv[1];
				}
				else // Color info only?  Grab the vertex uv anyway then
				{
					freyjaGetVertexTexcoord2fv(vertex, uv);
					mesh[k].texel[v].st[0] = uv[0];
					mesh[k].texel[v].st[1] = uv[1];
				}

				transV.pushBack(vertex); // transV[v] -> vertex
				mesh[k].tris[i].triangle[j] = v;
			}
		}


		// frame[1..n], WARNING Uses v from last loop
		for (f = 1; f < mesh[k].num_frames; ++f)
		{

			freyjaPrintMessage("Exporting frame %li/%i...",
							   f, mesh[k].num_frames);

			for (i = 0; i < mesh[k].num_triangles; ++i)
			{
				idx = freyjaGetMeshPolygonIndex(transM[k], i);

				for (j = 0; j < 3; ++j, ++v)
				{
					vertex = freyjaGetPolygonVertexIndex(idx, j);

					// FIXME: May need bounds ckecking/scaling for short
					freyjaGetVertexFrame(vertex, f-1, &frameIndex, xyz);
					mesh[k].vertex[v].pos[0] = (short)(xyz[x]*scale);
					mesh[k].vertex[v].pos[1] = (short)(xyz[y]*scale);
					mesh[k].vertex[v].pos[2] = (short)(xyz[z]*scale);

					// Normal isn't actually stored in md3's vertex.norm
					// it must be encoded into the 'vertex.st' normal
					freyjaGetVertexNormalXYZ3fv(vertex, xyz);
					mesh[k].vertex[v].norm[0] = (short)xyz[x];
					mesh[k].vertex[v].norm[1] = (short)xyz[y];
					mesh[k].vertex[v].norm[2] = (short)xyz[z];
					lng = acos(xyz[z]);
					lat = acos(xyz[x]/sin(lng)); 
					lat = (lat * 255.0) / (2 * M_PI);
					lng = (lng * 255.0) / (2 * M_PI);
					mesh[k].vertex[v].st[0] = (unsigned char)lat;  
					mesh[k].vertex[v].st[1] = (unsigned char)lng;

					// Use VertexUV, not polymapping for best results
					if (freyjaGetPolygonFlags(idx) & fPolygon_VertexUV)
					{
						freyjaGetVertexTexcoord2fv(vertex, uv);
						mesh[k].texel[v].st[0] = uv[0];
						mesh[k].texel[v].st[1] = uv[1];
					}
					else if (freyjaGetPolygonFlags(idx) & fPolygon_PolyMappedUV)
					{
						// This will make 'interesting' UVMaps in some cases
						// You can't fit more UVs than Vertices in md3!
						texcoord = freyjaGetPolygonTexCoordIndex(idx, j);
						freyjaGetTexCoord2fv(texcoord, uv);
						mesh[k].texel[v].st[0] = uv[0];
						mesh[k].texel[v].st[1] = uv[1];
					}
					else // Color info only?  Grab the vertex uv anyway then
					{
						freyjaGetVertexTexcoord2fv(vertex, uv);
						mesh[k].texel[v].st[0] = uv[0];
						mesh[k].texel[v].st[1] = uv[1];
					}

					transV.pushBack(vertex); // transV[v] -> vertex
				}
			}		
		}

		//FIXME: add skin info here
		mesh[k].skin = NULL;
	}


#ifdef OLD_OLD_PLUGIN_DEAD_DEAD_DEAD
   // Bone frames ////////////////////////
   num_bone_frames = 0;

   if (num_bone_frames)
   {
		bone = new md3_boneframe_t[num_bone_frames];
		memset(bone, 0, sizeof(md3_boneframe_t) * num_bone_frames);

		md3.setBones(bone, num_bone_frames);

		//FIXME: Add converted bone frames from freyja to bone
   }
   else
   {
	   md3.setBones(NULL, 0);
   }

   // Bone tags aka Bolt-ons ////////////////////////
   num_tags = 0;

   if (num_tags)
   {
		tag = new md3_tag_t[num_tags * num_bone_frames];
		memset(tag, 0, sizeof(md3_tag_t) * num_tags * num_bone_frames);

		md3.setTags(tag, num_tags * num_bone_frames);

		//FIXME: Add converted bone tags from freyja to bone
   }
   else
   {
	   md3.setTags(NULL, 0);
   }
#endif

   freyjaCriticalSectionUnlock();

   return md3.save(filename);
#else
	printf("md3_export> Not enabled in this build\n");
	return -1;
#endif
}


#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MD3
int main(int argc, char *argv[])
{
	Md3 md3;


	printf("[MD3 class test]\n");
	
	md3.setDebug(2);

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!md3.load(argv[2]))
			{
				printf("main: Load reports success.\n");
			}
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			// FIXME: Add code here to generate a small md3

			if (!md3.save(argv[2]))
			{
				printf("main: Save reports success.\n");
			}
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			if (!md3.load(argv[2]))
			{
				printf("main: Load reports success.\n");
			}

			if (!md3.save(argv[3]))
			{
				printf("main: Save reports success.\n");
			}
		}
		else
		{
			printf("\n\n%s [save | load | test] filename.md3 [testout.md3]\n", 
					 argv[0]);
		}
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.md3 [testout.md3]\n", 
				 argv[0]);
	}
}
#endif

