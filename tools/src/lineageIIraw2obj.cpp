/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry Hendrix ( Mongoose )
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: 
 * 
 * This is a pretty hacky lineageII raw SkeletalMesh to OBJ test app.
 * 
 * It should be used to design a better application or even finish reverse
 * engineering the various formats.  This app is a compromise of guessing 
 * patterns to avoid supporting versions I've never seen or can test myself.
 *
 ==========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>

using namespace mstl;

typedef struct {
	float x;
	float y;
	float z;

} l2_vert_t;


typedef struct {
	short s;
	float u;
	float v;

} l2_wedge_t;


typedef struct {
	short a;
	short b;
	short c;
	unsigned char mat;
	unsigned char aux;
    unsigned int group;

} l2_face_t;


int read_index(mstl::SystemIO::BufferedFileReader &r, unsigned int &bytes)
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


bool search_for_index(mstl::SystemIO::BufferedFileReader &r,
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


bool search_for_indexf(mstl::SystemIO::BufferedFileReader &r,
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


void search_for_index_setf(mstl::SystemIO::BufferedFileReader &r,
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


void read_index_set(mstl::SystemIO::BufferedFileReader &r,
					mstl::Vector<int> &set,
					unsigned int offset, unsigned int count) 
{
	for (unsigned int i = 0, bytes; i < count; ++i)
	{
		r.SetOffset(offset+i);
		set.push_back(read_index(r, bytes));
	}
}


bool test_vertex_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_vert_t &v)
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


bool test_wedge_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_wedge_t &w)
{
	r.SetOffset(offset);//fseek(f, offset, SEEK_SET);
	w.s = r.ReadInt16U();//fread(&w.s, 2, 1, f);
	w.u = r.ReadFloat32();//fread(&w.u, 4, 1, f);
	w.v = r.ReadFloat32();//fread(&w.v, 4, 1, f);
	
	return (w.s >= 0 && w.u >= 0.0f && w.u <= 1.0f && w.v >= 0.0f && w.v <= 1.0f);
}


bool test_face_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_face_t &face)
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


void search_for_vertices(mstl::Vector<mstl::String> &vertices, 
						 mstl::SystemIO::BufferedFileReader &r,
						 unsigned long offset, unsigned long end)
{
	l2_vert_t v;
	mstl::String str;
	unsigned long old = offset, pick = 0, lastOffset = 0;
	unsigned int last = 0, best = 0;

	for (offset = 0; offset < end; )
	{
		old = offset;

		if (test_vertex_offset(r, offset, v))
		{
			if (last == 0)
				lastOffset = old;

			++last;
			offset = old + 12;
		}
		else
		{
			if (last > best)
			{
				best = last;
				pick = lastOffset;
				//printf("# Possible vertex array @ %lu x %u\n", pick, best);	
				str.Set("%lu,%u", pick, best);
				vertices.push_back(str);
				
			}

			last = 0;
			++offset;
		}
	}

	//printf("# Vertices pick @ %lu x %u\n", pick, best);
}


bool test_wedge_candidates(mstl::SystemIO::BufferedFileReader &r,
						   unsigned int &wedgeOffset, unsigned int &wedgeCount)
{
	l2_wedge_t w;
	unsigned int addtionalWedgesPre = 0;
	unsigned int addtionalWedgesPost = 0;
	unsigned long offset = wedgeOffset - 10;
	
	// Search for precedding wedges
	while (test_wedge_offset(r, offset, w))
	{
		offset -= 10;
		++addtionalWedgesPre;
	}

	// Search for trailing wedges
	offset = wedgeOffset + (wedgeCount * 10);
	
	while (test_wedge_offset(r, offset, w))
	{
		offset += 10;
		++addtionalWedgesPost;
	}
	
	wedgeOffset -= (addtionalWedgesPre * 10);
	wedgeCount += addtionalWedgesPre + addtionalWedgesPost;


	// Test for Index WedgetCount precedding wedges
	unsigned int indexOffset, bytes;
	bool agrees = false;
	if (search_for_index(r, wedgeOffset, 8, wedgeCount, indexOffset, bytes))
	{
		if (indexOffset + bytes == wedgeOffset)
			agrees = true;
	}

	// Very strict, must find a completely valid wedge list AND matching Index
	return agrees; 
}


