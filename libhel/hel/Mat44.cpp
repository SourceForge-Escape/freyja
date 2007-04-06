/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Matrix
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: 3d Matrix in class form
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2003.06.17:
 * Mongoose - Now in column order to match OpenGL user needs,
 *            use transpose() to get row order back  =)
 *
 * 2002.05.11:
 * Mongoose - Created, based on my mtk3d matrix
 ==========================================================================*/

#include <mstl/Thread.h>

#include "hel/Mat44.h"


using namespace hel;

// Transpose(I) = I, This is row order, but doesn't matter in this case.
const matrix_t Mat44::mIdentity= { 1, 0, 0, 0, 
								   0, 1, 0, 0, 
								   0, 0, 1, 0, 
								   0, 0, 0, 1 };

bool Mat44::IsIdentity()
{
	matrix_t &m = mMatrix;
	bool t = false;

	// Hhhmm... floating point using direct comparisions
	if (m[ 0] == 1 && m[ 4] == 0 && m[ 8] == 0 && m[12] == 0 &&
		m[ 1] == 0 && m[ 5] == 1 && m[ 9] == 0 && m[13] == 0 && 
		m[ 2] == 0 && m[ 6] == 0 && m[10] == 1 && m[14] == 0 &&
		m[ 3] == 0 && m[ 7] == 0 && m[11] == 0 && m[15] == 1)
		t = true;

	return t;
}


void Mat44::MultiplyVertexArrayThreaded(uint32 threads, uint32 size, vec_t *array)
{
	mstl::DelegateThread dthreads[threads];

	for (uint32 i = 0; i < threads; ++i)
	{
		typedef mstl::CallbackArg3<Mat44, uint32, uint32, vec_t *> callback; 

		mstl::Delegate *d;
		d =	new callback(this, &Mat44::MultiplyVertexArrayThreadedPartition,
						 i*((size*3)/threads), (size/threads), array);

		dthreads[i].SetDelegate(d);
		dthreads[i].Create();
	}

	for (uint32 i = 0; i < threads; ++i)
	{
		dthreads[i].Join();
	}
}


Vec3 Mat44::operator *(Vec3 &v)
{
	Vec3 u;
	Multiply3fv(v.mVec, u.mVec);
	return u;
}


bool Mat44::Invert()
{
	// FIXME

	printf("Mat44::Invert() NOT IMPLEMENETED\n");

	return false;
}


void Mat44::GetRotation(matrix_t m, vec_t alpha, vec_t beta, vec_t gamma)
{
	memcpy(m, mIdentity, sizeof(matrix_t));

	vec_t cosA, sinA;
	helSinCosf(alpha, &sinA, &cosA);
	vec_t cosB, sinB;
	helSinCosf(beta, &sinB, &cosB);
	vec_t cosG, sinG;
	helSinCosf(gamma, &sinG, &cosG);

	m[ 0] = cosG * cosB; 
	m[ 1] = (cosG * sinB * sinA + sinG * cosA); 
	m[ 2] = (sinG * sinA - cosG * sinB * cosA); 

	m[ 4] = -sinG * cosB;
	m[ 5] = (cosG * cosA - sinG * sinB * sinA); 
	m[ 6] = (sinG * sinB * cosA + sinA * cosG); 
	
	m[ 8] = sinB;
	m[ 9] = -cosB * sinA;
	m[10] = cosB * cosA;
}


