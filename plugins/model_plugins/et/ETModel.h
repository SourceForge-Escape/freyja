/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : ETModel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: MDM and MDX formats
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ETMODEL - Builds ETModel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.16:
 * Mongoose - Created, Based on specs from: 
 *              http://games.theteamkillers.net/rtcw/mdx/
 *              http://mojo.gmaxsupport.com/Articles/MDSFileFormat.html
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ETMODEL_H_
#define GUARD__FREYJA_MONGOOSE_ETMODEL_H_

class MDMModel
{
public:
	
	typedef struct
	{
		char ident[4];               // MDM identification, should be"MDMW".
		long version;                // MDM version, should be 3.
		char name[64];               // MDM file location and name.
		float lodBias;               // a factor of LoD.
		float lodScale;              // a factor of LoD.
		long numSurfaces;            // Number of surfaces(meshes) MDM has.
		long surfaceDataOffset;      // Offset to surface data.
		long numTags;                // Number of tags MDM has
		long tagDataOffset;          // Offset to tag data
		long fileSize;               // Offset to EOF = MDM file size.
	} MDMHeader;


	typedef struct
	{
		long ident;               // Surface data identification, should be 9.
		char name[64];		      // Name of this surface.
		char shader_name[64];	  // Name of the shader applied to this surface.
		long shader_index;	 	  // Shader index.
		long LoD_minimum;		  // a factor of LoD.
		long surface_header_offset;	// Offset to the head of MDM from this surface header.
		long num_vertices;		// Number of vertices.
		long offset_vertices;		// Offset to vertex data.
		long num_triangles;		// Number of triangles.
		long offset_triangles;		// Offset to triangle data.
		long offset_collaspemap;		// Offset to collapsemap data.
		long number_bonerefs;		// Number of reference bones.
		long offset_bonerefs;		// Offset to reference bone data.
		long offset_surface_data_end;	// Offset to the end of this surface data.
	} SurfaceHeader;


	typedef struct
	{
		long bone_index;			// Number of the bone which affect this vertex.
		float weight;			// Weight coefficiency. Summation of all weights must be 1.
		float bone_space[3];		// XYZ coordinates of this vertex in the bone's frame of reference.
	} BoneWeight;

	typedef struct
	{
		float normal[3];	        // Normal vector.
		float uv[2];	            // UV coordinates in texture.
		long num_weights;		// Number of bone weightings.

		// The followings are repeated num_bone_weights times.
		BoneWeight *weights;
	} SurfaceVertex;


	typedef struct
	{
		long indices[3];	// vertex indices
	} SurfaceTriangle;



	// E. Surface Collapsemap
	// Please refer to http://mojo.gmaxsupport.com/Articles/MDSFileFormat.html.

	

	// F. Surface Bone Reference
	// a list of the bones which this suface uses to calcurate its vertex position.

	typedef struct {
		
		SurfaceHeader header;
		SurfaceVertex *vertices;
		SurfaceTriangle *triangles;
		long *map;
		long *bonerefs;
		
	} MDMSurface;


	typedef struct {
		char name[64];			// Name of this tag.
		float tag_triangles_vector[9];	// Coordinates of the three vetices which form the tag's figure.
		long attach_bone_number; //long		// Number of the bone attached to.
		float tag_pos_vector_to_bone[3];	// 3D vector which determines the direction of the tag triangle from the bone.
		long bone_number_list_size; //long	// an index size of the related bones.
		long bone_list_offset; //long		// Offset to the Index list. it should be 0x80.
		long tag_data_size; //long		// Offset to the end of this tag data.
		long *bone_index_list; // int32 * bone_number_list_size	// Index List
	} MDMTag;


	

	bool load(const char *filename);

	void print();

	MDMHeader mHeader;

	MDMSurface *mSurfaces;

	MDMTag *mTags;
};


class ETModel
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ETModel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ETModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~ETModel();
	/*------------------------------------------------------
	 * Pre  : ETModel object is allocated
	 * Post : Deconstructs an object of ETModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif
