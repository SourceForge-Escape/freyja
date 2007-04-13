/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Md5
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Doom3 model plugin
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.08.13:
 * Mongoose - Created
 ==========================================================================*/

#include <stdlib.h>
#include <math.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "Md5.h"

using namespace mstl;

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Md5::Md5()
{
	mVersion = 0;
	mCommandLine = 0x0;
	mNumJoints = 0;
	mNumMeshes = 0;
	mJoints = 0x0;
	mMeshes = 0x0;
}


Md5::~Md5()
{
	if (mCommandLine)
		delete [] mCommandLine;
	
	if (mNumJoints)
		delete [] mJoints;
	
	if (mMeshes)
		delete [] mMeshes;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

float Md5::decodeIdQuaternion(float qx, float qy, float qz)
{
	float qw, tmp;

	tmp = 1.0f - ((qx*qx) + (qy*qy) + (qz*qz));

	if (tmp >= 0)
	{
		qw = -(float)sqrt( tmp );
	}
	else
	{
		qw = 0;
	}

	return qw;
}


bool Md5::isMd5Model(const char *filename)
{
	SystemIO::TextFileReader r;

	if (r.Open(filename) == false)
		return false;

	if (r.ParseMatchingSymbol("MD5Version"))
		return true;

	r.Close();

	return false;
}


bool Md5::saveModel(const char *filename)
{
	SystemIO::TextFileWriter w;
	int i, j;  /* I hate everyone that uses signed indices in file formats */


	if (SystemIO::File::DoesFileExist(filename))
	{
		printf("Can't cycle Md5mesh with missing metadata -- won't overwrite '%s'.\n", filename);
		return false;
	}

	if (w.Open(filename) == false)
		return false;

	/* Header */
	w.Print("MD5Version 10\n");

	/* Some Id thingys */
	w.Print("commandline \"FIXME FOR YOUR MODEL\"\n\n");

	/* Joint and mesh counts */
	w.Print("numJoints %i\n", mNumJoints);
	w.Print("numMeshes %i\n\n", mNumMeshes);

	/* Joint data */
	w.Print("joints {\n");

	for (i = 0; i < mNumJoints; ++i)
	{
		w.Print("\t\"%s\"\t%i ", mJoints[i].name, mJoints[i].parent);

		/* translate X Y Z */
		w.Print("( %f %f %f ) ",
			mJoints[i].translate[0],
			mJoints[i].translate[1],
			mJoints[i].translate[2]);

		w.Print("( %f %f %f )\t\t",
			mJoints[i].rotate[0],
			mJoints[i].rotate[1],
			mJoints[i].rotate[2]);
		
		/* Recover metadata of parent comment */
		w.Print("// ");

		if (mJoints[i].parent > 0)
		{
			w.Print("%s", mJoints[mJoints[i].parent].name);
		}

		w.Print("\n");
	}

	w.Print("}\n");

	/* Write mesh data*/
	for (i = 0; i < mNumMeshes; ++i)
	{
		w.Print("\n");
		w.Print("mesh {\n");
		w.Print("\t// meshes: %s\n", mMeshes[i].name);
		w.Print("\tshader \"%s\"\n", mMeshes[i].shader);

		w.Print("\n\tnumverts %i\n", mMeshes[i].numverts);

		for (j = 0; j < mMeshes[i].numverts; ++j)
		{
			w.Print("\tvert %i ( %lf %lf ) %i %i\n", 
					j, // mMeshes[i].verts[j].index // Should == j
					mMeshes[i].verts[j].uv[0], 
					mMeshes[i].verts[j].uv[1],
					mMeshes[i].verts[j].weight,
					mMeshes[i].verts[j].numbones);
		}

		w.Print("\n\tnumtris %i\n", mMeshes[i].numtriangles);

		for (j = 0; j < mMeshes[i].numtriangles; ++j)
		{
			w.Print("\ttri %i %i %i %i\n",
				j,
				mMeshes[i].triangles[j].vertex[0],
				mMeshes[i].triangles[j].vertex[1],
				mMeshes[i].triangles[j].vertex[2]);
		}

		w.Print("\n\tnumweights %i\n", mMeshes[i].numweights);

		for (j = 0; j < mMeshes[i].numweights; ++j)
		{
			w.Print("\tweight %i %i %f ( %f %f %f )\n",
					j, // mMeshes[i].weights[j].index
					mMeshes[i].weights[j].joint,
					mMeshes[i].weights[j].weight,
					mMeshes[i].weights[j].pos[0],
					mMeshes[i].weights[j].pos[1],
					mMeshes[i].weights[j].pos[2]);			
		}
		
		w.Print("}\n");
	}

	w.Close();

	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool Md5::loadModel(const char *filename)
{
	SystemIO::TextFileReader r;
	int i, j;  /* I hate everyone that uses signed indices in file formats */


	if (isMd5Model(filename) == false)
		return false;

	if (r.Open(filename) == false)
		return false;

	if (!r.ParseMatchingSymbol("MD5Version"))
		return false;

	mVersion = r.ParseInteger();

	switch (mVersion)
	{
	case 10:
		break;
	default:
		//printf("<Md5> Don't know this version %i.\n", mVersion);
		return false;
	}


	/* Some Id thingys */
	if (!r.ParseMatchingSymbol("commandline"))
		return false;

	mCommandLine = String::Strdup(r.ParseStringLiteral());


	/* Joint setup */
	if (!r.ParseMatchingSymbol("numJoints"))
		return false;

	mNumJoints = r.ParseInteger();

	if (mNumJoints < 0)
	{
		mNumJoints = 0;
	}
	else
	{
		mJoints = new Md5Joint[mNumJoints];
	}


	/* Mesh setup */
	if (!r.ParseMatchingSymbol("numMeshes"))
		return false;

	mNumMeshes = r.ParseInteger();

	if (mNumMeshes < 0)
	{
		mNumMeshes = 0;
	}
	else
	{
		mMeshes = new Md5Mesh[mNumMeshes];
	}


	/* Joint data */
	if (mNumJoints && r.ParseMatchingSymbol("joints"))
	{
		if (!r.ParseMatchingSymbol("{"))
			return false;
			
		for (i = 0; i < mNumJoints; ++i)
		{
			mJoints[i].name = String::Strdup(r.ParseStringLiteral());
			mJoints[i].parent = r.ParseInteger();

			/* translate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;

			mJoints[i].translate[0] = r.ParseFloat();
			mJoints[i].translate[1] = r.ParseFloat();
			mJoints[i].translate[2] = r.ParseFloat();

			if (!r.ParseMatchingSymbol(")"))
				return false;


			/* rotate X Y Z */
			if (!r.ParseMatchingSymbol("("))
				return false;
			
			mJoints[i].rotate[0] = r.ParseFloat();
			mJoints[i].rotate[1] = r.ParseFloat();
			mJoints[i].rotate[2] = r.ParseFloat();

			if (!r.ParseMatchingSymbol(")"))
				return false;

			// FIXME: Save comment at end of line for something?
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}
	else
	{
		return false;
	}

	
	for (i = 0; i < mNumMeshes; ++i)
	{
		if (r.ParseMatchingSymbol("mesh"))
		{
			if (!r.ParseMatchingSymbol("{"))
				return false;

			// FIXME: // meshes: MESHNAME?
			strncpy(mMeshes[i].name, "mynameis?", 24);

			if (!r.ParseMatchingSymbol("shader"))
				return false;

			mMeshes[i].shader = String::Strdup(r.ParseStringLiteral());

			if (!r.ParseMatchingSymbol("numverts"))
				return false;

			mMeshes[i].numverts = r.ParseInteger();

			if (mMeshes[i].numverts < 0)
			{
				mMeshes[i].numverts = 0;
			}
			else
			{
				mMeshes[i].verts = new Md5Vertex[mMeshes[i].numverts];
			}

			for (j = 0; j < mMeshes[i].numverts; ++j)
			{
				if (!r.ParseMatchingSymbol("vert"))
					return false;

				mMeshes[i].verts[j].index = r.ParseInteger();

				if (!r.ParseMatchingSymbol("("))
					return false;

				mMeshes[i].verts[j].uv[0] = r.ParseFloat(); 
				mMeshes[i].verts[j].uv[1] = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
				
				mMeshes[i].verts[j].weight = r.ParseInteger();
				mMeshes[i].verts[j].numbones =r.ParseInteger();
			}

			if (!r.ParseMatchingSymbol("numtris"))
				return false;

			mMeshes[i].numtriangles = r.ParseInteger();
			mMeshes[i].triangles = new Md5Triangle[mMeshes[i].numtriangles];

			for (j = 0; j < mMeshes[i].numtriangles; ++j)
			{
				if (!r.ParseMatchingSymbol("tri"))
					return false;
				
				r.ParseInteger(); // integer == j
				mMeshes[i].triangles[j].vertex[0] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[1] = r.ParseInteger();
				mMeshes[i].triangles[j].vertex[2] = r.ParseInteger();
			}

			if (!r.ParseMatchingSymbol("numweights"))
				return false;

			mMeshes[i].numweights = r.ParseInteger();
			mMeshes[i].weights = new Md5Weight[mMeshes[i].numweights];

			for (j = 0; j < mMeshes[i].numweights; ++j)
			{
				if (!r.ParseMatchingSymbol("weight"))
					return false;

				mMeshes[i].weights[j].index = r.ParseInteger();
				mMeshes[i].weights[j].joint = r.ParseInteger();
				mMeshes[i].weights[j].weight = r.ParseFloat();

				if (!r.ParseMatchingSymbol("("))
					return false;

				mMeshes[i].weights[j].pos[0] = r.ParseFloat();
				mMeshes[i].weights[j].pos[1] = r.ParseFloat();
				mMeshes[i].weights[j].pos[2] = r.ParseFloat();

				if (!r.ParseMatchingSymbol(")"))
					return false;
			}
			
		}
		
		if (!r.ParseMatchingSymbol("}"))
			return false;
	}

	r.Close();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <hel/Quat.h>
#include <hel/Mat44.h>

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__md5_check(char *filename);
	int freyja_model__md5_import(char *filename);
	int freyja_model__md5_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginDescription1s("Doom3 Mesh (*.md5mesh)");
	freyjaPluginAddExtention1s("*.md5mesh");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__md5_check(filename))
		return freyja_model__md5_import(filename);

	return -1;
}


int freyja_model__md5_check(char *filename)
{
	Md5 md5;

	if (md5.isMd5Model(filename) == true)
		return 0; /* 0 is no error */

	return -1;
}


int freyja_model__md5_import(char *filename)
{
	const vec_t scale = 0.3;
	Md5 md5;
	Vector<unsigned int> weights;
	unsigned int vertex, texcoord;
	int i, m, v, w, t, j, bindex, index;
	float qw;
	vec4_t wxyz;
	Quat q, q2;
	Mat44 mat, mat2;
	Vec3 vec3, tmp;


	if (md5.loadModel(filename) == false)
		return -1;

	/* Start a new model */
	freyjaBegin(FREYJA_MODEL);

	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		Vector<unsigned int> vertices;
		Vector<unsigned int> texcoords;


		/* Start a new mesh */
		freyjaBegin(FREYJA_MESH);
	
		/* Start a new vertex group */
		freyjaBegin(FREYJA_VERTEX_GROUP);

		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			vec3.Zero();

			for (i = 0; i < md5.mMeshes[m].verts[v].numbones; ++i)
			{
				w = md5.mMeshes[m].verts[v].weight;
				w = md5.mMeshes[m].weights[w].index + i;
				j = md5.mMeshes[m].weights[w].joint;

				qw = md5.decodeIdQuaternion(md5.mJoints[j].rotate[0],
											md5.mJoints[j].rotate[1],
											md5.mJoints[j].rotate[2]);
				q = Quat(qw, 
							   md5.mJoints[j].rotate[0],
							   md5.mJoints[j].rotate[1],
							   md5.mJoints[j].rotate[2]);

				tmp = q.Rotate(md5.mMeshes[m].weights[w].pos);		
				tmp += Vec3(md5.mJoints[j].translate[0],
								md5.mJoints[j].translate[1],
								md5.mJoints[j].translate[2]);
				vec3 += tmp * md5.mMeshes[m].weights[w].weight;

			}

			vec3 *= scale;

			vertex = freyjaVertexCreate3f(vec3.mVec[0], vec3.mVec[2], vec3.mVec[1]);

			/* Store texels */
			texcoord = freyjaTexCoordCreate2f(md5.mMeshes[m].verts[v].uv[0],
										md5.mMeshes[m].verts[v].uv[1]);
			
			/* Generates id translator list */
			vertices.pushBack(vertex);
			texcoords.pushBack(texcoord);
		}

		freyjaEnd(); // FREYJA_GROUP

		for (t = 0; t < md5.mMeshes[m].numtriangles; ++t)
		{
			/* Start a new polygon */
			freyjaBegin(FREYJA_POLYGON);
			
			/* Store vertices and texels by true id, using translator lists */
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[0]]);
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[1]]);
			freyjaPolygonVertex1i(vertices[md5.mMeshes[m].triangles[t].vertex[2]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[0]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[1]]);
			freyjaPolygonTexCoord1i(texcoords[md5.mMeshes[m].triangles[t].vertex[2]]);

			freyjaPolygonMaterial1i(m); // mMeshes[m].shader
			
			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH
	}


	/* Load skeleton */
	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			w = md5.mMeshes[m].verts[v].weight;

			freyjaVertexWeight(v, 
							   md5.mMeshes[m].weights[w].weight,
							   md5.mMeshes[m].weights[w].joint);

			/* Generates id translator list */
			weights.pushBack(md5.mMeshes[m].weights[w].index);
		}
	}

	/* Generate normals to be pretty */
	freyjaMeshGenerateVertexNormals(freyjaGetFSMMeshIndex());
	freyjaMeshNormalFlip(freyjaGetFSMMeshIndex());

	freyjaEnd(); // FREYJA_MODEL

	freyjaBegin(FREYJA_SKELETON);
	index_t skeletonIndex = freyjaGetCurrent(FREYJA_SKELETON);
	Vec3 transforms[md5.mNumJoints];
	Vec3 origin;

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		transforms[j].Zero();
	}

	/* Mongoose 2004.12.21, 
	 * Md5 stores absolute bone pos -- make them offsets from parent */
	for (j = 0; j < md5.mNumJoints; ++j)
	{
		vec3 = Vec3(md5.mJoints[j].translate[0],
						md5.mJoints[j].translate[1],
						md5.mJoints[j].translate[2]);
		//printf("%3i   %.3f %.3f %.3f\n", j,
		//	   vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
		index = j;

		while (index > -1)
		{
			vec3 -= transforms[index];
			index = md5.mJoints[index].parent;
		}

		transforms[j] = vec3;
		//printf("->    %.3f %.3f %.3f\n",
		//	   vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
	}

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		/* Start a new tag */
		freyjaBegin(FREYJA_BONE);
		bindex = freyjaGetCurrent(FREYJA_BONE);
		freyjaSkeletonAddBone(skeletonIndex, bindex);
		freyjaBoneFlags(bindex, 0x0);
		freyjaBoneParent(bindex, md5.mJoints[j].parent);
		freyjaBoneName(bindex, md5.mJoints[j].name);

		/* Translate */
		vec3 = transforms[j];
		vec3 *= scale;

		if (!j || 1)
		{
			freyjaBoneTranslate3f(bindex,
								  vec3.mVec[0], vec3.mVec[2], vec3.mVec[1]);
		}
		else
		{
			freyjaBoneTranslate3f(bindex, 
								  vec3.mVec[0], vec3.mVec[1], vec3.mVec[2]);
		}

		/* Scale */
		index = j;
		mat.SetIdentity();

		while (index > -1)
		{
			index = md5.mJoints[index].parent;

			q2 = Quat(qw, 
							md5.mJoints[index].rotate[0],
							md5.mJoints[index].rotate[1],
							md5.mJoints[index].rotate[2]);
			mat2 = q2;
			mat = mat * mat2;
		}

		mat.Invert();
		mat2 = Quat(qw, 
						  md5.mJoints[j].rotate[0],
						  md5.mJoints[j].rotate[1],
						  md5.mJoints[j].rotate[2]);
		mat = mat * mat2;
		q = mat.ToQuat(); 
		// q.SetByMatrix(mat.mMatrix);
		q.GetQuat(wxyz);

		//freyjaBoneRotateQuatWXYZ4fv(bindex, wxyz); // argh

		for (int j2 = 0; j2 < md5.mNumJoints; ++j2)
		{
			if (md5.mJoints[j2].parent == j)
			{ 
				freyjaBoneAddChild(bindex, j2);
			}
		}

		freyjaEnd(); // FREYJA_TAG
	}

	freyjaEnd(); // FREYJA_SKELETON


	freyjaPrintMessage("The Doom3 plugin sez: FINISH ME!!!");

	return 0;
}

