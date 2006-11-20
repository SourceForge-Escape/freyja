/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This provides a command line interface translation/test tool.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.08:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <freyja/FreyjaPluginABI.h>

int main(int argc, char *argv[])
{
	switch (argc)
	{
	case 4: // freyjautil file.grn 3ds file.3ds
		freyjaSpawn();

		/* Search local paths first ( mostly debugging ) */
		freyjaPluginAddDirectory("plugins/model/debug");

		freyjaImportModel(argv[1]);
		freyjaExportModel(argv[3], argv[2]);
		freyjaFree();
		break;


	case 2: // freyjautil file.glm
		freyjaSpawn();

		/* Search local paths first ( mostly debugging ) */
		freyjaPluginAddDirectory("plugins/model/debug");

		freyjaImportModel(argv[1]);
		freyjaFree();
		break;

	default:
		printf("Usage: %s import_model [type export_model]\n", argv[0]);
	}

	return 0;
}


