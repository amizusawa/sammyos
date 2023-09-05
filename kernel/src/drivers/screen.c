
#include <screen.h>
#include <ports.h>
#include <arch/interrupt.h>

int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int row, int col);
int get_offset_row(int offset);
int get_offset_col(int offset);
int print_char(char c, int row, int col, char attr);
void screen_scroll();

void kprint_at(char* message, int row, int col) {
    int offset;

    if (row >= 0 && col >= 0) {
        offset = get_offset(row, col);
    }
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i], row, col, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
        i++;
    }
}

void kprint(char* message) {
    kprint_at(message, -1, -1);
}

int print_char(char c, int row, int col, char attr) {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;
    if (!attr) {
        attr = WHITE_ON_BLACK;
    }

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        //video_memory[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        //video_memory[2 * MAX_COLS * MAX_ROWS - 1] = 0xf4;
        screen_scroll();
        row--;
        col = 0;
    }

    int offset;
    if (row >= 0 && col >= 0) {
        offset = get_offset(row, col);
    }
    else {
        offset = get_cursor_offset();
    }

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(row+1, 0);
    }
    else {
        video_memory[offset] = c;
        video_memory[offset+1] = attr;
        offset += 2;
    }
    set_cursor_offset(offset);
    return offset;
}

void screen_scroll() {
    intr_disable();
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;

    int col = 0;
    int row = 0;
    int offset, offset_next_line;
    while (row < MAX_ROWS-1) {
        col = 0;
        while (col < MAX_COLS) {
            offset = get_offset(row, col);
            offset_next_line = get_offset(row+1, col);
            video_memory[offset] = video_memory[offset_next_line];
            video_memory[offset+1] = video_memory[offset_next_line+1];
            col++;
        }
        row++;
    }
    col = 0;
    while (col < MAX_COLS) {
        offset = get_offset(row, col);
        video_memory[offset] = ' ';
        video_memory[offset+1] = WHITE_ON_BLACK;
        col++;
    }
    intr_enable();
}

void screen_clear() {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;

    int offset;
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            offset = get_offset(row, col);
            video_memory[offset] = ' ';
            video_memory[offset+1] = WHITE_ON_BLACK;
        }
    }

    set_cursor_offset(get_offset(0,0));
}

int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset +=  port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_offset(int row, int col) {
    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}
