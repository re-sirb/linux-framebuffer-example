#ifndef JPG_H_ 
#define JPG_H_

struct imgRawImage {
	unsigned int numComponents;
	unsigned long int width, height;
	unsigned char* lpData;
};

struct imgRawImage* loadJpegImageFile(char* lpFilename);

#endif
