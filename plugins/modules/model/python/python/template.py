
#======================================================================
#  
#  Project : libfreyja
#  Author  : Terry 'Mongoose' Hendrix II
#  Website : http://icculus.org/~mongoose
#  Email   : mongoose@icculus.org
#  Object  : 
#  Comments: This is the template plugin for the python system.
#  
# -- History ---------------------------------------------------------- 
# 
#  2004.12.09:
#  Mongoose: Created
#======================================================================

import FreyjaModeler
from FreyjaModeler import *

import string
#import math
import types
import re

def InitPlugin(args):
	print "InitPlugin() called"
	print "Meshes %i" % freyjaGetMeshCount()
	return True


def ExportModel(filename):
	print "ExportModel() called"
	return False


def ImportModel(filename):
	print "ImportModel() called"

	try:
		f = open(filename, "r")

	except IOError:
		print "ERROR opening file"
		return False

	f.close()

	print "Opened file " + filename

	return True


def UnitTest(args):
	mesh = freyjaMeshCreate()
	v = []
	v.append(freyjaMeshVertexCreate3fv(mesh, 0, 0, 0))
	v.append(freyjaMeshVertexCreate3fv(mesh, 10, 0, 0))
	v.append(freyjaMeshVertexCreate3fv(mesh, 10, 10, 0))
	v.append(freyjaMeshVertexCreate3fv(mesh, 0, 10, 10))

	face = freyjaMeshPolygonCreate(mesh)

	for i in v:
		freyjaMeshPolygonAddVertex1i(mesh, face, v[i])

	return True



def PluginEntry(symbol, args):

	print symbol + " " + args

	if re.match('.*ImportModel', symbol):
		ImportModel(args)

	elif re.match('.*ExportModel', symbol):
		ExportModel(args)

	elif re.match('.*InitPlugin', symbol):
		InitPlugin(args)
	
	elif re.match('.*UnitTest', symbol):
		UnitTest(args)


PluginEntry(FreyjaSymbol, FreyjaArgs)



