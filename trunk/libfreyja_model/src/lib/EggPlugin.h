/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 *           EggPlugin alters Egg's state when used
 *
 *           Always assume Egg to have different iterator
 *           and data states once EggPlugin is called on it
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.23: (v0.0.3+)
 * Mongoose - New singleton implementation
 *            New plugin features for texture loading
 *            Removed last of old List STL use
 *
 *            New style, which affects compiler portability
 *            Mostly removing leading _underscores
 *
 * 200?.??.??: (v0.0.3)
 * Mongoose - Wha' Ha'pen?
 *
 * 2001.11.02: (v0.0.2)
 * Mongoose - Experimental export API added 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==============================================================*/

#ifndef GUARD__MONGOOSE_GOOSEEGG_EGGPLUGIN_H
#define GUARD__MONGOOSE_GOOSEEGG_EGGPLUGIN_H

#include <mstl/Stack.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "Egg.h"


#define GEGG_PLUGIN_VERSION  "0.0.3"
#define FREYJA_LIST_RESET    -2
#define FREYJA_LIST_NEXT     -1
#define FREYJA_LIST_CURRENT  -3
#define FREYJA_LIST_SIZE     -4
#define PLUGIN_ERROR         4294967295U


typedef enum {
	EGG_WRITE_LOCK = 1,
	EGG_WRITE_UNLOCK
	
} egg_lock_t;


typedef enum {
	FREYJA_MESH = 1,
	FREYJA_POLYGON,
	FREYJA_BONE_TAG,
	FREYJA_BONE_FRAME,
	FREYJA_GROUP,
	FREYJA_VERTEX,
	FREYJA_TEXEL,
	FREYJA_MESHTREE_ANIM
	
} egg_plugin_t;


typedef enum {
	FL_EGGP_LOAD_MAP = 1
	
} egg_plugin_flags_t;


typedef enum {
	EGG_COLORMODE_INDEXED = 1, 
	EGG_COLORMODE_RGB     = 2, 
	EGG_COLORMODE_RGBA    = 3
	
} egg_colormode_t;


class EggTextureData
{
public:
	EggTextureData()
	{
		mType = EGG_COLORMODE_RGBA;
		mImage = 0x0;
		mPalette = 0x0;
		mWidth = 0;
		mHeight = 0;
		mBitDepth = 0;
	}

	~EggTextureData()
	{
		if (mImage)
			delete [] mImage;

		if (mPalette)
			delete [] mPalette;
	}

	egg_colormode_t mType;
	unsigned char *mImage;
	unsigned char *mPalette;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mBitDepth;
};


/* Some helper functions for making meshes and UV generation */
void eggGenerateVertexNormals();

int eggGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v);

void eggGenerateCube(vec_t sideLenght);

void eggGenerateCylinder(unsigned int major, unsigned int minor, 
						 vec_t height, vec_t radius);

void eggGenerateTriangleStrip(unsigned int count, vec_t sz);

void eggGenerateSphere(unsigned int major, unsigned int minor, vec_t radius);

Vector<unsigned int> *eggFindVerticesByBox(bbox_t bbox);

Vector<unsigned int> *eggFindVerticesInBox(bbox_t bbox,
										   Vector<unsigned int> &vertices);


int eggGetTextureFilename(unsigned int index, char **filename);

int eggGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image);

///////////////////////////////////////////////////////////////////////


