/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja, ported from old GooseEgg loader
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : HalfLife
 * Comments: This is the HalfLife model class.
 *
 *           See file COPYING for license details.
 *
 *
 *           HalfLife (c) 1998, Valve LLC. All rights reserved.            
 *
 *-- History ---------------------------------------------------------- 
 *
 * See the source file's header. 
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_HALFLIFE_H
#define GUARD__FREYJA_MONGOOSE_HALFLIFE_H


#define MAXSTUDIOTRIANGLES	20000	// TODO: tune this
#define MAXSTUDIOVERTS		2048	// TODO: tune this
#define MAXSTUDIOSEQUENCES	256		// Total animation sequences
#define MAXSTUDIOSKINS		100		// Total textures
#define MAXSTUDIOSRCBONES	512		// Bones allowed at source movement
#define MAXSTUDIOBONES		128		// Total bones actually used
#define MAXSTUDIOMODELS		32		// Sub-models per model
#define MAXSTUDIOBODYPARTS	32
#define MAXSTUDIOGROUPS		4
#define MAXSTUDIOANIMATIONS	512		// Per sequence
#define MAXSTUDIOMESHES		256
#define MAXSTUDIOEVENTS		1024
#define MAXSTUDIOPIVOTS		256
#define MAXSTUDIOCONTROLLERS 8

/* Lighting options */
#define STUDIO_NF_FLATSHADE	    0x0001
#define STUDIO_NF_CHROME	    0x0002
#define STUDIO_NF_FULLBRIGHT    0x0004

/* Motion flags */
#define STUDIO_X		0x0001
#define STUDIO_Y		0x0002	
#define STUDIO_Z		0x0004
#define STUDIO_XR		0x0008
#define STUDIO_YR		0x0010
#define STUDIO_ZR		0x0020
#define STUDIO_LX		0x0040
#define STUDIO_LY		0x0080
#define STUDIO_LZ		0x0100
#define STUDIO_AX		0x0200
#define STUDIO_AY		0x0400
#define STUDIO_AZ		0x0800
#define STUDIO_AXR		0x1000
#define STUDIO_AYR		0x2000
#define STUDIO_AZR		0x4000
#define STUDIO_TYPES	0x7FFF
#define STUDIO_RLOOP    0x8000	    /* Controller that wraps 
									   shortest distance */

/* Sequence flags */
#define STUDIO_LOOPING	    0x0001

/* Bone flags */
#define STUDIO_HAS_NORMALS	0x0001
#define STUDIO_HAS_VERTICES 0x0002
#define STUDIO_HAS_BBOX		0x0004
#define STUDIO_HAS_CHROME	0x0008	    /* If any of the textures 
										   have chrome on them */

#define RAD_TO_STUDIO (32768.0 / M_PI)
#define STUDIO_TO_RAD (M_PI / 32768.0)


#ifndef vec_t
typedef float vec_t;
#endif

#ifndef vec3_t
typedef vec_t vec3_t[3];
#endif

#ifndef byte
typedef unsigned char byte;
#endif


typedef struct {
	long id;
	long version;
	char name[64];
	long length;
	vec3_t eyeposition;	        // Ideal eye position
	vec3_t min;			// Ideal movement hull size
	vec3_t max;			
	vec3_t bbmin;			// Clipping bounding box
	vec3_t bbmax;		
	long flags;

	long numbones;			// Bones
	long boneindex;
	long numbonecontrollers;       // Bone controllers
	long bonecontrollerindex;

	long numhitboxes;		// Complex bounding boxes
	long hitboxindex;			

	long numseq;			// Animation sequences
	long seqindex;

	long numseqgroups;		// Demand loaded sequences
	long seqgroupindex;

	long numtextures;		// Raw textures
	long textureindex;
	long texturedataindex;

	long numskinref;	      	// Replaceable textures
	long numskinfamilies;
	long skinindex;

	long numbodyparts;		
	long bodypartindex;

	long numattachments;		// Queryable attachable points
	long attachmentindex;

	long soundtable;
	long soundindex;

	long soundgroups;
	long soundgroupindex;

	long numtransitions;		/* Animation node to animation 
								 * node transition graph */
	long transitionindex;
} studiohdr_t;


// header for demand loaded sequence group data
typedef struct {
	long id;
	long version;
	char name[64];
	long length;
} studioseqhdr_t;


typedef struct {
	char name[32];            // bone name for symbolic links
	long parent;               // parent bone
	long flags;                // ??
	long bonecontroller[6];    // bone controller index, -1 == none
	float value[6];           // default DoF values
	float scale[6];           // scale for delta DoF values
} mstudio_bone_t;


typedef struct {
	void *data;
} cache_user_t;


typedef struct {
	long bone;                 // -1 == 0
	long type;                 // X, Y, Z, XR, YR, ZR, M
	float start;
	float end;
	long rest;                 // byte index value at rest
	long index;                // 0-3 user set controller, 4 mouth
} mstudio_bone_controller_t;


