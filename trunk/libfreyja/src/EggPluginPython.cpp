/*================================================================
 * 
 * Project : GooseEgg
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
 ==============================================================*/

/**********
   
   v0 = eggVertex(x0, y0, z0);
   v1 = eggVertex(x1, y1, z1);
   v2 = eggVertex(x2, y2, z2);
   
   st0 = eggTexel(s0, t0);
   st1 = eggTexel(s1, t1);
   st2 = eggTexel(s2, t2);

   eggBeginPolygon();
     eggVertexId(v0);
     eggTexelId(st0);
     eggVertexId(v1);
     eggTexelId(st1);
     eggVertexId(v2);
     eggTexelId(st2);
   eggEndPolygon();

**********/

PyObject *eggBeginPolygon(PyObject *self, PyObject *args)
{
  eggBeginPolygon();
}


PyObject *eggEndPolygon(PyObject *self, PyObject *args)
{
  eggEndPolygon();
}


PyObject *eggVertexId(PyObject *self, PyObject *args)
{
  int id;


  if (!PyArg_ParseTuple(args, "i", &id))
    return NULL;

  return PyInt_FromLong(eggVertex(id));
}


  ///////////////////////////////////////////////////////////////////////


PyMethodDef Plugin_methods[] = 
{
  { "eggBeginPolygon", eggBeginPolygon, METH_VARARGS },
  { "eggEndPolygon", eggEndPolygon, METH_VARARGS },
  { "eggVertexId", eggVertexId, METH_VARARGS },
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