unsigned int eggBegin(egg_plugin_t type);
/*------------------------------------------------------
 * Pre  : Type is valid egg plugin enum
 * Post : A new data object had begun construction
 *        Returns an object id
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggEnd();
/*------------------------------------------------------
 * Pre  : 
 * Post : A data object has finished construction
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

int eggTextureStore(EggTextureData *textureData);
/*------------------------------------------------------
 * Pre  : Texture data is valid
 *
 * Post : Stores texture data to be called by modeler
 *        and used to generate textures
 *
 *        Returns either texture id or -1 for error
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTexelStore2f(float s, float t);
/*------------------------------------------------------
 * Pre  : s, t are 0.0 to 1.0 texels
 * Post : A new texel is created in the model
 *        Returns the native index of that texel
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggVertexStore3f(float x, float y, float z);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_GROUP);
 *        x,y,z are valid 3space coors
 * Post : A new vertex created in the model
 *        Returns the native index of that vertex
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggVertexWeightStore(unsigned int index, vec_t weight, unsigned int bone);
/*------------------------------------------------------
 * Pre  : <weight> of influence of <bone> on vertex v
 *
 * Post : Vertex <index> in the model gets weight added
 *        to influence list, if their is a weight for the
 *        corresponding bone it is replaced
 *
 *        <weight> <= 0.0 removes weight
 *
 *        All weights for the vertex combined must be 1.0
 *-- History ------------------------------------------
 *
 * 2004.03.25:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggVertexUVStore2f(unsigned int vIndex, float u, float v);
/*------------------------------------------------------
 * Pre  : (u,v) is texel for vertex v
 *
 * Post : Vertex 'v' in the model gets normal set
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggVertexNormalStore3f(unsigned int vIndex, float x, float y, float z);
/*------------------------------------------------------
 * Pre  : (x,y,z) is the normal vector for vertex v
 *
 * Post : Vertex 'v' in the model gets normal set
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggVertex1i(unsigned int egg_id);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_POLYGON);
 *        Egg_id is the native index
 * Post : Adds a vertex to a polygon
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggTexel1i(unsigned int egg_id);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_POLYGON);
 *        Egg_id is the native index
 * Post : Adds a texel to a polygon
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

void eggTexture1i(int id);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_POLYGON);
 *        Egg_id is the native index
 * Post : Adds a texture to a polygon
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggFlags();
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets plugin flags
 *
 *-- History ------------------------------------------
 *
 * 2001.05.16:
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGroupCenter3f(float x, float y, float z);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_GROUP);
 * Post : Current group's center is set
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagName(char *name);

unsigned int eggTagPos3f(float x, float y, float z);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : Current tag's origin is set
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagFlags1u(unsigned int flags);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : Current tag's mesh/tag tree flag is set
 *
 *        0x00 - push
 *        0x01 - pop
 *
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagAddMesh1u(unsigned int mesh);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : mesh added to current tag's mesh list
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagAddSlave1u(unsigned int tag);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : mesh added to current tag's slave tag list
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggMeshTreeAddFrame3f(float x, float y, float z);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM);
 * Post : Adds meshtree frame to meshtree
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagRotate3f(float x, float y, float z);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : Sets tag rotation with eular angles
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTagRotateQuaternion4f(vec_t x, vec_t y, vec_t z, vec_t w);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_TAG);
 * Post : Sets tag rotation with quaterion
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggMeshTreeTag1u(unsigned int tag);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
 * Post : Appends tag to mesh tree frame
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggTextureStoreBuffer(unsigned char *image, unsigned int depth,
								   unsigned int width, unsigned int height,
								   egg_colormode_t type);
/*------------------------------------------------------
 * Pre  : Image is a valid image buffer 
 *        Depth, Width, and Height are correct for image
 *        Type is correct for image ( matches )
 *
 * Post : Loads texture into manager
 *        Returns 0 if sucessful
 *
 *-- History ------------------------------------------
 *
 * 2002.02.09:
 * Mongoose - Created, to replace current texture handler
 ------------------------------------------------------*/

unsigned int eggTextureStoreFilename(char *filename);
/*------------------------------------------------------
 * Pre  : Image is a valid image buffer 
 *        Depth, Width, and Height are correct for image
 *        Type is correct for image ( matches )
 *
 * Post : Loads texture into manager
 *        Returns 0 if sucessful
 *
 *-- History ------------------------------------------
 *
 * 2002.02.09:
 * Mongoose - Created, to replace current texture handler
 ------------------------------------------------------*/


// Accesors /////////////////////////////////////

