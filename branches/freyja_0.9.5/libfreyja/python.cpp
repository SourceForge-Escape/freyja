
#include <python2.4/Python.h>

void conf_stub()
{
	const char *plugin = "test.py";

	/* Start up python */
	Py_Initialize();
	
	FILE *f = fopen(plugin, "r");

	if (!f)
	{
		perror(plugin);
	}
	else
	{
		PyRun_SimpleFile(f, plugin);
		Py_Finalize();
		PyErr_Clear();
		fclose(f);
	}
}


int main(int argc, char *argv[])
{
	FILE *f = fopen("python.env", "w");

	if (f)
	{
		fprintf(f, "#################################################\n");
		fprintf(f, "# Python environment\n");
		fprintf(f, "#################################################\n");
		fprintf(f, "\n");
		fprintf(f, "CONFIGURE_DEF += -DUSING_PYTHON\n");
		fprintf(f, "CONFIGURE_LNK += -lpython2.4\n");
	}

	return 0;
}
