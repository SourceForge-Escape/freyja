/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator scripopt.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Centralized various graphics util functions here
 *            Normalized the API to be more consistant
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_

const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float ORANGE[]       = {  1.0,  7.0,  0.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6, 1.0 };


bool mglHardwareExtTest(const char *ext);

void mgl2dProjection(const unsigned int width, const unsigned int height);

void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor);

void mglDrawSelectBox(const vec3_t min, const vec3_t max, const vec4_t color);

void mglDrawAxis(const vec_t min, const vec_t mid, const vec_t max);

void mglDraw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max);

void mglDraw3dCursor(const vec_t min, const vec_t mid, const vec_t max);

void mglDrawSphere(int numMajor, int numMinor, float radius);

void mglDrawBone(unsigned char type, const vec3_t pos);

void mglDrawJoint(unsigned char type, const vec3_t pos);

void mglDrawGrid(vec3_t color, vec_t size, vec_t step, vec_t scale);

void mglGetOpenGLViewport(int *viewportXYWH); // int[4]

void mglGetOpenGLModelviewMatrix(double *modelview); // double[16]

void mglGetOpenGLProjectionMatrix(double *projection); // double[16]

void mglApplyMaterial(uint32 materialIndex);


#endif /* GUARD__FREYJA_MONGOOSE_FREYJAOPENGL_H_ */



