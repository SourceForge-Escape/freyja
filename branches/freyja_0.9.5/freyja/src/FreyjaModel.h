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

#include <freyja/FreyjaPluginABI.h>
#include <freyja/FreyjaPrinter.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/stack.h>

#include "FreyjaState.h"
#include "Texture.h"
#include "BezierPatch.h"
#include "freyja_events.h"


using namespace mstl;

typedef vec_t bbox2_t[3][3];


typedef enum { 
	
	PLANE_XY = 0, 
	PLANE_ZY = 1, 
	PLANE_XZ = 2,
	PLANE_FREE

} freyja_plane_t;  // view modes for model editing


void FreyjaModelEventsAttach();


class FreyjaModel
{
public:



	typedef enum {
		FL_DUMP_TEXTURE  = 1,    /* Toggle image file dumps of TR textures */
		FL_LOAD_MAP      = 2,    /* Toggle map loading in TR paks */
		FL_QUAKE_PAL     = 4,    /* Toggle quake/hexen2 palette in mdl loads */
		FL_VERTEX_UV     = 8,    /* Toggle polymapping of texcoords */
		fDontUpdateBoneName  = 16,
		fDeformBoneVertices  = 32,
		fLoadTextureInSlot   = 64,
		fLoadMaterialInSlot  = 128

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

	unsigned int getAnimationFrameCount(unsigned int animationIndex);
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

	static unsigned int getFlags();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getMeshBoundingBox(long index, vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : index is query for 'mesh' by index
	 *
	 * Post : This replaces the old Egg mesh -> group export
	 *        it pulls 'editable' vertices' bounding box.
	 *
	 ------------------------------------------------------*/


	void getVertexSelection(vec3_t min, vec3_t max,
							Vector<unsigned int> **list);

	Vector<unsigned int> &getVertexSelectionList();

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

	int loadMaterial(const char *filename);

	int loadTexture(const char *filename);

	int loadTextureBuffer(unsigned char *image, 
						  unsigned int width, 
						  unsigned int height, 
						  unsigned int bpp, 
						  Texture::ColorMode type);

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

	void mirrorUsingVertexBuffer(bool x, bool y, bool z);

	void moveObject(transform_t type, Vector3d xyz);

	void movePatchControlPoint(Vector3d xyz);

	bool pasteSelectedPatch();

	bool pasteSelectedMesh();

	bool pasteVertexBuffer();

	int saveMaterial(const char *filename);

	void selectObject(transform_t type, Vector3d xyz);


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

	static void setFlags(option_flag_t flag, int op);
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

	static bool toggleFlag(option_flag_t flag);

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

	void updateSkeletalUI();


	/////////////////////////////////////////////////////////////////////////
	// FIXME: Might be better to nuke these in favor of direct libfreyja ABI
	/////////////////////////////////////////////////////////////////////////

	int32 getCurrentModel();

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


	void VertexNew(float xx, float yy);
	void VertexMove(float xx, float yy);
	//void VertexSelect(float xx, float yy);
	void VertexBuffer(float xx, float yy);
	void VertexCombine(float xx, float yy);
	//void VertexDelete();

	void TexelCombine(float s, float t);
	void TexelSelect(float s, float t);
	void TexelMove(float s, float t);
	void UVMapMotion(float s, float t);

	void PolygonAddVertex(float xx, float yy);
	//void PolygonDelVertex(float xx, float yy);
	//void PolygonSelectVertex(float xx, float yy);

	void BBoxSelect(float xx, float yy);
	void BBoxMove(float xx, float yy);

	void MeshSelect(float xx, float yy);
	void MeshMove(float xx, float yy);
	void MeshMoveCenter(float xx, float yy);
	void MeshNew();
	void MeshDel();

	void moveBoneCenter(float xx, float yy);
	void moveBone(float xx, float yy);
	//void selectBone(float xx, float yy);




	///////////////////////////////////////////////////////

	// FIXME: It's clear why this is bad for the API


	// FIXME: Temp here to let this work during rewrite so it could be uploaded to public svn to fix revision corruption in public svn
	void initTexture()
	{
		unsigned char rgba[4] = {255, 255, 255, 255};
		mTexture.reset();
		mTexture.setMaxTextureCount(64);
		mTexture.setFlag(Texture::fUseMipmaps);
		mTexture.loadColorTexture(rgba, 32, 32);
		mTextureId = 1;
	}

	int32 mTextureId;

	Texture mTexture;

	static FreyjaModel *mInstance;


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	static unsigned int mFlags;     /* Stores option flags as bitmap */

	Vector<unsigned int> mList;     /* Temp generic vertex list buffer */

	bbox2_t mSelectBBox;            /* 3d selection box using 2 vertices */

	freyja_plane_t mCurrentPlane;   /* Which plane view has editing control */

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
};


#endif
