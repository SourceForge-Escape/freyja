/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : EggPlugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Egg plugin handler class.
 *
 *           EggPlugin alters Egg's states.
 *
 *           Always assume Egg to have different iterator
 *           and data states once EggPlugin is called on it.
 * 
 *-- Test Defines -------------------------------------------
 *
 * UNIT_TEST_EGGPLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  FreyjaPlugin ABI compatible EggPlugin
 *
 * 2004.03.23: (v0.0.3+)
 * Mongoose - New singleton implementation
 *            New plugin features for texture loading
 *            Removed last of old List STL use
 *
 *            New style, which affects compiler portability
 *            Mostly removing leading _underscores
 *
 * 2001.11.02: (v0.0.2)
 * Mongoose - Experimental export API added 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_EGGPLUGIN_H
#define GUARD__MONGOOSE_FREYJA_EGGPLUGIN_H

#include <stdarg.h>
#include <mstl/Stack.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "FreyjaPluginABI.h"
#include "FreyjaPrinter.h"
#include "FreyjaFileReader.h"
#include "FreyjaPlugin.h"
#include "Egg.h"


class EggTextureData
{
public:
	EggTextureData()
	{
		mType = RGBA_32;
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

	freyja_colormode_t mType;
	unsigned char *mImage;
	unsigned char *mPalette;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mBitDepth;
};


class EggPlugin
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	EggPlugin(Egg *egg);
	/*------------------------------------------------------
	 * Pre  : egg and plugin_dir are valid
	 * Post : Constructs an object of EggPlugin
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~EggPlugin();
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

	egg_polygon_t *getPolygon(long index);  // this isn't freyja exportable

	egg_vertex_t *getVertex(long index);  // this isn't freyja exportable

	egg_tag_t *getBone(long index);  // this isn't freyja exportable

	egg_mesh_t *getMesh(long index);  // this isn't freyja exportable
	
	long getPolygonVertexIndex(long polygonIndex, long element);

	long getPolygonTexCoordIndex(long polygonIndex, long element);

	long getPolygonMaterial(long polygonIndex);

	long getPolygonEdgeCount(long polygonIndex);

	long getPolygonFlags(long polygonIndex);

	long freyjaGetCount(freyja_object_t type);
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

	long freyjaIterator(freyja_object_t type, int item);
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

	void freyjaGetVertexByIndex(long index, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Doesn't affect internal iterator state
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.19:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaGetVertex(vec3_t xyz);
	void freyjaGetVertexNormal(vec3_t nxyz);
	void freyjaGetVertexTexCoord(vec2_t uv);
	void freyjaGetVertexWeights(Vector<unsigned int> &bones,
								Vector<float> &weights);
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

	void freyjaGetTexCoord(long index, vec2_t uv);

	long freyjaGetPolygon(freyja_object_t type, long item, long *value);
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

	long freyjaGetPolygon(freyja_object_t type, long item, float *value);
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

	long freyjaGetBoneMeshIndex(long element);
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

	long freyjaGetCurrent(freyja_object_t type);
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

	long freyjaGetBoneMeshCount();
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

	long freyjaGetBoneRotate(vec_t *x, vec_t *y, vec_t *z);
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

	void freyjaPrintError(const char *format, ...);
	virtual void freyjaPrintError(const char *format, va_list *args);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages
	 *
	 *        First methot makes va_list and passes it
	 *        to (format, arg) method to print to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.18:
	 * Mongoose - Created, split from Egg9 experimental 
	 ------------------------------------------------------*/

	void freyjaPrintMessage(const char *format, ...);
	virtual void freyjaPrintMessage(const char *format, va_list *args);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report error messages
	 *
	 *        First methot makes va_list and passes it
	 *        to (format, arg) method to print to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.18:
	 * Mongoose - Created, split from Egg9 experimental 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void addPluginDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long importModel(const char *filename);
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

	long exportModel(const char *filename, const char *type);
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

	void freyjaBegin(freyja_object_t type);
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

