/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : PSKModel
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: Unreal Tournament skeletal model
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2003.12.29:
 * Mongoose - Filled in more unknowns using a dead plugin project's
 *            API I found on the web, which could break something.
 * 
 *            They got some of the geometry wrong, but seemed to have
 *            good guesses about Material/Anim unknowns ( UT1 infos? )
 *
 * 2003.07.12:
 * Mongoose - UT Package prototype code moved into 'UTPackage'
 *            API changes to structs to clearify meaning
 *
 * 2003.06.12:
 * Mongoose - PSA keyframes fixed, special thanks to:
 *            Steven Fuller and Forest Hale for help picking
 *            apart those tough little 8 bytes   =)
 *
 * 2003.06.11:
 * Mongoose - Broke up rendering and model loading into seperate
 *            classes ( Closely coupled )
 *
 *            PSA experimental loader
 *
 * 2003.06.10:
 * Mongoose - Skeletal format fixed, should load perfectly
 *
 * 2003.01.20:
 * Mongoose - Fixed triangle rendering thanks to debugging by
 *            Steven Fuller, who found the tris -> UV -> vert 
 *            connection
 *
 *            Finished up basic rendering, lots of 
 *            small fixes/features
 *
 * 2003.01.06:
 * Mongoose - Created
 =================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#ifdef HAVE_UTPACKAGE
#   include "UTPackage.h"
#endif

#include "PSKModel.h"


//#define FUDGE_BONE_RENDER
//#define INTERACTIVE_BONE_RENDER


void multiply_matrix(float *a, float *b, float *result)
{
	/* Column order */
	result[ 0] = a[ 0] * b[ 0] + a[ 4] * b[ 1] + a[ 8] * b[ 2] + a[12] * b[ 3];
	result[ 1] = a[ 0] * b[ 4] + a[ 4] * b[ 5] + a[ 8] * b[ 6] + a[12] * b[ 7];
	result[ 2] = a[ 0] * b[ 8] + a[ 4] * b[ 9] + a[ 8] * b[10] + a[12] * b[11];
	result[ 3] = a[ 0] * b[12] + a[ 4] * b[13] + a[ 8] * b[14] + a[12] * b[15];
	
	result[ 4] = a[ 1] * b[ 0] + a[ 5] * b[ 1] + a[ 9] * b[ 2] + a[13] * b[ 3];
	result[ 5] = a[ 1] * b[ 4] + a[ 5] * b[ 5] + a[ 9] * b[ 6] + a[13] * b[ 7];
	result[ 6] = a[ 1] * b[ 8] + a[ 5] * b[ 9] + a[ 9] * b[10] + a[13] * b[11];
	result[ 7] = a[ 1] * b[12] + a[ 5] * b[13] + a[ 9] * b[14] + a[13] * b[15];
	
	result[ 8] = a[ 2] * b[ 0] + a[ 6] * b[ 1] + a[10] * b[ 2] + a[14] * b[ 3];
	result[ 9] = a[ 2] * b[ 4] + a[ 6] * b[ 5] + a[10] * b[ 6] + a[14] * b[ 7];
	result[10] = a[ 2] * b[ 8] + a[ 6] * b[ 9] + a[10] * b[10] + a[14] * b[11];
	result[11] = a[ 2] * b[12] + a[ 6] * b[13] + a[10] * b[14] + a[14] * b[15];
	
	result[12] = a[ 3] * b[ 0] + a[ 7] * b[ 1] + a[11] * b[ 2] + a[15] * b[ 3];
	result[13] = a[ 3] * b[ 4] + a[ 7] * b[ 5] + a[11] * b[ 6] + a[15] * b[ 7];
	result[14] = a[ 3] * b[ 8] + a[ 7] * b[ 9] + a[11] * b[10] + a[15] * b[11];
	result[15] = a[ 3] * b[12] + a[ 7] * b[13] + a[11] * b[14] + a[15] * b[15];
}


void copy_matrix(float *src, float *dest)
{
	unsigned int i;

	for (i = 0; i < 16; ++i)
		dest[i] = src[i];
}


void translate_matrix(float tx, float ty, float tz, float *m)
{
	unsigned int i;
   float tmat[16];
	float tmp[16];


   tmat[ 0]=1;  tmat[ 1]=0;  tmat[ 2]=0;  tmat[ 3]=0;
   tmat[ 4]=0;  tmat[ 5]=1;  tmat[ 6]=0;  tmat[ 7]=0;
   tmat[ 8]=0;  tmat[ 9]=0;  tmat[10]=1;  tmat[11]=0;
   tmat[12]=tx; tmat[13]=ty; tmat[14]=tz; tmat[15]=1;

	//copy_matrix(m, tmp);
	
	for (i = 0; i < 16; ++i)
		tmp[i] = m[i];

	multiply_matrix(tmp, tmat, m);
}


/* <M> is input matrix
	<Out> is output matrix ( M inverted )
	both are float[16] column major matrices */
bool invert_matrix(float *m, float *out)
{
	/* Mongoose: This code was from a Jeff Lander tutorial which was based
		on MESA GL's InvertMatrix */

	/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]
	
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;
	
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	
	r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
	r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
	
	r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
	r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
	
	r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
	r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
	
	r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
	r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  return false;
	
	/* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }
	
	/* choose pivot - or die */
	if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  return false;
	
	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }
	
	/* choose pivot - or die */
	if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return false;
	
	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];
	
	/* last check */
	if (0.0 == r3[3]) return false;
	
	s = 1.0/r3[3];              /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;
	
	m2 = r2[3];                 /* now back substitute row 2 */
	s  = 1.0/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	
	m1 = r1[2];                 /* now back substitute row 1 */
	s  = 1.0/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	
	m0 = r0[1];                 /* now back substitute row 0 */
	s  = 1.0/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	
	MAT(out,0,0) = r0[4]; 
	MAT(out,0,1) = r0[5], MAT(out,0,2) = r0[6]; 
	MAT(out,0,3) = r0[7], MAT(out,1,0) = r1[4];
	MAT(out,1,1) = r1[5], MAT(out,1,2) = r1[6];
	MAT(out,1,3) = r1[7], MAT(out,2,0) = r2[4];
	MAT(out,2,1) = r2[5], MAT(out,2,2) = r2[6];
	MAT(out,2,3) = r2[7], MAT(out,3,0) = r3[4];
	MAT(out,3,1) = r3[5], MAT(out,3,2) = r3[6];
	MAT(out,3,3) = r3[7]; 
	
	return true;
#undef MAT
#undef SWAP_ROWS
}


void quaternion_to_matrix(float mW, float mX, float mY, float mZ, float *m)
{
	m[ 0] = 1.0f - 2.0f * (mY*mY + mZ*mZ);
	m[ 1] = 2.0f * (mX*mY - mW*mZ);
	m[ 2] = 2.0f * (mX*mZ + mW*mY);
	m[ 3] = 0.0f;
	
	m[ 4] = 2.0f * (mX*mY + mW*mZ);
	m[ 5] = 1.0f - 2.0f * (mX*mX + mZ*mZ);
	m[ 6] = 2.0f * (mY*mZ - mW*mX);
	m[ 7] = 0.0f;
	
	m[ 8] = 2.0f * (mX*mZ - mW*mY);
	m[ 9] = 2.0f * (mY*mZ + mW*mX);
	m[10] = 1.0 - 2.0f * (mX*mX + mY*mY);
	m[11] = 0.0f;
	
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}


void quaternion_to_axis_angles(float qw, float qx, float qy, float qz, 
							   float *theta, float *x, float *y, float *z)
{
	*theta = (float)acos(qw) * 2.0;

	double scale = (
						 //sqrt(qx*qx + qy*qy + qz*qz);
						 (float)sin(*theta / 2.0));

	*x = qx / scale;
	*y = qy / scale;
	*z = qz / scale;
}


