/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Egg
 * License : GPL, also (C) 2000-2002 Mongoose
 * Comments: This is the data model agent of Egg models
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_EGG  Builds module as test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.04.29:
 * Mongoose - Removed mtk3d support and replaced with libhel,
 *            so transition to Egg v9 will be smoother.
 *
 *            Lots of legacy stuff changed, but this is not
 *            a true 8/9 hybrid like I've released before.
 *
 * 2004.03.15:
 * Mongoose - Replaced List collection use with Vector
 *            Some coding style changes to remove mismatch
 *            This is a bigger deal than it seems  =)
 *
 * 2002.08.31:
 * Mongoose - Introducing simple vectors to replace Lists and
 *            the Chain prototype collections.
 *
 *            Should reduce memory requirements and improve speed
 *
 * 2002.07.05:
 * Mongoose - Refactoring and rewritten to conform to new code style
 *
 *            The reason I do this IS to break the API for testing
 *            if you don't like it make an adpater
 *
 *            Removed marker system ( No longer used by anyone? )
 *
 * 2001.07.05:
 * Mongoose - New mtk3d API and other major changes started
 *
 * 2001-04-14:
 * Mongoose - Finishing rollover from EggV7 to EggV8 
 *
 * 2001-01-31:
 * Mongoose - Composite Eggv* starts, using pieces of 
 *            tracers ( prototypes ) and reformed to the
 *            EggV8 001 (20010101) spec.  
 *
 *            Also new major egg releases include major number 
 *            in classname, to ease transitions of other projects.
 *
 *            Removal of internal shaders, just id refernce now.
 *
 *            New focus on documentation, to aid other projects
 *            using this module.
 *
 * 2000-12-09:
 * Mongoose - Move to new data structures
 *
 * 2000-11-20:
 * Mongoose - Implementation of v8 design ( Freyja backend )
 *
 * 2000-10-16:
 * Mongoose - Documentation addtion and interface changes
 *
 * 2000-10-07:
 * Mongoose - Frame, mesh, and interface changes
 *
 * 2000-09-15:
 * Mongoose - API change!  Transforms have 1 new API call
 *            The 3 old methods per mesh, frame, list are gone
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_GOOSEEGG_EGG_H
#define GUARD__MONGOOSE_GOOSEEGG_EGG_H

#include <mstl/Vector.h>
#include <mstl/Map.h>

#include <hel/math.h>

typedef vec_t bbox_t[3][3];


#define EGG_LIB_VERSION        0x32312E38  // '8.12'

/* File header, versioning */
#define EGG_FILE               0x20676745  // 'EGG '
#define EGG_VERSION            0x32312E38  // '8.12'
#define EGG_8_10               0x30312E38  // '8.10'
#define EGG_8_09               0x39302E38  // '8.09'

/* Atomic model componets */
#define EGG_VERT_CHUNK_START   0x54524556
#define EGG_VERT_CHUNK_END     0x56455254
#define EGG_TEXL_CHUNK_START   0x4C584554
#define EGG_TEXL_CHUNK_END     0x5445584C
#define EGG_MARK_CHUNK_START   0x4B52414D
#define EGG_MARK_CHUNK_END     0x4D41524B

/* Polygon Mesh */
#define EGG_POLY_CHUNK_START   0x594C4F50
#define EGG_POLY_CHUNK_END     0x504F4C59
#define EGG_MESH_CHUNK_START   0x4853454D
#define EGG_MESH_CHUNK_END     0x4D455348

/* Tags aka 'bolt ons' */
#define EGG_BTAG_CHUNK_START   0x47415442
#define EGG_BTAG_CHUNK_END     0x42544147
#define EGG_TFRM_CHUNK_START   0x4D524654
#define EGG_TFRM_CHUNK_END     0x5446524D
#define EGG_AFRM_CHUNK_START   0x4D524641
#define EGG_AFRM_CHUNK_END     0x4146524D

