/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja, GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Egg
 * License : GPL, also (C) 2000-2004 Mongoose
 * Comments: This is the data model agent of Egg models
 *
 * 
 *-- Test Defines ----------------------------------------------------
 *           
 * UNIT_TEST_EGG  - Builds module as test code as a console program
 *
 *-- History --------------------------------------------------------- 
 *
 * 2004.04.08:
 * Mongoose - All new API using Hel math/physics lib, endian safe
 *            file I/O, and generic methods.
 *
 *            Metadata chunk/type back
 *
 *            Removed a ton of code.
 *
 *            Renamed texels to texcoords, since they haven't been
 *            texels for several years now.
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
 ===========================================================================*/

#ifndef GUARD__MONGOOSE_GOOSEEGG_EGG_H
#define GUARD__MONGOOSE_GOOSEEGG_EGG_H

#include <mstl/Vector.h>
#include <mstl/Map.h>

#include <hel/math.h>
#include <hel/Matrix.h>


#define EGG_LIB_VERSION        0x30302E39  // '9.00'

/* File header, versioning */
#define EGG_FILE               0x20676745  // 'EGG '
#define EGG_VERSION            0x30302E39  // '9.00'
#define EGG_8_12               0x32312E38  // '8.12'

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


typedef struct {
	unsigned int id;               /* Unique identifier */

	unsigned int type;
	unsigned int size;


} egg_metadata_t;

typedef struct {
	vec_t weight;                     /* Weight for vertex use */
	unsigned int bone;                /* Bone id */

} egg_weight_t;


typedef struct egg_vertex_s {
	unsigned int id;                 /* Unique identifier */
	Vector <unsigned int> ref;       /* Polygons referencing this vertex */ 

	unsigned int pos;                /* Index to Vertex position */
	unsigned int normal;             /* Index to Normal vector */
	unsigned int uvw;                /* Index of TexCoord coordinate */

	Vector<egg_weight_t *> weights;  /* Vector of weights */

} egg_vertex_t;


typedef struct egg_polygon_s {
	unsigned int id;                  /* Unique identifier */

	int material;                     /* Material id */
	Vector <unsigned int> vertex;     /* Vertices composing polygon */

} egg_polygon_t;


typedef struct egg_group_s {
	int id;                           /* Unique identifier */

	Vector <unsigned int> vertex;     /* Vertices composing group */
	Vector3d bboxMin;                 /* Min corner of bounding box */
	Vector3d bboxMax;                 /* Max corner of bounding box */
	Vector3d center;                  /* Center / bolt-on binding */

	vec_t scale;                      /* Scaling of group */

} egg_group_t;


typedef struct egg_vertexframe_s {

	unsigned int mesh;                /* Mesh using this frame */
	Vector<vec3_t> frame;             /* Vertex animation frames */

} egg_vertexframe_t;


typedef struct egg_mesh_s {
	int id;                             /* Unique identifier */
	Vector <unsigned int> groups;       /* Groups/Frames of this mesh */
	Vector <unsigned int> polygons;     /* Polygons of this mesh */

	unsigned int currentFrame;          /* Vertex morph frames */
	Vector <egg_vertexframe_t *> frames;

} egg_mesh_t;


typedef struct egg_bone_s {
	int id;                          /* Unique identifier */

	unsigned int parent;
	char name[64];

	Vector <unsigned int> children;  /* Children bones */
	Vector <unsigned int> meshes;    /* Meshes bound to this bone */

	Vector3d position;               /* Pivot point, center of rotation */
	Vector3d rotation;               /* Rotation on XYZ axies in degrees  */

	Matrix matrix;                   /* Transform mesh/slaves by this matrix */
} egg_bone_t;


typedef struct egg_skeleton_s {
	int id;                           /* Unique identifier */

	unsigned int root;                /* Root bone index */
	Vector<unsigned int> bones;       /* Bones in this skeleton */
	Vector3d center;                  /* Position */

} egg_skeleton_t;



typedef struct egg_animation_s {
	int id;                           /* Unique identifier */

	vec_t time;
	vec_t time2;

	Vector<unsigned int> boneframes; 
	Vector<unsigned int> meshframes; 

} egg_animation_t;


typedef struct {

	// meshes
	// bones
	// animations
	// metadata

} egg_model_t;

#ifndef __print_unsigned_int
void __print_unsigned_int(unsigned int u);
#endif
void __print_egg_vertex_t(egg_vertex_t *v);
void __print_egg_texcoord_t(egg_texcoord_t *t);
void __print_egg_polygon_t(egg_polygon_t *p);
/*!----------------------------------------
 * Created  : 2001-05-15, Mongoose
 * Modified : 
 * 
 * Pre  : 
 * Post : Various List support functions
 *        for local data types
 -----------------------------------------*/

class Egg
{
public:

