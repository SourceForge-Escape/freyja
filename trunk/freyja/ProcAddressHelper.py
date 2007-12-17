#!/usr/bin/python
# -*- coding: utf-8 -*-
# Helper for OpenGL GetProcAddress

import sys
import string
import re


def ProcAddressHelper(name):
	proc = "PFN" + string.upper(name) + "PROC"
	print "////////////////////////////////////////////////////////////////"
	print ""
	print "extern\t" + proc + " h_" + name + ";"
	print "extern\tvoid *h_" + name + ";"
	print ""
	print proc + " h_" + name + " = NULL;"
	print "void *h_" + name + " = NULL;"
	print ""
	print "h_" + name + " = (" + proc +  ")mglGetProcAddress(\"" + name + "\");"
	print ""
	print "////////////////////////////////////////////////////////////////"
	return True


if __name__ == "__main__":
	args = sys.argv[1:]

	if not args:
		print "Pass in the base proc name eg glGenProgramsARB"
	else:
		ProcAddressHelper(args[0])
		#eval(args[0])