// This can only find wedges in vertexIndex order, so we need a separate
// function for searching by clustered indices in case some model wedges 
// are serialized out of order.
void search_for_wedges(mstl::Vector<mstl::String> &wedges,
					   mstl::SystemIO::BufferedFileReader &r,
                       unsigned long offset, unsigned long end)
{
	mstl::String str;
	l2_wedge_t w, w2;
	unsigned long first = 0, old = offset, maxVertexIndex = 0;
	int count = 0;
	short oldS = 0;
	long state = 0;

	// Wedge search starting at offset 
	for (offset = 0; offset < end; )
	{
		old = offset;

		if (test_wedge_offset(r, offset, w))
		{
			switch (state)
			{	
			case 0: // Find the first wedge ( Need at least 3 in order )
				if (test_wedge_offset(r, offset+10, w2) && count == 0 
#ifdef ONLY_LOOK_FOR_ORDERED_WEDGES 
					&&
					w.s == 0 && w2.s == 1 && 
					test_wedge_offset(r, offset+20, w2) &&
					w2.s == 2
#endif
)
				{
					first = old;
					count = 1;
					state = 1;
					oldS = w.s;
					offset = old+10;

					maxVertexIndex = (w.s > (int)maxVertexIndex) ? w.s : maxVertexIndex;

					//printf("# Wedge @ %lu - %i, %f, %f\n", old, s, u, v);
				}
				else
				{
					++offset;
				}
				break;
			
			case 1:
				if (count && w.s > 0 &&	w.s == oldS + 1)
				{
					//printf("# Wedge @ %lu - %i, %f, %f\n", old, s, u, v);
					oldS = w.s;
					offset = old+10;
					maxVertexIndex = (w.s > (int)maxVertexIndex) ? w.s : maxVertexIndex;

					++count;
				}
				else
				{
					if ( count > 2 )
					{
						//printf("# Wedges @ %lu x %i\n", first, count);
						str.Set("%lu,%u,%u", first, count, maxVertexIndex);
						wedges.push_back(str);
					}
					maxVertexIndex = 0;
					state = 2;
					++offset;
				}
				break;

			case 2:
				// optional search for 'extra' wedges
				if (1)
				{
					count = 0;
					state = 0;
				}
				break;

			default:
				++offset;
			}
		}
		else
		{
			++offset;
		}
	}

	if (count)
	{
		//printf("# Wedges @ %lu x %i?\n", first, count);

		str.Set("%lu,%u,%u", first, count, maxVertexIndex);
		wedges.push_back(str);
	}
}


