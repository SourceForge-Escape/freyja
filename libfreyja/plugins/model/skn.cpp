/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the SKN plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.12.29:
 * Mongoose - Found some specs and a ref skeleton on the web
 *            http://simtech.sourceforge.net/tech/file_formats_skn.htm
 *
 *            adult_skeleton.cmx from a 3rd party fan sdk
 *
 * 2004.12.24:
 * Mongoose - Created, based on my best guesses
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>


extern "C" {

	int freyja_model__skn_check(char *filename);
	int freyja_model__skn_import(char *filename);
	int freyja_model__skn_export(char *filename);
	int import_model(char *filename);
}

int import_model(char *filename)
{
	if (!freyja_model__skn_check(filename))
		return freyja_model__skn_import(filename);

	return -1;
}


int freyja_model__skn_check(char *filename)
{
	if (FreyjaFileReader::compareFilenameExtention(filename, ".skn") == 0)
		return 0;

	return -1;
}


typedef struct {

	char name[64];

} skn_bone_t;


typedef struct {

	long boneIndex;
	long vertexOffset;
	long vertexCount;
	long blendedVertexOffset;
	long blendedVertexCount;

} skn_bone_binding_t;


typedef struct {

	char name[16];
	long parent;
	vec3_t xyz;
	vec4_t wxyz;

} cmx_bone_t;


void cmx_bone_set(cmx_bone_t &bone,
				  char *name, long parent,
				  vec_t x, vec_t y, vec_t z,
				  vec_t qw, vec_t qx, vec_t qy, vec_t qz)
{
	strncpy(bone.name, name, 16);
	bone.parent = parent;
	bone.xyz[0] = x;
	bone.xyz[1] = y;
	bone.xyz[2] = z;
	bone.wxyz[0] = qw;
	bone.wxyz[1] = qx;
	bone.wxyz[2] = qy;
	bone.wxyz[3] = qz;
}


