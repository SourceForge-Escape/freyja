/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : OpenRaider
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : OpenRaider
 * License : GPL, See file COPYING for details
 * Comments: Loads TR 1, 2, 3, and 4 maps, meshes, and textures
 *
 *           WARNING: No endian routines as of yet
 *
 *           Define ZLIB_SUPPORT to build TR4 support
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.06.19:
 * Mongoose - New texture API for the new TR4/TR5 bump map 
 *            support, also purged textile exposure
 *
 *
 * 2001.05.21:
 * Mongoose - Added to project OpenRaider, more documentation
 *            than Freyja version I wrote ( 3d modeler )
 *
 *            TR4 texel and texture fixes
 *
 *            Runtime debug output toggle
 *
 *
 * 2000.05.13:
 * Mongoose - Added gcc and VC++ pragmas for packing
 *
 *            id style typedefs for structs
 *
 *            Heavy clean up and ported to C++
 *
 *            I saved yuri's notes as best I could and 
 *            reformatted and corrected as needed.
 *
 * Mongoose - Created, based on:
 *                tr_view's tr2io.c by Yuri Zhivago, PhD,
 *                TR Rosetta Stone ( TombRaider pak format specs )
 ================================================================*/

#ifndef __OPENRAIDER_MONGOOSE_TOMBRAIDER_H_
#define __OPENRAIDER_MONGOOSE_TOMBRAIDER_H_

#ifdef WIN32
// Have to remove GCC packing, by removing in preprocessor
#define __attribute__(packed)

// MSVC++ pragma to pack structs
#define TR2_H 1
#pragma pack(push, tr2_h, 1)
#endif


typedef enum 
{
  TR_VERSION_UNKNOWN,
  TR_VERSION_1,
  TR_VERSION_2,
  TR_VERSION_3,
  TR_VERSION_4,
  TR_VERSION_5

} tr2_version_type;


typedef enum
{
	TR_FREAD_NORMAL = 0,
	TR_FREAD_COMPRESSED

} tr_fread_mode_t;


#define TR_ANIAMTION_RUN      0
#define TR_ANIAMTION_STAND   11

/*---------------------------------------------------------------
 * Basic 24-bit colour structure 
 * (although it appears that only 6 bits
 * per colour are actually used, making it 18-bit colour) 
 ---------------------------------------------------------------*/
typedef struct tr2_colour_s 
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
}  __attribute__ ((packed)) tr2_colour_t;


/*--------------------------------------------------------------
 * Basic vertex structure.
 * Note that all vertices are relative coordinates;
 * each mesh (room, object, etc.) has its own offset by
 * which the vertex coordinates are translated.
 --------------------------------------------------------------*/
typedef struct tr2_vertex_s 
{
  short x;
  short y;
  short z;
}  __attribute__ ((packed))   tr2_vertex_t;


/*--------------------------------------------------------------
 * A rectangular (quad) face definition.  
 * Four vertices (the values are indices into the 
 * appropriate vertex list) and a texture (an index
 * into the texture list) or colour (index into 8-bit palette).
 *
 * I've seen a few coloured polygons where Texture is 
 * greater than 255, but ANDing the value with 0xFF 
 * seems to produce acceptable results.
 --------------------------------------------------------------*/
typedef struct tr2_quad_s /* was tr2_face4  */
{
  unsigned short vertices[4];
  unsigned short texture;
}  __attribute__ ((packed))   tr2_quad_t;

typedef struct tr4_quad_s
{
	unsigned short vertices[4];  // The 4 vertices of a quad
	unsigned short texture;      // Object-texture index 
	unsigned short lighting;     // transparency flag & strength of 
	                             // the hilight
	// Bit 0: if set, alpha channel = intensity (same meaning that when the Attribute field of tr2_object_texture is 2. Cf TRosetta stone document)
	// Bit 1-7: strength of the hilight. In TR4, objects can exhibit some kind of light reflection when seen from some particular angles. These bits give the strength of this effect: the more bigger the value is, the more visible is the effect.
}  __attribute__ ((packed))   tr4_quad_t;

/*--------------------------------------------------------------
 * A triangular face definition.  
 * Three vertices (the values are indices into the 
 * appropriate vertex list) and a texture (an index into the
 * texture list) or colour (index into palette).
 *
 * In the case of a colour, (Texture & 0xff) is the index 
 * into the 8-bit palette, while (Texture >> 8) is 
 * the index into the 16-bit palette.
 --------------------------------------------------------------*/
typedef struct tr2_tri_s /* was tr2_face3  */
{
  unsigned short vertices[3];
  unsigned short texture;
}  __attribute__ ((packed))   tr2_tri_t;

typedef struct tr4_tri_s
{
	unsigned short vertices[3];  // The 3 vertices of a tri 
	unsigned short texture;      // Object-texture index 
	unsigned short lighting;     // transparency flag & strength of 
	                             // the hilight
}  __attribute__ ((packed))   tr4_tri_t;

/*--------------------------------------------------------------
 * An 8-bit texture tile (65536 bytes).  
 *
 * Each byte represents a pixel whose colour 
 * is in the 8-bit palette.
 --------------------------------------------------------------*/
typedef struct tr2_textile8_s 
{
  unsigned char tile[256 * 256];
}  __attribute__ ((packed))   tr2_textile8_t;


