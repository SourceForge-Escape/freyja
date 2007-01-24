/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : UTMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the Unreal mesh classes
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.03.04:
 * Mongoose - Created
 =================================================================*/

#include "UTMesh.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int UTMesh2003::read_index(mstl::SystemIO::BufferedFileReader &r, unsigned int &bytes)
{
	int val;
	char b0, b1, b2, b3, b4;

	bytes = 0;                                                         
	val = 0;
   
	b0 = r.ReadByte();//fread(&b0, 1, 1, f);
	++bytes;
   
	if (b0 & 0x40)
	{
		b1 = r.ReadByte();//fread(&b1, 1, 1, f);
 		++bytes;

		if (b1 & 0x80)
		{
			b2 = r.ReadByte();//fread(&b2, 1, 1, f);
	 		++bytes;
 
			if (b2 & 0x80)
			{
				b3 = r.ReadByte();//fread(&b3, 1, 1, f);
		 		++bytes;
 
				if (b3 & 0x80)
				{
					b4 = r.ReadByte();//fread(&b4, 1, 1, f);
			 		++bytes;
					val = b4;
				}
				
				val = (val << 7) + (b3 & 0x7f);
			}
			
			val = (val << 7) + (b2 & 0x7f);
		}
		
		val = (val << 7) + (b1 & 0x7f);
	}
	
	val = (val << 6) + (b0 & 0x3f);
   
	if (b0 & 0x80)
		val = -val;

	return val;
}


bool UTMesh2003::search_for_index(mstl::SystemIO::BufferedFileReader &r,
					  unsigned int offset, unsigned int count, 
					  unsigned int val, 
					  unsigned int &first, unsigned int &bytes) 
{
	int test;

	for (unsigned int i = 0; i < count; ++i)
	{
		if (offset-(i+1) <= 0)
			return false;

		r.SetOffset(offset-(i+1));
		test = read_index(r, bytes);

		if (test == (int)val)
		{
			first = offset-(i+1);
			return true;
		}
	}

	return false;
}



bool UTMesh2003::search_for_indexf(mstl::SystemIO::BufferedFileReader &r,
					  unsigned int offset, unsigned int count, 
					  unsigned int val, 
					  unsigned int &first, unsigned int &bytes) 
{
	int test;

	for (unsigned int i = 0; i < count; ++i)
	{
		r.SetOffset(offset+i);
		test = read_index(r, bytes);

		if (test == (int)val)
		{
			first = offset+i;
			return true;
		}
	}

	return false;
}


void UTMesh2003::search_for_index_setf(mstl::SystemIO::BufferedFileReader &r,
						   unsigned int offset, unsigned int count, 
						   unsigned int val, 
						   mstl::Vector<int> &size, 
						   mstl::Vector<unsigned int> &pos) 
{
	for (unsigned int i = 0, bytes; i < count; ++i)
	{
		r.SetOffset(offset+i);

		if (read_index(r, bytes) == (int)val)
		{
			size.push_back(bytes);
			pos.push_back(offset+i);
		}
	}
}


void UTMesh2003::read_index_set(mstl::SystemIO::BufferedFileReader &r,
					mstl::Vector<int> &set,
					unsigned int offset, unsigned int count) 
{
	for (unsigned int i = 0, bytes; i < count; ++i)
	{
		r.SetOffset(offset+i);
		set.push_back(read_index(r, bytes));
	}
}


bool UTMesh2003::test_vertex_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_vert_t &v)
{
	r.SetOffset(offset);//fseek(f, offset, SEEK_SET);
	v.x = r.ReadFloat32();//fread(&v.x, 4, 1, f);
	v.y = r.ReadFloat32();//fread(&v.y, 4, 1, f);
	v.z = r.ReadFloat32();//fread(&v.z, 4, 1, f);
	
	return (v.x < 100000.0f && v.x > -100000.0f && 
	        v.y < 100000.0f && v.y > -100000.0f &&
	        v.z < 100000.0f && v.z > -100000.0f &&
	        v.x != FP_NAN && v.y != FP_NAN && v.z != FP_NAN);
}


