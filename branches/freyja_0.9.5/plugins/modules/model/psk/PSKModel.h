/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : PSKModel
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: Unreal Tournament skeletal model
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_PSKMODEL - Builds PSKModel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.12.29:
 * Mongoose - Filled in more unknowns using a dead plugin project's
 *            API I found on the web, which could break something.
 * 
 *            They got some of the geometry wrong, but seemed to have
 *            good guesses about Material/Anim unknowns ( UT1 infos? )
 *
 * 2003.07.12:
 * Mongoose - UT Package prototype code moved into 'UTPackage'
 *            API changes to structs to clearify meaning
 *
 * 2003.06.12:
 * Mongoose - PSA keyframes fixed, special thanks to:
 *            Steven Fuller and Forest Hale for help picking
 *            apart those tough little 8 bytes   =)
 *
 * 2003.06.11:
 * Mongoose - Broke up rendering and model loading into seperate
 *            classes ( Closely coupled )
 *
 *            PSA experimental loader
 *
 * 2003.06.10:
 * Mongoose - Skeletal format fixed, should load perfectly
 *
 * 2003.01.20:
 * Mongoose - Fixed triangle rendering thanks to debugging by
 *            Steven Fuller, who found the tris -> UV -> vert 
 *            connection
 *
 *            Finished up basic rendering, lots of 
 *            small fixes/features
 *
 * 2003.01.06:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_PSKMODEL_H_
#define GUARD__FREYJA_MONGOOSE_PSKMODEL_H_


typedef struct /* 13 bytes */
{
	unsigned short x, y, z;          /* VTXW wedge Indices */
	char material;                   /* Material Index */
	char auxMaterial;                /* Material Index 2 */
	unsigned int smoothingGroups;    /* Bitflag for smoothing groups */

} psk_face_t;


typedef struct /* 16 bytes */
{
	unsigned short vertex;          /* XYZ Vertex Index */
	char material;                  /* Is it really? */     
	char reserved;                  /* Is it really? */  
	float uv[2];
	float weight;                   /* Is it really? */

} psk_vtxw_t;


typedef struct  /* 88 bytes */
{
	char name[64];
	int textureIndex; 
	unsigned int polygonFlags;
	int auxMaterial; 
	unsigned int auxFlags;
	int lodBias;                /* Level of detail bias */
	int lodStyle;               /* Level of detail style */

} psk_material_t;


typedef struct /* 120 bytes */
{
	char name[64];
	unsigned int flags;         /* Option flags? */
	unsigned int numChildren;
	unsigned int parentIndex;

	/* Joint position 
	 * ( Move to new struct? Matches UTSkeletalMesh's joint  ) */
	float restDir[4];           /* Orientation ( Quaternion x,y,z,w ) */
	float restLoc[3];           /* Position ( XYZ ) */
	float length;               /* Is it really? Collision testing/debugging */
	float xSize;                /* Is it really? */
	float ySize;                /* Is it really? */
	float zSize;                /* Is it really? */

} psk_bone_t;


typedef struct  /* 12 bytes */
{
	float weight;
	unsigned int vertexIndex;
	unsigned int boneIndex;

} psk_weight_t;


typedef struct /* 168 bytes */
{
	char animName[64];
	char groupName[64];
	int numBones;          /* Total number of bones*/
	int rootInclude;       /* 0 - None, 1 - Included */
	int keyCompression;    /* (Reserved?) Type of compression used */
	int keyQuotum;         /* Maximum key quotum for compression */
	float keyReduction;    /* */
	float trackTime;       /* Actual, but can be overridden by animation rate*/
	float animRate;        /* Frames per second */
	int startBone;         /* (Unused?) For partial animations ala tombraider */
	int firstRawFrame;     /* */
	int numRawFrames;      /* numRawFrames and animRate control tracktime */

} psa_anim_info_t;


typedef struct /* 32 bytes */
{
	float trans[3];	/* Position ( XYZ ) */
	float dir[4];		/* Orientation ( Quaternion x,y,z,w ) */
	float time;		   /* Time to next key frame */

} psa_key_frame_t;


class PSKModel
{
 public:

	enum PSKModelFlag
	{
		fDebugWeightLoad		= 1,
		fDebugPointLoad		= 2,
		fDebugFaceLoad			= 4,
		fDebugUVLoad			= 8,
		fDebugMattLoad			= 16,
		fDebugBoneLoad			= 32
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	PSKModel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of PSKModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~PSKModel();
	/*------------------------------------------------------
	 * Pre  : PSKModel object is allocated
	 * Post : Deconstructs an object of PSKModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void printSkeletion(bool printNames);
	/*------------------------------------------------------
	 * Pre  : <PrintNames> if true use names instead of Ids
	 *
	 * Post : Prints skeletion bone trace one bone per line
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	int load(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads PSK model from disk
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int save(char *filename);
	/*------------------------------------------------------
	 * Pre  : Convert your model file to this PSKModel object
	 * Post : Save PSKModel object to disk
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int mFlags;

	unsigned int mNumFrames;

	unsigned int mNumVertices;

	unsigned int mNumFaces;

	unsigned int mNumVTXWs;

	unsigned int mNumMaterials;

	unsigned int mNumBones;

	unsigned int mNumWeights;

	float *mVertices;					/* Vertices of the model */

