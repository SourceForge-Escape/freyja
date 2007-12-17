/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* A simple program for culling data out of model files/paks */

#include <stdio.h>
#include <stdlib.h>
#include <mstl/SystemIO.h>

using namespace mstl;

int main(int argc, char *argv[])
{
	SystemIO::FileWriter w;
	SystemIO::FileReader r;
	unsigned char *buffer;
	unsigned int offset, size;


	if (argc < 5)
	{
		printf("%s input_file output_file offset size\n", argv[0]);
		return -1;
	}

	if (!r.Open(argv[1]))
	{
		printf("File '%s' couldn't be accessed.", argv[1]);
		return -1;
	}

	offset = atoi(argv[3]);
	size = atoi(argv[4]);

	if (size < 1)
	{
		printf("Invalid size\n");
		return -1;
	}

	if (w.DoesFileExist(argv[2]))
	{
		printf("File '%s' exists\n", argv[2]);
		return -1;
	}

	buffer = new unsigned char[size];

	r.SetOffset(offset);
	r.ReadBuffer(size, buffer);
	r.Close();

	w.Open(argv[2]);
	w.WriteBuffer(size, buffer);
	w.Close();

	delete [] buffer;

	return 0;
}
