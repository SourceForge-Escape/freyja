#!/usr/bin/python
# -*- coding: utf-8 -*-
# ======================================================================
# 
# Project : freyja
# Author  : Terry 'Mongoose' Hendrix II
# Website : http://icculus.org/freyja/
# Email   : mongooseichiban@gmail.com
# Object  : 
# Comments: This is the binding generator for the python system.
#
# -- Notes ------------------------------------------------------------
#
# Enabling python support in libfreyja:
#
#  Set gForcePythonDefine = 1 for force compiliation of Python 
#  support, or add USING_PYTHON to compile flags in Makefile.
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
gPythonHeader = "python2.4/Python.h"
gFuncWrappers = []
gFuncBindings = []


def PrintOutFreyjaPythonABI():
	print "void freyjaPython1s(const char *plugin, const char *symbol, const char *s)"
	print "{"
	print "\tPyObject *module, *dict, *tmp;"
	print ""
	print "\t/* Start up python */"
	print "\tPy_Initialize();"
	print "\tmodule = Freyja_Plugin_Gobals();"
	print "\tdict = PyModule_GetDict(module);"
	print ""
	print "\t/* Append gobal constants, pass <s> as <symbol> in python */"
	print "\ttmp = PyString_FromFormat(s);"
	print "\tPyDict_SetItemString(dict, symbol, tmp);"
	print "\tPy_DECREF(tmp);"
	print ""
	print "\tfreyjaPrintMessage(\"[Module '%s' opened.]\", plugin);"
	print "\tFILE *f = fopen(plugin, \"r\");"
	print ""
	print "\tif (!f)"
	print "\t{"
	print "\t\tfreyjaPrintError(\"[Module '%s' failed to load.]\", plugin);"
	print "\t\tperror(plugin);"
	print "\t}"
	print "\telse"
	print "\t{"
	print "\t\tPyRun_SimpleFile(f, plugin);"
	print "\t\tPy_Finalize();"
	print "\t}"
	print "}"
	print "\n"


def PrintOutFreyjaPythonGobals():
	print "PyObject *Freyja_Plugin_Gobals()"
	print "{"
	print "\tPyObject *module, *dict, *tmp;"
	print ""
	print "\tmodule = Py_InitModule(\"FreyjaModeler\", Plugin_methods);"
	print "\tdict = PyModule_GetDict(module);"
	print "\t"
	print "\t/* Append gobal constants you want python to see */"
	print "\ttmp = PyInt_FromLong(FREYJA_MODEL);"
	print "\tPyDict_SetItemString(dict, \"FREYJA_MODEL\", tmp);"
	print "\tPy_DECREF(tmp);"
	print "\t"
	print "\treturn module;"
	print "}"
	print "\n"


def StoreWrapperFunction(li):
	count = nameat = 0

	for i in li:
		count = count + 1
		if re.match('.*freyja', i):
			name = i
			nameat = count
			break

	func_vars = ""
	parse_types = ""
	parse_vars = ""
	pass_vars = ""
	
	s = "PyObject *py_" + name + "(PyObject *self, PyObject *args)\n{\n"

	for i in range(nameat, len(li)):
		if re.match('.*const.*', li[i]):
			tmp = ""
		elif re.match('.*;', li[i]):
			tmp = re.sub(';.*', ';\n', li[i])
			func_vars += tmp
		else:
			if re.match('(.*char\\*)', li[i]):
				parse_types += "s"
				pass_vars += li[i+1] + ", "
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(index_t|int32|char|uint32|byte)', li[i]):
				parse_types += "i"
				pass_vars += li[i+1] + ", "
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(freyja_transform_action_t)', li[i]):
				parse_types += "i"
				pass_vars += li[i+1] + ", "
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec_t|float)', li[i]):
				parse_types += "f"
				pass_vars += li[i+1] + ", "
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec2_t)', li[i]):
				parse_types += "ff"
				pass_vars += li[i+1] + ", "
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec3_t)', li[i]):
				parse_types += "fff"
				pass_vars += li[i+1] + ", "
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, " + li[i+1] + "+2, "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec4_t)', li[i]):
				parse_types += "ffff"
				pass_vars += li[i+1] + ", "
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, " + li[i+1] + "+2, " + li[i+1] + "+3, "
				func_vars += "\t" + li[i] + " "
			elif re.match('(matrix_t)', li[i]):
				parse_types += "ffffffffffffffff"
				pass_vars += li[i+1] + ", "
				for n in range(0, 15):
					parse_vars += li[i+1] + "+" + "%i" % n + ", " 
				func_vars += "\t" + li[i] + " "
			else:
				func_vars += li[i]

	unsupported = False	

	if re.match('.*vec_t\\*', li[0]):
		unsupported = True

	if len(li) > 3 and not parse_types == "":
		# List dumped into code
		#for i in li:
		#	s += "\t/* " + i + " */\n"
		#s += "\n\n"

		pass_vars = re.sub(';', '', pass_vars)
		pass_vars = re.sub(',.$', '', pass_vars)
		parse_vars = re.sub(';', '', parse_vars)
		parse_vars = re.sub(',.$', '', parse_vars)
		s += func_vars

		s += "\n\tif (!PyArg_ParseTuple(args, "+ '"' + parse_types + '"' + ", " + parse_vars + "))\n" 
		s += "\t\treturn NULL;\n\n"

	if unsupported:
		s += "#if FIXME\n\t/* No support for returning pointers yet... */\n"


	if re.match('(.*char\\*)', li[0]) or re.match('(.*char\\*)', li[1]):
		# Returns string
		s += "\treturn PyString_FromString(" + name + "(" + pass_vars +"));"
	elif re.match('(index_t|int32|uint32|byte|char)', li[0]):
		# Returns integer
		s += "\treturn PyInt_FromLong(" + name + "(" + pass_vars +"));"
	elif re.match('(vec_t|float)', li[0]):
		# Returns float
		s += "\treturn PyFloat_FromDouble(" + name + "(" + pass_vars +"));"
	else:
		# Returns void
		s += "\t" + name + "(" + pass_vars + ");\n"
		s += "\treturn PyInt_FromLong(0);"

	if unsupported:
		s += "\n#endif // FIXME\n\n\treturn NULL;\n}\n\n"
	else:
		s += "\n}\n\n"

	gFuncWrappers.append(s)