/*--------------------------------------------------------------
 * A 16-bit texture tile (131072 bytes).  
 *
 * Each word represents a pixel
 * whose colour is of the form ARGB, MSB-to-LSB:
 *
 *    1-bit transparency (0: transparent, 1: opaque)
 *    5-bit red channel
 *    5-bit green channel
 *    5-bit blue channel
 --------------------------------------------------------------*/
typedef struct tr2_textile16_s
{ 
   unsigned short tile[256 * 256];
}   __attribute__ ((packed))  tr2_textile16_t;


/* --------------------------------------------------------------
 * 32bit textiles
 * BGRA with 4bytes each channel
 --------------------------------------------------------------*/
typedef struct tr2_textile32_s
{ 
   unsigned int tile[256 * 256];
}   __attribute__ ((packed))  tr2_textile32_t;


/*--------------------------------------------------------------
 * The "header" of a room.  
 * + X/Z indicate the base position of the room
 *   mesh in world coordinates.  
 * + YLowest and yHighest indicate the lowest and
 * + Highest points in this room 
 *    (even though their actual values appear to
 *     be reversed, since a "high" point will have a 
 *     smaller value than a "low" point).  
 *
 * When positioning objects/items, as well as the room meshes
 * themselves, y is always 0-relative (not room-relative).
 --------------------------------------------------------------*/
typedef struct tr2_room_info_s
{ 
  int x;             // X-offset of room (world coordinates)
  int z;             // Z-offset of room (world coordinates)
  int y_bottom;      // Y-offset of lowest point in room (world coordinates)
                     //   (actually highest value)
  int y_top;         // Y-offset of highest point in room (world coordinates)
                     //   (actually lowest value)
}  __attribute__ ((packed))   tr2_room_info_t;


/*--------------------------------------------------------------
 * Portal structure.  
 * This defines every viable exit from a given "room".
 *
 * Note that "rooms" are really just areas.  They aren't 
 * necessarily enclosed.  The door structure below defines 
 * areas of egress, not the actual moveable mesh, 
 * texture, and action (if any).
 --------------------------------------------------------------*/
typedef struct tr2_room_portal_s 
{ 
   unsigned short adjoining_room;     // Which room this "door" leads to
   tr2_vertex_t normal;               // Which way the "door" faces
   tr2_vertex_t vertices[4];          // The corners of the "door"
}  __attribute__ ((packed))   tr2_room_portal_t;


/*--------------------------------------------------------------
 * Room sector structure.
 *
 * Sectors are 1024 * 1024 (world coordinates).  Floor and 
 * Ceiling are signed number of 256 units of height 
 * (relative to 0), e.g. Floor 0x04 corresponds to
 * Y = 1024 in world coordinates.  
 *
 * Note: this implies that, while X and Z can be quite large, 
 * Y is constrained to -32768..32512.  Floor/Ceiling value of
 * 0x81 indicates impenetrable wall.
 *
 * Floor values are used by the game engine to determine what 
 * objects Lara can traverse and how.  
 *
 * Relative steps of 1 (-256) can be walked up;  
 * steps of 2..7 (-512..-1792) can/must be climbed;  
 * steps larger than 7 (-2048..-32768) cannot be climbed (too tall)
 *
 * In TR3, BoxIndex is more complicated. Only bits 4-14 are the
 * "real" index; bits 0-3 are most likely some kind of flag, 
 * such as what kind of footstep sound to make (wood, metal, snow). 
 * Furthermore, there is a special value of the "real" index, 
 * 2047, or 0x7ff.
 *     
 * RoomAbove and RoomBelow indicate what neighboring rooms are
 * in these directions; if RoomAbove is not "none", then the
 * ceiling is a collisional portal to that room, while if 
 * RoomBelow is not "none", then the floor is a collisional
 * portal to that room.
 --------------------------------------------------------------*/
typedef struct tr2_room_sector_s
{ 
	unsigned short fd_index;    // Index into FloorData[] 
	unsigned short box_index;   // Index into Boxes[]/Zones[] ( -1 if none ) 
	unsigned char room_below;   // The number of the room below this one 
	                            // ( -1 or 255 if none )
	char floor;                 // Absolute height of floor 
	                            // ( Multiply by 256 for world coordinates )
	unsigned char  room_above;  // The number of the room above this one 
	                            // ( -1 or 255 if none ) 
	char ceiling;               // Absolute height of ceiling 
	                            // ( Multiply by 256 for world coordinates )
}  __attribute__ ((packed))   tr2_room_sector_t;


/*--------------------------------------------------------------
 * Room lighting structure.  
 * X/Y/Z are in world coordinates.
 * Lighting values seem to range from 0..8192.
 --------------------------------------------------------------*/
typedef struct tr2_room_light_s 
{
  int  x;
  int  y;
  int  z;
  unsigned short intensity1;
  unsigned short intensity2;
  unsigned int fade1;
  unsigned int fade2;
}   __attribute__ ((packed))  tr2_room_light_t;

