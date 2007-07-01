/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry Hendrix ( Mongoose )
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : SystemIO w/ endian support
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: Lots of nice utils from libfreyja moved to mstl
 *           aids system abstraction and debugging.
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.23:
 * Mongoose - Barrowed from my libfreyja, and then made more generic
 ================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_H_
#define GUARD__MSTL_SYSTEMIO_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>

#ifdef WIN32
#   include <windows.h>
#   include <winbase.h> // Required for Sleep()
//#   include <libgen.h>  // Required for Basename()
#else // UNIX is default system
#   include <dlfcn.h>
#   include <sys/times.h>
#endif

// Define DISABLE_SYSTEMIO_DEBUG_MSG to disable this debug macro
#if defined(DEBUG) && !defined(DISABLE_SYSTEMIO_DEBUG_MSG)
#   define DEBUG_MSG SystemIO::Print("(%s:%i): %s() ", __FILE__, __LINE__, __func__); SystemIO::Print
#else
#   define DEBUG_MSG(...)
#endif

// This is for warnings and spew that should always bulid
#define MSTL_MSG SystemIO::Print("(%s:%i): %s() ", __FILE__, __LINE__, __func__); SystemIO::Print

// This is for assert messages that should always bulid
#define MSTL_ASSERTMSG(expr, format, ...)  if (!(expr)) SystemIO::AssertMsgMarker(__FILE__, __LINE__, __func__, false, #expr, format, ##__VA_ARGS__)


//INFINITY IEEE

#include "Timer.h"
#include "System/IO/File.h"
#include "System/IO/FileReader.h"
#include "System/IO/FileWriter.h"
#include "System/IO/BufferedFileReader.h"
#include "System/IO/TextFileReader.h"
#include "System/IO/BufferedTextFileReader.h"
#include "System/IO/TextFileWriter.h"


namespace mstl { 
namespace SystemIO {

typedef int (*AssertCallback)(const char *file, unsigned int line, 
										const char *function,
										const char *exprs);

// This is for assert messages that should always bulid
#define MSTL_ASSERTMSGC(callback, expr, format, ...)  if (!(expr)) SystemIO::AssertMsgMarkerWithCallback(callback, __FILE__, __LINE__, __func__, false, #expr, format, ##__VA_ARGS__)


	////////////////////////////////////////////////////////////
	// Namespace Methods.
	////////////////////////////////////////////////////////////