def StoreBindFunction(name):
	s = "\t{ " + '"' + name + '"' + ", py_" + name + ", METH_VARARGS },"
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
	buf = re.sub('&', '', buf)

	# Dump parsed 'lines' into generated file for debugging
	#print "/* $$$ " + buf + " $$$ */"

	return buf


def ImportBindings(filepath, basename):
	filename = filepath + "/" + basename

	# Uber forceful abi binding enforcement
    # Only ABI functions in the proper header will recieve a python binding
	abi = re.sub('ABI.h', '', basename)
	accessor = "freyjaGet" + abi
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

		if not re.match('.*freyja.*\\(.*\\);', i):
			i = ""

		elif re.match('^.*' + accessor + '.*\\(', i):
			# Don't expose C++ ABI to python
			if not re.match('.*::', i):
				p = re.sub('.\\*', '* ', i)
				p = re.sub(',', ';\n', p)
				p = re.sub('(\\(|\\)|\n)', ' ', p)
				li = p.split()
				StoreWrapperFunction(li)

				s = re.sub('\\(.*', '', i)
				s = re.sub('.* ', '', s)
				s = re.sub(' ', '', s)
				s = re.sub('(\\*|\n)', '', s)
				StoreBindFunction(s)
				over = ""

		elif re.match('^.*' + mutator + '.*\\(', i):
			# Don't expose C++ ABI to python
			if not re.match('.*::', i):
				p = re.sub('.\\*', '* ', i)
				p = re.sub(',', ';\n', p)
				p = re.sub('(\\(|\\)|\n)', ' ', p)
				li = p.split()
				StoreWrapperFunction(li)

				s = re.sub('\\(.*', '', i)
				s = re.sub('.* ', '', s)
				s = re.sub(' ', '', s)
				s = re.sub('\\*', '', s)
				s = re.sub('\n', '', s)
				StoreBindFunction(s)
				over = ""

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
	print " * License : No use w/o permission (C) 2004-2006 Mongoose"
	print " * Comments: This is the Python plugin ABI."
	print " *"
	print " *           This file was generated by Mongoose's UpdateBindings.py" 
 	print " *           - Mongoose <mongooseichiban@gmail.com>"
	print " *"
	print " * %i.%i.%i:" % (gDateStamp[0], gDateStamp[1], gDateStamp[2])
	print " * Mongoose - Generated, Based on C ABI for freyja"
	print " ==========================================================================*/"
	print ""

	if gForcePythonDefine == 1:
		print "#define USING_PYTHON"
	print "#ifdef USING_PYTHON"
	print "#include <" + gPythonHeader + ">"

	for i in li:
		if re.match('.*ABI.h', i) and not re.match('(Legacy|.*~)', i):
			print "#include \"" + i + '"'
			sucess = ImportBindings(gPath, i)

	print "#include \"freyja.h\""
	print "\n"

	for i in gFuncWrappers:
		print i

	print "PyMethodDef Plugin_methods[] = {"
	for i in gFuncBindings:
		print i
	print "\t{ NULL }\n};\n\n"

	PrintOutFreyjaPythonGobals()

	PrintOutFreyjaPythonABI()

	print "#endif // USING_PYTHON"


if __name__ == "__main__":
	UpdateBindings()

