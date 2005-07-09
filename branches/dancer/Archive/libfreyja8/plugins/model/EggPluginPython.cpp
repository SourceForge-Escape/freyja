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
 * 2001-02-24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/


// unsigned int eggBegin(egg_plugin_t type);
PyObject *eggBeginPython(PyObject *self, PyObject *args)
{
	unsigned int type;


	if (!PyArg_ParseTuple(args, "i", &type))
		return NULL;

	return PyInt_FromLong(eggBegin());
}


// unsigned int eggEnd();
PyObject *eggEndPython(PyObject *self, PyObject *args)
{
	return PyInt_FromLong(eggEnd());
}


// unsigned int eggTexCoordStore2f(float s, float t);
PyObject *eggTexCoordStore2fPython(PyObject *self, PyObject *args)
{
	float u, v;


	if (!PyArg_ParseTuple(args, "f", &u))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &v))
		return NULL;

	return PyInt_FromLong(eggTexCoordStore2f(u, v));

}


// unsigned int eggVertexStore3f(float x, float y, float z);
PyObject *eggVertexStore3fPython(PyObject *self, PyObject *args)
{
	float x, y, z;


	if (!PyArg_ParseTuple(args, "f", &x))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &y))
		return NULL;

	if (!PyArg_ParseTuple(args, "f", &z))
		return NULL;

	return PyInt_FromLong(eggVertexStore3f(x, y, z));
}


///////////////////////////////////////////////////////////////////////


PyMethodDef Plugin_methods[] = 
{
	{ "eggBegin", eggBeginPython, METH_VARARGS },
	{ "eggEnd", eggEndPython, METH_VARARGS },
	{ "eggVertexStore3f", eggVertexStore3fPython, METH_VARARGS },
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