	void freyjaEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : A data object has finished construction
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long freyjaGetTextureFilename(long index, char **filename);
	/*------------------------------------------------------
	 * Pre  : Don't alter filename once you get pointer to it
	 * Post : Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long freyjaTextureStoreFilename(const char *filename);
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

	long freyjaTextureStore(EggTextureData *textureData);
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

	long freyjaTexCoord2f(float s, float t);
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

	long freyjaVertex3f(float x, float y, float z);
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

	void freyjaVertexWeight(long index, vec_t weight, long bone);
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

	void freyjaVertexTexCoord2f(long index, vec_t u, vec_t v);
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

	void freyjaVertexNormal3f(long index, vec_t x, vec_t y, vec_t z);
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

	void freyjaPolygonVertex1i(long index);
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

	void freyjaPolygonTexCoord1i(long index);
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

	void freyjaPolygonMaterial1i(long index);
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

	long freyjaGetTextureImage(long index, 
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

	unsigned int freyjaFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaFlags(unsigned int set);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaGroupCenter(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_GROUP);
	 * Post : Current group's center is set
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneName(char *name);

	void freyjaBonePos(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Current tag's origin is set
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneFlags(unsigned int flags);
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

	void freyjaMeshFlags1u(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid egg_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddMesh(long mesh);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's mesh list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddChild(long tag);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's slave tag list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaMeshTreeAddFrame(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM);
	 * Post : Adds meshtree frame to meshtree
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneRotate(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_TAG);
	 * Post : Sets tag rotation with eular angles
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaMeshTreeFrameAddBone(long tag);
	/*------------------------------------------------------
	 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
	 * Post : Appends tag to mesh tree frame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long freyjaTextureStoreBuffer(unsigned char *image, unsigned int bpp,
								  unsigned int width, unsigned int height,
								  freyja_colormode_t type);
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

	void setPrinter(FreyjaPrinter *printer);

	void setupPlugins();

	bool checkModel(const char *filename);
	bool saveModel(const char *filename);
	bool loadModel(const char *filename);

	void setPluginDescription(const char *info_line);
	void setPluginImportFlags(long flags);
	void setPluginExportFlags(long flags);
	void setPluginExtention(const char *ext);
	void addPluginArgInt(const char *name, long defaults);
	void addPluginArgFloat(const char *name, float defaults);
	void addPluginArgString(const char *name, const char *defaults);
	long getPluginArgInt(long pluginIndex, const char *name);
	float getPluginArgFloat(long pluginIndex, const char *name);
	char *getPluginArgString(long pluginIndex, const char *name);
	FreyjaPluginDesc *getPluginDesc(long pluginIndex);
	FreyjaPluginDesc *getPluginDesc(const char *name);
	long getPluginDescCount();
	long getPluginId();

	static EggPlugin *mEggPlugin;       /* Singleton and public use */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	FreyjaPluginDesc *mCurrentPlugin;

	Vector<FreyjaPluginDesc *> mPlugins;/* Plugin infos */

	Vector<char *> mPluginDirectories;  /* Search dirs for plugins */

	Vector<EggTextureData *> mTextures; /* Texture data list */

	Vector<char *> mTextureFiles;       /* Texture filename list */

	Vector<unsigned int> mVertexList;   /* Current polygon's vertex list */

	Vector<unsigned int> mTexCoordList; /* Current polygon's texel list */

	Stack<freyja_object_t> mStack;      /* Object stack to keep up with 
										 * accumulation modes and etc */

	Egg *mEgg;                          /* Pointer to the modeler backend  */

	FreyjaPrinter *mPrinter;            /* Logging and output system */

	egg_tag_t *mTag;                    /* Current tag/bolt-on and|or bone */

	egg_mesh_t *mMesh;                  /* Current mesh */

	egg_group_t *mGroup;                /* Current vertex grouping */

	egg_boneframe_t *mBoneFrame;        /* MeshTree animation frame */

	egg_animation_t *mAnimation;        /* MeshTree animation group */

	unsigned int mIndexVertex;
	unsigned int mIndexTexCoord;
	unsigned int mIndexPolygon;
	unsigned int mIndexGroup;
	unsigned int mIndexMesh;
	unsigned int mIndexBone;
	unsigned int mIndexSkeleton;
	unsigned int mIndexSkeletonAnim;

	unsigned int mFlags;                /* Plugins' option flags */

	unsigned int mTextureId;            /* Texture id for current polygon */
};

#endif


