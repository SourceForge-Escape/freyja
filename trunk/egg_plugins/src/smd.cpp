/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Obj plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-09:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja8/EggPlugin.h>
#include <freyja8/EggFileReader.h>
#include <freyja8/EggFileWriter.h>


extern "C" {

	int freyja_model__smd_check(char *filename);
	int freyja_model__smd_import(char *filename);
	int freyja_model__smd_export(char *filename);
}



int freyja_model__smd_check(char *filename)
{
	if (EggFileReader::compareFilenameExtention(filename, ".smd") == 0)
		return 0;

	return -1;
}


typedef struct {

	int parent;
	char *name;

} smd_bone_t;


int freyja_model__smd_import(char *filename)
{
	EggFileReader r;
	Vector <unsigned int> transV;
	Vector <smd_bone_t *> bones;
	char *symbol;
	vec_t x, y, z, rx, ry, rz;
	int index, idx;
	unsigned int i, n;
	vec_t scale = 0.15;
	vec_t r2d = 57.295779513082323;


	if (freyja_model__smd_check(filename) < 0 || !r.openFile(filename))
	{
		return -1;
	}

	while ((symbol = r.parseSymbol()) && !r.endOfFile())
	{
		if (!strncmp(symbol, "version", 7))
		{
			index = r.parseInteger();
		}
		else if (!strncmp(symbol, "nodes", 5))
		{
			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0)
			{
				smd_bone_t *bone = new smd_bone_t;

				if (!symbol)
					break;

				index = atoi(symbol);
				bone->name = r.parseString();
				bone->parent = r.parseInteger();
				bones.pushBack(bone);
			}
		}
		else if (!strncmp(symbol, "skeleton", 8))
		{
			eggBegin(FREYJA_SKELETON);

			symbol = r.parseSymbol();
			index = r.parseInteger();  // time

			printf("%s = %i\n", symbol, index);

			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0 && !r.endOfFile())
			{
				if (!symbol)
					break;

				index = atoi(symbol);
				//printf("%s <- %i?\n", symbol, index);

				smd_bone_t *bone = bones[index];
				//printf("bone[%i]\n", index);

				if (bone && index < (int)bones.end())
				{
					x = r.parseFloat();
					y = r.parseFloat();
					z = r.parseFloat();

					rx = r.parseFloat();
					ry = r.parseFloat();
					rz = r.parseFloat();

					idx = eggBegin(FREYJA_BONE);
					eggTagFlags1u(0x0);
					eggSetBoneParent(bone->parent);
					eggTagName(bone->name);

					//printf("%3i: %s %i\n", idx, bone->name, bone->parent);

					if (!index)
					{
						eggTagPos3f(x*scale, z*scale, y*scale);
						eggTagRotate3f(rx*r2d, (ry*r2d) - 90.0, rz*r2d);
					}
					else
					{
						eggTagPos3f(x*scale, y*scale, z*scale);
						eggTagRotate3f(rx*r2d, ry*r2d, rz*r2d);
					}

					for (i = bones.begin(); i < bones.end(); ++i)
					{
						bone = bones[i];

						if (bone && bone->parent == index)
						{ 
							eggTagAddSlave1u(i);
						}
					}

					eggEnd(); // FREYJA_BONE
				}
			}

			eggEnd(); // FREYJA_SKELETON
		}
		else if (!strncmp(symbol, "triangles", 9))
		{
			n = 0;

			// Start a new mesh
			eggBegin(FREYJA_MESH);
			eggBegin(FREYJA_GROUP);

			while ((symbol = r.parseSymbol()) && strncmp(symbol, "end", 3) != 0 && !r.endOfFile())
			{
				if (!symbol)
					break;

				//printf("!!!%s %i\n", symbol, n);

				// symbol is material filename eg 'null.bmp'
				++n;

				eggBegin(FREYJA_POLYGON);

				index = r.parseInteger();
				eggTexture1i(index);

				eggVertex1i(i = eggVertexStore3f(r.parseFloat()*scale,
												 r.parseFloat()*scale,
												 r.parseFloat()*scale));
				eggVertexNormalStore3f(i, r.parseFloat(),
									   r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());


				index = r.parseInteger();
				eggVertex1i(i = eggVertexStore3f(r.parseFloat()*scale,
												 r.parseFloat()*scale,
												 r.parseFloat()*scale));
				eggVertexNormalStore3f(i, r.parseFloat(),
									   r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());


				index = r.parseInteger();
				eggVertex1i(i = eggVertexStore3f(r.parseFloat()*scale, 
												 r.parseFloat()*scale,
												 r.parseFloat()*scale));
				eggVertexNormalStore3f(i, r.parseFloat(), 
									   r.parseFloat(),
									   r.parseFloat());
				eggVertexUVStore2f(i, r.parseFloat(), r.parseFloat());
							  							  
				eggEnd(); // FREYJA_POLYGON
			}

			eggEnd(); // FREYJA_GROUP
			eggEnd(); // FREYJA_MESH
		}
	}


	return 0;
}


int freyja_model__smd_export(char *filename)
{
	EggFileWriter w;
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

	if (eggGetNum(FREYJA_BONE))
	{
		eggIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		eggIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		n = eggGetNum(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = eggIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
			index = eggGetCurrent(FREYJA_BONE);

			eggGetBoneName(index, 64, name);
			w.print("%3i \"%s\" %i\n", i, name, eggGetBoneParent(index));

			eggIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}

	w.print("end\n"); // nodes

	/* skeleton */
	w.print("skeleton\n");
	w.print("time 0\n");

	if (eggGetNum(FREYJA_BONE))
	{
		eggIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		eggIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		n = eggGetNum(FREYJA_BONE);

		for (i = 0; i < n; ++i)
		{
			index = eggIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
			index = eggGetCurrent(FREYJA_BONE);

			eggGetBoneTranslation3fv(index, translation);
			eggGetBoneRotationXYZ3fv(index, rotation);

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

			eggIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}
	w.print("end\n");

	/* triangles */
	w.print("triangles\n");
#ifdef SMD_MESH_EXPORT
	unsigned int j, k, group;
	vec3_t vert;
	vec2_t uv;


	if (eggGetNum(FREYJA_POLYGON))
	{
		eggIterator(FREYJA_MESH, FREYJA_LIST_RESET);
		eggIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
		n = eggGetNum(FREYJA_POLYGON);

		group = 0;

#ifdef SMD_MESH_EXPORT_ALL
		unsigned int meshCount = eggGetNum(FREYJA_MESH);
		for (i = 0; i < meshCount; ++i, group = i) // send all meshes
#endif
		for (j = 0; j < n; ++j)
		{
			index = eggIterator(FREYJA_POLYGON, FREYJA_LIST_CURRENT);
			index = eggGetCurrent(FREYJA_POLYGON);

			for (j = 0; j < n; ++j)
			{
				eggGetPolygon3f(FREYJA_VERTEX, j, &vert);
				eggGetPolygon3f(FREYJA_TEXCOORD, j, &uv);

				w.print("null.png");
				w.print("%3i  %f %f %f  %f %f %f  %f %f\n", group,
						vert[0]*scale, vert[2]*scale, vert[1]*scale, 
						norm[0], norm[2], norm[1],
						uv[0], uv[1]);
			}

			eggIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
		}
	}
#endif
	w.print("end\n");

	w.closeFile();

	return 0;
}
