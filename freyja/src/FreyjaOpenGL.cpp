/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator scripopt.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Centralized various graphics util functions here
 *            Normalized the API to be more consistant
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <freyja/freyja.h>
#include <freyja/MaterialABI.h>

#ifdef HAVE_FREYJA_IMAGE
#   include <freyja/FreyjaImage.h>
#endif

#include <hel/math.h>
#include <hel/Vec3.h>
#include <mstl/SystemIO.h>
#include "Cursor.h"
#include "Texture.h"
#include "FreyjaOpenGL.h"

using namespace hel;


#ifdef USING_OPENGL_EXT
PFNGLMULTITEXCOORD1FARBPROC h_glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC h_glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD3FARBPROC h_glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC h_glMultiTexCoord4fARB = NULL;
PFNGLACTIVETEXTUREARBPROC h_glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC h_glClientActiveTextureARB = NULL;

PFNGLGENPROGRAMSARBPROC h_glGenProgramsARB = NULL;
PFNGLBINDPROGRAMARBPROC h_glBindProgramARB = NULL;
PFNGLPROGRAMSTRINGARBPROC h_glProgramStringARB = NULL;
PFNGLGETPROGRAMIVARBPROC h_glGetProgramivARB = NULL;

PFNGLCREATESHADEROBJECTARBPROC h_glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC h_glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC h_glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC h_glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC h_glAttachObjectARB = NULL;	
PFNGLLINKPROGRAMARBPROC h_glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC h_glUseProgramObjectARB = NULL;
PFNGLGETINFOLOGARBPROC h_glGetInfoLogARB = NULL;
PFNGLDELETEOBJECTARBPROC h_glDeleteObjectARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC h_glGetObjectParameterivARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC h_glGetUniformLocationARB = NULL;
#else
void *h_glMultiTexCoord1fARB = NULL;
void *h_glMultiTexCoord2fARB = NULL;
void *h_glMultiTexCoord3fARB = NULL;
void *h_glMultiTexCoord4fARB = NULL;
void *h_glActiveTextureARB = NULL;
void *h_glClientActiveTextureARB = NULL;

void *h_glGenProgramsARB = NULL;
void *h_glBindProgramARB = NULL;
void *h_glProgramStringARB = NULL;
void *h_glGetProgramivARB = NULL;

void *h_glCreateShaderObjectARB = NULL;
void *h_glShaderSourceARB = NULL;
void *h_glCompileShaderARB = NULL;
void *h_glCreateProgramObjectARB = NULL;
void *h_glAttachObjectARB = NULL;	
void *h_glLinkProgramARB = NULL;
void *h_glUseProgramObjectARB = NULL;
void *h_glGetInfoLogARB = NULL;
void *h_glDeleteObjectARB = NULL;
void *h_glGetObjectParameterivARB = NULL;
void *h_glGetUniformLocationARB = NULL;
#endif

using namespace freyja3d;
using namespace hel;

bool OpenGL::arb_multitexture = false;
bool OpenGL::arb_texture_env_combine = false;
bool OpenGL::arb_vertex_shader = false;
bool OpenGL::arb_fragment_shader = false;
bool OpenGL::arb_shader_objects = false;
bool OpenGL::arb_shadow = false;
bool OpenGL::arb_depth_texture = false;
bool OpenGL::arb_shading_language_100 = false;
bool OpenGL::arb_vertex_buffer_object = false;
bool OpenGL::ext_cg_shader = false;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGL *OpenGL::mSingleton = NULL;
uint32 OpenGL::mObjects = 0;


OpenGL *OpenGL::Instance()
{
	return mSingleton ? mSingleton : new OpenGL();
}


OpenGL::OpenGL() :
	mTextureUnitCount(2),
	mMaxLightsCount(2),
	mFlags(fNone),
	mTextureIds(NULL),
	mTextureCount(2), // FIXME: Use new OpenGL ext wrapper class
	mTextureLimit(64),
	mTextureId(1),
	mTextureId2(-1)
{
	/* Log OpenGL driver support information */
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	FREYJA_ASSERTMSG(glGetString(GL_RENDERER) != NULL, "OpenGL Context creation failed");

	// Get hardware info
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &mTextureUnitCount);
	freyja_print("\tGL_MAX_TEXTURE_UNITS_ARB \t\t[%i]", mTextureUnitCount);

	glGetIntegerv(GL_MAX_LIGHTS, &mMaxLightsCount);
	freyja_print("\tGL_MAX_LIGHTS            \t\t[%i]", mMaxLightsCount);

	arb_multitexture = mglHardwareExtTest("GL_ARB_multitexture");
	arb_texture_env_combine = mglHardwareExtTest("GL_ARB_texture_env_combine");

	arb_shadow = mglHardwareExtTest("GL_ARB_shadow");
	arb_depth_texture = mglHardwareExtTest("GL_ARB_depth_texture");

	arb_vertex_buffer_object = mglHardwareExtTest("GL_ARB_vertex_buffer_object");
	arb_vertex_shader = mglHardwareExtTest("GL_ARB_vertex_shader");
	arb_fragment_shader = mglHardwareExtTest("GL_ARB_fragment_shader");
	arb_shader_objects = mglHardwareExtTest("GL_ARB_shader_objects");
	arb_shading_language_100 = mglHardwareExtTest("GL_ARB_shading_language_100");
	ext_cg_shader = mglHardwareExtTest("GL_EXT_Cg_shader");

	// Depends on class Texture replacement
	//SetMaxTextureCount(mTextureLimit);

	int stencil;
	glGetIntegerv(GL_STENCIL_BITS, &stencil);
	freyja_print("\tGL_STENCIL_BITS            \t\t[%i]", stencil);

	// Hook up functions
