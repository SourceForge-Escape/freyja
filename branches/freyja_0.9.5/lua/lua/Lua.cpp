/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose
 * Email   : mongooseichiban@gmail.com
 * Object  : Lua
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: This is the lua interface class.
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.05.29:
 * Mongoose - Created
 ==========================================================================*/

#include "Lua.h"

bool Lua::ExecuteCommandSilently(const char *command)
{
	if (!command || !command[0])
		return false;

	if ( luaL_dostring(mState, command) )
	{
		printf("-- Lua error %s\n", lua_tostring(mState, -1) );
		return false;
	}

	return true;
}


bool Lua::ExecuteCommand(const char *command)
{
	printf("$ %s\n", command);

	if (!command || !command[0])
		return false;

	if ( luaL_dostring(mState, command) )
	{
		printf("-- Lua error %s\n", lua_tostring(mState, -1) );
		return false;
	}

	return true;
}


bool Lua::ExecuteFile(const char *filename)
{
	printf("@ Lua::ExecuteFile '%s'...\n", filename);

	int s = -1;
    if ( luaL_loadfile(mState, filename) == 0 ) 
	{
		s = lua_pcall(mState, 0, LUA_MULTRET, 0);
	}

	return (s == 0);
}



