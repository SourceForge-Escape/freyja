/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Nif4
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: NetImmerse Model 4.0 class for Morrowind ( 4.0.0.2 )
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_NIF4 - Builds Nif4 class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.16:
 * Mongoose - Created, based on my reverse engineering which is based off:
 *
 *            NetImmerse 3.0 Fileformat Spec By:
 *               Chris Graham (gameRevolt@assimsoft.com)
 *               Banbury (baley5@excite.com)
 *
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_NIF4_H_
#define GUARD__FREYJA_MONGOOSE_NIF4_H_

#include <mstl/Vector.h>
#include "EggFileReader.h"


#define NIF4_HEADER_START "NetImmerse File Format, Version 4.0.0.2"


class Nif4
{
 public:

	typedef struct
	{
		float x,y,z;
	} vertex_t;


	class Chunk {
	public:           /* unsigned or signed? */

		bool readHeader(EggFileReader &r)
		{
			len = r.getInt32();
			name = new char[len];
			r.getCharString(len, name);
			id = r.getInt32();

			return true;
		}

		int len;
		char *name;
		int id;
	};

	class NiNode {
	public:

		bool readChunk(EggFileReader &r)
		{
			unsigned int i, n;


			 len = r.getInt32();
			 node_name = new char[len];
			 r.getCharString(len, node_name);
			 associated_keyframe_controller_id = r.getInt32U();
			 associated_lookat_controller_id = r.getInt32U();
			 unknown = r.getInt16();
			 x = r.getFloat32();
			 y = r.getFloat32();
			 z = r.getFloat32();
			 vector_x_axis_x = r.getFloat32();
			 vector_x_axis_y = r.getFloat32();
			 vector_x_axis_z = r.getFloat32();
			 vector_y_axis_x = r.getFloat32();
			 vector_y_axis_y = r.getFloat32();
			 vector_y_axis_z = r.getFloat32();
			 vector_z_axis_x = r.getFloat32();
			 vector_z_axis_y = r.getFloat32();
			 vector_z_axis_z = r.getFloat32();
			 unknown2[0] = r.getFloat32();
			 unknown2[1] = r.getFloat32();
			 unknown2[2] = r.getFloat32();
			 unknown3 = r.getInt32();

			 num_properties = r.getInt32();
			 propertyIDs = new unsigned int[num_properties];
			 
			 for (i = 0, n = num_properties; i < n; ++i)
				 propertyIDs[i]  = r.getInt32U();

			 unkn2 = r.getInt32U();
			 
			 num_children = r.getInt32();
			 childrenIDs = new unsigned int[num_children];
			 
			 for (i = 0, n = num_children; i < n; ++i)
				 childrenIDs[i]  = r.getInt32U();
			 
			 num_blocks = r.getInt32();
			 blockIDs = new unsigned int[num_blocks];

			 for (i = 0, n = num_blocks; i < n; ++i)
				 blockIDs[i]  = r.getInt32U();

			 return true;
		}

		int len;
		char *node_name; // char[len];
		unsigned int associated_keyframe_controller_id;//zeroifnot linked to one
		unsigned int associated_lookat_controller_id;//zero if not linked to one
		short unknown; //(12 once)
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		float unknown2[3]; //={1,0,0}; matrix col or aug vec?
		int unknown3; //=0;
		int num_properties;
		unsigned int *propertyIDs;//[num_properties];
		int unkn2;//=0;
		int num_children;
		unsigned int *childrenIDs;//[num_children];
		int num_blocks;
		unsigned int *blockIDs;//[num_blocks];
	};


	class NiTriShape {
	public:
		int len;
		char *mesh_name; //char[len];
		int unknown; //=0;
		unsigned int associated_skin_controller_id; // Set to zero if there is none
		short unknown1;//=4;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		float unknown2[3];//={1,0,0}; // Like a spare vector: maybe an artifact from a matrix
		int num_properties;
		unsigned int *propertyIDs;//word[num_properties];
		int unknown3;//=0;
		unsigned int associated_trishape_data_id; // The mesh
	};