void quaternion_to_euler_angles(float qw, float qx, float qy, float qz, 
								float *x, float *y, float *z)
{
	double qw2 = qw*qw;
	double qx2 = qx*qx;
	double qy2 = qy*qy;
	double qz2 = qz*qz;

	*x = atan(2.0 * (qx*qy+qz*qw)/(qx2 - qy2 + qz2 + qw2)); // heading
	*y = atan(2.0 * (qy*qz+qx*qw)/(-qx2 - qy2 + qz2 + qw2));  // bank
	*z = asin(-2.0 * (qx*qz-qy*qw)); // attitude
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

PSKModel::PSKModel()
{
	mFlags = 0;

	mNumFrames = 0;
	mNumVertices = 0;
	mNumFaces = 0;
	mNumVTXWs = 0;
	mNumMaterials = 0;
	mNumBones = 0;
	mNumWeights = 0;

	mVertices = 0x0;
	mWeights = 0x0;
	mVTXWs = 0x0;
	mFaces = 0x0;
	mMaterials = 0x0;
	mBones = 0x0;
}


PSKModel::~PSKModel()
{
	if (mVertices) 
		delete [] mVertices;

	if (mWeights) 
		delete [] mWeights;

	if (mVTXWs) 
		delete [] mVTXWs;

	if (mFaces) 
		delete [] mFaces;

	if (mMaterials) 
		delete [] mMaterials;

	if (mBones) 
		delete [] mBones;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void PSKModel::printSkeletion(bool printNames)
{
	unsigned int i, j;

	printf("Skeletion layout...\n");

	for (i = 0; i < mNumBones; ++i)
	{
		printf("%3i. ", i);
		j = i;
			
		while (j > 0)
		{
			if (printNames)
			{
				printf(" -> %s ", mBones[j].name);
			}
			else
			{
				printf(" -> %u ", j);
			}

			j = mBones[j].parentIndex;
		}
			
		printf("\n");
	}

	printf("\n");
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int PSKModel::load(char *filename)
{
	FILE *f;
	char buffer[128];
	unsigned int i, frame;
	float r;


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("PSKModel::load> ");
		return -1;
	}

	/* ACTRHEAD... */
	fread(buffer, 1, 28, f);
	buffer[8] = 0;

	if (strncmp("ACTRHEAD", buffer, 8) != 0)
	{
		printf("Looking for 'ACTRHEAD' found '%s'\n", buffer);
		printf("This isn't a valid or known PSK model.\n");
		return -2;
	}
	else
	{
		printf("Unreal Tourament (c) EpicGames Skeletal Model Viewer\n");
		printf("BETA release\n");
		printf("Give feedback on correctness and bugs\n");
		printf("by mongoose@icculus.org\n");
	}

	fread(&mNumFrames, 1, 4, f);
	printf(" NumFrames =\t %u\n", mNumFrames); // used to expand NumVerts?

	printf(" FORCING FRAMES TO 1 FOR TESTING NOW.\n");
	mNumFrames = 1;

	for (frame = 0; frame < mNumFrames; ++frame)
	{
		printf(" ===========================\n");
		printf(" Reading Frame[%03u]\n", frame);
		printf(" ===========================\n");		

		/* PNTS0000... */
		fread(buffer, 1, 28, f);
		buffer[8] = 0;
	
		if (frame == 0 && strncmp("PNTS0000", buffer, 8) != 0)
		{
			printf("Expected PNTS0000, found '%s'\n", buffer);
			printf("Test model Vertex offset 0x003c, at 0x%lx?\n", ftell(f));
			printf("This isn't a known PNTS0000\n");
			return -2;
		}

		fread(&mNumVertices, 1, 4, f);
		printf(" NumVertices =\t %u\n", mNumVertices);
		mVertices = new float[mNumVertices*3];

		for (i = 0; i < mNumVertices*3; i+= 3)
		{
			fread(&r, 1, 4, f);
			mVertices[i] = r;
			fread(&r, 1, 4, f);
			mVertices[i+1] = r;
			fread(&r, 1, 4, f);
			mVertices[i+2] = r;
			
			if (mFlags & fDebugPointLoad)
			{
				printf("\t%5u (%.3f, %.3f, %.3f)\n", 
						 i/3, mVertices[i], mVertices[i+1], mVertices[i+2]);
			}
		}


		/* VTXW0000... */
		fread(buffer, 1, 28, f);
		buffer[8] = 0;
		
		if (frame == 0 && strncmp("VTXW0000", buffer, 8) != 0)
		{
			printf("Expected VTXW0000, found '%s'\n", buffer);
			printf("Test model UV offset 0x2554, at 0x%lx?\n", ftell(f));
			printf("This isn't a known VTXW0000\n");
			return -2;
		}
		
		fread(&mNumVTXWs, 1, 4, f);
		printf(" NumVTXWs =\t %u\n", mNumVTXWs);

		mVTXWs = new psk_vtxw_t[mNumVTXWs];

		// <relnev> (uint vertex in lowest 16bits and something in upper 16, float u, float v, uint flags?)

		for (i = 0; i < mNumVTXWs; ++i)
		{
			// Vertex
			fread(&mVTXWs[i].vertex, 2, 1, f);

			// Material index?
			fread(&mVTXWs[i].material, 1, 1, f);

			// Reserved? ( From half right plugin documentation I saw one is res )
			fread(&mVTXWs[i].reserved, 1, 1, f);

			// UV
			fread(mVTXWs[i].uv+0, 4, 1, f);
			fread(mVTXWs[i].uv+1, 4, 1, f);

			// Used to compute weight index?
			fread(&mVTXWs[i].weight, 4, 1, f);



			if (mFlags & fDebugUVLoad)
			{
				printf("\t%5u vertex = %u, material = %i?, reserved = %i?, UV = (%.3f, %.3f), weight = %f?\n", 
						 i, 
						 mVTXWs[i].vertex,
						 mVTXWs[i].material, mVTXWs[i].reserved,
						 mVTXWs[i].uv[0], mVTXWs[i].uv[1],
						 mVTXWs[i].weight);
			}
		}


		/* FACE0000... */
		fread(buffer, 1, 28, f);
		buffer[8] = 0;
		
		if (frame == 0 && strncmp("FACE0000", buffer, 8) != 0)
		{
			printf("Expected FACE0000, found '%s'\n", buffer);
			printf("Test model FACE offset 0x7074, at 0x%lx?\n", ftell(f));
			printf("This isn't a known value for FACE\n");
			return -2;
		}

		fread(&mNumFaces, 1, 4, f);
		printf(" NumFaces =\t %u\n", mNumFaces);
		
		mFaces = new psk_face_t[mNumFaces];
		
		for (i = 0; i < mNumFaces; ++i)
		{
			// Vertex wedge indices
			fread(&mFaces[i].x, 1, 2, f);
			fread(&mFaces[i].y, 1, 2, f);
			fread(&mFaces[i].z, 1, 2, f);

			// Material Index
			fread(&mFaces[i].material, 1, 1, f);

			// Secondary Material Index
			fread(&mFaces[i].auxMaterial, 1, 1, f);

			// Smoothing Groups
			fread(&mFaces[i].smoothingGroups, 1, 4, f);


			if (mFlags & fDebugFaceLoad)
			{
				printf("%5u (%u, %u, %u), material = %i, auxMaterial = %i, smoothingGroups  = %i\n", 
						 i, 
						 mFaces[i].x, mFaces[i].y, mFaces[i].z,
						 mFaces[i].material, 
						 mFaces[i].auxMaterial, 
						 mFaces[i].smoothingGroups);
						 
			}
		}


		/* MATT0000... */
		fread(buffer, 1, 28, f);
		buffer[8] = 0;
		
		if (frame == 0 && strncmp("MATT0000", buffer, 8) != 0)
		{
			printf("Expected MATT0000, found '%s'\n", buffer);
			printf("Test model Material offset 0xb750, at 0x%lx?\n", ftell(f));
			printf("This isn't a known value for MATT0000\n");
			return -2;
		}

		fread(&mNumMaterials, 1, 4, f);
		printf(" NumMaterials =\t %u\n", mNumMaterials);
		mMaterials = new psk_material_t[mNumMaterials];
		
		for (i = 0; i < mNumMaterials; ++i)
		{
			fread(&mMaterials[i].name, 1, 64, f);
			fread(&mMaterials[i].textureIndex, 1, 4, f);
			fread(&mMaterials[i].polygonFlags, 1, 4, f);
			fread(&mMaterials[i].auxMaterial, 1, 4, f);
			fread(&mMaterials[i].auxFlags, 1, 4, f);
			fread(&mMaterials[i].lodBias, 1, 4, f);
			fread(&mMaterials[i].lodStyle, 1, 4, f);

			if (mFlags & fDebugMattLoad)
			{
				printf("\n# File offset 0x%lx\n", ftell(f) - 88);
				printf("mMaterial[%u].name = '%s'\n", i, mMaterials[i].name);
				printf("mMaterial[%u].textureIndex = %i\n",
						 i, mMaterials[i].textureIndex);
				printf("mMaterial[%u].polygonFlags = 0x%x\n",
						 i, mMaterials[i].polygonFlags);
				printf("mMaterial[%u].auxMaterial = %i\n",
						 i, mMaterials[i].auxMaterial);
				printf("mMaterial[%u].auxFlags = 0x%x\n",
						 i, mMaterials[i].auxFlags);
				printf("mMaterial[%u].lodBias = %i\n",
						 i, mMaterials[i].lodBias);
				printf("mMaterial[%u].lodStyle = %i\n",
						 i, mMaterials[i].lodStyle);
				printf("UVWs counting??\n");
			}
		}
	}


	/* REFSKELT... */
	fread(buffer, 1, 28, f);
	buffer[8] = 0;

	if (strncmp("REFSKELT", buffer, 8) != 0)
	{
		printf("Expected REFSKELT, found '%s'\n", buffer);
		printf("Test model Material offset 0xB820, at 0x%lx?\n", ftell(f));
		printf("This isn't a known value for REFSKELT\n");
		return -2;
	}

	fread(&mNumBones, 1, 4, f);
	printf(" NumBones =\t %u\n", mNumBones);
	
	mBones = new psk_bone_t[mNumBones];

	for (i = 0; i < mNumBones; ++i)
	{
		fread(buffer, 1, 64, f);
		strncpy(mBones[i].name, buffer, 64);
		fread(&mBones[i].flags, 1, 4, f);
		fread(&mBones[i].numChildren, 1, 4, f);
		fread(&mBones[i].parentIndex, 1, 4, f); 

		fread(&r, 1, 4, f);
		mBones[i].restDir[0] = r;
		fread(&r, 1, 4, f);
		mBones[i].restDir[1] = r;
		fread(&r, 1, 4, f);
		mBones[i].restDir[2] = r;
		fread(&r, 1, 4, f);
		mBones[i].restDir[3] = r;

		fread(&r, 1, 4, f);
		mBones[i].restLoc[0] = r;
		fread(&r, 1, 4, f);
		mBones[i].restLoc[1] = r;
		fread(&r, 1, 4, f);
		mBones[i].restLoc[2] = r;

		fread(&mBones[i].length, 1, 4, f);
		fread(&mBones[i].xSize, 1, 4, f);
		fread(&mBones[i].ySize, 1, 4, f);
		fread(&mBones[i].zSize, 1, 4, f);
		
		if (mFlags & fDebugBoneLoad)
		{
			unsigned int offset = ftell(f) - 110;
			printf("\n# File offset 0x%x + 110bytes\n", offset);

			printf("Bone[%03u] = { '%s'\n", i, buffer);
			printf("\tParent: %u, Children: %u, flags: 0x%80x\n", 
					 mBones[i].parentIndex, mBones[i].numChildren, 
					 mBones[i].flags);
			printf("\tRest Loc: (%.3f, %.3f, %.3f)\n", 
					 mBones[i].restLoc[0], mBones[i].restLoc[1], 
					 mBones[i].restLoc[2]);
			printf("\tRest Dir XYZ: (%.3f, %.3f, %.3f), W: %.3f?\n", 
					 mBones[i].restDir[0], mBones[i].restDir[1], 
					 mBones[i].restDir[2], mBones[i].restDir[3]);
			printf("\tScale: length %f, xSize: %f, ySize: %f, zSize: %f\n", 
					 mBones[i].length, 
					 mBones[i].xSize, mBones[i].ySize, mBones[i].zSize);
			printf("}\n");
		}
	}

	if (mFlags & fDebugBoneLoad)
	{
		printSkeletion(false);
	}

	/* RAWWEIGHTS... */
	fread(buffer, 1, 28, f);
	buffer[11] = 0;

	if (strncmp("RAWWEIGHTS", buffer, 10) != 0)
	{
		printf("Expected RAWWEIGHTS, found '%s'\n", buffer);
		printf("Test model Weight offset 0xcb00, at 0x%lx?\n", ftell(f));
		printf("This isn't a known value for RAWWEIGHTS\n");
		return -2;
	}

	fread(&mNumWeights, 1, 4, f);
	printf(" NumWeights =\t %u\n", mNumWeights);
	
	mWeights = new psk_weight_t[mNumWeights];

	for (i = 0; i < mNumWeights; ++i)
	{
		fread(&mWeights[i].weight, 4, 1, f);
		fread(&mWeights[i].vertexIndex, 4, 1, f);
		fread(&mWeights[i].boneIndex, 4, 1, f);

		if (mFlags & fDebugWeightLoad)
		{
			printf("\tWeight[%u] { w = %f, vertexIndex = %u,  boneIndex = %u }\n", i, 
					 mWeights[i].weight, 
					 mWeights[i].vertexIndex, mWeights[i].boneIndex);
		}
	}

	fclose(f);

	return 0;
}


int PSKModel::save(char *filename)
{
	FILE *f;
	char buffer[128];
	unsigned int i, frame;
	float r;


	f = fopen(filename, "wb");

	if (!f)
	{
		perror("PSKModel::save> ");
		return -1;
	}

	/* ACTRHEAD... */
	strcpy(buffer, "ACTRHEAD");
	fwrite(buffer, 1, 28, f);

	//	mNumFrames = 1; Would be a good idea  =p
	fwrite(&mNumFrames, 1, 4, f);

	for (frame = 0; frame < mNumFrames; ++frame)
	{
		printf(" ===========================\n");
		printf(" Writing Frame[%03u]\n", frame);
		printf(" ===========================\n");		

		/* PNTS0000... */
		strcpy(buffer, "PNTS0000");
		fwrite(buffer, 1, 28, f);

		fwrite(&mNumVertices, 1, 4, f);

		for (i = 0; i < mNumVertices; ++i)
		{
			r = mVertices[i];
			fwrite(&r, 1, 4, f);
		}


		/* VTXW0000... */
		strcpy(buffer, "VTXW0000");
		fwrite(buffer, 1, 28, f);
		
		fwrite(&mNumVTXWs, 1, 4, f);
		printf(" NumVTXWs =\t %u\n", mNumVTXWs);

		for (i = 0; i < mNumVTXWs; ++i)
		{
			// Vertex
			fwrite(&mVTXWs[i].vertex, 2, 1, f);

			// Material index?
			fwrite(&mVTXWs[i].material, 1, 1, f);

			// Reserved? ( From half right plugin documentation I saw one is res )
			fwrite(&mVTXWs[i].reserved, 1, 1, f);

			// UV
			fwrite(mVTXWs[i].uv+0, 4, 1, f);
			fwrite(mVTXWs[i].uv+1, 4, 1, f);

			// Used to compute weight index?
			fwrite(&mVTXWs[i].weight, 4, 1, f);
		}


		/* FACE0000... */
		strcpy(buffer, "FACE0000");
		fwrite(buffer, 1, 28, f);

		fwrite(&mNumFaces, 1, 4, f);
		printf(" NumFaces =\t %u\n", mNumFaces);
		
		for (i = 0; i < mNumFaces; ++i)
		{
			// Vertex wedge indices
			fwrite(&mFaces[i].x, 1, 2, f);
			fwrite(&mFaces[i].y, 1, 2, f);
			fwrite(&mFaces[i].z, 1, 2, f);

			// Material Index
			fwrite(&mFaces[i].material, 1, 1, f);

			// Secondary Material Index
			fwrite(&mFaces[i].auxMaterial, 1, 1, f);

			// Smoothing Groups
			fwrite(&mFaces[i].smoothingGroups, 1, 4, f);
		}


		/* MATT0000... */
		strcpy(buffer, "MATT0000");
		fwrite(buffer, 1, 28, f);

		fwrite(&mNumMaterials, 1, 4, f);
		printf(" NumMaterials =\t %u\n", mNumMaterials);
		
		for (i = 0; i < mNumMaterials; ++i)
		{
			fwrite(&mMaterials[i].name, 1, 64, f);
			fwrite(&mMaterials[i].textureIndex, 1, 4, f);
			fwrite(&mMaterials[i].polygonFlags, 1, 4, f);
			fwrite(&mMaterials[i].auxMaterial, 1, 4, f);
			fwrite(&mMaterials[i].auxFlags, 1, 4, f);
			fwrite(&mMaterials[i].lodBias, 1, 4, f);
			fwrite(&mMaterials[i].lodStyle, 1, 4, f);
		}
	}

	/* REFSKELT... */
	strcpy(buffer, "REFSKELT");
	fwrite(buffer, 1, 28, f);

	fwrite(&mNumBones, 1, 4, f);
	printf(" NumBones =\t %u\n", mNumBones);
	
	for (i = 0; i < mNumBones; ++i)
	{
		fwrite(buffer, 1, 64, f);
		strncpy(mBones[i].name, buffer, 64);
		fwrite(&mBones[i].flags, 1, 4, f);
		fwrite(&mBones[i].numChildren, 1, 4, f);
		fwrite(&mBones[i].parentIndex, 1, 4, f); 

		r = mBones[i].restDir[0];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restDir[1];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restDir[2];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restDir[3];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restLoc[0];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restLoc[1];
		fwrite(&r, 1, 4, f);

		r = mBones[i].restLoc[2];
		fwrite(&r, 1, 4, f);

		fwrite(&mBones[i].length, 1, 4, f);
		fwrite(&mBones[i].xSize, 1, 4, f);
		fwrite(&mBones[i].ySize, 1, 4, f);
		fwrite(&mBones[i].zSize, 1, 4, f);
	}

	/* RAWWEIGHTS... */
	strcpy(buffer, "RAWWEIGHTS");
	fwrite(buffer, 1, 28, f);

	fwrite(&mNumWeights, 1, 4, f);
	printf(" NumWeights =\t %u\n", mNumWeights);

	for (i = 0; i < mNumWeights; ++i)
	{
		fwrite(&mWeights[i].weight, 4, 1, f);
		fwrite(&mWeights[i].vertexIndex, 4, 1, f);
		fwrite(&mWeights[i].boneIndex, 4, 1, f);
	}

	fclose(f);

	return 0;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PSAAnimation Class
//////////////////////////////////////////////////////////////////////

PSAAnimation::PSAAnimation()
{
	mNumFrames = 0;
	mNumBones = 0;
	mNumAnimInfos = 0;
	mNumKeyFrames = 0;

	mBones = 0x0;
	mAnimInfos = 0x0;
	mKeyFrames = 0x0;
}


PSAAnimation::~PSAAnimation()
{
	if (mBones)
		delete [] mBones;

	if (mAnimInfos)
		delete [] mAnimInfos;

	if (mKeyFrames)
		delete [] mKeyFrames;
}


int PSAAnimation::load(char *filename)
{
	FILE *f = fopen(filename, "rb");
	char s[64];
	unsigned int i, u;


	if (!f)
	{
		printf("PSAAnimation::load() Error: \n");
		perror(filename);
		return -1;
	}

	/* Is this a PSA file? */
	fread(s, 28, 1, f);
	s[8] = 0;
	if (strncmp("ANIMHEAD", s, 8) != 0)
	{
		printf("Expected ANIMHEAD, found '%s'\n", s);
		printf("File offset: 0x%lx?\n", ftell(f));
		printf("This isn't a known value.\n");
		return -2;
	}

	fread(&mNumFrames, 4, 1, f);
	printf("numFrames = %u\n", mNumFrames);

	/* Vertex blending frames? */


	/* Check for new section */
	fread(s, 28, 1, f);
	s[9] = 0;
	if (strncmp("BONENAMES", s, 9) != 0)
	{
		printf("Expected BONENAMES, found '%s'\n", s);
		printf("File offset: 0x%lx?\n", ftell(f));
		printf("This isn't a known value.\n");
		return -2;
	}

	fread(&mNumBones, 4, 1, f);
	printf("numBones = %u\n", mNumBones);

	mBones = new psk_bone_t[mNumBones];

	for (i = 0; i < mNumBones; ++i)
	{
		fread(mBones[i].name, 1, 64, f);
		fread(&mBones[i].flags, 1, 4, f);
		fread(&mBones[i].numChildren, 1, 4, f);
		fread(&mBones[i].parentIndex, 1, 4, f); 

		fread(&mBones[i].restDir[0], 1, 4, f);
		fread(&mBones[i].restDir[1], 1, 4, f);
		fread(&mBones[i].restDir[2], 1, 4, f);
		fread(&mBones[i].restDir[3], 1, 4, f);

		fread(&mBones[i].restLoc[0], 1, 4, f);
		fread(&mBones[i].restLoc[1], 1, 4, f);
		fread(&mBones[i].restLoc[2], 1, 4, f);

		fread(&mBones[i].length, 1, 4, f);
		fread(&mBones[i].xSize, 1, 4, f);
		fread(&mBones[i].ySize, 1, 4, f);
		fread(&mBones[i].zSize, 1, 4, f);

		if (mFlags &fDebugBones)
		{
			printf("Bone[%03u] = { '%s'\n", i, mBones[i].name);
			printf("\tParent: %u, Children: %u, Flags: 0x%04x\n", 
					 mBones[i].parentIndex, mBones[i].numChildren,
					 mBones[i].flags);
			printf("\tRest Loc: (%.3f, %.3f, %.3f)\n", 
					 mBones[i].restLoc[0], mBones[i].restLoc[1], 
					 mBones[i].restLoc[2]);
			printf("\tRest Dir XYZ: (%.3f, %.3f, %.3f), W: %.3f\n", 
					 mBones[i].restDir[0], mBones[i].restDir[1], 
					 mBones[i].restDir[2], mBones[i].restDir[3]);
			printf("\tScale: length: %f, xSize: %f, ySize: %f, zSize: %f\n", 
					 mBones[i].length,
					 mBones[i].xSize, mBones[i].ySize, mBones[i].zSize);
			printf("}\n");
		}
	}

	/* Check for new section */
	fread(s, 28, 1, f);
	s[8] = 0;
	if (strncmp("ANIMINFO", s, 8) != 0)
	{
		printf("Expected ANIMINFO, found '%s'\n", s);
		printf("File offset: 0x%lx?\n", ftell(f));
		printf("This isn't a known value.\n");
		return -2;
	}

	fread(&mNumAnimInfos, 4, 1, f);
	printf("numAnimInfos = %u\n", mNumAnimInfos);

	mAnimInfos = new psa_anim_info_t[mNumAnimInfos];

	for (i = 0; i < mNumAnimInfos; ++i)
	{
		fread(mAnimInfos[i].animName, 64, 1, f);
		fread(mAnimInfos[i].groupName, 64, 1, f);
		fread(&mAnimInfos[i].numBones, 4, 1, f);
		fread(&mAnimInfos[i].rootInclude, 4, 1, f);
		fread(&mAnimInfos[i].keyCompression, 4, 1, f);
		fread(&mAnimInfos[i].keyQuotum, 4, 1, f);
		fread(&mAnimInfos[i].keyReduction, 4, 1, f);
		fread(&mAnimInfos[i].trackTime, 4, 1, f);
		fread(&mAnimInfos[i].animRate, 4, 1, f);
		fread(&mAnimInfos[i].startBone, 4, 1, f);
		fread(&mAnimInfos[i].firstRawFrame, 4, 1, f);
		fread(&mAnimInfos[i].numRawFrames, 4, 1, f);


		if (mFlags &fDebugAnimInfos)
		{
			printf("\n");
			printf("AnimInfo[%u] = { animName: '%s', groupName: '%s'\n", i, 
					 mAnimInfos[i].animName, mAnimInfos[i].groupName);
			printf(" numBones = %i, rootInclude = %i\n",
					 mAnimInfos[i].numBones, mAnimInfos[i].rootInclude);
			printf(" keyCompression: %i, keyQuotum: %i, keyReduction: %f\n", 
					 mAnimInfos[i].keyCompression, mAnimInfos[i].keyQuotum, 
					 mAnimInfos[i].keyReduction);
			printf(" trackTime: %f, animRate: %f\n", 
					 mAnimInfos[i].trackTime, mAnimInfos[i].animRate);
			printf(" startBone: %i, firstRawFrame: %i, numRawFrames: %i\n", 
					 mAnimInfos[i].startBone, mAnimInfos[i].firstRawFrame, 
					 mAnimInfos[i].numRawFrames);
			printf("}\n");
		}
	}
	
	printf("COMPUTE: psa.numKeyframes / info.numBones = frames per bone\n");
	printf("\n");

	/* Check for new section */
	fread(s, 28, 1, f);
	s[8] = 0;
	if (strncmp("ANIMKEYS", s, 8) != 0)
	{
		printf("Expected ANIMKEYS, found '%s'\n", s);
		printf("File offset: 0x%lx?\n", ftell(f));
		printf("This isn't a known value.\n");
		return -2;
	}

	fread(&mNumKeyFrames, 4, 1, f);
	printf("numKeyFrames = %u\n", mNumKeyFrames);

	mKeyFrames = new psa_key_frame_t[mNumKeyFrames];

	for (i = 0; i < mNumKeyFrames; ++i)
	{	
		// tx ty tz qx qy qz qw s
		fread(&mKeyFrames[i].trans[0], 4, 1, f);
		fread(&mKeyFrames[i].trans[1], 4, 1, f);
		fread(&mKeyFrames[i].trans[2], 4, 1, f);
		fread(&mKeyFrames[i].dir[0], 4, 1, f);
		fread(&mKeyFrames[i].dir[1], 4, 1, f);
		fread(&mKeyFrames[i].dir[2], 4, 1, f);
		fread(&mKeyFrames[i].dir[3], 4, 1, f);
		fread(&mKeyFrames[i].time, 4, 1, f);

		if (mFlags &fDebugKeyFrames)
		{
			printf("keyFrame[%u] = {\n", i);
			printf("Trans XYZ: ( %f %f %f )\n", 
					 mKeyFrames[i].trans[0], mKeyFrames[i].trans[1],
					 mKeyFrames[i].trans[2]);
			printf("Dir XYZ: ( %f %f %f ) %f\n", 
					 mKeyFrames[i].dir[0], mKeyFrames[i].dir[1], 
					 mKeyFrames[i].dir[2], mKeyFrames[i].dir[3]);
			printf("time (till next key) = %f\n", mKeyFrames[i].time);
			printf("}\n\n");
		}
	}

	/* Check for partial read */
	u = ftell(f);
	fseek(f, 0, SEEK_END);
	i = ftell(f);

	if (i - u > 0)
	{
		printf("ERROR: Didn't read all of file!\n");
		printf("Skipping %u bytes at end of file\n", i - u);
	}

	fclose(f);

	return 0;
}


//////////////////////////////////////////////////////////////////////
// PSKModelRenderer Class
//////////////////////////////////////////////////////////////////////

PSKModelRenderer::PSKModelRenderer()
{
	mBoneRotationCache = 0x0;
	mVertexTransformCache = 0x0;
	mInvertedMatrices = 0x0;
	mWorldMatrices = 0x0;
	mModel = 0x0;
	mAnimation = 0x0;

	mNumMatrices = 0;
}


PSKModelRenderer::~PSKModelRenderer()
{
	unsigned int i;


	if (mBoneRotationCache)
		delete [] mBoneRotationCache;

	if (mVertexTransformCache)
		delete [] mVertexTransformCache;

	if (mInvertedMatrices)
	{
		for (i = 0; i < mNumMatrices; ++i)
			delete [] mInvertedMatrices[i];

		delete [] mInvertedMatrices;
	}

	if (mWorldMatrices)
	{
		for (i = 0; i < mNumMatrices; ++i)
			delete [] mWorldMatrices[i];

		delete [] mWorldMatrices;
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void PSKModelRenderer::render()
{
#ifdef HAVE_OPENGL
	unsigned int i;


	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	//glRotatef(-90.0f,  1, 0, 0);  /* Make model stand up in our Y */
	//glTranslatef(0, 0, 2);

	if (mFlags & fRenderBones)
	{
		glPushMatrix();
		//glRotatef(180.0f,  0, 0, 1);
		renderBone(0);
		glPopMatrix();
	}

	if (mFlags & fRenderPoints)
	{	
		glBegin(GL_POINTS);

		for (i = 0; i < mNumVertices*3; i+= 3)
		{
			//glVertex3fv(mVertexTransformCache+i);

			glVertex3f(-mVertexTransformCache[i+0],
					   mVertexTransformCache[i+2],
					   mVertexTransformCache[i+1]); 
			glVertex3f(-mVertexTransformCache[i+0],
					   mVertexTransformCache[i+2],
					   mVertexTransformCache[i+1]); 
			glVertex3f(-mVertexTransformCache[i+0],
					   mVertexTransformCache[i+2],
					   mVertexTransformCache[i+1]); 
		}

		glEnd();
	}

	if (mFlags & fRenderTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}

	if (mFlags & fRenderFaces)
	{		
		unsigned int index[3];
		int lastTexture = 999999;

		for (i = 0; i < mNumFaces; ++i)
		{
			if (mFlags & fRenderTexture)
			{
				if (lastTexture != mFaces[i].material)
				{
					glBindTexture(GL_TEXTURE_2D, mTextures[mFaces[i].material]);
					lastTexture = mFaces[i].material;
				}
			}

			glBegin(GL_TRIANGLES);

			index[0] = mVTXWs[mFaces[i].x].vertex*3;
			index[1] = mVTXWs[mFaces[i].y].vertex*3;
			index[2] = mVTXWs[mFaces[i].z].vertex*3;

			if (mFlags & fDebugFaceRender)
			{
				printf("face[%u] = \n", i);
				printf("\t (%u = (%.3f..), %u = (%.3f...), %u = (%.3f..))\n",
						 index[0], mVertexTransformCache[index[0]],
						 index[1], mVertexTransformCache[index[1]],
						 index[2], mVertexTransformCache[index[2]]);

				printf("material = %u\n", mFaces[i].material);

				printf("UVs (%.3f, %.3f)\n",
						 mVTXWs[mFaces[i].z].uv[0], mVTXWs[mFaces[i].z].uv[1]);
			}

			if (mFlags & fRenderTexture)
			{
				glTexCoord2fv(mVTXWs[mFaces[i].x].uv);
				glVertex3fv(mVertexTransformCache+index[0]);
				glTexCoord2fv(mVTXWs[mFaces[i].y].uv);
				glVertex3fv(mVertexTransformCache+index[1]);
				glTexCoord2fv(mVTXWs[mFaces[i].z].uv);
				glVertex3fv(mVertexTransformCache+index[2]); 
			}
			else
			{
				((i % 2) ? 
				 glColor3f(0.0f, 0.0f, (((float)i)/(float)mNumFaces)*.5+.25) :
				 glColor3f(0.0f, (((float)i)/(float)mNumFaces)*.5+.25, 0.0f));

				//glVertex3fv(mVertexTransformCache+index[0]); 
				//glVertex3fv(mVertexTransformCache+index[1]); 
				//glVertex3fv(mVertexTransformCache+index[2]); 
				glVertex3f(-mVertexTransformCache[index[0]+0],
						   mVertexTransformCache[index[0]+2],
						   mVertexTransformCache[index[0]+1]); 
				glVertex3f(-mVertexTransformCache[index[1]+0],
						   mVertexTransformCache[index[1]+2],
						   mVertexTransformCache[index[1]+1]); 
				glVertex3f(-mVertexTransformCache[index[2]+0],
						   mVertexTransformCache[index[2]+2],
						   mVertexTransformCache[index[2]+1]); 
			}

			glEnd();
		}

		if (mFlags & fDebugFaceRender)
		{
			printf("Turning fDebugFaceRender off\n");
			mFlags ^= fDebugFaceRender;
		}
	}

	glPopMatrix();
#endif
}


#ifdef INTERACTIVE_BONE_RENDER
/* HACK */
float gBoneRotHackX = 0;
float gBoneRotHackY = 0;
float gBoneRotHackZ = 0;
unsigned int gBoneId = 0;
#endif

void PSKModelRenderer::renderBone(unsigned int id)
{
#ifdef HAVE_OPENGL
	unsigned int i;
	float pos[3] = {0, 0, 0};
	float posB[3] = {0, 0, 0};
	unsigned int f;



#ifdef INTERACTIVE_BONE_RENDER
	/* Hack */
	glPushMatrix();

	if (gBoneId == id)
	{
		glRotatef(gBoneRotHackZ,  0, 0, 1);
		glRotatef(gBoneRotHackY,  0, 1, 0);
		glRotatef(gBoneRotHackX,  1, 0, 0);
	}
#endif

	glPushMatrix();

	if (id == 0)
	{
		glPointSize(8.0);
#ifdef FUDGE_BONE_RENDER
		//glTranslatef(16.0, 16.0, 0.0);
		//glRotatef(180, 0, 0, 1);
		//glRotatef(-90, 0, 1, 0);
		//glRotatef(-90, 1, 0, 0);
		glPushMatrix();
#endif
	}

	/* Handle PSA animation use */
	if (mAnimationFrame > 0 &&
		 mAnimation && mAnimation->mBones == mBones)
	{
		f = id + (mAnimationFrame*mAnimation->mNumBones); 

		posB[0] = -mAnimation->mKeyFrames[f].trans[0];
		posB[1] = -mAnimation->mKeyFrames[f].trans[1];
		posB[2] = -mAnimation->mKeyFrames[f].trans[2];
		
		glTranslatef(mAnimation->mKeyFrames[f].trans[0],
					 mAnimation->mKeyFrames[f].trans[1],
					 mAnimation->mKeyFrames[f].trans[2]);
	}
	else
	{
		posB[0] = -mBones[id].restLoc[0];
		posB[1] = -mBones[id].restLoc[1];
		posB[2] = -mBones[id].restLoc[2];
		
		if (id == 0)
		{
			posB[0] = -mBones[id].restLoc[0];
			posB[1] = -mBones[id].restLoc[2];
			posB[2] = -mBones[id].restLoc[1];

			glTranslatef(mBones[id].restLoc[0],
						 mBones[id].restLoc[2],
						 mBones[id].restLoc[1]);
		}
		else
		{
			glTranslatef(mBones[id].restLoc[0],
						 mBones[id].restLoc[1],
						 mBones[id].restLoc[2]);
		}

		if (id == 1)
		{
			glRotatef(90, 0, 1, 0);
		}

	}

	/* Draw a visible 'bone',
		Gradient light (parent) to dark (child) */
	glLineWidth(3.5);
	glBegin(GL_LINES);
	glColor3f(0.33, 0.33, 0.33);
	glVertex3fv(pos);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3fv(posB);
	glEnd();


	if (mFlags & fConvertEuler)
	{
		// euler angles
		glRotatef(mBoneRotationCache[id*4],   1, 0, 0);
		glRotatef(mBoneRotationCache[id*4+1], 0, 1, 0);
		glRotatef(mBoneRotationCache[id*4+2], 0, 0, 1);
	}
	else
	{
		// axis angles
		glRotatef(mBoneRotationCache[id*4],
				  mBoneRotationCache[id*4+1],
				  mBoneRotationCache[id*4+2],
				  mBoneRotationCache[id*4+3]);
	}

	glLineWidth(1.5);

	/* Draw axis list to show bone orientation */
	glBegin(GL_LINES);

	/* X axis */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-8.0f, 0.0f, 0.0f);
	glVertex3f(8.0f, 0.0f, 0.0f);

	/* X direction */
	glVertex3f(8.0f, 0.0f, 0.0f);
	glVertex3f(7.0f, 1.0f, 0.0f);
	glVertex3f(8.0f, 0.0f, 0.0f);
	glVertex3f(7.0f, -1.0f, 0.0f);

	/* Y axis */
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -8.0f, 0.0f);
	glVertex3f(0.0f, 8.0f, 0.0f);

	/* Y direction */
	glVertex3f(0.0f, 8.0f, 0.0f);
	glVertex3f(0.0f, 7.0f, 1.0f);
	glVertex3f(0.0f, 8.0f, 0.0f);
	glVertex3f(0.0f, 7.0f, -1.0f);

	/* Z axis */
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -8.0f);
	glVertex3f(0.0f, 0.0f, 8.0f);

	/* Z direction */
	glVertex3f(0.0f, 0.0f, 8.0f);
	glVertex3f(0.0f, 1.0f, 7.0f);
	glVertex3f(0.0f, 0.0f, 8.0f);
	glVertex3f(0.0f, -1.0f, 7.0f);

	glEnd();

	/* Draw childless bone pivots darker */
	if (mBones[id].numChildren > 0)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.5, 0.5, 0.5);
	}

	glBegin(GL_POINTS);
	glVertex3fv(pos);
	glEnd();

	if (mBones[id].numChildren > 0)
	{
		for (i = 0; i < mNumBones; ++i)
		{
			if (i == mBones[i].parentIndex)
				continue;
			
			if (mBones[i].parentIndex == id)
			{
				renderBone(i);
			}
		}
	}

	glPopMatrix();

#ifdef INTERACTIVE_BONE_RENDER
	glPopMatrix();
#endif

	if (id == 0)
	{
#ifdef FUDGE_BONE_RENDER
		glPopMatrix();
#endif
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(1.0);
		glPointSize(1.0);
	}
#endif
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void PSKModelRenderer::convertBoneAngles()
{
	unsigned int i;


	if (!mBoneRotationCache || !mModel)
	{
		printf("convertBoneAngles()> Error: No PSKModel loaded?\n");
		return;
	}

	/* Use PSK bones */
	mBones = mModel->mBones;

	printf("Converting quaternions to [%s]\n",
			 (mFlags & fConvertEuler) ? "Eular angles" : "Axis angles");

	for (i = 0; i < mNumBones; ++i)
	{
		if (mFlags & fConvertEuler)
		{
			// eular angles
			quaternion_to_euler_angles(mBones[i].restDir[0],
									   mBones[i].restDir[1],
									   mBones[i].restDir[2],
									   mBones[i].restDir[3],
									   &mBoneRotationCache[i*4],    // x
									   &mBoneRotationCache[i*4+1],  // y
									   &mBoneRotationCache[i*4+2]); // z
			
			// Convert radians to degrees
			mBoneRotationCache[i*4] *= 57.295779513082323;
			mBoneRotationCache[i*4+1] *= 57.295779513082323;
			mBoneRotationCache[i*4+2] *= 57.295779513082323;
		}
		else
		{
			// axis angles
			quaternion_to_axis_angles(mBones[i].restDir[3], // qw is dir[3]?
									  mBones[i].restDir[0], // qx
									  mBones[i].restDir[1], // qy
									  mBones[i].restDir[2], // qz
									  &mBoneRotationCache[i*4],    // theta
									  &mBoneRotationCache[i*4+1],  // x
									  &mBoneRotationCache[i*4+2],  // y  
									  &mBoneRotationCache[i*4+3]); // z

			// Convert radians to degrees, negated to account for UT coords
			mBoneRotationCache[i*4] *= -57.295779513082323; 
		}
	}
}


void PSKModelRenderer::convertBoneAnglesPSA(unsigned int frame)
{
	unsigned int i, b, start, end;
	float w, x, y, z, w2, x2, y2, z2;
	float qw, qx, qy, qz;


	if (!mBoneRotationCache || !mAnimation)
	{
		printf("convertBoneAnglesPSA()> Error: No PSA Animation loaded?\n");
		return;
	}

	/* Use PSA bones */
	mBones = mAnimation->mBones;

	/* Check here for frame out of bounds */
	if (frame > mAnimation->mNumKeyFrames / mAnimation->mNumBones)
	{
		printf("convertBoneAnglesPSA()> Error: Frame out of bounds\n");
		return;
	}

	printf("Converting quaternions to [%s] for PSA frame [%u]\n",
			 (mFlags & fConvertEuler) ? "Eular angles" : "Axis angles", frame);

	/* Update frame tracker */
	mAnimationFrame = frame;

	/* Compute start and end bones from list */
	start = (mAnimationFrame*mAnimation->mNumBones);
	end = start + mAnimation->mNumBones;

	for (i = start, b = 0; i < end; ++i, ++b)
	{
		w = mBones[b].restDir[3];
		x = mBones[b].restDir[0];
		y = mBones[b].restDir[1];
		z = mBones[b].restDir[2];

		if (mAnimationFrame > 0)
		{
			w2 = mAnimation->mKeyFrames[i].dir[3];
			x2 = mAnimation->mKeyFrames[i].dir[0];
			y2 = mAnimation->mKeyFrames[i].dir[1];
			z2 = mAnimation->mKeyFrames[i].dir[2];

			//qw = w * w2 - x * x2 - y * y2 - z * z2;
			//qx = w * x2 + x * w2 + y * z2 - z * y2;
			//qy = w * y2 + y * w2 + z * x2 - x * z2;
			//qz = w * z2 + z * w2 + x * y2 - y * x2;

			qw = w2;  qx = x2;  qy = y2;  qz = z2;
		}
		else
		{
			qw = w;  qx = x;  qy = y;  qz = z;
		}

		if (mFlags & fConvertEuler)
		{
			/* FIXME: No support for frames */

			// eular angles
			quaternion_to_euler_angles(qw, qx, qy, qz,
									   &mBoneRotationCache[b*4],    // x
									   &mBoneRotationCache[b*4+1],  // y
									   &mBoneRotationCache[b*4+2]); // z
			
			// Convert radians to degrees
			mBoneRotationCache[b*4] *= 57.295779513082323;
			mBoneRotationCache[b*4+1] *= 57.295779513082323;
			mBoneRotationCache[b*4+2] *= 57.295779513082323;
		}
		else
		{
			// axis angles
			quaternion_to_axis_angles(qw, qx, qy, qz,
									  &mBoneRotationCache[b*4],    // theta
									  &mBoneRotationCache[b*4+1],  // x
									  &mBoneRotationCache[b*4+2],  // y  
									  &mBoneRotationCache[b*4+3]); // z

			// Convert radians to degrees, negated to account for UT coords
			mBoneRotationCache[b*4] *= -57.295779513082323;
		}
	}

	/* Setup matrices for mesh deformation use */
	setupWorldMatrices(frame);

	/* Reset vertices and then deform them */
	copyVertices();
	transformVertices(); // FIXME: pass frame time later
}


void PSKModelRenderer::copyVertices()
{
	unsigned int i;


	if (!mVertexTransformCache || !mModel)
	{
		printf("copyVertices()> Error: No PSKModel loaded?\n");
		return;
	}

	for (i = 0; i < mModel->mNumVertices; ++i)
	{
		mVertexTransformCache[i*3]   = mModel->mVertices[i*3];   // 0
		mVertexTransformCache[i*3+1] = mModel->mVertices[i*3+1]; // 2
		mVertexTransformCache[i*3+2] = mModel->mVertices[i*3+2]; // 1
	}
}


void PSKModelRenderer::setAnimation(PSAAnimation *anim)
{
	if (!anim || !mModel || anim->mNumBones != mModel->mNumBones)
	{
		printf("Error: Unable to load PSA Animation, [%s]\n",
				 (!mModel) ? "No PSK model loaded" : 
				 (!anim) ? "Null PSA given" :
				 "PSA animation doesn't match loaded PSK model");
		return;
	}

	mAnimation = anim;

	/* Bones are forced to same size to match model now,
		and there is no need to default to PSA skeletion now*/

	/* Wow, now I look smart able to switch out bones on the fly */
	//mBones = anim->mBones;
	//
	/* Setup bones */
	//if (mBoneRotationCache)
	//	delete [] mBoneRotationCache;
	//
	//mBoneRotationCache = new float[anim->mNumBones*4];

	/* No use loading PSA frame 0 anymore by default */
	//convertBoneAnglesPSA(0);
}


void PSKModelRenderer::setModel(PSKModel *model)
{
	unsigned int i;


	if (!model)
	{
		return;
	}

	mModel = model;

	/* Setup cheesy unneeded attributes to simulate inhertence */
	mNumFrames = model->mNumFrames;
	mNumVertices = model->mNumVertices;
	mNumFaces = model->mNumFaces;
	mNumVTXWs = model->mNumVTXWs;
	mNumMaterials = model->mNumMaterials;
	mNumBones = model->mNumBones;
	mNumWeights = model->mNumWeights;
	mVTXWs = model->mVTXWs;
	mFaces = model->mFaces;
	mMaterials = model->mMaterials;
	mBones = model->mBones;
	mWeights = model->mWeights;		

	/* Setup vertices */
	if (mVertexTransformCache)
		delete [] mVertexTransformCache;

	mVertexTransformCache = new float[model->mNumVertices*3];
	copyVertices();

	/* Setup bones */
	if (mBoneRotationCache)
		delete [] mBoneRotationCache;
	
	mBoneRotationCache = new float[model->mNumBones*4];
	convertBoneAngles();

	/* Setup rest / transform matrices */
	mNumMatrices = model->mNumBones;

	mInvertedMatrices = new float*[mNumMatrices];

	for (i = 0; i < mNumMatrices; ++i)
		mInvertedMatrices[i] = new float[16];

	mWorldMatrices = new float*[mNumMatrices];

	for (i = 0; i < mNumMatrices; ++i)
		mWorldMatrices[i] = new float[16];

	/* Setup matrices for mesh deformation use */
	setupRestMatrices(0);
}


void PSKModelRenderer::setupRestMatrices(unsigned int id)
{
#ifdef HAVE_OPENGL
	unsigned int i;
	float theta, x, y, z;
	float m[16];


	if (!mModel || !mModel->mBones)
		return;

	glPushMatrix();

	glTranslatef(mModel->mBones[id].restLoc[0],
					 mModel->mBones[id].restLoc[1],
					 mModel->mBones[id].restLoc[2]);

	/* Get the rotation in axis angles */
	quaternion_to_axis_angles(mModel->mBones[id].restDir[0], // qw
									  mModel->mBones[id].restDir[1], // qx
									  mModel->mBones[id].restDir[2], // qy
									  mModel->mBones[id].restDir[3], // qz
									  &theta, &x, &y, &z);

	/* Convert radians to degrees */
	theta *= -57.295779513082323;

	/* OpenGL axis angles rotation */
	glRotatef(theta, x, y, z);

	/* Store this bones tranform matrix inverted to 
		get bone rest position of model vertices */
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	invert_matrix(m, mInvertedMatrices[id]);

	if (mBones[id].numChildren > 0)
	{
		for (i = 0; i < mNumBones; ++i)
		{
			if (i == mBones[i].parentIndex)
				continue;
			
			if (mBones[i].parentIndex == id)
			{
				setupRestMatrices(i);
			}
		}
	}

	glPopMatrix();
#endif
}


/* Currently doesn't do what is says it does heh */
void PSKModelRenderer::setupWorldMatrices(unsigned int id)
{
#ifdef HAVE_OPENGL
	unsigned int i, f;


	glPushMatrix();

	/* Handle PSA animation use */
	if (mAnimationFrame > 0 &&
		 mAnimation && mAnimation->mBones == mBones)
	{
		f = id + (mAnimationFrame*mAnimation->mNumBones); 
		
		glTranslatef(mAnimation->mKeyFrames[f].trans[0],
						 mAnimation->mKeyFrames[f].trans[1],
						 mAnimation->mKeyFrames[f].trans[2]);
	}
	else
	{
		glTranslatef(mBones[id].restLoc[0],
						 mBones[id].restLoc[1],
						 mBones[id].restLoc[2]);
	}

	if (mFlags & fConvertEuler)
	{
		// euler angles
		glRotatef(mBoneRotationCache[id*4],   1, 0, 0);
		glRotatef(mBoneRotationCache[id*4+1], 0, 1, 0);
		glRotatef(mBoneRotationCache[id*4+2], 0, 0, 1);
	}
	else
	{
		// axis angles
		glRotatef(mBoneRotationCache[id*4],
					 mBoneRotationCache[id*4+1],
					 mBoneRotationCache[id*4+2],
					 mBoneRotationCache[id*4+3]);
	}

	/* Store world transform matrix */
	glGetFloatv(GL_MODELVIEW_MATRIX, mWorldMatrices[id]);

	if (mBones[id].numChildren > 0)
	{
		for (i = 0; i < mNumBones; ++i)
		{
			if (i == mBones[i].parentIndex)
				continue;
			
			if (mBones[i].parentIndex == id)
			{
				setupWorldMatrices(i);
			}
		}
	}

	glPopMatrix();
#elif FIXME_SOON
	unsigned int i, j;
	float x, y, z;
	float m[16], m2[16], m3[16];


	/* Currently inline transforms all vertices to produce 
		one deformed mesh keyframe */
	if (id == 0)
		copyVertices();

	/* Rest skeleton matrices */
   m[ 0]=1;  m[ 1]=0;  m[ 2]=0;  m[ 3]=0;
   m[ 4]=0;  m[ 5]=1;  m[ 6]=0;  m[ 7]=0;
   m[ 8]=0;  m[ 9]=0;  m[10]=1;  m[11]=0;
   m[12]=mModel->mBones[id].restLoc[0]; m[13]=mModel->mBones[id].restLoc[1]; m[14]=mModel->mBones[id].restLoc[2]; m[15]=1;

	quaternion_to_matrix(mModel->mBones[id].restDir[3], // qw
								mModel->mBones[id].restDir[0], // qx
								mModel->mBones[id].restDir[1], // qy
								mModel->mBones[id].restDir[2], // qz
								m2);
	
	multiply_matrix(m, m2, m3);

	if (id == 0)
	{
		invert_matrix(m3, mInvertedMatrices[id]);
	}
	else
	{
		invert_matrix(m3, m2);
		multiply_matrix(mInvertedMatrices[mModel->mBones[id].parent], 
							 m2, mInvertedMatrices[id]);	
	}

	/* Transformed skeleton matrices */
   m[ 0]=1;  m[ 1]=0;  m[ 2]=0;  m[ 3]=0;
   m[ 4]=0;  m[ 5]=1;  m[ 6]=0;  m[ 7]=0;
   m[ 8]=0;  m[ 9]=0;  m[10]=1;  m[11]=0;
   m[12]=mAnimation->mBones[id].restLoc[0]; m[13]=mAnimation->mBones[id].restLoc[1]; m[14]=mAnimation->mBones[id].restLoc[2]; m[15]=1;

	quaternion_to_matrix(mAnimation->mBones[id].restDir[3], // qw
								mAnimation->mBones[id].restDir[0], // qx
								mAnimation->mBones[id].restDir[1], // qy
								mAnimation->mBones[id].restDir[2], // qz
								m2);

	multiply_matrix(m, m2, m3);

	/* Transform vertices */
	//multiply_matrix(mInvertedMatrices[id], m3, m);

	if (id == 0)
	{
		multiply_matrix(mInvertedMatrices[id], m3, m);
	}
	else
	{
		multiply_matrix(mInvertedMatrices[id], m3, m2);
		multiply_matrix(mInvertedMatrices[mModel->mBones[id].parent], 
							 m2, m);
	}	
	//copy_matrix(m, mInvertedMatrices[id]);

	for (i = 0; i < mModel->mNumWeights; ++i)
	{
		if (mModel->mWeights[i].boneIndex == id)
		{
			j = mModel->mWeights[i].vertexIndex;

			x = mVertexTransformCache[j*3+0] * mModel->mWeights[i].weight;
			y = mVertexTransformCache[j*3+1] * mModel->mWeights[i].weight;
			z = mVertexTransformCache[j*3+2] * mModel->mWeights[i].weight;

			mVertexTransformCache[j*3+0] = m[0]*x + m[1]*y + m[ 2]*z + m[ 3];
			mVertexTransformCache[j*3+1] = m[4]*x + m[5]*y + m[ 6]*z + m[ 7];
			mVertexTransformCache[j*3+2] = m[8]*x + m[9]*y + m[10]*z + m[11];
		}
	}

	/* Transform by rest of model */
	for (i = 0; i < mAnimation->mNumBones; ++i)
	{
		if (i == mAnimation->mBones[i].parent)
			continue;

		if (mAnimation->mBones[i].parent == id)
		{
			setupMatrixTransforms(i);
		}
	}
#endif
}


void PSKModelRenderer::transformVertices()
{
	unsigned int id, i, j;
	float x, y, z;
	float m[16];

	for (id = 0; id < mModel->mNumBones; ++id)
	{
		multiply_matrix(mWorldMatrices[id], mInvertedMatrices[id], m);
		//copy_matrix(mWorldMatrices[id], m);

		for (i = 0; i < mModel->mNumWeights; ++i)
		{
			if (mModel->mWeights[i].boneIndex == id)
			{
				j = mModel->mWeights[i].vertexIndex;
				
				x = mVertexTransformCache[j*3+0] * mModel->mWeights[i].weight;
				y = mVertexTransformCache[j*3+1] * mModel->mWeights[i].weight;
				z = mVertexTransformCache[j*3+2] * mModel->mWeights[i].weight;
				
				mVertexTransformCache[j*3+0] = m[0]*x + m[4]*y + m[ 8]*z + m[12];
				mVertexTransformCache[j*3+1] = m[1]*x + m[5]*y + m[ 9]*z + m[13];
				mVertexTransformCache[j*3+2] = m[2]*x + m[6]*y + m[10]*z + m[14];
			}
		}
	}
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_PSKMODEL

PSKModel gModel;
PSAAnimation gAnim;


#ifdef HAVE_OPENGL
#   include "SDLUnitTest.h"

PSKModelRenderer gModelRenderer;
SDLUnitTest ut;
UTPackage utpak;

void gameFrame()
{
}

void handleKeyReleaseEvent(unsigned int button, unsigned int mod)
{
}

void handleEventCode(unsigned int event)
{
	switch (event)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;
	}

	handleKeyPressEvent(event, 0);
}

void displayFrame()
{
	static float lastTime = 0.0f;
	float time;

	glClearColor(0.3, 0.3, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0,    0.0, -256.0, 
			  0.0,    8.0,    0.0, 
			  0.0,    1.0,    0.0);

	time = getTicks() * 0.1f;

	if (time - lastTime > 5.0f)
	{
		lastTime = time;

		if (ut.mFlags & SDLUnitTest::fAutoRotate)
			++ut.mYaw;
	}

	/* Start solid color rendering of grid and axis lines */
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	/* Draw light symbol */
	glPushMatrix();
	glTranslatef(ut.mLightPos[0], ut.mLightPos[1], ut.mLightPos[2]);
	glDrawLight(5.0);
	glPopMatrix();

	/* Draw transformed origin axis and grid/floor */
	glPushMatrix();
	glTranslatef(0.0f, -128.0f, 512.0f);
	glRotatef(ut.mPitch, 1, 0, 0);
	glRotatef(ut.mYaw,   0, 1, 0);
	glDrawAxis3f(30.0, 26.25, 3.75);
	glDrawGrid(500.0, 50.0);
	glPopMatrix();

	/* End solid color rendering, restore old rendering flags */
	glPopAttrib();

	/* Draw model */
	glEnable(GL_TEXTURE_2D);

	if (ut.mFlags & SDLUnitTest::fLighting)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, ut.mLightPos);
		glEnable(GL_LIGHTING);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, -120.0f, 512.0f);
	glRotatef(ut.mPitch, 1, 0, 0);
	glRotatef(ut.mYaw,   0, 1, 0);
	glScalef(ut.mScale, ut.mScale, ut.mScale);
	// glDrawAxis3f(30.0, 26.25, 3.75);
	gModelRenderer.render();
	glPopMatrix();

	/* Render 2d text and masks now */