bool UTMesh2003::test_wedge_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_wedge_t &w)
{
	r.SetOffset(offset);//fseek(f, offset, SEEK_SET);
	w.s = r.ReadInt16U();//fread(&w.s, 2, 1, f);
	w.u = r.ReadFloat32();//fread(&w.u, 4, 1, f);
	w.v = r.ReadFloat32();//fread(&w.v, 4, 1, f);
	
	return (w.s >= 0 && w.u >= 0.0f && w.u <= 1.0f && w.v >= 0.0f && w.v <= 1.0f);
}


bool UTMesh2003::test_face_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_face_t &face)
{
	r.SetOffset(offset);//fseek(f, offset, SEEK_SET);
	face.a = r.ReadInt16U();//fread(&face.a, 2, 1, f);
	face.b = r.ReadInt16U();//fread(&face.b, 2, 1, f);
	face.c = r.ReadInt16U();//fread(&face.c, 2, 1, f);
	face.mat = r.ReadByte();//fread(&face.mat, 1, 1, f);
	face.aux = r.ReadByte();//fread(&face.aux, 1, 1, f);
	face.group = r.ReadInt32U();//fread(&face.group, 4, 1, f);
	
	return (face.a >= 0 && face.b >= 0 && face.c >= 0 &&
	        face.a != face.b && face.a != face.c && face.b != face.c);
}


