
#include "screen.h"
#include "ports.h"

int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int row, int col);
int get_offset_row(int offset);
int get_offset_col(int offset);
int print_char(char c, int row, int col, char attr);

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

int print_char(char c, int row, int col, char attr) {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;
    if (!attr) {
        attr = WHITE_ON_BLACK;
    }

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        video_memory[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        video_memory[2 * MAX_COLS * MAX_ROWS - 1] = 0xf4;
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