/* 'Real' bones aka skeletal models w/ weighted vertices */
#define EGG_BONE_CHUNK_START   0x454E4F42
#define EGG_BONE_CHUNK_END     0x424F4E45
#define EGG_BFRM_CHUNK_START   0x4D524642
#define EGG_BFRM_CHUNK_END     0x4246524D
#define EGG_WGHT_CHUNK_START   0x54484757
#define EGG_WGHT_CHUNK_END     0x57474854


#define COLORED_POLYGON -1


typedef struct {
	vec_t weight;                     /* Weight for vertex use */
	unsigned int bone;                /* Bone id */

} egg_weight_t;


typedef struct egg_vertex_s {
	vec3_t pos;                       /* Position in 3 space (XYZ) */
	vec3_t norm;                      /* Normal vector */
	vec2_t uv;                        /* Texel coordinate */

	Vector<egg_weight_t *> weights;   /* Vector of weights */

	unsigned int id;                  /* Unique identifier */
	Vector <unsigned int> ref;        /* Ids of polygons referencing vertex */ 

} egg_vertex_t;


typedef struct egg_texel_s {
	unsigned int id;                   /* Unique identifier */
	Vector <unsigned int> ref;         /* Ids of polygons referencing texel */ 
	vec2_t st;                         /* Position on texmap */

} egg_texel_t;


typedef struct egg_polygon_s {
	unsigned int id;                  /* Unique identifier */
	Vector <unsigned int> vertex;     /* Vertices composing polygon */
	Vector <unsigned int> texel;      /* Texels */
	int shader;                       /* Shader id */

	Vector <egg_vertex_t *> r_vertex;
	Vector <egg_texel_t *> r_texel;

} egg_polygon_t;


typedef struct egg_group_s {
	int id;                           /* Unique identifier */
	Vector <unsigned int> vertex;     /* Vertices composing group */
	vec3_t bbox_min;                  /* Min corner of bounding box */
	vec3_t bbox_max;                  /* Max corner of bounding box */
	vec3_t center;                    /* Center of rotation | bolt-on binding */
	float scale;                      /* Scaling of group */

} egg_group_t;


typedef enum {
	FL_MESH__VERTEX_FRAME_GROUPS = 1

} egg_mesh_flags_t;

// Mongoose 2002.03.02, add map for vertex translation from world to local ids
typedef struct egg_mesh_s {
	int id;                             /* Unique identifier */

	unsigned int flags;

	Vector <unsigned int> group;        /* Groups/Frames of this mesh */
	Vector <unsigned int> polygon;      /* Polygons of this mesh */

	Vector <egg_polygon_t *> r_polygon;

} egg_mesh_t;


// egg_tag mixed with egg_bone
typedef struct egg_tag_s {
	int id;                           /* Unique identifier */
	char name[64];
	Vector <unsigned int> slave;      /* Slave tags */
	Vector <unsigned int> mesh;       /* List of meshes bound to this */
	unsigned char flag;               /* Rendering flag, special use */
	vec3_t center;                    /* Pivot point, center of rotation */
	matrix_t transform;               /* Transform mesh/slaves by this matrix */
	float rot[3];
	int parent;  // Ext for 8.12, 20040917 ( not stored to disk )
} egg_tag_t;


// egg_meshtree_anim_frame
typedef struct egg_boneframe_s {
	int id;                           /* Unique identifier */
	Vector<unsigned int> tag;         /* Bone Tags in this list */
	vec3_t center;                    /* Pivot point, center of rotation */
} egg_boneframe_t;


// egg_meshtree_anim
typedef struct egg_animation_s {
	int id;                             /* Unique identifier */
	Vector<unsigned int> frame;         /* BoneFrames in this aniamtion */

} egg_animation_t;


class EggPolygon
{
public:
	unsigned int id;                  /* Unique identifier */
	Vector <unsigned int> vertex;     /* Vertices composing polygon */
	Vector <unsigned int> texel;      /* Texels */
	int shader;                       /* Shader id */