	class NiTextureModeProperty {
	public:
		int unknown[3];//=0;
		short flags; // (000000?? 110?0010)
		unsigned int some_value; // (not an id, FFB50000 once, FFFE0000 once)
	};

	
	class NiTextureProperty {
	public:

		int unknown[3];//=0;
		short unknown1;//=0;
		unsigned int associated_image_id;
	};

	class NiAlphaProperty {
	public:
		int unknown[3];//=0;
		unsigned char alpha_value;
		short unknown1;//=0;
	};

	class NiVertexColorProperty {
	public:
		int unknown[4];//=0;
		short unknown1;//=0;
		int unknown2;//=1;
	};


	class NiZBufferProperty {
	public:
		int unknown[3];//=0;
		short unknown1;//=3;
	};


	class NiMaterialProperty {
	public:
		//This sets up a material.
		
		int len; 
		char *material_name;//char[len];
		int unknown[2];//=0;
		short unknown1; //(1 once)
		float ambient_color[3];
		float diffuse_color[3];
		float specular_color[3];//=0;
		int unknown2[3];//=0;
		float glossiness;
		float opacity; //(1 for not at all transparent)
	};


	class NiImage {
	public:
		
		bool readChunk(EggFileReader &r)
		{	
			type = r.getInt8U();
			
			if (type == 0)
			{
				associated_raw_image_data_id = r.getInt32U();
				file_len = 0;
				filename = 0x0;
			}
			else // 1 only?
			{
				associated_raw_image_data_id = 0;
				file_len = r.getInt32();
				filename = new char[file_len];
				r.getCharString(file_len, filename);
			}

			unknown = r.getInt32();
			unknown2 = r.getFloat();

			return true;
		}
		
		unsigned char type;
		unsigned int associated_raw_image_data_id;
		int file_len;
		char *filename; // pathless filename
		
		// Somehow these are related to glow; set them to zero, and it will glow fully
		int unknown; //=7; // Maybe a set of flags?
		float unknown2; //=128.5 // Maybe an intensity?
	};


	class NiRawImageData {
	public:	

		typedef struct {
			char r,g,b,a;
		} pixel_t;


		bool readChunk(EggFileReader &r)
		{
			unsigned int i, n;

			w = r.getInt32();
			h = r.getInt32();
			type = r.getInt32();  // 1 for RGB, 2 for RGBA
			pixels = new pixel_t[w*h];

			for (i = 0, n = w*h; i < n; ++i)
			{
				pixels[i].r = r.getInt8();
				pixels[i].g = r.getInt8();
				pixels[i].b = r.getInt8();
				pixels[i].a = 255;

				if (type == 2) 
					pixels[i].a =r.getInt8();
			}

			return true;
		}

		int w;
		int h;
		int type;// (1 for RGB, 2 for RGBA);
		
		pixel_t *pixels;
	};


	class NiStringExtraData {
	public:
		
		bool readChunk(EggFileReader &r)
		{
			unknown = r.getInt32();
			unknown2 = r.getInt32();
			len = r.getInt32();
			string = new char[len];
			r.getCharString(len, string);

			return true;
		}

		int unknown;//=0;
		int unknown2;//; (29 and 35 came once)
		int len;
		char *string;//[len];
	};

	class NiTriShapeData {
	public:	

		typedef struct {
			float u, v;
		} uv_t;

		typedef struct {
			uv_t *textureinfo;
		} uv_set_t;

		typedef struct {
			short v[3];
		} triangle_t;

		typedef struct {
			short len;
			short *unknown; // short[len]
		} unknown_t;