//#define DEBUG_FANTEST
bool is_likely_face_candidate(mstl::SystemIO::BufferedFileReader &r,
							  unsigned int offset, unsigned int count)
{
	l2_face_t face;
	int candidates[16][2];
	unsigned int end = offset + count * 12, old = offset;//, best = 0, bestCount;
	int a, b, c;

	for (unsigned int i = 0; i < 16; ++i)
	{
		candidates[i][0] = 0;
		candidates[i][1] = 0;
	}

	//printf("# Faces @ %u x %u ? ( %u - %u )\n", offset, count, offset, end);

	for (; offset < end; offset += 12)
	{
		if (!test_face_offset(r, offset, face))
		{
			printf("# Faces @ %u x %u ? ( %u - %u )\n", old, count, old, end);
			printf("# FAIL %i %i %i\n", face.a, face.b, face.c);
			return false;
		}

		//printf("# %i %i %i\n", face.a, face.b, face.c);
		
		// Need to rewrite this crap with LRU cache or full ref counting...
		a = b = c = -1;
		for (unsigned int i = 0; i < 16; ++i)
		{
			if (candidates[i][0] == face.a)
			{
				candidates[i][1] += 1;
				a = i;
			}

			if (candidates[i][0] == face.b)
			{
				candidates[i][1] += 1;
				b = i;
			}

			if (candidates[i][0] == face.c)
			{
				candidates[i][1] += 1;
				c = i;
			}
		}

		for (unsigned int i = 0; i < 16; ++i)
		{
			if (candidates[i][1] == 0)
			{
				if (a < 0)
				{
					a = i;
					candidates[i][0] = face.a;
					candidates[i][1] = 1;
				}
				else if (a < 0)
				{
					b = i;
					candidates[i][0] = face.b;
					candidates[i][1] = 1;
				}
				else if (c < 0)
				{
					c = i;
					candidates[i][0] = face.c;
					candidates[i][1] = 1;
				}
			}
		}
	}

	for (unsigned int i = 0; i < 16; ++i)
	{
#ifdef DEBUG_FANTEST_ALL
		if (candidates[i][1])
			printf("# FanTest %i. idx = %i, count = %i\n", 
				   i, candidates[i][0], candidates[i][1]);
#endif

		if (candidates[i][1] > 8) // max connectivity allowed for triangle fans
		{
#ifdef DEBUG_FANTEST
			printf("# Faces @ %u x %u ? ( %u - %u )\n", old, count, old, end);
			printf("# FAIL FanTest %i. idx = %i, count = %i\n", 
				   i, candidates[i][0], candidates[i][1]);
#endif
			return false;
		}
	}

	//printf("# Faces @ %u x %u - PASS\n", offset, count);

	return true;
}


// Should look for clusters of same mat or aux
// Should look for overlapping contigous faces 
// Should check for all the indices in each face being in a valid range, and
// avoid gaps
void search_for_faces(mstl::Vector<mstl::String> &faces,
					  mstl::SystemIO::BufferedFileReader &r,
                      unsigned long offset, unsigned long end,
                      unsigned int maxIndex)
{
	l2_face_t face;
	mstl::String str;
	unsigned long old = offset, lastOffset = 0;
	unsigned int last = 0;
	int maxWedge = 0;

#if 0
	unsigned int best = 0;
	unsigned long pick = 0;
#endif

	// Face search starting at offset
	for (offset = 0; offset < end; )
	{
		old = offset;

		if (test_face_offset(r, offset, face) && 
			(maxIndex == 0 ||
			 face.a < (int)maxIndex && 
			 face.b < (int)maxIndex && 
			 face.c < (int)maxIndex))
		{
			if (face.a > maxWedge)
				maxWedge = face.a;
			if (face.b > maxWedge)
				maxWedge = face.b;
			if (face.c > maxWedge)
				maxWedge = face.c;

			//printf("# %sFace @ %lu, { %i, %i, %i,\t %u, %u, %u }\n", last ? "*" : "", offset, face.a, face.b, face.c, face.mat, face.aux, face.group);
			if (last == 0)
				lastOffset = old;

			offset = old + 12;
			++last;
		}
		else
		{
			if (last == 0)
			{
				++offset;
			}
			// Need at least _8_ faces to qualify as a candidate
			else if (last < 9)
			{
				offset = lastOffset + 1;
			}
			else if (is_likely_face_candidate(r, lastOffset, last))
			{
#if 0
				if (last > best)
				{
					best = last;
					pick = lastOffset;
					//printf("# Possible face array @ %lu x %u\n", pick, best);
					str.Set("%lu,%u", pick, best);
					faces.push_back(str);
				}
				else
				{
					offset = lastOffset + 1;
				}
#else
				//printf("# Possible face array @ %lu x %u\n", pick, best);
				str.Set("%lu,%u,%i", lastOffset, last, maxWedge);
				faces.push_back(str);

				++offset;
#endif
			}
			else
			{
				// Wait 10ms to let CPU rest, was seriously 
				// getting reports of overheating due to the brute force nature
				//SystemIO::MicroSleep(10); 
				offset = lastOffset + 1;
			}

			last = maxWedge = 0;
		}
	}	
}


