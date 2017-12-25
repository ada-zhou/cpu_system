#include "gl.h"
#include "fb.h"
#include "font.h"

#include "printf.h"

/* BASIC GRAPHICS LIBRARY */


void gl_init(unsigned width, unsigned height, unsigned mode) {
    fb_init (width, height, 4, mode);
}

void gl_swap_buffer() {
    fb_swap_buffer();
}

color gl_color(unsigned char r, unsigned char g, unsigned char b) {
    color c;
    c = (0xff << 24) | (r << 16) | (g << 8) | b;
    return c;
}

void gl_clear(color c) {
    
    for (int y = 0; y < fb_get_height(); y++) {
        for (int x = 0; x < fb_get_width(); x++) {
            gl_draw_pixel(x, y, c);
            
        }
    }
}

void gl_draw_pixel(int x, int y, color c) {
    if (x < fb_get_width() && y < fb_get_height()) {
        unsigned pixelWidth  = (fb_get_pitch()*8)/(fb_get_depth());
        unsigned (*im)[pixelWidth] = (unsigned (*)[pixelWidth])fb_get_draw_buffer();
        im[y][x] = c;
    }
    
    
}

color gl_read_pixel(int x, int y) {
    color c = 0;
    if (x < fb_get_width() && y < fb_get_height()) {
        unsigned pixelWidth  = (fb_get_pitch()*8)/(fb_get_depth());
        unsigned (*im)[pixelWidth] = (unsigned (*)[pixelWidth])fb_get_draw_buffer();
        c = im[y][x];
    }
    return c;
}

void gl_draw_rect(int x, int y, int w, int h, color c) {
    for (int i = y; i < y+h; i++) {
        for (int j = x; j < x+w; j++) {
            if (j < fb_get_width() && i < fb_get_height()) {
               gl_draw_pixel(j,i,c);
            }
        }

    }
    
}

/* ADD TEXT */

void gl_draw_char(int x, int y, char ch, color c) {
    
    if ((x+font_get_width()) > fb_get_width() || (y + font_get_height()) > fb_get_height()) {
        return;
    }
    
    int size = font_get_size();
    char buf [size];
    int success = font_get_char(ch, buf, size);
    
    if (!success) {
        return;
    }
    int bit_index = 0;
    
    for (int i = y; i < y+font_get_height(); i ++) {
        for (int j = x; j < x+font_get_width(); j++) {
            bit_index = ((i-y)*font_get_width()*font_get_depth())+((j-x)*font_get_depth());
            if (buf[bit_index] == 0xFF){
                gl_draw_pixel(j,i,c);
            } else {
                gl_draw_pixel(j,i,GL_BLACK);
            }
            
            
        }
    }
    
    
    
}

void gl_draw_string(int x, int y, char* str, color c) {
    while (*str != '\0'){
        gl_draw_char(x, y, *str, c);
        x += font_get_width();
        str++;
        
    }
}
