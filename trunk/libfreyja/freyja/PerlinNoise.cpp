/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : PerlinNoise
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class makes with the perlin noise ;)
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.03.11:
 * Mongoose - Created
 =================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "PerlinNoise.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

PerlinNoise::PerlinNoise()
{
	mPrimeSeeds[0] = 15731;
	mPrimeSeeds[1] = 789221;
	mPrimeSeeds[2] = 1376312589;
}


PerlinNoise::~PerlinNoise()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void PerlinNoise::clampBufferIntensity(byte *buffer, uint32 w, uint32 h,
										vec_t a, vec_t b, vec_t c)
{
	uint32 i, j, n;
	vec_t d;


	if (!buffer)
		return;

	for (i = 0; i < w; ++i)
	{
		for (j = 0; j < h; ++j)
		{
			n = j * w + i;
			d = linearInterpolate(buffer[n], a, b); // 3, 7 -- plasma

			buffer[n] = (byte)((d * 0.225f) * c); // 20.0f -- wood
		}
	}
}


vec_t PerlinNoise::interpolate(vec_t a, vec_t b, vec_t c)
{   
	return (a + (b - a) * c * c * (3 - 2 * c));
}


vec_t PerlinNoise::linearInterpolate(vec_t a, vec_t b, vec_t c)
{
	return (a * (1 - c) + b * c);
}


vec_t PerlinNoise::noise2D(vec_t x, vec_t y)
{
	int n;

	n = (int)(x + y * 57.0f);
	n = (n << 13) ^ n;
	return (1.0 - ( (n * (n * n * mPrimeSeeds[0] + mPrimeSeeds[1]) +
						  mPrimeSeeds[2]) & 0x7fffffff) / 
			  1073741824.0);    
}


vec_t PerlinNoise::noise(int32 n)
{
	n = (n << 13) ^ n;
	return (1.0 - ( (n * (n * n * mPrimeSeeds[0] + mPrimeSeeds[1]) +
						  mPrimeSeeds[2]) & 0x7fffffff) / 
			  1073741824.0);    
}


vec_t PerlinNoise::smoothedNoise2D(vec_t x, vec_t y)
{
	vec_t corners, sides, center;

	corners = (noise2D(x-1.0f, y-1.0f) + 
				  noise2D(x+1.0f, y-1.0f) +
				  noise2D(x-1.0f, y+1.0f) +
				  noise2D(x+1.0f, y+1.0f)) / 16.0f;
	sides   = (noise2D(x-1.0f, y) +
				  noise2D(x+1.0f, y) +
				  noise2D(x, y-1.0f) +
				  noise2D(x, y+1.0f)) /  8.0f;
	center  =  noise2D(x, y) / 4.0f;
	
	return corners + sides + center;
}


vec_t PerlinNoise::interpolatedNoise2D(vec_t x, vec_t y)
{
	int integerX = int(x);
	vec_t fractionalX = x - integerX;
	int integerY    = int(y);
	vec_t fractionalY = y - integerY;
	vec_t v1, v2, v3, v4, i1, i2;

	v1 = smoothedNoise2D(integerX,     integerY);
	v2 = smoothedNoise2D(integerX + 1, integerY);
	v3 = smoothedNoise2D(integerX,     integerY + 1);
	v4 = smoothedNoise2D(integerX + 1, integerY + 1);
	
	i1 = linearInterpolate(v1, v2, fractionalX);
	i2 = linearInterpolate(v3, v4, fractionalX);
	
	return linearInterpolate(i1, i2, fractionalY);
}


vec_t PerlinNoise::perlinNoise2D(vec_t x, vec_t y, uint32 octaves,
											uint32 seed, double period, 
											uint32 span)
{
	double a, b, frequency, zoneX, zoneY, amplitude = 120, total = 0;
	//p = persistence;
	uint32 i, num, box, stepX, stepY, noiseI;


	frequency = (1 / period);

	for (i = 0; i < octaves; ++i)
	{
		num = (uint32)(span * frequency);

		stepX = (uint32)(x * frequency);
		stepY = (uint32)(y * frequency);

		zoneX = x * frequency - stepX;
		zoneY = y * frequency - stepY;

		box = stepX + stepY * num;

		noiseI = box + seed;

		a = interpolate(noise(noiseI), noise(noiseI+1), zoneX);
		b = interpolate(noise(noiseI + num), noise(noiseI + num + 1), zoneX);

		total = interpolate(a, b, zoneY) * amplitude;
	}

	return total;
}


