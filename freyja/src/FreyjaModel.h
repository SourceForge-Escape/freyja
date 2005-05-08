/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*============================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : FreyjaModel
 * License : GPL
 * Comments: This is the backend of the modeler
 *
 *           This is based on my Model class in GooseEgg
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17:
 * Mongoose - A little API clean up, due to the fact this is staying
 *            in next release and needs to be maintained. 
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

#include <freyja/EggPlugin.h>
#include <freyja/Egg.h>

#include <hel/Vector3d.h>
#include <mstl/Vector.h>
#include "BezierPatch.h"
#include "Light.h"
#include "freyja_events.h"


typedef enum { 
	
	PLANE_XY = 0, 
	PLANE_ZY = 1, 
	PLANE_XZ = 2
} freyja_plane_t;


/* Oh noes! You know something is up when you see predecs of classes */
class RenderPolygon;
class RenderMesh;
class RenderModel;


class FreyjaModel
{
public:

	typedef enum {
		TransformMesh        = 0,
		TransformVertexFrame = 1,
		TransformScene       = 2,
		TransformBone        = 3,
		TransformPoint,
		TransformSelectedVertices
	} transform_t;

	typedef enum {
		FL_DUMP_TEXTURE  = 1,    /* Toggle image file dumps of TR textures */
		FL_LOAD_MAP      = 2,    /* Toggle map loading in TR paks */
		FL_QUAKE_PAL     = 4,    /* Toggle quake/hexen2 palette in mdl loads */
		FL_VERTEX_UV     = 8,    /* Toggle polymapping of texcoords */
		fDontUpdateBoneName = 16,
		fDeformBoneVertices = 32
	} option_flag_t;


	class FreyjaModelPrinter : public FreyjaPrinter
	{
	public:

		virtual void errorArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}


