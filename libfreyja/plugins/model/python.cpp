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

PyObject *initPlugins();


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
	int ret = -1;
	FILE *f;
#ifdef UNIT_TEST_PYTHON
	char *plugin = "plugins/model/demo.py";
#else
	char *plugin = "/usr/local/lib/freyja/modules/model/python/import.py";
#endif
	PyObject *module, *dict, *tmp;

	Py_Initialize();
	module = initPlugins();

	f = fopen(plugin, "r");

	if (!f)
	{
		printf("\n'%s' failed to load...\n", plugin);
		perror(plugin);
	}
	else
	{
		dict = PyModule_GetDict(module);

		/* Append gobal constants */
		tmp = PyString_FromFormat(filename);
		PyDict_SetItemString(dict, "FreyjaImportFilename", tmp);
		Py_DECREF(tmp);

		printf("\t[Running '%s'...]\n", plugin);
		PyRun_SimpleFile(f, plugin);
		printf("\t[done]\n");
	}

	Py_Finalize();

	// FIXME: Call to functions in python modules and get return values

	return ret;
}


PyObject *py_freyjaBegin(PyObject *self, PyObject *args)
{
	unsigned int type;


	if (!PyArg_ParseTuple(args, "i", &type))
		return NULL;

	freyjaBegin((freyja_object_t)type);

#ifdef UNIT_TEST_PYTHON
	char *s;

	switch ((freyja_object_t)type)
	{
	case FREYJA_MODEL:
		s = "FREYJA_MODEL";
		break;

	case FREYJA_MESH:
		s = "FREYJA_MESH";
		break;

	case FREYJA_VERTEX_GROUP:
		s = "FREYJA_VERTEX_GROUP";
		break;

	case FREYJA_SKELETON:
		s = "FREYJA_SKELETON";
		break;

	case FREYJA_BONE:
		s = "FREYJA_BONE";
		break;

	case FREYJA_POLYGON:
		s = "FREYJA_POLYGON";
		break;

	default:
		s = "FREYJA_UNKNOWN";
	}

	printf("freyjaBegin(%s)\n", s);
#endif

	return PyInt_FromLong(0);
}


PyObject *py_freyjaEnd(PyObject *self, PyObject *args)
{
#ifdef UNIT_TEST_PYTHON
	printf("freyjaEnd()\n");
#endif

	freyjaEnd();
	return PyInt_FromLong(0);
}


PyObject *py_freyjaTexCoord2f(PyObject *self, PyObject *args)
{
	float u, v;


	if (!PyArg_ParseTuple(args, "ff", &u, &v))
		return NULL;

	return PyInt_FromLong(freyjaTexCoord2f(u, v));
}


PyObject *py_freyjaVertex3f(PyObject *self, PyObject *args)
{
	float x, y, z;


	if (!PyArg_ParseTuple(args, "fff", &x, &y, &z))
		return NULL;

	return PyInt_FromLong(freyjaVertex3f(x, y, z));
}


PyObject *py_freyjaVertexNormal3f(PyObject *self, PyObject *args)
{
	float x, y, z;
	int index;

	if (!PyArg_ParseTuple(args, "ifff", &index, &x, &y, &z))
		return NULL;

	freyjaVertexNormal3f(index, x, y, z);
	return PyInt_FromLong(0);
}


PyObject *py_freyjaPolygonVertex1i(PyObject *self, PyObject *args)
{
	int index;

	if (!PyArg_ParseTuple(args, "i", &index))
		return NULL;

	freyjaPolygonVertex1i(index);
	return PyInt_FromLong(0);
}


PyObject *py_freyjaPolygonTexCoord1i(PyObject *self, PyObject *args)
{
	int index;

	if (!PyArg_ParseTuple(args, "i", &index))
		return NULL;

	freyjaPolygonTexCoord1i(index);
	return PyInt_FromLong(0);
}


PyObject *py_freyjaPolygonMaterial1i(PyObject *self, PyObject *args)
{
	int index;

	if (!PyArg_ParseTuple(args, "i", &index))
		return NULL;

	freyjaPolygonMaterial1i(index);
	return PyInt_FromLong(0);
}


///////////////////////////////////////////////////////////////////////


PyMethodDef Plugin_methods[] = 
{
	{ "freyjaBegin", py_freyjaBegin, METH_VARARGS },
	{ "freyjaEnd", py_freyjaEnd, METH_VARARGS },
	{ "freyjaVertex3f", py_freyjaVertex3f, METH_VARARGS },
	{ "freyjaTexCoord2f", py_freyjaTexCoord2f, METH_VARARGS },
	{ "freyjaVertexNormal3f", py_freyjaVertexNormal3f, METH_VARARGS },
	{ "freyjaPolygonVertex1i", py_freyjaPolygonVertex1i, METH_VARARGS },
	{ "freyjaPolygonTexCoord1i", py_freyjaPolygonTexCoord1i, METH_VARARGS },
	{ "freyjaPolygonMaterial1i", py_freyjaPolygonMaterial1i, METH_VARARGS },
	{ NULL }
};


PyObject *initPlugins()
{
	PyObject *module, *dict, *tmp;

	module = Py_InitModule("FreyjaModeler", Plugin_methods);
    dict = PyModule_GetDict(module);

	/* Append gobal constants */
    tmp = PyInt_FromLong(FREYJA_MODEL);
    PyDict_SetItemString(dict, "FREYJA_MODEL", tmp);
    Py_DECREF(tmp);

    tmp = PyInt_FromLong(FREYJA_MESH);
    PyDict_SetItemString(dict, "FREYJA_MESH", tmp);
    Py_DECREF(tmp);

    tmp = PyInt_FromLong(FREYJA_POLYGON);
    PyDict_SetItemString(dict, "FREYJA_POLYGON", tmp);
    Py_DECREF(tmp);

    tmp = PyInt_FromLong(FREYJA_VERTEX_GROUP);
    PyDict_SetItemString(dict, "FREYJA_VERTEX_GROUP", tmp);
    Py_DECREF(tmp);

    tmp = PyInt_FromLong(FREYJA_BONE);
    PyDict_SetItemString(dict, "FREYJA_BONE", tmp);
    Py_DECREF(tmp);

    tmp = PyInt_FromLong(FREYJA_SKELETON);
    PyDict_SetItemString(dict, "FREYJA_SKELETON", tmp);
    Py_DECREF(tmp);

	return module;
}


#ifdef UNIT_TEST_PYTHON

#include <freyja/EggPlugin.h>

int main(int argc, char *argv[])
{
	Egg egg;
	EggPlugin plugin(&egg);

	if (argc > 2)
	{
		import_model(argv[1]);
	}
	else
	{
		import_model("test.smd");
	}

	plugin.exportModel("python.ase", "ase");

	return 0;
}
#endif