#ifdef USING_OPENGL_EXT
	h_glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC)mglGetProcAddress("glMultiTexCoord1fARB");
	h_glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)mglGetProcAddress("glMultiTexCoord2fARB");
	h_glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC)mglGetProcAddress("glMultiTexCoord3fARB");
	h_glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC)mglGetProcAddress("glMultiTexCoord4fARB");
	h_glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)mglGetProcAddress("glActiveTextureARB");
	h_glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)mglGetProcAddress("glClientActiveTextureARB");

	h_glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)mglGetProcAddress("glGenProgramsARB");
	h_glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)mglGetProcAddress("glBindProgramARB");
	h_glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)mglGetProcAddress("glProgramStringARB");
	h_glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)mglGetProcAddress("glGetProgramivARB");

	h_glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)mglGetProcAddress("glCreateShaderObjectARB");
	h_glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)mglGetProcAddress("glShaderSourceARB");
	h_glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)mglGetProcAddress("glCompileShaderARB");
	h_glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)mglGetProcAddress("glCreateProgramObjectARB");
	h_glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)mglGetProcAddress("glAttachObjectARB");	
	h_glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)mglGetProcAddress("glLinkProgramARB");
	h_glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)mglGetProcAddress("glUseProgramObjectARB");
	h_glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)mglGetProcAddress("glGetInfoLogARB");
	h_glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)mglGetProcAddress("glDeleteObjectARB");
	h_glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)mglGetProcAddress("glGetObjectParameterivARB");
	h_glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)mglGetProcAddress("glGetUniformLocationARB");
#endif

	mSingleton = this;
}


