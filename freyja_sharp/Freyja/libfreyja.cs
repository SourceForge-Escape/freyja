// created on 4/13/2005 at 12:47 AM

//#define USING_LIBFREYJA

using System;
using System.Runtime.InteropServices;


#if USING_LIBFREYJA
using vec3 = System.Double;

public class libfreyja {


	public enum freyja_colormode_t
	{
		INDEXED_8 = 1, 
		RGB_24, 
		RGBA_32
	}

	public enum freyja_lock_t
	{
		FREYJA_WRITE_LOCK = 1,
		FREYJA_WRITE_UNLOCK
	} 

	public enum freyja_object_t
	{
        FREYJA_MODEL = 1,
        FREYJA_MESH,
        FREYJA_POLYGON,
        FREYJA_BONE,
        FREYJA_SKELETON,
        FREYJA_VERTEX_FRAME,
        FREYJA_VERTEX_GROUP,
        FREYJA_VERTEX,
        FREYJA_TEXCOORD,
        FREYJA_MATERIAL,
        FREYJA_TEXTURE,
        FREYJA_ANIMATION,
        FREYJA_MESHTREE_FRAME,
        FREYJA_MESHTREE_TAG
	}

	public enum freyja_transform_t
	{
		fTransformScene = 1,
		fTransformModel,
		fTransformMesh,
		fTransformVertexFrame,
		fTransformSkeleton,
		fTransformBone,
		fTransformUVMap,
		fTransformVertexGroup,
		fTransformVertex,
		fTransformTexCoord
	} 

	public struct freyja_file_header_t
	{
		[MarshalAs (UnmanagedType.ByValArray, SizeConst=16)]
		public char []magic; // [16]
		public long version;
		public long flags;
		public long reserved;
		[MarshalAs (UnmanagedType.ByValArray, SizeConst=64)]
		public char []comment; //[64]
	}

	public struct freyja_file_chunk_t
	{
		public long type;
		public long size;
		public long flags;
		public long version;
	}

	public enum freyja_file_chunk_type_t
	{
		FREYJA_CHUNK_MODEL    = 0x204C444D,
		FREYJA_CHUNK_MESH     = 0x4853454D,
		FREYJA_CHUNK_TEXCOORDS= 0x524F4F43,
		FREYJA_CHUNK_VERTICES = 0x54524556,
		FREYJA_CHUNK_POLYGONS = 0x594C4F50,
		FREYJA_CHUNK_SKELETON = 0x4C454B53,
		FREYJA_CHUNK_BONE     = 0x454E4F42,
		FREYJA_CHUNK_MATERIAL = 0x5454414D,
		FREYJA_CHUNK_TEXTURE  = 0x54584554,
		FREYJA_CHUNK_METADATA = 0x4154454D
	} 

	const uint fPolygon_VertexUV     = 2;
	const uint fPolygon_PolyMappedUV = 8;
	const uint fPolygon_PolyMapped   = 8; // alias
	const uint fPolygon_ColorMapped  = 16;

	const string libname = "/usr/local/lib/libfreyja.so";

    ///////////////////////////////////////////////////////////////////////        // Freyja Iterator functions
    // Freyja Iterator functions
    ///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaBegin")]
	public extern static void freyjaBegin(freyja_object_t type); 
	/*------------------------------------------------------
     * Pre  : <type> is valid
     * Post : A new object of type is constructed.
     ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaEnd")]
	public extern static void freyjaEnd(); 
	/*------------------------------------------------------
     * Pre  : freyjaBegin(...) is called to start the state
     * Post : The object is finialized and construction ends
     ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaIterator")]
	public extern static long freyjaIterator(freyja_object_t type, long item);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 *        <item> is a FREYJA_LIST_... command or item index
	 *
	 * Post : Sets current item in internal iterator to
	 *        do operations on util a new construction or
	 *        iterator operation of the same type is done.
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaCriticalSection")]
	public extern static long freyjaCriticalSection(freyja_lock_t request);
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin singleton exists
	 *
	 * Post : Returns 0 on sucess
	 *        Returns FREYJA_PLUGIN_ERROR on fail
	 *
	 *        If FreyjaPlugin is locked this will block
	 *        in the future. For now it's just to test if
	 *        the singleton is up and running.
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetCount")]
	public extern static long freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetCurrent")]
	public extern static long freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns index of current internal complex type
	 *        ( Last constructed or iterated )
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// libfreyja plugin ABI 0.9.1 object accessors
	///////////////////////////////////////////////////////////////////////

	// FREYJA_TEXTURE Accessors ///////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetTextureFilename")]
	public extern static int freyjaGetTextureFilename(uint index, char **filename);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <filename> on return
	 *
	 * Post : Gets a pointer to the filename of texture[index]
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetTextureImage")]
	public extern static int freyjaGetTextureImage(uint index,
							  uint *w, uint *h, 
							  uint *depth,  uint *type,
							  byte **image);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <image> on return
	 *
	 * Post : Gets pointers to texture[index]'s data members
	 *        Returns 0 on success.
	 ------------------------------------------------------*/


