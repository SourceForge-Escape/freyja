
#include <stdio.h>
#include <stdlib.h>


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
		printf("extractFace filename offset count\n");
		return -1;
	}

	offset = atoi(argv[2]);
	count = atoi(argv[3]);

	fseek(in, offset, SEEK_SET);

	for (i = 0; i < count; ++i)
	{
		printf("f%i ", i);
		fread(&s, 2, 1, in);
		printf("%i ", s);
		fread(&s, 2, 1, in);
		printf("%i ", s);
		fread(&s, 2, 1, in);
		printf("%i ", s);
		fread(&c, 1, 1, in);
		printf("%i ", c);
		fread(&c, 1, 1, in);
		printf("%i ", c);
		fread(&u, 4, 1, in);
		printf("%i\n", u);
	}

	fclose(in);

	return 0;
}