OpenGL::~OpenGL()
{
	if (mTextureIds) 
		delete [] mTextureIds;

	
	for (uint32 i = 0; i < mObjects; --i)
		DeleteFragmentGLSL(i);
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int32 OpenGL::BlendStringToInt(const char *str)
{
	mstl::String s(str);

	return ((s == "GL_ONE") ? GL_ONE :
			(s == "GL_ZERO") ? GL_ZERO :
			(s == "GL_SRC_COLOR") ? GL_SRC_COLOR :
			(s == "GL_DST_COLOR") ? GL_DST_COLOR :
			(s == "GL_SRC_ALPHA") ? GL_SRC_ALPHA :
			(s == "GL_DST_ALPHA") ? GL_DST_ALPHA :
			(s == "GL_CONSTANT_COLOR") ? GL_CONSTANT_COLOR :
			(s == "GL_CONSTANT_ALPHA") ? GL_CONSTANT_ALPHA :
			(s == "GL_SRC_ALPHA_SATURATE") ? GL_SRC_ALPHA_SATURATE :
			(s == "GL_ONE_MINUS_DST_ALPHA") ? GL_ONE_MINUS_DST_ALPHA :
			(s == "GL_ONE_MINUS_SRC_ALPHA") ? GL_ONE_MINUS_SRC_ALPHA :
			(s == "GL_ONE_MINUS_DST_COLOR") ? GL_ONE_MINUS_DST_COLOR :
			(s == "GL_ONE_MINUS_SRC_COLOR") ? GL_ONE_MINUS_SRC_COLOR :
			(s == "GL_ONE_MINUS_CONSTANT_COLOR") ? GL_ONE_MINUS_CONSTANT_COLOR :
			(s == "GL_ONE_MINUS_CONSTANT_ALPHA") ? GL_ONE_MINUS_CONSTANT_ALPHA :
			GL_ZERO);
}


const char *OpenGL::BlendIntToString(int32 i)
{
	return ((i == GL_ONE) ? "GL_ONE" :
			(i == GL_ZERO) ? "GL_ZERO" :
			(i == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			(i == GL_DST_COLOR) ? "GL_DST_COLOR" :
			(i == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			(i == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			(i == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			(i == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : 
			(i == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			(i == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			(i == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			(i == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			(i == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			(i == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			(i == GL_ONE_MINUS_CONSTANT_ALPHA) ? "GL_ONE_MINUS_CONSTANT_ALPHA" :
			"GL_ZERO");
}


// Very, very old TGA screenshot code - slightly updated
void OpenGL::TakeScreenShot(const char *base, uint32 width, uint32 height)
{
	if (!width || !height)
	{
		freyja_print("OpenGL::TakeScreenShot() ERROR: Invalid image size!\n");
		return;
	}

	// Don't overwrite old screenshots...
	static int count = 0;
	char filename[1024];
	bool done = false;

	FILE *f = fopen(filename, "wb");

	while (!done)
	{
		snprintf(filename, 1024, "%s-%04i.tga", base, count++);
		f = fopen(filename, "rb");
		(f) ? fclose(f) : done = true;
	}

	if (!f)
	{
		freyja_print("OpenGL::TakeScreenShot() ERROR: Couldn't write screenshot.\n");
		perror("OpenGL::TakeScreenShot() ERROR: ");
		return;
	}

	int sz = width * height;
	byte *image = new byte[sz*3];

	// Capture frame buffer
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

	byte *swap_row = new byte[width*3];

	// Flip vertical
	for (uint32 i = 0, j = height-1; i < height/2; ++i, --j)
	{
		memcpy(swap_row, &image[i*width*3], width*3);
		memcpy(&image[i*width*3], &image[j*width*3], width*3);
		memcpy(&image[j*width*3], swap_row, width*3);
	}

	delete [] swap_row;

	// Build a TGA header
	char comment[32] = "Mongoose TGA 0.0.1\0";
	byte comment_lenght = strlen(comment);
	byte colormap_type = 0;
	byte image_type = 2;
	unsigned short colormap_index = 0;
	unsigned short colormap_lenght = 0;
	byte colormap_bbp = 0;
	unsigned short origin_x = 0, origin_y = 0;
	unsigned short swidth = width;
	unsigned short sheight = height;
	byte bpp = 24;
	byte desc_flags = 32;

	// Write TGA header
	fwrite(&comment_lenght, 1, 1, f);
	fwrite(&colormap_type, 1, 1, f); 
	fwrite(&image_type, 1, 1, f);
	fwrite(&colormap_index, 2, 1, f);
	fwrite(&colormap_lenght, 2, 1, f);
	fwrite(&colormap_bbp, 1, 1, f);
	fwrite(&origin_x, 2, 1, f);
	fwrite(&origin_y, 2, 1, f);
	fwrite(&swidth, 2, 1, f);
	fwrite(&sheight, 2, 1, f);
	fwrite(&bpp, 1, 1, f);
	fwrite(&desc_flags, 1, 1, f);

	// Write comment
	fwrite(&comment, 1, comment_lenght, f);

	uint32 size = width * height * 3;
 
	for (uint32 i = 0; i < size; i += 3)
	{
		byte tmp = image[i];
		image[i] = image[i + 2];
		image[i + 2] = tmp;
	}

	// Write image data
	if (fwrite(image, size, 1, f) < 1)
	{
		freyja_print("OpenGL::TakeScreenShot()  Disk write failed.\n");
		perror("OpenGL::TakeScreenShot() \n");
		fclose(f);
		return;
	}

	fclose(f);

	delete [] image;

	freyja_print("Took screenshot '%s'.\n", filename);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void OpenGL::SetMaxTextureCount(uint32 max)
{
	mTextureLimit = max;

	if (mTextureIds) 
		delete [] mTextureIds;

	mTextureIds = new uint32[max];
	glGenTextures(max, mTextureIds);
}


bool OpenGL::LoadFragmentARB(const char *filename,uint32 &fragmentId)
{
	if (!filename || !filename[0])
		return false;

	mstl::SystemIO::BufferedFileReader r;
	uint32 length = 0;
	char *program = NULL;

	if (r.Open(filename))
	{
		// The pointer program addresses will be freed when r's scope ends
		program = (char *)r.GetCompleteFileBuffer();
		length = r.GetCompleteFileBufferSize();
	}
	else
	{
		freyja_print("!Failed to open fragment from disk.");
		return false;
	}


#ifdef USING_OPENGL_EXT
	if (!h_glGenProgramsARB || !h_glBindProgramARB || !h_glProgramStringARB ||
		!h_glGetProgramivARB)
	{
		freyja_print("!Failed to aquire fragment functions from OpenGL.");
		return false;
	}

	GLuint id = fragmentId;
	h_glGenProgramsARB(1, &id);
	fragmentId = id;
	
	if (id == 0)
	{
		freyja_print("!Failed to get fragment Id from OpenGL.\n");
		return false;
	}

    h_glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, id);
    h_glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, 
						 GL_PROGRAM_FORMAT_ASCII_ARB,
						 length, program);

	// Try to detect program load errors
	GLint program_error_pos, program_limits;
	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &program_error_pos);
	h_glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB,
						GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB, &program_limits);

	if ( program_error_pos != -1 || program_limits != 1 )
	{
		freyja_print("!Failed to load fragment in OpenGL: %s%s",
					 (!program_limits) ? "Exceeded native limit, " : "",
					 (char*)glGetString(GL_PROGRAM_ERROR_STRING_ARB));

		freyja_event_info_dialog("gtk-dialog-info", 
								 (const char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB));

		return false;
    }

	h_glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, id);

	return true;
#else
	freyja_print("!OpenGL extensions are disabled.");
	return false;
#endif
}


void OpenGL::BindFragmentARB(int32 fragmentId)
{
#ifdef USING_OPENGL_EXT
	if (fragmentId < 0)
	{
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
	}
	else if (h_glBindProgramARB)
	{
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		h_glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragmentId);
	}
#endif
}


bool OpenGL::LoadFragmentGLSL(const char *filename, uint32 &fragmentId)
{
	if (!filename || !filename[0])
		return false;

	if (mstl::SystemIO::File::CompareFilenameExtention(filename, ".frag"))
	{
		freyja_print("!Not a .frag file.");
		return false;
	}

	mstl::SystemIO::BufferedFileReader r;
	char *frag = NULL;
	uint32 fragLength = 0;

	if (r.Open(filename))
	{
		// The pointer program addresses will be freed when r's scope ends
		frag = (char *)r.GetCompleteFileBuffer();
		fragLength = r.GetCompleteFileBufferSize();
		frag[fragLength-1] = 0; // strip off EOF
	}
	else
	{
		freyja_print("!Failed to open .frag from disk.");
		return false;
	}


	String s = filename;

	for (uint32 i = s.length() - 1; i > 0; --i)
	{
		if (s[i] == '.')
		{
			s[i] = '\0';
			break;
		}
	}

	s += ".vert";

	mstl::SystemIO::BufferedFileReader r2;
	char *vert = NULL;
	uint32 vertLength = 0;

	if (r2.Open(s.c_str()))
	{
		// The pointer program addresses will be freed when r2's scope ends
		vert = (char *)r2.GetCompleteFileBuffer();
		vertLength = r2.GetCompleteFileBufferSize();
		vert[vertLength-1] = 0; // strip off EOF
	}
	else
	{
		freyja_print("!Failed to open .vert '%s' from disk.", s.c_str());
		return false;
	}

#ifdef USING_OPENGL_EXT
	if (!h_glCreateProgramObjectARB || !h_glCreateShaderObjectARB ||
		!h_glCompileShaderARB || !h_glLinkProgramARB || !h_glGetObjectParameterivARB || !h_glUseProgramObjectARB)
	{
		freyja_print("!Failed to aquire shader functions from OpenGL.");
		return false;
	}

	// Get object handles
	GLhandleARB program = h_glCreateProgramObjectARB();
	GLhandleARB vertex = h_glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLhandleARB fragment = h_glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Load source for objects
	const GLcharARB *ptrVert = (GLcharARB *)vert;
	h_glShaderSourceARB(vertex, 1, &ptrVert, NULL);
	const GLcharARB *ptrFrag = (GLcharARB *)frag;
	h_glShaderSourceARB(fragment, 1, &ptrFrag, NULL);

	bool failure = false;
	GLint ok;

	// Compile objects
	h_glCompileShaderARB(vertex);
	h_glGetObjectParameterivARB(vertex, GL_OBJECT_COMPILE_STATUS_ARB, &ok);

	if (!ok)
	{
		DebugFragmentGLSL("<b>Compiling vertex failed...</b>\n", vertex);
		failure = true;
	}

	h_glCompileShaderARB(fragment);
	h_glGetObjectParameterivARB(fragment, GL_OBJECT_COMPILE_STATUS_ARB, &ok);

	if (!ok)
	{
		DebugFragmentGLSL("<b>Compiling fragment failed...</b>\n", fragment);
		failure = true;
	}

	// Attach object dependices to program
	h_glAttachObjectARB(program, vertex);	
	h_glAttachObjectARB(program, fragment);	

	// Link program
	h_glLinkProgramARB(program);
	h_glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &ok);

	if (!ok)
	{
		DebugFragmentGLSL("<b>Program linking failed...</b>\n", program);
		failure = true;
	}

	// Use program for rendering
	if (!failure)
	{
		h_glUseProgramObjectARB(program);
		fragmentId = program;

		if (program > mObjects)
			mObjects = program;
	}

	r.Close();
	r2.Close();
	return !failure;
#else
	r.Close();
	r2.Close();
	return false;
#endif
}


void OpenGL::BindFragmentGLSL(int32 fragmentId)
{
#ifdef USING_OPENGL_EXT
	if (h_glUseProgramObjectARB)
	{
		GLhandleARB prog = fragmentId;
		h_glUseProgramObjectARB(prog);
	}
#endif
}


void OpenGL::DeleteFragmentGLSL(int32 obj)
{
#ifdef USING_OPENGL_EXT
	if (h_glDeleteObjectARB)
	{
		GLhandleARB object = obj;
		h_glDeleteObjectARB(object);
	}
#endif
}


void OpenGL::DebugFragmentGLSL(const char *comment, int32 obj)
{
#ifdef USING_OPENGL_EXT
	if (h_glGetInfoLogARB == NULL)
		return;

	char buffer[2048];
	GLhandleARB object = obj;
	GLsizei maxLenght = 2048;
	GLsizei length;
	GLcharARB *infoLog = (GLcharARB *)buffer;

	buffer[0] = '\0';
	h_glGetInfoLogARB(object, maxLenght, &length, infoLog);
	buffer[2047] = '\0';

	if (length == 0)
	{
		//String s;
		//s.Set("No GLSL errors for object (%i).", obj);
		//freyja_event_info_dialog("gtk-dialog-info", (char *)s.c_str());	
		//return;
	}

	freyja_print("! %s%s", comment, buffer);

	String s = comment;

	for (int32 i = 0; i < length; ++i)
	{
		switch (buffer[i])
		{
		// Strip out stuff that would confuse pango markup
		case '>':
		case '<':
			buffer[i] = '\'';
			break;
		}
	}

	s += (length) ? buffer : "Couldn't get a GLSL log.";
	freyja_event_info_dialog("gtk-dialog-info", s.c_str());
#endif
}


byte *OpenGL::GenerateColorTexture(byte rgba[4], uint32 width, uint32 height)
{
	byte *image = new unsigned char[height*width*4];

	for (uint32 i = 0, size = width*height; i < size; ++i)
	{
		/* RBGA */
		image[i*4]   = rgba[0];
		image[i*4+1] = rgba[1];
		image[i*4+2] = rgba[2];
		image[i*4+3] = rgba[3];
	}

	return image;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void OpenGLContext::Init(uint32 width, uint32 height)
{
	/* Log OpenGL driver support information */
	freyja_print("[GL Driver Info]");
	freyja_print("\tVendor     : %s", glGetString(GL_VENDOR));
	freyja_print("\tRenderer   : %s", glGetString(GL_RENDERER));
	freyja_print("\tVersion    : %s", glGetString(GL_VERSION));
	freyja_print("\tExtensions : %s", (char*)glGetString(GL_EXTENSIONS));

	/* Test for extentions */
	if (mglHardwareExtTest("GL_ARB_multitexture"))
		;

	// Set up Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_TEXTURE_2D);

	// Set up culling
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	// Set background to black
	glClearColor(BLACK[0], BLACK[1], BLACK[2], BLACK[3]);

	// Disable lighting 
	glDisable(GL_LIGHTING);

	// Setup shading
	glShadeModel(GL_SMOOTH);

	// Use some hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);
	glEnable(GL_DITHER);
		
	// AA polygon edges
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FOG);

	glDisable(GL_NORMALIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPolygonMode(GL_FRONT, GL_FILL);

	glMatrixMode(GL_MODELVIEW);

	mWidth = width;
	mHeight = height;
	mInitContext = true;
}

void OpenGLContext::Resize(uint32 width, uint32 height) 
{
	if (!width || !height)
	{
		return;
	}

	mWidth = width;
	mHeight = height;
	mAspectRatio = (float)width / (float)height;

	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 

	if (1) // pers
	{
		mNearHeight = 10.0f;
		mNear = 10.0f;
		mFar = 1000.0f;

		glFrustum( -mNearHeight * mAspectRatio, 
					mNearHeight * mAspectRatio,
					-mNearHeight, mNearHeight, 
					mNear,
					mFar );
	}
	else 
	{
		glOrtho(-mScaleEnv * mAspectRatio,
				mScaleEnv * mAspectRatio, 
				-mScaleEnv, mScaleEnv, 
				-400.0, // zNear
				400.0);
	}

	glMatrixMode(GL_MODELVIEW);
}



////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGL
int runOpenGLUnitTest(int argc, char *argv[])
{
	OpenGL test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[OpenGL class test]\n");

	return runOpenGLUnitTest(argc, argv);
}
#endif



////////////////////////////////////////////////////////////
// mgl utils
////////////////////////////////////////////////////////////

bool mglHardwareExtTest(const char *ext)
{
	bool ret = false;
	const char *substr = (const char*)glGetString(GL_EXTENSIONS);

	FREYJA_ASSERTMSG(substr != NULL, "OpenGL Extensions could not be found!\nYou likely either have a misconfigured system, or your build linked to the wrong OpenGL library.");

	if (substr && strstr(substr, ext))
	{
		ret = true;
	}

	// Make logs easier to read!
	char pretty[32];
	long l = strlen(ext);
	l = (l < 32) ? 32 - l : 2;
	strncpy(pretty, "                                ", l);
	pretty[l] = 0;
	freyja_print("\t%s%s\t[%s]", ext, pretty, ret ? "YES" : "NO");
	
	return ret;
}


void mgl2dProjection(const unsigned int width, const unsigned int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	glOrtho(0, width, 0, height, -99999, 99999);
	glTranslatef(0.0, 0.0, 99998.0);
	glMatrixMode(GL_MODELVIEW);
}



void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor)
{
	glPointSize(4.0);

	glColor4fv(pointColor);

	glBegin(GL_POINTS);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glEnd();


	glColor4fv(lineColor);

	glBegin(GL_LINES);
	// max, top quad
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);

	// max-min, vertical quads
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min-max, vertical quads
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min, bottom quad
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);  
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glEnd();
}


void mglDrawSelectionBox(const vec3_t min, const vec3_t max, const vec4_t lineColor)
{
	glColor4fv(lineColor);

	glBegin(GL_LINE_LOOP);
	glVertex3fv(min);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3fv(max);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3fv(max);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], min[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3fv(min);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glEnd();
}


void mglDrawSelectBox(const vec3_t min, const vec3_t max, const vec4_t lineColor)
{
	vec_t w = (max[0] - min[0]);
	vec_t h = (max[1] - min[1]);
	vec_t d = (max[2] - min[2]);
	vec_t sw = w * 0.33;
	vec_t sh = h * 0.33;
	vec_t sd = d * 0.33;


	glColor4fv(lineColor);

	//glBegin(GL_POINTS);
	//glVertex3f(max[0], max[1], max[2]);
	//glVertex3f(min[0], min[1], min[2]);
	//glVertex3f(max[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], max[2]);
	//glVertex3f(max[0], max[1], min[2]);
	//glVertex3f(min[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], min[2]);
	//glVertex3f(max[0], min[1], min[2]);
	//glEnd();


	glBegin(GL_LINES);

	// A
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0] + sw, max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1] - sh, min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2] + sd);

	// B
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0] + sw, max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1] - sh, max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2] -sd);

	// C ( max )
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0] - sw, max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1] - sh, max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2] - sd);

	// D
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0] - sw, max[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1] - sh, min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1], min[2] + sd);

	// E ( min )
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0] + sw, min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1] + sh, min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2] + sd);

	// F
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0] + sw, min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1] + sh, max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2] - sd);

	// G
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0] - sw, min[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1] + sh, max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], max[2] - sd);
	
	//H
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0] - sw, min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1] + sh, min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2] + sd);

	glEnd();
}