unsigned int eggGetNum(egg_plugin_t type);
/*------------------------------------------------------
 * Pre  : Type is valid egg plugin enum
 * Post : Returns total number of objects of type in model
 *        Returns PLUGIN_ERROR on error
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggIterator(egg_plugin_t type, int item);
/*------------------------------------------------------
 * Pre  : Type is valid egg plugin enum 
 *        Item is either LIST command or item id
 * Post : Sets current item in internal iterator
 *        Returns PLUGIN_ERROR on error
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetVertex3f(float *xyz);
/*------------------------------------------------------
 * Pre  : Current vertex exists
 * Post : Sets passed float array to vertex pos
 *        Returns internal id value
 *        Returns PLUGIN_ERROR on error
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetPolygon1u(egg_plugin_t type, int item, 
							 unsigned int *value);
/*------------------------------------------------------
 * Pre  : Type is either vertex or texel
 *        Item is index into polygon's type list 
 *        Value is valid pointer
 *
 * Post : Sets Value to the internal id value used by item
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetPolygon3f(egg_plugin_t type, int item, float *value);
/*------------------------------------------------------
 * Pre  : Type is either vertex or texel
 *        Item is index into polygon's type list 
 *        Value is valid pointer
 *
 * Post : Sets Value to the value used by item:
 *
 *           Texel  - float st[2]
 *           Vertex - float xyz[3]
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggCriticalSection(egg_lock_t request);
/*------------------------------------------------------
 * Pre  : Request is valid
 * Post : For later use - will provide CS protection later
 *        Returns 0 on sucess
 *        Returns 1 on fail
 *
 *        May also block - who knows - it's not
 *        implmented or considered much until threading
 *        gets added to the Egg backend 
 *
 *-- History ------------------------------------------
 *
 * 2001.11.03: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetTagMesh1u(unsigned int item, unsigned int *value);
/*------------------------------------------------------
 * Pre  : Tag selected
 *        Value set to Mesh id of Tag's mesh_list[item]
 * Post : Returns PLUGIN_ERROR if Tag doesn't exist
 *        or if item is invalid
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetCurrent(egg_plugin_t type);
/*------------------------------------------------------
 * Pre  : Complex type passed
 * Post : Returns id of current internal complex
 *        type ( last generated )
 *        Returns PLUGIN_ERROR on error
 *
 *-- History ------------------------------------------
 *
 * 2001.11.21: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetTagMeshNum();
/*------------------------------------------------------
 * Pre  : Tag selected
 * Post : Returns number of Meshes in this Tag
 *        Returns PLUGIN_ERROR if Tag doesn't exist
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

unsigned int eggGetTagRotate3f(float *x, float *y, float *z);
/*------------------------------------------------------
 * Pre  : Tag selected
 * Post : Gets current tag's rotation in eular angles
 *        Returns PLUGIN_ERROR if Tag doesn't exist
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/


class EggPlugin
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	EggPlugin(Egg *egg, char *plugin_dir);
	/*------------------------------------------------------
	 * Pre  : egg and plugin_dir are valid
	 * Post : Constructs an object of EggPlugin
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~EggPlugin();
	/*------------------------------------------------------
	 * Pre  : EggPlugin object is allocated
	 * Post : Deconstructs an object of EggPlugin
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int eggGetNum(egg_plugin_t type);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum
	 * Post : Returns total number of objects of type in model
	 *        Returns PLUGIN_ERROR on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggIterator(egg_plugin_t type, int item);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum 
	 *        Item is either LIST command or item id
	 * Post : Sets current item in internal iterator
	 *        Returns PLUGIN_ERROR on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetVertex(float *xyz);
	/*------------------------------------------------------
	 * Pre  : Current vertex exists
	 * Post : Sets passed float array to vertex pos
	 *        Returns internal id value
	 *        Returns PLUGIN_ERROR on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetPolygon(egg_plugin_t type, int item, 
							   unsigned int *value);
	/*------------------------------------------------------
	 * Pre  : Type is either vertex or texel
	 *        Item is index into polygon's type list 
	 *        Value is valid pointer
	 *
	 * Post : Sets Value to the internal id value used by item
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetPolygon(egg_plugin_t type, int item, float *value);
	/*------------------------------------------------------
	 * Pre  : Type is either vertex or texel
	 *        Item is index into polygon's type list 
	 *        Value is valid pointer
	 *
	 * Post : Sets Value to the value used by item:
	 *
	 *           Texel  - float st[2]
	 *           Vertex - float xyz[3]
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetTagMesh(unsigned int item, unsigned int *value);
	/*------------------------------------------------------
	 * Pre  : Tag selected
	 *        Value set to Mesh id of Tag's mesh_list[item]
	 * Post : Returns PLUGIN_ERROR if Tag doesn't exist
	 *        or if item is invalid
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetCurrent(egg_plugin_t type);
	/*------------------------------------------------------
	 * Pre  : Complex type passed
	 * Post : Returns id of current internal complex
	 *        type ( last generated )
	 *        Returns PLUGIN_ERROR on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetTagMeshNum();
	/*------------------------------------------------------
	 * Pre  : Tag selected
	 * Post : Returns number of Meshes in this Tag
	 *        Returns PLUGIN_ERROR if Tag doesn't exist
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGetTagRotate(float *x, float *y, float *z);
	/*------------------------------------------------------
	 * Pre  : Tag selected
	 * Post : Gets current tag's rotation in eular angles
	 *        Returns PLUGIN_ERROR if Tag doesn't exist
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	int importModel(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports model and returns 0, or
	 *        fails to import returns error -N
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int exportModel(char *filename, char *type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Exports model of type and returns 0, or
	 *        fails to export returns error -N
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggBegin(egg_plugin_t type);
	/*------------------------------------------------------
	 * Pre  : Type is valid egg plugin enum
	 * Post : A new data object had begun construction
	 *        Returns an object id
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : A data object has finished construction
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int eggGetTextureFilename(unsigned int index, char **filename);
	/*------------------------------------------------------
	 * Pre  : Don't alter filename once you get pointer to it
	 * Post : Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTextureStoreFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid
	 *
	 * Post : Stores texture filename to be loaded by modeler
	 *        and used to generate textures
	 *
	 *        Returns either texture id or PLUGIN_ERROR
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int eggTextureStore(EggTextureData *textureData);
	/*------------------------------------------------------
	 * Pre  : Texture data is valid
	 *
	 * Post : Stores texture data to be called by modeler
	 *        and used to generate textures
	 *
	 *        Returns either texture id or -1 for error
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTexelStore2f(float s, float t);
	/*------------------------------------------------------
	 * Pre  : s, t are 0.0 to 1.0 texels
	 * Post : A new texel is created in the model
	 *        Returns the native index of that texel
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggVertexStore3f(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggVertexWeightStore(unsigned int index, 
							  vec_t weight, unsigned int bone);
	/*------------------------------------------------------
	 * Pre  : <weight> of influence of <bone> on vertex v
	 *
	 * Post : Vertex <index> in the model gets weight added
	 *        to influence list, if their is a weight for the
	 *        corresponding bone it is replaced
	 *
	 *        <weight> <= 0.0 removes weight
	 *
	 *        All weights for the vertex combined must be 1.0
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggVertexUVStore2f(unsigned int vIndex, float u, float v);
	/*------------------------------------------------------
	 * Pre  : (u,v) is texel for vertex v
	 *
	 * Post : Vertex 'v' in the model gets normal set
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggVertexNormalStore3f(unsigned int vIndex, float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : x,y,z are the normal vector for vertex v
	 *
	 * Post : Vertex 'v' in the model gets normal set
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *eggGetVertex(unsigned int index);

	void eggVertex1i(unsigned int egg_id);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggTexel1i(unsigned int egg_id);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a texel to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggTexture1i(int id);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_POLYGON);
	 *        Egg_id is the native index
	 * Post : Adds a texture to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int eggGetTextureImage(unsigned int index, 
						   unsigned int *w, unsigned int *h, 
						   unsigned int *depth,  unsigned int *type,
						   unsigned char **image);
	/*------------------------------------------------------
	 * Pre  : Textures were loaded in texture storage
	 * Post : Passes texture data by using index
	 *        Returns 0 on sucess, else error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.09:
	 * Mongoose - Changes to support more buffer types
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eggFlags(unsigned int set);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggGroupCenter(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_GROUP);
	 * Post : Current group's center is set
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTagName(char *name);

	unsigned int eggTagPos(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Current tag's origin is set
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTagFlags(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Current tag's mesh/tag tree flag is set
	 *
	 *        0x00 - push
	 *        0x01 - pop
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTagAddMesh(unsigned int mesh);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's mesh list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTagAddSlave(unsigned int tag);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's slave tag list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggMeshTreeAddFrame(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM);
	 * Post : Adds meshtree frame to meshtree
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTagRotate(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Sets tag rotation with eular angles
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggMeshTreeTag(unsigned int tag);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
	 * Post : Appends tag to mesh tree frame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int eggTextureStoreBuffer(unsigned char *image, unsigned int bpp,
									   unsigned int width, unsigned int height,
									   egg_colormode_t type);
	/*------------------------------------------------------
	 * Pre  : <image> is a valid image buffer 
	 *        <width> <height> <bpp> are correct for image
	 *        <type> for image matches pixel format
	 *
	 * Post : Loads texture into manager
	 *        Returns 0 if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.09:
	 * Mongoose - Created, to replace current texture handler
	 ------------------------------------------------------*/

	void addModule(char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds lookup for libfreyja_loader-NAME.so in
	 *        your module/plugin path
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	static EggPlugin *mEggPlugin;       /* Singleton and public use */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Egg *mEgg;                          /* Pointer to the modeler backend  */

	Vector<char *> mModules;            /* Names of plugin modules managed by 
										 * EggPlugin class */

	Vector<unsigned int> mVertexList;   /* Current polygon's vertex list */

	Vector<unsigned int> mTexelList;    /* Current polygon's texel list */

	Vector<EggTextureData *> mTextures; /* Texture data list */

	Vector<char *> mTextureFiles;       /* Texture filename list */

	Stack<egg_plugin_t> mStack;         /* Object stack to keep up with 
										 * accumulation modes and etc */

	egg_tag_t *mTag;                    /* Current tag/bolt-on */

	egg_mesh_t *mMesh;                  /* Current mesh */

	egg_group_t *mGroup;                /* Current vertex grouping */

	egg_boneframe_t *mBoneFrame;        /* MeshTree animation frame */

	egg_animation_t *mAnimation;        /* MeshTree animation group */

	unsigned int mFlags;                /* Plugins' option flags */

	unsigned int mTextureId;            /* Texture id for current polygon */

	char *mPluginDir;                   /* Plugin directory */
};

#endif