bool UTMesh2003::read_skeletalmesh(mstl::SystemIO::BufferedFileReader &r)
{
	unsigned int i, bytes, idx;
	float n;
	short s;
	unsigned char b;

	// 0 -- likely player, 8 -- likely monster
	idx = read_index(r, bytes);
	printf("# %i\n", idx);

	for (i = 0; i < 6; ++i)
	{
		n = r.ReadFloat32();
		printf("# %f\n", n);
	}

	idx = read_index(r, bytes);
	printf("# %i == 1?\n", idx);	

	for (i = 0; i < 4; ++i)
	{
		n = r.ReadFloat32();
		printf("# %f\n", n);
	}

	s = r.ReadInt16U();
	printf("# %i == 5?\n", s);

	s = r.ReadInt16U();
	printf("# %i == 0?\n", s);

	unsigned short vertCount = r.ReadInt16U();
	printf("# vertexCount = %u\n", vertCount);

	s = r.ReadInt16U();
	printf("# %i == 0?\n", s);
	
	b = r.ReadByte();
	printf("# %u == 0?\n", b);


	// Would need to link to UTPackage to get the actual import table names
	int materialRefCount = read_index(r, bytes);
	printf("# @ %li\tIndex materialRefCount = %u\n", r.GetOffset()-1, materialRefCount);
	
	printf("# @ %li\tIndex[%u] materialReference = { ", 
		   r.GetOffset(), materialRefCount);
	
	for (unsigned int j = 0; (int)j < materialRefCount; ++j)
	{
		int materialRef = read_index(r, bytes);
		
		if (j)
			printf(", ");
		
		printf("<%u bytes, %i importTableID>", bytes, materialRef);
	}
	
	printf(" }\n");



	for (i = 0; i < 3; ++i)
	{
		n = r.ReadFloat32();
		printf("# %f == 1.0?\n", n);
	}

	for (i = 0; i < 4; ++i)
	{
		n = r.ReadFloat32();
		printf("# %f\n", n);
	}

	for (i = 0; i < 4; ++i)
	{
		s = r.ReadInt16U();
		printf("# %i\n", s);
	}

	int faceCount = read_index(r, bytes);
	printf("# @ %li\tIndex faceCount = %i bytes, %i\n", r.GetOffset(), bytes, faceCount);

	printf("# %li + %i*2 = %li\n", 
		   r.GetOffset(), faceCount, r.GetOffset() + faceCount * 2);

	int max = 0;

	for (i = 0; (int)i < faceCount; ++i)
	{
		const unsigned int count = 1;

#if SHOW_FACES
		printf("# @ %li\t%u/%i. short[%u] ? = { ", 
			   r.GetOffset(), i, faceCount, count);
#endif

		for (unsigned int j = 0; j < count; ++j)
		{
			s = r.ReadInt16U();
			if (s > max) max = s;
#if SHOW_FACES
			if (j)
				printf(", ");

			printf("%u", s);
#endif
		}

#if SHOW_FACES
		printf(" }\n");
#endif
	}

	printf("# max = %i\n", max);

	printf("# vertCount = %u, faceCount = %i\n", vertCount, faceCount);

	printf("# wedgeGuess =  vertOffset + %u + bytes\n", (vertCount*12)+8);

	bool dumped = false;

	for (unsigned int j = 0, count = r.GetFileSize(); j < count; ++j)
	{
		r.SetOffset(j);
		int value = read_index(r, bytes);
		
		if (value >= 1 && value == faceCount)
		{
			printf("# @ %u, %i bytes, %i\n", j, bytes, value);
			unsigned int faceOffset = j+bytes;
			int maxWedge = -1;
			l2_face_t face;

			for (unsigned int k = 0; (int)k < value; ++k)
			{
				if (test_face_offset(r, faceOffset+k*12, face))
				{
					if (face.a > maxWedge) maxWedge = face.a;
					if (face.b > maxWedge) maxWedge = face.b;
					if (face.c > maxWedge) maxWedge = face.c;
				}
			}

			unsigned int wedgeOffset = faceOffset - ((maxWedge+1) * 10) - 4 - bytes;
			unsigned int wedgeCount = maxWedge+1;
			
			if (maxWedge == -1 || wedgeOffset > count)
				continue;

			int maxVertex = -1;

			r.SetOffset(wedgeOffset);
			for (unsigned int k = 0; k < wedgeCount; ++k)
			{
				l2_wedge_t w;
				unsigned long off = r.GetOffset();
				
				if (test_wedge_offset(r, off, w))
				{
					if (w.s > maxVertex) maxVertex = w.s;
				}
			}


			if (maxVertex > -1)
			{
				unsigned int vertGuess = wedgeOffset - ((maxVertex+1) * 12) + bytes - 8;

				printf("#\tVertices @ %i x %u\n", vertGuess, maxVertex+1);
				printf("#\tWedges @ %u x %u\n", wedgeOffset, wedgeCount);
				printf("#\tFaces @ %u x %u\n", faceOffset, faceCount);
				printf("#\t%s%u %u %u %u %u %u\n",
					   (vertCount == maxVertex+1) ? "*Agrees " : "",
					   vertGuess, maxVertex+1,
					   wedgeOffset, wedgeCount, 
					   faceOffset, faceCount);

				// Go ahead and dump it out...
				if (!dumped && vertCount == maxVertex+1)
				{
					dumped = true; // Only try one dump per session!
					unsigned int vc = maxVertex+1, vo = vertGuess, 
					wc = wedgeCount, wo = wedgeOffset, 
					fc = faceCount, fo = faceOffset;

					if (generic_mesh_search(r, vc, vo, wc, wo, fc, fo))
					{
						printf("# Bob's your uncle\n");
					}
				}

				printf("#\tVertices @ %i x %u\n", vertGuess, maxVertex+1);
				printf("#\tWedges @ %u x %u\n", wedgeOffset, wedgeCount);
				printf("#\tFaces @ %u x %u\n", faceOffset, faceCount);
				printf("#\t%s%u %u %u %u %u %u\n",
					   (vertCount == maxVertex+1) ? "*Agrees " : "",
					   vertGuess, maxVertex+1,
					   wedgeOffset, wedgeCount, 
					   faceOffset, faceCount);
			}
			else
			{
				printf("#\tVertices @ ? x ?\n");
				printf("#\tWedges @ %u x %u\n", wedgeOffset, wedgeCount);
				printf("#\tFaces @ %u x %u\n", faceOffset, faceCount);
				printf("#\t? ? %u %u %u %u\n", 
					   wedgeOffset, wedgeCount, 
					   faceOffset, faceCount);
			}


		}
	}

	printf("%i\n", 1<<20);

	return false;
}


