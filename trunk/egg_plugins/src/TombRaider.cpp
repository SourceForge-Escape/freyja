/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : OpenRaider
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : OpenRaider
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: Loads TR 1, 2, 3, and 4 maps, meshes, and textures
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.05.21:
 * Mongoose - Added to project OpenRaider, more documentation
 *            than Freyja version I wrote ( 3d modeler )
 *
 *
 * 2000-05-13:
 * Mongoose - Added gcc and VC++ pragmas for packing
 *
 *            id style typedefs for structs
 *
 *            Heavy clean up and ported to C++
 *
 *            I saved yuri's notes as best I could and 
 *            reformatted and corected as needed
 *
 * Mongoose - Created, based on:
 *                tr_view's tr2io.c by Yuri Zhivago, PhD,
 *                TR Rosetta Stone ( TombRaider pak format specs )
 ================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "TombRaider.h"

#ifdef ZLIB_SUPPORT
#   include "zlib.h"
#endif

#ifdef __TEST_TR5_DUMP_TGA
#   include "mtk_tga.h"
#endif

#ifdef DEBUG_MEMEORY
#   include "memeory_test.h"
#endif



TombRaider::TombRaider()
{
  _textile8 = NULL;
  _textile16 = NULL;
  _textile32 = NULL;
  _tex_special = NULL;
  _rooms = NULL;
  _floor_data = NULL;
  _meshes = NULL;                
  _animations = NULL;       
  _state_changes = NULL; 
  _anim_dispatches = NULL;
  _anim_commands = NULL;  
  _mesh_trees = NULL;     
  _frames = NULL;        
  _moveables = NULL;      
  _static_meshes = NULL; 
  _object_textures = NULL;  
  _sprite_textures = NULL;   
  _sprite_sequences = NULL;  
  _cameras = NULL;
  _sound_sources = NULL;
  _boxes = NULL;   
  _overlaps = NULL;  
  _zones = NULL;      
  _animated_textures = NULL; 
  _items = NULL;         
  _light_map = NULL; 
  _cinematic_frames = NULL;
  _demo_data = NULL;              
  _sound_map = NULL;                      
  _sound_details = NULL;     
  _sample_indices = NULL;
  _debug_on = false;
  _compressed_level_data = NULL;
  _clear = false;

  Clear();
}

TombRaider::~TombRaider()
{
	Clear();
}

void TombRaider::Clear()
{
	unsigned int i;


	// Mongoose 2002.01.02, Some memory bug requires this
	if (_clear)
	{
		return;
	}

  // FIXME: Palettes aren't the same size anymore
  //memset(_palette8, 0, 256);
  //memset(_palette16, 0, 256);

  if (_anim_dispatches)
    delete [] _anim_dispatches;

  if (_anim_commands)
    delete [] _anim_commands;

  if (_mesh_trees)
    delete [] _mesh_trees;

  if (_frames)
    delete [] _frames;

  if (_moveables)
    delete [] _moveables;       

  if (_static_meshes)
    delete [] _static_meshes; 

  if (_object_textures)
    delete [] _object_textures; 

  if (_sprite_textures)
    delete [] _sprite_textures; 

  if (_sprite_sequences)
    delete [] _sprite_sequences; 

  if (_cameras)
    delete [] _cameras; 

  if (_sound_sources)
    delete [] _sound_sources;

  if (_boxes)
    delete [] _boxes;

  if (_overlaps)
    delete [] _overlaps;

  if (_zones)
    delete [] _zones;

  if (_animated_textures)
    delete [] _animated_textures;

  if (_items)
    delete [] _items;

  if (_light_map)
    delete [] _light_map;

  if (_cinematic_frames)
    delete [] _cinematic_frames;

  if (_demo_data)
    delete [] _demo_data;

  if (_sound_map)
    delete [] _sound_map;

  if (_sound_details)
    delete [] _sound_details;

  if (_sample_indices)
    delete [] _sample_indices;

  if (_rooms)
  {
	  for (i = 0; i < _num_rooms; ++i)
	  {
		  if (_rooms[i].room_data.num_vertices > 0)
			  delete [] _rooms[i].room_data.vertices;

		  if (_rooms[i].room_data.num_rectangles > 0)
			  delete [] _rooms[i].room_data.rectangles;

		  if (_rooms[i].room_data.num_triangles > 0) 
			  delete [] _rooms[i].room_data.triangles;

		  if (_rooms[i].room_data.num_sprites > 0)
			  delete [] _rooms[i].room_data.sprites;

		  if (_rooms[i].num_portals > 0)
			  delete []_rooms[i].portals;

		  if (_rooms[i].sector_list)
			  delete [] _rooms[i].sector_list;

		  if (_rooms[i].lights)
			  delete [] _rooms[i].lights;

		  if (_rooms[i].static_meshes)
			  delete [] _rooms[i].static_meshes;
	  }

	  delete [] _rooms;
  }

  if (_floor_data)
  {
    delete [] _floor_data;
  }

  if (_meshes)
  {
	  for (i = 0; (int)i < _num_meshes; ++i)
	  {
		  if (_meshes[i].vertices)
			 delete [] _meshes[i].vertices;

		  if (_meshes[i].mesh_lights)
			  delete [] _meshes[i].mesh_lights;

		  if (_meshes[i].normals)
			  delete [] _meshes[i].normals;

		  if (_meshes[i].textured_rectangles)
			  delete [] _meshes[i].textured_rectangles;

		  if (_meshes[i].textured_triangles)
			  delete [] _meshes[i].textured_triangles;

		  if (_meshes[i].coloured_rectangles)
			  delete [] _meshes[i].coloured_rectangles;

		  if (_meshes[i].coloured_triangles)
			  delete [] _meshes[i].coloured_triangles;
	  }
	  
    delete [] _meshes;
  }

  if (_animations)
    delete [] _animations;

  if (_state_changes)
	  delete _state_changes;
	  //delete [] _state_changes;

  // Texture use
  if (_tex_special)
    delete [] _tex_special;

  if (_textile8)
    delete [] _textile8;

  if (_textile16)
    delete [] _textile16;

  if (_textile32)
    delete [] _textile32;

  // FIXME: Damaged memory causes delete to not set pointer to NULL?
  _tex_special = NULL;
  _textile8 = NULL;
  _textile16 = NULL;
  _textile32 = NULL;

  // Compressed level use
  if (_compressed_level_data)
	  delete [] _compressed_level_data;

  _compressed_level_data_offset  = 0;
  _compressed_level_size = 0;
  _fread_mode = TR_FREAD_NORMAL;

  // Clear out vars
  _version = 0;
  _engine_version = TR_VERSION_UNKNOWN;
  _num_floor_data = 0;
  _num_textiles = 0;
  _num_tex_special = 0;
  _num_room_textures = 0;
  _num_misc_textures = 0;
  _num_bump_map_textures = 0;
  _unknown_t = 0;
  _num_rooms = 0;
  _num_anim_dispatches = 0;  
  _num_meshes = 0;
  _num_state_changes = 0;    
  _num_animations = 0;
  _num_anim_commands = 0;     
  _num_mesh_trees = 0;
  _num_frames = 0;   
  _num_moveables = 0;                
  _num_sample_indices = 0;
  _num_sound_details = 0;
  _num_demo_data = 0;
  _num_cinematic_frames = 0;
  _num_items = 0;            
  _num_animated_textures = 0;
  _num_cameras = 0;                         
  _num_sound_sources = 0;                   
  _num_boxes = 0;
  _num_static_meshes = 0; 
  _num_object_textures = 0;  
  _num_sprite_textures = 0;     
  _num_sprite_sequences = 0;   
  _num_overlaps = 0; 


  _clear = true;
}


void TombRaider::Print(char *s, ...)
{
    va_list args;

    va_start(args, s);
    fprintf(stderr, "TombRaider::");
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
}


void TombRaider::DebugPrint(char *s, ...)
{
  va_list args;

  if (!_debug_on)
    return;

  va_start(args, s);
  fprintf(stdout, "TombRaider::");
  vfprintf(stdout, s, args);
  fprintf(stdout, "\n");
  va_end(args);
}


int TombRaider::Fread(void *buffer, size_t size, size_t count, FILE *f)
{
#ifdef ZLIB_SUPPORT
   int num_read;


   if (_fread_mode == TR_FREAD_COMPRESSED)
   {
     num_read = count;
     num_read *= size;

     if ((_compressed_level_data_offset + num_read) <= _compressed_level_size) 
     {
       memcpy(buffer, &_compressed_level_data[_compressed_level_data_offset], 
				  num_read);

       _compressed_level_data_offset += num_read;
       
       return count;
     }
     else 
     {
       Print("Fread(%p, %d, %d, %p) returned %d",
				 buffer, size, count, f, num_read);
		 Clear();
       exit(2);
     }
   }
#endif

   if (fread(buffer, size, count, f) != count)
   {
      Print("Fread> Failed fread. Should Abort.");
      Clear();
      exit(2);
      return -1;
   }

   return count;
}


int TombRaider::NumMoveables()
{
	return _num_moveables;
}


int TombRaider::NumRooms()
{
  return _num_rooms;
}


int TombRaider::NumAnimations()
{
	return _num_animations;
}


unsigned int TombRaider::NumFrames()
{
	if (_num_frames < 0)
		return 0;

	return _num_frames;
}


int TombRaider::NumMeshes()
{
	return _num_meshes;
}


int TombRaider::NumStaticMeshes()
{
	return _num_static_meshes;
}


int TombRaider::NumSprites()
{
	return _num_sprite_textures;
}


int TombRaider::NumSpriteSequences()
{
	return _num_sprite_sequences;
}


int TombRaider::NumItems()
{
	return _num_items;
}


int TombRaider::NumTextures()
{
	//return _num_room_textures + _num_misc_textures + _num_bump_map_textures / 2;
	return _num_textiles - _num_bump_map_textures / 2;
}


tr2_room_t *TombRaider::Room()
{
  return _rooms;
}


tr2_item_t *TombRaider::Item()
{
  return _items;
}


tr2_object_texture_t *TombRaider::ObjectTextures()
{
  return _object_textures;
}


unsigned int TombRaider::getNumBoxes()
{
	return _num_boxes;
}


tr2_box_t *TombRaider::Box()
{
	return _boxes;
}


tr2_mesh_t *TombRaider::Mesh()
{
  /*
  if (n > 0 || n > _num_meshes)
    return NULL;

  return _meshes+n;
  */

  return _meshes;
}


int TombRaider::getNumAnimsForMoveable(int moveable_index)
{
	/***************************************************************************
    * It seems the number of animations isn't available in the moveable, 
	 * so we have to calculate it:
    *  - Get the "# starting anim" for the next moveable (->N)
    *  - Substract the "# starting anim" for moveable to N
	 *
    * Doing this, we assume that the next moveable has its animations following
    * the animations of the current moveable (seems right for all tested
    * levels, but...)
	 *
    * We also have to deal with the fact that the next moveable
	 * could have "# starting anim" == -1
	 * (ie. anim handled by the engine, like the ponytail anim). 
	 * If it's the case, we skip the moveable
    * and use the next moveable for our computation
	 *
	 * - Mongoose, Notes I edited from TRViewer
	 **************************************************************************/

	int start_anim;
	int next_start_anim;
	tr2_moveable_t *last_moveable = 0x0;
	tr2_moveable_t *moveable = 0x0;
	tr2_moveable_t *next_moveable = 0x0;


	if (moveable_index >= 0 && 
		 moveable_index <= (int)_num_moveables || _num_moveables < 1)
	{
		moveable = &_moveables[moveable_index];
	}

	if (!moveable)
	{
		return -1; // Was 0
	}

	last_moveable = &_moveables[moveable_index-1];

	start_anim = moveable->animation;

	while (moveable != last_moveable)
	{
		next_moveable = moveable + 1;
		next_start_anim = next_moveable->animation;
		 
		if (next_start_anim != 0xFFFF) 
			break;

		moveable = next_moveable++;
	}

	if (moveable == last_moveable)
	{
		next_start_anim = _num_animations;
	}

	return ((start_anim != 0xFFFF) ? next_start_anim - start_anim : 0);
}


void TombRaider::ColorLookUp(int index, float color[4])
{
	switch (Engine())
	{
	case TR_VERSION_1:
		color[0] = _palette8[index].r / 64.0;
		color[1] = _palette8[index].g / 64.0;
		color[2] = _palette8[index].b / 64.0;
		color[3] = 1.0;
		break;
	default:
		color[0] = (float)(_palette16[index] & 0xff) / 256.0;
		color[1] = (float)((_palette16[index] >> 8) & 0xff) / 256.0;
		color[2] = (float)((_palette16[index] >> 16) & 0xff) / 256.0;
		color[3] = 1.0;
	}	
}


tr2_staticmesh_t *TombRaider::StaticMesh()
{
	return _static_meshes;
}


tr2_version_type TombRaider::Engine()
{
  return _engine_version;
}


tr2_animation_t *TombRaider::Animation()
{
  return _animations;
}


unsigned short *TombRaider::Frame()
{
  return _frames;
}


tr2_moveable_t *TombRaider::Moveable()
{
  /*
  if (n > 0 || n > (int)_num_moveables)
    return NULL;
  */

  return _moveables;
}


tr2_meshtree_t *TombRaider::MeshTree()
{
  /*
  if (n > 0 || n > (int)_num_mesh_trees)
    return NULL;
  */

  return _mesh_trees;
}


tr2_sprite_texture_t *TombRaider::Sprite()
{
	return _sprite_textures;
}


tr2_sprite_sequence_t *TombRaider::SpriteSequence()
{
	return _sprite_sequences;
}

