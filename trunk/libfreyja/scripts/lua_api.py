#!/usr/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
# 
# Project : freyja
# Author  : Terry 'Mongoose' Hendrix II
# Website : http://icculus.org/freyja/
# Email   : mongooseichiban@gmail.com
# Object  : 
# Comments: This is the binding generator for Lua.
#
# -- Notes ------------------------------------------------------------
#
# Enabling python support in libfreyja:
#
#  Set gForcePythonDefine = 1 for force compiliation of Python 
#  support, or add LUAWRAPPER_FOUND to compile flags in Makefile.
#
#  Then (re)build libfreyja.
#
# -- History ---------------------------------------------------------- 
#
# 2006.12.09:
# Mongoose: Created
# ======================================================================

import os
import re
import string
import time

gDateStamp = time.localtime(time.time())
gForcePythonDefine = 0
gPath = "./freyja"
gLuaHeaders = ""
#\n\n#include <lua/Lua.h>\n\n Lua gLuaVM;"

gFuncWrappers = []
gFuncBindings = []

def PrintOutFreyaQueryBinding():
	print "// Query hooks"
	print "extern \"C\" {	"
	print "	index_t freyjaQueryBegin();"
	print "	void freyjaQueryEnd(index_t query);"
	print "	void freyjaQueryInteger(index_t query, const char *symbol, int32 *i);"
	print "	void freyjaQueryFloat(index_t query, const char *symbol, vec_t *r);"
	print "	void freyjaQueryString(index_t query, const char *symbol, const char **s);"
	print "}"
	print ""
	print ""
	print "int lua_freyjaQueryInteger(lua_State *L)"
	print "{"
	print "	int stack_count = lua_gettop(L);"
	print "	if ( stack_count < 1 )"
	print "		return 0;"
	print ""
	print "	const char *symbol = lua_tostring(L, 1);"
	print ""
	print "	int integer_value;"
	print "	index_t q = freyjaQueryBegin();"
	print "	freyjaQueryInteger(q, symbol, &integer_value);"
	print "	freyjaQueryEnd(q);"
	print ""
	print "	lua_pushnumber(L, integer_value);"
	print "	return 1;"
	print "}"
	print ""
	print ""
	print "int lua_freyjaQueryFloat(lua_State *L)"
	print "{"
	print "	int stack_count = lua_gettop(L);"
	print "	if ( stack_count < 1 )"
	print "		return 0;"
	print ""
	print "	const char *symbol = lua_tostring(L, 1);"
	print ""
	print "	float real_value;"
	print "	index_t q = freyjaQueryBegin();"
	print "	freyjaQueryFloat(q, symbol, &real_value);"
	print "	freyjaQueryEnd(q);"
	print ""
	print "	lua_pushnumber(L, real_value);"
	print "	return 1;"
	print "}"
	print ""
	print ""
	print "int lua_freyjaQueryString(lua_State *L)"
	print "{"
	print "	int stack_count = lua_gettop(L);"
	print "	if ( stack_count < 1 )"
	print "		return 0;"
	print ""
	print "	const char *symbol = lua_tostring(L, 1);"
	print ""
	print "	const char *string_value;"
	print "	index_t q = freyjaQueryBegin();"
	print "	freyjaQueryString(q, symbol, &string_value);"
	print "	freyjaQueryEnd(q);"
	print ""
	print "	lua_pushstring(L, string_value);"
	print "	return 1;"
	print "}"
	print ""
	print ""
	print ""
	print "int lua_freyjaPrintMessage(lua_State *L)"
	print "{"
	print "	int stack_count = lua_gettop(L);"
	print "	if ( stack_count < 1 )"
	print "		return 0;"
	print ""
	print "	const char *msg = lua_tostring(L, 1);"
	print "	freyjaPrintMessage(msg);"
	print "	return 0;"
	print "}"
	print ""
	print ""