	// FREYJA_BONE Accessors //////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetSkeletonBoneCount")]
	public extern static long freyjaGetSkeletonBoneCount(long skeletonIndex);
	[DllImport(libname, EntryPoint="freyjaGetSkeletonBoneIndex")]
	public extern static long freyjaGetSkeletonBoneIndex(long skeletonIndex, long element);

	[DllImport(libname, EntryPoint="freyjaGetBoneName")]
	public extern static long freyjaGetBoneName(long boneIndex, uint size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneParent")]
	public extern static long freyjaGetBoneParent(long boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 *        Returns -2 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneRotationWXYZ4fv")]
	public extern static long freyjaGetBoneRotationWXYZ4fv(long boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneRotationXYZ3fv")]
	public extern static long freyjaGetBoneRotationXYZ3fv(long boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneTranslation3fv")]
	public extern static long freyjaGetBoneTranslation3fv(long index, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	// FREYJA_TEXCOORD Accessors //////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetTexCoord2fv")]
	public extern static void freyjaGetTexCoord2fv(long texcoordIndex, vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : texcoord[index] exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/


	// FREYJA_VERTEX Accessors ////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetVertexTexCoord2fv")]
	public extern static void freyjaGetVertexTexCoord2fv(vec2_t uv);
	[DllImport(libname, EntryPoint="freyjaGetVertexNormal3fv")]
	public extern static void freyjaGetVertexNormal3fv(vec3_t xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertex3fv")]
	public extern static void freyjaGetVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Sets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 *
	 *        This is deprecated
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetVertexTexCoordUV2fv")]
	public extern static long freyjaGetVertexTexCoordUV2fv(long vertexIndex, vec2_t uv);
	[DllImport(libname, EntryPoint="freyjaGetVertexNormalXYZ3fv")]
	public extern static long freyjaGetVertexNormalXYZ3fv(long vertexIndex, vec3_t nxyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexXYZ3fv")]
	public extern static long freyjaGetVertexXYZ3fv(long vertexIndex, vec3_t xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexFrame")]
	public extern static long freyjaGetVertexFrame(long vertexIndex, long element,
							  long *frameIndex, vec3_t xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexFrameCount")]
	public extern static long freyjaGetVertexFrameCount(long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaGetVertexWeight")]
	public extern static long freyjaGetVertexWeight(long vertexIndex, long element,
							   long *bone, vec_t *weight);
	[DllImport(libname, EntryPoint="freyjaGetVertexWeightCount")]
	public extern static long freyjaGetVertexWeightCount(long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaGetVertexFlags")]
	public extern static long freyjaGetVertexFlags(long vertexIndex);

	// FREYJA_MODEL Accessors
	[DllImport(libname, EntryPoint="freyjaGetModelFlags")]
	public extern static long freyjaGetModelFlags(long modelIndex);
	[DllImport(libname, EntryPoint="freyjaGetModelMeshIndex")]
	public extern static long freyjaGetModelMeshIndex(long modelIndex, long element);
	[DllImport(libname, EntryPoint="freyjaGetModelMeshCount")]
	public extern static long freyjaGetModelMeshCount(long modelIndex);

	// FREYJA_MESH Accessors
	//int freyjaGetMeshBoundingBox(long meshIndex, vec3_t min, vec3_t max);
	[DllImport(libname, EntryPoint="freyjaGetMeshFlags")]
	public extern static long freyjaGetMeshFlags(long meshIndex);
	[DllImport(libname, EntryPoint="freyjaGetMeshPosition")]
	public extern static long freyjaGetMeshPosition(long meshIndex, vec3_t xyz);
	[DllImport(libname, EntryPoint="freyjaGetMeshNameString")]
	public extern static char *freyjaGetMeshNameString(long meshIndex); // don't alter string
	[DllImport(libname, EntryPoint="freyjaGetMeshName1s")]
	public extern static long freyjaGetMeshName1s(long meshIndex, long lenght, char *name);

	[DllImport(libname, EntryPoint="freyjaGetMeshVertexIndex")]
	public extern static long freyjaGetMeshVertexIndex(long meshIndex, long element);
	/*------------------------------------------------------
	 * Pre  : freyjaGetMeshVertexCount must be called before
	 *        this is valid while using the Egg backend
	 *
	 * Post : Returns the index of the local vertex <element>
	 *        for mesh <meshIndex> or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMeshVertexCount")]
	public extern static long freyjaGetMeshVertexCount(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of local vertices
	 *        for mesh <meshIndex> or 0 on error
	 *
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMeshPolygonVertexIndex")]
	public extern static long freyjaGetMeshPolygonVertexIndex(long meshIndex, long faceVertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the index of local vertices
	 *        for mesh <meshIndex> with local
	 *        mapping from polygon to mesh ids or -1 on error
	 *
	 ------------------------------------------------------*/
	

	[DllImport(libname, EntryPoint="freyjaGetMeshPolygonIndex")]
	public extern static long freyjaGetMeshPolygonIndex(long meshIndex, long element);
	[DllImport(libname, EntryPoint="freyjaGetMeshPolygonCount")]
	public extern static long freyjaGetMeshPolygonCount(long meshIndex);
	[DllImport(libname, EntryPoint="freyjaGetMeshVertexGroupIndex")]
	public extern static long freyjaGetMeshVertexGroupIndex(long meshIndex, long element);
	[DllImport(libname, EntryPoint="freyjaGetMeshVertexGroupCount")]
	public extern static long freyjaGetMeshVertexGroupCount(long meshIndex);
	[DllImport(libname, EntryPoint="freyjaGetMeshVertexFrameIndex")]
	public extern static long freyjaGetMeshVertexFrameIndex(long meshIndex, long element); // Not Implemented
	[DllImport(libname, EntryPoint="freyjaGetMeshVertexFrameCount")]
	public extern static long freyjaGetMeshVertexFrameCount(long meshIndex); // Not Implemented

// FREYJA_POLYGON Accessors
[DllImport(libname, EntryPoint="freyjaGetPolygonMaterial")]
	public extern static long freyjaGetPolygonMaterial(long polygonIndex);
[DllImport(libname, EntryPoint="freyjaGetPolygonFlags")]
	public extern static long freyjaGetPolygonFlags(long polygonIndex);
[DllImport(libname, EntryPoint="freyjaGetPolygonEdgeCount")]
	public extern static long freyjaGetPolygonEdgeCount(long polygonIndex);
[DllImport(libname, EntryPoint="freyjaGetPolygonVertexCount")]
	public extern static long freyjaGetPolygonVertexCount(long polygonIndex);
[DllImport(libname, EntryPoint="freyjaGetPolygonTexCoordCount")]
	public extern static long freyjaGetPolygonTexCoordCount(long polygonIndex);
[DllImport(libname, EntryPoint="freyjaGetPolygonVertexIndex")]
	public extern static long freyjaGetPolygonVertexIndex(long polygonIndex, long element);
[DllImport(libname, EntryPoint="freyjaGetPolygonTexCoordIndex")]
	public extern static long freyjaGetPolygonTexCoordIndex(long polygonIndex, long element);


	[DllImport(libname, EntryPoint="freyjaGetPolygon1")]
	public extern static long freyjaGetPolygon1u(freyja_object_t type, long item, long *value);
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

[DllImport(libname, EntryPoint="freyjaGetPolygon3f")]
	public extern static long freyjaGetPolygon3f(freyja_object_t type, long item, vec_t *value);
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

//FIXME [DllImport(libname, EntryPoint="freyjaPrintError")]
//FIXME	public extern static void freyjaPrintError(const char *format, ...);
/*------------------------------------------------------
 * Pre  : Format string and args are valid
 * Post : Report messages to stdout
 *
 *-- History ------------------------------------------
 *
 * 2004.05.18:
 * Mongoose - Created, split from experimental branch
 ------------------------------------------------------*/

//FIXME	[DllImport(libname, EntryPoint="freyjaPrintMessage")]
//FIXME		public extern static void freyjaPrintMessage(const char *format, ...);
/*------------------------------------------------------
 * Pre  : Format string and args are valid
 * Post : Report messages to stdout
 *
 *-- History ------------------------------------------
 *
 * 2004.05.18:
 * Mongoose - Created, split from experimental branch
 ------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////
// Mutator functions to operate on Scene
///////////////////////////////////////////////////////////////////////

[DllImport(libname, EntryPoint="freyjaSetNormal3f")]
	public extern static void freyjaSetNormal3f(uint index, vec_t x, vec_t y, vec_t z);
[DllImport(libname, EntryPoint="freyjaSetNormal3fv")]
	public extern static void freyjaSetNormal3fv(uint index, vec3_t xyz);

[DllImport(libname, EntryPoint="freyjaSetTexCoord2f")]
	public extern static void freyjaSetTexCoord2f(uint index, vec_t u, vec_t v);
[DllImport(libname, EntryPoint="freyjaSetTexCoord2fv")]
	public extern static void freyjaSetTexCoord2fv(uint index, vec2_t uv);

[DllImport(libname, EntryPoint="freyjaSetVertex3f")]
	public extern static void freyjaSetVertex3f(uint index, vec_t x, vec_t y, vec_t z);
	//void freyjaSetVertex3fv(uint index, vec3_t xyz);

[DllImport(libname, EntryPoint="freyjaNormal3f")]
	public extern static long freyjaNormal3f(vec_t x, vec_t y, vec_t z);
[DllImport(libname, EntryPoint="freyjaNormal3fv")]
	public extern static long freyjaNormal3fv(vec3_t xyz);
/*------------------------------------------------------
 * Pre  : (x,y,z) is a valid normal vector
 * Post : A new normal is created in the mesh
 *        Returns the native index of that normal
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaTexCoord2f")]
	public extern static long freyjaTexCoord2f(vec_t u, vec_t v);
[DllImport(libname, EntryPoint="freyjaTexCoord2fv")]
	public extern static long freyjaTexCoord2fv(vec2_t uv);
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

[DllImport(libname, EntryPoint="freyjaVertexNormalFlip")]
	public extern static void freyjaVertexNormalFlip(long index);
[DllImport(libname, EntryPoint="freyjaVertexTexCoord2f")]
	public extern static void freyjaVertexTexCoord2f(long index, vec_t u, vec_t v);
[DllImport(libname, EntryPoint="freyjaVertexTexCoord2fv")]
	public extern static void freyjaVertexTexCoord2fv(long index, vec2_t uv);
[DllImport(libname, EntryPoint="freyjaVertexNormal3f")]
	public extern static void freyjaVertexNormal3f(long index, vec_t nx, vec_t ny, vec_t nz);
[DllImport(libname, EntryPoint="freyjaVertexNormal3fv")]
	public extern static void freyjaVertexNormal3fv(long index, vec3_t nxyz);
[DllImport(libname, EntryPoint="freyjaVertex3f")]
	public extern static long freyjaVertex3f(vec_t x, vec_t y, vec_t z);
[DllImport(libname, EntryPoint="freyjaVertexXYZ3fv")]
	public extern static long freyjaVertexXYZ3fv(long vertexIndex, vec3_t xyz);
[DllImport(libname, EntryPoint="freyjaVertex3fv")]
	public extern static long freyjaVertex3fv(vec3_t xyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_GROUP);
 *        x,y,z are valid 3space coors
 * Post : A new vertex created in the model
 *        Returns the native index of that vertex
 *
 *-- History ------------------------------------------
 *
 * 1999.07.31:
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaVertexWeight")]
	public extern static void freyjaVertexWeight(long index, vec_t weight, long bone);
/*------------------------------------------------------
 * Pre  : <weight> of influence of <bone> on vertex[<index>]
 *
 * Post : Vertex <index> in the model gets weight added
 *        to influence list, if there is a weight for the
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

[DllImport(libname, EntryPoint="freyjaMeshTreeAddFrame")]
	public extern static void freyjaMeshTreeAddFrame(vec_t x, vec_t y, vec_t z);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_MESHTREE_ANIM);
 * Post : Adds a new meshtree frame to meshtree
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaMeshTreeFrameAddBone")]
	public extern static void freyjaMeshTreeFrameAddBone(long tag);
/*------------------------------------------------------
 * Pre  : eggBegin(FREYJA_MESHTREE_ANIM_FRAME);
 * Post : Appends tag to mesh tree frame
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaTextureFilename1s")]
	public extern static long freyjaTextureFilename1s(char *filename);

[DllImport(libname, EntryPoint="freyjaTextureStoreBuffer")]
	public extern static long freyjaTextureStoreBuffer(byte *image, uint depth,
							  uint width, uint height,
							  freyja_colormode_t type);

[DllImport(libname, EntryPoint="freyjaBoneFlags1i")]
	public extern static void freyjaBoneFlags1i(long boneIndex, long flags);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone flags
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneParent1i")]
	public extern static void freyjaBoneParent1i(long boneIndex, long parentIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone parent
 *
 *        This doesn't affect skeleton, a follow up
 *        call to freyjaBoneAddChild1i is needed after
 *        all bones in skeleton are allocated
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneName1s")]
	public extern static void freyjaBoneName1s(long boneIndex, char *name);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set human readable bone name
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneAddMesh1i")]
	public extern static void freyjaBoneAddMesh1i(long boneIndex, long meshIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Mesh is added to Bone's child list
 *
 *        Either makes mesh tree connection or
 *        simulates by vertex weights and pivots
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneAddChild1i")]
	public extern static void freyjaBoneAddChild1i(long boneIndex, long childIndex);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Child is added to Bone's child list
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneTranslate3f")]
	public extern static void freyjaBoneTranslate3f(long boneIndex, vec_t x, vec_t y, vec_t z);
[DllImport(libname, EntryPoint="freyjaBoneTranslate3fv")]
	public extern static void freyjaBoneTranslate3fv(long boneIndex, vec3_t xyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone relative position
 *
 *-- History ------------------------------------------
 *
 * 2001.10.27: 
 * Mongoose - Created
 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaBoneRotateEulerXYZ3f")]
	public extern static void freyjaBoneRotateEulerXYZ3f(long boneIndex, vec_t x, vec_t y, vec_t z);
[DllImport(libname, EntryPoint="freyjaBoneRotateEulerXYZ3fv")]
	public extern static void freyjaBoneRotateEulerXYZ3fv(long boneIndex, vec3_t xyz);
[DllImport(libname, EntryPoint="freyjaBoneRotateQuatWXYZ4f")]
	public extern static void freyjaBoneRotateQuatWXYZ4f(long boneIndex,vec_t w,vec_t x,vec_t y,vec_t z);
[DllImport(libname, EntryPoint="freyjaBoneRotateQuatWXYZ4fv")]
	public extern static void freyjaBoneRotateQuatWXYZ4fv(long boneIndex, vec4_t wxyz);
/*------------------------------------------------------
 * Pre  : freyjaBegin(FREYJA_BONE);
 * Post : Set bone orientation
 *
 *-- History ------------------------------------------
 *
 * 2001.11.18: 
 * Mongoose - Created
 ------------------------------------------------------*/


[DllImport(libname, EntryPoint="freyjaGroupCenter3f")]
	public extern static void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);

[DllImport(libname, EntryPoint="freyjaGenerateQuadPlaneMesh")]
	public extern static void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side);

[DllImport(libname, EntryPoint="freyjaGenerateQuadCubeMesh")]
	public extern static void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side);

[DllImport(libname, EntryPoint="freyjaGenerateCircleMesh")]
	public extern static void freyjaGenerateCircleMesh(vec3_t origin, long count); // radius

[DllImport(libname, EntryPoint="freyjaGenerateConeMesh")]
	public extern static void freyjaGenerateConeMesh(vec3_t origin, vec_t height, long count); // radius

[DllImport(libname, EntryPoint="freyjaGenerateCylinderMesh")]
	public extern static void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								long count, long segments); // radius

[DllImport(libname, EntryPoint="freyjaGenerateSphereMesh")]
	public extern static void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  long count, long segments);

[DllImport(libname, EntryPoint="freyjaGenerateTubeMesh")]
	public extern static void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							long count, long segments); // radius

	[DllImport(libname, EntryPoint="freyjaMeshPromoteTexcoordsToPloymapping")]
	public extern static void freyjaMeshPromoteTexcoordsToPloymapping(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Takes vertex UVs and promotes them to polymapped
	 *        texcoords ( stored in polygon ala texture polygons )
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshRemovePolygon")]
	public extern static int freyjaMeshRemovePolygon(long meshIndex, long polygonIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Removes references to polygonIndex in mesh, but
	 *        it doesn't free the allocated polygon
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshUVMapSpherical")]
	public extern static void freyjaMeshUVMapSpherical(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Spherical mapping algorithm
	 *
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshUVMapCylindrical")]
	public extern static void freyjaMeshUVMapCylindrical(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Cylindrical mapping algorithm
	 *
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshTesselateTriangles")]
	public extern static void freyjaMeshTesselateTriangles(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 *
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshNormalFlip")]
	public extern static void freyjaMeshNormalFlip(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMeshGenerateVertexNormals")]
	public extern static void freyjaMeshGenerateVertexNormals(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Recalculates all vertex normals in mesh
	 *
	 ------------------------------------------------------*/

[DllImport(libname, EntryPoint="freyjaMeshPosition")]
	public extern static long freyjaMeshPosition(long meshIndex, vec3_t xyz);

[DllImport(libname, EntryPoint="freyjaMeshName1s")]
	public extern static void freyjaMeshName1s(long meshIndex, char *name);

[DllImport(libname, EntryPoint="freyjaMeshFlags1u")]
	public extern static void freyjaMeshFlags1u(uint flags);
/*------------------------------------------------------
 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
 * Post : Sets flags for current mesh
 *
 *-- History ------------------------------------------
 *
 * 2004.05.16:
 * Mongoose - Created
 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGenerateVertexNormals")]
	public extern static void freyjaGenerateVertexNormals();

	[DllImport(libname, EntryPoint="freyjaGenerateUVFromXYZ")]
	public extern static void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v);

	[DllImport(libname, EntryPoint="freyjaVertexFrame3f")]
	public extern static void freyjaVertexFrame3f(long index, vec_t x, vec_t y, vec_t z);


	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaPolygonSplit")]
	public extern static void freyjaPolygonSplit(long meshIndex, long polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonTexCoordPurge")]
	public extern static void freyjaPolygonTexCoordPurge(long polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonExtrudeQuad1f")]
	public extern static int freyjaPolygonExtrudeQuad1f(long polygonIndex, vec_t dist);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by face normal scaled by dist
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonExtrudeQuad")]
	public extern static int freyjaPolygonExtrudeQuad(long polygonIndex, vec3_t normal);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal'
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonAddVertex1i")]
	public extern static void freyjaPolygonAddVertex1i(long polygonIndex, long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaPolygonVertex1i")]
	public extern static void freyjaPolygonVertex1i(long index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonAddTexCoord1i")]
	public extern static void freyjaPolygonAddTexCoord1i(long polygonIndex, long texcoordIndex);
	[DllImport(libname, EntryPoint="freyjaPolygonTexCoord1i")]
	public extern static void freyjaPolygonTexCoord1i(long index);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPolygonSetMaterial1i")]
	public extern static void freyjaPolygonSetMaterial1i(long polygonIndex, long materialIndex);
	[DllImport(libname, EntryPoint="freyjaPolygonMaterial1i")]
	public extern static void freyjaPolygonMaterial1i(long id);  // DEPRECATED
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Animation ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaAnimationCreate")]
	public extern static long freyjaAnimationCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Animation object
	 *        Returns the new Animation's index or -1 on error
	 *
	 * NOTES: The new skeletal animation system allows for
	 *        sharing (subset) animations across multiple
	 *        skeletons using name matching to assure the
	 *        skeletons match. 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationBoneCreate")]
	public extern static long freyjaAnimationBoneCreate(long animationIndex,
								    char *name, long boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Creates a new Freyja Animation Bone object
	 *        Returns the new Bone's index or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationBoneKeyFrameCreate")]
	public extern static long freyjaAnimationBoneKeyFrameCreate(long animationIndex,
										   long boneIndex,
										   vec_t time, vec3_t xyz, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the keyframe's local Animation's Bone's
	 *        Keyframe element index or -1 on error
	 ------------------------------------------------------*/


	/* Animation Accessors */

	[DllImport(libname, EntryPoint="freyjaGetAnimationCount")]
	public extern static long freyjaGetAnimationCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Animations being managed
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetAnimationBoneCount")]
	public extern static long freyjaGetAnimationBoneCount(long animationIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Returns the number of bones or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetAnimationBoneKeyFrameCount")]
	public extern static long freyjaGetAnimationBoneKeyFrameCount(long animationIndex, 
											 long boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the number of keyframes or -1 on error
	 ------------------------------------------------------*/

	// Finish me


	/* Animation Mutators */

	[DllImport(libname, EntryPoint="freyjaAnimationName")]
	public extern static void freyjaAnimationName(long animationIndex,  char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationBoneName")]
	public extern static void freyjaAnimationBoneName(long animationIndex, long boneIndex,
								  char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationFrameRate")]
	public extern static void freyjaAnimationFrameRate(long animationIndex, vec_t frameRate);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationTime")]
	public extern static void freyjaAnimationTime(long animationIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationSubsetRoot")]
	public extern static void freyjaAnimationSubsetRoot(long animationIndex, long startBone);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFrameTime")]
	public extern static void freyjaAnimationKeyFrameTime(long animationIndex, long boneIndex,
									 long keyFrameIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets time for <keyFrameIndex> keyframe in animation
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFramePosition")]
	public extern static void freyjaAnimationKeyFramePosition(long animationIndex, long boneIndex, 
										 long keyFrameIndex, vec3_t position);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's position in animation
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFrameOrientationXYZ")]
	public extern static void freyjaAnimationKeyFrameOrientationXYZ(long animationIndex,
											   long boneIndex, 
											   long keyFrameIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Euler angles
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFrameOrientationWXYZ")]
	public extern static void freyjaAnimationKeyFrameOrientationWXYZ(long animationIndex,
												long boneIndex,
												long keyFrameIndex,vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Quaternion ( note the w, xyz order )
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Material ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaMaterialCreate")]
	public extern static long freyjaMaterialCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 ------------------------------------------------------*/


	/* Material Accessors */

	[DllImport(libname, EntryPoint="freyjaGetMaterialCount")]
	public extern static long freyjaGetMaterialCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialIndex")]
	public extern static long freyjaGetMaterialIndex(long materialIndex, long element);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns the gobal object index ( from the local
	 *        Material element index ) or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialName")]
	public extern static char *freyjaGetMaterialName(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        Don't alter the returned string
	 *
	 * Post : Returns a pointer to NULL terminated name string
	 *        Returns 0x0 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialFlags")]
	public extern static long freyjaGetMaterialFlags(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns flags or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialTexture")]
	public extern static long freyjaGetMaterialTexture(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns texture index or -1 for error or no texture
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialAmbient")]
	public extern static void freyjaGetMaterialAmbient(long materialIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialDiffuse")]
	public extern static void freyjaGetMaterialDiffuse(long materialIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialSpecular")]
	public extern static void freyjaGetMaterialSpecular(long materialIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialEmissive")]
	public extern static void freyjaGetMaterialEmissive(long materialIndex, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <emissive> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialShininess")]
	public extern static vec_t freyjaGetMaterialShininess(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns specular exponent or -1.0 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialTransparency")]
	public extern static vec_t freyjaGetMaterialTransparency(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns transparency or -1.0 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialBlendSource")]
	public extern static long freyjaGetMaterialBlendSource(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend source factor or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialBlendDestination")]
	public extern static vec_t freyjaGetMaterialBlendDestination(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend destination factor or -1 on error
	 ------------------------------------------------------*/


	/* Material Mutators */

	[DllImport(libname, EntryPoint="freyjaMaterialName")]
	public extern static void freyjaMaterialName(long materialIndex, char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <name> id is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialFlags")]
	public extern static void freyjaMaterialFlags(long materialIndex, long flags);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's bit <flags> are set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialTextureFilename")]
	public extern static void freyjaMaterialTextureFilename(long materialIndex, char *filename);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material textures's <filename> id is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialTexture")]
	public extern static void freyjaMaterialTexture(long materialIndex, long textureIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <texture> index is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialAmbient")]
	public extern static void freyjaMaterialAmbient(long materialIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <ambient> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialDiffuse")]
	public extern static void freyjaMaterialDiffuse(long materialIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <diffuse> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialSpecular")]
	public extern static void freyjaMaterialSpecular(long materialIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <specular> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialEmissive")]
	public extern static void freyjaMaterialEmissive(long materialIndex, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <emissive> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialShininess")]
	public extern static void freyjaMaterialShininess(long materialIndex, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's Specular <exponent> is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialTransparency")]
	public extern static void freyjaMaterialTransparency(long materialIndex, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialBlendSource")]
	public extern static void freyjaMaterialBlendSource(long materialIndex, ulong factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend source <factor> is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialBlendDestination")]
	public extern static void freyjaMaterialBlendDestination(long materialIndex,
										ulong factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend destination <factor> is set
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Faster access API for modeler use
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetVertexXYZ")]
	public extern static vec3_t *freyjaGetVertexXYZ(long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaGetVertexUV")]
	public extern static vec2_t *freyjaGetVertexUV(long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaGetTexCoordUV")]
	public extern static vec2_t *freyjaGetTexCoordUV(long texcoordIndex);



	///////////////////////////////////////////////////////////////////////
	// Plugin import/export iteraction setup
	///////////////////////////////////////////////////////////////////////

	public enum freyja_plugin_options_t
	{	
		FREYJA_PLUGIN_NONE            = 0,
		FREYJA_PLUGIN_MESH            = 1,
		FREYJA_PLUGIN_SKELETON        = 2,
		FREYJA_PLUGIN_VERTEX_MORPHING = 4,
		FREYJA_PLUGIN_VERTEX_BLENDING = 8
	} 


	[DllImport(libname, EntryPoint="freyjaPluginBegin")]
	public extern static void freyjaPluginBegin();

	[DllImport(libname, EntryPoint="freyjaPluginDescription1s")]
	public extern static void freyjaPluginDescription1s(char *info_line);

	[DllImport(libname, EntryPoint="freyjaPluginAddExtention1s")]
	public extern static void freyjaPluginAddExtention1s(char *ext);

	[DllImport(libname, EntryPoint="freyjaPluginImport1i")]
	public extern static void freyjaPluginImport1i(long flags);

	[DllImport(libname, EntryPoint="freyjaPluginExport1i")]
	public extern static void freyjaPluginExport1i(long flags);

	[DllImport(libname, EntryPoint="freyjaPluginArg1i")]
	public extern static void freyjaPluginArg1i(char *name, long defaults);

	[DllImport(libname, EntryPoint="freyjaPluginArg1f")]
	public extern static void freyjaPluginArg1f(char *name, float defaults);

	[DllImport(libname, EntryPoint="freyjaPluginArg1s")]
	public extern static void freyjaPluginArg1s(char *name, char *defaults);

	[DllImport(libname, EntryPoint="freyjaPluginEnd")]
	public extern static void freyjaPluginEnd();


	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetPluginId")]
	public extern static long freyjaGetPluginId();

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1f")]
	public extern static int freyjaGetPluginArg1f(long pluginId, char *name, float *arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1i")]
	public extern static int freyjaGetPluginArg1i(long pluginId, char *name, long *arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1s")]
	public extern static int freyjaGetPluginArg1s(long pluginId, char *name, char **arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArgString")]
	public extern static int freyjaGetPluginArgString(long pluginId, char *name, 
								 long len, char *arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaPakBegin")]
	public extern static long freyjaPakBegin(char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts a new VFS from a 'pak file'
	 *
	 *        Returns vfs index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPakAddDecoderFunction2s")]
	public extern static void freyjaPakAddDecoderFunction2s(char *module, char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used to decrypt or uncompress files in a pak
	 *        using an external module and one of it's
	 *        C accessable functions using freyjaPak ABI.
	 *
	 *        NOT IMPLEMENTED!
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPakAddFullPathFile")]
	public extern static long freyjaPakAddFullPathFile(long pakIndex,
								  char *vfsFilename,
								  long offset, long size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds a new entry to VFS mapping a chunk from
	 *        offset to offset+size as a file named vfsFilename
	 *
	 *        Returns vfs file entry index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaPakEnd")]
	public extern static void freyjaPakEnd(long pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaCheckModel")]
	public extern static long freyjaCheckModel(char *filename);

	[DllImport(libname, EntryPoint="freyjaLoadModel")]
	public extern static long freyjaLoadModel(char *filename);

	[DllImport(libname, EntryPoint="freyjaSaveModel")]
	public extern static long freyjaSaveModel(char *filename);


	///////////////////////////////////////////////////////////////////////
	// Internal ABI calls
	//
	//   If used externally you'll likely get a lot of breakage or
	//   slower and possibly incorrect object states.
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyja__MeshUpdateMappings")]
	public extern static void freyja__MeshUpdateMappings(long meshIndex);
	/*------------------------------------------------------
	 * Pre  : Only use this if you're a core developer writing
	 *        special test plugins, or internal code.
	 *
	 * Post : Updates Egg backend egg_mesh_t to simulate
	 *        FreyjaMesh local vertex mappings
	 ------------------------------------------------------*/



/* Mongoose 2004.12.19, 
 * C++ fun */


//Vector<uint> *freyjaFindVerticesByBox(vec3_t bbox[2]);

//void freyjaGetVertexPolygonRef1i(long vertexIndex, Vector<long> &polygons);
//void freyjaGetVertexPolygonRef(Vector<long> &polygons);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns a list of indices of polygons that
 *        use current vertex in iterator
 *
 *        List can be empty
 *
 *-- History ------------------------------------------
 *
 * 2004.12.17:
 * Mongoose - Created, wrapper for old Egg style
 *            reverse reference system ( very handy )
 ------------------------------------------------------*/
#if UNIT_TEST_LIBFREYJA
	static void Main()
	{
		freyjaBegin(FREYJA_MESH);
		freyjaEnd();

		Console.WriteLine ("Double up: {0}", DoubleUp (4));
	}
#endif

}
#endif




