/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Matrix
 * License : LGPL, (C) 2002-2007 Mongoose
 * Comments: Matrix 4x4 class, non SSE version
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Replaced old algorithms and API.
 *
 * 2003.06.17:
 * Mongoose - Now in column order to match OpenGL user needs.
 *
 * 2002.05.11:
 * Mongoose - Created, based on my mtk3d matrix, C implemnentation.
 ==========================================================================*/

#ifndef GUARD__HEL_MAT44_H_
#define GUARD__HEL_MAT44_H_

#include "hel/math.h"
#include "hel/Quat.h"
#include "hel/Vec3.h"

#include <mstl/String.h>


namespace hel {

class Mat44
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mat44() { SetIdentity(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Matrix
	 *
	 ------------------------------------------------------*/

	Mat44(const Mat44 &m) { memcpy(mMatrix, m.mMatrix, sizeof(matrix_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Matrix
	 *
	 ------------------------------------------------------*/

	Mat44(matrix_t m) { memcpy(mMatrix, m, sizeof(matrix_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Matrix
	 *
	 ------------------------------------------------------*/

	Mat44(const Quat &q) { q.GetMatrix(mMatrix); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Contruct Mat44 from quaternion.
	 *
	 ------------------------------------------------------*/

	~Mat44() { }
	/*------------------------------------------------------
	 * Pre  : Matrix object is allocated
	 * Post : Deconstructs an object of Matrix
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	vec_t Det()
	{
		vec_t *a = mMatrix, *b = mMatrix+4, *c = mMatrix+8, *d = mMatrix+12;
		return (a[0] * Det3x3(b+1, c+1, d+1) - b[0] * Det3x3(a+1, c+1, d+1) +
				c[0] * Det3x3(a+1, b+1, d+1) - d[0] * Det3x3(a+1, b+1, c+1));
	}
	/*------------------------------------------------------
	 * Pre  : Matrix object is allocated
	 * Post : Computes Determinate of this 4x4 matrix.
	 *
	 ------------------------------------------------------*/

	static vec_t Det2x2(const vec2_t a, const vec2_t b)
	{ return a[0] * b[1] - a[1] * b[0];	}
	/*------------------------------------------------------
	 * Pre  : Given a 2x2 matrix with columns [ <a> <b> ]
	 * Post : Computes Determinate of 2x2 matrix.
	 *
	 ------------------------------------------------------*/

	static vec_t Det3x3(const vec3_t a, const vec3_t b, const vec3_t c)
	{
		return (a[0] * Det2x2(b+1, c+1) - b[0] * Det2x2(a+1, c+1) +
				c[0] * Det2x2(a+1, b+1));
	}
	/*------------------------------------------------------
	 * Pre  : Given a 3x3 matrix with columns [ <a> <b> <c> ]
	 * Post : Computes Determinate of 3x3 block.
	 *
	 ------------------------------------------------------*/	

	void GetColumn4fv(const uint32 column, vec4_t v)
	{ memcpy(v, (mMatrix + 4 * column), sizeof(vec4_t)); }
	/*------------------------------------------------------
	 * Pre  : column < 4 
	 * Post : Returns <column>th column.
	 *
	 ------------------------------------------------------*/

	void GetColumn3fv(const uint32 column, const uint32 offset, vec3_t v)
	{ memcpy(v, (mMatrix + offset + 4 * column), sizeof(vec3_t)); }
	/*------------------------------------------------------
	 * Pre  : column < 4, offset < 2 
	 * Post : Returns <offset> section of <column>th column.
	 *
	 ------------------------------------------------------*/

	void GetColumn2fv(const uint32 column, const uint32 offset, vec2_t v)
	{ memcpy(v, (mMatrix + offset + 4 * column), sizeof(vec2_t)); }
	/*------------------------------------------------------
	 * Pre  : column < 4, offset < 3 
	 * Post : Returns <offset> section of <column>th column.
	 *
	 ------------------------------------------------------*/

	bool GetInverse(Mat44 &m) { m = *this; return m.Invert(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if a copy of this matrix inverted.
	 *
	 ------------------------------------------------------*/

	bool GetInverseMatrix(matrix_t m) { return false; }  // FIXME
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if a copy of this matrix inverted.
	 *
	 ------------------------------------------------------*/

	void const GetMatrix(matrix_t m) { memcpy(m, mMatrix, sizeof(matrix_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a copy of this matrix
	 *
	 ------------------------------------------------------*/

	void GetTranspose(Mat44 &m) { m = *this; m.Transpose(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a copy of this matrix transposed
	 *
	 ------------------------------------------------------*/

	static void Invert2x2(vec2_t a, vec2_t b)
	{ 
		const vec_t detInv = 1.0f / Det2x2(a, b);
		a[1] *= -detInv;  b[0] *= -detInv;
		const vec_t tmp = detInv * a[0];  a[0] = detInv * b[1];  b[1] = tmp;
	}
	/*------------------------------------------------------
	 * Pre  : Given a 2x2 matrix with columns [ <a> <b> ]
	 * Post : Computes Determinate of 2x2 matrix.
	 *
	 *        m^-1 = (1/det(m)) [ m11 -m01 -m10 m00 ]
	 *
	 ------------------------------------------------------*/

	bool IsIdentity();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Is this matrix the identity matrix?
	 *
	 ------------------------------------------------------*/

	void Multiply(const Mat44 &a, const Mat44 &b, Mat44 &result)
	{ helMatrixMultiply(a.mMatrix, b.mMatrix, result.mMatrix); }
	/*------------------------------------------------------
	 * Pre  : Multiplies 2 matrices
	 * Post : Returns resultant matrix
	 *
	 ------------------------------------------------------*/

	void MultiplyVertexArray( const uint32 size, vec_t* array ) const
	{
		for ( uint32 i = 0, j = 0; i < size; ++i, j += 3 )
		{
			const vec_t x = array[  j], y = array[j+1], z = array[j+2];
			array[  j] = mMatrix[0]*x + mMatrix[4]*y + mMatrix[ 8]*z + mMatrix[12];
			array[j+1] = mMatrix[1]*x + mMatrix[5]*y + mMatrix[ 9]*z + mMatrix[13];
			array[j+2] = mMatrix[2]*x + mMatrix[6]*y + mMatrix[10]*z + mMatrix[14];
		}
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transforms array by this matrix
	 *
	 ------------------------------------------------------*/

	void Multiply3fv(vec3_t v) const
	{
		vec_t x = v[0], y = v[1], z = v[2];
		v[0] = mMatrix[0]*x + mMatrix[4]*y + mMatrix[ 8]*z + mMatrix[12];
		v[1] = mMatrix[1]*x + mMatrix[5]*y + mMatrix[ 9]*z + mMatrix[13];
		v[2] = mMatrix[2]*x + mMatrix[6]*y + mMatrix[10]*z + mMatrix[14];
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies vector <v> and this matrix.
	 * Post : Returns result in <v> vector.
	 *
	 ------------------------------------------------------*/

	void Multiply3fv(vec3_t v, vec3_t result) const
	{
		memcpy(result, v, sizeof(vec3_t));
		Multiply3fv(result);
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies <V> vector and <This> matrix
	 *
	 * Post : Returns <Result> vector, 
	 *        <V> and <Result> maybe be the same vector
	 *
	 ------------------------------------------------------*/

	void Multiply4fv(vec4_t v)
	{
		vec_t x = v[0], y = v[1], z = v[2], w = v[3];

		v[0] = mMatrix[0]*x + mMatrix[4]*y + mMatrix[ 8]*z + mMatrix[12]*w;
		v[1] = mMatrix[1]*x + mMatrix[5]*y + mMatrix[ 9]*z + mMatrix[13]*w;
		v[2] = mMatrix[2]*x + mMatrix[6]*y + mMatrix[10]*z + mMatrix[14]*w;
		v[3] = mMatrix[3]*x + mMatrix[7]*y + mMatrix[11]*z + mMatrix[15]*w;
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies vector <v> and this matrix.
	 * Post : Returns result in <v> vector.
	 *
	 ------------------------------------------------------*/

	void Multiply4fv(vec4_t v, vec4_t result)
	{
		memcpy(result, v, sizeof(vec4_t));
		Multiply4fv(result);
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies <V> vector and <This> matrix
	 *
	 * Post : Returns <Result> vector, 
	 *        <V> and <Result> maybe be the same vector
	 *
	 ------------------------------------------------------*/

	Quat ToQuat()
	{
		Quat q;
		q.SetByMatrix(mMatrix);
		return q;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Quaternion representation.
	 *
	 ------------------------------------------------------*/

	mstl::String ToString()
	{
		mstl::String s;
		s.Set("{ %f %f %f %f |  %f %f %f %f |  %f %f %f %f |  %f %f %f %f }",
			  mMatrix[ 0], mMatrix[ 4], mMatrix[ 8], mMatrix[12],
			  mMatrix[ 1], mMatrix[ 5], mMatrix[ 9], mMatrix[13],
			  mMatrix[ 2], mMatrix[ 6], mMatrix[10], mMatrix[14],
			  mMatrix[ 3], mMatrix[ 7], mMatrix[11], mMatrix[15]);
		return s;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns String representation.
	 *
	 ------------------------------------------------------*/

	void Print()
	{
			mstl::String s = ToString();
			s.Replace('|', '\n');
			printf("%s\n", s.c_str());
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Outputs pretty String representation with libc printf.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	vec_t &operator [] (uint32 i) { return mMatrix[i]; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	Mat44 &operator =(const Mat44 &m)
	{
		memcpy(mMatrix, m.mMatrix, sizeof(matrix_t));
		return *this;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 ------------------------------------------------------*/

	Mat44 &operator =(const Quat &q)
	{
		Quat(q).GetMatrix(mMatrix);
		return *this;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 ------------------------------------------------------*/

	// prove, order check, opt
	Mat44 operator *(const Mat44 &m)
	{
		Mat44 n;
		helMatrixMultiply(mMatrix, m.mMatrix, n.mMatrix);
		return n;
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies m and this matrices
	 * Post : Returns resultant matrix
	 *
	 ------------------------------------------------------*/

	Vec3 operator *(Vec3 &v);
	/*------------------------------------------------------
	 * Pre  : <V> is vector to multiply by this matrix
	 * Post : Returns resultant vector ( mult )
	 *
	 ------------------------------------------------------*/

	bool Invert();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This matrix is inverted
	 *
	 ------------------------------------------------------*/

	void SetIdentity()
	{
		memcpy(mMatrix, mIdentity, sizeof(matrix_t));
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets to identity matrix
	 *
	 ------------------------------------------------------*/

	void SetMatrix(matrix_t m)
	{
		memcpy(mMatrix, m, sizeof(matrix_t));
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets to given matrix
	 ------------------------------------------------------*/

	static void GetRotation(matrix_t m, vec_t alpha, vec_t beta, vec_t gamma);
	/*------------------------------------------------------
	 * Pre  : Angles in radians
	 * Post : Sets matrix <m> to corresponding rotation matrix.
	 *
	 ------------------------------------------------------*/

	void SetRotation(vec_t alpha, vec_t beta, vec_t gamma);
	void SetRotation2(vec_t alpha, vec_t beta, vec_t gamma);
	/*------------------------------------------------------
	 * Pre  : Angles in radians
	 * Post : Sets this matrix to corresponding rotation matrix.
	 *
	 ------------------------------------------------------*/

	void SetRotationX(vec_t a)
	{
		SetIdentity();
		vec_t cosA, sinA;
		helSinCosf(a, &sinA, &cosA);
		mMatrix[5] = cosA;  mMatrix[ 9] = -sinA; 
		mMatrix[6] = sinA;  mMatrix[10] = cosA;
	}
	/*------------------------------------------------------
	 * Pre  : Angle in radians
	 * Post : Sets to corresponding rotation matrix
	 *
	 ------------------------------------------------------*/

	void SetRotationY(vec_t a)
	{
		SetIdentity();
		vec_t cosA, sinA;
		helSinCosf(a, &sinA, &cosA);
		mMatrix[0] = cosA;   mMatrix[ 8] = sinA; 
		mMatrix[3] = -sinA;  mMatrix[10] = cosA;
	}
	/*------------------------------------------------------
	 * Pre  : Angle in radians
	 * Post : Sets to corresponding rotation matrix
	 *
	 ------------------------------------------------------*/

	void SetRotationZ(vec_t a)
	{
		SetIdentity();
		vec_t cosA, sinA;
		helSinCosf(a, &sinA, &cosA);
		mMatrix[0] = cosA;  mMatrix[4] = -sinA; 
		mMatrix[1] = sinA;  mMatrix[5] = cosA;
	}
	/*------------------------------------------------------
	 * Pre  : Angle in radians
	 * Post : Sets to corresponding rotation matrix
	 *
	 ------------------------------------------------------*/

	void SetScale(vec_t x, vec_t y, vec_t z)
	{
		SetIdentity();
		mMatrix[0] = x; mMatrix[5] = y; mMatrix[10] = z;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets to corresponding translation matrix
	 *
	 ------------------------------------------------------*/

	void SetTranslation(vec_t x, vec_t y, vec_t z)
	{
		SetIdentity();
		mMatrix[12] = x; mMatrix[13] = y; mMatrix[14] = z;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets to corresponding translation matrix
	 *
	 ------------------------------------------------------*/
	
	void SetTransform(vec3_t scale, vec3_t rotation, vec3_t translation);
	/*------------------------------------------------------
	 * Pre  : <x, y, z>, <a, b, g>, <x, y, z>
	 * Post : Sets to corresponding transform matrix
	 *
	 ------------------------------------------------------*/

	void Rotate(const Vec3 &v) { Rotate(v.mVec); }
	void Rotate(const vec3_t &v) { Rotate(v[0], v[1], v[2]); }
	void Rotate(vec_t alpha, vec_t beta, vec_t gamma);
	/*------------------------------------------------------
	 * Pre  : Angles are in radians
	 * Post : Rotates object in 3 space
	 *
	 ------------------------------------------------------*/

	void Scale(const Vec3 &v) { Scale(v.mVec); }
	void Scale(const vec3_t &v) { Scale(v[0], v[1], v[2]); }
	void Scale(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Scales object in 3 space
	 *
	 ------------------------------------------------------*/

	// Adding translation vector same as set tranlation and then mat mult?
	void Translate(const Vec3 &v) { Translate(v.mVec); }
	void Translate(const vec3_t &v) { Translate(v[0], v[1], v[2]); }
	void Translate(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : This matrix becomes the translation x,y,z
	 *
	 ------------------------------------------------------*/

	void Transpose()
	{
#define HEL_MAT44_SWAP(a, b) tmp = mMatrix[a]; mMatrix[a] = mMatrix[b]; mMatrix[b] = tmp;
		vec_t tmp;
		HEL_MAT44_SWAP(1, 4);
		HEL_MAT44_SWAP(2, 8);
		HEL_MAT44_SWAP(3, 12);
		HEL_MAT44_SWAP(6, 9);
		HEL_MAT44_SWAP(7, 13);
		HEL_MAT44_SWAP(11, 14);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Transpose this matrix
	 *
	 ------------------------------------------------------*/

	static const matrix_t mIdentity;  /* Identity matrix */

#if 1
	matrix_t mMatrix;                 /* Matrix data */
#else
	union {          /* Vector data */
		matrix_t mMatrix;        

		struct { // mRowCol             // OpenGL, Column order matrix_t
			vec_t m00, m10, m20, m30;	// | 0  4  8  12 |  | m00 m01 m02 m03 |
			vec_t m01, m11, m21, m31;	// | 1  5  9  13 |  | m10 m11 m12 m13 |
			vec_t m02, m12, m22, m32;	// | 2  6 10  14 |  | m20 m21 m22 m23 |
			vec_t m03, m13, m23, m33;	// | 3  7 11  15 |  | m30 m31 m32 m33 |
		};
	};
#endif

	////////////////////////////////////////////////////////////
	// Experimental cruft not really part of API
	////////////////////////////////////////////////////////////

	void MultiplyVertexArrayThreaded(uint32 threads, uint32 size, vec_t *array);

	void MultiplyVertexArrayThreadedPartition(uint32 offset, uint32 size,
											  vec_t *array)
	{
		for ( uint32 i = 0, j = offset; i < size; ++i, j += 3 )
		{
			vec_t x = array[  j], y = array[j+1], z = array[j+2];
			array[  j] = mMatrix[0]*x + mMatrix[4]*y + mMatrix[ 8]*z + mMatrix[12];
			array[j+1] = mMatrix[1]*x + mMatrix[5]*y + mMatrix[ 9]*z + mMatrix[13];
			array[j+2] = mMatrix[2]*x + mMatrix[6]*y + mMatrix[10]*z + mMatrix[14];
		}
	}


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

} // End namespace hel


#endif
