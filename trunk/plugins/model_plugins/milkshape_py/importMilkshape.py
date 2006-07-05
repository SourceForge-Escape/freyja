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

## ARGH my python is seriously broke on my system 
## I can't import math/struct/array/etc so fuck this for now

import FreyjaModeler
from FreyjaModeler import *

import string
#import math
import types
#import struct
#from struct import *
import array
#from array import *
import sys


class FileReader(file):
	def read(self, f, fmt):
		b = f.read(struct.calcsize(fmt)) # check length, etc
		return struct.unpack(fmt, b)


class Ms3dVertex:
	flags = 0            # byte, SELECTED | SELECTED2 | HIDDEN
	vertices = array('f')# float[3]
	boneId = 0           # char, -1 = no bone
	referenceCount = 0   # byte
	def read(self, f):
		r = FileReader()
		self.flags = f.read(1)
		self.vertices = r.read(f, 'f')
		self.boneId = f.read(1)
		self.referenceCount = f.read(1)
		print 


class Ms3dmodel:
	hid = "MS3D000000"
	hversion = 3
	#Ms3dVertex vertices []

	def read(self, f):
		r = FileReader()
		hid = f.read(10)
		hversion = r.read(f, 'l')

		print hid, hversion

		if hid != "MS3D000000" or hversion != 3:
			return -1

		return -1 # FIXME




def importmodel(filename):
	mdl = Ms3dmodel()
	MAX_VERTICES  =  8192
	MAX_TRIANGLES =  16384
	MAX_GROUPS    =  128
	MAX_MATERIALS =  128
	MAX_JOINTS    =  128
	MAX_KEYFRAMES =  216     # increase when needed

	# Flags
	SELECTED       = 1
	HIDDEN         = 2
	SELECTED2      = 4
	DIRTY          = 8

	setImportValid(-1)


	try:
		f = open(filename, "rb")

	except IOError:
		print "\tERROR opening file"
		return -1

	print "\tOpened", filename

	if mdl.read(f) != 0:
		return -1

	print "\t Sorry the plugin isn't finished yet"

	f.close()

	setImportValid(-1) # 0 if it actually loaded something

	return 0


importmodel(FreyjaImportFilename)