typedef struct tr4_room_light_s 
{ 
	int x_position;  // world coords 
	int y_position;  // world coords 
	int z_position;  // world coords 
   tr2_colour_t color; // three bytes rgb values 
   unsigned char light_type;  // same as D3D (i.e. 2 is for spotlight) 
   unsigned char unknown;    // always 0xff? 
   unsigned char intensity;  
   float in;  
	float out;  
	float length;  
	float cutoff;  
	float x, y, z;    // direction

}   __attribute__ ((packed))  tr4_room_light_t;


/*--------------------------------------------------------------
 * Room vertex structure.  
 * This defines the vertices within a room.
 --------------------------------------------------------------*/
typedef struct tr2_vertex_room_s
{
   tr2_vertex_t vertex;
                               // Following 3 entries
   short lighting1;            // Values range from 0 to 32767, 
                               //   0 = total darkness. (TR3)
                               //   I think the values ranged from 
                               //   0 to 8192 in TR1/2, 0=total brightness

   unsigned short attributes;  // 0x8000 Something to do with water surface
                               // 0x4000 Under water lighting modulation
                               //        and movement if viewed from 
                               //        above water surface
                               // 0x2000 Water/quicksand surface movement
                               // 0x1fef Nothing?
                               // 0x0010 Everything?

   short lighting2;            // Seems to be the same as lighting1
}  __attribute__ ((packed))  tr2_vertex_room_t;


/*--------------------------------------------------------------
 * Sprite structure
 --------------------------------------------------------------*/
typedef struct tr2_room_sprite_s 
{ 
   short vertex;     // offset into vertex list
   short texture;    // offset into texture list
}  __attribute__ ((packed))   tr2_room_sprite_t;


/*--------------------------------------------------------------
 * Room mesh structure.  
 * This is the geometry of the "room," including
 * walls, floors, rocks, water, etc.  It does _not_ include 
 * objects that Lara can interact with (keyboxes, 
 * moveable blocks, moveable doors, etc.)
 --------------------------------------------------------------*/
typedef struct tr2_room_data_s 
{
  short           num_vertices;    // number of vertices in the following list
  tr2_vertex_room_t *vertices;     // list of vertices (relative coordinates)
  short           num_rectangles;  // number of textured rectangles
  tr2_quad_t       *rectangles;    // list of textured rectangles
  short           num_triangles;   // number of textured triangles
  tr2_tri_t      *triangles;       // list of textured triangles
  short           num_sprites;     // number of sprites
  tr2_room_sprite_t *sprites;      // list of sprites
}  __attribute__ ((packed)) tr2_room_data_t;


/*--------------------------------------------------------------
 * Room static mesh data.  
 * Positions and IDs of static meshes 
 * (e.g. skeletons, spiderwebs, furniture)
 --------------------------------------------------------------*/
typedef struct tr2_room_staticmesh_s 
{ 
   int  x;                   // absolute position in world coordinates
   int  y;
   int  z;
   unsigned short rotation;  // high two bits (0xc000) 
                             //  indicate steps of 90 degrees
   unsigned short intensity1;
   unsigned short intensity2;
   unsigned short object_id; // which StaticMesh item to draw
}  __attribute__ ((packed)) tr2_room_staticmesh_t;


/*--------------------------------------------------------------
 * Room structure.  
 * Here's where all the room data come together.
 --------------------------------------------------------------*/
typedef struct tr2_room_s
{
  tr2_room_info_t info;          // where the room exists, in world coordinates
  unsigned int num_data_words;   // number of data words (bitu16)
  unsigned char *data;           // the raw data from which the rest 
                                 //  of this is derived
  tr2_room_data_t room_data;     // the room mesh
  unsigned short num_portals;    // number of visibility portals
                                 //  that leave this room
  tr2_room_portal_t *portals;    // list of visibility portals
  unsigned short num_zsectors;   // width of sector list
  unsigned short num_xsectors;   // height of sector list
  tr2_room_sector_t *sector_list;// list of sectors in this room
  short intensity1;
  short intensity2;
  short light_mode;
  unsigned short num_lights;            // number of lights in this room
  tr2_room_light_t *lights;             // list of lights
  unsigned short num_static_meshes;     // number of static meshes
  tr2_room_staticmesh_t *static_meshes; // static meshes
  short  alternate_room;
  short  flags;                         // 0x0001 - room is filled with water
                                        // 0x0020 - Lara's ponytail 
                                        // gets blown by the wind
  tr2_colour_t room_light_colour;       // TR3 ONLY!
}  __attribute__ ((packed))  tr2_room_t;

/*--------------------------------------------------------------
 * Animation structure.
 --------------------------------------------------------------*/
typedef struct tr2_animation_s 
{
  unsigned int frame_offset;    // byte offset into Frames[] 
                                //   (divide by 2 for Frames[i])
  unsigned char frame_rate;     // "ticks" per frame
  unsigned char frame_size;     // number of words in Frames[] 
                                //   used by this animation
  short state_id;
  short unknown1;
  short unknown2;
  short unknown3;
  short unknown4;

  unsigned short frame_start;   // first frame in this animation
  unsigned short frame_end;     // last frame in this animation 
                                //  (numframes = (End - Start) + 1)
  unsigned short next_animation;
  unsigned short next_frame;
  unsigned short num_state_changes;

  unsigned short state_change_offset;  // offset into StateChanges[]
  unsigned short num_anim_commands;
  unsigned short anim_command;         // offset into AnimCommands[]
}  __attribute__ ((packed))   tr2_animation_t;