	typedef enum {
		VERTEX     = 0x54524556,
		TEXCOORD   = 0x54455843,
		POLYGON    = 0x504F4C59,
		GROUP      = 0x56475250,
		MESH       = 0x4D455348,
		BONE       = 0x424F4E45,
		SKELETON   = 0x534B454C,
		ANIMATION  = 0x414E494D,
		METADATA   = 0x4D455441
		
	} egg_type_t;


	typedef enum {
		
		SCALE               = 0, 
		ROTATE              = 1, 
		TRANSLATE           = 2, 
		ROTATE_ABOUT_CENTER = 3,
		SCALE_ABOUT_CENTER  = 4
	} egg_transform_t;
	

	typedef enum {
		UNION = 1,
        INTERSECTION,
		DIFFERENCE
		
	} egg_csg_t;


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

	~Egg();
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

	int checkFile(const char *filename);
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

	unsigned int getCount(egg_type_t type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns nearest TYPE object id to given point
	 *
	 *-- History ------------------------------------------
	 * 
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2001.11.29: 
	 * Mongoose - API update to use unsigned count
	 *
	 * 1999.08.01:
	 * Mongoose - Created ( As separate methods per type )
	 ------------------------------------------------------*/

	unsigned int getNearest(egg_type_t type, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns nearest TYPE object id to given point
	 *
	 *-- History ------------------------------------------
	 * 
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *
	 * 2001.11.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int saveFileASCII(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int saveFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves model to disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void erase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :  All data is reset and cleared
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void combine(egg_type_t type, unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Combines TYPE object A and B in model
	 *
	 *        Destroys A and B then replaces them with new 
	 *        object with index A where: A = A + B
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int csg(egg_type_t type, egg_csg_t operation,
					 unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B in model
	 *
	 *        A and B are perserved, creates new object C 
	 *        where: C = A OPERATION B
	 *
	 *        UNION        : C = A u B
	 *                       if A eq B then copy of A is made
	 *
	 *        INTERSECTION : C = A n B
	 *                       if A eq B then copy of A is made
	 *
	 *        DIFFERENCE   : C = A - B
	 *                       B culls A, order matters
	 *                       if A eq B then undefined behavior
	 *
	 *        Returns C's index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created with new generic API based on mtk
	 ------------------------------------------------------*/

	void remove(egg_type_t type, unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Remove TYPE object[index] from model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void transform(egg_type_t type, egg_transform_t transform, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setDebugLevel(unsigned int level);
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

	void addMetaData(unsigned int id, unsigned int type, unsigned int size, 
					 unsigned char *metadata);

	egg_texcoord_t *getTexCoord(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return texcoord by id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int addTexCoord(vec_t u, vec_t v, vec_t w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Add a new texcoord, return id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void delTexCoord(egg_texcoord_t *t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete texcoord by data value
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
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
							vec_t u, vec_t v, vec_t w);
	/*------------------------------------------------------
	 * Pre  : x, y, z is position in 3 space 
	 *        nx, ny, nz is normal vector
	 *        u, v, w is the texcoord coord
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

	void delVertex(egg_vertex_t *v);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete vertex by data
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
							Vector<unsigned int> &texcoord, 
							int shader);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  : vertex and texcoord lists are valid
	 *        shader/texture id is valid
	 * Post : Add a new polygon, return id
	 -----------------------------------------*/

	unsigned int selectPolygon(Vector<unsigned int> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by matching 
	 *        vertex list
	 -----------------------------------------*/

	void delPolygon(Vector<unsigned int> &list);
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

	/// MESHES ////////////////////////////////////

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


    




	/// TAGS //////////////////////////////////////



	void delTag(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid tag id
	 * Post : tag is removed
	 -----------------------------------------*/  

	void delTag(egg_bone_t *bone);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is valid
	 * Post : tag is removed
	 -----------------------------------------*/

	egg_bone_t *getTag(unsigned int id);
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
 
	void disconnectTag(unsigned int master, unsigned int slave);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : master and slave are valid tag ids
	 * Post : Disconnects slave from master
	 -----------------------------------------*/

	egg_bone_t *addTag(vec_t x, vec_t y, vec_t z, char flag);
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

	void addTag(egg_bone_t *tag);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is a valid bone
	 * Post : tag is added to the bone list
	 -----------------------------------------*/
  
	void rotateTagAboutOrigin(unsigned int tag, vec_t rx, vec_t ry, vec_t rz);
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

	void rotateTagAboutPoint(unsigned int bone, vec3_t p,
							 vec_t rx, vec_t ry, vec_t rz);
	/*--------------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid bone id
	 *        frame is valid frame id
	 *        p is a valid mtk point
	 *        r(x, y, z) are in degrees
	 *
	 * Post : Rotates 'banded' bone on point p,
	 *        which in turn rotates meshes bound
	 *        to this bone and slave bones
	 --------------------------------------------*/

	void rotateTag(int bone, int frame, vec_t rx, vec_t ry, vec_t rz);
	/*-----------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is a valid bone id
	 *        frame is a valid frame id
	 *        rx, ry, rz are in degrees
	 * Post : Rotates bone by rx,ry,rz in frame
	 -----------------------------------------*/

	void addTagMesh(egg_bone_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid 
	 * Post : mesh is added to the bone's mesh list
	 -----------------------------------------*/

	void delTagMesh(egg_bone_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid
	 * Post : mesh is removed to the bone's mesh list
	 -----------------------------------------*/
  

	/// BoneFrame ///////////////////

	void addBoneFrame(egg_skeleton_t *boneframe);

	unsigned int addBoneFrame(vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : x,y,z are valid coors
	 * Post : Adds a new skeletal animation 
	 *        frame
	 -----------------------------------------*/

	egg_skeleton_t *getBoneFrame(unsigned int frame);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : frame is valid boneframe id
	 * Post : Returns boneframe
	 -----------------------------------------*/
    

	// Animations ///////////////////

	void addAnimation(egg_animation_t *a);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Creates new animframe
	 -----------------------------------------*/

	egg_animation_t *getAnimation(unsigned int frame);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : frame is valid animframe id
	 * Post : Returns boneframe
	 -----------------------------------------*/





	////////////////////////////////////////////////////////////
	// Public, but should be protected Mutators  ;)
	////////////////////////////////////////////////////////////

	Vector <egg_texcoord_t *> *getTexCoordList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return texcoord list
	 -----------------------------------------*/

	Vector <egg_vertex_t *> *getVertexList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_polygon_t *> *getPolygonList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_group_t *> *getGroupList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_mesh_t *> *getMeshList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return mesh list
	 -----------------------------------------*/

	Vector<egg_bone_t *> *getTagList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return bone list
	 -----------------------------------------*/

	Vector<egg_skeleton_t *> *getBoneFrameList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return BoneFrame list
	 -----------------------------------------*/

private:
	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	int saveAnimation(egg_animation_t *frame, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Saves aframe chunk to disk file
	 -----------------------------------------*/

	int saveBoneFrame(egg_skeleton_t *frame, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Saves frame chunk to disk file
	 -----------------------------------------*/

	int saveMesh(egg_mesh_t *mesh, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid mesh chunk
	 * Post : Saves mesh chunk to disk file
	 -----------------------------------------*/

	int saveTag(egg_bone_t *bone, FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid tag chunk
	 * Post : Saves tag chunk to disk file
	 -----------------------------------------*/

	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool isMatchForPolygon(Vector<unsigned int> &list, 
						   egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2001-02-03, Mongoose
	 * Modified : 
	 *
	 * Pre  : list is valid
	 *        polygon is valid
	 * Post : True if the vertex index list
	 *        matches the one used by the polygon
	 -----------------------------------------*/

	bool isVertexInPolygon(unsigned int vertex, egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : Vertex id vertex is valid
	 *        polygon is valid
	 * Post : True if the vertex is used by 
	 *        the polygon
	 -----------------------------------------*/

	egg_animation_t *loadAnimation(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Loads aframe chunk from disk file
	 -----------------------------------------*/

	egg_skeleton_t *loadBoneFrame(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid frame chunk
	 * Post : Loads frame chunk to disk file
	 -----------------------------------------*/

	egg_mesh_t *loadMesh(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid mesh chunk
	 * Post : Loads mesh chunk from disk file 
	 -----------------------------------------*/

	egg_bone_t *loadTag(FILE *f);
	/*!----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  : f is set to start of valid tag chunk
	 * Post : Loads tag chunk from disk file
	 -----------------------------------------*/

	void printDebug(unsigned int level, char *s, ...);
	/*------------------------------------------------------
	 * Pre  : String, level, and args are valid
	 * Post : Report debug messages to stdout
	 *
	 *-- History ------------------------------------------
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

	void printError(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : String and args are valid
	 * Post : Report an error to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

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

	/* The arrays are pointers to the data in the vectors for speed */

	Vector<egg_vertex_t *> mVertices;        /* Vertex containers */
	Vector<vec_t> mVertexVector;             /* XYZ position of vertex */
	Vector<unsigned int> mDeadVertex;        /* Indices of unreferenced data */
	vec3_t *mVertexArray;                    /* Pointer to data vector array */

	Vector<vec_t> mTexCoordVector;
	vec3_t *mTexCoordArray;

	Vector<vec_t> mNormalVector;
	vec3_t *mNormalArray;

	Vector<egg_texcoord_t *> mTexCoords;     /* TexCoord list */
	
	Vector<egg_polygon_t *> mPolygons;       /* Polygon list */
	
	Vector<egg_group_t *> mGroups;           /* Group list */
	
	Vector<egg_mesh_t *> mMeshes;            /* Mesh list */
	
	Vector<egg_bone_t *> mTags;               /* Bone tag list */
	
	Vector<egg_skeleton_t *> mBoneFrames;   /* BoneFrame list */
	
	Vector<egg_animation_t *> mAnimations;   /* Animation list */

	unsigned int mDebugLevel;                /* Set debug output at runtime */
};

#endif
