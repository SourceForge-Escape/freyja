/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===============================================================
 * Project: Mongoose Tool Kit
 * Author : Terry 'Mongoose' Hendrix
 * Website: http://www.westga.edu/~stu7440/
 * Email  : stu7440@westga.edu
 * Object : 
 * Comment: 3d functions.
 * 
 *-- History -----------------------------------------------
 *
 * 2001.07.05:
 * Mongoose - Backport of OpenRaider mtk3d
 *
 * 1999-06-14:
 * Mongoose - Created
 ==============================================================*/

#ifndef GUARD__MONGOOSE_MTK_3D_MTK3D_H
#define GUARD__MONGOOSE_MTK_3D_MTK3D_H


#define MTK_PI           3.14159265358979323846  /* pi */
#define MTK_PI_OVER_2    1.57079632679489661923  /* pi/2 */
#define MTK_2_PI         6.28318530717958647692  /* pi*2 */
#define MTK_PI_OVER_4    0.78539816339744830962  /* pi/4 */

//////////////////////////////////////////////////
// Backwards compatibility                      //
//////////////////////////////////////////////////
#define point_t vec3_t
#define mtkMult mtkMatrixMult
#define mtkCopy mtkMatrixCopy
#define mtkTransform mtkMatrixTransform
#define mtkIdentity mtkMatrixIdentity
#define mtkScale mtkMatrixScale
#define mtkRotate mtkMatrixRotate
#define mtkTranslate mtkMatrixTranslate
//////////////////////////////////////////////////


typedef float vec_t;
typedef vec_t unit_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t bbox_t[3][3];
typedef vec_t matrix_t[4][4];
//typedef unit_t matrix_t[16];  /* Column order */


typedef struct quaternion_s
{
  float w, x, y, z;

} quaternion_t;

void mtkQuaternionToEuler(vec_t x, vec_t y, vec_t z, vec_t w,
						  vec_t *heading, vec_t *bank, vec_t *attitude);

void mtkEulerToQuaternion(vec_t roll, vec_t pitch, vec_t yaw, quaternion_t q);

unit_t mtkDist2d(unit_t x, unit_t y, unit_t x2, unit_t y2);

unit_t mtkDist3d(unit_t x,  unit_t y,  unit_t z,
		 unit_t x2, unit_t y2, unit_t z2);


//////////////////////////////////////////////////

unit_t mtkDegToRad(unit_t degrees);

//////////////////////////////////////////////////

void mtkVectorMatrixMult(double v[4], matrix_t m, double result[4]);
void mtkMatrixMult(matrix_t m1, matrix_t m2, matrix_t mp);
void mtkMatrixCopy(matrix_t source, matrix_t dest);
void mtkMatrixTransform(matrix_t t, vec3_t p);
void mtkMatrixIdentity(matrix_t matrix);
void mtkMatrixScale(matrix_t matrix, unit_t sx, unit_t sy, unit_t sz);
void mtkMatrixRotate(matrix_t matrix, unit_t ax, unit_t ay, unit_t az);
void mtkMatrixTranslate(matrix_t matrix, unit_t tx, unit_t ty, unit_t tz);


//////////////////////////////////////////////////


void mtkQuaternionIdentity(quaternion_t q);
void mtkQuaternionCopy(quaternion_t source, quaternion_t dest);
void mtkQuaternionToMatrix(quaternion_t q, matrix_t m);
void mtkQuaternionSet(quaternion_t q, float angle, float x, float y, float z);
void mtkQuaternionMult(quaternion_t a, quaternion_t b, quaternion_t result);
void mtkQuaternionNormalize(quaternion_t q);

#endif
