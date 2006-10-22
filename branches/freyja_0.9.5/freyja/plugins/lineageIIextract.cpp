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
#include <freyja/FreyjaPluginABI.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_lineageIIextract_init(void (*func)(const char*, void*));
}

void L2ExtractEventsAttach();
void L2ExtractGUIAttach();

char *gL2ExtractFilename = 0x0;
char *gL2ExtractOut = 0x0;
uint32 gL2ExtractOffset = 0;
uint32 gL2ExtractSize = 0;


void freyja_lineageIIextract_init(void (*func)(const char*, void*))
{
	ResourceAppPluginTest *plugin;
	plugin = new ResourceAppPluginTest(L2ExtractEventsAttach,
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

	gL2ExtractFilename = String::Strdup(s);
}


void eL2ExtractOut(char *s)
{
	if (gL2ExtractOut)
		delete [] gL2ExtractOut;

	gL2ExtractOut= String::Strdup(s);
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



//////////////////////////////////////////////////////////////////////////
// Weak 'expert system' pattern matcher
//////////////////////////////////////////////////////////////////////////

#ifdef DIRECT_SKELETALMESH_IMPORT
#include <stdio.h>
#include <stdlib.h>
#ifdef UNIX
#   include <unistd.h>
#endif

#define RANGE_CHECK
#define AVG_SCALE 5 //4
#define MAX(max, s) max = (s > max) ? s : max
#define SKIP if (dump != 1)
#define UV_OUT_OF_RANGE -4
#define XYZ_OUT_OF_RANGE -5
#define FATAL_ERROR -2
#define UNKNOWN 0
#define FOUND 311


// Facet pattern, the 2nd byte is often varied while others very common
// 0x00, 0x09, 0x01, 0x00, 0x00, 0x00

byte gMat2Bytes[] = 
{
	0x09, 0x06, 0x08, 0x14, 0x29, 0x1D
};

unsigned int gGuessFailSafe = 48; //12;
unsigned int gAdpativeMat2 = 0x00; // start guessing new ones ( very slow )
unsigned int gWedgeOffset, gWedgeCount, gFaceOffset, gFaceCount, gVertexOffset, gVertexCount;


// FIXME: Make it adaptive to match mat2 patterns later
char isKnownMat2(unsigned char mat2)//, unsigned int depth)
{
	return (mat2 == 0x9 || mat2 == 0x6 || mat2 == 0x8 ||   //  players
	        mat2 == 0x14 || mat2 == 0x29 || mat2 == 0x1D); //  weapons
}


// Old school UT Index reader, see my UTPackage for a newer algorithm
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


float obj_get_float(ut_buffer_t &obj)
{
	float f = 0.0f;

	if (obj.pos < obj.size)
	{
		memcpy(&f, obj.data, 4);
		obj.pos += 4;
	}

	return f;
}


int load_lineageII_obj(ut_buffer_t &obj,
				       unsigned int vertOffset, unsigned int vertCount,
				       unsigned int wedgeOffset, unsigned int wedgeCount,
				       unsigned int faceOffset, unsigned int faceCount)
{
	unsigned int i, u;
	unsigned short s;
	char c;
	float uv[2];

	if (obj.size == 0)
	{
		return -1;
	}

	float vertices[vertCount][3];

	obj.pos = vertOffset;

	for (i = 0; i < vertCount; ++i)
	{
		vertices[i][0] = obj_get_float(obj);
		vertices[i][1] = obj_get_float(obj);
		vertices[i][2] = obj_get_float(obj);
	}


	obj.pos = wedgeOffset;

	for (i = 0; i < wedgeCount; ++i)
	{
		fread(&s, 2, 1, in);
		fread(uv+0, 4, 1, in);
		fread(uv+1, 4, 1, in);

		mgtk_print("v %f %f %f\n", vertices[s][0],vertices[s][2],vertices[s][1]);
		mgtk_print("vt %f %f\n", uv[0], uv[1]);
	}


	obj.pos = faceOffset;

	for (i = 0; i < faceCount; ++i)
	{
		mgtk_print("f ");
		fread(&s, 2, 1, in);
		mgtk_print("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		mgtk_print("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		mgtk_print("%i/%i ", s+1, s+1);
		fread(&c, 1, 1, in); // mat
		fread(&c, 1, 1, in); // mat2
		fread(&u, 4, 1, in); // smoothing group
		mgtk_print("\n");
	}

	return 0;
}


// FIXME move extractDriver into this file
// extractDriver looks for face offset and passes it's best guess to extractFace
// * Checks range of each type against each other to ensure good guesses
// * Cheats using known common values!  =)

unsigned int guess_face_offset(ut_buffer_t &obj, unsigned long size)
{
	// { s s s 0x00 0x09 0x01 0x00 0x00 0x00 } is the most common face form
	unsigned char b;
	unsigned int offset, state, read, fix = 12;


	offset = ftell(f);

	mgtk_print("# guess_face_offset @ %u / %lu\n", offset, size);

	state = 0;

	while (offset < size && state < 5)
	{
		read = fread(&b, 1, 1, f); 	// Slow, but very turing tape of us
		++offset;

#ifdef UNIX
		if ((offset % 1000) == 0)		// Don't be greedy
			usleep(1);
#endif

		switch (state)
		{
		case 10:
		case 0:
			if (b == 0x00)
			{
				state = 1;
			}
			else
			{
				state = 0;
			}
			break;

		case 11:
		case 1:
			if (isKnownMat2(b))
			{
				++state;
				//mgtk_print("@ %i, state = %i\n", offset, state);
			}
			else if (b == 0x00)
			{
				//mgtk_print("@ %i '0x%x != 0x9' rewind to %i\n", offset, b, offset - state);
				fseek(f, -1, SEEK_CUR);
				--offset;
				state = 0;
			}
			else
			{
				state = 0;
			}
			break;

		case 12:
		case 2:
			if (b == 0x01)
			{
				state = 3;
				//mgtk_print("@ %i, state = %i\n", offset, state);
			}
			else if (b == 0x00)
			{
				fseek(f, -1, SEEK_CUR);
				--offset;
				state = 0;
			}
			else
			{
				state = 0;
			}
			break;

		case 13:
		case 14:
		case 15:
		case 3:
		case 4:
		case 5:
			if (b == 0x00)
			{
				++state;

				if (state == 5)
				{
					fix = 11;
					state = 6; // partial match will be fine today
				}
				else if (state == 15)
				{
					fix = 22;
					state = 16; // partial match will be fine today
				}

				//mgtk_print("@ %i, state = %i\n", offset, state);
			}
			else
			{
				//mgtk_print("@ %i '0x%x != 0x0' rewind to %i\n", offset, b, offset - state);
				fseek(f, -3/*state*/, SEEK_CUR);
				offset -= 3;
				state = 0;
			}
			break;

		case 6:		// Trap state or double check state
			//state = 10;
			
		case 16:
			break;

		default:
			;
		}
	}

	if (state == 6 || state == 16)
		return offset - fix; // account for 's s s' too

	return offset;
}




int find_mesh(ut_buffer_t &obj, unsigned int offset, unsigned int count)
{
	const unsigned int failsafeLimit = 5;
	unsigned int i, u, total = 0, avg;
	unsigned short x, y, z, s, max = 0, oldmax;
	float f;
	char mat, mat2, highmatch = 0;
	unsigned int bytes = 0, guess = 1, failsafe = 0, range = 12;
	int val;
	char dump = 0;


	for (i = 0; i < count; ++i)
	{
		fread(&x, 2, 1, in);
		MAX(max, x);
		total += x;

		fread(&y, 2, 1, in);
		MAX(max, y);
		total += y;

		fread(&z, 2, 1, in);
		MAX(max, z);
		total += z;

		fread(&mat, 1, 1, in);
		fread(&mat2, 1, 1, in);
		fread(&u, 4, 1, in);

		avg = total/((i+1)*3);

		if (x > avg*AVG_SCALE || y > avg*AVG_SCALE || z > avg*AVG_SCALE)
		{
			if (i == 0) i = 1;

			SKIP mgtk_print("# Indices outside safety range of avg\n");
			SKIP mgtk_print("#    (%i, %i, %i) vs %i\n", x, y, z, avg);
			SKIP mgtk_print("#    faceCount %i <= %i\n", count, i - 1);
			SKIP mgtk_print("#    wedgeMax %i <= %i\n", max, oldmax);
			count = i - 1;
			max = oldmax;
		}
		else if (mat != 0x0 || !isKnownMat2(mat2) || u != 1)
		{
			SKIP mgtk_print("# Material ids not expected\n");
			SKIP mgtk_print("#    f%i %i %i %i %i %i %i\n", i, x, y, z, mat, mat2, u);
			SKIP mgtk_print("#    faceCount %i <= %i\n", count, i - 1);
			SKIP mgtk_print("#    wedgeMax %i <= %i\n", max, oldmax);
			count = i - 1;
			max = oldmax;
		}

		oldmax = max;
	}

	wedgeCount = max + 1;
	wedgeOffset = 0;

	while (offset > 6 && ++failsafe < failsafeLimit)
	{
		fseek(in, offset-guess, SEEK_SET);
        val = read_index(in, bytes);

		SKIP mgtk_print("# %i bytes? -> %i\n", guess, val);

		if (val == (int)count)
		{
			SKIP mgtk_print("# *** Face index fit ***\n");
			SKIP mgtk_print("# Faces likely @ %i x %i\n", offset, count);

			// FIXME this is really an error for non interactive mode
			break;
		}

		if (val < 0 || 
		    val < (int)(count - range) ||
		    val > (int)(count + range))
		{
			++guess;
			continue;
		}

		break;
	}

	faceOffset = offset;
	faceCount = count;
	offset = wedgeOffset = offset - ((max+1) * 10) - 4 - bytes;
	count = wedgeCount = max + 1;
	max = 0;
	guess = 1;
	failsafe = 0;

	while (offset > 6 && ++failsafe < failsafeLimit) 
	{
		fseek(in, offset-guess, SEEK_SET);
        val = read_index(in, bytes);

		SKIP mgtk_print("# %i bytes? -> %i\n", guess, val);

		if (val == (int)count)
		{
			mgtk_print("# *** Wedge index fit ***\n");
			mgtk_print("# Wedges @ %i x %i\n", wedgeOffset, wedgeCount);
			mgtk_print("# Faces @ %i x %i\n", faceOffset, faceCount);
			highmatch = 1;
			break;
		}

		if (val < 0 || 
		    val < (int)(count - range) ||
		    val > (int)(count + range))
		{
			++guess;
			continue;
		}

		break;
	}

	if (highmatch)
	{
		max = 0;
		fseek(in, offset, SEEK_SET);

		for (i = 0; i < count; ++i)
		{
			fread(&s, 2, 1, in);
			MAX(max, s);
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1.2f || f < -0.34f)
			{
				mgtk_print("#Range check fail on uv[%i][0] = %f\n", i, f);
				return UV_OUT_OF_RANGE;
			}
#endif

			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1.2f || f < -0.34f)
			{
				mgtk_print("#Range check fail on uv[%i][1] = %f\n", i, f);
				return UV_OUT_OF_RANGE;
			}
#endif
		}

		++max;
		offset -= ((max+1) * 12) + bytes - 8;

#ifdef RANGE_CHECK
		fseek(in, offset, SEEK_SET);
#endif

		for (i = 0; i < max && i < 12; ++i)
		{
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				mgtk_print("#Range check fail on xyz[%i][0] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif
			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				mgtk_print("#Range check fail on xyz[%i][1] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif

			fread(&f, 4, 1, in);
#ifdef RANGE_CHECK
			if (f > 1000000.0f || f < -1000000.0f)
			{
				mgtk_print("#Range check fail on xyz[%i][2] = %f\n", i, f);
				return XYZ_OUT_OF_RANGE;
			}
#endif
		}


		if (offset > 0 && max > 0 && max <= wedgeCount)
		{
			vertexOffset = offset;
			vertexCount = max;
			mgtk_print("# Vertices @ %i x %i\n", offset, max);

			return FOUND;
		}
	}

	mgtk_print("# Wedges @ %i x %i ???\n", wedgeOffset, wedgeCount);
	mgtk_print("# Faces @ %i x %i  ???\n", faceOffset, faceCount);

	return -1;
}


int findmesh_driver(byte *buffer, uint32 size)
{
	uint32 count, offset, bytes, found = !FOUND;
	uint32 guessFailsafe = 0, guessOffset;
	ut_buffer_t obj;


	if (buffer == 0x0)
	{
		mgtk_print("Buffer was empty");
		return FATAL_ERROR;
	}

	obj.data = buffer;
	obj.size = size;
	obj.pos = 0;

	obj.pos = 89;
	count = get_index(obj, bytes);

	if (count == 0)
	{
		// For count to be 0 obj.pos will be 90 after 1 byte read
		count = get_index(obj, bytes);
	}

	mgtk_print("FaceCount is likely to be %i", count);

	guessOffset = obj.pos;

	/* Managed Driver ( Super Top-Level ) seeding mode */
	//if (gCountGuessSeed > 0)
	//	count = gCountGuessSeed;
	//
	//if (gGuessOffset > 0)
	//	guessOffset = gGuessOffset;

	while (found != FOUND && ++guessFailsafe < gGuessFailSafe)
	{
		obj.pos = guessOffset;
		offset = guess_face_offset(obj, size);

		mgtk_print("# Pass %i, @ %i x %i", guessFailsafe, offset, count);

		guessOffset = offset + 12;
		obj.pos = offset;
		found = find_mesh(in, offset, count);

		if (offset >= size)
		{
			mgtk_print("# Reached end of obj @ %i / %i", offset, size);
			break;
		}
	}

	if (found == FOUND)
	{
		mgtk_print("Generated from %s", filename);
		load_lineageII_obj(obj, 
		                   vertexOffset, vertexCount,
		                   wedgeOffset, wedgeCount,
		                   faceOffset, faceCount);
		return FOUND;
	}
	else
	{
		mgtk_print("Failed to extract skeletalmesh");
	}

	return UNKNOWN;
}

#endif


//////////////////////////////////////////////////////////////////////////
// Event and GUI init signals
//////////////////////////////////////////////////////////////////////////

void L2ExtractEventsAttach()
{
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
	char *filename;
	char *basename = "plugins/lineageIIextract.mlisp";
	int id, menuId;
	uint32 e;


	id = Resource::mInstance->getIntByName("eDialogL2Extract");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	mgtk_append_item_to_menu(menuId, "LineageII extractor", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	e = resourceGetEventId1s("eL2ExtractOut");
	mgtk_textentry_value_set(e, "/tmp/dump");
}

