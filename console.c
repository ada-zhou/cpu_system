#include "console.h"
#include "malloc.h"
#include "font.h"
#include "gl.h"

#include "printf.h"

static unsigned int xframe = 0;
static unsigned int yframe = 0;

static unsigned int rows;
static unsigned int cols;
static char * consoleBuff;
static int consoleStart;


static char read_console (int x, int y);
static void write_to_frame(unsigned ch, int x, int y);
static void write_to_console(unsigned ch, int x, int y);
static void redraw_frame(void);
static void clear_row(void);


void console_init(unsigned nrows, unsigned ncols) {
    
    rows = nrows;
    cols = ncols;
    
    consoleBuff = (char *) malloc(rows*cols);
    
    //sets everything in console buffer to be null
    unsigned (*con)[cols] = (unsigned (*)[cols])consoleBuff;
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            con[i][j] = '\0';
        }
    }
    
    consoleStart = 0;
    
    gl_init(cols*font_get_width(), nrows*font_get_height(), GL_DOUBLEBUFFER);

}

//clears row that goes off screen
static void clear_row(void){
    for (int c = 0; c < cols; c++) {
        write_to_console('\0', c, yframe-1);
    }
}

//redraws the framebuffer according to what's in the console buffer
static void redraw_frame(void){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            write_to_frame(read_console(j,i),j,i);
        }
    }
    
}

//reads what's in the console buffer at x, y
static char read_console (int x, int y) {
    return consoleBuff[(((consoleStart+y)%rows)*cols)+ x];
}

//draws to the framebuffer at x, y
static void write_to_frame(unsigned ch, int x, int y) {
    gl_draw_char(x*font_get_width(), y*font_get_height(), ch, GL_WHITE);
}

//writes to the console at appropriate spot for ch to be at x,y in framebuffer
static void write_to_console(unsigned ch, int x, int y){
    consoleBuff[(((consoleStart+y)%rows)*cols)+ x] = ch;
}

/*
 *checks if cursor is in bounds
 *checks if the character is a special case - increments as necessary for each special character
 *adding characters to screen involves changing the consolebuffer to match what changes you want to make
 *then changes the framebuffer to match changes and swaps the framebuffer and change the other one as well
*/
void console_putc(unsigned ch) {
    if (xframe == cols){
        xframe = 0;
        yframe++;
    }
    if (yframe == rows) {
        consoleStart = (consoleStart + 1) % rows;
        clear_row();
        gl_clear(GL_BLACK);
        redraw_frame();
        gl_swap_buffer();
        gl_clear(GL_BLACK);
        redraw_frame();
        yframe--;
        xframe= 0;
    }
    
    if (ch == '\n') { //move to next line: so increment where you are in framebuffer by 1
        yframe++;
        xframe = 0;
    } else if (ch == '\r') { //returns to beginning of line: x -> 0
        xframe = 0;
    } else if (ch == '\b'){ //backspace: changes the char before as a space char then go back x by 1
        if (xframe == 0 && yframe != 0){
            xframe = cols-1;
            yframe--;
            write_to_console('\0',xframe,yframe);
            gl_clear(GL_BLACK);
            redraw_frame();
            gl_swap_buffer();
            gl_clear(GL_BLACK);
            redraw_frame();
        
        } else if (xframe != 0 && yframe != 0) {
            xframe = (xframe+cols-1)%cols;
            write_to_console('\0',xframe,yframe);
            gl_draw_char(xframe*font_get_width(), yframe*font_get_height(), ' ', GL_BLACK);
            gl_swap_buffer();
            gl_draw_char(xframe*font_get_width(), yframe*font_get_height(), ' ', GL_BLACK);
        }
        
    } else if (ch == '\f') {
        //clears console and sets cursor at home position: sets everything in framebuffer to color (clear) then sets cursor to 0,0
        gl_clear(GL_BLACK);
        gl_swap_buffer();
        gl_clear(GL_BLACK);
        for (int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                write_to_console('\0', j, i);
            }
            
        }
        xframe = 0;
        yframe = 0;
    } else {
        write_to_console(ch,xframe,yframe);
        write_to_frame(ch, xframe, yframe);
        gl_swap_buffer();
        write_to_frame(ch, xframe, yframe);
        xframe++;
    }
    
}

//creates an array to store the string in vsnprintf
//puts each character in the array into the console by calling console_putc
int console_printf(const char *format, ...) {
    int size = rows*cols*3; //ASK ABOUT SIZE OF THIS
    char s[size];
    
    char* sindex = s;
  
    va_list ap;
    va_start(ap, format);
    int count = vsnprintf (s, size, format, ap);
    va_end(ap);
    
    
    while (*sindex != '\0'){
        console_putc(*sindex);
        sindex++;
    }
    
	return count;
}

//returns character in console at row and col
char console_get_char(const unsigned row, unsigned col) {
	return consoleBuff[row*cols + col];
}
