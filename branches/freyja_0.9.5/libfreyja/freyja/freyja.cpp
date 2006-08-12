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

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include "FreyjaPrinter.h"
#include "FreyjaFSM.h"


const char *gStoreFile = 0x0;
unsigned int gStoreLine = 0;
const char *gStoreFunc = 0x0;


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
	return SystemIO::ImportFunction(handle, name);
}

void *freyjaModuleLoad(const char *module)
{
	return SystemIO::ModuleLoad(module);
}


void freyjaModuleUnload(void *handle)
{
	SystemIO::ModuleUnload(handle);
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
	freyjaPrintMessage("\nMemoryPool stats:\n %u allocations\n %u deallocations\n %u operations\n\n", gFreyjaMemoryNews, gFreyjaMemoryDeletes, gFreyjaMemoryTick);
}

void freyjaStoreMark(const char *file, unsigned int line, const char *func)
{
	gStoreFile = file;
	gStoreLine = line;
	gStoreFunc = func;
}


byte freyjaAssertMessage(bool expr, const char *format, ...)
{
	if (expr)
		return 0;

	freyjaPrintMessage("Assert encountered: %s:%i %s", gStoreFile, gStoreLine, gStoreFunc);

	if (gPrinter)
	{
		va_list args;
		va_start(args, format);	
		gPrinter->messageArgs(format, &args);
		va_end(args);
	}
	else
	{
		va_list args;
		va_start(args, format);	
		vfprintf(stdout, format, args);
		fprintf(stdout, "\n");
		va_end(args);
	}

	SystemIO::Assert(expr);

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
	//if (gPrinter) gPrinter->Log("memory.csv", "%u, %p, %u, %s, %i, %s", gFreyjaMemoryTick, ptr, size, file, line, func);

	++gFreyjaMemoryNews;
	++gFreyjaMemoryTick;
}


void freyjaRemoveTrack(void *ptr, const char *file, int line, const char *func, uint32 flags)
{
	//if (gPrinter) gPrinter->Log("memory.csv", "%u, %p, ?, %s, %i, %s", gFreyjaMemoryTick, ptr, file, line, func);

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