// When you just don't want to do things the easy way... manually inlined
void mglDrawAxisWithCones(const vec_t min, const vec_t mid, const vec_t max,
						  uint32 count)
{  
	if (count < 3)
		count = 3;

	const float delta = helDegToRad(360.0f) * ( 1.0f / (float)count );
	Vec3 o, point, ith, last;
	vec_t s, t;


	// X Axis, Red
	glColor3fv(RED);
	glBegin(GL_LINES);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glEnd();

	o = Vec3(mid, 0.0f, 0.0f);
	point = o;
	point.mVec[0] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0], o.mVec[1] + s, o.mVec[2] + t);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0], o.mVec[1] + s, o.mVec[2] + t);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();


	// Y Axis, Green
	glColor3fv(GREEN);
	glBegin(GL_LINES);
	glVertex3f(0.0, -mid, 0.0);
	glVertex3f(0.0, mid, 0.0);
	glEnd();

	o = Vec3(0.0f, mid, 0.0f);
	point = o;
	point.mVec[1] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0] + s, o.mVec[1], o.mVec[2] + t);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0] + s, o.mVec[1], o.mVec[2] + t);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();


	// Z Axis, Blue
	glColor3fv(BLUE);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, -mid);
	glVertex3f(0.0, 0.0, mid);
	glEnd();

	o = Vec3(0.0f, 0.0f, mid);
	point = o;
	point.mVec[2] += mid+min;
	glBegin(GL_TRIANGLES);
	helSinCosf((count-1) * delta, &s, &t);
	last = Vec3(o.mVec[0] + s, o.mVec[1] + t, o.mVec[2]);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &s, &t);

		ith = Vec3(o.mVec[0] + s, o.mVec[1] + t, o.mVec[2]);

		/* Base */
		glVertex3fv(o.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();
}