// FIXME Basically this doesn't reencode format properly, and is here
//       as a framework for correct allocation of the md5
// NOTE  You CAN NOT export md5 from freyja 9.1, and this won't work
//       until 10.0 or 9.3 are released -- there is an issue with
//       skeletal support for certain types of multibone in 9.1
//       The problem?  0.0 weights are removed in optimization! heh  =)
int freyja_model__md5_export(char *filename)
{
	const vec_t scale = (1.0 / 0.3);
	Md5 md5;
	int i, m, v, t, j, count;
	vec4_t wxyz;
	vec3_t xyz;
	vec_t weight;
	Quat q, q2;
	Mat44 mat, mat2;
	Vec3 vec3, tmp;
	int32 boneIndex, meshIndex, faceIndex, vertexIndex, texcoordIndex;
	index_t modelIndex, skeletonIndex;
	index_t bone;


	/* Encode the model into md5 to save */
	modelIndex = freyjaGetCurrentModelIndex();
	skeletonIndex = freyjaGetCurrentSkeleton();

	md5.mVersion = 10;
	md5.mNumMeshes = freyjaGetModelMeshCount(modelIndex);
	md5.mNumJoints = freyjaGetSkeletonBoneCount(skeletonIndex);
	md5.mCommandLine = new char[64];
	snprintf(md5.mCommandLine, 64, "No CommandLine");
	md5.mMeshes = new Md5Mesh[md5.mNumMeshes];
	md5.mJoints = new Md5Joint[md5.mNumJoints];

	/* Load skeleton */

	for (j = 0; j < md5.mNumJoints; ++j)
	{
		boneIndex = freyjaGetSkeletonBoneIndex(modelIndex, j);

		md5.mJoints[j].parent = freyjaGetBoneParent(boneIndex);
		md5.mJoints[j].name = new char[64];
		freyjaGetBoneName(boneIndex, 64, md5.mJoints[j].name);

		freyjaGetBoneRotationQuat4fv(boneIndex, wxyz);
		//FIXME encode quaternion wxyz to id format here
		md5.mJoints[j].rotate[0] = wxyz[0];
		md5.mJoints[j].rotate[1] = wxyz[1];
		md5.mJoints[j].rotate[2] = wxyz[2];

		freyjaGetBoneTranslation3fv(boneIndex, xyz);

		xyz[0] *= scale;
		xyz[1] *= scale;
		xyz[2] *= scale;
		//FIXME Md5 stores absolute bone pos; freyja has offsets from parent
		
		md5.mJoints[j].translate[0] = xyz[0];
		md5.mJoints[j].translate[1] = xyz[2];
		md5.mJoints[j].translate[2] = xyz[1];
	}

	/* Load meshes */

	for (m = 0; m < md5.mNumMeshes; ++m)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, m);
		snprintf(md5.mMeshes[m].name, 32, "mesh%03i", m);
		md5.mMeshes[m].name[8] = 0;
		md5.mMeshes[m].shader = new char[64];
		snprintf(md5.mMeshes[m].shader, 64, "No Shader");
		
		md5.mMeshes[m].weights = NULL; 
		md5.mMeshes[m].numweights = 0; 

		md5.mMeshes[m].numverts = freyjaGetMeshVertexCount(meshIndex);
		md5.mMeshes[m].verts = new Md5Vertex[md5.mMeshes[m].numverts];

		md5.mMeshes[m].numtriangles = freyjaGetMeshPolygonCount(meshIndex);
		md5.mMeshes[m].triangles = new Md5Triangle[md5.mMeshes[m].numtriangles];

		for (v = 0, count = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			vertexIndex = v;//freyjaGetMeshVertexIndex(meshIndex, v);
			count += freyjaGetVertexWeightCount(vertexIndex);
		}

		md5.mMeshes[m].numweights = count;
		md5.mMeshes[m].weights = new Md5Weight[md5.mMeshes[m].numweights];

		for (v = 0; v < md5.mMeshes[m].numweights; ++v)
		{
			vertexIndex = v;//freyjaGetMeshVertexIndex(meshIndex, v);
			count = freyjaGetVertexWeightCount(vertexIndex);

			for (i = 0; i < count; ++i)
			{
				freyjaGetVertexWeight(vertexIndex, i, &bone, &weight);

				// FIXME have to encode position into these partial wedges
				md5.mMeshes[m].weights[v].pos[0] = xyz[0];
				md5.mMeshes[m].weights[v].pos[1] = xyz[1];
				md5.mMeshes[m].weights[v].pos[2] = xyz[2];
			}
		}

		for (v = 0; v < md5.mMeshes[m].numverts; ++v)
		{
			vertexIndex = v;//freyjaGetMeshVertexIndex(meshIndex, v);

			freyjaGetVertexXYZ3fv(vertexIndex, xyz);

			// FIXME encode to id vertices / weights
			md5.mMeshes[m].verts[v].index = v;

			md5.mMeshes[m].verts[v].numbones = 0; 

			for (i = 0; i < md5.mMeshes[m].verts[v].numbones; ++i)
			{
				md5.mMeshes[m].verts[v].weight = 0;
			}
		}

		// Assumes user tesselated model
		for (t = 0; t < md5.mMeshes[m].numtriangles; ++t)
		{
			faceIndex = t;//freyjaGetMeshPolygonIndex(meshIndex, t);

			vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, t, 0);
			texcoordIndex = freyjaGetMeshPolygonTexCoordIndex(meshIndex, t, 0);
			vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, t, vertexIndex);
			freyjaGetMeshTexCoord2fv(meshIndex, texcoordIndex, md5.mMeshes[m].verts[vertexIndex].uv);
			md5.mMeshes[m].triangles[t].vertex[0] = vertexIndex;

			vertexIndex = freyjaGetPolygonVertexIndex(faceIndex, 1);
			texcoordIndex = freyjaGetPolygonTexCoordIndex(faceIndex, 2);
			vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, faceIndex, vertexIndex);
			freyjaGetMeshTexCoord2fv(meshIndex, texcoordIndex, md5.mMeshes[m].verts[vertexIndex].uv);
			md5.mMeshes[m].triangles[t].vertex[1] = vertexIndex;

			vertexIndex = freyjaGetPolygonVertexIndex(faceIndex, 2);
			texcoordIndex = freyjaGetPolygonTexCoordIndex(faceIndex, 2);
			vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, vertexIndex, vertexIndex);
			freyjaGetMeshTexCoord2fv(meshIndex,texcoordIndex, md5.mMeshes[m].verts[vertexIndex].uv);
			md5.mMeshes[m].triangles[t].vertex[2] = vertexIndex;


		}
	}

	if (md5.saveModel(filename) == false)
		return -1;

	freyjaPrintError("md5.so export isn't working yet.  This is a test.");

	return 0;
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MD5
int runMd5UnitTest(int argc, char *argv[])
{
	Md5 test;

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (test.loadModel(argv[2]) == false)
				printf("UNIT_TEST: Load reports error.\n");

			//test.print();
			return 0;
		}
		else if (strcmp(argv[1], "save") == 0 && argc > 3)
		{
			if (test.loadModel(argv[2]) == false)
				printf("UNIT_TEST: Load reports error.\n");

			//test.print();

			if (test.saveModel(argv[3]) == false)
				printf("UNIT_TEST: Save reports error.\n");

			//test.print();
			return 0;
		}
	}

	printf("\n\n");
	printf("%s load filenameLoad.md5mesh\n", argv[0]);
	printf("%s save filenameLoad.md5mesh filenameSave.md5mesh\n", argv[0]);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Md5 class test]\n");

	return runMd5UnitTest(argc, argv);
}
#endif
