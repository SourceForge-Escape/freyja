#======================================================================
# 
# Project : libfreyja
# Author  : Terry 'Mongoose' Hendrix II
# Website : http://icculus.org/~mongoose
# Email   : mongoose@icculus.org
# Object  : 
# Comments: This is the demo plugin for the python system.
# 
#-- History ---------------------------------------------------------- 
#
# 2006.12.12:
# Mongoose, Updated for new 0.9.5 ABI
#
# 2004.12.09:
# Mongoose, Created
#======================================================================

import FreyjaModeler
from FreyjaModeler import *

import string
import math
import types
import re


def GenerateCube(sz):

	mesh = freyjaMeshCreate()

	vertices = []
	vertices.append(freyjaMeshVertexCreate3fv(mesh, 0.0, sz, 0.0))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, sz, sz, 0.0))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, sz, 0.0, 0.0))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, 0.0, 0.0, 0.0))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, 0.0, sz, sz))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, sz, sz, sz))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, sz, 0.0, sz))
	vertices.append(freyjaMeshVertexCreate3fv(mesh, 0.0, 0.0, sz))

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	face = freyjaMeshPolygonCreate(mesh)
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6])
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7])
	freyjaMeshPolygonMaterial(mesh, face, 0)

	return True



def PluginEntry(symbol, args):

	print symbol + " " + args

	if re.match('.*GenerateCube', symbol):
		GenerateCube(9.0)

#	if re.match('.*ImportModel', symbol):
#		ImportModel(args)
#
#	elif re.match('.*ExportModel', symbol):
#		ExportModel(args)
#
#	elif re.match('.*InitPlugin', symbol):
#		InitPlugin(args)
#	
#	elif re.match('.*UnitTest', symbol):
#		UnitTest(args)


PluginEntry(FreyjaSymbol, FreyjaArgs)