void mglDrawEditorAxis()
{
	static int drawList = -1;

	if (drawList == -1)
	{
		const vec_t min = 0.5f, mid = 2.4f, max = 1.744f;
		//const vec_t min = 0.5f, mid = 1.2f, max = 0.872f;
		vec_t scale = 0.5f;
		drawList = glGenLists(1);
		glNewList(drawList, GL_COMPILE);

		glPushAttrib(GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); // White color texture ID

		glPushMatrix();
		glScalef(scale, scale, scale);

		mglDrawAxisWithCones(min, mid, max, 8);

		glPopMatrix();

		glPopAttrib();

		glEndList();
	}
	else
	{
		glCallList(drawList);
	}
}


void mglDrawAxisWithLines(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	//  Y arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
	//  Z arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, min);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, -min);


	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0, -mid, 0.0);	
	//  X arrowhead		
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
	//  Z arrowhead
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, min);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, -min);

      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0, -mid);
	//  Y arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  min,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0, -min,  max);
	//  X arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(min,  0.0,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(-min, 0.0,  max);
	glEnd();
}


void mglDrawCone(vec3_t origin, vec_t height, uint32 count)
{
	Vec3 point(origin[0], origin[1] + height, origin[2]);
	Vec3 center(origin[0], origin[1], origin[2]);
	vec_t x, z;
	const float delta = helDegToRad(360.0f) * ( 1.0f / (float)count );

	
	if (count < 3)
		count = 3;

	glBegin(GL_TRIANGLES);
	
	helSinCosf((count-1) * delta, &x, &z);
	Vec3 ith;
	Vec3 last(origin[0] + x, origin[1], origin[2] + z);

	for (uint32 i = 0; i < count; ++i)
	{ 
		helSinCosf((float)i * delta, &x, &z);

		ith = Vec3(origin[0] + x, origin[1], origin[2] + z);

		/* Base */
		glVertex3fv(center.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		/* Cone */
		glVertex3fv(point.mVec);
		glVertex3fv(ith.mVec);
		glVertex3fv(last.mVec);

		last = ith;
	}

	glEnd();
}


void mglDraw3dCircle(const vec3_t center, const vec_t radius, uint32 count, 
					 uint32 plane, bool solid)
{
	vec_t fCount = (float)((count < 8) ? 8 : count);
	vec_t x, z, i;

	glBegin(solid ? GL_LINE_LOOP : GL_LINES);

	for (i = 0.0f; i < fCount; ++i)
	{
		helSinCosf(helDegToRad(360.0f * (i / fCount)), &x, &z);

		x *= radius;
		z *= radius;

		switch (plane)
		{
		case 0:
			glVertex3f(center[0] + x, center[1] + z, center[2] + 0.0f);
			break;

		case 1:
			glVertex3f(center[0] + 0.0f, center[1] + x, center[2] + z);
			break;

		default:
			glVertex3f(center[0] + x, center[1] + 0.0f, center[2] + z);
			break;
		}
	}

	glEnd();
}


/* Based on draw_sphere (public domain), 1995, David G Yu */
void mglDrawSphere(int numMajor, int numMinor, float radius)
{
	double majorStep = (M_PI / numMajor);
	double minorStep = (2.0 * M_PI / numMinor);
	double a, b, c, r0, r1;
	float z0, z1, x, y;
	int i, j;
	
	for (i = 0; i < numMajor; ++i)
	{
		a = i * majorStep;
		b = a + majorStep;

		helSinCosf(a, &r0, &z0);
		r0 *= radius;
		z0 *= radius;

		helSinCosf(b, &r1, &z1);
		r1 *= radius;
		z1 *= radius;
		
		glBegin(GL_TRIANGLE_STRIP);
		
		for (j = 0; j <= numMinor; ++j) 
		{
			c = j * minorStep;
			helSinCosf(c, &y, &x);

			glNormal3f((x * r0) / radius, z0 / radius, (y * r0) / radius);
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			glVertex3f(x * r0, z0, y * r0);
			
			glNormal3f((x * r1) / radius, z1 / radius, (y * r1) / radius);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			glVertex3f(x * r1, z1, y * r1);
		}
		
		glEnd();
	}
}


void mglDrawControlPoint()
{
	static int drawList = -1;

	if (drawList == -1)
	{
		const unsigned int major = 10, minor = 10;
		const vec_t radius = 0.5f;

		drawList = glGenLists(1);
		glNewList(drawList, GL_COMPILE);

#if FORCE_ICON_SHADING
		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glEnable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, 0); // 'color' texture
#endif
		// Control point default
		mglDrawSphere(major, minor, radius);

#if FORCE_ICON_SHADING
		glPopAttrib();
#endif
		glEndList();
	}
	else
	{
		glCallList(drawList);
	}
}


