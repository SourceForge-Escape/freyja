
/* A simple program for culling RIFFs out of UT raw dumps for LineageII */

#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
	const unsigned int size = 10240;
	FILE *f, *f2;
	unsigned int i, state;
	char filename[size];
	char c;


	if (argc == 1 || argc == 2 && strncmp("--help", argv[1], 6) == 0)
	{
		printf("raw2riff trims the RIFFs out of raw files\n\traw2riff [filenames ...]\n");
		return 0;
	}

	for (i = 1; (int)i < argc; ++i)
	{
		f = fopen(argv[i], "rb");

		printf("Opening %s ...\n", argv[i]);

		if (!f)
		{
			perror(argv[i]);
			continue;
		}

		state = 0;

		while (fread(&c, 1, 1, f) > 0)
		{
			switch (state)
			{
			case 0:
				if (c == 'R') state = 1;
				break;
			case 1:
				if (c == 'I') state = 2; else state = 0;
				break;
			case 2:
				if (c == 'F') state = 3; else state = 0;
				break;
			case 3:
				if (c == 'F') state = 4; else state = 0;
				break;
			case 4:
				state = 5;

				snprintf(filename, size, "%s.wav", argv[i]);
				f2 = fopen(filename, "wb");

				printf("Writing %s ...\n", filename);

				if (!f2)
				{
					state = 6;
					perror(filename);
					continue;
				}

				snprintf(filename, size, "RIFF");
				fwrite(filename, 1, 4, f2);
			case 5:
				fwrite(&c, 1, 1, f2);
				break;
			default:
				printf("Error: in invalid state %u\n", state);
				return -1;
			}
		}

		if (state == 5) fclose(f2);

		fclose(f);
	}

	return 0;
}
