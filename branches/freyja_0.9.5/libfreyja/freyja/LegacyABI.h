/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the mesh ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_LEGACYABI_H_
#define GUARD__FREYJA_LEGACYABI_H_

#include "freyja.h"

extern "C" {

	//////////////////////////////////////////////////////////////////////////
	// Legacy ABI methods. 
	//
	// Legacy calls are often dependent on a gobal index to work.
	// They are deprecated as of 0.9.5, since public 0.10.x will be threaded.
	//////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	// Freyja Iterator functions
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaBegin(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : A new object of type is constructed.
	 ------------------------------------------------------*/

	void freyjaEnd();
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(...) is called to start the state
	 * Post : The object is finialized and construction ends
	 ------------------------------------------------------*/

	index_t freyjaCriticalSectionLock();
	void freyjaCriticalSectionUnlock(index_t lockId);
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin singleton exists
	 *
	 * Post : If FreyjaPlugin is locked this will block
	 *        in the future. For now it's just to test if
	 *        the singleton is up and running.
	 ------------------------------------------------------*/

	index_t freyjaIterator(freyja_object_t type, index_t item);
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

	uint32 freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns index of current internal complex type
	 *        ( Last constructed or iterated )
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertexgroup
	 * Post : Sets vertexgroups[FREYJA_LIST_CURRENT]'s
	 *        pivot / center
	 ------------------------------------------------------*/

	void freyjaPolygonVertex1i(index_t index);
	void freyjaPolygonTexCoord1i(index_t index);
	void freyjaPolygonMaterial1i(index_t id);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a polygon
	 *
	 * Post : Appends/sets polygon[FREYJA_LIST_CURRENT]'s
	 *        vertices, texcoord, or material
	 ------------------------------------------------------*/

	void freyjaGetVertexTexCoord2fv(vec2_t uv);
	void freyjaGetVertexNormal3fv(vec3_t xyz);
	void freyjaGetVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Gets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 ------------------------------------------------------*/

	index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z);
	index_t freyjaVertexCreate3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Texcoords
	//
	////////////////////////////////////////////////////////////////

	char freyjaIsTexCoordAllocated(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if texcoord exists
	 ------------------------------------------------------*/

	index_t freyjaTexCoordCreate2f(vec_t u, vec_t v);
	index_t freyjaTexCoordCreate2fv(const vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : u, v are clamped/scaled to 0.0f to 1.0f
	 * Post : A new texcoord is created
	 *        Returns the native index of that texcoord
	 ------------------------------------------------------*/

	vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/

	void freyjaTexCoordCombine(uint32 A, uint32 B);
	/*------------------------------------------------------
	 * Pre  : texcoord indices A and B exist
	 * Post : replaces all references to B with A
	 ------------------------------------------------------*/

	void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Alters the texcoord's values
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Vertices
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB);

	void freyjaVertexDelete(index_t vertexIndex);

	index_t freyjaGetCurrentVertex();

	void freyjaCurrentVertex(index_t vertexIndex);

	char freyjaIsVertexAllocated(index_t vertexIndex);

	void freyjaVertexFrame3f(index_t vertexIndex, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends a new mesh animation frame to this vertex
	 ------------------------------------------------------*/

	void freyjaVertexNormalFlip(index_t vertexIndex);

	void freyjaVertexTexcoord2f(index_t vertexIndex, vec_t u, vec_t v);

	void freyjaVertexTexCoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaVertexNormal3f(index_t vertexIndex, vec_t nx, vec_t ny, vec_t nz);

	void freyjaVertexNormal3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaVertexPosition3fv(index_t vertexIndex, vec3_t xyz);



	index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element);

	uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex);

	void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaGetVertexXYZ3fv(index_t vertexIndex, vec3_t xyz);

	void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
							  index_t *frameIndex, vec3_t xyz);

	uint32 freyjaGetVertexFrameCount(index_t vertexIndex);

	uint32 freyjaGetVertexFlags(index_t vertexIndex);

	vec3_t *freyjaGetVertexXYZ(index_t vertexIndex);

	vec2_t *freyjaGetVertexUV(index_t vertexIndex);



	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPolygonCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes invalid/empty polygon to be filled, returns index
	 ------------------------------------------------------*/

	void freyjaPolygonDelete(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes polygon 
	 ------------------------------------------------------*/

	char freyjaIsPolygonAllocated(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if exists
	 ------------------------------------------------------*/

	void freyjaPolygonFlagAlpha(index_t polygonIndex, char on);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	void freyjaPolygonTexCoordPurge(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by face normal scaled by dist
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t v);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal' v ( or partial spline or ray )
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 ------------------------------------------------------*/

	index_t freyjaGetPolygonMaterial(index_t polygonIndex);

	uint32 freyjaGetPolygonFlags(index_t polygonIndex);

	uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex);

	uint32 freyjaGetPolygonVertexCount(index_t polygonIndex);

	uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex);

	index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element);

	index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element);

	index_t freyjaGetCurrentModelIndex();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaModelCreateMesh(index_t model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelClampTexCoords(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clamps UVs and TexCoords to 0.0 to 1.0
	 ------------------------------------------------------*/

	void freyjaModelGenerateVertexNormals(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelMeshTransform3fv(index_t model, index_t mesh, 
									 freyja_transform_action_t action,vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaModelMeshPolygonCreate(index_t model, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	
	void freyjaModelMeshPolygonAddTexCoord1i(index_t modelIndex, index_t meshIndex, 
											 index_t polygonIndex, index_t texcoordIndex);

	void freyjaModelMeshPolygonAddVertex1i(index_t modelIndex, index_t meshIndex, 
										   index_t polygonIndex, index_t vertexIndex);

	index_t freyjaGetFSMMeshIndex();
	/*------------------------------------------------------
	 * Pre  : Gobal index is locked
	 * Post : Returns mesh index state in FSM
	 ------------------------------------------------------*/
	
	index_t freyjaGetCurrentMesh();
	/*------------------------------------------------------
	 * Pre  : Gobal index is locked
	 * Post : Returns gobal mesh index state 
	 ------------------------------------------------------*/

	void freyjaCurrentMesh(index_t idx);
	/*------------------------------------------------------
	 * Pre  : Gobal index is locked
	 * Post : Sets index, which is useful for 0.9.3 usage 
	 ------------------------------------------------------*/

	// Historical note -- it's amazing how long that's been mispelled
	// good thing it's deprecated now =)
	void freyjaMeshPromoteTexcoordsToPloymapping(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : mesh references a valid mesh
	 * Post : Takes vertex UVs and promotes them to polymapped
	 *        texcoords. 
	 *
	 *        This means texcoords are stored polygon relative
	 *        aka texture polygons.  
	 *
	 *        Freyja still supports UV wedges and maps natively.
	 ------------------------------------------------------*/

	index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSplitTexCoords(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All TexCoords are duplicated, then references to old
	 *        TexCoords are removed -- this is useful for making a single
	 *        polymapped texture polygon when all others will remain
	 *        in a UV Mesh
	 ------------------------------------------------------*/


	void freyjaGetMeshFrameBoundingBox(index_t meshIndex, 
										uint32 frame, vec3_t min, vec3_t max);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t freyjaMeshVertexCreate3f(index_t mesh, index_t group,
									 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns mesh local index of vertex created.
	 *
	 *        After 0.9.3 API vertex grouping was removed, but
	 *        we keep the same API.  Groups might come back
	 *        for use in special subdiv algorithms or the like.
	 ------------------------------------------------------*/

	void freyjaMeshAddPolygon(index_t mesh, index_t polygon);
	/*------------------------------------------------------
	 * Pre  : Legacy ABI when faces were gobal objects.
	 * Post : 
	 ------------------------------------------------------*/


	void freyjaMeshFrameCenter(index_t mesh, uint32 frame, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMeshFrameCenter(index_t mesh, uint32 frame, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshVertexTranslate3fv(index_t mesh, 
									  index_t vertex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets vertex position.  
	 *
	 *        Doesn't actually translate!  
	 *        Legacy API naming, since all mesh vertices 
	 *        are now stored in world coordinates.
	 ------------------------------------------------------*/

	void freyjaMeshFrameTransform(index_t mesh, uint32 frame,
								  freyja_transform_action_t action, 
								  vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshTransform(index_t mesh, uint32 frame,
							 freyja_transform_action_t action, 
							 vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generic transform function.
	 ------------------------------------------------------*/

	void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
							   index_t *bone, vec_t *weight);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Simulated call that iterates weights to find
	 *        matching <vertexIndex>:<element> data (<bone>, <weight>)
	 ------------------------------------------------------*/

	uint32 freyjaGetVertexWeightCount(index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Simulated call that iterates weights to ref count
	 ------------------------------------------------------*/

	void freyjaVertexWeight(index_t vertex, vec_t weight, index_t bone);
	/*------------------------------------------------------------
	 * Pre  : <weight> of influence of <bone> on <vertex>
	 *
	 * Post : <vertex> in the model gets weight added	
	 *        to influence list, if there is a weight for the
	 *        corresponding bone it is replaced
	 *
	 *        <weight> <= 0.0 removes weight	
	 *
	 *        All weights for the vertex combined must be 1.0
	 -----------------------------------------------------------*/

	void freyjaTexCoordCombine(uint32 a, uint32 b);
	/*------------------------------------------------------
	 * Pre  : Texcoords <a> and <b> exist
	 * Post : Replaces all references to <b> with <a>
	 ------------------------------------------------------*/

	index_t freyjaVertexCombine(index_t a, index_t b);
	/*------------------------------------------------------
	 * Pre  : Vertices <a> and <b> exist
	 * Post : Replaces all references to <b> with <a>
	 ------------------------------------------------------*/

	void freyjaPolygonGroup1u(uint32 group);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side);

	void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side);

	void freyjaGenerateCircleMesh(vec3_t origin, int32 count);

	void freyjaGenerateConeMesh(vec3_t origin, vec_t height, int32 count);

	void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
								  int32 count, int32 segments);

	void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
									int32 count, int32 segments);

	void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
								int32 count, int32 segments);
}

#   if defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )
#      include <mstl/Vector.h>
#      include <mstl/SystemIO.h>

#      if FREYJA_OBSOLETE_ABI
void freyjaVertexListTransform(mstl::Vector<uint32> &list,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

void freyjaModelMirrorTexCoord(uint32 modelIndex, uint32 texCoordIndex,
							   mstl::Vector<int32> uvMap, bool x, bool y);

char freyjaModelCopyVertexList(index_t modelIndex, 
							   mstl::Vector<unsigned int> &list,
							   int mesh, int frame);

int32 freyjaFindPolygonByVertices(mstl::Vector<uint32> vertices);

mstl::Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2]);

void freyjaGetVertexPolygonRef1i(index_t vertexIndex, mstl::Vector<long> &polygons);
void freyjaGetVertexPolygonRef(mstl::Vector<long> &polygons);


#      endif // FREYJA_OBSOLETE_ABI


int32 freyjaMeshSaveChunkJA(mstl::SystemIO::FileWriter &w, index_t mesh);
/*------------------------------------------------------
 * Pre  : 
 * Post : Serialiser for JA format mesh chunk
 ------------------------------------------------------*/


int32 freyjaBoneSaveChunkJA(mstl::SystemIO::FileWriter &w, index_t bone);
/*------------------------------------------------------
 * Pre  : 
 * Post : Serialiser for JA format bone chunk
 ------------------------------------------------------*/


#ifdef OBSOLETE

class InterfaceTrack
{
	////////////////////////////////////////////////////////////
	// This class is the base for keyframe 'typing' aggregate.
	// Notice to add a new kind of transform it must be added 
	// here first.  This helps encourage more uniform extension.
	// However feel free to use the interface however works best.
	//
	// The methods here should drive your UI / curve control design.
	// By providing commonly used curve transforms here, you shouldn't
	// have to hack around anything.  
	//
	////////////////////////////////////////////////////////////

 public:

	InterfaceTrack() { }

	virtual ~InterfaceTrack() { }

	// Might want to put 'tmp/edit' keyframe support in here instead of UI
	// for use when creating a keyframe using auto

	// 'Location' interface
	virtual void SetPosition(vec3_t xyz) {}
	virtual void SetPositionX(float x) {}
	virtual void SetPositionY(float y) {}
	virtual void SetPositionZ(float z) {}
	virtual void SetDeltaPosition(vec3_t xyz) {}
	virtual void SetDeltaPositionX(float x) {}
	virtual void SetDeltaPositionY(float y) {}
	virtual void SetDeltaPositionZ(float z) {}

	// 'Rotation' Quaternion interface 
	virtual void SetRotationQuat(vec4_t wxyz) {}
	virtual void SetDeltaRotationQuat(vec4_t wxyz) {}

	// 'Rotation'Euler angle interface 
	virtual void SetRotationEuler(vec3_t xyz) {}
	virtual void SetRotationEulerX(float x) {}
	virtual void SetRotationEulerY(float y) {}
	virtual void SetRotationEulerZ(float z) {}
	virtual void SetDeltaRotationEuler(vec3_t xyz) {}	
	virtual void SetDeltaRotationEulerX(float x) {}
	virtual void SetDeltaRotationEulerY(float y) {}
	virtual void SetDeltaRotationEulerZ(float z) {}

	// 'Size' interface
	virtual void SetScale(vec_t s) {}
	virtual void SetScale(vec3_t xyz) {}	
	virtual void SetScaleX(float x) {}
	virtual void SetScaleY(float y) {}
	virtual void SetScaleZ(float z) {}
	virtual void SetDeltaScale(vec3_t xyz) {}	
	virtual void SetDeltaScaleX(float x) {}
	virtual void SetDeltaScaleY(float y) {}
	virtual void SetDeltaScaleZ(float z) {}

	// 'Color' interface
	virtual void SetColorARGB(vec4_t argb) {}
	virtual void SetColorRGB(vec3_t rgb) {}
	virtual void SetColorRGBA(vec4_t rgba) {}
	virtual void SetColorR(vec_t r) {}
	virtual void SetColorB(vec_t b) {}
	virtual void SetColorG(vec_t g) {}
	virtual void SetColorA(vec_t a) {}
	virtual void SetTransparentcy(vec_t t) {}
	virtual void SetVisibility(vec_t v) {}

	// 'Light' interface
	virtual void SetFalloff(vec_t d) {}

};
#endif

#   endif // defined( __cplusplus ) && defined( USING_FREYJA_CPP_ABI )

#endif // GUARD__FREYJA_LEGACYABI_H_