void mglDrawBoneSolid(const vec3_t pos)
{
	Vec3 tip(0.0f,0.0f,0.0f), end(0.0f,1.0f,0.0f);
	Vec3 a(0.1,0.1,0.1), b(-0.1,0.1,-0.1), c(0.1,0.1,-0.1), d(-0.1,0.1,0.1);
	Vec3 p(pos);

	const float cutoff = 4.5f;
	const float scale = p.Magnitude();
	const float scale2 = (scale > cutoff) ? cutoff : scale;

#if 1
	Vec3 delta = end - p;
	delta.Norm();
	delta *= 0.1f;

	delta = p;
	delta.Norm();
	delta *= 0.1f;

	a = a + delta;
	b = b + delta;
	c = c + delta;
	d = d + delta;
#endif

	a *= scale2;
	b *= scale2;
	c *= scale2;
	d *= scale2;

#if FORCE_ICON_SHADING
	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0); // 'color' texture
	
	//vec4_t ambient = {0.326f, 0.666f, 0.779f, 1.0f};
	//vec4_t diffuse = {0.318f, 0.47f, 0.77f, 1.0f};
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
#endif


#if 1
	end = p;
	//tip.mY -= 0.1f * scale2;
	//tip = delta * -2.0f;
	tip = p * 0.2f * scale2;
	const float off = -0.1f * scale2;
	a.mY += off;
	b.mY += off;
	c.mY += off;
	d.mY += off;

	// might be better to scale control point
	mglDrawSphere(8, 8, 0.1f * scale2);
