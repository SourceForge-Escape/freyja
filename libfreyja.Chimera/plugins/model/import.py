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

def importmodel(filename):

	try:
		f = open(filename, "r")

	except IOError:
		print "ERROR opening file"
		return -1

	print "Sorry the roundabout plugin loader isn't here yet"

	f.close()

	return -1


importmodel(FreyjaImportFilename)

