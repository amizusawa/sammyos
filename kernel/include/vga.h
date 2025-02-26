#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_MEMORY 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void kprint_at(char* message, int row, int col);
void kprint(char* message);
void vga_screen_clear();

#endif
