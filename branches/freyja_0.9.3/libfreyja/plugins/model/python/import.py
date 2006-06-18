'''
 ======================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the template plugin for the python system.
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
#import math
import types

def importmodel(filename):

	setImportValid(-1)

	try:
		f = open(filename, "r")

	except IOError:
		print "ERROR opening file"
		return -1

	# Check and see if the passed model is the type supported here
	# via magic/header checking or some other means

	f.close()

	# Now that you loaded your model file you must import it

	#vertices []
	#texcoords []
	#
	#freyjaBegin(FREYJA_MODEL)
	#freyjaBegin(FREYJA_MESH)
	#freyjaBegin(FREYJA_VERTEX_GROUP)
	#vertices.append(freyjaVertex3f(0.0, sz, 0.0))
	#...
	#freyjaEnd()
	#
	#texcoords.append(freyjaTexCoord2f(0.0, 0.0))
	#...
	#
	#for i in range(0, len(polygons)):
	#	freyjaBegin(FREYJA_POLYGON);
	#	freyjaPolygonVertex1i(texcoords[polygons[i].uvIndex0]);
	#	freyjaPolygonVertex1i(vertices[polygons[i].xyzIndex0]);
	#	...
	#	freyjaPolygonMaterial1i(0);
	#	freyjaEnd() # FREYJA_POLYGON
	#
	#
	#freyjaEnd() # FREYJA_MESH
	#freyjaEnd() # FREYJA_MODEL

	setImportValid(-1)  # Set 0 if you sucessfully load a model

	return 0


importmodel(FreyjaImportFilename)

