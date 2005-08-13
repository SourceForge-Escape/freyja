'''
 ======================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the demo plugin for the python system.
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2004.12.09:
 * Mongoose: Created
 ======================================================================
'''

import FreyjaModeler
from FreyjaModeler import *

import string
import math
import types

def generatecube():
	sz = 9.0
	vertices = []   # Trans maps for indices ( Egg builds still need this )
	texcoords = []

	freyjaBegin(FREYJA_MESH)

	freyjaBegin(FREYJA_GROUP)
	vertices.append(freyjaVertex3f(0.0, sz, 0.0))
	vertices.append(freyjaVertex3f(sz, sz, 0.0))
	vertices.append(freyjaVertex3f(sz, 0.0, 0.0))
	vertices.append(freyjaVertex3f(0.0, 0.0, 0.0))
	vertices.append(freyjaVertex3f(0.0, sz, sz))
	vertices.append(freyjaVertex3f(sz, sz, sz))
	vertices.append(freyjaVertex3f(sz, 0.0, sz))
	vertices.append(freyjaVertex3f(0.0, 0.0, sz))
	freyjaEnd() #FREYJA_GROUP

	#freyjaTexCoord2f(0.0, 0.0)

	#for i in range(0, len(vertices)):
	#	print vertices[i]

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd() # FREYJA_POLYGON

	freyjaEnd() # FREYJA_MESH


generatecube()