#ifdef HAVE_SDL_TTF
	if (ut.mFlags & SDLUnitTest::fWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnterMode2d(ut.getWidth(), ut.getHeight());
	ut.drawWidgets();
	glExitMode2d();

	if (ut.mFlags & SDLUnitTest::fWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glFlush();
	ut.swapBuffersGL();
}


void initScene(int argc, char *argv[])
{
	/* OpenGL setup */
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT); 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	/* Texture setup */
	ut.mTexture.reset();
	ut.mTexture.setFlag(Texture::fUseMipmaps);
	ut.mTexture.setMaxTextureCount(64);
	printf("Loading textures: ");

	if (argc > 3 && argv[3] && argv[3][0])
	{
		gModelRenderer.mFlags |= PSKModelRenderer::fRenderTexture;
		for (unsigned int i = 0; i < gModel.mNumMaterials; ++i)
		{
			char texture[256];
			sprintf(texture, "%s/%s.tga", argv[3], gModel.mMaterials[i].name);
			gModelRenderer.mTextures[i] = ut.mTexture.loadTGA(texture);
			
			if (gModelRenderer.mTextures[i] < 0)
			{
				sprintf(texture, "%s/%s.png", argv[3], 
						gModel.mMaterials[i].name);
				gModelRenderer.mTextures[i] = ut.mTexture.loadPNG(texture);
			}
		}
	}

	printf(" done\n\n");

#ifdef HAVE_SDL_TTF
	ut.mTexture.loadFontTTF("font.ttf", 32, 126 - 32);
	bufferedPrintf(ut.mMessage, 256, "PSKModel Unit Test");
#endif

	printf("\n");
	printf("----------------------------------\n");
	printf("ESC        - Exit\n");
	printf("F1         - Take screenshot\n");
	printf("F10        - Set window to 640x460\n");
	printf("F11        - Set window to 800x600\n");
	printf("F12        - Set window to 1024x768\n");
	printf("ALT+ENTER  - Toogle fullscreen\n");
	printf("UP/DOWN    - Adjust scene pitch\n");
	printf("RIGHT/LEFT - Adjust scene yaw\n");
	printf("-/=        - Adjust model scaling\n");
	printf(";/'        - Switch PSA frame (EXPERIMENTAL)\n");
	printf("/          - Show PSK frame\n");
	printf("----------------------------------\n");
	printf("1 - Toggle polygon rendering\n");   
	printf("2 - Toggle polygon render debugging\n"); 
	printf("3 - Toggle textured polygons\n");  
	printf("4 - Toggle points\n");  
	printf("7 - Toggle scene rotation\n");
	printf("8 - Toggle alpha blending\n");      
	printf("8 - Toggle OpenGL Lighting\n"); 
	printf("r - Reset mesh to default\n");  
	printf("w - Toggle wireframe rendering\n");
#ifdef INTERACTIVE_BONE_RENDER
	printf("g,h - Select bone Id\n");
	printf("z,x,c, b,n,m - Adjust bone rotatation\n");
#endif
	printf("f - Convert rotation to angles to render correctly\n");
	printf("i,k,j,l - Move light\n");
	printf("----------------------------------\n");
}


