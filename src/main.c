#include <stdio.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/kd.h>
#include <assert.h>
#include <stdint.h>

#include "jpg.h"

//#define NDEBUG

int create_rgb_color(int r, int g, int b);
void draw_jpeg(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, char* filename);
void clear_screen(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int color);
void draw_rect(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, int end_x, int end_y, int color);
void draw_line(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, int end_x, int end_y, int color);



void draw_jpeg(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, char* filename)
{
	int s_x = screen_info->xres;
	int s_y = screen_info->yres;

	struct imgRawImage *a;
	a=loadJpegImageFile(filename);

	int i_x = start_x;
	int i_y = start_y;
	int end_x = start_x + a->width-1;
	int end_y = start_y + a->height-1;

	long int buffer_pos = 0;

	while(i_y <= end_y){
		i_x=start_x;
		while(i_x <= end_x){
			if((i_y * s_x + i_x) < (s_x * s_y)){
				mem_buff[i_y * s_x + i_x] = create_rgb_color(a->lpData[buffer_pos],a->lpData[buffer_pos+1],a->lpData[buffer_pos+2]);
				buffer_pos=buffer_pos+3;
			}
			i_x++;
		}
		i_y++;
	}
}

int create_rgb_color(int r, int g, int b)
{
	return (b+(g*256)+(r*65536));
}

void clear_screen(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int color)
{
	int s_x = screen_info->xres;
	int s_y = screen_info->yres;
	int i = 0;

	while(i<(s_x*s_y)){
		mem_buff[i]=color;
		i++;
	}
}

void draw_rect(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, int end_x, int end_y, int color)
{
	int s_x = screen_info->xres;
	int s_y = screen_info->yres;
	int i_x = start_x;
	int i_y = start_y;

	while(i_y <= end_y){
		i_x=start_x;
		while(i_x <= end_x){
			if((i_y * s_x + i_x) < (s_x * s_y)){
				mem_buff[i_y * s_x + i_x] = color;
			}
			i_x++;
		}
		i_y++;
	}
}

void draw_line(uint32_t *mem_buff, struct fb_var_screeninfo *screen_info, int start_x, int start_y, int end_x, int end_y, int color)
{
	int s_x = screen_info->xres;
	int s_y = screen_info->yres;
	int i_x = start_x;
	int i_y = start_y;
	float slope = (end_y-start_y)/(end_x-start_x);

	if(start_x==end_x){
		while(i_y<=end_y){
			if((i_y * s_x + i_x) < (s_x * s_y)){
				mem_buff[i_y * s_x + i_x] = color;
			}
			i_y++;
		}
	}
	else{
		while(i_x<=end_x){
			if((i_y * s_x + i_x) < (s_x * s_y)){
				mem_buff[i_y * s_x + i_x] = color;
			}
			i_x++;
			i_y = start_y + (int)((float)(i_x-start_x)*slope);
		}
	}
}

int main()
{
	//open framebuffer
	int fb = open("/dev/fb0", O_RDWR);
	assert(fb > 0);
	struct fb_var_screeninfo info;
	//get screen resolution & color depth
	assert(0 == ioctl(fb, FBIOGET_VSCREENINFO, &info));
	
	//Debug
	//printf("X: %d \nY: %d \nbpp: %d \n", info.xres, info.yres, info.bits_per_pixel);
	//printf("framebuffer test...\n");
	
	//open current console and switch console to graphics-mode (no text) 
	int console_0 = open("/dev/tty", O_RDWR);
	ioctl(console_0, KDSETMODE, KD_GRAPHICS);

	//buffer
	size_t len = 4 * info.xres * info.yres;
	uint32_t *buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	assert(buf != MAP_FAILED);
	
	//draw something
	clear_screen(buf, &info, create_rgb_color(255,255,255));
	
	draw_rect(buf, &info, 400, 200, 800, 600, create_rgb_color(200,200,0));
	draw_rect(buf, &info, 420, 220, 780, 580, create_rgb_color(255,255,255));
	draw_line(buf, &info, 400, 200, 800, 600, create_rgb_color(200,0,0));
	
	//remember to insert the correct path to an existing jpeg-file to run this demo
	draw_jpeg(buf, &info, 650, 300,"1.jpg");

	//wait for new line command to exit program
	char buffer[1];
	fread(&buffer, sizeof(char), 1, stdin);

	//switch console back to text-mode 
	ioctl(console_0, KDSETMODE, KD_TEXT);
	close(console_0);
	
	close(fb);
	return 0;
}
