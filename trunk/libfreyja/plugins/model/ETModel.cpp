/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : ETModel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: MDM and MDX formats
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.16:
 * Mongoose - Created
 ==========================================================================*/

#include <freyja8/EggFileReader.h>

#include "ETModel.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

ETModel::ETModel()
{
}


ETModel::~ETModel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void MDMModel::print()
{
	printf("%li surfaces @ %li\n", 
		   mHeader.numSurfaces, mHeader.surfaceDataOffset);
	printf("%li tags @ %li\n", 
		   mHeader.numTags, mHeader.tagDataOffset);
	printf("%li bytes in file\n", mHeader.fileSize);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool MDMModel::load(const char *filename)
{
	EggFileReader r;
	long i, j, k;


	if (r.openFile(filename) == false)
		return false;

	r.readCharString(4, mHeader.ident);
	mHeader.version = r.readLong();
	r.readCharString(64, mHeader.name);
	mHeader.lodBias = r.readFloat32();
	mHeader.lodScale = r.readFloat32();
	mHeader.numSurfaces = r.readLong();
	mHeader.surfaceDataOffset = r.readLong();
	mHeader.numTags = r.readLong();
	mHeader.tagDataOffset = r.readLong();
	mHeader.fileSize = r.readLong();

	if (strncmp(mHeader.ident, "MDMW", 4) || mHeader.version != 3)
	{
		printf("Not a valid MDM model.\n");
		r.closeFile();
		return false;
	}

	mSurfaces = new MDMSurface[mHeader.numSurfaces];

	for (i = 0; i < mHeader.numSurfaces; ++i)
	{
		mSurfaces[i].header.ident = r.readLong();
		r.readCharString(64, mSurfaces[i].header.name);
		r.readCharString(64, mSurfaces[i].header.shader_name);
		mSurfaces[i].header.shader_index = r.readLong();
		mSurfaces[i].header.LoD_minimum = r.readLong();
		mSurfaces[i].header.surface_header_offset = r.readLong();
		mSurfaces[i].header.num_vertices = r.readLong();
		mSurfaces[i].header.offset_vertices = r.readLong();
		mSurfaces[i].header.num_triangles = r.readLong();
		mSurfaces[i].header.offset_triangles = r.readLong();
		mSurfaces[i].header.offset_collaspemap = r.readLong();
		mSurfaces[i].header.number_bonerefs = r.readLong();
		mSurfaces[i].header.offset_bonerefs = r.readLong();
		mSurfaces[i].header.offset_surface_data_end = r.readLong();

		printf("surface = '%s', %li v %li f %li br\n", 
			   mSurfaces[i].header.name, 
			   mSurfaces[i].header.num_vertices,
			   mSurfaces[i].header.num_triangles,
			   mSurfaces[i].header.number_bonerefs);

		mSurfaces[i].vertices = new SurfaceVertex[mSurfaces[i].header.num_vertices];

		for (j = 0; j < mSurfaces[i].header.num_vertices; ++j)
		{
			mSurfaces[i].vertices[j].normal[0] = r.readFloat32();
			mSurfaces[i].vertices[j].normal[1] = r.readFloat32();
			mSurfaces[i].vertices[j].normal[2] = r.readFloat32();
			mSurfaces[i].vertices[j].uv[0] = r.readFloat32();
			mSurfaces[i].vertices[j].uv[1] = r.readFloat32();
			mSurfaces[i].vertices[j].num_weights = r.readLong();


			mSurfaces[i].vertices[j].weights = new BoneWeight[mSurfaces[i].vertices[j].num_weights];

			for (k = 0; k < mSurfaces[i].vertices[j].num_weights; ++k)
			{
				mSurfaces[i].vertices[j].weights[k].bone_index = r.readLong();
				mSurfaces[i].vertices[j].weights[k].weight = r.readFloat32();
				mSurfaces[i].vertices[j].weights[k].bone_space[0] = r.readFloat32();
				mSurfaces[i].vertices[j].weights[k].bone_space[1] = r.readFloat32();
				mSurfaces[i].vertices[j].weights[k].bone_space[2] = r.readFloat32();
			}
		}
		

		mSurfaces[i].triangles = new SurfaceTriangle[mSurfaces[i].header.num_triangles];

		for (j = 0; j < mSurfaces[i].header.num_triangles; ++j)
		{
			mSurfaces[i].triangles[j].indices[0] = r.readLong();
			mSurfaces[i].triangles[j].indices[1] = r.readLong();
			mSurfaces[i].triangles[j].indices[2] = r.readLong();
		}

		/* Collapse maps */
		printf("@ %u, offset_collaspemap = %li?\n",
			   r.getFileOffset(), mSurfaces[i].header.offset_collaspemap);
		mSurfaces[i].map= new long[mSurfaces[i].header.num_vertices];
			
		for (j = 0; j < mSurfaces[i].header.num_vertices; ++j)
		{
			mSurfaces[i].map[j] = r.readLong();
		}	


		/* Bone refs */
		printf("@ %u, bone off = %li?\n",
			   r.getFileOffset(), mSurfaces[i].header.offset_bonerefs);

		mSurfaces[i].bonerefs = new long[mSurfaces[i].header.number_bonerefs];

		for (j = 0; j < mSurfaces[i].header.number_bonerefs; ++j)
		{
			mSurfaces[i].bonerefs[j] = r.readLong();
		}

		printf("@ %u, offset_surface_data_end = %li?\n",
			   r.getFileOffset(), mSurfaces[i].header.offset_surface_data_end);

	}

	/* Dem Bones */
	mTags = new MDMTag[mHeader.numTags];

	for (i = 0; i < mHeader.numTags; ++i)
	{
		r.readCharString(64, mTags[i].name);

		for (j = 0; j < 9; ++j)	
			mTags[i].tag_triangles_vector[j] = r.readFloat32();

		mTags[i].attach_bone_number = r.readLong();

		printf("bone '%s', parent = %li\n", 
			   mTags[i].name, mTags[i].attach_bone_number);

		mTags[i].tag_pos_vector_to_bone[0] = r.readFloat32();
		mTags[i].tag_pos_vector_to_bone[1] = r.readFloat32();
		mTags[i].tag_pos_vector_to_bone[2] = r.readFloat32();

		mTags[i].bone_number_list_size = r.readLong();
		mTags[i].bone_list_offset = r.readLong();
		mTags[i].tag_data_size = r.readLong();

		mTags[i].bone_index_list = new long[mTags[i].bone_number_list_size];

		for (j = 0; j < mTags[i].bone_number_list_size; ++j)
			mTags[i].bone_index_list[j] = r.readLong();
	}

	r.closeFile();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


#include <freyja8/EggPlugin.h>

extern "C" {

	int freyja_model__mdm_check(char *filename);
	int freyja_model__mdm_import(char *filename);
	int freyja_model__mdm_export(char *filename);
}



int freyja_model__mdm_check(char *filename)
{
	EggFileReader r;
	char ident[4];
	long version;


	if (r.openFile(filename) == false)
		return false;

	r.readCharString(4, ident);
	version = r.readLong();
	r.closeFile();

	if (!strncmp(ident, "MDMW", 4) && version == 3)
		return 0;

	return -1;
}


int freyja_model__mdm_import(char *filename)
{
	MDMModel mdm;
	long i, j, k, b;
	unsigned int index;
	float x, y, z, w;
	float xx, yy, zz;
	float *mat;


	if (mdm.load(filename) == false)
		return -1;

	mdm.print();


	Vector<unsigned int> transV;

	for (i = 0; i < mdm.mHeader.numSurfaces; ++i)
	{
		eggBegin(FREYJA_MESH);
		eggBegin(FREYJA_GROUP);

		for (j = 0; j < mdm.mSurfaces[i].header.num_vertices; ++j)
		{
			// It's 0600+ no sleep no real deforms you bitches!
			for (k = 0; k < mdm.mSurfaces[i].vertices[j].num_weights; ++k)
			{
				b = mdm.mSurfaces[i].vertices[j].weights[k].bone_index;
				w = mdm.mSurfaces[i].vertices[j].weights[k].weight;
				mat = mdm.mTags[b].tag_triangles_vector;

				x = mdm.mSurfaces[i].vertices[j].weights[k].bone_space[0]; 
				y = mdm.mSurfaces[i].vertices[j].weights[k].bone_space[1];
				z = mdm.mSurfaces[i].vertices[j].weights[k].bone_space[2];

				xx = x + mdm.mTags[b].tag_pos_vector_to_bone[0];
				yy = y + mdm.mTags[b].tag_pos_vector_to_bone[1];
				zz = z + mdm.mTags[b].tag_pos_vector_to_bone[2];
			}

			index = eggVertexStore3f(xx, yy, zz);
			eggVertexUVStore2f(index,
							   mdm.mSurfaces[i].vertices[j].uv[0],
							   mdm.mSurfaces[i].vertices[j].uv[1]);
			eggVertexNormalStore3f(index, 
								   mdm.mSurfaces[i].vertices[j].normal[0],
								   mdm.mSurfaces[i].vertices[j].normal[1],
								   mdm.mSurfaces[i].vertices[j].normal[2]);
			transV.pushBack(index);
		}

		eggEnd(); // FREYJA_GROUP


		for (j = 0; j < mdm.mSurfaces[i].header.num_triangles; ++j)
		{
			eggBegin(FREYJA_POLYGON);
			eggVertex1i(transV[mdm.mSurfaces[i].triangles[j].indices[0]]);
			eggVertex1i(transV[mdm.mSurfaces[i].triangles[j].indices[1]]);
			eggVertex1i(transV[mdm.mSurfaces[i].triangles[j].indices[2]]);
			eggEnd(); // FREYJA_POLYGON
		}

		transV.clear();

		eggEnd(); // FREYJA_MESH
	}


	eggBegin(FREYJA_SKELETON);

	for (i = 0; i < mdm.mHeader.numTags; ++i)
	{
		printf("b '%s', %li\n", mdm.mTags[i].name, mdm.mTags[i].attach_bone_number);

		eggBegin(FREYJA_BONE);
		eggTagFlags1u(0x0);
		eggSetBoneParent(mdm.mTags[i].attach_bone_number);
		eggTagName(mdm.mTags[i].name);

		eggTagPos3f(mdm.mTags[i].tag_pos_vector_to_bone[0],
					mdm.mTags[i].tag_pos_vector_to_bone[1],
					mdm.mTags[i].tag_pos_vector_to_bone[2]);

		printf("%f %f %f\n",
			   mdm.mTags[i].tag_pos_vector_to_bone[0],
			   mdm.mTags[i].tag_pos_vector_to_bone[1],
			   mdm.mTags[i].tag_pos_vector_to_bone[2]);

		mat = mdm.mTags[i].tag_triangles_vector;

		// just some crap to remember it's a matrix
		// mat[0*3+0]  mat[1*3+0]  mat[2*3+0]
		// mat[0*3+1]  mat[1*3+1]  mat[2*3+1]
		// mat[0*3+2]  mat[1*3+2]  mat[2*3+2]

		xx = atan2(-mat[2*3+0], mat[2*3+2]);
		yy = atan2(-mat[1*3+2], mat[1*3+1]);
		zz = asin(mat[1*3+0]);

		printf("%f %f %f\n", xx, yy, zz);

		eggTagRotate3f(xx, yy, zz);
 
		for (j = 0; j < mdm.mHeader.numTags; ++j)
		{
			if (mdm.mTags[j].attach_bone_number == i)
			{
				printf("%li %li  %li\n", j, i, mdm.mTags[j].attach_bone_number);
				eggTagAddSlave1u(i);
			}
		}
		
		eggEnd(); // FREYJA_BONE
	}

	eggEnd(); // FREYJA_SKELETON	

	return 0;
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_ETMODEL
int runETModelUnitTest(int argc, char *argv[])
{
	ETModel test;

	MDMModel t;

	t.load(argv[1]);
	t.print();

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[ETModel class test]\n");

	return runETModelUnitTest(argc, argv);
}
#endif