void handleKeyPressEvent(unsigned int key, unsigned int mod)
{
	switch (key)
	{
	case SDLK_ESCAPE: /* 0x1B, 27d, ESC */
		exit(0);
		break;
	case SDLK_F1:
		ut.mTexture.glScreenShot("PSKModel.test", 
								ut.getWidth(), ut.getHeight());
		bufferedPrintf(ut.mMessage, 256, "Took screenshot...");
		break;
	case SDLK_F2:
		ut.toggleFullscreen();
		break;
	case SDLK_F9:
		ut.resize(640, 460);
		bufferedPrintf(ut.mMessage, 256, "Switching to 640x480 resolution");
		break;
	case SDLK_F10:
		ut.resize(800, 600);
		bufferedPrintf(ut.mMessage, 256, "Switching to 800x600 resolution");
		break;
	case SDLK_F11:
		ut.resize(1024, 768);
		bufferedPrintf(ut.mMessage, 256, "Switching to 1024x768 resolution");
		break;
	case SDLK_F12:
		ut.resize(1280, 1024);
		bufferedPrintf(ut.mMessage, 256, "Switching to 1280x1024 resolution");
		break;
	case SDLK_RIGHT:
		++ut.mYaw;
		break;
	case SDLK_LEFT:
		--ut.mYaw;
		break;
	case SDLK_UP:
		++ut.mPitch;
		break;
	case SDLK_DOWN:
		--ut.mPitch;
		break;
	case 't':
		ut.setShowCursor(true);
		break;
	case 'f':
		printf("Converting bone angles...\n");
		gModelRenderer.convertBoneAngles();
		break;
#ifdef INTERACTIVE_BONE_RENDER
	case 'g':
		--gBoneId;
		printf("gBoneId = %i\n", gBoneId);
		bufferedPrintf(ut.mMessage, 256, "gBoneId = %i\n", gBoneId);
		break;
	case 'h':
		++gBoneId;
		printf("gBoneId = %i\n", gBoneId);
		bufferedPrintf(ut.mMessage, 256, "gBoneId = %i\n", gBoneId);
		break;
	case 'z':
		--gBoneRotHackX;
		printf("gBoneRotHackX = %f\n", gBoneRotHackX);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackX = %f\n", gBoneRotHackX);
		break;
	case 'x':
		--gBoneRotHackY;
		printf("gBoneRotHackY = %f\n", gBoneRotHackY);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackY = %f\n", gBoneRotHackY);
		break;
	case 'c':
		--gBoneRotHackZ;
		printf("gBoneRotHackZ = %f\n", gBoneRotHackZ);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackZ = %f\n", gBoneRotHackZ);
		break;
	case 'b':
		++gBoneRotHackX;
		printf("gBoneRotHackX = %f\n", gBoneRotHackX);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackX = %f\n", gBoneRotHackX);
		break;
	case 'n':
		++gBoneRotHackY;
		printf("gBoneRotHackY = %f\n", gBoneRotHackY);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackY = %f\n", gBoneRotHackY);
		break;
	case 'm':
		++gBoneRotHackZ;
		printf("gBoneRotHackZ = %f\n", gBoneRotHackZ);
		bufferedPrintf(ut.mMessage, 256, "gBoneRotHackZ = %f\n", gBoneRotHackZ);
		break;
#endif
	case '-':
		if (ut.mScale * 0.15 > 0.000001)
			ut.mScale *= 0.15f;
		break;
	case '=':
		ut.mScale *= 1.15f;
		break;
	case '0':
		gModelRenderer.mFlags ^= PSKModelRenderer::fRenderBones;
		break;
	case '1':
		gModelRenderer.mFlags ^= PSKModelRenderer::fRenderFaces;
		break;
	case '2':
		gModelRenderer.mFlags ^= PSKModelRenderer::fDebugFaceRender;
		break;
	case '3':
		gModelRenderer.mFlags ^= PSKModelRenderer::fRenderTexture;

		if (gModelRenderer.mFlags & PSKModelRenderer::fRenderTexture)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		break;
	case '4':
		gModelRenderer.mFlags ^= PSKModelRenderer::fRenderPoints;
		break;
	case '7':
		ut.mFlags ^= SDLUnitTest::fAutoRotate;
		break;
	case '8':
		ut.mFlags ^= SDLUnitTest::fAlphaBlending;

		if (ut.mFlags & SDLUnitTest::fAlphaBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glAlphaFunc(GL_GREATER, 0);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		printf("%sabled alpha blending\n",
				(ut.mFlags & SDLUnitTest::fAlphaBlending) ? "En" : "Dis");
		break;
	case '9':
		ut.mFlags ^= SDLUnitTest::fLighting;

		if (ut.mFlags & SDLUnitTest::fLighting)
		{
			static bool firsttime = true;


			glEnable(GL_LIGHTING);

			if (firsttime)
			{
				float specular[] = { 1.0, 1.0, 1.0, 1.0 };
				float ambient[] = { 0.64, 0.64, 0.64, 1.0 };
				float shine[] = { 50.0 };


				// Caustic fx possible with mod amb?
				glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
				glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE,   ambient);
				glMaterialfv(GL_FRONT, GL_SHININESS, shine);
				glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
				glEnable(GL_LIGHT0);

				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

				firsttime = false;
			}
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
		break;
	case '/':
		gModelRenderer.convertBoneAngles();
		break;
	case '\'':
		gModelRenderer.convertBoneAnglesPSA(gModelRenderer.mAnimationFrame+1);
		break;
	case ';':
		gModelRenderer.convertBoneAnglesPSA(gModelRenderer.mAnimationFrame-1);
		break;
	case 'r':
		gModelRenderer.copyVertices();
		break;
	case 'i':
		ut.mLightPos[1] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'k':
		ut.mLightPos[1] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'j':
		ut.mLightPos[0] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'l':
		ut.mLightPos[0] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'o':
		ut.mLightPos[2] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'u':
		ut.mLightPos[2] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'w':
		ut.mFlags ^= SDLUnitTest::fWireframe;

		if (ut.mFlags & SDLUnitTest::fWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		printf("%sabled wireframe rendering\n", 
				(ut.mFlags & SDLUnitTest::fWireframe) ? "En" : "Dis");
		break;
	}
}

#endif

///////////////////////////////////////////////////


void print_pskmodel_help()
{
	printf("PSKModel.test <options> file.psk [skindir] [file.psa]\n");
	printf("   eg 'PSKModel.test lg model.psk textureDir/ anim.psa'\n");
	printf("\tl - Load model\n");
	printf("\tg - Start GL visual debugger\n");
	printf("\tv - Debug Vertex loader\n");
	printf("\tm - Debug Material loader\n");
	printf("\tf - Debug Face loader\n");
	printf("\tw - Debug Weight loader\n");
	printf("\tb - Debug Bone loader\n");
	printf("\tu - Debug UV loader\n");
	printf("\tp - Debug PSA loader\n");
	printf("\t. - PSA debugger (PSKModel.test . file.psa)\n");
	printf("\t0 - UMOD debugger (PSKModel.test 0 file.utx)\n");
	printf("\t1 - UMOD ascii hex dump  (PSKModel.test 1 file.utx)\n");
	printf("\t2 - UMOD bin obj dump    (PSKModel.test 2 file.utx)\n");
	printf("\t3 - Decrypt pak only     (PSKModel.test 3 file.utx)\n");
	printf("\th - Help\n");
}


int runPSKModelUnitTest(int argc, char *argv[])
{
	bool gl = false;

	ut.mResource.registerSymbolValue("EVENT_SHOWBONES",    (int)'0');
	ut.mResource.registerSymbolValue("EVENT_WIREFRAME",    (int)'w');
	ut.mResource.registerSymbolValue("EVENT_QUIT",         SDLK_ESCAPE);
	ut.mResource.registerSymbolValue("EVENT_SCREENSHOT",   SDLK_F1);
	ut.mResource.registerSymbolValue("EVENT_FULLSCREEN",   SDLK_F2);
	ut.mResource.registerSymbolValue("EVENT_RES640x480",   SDLK_F9);
	ut.mResource.registerSymbolValue("EVENT_RES800x600",   SDLK_F10);
	ut.mResource.registerSymbolValue("EVENT_RES1024x768",  SDLK_F11);
	ut.mResource.registerSymbolValue("EVENT_RES1280x1024", SDLK_F12);
	ut.mResource.evalFile("src/PSKModelUnitTest.lsp");

	ut.mFlags |= SDLUnitTest::fAutoRotate;
	ut.mYaw = 0.0f;
	ut.mPitch = -20.0f;
	ut.mScale = 1.75f;
	ut.mLightPos[0] = 0.0;
	ut.mLightPos[1] = 512.0;
	ut.mLightPos[2] = 1024.0;
	ut.mLightPos[3] = 0.0;

	if (argc > 2)
	{
		for (int i = 0; argv[1][i] != 0; ++i)
		{
			switch (argv[1][i])
			{
			case 'P':
				utpak.load(argv[2]);
				break;
			case '3':  /* Bindump UTX to disk decrypted */
				utpak.setFlags(UTPackage::fDecryptOnly);
				utpak.load(argv[2]);
				return 0;
				break;
			case '2':  /* Bindump UTX to disk */
				utpak.setFlags(UTPackage::fDiskDump);
				utpak.load(argv[2]);
				return 0;
				break;
			case '1':  /* Hexdump UTX to stdout */
				utpak.setFlags(UTPackage::fHexDump);
				utpak.load(argv[2]);
				return 0;
				break;
			case '0':
				utpak.load(argv[2]);
				return 0;
				break;
			case '.':
				gAnim.mFlags = (PSAAnimation::fDebugBones |
								PSAAnimation::fDebugAnimInfos | 
								PSAAnimation::fDebugKeyFrames);
				gAnim.load(argv[2]);
				break;
			case 'p':
				gAnim.mFlags = (PSAAnimation::fDebugBones |	
								PSAAnimation::fDebugAnimInfos | 
								PSAAnimation::fDebugKeyFrames);
				break;
			case 'g':
				gl = true;
				break;
			case 'w':
				gModel.mFlags |= PSKModel::fDebugWeightLoad;
				break;
			case 'b':
				gModel.mFlags |= PSKModel::fDebugBoneLoad;
				break;
			case 'v':
				gModel.mFlags |= PSKModel::fDebugPointLoad;
				break;
			case 'm':
				gModel.mFlags |= PSKModel::fDebugMattLoad;
				break;
			case 'f':
				gModel.mFlags |= PSKModel::fDebugFaceLoad;
				break;
			case 'u':
				gModel.mFlags |= PSKModel::fDebugUVLoad;
				break;
			case 'h':
				print_pskmodel_help();
				break;
			}

			/* Load PSK model */
			gModel.load(argv[2]);

			/* Load PSK model to renderer */
			gModelRenderer.mFlags |= PSKModelRenderer::fRenderFaces;
			gModelRenderer.setModel(&gModel);

			if (argc > 4)
			{
				gAnim.load(argv[4]);
				gModelRenderer.setAnimation(&gAnim);
			}

			if (gl)
			{
#ifdef HAVE_OPENGL
				ut.initVideo(800, 600, false);
				initScene(argc, argv);
				ut.runGame();
#else
				printf("Build not OpenGL enabled\n");
#endif
			}
		}
	}
	else
	{
		printf("%s:\n", argv[0]);
		print_pskmodel_help();
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[PSKModel class test]\n");
	runPSKModelUnitTest(argc, argv);

	return 0;
}
#endif

#ifdef FREYJA_PLUGINS
#include <freyja/FreyjaPlugin.h>
#include <mstl/Map.h>
#include <hel/Matrix.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>


extern "C" {

	int freyja_model__psk_check(char *filename);
	int freyja_model__psk_import(char *filename);
	int freyja_model__psk_export(char *filename);
	int import_model(char *filename);
}



int import_model(char *filename)
{
	if (!freyja_model__psk_check(filename))
		return freyja_model__psk_import(filename);

	return -1;
}


int freyja_model__psk_check(char *filename)
{
	FILE *f = fopen(filename, "rb");;
	char buffer[32];


	if (!f)
	{
		perror(filename);
		return -1;
	}

    fread(buffer, 1, 28, f);
    buffer[8] = 0;

	if (strncmp("ACTRHEAD", buffer, 8) != 0)
	{
		//printf("Not a valid PSK Model\n");
		return -1;
	}																		   

	return 0;
}


int freyja_model__psk_import(char *filename)
{
	PSKModel psk;
	unsigned int i, j, v, t, m;
	const float scale = 0.1f;
	Vector<unsigned int> transV;
	Quaternion q, r, r2;
	Vector3d u;
	Matrix M;
	vec4_t wxyz;
	long index;


	if (freyja_model__psk_check(filename) < 0)
		return -1;

	if (psk.load(filename) < 0)
		return -2;
	
	/* Start a new mesh */
	freyjaBegin(FREYJA_MODEL);
	freyjaBegin(FREYJA_MESH);
	m = freyjaGetCurrent(FREYJA_MESH);

	/* Start a new vertex group */
	freyjaBegin(FREYJA_VERTEX_GROUP);
	
	for (i = 0; i < psk.mNumVertices; ++i)
	{
		/* Store vertices in group as freyja XYZ */
		v = freyjaVertex3f(psk.mVertices[i*3+0]*scale, 
						   psk.mVertices[i*3+2]*scale, 
						   -psk.mVertices[i*3+1]*scale);

		/* Generates id translator list */
		transV.pushBack(v);
	}
	
	freyjaEnd(); // FREYJA_GROUP
	
	//for (i = 0; i < psk.mNumVTXWs; ++i)
	//{
	//	/* Store texels */
	//	t = freyjaTexelStore2f(psk.mVTXWs[i].uv[0], psk.mVTXWs[i].uv[1]);
	//	
	//	/* Generates id translator list */
	//	trans2.Add(i, t);
	//}
	
	for (i = 0; i < psk.mNumFaces; ++i)
	{
		/* Start a new polygon */
		freyjaBegin(FREYJA_POLYGON);

		/* Store vertices and texels by true id, using translator lists */
		freyjaPolygonVertex1i(transV[psk.mVTXWs[psk.mFaces[i].x].vertex]);
		t = freyjaTexCoord2f(psk.mVTXWs[psk.mFaces[i].x].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].x].uv[1]);
		freyjaPolygonTexCoord1i(t);

		freyjaPolygonVertex1i(transV[psk.mVTXWs[psk.mFaces[i].y].vertex]);
		t = freyjaTexCoord2f(psk.mVTXWs[psk.mFaces[i].y].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].y].uv[1]);
		freyjaPolygonTexCoord1i(t);

		freyjaPolygonVertex1i(transV[psk.mVTXWs[psk.mFaces[i].z].vertex]);
		t = freyjaTexCoord2f(psk.mVTXWs[psk.mFaces[i].z].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].z].uv[1]);
		freyjaPolygonTexCoord1i(t);
		
		freyjaPolygonMaterial1i(psk.mFaces[i].material);
		
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	/*** Import skeleton *********************/

	for (i = 0; i < psk.mNumWeights; ++i)
	{
		freyjaVertexWeight(transV[psk.mWeights[i].vertexIndex], 
						   psk.mWeights[i].weight,
						   psk.mWeights[i].boneIndex);
	}


	freyjaBegin(FREYJA_SKELETON);

	for (i = 0; i < psk.mNumBones; ++i)
	{
		if (!i)
		{
			q = Quaternion(psk.mBones[i].restDir[3],   // qw
						   -psk.mBones[i].restDir[0],  // qx
						    psk.mBones[i].restDir[1],  // qy
						   -psk.mBones[i].restDir[2]); // qz
		}
		else
		{
			q = Quaternion(psk.mBones[i].restDir[3],  // qw
						   psk.mBones[i].restDir[0],  // qx
						   psk.mBones[i].restDir[1],  // qy
						   psk.mBones[i].restDir[2]); // qz
		}

		/* Start a new bone */
		freyjaBegin(FREYJA_BONE);
		index = freyjaGetCurrent(FREYJA_BONE);
		freyjaBoneFlags1i(index, 0x0);
		freyjaBoneName1s(index, psk.mBones[i].name);
		freyjaBoneTranslate3f(index, 
							  psk.mBones[i].restLoc[0]*scale,
							  psk.mBones[i].restLoc[1]*scale,
							  psk.mBones[i].restLoc[2]*scale);

		j = i;
		r.setIdentity();
		printf("%i> ", 1);
		while (j > 0)
		{
			j = psk.mBones[j].parentIndex;  

			if (j == 1)
			{
				break;
			}

			printf("%i ", j);

			r2 = Quaternion(psk.mBones[j].restDir[3],  // qw
							psk.mBones[j].restDir[0],  // qx
							psk.mBones[j].restDir[1],  // qy
							psk.mBones[j].restDir[2]); // qz
			//	r2.normalize();
			r = r * r2;
		}
		printf("\n");

		//	q.normalize();
		q = q * r.conjugate();
		q.getQuaternion4fv(wxyz);
		freyjaBoneRotateQuatWXYZ4fv(index, wxyz);

		switch (i)
		{
		case 0:
			{
				freyjaBoneTranslate3f(index,
									  psk.mBones[i].restLoc[0]*scale,
									  psk.mBones[i].restLoc[2]*scale,
									  psk.mBones[i].restLoc[1]*scale);
			}
			break;
		case 1:
			{
				r.set(helDegToRad(90.0f), 0, 1, 0);
				q = r * q;
				q.getQuaternion4fv(wxyz);
				freyjaBoneRotateQuatWXYZ4fv(index, wxyz);
			}
			break;
		default:
			;
		}

		/* Setup children */
		for (j = 0; j < psk.mNumBones; ++j)
		{
			if (psk.mBones[j].parentIndex == i && i != j)
			{
				freyjaBoneAddChild1i(index, j);
			}
		}

		freyjaEnd(); // FREYJA_BONE
	}

	freyjaEnd(); // FREYJA_SKELETON

	return 0;
}


int freyja_model__psk_export(char *filename)
{
	freyjaPrintError("freyja_model__psk_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);

	return -1;
}
#endif
