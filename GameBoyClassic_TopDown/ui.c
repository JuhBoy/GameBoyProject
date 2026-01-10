#include "inputs.h"
#include "math.h"
#include "helpers.h"
#include "ui.h"
#include "res/Font.h"
#include "helpers.h"
#include <gb/gb.h>

#define UI_FONT_VRAM_OFFSET 128
#define WIN_ROWS (uint8_t)18
#define WIN_COLS (uint8_t)20
#define WIN_ROWS_HALF (uint8_t)9
#define WIN_COLS_HALF (uint8_t)10

void ui_init(void)
{
    set_win_data(UI_FONT_VRAM_OFFSET, Font_TILE_COUNT, Font_tiles);
}

void ui_draw(void)
{
}

void ui_show_pause(void)
{
    const char *pause_text = "game paused";
    draw_centered_text(pause_text, 11);
}

void draw_centered_text(const char *text, u8 len)
{
    u8 col = 0;
    u8 row = 0;
    u8 stride_p = 0;
    const u8 stride = 12;

    u8 i = 0;
    char c = text[i++];

    while (c != '\0')
    {
        u8 curr = c - 'a' + 1;

        if (c == ' ')
        {
            curr = 0;
        }

        curr += UI_FONT_VRAM_OFFSET;
        u8 *addr = get_win_xy_addr(WIN_ROWS_HALF - (len >> 1) + col, WIN_ROWS_HALF - 1 + row);
        set_vram_byte(addr, curr);

        c = text[i++];
        col++;

        if (++stride_p >= stride)
        {
            row += 1;
            col = 0;
            stride_p = 0;
        }
    }
}

void ui_typewritter_draw(String *text, u8 start)
{
    SHOW_WIN;

    move_win(0, WIN_ROWS * 8 - 24);

    u8 index = 0;
    char c = 'a';

    while (index < text->len)
    {
        c = text->str[index++] - 'a';

        u8 *addr = get_win_xy_addr(start + index, 1);
        u16 offset = UI_FONT_VRAM_OFFSET + c + 1;
        set_vram_byte(addr, offset);

        vsync();
        vsync();
        vsync();
        vsync();
        vsync();
    }
}

void play_dialog_sequence(const DialogSequence *seq)
{
    SHOW_WIN;

    for (u8 i = 0; i < 5; i++)
    {
        SequenceAction action = seq->actions[i];
        String *text = seq->texts[i];
        u8 start = WIN_ROWS_HALF - div_2(text->len);

        if (mod_2(text->len) == 0)
        {
            start += 1;
        }

        if (action == SEQ_NONE)
        {
            break;
        }

        ui_typewritter_draw(text, start);
        WAIT_ONE_SECOND();
        fill_win_rect(0, 0, 31, 31, UI_FONT_VRAM_OFFSET);
    }

    HIDE_WIN;
}