def PrintOutFreyjaLuaABI_old():
	print "void freyjaLuaCommand1s(const char *s)"
	print "{"
	print "#ifdef LUAWRAPPER_FOUND"
	print "\tFreyja_BindLua();"
	print "\t"
	print "\tif (s && s[0])\n\t\tgLuaVM.ExecuteCommandSilently(s);"
	print "\t"
	print "#else"
	print "\tfreyjaPrintError(\"[Module '%s' failed to load.  Rebuild with Lua support.]\", plugin);"
	print "#endif // LUAWRAPPER_FOUND"
	print "}"
	print "\n"
	print "\n"
	print "void freyjaLuaScript1s(const char *s)"
	print "{"
	print "#ifdef LUAWRAPPER_FOUND"
	print "\tFreyja_BindLua();"
	print "\t"
	print "\tif (s && s[0])\n\t\tgLuaVM.ExecuteFile(s);"
	print "\t"
	print "#else"
	print "\tfreyjaPrintError(\"[Module '%s' failed to load.  Rebuild with Lua support.]\", plugin);"
	print "#endif // LUAWRAPPER_FOUND"
	print "}"
	print "\n"


def PrintOutFreyjaLuaABI():
	print ""


def PrintOutFreyjaBindLua_Old():
	print "void Freyja_BindLua()"
	print "{"
	print "\tstatic bool init = false;"
	print "\t"
	print "\tif (init)\n\t\treturn;\n"
	print "\tinit = true;"
	print "\t"
	print "\tlua_freyja_bind_t *cur;"
	print "\tunsigned int i = 0;"
	print "\t"
	print "\twhile ( (cur = &gLibFreyja_LuaBinds[i++]) && cur->symbol )"
	print "\t{"
	print "\t\tgLuaVM.RegisterFunction(cur->symbol, cur->func);"
	print "\t}"
	print "\t"
	print "\t// FIXME: Bind some useful gobals here too."
	print "}"
	print "\n"

def PrintOutFreyjaBindLua():
	print "#ifdef LUAWRAPPER_FOUND"
	print "void freyja_lua_register_functions( lua_State* state )"
	print "{"
	print "\tstatic bool init = false;"
	print "\t"
	print "\tif ( !init )"
	print "\t{"
	print "\t\tinit = true;"
	print "\t\tlua_freyja_bind_t *cur;"
	print "\t\tunsigned int i = 0;"	
	print "\t\t"
	print "\t\twhile ( (cur = &gLibFreyja_LuaBinds[i++]) && cur->symbol )"
	print "\t\t{"
	print "\t\t\tlua_register( state, cur->symbol, cur->func );"
	print "\t\t}"
	print "\t}"	
	print "}"
	print "#endif // LUAWRAPPER_FOUND"