	psk_vtxw_t *mVTXWs;					/* UV Wedges */

	psk_face_t *mFaces;					/* The triangle mesh */

	psk_material_t *mMaterials;			/* Materials/textures */

	psk_bone_t *mBones;					/* The bones of the model */

	psk_weight_t *mWeights;				/* The weights of the bones */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};


//////////////////////////////////////////////////////////////////////
// PSAAnimation Class
//////////////////////////////////////////////////////////////////////

class PSAAnimation
{
public:

	enum PSKModelRendererFlag
	{
		fReserved1		= 1,
		fReserved2		= 2,
		fDebugBones		= 4,
		fDebugAnimInfos	= 8,
		fDebugKeyFrames	= 16
	};

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	PSAAnimation();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of PSAAnimation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~PSAAnimation();
	/*------------------------------------------------------
	 * Pre  : PSAAnimation object is allocated
	 * Post : Deconstructs an object of PSAAnimation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int load(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads PSA animation from disk
	 *
	 *        Returns < 0 on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	unsigned int mFlags;

	unsigned int mNumFrames;

	unsigned int mNumBones;

	unsigned int mNumAnimInfos;

	unsigned int mNumKeyFrames;

	psk_bone_t *mBones;

	psa_anim_info_t *mAnimInfos;

	psa_key_frame_t *mKeyFrames;


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// PSKModelRenderer Class
//////////////////////////////////////////////////////////////////////

class PSKModelRenderer
{
 public:

	enum PSKModelRendererFlag
	{
		fRenderFaces		= 1,
		fRenderTexture		= 2,
		fRenderPoints		= 4,
		fRenderBones 		= 8,
		fReservedBit16		= 16,
		fDebugFaceRender 	= 32
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	PSKModelRenderer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of PSKModelRenderer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~PSKModelRenderer();
	/*------------------------------------------------------
	 * Pre  : PSKModelRenderer object is allocated
	 * Post : Deconstructs an object of PSKModelRenderer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void render();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders PSK model in OpenGL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void renderBone(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders PSK model's bone in OpenGL, 
	 *        ( Recursive calling )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.06:
	 * Mongoose - Created, from nonpublic render()
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void generateNormals();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generate normals for faces and vertices
	 ------------------------------------------------------*/

	void convertBoneAngles();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Converts bone angles for rendering use
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void convertBoneAnglesPSA(unsigned int frame);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Converts PSA bone angles for rendering use
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void copyVertices();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resets vertices to match PSKModel's default
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setAnimation(PSAAnimation *anim);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets PSA animation to render, and sets up cache
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.11:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setModel(PSKModel *model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets PSK model to render, and sets up cache
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setupRestMatrices(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : <Id> the bone to setup matching invert matrix 
	 *
	 * Post : Generates inverted matrices to render deformed
	 *        mesh by recursive call from root bone
	 *
	 *        OpenGL accelerated where avalibable
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.07:
	 * Mongoose - Created, was boneTransforms
	 ------------------------------------------------------*/

	void setupWorldMatrices(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : <Id> the bone to setup matching transform matrix 
	 *
	 * Post : Generates transform matrices to render deformed
	 *        mesh by recursive call from root bone
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created, was boneTransforms
	 ------------------------------------------------------*/

	void transformVertices();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transforms vertices to deformed mesh for 
	 *        current bone frame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.10:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int mFlags;

	int mTextures[8];							/* Texture Ids of loaded materials */

	unsigned int mAnimationFrame;			/* Used to keep up with animation 
														frame externally, 
													
														And updated	internally

														Should be moved to 
														private+acessor method
													*/

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	float *mFaceNormals;

	float *mVertexNormals;

	float *mBoneRotationCache;				/* Cache of the computed axis angles */

	float *mVertexTransformCache;			/* Cache of vertex transforms */

	float **mInvertedMatrices;          /* Cache of computed inverted 
														bone transform matrices for the
														vertices at rest position */

	float **mWorldMatrices;

	unsigned int mNumMatrices;


	/* Simulate inheritence here, cheesy waste */

	unsigned int mNumFrames;

	unsigned int mNumVertices;

	unsigned int mNumFaces;

	unsigned int mNumVTXWs;

	unsigned int mNumMaterials;

	unsigned int mNumBones;

	unsigned int mNumWeights;

	PSKModel *mModel;						/* Current PSK model to render */

	PSAAnimation *mAnimation;				/* Current PSA anim to render */

	psk_vtxw_t *mVTXWs;						/* UV Wedges */

	psk_face_t *mFaces;						/* The triangle mesh */

	psk_material_t *mMaterials;		 	    /* Materials/textures */

	psk_bone_t *mBones;						/* The bones of the model */

	psk_weight_t *mWeights;					/* The weights of the bones */
};

#endif