	static bool AssertMsgMarkerWithCallback(AssertCallback func,
														 const char *file, unsigned int line, 
														 const char *function,
														 bool expr, const char *exprs,
														 const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool AssertMsgMarker(const char *file, unsigned int line, 
										 const char *function,
										 bool expr, const char *exprs,
										 const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool AssertMsg(bool expr, const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void Assert(bool expr);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void Basename(const char *filename, char *basename, long size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool CopyFileToPath(const char *filename, const char *path);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 * NOTE : This defaults to overwrite, so check with DoesFileExist()
	 *        first if you want to avoid that.
	 ------------------------------------------------------*/

	static void Log(const char *filename, const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void MicroSleep(long usec);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void BufferedPrint(char *string, unsigned int len, char *s, ...);
	/*------------------------------------------------------
	 * Pre  : <String> is allocated to <Len> characters
	 *        the rest is just like a printf() call
	 *
	 * Post : Generates the string and buffers it in <String>
	 *
	 ------------------------------------------------------*/

	static bool IsFloatNan(float r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static int DebugFloat(float r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void Print(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void PrintArgs(const char *format, va_list *args);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void PrintError(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void PrintErrorArgs(const char *format, va_list *args);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void *ImportFunction(void *handle, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static bool CheckFilenameExt(const char *filename, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static const char *GetModuleExt();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	static bool CheckModuleExt(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void *ModuleLoad(const char *module);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void ModuleUnload(void *handle);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void SetLocaleNumericToC();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void OpenURLInWebBrowser(const char *url);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


////////////////////////////////////////////////////////////
// Namespace Methods 
////////////////////////////////////////////////////////////

static inline
bool AssertMsgMarkerWithCallback(AssertCallback func,
											 const char *file, unsigned int line, 
											 const char *function,
											 bool expr, const char *exprs,
											 const char *format, ...)
{
	if (expr)
		return false;

	printf("(%s:%u): %s Assertion '%s' failed: \n", file, line, function, exprs);

	va_list args;
	va_start(args, format);	
	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");
	va_end(args);

	// Use callback function here to handle assert events, which are
	// useful for making GUI MsgBox asserts handlers, etc.
	if (func)
	{
		if ((*func)(file, line, function, exprs) == 0)
			return false;
	}

	// Soft breakpoint insertion
	Assert(expr);

	return true;
}


static inline
bool AssertMsgMarker(const char *file, unsigned int line, 
									 const char *function,
									 bool expr, const char *exprs,
									 const char *format, ...)
{
	if (expr)
		return false;

	printf("(%s:%u): %s Assertion '%s' failed: \n", file, line, function, exprs);

	va_list args;
	va_start(args, format);	
	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");
	va_end(args);

	// TODO: Add attachable helper objects here to handle assert events
	// useful for making GUI MsgBox asserts, etc
	

	// Soft breakpoint insertion
	Assert(expr);

	return true;
}


static inline
bool AssertMsg(bool expr, const char *format, ...)
{
	if (expr)
		return false;

	va_list args;
	va_start(args, format);	
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, "\n");

	// TODO: Add attachable helper objects here to handle assert events
	// useful for making GUI MsgBox asserts, etc
	

	// Soft breakpoint insertion
	Assert(expr);

	return true;
}


static inline
void Assert(bool expr)
{
	if (expr)
		return;

	// Soft breakpoint insertion
#if defined(__x86_64__) || defined(__i386__)
	asm(
		 "int $3 \n" 
	);
#elif defined(__powerpc__)
	/* Trap on any condition */
	asm(
		 "tw 31, r1, r1 \n"
	);
#else
#   warning "No soft breakpoint can be inserted for this machine language"
#endif
}


static inline
void Basename(const char *filename, char *basename, long size)
{
	long len_f, len, i;


	/* Check for null or blank input strings */
	if (!filename || !filename[0] || !basename)
	{
		return;
	}

	basename[0] = 0;

	len = len_f = strlen(filename);

	while (len_f-1)
	{
		--len_f;

		if (filename[len_f] == '/' || filename[len_f] == '\\')
		{
			--len_f;
			break;
		}
	}

	len = len - len_f;

	if (size > len)
	{
		for (i = 0; i < len; ++i)
		{
			basename[i] = filename[len_f+i];
			basename[i+1] = 0;
		}
	}
}


static bool inline
CopyFileToPath(const char *filename, const char *path)
{
	FILE *src, *dest;
	char copy[1024];
	char buffer[1024];
	long size, sz = 1024;
	long len;

	src = fopen(filename, "rb");

	if (!src)
		return false;

	fseek(src, 0L, SEEK_END);
	size = ftell(src);
	fseek(src, 0L, SEEK_SET);

	Basename(filename, buffer, 1024);
	len = strlen(path) + 2 + strlen(buffer);
	snprintf(copy, len, "%s/%s", path, buffer);
	
	//printf("copy := '%s'\n", copy);
	dest = fopen(copy, "wb");

	if (!dest)
	{
		fclose(src);
		return false;
	}

	for (; size > sz; size -= sz)
	{
		fread(buffer, sz, 1, src);
		fwrite(buffer, sz, 1, dest);
	}

	if (size > 0)
	{
		fread(buffer, size, 1, src);
		fwrite(buffer, size, 1, dest);
	}

	fclose(src);
	fclose(dest);

	return true;
}


static inline
void Log(const char *filename, const char *format, ...)
{
	FILE *f = fopen(filename, "a");
	va_list args;
	char buffer[1024];
	unsigned int l;
	
	if (!f)
	{
		perror( filename );
		return;
	}

	va_start(args, format);

	vsnprintf(buffer, 1024, format, args);

	l = strlen(buffer);

	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(f, "%s\n", buffer);

	va_end(args);
}


static inline
void MicroSleep(long usec)
{
#ifdef WIN32
	Sleep(usec);
#else
	usleep(usec); 
#endif
}


static inline
void BufferedPrint(char *string, unsigned int len, char *s, ...)
{
	va_list args;

	if (s && s[0])
	{
		va_start(args, s);
		vsnprintf(string, len-1, s, args);	
		string[len-1] = 0;
		va_end(args);
	}	
}


static inline
bool IsFloatNan(float r) 
{ 
	return (r == FP_NAN); 
}


static inline
int DebugFloat(float r)
{
	int i = fpclassify(r);

	switch (i)
	{
	case FP_NAN:
		fprintf(stdout, "FP_NAN: %f\n", r);
		break;

	case FP_INFINITE:
		fprintf(stdout, "FP_INFINITE: %f\n", r);
		break;

	case FP_ZERO:
		fprintf(stdout, "FP_ZERO: %f\n", r);
		break;

	case FP_SUBNORMAL:
		fprintf(stdout, "FP_SUBNORMAL: %f\n", r);
		break;

	case FP_NORMAL:
		//fprintf(stdout, "FP_NORMAL: %f\n", r);
		break;

	default:
		fprintf(stdout, "Unknown state: %f\n", r);
	}

	return i;
}

static inline
void Print(const char *format, ...)
{
	va_list args;
	char buffer[1024];
	unsigned int l;
	
	va_start(args, format);

	vsnprintf(buffer, 1024, format, args);

	l = strlen(buffer);

	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stdout, "%s\n", buffer);

	va_end(args);
}


static inline
void PrintArgs(const char *format, va_list *args)
{
	char buffer[1024];
	unsigned int l;

	/* Strip message of an trailing carrage return 
	 *  and print to stdout and the status bar */
	vsnprintf(buffer, 1024, format, *args);

	l = strlen(buffer);

	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stdout, "%s\n", buffer);
}


static inline
void PrintError(const char *format, ...)
{
	va_list args;
	char buffer[1024];
	unsigned int l;
	
	va_start(args, format);

	vsnprintf(buffer, 1024, format, args);

	l = strlen(buffer);

	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stderr, "%s\n", buffer);

	va_end(args);
}


static inline
void PrintErrorArgs(const char *format, va_list *args)
{
	char buffer[1024];
	unsigned int l;

	/* Strip message of an trailing carrage return 
	 *  and print to stdout and the status bar */
	vsnprintf(buffer, 1024, format, *args);

	l = strlen(buffer);

	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stderr, "%s\n", buffer);
}


static inline
void *ImportFunction(void *handle, const char *name)
{
	char *loaderror = 0x0;
	void *symbol = NULL;

#ifdef WIN32
	/* Thanks to Sam for the WIN32 module loader example */
	char errbuf[512];

	symbol = (void *)GetProcAddress((HMODULE)handle, name);

	if (symbol == NULL)
	{
		FormatMessage((FORMAT_MESSAGE_IGNORE_INSERTS |
							FORMAT_MESSAGE_FROM_SYSTEM),
						  NULL, GetLastError(), 
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  errbuf, 512, NULL);
		loaderror = errbuf;
	}

#else // UNIX is always assumed here, everything else is special case

	symbol = dlsym(handle, name);

	if (symbol == NULL)
	{
		loaderror = (char *)dlerror();
	}

#endif

	if (symbol == NULL)
	{
		PrintError("SystemIO failed to import %s: %s\n", name, loaderror);
	}

	return symbol;
}


static inline
bool CheckFilenameExt(const char *filename, const char *ext)
{
	return (File::CompareFilenameExtention(filename, ext) == 0);
}


static inline
const char *GetModuleExt()
{
#ifdef WIN32
	return ".dll";
#else
	return ".so";
#endif
}


static inline
bool CheckModuleExt(const char *filename)
{
#ifdef WIN32
	return (File::CompareFilenameExtention(filename, ".dll") == 0);
#else
	return (File::CompareFilenameExtention(filename, ".so") == 0);
#endif
}


static inline
void *ModuleLoad(const char *module)
{
	void *handle = NULL;
	char *loaderror;

#ifdef WIN32
	char errbuf[512];

	if (File::CompareFilenameExtention(module, ".dll") != 0)
	{
		return NULL;
	}

	handle = (void *)LoadLibrary(module);

	/* Generate an error message if all loads failed */
	if (handle == NULL) 
	{
		FormatMessage((FORMAT_MESSAGE_IGNORE_INSERTS |
							FORMAT_MESSAGE_FROM_SYSTEM),
						  NULL, GetLastError(), 
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  errbuf, 512, NULL);
		loaderror = errbuf;
	}
#else

	if (File::CompareFilenameExtention(module, ".so") != 0)
	{
		return NULL;
	}

	handle = dlopen(module, RTLD_NOW);
	loaderror = (char *)dlerror();
#endif

	if (handle == NULL)
	{
		PrintError("SystemIO Failed to load %s: %s\n", module, loaderror);
	}

	return handle;
}


static inline
void ModuleUnload(void *handle)
{
	if (handle == NULL)
	{
		return;
	}

#ifdef WIN32
	FreeLibrary((HMODULE)handle);
#else
	dlclose(handle);
#endif
}


static inline
void SetLocaleNumericToC()
{
	setlocale(LC_NUMERIC, "C");
}


static inline
void OpenURLInWebBrowser(const char *url)
{
#ifdef WIN32
	ShellExecute(NULL, "open", url, NULL, "C:\\", SW_SHOW );
#else
	char *browser = getenv("BROWSER");
	
	if (!browser)
	{
		if ( File::DoesFileExist("/usr/bin/epiphany") ) 
			browser = "/usr/bin/epiphany";
		else if ( File::DoesFileExist("/usr/bin/firefox") ) 
			browser = "/usr/bin/firefox";
		else if ( File::DoesFileExist("/usr/bin/opera") ) 
			browser = "/usr/bin/opera";
		else 
			browser = "firefox"; // if it fails no big deal
	}

	if (browser)
	{
		// FIXME: How can this be taken seriously?
		char buf[512];
		snprintf(buf, 512, "%s %s &", browser, url);
		buf[511] = 0;

		//printf("$ %s\n", buf);
		system(buf);
	}
#endif
}


} // namespace SystemIO

} // namespace mstl

#endif // GUARD__MSTL_SYSTEMIO_H_
