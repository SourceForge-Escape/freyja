
#include <stdio.h>
#include <stdlib.h>
#include <mstl/SystemIO.h>

int read_index(FILE *f, unsigned int &bytes)
{
	int val;
	char b0, b1, b2, b3, b4;

	bytes = 0;                                                         
	val = 0;
   
	fread(&b0, 1, 1, f);
	++bytes;
   
	if (b0 & 0x40)
	{
		fread(&b1, 1, 1, f);
 		++bytes;

		if (b1 & 0x80)
		{
			fread(&b2, 1, 1, f);
	 		++bytes;
 
			if (b2 & 0x80)
			{
				fread(&b3, 1, 1, f);
		 		++bytes;
 
				if (b3 & 0x80)
				{
					fread(&b4, 1, 1, f);
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

	return val;
}


bool test_wedge_offset(FILE *f, unsigned long offset, 
                       short &s, float &u, float &v)
{
	//short s;
	//float u, v;

	fseek(f, offset, SEEK_SET);
	fread(&s, 2, 1, f);
	fread(&u, 4, 1, f);
	fread(&v, 4, 1, f);

#ifdef __BIG_ENDIAN__
	//FIX_SHORT(s);
	//FIX_FLOAT(u);
	//FIX_FLOAT(v);
#endif
	
	return (s >= 0 && u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f);
}


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("%s filename offset\n\toffset < 0 dumps entire file from each byte\n", argv[0]);
		return -1;
	}

	FILE *f = fopen(argv[1], "rb");
	int offset = atoi(argv[2]);

	if (!f)
	{
		printf("%s filename offset\n", argv[0]);
		return -1;
	}

	

	if (offset == -311)
	{
		fseek(f, 0, SEEK_END);
		unsigned long first = 0, old, end = ftell(f);
		int count = 0;
		
		short s = 0, oldS = 0, s2;
		float u, v, u2, v2;

		long state = 0;

		// Wedge search starting at offset ( here it's byte 0 )
		for (offset = 0; offset < (long)end; )
		{
			old = offset;

			if (test_wedge_offset(f, offset, s, u, v))
			{
				switch (state)
				{	
				case 0: // Find the first wedge
					if (test_wedge_offset(f, offset+10, s2, u2, v2) &&
						s == 0 && s2 == 1 && count == 0)
					{
						if (test_wedge_offset(f, offset+20, s2, u2, v2) &&
							s2 == 2)
						{
							first = old;
							count = 1;
							state = 1;
							oldS = s;
							offset = old+10;

							printf("Wedge @ %lu - %i, %f, %f\n", old, s, u, v);
						}
						else
						{
							++offset;
						}
					}
					else
					{
						++offset;
					}
					break;
			
				case 1:
					if (s == oldS + 1 && count)
					{
						printf("Wedge @ %lu - %i, %f, %f\n", old, s, u, v);
						oldS = s;
						offset = old+10;
						++count;
					}
					else
					{
						printf("Wedges @ %lu x %i\n", first, count);
						state = 2;
						++offset;
					}
					break;

				case 2:
					// optional search for 'extra' wedges
					if (1)
					{
						count = 0;
						state = 0;
					}
					break;

				default:
					++offset;
				}
			}
			else
			{
				++offset;
			}
		}

		if (count) 
			printf("Wedges @ %lu x %i?\n", first, count);
	}
	else if (offset == -1)
	{
		fseek(f, 0, SEEK_END);
		long i, count = ftell(f);

		for (i = 0; i < count; ++i)
		{
			unsigned int bytes;
			fseek(f, i, SEEK_SET);
			int value = read_index(f, bytes);

			if (value >= 1)
			  printf("%li, %i bytes, %i\n", i, bytes, value);	
		}
	}
	else if (offset < 0)
	{
		fseek(f, 0, SEEK_END);
		long i, count = ftell(f);

		for (i = 0; i < count; ++i)
		{
			unsigned int bytes;
			fseek(f, i, SEEK_SET);
			int value = read_index(f, bytes);

			fseek(f, i, SEEK_SET);
			float r;
			fread(&r, 4, 1, f);

			fseek(f, i, SEEK_SET);
			short s;
			fread(&s, 2, 1, f);

			#ifdef __BIG_ENDIAN__
			FIX_FLOAT(r);
			FIX_SHORT(s);
			#endif

			printf("%li, %i bytes, %i\n", i, bytes, value);	
		}
	}
	else
	{
		fseek(f, offset, SEEK_SET);
		unsigned int bytes;
		int value = read_index(f, bytes);
		printf("%i, %i bytes, %i\n", offset, bytes, value);
	}

	fclose(f);

	return 0;
}