	Vector <egg_vertex_t *> r_vertex;
	Vector <egg_texel_t *> r_texel;
};



class Egg
{
public:

	typedef enum egg_transform	{
		
		SCALE               = 0, 
		ROTATE              = 1, 
		TRANSLATE           = 2, 
		ROTATE_ABOUT_CENTER = 3,
		SCALE_ABOUT_CENTER  = 4
	};
	
	typedef enum egg_plane { 
		
		PLANE_XY = 0, 
		PLANE_YZ = 1, 
		PLANE_XZ = 2
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Egg();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Egg();
	/*------------------------------------------------------
	 * Pre  : This object is allocated
	 * Post : Deconstructs the Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	egg_group_t *getNearestGroup(vec_t x, vec_t y, egg_plane plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns nearest group to x, y in given plane
	 *        ( XY, YZ, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_tag_t *getNearestTag(vec_t x, vec_t y, egg_plane plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns nearest tag to x, y in given plane
	 *        ( XY, YZ, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *getNearestVertex(egg_group_t *group, 
								   vec_t x, vec_t y, egg_plane plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns nearest vertex to x, y in given plane
	 *        ( XY, YZ, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void print();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : if debugging dumps partial ASCII representation 
	 *        of model to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.15:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static int checkFile(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Checks to see if it's valid model file
	 *        Returns 0 if valid and no error ocurred
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.18:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setDebugLevel(unsigned int n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Debugging is set 0 ( Off ) to 5 ( Highest )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.05:
	 * Mongoose - Now supports multiple levels 
	 *
	 * 2001.01.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :  All data is reset and cleared
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int saveFile(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves model to disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadFile(char *filename);
	/*------------------------------------------------------
	 * Pre  : File is valid format and exists
	 * Post : Loads model from disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	// Mongoose 2002.07.05, Reorder from here down later... to be broken up

	unsigned int getTexelCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return number of texels
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_texel_t *getTexel(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return texel by id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int addTexel(float s, float t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Add a new texel, return id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void delTexel(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete texel by id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void delTexel(egg_texel_t *t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete texel by data value
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void combineTexels(unsigned int A, unsigned int B);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete texel B and replace references to B with A
	 *        ( The texels are combined into one )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.02.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getVertexCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return number of vertices
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void combineVertices(unsigned int A, unsigned int B);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete vertex B and replace references to B with A
	 *        ( The vertices are combined into one )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.02.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *getVertex(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex with id
	 -----------------------------------------*/

	egg_vertex_t *addVertex(vec_t x, vec_t y, vec_t z,
							vec_t nx, vec_t ny, vec_t nz,
							vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : x, y, z is position in 3 space 
	 *        nx, ny, nz is normal vector
	 *        u, v is the texel coord
	 *
	 * Post : Generates a new vertex and returns a 
	 *        pointer to it
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.07.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *addVertex(vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Add a new vertex, return id
	 -----------------------------------------*/

	void delVertex(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete vertex by id
	 -----------------------------------------*/

	void delVertex(egg_vertex_t *v);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete vertex by data
	 -----------------------------------------*/


	unsigned int getPolygonCount();
	/*-----------------------------------------
	 * Created  : 2000-10-25, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return number of polygons
	 -----------------------------------------*/


	egg_polygon_t *getPolygon(unsigned int);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return polygon with id
	 -----------------------------------------*/

	unsigned int addPolygon(Vector<unsigned int> &vertex, 
							Vector<unsigned int> &texel, 
							int shader);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  : vertex and texel lists are valid
	 *        shader/texture id is valid
	 * Post : Add a new polygon, return id
	 -----------------------------------------*/

	unsigned int selectPolygon(Vector<unsigned int> *list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by matching 
	 *        vertex list
	 -----------------------------------------*/

	void delPolygon(Vector<unsigned int> *list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by matching 
	 *        vertex list
	 -----------------------------------------*/

	void delPolygon(egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by data
	 -----------------------------------------*/

	void delPolygon(unsigned int polygon);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by id
	 -----------------------------------------*/


	/// GROUPS ////////////////////////////////////

	unsigned int getGroupCount();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return number of vertex groupings
	 -----------------------------------------*/

	egg_group_t *getGroup(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return vertex grouping with id
	 -----------------------------------------*/

	egg_group_t *newGroup();
	/*-----------------------------------------
	 * Created  : 2000-08-15, Mongoose
	 * Modified : 
	 * Comments : This makes a new initialized
	 *            egg_group_t, without linking
	 *            it to the main _group list
	 *            ( id is set to error value )
	 -----------------------------------------*/

	void addGroup(egg_group_t *group);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * Comments : This takes a group and 
	 *            registers it with the egg list
	 -----------------------------------------*/

	void delGroup(unsigned int group);
	void delGroup(egg_group_t *group);

	void copyGroup(unsigned int group);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : This buffers a copy of this
	 *            group internally in Egg
	 -----------------------------------------*/

	void pasteGroup(unsigned int group);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : ( paste into another group )
	 *            This takes the group_buffer
	 *            internally in Egg and merges
	 *            it with the linked group
	 *            with id 'group' 
	 -----------------------------------------*/

	void cutGroup(unsigned int group);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : This buffers a copy of this
	 *            group internally in Egg,
	 *            and unlinks it from Egg list
	 -----------------------------------------*/

	void GroupTransform(unsigned int group, enum egg_transform type, 
						vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : Performs matrix transform 
	 *            on group
	 *
	 *            Also See Transform()
	 -----------------------------------------*/

	void GroupMirror(unsigned int group, bool x, bool y, bool z);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : Mirror this group on x, y,
	 *            and/or z axes
	 -----------------------------------------*/

	egg_group_t *GroupUnion(unsigned int groupA, unsigned int groupB);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : Return a new group that is
	 *            the union of A and B
	 -----------------------------------------*/

	egg_group_t *GroupIntersection(unsigned int groupA, unsigned int groupB);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : group is valid group id
	 * Comments : Return a new group that is
	 *            the intersection of A and B
	 -----------------------------------------*/


	/// MESHES ////////////////////////////////////

	unsigned int getMeshCount();
	/*-----------------------------------------
	 * Created  : 2000-10-25, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return number of meshes
	 -----------------------------------------*/

	egg_mesh_t *newMesh();
	/*-----------------------------------------
	 * Created  : 2000-08-15, Mongoose
	 * Modified : 
	 * Comments : This makes a new initialized
	 *            egg_mesh_t, without linking
	 *            it to the main _mesh list
	 *            ( id is set to error value )
	 -----------------------------------------*/ 
  
	void addMesh(egg_mesh_t *mesh);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * Comments : This takes a mesh and 
	 *            registers it with the egg list
	 -----------------------------------------*/

	void delMesh(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid mesh id
	 * Post : Removes mesh from model
	 -----------------------------------------*/

	void delMesh(egg_mesh_t *mesh);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh, that's
	 *        in the list
	 * Post : Removes mesh from model
	 -----------------------------------------*/

	egg_mesh_t *getMesh(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid mesh id
	 * Post : Returns mesh with id from model
	 -----------------------------------------*/

	int MeshSave(egg_mesh_t *mesh, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid mesh chunk
	 * Post : Saves mesh chunk to disk file
	 -----------------------------------------*/

	egg_mesh_t *MeshLoad(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid mesh chunk
	 * Post : Loads mesh chunk from disk file 
	 -----------------------------------------*/
    
	void MeshMirror(egg_mesh_t *mesh);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 * Post : Mirrors the mesh over the y axis
	 -----------------------------------------*/

	void MeshGenerateDuplicateFrame(int frame_id);
	/*-----------------------------------------
	 * Created  : 2000-10-17, Mongoose
	 * Modified : 
	 *
	 * Pre  : frame_id is a vliad frame id
	 *        
	 * Post : Makes a new vertex frame for all
	 *        meshs, using it's frame[frame_id]
	 *        to a copy from
	 -----------------------------------------*/

	void MeshGenerateDuplicateFrame(egg_mesh_t *mesh, int frame_id);
	/*-----------------------------------------
	 * Created  : 2000-10-17, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 *        frame_id is a vliad frame id
	 * Post : Makes a new vertex frame for this
	 *        mesh, it's a copy of frame_id
	 -----------------------------------------*/

	egg_mesh_t *MeshCopy(egg_mesh_t *mesh, Vector<unsigned int> *list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 *        list is a valid vertex list of mesh
	 * Post : Returns newly allocated mesh
	 *        that is a copy of mesh 
	 *        determined by the list
	 -----------------------------------------*/

	egg_mesh_t *MeshMerge(egg_mesh_t *meshA, Vector<egg_vertex_t *> *list, 
						  egg_mesh_t *meshB);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : meshA is a valid mesh
	 *        list is a valid vertex list of meshA
	 *        meshB is a valid mesh
	 * Post : Makes one mesh of of two, by
	 *        merging vertices in list with B
	 *        and removing from A 
	 *
	 *        Passing all vertices makes a 
	 *        full merge
	 -----------------------------------------*/

	egg_mesh_t *MeshSplit(egg_mesh_t *mesh, Vector<egg_vertex_t *> *list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh
	 *        list is a valid vertex list of mesh
	 * Post : Removes the vertices in list
	 *        from mesh, then generates a new
	 *        mesh ( splits one mesh into two )
	 *        The newly made mesh is returned
	 -----------------------------------------*/    


	/// TAGS //////////////////////////////////////

	unsigned int getTagCount();
	/*-----------------------------------------
	 * Created  : 2000-10-25, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return number of bone tags
	 -----------------------------------------*/

	int saveTag(egg_tag_t *bone, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid tag chunk
	 * Post : Saves tag chunk to disk file
	 -----------------------------------------*/

	egg_tag_t *loadTag(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid tag chunk
	 * Post : Loads tag chunk from disk file
	 -----------------------------------------*/

	void delTag(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid tag id
	 * Post : tag is removed
	 -----------------------------------------*/  

	void delTag(egg_tag_t *bone);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is valid
	 * Post : tag is removed
	 -----------------------------------------*/

	egg_tag_t *getTag(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid tag id
	 * Post : Returns tag
	 -----------------------------------------*/

	void connectTag(unsigned int master, unsigned int slave);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : master and slave are valid tag ids
	 * Post : Connects slave to master
	 -----------------------------------------*/
 
	void TagDisconnect(unsigned int master, unsigned int slave);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : master and slave are valid tag ids
	 * Post : Disconnects slave from master
	 -----------------------------------------*/

	egg_tag_t *addTag(vec_t x, vec_t y, vec_t z, char flag);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : x,y,z are valid coors
	 *        flag is valid flag
	 *        
	 * Post : Returns newly allocated tag
	 *        with pivot set at x,y,z with flag,
	 *        and transform is set to I
	 -----------------------------------------*/

	void addTag(egg_tag_t *tag);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is a valid bone
	 * Post : tag is added to the bone list
	 -----------------------------------------*/
  
	void TagRotateAbout(unsigned int tag, vec_t rx, vec_t ry, vec_t rz);
	/*--------------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid bone id
	 *        frame is valid frame id
	 *        r(x, y, z) are in degrees
	 *
	 * Post : Rotates 'banded' bone on a pivot,
	 *        which in turn rotates meshes bound
	 *        to this bone and slave bones
	 --------------------------------------------*/


	void TagRotateAboutPoint(unsigned int bone, vec3_t p,
							 vec_t rx, vec_t ry, vec_t rz);
	/*--------------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid bone id
	 *        frame is valid frame id
	 *        p is a valid point in 3 space
	 *        r(x, y, z) are in degrees
	 *
	 * Post : Rotates 'banded' bone on point p,
	 *        which in turn rotates meshes bound
	 *        to this bone and slave bones
	 --------------------------------------------*/

	void TagRotate(int bone, int frame, vec_t rx, vec_t ry, vec_t rz);
	/*-----------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is a valid bone id
	 *        frame is a valid frame id
	 *        rx, ry, rz are in degrees
	 * Post : Rotates bone by rx,ry,rz in frame
	 -----------------------------------------*/

	void TagAddMesh(egg_tag_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid 
	 * Post : mesh is added to the bone's mesh list
	 -----------------------------------------*/

	void TagDelMesh(egg_tag_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid
	 * Post : mesh is removed to the bone's mesh list
	 -----------------------------------------*/
  

	/// BoneFrame ///////////////////


	unsigned int getBoneFrameCount();
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : 
	 * Post : Returns number of bone frames
	 -----------------------------------------*/

	void BoneFrameAdd(egg_boneframe_t *boneframe);

	int BoneFrameSave(egg_boneframe_t *frame, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Saves frame chunk to disk file
	 -----------------------------------------*/

	egg_boneframe_t *BoneFrameLoad(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Loads frame chunk to disk file
	 -----------------------------------------*/

	unsigned int BoneFrameAdd(vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : x,y,z are valid coors
	 * Post : Adds a new skeletal animation 
	 *        frame
	 -----------------------------------------*/

	egg_boneframe_t *BoneFrame(unsigned int frame);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : frame is valid boneframe id
	 * Post : Returns boneframe
	 -----------------------------------------*/
    

	// Animations ///////////////////

	unsigned int getAnimationCount();
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : 
	 * Post : Returns number of bone animframes
	 -----------------------------------------*/

	void addAnimation(egg_animation_t *a);

	egg_animation_t *getAnimation(unsigned int frame);

	int AnimationSave(egg_animation_t *frame, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Saves aframe chunk to disk file
	 -----------------------------------------*/

	egg_animation_t *AnimationLoad(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Loads aframe chunk from disk file
	 -----------------------------------------*/

	////////////////////////////////////////////////
	// Transforms
	////////////////////////////////////////////////

	void Transform(egg_tag_t *tag, enum egg_transform type,
				   vec_t x, vec_t y, vec_t z);
	/*--------------------------------------------
	 * Created  : 2000-09-15 by Mongoose
	 * Modified : 
	 *
	 * Pre  : tag exists
	 *        Var type E { ROTATE, SCALE, TRANSLATE }
	 *        Vars x, y, z are valid for type
	 *
	 * Post : Transform 'type' is done to tag
	 --------------------------------------------*/

	void Transform(Vector<egg_vertex_t *> *list, enum egg_transform type,
				   vec_t x, vec_t y, vec_t z);
	/*--------------------------------------------
	 * Created  : 2000-09-15 by Mongoose
	 * Modified : 
	 *
	 * Pre  : list exists
	 *        Var type E { ROTATE, SCALE, TRANSLATE }
	 *        Vars x, y, z are valid for type
	 *
	 * Post : Transform 'type' is done to list
	 --------------------------------------------*/

	void Transform(egg_group_t *frame, enum egg_transform type, 
				   vec_t x, vec_t y, vec_t z);
	/*--------------------------------------------
	 * Created  : 2000-09-15 by Mongoose
	 * Modified : 
	 *
	 * Pre  : frame exists
	 *        Var type E { ROTATE, SCALE, TRANSLATE }
	 *        Vars x, y, z are valid for type
	 *
	 * Post : Transform 'type' is done to frame
	 --------------------------------------------*/

	void Transform(egg_mesh_t *mesh, enum egg_transform type, 
				   vec_t x, vec_t y, vec_t z);
	/*--------------------------------------------
	 * Created  : 2000-09-15 by Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh exists
	 *        Var type E { ROTATE, SCALE, TRANSLATE }
	 *        Vars x, y, z are valid for type
	 *
	 * Post : Transform 'type' is done to mesh
	 --------------------------------------------*/

	void Transform(enum egg_transform type, vec_t x, vec_t y, vec_t z);
	/*--------------------------------------------
	 * Created  : 2000-07-31 by Mongoose
	 * Modified : 
	 *
	 * Pre  : Var type E { ROTATE, SCALE, TRANSLATE }
	 *        Vars x, y, z are valid for type
	 *
	 * Post : Transform 'type' is done to entire scene
	 --------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public, but should be protected Mutators  ;)
	////////////////////////////////////////////////////////////

	Vector <egg_texel_t *> *TexelList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return texel list
	 -----------------------------------------*/

	Vector <egg_vertex_t *> *VertexList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_polygon_t *> *PolygonList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_group_t *> *GroupList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_mesh_t *> *MeshList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return mesh list
	 -----------------------------------------*/

	Vector<egg_tag_t *> *TagList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return bone list
	 -----------------------------------------*/

	Vector<egg_boneframe_t *> *BoneFrameList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return BoneFrame list
	 -----------------------------------------*/

	Vector<egg_animation_t *> *AnimationList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return BoneFrame list
	 -----------------------------------------*/

protected:

	////////////////////////////////////////////////////////////
	// Protected Accessors
	////////////////////////////////////////////////////////////

	bool isDebugLevel(unsigned int level);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if debug level is greater or equal
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.06:
	 * Mongoose - Created, split from printDebug
	 ------------------------------------------------------*/

	virtual void print(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.06:
	 * Mongoose - Removed internal level check for speed
	 *            and lower overhead, new ABI was printDebug
	 *
	 * 2002.07.05: 
	 * Mongoose - Debug level
	 *
	 * 2001.01.31: 
	 * Mongoose - Debug toggle
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void printError(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : String and args are valid
	 * Post : Report an error to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	////////////////////////////////////////////////////////////
	// Protected Mutators
	////////////////////////////////////////////////////////////


private:
	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
	bool PolygonMatch(Vector<unsigned int> *list, egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2001-02-03, Mongoose
	 * Modified : 
	 *
	 * Pre  : list is valid
	 *        polygon is valid
	 * Post : True if the vertex index list
	 *        matches the one used by the polygon
	 -----------------------------------------*/

	bool VertexInPolygon(unsigned int vertex, egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : Vertex id vertex is valid
	 *        polygon is valid
	 * Post : True if the vertex is used by 
	 *        the polygon
	 -----------------------------------------*/

	void resizeBoundingBox(egg_group_t *grp, vec3_t p);
	/*-----------------------------------------
	 * Created  : 2000-10-14, Mongoose
	 * Modified : 2001-01-31, Mongoose
	 *            + Eggv8 port
	 *
	 * Pre  : grp and point are valid
	 * Post : Resizes the bounding box of
	 *        group, given a newly added point
	 *        as the only changed vertex
	 -----------------------------------------*/

	void resizeBoundingBox(egg_group_t *grp);
	/*-----------------------------------------
	 * Created  : 2000-10-14, Mongoose
	 * Modified : 2001-01-31, Mongoose
	 *            + Eggv8 port
	 *
	 * Pre  : grp is valid
	 * Post : Resizes the bounding box of
	 *        group, doing a full reclaculation
	 -----------------------------------------*/

	Vector<egg_vertex_t *> mVertices;        /* Vertex list */

	Vector<egg_texel_t *> mTexels;           /* Texel list */
	
	Vector<egg_polygon_t *> mPolygons;       /* Polygon list */
	
	Vector<egg_group_t *> mGroups;           /* Group list */
	
	Vector<egg_mesh_t *> mMeshes;            /* Mesh list */
	
	Vector<egg_tag_t *> mTags;               /* Bone tag list */
	
	Vector<egg_boneframe_t *> mBoneFrames;   /* BoneFrame list */
	
	Vector<egg_animation_t *> mAnimations;   /* Animation list */

	unsigned int mDebugLevel;                /* Set debug output at runtime */
};

#endif