// Data for an animation structure (40 bytes vice 32 in TR1/2/3)
typedef struct tr4_animation_s 
{
	unsigned int frame_offset;// same meaning than in TR3
	unsigned char frame_rate;// same meaning than in TR3
	unsigned char frame_size;// same meaning than in TR3
	unsigned short state_id;// same meaning than in TR3
	short unknown;// same meaning than in TR3
	short speed;// same meaning than in TR3
	unsigned short accel_lo;// same meaning than in TR3
	short accel_hi;// same meaning than in TR3
	unsigned char unknown2[8];// new in TR4

	unsigned short frame_start;// same meaning than in TR3
	unsigned short frame_end;// same meaning than in TR3
	unsigned short next_animation;// same meaning than in TR3
	unsigned short next_frame;// same meaning than in TR3

	unsigned short num_state_changes;// same meaning than in TR3
	unsigned short state_change_offset;// same meaning than in TR3
	unsigned short num_anim_commands;// same meaning than in TR3
	unsigned short anim_command;// same meaning than in TR3
}  __attribute__ ((packed))   tr4_animation_t;

/*--------------------------------------------------------------
 * State Change structure
 --------------------------------------------------------------*/
typedef struct tr2_state_change_s
{
  unsigned short state_id;
  unsigned short num_anim_dispatches;  // number of dispatches 
                                       //  (seems to always be 1..5)
  unsigned short anim_dispatch;        // Offset into AnimDispatches[]
}  __attribute__ ((packed))   tr2_state_change_t;


/*--------------------------------------------------------------
 * Animation Dispatch structure
 --------------------------------------------------------------*/
typedef struct tr2_anim_dispatch_s 
{
  short low;
  short high;
  short next_animation;
  short next_frame;
}   __attribute__ ((packed))  tr2_anim_dispatch_t;


/*--------------------------------------------------------------
 * AnimCommand structure
 --------------------------------------------------------------*/
typedef struct tr2_anim_command_s
{
   short value;
}  __attribute__ ((packed))   tr2_anim_command_t;


/*--------------------------------------------------------------
 * MeshTree structure
 *
 * MeshTree[] is actually groups of four ints.  The first one is a
 * "flags" word;  bit 1 (0x0002) indicates "make previous mesh an
 * anchor (e.g. PUSH)";  bit 0 (0x0001) indicates "return to previous
 * anchor (e.g. POP)".
 * The next three ints are X, Y, Z offsets from the last mesh position.
 --------------------------------------------------------------*/
typedef struct tr2_meshtree_s 
{
  int flags;   // 0x0001 = POP, 0x0002 = PUSH
  int x;
  int y;
  int z;
}  __attribute__ ((packed))  tr2_meshtree_t;


/*--------------------------------------------------------------
 * Moveable structure.  This defines a list of contiguous meshes that
 * comprise one object, e.g. in WALL.TR2,
 *    moveable[0]  = Lara (StartingMesh 0, NumMeshes 15),
 *    moveable[13] = Tiger (StartingMesh 215, NumMeshes 27)
 *    moveable[15] = Spikes (StartingMesh 249, NumMeshes 1)
 *    moveable[16] = Boulder (StartingMesh 250, NumMeshes 1)
 *    moveable[20] = Rolling Blade (StartingMesh 254, NumMeshes 1)
 --------------------------------------------------------------*/
typedef struct tr2_moveable_s
{ 
  unsigned int object_id;        // Item Identifier
  unsigned short num_meshes;     // number of meshes in this object
  unsigned short starting_mesh;  // first mesh
  unsigned int mesh_tree;        // offset into MeshTree[]
  unsigned int frame_offset;     // byte offset into Frames[] 
                                 //  (divide by 2 for Frames[i])
  unsigned short animation;      // offset into Animations[]
}  __attribute__ ((packed))  tr2_moveable_t;


/*--------------------------------------------------------------
 * StaticMesh structure.  
 *
 * This defines meshes that don't move (e.g. skeletons
 * lying on the floor, spiderwebs, etc.)
 --------------------------------------------------------------*/
typedef struct tr2_staticmesh_s
{
  unsigned int object_id;             // Item Identifier
  unsigned short starting_mesh;       // first mesh
  tr2_vertex_t bounding_box[2][2];
  unsigned short flags;
}  __attribute__ ((packed))  tr2_staticmesh_t;


/*--------------------------------------------------------------
 * Object texture vertex structure.  
 *
 * Maps coordinates into texture tiles.
 --------------------------------------------------------------*/
typedef struct tr2_object_texture_vert_s 
{
  unsigned char xcoordinate;
  unsigned char xpixel;
  unsigned char ycoordinate;
  unsigned char ypixel;
}  __attribute__ ((packed))  tr2_object_texture_vert_t;


/*--------------------------------------------------------------
 * Object texture structure.
 --------------------------------------------------------------*/
typedef struct tr2_object_texture_s
{
  unsigned short transparency_flags;  // 0: Opaque 
                                      // 1: Use transparency
                                      // 2: Use partial transparency 
                                      //    [grayscale intensity :: transparency]

  unsigned short tile;                     // index into textile list
  tr2_object_texture_vert_t vertices[4];   // the four corners of the texture
}  __attribute__ ((packed))   tr2_object_texture_t;