unsigned char *TombRaider::SpecialTexTile(int texture)
{
	unsigned char *image;
	unsigned char *ptr;


	image = NULL;

	if (texture >=0 && texture < NumSpecialTextures())
   {
		// Get base and offset into 32bit special textures/bump maps
		ptr = _tex_special;
		ptr += 256*256*4*texture;

		// Clone it as a single 256x256 @ 32bpp image
		image = new unsigned char[256*256*4];
		memcpy(image, ptr, 256*256*4);
	}

	return image;
}


int TombRaider::NumSpecialTextures()
{
	return _num_tex_special;
}


void TombRaider::Texture(int texture, unsigned char **image, 
								 unsigned char **bumpmap)
{
	int bumpmap_base = _num_room_textures + _num_misc_textures;


	*image = TexTile(texture);
	*bumpmap = NULL;

	if (_num_bump_map_textures && texture >= bumpmap_base)
	{
		*bumpmap = TexTile(texture + _num_bump_map_textures/2);
	}
}


unsigned int *TombRaider::Palette16()
{
  return _palette16;
}


unsigned char *TombRaider::Palette8()
{
  return (unsigned char *)_palette8;
}


int TombRaider::LoadTR5(FILE *f, void (*percent)(int))
{
	char s[16];
	unsigned short unknown_ushort, unknown_ushort2, unknown_ushort3;
	unsigned char unknown_uchar;
	unsigned int level_data_sz;
	unsigned int tr5_num_lights;
	unsigned int tr5_num_sectors;
	unsigned int tr5_num_portals;
	unsigned int tr5_num_unknown_36byte;
	unsigned int unknown_uint;
	int num_static_meshes, num_layers;
	int i, j;
	

   if (percent)
		(*percent)(5);

   DebugPrint("Load> _engine_version = 0x%x", _version);

   if (_engine_version != TR_VERSION_5)
     return -1;

#ifdef ZLIB_SUPPORT
	unsigned int sz, usz; // compressed and uncompressed size  
	unsigned char *compressed_data = NULL;
	int unknown_int, zerr;
	uLongf foo;
	
	
	// Read texture type offsets
	Fread(&_num_room_textures, 2, 1, f);
	DebugPrint("LoadTR5> _num_room_textures = %u", _num_room_textures);
	Fread(&_num_misc_textures, 2, 1, f);
	DebugPrint("LoadTR5> _num_misc_textures = %u", _num_misc_textures);
	Fread(&_num_bump_map_textures, 2, 1, f);
	DebugPrint("LoadTR5> _num_bump_map_textures  = %u", _num_bump_map_textures);

		
	// Read the sizes of the 32-bit textures
	Fread(&usz, sizeof(usz), 1, f);
	Fread(&sz, sizeof(sz), 1, f);

	DebugPrint("LoadTR5> 32-bit textures compressed size   = %u bytes", sz);
	DebugPrint("LoadTR5> 32-bit textures uncompressed size = %u bytes", usz);

	_num_textiles = usz / sizeof(tr2_textile32_t);

	DebugPrint("LoadTR5> _num_textiles = %i/%i = %i", 
				  usz, sizeof(tr2_textile32_t), _num_textiles);

	_textile32 = new tr2_textile32_t[_num_textiles];

	// Allocate a temporary buffer for decompression
	compressed_data = new unsigned char[sz];
	Fread(compressed_data, sz, 1, f);

	// Decompress the textures
	foo = usz;
	zerr = uncompress((unsigned char *)_textile32,
							&foo,
							compressed_data, 
							sz);
	usz = foo;

	DebugPrint("LoadTR5> textile decompress  [%s]", 
				  (zerr == Z_OK) ? "OK" : "ERROR");

	switch (zerr)
	{
	case Z_MEM_ERROR:
		DebugPrint("LoadTR5> There was not enough memory");
		break;
	case Z_BUF_ERROR:
		DebugPrint("LoadTR5> There was not enough room in the output buffer");
		break;
	case Z_DATA_ERROR:
		DebugPrint("LoadTR5> The input data was corrupted");
		break;
	default:
		DebugPrint("LoadTR5> textile decompress %i", zerr);
	}
     
	// Free the temporary buffer
	delete [] compressed_data;

	// Read in the 16-bit textures, set NumTextiles
	Fread(&usz, sizeof(usz), 1, f);
	Fread(&sz, sizeof(sz), 1, f);

	DebugPrint("LoadTR5> 16-bit textures compressed size   = %u bytes", sz);
	DebugPrint("LoadTR5> 16-bit textures uncompressed size = %u bytes", usz);
      
	_num_textiles = usz / sizeof(tr2_textile16_t);

	DebugPrint("LoadTR5> _num_textiles = %i/%i = %i", 
				  usz, sizeof(tr2_textile16_t), _num_textiles);

	_textile16 = new tr2_textile16_t[_num_textiles];

	// Allocate a temporary buffer for decompression
	compressed_data = new unsigned char[sz];
	Fread(compressed_data, sz, 1, f);

	// Decompress the textures
	foo = usz;
	zerr = uncompress((unsigned char *)_textile16,
							&foo,
							compressed_data, 
							sz);
	usz = foo;

   if (percent)
		(*percent)(7);

	DebugPrint("LoadTR5> textile decompress  [%s]", 
				  (zerr == Z_OK) ? "OK" : "ERROR");

	switch (zerr)
	{
	case Z_MEM_ERROR:
		DebugPrint("LoadTR5> There was not enough memory");
		break;
	case Z_BUF_ERROR:
		DebugPrint("LoadTR5> There was not enough room in the output buffer");
		break;
	case Z_DATA_ERROR:
		DebugPrint("LoadTR5> The input data was corrupted");
		break;
	default:
		DebugPrint("LoadTR5> textile decompress %i", zerr);
	}
     
	// Free the temporary buffer
	delete [] compressed_data;

	// Read the sizes of the sprite textures
	Fread(&usz, sizeof(usz), 1, f);
	Fread(&sz, sizeof(sz), 1, f);

	DebugPrint("LoadTR5> sprite textures compressed size   = %u bytes", sz);
	DebugPrint("LoadTR5> sprite textures uncompressed size = %u bytes", usz);

	// Load sprite/bump map/gui/etc textures also	
	_num_tex_special = usz/(256*256*4);

	DebugPrint("LoadTR5> _num_tex_special = %i/%i = %i", 
				  usz, 256*256*4, _num_tex_special);

	DebugPrint("LoadTR5> Reading %ibytes of sprite textures", usz);

	if (usz)
	{
		_tex_special = new unsigned char[usz];

		// Allocate a temporary buffer for decompression
		compressed_data = new unsigned char[sz];
		Fread(compressed_data, sz, 1, f);

		// Decompress the textures
		foo = usz;
		zerr = uncompress(_tex_special,
								&foo,
								compressed_data, 
								sz);
		usz = foo;

		DebugPrint("LoadTR5> special texture decompress  [%s]", 
					  (zerr == Z_OK) ? "OK" : "ERROR");

		switch (zerr)
		{
		case Z_MEM_ERROR:
			DebugPrint("LoadTR5> There was not enough memory");
			break;
		case Z_BUF_ERROR:
			DebugPrint("LoadTR5> There was not enough room in the output buffer");
			break;
		case Z_DATA_ERROR:
			DebugPrint("LoadTR5> The input data was corrupted");
			break;
		default:
			DebugPrint("LoadTR5> textile decompress %i", zerr);
		}
     
		// Free the temporary buffer
		delete [] compressed_data;		
	}
#else
	Print("LoadTR5> ERROR: TR5 support not compiled in this build.");
	Print("LoadTR5> Requires zlib-devel.");
	return -1;	  
#endif

   if (percent)
		(*percent)(10);


	// Mongoose 2002.01.08, Michiel has discovered the 
	//   first 4 bytes here are 2 bitu16 flags for Lara type and weather
	//   Seek past it and the rest which is unknown for now
	fseek(f, 32, SEEK_CUR);
	DebugPrint("LoadTR5> skipping 32bytes of unknown");

	Fread(&level_data_sz, 4, 1, f);
	DebugPrint("LoadTR5> Level data size = %u", level_data_sz);

	Fread(&unknown_uint, 4, 1, f);
	DebugPrint("LoadTR5> Same as last, also offset to RIFFs = %u", 
				  unknown_uint);

	Fread(&unknown_uint, 4, 1, f);
	DebugPrint("LoadTR5> unknown, but always 0? = %u", unknown_uint);

	Fread(&_num_rooms, 4, 1, f);
	DebugPrint("LoadTR5> _num_rooms = %u", _num_rooms);

	unsigned int room_sz;

	for (i = 0; i < _num_rooms; i++)
	{
		if (percent)
			(*percent)(11 + (int)(((float)i/(float)_num_rooms)*90.0));

		DebugPrint("LoadTR5> room[%i]", i); 

		Fread(s, 4, 1, f);
		s[4] = 0;
		DebugPrint("LoadTR5> XELA? = %s", s);  

		Fread(&room_sz, 4, 1, f);
		DebugPrint("LoadTR5> offset to next room = %u", room_sz);

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

		// Start 60byte struct /////////////
		DebugPrint("LoadTR5> 60byte struct {");

		// Often start of "XELA" +216 + ublock1 = FD end,
		// but have seen 0xffffffff (-1). Better using next data
		// and compute FD size the old way of X*Z*8
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> %u + 216 + XELA start = FD end", unknown_uint);

		// Start of "XELA" + 216 + ublock2 = FD start
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> %u + 216 + XELA start = FD start", unknown_uint);

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = %u", unknown_uint);

		// Possibly start of "XELA" + 216 + ublock4 = end portals
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> %u + 216 + XELA start = end portals", unknown_uint);
			  
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> room.x = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> room.z = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> room.y_bottom = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> room.y_top = %u", unknown_uint);

		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> num_z_sectors = %i", unknown_ushort);
		tr5_num_sectors = unknown_ushort;
		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> num_x_sectors = %i", unknown_ushort);
		tr5_num_sectors *= unknown_ushort;

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> room_ambient_color? = 0x%x", unknown_uint);

		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> num_lights = %i", unknown_ushort);
		Fread(&unknown_ushort, 2, 1, f);
		num_static_meshes = unknown_ushort;
		DebugPrint("LoadTR5> num_static_meshes = %i", num_static_meshes);

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown 0x0001? = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown 0x7fff? = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown 0x7fff? = 0x%x", unknown_uint);

		DebugPrint("LoadTR5> }");
		// End 60byte structure /////////////////

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

		// Start 20byte structure ///////////////
		DebugPrint("LoadTR5> 20byte struct {");
		Fread(s, 6, 1, f);
		DebugPrint("LoadTR5> 6 bytes 0xFF = 0x%x%x%x%x%x", 
					  s[0], s[1], s[2], s[3], s[4], s[5]);
		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> room_flag = %i", unknown_ushort);
		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> unknown = %i", unknown_ushort);
		Fread(s, 10, 1, f);
		DebugPrint("LoadTR5> 10 bytes 0x00 = 0x%x%x%x%x%x%x%x%x%x%x", 
					  s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9]);
		DebugPrint("LoadTR5> }");
		// End 20byte structure /////////////////

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

		DebugPrint("LoadTR5> 16byte struct {");
		Fread(s, 16, 1, f);
		DebugPrint("LoadTR5> unknown %x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x", 
					  s[0], s[1], s[2], s[3], s[4], s[5], s[6],
					  s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15]);
		DebugPrint("LoadTR5> }");


		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
	  
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = %u", unknown_uint);

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

		// 56byte struct /////////////
		DebugPrint("LoadTR5> 56byte struct {");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> num_triangles = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> num_rectangles = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown, 0x00? = 0x%x", unknown_uint);

		// Num lights * 88bytes
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> light_size = %u", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> num_lights = %u", unknown_uint);
		tr5_num_lights = unknown_uint;

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> num_unknown_36byte structs = %u", unknown_uint);
		tr5_num_unknown_36byte = unknown_uint;
		

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);

		Fread(&unknown_uint, 4, 1, f);
		num_layers = unknown_uint;
		DebugPrint("LoadTR5> num_layers = %u", num_layers);

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> unknown = 0x%x", unknown_uint);

		DebugPrint("LoadTR5> }");
		//////////////////////////////

		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");
		Fread(&unknown_uint, 4, 1, f);
		DebugPrint("LoadTR5> CDCDCDCD = 0x%x\t\t\t\t[%s]", unknown_uint, 
					  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

		// Lights
		DebugPrint("LoadTR5> Reading %u lights @ 88bytes each", tr5_num_lights);

		for (j = 0; j < (int)tr5_num_lights; j++)
		{
			Fread(&unknown_int, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  x? = %i | %f", 
						  j, unknown_int, *((float*)(&unknown_int)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  y? = %i | %f", 
						  j, unknown_int, *((float*)(&unknown_int)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  z? = %i | %f", j, 
						  unknown_int, *((float*)(&unknown_int)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  r? = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  g? = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown  b? = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			// Set offset to break
			unknown_uint = 0;

			if (unknown_uint)
			{
				fseek(f, unknown_uint, SEEK_CUR);
				DebugPrint("LoadTR5> skipping %ubytes of unknown", unknown_uint);
			}

			// 24bytes from start of light
			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> CDCDCDCD for some maps? = 0x%8x\t\t[%s]",
						  unknown_uint, 
						  (unknown_uint == 0xcdcdcdcd) ? "OK" : "ERROR");

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> light[%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			unknown_uint = 44;
			fseek(f, unknown_uint, SEEK_CUR);
			DebugPrint("LoadTR5> skipping %ubytes of unknown", unknown_uint);

			Fread(&unknown_uchar, 1, 1, f);
			DebugPrint("LoadTR5> light[%i].d3d_flag = 0x%x (%s)", 
						  j, unknown_uchar,
						  (unknown_uchar == 1) ? "Point" :
						  (unknown_uchar == 2) ? "Spot" : 
						  (unknown_uchar == 3) ? "Directional" : "Unknown");

			// 85bytes from start of light
			Fread(&unknown_uint, 3, 1, f);
			DebugPrint("LoadTR5> CDCDCD = 0x%x\t\t\t\t\t[%s]", unknown_uint, 
						  (unknown_uint == 0xcdcdcd) ? "OK" : "ERROR");
		}

		//Unknown 36byte structs
		for (j = 0; j < (int)tr5_num_unknown_36byte; j++)
		{
			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));	

			Fread(&unknown_uint, 4, 1, f);
			DebugPrint("LoadTR5> unknown_36byte_struct%i].unknown = 0x%x | %f", 
						  j, unknown_uint, *((float*)(&unknown_uint)));
		}

		// Sectors
		DebugPrint("LoadTR5> Reading %u sectors @ 8bytes each", 
					  tr5_num_sectors);

		for (j = 0; j < (int)tr5_num_sectors; j++)
		{
			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].fd_index = %u", j, unknown_ushort);
			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].box_index = %u", j, unknown_ushort);
			Fread(&unknown_uchar, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].room_below = %u", j, unknown_uchar);
			Fread(&unknown_uchar, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].floor = %i", j,  
						  (*(char*)(&unknown_uchar)));
			Fread(&unknown_uchar, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].room_above = %u", j, unknown_uchar);
			Fread(&unknown_uchar, 2, 1, f);
			DebugPrint("LoadTR5> sector[%i].ceiling = %i", j,
						  (*(char*)(&unknown_uchar)));
		}


		// Portals //////////////////////
		Fread(&unknown_ushort, 2, 1, f);
		tr5_num_portals = unknown_ushort;
		DebugPrint("LoadTR5> tr5_num_portals = %u", tr5_num_portals);

		//tr5_num_portals = 0;
		//DebugPrint("LoadTR5> OVERRIDE tr5_num_portals %i", tr5_num_portals);

		DebugPrint("LoadTR5> Reading %u portals @ 32bytes each", 
					  tr5_num_portals);

		for (j = 0; j < (int)tr5_num_portals; j++)
		{
			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].adjoining_room = %u", 
						  j, unknown_ushort);

			Fread(&unknown_ushort, 2, 1, f);
			Fread(&unknown_ushort2, 2, 1, f);
			Fread(&unknown_ushort3, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].normal = ( %i, %i, %i )", j, 
						  *((short*)(&unknown_ushort)),
						  *((short*)(&unknown_ushort2)),
						  *((short*)(&unknown_ushort3)));

			Fread(&unknown_ushort, 2, 1, f);
			Fread(&unknown_ushort2, 2, 1, f);
			Fread(&unknown_ushort3, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].vertex[0] = ( %i, %i, %i )", j, 
						  *((short*)(&unknown_ushort)),
						  *((short*)(&unknown_ushort2)),
						  *((short*)(&unknown_ushort3)));

			Fread(&unknown_ushort, 2, 1, f);
			Fread(&unknown_ushort2, 2, 1, f);
			Fread(&unknown_ushort3, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].vertex[1] = ( %i, %i, %i )", j, 
						  *((short*)(&unknown_ushort)),
						  *((short*)(&unknown_ushort2)),
						  *((short*)(&unknown_ushort3)));

			Fread(&unknown_ushort, 2, 1, f);
			Fread(&unknown_ushort2, 2, 1, f);
			Fread(&unknown_ushort3, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].vertex[2] = ( %i, %i, %i )", j, 
						  *((short*)(&unknown_ushort)),
						  *((short*)(&unknown_ushort2)),
						  *((short*)(&unknown_ushort3)));

			Fread(&unknown_ushort, 2, 1, f);
			Fread(&unknown_ushort2, 2, 1, f);
			Fread(&unknown_ushort3, 2, 1, f);
			DebugPrint("LoadTR5> portal[%i].vertex[3] = ( %i, %i, %i )", j, 
						  *((short*)(&unknown_ushort)),
						  *((short*)(&unknown_ushort2)),
						  *((short*)(&unknown_ushort3)));
		}

		Fread(&unknown_ushort, 2, 1, f);
		DebugPrint("LoadTR5> CDCD = 0x%x\t\t\t\t[%s]", unknown_ushort, 
					  (unknown_ushort == 0xcdcd) ? "OK" : "ERROR");

		// Static meshes
		for (j = 0; j < (int)tr5_num_portals; j++)
		{
			Fread(&unknown_int, 4, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].x = %i", j, unknown_int);
			Fread(&unknown_int, 4, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].y = %i", j, unknown_int);
			Fread(&unknown_int, 4, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].z = %i", j, unknown_int);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].rot = %i", j, unknown_ushort);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].intensity1 = %i", j, unknown_ushort);
			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].intensity2 = %i", j, unknown_ushort);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> static_mesh[%i].object_id = %i", j, unknown_ushort);
		}

		// Layers /////////////////
		for (j = 0; j < (int)num_layers; j++)
		{

			Fread(&unknown_int, 4, 1, f);
			DebugPrint("LoadTR5> layer[%i].num_vertices = %i", j, unknown_int);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> layer[%i].unknown = %i", j, unknown_ushort);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> layer[%i].num_rectangles = %i", j, unknown_ushort);

			Fread(&unknown_ushort, 2, 1, f);
			DebugPrint("LoadTR5> layer[%i].num_triangles = %i", j, unknown_ushort);

			// Set offset to break
			unknown_uint = 46;

			if (unknown_uint)
			{
				fseek(f, unknown_uint, SEEK_CUR);
				DebugPrint("LoadTR5> skipping %ubytes of unknown", unknown_uint);
			}
		}

		break; // blah abort after half way in room 0
	}

	fclose(f);

	printf("\nAborting: TR5 is unknown by this reader past this point\n");

