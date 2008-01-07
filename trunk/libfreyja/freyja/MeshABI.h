/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the mesh ABI.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_MESHABI_H_
#define GUARD__FREYJA_MESHABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Mesh 0.10.0 ABI
	//
	// Implements: Node, SceneNode, XMLSerializer
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaMeshCreate( const char* name );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns pointer of mesh spawned.
	 *        The mesh won't be part of any scene by default.
	 ------------------------------------------------------*/

	void freyjaMeshDelete( freyja_ptr mesh );
	/*------------------------------------------------------
	 * Pre  : Mesh exists.
	 * Post : Mesh is deleted and removed from any graphs.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// freyjaMeshGet
	///////////////////////////////////////////////////////////////////////

    freyja_ptr freyjaMeshGetVertex( freyja_ptr mesh, freyja_id vertex ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Vertex with UID or NULL.
	 * Notes: Obsolete, marked for removal.
	 ------------------------------------------------------*/

    freyja_ptr freyjaMeshGetFace( freyja_ptr mesh, freyja_id face ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Face with UID or NULL.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// freyjaMeshGetWeight
	///////////////////////////////////////////////////////////////////////

    freyja_ptr freyjaMeshGetWeight( freyja_ptr mesh, freyja_id weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Weight with UID or NULL.
	 ------------------------------------------------------*/

	uint32 freyjaMeshGetWeightCount(freyja_ptr mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets number of vertex:bone weights in mesh
	 ------------------------------------------------------*/




	///////////////////////////////////////////////////////////////////////
	// freyjaMeshSet
	///////////////////////////////////////////////////////////////////////

	void freyjaMeshSetMaterial( freyja_ptr mesh, freyja_ptr material );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <mesh> material reference to <material>.
	 ------------------------------------------------------*/

	void freyjaMeshSetPosition( freyja_ptr meshIndex, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the mesh position attribute.
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Face ABI
	////////////////////////////////////////////////////////////////

	freyja_ptr freyjaFaceCreate( freyja_ptr mesh );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns face or NULL.
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Mesh utilities ABI
	////////////////////////////////////////////////////////////////

	freyja_ptr freyjaMeshCreateSheet(vec3_t origin, vec_t size, 
									 uint32 rows, uint32 columns);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral mesh sheet is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateLattice(vec3_t origin, vec_t size, 
									   uint32 rows, uint32 columns, uint32 layers);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Quadrilateral lattice mesh is created.
	 *        <layers> tall with <rows> x <cols> of quads div per layer.
	 *
	 *        Useful for quickier extruding.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCube(vec3_t origin, vec_t size);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral mesh cube is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateRing(vec3_t origin, vec_t radius, uint32 count,
									uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point circle mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCircle(vec3_t origin, vec_t radius, uint32 count);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point circle mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCone(vec3_t origin, vec_t height, vec_t radius,
									uint32 wedges);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point cone mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateCylinder(vec3_t origin, vec_t height, vec_t radius, 
										uint32 sides, uint32 rings); 
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Triangular center point capped, 
	 *        quadrilateral cylinder mesh is created.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateSphere(vec3_t origin, vec_t radius, 
									  uint32 sides, uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	freyja_ptr freyjaMeshCreateTube(vec3_t origin, vec_t height, vec_t radius, 
									uint32 sides, uint32 rings);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Quadrilateral cylinder mesh is created.
	 *
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Vertex
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaVertexCreate3fv( freyja_ptr mesh, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vertex or NULL.
	 ------------------------------------------------------*/

	void freyjaVertexGetTexCoord2fv( freyja_ptr vertex, vec2_t uv );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns <mesh> <vertex> texcoord index.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Weight
	///////////////////////////////////////////////////////////////////////

	void freyjaWeightCreate( freyja_ptr skelmesh, 
							 freyja_ptr vertex, freyja_ptr bone, vec_t weight );
	/*------------------------------------------------------------
	 * Pre  : All <weight>s for <vertex> combined must be 1.0
	 *        Each weight must fit 0.0f < w <= 1.0f
	 *
	 * Post : Sets <weight> of influence of <bone> on <vertex> in <mesh>
	 *
	 *        If there is already a weight for the corresponding
	 *        bone it is replaced.
	 *
	 -----------------------------------------------------------*/

	freyja_ptr freyjaWeightGetBone( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute bone.
	 ------------------------------------------------------*/

	vec_t freyjaWeightGetValue( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute weight value.
	 ------------------------------------------------------*/

	freyja_ptr freyjaWeightGetVertex( freyja_ptr weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <weight> attribute vertex.
	 ------------------------------------------------------*/


} // extern "C"

#endif // GUARD__FREYJA_MESHABI_H_