void Mat44::SetRotation2(vec_t alpha, vec_t beta, vec_t gamma)
{
	vec_t cosA, sinA;
	helSinCosf(alpha, &sinA, &cosA);
	vec_t cosB, sinB;
	helSinCosf(beta, &sinB, &cosB);
	vec_t cosG, sinG;
	helSinCosf(gamma, &sinG, &cosG);

	mMatrix[ 0] = cosG * cosB; 
	mMatrix[ 1] = (cosG * sinB * sinA + sinG * cosA); 
	mMatrix[ 2] = (sinG * sinA - cosG * sinB * cosA); 

	mMatrix[ 4] = -sinG * cosB;
	mMatrix[ 5] = (cosG * cosA - sinG * sinB * sinA); 
	mMatrix[ 6] = (sinG * sinB * cosA + sinA * cosG); 
	
	mMatrix[ 8] = sinB;
	mMatrix[ 9] = -cosB * sinA;
	mMatrix[10] = cosB * cosA;     

	mMatrix[3] = mMatrix[7] = mMatrix[11] = mMatrix[12] = mMatrix[13] = mMatrix[14] = 0.0f;
	mMatrix[15] = 1.0f;
}


void Mat44::SetRotation(vec_t alpha, vec_t beta, vec_t gamma)
{
	SetIdentity();

	vec_t cosA, sinA;
	helSinCosf(alpha, &sinA, &cosA);
	vec_t cosB, sinB;
	helSinCosf(beta, &sinB, &cosB);
	vec_t cosG, sinG;
	helSinCosf(gamma, &sinG, &cosG);

	mMatrix[ 0] = cosG * cosB; 
	mMatrix[ 1] = (cosG * sinB * sinA + sinG * cosA); 
	mMatrix[ 2] = (sinG * sinA - cosG * sinB * cosA); 

	mMatrix[ 4] = -sinG * cosB;
	mMatrix[ 5] = (cosG * cosA - sinG * sinB * sinA); 
	mMatrix[ 6] = (sinG * sinB * cosA + sinA * cosG); 
	
	mMatrix[ 8] = sinB;
	mMatrix[ 9] = -cosB * sinA;
	mMatrix[10] = cosB * cosA;     
}


void Mat44::SetTransform(vec3_t scale, vec3_t rotation, vec3_t translation)
{
	SetIdentity();
	mMatrix[12] = translation[0]; 
	mMatrix[13] = translation[1];
	mMatrix[14] = translation[2];

	vec_t cosA, sinA;
	helSinCosf(rotation[0], &sinA, &cosA);
	vec_t cosB, sinB;
	helSinCosf(rotation[1], &sinB, &cosB);
	vec_t cosG, sinG;
	helSinCosf(rotation[2], &sinG, &cosG);
		
	mMatrix[ 0] = scale[0] * cosG * cosB; 
	mMatrix[ 1] = scale[0] * (cosG * sinB * sinA + sinG * cosA); 
	mMatrix[ 2] = scale[0] * (sinG * sinA - cosG * sinB * cosA); 

	mMatrix[ 4] = -scale[1] * sinG * cosB;
	mMatrix[ 5] = scale[1] * (cosG * cosA - sinG * sinB * sinA); 
	mMatrix[ 6] = scale[1] * (sinG * sinB * cosA + sinA * cosG); 
	
	mMatrix[ 8] = scale[2] * sinB;
	mMatrix[ 9] = -scale[2] * cosB * sinA;
	mMatrix[10] = scale[2] * cosB * cosA;
}


void Mat44::Rotate(vec_t alpha, vec_t beta, vec_t gamma)
{
	Mat44 r;
	r.SetRotation(alpha, beta, gamma);
	Mat44 m(*this);
	helMatrixMultiply(r.mMatrix, m.mMatrix, mMatrix);
}


// prove
void Mat44::Scale(vec_t x, vec_t y, vec_t z)
{
	mMatrix[ 0] *= x;
	mMatrix[ 1] *= x;
	mMatrix[ 2] *= x;

	mMatrix[ 4] *= y;
	mMatrix[ 5] *= y;
	mMatrix[ 6] *= y;
	
	mMatrix[ 8] *= z;
	mMatrix[ 9] *= z;
	mMatrix[10] *= z;
}


// prove
void Mat44::Translate(vec_t x, vec_t y, vec_t z)
{
	mMatrix[12] += x;
	mMatrix[13] += y;
	mMatrix[14] += z;	
}

