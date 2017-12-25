#include "bcm.h"
#include "mailbox.h"
#include "fb.h"

typedef struct {
    unsigned int width;       // width of the display
    unsigned int height;      // height of the display
    unsigned int virtual_width;  // width of the virtual framebuffer
    unsigned int virtual_height; // height of the virtual framebuffer
    unsigned int pitch;       // number of bytes per row
    unsigned int depth;       // number of bits per pixel
    unsigned int x_offset;    // x of the upper left corner of the virtual fb
    unsigned int y_offset;    // y of the upper left corner of the virtual fb
    unsigned int framebuffer; // pointer to the start of the framebuffer
    unsigned int size;        // number of bytes in the framebuffer
} fb_config_t;

// fb is volatile because the GPU will write to it
static volatile fb_config_t fb __attribute__ ((aligned(16)));

void fb_init(unsigned width, unsigned height, unsigned depth, unsigned db) {
    // add code to handle double buffering
    if (db){
        fb.virtual_height = height*2;
    } else {
        fb.virtual_height = height;
    }
    
    
    fb.width = width;
    fb.virtual_width = width;
    fb.height = height;
    fb.depth = depth * 8; // convert number of bytes to number of bits
    fb.x_offset = 0;
    fb.y_offset = 0;
    
    // set values returned by the GPU to 0; see mailbox manual
    fb.pitch = 0;
    fb.framebuffer = 0;
    fb.size = 0;
    
    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
    (void) mailbox_read(MAILBOX_FRAMEBUFFER);
}


void fb_swap_buffer(void) {
    if (fb.y_offset == 0) {
        fb.y_offset = fb.height;
    } else {
        fb.y_offset = 0;
    }
    
    
    mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
    (void) mailbox_read(MAILBOX_FRAMEBUFFER);
}


unsigned char* fb_get_draw_buffer(void) {
    unsigned char * start = (unsigned char *) fb.framebuffer;
    if (fb.y_offset == 0){
        start += fb.pitch*fb.height;
    }
    
    return start;
}


unsigned fb_get_width(void) {
    return fb.width;
}

unsigned fb_get_height(void) {
    return fb.height;
}

unsigned fb_get_depth(void) {
    return fb.depth;
}

unsigned fb_get_pitch(void) {
    return fb.pitch;
}