/*--------------------------------------------------------------
 * Sprite texture structure.
 --------------------------------------------------------------*/
typedef struct tr2_sprite_texture_s
{
  unsigned short tile;
  unsigned char  x;
  unsigned char  y;
  unsigned short width;     // actually, (width * 256) + 255
  unsigned short height;    // actually, (height * 256) + 255
  short  left_side;
  short  top_side;
  short  right_side;
  short  bottom_side;
}  __attribute__ ((packed))   tr2_sprite_texture_t;


/*--------------------------------------------------------------
 * Sprite Sequence structure
 --------------------------------------------------------------*/
typedef struct tr2_sprite_sequence_s
{
  int object_id;         // Item identifier (same numbering as in tr2_moveable)
  short negative_length; // negative of "how many sprites are in this sequence"
  short offset;          // where (in sprite texture list) this sequence starts
}  __attribute__ ((packed))   tr2_sprite_sequence_t;


/*--------------------------------------------------------------
 * Mesh structure.
 *  
 * The mesh list contains the mesh info for Lara (in all her
 * various incarnations), blocks, enemies (tigers, birds, bad guys), 
 * moveable blocks, zip line handles, boulders, spinning blades, 
 * you name it.
 *
 * If NumNormals is negative, Normals[] represent vertex 
 * lighting values (one per vertex).
 --------------------------------------------------------------*/
typedef struct tr2_mesh_s
{
  tr2_vertex_t centre;       // this seems to describe the 
                             //  approximate geometric centre
                             //  of the mesh (possibly the centre of gravity?)
                             // (relative coordinates, just like the vertices)
  int      collision_size;   // radius of collisional sphere
  short     num_vertices;    // number of vertices in this mesh
  tr2_vertex_t *vertices;    // list of vertices (relative coordinates)
  short     num_normals;     // number of normals in this mesh 
                             //  (should always equal NumVertices)
  tr2_vertex_t *normals;     // list of normals (NULL if NumNormals < 0)
  short      *mesh_lights;   // if NumNormals < 0

  short num_textured_rectangles;   // number of textured rectangles 
  tr2_quad_t *textured_rectangles; // list of textured rectangles

  short num_textured_triangles;    // number of textured triangles in this mesh
  tr2_tri_t  *textured_triangles;  // list of textured triangles

  short  num_coloured_rectangles;  // number of coloured rectangles 
  tr2_quad_t *coloured_rectangles; // list of coloured rectangles

  short    num_coloured_triangles; // number of coloured triangles in this mesh
  tr2_tri_t  *coloured_triangles;  // list of coloured triangles
}  __attribute__ ((packed))   tr2_mesh_t;


/*--------------------------------------------------------------
 * Frame structure.
 *
 * Frames indicates how composite meshes are positioned and rotated.  
 * They work in conjunction with Animations[] and Bone2[].
 *  
 * A given frame has the following format:
 *    short BB1x, BB1y, BB1z           // bounding box (low)
 *    short BB2x, BB2y, BB2z           // bounding box (high)
 *    short OffsetX, OffsetY, OffsetZ  // starting offset for this moveable
 *    (TR1 ONLY: short NumValues       // number of angle sets to follow)
 *    (TR2/3: NumValues is implicitly NumMeshes (from moveable))
 *   
 * What follows next is a list of angle sets.  In TR2/3, an angle set can
 * specify either one or three axes of rotation.  If either of the high two
 * bits (0xc000) of the first angle unsigned short are set, it's one axis:
 *  only one  unsigned short, 
 *  low 10 bits (0x03ff), 
 *  scale is 0x100 == 90 degrees;  
 * the high two  bits are interpreted as follows:  
 *  0x4000 == X only, 0x8000 == Y only,
 *  0xC000 == Z only.
 *   
 * If neither of the high bits are set, it's a three-axis rotation.  The next
 * 10 bits (0x3ff0) are the X rotation, the next 10 (including the following
 * unsigned short) (0x000f, 0xfc00) are the Y rotation, 
 * the next 10 (0x03ff) are the Z rotation, same scale as 
 * before (0x100 == 90 degrees).
 *
 * Rotations are performed in Y, X, Z order.
 * TR1 ONLY: All angle sets are two words and interpreted like the two-word
 * sets in TR2/3, EXCEPT that the word order is reversed.
 --------------------------------------------------------------*/
typedef struct tr2_frame_s 
{
  tr2_vertex_t vector[3];
  int        num_words;
  unsigned short     *words;
}  __attribute__ ((packed))   tr2_frame_t;


/*--------------------------------------------------------------
 * Item structure
 --------------------------------------------------------------*/
typedef struct tr2_item_s
{
  short object_id;
  short room;
  int x;
  int y;
  int z;
  short angle;
  short intensity1;
  short intensity2;
  short flags;   // 0x0100 indicates "inactive" or "invisible"
}  __attribute__ ((packed))   tr2_item_t;


/*--------------------------------------------------------------
 * SoundSource structure
 --------------------------------------------------------------*/
typedef struct tr2_sound_source_s
{
  int x;                           // position of sound source
  int y;
  int z;
  unsigned short sound_id;         // internal sound index
  unsigned short flags;            // 0x40, 0x80, or 0xc0
}  __attribute__ ((packed))   tr2_sound_source_t;


