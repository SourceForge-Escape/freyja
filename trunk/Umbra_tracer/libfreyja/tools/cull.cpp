/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */

/* A simple program for culling data out of model files/paks */

#include <stdio.h>
#include <stdlib.h>
#include <freyja8/EggFileReader.h>
#include <freyja8/EggFileWriter.h>

int main(int argc, char *argv[])
{
	EggFileWriter w;
	EggFileReader r;
	unsigned char *buffer;
	unsigned int offset, size;


	if (argc < 5)
	{
		printf("%s input_file output_file offset size\n", argv[0]);
		return -1;
	}

	if (!r.openFile(argv[1]))
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

	if (w.doesFileExist(argv[2]))
	{
		printf("File '%s' exists\n", argv[2]);
		return -1;
	}

	buffer = new unsigned char[size];

	r.setFileOffset(offset);
	r.readBufferUnsignedChar(size, buffer);
	r.closeFile();

	w.openFile(argv[2]);
	w.writeBufferUnsignedChar(size, buffer);
	w.closeFile();

	delete [] buffer;

	return 0;
}
