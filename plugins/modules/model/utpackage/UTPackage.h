/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : OpenRaider, PSKModel viewer, libhel_modelloader
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : UTPackage
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This class handles UT packages
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_UTPACKAGE - Builds UTPackage class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.07.11:
 * Mongoose - Created, form old PSKModel prototype
 ================================================================*/


#ifndef GUARD__OPENRAIDER_MONGOOSE_UTPACKAGE_H_
#define GUARD__OPENRAIDER_MONGOOSE_UTPACKAGE_H_

#include <stdio.h>
#include <sys/stat.h>

typedef struct {

	unsigned int exportCount;
	unsigned int nameCount;

} utx_generation_t;

typedef struct {

	char *objName;;
	unsigned int objFlags;

} utx_name_table_t;

typedef struct {

	int objClass;
	int objSuper;
	unsigned int package;
	int objName;
	unsigned int objFlags;
	int serialSize;
	int serialOffset;

} utx_export_table_t;

typedef struct {

	int objClass;
	int objSuper;
	unsigned int package;
	int objName;

} utx_import_table_t;

typedef struct {

	unsigned int signature;
	unsigned short version;
	unsigned short version2; // aka mod
	unsigned int flags;
	unsigned int nameCount;
	unsigned int nameOffset;
	unsigned int exportCount;
	unsigned int exportOffset;
	unsigned int importCount;
	unsigned int importOffset;

	unsigned int heritageCount; // if version < 68
	unsigned int heritageOffset; // if version < 68

	unsigned int guid[4]; // else if version > 67
	unsigned int generationCount; // else if version > 67
	utx_generation_t *generations; // else if version > 67

	utx_name_table_t *nameTable;
	utx_export_table_t *exportTable;
	utx_import_table_t *importTable;
	unsigned int *heritageTable; // 16byte IDs stored as 4 u32s

} utx_header_t;


/* Index parser algorithm and defines from 'Unreal file reader' source */
#define INDEX_SIGN_BIT          0x80    /* Sign bit on first byte */
#define INDEX_CONT1_BIT         0x40    /* Continuation bit on first byte */
#define INDEX_BYT1_MASK         0x3f    /* Data mast on first byte */
#define INDEX_CONT_BIT          0x80    /* Continuation bit on other bytes */
#define INDEX_BYTE_MASK         0x7f    /* Data mask on other bytes */

#define UNP_ByteProperty        1
#define UNP_IntProperty         2
#define UNP_BoolProperty        3
#define UNP_FloatProperty       4
#define UNP_ObjectProperty      5
#define UNP_NameProperty        6
#define UNP_StringProperty      7
#define UNP_ClassProperty       8
#define UNP_ArrayProperty       9
#define UNP_StructProperty      10
#define UNP_VectorProperty      11
#define UNP_RotatorProperty     12
#define UNP_StrProperty         13
#define UNP_MapProperty         14
#define UNP_FixedArrayProperty  15

#define PROPERTY_TYPE_MASK      0x0f
#define PROPERTY_SIZE_MASK      0x70
#define PROPERTY_ARRAY_MASK     0x80


//////////////////////////////////////////////////////////////////////
// UTMesh Class
//////////////////////////////////////////////////////////////////////

typedef struct {

	float x;
	float y;
	float z;

} UTVector;

typedef struct {

	UTVector min;
	UTVector max;
	unsigned char isValid;

} UTBoundingBox;

typedef struct {

	UTVector pos;
	float w;           /* Only on disk if package version > 61 */

} UTBoundingSphere;

typedef struct {

	float x;
	float y;
	float z;

} UTMesh_Point;

typedef struct {

	unsigned short int vertexIndexABC[3];
	unsigned char uvA[2];
	unsigned char uvB[2];
	unsigned char uvC[2];
	unsigned int flags;
	unsigned int textureIndex;

} UTMesh_Triangle;

typedef struct {

	unsigned int time;
	unsigned int function; /* INDEX, object reference */
} UTMesh_Function;

typedef struct {

	char name[64]; /* INDEX to name on disk to load */
	unsigned int group; /* INDEX */
	unsigned int startFrame;
	unsigned int endFrame;
	unsigned int functionCount; /* INDEX */
	UTMesh_Function *functions;
	float rate;
	
} UTMesh_AnimSequence;

typedef struct {

	unsigned int numVertTriangles;
	unsigned int triangleListOffset;
} UTMesh_Connect;

class UTMesh
{
public:

	UTMesh() 
	{
	}

	~UTMesh()
	{
	}

	int load();