/*--------------------------------------------------------------
 * Boxes structure
 --------------------------------------------------------------*/
typedef struct tr2_boxes_s
{
  unsigned char zmin;             // sectors (* 1024 units)
  unsigned char zmax;
  unsigned char xmin;
  unsigned char xmax;
  short true_floor;        // Y value (no scaling)
  short overlap_index;     // index into Overlaps[]
}  __attribute__ ((packed))   tr2_box_t;


/*--------------------------------------------------------------
 * AnimatedTexture structure
 * - really should be simple short[], since it's variable length
 --------------------------------------------------------------*/
typedef struct tr2_animated_texture_s
{
  short num_texture_ids;    // Number of Texture IDs - 1
  short *texture_list;      // list of textures to cycle through
}  __attribute__ ((packed))   tr2_animated_texture_t;


/*--------------------------------------------------------------
 * Camera structure
 --------------------------------------------------------------*/
typedef struct tr2_camera_s
{
  int x;
  int y;
  int z;
  short room;
  unsigned short unknown1;  // correlates to Boxes[]?
} __attribute__ ((packed))    tr2_camera_t;

// Data for a flyby camera (40 bytes)
typedef struct tr4_extra_camera_s
{
	int pos[6];  // Positions ? (x1,y1,z1,x2,y2,z2) roatations?
	unsigned char index[2]; // A pair of indices 
	unsigned short unknown[5];// ?? 
	int id ;// Index of something 
} __attribute__ ((packed))    tr4_extra_camera_t;


/*--------------------------------------------------------------
 * Sound sample structure
 --------------------------------------------------------------*/
typedef struct tr2_sound_details_s
{
  short sample;
  short volume;
  short sound_range;
  short flags;         // bits 8-15: priority?, 2-7: number of sound samples
                       // in this group, bits 0-1: channel number
} __attribute__ ((packed)) tr2_sound_details_t;


/*--------------------------------------------------------------
 * Cutscene Camera structure
 --------------------------------------------------------------*/
typedef struct tr2_cinematic_frame_s
{
  short roty;      // Rotation about Y axis, +/-32767 ::= +/- 180 degrees
  short rotz;      // Rotation about Z axis, +/-32767 ::= +/- 180 degrees
  short rotz2;     // Rotation about Z axis (why two?), 
                   //    +/-32767 ::= +/- 180 degrees
  short posz;      // Z position of camera, relative to something
  short posy;      // Y position of camera, relative to something
  short posx;      // X position of camera, relative to something
  short unknown1;
  short rotx;      // Rotation about X axis, +/-32767 ::= +/- 180 degrees
} __attribute__ ((packed)) tr2_cinematic_frame_t;


/*--------------------------------------------------------------
 * Used to read TR4 AI data
 *  this field replaces the bitu16 NumCinematicFrames of TR1/2/3 levels 
 *  Data for a AI object (24 bytes) 
 --------------------------------------------------------------*/
typedef struct tr4_ai_object_s
{ 
	unsigned short int object_id; // the objectID from the AI object 
	                              // (AI_FOLLOW is 402) 
	unsigned short int room;  
	int x, y, a;  
	unsigned short int ocb;  
	unsigned short int flags; // The trigger flags 
	                          // (button 1-5, first button has value 2)
	int angle;  // rotation 
} __attribute__ ((packed)) tr4_ai_object_t;


/*--------------------------------------------------------------
 * Used to read packed TR4 texels
 --------------------------------------------------------------*/
typedef struct tr4_object_texture_s
{
	unsigned short	attribute; // same meaning than in TR3 
	unsigned short tile;      // same meaning than in TR3 
	unsigned short flags;// new in TR4 
	tr2_object_texture_vert_t vertices[4];// same meaning than in TR3 
	unsigned int unknown1, unknown2;  // new in TR4: x & y offset in something
	unsigned int xsize,ysize;// new in TR4: width-1 & height-1 of the object texture
  
}  __attribute__ ((packed)) tr4_object_texture_t;


#ifdef WIN32
#pragma pack(pop, tr2_h, 1)
#endif

class TombRaider
{
 public:

  TombRaider();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Constructs an object of TombRaider
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  ~TombRaider();
  /*------------------------------------------------------
   * Pre  : TombRaider object is allocated
   * Post : Deconstructs an object of TombRaider
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumRooms();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumMoveables();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumTextures();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumSpecialTextures();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Returns number of "special" textures/images
   *
   *-- History ------------------------------------------
   *
   * 2001.06.19: 
   * Mongoose - Created
   ------------------------------------------------------*/


  int NumAnimations();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  unsigned int NumFrames();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumMeshes();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumStaticMeshes();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumSprites();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumSpriteSequences();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2001.06.06: 
   * Mongoose - Created
   ------------------------------------------------------*/

  int NumItems();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2001.06.06: 
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_version_type Engine();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void ColorLookUp(int index, float color[4]);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.30:
   * Mongoose - Created
   ------------------------------------------------------*/

  unsigned short *Frame();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_animation_t *Animation();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_item_t *Item();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_object_texture_t *ObjectTextures();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

	unsigned int getNumBoxes();

  tr2_box_t *Box();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_mesh_t *Mesh();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/