#endif

	glBegin(GL_TRIANGLES);
	glNormal3f(.3,.3,-.3);
	glVertex3fv(tip.mVec);
	glVertex3fv(a.mVec);
	glVertex3fv(c.mVec);

	glNormal3f(-.3,-.3,-.3);
	glVertex3fv(tip.mVec);
	glVertex3fv(b.mVec);
	glVertex3fv(d.mVec);

	glNormal3f(.3,-.3,-.3);
	glVertex3fv(tip.mVec);
	glVertex3fv(b.mVec);
	glVertex3fv(c.mVec);

	glNormal3f(-.3,.3,-.3);
	glVertex3fv(tip.mVec);
	glVertex3fv(a.mVec);
	glVertex3fv(d.mVec);


	glNormal3f(-.3,-.3,.3);
	glVertex3fv(end.mVec);
	glVertex3fv(a.mVec);
	glVertex3fv(c.mVec);

	glNormal3f(.3,.3,.3);
	glVertex3fv(end.mVec);
	glVertex3fv(b.mVec);
	glVertex3fv(d.mVec);

	glNormal3f(-.3,.3,.3);
	glVertex3fv(end.mVec);
	glVertex3fv(b.mVec);
	glVertex3fv(c.mVec);

	glNormal3f(.3,-.3,.3);
	glVertex3fv(end.mVec);
	glVertex3fv(a.mVec);
	glVertex3fv(d.mVec);
	glEnd();