	UTBoundingBox mPrimativeBoundingbox;
	UTBoundingSphere mPrimativeBoundingSphere;

	unsigned int mVertexJump;  /* In package version > 61 only */
	unsigned int mVertexCount; /* INDEX */
	UTMesh_Point *mVertices; /* UT diskformat differs from DEUS EX 
									  *------------------------------------
									  *	UT: DWORD xyz
									  * x = (xyz && 0x7ff) / 8;
									  * x = - ((x > 128) ? (x - 256) : x);
									  * y = ((xyz >> 11) && 0x7ff) / 8;
									  * y = - ((y > 128) ? (y - 256) : y);
									  * z = ((xyz >> 22) && 0x3ff) / 4;
									  * z = ((z > 128) ? (z - 256) : z);
									  *------------------------------------
									  *	DeusEx: QUADWORD xyz
									  * x = (xyz && 0xffff) / 256;
									  * x = - ((x > 128) ? (x - 256) : x);
									  * y = ((xyz >> 16) && 0xffff) / 256;
									  * y = - ((y > 128) ? (y - 256) : y);
									  * z = ((xyz >> 32) && 0xffff) / 256;
									  * z = ((z > 128) ? (z - 256) : z);
									  *------------------------------------
									  */

	unsigned int mTriangleJump;  /* In package version > 61 only */

	unsigned int mTriangleCount; /* INDEX */
	UTMesh_Triangle *mTriangles;

	unsigned int mAnimSequenceCount; /* INDEX */
	UTMesh_AnimSequence *mAnimSequences;

	unsigned int mConnectsJump;
	unsigned int mConnectsCount; /* INDEX */
	UTMesh_Connect *mConnects;
	
	UTBoundingBox mBoundingBox;
	UTBoundingSphere mBoundingSphere;

	unsigned int mVertexLinksJump;
	unsigned int mVertexLinksCount; /* INDEX */
	unsigned int *mVertexLinks;

	unsigned int mTextureCount; /* INDEX */
	unsigned int *mTextures; /* INDEXs, Object references */

	unsigned int mBoundingBoxesCount; /* INDEX */
	UTBoundingBox *mBoundingBoxes;

	unsigned int mBoundingSpheresCount; /* INDEX */
	UTBoundingSphere *mBoundingSpheres;

	unsigned int mFrameVertices;
	unsigned int mAnimFrames;
	unsigned int mANDFlags;
	unsigned int mORFlags;

	UTVector mScale;
	UTVector mOrigin;

	unsigned int mCurrentPolygon;
	unsigned int mCurrentVertex;

	
	unsigned int mTextureLODCount; /* INDEX, 
											  * Only read if Package version >= 66,
											  *
											  * If version == 65, then set this to 1
											  * and read 1 float for mTextureLOD
											  */
	
	float mTextureLOD;
};


//////////////////////////////////////////////////////////////////////
// UTSkeletalMesh Class
//////////////////////////////////////////////////////////////////////

typedef struct {

	unsigned short int vertexIndex;
	unsigned short int flags;
	float u;
	float v;

} UTSkeletalMesh_ExtWedge;

typedef struct {

	float x;
	float y;
	float z;

} UTSkeletalMesh_Point;

typedef struct {

	char name[64];              /* INDEX, replace with string name */
	unsigned int flags;

	/* Joint position 
	 * ( Move to new struct? Matches PSK's joint ) */
	float restDir[4];           /* Orientation ( Quaternion x,y,z,w ) */
	float restLoc[3];           /* Position ( XYZ ) */
	float length;               /* Is it really? Collision testing/debugging */
	float xSize;                /* Is it really? */
	float ySize;                /* Is it really? */
	float zSize;                /* Is it really? */	

	unsigned int numChildren;
	unsigned int parentIndex;

} UTSkeletalMesh_Bone;

typedef struct {

	unsigned short weightIndex;
	unsigned short number;
	unsigned short detailA;
	unsigned short detailB;

} UTSkeletalMesh_WeightIndex;

typedef struct {

	unsigned short pointIndex;
	unsigned short boneWeight;

} UTSkeletalMesh_Weight;

typedef struct {

	float originXYZ[3];
	float xAxisXYZ[3];
	float yAxisXYZ[3];
	float zAxisXYZ[3];

} UTSkeletalMesh_WeaponAdjust;


class UTSkeletalMesh  /* Pretty close to PSK, but it is _NOT_ */
{
public:

	UTSkeletalMesh()
	{
		mExtWedgeCount = 0;
		mExtWedges = 0x0;
		
		mPointCount = 0;
		mPoints = 0x0;
		
		mBoneCount = 0;
		mBones = 0x0;
		
		mWeightIndexCount = 0;
		mWeightIndices = 0x0;
		
		mWeightCount = 0;
		mWeights = 0x0;
		
		mLocalPointCount = 0; 
		mLocalPoints = 0x0;
		
		mSkeletalDepth = 0;

		mDefaultAnimation[0] = 0;

		mWeaponBoneIndex = 0;

		// Don't bother setting mWeaponAdjust floats to 0.0
	}

	~UTSkeletalMesh()
	{
		clear();
	}

	void clear()
	{
		mExtWedgeCount = 0;
		if (mExtWedges) delete [] mExtWedges;
		
		mPointCount = 0;
		if (mPoints) delete [] mPoints;
		
		mBoneCount = 0;
		if (mBones) delete [] mBones;
		
		mWeightIndexCount = 0;
		if (mWeightIndices) delete [] mWeightIndices;
		
		mWeightCount = 0;
		if (mWeights) delete [] mWeights;
		
		mLocalPointCount = 0; 
		if (mLocalPoints) delete [] mLocalPoints;
		
		mSkeletalDepth = 0;

		mDefaultAnimation[0] = 0;

		mWeaponBoneIndex = 0;
	}

	int serialize();

	int exportPSK(char *filename);

	int load();

	void render();

	unsigned int mExtWedgeCount; /* INDEX */
	UTSkeletalMesh_ExtWedge *mExtWedges;

	unsigned int mPointCount; /* INDEX */
	UTSkeletalMesh_Point *mPoints;

	unsigned int mBoneCount;  /* INDEX, also aka RefSkeleton */
	UTSkeletalMesh_Bone *mBones;

	unsigned int mWeightIndexCount; /* INDEX */
	UTSkeletalMesh_WeightIndex *mWeightIndices;

	unsigned int mWeightCount; /* INDEX */
	UTSkeletalMesh_Weight *mWeights;

	unsigned int mLocalPointCount; /* INDEX */
	UTSkeletalMesh_Point *mLocalPoints;

	unsigned int mSkeletalDepth;

	char mDefaultAnimation[64];    /* INDEX, replace with dereferenced string */

	unsigned int mWeaponBoneIndex;

	UTSkeletalMesh_WeaponAdjust mWeaponAdjust;
};


//////////////////////////////////////////////////////////////////////
// UTPackageProperty Class
//////////////////////////////////////////////////////////////////////

class UTPackageProperty
{
public:

	UTPackageProperty();

	~UTPackageProperty();

	void clear();

	int index;

	unsigned int nameIndex;

	char *name;    /* Points to name table entry or NULL */
	
	unsigned int size;

	unsigned int type;

	unsigned int flags;

	unsigned char *data;
};


//////////////////////////////////////////////////////////////////////
// UTPackage Class
//////////////////////////////////////////////////////////////////////

class UTPackage
{
 public:

	enum
	{
		UT_NULL,
		UT_IMPORT,
		UT_EXPORT
		
	} UT_INDEX;

	typedef enum
	{
		fDiskDump     = 1,   /* Write each class in package to disk */
		fHexDump      = 2,   /* Write a hexdump of each class to stdout */
		fDecryptOnly  = 4    /* Just dump a decrypted file to disk */
	} UTPackageFlags;


	class UTVFSObj
	{
	public:
		char filename[64];
		unsigned int signature;
		unsigned int offset;
		unsigned int size;
		unsigned char key;
		UTVFSObj() : next(NULL) { }
		UTVFSObj *next;
		void add(UTVFSObj*d) { next ? d->next = next, next = d : next = d; }
	};

	class UTVFSDir
	{
	public:
		UTVFSDir() { next = NULL; name[0] = 0; }
		char name[64];
		UTVFSDir *next;
		UTVFSObj *obj;
		void add(UTVFSDir*d) { next ? d->next = next, next = d : next = d; }
	};


	class UTVFS
	{
	public:
		UTVFS() { root = NULL; }

		UTVFSObj *Find( const char *dir, const char *file )
		{
			UTVFSDir *d = FindDir(dir);
			UTVFSObj *obj = FindFile(d, file);
			
			return obj;
		}


		UTVFSObj *FindFile( UTVFSDir *d, const char *file )
		{
			if ( d == NULL )
				return NULL;
			
			UTVFSObj *cur = d->obj;

			while (cur)
			{
				if ( strcmp(cur->filename, file) == 0 )
				{
					return cur;
				}

				cur = cur->next;
			}

			return NULL;
		}


