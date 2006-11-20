#include <stdlib.h>
#include <stdio.h>

#define MAX(max, s) max = (s > max) ? s : max


int main(int argc, char *argv[])
{
	FILE *in;
	unsigned int i, u, count, offset;
	unsigned short s, max = 0;
	float f;
	char c;
	unsigned int bytes = 0, guess = 1, failsafe = 0, range = 12;
	int val;
	char b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0;


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
		MAX(max, s);
		printf("%i ", s);

		fread(&f, 4, 1, in);
		printf("%f ", f);

		fread(&f, 4, 1, in);
		printf("%f\n", f);
	}

	printf("# %i\n", ftell(in));

#define HINTHINT
#ifdef HINTHINT
	while (offset > 6 && ++failsafe < 10)
	{
		bytes = 0;
		fseek(in, offset-guess, SEEK_SET);
                                                         
		val = 0;
   
		fread(&b0, 1, 1, in);
		++bytes;
   
		if (b0 & 0x40)
		{
			fread(&b1, 1, 1, in);
			++bytes;
 
			if (b1 & 0x80)
			{
				fread(&b2, 1, 1, in);
				++bytes;
 
				if (b2 & 0x80)
				{
					fread(&b3, 1, 1, in);
					++bytes;
 
					if (b3 & 0x80)
					{
						fread(&b4, 1, 1, in);
						++bytes;
						val = b4;
					}
					
					val = (val << 7) + (b3 & 0x7f);
				}
			
				val = (val << 7) + (b2 & 0x7f);
			}
		
			val = (val << 7) + (b1 & 0x7f);
		}
	
		val = (val << 6) + (b0 & 0x3f);
   
		if (b0 & 0x80)
			val = -val;

		printf("%i bytes @ %i? -> %i\n", guess, offset-guess, val);

		if (val == count)
		{
			printf("Index Fit!\n");
			break;
		}

		if (val < 0 || 
		    val < (count - range) ||
		    val > (count + range))
		{
			++guess;
			continue;
		}

		break;
	}

	printf("vertexOffset = %i ?\n", offset - ((max+1) * 12) - 4 - bytes);
#endif

	fclose(in);

	return 0;
}