#if FORCE_ICON_SHADING
	glPopAttrib();
#endif
}


void mglDrawBone(unsigned char type, const vec3_t pos)
{
	const vec_t min = 0.05f;
	const vec_t max = 0.50f;


	switch (type)
	{
	case 1:
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();
		break;

	case 2:
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0f,   min,  0.0f);    // 0
		glVertex3f(-max,  0.0f, -max);     // 1
		glVertex3f( max,  0.0f, -max);     // 2
		glVertex3fv(pos);                        // Base
		glVertex3f(-max,  0.0f,-max);      // 1
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f( max,  0.0f,-max);      // 2
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3fv(pos);                        // Base
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f(-max,  0.0f, max);      // 4
		glEnd();
		break;

	case 3:
		mglDrawBoneSolid(pos);
		break;
	}
}


void mglDrawJoint(unsigned char type, const vec3_t pos)
{
	switch (type)
	{
	case 1:
		glBegin(GL_POINTS);
		glVertex3fv(pos);
		glEnd();
		break;

	case 2:
		//mglDrawSphere(12, 12, 0.5f);
		mglDrawControlPoint();
		break;

	case 3:
		mglDrawEditorAxis();
		break;
	}
}


void mglDrawPlane(vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;

	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_QUADS);

		for (z = -size; z < size; z += step)
		{
			glNormal3f(0, 1, 0);
			glTexCoord2f((x + step)/size, z/size);
			glVertex3f((x + step) * scale, 0.0f, z * scale);

			glNormal3f(0, 1, 0);
			glTexCoord2f(x/size, z/size);
			glVertex3f(x * scale, 0.0f, z * scale);	

			glNormal3f(0, 1, 0);
			glTexCoord2f(x/size, (z + step)/size);
			glVertex3f(x * scale, 0.0f, (z + step) * scale);

			glNormal3f(0, 1, 0);
			glTexCoord2f((x + step)/size, (z + step)/size);
			glVertex3f((x + step) * scale, 0.0f, (z + step) * scale);
		}

		glEnd();
	}

	glPopMatrix();
}


void mglDrawGrid(vec3_t color, vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;


	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();
	glColor3fv(color);

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_LINE_LOOP);

		for (z = -size; z < size; z += step)
		{
			glVertex3f((x + step) * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, (z + step) * scale);
			glVertex3f((x + step) * scale, 0.0f, (z + step) * scale);
		}

		glEnd();
	}

	glPopMatrix();
}


void mglGetOpenGLViewport(int *viewportXYWH) // int[4]
{
	glGetIntegerv(GL_VIEWPORT, viewportXYWH);
}


#if 0
void mglGetOpenGLModelviewMatrix16dv(double *modelview) // double[16]
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
}


void mglGetOpenGLProjectionMatrix16dv(double *projection) // double[16]
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
}
#endif


void mglGetOpenGLModelviewMatrix16fv(matrix_t modelview)
{
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
}


void mglGetOpenGLProjectionMatrix16fv(matrix_t projection)
{
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
}


void mglApplyMaterial(uint32 materialIndex)
{
	vec4_t ambient, diffuse, specular, emissive;
	vec_t shininess;
	uint32 flags, texture, texture2, blendSrc, blendDest;


	if (materialIndex > freyjaGetMaterialCount())
	{
		materialIndex = 0;
	}

	// Id 0 disables ( no weird index scheme here )
	freyja3d::OpenGL::BindFragmentGLSL(freyjaGetMaterialShader(materialIndex));

	flags = freyjaGetMaterialFlags(materialIndex);
	texture = freyjaGetMaterialTexture(materialIndex);
	blendSrc = freyjaGetMaterialBlendSource(materialIndex);
	blendDest = freyjaGetMaterialBlendDestination(materialIndex);
	freyjaGetMaterialAmbient(materialIndex, ambient);
	freyjaGetMaterialDiffuse(materialIndex, diffuse);
	freyjaGetMaterialSpecular(materialIndex, specular);
	freyjaGetMaterialEmissive(materialIndex, emissive);
	shininess = freyjaGetMaterialShininess(materialIndex);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	glMaterialfv(GL_FRONT, GL_SHININESS, &(shininess));

	// 2006.07.15 - Hey, these should use Texture class binding to
	// make sure the array ids match in the gl texture binding
	if (flags & fFreyjaMaterial_DetailTexture)
	{
		Texture::mSingleton->bindMultiTexture(texture, texture2);
	}
	else if (flags & fFreyjaMaterial_Texture) // Non-colored is ( id + 1)
	{
		glBindTexture(GL_TEXTURE_2D, texture+1);
	}
	else // Colored, first texture is a generated WHITE 32x32
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (flags & fFreyjaMaterial_Blending)
	{
		glBlendFunc(blendSrc, blendDest);
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}


void mglEnterMode2d(unsigned int width, unsigned int height)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	/* This allows alpha blending of 2D textures with the scene */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void mglExitMode2d()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
}
