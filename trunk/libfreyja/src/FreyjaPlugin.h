/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
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
 * 2004.10.10: (v0.0.4)
 * Mongoose - New interactive plugins
 *            API changes for new backend ( Egg9 )
 *
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
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_FREYJAPLUGIN_H
#define GUARD__MONGOOSE_FREYJA_FREYJAPLUGIN_H

#include <stdarg.h>
#include <mstl/Stack.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "Freyja.h"
#include "FreyjaPrinter.h"
#include "FreyjaPluginABI.h"


class FreyjaPlugin
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaPlugin(FreyjaScene *scene, char *plugin_dir);
	/*------------------------------------------------------
	 * Pre  : freyja and plugin_dir are valid
	 * Post : Constructs an object of FreyjaPlugin
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~FreyjaPlugin();
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin object is allocated
	 * Post : Deconstructs an object of FreyjaPlugin
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : Type is valid freyja plugin enum
	 *
	 * Post : Returns total number of objects of type in model
	 *
	 *        Sets plugin error on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	FreyjaBone *getBone(unsigned int index);
	void getVertex(unsigned int index, vec3_t xyz);
	void getNormal(unsigned int index, vec3_t xyz);
	void getTexCoord(unsigned int index, vec3_t uv);
	void getVertexWeights(Vector<unsigned int> &bones,
						  Vector<float> &weights);
	/*------------------------------------------------------
	 * Pre  : Current vertex exists
	 * Post : Sets passed float array to vertex pos
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getPolygon(freyja_object_t obj, int item, unsigned int *value);
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

	int getPolygon(freyja_object_t obj, int item, vec_t *value);
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

	long getBoneMeshIndex(long element);
	/*------------------------------------------------------
	 * Pre  : Bone selected
	 *        Value set to Mesh id of Bone's mesh_list[item]
	 * Post : Returns PLUGIN_ERROR if Bone doesn't exist
	 *        or if item is invalid
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long getCurrentIndex(freyja_object_t type);
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

	unsigned int getCurrentBone() { return mIndexBone; }
	unsigned int getCurrentSkeleton() { return mIndexSkeleton; }
	unsigned int getCurrentModel() { return mIndexModel; }
	unsigned int getCurrentMesh() { return mIndexMesh; }


	virtual void printError(char *format, ...);
	virtual void printErrorArgs(char *format, va_list *args);
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
	 * Mongoose - Created, split from Freyja9 experimental 
	 ------------------------------------------------------*/

	virtual void printMessage(char *format, ...);
	virtual void printMessageArgs(char *format, va_list *args);
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
	 * Mongoose - Created, split from Freyja9 experimental 
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

	void setTransformPoint(vec3_t p);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets transform() pivot/origin for '...AboutPoint'
	 *        operations
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setMeshPolygonsMaterial(unsigned int index, int material);
	/*------------------------------------------------------
	 * Pre  : index - index of mesh
	 *        material - material index
	 *
	 * Post : Sets all polygon's materials to material
	 *        Sets mesh's material to material
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setPrinter(FreyjaPrinter *printer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sends text output/errors to printer for logging
	 *        or special output 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void transform(freyja_transform_t object, freyja_transform_action_t action,
				   vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : object is valid
	 *        action is valid for object
	 *        x, y, z are in eular degrees or units
	 *
	 * Post : Transform is performed
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created, from old generic transform system
	 *            in GooseEgg and Freyja -- now in lib
	 ------------------------------------------------------*/

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resets/Frees data controlled by data model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created, from FreyjaModel
	 ------------------------------------------------------*/

	long freyjaIterator(freyja_object_t type, int item);
	/*------------------------------------------------------
	 * Pre  : Type is valid freyja plugin enum 
	 *        Item is either LIST command or item id
	 *
	 * Post : Sets current item in internal iterator
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.03: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void importPluginDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports plugins in dir and returns 0, or
	 *        fails to import returns error -N
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int importModel(const char *filename);
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

	int exportModel(const char *filename, char *type);
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
	 * Pre  : Type is valid freyja plugin enum
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

	int getTextureFilename(unsigned int index, char **filename);
	/*------------------------------------------------------
	 * Pre  : Don't alter filename once you get pointer to it
	 * Post : Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int setTextureFilename(const char *filename);
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

	int freyjaTextureStoreBuffer(unsigned char *image, unsigned char bpp, 
							  unsigned int width, unsigned int height);
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

	unsigned int freyjaNormal3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_MESH);
	 *        <x,y,z> is valid normal vector
	 * Post : A new normal created in the current mesh
	 *        Returns the native index of that normal
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int freyjaTexCoord2fv(vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_MESH);
	 *        u, v are valid: 0.0 to 1.0 range, etc
	 * Post : A new texcoord is created in the current mesh
	 *        Returns the native index of that texcoord
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int freyjaVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_MESH);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the current mesh
	 *        Returns the native index of that vertex
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaVertexWeightStore(unsigned int index, 
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

	void freyjaSetNormal3fv(unsigned int index, vec3_t xyz);
	void freyjaSetTexCoord2fv(unsigned int index, vec2_t uv);
	void freyjaSetVertex3fv(unsigned int index, vec3_t xyz);

	void freyjaVertex1i(unsigned int freyja_id);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_POLYGON);
	 *        Freyja_id is the native index
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaTexCoord1i(unsigned int freyja_id);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_POLYGON);
	 *        Freyja_id is the native index
	 * Post : Adds a texel to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaMaterial1i(int id);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_POLYGON);
	 *        Freyja_id is the native index
	 * Post : Adds a texture to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int freyjaGetTextureImage(unsigned int index, 
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

	unsigned int freyjaGroupCenter(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_GROUP);
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
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : Current tag's origin is set
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneFlags(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
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

	//void getMeshFlags1u(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddMesh(unsigned int mesh);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's mesh list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddChild(unsigned int tag);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's slave tag list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int freyjaMeshTreeAddFrame(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_MESHTREE_ANIM);
	 * Post : Adds meshtree frame to meshtree
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneRotate(Quaternion q);
	void freyjaBoneRotate(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : Sets tag rotation with eular angles
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	//unsigned int freyjaMeshTreeBone(unsigned int tag);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_MESHTREE_ANIM_FRAME);
	 * Post : Appends tag to mesh tree frame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.18: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getTextureFilename(const char *filename);

	int getTexture(unsigned int index, unsigned int *w, unsigned int *h, 
				   unsigned int *numBytes,  unsigned char **image);

	int setTexture(unsigned char *image, unsigned int numBytes,
				   unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : <image> is a valid image buffer 
	 *        <width> <height> <bpp> are correct for image
	 *        <type> for image matches pixel format
	 *
	 *         numBytes = 3 : RGB, 4: RGBA
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

	void importPlugins();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads all plugins in all cached directories
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.xx.xx:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	static FreyjaPlugin *mPlugin;       /* Singleton and public use */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Vector<char *> mModules;            /* Names of plugin modules managed by 
										 * FreyjaPlugin class */

	//Vector<unsigned int> mVertexList;   /* Current polygon's vertex list */

	//Vector<unsigned int> mTexCoordList; /* Current polygon's texel list */

	Stack<freyja_object_t> mStack;      /* Object stack to keep up with 
										 * accumulation modes and etc */

	FreyjaPrinter *mPrinter;            /* Printer handles logging and
										 * message/error output */


	vec3_t mPoint;                      /* Used to mutate transform origin */


	////////////////////////////////////////////////////////////////////////

	FreyjaScene *mScene;                /* Pointer to the modeler backend  */

	FreyjaModel *mModel;                /* Current Model of Scene */

	FreyjaMaterial *mMaterial;          /* Current Material of Scene */

	FreyjaTexture *mTexture;            /* Current Texture of Scene */

	unsigned int mIndexModel;           /* Current sub-Model references */
	unsigned int mIndexMesh;
	unsigned int mIndexAnimation;
	unsigned int mIndexSkeleton;
	unsigned int mIndexMaterial;
	unsigned int mIndexTexture;
	unsigned int mIndexVertexGroup;

	FreyjaVertexGroup *mVertexGroup;    /* Vertex list for various 
										 * content creation uses */

	FreyjaMesh *mMesh;                  /* Current Mesh of Scene */

	FreyjaVertexFrame *mVertexFrame;    /* Current VertexFrame of Mesh */

	FreyjaUVMap *mUVMap;                /* Current TexCoord skin of Mesh */

	FreyjaPolygon *mPolygon;            /* Current Polygon of Mesh */

	unsigned int mIndexVertex;          /* Current sub-Mesh references */
	unsigned int mIndexTexCoord;
	unsigned int mIndexVertexFrame;
	unsigned int mIndexPolygon;

	FreyjaSkeleton *mSkeleton;          /* Current Skeleton of Scene */

	FreyjaBone *mBone;                  /* Current Bone of Skeleton */

	unsigned int mIndexBone;            /* Current sub-Skeleton references */

	FreyjaAnimation *mAnimation;        /* Current Animation of Scene */

	////////////////////////////////////////////////////////////////////////

	unsigned int mOptions;              /* Option flags */

	Vector<char *> mPluginDirectories;

	char *mPluginDir;                   /* Plugin directory */
};

#endif