bool read_skeletalmesh(SystemIO::BufferedFileReader &r)
{
	unsigned int i, bytes, idx;
	float n;
	short s;
	unsigned char b;

	// 0 -- likely player, 8 -- likely monster
	idx = read_index(r, bytes);
	printf("%i\n", idx);

	for (i = 0; i < 6; ++i)
	{
		n = r.ReadFloat32();
		printf("%f\n", n);
	}

	idx = read_index(r, bytes);
	printf("%i == 1?\n", idx);	

	for (i = 0; i < 4; ++i)
	{
		n = r.ReadFloat32();
		printf("%f\n", n);
	}

	s = r.ReadInt16U();
	printf("%i == 5?\n", s);

	s = r.ReadInt16U();
	printf("%i == 0?\n", s);

	unsigned short vertCount = r.ReadInt16U();
	printf("vertexCount = %u\n", vertCount);

	s = r.ReadInt16U();
	printf("%i == 0?\n", s);
	
	b = r.ReadByte();
	printf("%u == 0?\n", b);

	b = r.ReadByte();
	printf("readCount = %u\n", b);

	switch (b)
	{
	case 1:
		b = r.ReadByte();
		printf("%u\n", b);
		break;

	case 2:
		s = r.ReadInt16U();
		printf("%i\n", s);
		s = r.ReadInt16U();
		printf("%i\n", s);
		break;

	case 3:
		s = r.ReadInt16U();
		printf("%i\n", s);
		s = r.ReadInt16U();
		printf("%i\n", s);
		s = r.ReadInt16U();
		printf("%i\n", s);
		break;
	}

	for (i = 0; i < 3; ++i)
	{
		n = r.ReadFloat32();
		printf("%f == 1.0?\n", n);
	}

	for (i = 0; i < 4; ++i)
	{
		n = r.ReadFloat32();
		printf("%f\n", n);
	}

	for (i = 0; i < 4; ++i)
	{
		s = r.ReadInt16U();
		printf("%i\n", s);
	}

	int faceCount = read_index(r, bytes);
	printf("faceCount = %i\n", faceCount);

	return false;
}