byte *PerlinNoise::generateBuffer(uint32 width, uint32 height, uint32 seed)
{
	byte *buffer = 0x0;
	uint32 x, y, span = 181; // 12413;
	vec_t disp1, disp2, disp3, disp4, disp5, disp6, total, scale = 1;


	if (width < 0 || height < 1)
	{
		return 0x0;
	}

	buffer = new byte[width*height];

	for (y = 0; y < width; ++y)
	{
		for (x = 0; x < height; ++x)
		{
			disp1 = perlinNoise2D(x*scale, y*scale, 2, seed, 100, span);
			disp2 = perlinNoise2D(x*scale, y*scale, 2, seed, 25, span);
			disp3 = perlinNoise2D(x*scale, y*scale, 2, seed, 12.5, span);
			disp4 = perlinNoise2D(x*scale, y*scale, 2, seed, 6.25, span);
			disp5 = perlinNoise2D(x*scale, y*scale, 2, seed, 3.125, span);
			disp6 = perlinNoise2D(x*scale, y*scale, 2, seed, 1.56, span);

			total = ((int)disp1 + 
						(int)(disp2 * 0.25) +
						(int)(disp3 * 0.125) +
						(int)(disp4 * 0.0625) +
						(int)(disp5 * 0.03125) +
						(int)(disp6 * 0.0156));

			buffer[height*y+x] = (byte)total;
		}
	}

	return buffer;
}       


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_PERLINNOISE

#include "../plugins/image/tga.h"

int dumpTGA(char *filename, byte *buffer, 
				uint32 w, uint32 h)
{
	FILE *f;
	int ret = 0;


	printf("Writing '%s'...\n", filename);

	f = fopen(filename, "wb");

	if (!f)
	{
		perror("Failed to open file for writing\n");
		return -1;
	}

	if (mtk_image__tga_save(f, buffer, w, h, 8) < 0)
	{
		printf("Failed to write TGA file\n");
		ret = -1;
	}

	fclose(f);

	return ret;
}


int runPerlinNoiseUnitTest(int argc, char *argv[])
{
	PerlinNoise test;
	byte *buf;
	char filename[64];
	uint32 seeds[8];
	uint32 i, w, h, runs;


	w = h = 256;

	if (argc > 1)
	{
		w = h = atoi(argv[1]);
	}

	runs = 4;
	seeds[0] = 101;
	seeds[1] = 3;//709;
	seeds[2] = 199;
	seeds[3] = 257;

	for (i = 0; i < runs; ++i)
	{
		buf = test.generateBuffer(w, h, seeds[i]);
		
		if (!buf)
		{
			printf("PerlinNoise generator failed!\n");
			return -1;
		}
		
		snprintf(filename, 64, "/tmp/perlin-noise_%i-%ix%i.tga", seeds[i], w, h);
		dumpTGA(filename, buf, w, h);

		snprintf(filename, 64, "/tmp/perlin-noise_%ii-%ix%i.tga",seeds[i], w, h);
		test.clampBufferIntensity(buf, w, h, 0.0f, 0.0f, 1.0f);
		dumpTGA(filename, buf, w, h);

		snprintf(filename, 64, "/tmp/perlin-noise_%ii2-%ix%i.tga",seeds[i], w, h);
		test.clampBufferIntensity(buf, w, h, 3.0f, 7.0f, 21.0f);
		dumpTGA(filename, buf, w, h);

		snprintf(filename, 64, "/tmp/perlin-noise_%ii3-%ix%i.tga",seeds[i], w, h);
		test.clampBufferIntensity(buf, w, h, 4.0f, 2.0f, 30.0f);
		dumpTGA(filename, buf, w, h);

		delete [] buf;
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[PerlinNoise class test]\n");

	return runPerlinNoiseUnitTest(argc, argv);
}
#endif
