/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is a general library lib util module.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, This is a general library lib util module
 ==========================================================================*/

#if defined (FREYJA_PLUGINS) && !(WIN32)
#   include <dlfcn.h>
#endif

#ifdef WIN32
#   include <windows.h>
#endif

#include <mstl/Vector.h>
#include "FreyjaPrinter.h"
#include "FreyjaFSM.h"

#include "freyja.h"


using namespace mstl;

FreyjaPrinter *gPrinter = NULL;
extern Vector<FreyjaPluginDesc *> gFreyjaPlugins;
extern Vector<char *> gPluginDirectories;

// We want to keep up with the order of operations too for future logging
uint32 gFreyjaMemoryTick = 0;
uint32 gFreyjaMemoryNews = 0;
uint32 gFreyjaMemoryDeletes = 0;


///////////////////////////////////////////////////////////////////////
// Managed ABI ( Not 'public' )
///////////////////////////////////////////////////////////////////////

void freyja__setPrinter(FreyjaPrinter *printer, bool freyjaManaged)
{
	if (!printer)
		return;

	if (freyjaManaged)
	{
		if (gPrinter && gPrinter != printer)
		{
			delete gPrinter;
		}

		gPrinter = printer;
	}
}


/* Thanks to Sam for the WIN32 module loader example */
void *freyjaModuleImportFunction(void *handle, const char *name)
{
	char *loaderror = 0x0;
	void *symbol = NULL;

#ifdef WIN32
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
		freyjaPrintError("Failed to import %s: %s", name, loaderror);
	}

	return symbol;
}


void *freyjaModuleLoad(const char *module)
{
	void *handle = NULL;
	char *loaderror;

#ifdef WIN32
	char errbuf[512];

	if (FreyjaFileReader::compareFilenameExtention(module, ".dll") != 0)
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

	if (FreyjaFileReader::compareFilenameExtention(module, ".so") != 0)
	{
		return NULL;
	}

	handle = dlopen(module, RTLD_NOW);
	loaderror = (char *)dlerror();

#endif

	if (handle == NULL)
	{
		freyjaPrintError("Failed to load %s: %s", module, loaderror);
	}

	return handle;
}


void freyjaModuleUnload(void *handle)
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


///////////////////////////////////////////////////////////////////////
// Public ABI
///////////////////////////////////////////////////////////////////////

void freyjaSpawn()
{
	if (!FreyjaFSM::GetInstance())
	{
		FreyjaFSM *fsm = FreyjaFSM::GetInstance();

		/* Here just to avoid compiler warnings and removal by opt */
		fsm->freyjaGetCount(FREYJA_VERTEX);

		/* Setup basic default stdout printer */
		freyja__setPrinter(new FreyjaPrinter(), true);

		/* Setup plugins */
		freyjaPluginDirectoriesInit();
		freyjaPluginsInit();

		freyjaPrintMessage("libfreyja invoked using freyjaSpawn()");
	}
}


void freyjaFree()
{
	FreyjaFSM *FreyjaFSM = FreyjaFSM::GetInstance();
	

	if (FreyjaFSM)
	{
		delete FreyjaFSM;
	}

	if (gPrinter)
	{
		delete gPrinter;
	}

	gPluginDirectories.erase();
	gFreyjaPlugins.erase();

	freyjaPrintMessage("\nlibfreyja stopped using freyjaFree()");

	// Memory stats
	freyjaPrintMessage("\nMemory stats:\n %u allocations\n %u deallocations\n %u operations\n\n", gFreyjaMemoryNews, gFreyjaMemoryDeletes, gFreyjaMemoryTick);
}


byte freyjaAssertMessage(bool expr, const char *format, ...)
{
	if (expr)
		return 0;

	va_list args;
	

#if __x86_64__ || __x86_32__
	asm(
		 "int $3 \n" 
	);
#else
	MARK_MSGF("No soft breakpoint inserted for this arch");
#endif

	va_start(args, format);	

	if (gPrinter)
	{
		gPrinter->messageArgs(format, &args);
	}
	else
	{
		vfprintf(stdout, format, args);
		printf("\n");
	}

	va_end(args);

	return 1;
}


void freyjaPrintMessage(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);	

	if (gPrinter)
	{
		gPrinter->messageArgs(format, &args);
	}
	else
	{
		vfprintf(stdout, format, args);
		printf("\n");
	}

	va_end(args);
}


void freyjaPrintError(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);	

	if (gPrinter)
	{
		gPrinter->errorArgs(format, &args);
	}
	else
	{
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n");
	}

	va_end(args);
}


///////////////////////////////////////////////////////////////////////
// Freyja Memory management and logging
// Based on mtk_mememory, however I removed the expensive RBTree  =)
///////////////////////////////////////////////////////////////////////
#define USING_FREYJA_MEMORY 1

#if USING_FREYJA_MEMORY
void freyjaAddTrack(void *ptr, size_t size, const char *file, int line, const char *func, uint32 flags)
{
	if (gPrinter) 
		gPrinter->Log("memory.csv", "%u, %p, %u, %s, %i, %s", gFreyjaMemoryTick, ptr, size, file, line, func);

	++gFreyjaMemoryNews;
	++gFreyjaMemoryTick;
}


void freyjaRemoveTrack(void *ptr, const char *file, int line, const char *func, uint32 flags)
{
	if (gPrinter) 
		gPrinter->Log("memory.csv", "%u, %p, ?, %s, %i, %s", gFreyjaMemoryTick, ptr, file, line, func);

	++gFreyjaMemoryDeletes;
	++gFreyjaMemoryTick;
}


void operator delete(void *ptr)//, const char *file, int line, const char *func)
{
	freyjaRemoveTrack(ptr, "", 0, "", 0x0); //file, line, func, 0x0);

	if (ptr == NULL)
		return;

	free(ptr);
};


void operator delete [](void *ptr)//, const char *file, int line, const char *func)
{
	freyjaRemoveTrack(ptr, "", 0, "", 0x1); //file, line, func, 0x1);

	if (ptr == NULL)
		return;

	free(ptr);
};

void *operator new(size_t size, const char *file, int line, const char *func)
{
	void *ptr = (void *)malloc(size);

	freyjaAddTrack(ptr, size, file, line, func, 0x0);
	return ptr;
};


void *operator new [](size_t size, const char *file, int line, const char *func)
{
	void *ptr = (void *)malloc(size);

	freyjaAddTrack(ptr, size, file, line, func, 0x1);
	return ptr;
}
#endif






