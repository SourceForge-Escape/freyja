/*
 This is a pretty hacky lineageII raw SkeletalMesh to OBJ test app.

 It should be used to design a better application or even finish reverse
 engineering the various formats.  This app is a compromise of guessing 
 patterns to avoid supporting versions I've never seen or can test myself.

 Terry Hendrix II <mongooseichiban@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>

typedef struct {
	short x;
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


int read_index(FILE *f, unsigned int &bytes)
{
	int val;
	char b0, b1, b2, b3, b4;

	bytes = 0;                                                         
	val = 0;
   
	fread(&b0, 1, 1, f);
	++bytes;
   
	if (b0 & 0x40)
	{
		fread(&b1, 1, 1, f);
 		++bytes;

		if (b1 & 0x80)
		{
			fread(&b2, 1, 1, f);
	 		++bytes;
 
			if (b2 & 0x80)
			{
				fread(&b3, 1, 1, f);
		 		++bytes;
 
				if (b3 & 0x80)
				{
					fread(&b4, 1, 1, f);
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


bool test_vertex_offset(FILE *f, unsigned long offset, l2_vert_t &v)
{
	fseek(f, offset, SEEK_SET);
	fread(&v.x, 4, 1, f);
	fread(&v.y, 4, 1, f);
	fread(&v.z, 4, 1, f);
	
	return (v.x < 100000.0f && v.x > -100000.0f && 
	        v.y < 100000.0f && v.y > -100000.0f &&
	        v.z < 100000.0f && v.z > -100000.0f &&
	        v.x != FP_NAN && v.y != FP_NAN && v.z != FP_NAN);
}


bool test_wedge_offset(FILE *f, unsigned long offset, l2_wedge_t &w)
{
	fseek(f, offset, SEEK_SET);
	fread(&w.s, 2, 1, f);
	fread(&w.u, 4, 1, f);
	fread(&w.v, 4, 1, f);
	
	return (w.s >= 0 && w.u >= 0.0f && w.u <= 1.0f && w.v >= 0.0f && w.v <= 1.0f);
}


bool test_face_offset(FILE *f, unsigned long offset, l2_face_t &face)
{
	fseek(f, offset, SEEK_SET);
	fread(&face.a, 2, 1, f);
	fread(&face.b, 2, 1, f);
	fread(&face.c, 2, 1, f);
	fread(&face.mat, 1, 1, f);
	fread(&face.aux, 1, 1, f);
	fread(&face.group, 4, 1, f);
	
	return (face.a >= 0 && face.b >= 0 && face.c >= 0 &&
	        face.a != face.b && face.a != face.c && face.b != face.c);
}


void search_for_vertices(mstl::Vector<mstl::String> &vertices, FILE *f,
                       unsigned long offset, unsigned long end)
{
	l2_vert_t v;
	mstl::String str;
	unsigned long old = offset, pick = 0, lastOffset = 0;
	unsigned int last = 0, best = 0;

	for (offset = 0; offset < end; )
	{
		old = offset;

		if (test_vertex_offset(f, offset, v))
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


// This can only find wedges in vertexIndex order, so we need a separate
// function for searching by clustered indices in case some model wedges 
// are serialized out of order.
void search_for_wedges(mstl::Vector<mstl::String> &wedges, FILE *f,
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

		if (test_wedge_offset(f, offset, w))
		{
			switch (state)
			{	
			case 0: // Find the first wedge ( Need at least 3 in order )
				if (test_wedge_offset(f, offset+10, w2) && count == 0 
#ifdef ONLY_LOOK_FOR_ORDERED_WEDGES 
					&&
					w.s == 0 && w2.s == 1 && 
					test_wedge_offset(f, offset+20, w2) &&
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
				if (w.s == oldS + 1 && count)
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


// Should look for clusters of same mat or aux
// Should look for overlapping contigous faces 
// Should check for all the indices in each face being in a valid range, and
// avoid gaps
void search_for_faces(mstl::Vector<mstl::String> &faces, FILE *f,
                      unsigned long offset, unsigned long end,
                      unsigned int maxIndex)
{
	l2_face_t face;
	mstl::String str;
	unsigned long old = offset, pick = 0, lastOffset = 0;
	unsigned int last = 0, best = 0;

	// Face search starting at offset
	for (offset = 0; offset < end; )
	{
		old = offset;

		if (test_face_offset(f, offset, face) && maxIndex == 0 ||
			face.a < (int)maxIndex && face.b < (int)maxIndex && face.c < (int)maxIndex)
		{
			//printf("# %sFace @ %lu, { %i, %i, %i,\t %u, %u, %u }\n", last ? "*" : "", offset, face.a, face.b, face.c, face.mat, face.aux, face.group);
			if (last == 0)
				lastOffset = old;

			offset = old + 12;
			++last;
		}
		else
		{
			if (last > best)
			{
				best = last;
				pick = lastOffset;
				//printf("# Possible face array @ %lu x %u\n", pick, best);
				str.Set("%lu,%u", pick, best);
				faces.push_back(str);
			}

			last = 0;
			++offset;
			//offset = lastOffset + 1; // Brute force overlapping check
		}
	}	
}


int main(int argc, char *argv[])
{
	FILE *in;
	unsigned int i;
	unsigned int wedgeCount, wedgeOffset;
	unsigned int vertCount, vertOffset;
	unsigned int faceCount, faceOffset;

	if (argc < 8 && argc != 2)
	{
		printf("%s filename vertOffset vertCount wedgeOffset wedgeCount faceOffset faceCount\n\tYou can enter guessing mode using ? and ?! as input.\n", argv[0]);
		return -1;
	}

	in = fopen(argv[1], "rb");

	if (!in)
	{
		perror(argv[1]);
		return -2;
	}

	mstl::Vector<mstl::String> vertices;

	if (argc < 8 || argv[2][0] == '?' || argv[3][0] == '?')
	{
		fseek(in, 0, SEEK_END);
		unsigned long end = ftell(in);
		unsigned int offset = 0, count = 0;
		vertOffset = 0;
		vertCount = 0;
		
		search_for_vertices(vertices, in, offset, end);

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


	if (argc < 8 || argv[4][0] == '?' || argv[5][0] == '?')
	{
		fseek(in, 0, SEEK_END);
		unsigned long end = ftell(in);
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
		search_for_wedges(wedges, in, wedgeOffset, end);

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

		// We make sure vertices come before wedges
		if (vertOffset > wedgeOffset)
		{
			vertCount = count = 0;
			foreach ( vertices, i )
			{
				sscanf(vertices[i].c_str(), "%u,%u", &offset, &count);
				mstl::SystemIO::Print("# Candidate vertices @ %u x %u\n", offset, count);

				// We want max count
				if (count > vertCount && offset < wedgeOffset) 
				{
					vertOffset = offset;
					vertCount = count;
				}
			}

			mstl::SystemIO::Print("# Vertices @ %u x %u\n", vertOffset, vertCount);
		}

		mstl::SystemIO::Print("# Wedges @ %u x %u\n", wedgeOffset, wedgeCount);
	}
	else
	{
		wedgeOffset = atoi(argv[4]);
		wedgeCount = atoi(argv[5]);
	}

	if (argc < 8 || argv[6][0] == '?' || argv[7][0] == '?')
	{
		mstl::Vector<mstl::String> faces;
		fseek(in, 0, SEEK_END);
		unsigned long end = ftell(in);
		unsigned int offset = 0, count = 0, bytes;
		faceOffset = 0;
		faceCount = 0;

		fseek(in, 89, SEEK_SET);
		count = read_index(in, bytes);
		mstl::SystemIO::Print("# Face count guess 89th byte = %i\n", count);
		
 		count = (wedgeCount > 0) ? wedgeCount : 1200;
 		count = (vertCount > wedgeCount) ? vertCount : count;

		// Disable 'wedge leading faces', which limits the max wedgeIndex
		// faces will look for...
		//  eg the triangle 0 2 4 can't be found if count = 3
		//count = 0;

		search_for_faces(faces, in, offset, end, count);

		foreach ( faces, i )
		{
			sscanf(faces[i].c_str(), "%u,%u", &offset, &count);
			mstl::SystemIO::Print("# Candidate faces @ %u x %u\n", offset, count);

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

	fseek(in, vertOffset, SEEK_SET);

	for (i = 0; i < vertCount; ++i)
	{
		fread(verticesArray[i]+0, 4, 1, in);
		fread(verticesArray[i]+1, 4, 1, in);
		fread(verticesArray[i]+2, 4, 1, in);
	}

	fseek(in, wedgeOffset, SEEK_SET);
	unsigned int maxVertex = 0;

	for (i = 0; i < wedgeCount; ++i)
	{
		l2_wedge_t w;
		unsigned long off = ftell(in);
		test_wedge_offset(in, off, w);

		printf("# Wedge[%i] %i %f %f\n", i, w.s, w.u, w.v);

		if ((int)maxVertex < w.s) maxVertex = w.s;

		if (vertCount && w.s < (int)vertCount)
			printf("v %f %f %f\n", verticesArray[w.s][0], verticesArray[w.s][2], verticesArray[w.s][1]);
		else
			printf("v 0.0 0.0 0.0\n");
		
		printf("vt %f %f\n", w.u, w.v);
	}

	unsigned int addtionalWedges = 0;

	if (argc < 8 || argv[4][1] == '?' || argv[5][1] == '?')
	{
		l2_wedge_t w;
		unsigned long off = ftell(in);
		unsigned int count = 0;		

		while (test_wedge_offset(in, off, w))
		{
			printf("# Possible wedge %i %f %f @ %lu??\n", w.s, w.u, w.v, off);
			off += 10;
			++count;

			if ((int)maxVertex < w.s) maxVertex = w.s;

			if (vertCount && w.s < (int)vertCount)
				printf("v %f %f %f\n", verticesArray[w.s][0],verticesArray[w.s][2],verticesArray[w.s][1]);
			printf("vt %f %f\n", w.u, w.v);
		}

		addtionalWedges = count;
	}

	fseek(in, faceOffset, SEEK_SET);
	unsigned int maxWedge = 0, maxFace = 0;

	for (i = 0; i < faceCount; ++i)
	{
		l2_face_t face;
		unsigned long off = ftell(in);
		bool err = false;

		if (test_face_offset(in, off, face))
		{
			printf("# Face[%i] %i %i %i %u %u %u\n", i, face.a, face.b, face.c, face.mat, face.aux, face.group);
			if (!err)
			{
				printf("f %i/%i %i/%i %i/%i\n", face.a+1, face.a+1, face.b+1, face.b+1, face.c+1, face.c+1);
				if ((int)maxWedge < face.a) maxWedge = face.a;
				if ((int)maxWedge < face.b) maxWedge = face.b;
				if ((int)maxWedge < face.c) maxWedge = face.c;
			}
			else
			{
				printf("#f %i/%i %i/%i %i/%i\n", face.a+1, face.a+1, face.b+1, face.b+1, face.c+1, face.c+1);
			}

			off += 12;
		}
		else
		{
			// By cutting off at first Error you can find max wedge index
			// for comparing with expected max wedge index
			if (!err)
				maxFace = i;

			err = true;

			printf("#Error @ %lu face[%i] %i %i %i %u %u %u\n", off, i, face.a, face.b, face.c, face.mat, face.aux, face.group);
			off += 12; // Just to keep moving the tape
		}
	}

	fclose(in);

	printf("#####################################################\n");
	printf("# Generated by lineageIIraw2obj\n");
	printf("# by Terry Hendrix <mongooseichiban@gmail.com>\n");
	printf("#\n");
	printf("#  Filename %s\n", argv[1]);
	printf("#  Vertices @ %i x %i\n", vertOffset, vertCount);
	printf("#  Wedges @ %i x %i\n", wedgeOffset, wedgeCount); 
	printf("#  Faces @ %i x %i\n", faceOffset, faceCount);
	printf("#\n");
	printf("# Possible count corrections:\n");
	if (maxVertex) printf("#  maxVertexIndex = %u\n", maxVertex);
	if (maxWedge) printf("#  maxWedgeIndex = %u\n", maxWedge);
	if (addtionalWedges) printf("# Possible wedges total count: %u\n#   found %u trailing\n", addtionalWedges + wedgeCount, addtionalWedges);

	if (maxFace) printf("#  maxFaceCount = %u\n", maxFace);
	printf("#####################################################\n");

	return 0;
}
