/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*============================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaModel
 * License : GPL
 * Comments: This is the backend of the modeler
 *
 *           This is based on my Model class in GooseEgg
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.04.12:
 * Mongoose - Mostly new API, refactoring, and new dependences
 *
 *            Note most of the documentation doesn't mention method
 *            name changes due to new API.  For most API changes
 *            the old documentation was merely removed / replaced.
 *
 * 2000.09.09:
 * Mongoose - Created
 ============================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMODEL_H
#define GUARD__FREYJA_MONGOOSE_FREYJAMODEL_H

#include <freyja8/EggPlugin.h>
#include <freyja8/Egg.h>
#include <mstl/Vector.h>
#include "Light.h"
#include "freyja_events.h"


class CopyGroup
{
public:
	Vector <unsigned int> vertices;     /* Vertices composing group */	
	vec3_t center;
};


// FIXME: dumps vertex weights and for now
// Polygon, etc references are never copied -- since objects will be duped
class CopyVertex
{
public:
	vec3_t pos;
	vec3_t uv;       /* Remember only uv is used in this build */
	vec3_t norm;
	// vec4_t color;  // In this build color is func of polymapped face
};


class CopyTexCoord
{
public:
	vec2_t uv;
};


class CopyPolygon
{
public:
	Vector <unsigned int> vertices;
	Vector <unsigned int> texcoords; // this build also uses this for color
	int material;
};


// FIXME: Dumps animation frames
class CopyMesh
{
public:
	CopyMesh()
	{
		flags = 0;
	}

	~CopyMesh()
	{
		erase();
	}

	void erase()
	{
		flags = 0;
		vertices.erase();
		texcoords.erase();
		polygons.erase();
		groups.erase();
	}

	Vector <CopyVertex *> vertices;
	Vector <CopyTexCoord *> texcoords;
	Vector <CopyPolygon *> polygons;
	Vector <CopyGroup *> groups;
	vec3_t center;
	unsigned int flags;
};


class FreyjaEgg : public Egg
{
public:
	FreyjaEgg::FreyjaEgg() : Egg()
	{
	}


	virtual void print(char *format, ...)
	{	
		va_list args;

		va_start(args, format);
		freyja_print_args(format, &args);
		va_end(args);
	}
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

