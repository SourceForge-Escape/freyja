/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : GooseEgg, Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.25:
 * Mongoose - Updated for freyja v9 prerelease test
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 ==========================================================================*/

#include <python2.3/Python.h>
#include <freyja/FreyjaPlugin.h>


extern "C" {

	void import_functions();
	int import_model(char *filename);
	int export_model(char *filename);
}


void import_functions()
{
	/* Mongoose 2004.11.01, 
	 * FIXME: Here is the entry to load the interactive python plugins */
}


int export_model(char *filename)
{
	/* Mongoose 2004.11.01, 
	 * FIXME: Here is the entry to load the python plugins for model export */
	return -1;
}


int import_model(char *filename)
{
	/* Mongoose 2004.11.01, 
	 * FIXME: Here is the entry to load the python plugins for model import */
	return -1;
}


PyObject *py_freyjaBegin(PyObject *self, PyObject *args)
{
	unsigned int type;


	if (!PyArg_ParseTuple(args, "i", &type))
		return NULL;

	freyjaBegin((freyja_object_t)type);

	return PyInt_FromLong(0);
}


PyObject *py_freyjaEnd(PyObject *self, PyObject *args)
{
	freyjaEnd();
	return PyInt_FromLong(0);
}


PyObject *py_freyjaTexCoord2f(PyObject *self, PyObject *args)
{
	float u, v;


	if (!PyArg_ParseTuple(args, "f", &u))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &v))
		return NULL;

	return PyInt_FromLong(freyjaTexCoord2f(u, v));
}


PyObject *py_freyjaVertex3f(PyObject *self, PyObject *args)
{
	float x, y, z;


	if (!PyArg_ParseTuple(args, "f", &x))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &y))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &z))
		return NULL;

	return PyInt_FromLong(freyjaVertex3f(x, y, z));
}


///////////////////////////////////////////////////////////////////////


PyMethodDef Plugin_methods[] = 
{
	{ "freyjaBegin", py_freyjaBegin, METH_VARARGS },
	{ "freyjaEnd", py_freyjaEnd, METH_VARARGS },
	{ "freyjaTexCoord2f", py_freyjaTexCoord2f, METH_VARARGS },
	{ "freyjaVertex3f", py_freyjaVertex3f, METH_VARARGS },
	{ NULL }
};


void initPlugins()
{
	Py_InitModule("plugin", Plugin_methods);
}


#ifdef TEST_PLUGIN
int main(int argv, char *argc[])
{
	return 0;
}

#endif