#ifdef __TOMBRAIDER_TEST__
	return 0;  // Always lie when testing to avoid aborting
#else
	Clear();
	return -1000;
#endif
}


int TombRaider::Load(char *filename, void (*percent)(int))
{
   FILE *f;
   int i, j, l;
   unsigned int num_mesh_data_words, num_mesh_pointers, data_size, data_offset;
   unsigned int *mesh_pointer_list;
   unsigned char *raw_mesh_data;
	bool tr5;
   long debugf;


   f = fopen(filename, "rb");

   if (!f)
   {
     perror(filename);
     return -1;
   }


   if (percent)
		(*percent)(1);

	_clear = false;

   Fread(&_version, sizeof(_version), 1, f);
   //FIXME: endian

   DebugPrint("Load> _version = %u", _version);

	tr5 = false;

   switch (_version)
   {
   case 0x00000020:
     _engine_version = TR_VERSION_1;
     break;
   case 0x0000002d:
     _engine_version = TR_VERSION_2;
     break;
   case 0xff080038:
   case 0xff180038:
     _engine_version = TR_VERSION_3;
     break;
   case 0xfffffff0:           // bogus
   case 0x00345254:           // "TR4\0"
     _engine_version = TR_VERSION_4;

	  // Check to see if this is really TR5 demo
	  l = strlen(filename);

	  // Looking for pattern "filename.trc"
	  if (filename[l-1] == 'c' || filename[l-1] == 'C' &&
			filename[l-2] == 'r' || filename[l-2] == 'R' &&
			filename[l-3] == 't' || filename[l-3] == 'T')
	  {
		  DebugPrint("Load> This is really TR5");
		  _engine_version = TR_VERSION_5;
		  return LoadTR5(f, percent);
	  }

     break;
   default:
     _engine_version = TR_VERSION_UNKNOWN;
   }

   if (percent)
		(*percent)(5);

   DebugPrint("Load> _engine_version = 0x%x", _version);

   if (_engine_version == TR_VERSION_UNKNOWN)
     return -1;

   if (_engine_version == TR_VERSION_4) 
   {
#ifdef ZLIB_SUPPORT
		unsigned int sz, usz; // compressed and uncompressed size  
		unsigned char *compressed_data = NULL;
		int zerr;
		uLongf foo;
     

		// Read texture type offsets
		Fread(&_num_room_textures, 2, 1, f);
		DebugPrint("LoadTR4> _num_room_textures = %u", _num_room_textures);
		Fread(&_num_misc_textures, 2, 1, f);
		DebugPrint("LoadTR4> _num_misc_textures = %u", _num_misc_textures);
		Fread(&_num_bump_map_textures, 2, 1, f);
		DebugPrint("LoadTR4> _num_bump_map_textures  = %u", _num_bump_map_textures);
		
     // Read the sizes of the 32-bit textures
     Fread(&usz, sizeof(usz), 1, f);
     Fread(&sz, sizeof(sz), 1, f);

     DebugPrint("Load> TR4 32-bit textures compressed size   = %u bytes", sz);
     DebugPrint("Load> TR4 32-bit textures uncompressed size = %u bytes", usz);

	  _num_textiles = usz / sizeof(tr2_textile32_t);

	  DebugPrint("LoadTR4> _num_textiles = %i/%i = %i", 
					 usz, sizeof(tr2_textile32_t), _num_textiles);

	  _textile32 = new tr2_textile32_t[_num_textiles];

	  // Allocate a temporary buffer for decompression
	  compressed_data = new unsigned char[sz];
	  Fread(compressed_data, sz, 1, f);

	  // Decompress the textures
	  foo = usz;
	  zerr = uncompress((unsigned char *)_textile32,
							  &foo,
							  compressed_data, 
							  sz);
	  usz = foo;

	  if (percent)
		  (*percent)(6);

	  DebugPrint("LoadTR4> textile decompress  [%s]", 
					 (zerr == Z_OK) ? "OK" : "ERROR");

	  switch (zerr)
	  {
	  case Z_MEM_ERROR:
		  DebugPrint("LoadTR4> There was not enough memory");
		  break;
	  case Z_BUF_ERROR:
		  DebugPrint("LoadTR4> There was not enough room in the output buffer");
		  break;
	  case Z_DATA_ERROR:
		  DebugPrint("LoadTR4> The input data was corrupted");
		  break;
	  default:
		  DebugPrint("LoadTR4> textile decompress %i", zerr);
	  }
     
	  // Free the temporary buffer
	  delete [] compressed_data;

     // Read in the 16-bit textures, set NumTextiles
     Fread(&usz, sizeof(usz), 1, f);
     Fread(&sz, sizeof(sz), 1, f);

     DebugPrint("Load> TR4 16-bit textures compressed size   = %u bytes", sz);
     DebugPrint("Load> TR4 16-bit textures uncompressed size = %u bytes", usz);
      
     _num_textiles = usz / sizeof(tr2_textile16_t);

     DebugPrint("Load> TR4 _num_textiles = %i/%i = %i", 
		usz, sizeof(tr2_textile16_t), _num_textiles);

     _textile16 = new tr2_textile16_t[_num_textiles];

     // Allocate a temporary buffer for decompression
     compressed_data = new unsigned char[sz];
     Fread(compressed_data, sz, 1, f);

     // Decompress the textures
	  foo = usz;
     zerr = uncompress((unsigned char *)_textile16,
		       &foo,
		       compressed_data, 
		       sz);
     usz = foo;

	  if (percent)
		  (*percent)(7);

	  //     DebugPrint("Load> TR4 textile decompress  [%s]", 
	  //	(zerr == Z_OK) ? "OK" : "ERROR");

     switch (zerr)
     {
     case Z_MEM_ERROR:
		  DebugPrint("Load> TR4 textile decompress [ERROR]");
		  DebugPrint("Load> TR4 There was not enough memory");
		  break;
     case Z_BUF_ERROR:
		  DebugPrint("Load> TR4 textile decompress [ERROR]");
		  DebugPrint("Load> TR4 There was not enough room in the output buffer");
		  break;
     case Z_DATA_ERROR:
		  DebugPrint("Load> TR4 textile decompress [ERROR]");
		  DebugPrint("Load> TR4 The input data was corrupted");
		  break;
	  case Z_OK:
		  DebugPrint("Load> TR4 textile decompress [OK]");
		  break;
	  default:
		  DebugPrint("Load> TR4 textile decompress %i", zerr);
     }
     
     // Free the temporary buffer
     delete [] compressed_data;

     // Read the sizes of the sprite textures
     Fread(&usz, sizeof(usz), 1, f);
     Fread(&sz, sizeof(sz), 1, f);

     DebugPrint("Load> TR4 sprite textures compressed size   = %u bytes", sz);
     DebugPrint("Load> TR4 sprite textures uncompressed size = %u bytes", usz);

	  // Load sprite/bump map/gui/etc textures also	
	  _num_tex_special = usz/(256*256*4);

	  DebugPrint("LoadTR5> _num_tex_special = %i/%i = %i", 
					 usz, 256*256*4, _num_tex_special);

	  DebugPrint("LoadTR5> Reading %ibytes of sprite textures", usz);

	  if (usz)
	  {
		  _tex_special = new unsigned char[usz];

		  // Allocate a temporary buffer for decompression
		  compressed_data = new unsigned char[sz];
		  Fread(compressed_data, sz, 1, f);

		  // Decompress the textures
		  foo = usz;
		  zerr = uncompress(_tex_special,
								  &foo,
								  compressed_data, 
								  sz);
		  usz = foo;

		  DebugPrint("LoadTR5> special texture decompress  [%s]", 
						 (zerr == Z_OK) ? "OK" : "ERROR");

		  switch (zerr)
		  {
		  case Z_MEM_ERROR:
			  DebugPrint("LoadTR5> There was not enough memory");
			  break;
		  case Z_BUF_ERROR:
			  DebugPrint("LoadTR5> There was not enough room in the output buffer");
			  break;
		  case Z_DATA_ERROR:
			  DebugPrint("LoadTR5> The input data was corrupted");
			  break;
		  default:
			  DebugPrint("LoadTR5> textile decompress %i", zerr);
		  }
     
		  // Free the temporary buffer
		  delete [] compressed_data;		
	  }

	  if (percent)
		  (*percent)(9);

     // Read the sizes of the level data
     Fread(&usz, sizeof(usz), 1, f);
     Fread(&sz, sizeof(sz), 1, f);

     DebugPrint("Load> TR4 level data compressed size   = %u bytes", sz);
     DebugPrint("Load> TR4 level data uncompressed size = %u bytes", usz);

     // Allocate a temporary buffer for decompression
     compressed_data = new unsigned char[sz];
     Fread(compressed_data, sz, 1, f);
     _compressed_level_data = new unsigned char[usz];
      
     // Decompress the level data
	  foo = usz;
     zerr = uncompress(_compressed_level_data, &foo, compressed_data, sz);

     usz = foo;

     DebugPrint("Load> TR4 level data decompress  [%s]", 
		(zerr == Z_OK) ? "OK" : "ERROR");

     switch (zerr)
     {
     case Z_MEM_ERROR:
       DebugPrint("Load> TR4 There was not enough memory");
       break;
     case Z_BUF_ERROR:
       DebugPrint("Load> TR4 There was not enough room in the output buffer");
       break;
     case Z_DATA_ERROR:
       DebugPrint("Load> TR4 The input data was corrupted");
       break;
     }

     delete [] compressed_data;

     _compressed_level_data_offset = 0;
     _compressed_level_size = usz;

     // Toggle Fread mode to read from decompressed data in memory, not diskfile
	  _fread_mode = TR_FREAD_COMPRESSED;
#else
     Print("Load> ERROR: TR4 support not compiled in this build.");
     Print("Load> Try 'make tr4' next build.  Requires zlib.");
     return -1;
#endif   
   }

   if (_engine_version == TR_VERSION_2 || _engine_version == TR_VERSION_3)
   {
     /* Read the 8-bit palette */
     Fread(_palette8, sizeof(tr2_colour_t), 256, f);

     /* Read 16-bit palette */
     Fread(_palette16, sizeof(_palette16), 1, f);     

     DebugPrint("Load> Read TR 2|3 8bit and 16bit palettes");
   }

   if (_engine_version != TR_VERSION_4) 
   {
     /* Read the textiles */
     Fread(&_num_textiles, sizeof(_num_textiles), 1, f);

     DebugPrint("Load> _num_textiles = %i", _num_textiles);

     /* 8-bit textiles come first */
     _textile8 = new tr2_textile8_t[_num_textiles];
     Fread(_textile8, sizeof(tr2_textile8_t), _num_textiles, f);

     /* 16-bit textiles come second */
     _textile16 = new tr2_textile16_t[_num_textiles];

     if (_engine_version != TR_VERSION_1) 
     {
       //FIXME: need endian checking here
       Fread(_textile16, sizeof(tr2_textile16_t), _num_textiles, f);
       DebugPrint("Load> Read in 16bit texture tiles");
     }
   }

   if (percent)
		(*percent)(10);

   /* 32-bit unknown - seems to always be 0 */
   Fread(&_unknown_t, sizeof(_unknown_t), 1, f);
   DebugPrint("Load> _unknown_t = 0x%x", _unknown_t);
   
   /* Read raw room data */
   //FIXME: needs endian checking
   Fread(&_num_rooms, sizeof(_num_rooms), 1, f);   
   DebugPrint("Load> _num_rooms = %i", _num_rooms);

   data_size = _num_rooms * sizeof(tr2_room_t);
   _rooms = new tr2_room_t[_num_rooms];

   /* Extract room details */
   for (i = 0; i < _num_rooms; ++i) 
   {
		if (percent)
		{
			(*percent)(11 + (int)(((float)i/(float)_num_rooms)*70.0));
		}

     /* Read RoomInfo */
     //FIXME: endian check needed
     Fread(&_rooms[i].info, sizeof(tr2_room_info_t), 1, f);     

     DebugPrint("Load> _rooms[%i].info =\n { x=%i, z=%i, yt=%i, yb=%i}", 
					 i, 
					 _rooms[i].info.x, _rooms[i].info.z,
					 _rooms[i].info.y_top, _rooms[i].info.y_bottom);

     /* Read raw data for rest of room */
     Fread(&_rooms[i].num_data_words, sizeof(_rooms[i].num_data_words), 1, f);

     DebugPrint("Load> _rooms[%i].num_data_words = %u",
					 i, _rooms[i].num_data_words);

     _rooms[i].data = new unsigned char[_rooms[i].num_data_words*2];
     Fread(_rooms[i].data, 2, _rooms[i].num_data_words, f);
     
     /* Identify vertices */
     data_offset = 0;
     //FIXME: endian
     _rooms[i].room_data.num_vertices = *(short *)(_rooms[i].data);

     data_offset += sizeof(_rooms[0].room_data.num_vertices);
     data_size = _rooms[i].room_data.num_vertices * sizeof(tr2_vertex_room_t);
     
     DebugPrint("Load> _rooms[%i].room_data.num_vertices = %u",
					 i, _rooms[i].room_data.num_vertices);

	  _rooms[i].room_data.vertices = 0x0;

     if (_rooms[i].room_data.num_vertices > 0) 
     {
       _rooms[i].room_data.vertices = 
			 new tr2_vertex_room_t[_rooms[i].room_data.num_vertices];
       
       if (_engine_version == TR_VERSION_1) 
       {
			 data_size = _rooms[i].room_data.num_vertices * 
				 (sizeof(tr2_vertex_room_t) - 4);
	 
			 for (j = 0; j < _rooms[i].room_data.num_vertices; ++j) 
          {
				 memcpy(&_rooms[i].room_data.vertices[j], 
						  _rooms[i].data + data_offset + 
						  (j * (sizeof(tr2_vertex_room_t) - 4)), 
						  sizeof(tr2_vertex_room_t) - 4);
	   
				 // ??? Adjust for what's missing?
				 _rooms[i].room_data.vertices[j].lighting2 = 
					 _rooms[i].room_data.vertices[j].lighting1;
				 _rooms[i].room_data.vertices[j].attributes = 0;
			 }
       }
       else 
       {
			 memcpy(_rooms[i].room_data.vertices, 
					  _rooms[i].data + data_offset, data_size);
       }
       
       //FIXME: endian conversions for verts needed
     }
     
     data_offset += data_size;

     /* identify rectangles */
     //FIXME: endian conversion
     _rooms[i].room_data.num_rectangles = 
       *(short *)(_rooms[i].data + data_offset);
     
     data_offset += sizeof(_rooms[0].room_data.num_rectangles);
     data_size = _rooms[i].room_data.num_rectangles * sizeof(tr2_quad_t);

     DebugPrint("Load> _rooms[%i].room_data.num_rectangles = %i", 
					 i, _rooms[i].room_data.num_rectangles);

	  _rooms[i].room_data.rectangles = 0x0;

     if (_rooms[i].room_data.num_rectangles > 0) 
     {
       _rooms[i].room_data.rectangles = 
			 new tr2_quad_t[_rooms[i].room_data.num_rectangles];
       memcpy(_rooms[i].room_data.rectangles, 
				  _rooms[i].data + data_offset, data_size);
       
       if (_engine_version >= TR_VERSION_3) 
       {
			 int j;
          
			 for (j = 0; j < _rooms[i].room_data.num_rectangles; ++j) 
	       {
				 _rooms[i].room_data.rectangles[j].texture &= 0x7fff;
			 }
       }
       //FIXME: endian conversion
     }

     data_offset += data_size;

     /* Identify triangles */
     _rooms[i].room_data.num_triangles = 
       *(short *)(_rooms[i].data + data_offset);
     //FIXME: endian
     
     data_offset += sizeof(_rooms[0].room_data.num_triangles);
     data_size = _rooms[i].room_data.num_triangles * sizeof(tr2_tri_t);

     DebugPrint("Load> _rooms[%i].room_data.num_triangles = %i", 
					 i, _rooms[i].room_data.num_triangles);
     
	  _rooms[i].room_data.triangles = 0x0;

     if (_rooms[i].room_data.num_triangles > 0) 
     {
       _rooms[i].room_data.triangles = 
			 new tr2_tri_t[_rooms[i].room_data.num_triangles];

       memcpy(_rooms[i].room_data.triangles, 
				  _rooms[i].data + data_offset, data_size);

       if (_engine_version >= TR_VERSION_3)
       {
			 int j;
          
			 for (j = 0; j < _rooms[i].room_data.num_triangles; ++j) 
			 {
				 _rooms[i].room_data.triangles[j].texture &= 0x7fff;
			 }
			 //FIXME: endian
       }
     }      
	
     data_offset += data_size;

      /* Identify sprites */
      _rooms[i].room_data.num_sprites = 
			*(short *)(_rooms[i].data + data_offset);
      //FIXME: endian

      data_offset += sizeof(_rooms[0].room_data.num_sprites);
      data_size = _rooms[i].room_data.num_sprites * sizeof(tr2_room_sprite_t);

		DebugPrint("Load> _rooms[%i].room_data.num_sprites = %i", 
					  i, _rooms[i].room_data.num_sprites);

		_rooms[i].room_data.sprites = 0x0;

      if (_rooms[i].room_data.num_sprites > 0)
      {
			_rooms[i].room_data.sprites = 
				new tr2_room_sprite_t[_rooms[i].room_data.num_sprites];
			memcpy(_rooms[i].room_data.sprites, 
					 _rooms[i].data + data_offset, data_size);

			if (_engine_version >= TR_VERSION_3)
	      {
				int j;

				for (j = 0; j < _rooms[i].room_data.num_sprites; j++) 
	         {
					_rooms[i].room_data.sprites[j].texture &= 0x7fff;
				}
			}

			//FIXME: endian
      }

      /* Free the raw room data */
      delete [] _rooms[i].data;
      _rooms[i].data = NULL;

      /* Read door info */
      //FIXME: endian
      Fread(&_rooms[i].num_portals, sizeof(_rooms[0].num_portals), 1, f);

      DebugPrint("Load> _rooms[%i].num_portals = %i", 
		i, _rooms[i].num_portals);

		if (_rooms[i].num_portals > 0)
			_rooms[i].portals = new tr2_room_portal_t[_rooms[i].num_portals];
		else
			_rooms[i].portals = 0;

      Fread(_rooms[i].portals, sizeof(tr2_room_portal_t), 
	    _rooms[i].num_portals, f);
      //FIXME: endian

      /* Read sector info */
      //FIXME: endian
      Fread(&_rooms[i].num_zsectors, sizeof(_rooms[0].num_zsectors), 1, f);
      Fread(&_rooms[i].num_xsectors, sizeof(_rooms[0].num_xsectors), 1, f);

      DebugPrint("Load> _rooms[%i].num_zsectors = %i", 
		i, _rooms[i].num_zsectors);
      DebugPrint("Load> _rooms[%i].num_xsectors = %i", 
		i, _rooms[i].num_xsectors);

		if (_rooms[i].num_zsectors > 0 && _rooms[i].num_xsectors > 0)
		{
			_rooms[i].sector_list = 
			new tr2_room_sector_t[_rooms[i].num_zsectors * _rooms[i].num_xsectors];
		}
		else
		{
			_rooms[i].sector_list = 0x0;
		}
   
      Fread(_rooms[i].sector_list, sizeof(tr2_room_sector_t),
	    _rooms[i].num_zsectors * _rooms[i].num_xsectors, f);
      //FIXME: endian

      DebugPrint("Load> Read %u room sectors", 
		 _rooms[i].num_zsectors * _rooms[i].num_xsectors);

      /* Read room lighting & mode */
      if (_engine_version >= TR_VERSION_3) 
      {
			Fread(&_rooms[i].intensity1, 4, 1, f);
			// Fake TR2 record:
			_rooms[i].light_mode = 0;
      }
      else if (_engine_version == TR_VERSION_1) 
      {
			Fread(&_rooms[i].intensity1, 2, 1, f);
			// Is this intensity or LightMode?

			DebugPrint("Load> _rooms[%i].intensity1 = %u", 
						  i, _rooms[i].intensity1);

			_rooms[i].intensity2 = _rooms[i].intensity1;
			_rooms[i].light_mode = 0;
      }
      else 
      {   // TR2
			Fread(&_rooms[i].intensity1, 6, 1, f);
			DebugPrint("Load> TR2 _rooms[%i].intensity1 = %u", 
						  i, _rooms[i].intensity1);
      }
    
      /* Read room lighting info */
       //FIXME: endian
      Fread(&_rooms[i].num_lights, sizeof(_rooms[i].num_lights), 1, f);

      DebugPrint("Load> _rooms[%i].num_lights = %u", 
		 i, _rooms[i].num_lights);

		_rooms[i].lights = 0x0;

		// Mongoose 2002.04.03, New TR4 light struct, removed old 
		//   double size for others
      if (_rooms[i].num_lights > 0) 
      {
			if (_engine_version == TR_VERSION_1) 
			{
				_rooms[i].lights = new tr2_room_light_t[_rooms[i].num_lights];

				for (j = 0; j < _rooms[i].num_lights; ++j) 
				{
					Fread(&_rooms[i].lights[j].x, sizeof(_rooms[0].lights[0].x), 3,f);
					// x, y, z
	    
					DebugPrint("Load> _rooms[%i].lights[%i] = <%i %i %i>", 
								  i, j, 
								  _rooms[i].lights[j].x,
								  _rooms[i].lights[j].y,
								  _rooms[i].lights[j].z);

					Fread(&_rooms[i].lights[j].intensity1, sizeof(short), 1, f); 
					// Intensity1

					DebugPrint("Load> _rooms[%i].lights[%i].intensity1 = %u", 
								  i, j, 
								  _rooms[i].lights[j].intensity1);

					_rooms[i].lights[j].intensity2 = _rooms[i].lights[j].intensity1;
					Fread(&_rooms[i].lights[j].fade1, sizeof(unsigned int), 1, f);
					// Fade1

					DebugPrint("Load> _rooms[%i].lights[%i].fade1 = %u", 
								  i, j, 
								  _rooms[i].lights[j].fade1);

					_rooms[i].lights[j].fade2 = _rooms[i].lights[j].fade1;
				}
			}
			else if (_engine_version == TR_VERSION_4) 
	      {
				tr4_room_light_t *tr4_lights =
				   new tr4_room_light_t[_rooms[i].num_lights];

				Fread(tr4_lights, sizeof(tr4_room_light_t), _rooms[i].num_lights, f);

				delete [] tr4_lights;
			}
			else 
	      {
				_rooms[i].lights = new tr2_room_light_t[_rooms[i].num_lights];

				Fread(_rooms[i].lights, sizeof(tr2_room_light_t),
						_rooms[i].num_lights, f);
			}
      }
      //FIXME: endian
    
      /* Read Static Mesh Data */
      Fread(&_rooms[i].num_static_meshes, sizeof(unsigned short), 1, f);
      //FIXME: endian

      DebugPrint("Load> _rooms[%i].num_static_meshes = %u", 
					  i, _rooms[i].num_static_meshes);

		_rooms[i].static_meshes = 0x0;

      if (_rooms[i].num_static_meshes > 0) 
      {
         _rooms[i].static_meshes = 
				new tr2_room_staticmesh_t[_rooms[i].num_static_meshes];
         
			if (_engine_version == TR_VERSION_1) 
	      {
				for (j = 0; j < _rooms[i].num_static_meshes; j++) 
	         {
					Fread(&_rooms[i].static_meshes[j], 18, 1, f); 
					// Account for the missing .intensity2
	     
					_rooms[i].static_meshes[j].object_id = 
						_rooms[i].static_meshes[j].intensity2;
	     
					_rooms[i].static_meshes[j].intensity2 = 
						_rooms[i].static_meshes[j].intensity1;
				}
			}
         else 
			{
				Fread(_rooms[i].static_meshes, sizeof(tr2_room_staticmesh_t), 
						_rooms[i].num_static_meshes, f);
			}
      }
      //FIXME: endian
    
      Fread(&_rooms[i].alternate_room, sizeof(short), 1, f);
      //FIXME: endian

      DebugPrint("Load> _rooms[%i].alternate_room = %i", 
		 i, _rooms[i].alternate_room);

      Fread(&_rooms[i].flags, sizeof(short), 1, f);
      //FIXME: endian

      DebugPrint("Load> _rooms[%i].flags = 0x%x", 
					  i, _rooms[i].flags);
      
      /* Read TR3 room light colour */
      if (_engine_version >= TR_VERSION_3)
      {
			/* we force this to be 3 bytes 
				(instead of just sizeof(room_light_colour)) 
				for Macs and others that can't handle odd-length structures...
			*/
			Fread(&_rooms[i].room_light_colour, 3, 1, f);
			DebugPrint("Load> TR3 _rooms[%i].room_light_colour {%i %i %i}", 
						  i, 
						  _rooms[i].room_light_colour.r,
						  _rooms[i].room_light_colour.g,
						  _rooms[i].room_light_colour.b);
      }
    }    
  
   /* Read floor data */
   /*
    * Really, FloorData should be a per-sector dynamic allocation;  however,
    * that requires a parser that can accurately determine where one sector's
    * FloorData ends and another's begins.  Until we have that, we'll stick to
    * this crude (but effective) method...
    */
   Fread(&_num_floor_data, sizeof(_num_floor_data), 1, f);
   DebugPrint("Load> _num_floor_data = %u", _num_floor_data);

	_floor_data = 0x0;

   if (_num_floor_data > 0) 
   {
     _floor_data = new unsigned short[_num_floor_data];
     Fread(_floor_data, sizeof(short), _num_floor_data, f);
     //FIXME: endian
   }
  
   /* Read mesh data */
   Fread(&num_mesh_data_words, sizeof(num_mesh_data_words), 1, f);
   //FIXME: endian

   DebugPrint("Load> num_mesh_data_words = %u", num_mesh_data_words);

   raw_mesh_data = new unsigned char[num_mesh_data_words*2];
   Fread(raw_mesh_data, 2, num_mesh_data_words, f);
   // Endian-conversion of this data occurs in ExtractMeshes()

   DebugPrint("Load> Read raw_mesh_data");

   /* Read mesh pointers */
   Fread(&num_mesh_pointers, sizeof(num_mesh_pointers), 1, f);
   //FIXME: endian

   DebugPrint("Load> num_mesh_pointers = %u", num_mesh_pointers);

   mesh_pointer_list = new unsigned int[num_mesh_pointers];
   Fread(mesh_pointer_list, sizeof(unsigned int), num_mesh_pointers, f);
   //FIXME: endian

   DebugPrint("Load> Read mesh_pointer_list");

   /* Extract meshes */
   ExtractMeshes(raw_mesh_data, num_mesh_pointers, mesh_pointer_list);
   delete [] raw_mesh_data;
   delete [] mesh_pointer_list; 

   /* Read animations */
   Fread(&_num_animations, sizeof(_num_animations), 1, f);
   //FIXME: endian

   DebugPrint("Load> _num_animations = %u", _num_animations);

	_animations = 0x0;

   if (_num_animations > 0) 
   {
		_animations = new tr2_animation_t[_num_animations];
     
     if (_engine_version == TR_VERSION_4) 
     {
		  tr4_animation_t tr4_anim;

       for (i = 0; i < (int)_num_animations; ++i) 
       {
			 Fread(&tr4_anim, 40, 1, f);

			 _animations[i].frame_offset = tr4_anim.frame_offset;
			 _animations[i].frame_rate = tr4_anim.frame_rate;  
			 _animations[i].frame_size = tr4_anim.frame_size;                   
			 _animations[i].state_id = tr4_anim.state_id;
			 _animations[i].unknown1 = tr4_anim.unknown;
			 _animations[i].unknown2 = tr4_anim.speed;
			 _animations[i].unknown3 = tr4_anim.accel_lo;
			 _animations[i].unknown4 = tr4_anim.accel_hi;
			 _animations[i].frame_start = tr4_anim.frame_start;
			 _animations[i].frame_end = tr4_anim.frame_end;
			 _animations[i].next_animation = tr4_anim.next_animation;
			 _animations[i].next_frame = tr4_anim.next_frame;
			 _animations[i].num_state_changes = tr4_anim.num_state_changes;
			 _animations[i].state_change_offset = tr4_anim.state_change_offset;
			 _animations[i].num_anim_commands = (tr4_anim.num_anim_commands > 256)
? 0 : tr4_anim.num_anim_commands;
			 _animations[i].anim_command = tr4_anim.anim_command;       
       }
     }
     else 
     {
         Fread(_animations, sizeof(tr2_animation_t), _num_animations, f);
     }
   }
   //FIXME: endian
  
   /* Read state changes */
   Fread(&_num_state_changes, sizeof(_num_state_changes), 1, f);
   //FIXME: endian


   if (percent)
		(*percent)(80);

   DebugPrint("Load> _num_state_changes = %u", _num_state_changes);

   if (_num_state_changes > 0) 
   {
     _state_changes = new tr2_state_change_t[_num_state_changes];

     Fread(_state_changes, sizeof(tr2_state_change_t), _num_state_changes, f);
   }
   //FIXME: endian

   /* Read AnimDispatches */
   Fread(&_num_anim_dispatches, sizeof(_num_anim_dispatches), 1, f);
   //FIXME: endian  

   DebugPrint("Load> _num_anim_dispatches = %u", _num_anim_dispatches);

	_anim_dispatches = 0x0;

   if (_num_anim_dispatches > 0) 
   {
      _anim_dispatches = new tr2_anim_dispatch_t[_num_anim_dispatches];
      Fread(_anim_dispatches, sizeof(tr2_anim_dispatch_t),
	    _num_anim_dispatches, f);
   }
   //FIXME: endian
  
   /* Read anim commands */
   Fread(&_num_anim_commands, sizeof(_num_anim_commands), 1, f);
   //FIXME: endian   

   DebugPrint("Load> _num_anim_commands = %u", _num_anim_commands);

	_anim_commands = 0x0;

   if (_num_anim_commands > 0) 
   {
     _anim_commands = new tr2_anim_command_t[_num_anim_commands];
     Fread(_anim_commands, sizeof(tr2_anim_command_t), _num_anim_commands, f);
   }
   //FIXME: endian 

   /* Read MeshTrees */
   Fread(&_num_mesh_trees, sizeof(_num_mesh_trees), 1, f);
   //FIXME: endian 

   DebugPrint("Load> _num_mesh_trees = %u", _num_mesh_trees);

	_mesh_trees = 0x0;

   if (_num_mesh_trees > 0) 
   {
     _mesh_trees = new tr2_meshtree_t[_num_mesh_trees];
     Fread(_mesh_trees, sizeof(int), _num_mesh_trees, f);
   }
   //FIXME: endian

   /* Read frames */
   Fread(&_num_frames, sizeof(_num_frames), 1, f);
   //FIXME: endian

   DebugPrint("Load> _num_frames = %u", _num_frames);

	_frames = 0x0;

   if (_num_frames > 0) 
   {
     _frames = new unsigned short[_num_frames];
     Fread(_frames, 2, _num_frames, f);
     //FIXME: endian

     if (_engine_version == TR_VERSION_1) 
     {
       // re-format the frames[] to look like TR2 frames
       int num_frames;

       for (j = 0; j < (int)_num_animations; ++j) 
       {
			 int fo = _animations[j].frame_offset / 2;
			 _animations[j].frame_size = (_frames[fo + 9] * 2) + 10;
       }

       for (i = 0; i < (int)_num_frames; ) 
       {
			 i += 9;  // point to num_frames;
			 j = i;   // get rid of (overwrite) num_frames
			 num_frames = _frames[i++];

			 while (num_frames--) 
			 {
				 _frames[j++] = _frames[i + 1];   // reverse the words as we go
				 _frames[j++] = _frames[i];
				 i += 2;
			 }
       }
     }
   }

   /* Read moveables */
   Fread(&_num_moveables, sizeof(_num_moveables), 1, f);
   //FIXME: endian 
   DebugPrint("Load> _num_moveables = %u", _num_moveables);

	_moveables = 0x0;

   if (_num_moveables > 0) 
   {
     debugf = ftell(f);
     _moveables = new tr2_moveable_t[_num_moveables];
     Fread(_moveables, 18, _num_moveables, f);
   } 
   //FIXME: endian

   Fread(&_num_static_meshes, sizeof(int), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_static_meshes = %u", _num_static_meshes);
   
   // SAFE EXIT //////////////////////////

	_static_meshes = 0x0;

   if (_num_static_meshes > 0) 
   {  
     _static_meshes = new tr2_staticmesh_t[_num_static_meshes];
     Fread(_static_meshes, sizeof(tr2_staticmesh_t), 
	   _num_static_meshes, f);
     //FIXME: endian
   }

	_object_textures = 0x0;

   if (_engine_version < TR_VERSION_3) 
   {
     /* Read object textures */
     Fread(&_num_object_textures, sizeof(int), 1, f);
     DebugPrint("Load> _num_object_textures = %u", _num_object_textures);
     //FIXME: endian

     if (_num_object_textures > 0) 
     {
       _object_textures = new tr2_object_texture_t[_num_object_textures];

       Fread(_object_textures, sizeof(tr2_object_texture_t), 
	     _num_object_textures, f);
     }
     //FIXME: endian
   }


   if (percent)
		(*percent)(90);

   if (_engine_version == TR_VERSION_4) 
   {
     unsigned char zzbuf[4];
     Fread(zzbuf, 1, 3, f); // skip "SPR"
     zzbuf[3] = 0;

     DebugPrint("Load> TR4 checking if %s == SPR", zzbuf);
   }

   /* Read sprite textures */
   Fread(&_num_sprite_textures, sizeof(int), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_sprite_textures = %u", _num_sprite_textures);

	_sprite_textures = 0x0;

   if (_num_sprite_textures > 0) 
   {
     _sprite_textures = new tr2_sprite_texture_t[_num_sprite_textures];
     Fread(_sprite_textures, sizeof(tr2_sprite_texture_t),
	   _num_sprite_textures, f);
   }
   //FIXME: endian

   /* Read sprite texture data (?) */
   Fread(&_num_sprite_sequences, sizeof(int), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_sprite_sequences = %u", _num_sprite_sequences);

	_sprite_sequences = 0x0;

   if (_num_sprite_sequences > 0) 
   {
     _sprite_sequences = new tr2_sprite_sequence_t[_num_sprite_sequences];
     Fread(_sprite_sequences, sizeof(tr2_sprite_sequence_t),
	   _num_sprite_sequences, f);
   }
   //FIXME: endian

   /* Read cameras */
   Fread(&_num_cameras, sizeof(_num_cameras), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_cameras = %i", _num_cameras);

	_cameras = 0x0;

   if (_num_cameras > 0) 
   {
     _cameras = new tr2_camera_t[_num_cameras];
     Fread(_cameras, sizeof(tr2_camera_t), _num_cameras, f);
     //FIXME: endian
   }

	if (_engine_version == TR_VERSION_4) 
	{
		int num_ex_cam;
		tr4_extra_camera_t *ex_cam;

		Fread(&num_ex_cam, 4, 1, f);

		DebugPrint("Load> num_extra_cam = %i", num_ex_cam);

		if (num_ex_cam > 0)
		{
			ex_cam = new tr4_extra_camera_t[num_ex_cam];
			Fread(ex_cam, sizeof(tr4_extra_camera_t), num_ex_cam, f);
			delete [] ex_cam;
		}
	}

   /* Read sound effects (?) */
   Fread(&_num_sound_sources, sizeof(_num_sound_sources), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_sound_sources = %i", _num_sound_sources);

	_sound_sources = 0x0;

   if (_num_sound_sources > 0) 
   {
     _sound_sources = 
       (tr2_sound_source_t*) new unsigned char[_num_sound_sources*40];
     
     if (_engine_version == TR_VERSION_4) 
     {
		  // Mongoose 2002.04.04, was 40 sized reads before extra_camera
       Fread(_sound_sources, sizeof(tr2_sound_source_t), 
				 _num_sound_sources, f);
     }
     else 
     {
       Fread(_sound_sources, sizeof(tr2_sound_source_t), 
	     _num_sound_sources, f);
     }
     //FIXME: endian
   }

#ifdef OBSOLETE
   if (_engine_version == TR_VERSION_4) 
   {  
     unsigned int num_ZZ;
     unsigned char  zzbuf[17];
     Fread(&num_ZZ, 1, sizeof(num_ZZ), f);
     
     while (num_ZZ--) 
     {
       Fread(zzbuf, 1, 16, f);
       zzbuf[16] = 0;
       DebugPrint("Load> TR4 zz dump '%s'", zzbuf);
     }
   }
#endif

   /* Read boxes */
   Fread(&_num_boxes, sizeof(_num_boxes), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_boxes = %i", _num_boxes);

	_boxes = 0x0;

   if (_num_boxes > 0) 
   {
     _boxes = new tr2_box_t[_num_boxes];

     if (_engine_version == TR_VERSION_1)
     {
		  struct tr1_box 
		  {
			  int zmin, zmax, xmin, xmax;
			  short true_floor, overlap_index;
		  }  __attribute__ ((packed)) *tr1box;
		  
		  tr1box = new tr1_box[_num_boxes];
        
		  Fread(tr1box, sizeof(struct tr1_box), _num_boxes, f);
		  //FIXME: endian

         for (j = 0; j < _num_boxes; ++j) 
			{
            _boxes[j].zmin = tr1box[j].zmin / 1024;
            _boxes[j].zmax = tr1box[j].zmax / 1024;
            _boxes[j].xmin = tr1box[j].xmin / 1024;
            _boxes[j].xmax = tr1box[j].xmax / 1024;
            _boxes[j].true_floor = tr1box[j].true_floor;
            _boxes[j].overlap_index = tr1box[j].overlap_index;
			}
         
			delete [] tr1box;
     }
     else 
     {
       Fread(_boxes, sizeof(tr2_box_t), _num_boxes, f);
     }
     //FIXME: endian
   }

   /* Read overlaps (?) */
   Fread(&_num_overlaps, sizeof(_num_overlaps), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_overlaps = %i", _num_overlaps);   

	_overlaps = 0x0;

   if (_num_overlaps > 0) 
   {
     _overlaps = new short[_num_overlaps];
     Fread(_overlaps, 2, _num_overlaps, f);
     //FIXME: endian
   }
   
	_zones = 0x0;

   /* Read Zones */
   if (_num_boxes > 0) 
   {
     _zones = new short[_num_boxes*10];
     
     if (_engine_version == TR_VERSION_1) 
     {
       Fread(_zones, 12, _num_boxes, f);
     }
     else 
     {
       Fread(_zones, 20, _num_boxes, f);
     }
     //FIXME: endian
   }

   /* Read animation textures (?) */
   Fread(&_num_animated_textures, sizeof(_num_animated_textures), 1, f);
   //FIXME: endian 
   DebugPrint("Load> _num_animated_textures = %i", _num_animated_textures);

	_animated_textures = 0x0;

   if (_num_animated_textures > 0) 
   {
     _animated_textures = new short[_num_animated_textures];
     Fread(_animated_textures, 2, _num_animated_textures, f);
     //FIXME: endian  
   }

   if (_engine_version >= TR_VERSION_3)
   {
      /* Read object textures */
      if (_engine_version == TR_VERSION_4) 
      {
			unsigned char zzbuf[5];
			Fread(zzbuf, 1, 4, f); // skip "TEX"  
			//!! this should be 3, but we have a bug...
			zzbuf[4] = 0;
			DebugPrint("Load> TR4 checking %s == TEX", zzbuf);
      }
      
      Fread(&_num_object_textures, sizeof(_num_object_textures), 1, f);
      //FIXME: endian 

      DebugPrint("Load> _num_object_textures = %i", _num_object_textures);

		_object_textures = 0x0;

      if (_num_object_textures > 0) 
      {
			// Used to be 2 * num, and I forgot why...
			_object_textures = new tr2_object_texture_t[_num_object_textures];

			//FIXME: This is fu fu fu fu fu fu 
			if (_engine_version == TR_VERSION_4)
	      {
				int jjj, kkk;
				tr4_object_texture_t *tr4_tex;
	  

				tr4_tex = new tr4_object_texture_t[_num_object_textures];

				Fread(tr4_tex, 38, _num_object_textures, f);
	  
				for (jjj = 0; jjj < (int)_num_object_textures; ++jjj) 
				{
					_object_textures[jjj].transparency_flags = 
						tr4_tex[jjj].attribute;
	    
					_object_textures[jjj].tile = 
						(unsigned short)tr4_tex[jjj].tile & 0x7fff;

					for (kkk = 0; kkk < 4; ++kkk) 
	            {
						_object_textures[jjj].vertices[kkk].xcoordinate = 
							tr4_tex[jjj].vertices[kkk].xcoordinate;
						_object_textures[jjj].vertices[kkk].xpixel = 
							tr4_tex[jjj].vertices[kkk].xpixel;
						_object_textures[jjj].vertices[kkk].ycoordinate =	
							tr4_tex[jjj].vertices[kkk].ycoordinate;
						_object_textures[jjj].vertices[kkk].ypixel = 
							tr4_tex[jjj].vertices[kkk].ypixel;
					}
				}

				delete [] tr4_tex;
			}
			else 
			{
				Fread(_object_textures, sizeof(tr2_object_texture_t),
						_num_object_textures, f);
			}
      }
      //FIXME: endian
   }

   /* Read items */
   Fread(&_num_items, sizeof(_num_items), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_items = %i", _num_items);

	_items = 0x0;

   if (_num_items > 0)
   {
     _items = new tr2_item_t[_num_items];

     if (_engine_version == TR_VERSION_1)
     {
       for (i = 0; i < _num_items; ++i) 
       {
			 Fread(&_items[i], sizeof(tr2_item_t) - 2, 1, f);
			 _items[i].flags = _items[i].intensity2;
			 _items[i].intensity2 = _items[i].intensity1;
       }
     }
     else 
     {
       Fread(_items, sizeof(tr2_item_t), _num_items, f);
     }
   }
   //FIXME: endian

   /* Read LightMaps */
   _light_map = new unsigned char[32 * 256];

   if (_engine_version != TR_VERSION_4) 
   {
     Fread(_light_map, 32, 256, f);
   }

   if (_engine_version == TR_VERSION_1) 
   {
      /* read the 8-bit palette */
      Fread(_palette8, sizeof(tr2_colour_t), 256, f);
      DebugPrint("Load> Read TR 1 palette");  

      // build 16-bit textiles from 8-bit 
      // (no extra colours, but creates consistent .TR2 file)
      for (i = 0; i < (int)_num_textiles; ++i) 
      {
         unsigned short argb;
         double colour_tmp;

         for (j = 0; j < (256 * 256); ++j) 
			{
				colour_tmp = _palette8[_textile8[i].tile[j]].r & 0x3f;
				colour_tmp = colour_tmp * 31.0 / 63.0;
				argb = ((int)colour_tmp) << 10;
				colour_tmp = _palette8[_textile8[i].tile[j]].g & 0x3f;
				colour_tmp = colour_tmp * 31.0 / 63.0;
				argb |= ((int)colour_tmp) << 5;
				colour_tmp = _palette8[_textile8[i].tile[j]].b & 0x3f;
				colour_tmp = colour_tmp * 31.0 / 63.0;
				argb |= ((int)colour_tmp);
				argb &= 0x7fff;   // ???
           
				if (_textile8[i].tile[j] != 0)
					argb |= 0x8000;

				_textile16[i].tile[j] = argb;
			}
      }
   }

   /* Read cinematic frames */
   if (_engine_version == TR_VERSION_4) 
   {
		unsigned int num_ai_data;

		
      Fread(&num_ai_data, 4, 1, f);
		DebugPrint("Load> num_ai_data = %i", num_ai_data);

		tr4_ai_object_t *ai_obj = 0x0;

      if (num_ai_data > 0) 
      {
			ai_obj = new tr4_ai_object_t[num_ai_data];
			Fread(ai_obj, sizeof(tr4_ai_object_t), num_ai_data, f);

			delete [] ai_obj;
      }
   }
   else 
   {
		unsigned short num_cinematic_frames;

     Fread(&num_cinematic_frames, sizeof(num_cinematic_frames), 1, f);
     //FIXME: endian

	  _num_cinematic_frames = num_cinematic_frames;

     DebugPrint("Load> _num_cinematic_frames = %i", _num_cinematic_frames);

	  _cinematic_frames = 0x0;

     if (_num_cinematic_frames > 0) 
     {
       _cinematic_frames = new tr2_cinematic_frame_t[_num_cinematic_frames];

       Fread(_cinematic_frames, sizeof(tr2_cinematic_frame_t),
	     _num_cinematic_frames, f);
         // There may or may not be endian conversion required here - I have
         // no idea what this data is.
     }
   }

   /* Read demodata (?) */
   Fread(&_num_demo_data, sizeof(_num_demo_data), 1, f);
   //FIXME: endian
   DebugPrint("Load> _num_demo_data = %i", _num_demo_data);

	_demo_data = 0x0;

   if (_num_demo_data > 0) 
   {
     _demo_data = new unsigned char[_num_demo_data];
     Fread(_demo_data, 1, _num_demo_data, f);
     // There may or may not be endian conversion required here - I have
     // no idea what this data is.
   }

   /* Read SoundMap */
   _sound_map = new short[370];

   if (_engine_version == TR_VERSION_1) 
   {
     Fread(_sound_map, sizeof(short), 256, f);
     //memset(_sound_map, 0, 370 * sizeof(short));   //FIXME: KLUDGE!!!
   }
   else 
   {
     if (_engine_version == TR_VERSION_4)
     {
       Fread(_sound_map, sizeof(short), 370, f);
     }
     else 
     {
       Fread(_sound_map, sizeof(short), 370, f);
     }
   }
   //FIXME: endian

   /* Read SoundDetails */
   Fread(&_num_sound_details, sizeof(_num_sound_details), 1, f);
   DebugPrint("Load> _num_sound_details = %i", _num_sound_details);
   //FIXME: endian  

	_sound_details = 0x0;

   if (_num_sound_details > 0) 
   {
     _sound_details = new tr2_sound_details_t[_num_sound_details];
     Fread(_sound_details, sizeof(tr2_sound_details_t), _num_sound_details, f);
   }
   //FIXME: endian

   if (_engine_version != TR_VERSION_1) //FIXME: KLUDGE!
   {  
     /* Read sampleindices */
     Fread(&_num_sample_indices, sizeof(_num_sample_indices), 1, f);
     //FIXME: endian    
     DebugPrint("Load> _num_sample_indices = %i", _num_sample_indices);

     if (_num_sample_indices > 0) 
     { 
       _sample_indices = new int[_num_sample_indices]; //FIXME: signedness?
       Fread(_sample_indices, sizeof(unsigned int), _num_sample_indices, f);
       //FIXME: endian
     }
   }

#ifdef ZLIB_SUPPORT
   if (_compressed_level_data)
   {
     DebugPrint("Load> Freeing uncompressed TR4 data");
     delete [] _compressed_level_data;
   }

	// FIXME: memory damage?
	_compressed_level_data = NULL;
#endif

   fclose(f);
   
   if (percent)
		(*percent)(100);
   
   return 0;
}



void TombRaider::ExtractMeshes(unsigned char *mesh_data, 
			       unsigned int num_mesh_pointers,
			       unsigned int *mesh_pointers)
{
  unsigned int size, i;
  unsigned char *mesh_pointer;
  int negative_size;


  /* Alloc space for mesh */
  _num_meshes = num_mesh_pointers;

  DebugPrint("ExtractMeshes> _num_meshes = %u", _num_meshes);

  _meshes = new tr2_mesh_t[_num_meshes];

  DebugPrint("ExtractMeshes> num_mesh_pointers = %u", num_mesh_pointers);

  for (i = 0; i < num_mesh_pointers; ++i)
  {
    /* Get mesh start */
    mesh_pointer = &mesh_data[mesh_pointers[i]];

    /* Get Centre + Unknowns */
    memcpy(&_meshes[i].centre.x, mesh_pointer, 10);
    //FIXME: endian

    // depending on the interpretation of the unknowns that follow the Centre
    // element, more endian conversion may be necessary
    mesh_pointer += 10;

    /* Get number of vertices */
    memcpy(&_meshes[i].num_vertices, mesh_pointer, 2);
    //FIXME: endian

    DebugPrint("ExtractMeshes> _meshes[%i].num_vertices = %u", 
	       i, _meshes[i].num_vertices);

    mesh_pointer += sizeof(unsigned short);
    _meshes[i].num_vertices = (short)abs(_meshes[i].num_vertices);

    /* Get vertex list */
    size = sizeof(tr2_vertex_t) * _meshes[i].num_vertices;
    _meshes[i].vertices = 0x0;

	 if (_meshes[i].num_vertices > 0)
		 _meshes[i].vertices = new tr2_vertex_t[_meshes[i].num_vertices];
	 
    memcpy(_meshes[i].vertices, mesh_pointer, size);
    //FIXME: endian

    mesh_pointer += size;

    /* Get number of normals */
    memcpy(&_meshes[i].num_normals, mesh_pointer, sizeof(unsigned short));
    //FIXME: endian

    mesh_pointer += sizeof(unsigned short);
    negative_size = (_meshes[i].num_normals < 0);
    _meshes[i].num_normals = (short)abs(_meshes[i].num_normals);

    DebugPrint("ExtractMeshes> negative_size = %u", negative_size);

	 _meshes[i].mesh_lights = 0x0;
	 _meshes[i].normals = 0x0;

    /* Get normal list */
    if (negative_size) 
    {
      negative_size = 0;
      
      size = _meshes[i].num_normals * sizeof(unsigned short);

		_meshes[i].mesh_lights = 0x0;
	
		if (_meshes[i].num_normals > 0) 
			_meshes[i].mesh_lights = new short[_meshes[i].num_normals];

      memcpy(_meshes[i].mesh_lights, mesh_pointer, size);
    }
    else 
    {
      size = sizeof(tr2_vertex_t) * _meshes[i].num_normals;

		_meshes[i].normals = 0x0;

		if (_meshes[i].num_normals > 0) 
			_meshes[i].normals = new tr2_vertex_t[_meshes[i].num_normals];

      memcpy(_meshes[i].normals, mesh_pointer, size);
    }
    //FIXME: endian

    mesh_pointer += size;

    /* Get number of textured rectangles */
    memcpy(&_meshes[i].num_textured_rectangles, 
			  mesh_pointer, sizeof(unsigned short));
    //FIXME: endian
      
    mesh_pointer += sizeof(unsigned short);
    _meshes[i].num_textured_rectangles = 
      (short)abs(_meshes[i].num_textured_rectangles);

    size = sizeof(tr2_quad_t) * _meshes[i].num_textured_rectangles;

	 _meshes[i].textured_rectangles = 0x0;

	 if (_meshes[i].num_textured_rectangles > 0)
		 _meshes[i].textured_rectangles = 
		 new tr2_quad_t[_meshes[i].num_textured_rectangles];
		 
    DebugPrint("ExtractMeshes> _meshes[%i].num_textured_rectangles = %u", 
	       i, _meshes[i].num_textured_rectangles);

    /* Get list of textured rectangles */
    if (_meshes[i].num_textured_rectangles > 0) 
    {
		 if (_engine_version == TR_VERSION_4)
		 {
			 int j;
        
			 for (j = 0; j < _meshes[i].num_textured_rectangles; ++j) 
			 {
				 memcpy(&_meshes[i].textured_rectangles[j], 
						  mesh_pointer, sizeof(tr2_quad_t));
				 mesh_pointer += sizeof(tr2_quad_t) + sizeof(unsigned short);
			 }
		 }
		 else 
		 {
			 memcpy(_meshes[i].textured_rectangles, mesh_pointer, size);
		 }
      //FIXME: endian

      if (_engine_version != TR_VERSION_4)
			mesh_pointer += size;
    }

    /* Get number of textured triangles */
    memcpy(&_meshes[i].num_textured_triangles, 
	   mesh_pointer, sizeof(unsigned short));
    //FIXME: endian

    mesh_pointer += sizeof(unsigned short);
    _meshes[i].num_textured_triangles = 
      (short)abs(_meshes[i].num_textured_triangles);

    size = sizeof(tr2_tri_t) * _meshes[i].num_textured_triangles;

	 //if (_engine_version == TR_VERSION_4)
	 //	 size += 2 * _meshes[i].num_textured_triangles;

	 _meshes[i].textured_triangles = 0x0;

	 if (_meshes[i].num_textured_triangles > 0)
	 {
		 _meshes[i].textured_triangles = 
		 new tr2_tri_t[_meshes[i].num_textured_triangles];
	 }

    DebugPrint("ExtractMeshes>  _meshes[%i].num_textured_triangles = %u", 
					i, _meshes[i].num_textured_triangles);

    /* Get list of textured triangles */
    if (_meshes[i].num_textured_triangles > 0)
    {
		 if (_engine_version == TR_VERSION_4) 
		 {
			 int j;

			 for (j = 0; j < _meshes[i].num_textured_triangles; ++j) 
			 {
				 memcpy(&_meshes[i].textured_triangles[j], 
						  mesh_pointer, sizeof(tr2_tri_t));
				 mesh_pointer += sizeof(tr2_tri_t) + sizeof(unsigned short);
			 }
		 }
		 else 
		 {
			 memcpy(_meshes[i].textured_triangles, mesh_pointer, size);
		 }
		 //FIXME: endian
      
		 if (_engine_version != TR_VERSION_4)
			 mesh_pointer += size;
    }

    if (_engine_version == TR_VERSION_4) 
    {
      _meshes[i].num_coloured_rectangles = 0;
      _meshes[i].num_coloured_triangles = 0;
		_meshes[i].coloured_rectangles = 0x0;
      _meshes[i].coloured_triangles = 0x0;

		// Mongoose 2002.04.04, FIXME is this right?
      mesh_pointer += 2;
      continue;
    }

    /* Get number of coloured rectangles */
    memcpy(&_meshes[i].num_coloured_rectangles, mesh_pointer, 
	   sizeof(unsigned short));
    //FIXME: endian
    mesh_pointer += sizeof(unsigned short);
    _meshes[i].num_coloured_rectangles = 
      (short)abs(_meshes[i].num_coloured_rectangles);

    _meshes[i].coloured_rectangles = 0x0;

    size = sizeof(tr2_quad_t) * _meshes[i].num_coloured_rectangles;

	 if (_meshes[i].num_coloured_rectangles > 0)
		 _meshes[i].coloured_rectangles = 
		 new tr2_quad_t[_meshes[i].num_coloured_rectangles];

    DebugPrint("ExtractMeshes> _meshes[%i].num_coloured_rectangles = %u", 
					i, _meshes[i].num_coloured_rectangles);

    /* Get list of coloured rectangles */
    if (_meshes[i].num_coloured_rectangles > 0) 
    {
      memcpy(_meshes[i].coloured_rectangles, mesh_pointer, size);
      //FIXME: endian
      mesh_pointer += size;
    }
    
    /* Get number of coloured triangles */
    memcpy(&_meshes[i].num_coloured_triangles, mesh_pointer, 
	   sizeof(unsigned short));
    //FIXME: endian

    mesh_pointer += sizeof(unsigned short);
    _meshes[i].num_coloured_triangles = 
      (short)abs(_meshes[i].num_coloured_triangles);

    size = sizeof(tr2_tri_t) * _meshes[i].num_coloured_triangles;

	 _meshes[i].coloured_triangles = 0x0;

	 if (_meshes[i].num_coloured_triangles > 0)
		 _meshes[i].coloured_triangles = 
      new tr2_tri_t[_meshes[i].num_coloured_triangles];

    DebugPrint("ExtractMeshes> _meshes[%i].num_coloured_triangles = %u", 
	       i, _meshes[i].num_coloured_triangles);

    /* Get list of coloured triangles */
    if (_meshes[i].num_coloured_triangles > 0) 
    {
      memcpy(_meshes[i].coloured_triangles, mesh_pointer, size);
      //FIXME: endian

      mesh_pointer += size;
    }
  }
}


int TombRaider::SaveTR2(char *filename)
{
	printf("TombRaider::SaveTR2> Implmentation removed.\n");
	return -1;
}


void TombRaider::Debug(bool set)
{
  _debug_on = set;
}


void TombRaider::ComputeRotationAngles(unsigned short **frame,
													unsigned int *frame_offset,
													unsigned int *angle_offset,
													float *x, float *y, float *z)
{
	unsigned short itmp, itmp2;
	double angle;


	itmp = (*frame)[(*frame_offset) + (*angle_offset)++];

	if (Engine() == TR_VERSION_1)
	{
		// All angles are three-axis
		angle = (itmp >> 4) & 0x03ff;
		angle *= 360.0 / 1024.0;
		*x = angle;

		itmp2 = (itmp << 6) & 0x03c0;

		// Get Z rotation
		itmp = (*frame)[(*frame_offset) + (*angle_offset)];
		++(*angle_offset);

		itmp2 |= (itmp >> 10) & 0x003f;
		angle = itmp2;
		angle *= 360.0 / 1024.0;
		*y = angle;

		angle = itmp & 0x3ff;
		angle *= 360.0 / 1024.0;
		*z = angle;
	}
	else if (itmp & 0xc000) 
	{ 
		// TR2, TR3, TR4 - single axis of rotation
		if (Engine() == TR_VERSION_4)
		{
			angle = itmp & 0x0fff;
			angle /= 4096.0;
			angle *= 360.0;
		}
		else 
		{
			angle = itmp & 0x3ff;
			angle /= 1024.0;
			angle *= 360.0;
		}
	  
		switch (itmp & 0xc000)
		{
		case 0x4000:
			*x = angle;
			break;
		case 0x8000:
			*y = angle;
			break;
		case 0xc000:
			*z = angle;
			break;
		}
	}
	else    // TR2, TR3, TR4 - three axes
	{
		angle = (itmp >> 4) & 0x03ff;
		angle *= 360.0 / 1024.0;
		*x = angle;

		itmp2 = (itmp << 6) & 0x03c0;
		itmp = (*frame)[(*frame_offset) + (*angle_offset)++]; // get Z rotation

		itmp2 |= (itmp >> 10) & 0x003f;
		angle = itmp2;
		angle *= 360.0 / 1024.0;
		*y = angle;

		angle = itmp & 0x3ff;
		angle *= 360.0 / 1024.0;
		*z = angle;
	}
}


void TombRaider::ComputeUV(tr2_object_texture_vert_t *st, float *u, float *v)
{
	unsigned char x, y;

	
	if (!st || !u || !v)
		return;

#ifdef OBSOLETE
	x = st->xpixel;
	y = st->ypixel;

	x += (char)st->xcoordinate;
	y += (char)st->ycoordinate;

	//x += ((char)st->xcoordinate >= 0) ? 1 : -1;
	//y += ((char)st->ycoordinate >= 0) ? 1 : -1;

	*u = (float)x / 255.0;
	*v = (float)y / 255.0;
#else
	*u = AdjustTexel(st->xpixel, st->xcoordinate);
	*v = AdjustTexel(st->ypixel, st->ycoordinate);
#endif
}


float TombRaider::AdjustTexel(unsigned char texel, char offset)
{
   if (offset >= 0)
      ++texel;
   else
      --texel;

   return ((double)texel / 255.0);
}


//////////// PRIVATE METHODS //////////////////////////////////


unsigned char *TombRaider::TexTile(int texture)
{
	unsigned char *image;
	unsigned int color;
	int j, k, index, offset;
	int xmin, xmax, ymin, ymax, x, y;	


	image = NULL;

	if (texture >=0 && texture < (int)_num_textiles)
   {
		image = new unsigned char[256*256*4];
		memset(image, 0, 256*256*4);

		if (_textile32)
		{
			// Convert 32bit BGRA image format to 32bit RGBA
			for (j = 0; j < 256; j++) 
			{
				for (k = 0; k < 256; k++) 
				{
					index = (j * 256) + k;
					color = _textile32[texture].tile[index];
					index = (j * 1024) + (k * 4);
					
					image[index + 2] = *((unsigned char *)(&color));
					image[index + 1] = *((unsigned char *)(&color)+1);
					image[index + 0] = *((unsigned char *)(&color)+2);
					image[index + 3] = *((unsigned char *)(&color)+3);
				}
			}
		}
		else
		{
			// Convert 16bit ARGB image format to 32bit RGBA
			for (j = 0; j < 256; j++) 
			{
				for (k = 0; k < 256; k++) 
				{
					index = (j * 256) + k;
					offset = _textile16[texture].tile[index];
					index = (j * 1024) + (k * 4);
					
					image[index + 0] = ((offset >> 10) & 0x1f) * 8;
					image[index + 1] = ((offset >> 5) & 0x1f) * 8;
					image[index + 2] = (offset & 0x1f) * 8;
					image[index + 3] = (offset & 0x8000) ? 0xFF : 0;
				}
			}
		}

		switch (Engine())
		{
		case TR_VERSION_4:
		case TR_VERSION_3: // Account for alpha flags
			for (j = 0; j < (int)_num_object_textures; j++) 
			{
				// FIXME: This kind of works for lighting - but messes up lara
#ifdef FIXME
				if (_object_textures[j].tile == texture &&
					 _object_textures[j].transparency_flags == 1)
				{
					xmin = 999;						
					xmax = 0;
					ymin = 999;
					ymax = 0;
					y = 4;

					// Account for triangles
					if (_object_textures[j].vertices[3].xpixel == 0 &&
						 _object_textures[j].vertices[3].ypixel == 0)
						y = 3;

					for (k = 0; k < y; k++)
					{
						if (_object_textures[j].vertices[k].xpixel > xmax)
							xmax = _object_textures[j].vertices[k].xpixel;

						if (_object_textures[j].vertices[k].xpixel < xmin)
							xmin = _object_textures[j].vertices[k].xpixel;

						if (_object_textures[j].vertices[k].ypixel > ymax)
							ymax = _object_textures[j].vertices[k].ypixel;
	    
						if (_object_textures[j].vertices[k].ypixel < ymin)
							ymin = _object_textures[j].vertices[k].ypixel;
					}
               
					for (x = xmin; x <= xmax; x++) 
					{
						for (y = ymin; y <= ymax; y++) 
						{
							index = (y * 256) + x;
							offset = _textile16[texture].tile[index];
							index = (y * 1024) + (x * 4);

							image[index + 0] = ((offset >> 10) & 0x1f) * 8;
							image[index + 1] = ((offset >> 5) & 0x1f) * 8;
							image[index + 2] = (offset & 0x1f) * 8;

							// Set transparency to full
							if (offset & 0x8000)
							{
								image[index + 3] = 0x00;
							}
						}
					}					
				}
				else 
#endif
					if (_object_textures[j].tile == texture &&
						 _object_textures[j].transparency_flags == 2)
				{
					xmin = 999;						
					xmax = 0;
					ymin = 999;
					ymax = 0;
					y = 4;

					// Account for triangles
					if (_object_textures[j].vertices[3].xpixel == 0 &&
						 _object_textures[j].vertices[3].ypixel == 0)
						y = 3;

					for (k = 0; k < y; k++)
					{
						if (_object_textures[j].vertices[k].xpixel > xmax)
							xmax = _object_textures[j].vertices[k].xpixel;

						if (_object_textures[j].vertices[k].xpixel < xmin)
							xmin = _object_textures[j].vertices[k].xpixel;

						if (_object_textures[j].vertices[k].ypixel > ymax)
							ymax = _object_textures[j].vertices[k].ypixel;
	    
						if (_object_textures[j].vertices[k].ypixel < ymin)
							ymin = _object_textures[j].vertices[k].ypixel;
					}
               
					for (x = xmin; x <= xmax; x++) 
					{
						for (y = ymin; y <= ymax; y++) 
						{
							if (_textile32)
							{
								index = (y * 256) + x;
								color = _textile32[texture].tile[index];
								index = (y * 1024) + (x * 4);

								image[index + 2] = *((unsigned char *)(&color));
								image[index + 1] = *((unsigned char *)(&color)+1);
								image[index + 0] = *((unsigned char *)(&color)+2);
								image[index + 3] = *((unsigned char *)(&color)+3);

								k = image[index] + image[index + 1] + image[index + 2];
								
								// Set transparency based upon intensity
								image[index + 3] =  (k / 3);						
							}
							else
							{
								index = (y * 256) + x;
								offset = _textile16[texture].tile[index];
								index = (y * 1024) + (x * 4);
								
								image[index + 0] = ((offset >> 10) & 0x1f) * 8;
								image[index + 1] = ((offset >> 5) & 0x1f) * 8;
								image[index + 2] = (offset & 0x1f) * 8;
								image[index + 3] = (offset & 0x8000) ? 0xFF : 0;
								
								k = image[index] + image[index + 1] + image[index + 2];
								
								// Set transparency based upon intensity
								if (offset & 0x8000)
									image[index + 3] = (k / 3); 
								else
									image[index + 3] = 0;
							}
						}
					}
				}
			}
			break;
		default:
			;
		}
	}

	return image;
}



#ifdef __TOMBRAIDER_TEST__

void dump_textures(TombRaider *tr, char *mapname)
{
#ifdef __TEST_TR5_DUMP_TGA
	int i;
	unsigned char *image;
	unsigned char *bumpmap;
	char buffer[128];
	FILE *f;


	if (!tr || !mapname)
		return;
	
	// Dump textures
	printf("\n\t[Texture dumping for '%s']\n", mapname);

	for (i = 0; i < tr->NumTextures(); i++)
   {
		tr->Texture(i, &image, &bumpmap);

		if (image)
		{
			snprintf(buffer, 128, "/tmp/%s-%03i-texture.tga", mapname, i);

			f = fopen(buffer, "wb");

			if (f)
			{
				if (!mtk_image__tga_save(f, image, 256, 256, 4))
					printf("\tWrote texture %s\n", buffer);

				fclose(f);
			}

			delete [] image;
		}

		if (bumpmap)
		{
			snprintf(buffer, 64, "/tmp/%s-%03i-bumpmap.tga", mapname, i);
			
			f = fopen(buffer, "wb");
				
			if (f)
			{
				if (!mtk_image__tga_save(f, bumpmap, 256, 256, 4))
					printf("\tWrote texture %s\n", buffer);

				fclose(f);
			}

			delete [] bumpmap;
		}
	}

	for (i = 0; i < tr->NumSpecialTextures(); i++)
   {
		image = tr->SpecialTexTile(i);

		snprintf(buffer, 128, "/tmp/%s-%03i-special.tga", mapname, i);

		f = fopen(buffer, "wb");

		if (f)
		{
			if (!mtk_image__tga_save(f, image, 256, 256, 4))
				printf("\tWrote texture %s\n", buffer);

			fclose(f);
		}
		else
		{
			printf("\tFailed to write texture %s\n", buffer);
		}

		delete [] image;	
	}
#else
	printf("Texture dumping not in this build\n");
#endif
}


void dump_mesh(TombRaider *tr, char *mapname, int index)
{
	tr2_object_texture_t *object_texture = NULL;
	tr2_mesh_t *meshes = NULL;
	unsigned int v, check;
   int i, triangles, rectangles, t_index;
	char buffer[128];
	float rgba[4];
	float s, t;
	char id[8];
	FILE *f;
	

	if (!mapname || !tr)
	{
		return;
	}

	snprintf(buffer, 128, "/tmp/%s-%03i.mesh", mapname, index);

	object_texture = tr->ObjectTextures();
	meshes = tr->Mesh();


	f = fopen(buffer, "wb");

	if (!f)
	{
		perror("Failed to write mesh :");
		return;
	}
	
	// Setup header id and check points
	strncpy(id, "TRMESH", 7);
	id[7] = 0;
	check = 0xcdcdcdcd;
	

	fwrite(id, 8, 1, f);
	fwrite(&meshes[index].num_vertices, 2, 1, f);
	fwrite(&meshes[index].num_textured_triangles, 2, 1, f);
	fwrite(&meshes[index].num_textured_rectangles, 2, 1, f);
	fwrite(&meshes[index].num_coloured_triangles, 2, 1, f);
	fwrite(&meshes[index].num_coloured_rectangles, 2, 1, f);
	fwrite(&meshes[index].collision_size, 4, 1, f);


   // Textured triangles ////////////////////////
	fwrite(&check, 4, 1, f);
	triangles = meshes[index].num_textured_triangles;

	for (i = 0; triangles > 0 && i < triangles; i++)
   {
		t_index = meshes[index].textured_triangles[i].texture;

		// Store texture info
		fwrite(&object_texture[t_index].tile, 2, 1, f);
		fwrite(&object_texture[t_index].transparency_flags, 2, 1, f);

		// Store vertices
		v = meshes[index].textured_triangles[i].vertices[0];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].textured_triangles[i].vertices[1];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].textured_triangles[i].vertices[2];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		// Store texels
		s = tr->AdjustTexel(object_texture[t_index].vertices[0].xpixel,
								  object_texture[t_index].vertices[0].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[0].ypixel,
								  object_texture[t_index].vertices[0].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
		
		
		s = tr->AdjustTexel(object_texture[t_index].vertices[1].xpixel,
								  object_texture[t_index].vertices[1].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[1].ypixel,
								  object_texture[t_index].vertices[1].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
		
		s = tr->AdjustTexel(object_texture[t_index].vertices[2].xpixel,
								  object_texture[t_index].vertices[2].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[2].ypixel,
								  object_texture[t_index].vertices[2].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
	}

	fwrite(&check, 4, 1, f);


   // Textured rectangles ////////////////////////
	fwrite(&check, 4, 1, f);
	rectangles = meshes[index].num_textured_rectangles;

	for (i = 0; rectangles > 0 && i < rectangles; i++)
   {	
		t_index = meshes[index].textured_rectangles[i].texture;

		// Store texture info
		fwrite(&object_texture[t_index].tile, 2, 1, f);
		fwrite(&object_texture[t_index].transparency_flags, 2, 1, f);

		// Store vertices
		v = meshes[index].textured_rectangles[i].vertices[0];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].textured_rectangles[i].vertices[1];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].textured_rectangles[i].vertices[2];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].textured_rectangles[i].vertices[3];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		// Store texels
		s = tr->AdjustTexel(object_texture[t_index].vertices[0].xpixel,
								  object_texture[t_index].vertices[0].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[0].ypixel,
								  object_texture[t_index].vertices[0].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
		
		
		s = tr->AdjustTexel(object_texture[t_index].vertices[1].xpixel,
								  object_texture[t_index].vertices[1].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[1].ypixel,
								  object_texture[t_index].vertices[1].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
		
		s = tr->AdjustTexel(object_texture[t_index].vertices[2].xpixel,
								  object_texture[t_index].vertices[2].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[2].ypixel,
								  object_texture[t_index].vertices[2].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);

		s = tr->AdjustTexel(object_texture[t_index].vertices[3].xpixel,
								  object_texture[t_index].vertices[3].xcoordinate);
		t = tr->AdjustTexel(object_texture[t_index].vertices[3].ypixel,
								  object_texture[t_index].vertices[3].ycoordinate);
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
	}

	fwrite(&check, 4, 1, f);

   // Coloured triangles ////////////////////////
	fwrite(&check, 4, 1, f);
	triangles = meshes[index].num_coloured_triangles;

	for (i = 0; triangles > 0 && i < triangles; i++)
   {
		// Store vertices
		v = meshes[index].coloured_triangles[i].vertices[0];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].coloured_triangles[i].vertices[1];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].coloured_triangles[i].vertices[2];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		// Store color
		switch (tr->Engine())
		{
		case TR_VERSION_1:
			tr->ColorLookUp(meshes[index].coloured_triangles[i].texture
								 & 0xff, rgba);
			break;
		default:
			tr->ColorLookUp((meshes[index].coloured_triangles[i].texture>>8)
								 & 0xff, rgba);
		}

		s = rgba[0];
		t = rgba[1];
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);

		s = rgba[2];
		t = rgba[3];
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
	}

	fwrite(&check, 4, 1, f);

   // Coloured rectangles ////////////////////////
	fwrite(&check, 4, 1, f);
	rectangles = meshes[index].num_coloured_rectangles;

	for (i = 0; rectangles > 0 && i < rectangles; i++)
   {
		// Store vertices
		v = meshes[index].coloured_rectangles[i].vertices[0];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].coloured_rectangles[i].vertices[1];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].coloured_rectangles[i].vertices[2];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		v = meshes[index].coloured_rectangles[i].vertices[3];
		fwrite(&meshes[index].vertices[v].x, 2, 1, f);
		fwrite(&meshes[index].vertices[v].y, 2, 1, f);
		fwrite(&meshes[index].vertices[v].z, 2, 1, f);

		// Store color
		switch (tr->Engine())
		{
		case TR_VERSION_1:
			tr->ColorLookUp(meshes[index].coloured_rectangles[i].texture
								 & 0xff, rgba);
			break;
		default:
			tr->ColorLookUp((meshes[index].coloured_rectangles[i].texture>>8)
								 & 0xff, rgba);
		}

		s = rgba[0];
		t = rgba[1];
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);

		s = rgba[2];
		t = rgba[3];
		fwrite(&s, 4, 1, f);
		fwrite(&t, 4, 1, f);
	}

	fwrite(&check, 4, 1, f);
	fclose(f);

	printf(".");
	fflush(stdout);	
}


void percent_callback(int p)
{
	printf("Level %i%% loaded\n", p);
}


int test_main(int argc, char *argv[])
{
  TombRaider tr;
  char mapname[128];
  int len, i, j;
 

  printf("[TombRaider class test]\n");

  tr.Debug(true);

  if (argc > 2)
  {
	  // Strip for mapname //////////////////////////////
	  len = strlen(argv[2]);
	  
	  for (i = len, j = 0; i > 0; i--, j++)
		  if (argv[2][i] == '/' || argv[2][i] == '\\')
			  break;
	  
	  j--;
	  
	  memset(mapname, 0, 128);
	  
	  for (i = 0; i < len - j && i < 30; i++)
		  mapname[i] = argv[2][i + len - j];
	  
	  ////////////////////////////////////////////////////


    if (strncmp(argv[1], "load", 4) == 0)
    {
      if (!tr.Load(argv[2], percent_callback))
		{
        printf("\nmain: Load reports success.\n");
		}
    }
    else if (strncmp(argv[1], "dump", 4) == 0)
    {
      if (!tr.Load(argv[2], percent_callback))
		{
        printf("\nmain: Load reports success.\n");

		  dump_textures(&tr, mapname);

		  printf("Mesh dumping: ");
		  for (i = 0; i < tr.NumMeshes(); i++)
			  dump_mesh(&tr, mapname, i);
		  printf("\n");
		}
		else
		{
			printf("\nmain: Load failed.\n");
		}
    }
    else
      printf("\n\n%s [ load | dump ] filename\n", argv[0]);
  }
  else
  {
    printf("\n\n%s [ load | dump ] filename\n", argv[0]);
  }

  return 0;
}


int main(int argc, char *argv[])
{
	test_main(argc, argv);

#ifdef DEBUG_MEMEORY
  dump_memory_report();
#endif

  return 0;
}
#endif