		bool readChunk(EggFileReader &r)
		{
			unsigned int i, n, j, jn;

			num_vertices = r.getInt16();
			id_of_vertices = r.getInt32U();
			coordinates = new vertex_t[num_vertices];

			for (i = 0, n = num_vertices; i < n; ++i)
			{
				coordinates[i].x = r.getFloat32();
				coordinates[i].y = r.getFloat32();
				coordinates[i].z = r.getFloat32();
			}

			id_of_normals = r.getInt32U();

			for (i = 0, n = num_vertices; i < n; ++i)
			{
				normals[i].x = r.getFloat32();
				normals[i].y = r.getFloat32();
				normals[i].z = r.getFloat32();
			}

			unknown[0] = r.getInt32();
			unknown[1] = r.getInt32();
			unknown1[0] = r.getInt32U();
			unknown1[1] = r.getInt32U();
			unknown2 = r.getInt32();
			num_uv_sets = r.getInt16U();
			unknown3 = r.getInt32U();

			uv_set = new uv_set_t[num_uv_sets];

			for (i = 0, n = num_uv_sets; i < n; ++i)
			{
				uv_set[i].textureinfo = new uv_t[num_vertices];
				
				for (j = 0, jn = num_vertices; j < jn; ++j)
				{
					uv_set[i].textureinfo[j].u = r.getFloat32();
					uv_set[i].textureinfo[j].v = r.getFloat32();
				}
			}
		
			if (num_uv_sets == 0) 
				unknown4 = r.getInt32();
			
			num_triangles = r.getInt16U();
			unknown5 = r.getInt32U(); // num_trianges*3?
			
			triangles = new triangle_t[num_triangles];

			for (i = 0, n = num_triangles; i < n; ++i)
			{
				triangles[i].v[0] = r.getInt16U();
				triangles[i].v[1] = r.getInt16U();
				triangles[i].v[2] = r.getInt16U();
			}

			num_something_3 = r.getInt16U();  	// 0 or num_vertices?
			something = new unknown_t[num_something_3];	

			for (i = 0, n = num_something_3; i < n; ++i)
			{
				something[i].len = r.getInt16U();
				something[i].unknown = new short[something[i].len];

				for (j = 0, jn = something[i].len; j < jn; ++j)
				{
					something[i].unknown[j] = r.getInt16U();
				}
			}

			return true;
		}

		short num_vertices;
		unsigned int id_of_vertices;
		vertex_t *coordinates;
		unsigned int id_of_normals;
		vertex_t *normals;
		
		int unknown[2];//=0;
		unsigned int unknown1[2];
		int unknown2;//=0;
		short num_uv_sets;
		unsigned int unknown3; // (not an id, once 0464C298, but varies);

		uv_set_t *uv_set;
		
		int unknown4;
		short num_triangles;
		int unknown5; // value varies a lot: might be num_trianges*3
		triangle_t *triangles;
		
		short num_something_3;  	// This seems to be either 0 or num_vertices
		unknown_t *something;
	};

	class NiSpotlight {
	public:
		//This chunk represents a Max Targetted/Free spot light.

		int len;
		char *light_name;//[len]; 
		int unknown[2];//=0;
		short unknown2;//=4;
		int unknown3;//=0;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		int unknown4[5];//=0;
		int num_parents;
		unsigned int *parent_id;//[num_parents];
		float opacity;//=1;
		float ambient_color[3];
		float diffuse_color[3];
		float specular_color[3];
		float unknown4_5;//=1;
		int unknown5[2];//=0;
		float unknown6;//; (once 22.5)
		int unknown7;//=0;
	};

	class NiPointLight {
	public:
		//This chunk represents a Max Omni light.

		int len;
		char *light_name;//[len]; 
		int unknown[2];//=0;
		short unknown1;//=4;
		int unknown2;//=0;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		int unknown3[5];//=0;
		int num_parents;
		unsigned int *parent_id;//[num_parents];
		float opacity;//=1;
		float ambient_color[3];
		float diffuse_color[3];
		float specular_color[3];
		float unknown4;//=1;
		int unknown5[2];//=0;
	};

	class NiDirectionalLight {
	public:

		//This chunk represents a Max Targetted/Free directional light.

