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

#ifndef GUARD__MIDGARD_LUA_H_
#define GUARD__MIDGARD_LUA_H_

extern "C" {
#include "lua5.1/lua.h"
#include "lua5.1/lualib.h"
#include "lua5.1/lauxlib.h"
}


class Lua
{
 public:

	typedef int (*FuncPtr)(lua_State *);


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Lua() : mState(NULL) { mState = lua_open(); luaL_openlibs(mState); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs Lua script interface.
	 *
	 ------------------------------------------------------*/

	~Lua() { lua_close(mState); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int GetGobal(const char *symbol, bool &err)
	{
		err = true;

		/* Push the Lua global variable's symbol onto the stack */
		lua_getglobal(mState, symbol);

		/* Check for numeric type. The -1 indicates the
		 * first stack position from the top of the stack. */
		if ( !lua_isnumber(mState, -1) )
		   return 0;

		err = false;
		
		/* Pop the value off the stack. */
		return (int)lua_tonumber(mState, -1);
	}

	int PopInteger()
	{
		lua_gettable(mState, LUA_GLOBALSINDEX);
		if ( !lua_isnumber(mState, -1) )
			return 0;
		return (int)lua_tonumber(mState, -1);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool QueryInteger(const char *symbol, int &i)
	{
		lua_getglobal(mState, symbol);
		//lua_gettable(mState, LUA_GLOBALSINDEX);
		if ( !lua_isnumber(mState, -1) )
			return false;
		i = (int)lua_tonumber(mState, -1);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetGobal(const char *symbol)
	{
		lua_getglobal(mState, symbol);
		lua_gettable(mState, LUA_GLOBALSINDEX);
		return lua_tostring(mState, -1);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

    bool ExecuteCommand(const char *command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Execute lua script.
	 *
	 ------------------------------------------------------*/

	bool ExecuteCommandSilently(const char *command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Execute lua script.  Doesn't log to console.
	 *
	 ------------------------------------------------------*/

    bool ExecuteFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Execute lua script.
	 *
	 ------------------------------------------------------*/

	void RegisterFunction(const char *symbol, Lua::FuncPtr func)
	{ lua_register(mState, symbol, func); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Export C function to lua.
	 *
	 ------------------------------------------------------*/

	bool Run(const char *s) { return ExecuteCommandSilently(s); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Run command.
	 *
	 ------------------------------------------------------*/


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	lua_State *mState;          /* State handle */
};

#endif

