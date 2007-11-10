/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2005.01.03: (v0.9.1)
 * Mongoose - Using Freyja versioning numbers for ABI reporting.
 *
 *            Lots of pruning and documentation updates to the ABI.  Also
 *            some new features to go with the refactoring.
 *
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#ifndef GUARD__FREYJA_PLUGINABI_H_
#define GUARD__FREYJA_PLUGINABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Plugin ABI 0.9.3 - 0.9.5
	///////////////////////////////////////////////////////////////////////

	void freyjaPluginDirectoriesInit();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets default plugin directories.
	 ------------------------------------------------------*/

	void freyjaPluginsInit();
	/*------------------------------------------------------
	 * Pre  : freyjaPluginDirectoriesInit() is called, or you've
	 *        set some custom paths with freyjaPluginAddDirectory(...)
	 * Post : Scans plugin directories and sets up newly found plugins.
	 ------------------------------------------------------*/

	void freyjaPluginAddDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : <dir> is valid string representing valid directory
	 * Post : Directory is added to runtime linked library
	 *        search path list.
	 ------------------------------------------------------*/

	void freyjaPluginName(uint32 pluginIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginDescription(uint32 pluginIndex, const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExtention(uint32 pluginIndex, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetPluginCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	
	void freyjaPluginShutdown();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : After calling this libfreyja disables plugins.
	 ------------------------------------------------------*/

	void freyjaPluginBegin();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginName1s(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginDescription1s(const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginAddExtention1s(const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImport1i(int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExport1i(int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1i(const char *name, int32 defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1f(const char *name, float defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1s(const char *name, const char *defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginRegisterMenuCallback(int (*)(int menuId, const char *label, int (*)(int event)));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	const char* freyjaPluginFindByDescription(const char* desc);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns filename of plugin with description or
	 *        NULL if not found.
	 *
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	int32 freyjaGetPluginId();

	int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg);

	int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg);

	int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg);

	int freyjaGetPluginArgString(int32 pluginId, const char *name, 
								 int32 len, char *arg);



}

#   if defined(__cplusplus) && defined(USING_FREYJA_CPP_ABI)
#   include <mstl/String.h>
#   include <freyja/Plugin.h>

    mstl::Vector<freyja::PluginDesc *> &freyjaGetPluginDescriptions();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

    mstl::Vector<mstl::String> &freyjaGetPluginDirectories();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

    freyja::PluginDesc *freyjaGetPluginClassByFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

    freyja::PluginDesc *freyjaGetPluginClassByIndex(long pluginIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

#   endif

#endif // GUARD__FREYJA_PLUGINABI_H_
