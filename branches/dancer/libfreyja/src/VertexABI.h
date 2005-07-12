/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the vertex data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAVERTEXABI_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAVERTEXABI_H_

#include "freyja.h"

extern "C" {

	//index_t freyjaGetCurrentVertex();

	//void freyjaCurrentVertex(index_t vertexIndex);


	/* FREYJA_VERTEX Accessors */

	byte freyjaIsVertexAllocated(index_t vertexIndex);

	index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element);

	uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex);

	void freyjaGetVertexTexCoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaGetVertexNormal3fv(index_t vertexIndex, vec3_t xyz);

	void freyjaGetVertexPosition3fv(index_t vertexIndex, vec3_t xyz);

	/* VertexFrame calls are moving to new AnimationMesh API */
	void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
							  index_t *frameIndex, vec3_t xyz);
	uint32 freyjaGetVertexFrameCount(index_t vertexIndex);

	void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
							   index_t *bone, vec_t *weight);

	uint32 freyjaGetVertexWeightCount(index_t vertexIndex);

	byte freyjaGetVertexFlags(index_t vertexIndex);


	/* FREYJA_VERTEX Mutators */

	index_t freyjaVertexCreate3fv(const vec3_t xyz);

	index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z);

	void freyjaVertexDelete(index_t vertexIndex);

	index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB);

	void freyjaVertexPosition3fv(index_t vertexIndex, const vec3_t xyz);

	void freyjaVertexNormalFlip(index_t vertexIndex);

	void freyjaVertexTexCoord2f(index_t vertexIndex, vec_t u, vec_t v);

	void freyjaVertexTexCoord2fv(index_t vertexIndex, const vec2_t uv);

	void freyjaVertexNormal3f(index_t vertexIndex, vec_t x, vec_t y, vec_t z);

	void freyjaVertexNormal3fv(index_t vertexIndex, const vec3_t xyz);

	void freyjaVertexAddWeight(index_t vertexIndex, vec_t weight, index_t bone);
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
	 ------------------------------------------------------*/

}

#endif
