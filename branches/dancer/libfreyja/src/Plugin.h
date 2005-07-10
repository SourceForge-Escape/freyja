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
#include <string.h>
#include <mstl/Stack.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "Scene.h"
#include "Printer.h"
#include "PluginABI.h"

#ifdef MACOSX
#   ifndef strnlen
#      include <mstl/String.h>
#      define strnlen String::strnlen
#   endif
#endif


class FreyjaPluginDescEvent
{
public:

	char name[64];

	int eventId;

	void *callback();
};


class FreyjaPluginDesc
{
public:
	
	FreyjaPluginDesc()
	{
		mImportFlags = FREYJA_PLUGIN_NONE;
		mExportFlags = FREYJA_PLUGIN_NONE;
		mFilename = 0x0;
		mId = mNextId;
		++mNextId;
	}

	~FreyjaPluginDesc()
	{
		if (mFilename) delete [] mFilename;
		
	}

	void setFilename(const char *filename)
	{
		if (filename && filename[0])
		{
			if (mFilename) delete [] mFilename;
			mFilename = strdup(filename);
		}
	}

	void setDescription(const char *s)
	{
		strncpy(mDescription, s, 64);
		mDescription[63] = 0;
	}

	void setExtention(const char *s)
	{
		strncpy(mExtention, s, 64);
		mExtention[63] = 0;
	}

	void setId(long id)
	{
		mId = id;
	}

	long getId()
	{
		return mId;
	}

	void print()
	{
		freyjaPrintMessage("%s", mDescription);
		freyjaPrintMessage("\tImport: %s%s%s",
						   (mImportFlags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
						   (mImportFlags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
						   (mImportFlags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
		freyjaPrintMessage("\tExport: %s%s%s",
						   (mExportFlags & FREYJA_PLUGIN_MESH) ? "(mesh) " : "", 
						   (mExportFlags & FREYJA_PLUGIN_SKELETON) ? "(skeleton) " : "", 
						   (mExportFlags & FREYJA_PLUGIN_VERTEX_MORPHING) ? "(vertex morph aniamtion) " : "");
	}


	long getIntArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return -1; // oh well

		l = strnlen(name, 64);

		for (i = mIntArgsDesc.begin(); i < (long)mIntArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mIntArgsDesc[i], l))
				return mIntArgs[i];
		}

		return -1;
	}


	float getFloatArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return -1; // oh well

		l = strnlen(name, 64);

		for (i = mFloatArgsDesc.begin(); i < (long)mFloatArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mFloatArgsDesc[i], l))
				return mFloatArgs[i];
		}

		return -1;
	}


	char *getStringArg(const char *name)
	{
		long i, l;

		if (!name || !name[0])
			return 0x0; // oh well

		l = strnlen(name, 64);

		for (i = mStringArgsDesc.begin(); i < (long)mStringArgsDesc.end(); ++i)
		{
			if (!strncmp(name, mStringArgsDesc[i], l))
				return mStringArgs[i];
		}

		return 0x0;
	}

	void addIntArg(const char *name, long data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mIntArgs.pushBack(data);
		mIntArgsDesc.pushBack(s);
	}


	void addFloatArg(const char *name, float data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mFloatArgs.pushBack(data);
		mFloatArgsDesc.pushBack(s);
	}


	void addStringArg(const char *name, const char *data)
	{
		char *s;
		long l = 0;

		if (!name || !name[0])
			return;

		l = strnlen(name, 64);
		s = new char[64];
		strncpy(s, name, 64);
		s[63] = 0;

		mStringArgsDesc.pushBack(s);

		if (data && data[0])
		{
			l = strnlen(name, 64);
			s = new char[l+1];
			strncpy(s, data, l);
			s[l-1] = 0;
		}
		else
		{
			s = new char[64];
			s[0] = 0;
		}

		mStringArgs.pushBack(s);
	}


	char *mFilename;

	char mDescription[64];

	char mExtention[64];

	long mImportFlags;

	long mExportFlags;

	Vector<char *> mFloatArgsDesc;
	Vector<float> mFloatArgs;

	Vector<char *> mIntArgsDesc;
	Vector<long> mIntArgs;

	Vector<char *> mStringArgsDesc;
	Vector<char *> mStringArgs;

private:

	static long mNextId;

	long mId;
};


// ABI functions
FreyjaPluginDesc *freyjaGetPluginClassByName(const char *name);
FreyjaPluginDesc *freyjaGetPluginClassByIndex(long pluginIndex);