typedef struct { // demand loaded sequence groups
	char label[32];   // textual name
	char name[64];   // file name
	cache_user_t cache;      // cache index pointer
	long data;      // hack for group 0
} mstudio_seqgroup_t;


typedef struct { // sequence descriptions
	char label[32]; // sequence label
	float fps; // frames per second 
	long flags; // looping/non-looping flags
	long activity;
	long actweight;
	long numevents;
	long eventindex;
	long numframes; // number of frames per sequence
	long numpivots; // number of foot pivots
	long pivotindex;
	long motiontype; 
	long motionbone;
	vec3_t linearmovement;
	long automoveposindex;
	long automoveangleindex;
	vec3_t bbmin; // per sequence bounding box
	vec3_t bbmax; 
	long numblends;
	long animindex; /* mstudioanim_t pointer relative to start of 
					  sequence group data */
	// [blend][bone][X, Y, Z, XR, YR, ZR]
	long blendtype[2]; // X, Y, Z, XR, YR, ZR
	float blendstart[2]; // starting value
	float blendend[2]; // ending value
	long blendparent;
	long seqgroup; // sequence group for demand loading
	long entrynode; // transition node at entry
	long exitnode; // transition node at exit
	long nodeflags; // transition rules
	long nextseq; // auto advancing sequences
} mstudio_seq_desc_t;


typedef struct {
	unsigned short offset[6];
} mstudio_anim_t; // tag

// animation frames
typedef union {
	struct {
		byte valid;
		byte total;
	} num;
	short value;
} mstudio_anim_value_t;



// body part index
typedef struct {
	char name[64];
	long nummodels;
	long base;
	long modelindex; // index into models array
} mstudio_bodyparts_t;


// skin families
// short   index[skinfamilies][skinref]

// Studio models
typedef struct {
	char name[64];
	long type;
	float boundingradius;
	long nummesh;
	long meshindex;
	long numverts;         // number of unique vertices
	long vertinfoindex;    // vertex bone info
	long vertindex;        // vertex vec3_t
	long numnorms;         // number of unique surface normals
	long norminfoindex;    // normal bone info
	long normindex;        // normal vec3_t
	long numgroups;        // deformation groups
	long groupindex;
} mstudio_model_t;


// vec3_t   boundingbox[model][bone][2];   // complex intersection info


// meshes
typedef struct {
	long numtris;
	long triindex;
	long skinref;
	long numnorms;       // per mesh normals
	long index;      // normal vec3_t
} mstudio_mesh_t;

// triangles
typedef struct {
	short            vertindex;      // index into vertex array
	short            normindex;      // index into normal array
	short            s,t;            // s,t position on skin
} mstudio_trivert_t;


#ifdef UNUSED


typedef struct {
	int             frame;
	int               event;
	int               type;
	char            options[64];
} mstudio_event_t;


// pivots
typedef struct {
	vec3_t            org;   // pivot point
	int               start;
	int               end;
} mstudio_pivot_t;

// attachment
typedef struct {
	char            name[32];
	int               type;
	int               bone;
	vec3_t            org;   // attachment point
	vec3_t            vectors[3];
} mstudio_attachment_t;

typedef struct { // intersection boxes
	int               bone;
	int               group;         // intersection group
	vec3_t            bbmin;      // bounding box
	vec3_t            bbmax;      
} mstudio_bbox_t;

#endif


typedef struct {
	byte *image;
	int w;
	int h;

} hl_image_t;

// skin info
typedef struct {
	char name[64];
	long flags;
	long width;
	long height;
	long index;
} mstudio_texture_t;

/* Actual data storage records */

typedef struct {

	mstudio_trivert_t vertex[3];

} hl_face_t;


typedef void * hl_group_t;


typedef struct {

	unsigned int faceCount;
	unsigned int normalCount;
	unsigned int material;
	hl_face_t *faces;

} hl_mesh_t;

typedef struct {

	unsigned int meshCount;
	unsigned int groupCount;
	unsigned int vertexCount;
	unsigned int normalCount;

	hl_mesh_t *meshes;
	hl_group_t *groups;
	vec3_t *vertices;
	byte *weights;
	vec3_t *normals;

} hl_model_t;

typedef struct {

	unsigned int modelCount;
	hl_model_t *models;

} hl_bodypart_t;



class HalfLife
{
public:

	HalfLife();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 199?.??.??:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~HalfLife();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 199?.??.??:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid
	 * Post : Returns 0 on sucess and loads model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.05:
	 * Mongoose - Created, from old hacky API
	 ------------------------------------------------------*/


	studiohdr_t mHeader;

	mstudio_model_t *mModels;

	mstudio_mesh_t *mMeshes;

	mstudio_bone_t *mBones;

	mstudio_texture_t *mTextures;

	hl_bodypart_t *mBodyParts;

	hl_face_t *mFaces;

	hl_image_t *mImages;

	vec3_t *mVertices;

	unsigned int mMeshCount;
	unsigned int mVertexCount;
	unsigned int mBoneCount;
	unsigned int mModelCount;
	unsigned int mBodyPartCount;
	unsigned int mTextureCount;

private:

};

#endif

