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

#define USING_FREYJA_CPP_ABI

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include "Printer.h"
#include "PluginABI.h"
#include "FreyjaFSM.h"
#include "freyja.h"

using namespace mstl;
using namespace freyja;

Printer *gPrinter = NULL;
uint32 gFreyjaMemoryTick = 0;
uint32 gFreyjaMemoryNews = 0;
uint32 gFreyjaMemoryDeletes = 0;
FreyjaAssertCallback gFreyjaAssertHandler = NULL;


///////////////////////////////////////////////////////////////////////
// Managed ABI ( Not 'public' )
///////////////////////////////////////////////////////////////////////

void freyja__setPrinter(Printer *printer, bool freyjaManaged)
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
	if (FreyjaFSM::GetInstance())
	{
		FreyjaFSM *fsm = FreyjaFSM::GetInstance();

		/* Here just to avoid compiler warnings and removal by opt */
		fsm->freyjaGetCount(FREYJA_VERTEX);

		/* Setup basic default stdout printer */
		freyja__setPrinter(new Printer(), true);

		/* Setup plugins */
		freyjaPluginDirectoriesInit();
		freyjaPluginsInit();

		freyjaPrintMessage("libfreyja invoked using freyjaSpawn()");
	}
	else
	{
		freyjaPrintMessage("libfreyja freyjaSpawn() failed unexpectedly");
	}
}


void freyjaFree()
{
	FreyjaFSM *FreyjaFSM = FreyjaFSM::GetInstance();
	
	if (FreyjaFSM)
	{
		delete FreyjaFSM;
	}

	freyjaPluginShutdown();

	freyjaPrintMessage("\nlibfreyja stopped using freyjaFree()");

	// Memory stats
	freyjaPrintMessage("\nMemoryPool stats:\n %u allocations\n %u deallocations\n %u operations\n\n", gFreyjaMemoryNews, gFreyjaMemoryDeletes, gFreyjaMemoryTick);

	if (gPrinter)
	{
		freyjaPrintMessage("\nlibfreyja stopping line printer");
		delete gPrinter;
	}
}


void freyjaAssertHandler(FreyjaAssertCallback func)
{
	gFreyjaAssertHandler = func;
}


byte freyjaAssertMessage(const char *file, unsigned int line, 
						 const char *function, const char *exprString,
						 bool expr, const char *format, ...)
{
	if (expr)
		return 0;

	freyjaPrintMessage("Assert encountered: %s:%i %s() '%s'", 
					   file, line, function, exprString);

	if (gPrinter)
	{
		va_list args;
		va_start(args, format);	
		gPrinter->MessageArgs(format, &args);
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

	if (gFreyjaAssertHandler)
	{
		char msg[1024];
		va_list args;
		va_start(args, format);	
		vsnprintf(msg, 1023, format, args);
		va_end(args);
		msg[1023] = 0;

		if ((*gFreyjaAssertHandler)(file, line, function, exprString, msg))
		{
			freyjaPrintMessage("Assert ignored by event handler...");
			return 0;
		}
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
		gPrinter->MessageArgs(format, &args);
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
		gPrinter->ErrorArgs(format, &args);
	}
	else
	{
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n");
	}

	va_end(args);
}


const char *freyjaObjectToString(freyja_object_t obj)
{
	switch (obj)
	{
	case FREYJA_MODEL:
		return "FREYJA_MODEL";
		break;

	case FREYJA_MESH:
		return "FREYJA_MESH";
		break;

	case FREYJA_POLYGON:
		return "FREYJA_POLYGON";
		break;

	case FREYJA_BONE:
		return "FREYJA_BONE";
		break;

	case FREYJA_SKELETON:
		return "FREYJA_SKELETON";
		break;

	case FREYJA_VERTEX_FRAME:
		return "FREYJA_VERTEX_FRAME";
		break;

	case FREYJA_VERTEX_GROUP:
		return "FREYJA_VERTEX_GROUP";
		break;

	case FREYJA_VERTEX:
		return "FREYJA_VERTEX";
		break;

	case FREYJA_TEXCOORD:
		return "FREYJA_TEXCOORD";
		break;

	case FREYJA_MATERIAL:
		return "FREYJA_MATERIAL";
		break;

	case FREYJA_TEXTURE:
		return "FREYJA_TEXTURE";
		break;

	case FREYJA_SKEL_ANIMATION:
		return "FREYJA_SKEL_ANIMATION";
		break;

	case FREYJA_SKEL_KEYFRAME:
		return "FREYJA_SKEL_KEYFRAME";
		break;
	}

	return "UNKNOWN";
}


const char *freyjaTransformToString(freyja_transform_t transform)
{
	switch (transform)
	{
	case fTransformScene:
		return "fTransformScene";
		break;

	case fTransformModel:
		return "fTransformModel";
		break;

	case fTransformMesh:
		return "fTransformMesh";
		break;

	case fTransformVertexFrame:
		return "fTransformVertexFrame";
		break;

	case fTransformSkeleton:
		return "fTransformSkeleton";
		break;

	case fTransformBone:
		return "fTransformBone";
		break;

	case fTransformUVMap:
		return "fTransformUVMap";
		break;

	case fTransformVertexGroup:
		return "fTransformVertexGroup";
		break;

	case fTransformVertex:
		return "fTransformVertex";
		break;

	case fTransformTexCoord:
		return "fTransformTexCoord";
		break;

	default:
		;
	}

	return "Unknown_Transform";	
}


const char *freyjaActionToString(freyja_transform_action_t action)
{
	switch (action)
	{
	case fTranslate:
		return "fTranslate";
		break;

	case fRotateAboutOrigin:
		return "fRotateAboutOrigin";
		break;

	case fRotate:
		return "fRotate";
		break;

	case fScaleAboutOrigin:
		return "fScaleAboutOrigin";
		break;

	case fScale:
		return "fScale";
		break;

	default:
		;
	}

	return "Unknown_Action";
}


///////////////////////////////////////////////////////////////////////
// Freyja Memory management and logging
// Based on mtk_mememory, however I removed the expensive RBTree 
// and all the cool features like profiling if you didn't notice  =p
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