	virtual void printError(char *format, ...)
	{	
		va_list args;

		va_start(args, format);
		freyja_print_args(format, &args);
		va_end(args);
	}
	/*------------------------------------------------------
	 * Pre  : String and args are valid
	 * Post : Report an error to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/
};


class FreyjaEggPlugin : public EggPlugin
{
public:
	FreyjaEggPlugin(FreyjaEgg *egg, char *plugindir) : 
		EggPlugin(egg, plugindir)
	{
	}

	
	virtual void eggPrintError(char *format, va_list *args)
	{
		freyja_print_args(format, args);
	}

	
	virtual void eggPrintMessage(char *format, va_list *args)
	{
		freyja_print_args(format, args);
	}
};


class FreyjaModel
{
public:

	typedef enum {
		TransformMesh        = 0,
		TransformVertexFrame = 1,
		TransformScene       = 2,
		TransformBone        = 3,
		TransformPoint
	} transform_t;

	typedef enum {
		FL_DUMP_TEXTURE  = 1,    /* Toggle image file dumps of TR textures */
		FL_LOAD_MAP      = 2,    /* Toggle map loading in TR paks */
		FL_QUAKE_PAL     = 4,    /* Toggle quake/hexen2 palette in mdl loads */
		FL_VERTEX_UV     = 8     /* Toggle polymapping of texcoords */
	} option_flag_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaModel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaModel object is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaModel();
	/*------------------------------------------------------
	 * Pre  : This object exists
	 * Post : FreyjaModel object is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void getBoneRotation(vec_t *pitch, vec_t *yaw, vec_t *roll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get pointers to the current bone's 
	 *        orientation in 3 space ( Euler angles )
	 *
	 *-- History ------------------------------------------
	 *
	 * 200?.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getBoneTranslation(vec_t *x, vec_t *y, vec_t *z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get pointers to the current bone's X Y Z offset
	 *        in 3 space
	 *
	 *-- History ------------------------------------------
	 *
	 * 200?.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentAnimation();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current animation index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentAnimationFrame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current animation frame index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentBone();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current bone index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getFlags();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentGroup();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current texcoord index for
	 *        polymapped texcoords
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getCurrentMeshCenter(vec3_t center);
	unsigned int getCurrentMesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current mesh index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentPolygon();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current texcoord index for
	 *        polymapped texcoords
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentPolygonEdgeCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current edge count for polygon
	 *        creation interally in FreyjaModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentSkeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current skeleton index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentTexCoord();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current texcoord index for
	 *        polymapped texcoords
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentTextureIndex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current texture index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentVertex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current vertex index for editing
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getCurrentVertexFrame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns current vertex animation frame index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getSceneTranslation(vec3_t offset);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the offset of the scene in X Y Z in 3 space
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.01:
	 * Mongoose - Created, replaces old API 'Scroll' methods
	 ------------------------------------------------------*/

	int saveAnimation(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid
	 * Post : Animation is saved as SKEL format
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int saveModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid
	 * Post : Model is saved as EGG format
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void adjustSceneTranslation(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the scene's position in 3 space
	 *        by translating X Y Z from it's local origin
	 *
	 *-- History ------------------------------------------
	 *
	 * 200?.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void generateUVMap();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This currently doesn't actually use a true
	 *        UV algorithm, when it does this will allow
	 *        toggling the algorithm:
	 *           cylindrical / plane / etc
	 *
	 *-- History ------------------------------------------
	 *
	 * 2???.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid, model format supported
	 * Post : Model is translated to Egg format and loaded
	 *        into data structures
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setBoneRotation(float pitch, float yaw, float roll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the current bone's X Y Z orientation in
	 *        3 space by Euler angles
	 *
	 *-- History ------------------------------------------
	 *
	 * 200?.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setBoneTranslation(float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the current bone's X Y Z offset in
	 *        3 space
	 *
	 *-- History ------------------------------------------
	 *
	 * 200?.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentAnimation(unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current animation index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentAnimationFrame(unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current animation frame index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentBone(unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current bone index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentGroup(unsigned int index);

	void setCurrentMesh(unsigned int index);

	void setCurrentPolygon(unsigned int index);

	void setCurrentPolygonEdgeCount(unsigned int count);
	/*------------------------------------------------------
	 * Pre  : COUNT must be between 3 and 7
	 * Post : Sets the number of polygon vertices ( sides )
	 *
	 * Notes: There is no true hard limit in the polygon
	 *        implementation, however this enforces a 
	 *        reasonable edge count for testing complex
	 *        polygon rendering if you so wish.
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentSkeleton(unsigned int index);

	void setCurrentTexCoord(unsigned int index);

	void setCurrentTextureIndex(unsigned int index);

	void setCurrentVertex(unsigned int index);

	void setCurrentVertexFrame(unsigned int index);

	void setFlags(option_flag_t flag, int op);
	/*------------------------------------------------------
	 * Pre  : The flag and operator are valid
	 * Post : Sets control flags for model
	 * Notes: op { 0 = clear, 1 = set }
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setSceneTranslation(vec_t x, vec_t y, vec_t z);


	////////////////////////////////////////////////////////////////////////

	void Clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resets|Init the data controled by data model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void CurrentPlane(Egg::egg_plane p);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current edit plane
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	enum Egg::egg_plane CurrentPlane();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current edit plane
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void transform(int mode, Egg::egg_transform type, 
				   float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : mode is {FRAME, MESH, SCENE, BONE, etc}
	 *        type is {SCALE, ROTATE, TRANSLATE}
	 *        x, y, z are in degrees or units
	 *
	 * Post : Transform is performed
	 *
	 * Notes: FRAME  : Transform current frame of current mesh
	 *        MESH   : Transform current mesh
	 *        SCENE  : Transform entire scene
	 *        BONETAG: Transform current bone tag
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool copySelectedMesh();
	bool pasteSelectedMesh();


	///////////////////////////////////////////////////////
	/// FIXME: decide where to put these, then doc ////////
	///////////////////////////////////////////////////////

	void printInfo();

	void VertexNew(float xx, float yy);
	void VertexMove(float xx, float yy);
	void VertexSelect(float xx, float yy);
	void VertexBuffer(float xx, float yy);
	void VertexCombine(float xx, float yy);
	void VertexDelete();

	void TexelCombine(float s, float t);
	void TexelSelect(float s, float t);
	void TexelMove(float s, float t);

	void PolygonAddVertex(float xx, float yy);
	void PolygonDelVertex(float xx, float yy);
	void PolygonSelectVertex(float xx, float yy);

	void BBoxSelect(float xx, float yy);
	void BBoxMove(float xx, float yy);
	void Bbox(vec3_t min, vec3_t max, Vector<unsigned int> **list);
	void BBoxListBuild();

	void MeshSelect(float xx, float yy);
	void MeshMove(float xx, float yy);
	void MeshMoveCenter(float xx, float yy);
	void MeshNew();
	void MeshDel();
	void MeshCopy();

	/// BONES /////
	unsigned int newBone(vec_t x, vec_t y, vec_t z, unsigned char flag);
	void moveBoneCenter(float xx, float yy);
	void moveBone(float xx, float yy);
	void selectBone(float xx, float yy);
	void nameBone(unsigned int bone, const char *name);
	void addVertexToBone(unsigned int bone, unsigned int vertex);
	void removeVertexFromBone(unsigned int bone, unsigned int vertex);
	void addMeshToBone(unsigned int bone, unsigned int mesh);
	void removeMeshFromBone(unsigned int bone, unsigned int mesh);
	void connectBone(unsigned int master, unsigned int slave);
	void disconnectBone(unsigned int master, unsigned int slave);



	void TextureShift();
	///////////////////////////////////////////////////////


#ifdef FIXME
#   error "FreyjaModel.h, FIXME: Hacky stuff that's mainly temp"
#else
	Egg *CurrentEgg() 
	{
		return _egg;
	}

	void Debug(unsigned int n)
	{
		_egg_debug = n;
		_egg->setDebugLevel(n);
	}

	bool Debug()
	{
		return _egg_debug;
	}

	float *GetLight0Pos()
	{
		return _light0_pos;
	}
#endif

	bool appendMode;


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	egg_group_t *CachedGroup();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current vertex grouping
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_tag_t *CachedTag();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current bone tag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	egg_mesh_t *CachedMesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	
	FreyjaEgg *_egg;                    /* The 3d model */
	
	FreyjaEggPlugin *_plugin;           /* Model plugin system */

	CopyMesh mCopyMesh;             /* This is the fucking buffer for meshes
									 * it handles things the backend should
									 * but the backend can't in this build for
									 * copy/cut/paste */

	char *_palette_filename;            /* Current palette */

	egg_vertex_t *_cached_vertex;       /* Current vertex ( cached ) */

	Vector<unsigned int> mList;         /* Temp generic vertex list buffer */

	bbox_t mSelectBBox;                 /* 3d selection box using 2 vertices */

	unsigned int _bbox;                 /* Used to determine bbox min/max 
										   points */
  
	Vector<unsigned int> _selection_list; /* Temp generic vertex list buffer */

	Egg::egg_plane _current_plane;           /* Which plane view is this? */

	unsigned int _defaults;             /* Stores Control flags */

	unsigned int _poly_sz;              /* Number of edges for a new polygon */
	
	unsigned int _texture_num;          /* Index to texture array */
	
	unsigned int _current_texel;        /* Currently selected texel id */
	
	unsigned int _current_vertex;       /* Currently selected vertex id */
	
	unsigned int _current_frame;        /* Current vertex frame */
	
	unsigned int _current_animation_frame;  /* Current animation frame */

	unsigned int _current_polygon;      /* Current polygon */
	
	unsigned int _current_group;        /* Current vertex grouping id */
	
	unsigned int _current_mesh;         /* Currently selected mesh id */
	
	unsigned int _current_tag;          /* Currently selected bone tag id */
	
	unsigned int _current_bone_frame;   /* Currently selected skeletal frame */
	
	bool _egg_debug;                    /* Debugging egg? cached query */
	
	float _zoom;                        /* Scaling of scene */
	
	float _scroll[3];                   /* Scrolling in edit planes */

	float _light0_pos[4];               /* Testing for light system */
};

//	unsigned int _current_morph_frame;  /* Current vertexmorph frame */

#endif