bool UTMesh2003::generic_mesh_search(mstl::SystemIO::BufferedFileReader &r,
						 unsigned int &vertCount, unsigned int &vertOffset,
						 unsigned int &wedgeCount, unsigned int &wedgeOffset,
						 unsigned int &faceCount, unsigned int &faceOffset)
{
	if (!vertCount || !vertOffset || 
		 !wedgeCount || !wedgeOffset ||
		 !faceCount || !faceOffset ||
		 vertCount > MAX_VERTICES ||
		 wedgeCount > MAX_WEDGES ||
		 faceCount > MAX_FACES)
	{
		return false;
	}

	mstl::Vector<mstl::String> vertices;
	mstl::Vector<mstl::String> wedges;
	mstl::Vector<mstl::String> faces;

	// Wedges
	r.SetOffset(wedgeOffset);
	unsigned int maxVertex = 0;
	unsigned int addtionalWedgesPre = 0;
	unsigned int addtionalWedgesPost = 0;

	for (unsigned int i = 0, off = wedgeOffset; i < wedgeCount; ++i)
	{
		l2_wedge_t w;
		test_wedge_offset(r, off, w);
		off += 10;

		printf("# Wedge[%i] %i %f %f\n", i, w.s, w.u, w.v);

		if ((int)maxVertex < w.s) maxVertex = w.s;

		if (vertCount && w.s < (int)vertCount)
		{
			// It's already buffered in memory, so just read it out
			r.SetOffset(vertOffset + w.s * 12);
			float x = r.ReadFloat32(), y = r.ReadFloat32(), z = r.ReadFloat32();
			printf("v %f %f %f\n", x, z, y);
		}
		else
		{
			printf("v 0.0 0.0 0.0\n");
		}

		printf("vt %f %f\n", w.u, w.v);
	}


	// Faces
	r.SetOffset(faceOffset);
	unsigned int maxWedge = 0, faceMissing = 0;
	int errFace = -1;
	unsigned char mat = 61;

	for (unsigned int i = 0; i < faceCount; ++i)
	{
		l2_face_t face;
		unsigned long off = r.GetOffset();
		bool err = false;

		if (test_face_offset(r, off, face))
		{
			printf("# Face[%i] %i %i %i %u %u %u\n", 
				   i, face.a, face.b, face.c, face.mat, face.aux, face.group);

			if (!err)
			{
				if (face.a > (int)wedgeCount || face.b > (int)wedgeCount || 
					face.c > (int)wedgeCount)
				{
					++faceMissing;
					printf("# Missing wedge index f %i/%i %i/%i %i/%i\n", 
						   face.a+1, face.a+1, 
						   face.b+1, face.b+1, 
						   face.c+1, face.c+1);
				}
				else
				{
					if (face.mat != mat)
					{
						mat = face.mat;
						printf("s %u\n", face.mat + 1);	

						printf("usemap texmap%u\n", face.mat + 1);
					}

					printf("f %i/%i %i/%i %i/%i\n", 
						   face.a+1, face.a+1, 
						   face.b+1, face.b+1, 
						   face.c+1, face.c+1);
				}

				if ((int)maxWedge < face.a) maxWedge = face.a;
				if ((int)maxWedge < face.b) maxWedge = face.b;
				if ((int)maxWedge < face.c) maxWedge = face.c;
			}
			else
			{
				++faceMissing;
				printf("#f %i/%i %i/%i %i/%i\n", 
					   face.a+1, face.a+1, 
					   face.b+1, face.b+1, 
					   face.c+1, face.c+1);
			}

			off += 12;
		}
		else
		{
			++faceMissing;

			// By cutting off at first Error you can find max wedge index
			// for comparing with expected max wedge index
			if (!err)
				errFace = i;

			err = true;

			printf("#Error @ %lu face[%i] %i %i %i %u %u %u\n", off, i, face.a, face.b, face.c, face.mat, face.aux, face.group);
			off += 12; // Just to keep moving the tape
		}
	}


	printf("#####################################################\n");
	printf("# Model and report generated by lineageIIraw2obj\n");
	printf("# by Terry Hendrix <mongooseichiban@gmail.com>\n");
	printf("#\n");
	//printf("#  Filename %s\n", argv[1]);
	printf("#  Vertices @ %i x %i\n", vertOffset, vertCount);
	printf("#  Wedges @ %i x %i\n", wedgeOffset, wedgeCount); 
	printf("#  Faces @ %i x %i\n", faceOffset, faceCount);
	printf("#\n");
	printf("# Possible count corrections:\n");
	printf("#  maxVertexIndex = %u\n", maxVertex);
	printf("#  maxWedgeIndex = %u\n", maxWedge);
	printf("#\n");
	printf("# First face with error: %i%s\n", 
		   errFace, (errFace == -1) ? ", No errors" : "");
	printf("# Disabled faces: %u of %u\n", faceMissing, faceCount);
	printf("#\n");
	printf("# Extra wedges found: %u preceding, %u trailing.\n", 
		   addtionalWedgesPre, addtionalWedgesPost);
	printf("#\n");

	
	if (wedgeCount) 
	{
		unsigned int i, offset, bytes;
		if (search_for_index(r, wedgeOffset, 4, wedgeCount, offset, bytes))
		{
			printf("#  Index WedgeCount @ %u, %u bytes; %s\n", 
				   offset, bytes,
				   (offset + bytes == wedgeOffset) ? "*Agrees" : "Disagrees");

			unsigned int vertGuess = wedgeOffset - ((maxVertex+1) * 12) + bytes - 8;

			printf("#  Old method VertexGuess @ %i x %u\n", vertGuess, maxVertex+1);
			vertOffset = vertGuess;
			vertCount = maxVertex + 1;
		}
		else
		{
			// Might want to pull a set here and dump it like the prev version
			printf("#  Index WedgeCount not found precedding wedges\n");

			mstl::Vector<int> set;
			//read_index_set(r, set, wedgeOffset, 4);

			printf("#  Possible counts: { ");

			foreach (set, i)
			{
				if (i)
					printf(", ");

				printf("%i", set[i]);
			}

			printf(" }\n");
		}
	}


	// Look for face count in the 'header'
	if (faceCount) 
	{
		printf("#\n");

		unsigned int i, offset, bytes;
		mstl::Vector<int> set;
		if (search_for_indexf(r, 80, 32, faceCount, offset, bytes))
		{
			printf("#  Index FaceCount @ %u, %u bytes\n", offset, bytes);
		}
		else
		{
			// Might want to pull a set here and dump it like the prev version
			printf("#  Index FaceCount not found in 'header'\n");

			read_index_set(r, set, 80, 32);

			printf("#  Possible counts: { ");

			foreach (set, i)
			{
				if (i)
					printf(", ");

				printf("%i", set[i]);
			}

			printf(" }\n");
		}


		// Now look for Index FaceCount near Faces
		int idx[4];
		unsigned int pick = 0;

		printf("#  Face index set = { ");

		for (i = 0; i < 4; ++i)
		{
			if (i)
			{
				printf(", ");
			}

			r.SetOffset(faceOffset-(i+1));
			idx[i] = read_index(r, bytes);

			if (idx[i] == (int)faceCount)
			{
				pick = bytes;
				printf("*");
			}

			printf("%i", idx[i]);
		}

		printf(" }\n");

		if (pick)
		{
			bytes = pick;
			unsigned wedgeGuess = faceOffset - ((maxWedge+1) * 10) - 4 - bytes;
			printf("#  Old method WedgeGuess @ %i x %u\n", wedgeGuess, maxWedge+1);
			wedgeOffset = wedgeGuess;
			wedgeCount = maxWedge+1;
		}
		else
		{
			// First see if we have a local 'fix count', which can happen 
			int found = -1;

			foreach (set, i)
			{
				for (unsigned int j = 0; j < 4; ++j)
				{
					if (idx[j] == set[i])
					{
						found = idx[j];
						offset = faceOffset - j + 2; // Assuming Index size == 2
						break;
					}
				}
			}

			printf("#\n");

			if (found > -1)
			{
				printf("#  Possible FaceGuess @ %u x %i\n", offset, found);
			}



			// Always do 'full Index scans' in case a bad face candidate won out
			foreach (set, i)
			{
				// FIXME: Allow for picking instead of just using first floor

				// Use current faceCount as a floor
				if (set[i] >= (int)faceCount)
				{
					for (unsigned int j = 0, n = r.GetFileSize(); j < n; ++j)
					{
						r.SetOffset(j);

						if (read_index(r, bytes) == set[i])
						{
							// Ceiling at current Faces offset
							if (j > faceOffset)
							{
								//printf("#  Least FaceGuess @ %u x %i\n", j+bytes, set[i]);
								printf("# If none of those look valid use index search mode\n");
								break;
							}
							else
							{
								printf("#  Likely FaceGuess @ %u x %i\n", j+bytes, set[i]);
								wedgeOffset = j+bytes;
								wedgeCount = set[i];
							}
						}
					}

					break;
				}
			}
		}
	}


	// TODO: Add a summary guess here... and then write auto cycling option then that's it
	

	printf("#####################################################\n");


	return false;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_UTMESH
int runUTMeshUnitTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
	UTMesh test;


	printf("[UTMesh class test]\n");

	return runUTMeshUnitTest(argc, argv);
}
#endif
