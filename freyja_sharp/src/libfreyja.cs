// created on 4/13/2005 at 12:47 AM

#define USING_LIBFREYJA

using System;
using System.Runtime.InteropServices;


#if USING_LIBFREYJA
namespace Freyja
{
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


	//using float = System.Double;


public class LibFreyja 
{
	const string libname = "freyja";

	const uint fPolygon_VertexUV     = 2;
	const uint fPolygon_PolyMappedUV = 8;
	const uint fPolygon_PolyMapped   = 8;  /* Alias for PolyMappedUV */
	const uint fPolygon_ColorMapped  = 16;


	public LibFreyja()
	{
	}
	
	public static void spawn()
	{
		/* Mono seems to require these to preload? */
		Console.WriteLine("* libc test: username is " + getenv("USER"));
		Console.WriteLine("* libhel0 test: 90 -> {0}", deg2rad(90));
		
		freyjaSpawn();
	} 
	
	public static void kill()
	{
		freyjaKill();
	}


	[DllImport("hel0", EntryPoint="_Z11helDegToRadf")]
	public extern static float deg2rad(float d);

	[DllImport("libc", EntryPoint="getenv")]
	public extern static string getenv(string var);
		
	[DllImport(libname)]
	public extern static void freyjaSpawn();
	
	[DllImport(libname)]
	public extern static void freyjaKill();
	
	
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
	public extern static int freyjaGetTextureFilename(uint index, out string filename);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <filename> on return
	 *
	 * Post : Gets a pointer to the filename of texture[index]
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetTextureImage")]
	public extern static int freyjaGetTextureImage(uint index,
								ref uint w, ref uint h, 
							 	ref uint depth,  ref uint type,
							 	ref byte []image);
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
	public extern static long freyjaGetBoneName(long boneIndex, uint size, string name);
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
	public extern static long freyjaGetBoneRotationWXYZ4fv(long boneIndex, 
				[MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float [] wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneRotationXYZ3fv")]
	public extern static long freyjaGetBoneRotationXYZ3fv(long boneIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetBoneTranslation3fv")]
	public extern static long freyjaGetBoneTranslation3fv(long index, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s position
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/


	// FREYJA_TEXCOORD Accessors //////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetTexCoord2fv")]
	public extern static void freyjaGetTexCoord2fv(long texcoordIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);
	/*------------------------------------------------------
	 * Pre  : texcoord[index] exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/


	// FREYJA_VERTEX Accessors ////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetVertexTexCoord2fv")]
	public extern static void freyjaGetVertexTexCoord2fv([MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);
	[DllImport(libname, EntryPoint="freyjaGetVertexNormal3fv")]
	public extern static void freyjaGetVertexNormal3fv([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertex3fv")]
	public extern static void freyjaGetVertex3fv([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Sets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 *
	 *        This is deprecated
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetVertexTexCoordUV2fv")]
	public extern static long freyjaGetVertexTexCoordUV2fv(long vertexIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);
	[DllImport(libname, EntryPoint="freyjaGetVertexNormalXYZ3fv")]
	public extern static long freyjaGetVertexNormalXYZ3fv(long vertexIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  nxyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexXYZ3fv")]
	public extern static long freyjaGetVertexXYZ3fv(long vertexIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexFrame")]
	public extern static long freyjaGetVertexFrame(long vertexIndex, long element,
							  ref int frameIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	[DllImport(libname, EntryPoint="freyjaGetVertexFrameCount")]
	public extern static long freyjaGetVertexFrameCount(long vertexIndex);
	[DllImport(libname, EntryPoint="freyjaGetVertexWeight")]
	public extern static long freyjaGetVertexWeight(long vertexIndex, long element,
							   ref long bone, ref float weight);
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
	//int freyjaGetMeshBoundingBox(long meshIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)] float []  min, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)] float []  max);
	[DllImport(libname, EntryPoint="freyjaGetMeshFlags")]
	public extern static long freyjaGetMeshFlags(long meshIndex);
	[DllImport(libname, EntryPoint="freyjaGetMeshPosition")]
	public extern static long freyjaGetMeshPosition(long meshIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	[DllImport(libname, EntryPoint="freyjaGetMeshNameString")]
	public extern static string freyjaGetMeshNameString(long meshIndex); // don't alter string
	[DllImport(libname, EntryPoint="freyjaGetMeshName1s")]
	public extern static long freyjaGetMeshName1s(long meshIndex, long lenght, string name);

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
	public extern static long freyjaGetPolygon1u(freyja_object_t type, long item, ref long value);
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
	public extern static long freyjaGetPolygon3f(freyja_object_t type, long item, ref float value);
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
//FIXME	public extern static void freyjaPrintError(const string format, ...);
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
//FIXME		public extern static void freyjaPrintMessage(const string format, ...);
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
	public extern static void freyjaSetNormal3f(uint index, float x, float y, float z);
[DllImport(libname, EntryPoint="freyjaSetNormal3fv")]
	public extern static void freyjaSetNormal3fv(uint index, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);

	[DllImport(libname, EntryPoint="freyjaSetTexCoord2f")]
	public extern static void freyjaSetTexCoord2f(uint index, float u, float v);

	[DllImport(libname, EntryPoint="freyjaSetTexCoord2fv")]
	public extern static void freyjaSetTexCoord2fv(uint index, [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);

	[DllImport(libname, EntryPoint="freyjaSetVertex3f")]
	public extern static void freyjaSetVertex3f(uint index, float x, float y, float z);

//	[DllImport(libname, EntryPoint="freyjaSetVertex3fv")]
//	public extern static void freyjaSetVertex3fv(uint index, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)] float []  xyz);
				
	[DllImport(libname, EntryPoint="freyjaNormal3f")]
	public extern static long freyjaNormal3f(float x, float y, float z);
	
	[DllImport(libname, EntryPoint="freyjaNormal3fv")]
	public extern static long freyjaNormal3fv([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
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
	public extern static long freyjaTexCoord2f(float u, float v);
	
	[DllImport(libname, EntryPoint="freyjaTexCoord2fv")]
	public extern static long freyjaTexCoord2fv([MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);
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
	public extern static void freyjaVertexTexCoord2f(long index, float u, float v);
[DllImport(libname, EntryPoint="freyjaVertexTexCoord2fv")]
	public extern static void freyjaVertexTexCoord2fv(long index, [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
				float []  uv);
[DllImport(libname, EntryPoint="freyjaVertexNormal3f")]
	public extern static void freyjaVertexNormal3f(long index, float nx, float ny, float nz);
[DllImport(libname, EntryPoint="freyjaVertexNormal3fv")]
	public extern static void freyjaVertexNormal3fv(long index, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  nxyz);
[DllImport(libname, EntryPoint="freyjaVertex3f")]
	public extern static long freyjaVertex3f(float x, float y, float z);
[DllImport(libname, EntryPoint="freyjaVertexXYZ3fv")]
	public extern static long freyjaVertexXYZ3fv(long vertexIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
[DllImport(libname, EntryPoint="freyjaVertex3fv")]
	public extern static long freyjaVertex3fv([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
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
	public extern static void freyjaVertexWeight(long index, float weight, long bone);
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
	public extern static void freyjaMeshTreeAddFrame(float x, float y, float z);
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
	public extern static long freyjaTextureFilename1s(string filename);

[DllImport(libname, EntryPoint="freyjaTextureStoreBuffer")]
	public extern static long freyjaTextureStoreBuffer(byte [] image, uint depth,
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
	public extern static void freyjaBoneName1s(long boneIndex, string name);
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
	public extern static void freyjaBoneTranslate3f(long boneIndex, float x, float y, float z);
[DllImport(libname, EntryPoint="freyjaBoneTranslate3fv")]
	public extern static void freyjaBoneTranslate3fv(long boneIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
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
	public extern static void freyjaBoneRotateEulerXYZ3f(long boneIndex, float x, float y, float z);
[DllImport(libname, EntryPoint="freyjaBoneRotateEulerXYZ3fv")]
	public extern static void freyjaBoneRotateEulerXYZ3fv(long boneIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
[DllImport(libname, EntryPoint="freyjaBoneRotateQuatWXYZ4f")]
	public extern static void freyjaBoneRotateQuatWXYZ4f(long boneIndex,float w,float x,float y,float z);
[DllImport(libname, EntryPoint="freyjaBoneRotateQuatWXYZ4fv")]
	public extern static void freyjaBoneRotateQuatWXYZ4fv(long boneIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  wxyz);
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
	public extern static void freyjaGroupCenter3f(float x, float y, float z);

[DllImport(libname, EntryPoint="freyjaGenerateQuadPlaneMesh")]
	public extern static void freyjaGenerateQuadPlaneMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float side);

[DllImport(libname, EntryPoint="freyjaGenerateQuadCubeMesh")]
	public extern static void freyjaGenerateQuadCubeMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float side);

[DllImport(libname, EntryPoint="freyjaGenerateCircleMesh")]
	public extern static void freyjaGenerateCircleMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, long count); // radius

[DllImport(libname, EntryPoint="freyjaGenerateConeMesh")]
	public extern static void freyjaGenerateConeMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float height, long count); // radius

[DllImport(libname, EntryPoint="freyjaGenerateCylinderMesh")]
	public extern static void freyjaGenerateCylinderMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float height, 
								long count, long segments); // radius

[DllImport(libname, EntryPoint="freyjaGenerateSphereMesh")]
	public extern static void freyjaGenerateSphereMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float radius, 
							  long count, long segments);

[DllImport(libname, EntryPoint="freyjaGenerateTubeMesh")]
	public extern static void freyjaGenerateTubeMesh([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  origin, float height, 
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
	public extern static long freyjaMeshPosition(long meshIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);

[DllImport(libname, EntryPoint="freyjaMeshName1s")]
	public extern static void freyjaMeshName1s(long meshIndex, string name);

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
	public extern static void freyjaGenerateUVFromXYZ([MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz, ref float u, ref float v);

	[DllImport(libname, EntryPoint="freyjaVertexFrame3f")]
	public extern static void freyjaVertexFrame3f(long index, float x, float y, float z);


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
	public extern static int freyjaPolygonExtrudeQuad1f(long polygonIndex, float dist);
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
	public extern static int freyjaPolygonExtrudeQuad(long polygonIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  normal);
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
								    string name, long boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Creates a new Freyja Animation Bone object
	 *        Returns the new Bone's index or -1 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationBoneKeyFrameCreate")]
	public extern static long freyjaAnimationBoneKeyFrameCreate(long animationIndex,
										   long boneIndex,
										   float time, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  wxyz);
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
	public extern static void freyjaAnimationName(long animationIndex,  string name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationBoneName")]
	public extern static void freyjaAnimationBoneName(long animationIndex, long boneIndex,
								  string name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationFrameRate")]
	public extern static void freyjaAnimationFrameRate(long animationIndex, float frameRate);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationTime")]
	public extern static void freyjaAnimationTime(long animationIndex, float time);
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
									 long keyFrameIndex, float time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets time for <keyFrameIndex> keyframe in animation
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFramePosition")]
	public extern static void freyjaAnimationKeyFramePosition(long animationIndex, long boneIndex, 
										 long keyFrameIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  position);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's position in animation
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFrameOrientationXYZ")]
	public extern static void freyjaAnimationKeyFrameOrientationXYZ(long animationIndex,
											   long boneIndex, 
											   long keyFrameIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
				float []  xyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Euler angles
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaAnimationKeyFrameOrientationWXYZ")]
	public extern static void freyjaAnimationKeyFrameOrientationWXYZ(long animationIndex,
												long boneIndex,
												long keyFrameIndex,[MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  wxyz);
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
	public extern static string freyjaGetMaterialName(long materialIndex);
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
	public extern static void freyjaGetMaterialAmbient(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialDiffuse")]
	public extern static void freyjaGetMaterialDiffuse(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialSpecular")]
	public extern static void freyjaGetMaterialSpecular(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialEmissive")]
	public extern static void freyjaGetMaterialEmissive(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <emissive> color
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialShininess")]
	public extern static float freyjaGetMaterialShininess(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns specular exponent or -1.0 on error
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaGetMaterialTransparency")]
	public extern static float freyjaGetMaterialTransparency(long materialIndex);
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
	public extern static float freyjaGetMaterialBlendDestination(long materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend destination factor or -1 on error
	 ------------------------------------------------------*/


	/* Material Mutators */

	[DllImport(libname, EntryPoint="freyjaMaterialName")]
	public extern static void freyjaMaterialName(long materialIndex, string name);
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
	public extern static void freyjaMaterialTextureFilename(long materialIndex, string filename);
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
	public extern static void freyjaMaterialAmbient(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <ambient> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialDiffuse")]
	public extern static void freyjaMaterialDiffuse(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <diffuse> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialSpecular")]
	public extern static void freyjaMaterialSpecular(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <specular> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialEmissive")]
	public extern static void freyjaMaterialEmissive(long materialIndex, [MarshalAs (UnmanagedType.LPArray, SizeConst=4)]
				float []  emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <emissive> color is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialShininess")]
	public extern static void freyjaMaterialShininess(long materialIndex, float exponent);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's Specular <exponent> is set
	 ------------------------------------------------------*/

	[DllImport(libname, EntryPoint="freyjaMaterialTransparency")]
	public extern static void freyjaMaterialTransparency(long materialIndex, float transparency);
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

	//[DllImport(libname, EntryPoint="freyjaGetVertexXYZ")]
	//public extern static [MarshalAs (UnmanagedType.LPArray, SizeConst=3)]
	//			ref float [] freyjaGetVertexXYZ(long vertexIndex);
	//[DllImport(libname, EntryPoint="freyjaGetVertexUV")]
	//public extern static [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
	//			ref float []  freyjaGetVertexUV(long vertexIndex);
	//[DllImport(libname, EntryPoint="freyjaGetTexCoordUV")]
	//public extern static [MarshalAs (UnmanagedType.LPArray, SizeConst=2)]
	//			ref float []  freyjaGetTexCoordUV(long texcoordIndex);


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
	public extern static void freyjaPluginDescription1s(string info_line);

	[DllImport(libname, EntryPoint="freyjaPluginAddExtention1s")]
	public extern static void freyjaPluginAddExtention1s(string ext);

	[DllImport(libname, EntryPoint="freyjaPluginImport1i")]
	public extern static void freyjaPluginImport1i(long flags);

	[DllImport(libname, EntryPoint="freyjaPluginExport1i")]
	public extern static void freyjaPluginExport1i(long flags);

	[DllImport(libname, EntryPoint="freyjaPluginArg1i")]
	public extern static void freyjaPluginArg1i(string name, long defaults);

	[DllImport(libname, EntryPoint="freyjaPluginArg1f")]
	public extern static void freyjaPluginArg1f(string name, float defaults);

	[DllImport(libname, EntryPoint="freyjaPluginArg1s")]
	public extern static void freyjaPluginArg1s(string name, string defaults);

	[DllImport(libname, EntryPoint="freyjaPluginEnd")]
	public extern static void freyjaPluginEnd();


	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaGetPluginId")]
	public extern static long freyjaGetPluginId();

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1f")]
	public extern static int freyjaGetPluginArg1f(long pluginId, string name, ref float arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1i")]
	public extern static int freyjaGetPluginArg1i(long pluginId, string name, ref long arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArg1s")]
	public extern static int freyjaGetPluginArg1s(long pluginId, string name, ref string arg);

	[DllImport(libname, EntryPoint="freyjaGetPluginArgString")]
	public extern static int freyjaGetPluginArgString(long pluginId, string name, 
								 long len, string arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	[DllImport(libname, EntryPoint="freyjaPakBegin")]
	public extern static long freyjaPakBegin(string filename);
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
	public extern static void freyjaPakAddDecoderFunction2s(string module, string symbol);
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
								  string vfsFilename,
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
	public extern static int freyjaCheckModel(string filename);

	[DllImport(libname, EntryPoint="freyjaLoadModel")]
	public extern static int freyjaLoadModel(string filename);

	[DllImport(libname, EntryPoint="freyjaSaveModel")]
	public extern static int freyjaSaveModel(string filename);



#if UNIT_TEST_LIBFREYJA
	static void Main()
	{
		freyjaBegin(FREYJA_MESH);
		freyjaEnd();

		Console.WriteLine ("Double up: {0}", DoubleUp (4));
	}
#endif

}
}
#endif




