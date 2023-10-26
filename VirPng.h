#ifndef VIRPNG_H_INCLUDED
#define VIRPNG_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <time.h>
#include <assert.h>

int read_png_file(const char *filename, png_bytep * *row_pointers, unsigned *width, unsigned *height, png_byte *color_type, png_byte *bit_depth);
int write_png_file(const char *filename, png_bytep *row_pointers, unsigned width, unsigned height);
int set_png_pixels(png_bytep *row_pointers, unsigned x, unsigned y, png_byte wanted_color[]);
int create_png_image_from_video_ram(const char *filename, const png_byte *video_ram, const unsigned width, const unsigned height);

#endif //VIRPNG_H_INCLUDED