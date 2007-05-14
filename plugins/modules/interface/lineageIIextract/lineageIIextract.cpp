/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : reverseengineer plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <freyja/PluginABI.h>
#include <freyja/FreyjaImage.h>
#include <freyja/freyja.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_lineageIIextract_init(void (*func)(const char*, void*));
}

char *gL2ExtractFilename = 0x0;
char *gL2ExtractOut = 0x0;
uint32 gL2ExtractOffset = 0;
uint32 gL2ExtractSize = 0;
uint32 gWedgeOffset = 0;
uint32 gWedgeCount = 0;
uint32 gFaceOffset = 0;
uint32 gFaceCount = 0;
uint32 gVertexOffset = 0;
uint32 gVertexCount = 0;

void L2ExtractEventsAttach();
void L2ExtractGUIAttach();
int guess_raw(const char *filename);

//////////////////////////////////////////////////////////////////////////
// Event and GUI init signals
//////////////////////////////////////////////////////////////////////////

void freyja_lineageIIextract_init(void (*func)(const char*, void*))
{
	mgtk::ResourcePlugin *plugin;
	plugin = new mgtk::ResourcePlugin(L2ExtractEventsAttach,
									  L2ExtractGUIAttach);	
}


// From my pak vfs plugin for lineageII, which is too buggy to put in svn
byte *extract_from_lineageII(const char *filename, uint32 offset, uint32 size)
{
	FILE *f;
	byte *buffer = 0x0;
	uint32 signature, i;
	byte key;
	char stringHeader[16];
	char stringVersion[16];


	f = fopen(filename, "rb");

	if (!f)
	{
		perror(filename);
		return 0x0;
	}

	fread(&signature, 4, 1, f);

	/* UT package signature 0x9E2a83c1 */
	switch (signature)
	{
	case 0x9E2a83c1:
		mgtk_print("Standard Unreal package format?\n");
		break;


	case 0x0069004c:
		/*  16 bytes Header   ( 16bit char string )
		 *  12 bytes Version  ( 16bit char string )
		 * [ 2 bytes Minor? ] ( Always Ver111 0x53 0x52  "SR"? )
		 * [ 2 bytes longer?] ( Always Ver121 0x50E44D0F ".M.P"? )
		 *
		 *  The rest seems to be XOR encrypted data by 0xAC in Ver111	
		 */

		fread(stringHeader, 12, 1, f);
		fread(stringVersion, 12, 1, f);
	
		/* Mongoose: 2 Byte wide chars used in string header, 
			assuming english, so strip every other byte */
		stringHeader[5] = stringHeader[6];
		stringHeader[4] = stringHeader[4];
		stringHeader[3] = stringHeader[2];
		stringHeader[2] = stringHeader[0];
		stringHeader[6] = stringHeader[8];
		stringHeader[7] = stringHeader[10];
		stringHeader[1] = ((char *)(&signature))[2];
		stringHeader[0] = ((char *)(&signature))[0];
		stringHeader[8] = 0;

		for (i = 1; i < 6; ++i)
		{
			stringVersion[i] = stringVersion[i*2];
		}

		stringVersion[6] = 0;

		mgtk_print("Lineage II encrypted package '%s' '%s'\n", 
				stringHeader, stringVersion);

		if (strcmp(stringVersion, "Ver121") == 0)
		{
			/* Multiple valid keys, so let's look for common 0 byte */
			fseek(f, 0x23, SEEK_SET);
			fread(&key, 1, 1, f);
		}
		else
		{
			key = 0xAC;  /* Ver111 key is a constant */
		}

		fseek(f, offset, SEEK_SET);
		buffer = new byte[size];
		fread(buffer, 1, size, f);

		for (i = 0; i < size; ++i)
		{
			buffer[i] ^= key;
		}

		fclose(f);
		break;


	default:
		mgtk_print("Not a known UT package 0x%x\n", signature);
	}


	return buffer;
}