	int getNumAnimsForMoveable(int moveable_index);
  /*------------------------------------------------------
   * Pre  : moveable_id is valid
   * Post : Returns number of animations for a moveable
   *
   *-- History ------------------------------------------
   *
	* 2002.08.15:
	* Mongoose - Using signed values now ( like it should have )
	*
   * 2002.04.04:
   * Mongoose - Created, based on TRViewer algorithm
   ------------------------------------------------------*/


  tr2_staticmesh_t *StaticMesh();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_moveable_t *Moveable();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_meshtree_t *MeshTree();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_sprite_texture_t *Sprite();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Returns the sprite array
   *
   *-- History ------------------------------------------
   *
   * 2001.06.02: 
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_sprite_sequence_t *SpriteSequence();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2001.06.06: 
   * Mongoose - Created
   ------------------------------------------------------*/


  unsigned char *SpecialTexTile(int texture);
  /*------------------------------------------------------
   * Pre  : texture is valid index into tex_special list
	*
   * Post : Makes a 32bit RGBA image from a stexture/bmap 
	*        and returns it.  Returns NULL on error.
   *
   *-- History ------------------------------------------
   *
   * 2001.06.19:
   * Mongoose - Created
   ------------------------------------------------------*/


  void Texture(int texture, unsigned char **image, unsigned char **bumpmap);
  /*------------------------------------------------------
   * Pre  : texture is valid textile index
   * Post : Returns copies of texture and it's bumpmap
	*        if found, otherwise returns NULL
   *
   *-- History ------------------------------------------
   *
   * 2001.06.19: 
   * Mongoose - Created
   ------------------------------------------------------*/


  unsigned int *Palette16();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  unsigned char *Palette8();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  tr2_room_t *Room();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void Clear();
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void Print(char *s, ...);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void DebugPrint(char *s, ...);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/


  int Load(char *filename, void (*percent)(int));
  /*------------------------------------------------------
   * Pre  : filename is valid string and points
	*        to vaild TombRaider 1-5 pak
	*
   * Post : Loads TombRaider 1-5 pak into memory
	*        and does some processing
	*
	*        Returns 0 on sucess, and less than 0 on error
   *
	*        At 10% all textures are loaded
	*        the exact figure '10' will always be passed to
	*        allow for texture caching while meshes load
	*        for TR4
	*
   *-- History ------------------------------------------
   *
	* 2002.01.02:
	* Mongoose - New callback for percent loaded feedback
	*
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int SaveTR2(char *filename);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void Debug(bool set);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

	void ComputeRotationAngles(unsigned short **frame,
										unsigned int *frame_offset,
										unsigned int *angle_offset,
										float *x, float *y, float *z);
  /*------------------------------------------------------
   * Pre  : 
   * Post : Returns computed angles in x,y,z pointers
   *
   *-- History ------------------------------------------
   *
   * 2002.04.05:
   * Mongoose - Created
   ------------------------------------------------------*/

	void ComputeUV(tr2_object_texture_vert_t *st, float *u, float *v);
  /*------------------------------------------------------
   * Pre  : Args are valid pointers
   * Post : Returns computed UV in u and v
   *
   *-- History ------------------------------------------
   *
   * 2002.04.05:
   * Mongoose - Created
   ------------------------------------------------------*/

  float AdjustTexel(unsigned char texel, char offset);
  /*------------------------------------------------------
   * Pre  : Passed the correct coor/pixel pair from 
	*        object texture
	*
   * Post : Makes a 0.0 to 1.0 texel from pair
   *
   *-- History ------------------------------------------
   *
   * 2001.06.21: 
   * Mongoose - Created
   ------------------------------------------------------*/


 private:

	unsigned char *TexTile(int texture);
  /*------------------------------------------------------
   * Pre  : texture is valid index into textile list
	*
   * Post : Makes a 32bit RGBA image from a textile 
	*        and returns it.  Returns NULL on error.
	*
	*        This handles all selection and conversion
	*        including alpha layering flags now.
   *
   *-- History ------------------------------------------
   *
   * 2001.05.28:
   * Mongoose - Created
   ------------------------------------------------------*/

  int LoadTR5(FILE *f, void (*percent)(int));
  /*------------------------------------------------------
   * Pre  : f is a valid FILE and percent is allocated
   * Post : Loads a TR5 pak into memory
	*
	*        At 10% all textures are loaded
	*        the exact figure '10' will always be passed to
	*        allow for texture caching while meshes load
   *
   *-- History ------------------------------------------
   *
	* 2002.01.02:
	* Mongoose - New callback for percent loaded feedback
	*
   * 2001.06.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  void HexDump(unsigned char *buffer, int len, int offset, 
	       char *prefix, FILE *f);
  /*------------------------------------------------------
   * Pre  : 
   * Post : A 16-byte-wide dump, with offsets starting at 
   *        the specified value.
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  void ExtractMeshes(unsigned char *mesh_data, 
		     unsigned int num_mesh_pointers,
		     unsigned int *mesh_pointers);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

  int Fread(void *buffer, size_t size, size_t count, FILE *f);
  /*------------------------------------------------------
   * Pre  : 
   * Post : 
   *
   *-- History ------------------------------------------
   *
   * 2000.05.13:
   * Mongoose - Created
   ------------------------------------------------------*/

