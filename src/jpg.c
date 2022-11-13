#include <stdio.h>
#include <stdlib.h>

#include <jpeglib.h>
#include <jerror.h>

#include "jpg.h"


struct imgRawImage* loadJpegImageFile(char* lpFilename) 
{
	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;

	struct imgRawImage* NewImage;

	unsigned long int imgWidth, imgHeight;
	int numComponents;

	unsigned long int dwBufferBytes;
	unsigned char* lpData;

	unsigned char* lpRowBuffer[1];

	FILE* fh;

	fh = fopen(lpFilename, "rb");
	if(fh == NULL) {
		fprintf(stderr, "%s:%u: Failed to read file %s\n", __FILE__, __LINE__, lpFilename);
		exit(EXIT_FAILURE);
	}

	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info);

	jpeg_stdio_src(&info, fh);
	jpeg_read_header(&info, TRUE);

	jpeg_start_decompress(&info);
	imgWidth = info.output_width;
	imgHeight = info.output_height;
	numComponents = info.num_components;

	dwBufferBytes = imgWidth * imgHeight * 3;
	lpData = (unsigned char*)malloc(sizeof(unsigned char)*dwBufferBytes);

	NewImage = (struct imgRawImage*)malloc(sizeof(struct imgRawImage));
	NewImage->numComponents = numComponents;
	NewImage->width = imgWidth;
	NewImage->height = imgHeight;
	NewImage->lpData = lpData;

	while(info.output_scanline < info.output_height) {
		lpRowBuffer[0] = (unsigned char *)(&lpData[3*info.output_width*info.output_scanline]);
		jpeg_read_scanlines(&info, lpRowBuffer, 1);
	}

	jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);
	fclose(fh);

	return NewImage;
}