bool generic_mesh_search(SystemIO::BufferedFileReader &r,
						 unsigned int &wedgeCount, unsigned int &wedgeOffset,
						 unsigned int &vertCount, unsigned int &vertOffset,
						 unsigned int &faceCount, unsigned int &faceOffset)
{
	mstl::Vector<mstl::String> vertices;
	mstl::Vector<mstl::String> wedges;
	mstl::Vector<mstl::String> faces;
	unsigned long end = r.GetFileSize();


	bool vertsGuess = !vertCount || !vertOffset;
	bool wedgeGuess = !wedgeCount || !wedgeOffset;


#if 0
	void search_for_index_setf(mstl::SystemIO::BufferedFileReader &r,
							   unsigned int offset, unsigned int count, 
							   unsigned int val, 
							   mstl::Vector<int> &size, 
							   mstl::Vector<unsigned int> &pos);
#endif

	if (vertCount == 0 || vertOffset == 0)
	{
		unsigned int i, offset = 0, count = 0;
		vertOffset = vertCount = 0;
		
		search_for_vertices(vertices, r, offset, end);

		foreach ( vertices, i )
		{
			sscanf(vertices[i].c_str(), "%u,%u", &offset, &count);
			//SystemIO::Print("# Candidate vertices @ %u x %u\n", offset, count);

			// We want max count
			if (count > vertCount) 
			{
				vertOffset = offset;
				vertCount = count;
			}
		}

		//SystemIO::Print("# Vertices @ %u x %u\n", vertOffset, vertCount);
	}


	if (wedgeOffset == 0 || wedgeCount == 0)
	{
		unsigned int i, offset = 0, count = 0, maxV = 0;
		wedgeOffset = wedgeCount = 0;

		search_for_wedges(wedges, r, wedgeOffset, end);

		foreach ( wedges, i )
		{
			sscanf(wedges[i].c_str(), "%u,%u,%u", &offset, &count, &maxV);
			//SystemIO::Print("# Candidate wedges @ %u x %u, maxVertexIndex = %u\n", offset, count, maxV);

			// We want max count
			if (count > wedgeCount) 
			{
				wedgeOffset = offset;
				wedgeCount = count;
			}
		}

		//SystemIO::Print("# Wedges @ %u x %u\n", wedgeOffset, wedgeCount);
	}


	// We make sure vertices come before wedges, but
	// let people override this behaviour still by passing 
	// vertex offset/count arguments directly
	if (vertOffset > wedgeOffset && vertsGuess)
	{
		unsigned int i, count, offset;

		//SystemIO::Print("# Vertex offset does not agree with Wedge offset\n");
		//SystemIO::Print("# Picking a new candidate based on this...\n");

		vertCount = count = 0;
		foreach ( vertices, i )
		{
			sscanf(vertices[i].c_str(), "%u,%u", &offset, &count);
			//SystemIO::Print("# Candidate vertices @ %u x %u\n", offset, count);

			// We want max count
			if (count > vertCount && offset < wedgeOffset) 
			{
				vertOffset = offset;
				vertCount = count;
			}
		}

		//SystemIO::Print("# Vertices @ %u x %u\n", vertOffset, vertCount);
	}


	if (faceCount == 0 || faceOffset == 0)
	{
		unsigned long end = r.GetFileSize();
		unsigned int i, offset = 0, count = 0;
		int max;
		faceOffset = faceCount = 0;
		
 		count = (wedgeCount > 0) ? wedgeCount : 1200;
 		count = (vertCount > wedgeCount) ? vertCount : count;

		// Disable 'wedge leading faces', which limits the max wedgeIndex
		// faces will look for...
		//  eg the triangle 0 2 4 can't be found if count = 3
		//count = 0;

		search_for_faces(faces, r, offset, end, count);

		foreach ( faces, i )
		{
			sscanf(faces[i].c_str(), "%u,%u,%i", &offset, &count, &max);
			//SystemIO::Print("# Candidate faces @ %u x %u, maxWedge = %i\n", offset, count, max);

			// We want max count
			if (count > faceCount) 
			{
				faceOffset = offset;
				faceCount = count;
			}
		}

		//SystemIO::Print("# Faces @ %u x %u\n", faceOffset, faceCount);
	}

#if 0
	printf("#####################################################\n");
	printf("# Generated by lineageIIraw2obj\n");
	printf("# by Terry Hendrix <mongooseichiban@gmail.com>\n");
	printf("# Input:\n");
	printf("#  Filename %s\n", argv[1]);
	printf("#  Vertices @ %i x %i\n", vertOffset, vertCount);
	printf("#  Wedges @ %i x %i\n", wedgeOffset, wedgeCount); 
	printf("#  Faces @ %i x %i\n", faceOffset, faceCount);
	printf("#####################################################\n");
#endif


	r.SetOffset(wedgeOffset);
	unsigned int maxVertex = 0;
	unsigned int addtionalWedgesPre = 0;
	unsigned int addtionalWedgesPost = 0;

	// Look for wedges behind and infront of this candidate group
	if (wedgeGuess)
	{
		l2_wedge_t w;
		unsigned long off = wedgeOffset - 10;

		while (test_wedge_offset(r, off, w))
		{
			off -= 10;
			++addtionalWedgesPre;
		}

		off = wedgeOffset + (wedgeCount * 10);

		while (test_wedge_offset(r, off, w))
		{
			off += 10;
			++addtionalWedgesPost;
		}

		wedgeOffset -= (addtionalWedgesPre * 10);
		wedgeCount += addtionalWedgesPre + addtionalWedgesPost;
	}

	// Wedges are actually read out
	r.SetOffset(wedgeOffset);
	for (unsigned int i = 0; i < wedgeCount; ++i)
	{
		l2_wedge_t w;
		unsigned long off = r.GetOffset();

		test_wedge_offset(r, off, w);

		printf("# Wedge[%i] %i %f %f\n", i, w.s, w.u, w.v);

		if ((int)maxVertex < w.s) maxVertex = w.s;

		if (vertCount && w.s < (int)vertCount)
		{
			unsigned long old = off;
			// It's already buffered in memory, so just read it
			r.SetOffset(vertOffset + w.s * 3);

			printf("v %f %f %f\n", 
				   r.ReadFloat32(), r.ReadFloat32(), r.ReadFloat32());

			r.SetOffset(old);
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

	for (unsigned int i = 0; i < faceCount; ++i)
	{
		l2_face_t face;
		unsigned long off = r.GetOffset();
		bool err = false;
		unsigned char mat = 61;

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
			read_index_set(r, set, wedgeOffset, 4);

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


int main(int argc, char *argv[])
{
	mstl::SystemIO::BufferedFileReader r;
	unsigned int i;
	unsigned int wedgeCount, wedgeOffset;
	unsigned int vertCount, vertOffset;
	unsigned int faceCount, faceOffset;

	if (argc < 8 && argc != 2 && argc != 3)
	{
		printf("%s filename vertOffset vertCount wedgeOffset wedgeCount faceOffset faceCount\n\tYou can enter guess mode just for certain objects using ? as input for the counts and offsets.\n\t\n%s filename i  - spits out every possible offset Index = i or pass -1 to list all\n", argv[0], argv[0]);
		return -1;
	}

	if (!r.Open(argv[1]))
	{
		perror(argv[1]);
		return -2;
	}

	if (argc == 3)
	{
		long i, count = r.GetFileSize();
		unsigned int bytes;
		int search = atoi(argv[2]);


		if (search == -311)
		{
			return read_skeletalmesh(r);
		}

		for (i = 0; i < count; ++i)
		{
			r.SetOffset(i);
			int value = read_index(r, bytes);

			if (value >= 1 && search == -1 || search == value)
			{
				printf("@ %li, %i bytes, %i\n", i, bytes, value);	
			}
		}

		r.Close();

		return 0;
	}

#if 0
	// Options parser
	for (i = 2; i < argc; ++i)
	{
		if (argv[i][0] != '-')
			continue;

		mstl::String s = argv[i];

		if (s == "-vcount")
		{
			
		}
	}
#endif


	mstl::Vector<mstl::String> vertices;

	if (argc < 8 || argv[2][0] == '?' || argv[3][0] == '?')
	{
		//fseek(in, 0, SEEK_END);
		unsigned long end = r.GetFileSize();//ftell(in);
		unsigned int offset = 0, count = 0;
		vertOffset = 0;
		vertCount = 0;
		
		search_for_vertices(vertices, r, offset, end);

		foreach ( vertices, i )
		{
			sscanf(vertices[i].c_str(), "%u,%u", &offset, &count);
			mstl::SystemIO::Print("# Candidate vertices @ %u x %u\n", offset, count);

			// We want max count
			if (count > vertCount) 
			{
				vertOffset = offset;
				vertCount = count;
			}
		}

		mstl::SystemIO::Print("# Vertices @ %u x %u\n", vertOffset, vertCount);
	}
	else
	{
		vertOffset = atoi(argv[2]);
		vertCount = atoi(argv[3]);
	}

	printf("#\n");

	if (argc < 8 || argv[4][0] == '?' || argv[5][0] == '?')
	{
		unsigned long end = r.GetFileSize();
		unsigned int offset = 0, count = 0, maxV = 0;
		wedgeOffset = 0;
		wedgeCount = 0;

#if 0
		// Optionally prompt here for offset min / max you want to search
		unsigned int min, max;
		scanf("Enter min : %u", &min);
		wedgeOffset = min;
		end = scanf("Enter max : %u", &max);
		end = (max < end) ? max : end; 
#endif

		mstl::Vector<mstl::String> wedges;
		search_for_wedges(wedges, r, wedgeOffset, end);

		foreach ( wedges, i )
		{
			sscanf(wedges[i].c_str(), "%u,%u,%u", &offset, &count, &maxV);
			mstl::SystemIO::Print("# Candidate wedges @ %u x %u, maxVertexIndex = %u\n", offset, count, maxV);

			// We want max count
			if (count > wedgeCount) 
			{
				wedgeOffset = offset;
				wedgeCount = count;
			}
		}

		SystemIO::Print("# Wedges @ %u x %u\n", wedgeOffset, wedgeCount);
	}
	else
	{
		wedgeOffset = atoi(argv[4]);
		wedgeCount = atoi(argv[5]);
	}

	printf("#\n");

	// We make sure vertices come before wedges, but
	// let people override this behaviour still by passing 
	// vertex offset/count arguments directly
	printf("# Test VertexOffset %u > WedgeOffset %u\n", vertOffset, wedgeOffset);
	if (vertOffset > wedgeOffset &&
		(argc < 8 || (argv[2][0] == '?' || argv[3][0] == '?')))
	{
		unsigned int count, offset;

		SystemIO::Print("#\n");
		SystemIO::Print("# Vertex offset does not agree with Wedge offset\n");
		SystemIO::Print("# Picking a new candidate based on this...\n");

		vertCount = count = 0;
		foreach ( vertices, i )
		{
			sscanf(vertices[i].c_str(), "%u,%u", &offset, &count);
			SystemIO::Print("# Candidate vertices @ %u x %u\n", offset, count);

			// We want max count
			if (count > vertCount && offset < wedgeOffset) 
			{
				vertOffset = offset;
				vertCount = count;
			}
		}

		SystemIO::Print("# Vertices @ %u x %u\n", vertOffset, vertCount);
	}
 
	printf("#\n");

	if (argc < 8 || argv[6][0] == '?' || argv[7][0] == '?')
	{
		mstl::Vector<mstl::String> faces;
		unsigned long end = r.GetFileSize();
		unsigned int offset = 0, count = 0;
		int max;
		faceOffset = 0;
		faceCount = 0;
		
 		count = (wedgeCount > 0) ? wedgeCount : 1200;
 		count = (vertCount > wedgeCount) ? vertCount : count;

		// Disable 'wedge leading faces', which limits the max wedgeIndex
		// faces will look for...
		//  eg the triangle 0 2 4 can't be found if count = 3
		//count = 0;

		search_for_faces(faces, r, offset, end, count);

		foreach ( faces, i )
		{
			sscanf(faces[i].c_str(), "%u,%u,%i", &offset, &count, &max);
			mstl::SystemIO::Print("# Candidate faces @ %u x %u, maxWedge = %i\n", offset, count, max);

			// We want max count
			if (count > faceCount) 
			{
				faceOffset = offset;
				faceCount = count;
			}
		}

		mstl::SystemIO::Print("# Faces @ %u x %u\n", faceOffset, faceCount);
	}
	else
	{
		faceOffset = atoi(argv[6]);
		faceCount = atoi(argv[7]);
	}

	printf("#####################################################\n");
	printf("# Generated by lineageIIraw2obj\n");
	printf("# by Terry Hendrix <mongooseichiban@gmail.com>\n");
	printf("# Input:\n");
	printf("#  Filename %s\n", argv[1]);
	printf("#  Vertices @ %i x %i\n", vertOffset, vertCount);
	printf("#  Wedges @ %i x %i\n", wedgeOffset, wedgeCount); 
	printf("#  Faces @ %i x %i\n", faceOffset, faceCount);
	printf("#####################################################\n");

	float verticesArray[vertCount][3];

	//fseek(in, vertOffset, SEEK_SET);
	r.SetOffset(vertOffset);

	for (i = 0; i < vertCount; ++i)
	{
		verticesArray[i][0] = r.ReadFloat32();//fread(verticesArray[i]+0, 4, 1, in);
		verticesArray[i][1] = r.ReadFloat32();//fread(verticesArray[i]+1, 4, 1, in);
		verticesArray[i][2] = r.ReadFloat32();//fread(verticesArray[i]+2, 4, 1, in);
	}

	//fseek(in, wedgeOffset, SEEK_SET);
	r.SetOffset(wedgeOffset);
	unsigned int maxVertex = 0;
	unsigned int addtionalWedgesPre = 0;
	unsigned int addtionalWedgesPost = 0;

	if (argc < 8 || argv[4][1] == '?' || argv[5][1] == '?')
	{
		l2_wedge_t w;
		unsigned long off = wedgeOffset - 10;

		while (test_wedge_offset(r, off, w))
		{
			off -= 10;
			++addtionalWedgesPre;
		}

		off = wedgeOffset + (wedgeCount * 10);

		while (test_wedge_offset(r, off, w))
		{
			off += 10;
			++addtionalWedgesPost;
		}

		wedgeOffset -= (addtionalWedgesPre * 10);
		wedgeCount += addtionalWedgesPre + addtionalWedgesPost;
	}

	// Wedges may have to be REORDERED here!
	r.SetOffset(wedgeOffset);
	for (i = 0; i < wedgeCount; ++i)
	{
		l2_wedge_t w;
		unsigned long off = r.GetOffset();

		test_wedge_offset(r, off, w);

		printf("# Wedge[%i] %i %f %f\n", i, w.s, w.u, w.v);

		if ((int)maxVertex < w.s) maxVertex = w.s;

		if (vertCount && w.s < (int)vertCount)
			printf("v %f %f %f\n", verticesArray[w.s][0], verticesArray[w.s][2], verticesArray[w.s][1]);
		else
			printf("v 0.0 0.0 0.0\n");
		
		printf("vt %f %f\n", w.u, w.v);
	}


	//fseek(in, faceOffset, SEEK_SET);
	r.SetOffset(faceOffset);
	unsigned int maxWedge = 0, faceMissing = 0;
	int errFace = -1;

	for (i = 0; i < faceCount; ++i)
	{
		l2_face_t face;
		unsigned long off = r.GetOffset();
		bool err = false;
		unsigned char mat = 61;

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
	printf("#  Filename %s\n", argv[1]);
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
		unsigned int offset, bytes;
		if (search_for_index(r, wedgeOffset, 4, wedgeCount, offset, bytes))
		{
			printf("#  Index WedgeCount @ %u, %u bytes; %s\n", 
				   offset, bytes,
				   (offset + bytes == wedgeOffset) ? "*Agrees" : "Disagrees");

			unsigned int vertGuess = wedgeOffset - ((maxVertex+1) * 12) + bytes - 8;

			printf("#  Old method VertexGuess @ %i x %u\n", vertGuess, maxVertex+1);
		}
		else
		{
			// Might want to pull a set here and dump it like the prev version
			printf("#  Index WedgeCount not found precedding wedges\n");

			mstl::Vector<int> set;
			read_index_set(r, set, wedgeOffset, 4);

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

		unsigned int offset, bytes;
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

	r.Close();

	return 0;
}


// Look at the set for the faces to pick a good face count
// You can then compare the face candidate list to utindex output to find faces
// Once you have faces you'll get an 'Old method' output for wedges
// Continue until the vertices are found and match