		UTVFSDir *FindDir( const char *dir )
		{
			UTVFSDir *cur = root;

			while (cur)
			{
				if ( strcmp(cur->name, dir) == 0 )
				{
					return cur;
				}

				cur = cur->next;
			}

			return NULL;
		}


		void AddFile( const char *dir, const char *file, 
						  unsigned int signature, unsigned int offset, 
						  unsigned int size, unsigned char key)
		{
			UTVFSDir *cur = root;

			while (cur)
			{
				if ( strcmp(cur->name, dir) == 0 )
				{
					UTVFSObj *obj = new UTVFSObj();

					strncpy( obj->filename, file, 63 );
					obj->filename[63] = 0;
					obj->signature=signature;
					obj->offset=offset;
					obj->size=size;
					obj->key=key;
					
					if ( cur->obj == NULL )
					{
						cur->obj = obj;
					}
					else
					{
						cur->obj->add( obj );
					}

					break;
				}

				cur = cur->next;
			}
		}

		void AddDir( const char *dir )
		{
			if (FindDir(dir))
				return;

			UTVFSDir *d = new UTVFSDir();
			strcpy( d->name, dir );

			if ( root == NULL )
			{
				root = d;
			}
			else
			{
				root->add(d);
			}
		}

		char pakfile[512];
		UTVFSDir *root;	
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	UTPackage();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of UTPackage
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~UTPackage();
	/*------------------------------------------------------
	 * Pre  : UTPackage object is allocated
	 * Post : Deconstructs an object of UTPackage
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.11: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	unsigned char *GetVFSObject( const char *type, const char *file );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	UTVFSDir *GetVFSRoot( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool LoadPakVFS(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads UT Package as a VFS
	 ------------------------------------------------------*/

	int load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads UT Package, returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setFlags(unsigned int flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	long dTell();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : fread wrapper that also handles decryption in
	 *        Lineage II files ( no stream passing needed )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int dReadIndexValue();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : fread wrapper that also handles decryption in
	 *        Lineage II files ( no stream passing needed )
	 *
	 *        Ignore the table type and get unsigned value, 
	 *        w/o passing stream
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	size_t dRead(void *ptr, size_t size, size_t nmemb);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : fread wrapper that also handles decryption in
	 *        Lineage II files ( no stream passing needed )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	size_t dSeek(long offset, int wence);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : fread wrapper that also handles decryption in
	 *        Lineage II files ( no stream passing needed )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadProperty(UTPackageProperty &prop);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This is for external to class use to allow
	 *        per UnReal Class 'RAW' handling
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from UTPackageProperty::load()
	 ------------------------------------------------------*/

	unsigned int useIndex(int index, unsigned int *type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from use_utx_index()
	 ------------------------------------------------------*/

	utx_header_t mHeader;         /* Package header, holds tables, ver, etc */

	char *mDecyrptFilename;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	/* Ignore the table type and get unsigned numeric value */
	unsigned int getIndexValue(FILE *f);

	/* Hhhmm... should move to abstract UT file i/o class use */
	int loadSkeletalMesh(FILE *f);
	int loadMesh(FILE *f);

	size_t dRead(void *ptr, size_t size, size_t nmemb, FILE *stream);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : fread wrapper that also handles decryption in
	 *        Lineage II files
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int dumpFile(FILE *f, unsigned int size, char *s, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This allows use of dRead() when loading from 
	 *        diskfile
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from dump_file()
	 ------------------------------------------------------*/

	void hexDump(FILE *f, unsigned char *buffer, unsigned int size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This allows use of dRead() when loading from 
	 *        diskfile
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from hex_dump()
	 ------------------------------------------------------*/

	int loadProperty(UTPackageProperty &prop,
						  utx_header_t *header, FILE *f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This allows use of dRead() when loading from 
	 *        diskfile
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from UTPackageProperty::load()
	 ------------------------------------------------------*/

	char *getName(unsigned int version, FILE *f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from get_utx_name()
	 ------------------------------------------------------*/
	
	int getArrayIndex(FILE *f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from get_utx_array_index()
	 ------------------------------------------------------*/

	int getIndex(FILE *f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.24:
	 * Mongoose - Created from get_utx_index()
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	UTVFS mVFS;                   /* Pak VFS */

	unsigned int mFlags;          /* Option flags */

	unsigned char mKeyXOR;        /* XOR Key */

	unsigned int mOffset;         /* Decrypted files aren't the same size
											   as actual encrypted diskfile */

	FILE *mStream;                /* Stream of this package's diskfile */

	char *mRootDir;               /* Root diskfile dump directory */
};


#endif
