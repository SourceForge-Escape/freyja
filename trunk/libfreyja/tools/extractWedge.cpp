#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *in;
	unsigned int i, u, count, offset;
	unsigned short s;
	float f;
	char c;

	in = fopen(argv[1], "rb");

	if (!in)
	{
		printf("extractWedge filename offset count\n");
		return -1;
	}

	offset = atoi(argv[2]);
	count = atoi(argv[3]);

	fseek(in, offset, SEEK_SET);

	for (i = 0; i < count; ++i)
	{
		printf("w%i ", i);
		fread(&s, 2, 1, in);
		printf("%i ", s);
		fread(&f, 4, 1, in);
		printf("%f ", f);
		fread(&f, 4, 1, in);
		printf("%f\n", f);
	}

	fclose(in);

	return 0;
}
