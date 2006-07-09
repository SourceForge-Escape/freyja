/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* A simple program for converting image files */

#include <stdio.h>
#include <stdlib.h>
#include <freyja/FreyjaImage.h>

int main(int argc, char *argv[])
{
	FreyjaImage img;

	switch (argc)
	{
	case 4: // util file.dds tga file.tga
		img.loadImage(argv[1]);
		img.saveImage(argv[3], argv[2]);
		break;

	case 2:
		img.loadImage(argv[1]);
		break;

	default:
		printf("Usage: %s import_image [type export_image]\n", argv[0]);
	}

	return 0;
}