void eL2Extract()
{
	FILE *f;
	byte *buffer = 0x0;

	buffer = extract_from_lineageII(gL2ExtractFilename,
                                    gL2ExtractOffset,
                                    gL2ExtractSize);

	if (buffer)
	{
		f = fopen(gL2ExtractOut, "wb");

		if (f)
		{
			fwrite(buffer, 1, gL2ExtractSize, f);
			fclose(f);
			mgtk_print("! Wrote file '%s'", gL2ExtractOut);
		}

		delete [] buffer;
	}
}


void eL2ExtractFilename(char *s)
{
	if (gL2ExtractFilename)
		delete [] gL2ExtractFilename;

	gL2ExtractFilename = mstl::String::Strdup(s);
}


void eL2ExtractOut(char *s)
{
	if (gL2ExtractOut)
		delete [] gL2ExtractOut;

	gL2ExtractOut = mstl::String::Strdup(s);
}


void eL2ExtractOffset(unsigned int value)
{
	gL2ExtractOffset = value;
}


void eL2ExtractSize(unsigned int value)
{
	gL2ExtractSize = value;
}


void eDialogL2Extract()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogL2Extract"), 1);
} 


void eDialogL2ExtractClose()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogL2Extract"), 0);
} 


void eL2RawLoad(char *filename)
{
	gL2ExtractOffset = 0;
	gL2ExtractSize = 0;
	gWedgeOffset = 0;
	gWedgeCount = 0;
	gFaceOffset = 0;
	gFaceCount = 0;
	gVertexOffset = 0;
	gVertexCount = 0;

	uint32 e = resourceGetEventId1s("eL2ExtractOut");
	mgtk_textentry_value_set(e, filename);

	if (!guess_raw(filename))
	{
		mgtk_print("! Plugin attempted to extract '%s'", filename);
	}
	else
	{
		mgtk_print("! Plugin could not extract '%s'", filename);
	}
}


void eL2ExtractMenu()
{
}


void eL2ExtractOpenRaw()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Raw...", path, 0,
							  "UE2 SkeletalMesh (*.raw)", "*.raw");

	if (path) 
	{
		delete [] path;
	}

	mgtk_print("! Importing: '%s'\n", filename);
	eL2RawLoad(filename);
	mgtk_filechooser_blocking_free(filename);
}


void eL2ExtractOpenUT()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open UT2 Pak...", path, 0,
							  "UE2 Pak File (*.utx)", "*.utx");

	if (path) 
	{
		delete [] path;
	}

	// FIXME

	mgtk_filechooser_blocking_free(filename);
}


void L2ExtractEventsAttach()
{
	ResourceEventCallback::add("eL2ExtractMenu", &eL2ExtractMenu);
	ResourceEventCallback::add("eL2ExtractOpenRaw", &eL2ExtractOpenRaw);
	//ResourceEventCallbackString::add("eL2RawLoad", &eL2RawLoad);
	ResourceEventCallback::add("eL2ExtractOpenUT", &eL2ExtractOpenUT);

	ResourceEventCallback::add("eL2Extract", &eL2Extract);
	ResourceEventCallbackString::add("eL2ExtractFilename", &eL2ExtractFilename);
	ResourceEventCallbackString::add("eL2ExtractOut", &eL2ExtractOut);
	ResourceEventCallbackUInt::add("eL2ExtractOffset", &eL2ExtractOffset);
	ResourceEventCallbackUInt::add("eL2ExtractSize", &eL2ExtractSize);
	ResourceEventCallback::add("eDialogL2Extract", &eDialogL2Extract);
	ResourceEventCallback::add("eDialogL2ExtractClose", &eDialogL2ExtractClose);
}


void L2ExtractGUIAttach()
{
	char *basename = "plugins/lineageIIextract.mlisp";
	char *filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	int menu = Resource::mInstance->getIntByName("ePluginMenu");
	int submenu = Resource::mInstance->getIntByName("eL2ExtractMenu");
	mgtk_append_menu_to_menu(menu, "Lineage II Extract", submenu);

	int item = Resource::mInstance->getIntByName("eL2ExtractOpenRaw");
	mgtk_append_item_to_menu(submenu, "Open Raw SkeletalMesh...", item);

	item = Resource::mInstance->getIntByName("eL2ExtractOpenUT");
	mgtk_append_item_to_menu(submenu, "Open UTPak...", item);

	uint32 e = resourceGetEventId1s("eL2ExtractOut");
	mgtk_textentry_value_set(e, " ");
}


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