void cmx_import_adult_skeleton()
{
	const vec_t scale = 5.0;
	cmx_bone_t skeleton[29];
	long i, j, idx;


	/* Mongoose 2004.12.29, 
	 * in CMX format they're linked by string id, not a numeric */
	cmx_bone_set(skeleton[0], "ROOT", -1,
				 -5.88894e-005, 3.01362, 0.00807638,
				 -0.0262998, -0.706899, 0.0262789, -0.706336);

	cmx_bone_set(skeleton[1], "PELVIS", 0,
				 -0.0366778, -1.72489e-008, -2.62766e-012,
				 0.499999, 0.5, 0.5, -0.500001);
	
	cmx_bone_set(skeleton[2], "L_LEG", 1,
				 -4.26205e-007, 0.241228, 2.06089e-007,
				 0.0137635, 2.41525e-006, -0.999905, -3.00579e-007);
	
	cmx_bone_set(skeleton[3], "L_LEG1", 2,
				 1.44737, -3.40221e-010, -3.65496e-009,
				 -2.21297e-011, 0.0357504,-4.92061e-013, -0.999361);

	cmx_bone_set(skeleton[4], "L_FOOT", 3,
				 1.31579, 6.72219e-010, 1.85087e-009,
				 1.18358e-006, -0.0116309, 5.24389e-007, -0.999932);

	cmx_bone_set(skeleton[5], "L_TOE0", 4,
				 0.253839, -4.82293e-009, 0.339987,
				 -1.9198e-011, -0.707107, 1.91956e-011, -0.707107);

	cmx_bone_set(skeleton[6], "L_TOE01", 5,
				 0.251088, -3.00624e-009, -2.91078e-010,
				 -2.95136e-015, 1.25923e-014, 1.67305e-011, -1);

	cmx_bone_set(skeleton[7], "L_TOE02", 6,
				 0.056929, -3.05933e-009, 3.05676e-010,
				 7.81281e-017, -5.38587e-015, -1.04296e-011, -1);

	cmx_bone_set(skeleton[8], "R_LEG", 1,
				 2.44632e-007, -0.241228, -4.62491e-007,
				 0.0165402, 5.5519e-007, -0.999863, -1.00484e-006);

	cmx_bone_set(skeleton[9], "R_LEG1", 8,
				 1.44737, 1.03675e-008, 8.03626e-009,
				 -2.11848e-011, 0.0298356, 8.86403e-013, -0.999555);

	cmx_bone_set(skeleton[10], "R_FOOT", 9,
				 1.31579, 1.12555e-008, -6.11749e-009,
				 -6.65177e-007, -0.00860503, -3.73375e-007, -0.999963);

	cmx_bone_set(skeleton[11], "R_TOE0", 10,
				 0.253839, -4.72826e-009, 0.339987,
				 1.97569e-011, -0.707107, -1.97538e-011, -0.707107);

	cmx_bone_set(skeleton[12], "R_TOE01", 11,
				 0.251088, -2.92067e-009, -5.78397e-010,
				 -4.94409e-015, 1.09617e-014, 5.30648e-012, -1);

	cmx_bone_set(skeleton[13], "R_TOE02", 12,
				 0.056929, -2.8047e-009, 5.1338e-010,
				 7.98796e-016, 2.99644e-015, -1.37573e-011, -1);

	cmx_bone_set(skeleton[14], "SPINE", 1,
				 0.381066, 5.29048e-007, -0.000334763,
				 2.10104e-006, -0.0315231, 7.14932e-007, -0.999503);

	cmx_bone_set(skeleton[15], "SPINE1", 14,
				 0.421059, -9.27955e-010, -0.00033528,
				 -3.30452e-012, 7.52235e-011, -8.18583e-015, -1);

	cmx_bone_set(skeleton[16], "SPINE2", 15,
				 0.421059, -9.33352e-010, -0.000335277,
				 -3.30452e-012, 7.52235e-011, -8.18583e-015, -1);

	cmx_bone_set(skeleton[17], "NECK", 16,
				 0.421097, -8.79169e-010, -0.000315105,
				 -1.2815e-011, -0.0604161, 1.67573e-007, -0.998173);

	cmx_bone_set(skeleton[18], "HEAD", 17,
				 0.398615, -1.0225e-012, -1.18576e-008,
				 2.9478e-012, -0.0668782, 1.85489e-007, -0.997761);

	cmx_bone_set(skeleton[19], "L_ARM", 17,
				 4.56552e-007, 0.130263, 0.000317027,
				 -0.618227, -0.783643, 0.0376674, -0.0477436);

	cmx_bone_set(skeleton[20], "L_ARM1", 19,
				 0.376316, -5.49453e-008, 1.60809e-009,
				 -0.00849216, -0.0173039, -0.57938, -0.81483);

	cmx_bone_set(skeleton[21], "L_ARM2", 20,
				 0.881536, 2.63531e-008, 9.20282e-009,
				 -8.57358e-011, 0.0500412, 5.23877e-009, -0.998747);

	cmx_bone_set(skeleton[22], "L_HAND", 21,
				 0.820474, 1.81877e-008, 3.02261e-009,
				 0.705821, 0.0376602, 0.0376902, -0.706383);

	cmx_bone_set(skeleton[23], "L_FINGER0", 22,
				 0.302072, 8.54501e-009, -1.60412e-008,
				 0.000391826, -0.177342, -7.06052e-005, -0.984149);

	cmx_bone_set(skeleton[24], "R_ARM", 17,
				 4.57471e-007, -0.130263, 0.000317749,
				 0.618227, -0.783643, -0.0376652, -0.0477453);

	cmx_bone_set(skeleton[25], "R_ARM1", 24,
				 0.376316, 5.23017e-008, 1.85675e-009,
				 0.00849222, -0.0173038, 0.57938, -0.81483);

	cmx_bone_set(skeleton[26], "R_ARM2", 25,
				 0.881536, -1.63993e-008, 5.11895e-009,
				 1.00755e-010, 0.0500412, -3.82142e-009, -0.998747);

	cmx_bone_set(skeleton[27], "R_HAND", 26,
				 0.820474, -4.87935e-008, -3.3423e-009,
				 -0.706605, 0.0176475, -0.0176615, -0.707168);

	cmx_bone_set(skeleton[28], "R_FINGER0", 27,
				 0.302072, 1.54253e-010, 6.47549e-008,
				 -0.000394579, -0.133423, 5.31174e-005, -0.991059);


	freyjaBegin(FREYJA_SKELETON);

	for (i = 0; i < 29; ++i)
	{
		freyjaBegin(FREYJA_BONE);
		idx = freyjaGetCurrent(FREYJA_BONE);
		freyjaBoneParent1i(idx, skeleton[i].parent);
		freyjaBoneName1s(idx, skeleton[i].name);
		
		switch (i)
		{
		case 0:
			{
				vec3_t xyz;

				freyjaBoneRotateQuatWXYZ4fv(idx, skeleton[i].wxyz);
				freyjaGetBoneRotationXYZ3fv(idx, xyz);
				xyz[1] -= 90.0f;
				freyjaBoneRotateEulerXYZ3f(idx, xyz[0], xyz[1], xyz[2]);
				freyjaBoneTranslate3f(idx, 
									  skeleton[i].xyz[0] * scale, 
									  skeleton[i].xyz[1] * scale,
									  skeleton[i].xyz[2] * scale);
			}
			break;

		default:
			{
				vec3_t xyz;

				freyjaBoneRotateQuatWXYZ4fv(idx, skeleton[i].wxyz);

				if (i == 2 || i == 8)
				{
					freyjaGetBoneRotationXYZ3fv(idx, xyz);
					xyz[0] += 0.0f;
					xyz[1] -= 180.0f;
					xyz[2] += 0.0f;
					freyjaBoneRotateEulerXYZ3f(idx, xyz[2], xyz[0], xyz[1]);
				}
				else if (i == 19)
				{
					freyjaGetBoneRotationXYZ3fv(idx, xyz);
					xyz[0] += 0.0f;
					xyz[1] -= 90.0f;
					xyz[2] += 0.0f;
					freyjaBoneRotateEulerXYZ3f(idx, xyz[0], xyz[1], xyz[2]);
				}
				else if (i == 24)
				{
					freyjaGetBoneRotationXYZ3fv(idx, xyz);
					xyz[0] += 0.0f;
					xyz[1] += 90.0f;
					xyz[2] += 0.0f;
					freyjaBoneRotateEulerXYZ3f(idx, xyz[0], xyz[1], xyz[2]);
				}

				freyjaBoneTranslate3f(idx, 
									  skeleton[i].xyz[0] * scale, 
									  skeleton[i].xyz[2] * scale,
									  skeleton[i].xyz[1] * scale);
			}
		}

		for (j = 0; j < 29; ++j)
		{
			if (skeleton[j].parent == i)
			{ 
				freyjaBoneAddChild1i(idx, j);
			}
		}
		
		freyjaEnd(); // FREYJA_BONE
	}

	freyjaEnd();
}