	bool _clear;                       /* Memory corruption hack */

  unsigned int _version;              /* TR file header version */

  tr2_version_type _engine_version;   /* TombRaider engine version  */

  tr2_colour_t _palette8[256];        /* 8-bit palette */

  unsigned int _palette16[256];       /* 16-bit palette */

  unsigned int _num_textiles;         /* Total number of texture tiles */

	unsigned short	_num_room_textures;     /* Num textures only for room use? */
	
	unsigned short _num_misc_textures;     /* Num of textures for misc use? */
	
	unsigned short _num_bump_map_textures; /* Num of textures that are 
														   bump map, texture pairs */

  tr2_textile8_t *_textile8;          /* 8-bit (palettised) textiles */

  tr2_textile16_t *_textile16;        /* 16-bit (ARGB) textiles */

  tr2_textile32_t *_textile32;        /* 32-bit (BGRA) textiles */

	unsigned int _num_tex_special;     /* Special textures and bump maps count */

	unsigned char *_tex_special;       /* Special textures and bump maps */

  unsigned int _unknown_t;            /* 32-bit unknown
                                         (always 0 in real TR2 levels) */

  unsigned short _num_rooms;          /* Number of rooms in this level */

  tr2_room_t *_rooms;                 /* List of rooms */

  unsigned int _num_floor_data;       /* Num of words of floor data 
													  this level */

  unsigned short *_floor_data;        /* Floor data */

  int    _num_meshes;                 /* Number of meshes this level */

  tr2_mesh_t *_meshes;                /* list of meshes */

  unsigned int _num_animations;       /* number of animations this level */

  tr2_animation_t *_animations;       /* list of animations */

  unsigned int _num_state_changes;    /* number of structures(?) this level */

  tr2_state_change_t *_state_changes; /* list of structures */

  unsigned int _num_anim_dispatches;  /* number of ranges(?) this level */

  tr2_anim_dispatch_t *_anim_dispatches; /* list of ranges */

  unsigned int _num_anim_commands;       /* number of Bone1s this level */

  tr2_anim_command_t *_anim_commands;    /* list of Bone1s */

  unsigned int _num_mesh_trees;          /* number of Bone2s this level */

  tr2_meshtree_t *_mesh_trees;      /* list of Bone2s */

  unsigned int _num_frames;         /* num of words of frame data this level */

  unsigned short *_frames;          /* frame data */

  unsigned int _num_moveables;      /* number of moveables this level */

  tr2_moveable_t *_moveables;       /* list of moveables */

  unsigned int _num_static_meshes;  /* number of static meshes this level */

  tr2_staticmesh_t *_static_meshes; /* static meshes */

  unsigned int _num_object_textures;   /* number of object textures this level */

  tr2_object_texture_t *_object_textures; /* list of object textures */


  unsigned int _num_sprite_textures;      /* num of sprite textures this level */

  tr2_sprite_texture_t *_sprite_textures; /* list of sprite textures */

  unsigned int _num_sprite_sequences;     /* num of sprite sequences this level */

  tr2_sprite_sequence_t *_sprite_sequences;  /* sprite sequence data */

  int  _num_cameras;                         /* Number of Cameras */

  tr2_camera_t  *_cameras;                   /* cameras */

  int  _num_sound_sources;                   /* Number of Sounds */

  tr2_sound_source_t *_sound_sources;          /* sounds */

  int  _num_boxes;                           /* Number of Boxes */


  tr2_box_t *_boxes;                    /* boxes - looks like 
														 struct { unsigned short value[4]; } 
														 - value[0..2] might be a vector; 
														 value[3] seems to be index into 
														 Overlaps[] */

  int  _num_overlaps;                       /* Number of Overlaps */

  short  *_overlaps;                        /* Overlaps - 
															  looks like ushort; 0x8000 is flag
															  of some sort appears to be an 
															  offset into Boxes[] and/or 
															  Boxes2[] */

  short  *_zones;                           /* Boxes2 */

  int  _num_animated_textures;              /* Number of AnimTextures */

  short  *_animated_textures;               /* Animtextures */

  int  _num_items;                          /* Number of Items */

  tr2_item_t *_items;                       /* Items */

  unsigned char  *_light_map;               /* Colour-light maps */

  unsigned int _num_cinematic_frames;       /* Number of cut-scene frames */

  tr2_cinematic_frame_t *_cinematic_frames; /* Cut-scene frames */

  short  _num_demo_data;                    /* Number of Demo Data */

  unsigned char  *_demo_data;               /* Demo data */

  short  *_sound_map;                       /* Sound map */

  int  _num_sound_details;                  /* Number of SampleModifiers */

  tr2_sound_details_t *_sound_details;      /* Sample modifiers */

  int  _num_sample_indices;                 /* Number of Sample Indices */

  int  *_sample_indices;                    /* Sample indices */

  bool _debug_on;                           /* Debug output toggle */

	// For packed Fread emu/wrapper
	unsigned char *_compressed_level_data;      /* Buffer used to emulate fread
																  with uncompressed libz data */

	unsigned int _compressed_level_data_offset; /* Offset into buffer */

	unsigned int _compressed_level_size;        /* Size of buffer */

	tr_fread_mode_t _fread_mode;                /* Fread mode file|buffer */
};

#endif