class FreyjaPlugin
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaPlugin(Scene *scene, char *plugin_dir);
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

	Bone *getBone(index_t index);
	void getVertex(index_t index, vec3_t xyz);
	void getNormal(index_t index, vec3_t xyz);
	void getTexCoord(index_t index, vec3_t uv);
	void getVertexWeights(Vector<index_t> &bones,
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

	int getPolygon(freyja_object_t obj, int item, index_t *value);
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

	index_t getCurrentBone() { return mIndexBone; }
	index_t getCurrentSkeleton() { return mIndexSkeleton; }
	index_t getCurrentModel() { return mIndexModel; }
	index_t getCurrentMesh() { return mIndexMesh; }


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

	void setMeshPolygonsMaterial(index_t index, int material);
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

	int getTextureFilename(index_t index, char **filename);
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

	index_t freyjaNormal3fv(vec3_t xyz);
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

	index_t freyjaTexCoord2fv(vec2_t uv);
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

	index_t freyjaVertex3fv(vec3_t xyz);
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

	void freyjaVertexWeightStore(index_t index, 
							  vec_t weight, index_t bone);
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

	void freyjaSetNormal3fv(index_t index, vec3_t xyz);
	void freyjaSetTexCoord2fv(index_t index, vec2_t uv);
	void freyjaSetVertex3fv(index_t index, vec3_t xyz);

	void freyjaVertex1i(index_t freyja_id);
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

	void freyjaTexCoord1i(index_t freyja_id);
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

	int freyjaGetTextureImage(index_t index, 
						   index_t *w, index_t *h, 
						   index_t *depth,  index_t *type,
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

	index_t freyjaFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaFlags(index_t set);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets plugin flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	index_t freyjaGroupCenter(float x, float y, float z);
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

	void freyjaBoneFlags(index_t flags);
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

	//void getMeshFlags1u(index_t flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddMesh(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's mesh list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void freyjaBoneAddChild(index_t tag);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_TAG);
	 * Post : mesh added to current tag's slave tag list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.10.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	index_t freyjaMeshTreeAddFrame(float x, float y, float z);
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

	//index_t freyjaMeshTreeBone(index_t tag);
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

	int getTexture(index_t index, index_t *w, index_t *h, 
				   index_t *numBytes,  unsigned char **image);

	int setTexture(unsigned char *image, index_t numBytes,
				   index_t width, index_t height);
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

	//Vector<index_t> mVertexList;   /* Current polygon's vertex list */

	//Vector<index_t> mTexCoordList; /* Current polygon's texel list */

	Stack<freyja_object_t> mStack;      /* Object stack to keep up with 
										 * accumulation modes and etc */

	FreyjaPrinter *mPrinter;            /* Printer handles logging and
										 * message/error output */


	vec3_t mPoint;                      /* Used to mutate transform origin */


	////////////////////////////////////////////////////////////////////////

	Scene *mScene;                /* Pointer to the modeler backend  */

	Model *mModel;              /* Current Model of Scene */

	Material *mMaterial;          /* Current Material of Scene */

	Texture *mTexture;            /* Current Texture of Scene */

	index_t mIndexModel;           /* Current sub-Model references */
	index_t mIndexMesh;
	index_t mIndexAnimation;
	index_t mIndexSkeleton;
	index_t mIndexMaterial;
	index_t mIndexTexture;
	index_t mIndexVertexGroup;

	VertexGroup *mVertexGroup;    /* Vertex list for various 
										 * content creation uses */

	Mesh *mMesh;                  /* Current Mesh of Scene */

	VertexFrame *mVertexFrame;    /* Current VertexFrame of Mesh */

	UVMap *mUVMap;                /* Current TexCoord skin of Mesh */

	Polygon *mPolygon;            /* Current Polygon of Mesh */

	index_t mIndexVertex;          /* Current sub-Mesh references */
	index_t mIndexTexCoord;
	index_t mIndexVertexFrame;
	index_t mIndexPolygon;

	Skeleton *mSkeleton;          /* Current Skeleton of Scene */

	Bone *mBone;                  /* Current Bone of Skeleton */

	index_t mIndexBone;            /* Current sub-Skeleton references */

	Animation *mAnimation;        /* Current Animation of Scene */

	////////////////////////////////////////////////////////////////////////

	index_t mOptions;              /* Option flags */

	Vector<char *> mPluginDirectories;

	char *mPluginDir;                   /* Plugin directory */
};

#endif


