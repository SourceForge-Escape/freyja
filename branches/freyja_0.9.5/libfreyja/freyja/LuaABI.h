/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: This is the Lua ABI.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.21:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_LUAABI_H_
#define GUARD__FREYJA_LUAABI_H_

#include "freyja.h"

extern "C" {

#if LUA_FOUND_INCLUDE
#   include <lua.h>
#   include <lualib.h>
#   include <lauxlib.h>
#else // LUA_FOUND_INCLUDE
#   include <lua5.1/lua.h>
#   include <lua5.1/lualib.h>
#   include <lua5.1/lauxlib.h>
#endif // LUA_FOUND_INCLUDE


	////////////////////////////////////////////////////////////////
	// Lua ABI 0.9.5, 0.10 backport
	////////////////////////////////////////////////////////////////

#ifdef LUA_FOUND
	void freyja_lua_register_functions( lua_State* state );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/
#endif

	//void freyjaLuaBindFunc( const char* symbol, void* function );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//void freyjaLuaCommand1s( const char* command );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	//void freyjaLuaScript1s( const char* filename );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

} // extern "C"

#endif // GUARD__FREYJA_LUAABI_H_
