/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : 
 * Comments: Config control header.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.06.12:
 * Mongoose - Centralized various defines, etc here.
 * 
 ==========================================================================*/

#ifndef GUARD__FREYJA_CONFIG_H_
#define GUARD__FREYJA_CONFIG_H_

// General /////////////////////////////////////

#define ENABLE_FREYJA_ASSERTS 1


// Hardcoded strings ///////////////////////////

#define FREYJA_LOG_FILE "freyja-dev.log"

#define FREYJA_RECENT_FILES "recent_files-dev"


// Renderer ////////////////////////////////////

#define WIN32_GL_EXT				1

#define CHECK_FOR_OPENGL_ERRORS		0

#define USING_FREYJA_CPP_ABI		1

#define SHADOW_VOLUME_WITH_ARRAYS	1

#define TEST_SHADOW_VOLUME_SURFACES	0

#define USE_TORUS_TEST				0

#define PLANE_NOTIFY_WITH_AXIS      1


// UI //////////////////////////////////////////

#define DEBUG_PICK_RAY				0

#define DEBUG_SCREEN_TO_WORLD		0

#define DEBUG_VIEWPORT_MOUSE		0

#define FREYJA_APP_PLUGINS			1

#define DISABLED_EVENT				0

#define ENABLE_ASSERT_LOG			1

#define DEBUG_RESOURCE_COLOR        0

#define DEBUG_BONE_LOAD             0



#endif // GUARD__FREYJA_CONFIG_H_


