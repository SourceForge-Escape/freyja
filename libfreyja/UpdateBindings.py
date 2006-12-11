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
# -- History ---------------------------------------------------------- 
#
# 2006.12.09:
# Mongoose: Created
# ======================================================================

import os
import re
import string


def PrintOutFreyjaPythonABI():
	print "int freyjaPython1s(const char *plugin, const char *symbol, const char *s)"
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
	print "\tPyRun_SimpleFile(f, plugin);"
	print "\tPy_Finalize();"
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


gFuncWrappers = []

def StoreWrapperFunction(li):
	s = "PyObject *py_" + li[1] + "(PyObject *self, PyObject *args)\n{\n"

	count = nameat = 0

	for i in li:
		count = count + 1
		if re.match('.*freyja.*', i):
			name = i
			nameat = count

	func_vars = ""
	parse_types = ""
	parse_vars = ""

	# List dumped into code
	#for i in li:
	#	s += "\t/* " + i + " */\n"
	#s += "\n\n"
	
	for i in range(nameat, len(li)):
		if re.match('.*const.*', li[i]):
			tmp = ""
		elif re.match('.*;', li[i]):
			tmp = re.sub(';.*', ';\n', li[i])
			func_vars += tmp
		else:
			if re.match('(index_t|char|uint32|byte)', li[i]):
				parse_types += "i"
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(.*char\\*)', li[i]):
				parse_types += "s"
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec_t)', li[i]):
				parse_types += "f"
				parse_vars += "&" + li[i+1] + ", "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec2_t)', li[i]):
				parse_types += "ff"
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec3_t)', li[i]):
				parse_types += "fff"
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, " + li[i+1] + "+2, "
				func_vars += "\t" + li[i] + " "
			elif re.match('(vec4_t)', li[i]):
				parse_types += "ffff"
				parse_vars += li[i+1] + "+0, " +  li[i+1] + "+1, " + li[i+1] + "+2, " + li[i+1] + "+3, "
				func_vars += "\t" + li[i] + " "
			else:
				func_vars += li[i]

	if len(li) > 3:
		parse_vars = re.sub(';', '', parse_vars)
		parse_vars = re.sub(',.$', '', parse_vars)
		s += func_vars
		s += "\n\tif (!PyArg_ParseTuple(args, "+ '"' + parse_types + '"' + ", " + parse_vars + "))\n" 
		s += "\t\treturn NULL;\n\n"

	if re.match('(index_t|uint32|byte|char)', li[0]):
		# Returns integer
		s += "\treturn PyInt_FromLong(" + name + "(" + parse_vars +"));"
	else:
		# Returns void
		s += "\t" + name + "(" + parse_vars + ");\n"
		s += "\treturn PyInt_FromLong(0);"

	s += "\n}\n\n"

	gFuncWrappers.append(s)



def PrintBindFunction(name):
	print "\t{ " + '"' + name + '"' + ", py_" + name + ", METH_VARARGS },"



def ImportBindings(filepath, basename):
	filename = filepath + "/" + basename

	# Uber forceful abi binding enforcement
    # If any ABI functions is in the proper header it won't get a python binding
	abi = re.sub('ABI.h', '', basename)
	accessor = "freyjaGet" + abi
	mutator = "freyja" + abi
	
	try:
		f = open(filename, "r")

	except IOError:
		print "ERROR opening file '" + filename + "'" 
		return 0

	for i in f:
		if re.match('^.*' + accessor + '.*', i):

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
			PrintBindFunction(s)
		elif re.match('^.*' + mutator + '.*', i):
			s = re.sub('\\(.*', '', i)
			s = re.sub('.* ', '', s)
			s = re.sub(' ', '', s)
			s = re.sub('\\*', '', s)
			s = re.sub('\n', '', s)
			PrintBindFunction(s)

	f.close()

	return 1


def UpdateBindings():

	p = "./freyja"
	li = os.listdir(p)

	print "#include <python2.4/Python.h>"
	print "#include \"freyja.h\""
	print "\n"
	print "PyMethodDef Plugin_methods[] = {"

	for i in li:
		if re.match('.*ABI.h', i):
			sucess = ImportBindings(p, i)

	print "\t{ NULL }\n};\n\n"

	for i in gFuncWrappers:
		print i

	PrintOutFreyjaPythonGobals()
	PrintOutFreyjaPythonABI()


if __name__ == "__main__":
	UpdateBindings()