int freyja_model__skn_import(char *filename)
{
	const vec_t scale = 5.0;
	Vector <unsigned int> transV, transT;
	FreyjaFileReader r;
	char *symbol;
	char name[64];
	char material[64];
	long i, index;


	if (freyja_model__skn_check(filename) < 0 || !r.openFile(filename))
	{
		return -1;
	}

	cmx_import_adult_skeleton();

	symbol = r.parseSymbol();
	strncpy(name, symbol, 64);
	name[63] = 0;

	symbol = r.parseSymbol();
	strncpy(material, symbol, 64);
	material[63] = 0;


	long boneCount = r.parseInteger();
	skn_bone_t bones[boneCount];
	printf("boneCount = %li\n", boneCount);

	for (i = 0; i < boneCount; ++i)
	{
		symbol = r.parseSymbol();
		strncpy(bones[i].name, symbol, 64);
		bones[i].name[63] = 0;
	}


	long trisCount = r.parseInteger();
	long tris[trisCount][3];
	printf("trisCount = %li\n", trisCount);

	for (i = 0; i < trisCount; ++i)
	{
		tris[i][0] = r.parseInteger();
		tris[i][1] = r.parseInteger();
		tris[i][2] = r.parseInteger();
	}


	long bindingCount = r.parseInteger();
	skn_bone_binding_t bindings[bindingCount];
	printf("bindingCount = %li\n", bindingCount);

	for (i = 0; i < bindingCount; ++i)
	{
		bindings[i].boneIndex = r.parseInteger();
		bindings[i].vertexOffset = r.parseInteger();
		bindings[i].vertexCount = r.parseInteger();
		bindings[i].blendedVertexOffset = r.parseInteger();
		bindings[i].blendedVertexCount = r.parseInteger();

		printf("binding[%li] = { %li Vert @ %li x %li, Blended Vert @ %li x %li}\n", i,
			   bindings[i].boneIndex, 
			   bindings[i].vertexOffset, 
			   bindings[i].vertexCount,
			   bindings[i].blendedVertexOffset,
			   bindings[i].blendedVertexCount);
	}

	
	long texcoordCount = r.parseInteger();
	vec2_t texcoords[texcoordCount];
	printf("texcoordCount = %li\n", texcoordCount);

	for (i = 0; i < texcoordCount; ++i)
	{
		texcoords[i][0] = r.parseFloat();
		texcoords[i][1] = r.parseFloat();
	}

	
	long q2Count = r.parseInteger();
	long q2[q2Count][2];
	printf("q2Count = %li\n", q2Count);

	for (i = 0; i < q2Count; ++i)
	{
		q2[i][0] = r.parseInteger();
		q2[i][1] = r.parseInteger();
	}


	long vertexCount = r.parseInteger();
	vec3_t vertices[vertexCount];
	vec3_t normals[vertexCount];
	printf("vertexCount = %li\n", vertexCount);

	for (i = 0; i < vertexCount; ++i)
	{
		vertices[i][0] = r.parseFloat();
		vertices[i][1] = r.parseFloat();
		vertices[i][2] = r.parseFloat();

		normals[i][0] = r.parseFloat();
		normals[i][1] = r.parseFloat();
		normals[i][2] = r.parseFloat();
	}


	freyjaBegin(FREYJA_MODEL);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	for (i = 0; i < vertexCount; ++i)
	{
		index = freyjaVertex3f(vertices[i][0]*scale,
							   vertices[i][2]*scale,
							   vertices[i][1]*scale);

		freyjaVertexNormal3f(index, 
							 normals[i][0],
							 normals[i][2],
							 normals[i][1]);

		transV.pushBack(index);
	}

	freyjaEnd(); // FREYJA_GROUP

	for (i = 0; i < texcoordCount; ++i)
	{
		transT.pushBack(freyjaTexCoord2f(texcoords[i][0], texcoords[i][1]));
	}

	for (i = 0; i < trisCount; ++i)
	{
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(0);
		freyjaPolygonVertex1i(transV[tris[i][0]]);
		freyjaPolygonTexCoord1i(transT[tris[i][0]]);
		freyjaPolygonVertex1i(transV[tris[i][1]]);
		freyjaPolygonTexCoord1i(transT[tris[i][1]]);
		freyjaPolygonVertex1i(transV[tris[i][2]]);
		freyjaPolygonTexCoord1i(transT[tris[i][2]]);
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__skn_export(char *filename)
{
	FreyjaFileWriter w;
	char name[64];
	int index;
	unsigned int i, n;
	vec_t scale = 1.0 / 0.15;
	vec_t d2r = 0.017453292519943295;
	vec3_t translation, rotation;


	if (!w.openFile(filename))
	{
		return -1;
	}

	/* version */
	w.print("version 1\n");

	/* nodes */
	w.print("nodes\n");

	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		n = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
			index = freyjaGetCurrent(FREYJA_BONE);

			freyjaGetBoneName(index, 64, name);
			w.print("%3i \"%s\" %i\n", i, name, freyjaGetBoneParent(index));

			freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}

	w.print("end\n"); // nodes

	/* skeleton */
	w.print("skeleton\n");
	w.print("time 0\n");

	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		n = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
			index = freyjaGetCurrent(FREYJA_BONE);

			freyjaGetBoneTranslation3fv(index, translation);
			freyjaGetBoneRotationXYZ3fv(index, rotation);

			translation[0] *= scale; 
			translation[2] *= scale; 
			translation[1] *= scale; 

			if (!i)
			{
				rotation[1] += 90.0;

				w.print("%3i %f %f %f %f %f %f\n", i,
						translation[0], translation[2], translation[1], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}
			else
			{
				w.print("%3i %f %f %f %f %f %f\n", i,
						translation[0], translation[1], translation[2], 
						rotation[0]*d2r, rotation[1]*d2r, rotation[2]*d2r);
			}

			freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}
	w.print("end\n");

	/* triangles */
	w.print("triangles\n");
#ifdef SKN_MESH_EXPORT
	unsigned int j, k, group;
	vec3_t vert;
	vec2_t uv;


	if (freyjaGetNum(FREYJA_POLYGON))
	{
		freyjaIterator(FREYJA_MESH, FREYJA_LIST_RESET);
		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
		n = freyjaGetNum(FREYJA_POLYGON);

		group = 0;

#ifdef SKN_MESH_EXPORT_ALL
		unsigned int meshCount = freyjaGetNum(FREYJA_MESH);
		for (i = 0; i < meshCount; ++i, group = i) // send all meshes
#endif
		for (j = 0; j < n; ++j)
		{
			index = freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_CURRENT);
			index = freyjaGetCurrent(FREYJA_POLYGON);

			for (j = 0; j < n; ++j)
			{
				freyjaGetPolygon3f(FREYJA_VERTEX, j, &vert);
				freyjaGetPolygon3f(FREYJA_TEXCOORD, j, &uv);

				w.print("null.png");
				w.print("%3i  %f %f %f  %f %f %f  %f %f\n", group,
						vert[0]*scale, vert[2]*scale, vert[1]*scale, 
						norm[0], norm[2], norm[1],
						uv[0], uv[1]);
			}

			freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
		}
	}
#endif
	w.print("end\n");

	w.closeFile();

	return 0;
}