def StoreWrapperFunction( abi, li ):
	count = nameat = 0

	for i in li:
		count = count + 1
		if re.match(abi, i):
			name = i
			nameat = count
			break

	pass_vars = ""
	
	s = "int lua_" + name + "(lua_State *L)\n{\n"

	lua_stack = ""
	lua_stack_count = 0

	unsupported = False	

	# Function blacklist, these are mostly for deprecated functions
	#if name == "freyjaGetBoneName":
	#	unsupported = True
	#elif name == "freyjaMeshCreateLattice":
	#	unsupported = True
	#elif name == "freyjaLuaBindFunc":
	#	unsupported = True


	for i in range(nameat, len(li)):
		# Strings
		if re.match('(char *|const char *|const char*|char*)', li[i]):
			identifier = re.sub(';', '', li[i+1])
			pass_vars += identifier + ", "
			lua_stack_count += 1
			lua_stack += "\tconst char* " + identifier  + " = lua_tostring(L, " + `lua_stack_count` + ");\n"
		elif re.match('.*const.*', li[i]):
			tmp = ""
		#elif re.match('.*;', li[i]):
		#	tmp = re.sub(';.*', ';\n', li[i])
		else:
			# Integers
			if re.match('(index_t|int32|char|uint32|byte|int16|uint16|freyja_id)', li[i]):
				identifier = re.sub(';', '', li[i+1])

				if re.match('(.*\\*)', li[i]):
					unsupported = True	

				pass_vars += identifier + ", "
				lua_stack_count += 1
				lua_stack += "\t" + li[i] + " " + identifier  + " = (int)lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Enumerations
			elif re.match('(freyja_transform_action_t|freyja_pixelformat_t)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack += "\t" + li[i] + " " + identifier  + " = ("+li[i]+")(int)lua_tonumber(L, " + `lua_stack_count` + ");\n"
				lua_stack_count += 1

			# Floating point numbers
			elif re.match('(vec_t|float)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack_count += 1
				lua_stack += "\t" + li[i] + " " + identifier  + " = lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Vec2
			elif re.match('(vec2_t)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack += "\t" + li[i] + " " + identifier  + ";\n"
				lua_stack_count += 1
				lua_stack += "\t" + identifier + "[0] = lua_tonumber(L, " + `lua_stack_count` + ");\n"
				lua_stack_count += 1
				lua_stack += "\t" + identifier + "[1] = lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Vec3
			elif re.match('(vec3_t)', li[i]):
				identifier = re.sub(';', '', li[i+1])

				if identifier == "":
					unsupported = True	

				pass_vars += identifier + ", "
				lua_stack += "\t" + li[i] + " " + identifier  + ";\n"

				for n in range(0, 3):
					lua_stack_count += 1
					lua_stack += "\t" + identifier + "["+`n`+"] = lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Vec4
			elif re.match('(vec4_t)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack += "\t" + li[i] + " " + identifier  + ";\n"

				for n in range(0, 4):
					lua_stack_count += 1
					lua_stack += "\t" + identifier + "["+`n`+"] = lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Matrix
			elif re.match('(matrix_t)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack += "\t" + li[i] + " " + identifier  + ";\n"

				for n in range(0, 16):
					lua_stack_count += 1
					lua_stack += "\t" + identifier + "["+`n`+"] = lua_tonumber(L, " + `lua_stack_count` + ");\n"

			# Userdata
			elif re.match('(freyja_ptr)', li[i]):
				identifier = re.sub(';', '', li[i+1])
				pass_vars += identifier + ", "
				lua_stack_count += 1
				lua_stack += "\t" + li[i] + " " + identifier + " = lua_touserdata(L, " + `lua_stack_count` + ");\n"

			else:
				identifier = li[i]



	#if re.match('.*vec_t\\*', li[0]):
	#	unsupported = True

	if unsupported:
		s += "#warning \"Unsupported function binding.\"\n"
		s += "\tfreyjaPrintError(\"Invalid binding %s(), %s:%i\", __func__, __FILE__, __LINE__);\n"
		s += "#if FIXME\n\t/* No binding support for this type of method. */\n"

	# Pop values off the lua stack
	if lua_stack_count:
		# Trim extra ',' off passed parameters.
		pass_vars = re.sub(',.$', '', pass_vars)

		# Generate stack access routine.
		s += "\tint stack_count = lua_gettop(L);\n"
		s += "\tif ( stack_count < " + `lua_stack_count` + " )\n\t\treturn 0;\n\n"
		s += lua_stack

	# Returns string
	if re.match('(.*char\\*)', li[0]) or re.match('(.*char\\*)', li[1]):
		s += "\tconst char *string_value = " + name + "(" + pass_vars +");\n"
		s += "\tlua_pushstring(L, string_value);\n"
		s += "\treturn 1;"

	# Returns integer
	elif re.match('(index_t|int32|uint32|byte|char|uint16|int16|freyja_id)', li[0]):
		s += "\tint integer_value = " + name + "(" + pass_vars +");\n"
		s += "\tlua_pushnumber(L, integer_value);\n"
		s += "\treturn 1;"

	# Returns float
	elif re.match('(vec_t|float)', li[0]):
		s += "\tvec_t real_value = " + name + "(" + pass_vars +");\n"
		s += "\tlua_pushnumber(L, real_value);\n"
		s += "\treturn 1;"

	# Returns userdata
	elif re.match('(freyja_ptr)', li[0]):
		s += "\tfreyja_ptr userdata_value = " + name + "(" + pass_vars +");\n"
		s += "\tlua_pushlightuserdata(L, userdata_value);\n"
		s += "\treturn 1;"

	# Returns void
	else:
		s += "\t" + name + "(" + pass_vars + ");\n"
		s += "\treturn 0;"

	if unsupported:
		s += "\n#endif // FIXME\n\n\treturn 0;\n}\n\n"
	else:
		s += "\n}\n\n"

	gFuncWrappers.append(s)



def StoreBindFunction(name):
	s = "\t{ " + '"' + name + '"' + ", lua_" + name + " },"
	gFuncBindings.append(s)


# Yes, this is lame -- however the python file iterator has issues with
# multiline statement parsing on my machine even using f.next().
def BufferFunctionDeclaration(f):
	buf = ""
	while True:
		c = f.read(1)
		buf += c
		if c == '' or c == ';':
			break;

	# Strip comments //, newlines, and directives
	buf = re.sub('(//.*\n|\n|#.*\n)', '', buf)
	
	# Strip comments /* */
	buf = re.sub('\/\\*.*\\*\/', '', buf)

	# Strip out references
	# Should not have references anywhere in C ABI  :3  
	#buf = re.sub('&', '', buf)

	# Dump parsed 'lines' into generated file for debugging
	#print "/* $$$ " + buf + " $$$ */"

	return buf


def ImportBindings(filepath, basename):
	filename = filepath + "/" + basename

	# Uber forceful abi binding enforcement
    # Only ABI functions in the proper header will recieve a python binding
	abi = re.sub('ABI.h', '', basename)
	#accessor = "freyjaGet" + abi
	mutator = "freyja" + abi
	
	try:
		f = open(filename, "r")

	except IOError:
		print "ERROR opening file '" + filename + "'" 
		return False

	over = ""

	#for i in f:
	while True:
		i = BufferFunctionDeclaration(f)

		if i == "":
			break

		if re.match('^.*' + mutator + '.*\\(', i):
			# Don't expose C++ ABI to python
			if not re.match('.*::', i):
				p = re.sub('.\\*', '* ', i)
				p = re.sub(',', ';\n', p)
				p = re.sub('(\\(|\\)|\n)', ' ', p)
				li = p.split()
				StoreWrapperFunction(mutator, li)

				s = re.sub('\\(.*', '', i)
				s = re.sub('.* ', '', s)
				s = re.sub(' ', '', s)
				s = re.sub('\\*', '', s)
				s = re.sub('\n', '', s)
				StoreBindFunction(s)
				over = ""

		#elif not re.match('.*freyja.*\\(.*\\);', i):
		#	i = ""

		#elif re.match('^.*' + accessor + '.*\\(', i):
			# Don't expose C++ ABI to python
		#	if not re.match('.*::', i):
		#		p = re.sub('.\\*', '* ', i)
		#		p = re.sub(',', ';\n', p)
		#		p = re.sub('(\\(|\\)|\n)', ' ', p)
		#		li = p.split()
		#		StoreWrapperFunction(li)

		#		s = re.sub('\\(.*', '', i)
		#		s = re.sub('.* ', '', s)
		#		s = re.sub(' ', '', s)
		#		s = re.sub('(\\*|\n)', '', s)
		#		StoreBindFunction(s)
		#		over = ""


	f.close()

	return True



def UpdateBindings():
	li = os.listdir(gPath)

	print "/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */"
	print "/*==========================================================================="
	print " * "
	print " * Project : freyja"
	print " * Author  : Terry 'Mongoose' Hendrix II"
	print " * Website : http://www.icculus.org/freyja/"
	print " * Email   : mongooseichiban@gmail.com"
	print " * Object  : "
	print " * License : No use w/o permission (C) 2004-2007 Mongoose"
	print " * Comments: This is the Lua plugin ABI."
	print " *"
	print " *           This file was generated by Mongoose's UpdateBindings-Lua.py" 
 	print " *           - Mongoose <mongooseichiban@gmail.com>"
	print " *"
	print " * %i.%i.%i:" % (gDateStamp[0], gDateStamp[1], gDateStamp[2])
	print " * Mongoose - Generated, Based on C ABI for freyja"
	print " ==========================================================================*/"
	print ""

	if gForcePythonDefine == 1:
		print "#define LUAWRAPPER_FOUND"

	for i in li:
		if re.match('.*ABI.h', i) and not re.match('(Plugin|Legacy|QueryABI|ControlPoint|LuaABI|PythonABI|.*~)', i):
			print "#include \"" + i + '"'
			sucess = ImportBindings(gPath, i)

	print "#include \"freyja.h\""
	print "#include \"LuaABI.h\""
	print "#ifdef LUAWRAPPER_FOUND"
	print gLuaHeaders
	print "\n"

	for i in gFuncWrappers:
		print i

	PrintOutFreyaQueryBinding()

	print "typedef struct {\n\tconst char *symbol;\n\tint (*func)(lua_State*);\n} lua_freyja_bind_t;\n\n"
	print "lua_freyja_bind_t gLibFreyja_LuaBinds[] = {"
	for i in gFuncBindings:
		print i
	print "\t{ \"freyjaQueryInteger\", lua_freyjaQueryInteger },"
	print "\t{ NULL, NULL }\n};\n\n"

	PrintOutFreyjaBindLua()

	PrintOutFreyjaLuaABI()

	print "#endif // LUAWRAPPER_FOUND"


if __name__ == "__main__":
	UpdateBindings()