		int len;
		char *light_name;//[len]; 
		int unknown[2];//=0;
		short unknown1;//=4;
		int unknown2;//=0;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		int unknown3[5];//=0;
		int num_parents;
		unsigned int *parent_id;//[num_parents];
		float opacity;//=1;
		float ambient_color[3];
		float diffuse_color[3];
		float specular_color[3];
	};

	class NiAmbientLight {
	public:
		//This chunks represents the Max rendering environment.

		int len;
		char *light_name;//[len]; 
		int unknown[2];//=0;
		short unknown1;//=4;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		int unknown2[5];//=0;
		int num_parents;
		unsigned int *parent_id;//[num_parents];
		float opacity;//=1;
		float ambient_color[3];
		float diffuse_color[3];
		float specular_color[3];
	};


	class NiLookAtController {
	public:

		unsigned int associated_roll_controller_id; // If 0, no rollcontroller
		short unknown;//=0x4C;
		float unknown1;//=1;
		int unknown2;//=0;
		float unknown3[4]; //(some sort of positional/orientational information)
	};


	class NiVisData {
	public:
		int x;
		int *unknown;//int[x];
	};


	class NiFloatData {
	public:

		int a;
		int b;
		int *unknown;//int[a];
		int *unknown1;//int[b];
	};


	class NiVisController {
	public:
		
		int unknown;//=0;
		short unknown1;//=5;
		float unknown2;//=1;
		int unknown3;//=0;
		unsigned int unknown4[2];
		unsigned int associated_parent_node_id;
		unsigned int associated_visdata_id;
	};


	class NiKeyframeController {
	public:

		unsigned int associated_vis_controller_id; // Set to zero if there is none
		short unknown;//=5;
		float unknown1;//=1;
		int unknown2;//=0;
		unsigned int unknown3[2];
		unsigned int associated_parent_node_id;
		unsigned int associated_keyframedata_id;
	};


	class NiTriShapeSkinController {
	public:
		typedef struct {
			unsigned char unknown[14];
		} unknown_t;


		int unknown;//=0;
		short unknown1;//=0x1C;
		float unknown2;//=1;
		int unknown3;//=0;
		unsigned int unknown4[2];
		unsigned int associated_parent_node_id;

		int x;
		int *y;//int[x];

		unknown_t *unknown5; //unknown_t[x][y]

		//struct
		//{
		//	byte unknown[14];
		//} unknown[x][y];
	};


	class NiMultiTextureProperty {
	public:
		
		
		short unknown[7];//=0;
		int unknown1;//=5;
		unsigned int unknown2;
		unsigned int associated_image_id;
		int unknown3;//=3;
		int unknown4;//=2;
		int unknown5;//=1;
		short unknown6;//=0;
		unsigned int unknown7;//; (not an ID)
		int unknown8[4];//=0;
	};
	
	class NiBone {
	public:
		int len;
		char *bone_name;//[len];
		int unknown;//=0;
		unsigned int associated_keyframe_controller_id;
		short unknown1;//=12;
		float x;
		float y;
		float z;
		float vector_x_axis_x;
		float vector_x_axis_y;
		float vector_x_axis_z;
		float vector_y_axis_x;
		float vector_y_axis_y;
		float vector_y_axis_z;
		float vector_z_axis_x;
		float vector_z_axis_y;
		float vector_z_axis_z;
		float unknown2[3];//={1,0,0}; // Like a spare vector: maybe an artifact from a matrix
		int unknown3[3];//=0;
		int num_properties;
		unsigned int *propertyIDs;//[num_properties];
		int unknown4;//=0;
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Nif4();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Nif4
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Nif4();
	/*------------------------------------------------------
	 * Pre  : Nif4 object is allocated
	 * Post : Deconstructs an object of Nif4
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool loadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Parses and stores a Nif4 model from a file
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	unsigned int mHeaderSz;

	char mHeader[48];	              /* Header is just a string */

	EggFileReader mReader;
};

#endif