		virtual void messageArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}
	};



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

	unsigned int getAnimationFramesIn(unsigned int animationIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of frames in animation
	 *        
	 ------------------------------------------------------*/

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

	void getCurrentMeshCenter(vec3_t center);

	freyja_plane_t getCurrentPlane();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the current edit plane
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

	bool getDebug();

	unsigned int getModelCount();

	void getMeshBoundingBox(long index, vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : index is query for 'mesh' by index
	 *
	 * Post : This replaces the old Egg mesh -> group export
	 *        it pulls 'editable' vertices' bounding box.
	 *
	 ------------------------------------------------------*/

	void getMeshVertices(long index, Vector<unsigned int> **list);
	/*------------------------------------------------------
	 * Pre  : index is query for 'mesh' by index
	 *
	 * Post : Returns list of currently 'editable' vertices
	 *        of that mesh OR Null. 
	 *
	 *        This replaces the old Egg mesh -> group export
	 *        it pulls 'editable' vertices out of given mesh.
	 *
	 *        It does this without exposing groups, and makes
	 *        Egg removable again.
	 *
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

	void getVertexSelection(vec3_t min, vec3_t max,
							Vector<unsigned int> **list);

	Vector<unsigned int> &getVertexSelectionList();

	vec3_t *getVertexXYZ(long vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Exports vertex via base libhel math type, or
	 *        returns NULL if invalid vertexIndex
	 *
	 ------------------------------------------------------*/

	bool isCurrentBoneAllocated();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if current bone index has a 
	 *        matching allocated object
	 *
	 ------------------------------------------------------*/

	void printInfo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Dumps some infomation about model element counts
	 *
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
	 * Post : Model is saved as format defined by extention
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

	//void boxSelection(vec3_t min, vec3_t max, Vector<unsigned int> &list);

	void boxSelectionListBuild();

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resets|Init the data controled by data model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void createPolyMappedUVMap(long seedPolygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Uses an expensive algorithm to generate a UVMap
	 *        by using edge neighbors of a seed polygon until
	 *        no more are found
	 *
	 *        Here 'UVMap' is a collection of texture polygons
	 *
	 ------------------------------------------------------*/

	bool copySelectedMesh();

	bool copyVertexBuffer();

	void cullUsingVertexBuffer();

	void deleteAnimationFrame(unsigned int frame);

	void generateUVMap(); // texture projection
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
	 * Post : Model is imported from disk
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void mirrorTexCoord(long texCoordIndex, bool x, bool y);

	void mirrorUsingVertexBuffer(bool x, bool y, bool z);

	void moveObject(transform_t type, Vector3d xyz);

	void movePatchControlPoint(Vector3d xyz);

	bool pasteSelectedPatch();

	bool pasteSelectedMesh();

	bool pasteVertexBuffer();

	void selectObject(transform_t type, Vector3d xyz);

	void selectPatchControlPoint(Vector3d xyz);

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

	void setCurrentPlane(freyja_plane_t p);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current edit plane
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

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

	void setDebug(unsigned int n);

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

	void setMeshMaterial(long meshIndex, long material);

	void setPolygonMaterial(long polygonIndex, long material);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets mesh[<meshIndex>] and all it's polygon's
	 *        material ids to <material>
	 *
	 * Notes: 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.17: 
	 * Mongoose - Created, an improved API for TextureShift()
	 ------------------------------------------------------*/

	void setSceneTranslation(vec_t x, vec_t y, vec_t z);

	void transform(int mode, freyja_transform_action_t action, 
				   float x, float y, float z);
	/*------------------------------------------------------
	 * Pre  : mode is {FRAME, MESH, SCENE, BONE, etc}
	 *        action is { fTranslate, fRotate, fScale, etc }
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

	void transformTexCoord(long texCoordIndex,
						   freyja_transform_action_t action,
						   vec_t x, vec_t y);

	void updateSkeletalUI();


	/////////////////////////////////////////////////////////////////////////
	// FIXME: Might be better to nuke these in favor of direct libfreyja ABI
	/////////////////////////////////////////////////////////////////////////

	unsigned int newBone(vec_t x, vec_t y, vec_t z, unsigned char flag);
	void setNameBone(unsigned int bone, const char *name);
	const char *getNameBone(unsigned int bone);
	void addVertexToBone(unsigned int bone, unsigned int vertex);
	void removeVertexFromBone(unsigned int bone, unsigned int vertex);
	void addMeshToBone(unsigned int bone, unsigned int mesh);
	void removeMeshFromBone(unsigned int bone, unsigned int mesh);
	void connectBone(unsigned int master, unsigned int slave);
	void disconnectBone(unsigned int master, unsigned int slave);


	/////////////////////////////////////////////////////////////////////////
	/// FIXME: These should use an event mapping and/or selection system
	/////////////////////////////////////////////////////////////////////////

	void movePatchControlPoint(float xx, float yy);
	void selectPatchControlPoint(float xx, float yy);

	void VertexNew(float xx, float yy);
	void VertexMove(float xx, float yy);
	void VertexSelect(float xx, float yy);
	void VertexBuffer(float xx, float yy);
	void VertexCombine(float xx, float yy);
	void VertexDelete();

	void TexelCombine(float s, float t);
	void TexelSelect(float s, float t);
	void TexelMove(float s, float t);
	void UVMapMotion(float s, float t);

	void PolygonAddVertex(float xx, float yy);
	void PolygonDelVertex(float xx, float yy);
	void PolygonSelectVertex(float xx, float yy);

	void BBoxSelect(float xx, float yy);
	void BBoxMove(float xx, float yy);

	void MeshSelect(float xx, float yy);
	void MeshMove(float xx, float yy);
	void MeshMoveCenter(float xx, float yy);
	void MeshNew();
	void MeshDel();

	void moveBoneCenter(float xx, float yy);
	void moveBone(float xx, float yy);
	void selectBone(float xx, float yy);

	///////////////////////////////////////////////////////

	// FIXME: It's clear why this is bad for the API

	Vector<long> mUVMap;                /* 'Texture polygon' grouping */

	static BezierPatch gTestPatch;      /* Testing for curved surfaces */

	vec4_t mLight0Pos;                  /* Testing for light system */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	egg_group_t *getNearestGroup(vec_t x, vec_t y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns nearest group to x, y in given plane
	 *        ( XY, ZY, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_tag_t *getNearestTag(vec_t x, vec_t y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns nearest tag to x, y in given plane
	 *        ( XY, ZY, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *getNearestVertex(egg_group_t *group, 
								   vec_t x, vec_t y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns nearest vertex (in group) to x, y in given plane
	 *        ( XY, ZY, XZ )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.29:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	FreyjaModelPrinter mPrinter;    /* Used to reroute logging for backend */

	Vector<unsigned int> mList;     /* Temp generic vertex list buffer */

	Egg *mEgg;                      /* The 3d data model */

	EggPlugin *mPlugin;             /* Model plugin system */

	unsigned int mFlags;            /* Stores option flags as bitmap */

	bool mEggDebug;                 /* Egg debugging? on/off */

	bbox_t mSelectBBox;             /* 3d selection box using 2 vertices */

	freyja_plane_t mCurrentPlane;   /* Which plane view has editing control */

	vec3_t mScroll;                 /* Offset of scene from origin */

	unsigned int mTextureIndex;     /* Index to texture array */

	unsigned int mAnimationFrameIndex;  /* Current animation frame index */

	unsigned int mSelectionBoxOrdering; /* Determine bbox min/max ordering */

	unsigned int mBoneIndex;        /* Currently selected bone tag id */

	unsigned int mEdgeCount;        /* Number of edges for a new polygon */

	unsigned int mTexCoordIndex;    /* Currently selected texel id */

	unsigned int mVertexIndex;      /* Currently selected vertex id */

	unsigned int mVertexFrameIndex; /* Current vertex frame */

	unsigned int mPolygonIndex;     /* Current polygon */

	unsigned int mGroupIndex;       /* Current vertex grouping id */

	unsigned int mMeshIndex;         /* Currently selected mesh id */

	unsigned int mSkeletalFrameIndex;   /* Currently selected skeletal frame */


	egg_vertex_t *mCachedVertex;       /* Current vertex ( cached ) -
										* This is the lynchpin and bane
										* of this modeler at once
										* replacing this with a new
										* freyja centric selection system
										* is important */
};


#endif