#define CONSOLE_APPLICATION 0
#define printf freyjaPrintMessage

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

bool generic_mesh_search(SystemIO::BufferedFileReader &r,
						 unsigned int &vertCount, unsigned int &vertOffset,
						 unsigned int &wedgeCount, unsigned int &wedgeOffset,
						 unsigned int &faceCount, unsigned int &faceOffset);

bool read_skeletalmesh(SystemIO::BufferedFileReader &r)
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


				gVertexOffset = vertGuess;
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


			gWedgeOffset = wedgeOffset;
			gWedgeCount = wedgeCount;
			gFaceOffset = faceOffset;
			gFaceCount = faceCount;
			gVertexCount = vertCount;
		}
	}

	FREYJA_INFOMSG(dumped, "Failed to import Lineage II RAW.\n%u %u %u %u %u %u\n", gWedgeOffset, gWedgeCount, gFaceOffset, gFaceCount, gVertexOffset, gVertexCount);

#if 0
\t%s%u %u %u %u %u %u\n",
					   (vertCount == maxVertex+1) ? "*Agrees " : "",
					   vertGuess, maxVertex+1,
					   wedgeOffset, wedgeCount, 
					   faceOffset, faceCount);
	//printf("%i\n", 1<<20);
#endif

	return dumped;
}


#include <freyja/MeshABI.h>
#include <freyja/ModelABI.h>
#include <freyja/MaterialABI.h>

bool generic_mesh_search(SystemIO::BufferedFileReader &r,
						 unsigned int &vertCount, unsigned int &vertOffset,
						 unsigned int &wedgeCount, unsigned int &wedgeOffset,
						 unsigned int &faceCount, unsigned int &faceOffset)
{
	mstl::Vector<mstl::String> vertices;
	mstl::Vector<mstl::String> wedges;
	mstl::Vector<mstl::String> faces;
	unsigned long end = r.GetFileSize();

	r.SetOffset(0);

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
	if (vertsGuess && vertOffset > wedgeOffset)
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


	index_t model = 0;// append freyjaModelCreate();
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	// Wedges are actually read out
	r.SetOffset(wedgeOffset);
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

			vec3_t v = { x, z, y };
			vec3_t t = { w.u, w.v, 0.0f };
			index_t vertex = freyjaMeshVertexCreate3fv(mesh, v);
			freyjaMeshVertexTexCoord3fv(mesh, vertex, t);
		}
		else
		{
			printf("v 0.0 0.0 0.0\n");
		}

		printf("vt %f %f\n", w.u, w.v);
		//freyjaMeshTexCoordCreate2f(mesh, w.u, w.v);
	}


	// Faces
	r.SetOffset(faceOffset);
	unsigned int maxWedge = 0, faceMissing = 0;
	int errFace = -1;
	unsigned char mat = 61;
	index_t material = 0;

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
						material = freyjaMaterialCreate();
					}

					printf("f %i/%i %i/%i %i/%i\n", 
						   face.a+1, face.a+1, 
						   face.b+1, face.b+1, 
						   face.c+1, face.c+1);

					
					index_t facei = freyjaMeshPolygonCreate(mesh);
					freyjaMeshPolygonMaterial(mesh, facei, material);
					freyjaMeshPolygonGroup1u(mesh, facei, material);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.a);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.b);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.c);
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

					index_t facei = freyjaMeshPolygonCreate(mesh);
					freyjaMeshPolygonMaterial(mesh, facei, material);
					freyjaMeshPolygonGroup1u(mesh, facei, material);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.a);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.b);
					freyjaMeshPolygonAddVertex1i(mesh, facei, face.c);
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


	/* Generate normals to have lighting. */
	freyjaMeshGenerateVertexNormals(mesh);
	freyjaMeshNormalFlip(mesh);

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


int guess_raw(const char *filename)
{
	mstl::SystemIO::BufferedFileReader r;

	if (r.Open(filename))
	{
		read_skeletalmesh(r);
		r.Close();
	}
	else
	{
		perror(filename);
		return -2;
	}

	return 0;
}

