#include "inputs.h"
#include "math.h"
#include "helpers.h"
#include "ui.h"
#include "res/Font.h"
#include "helpers.h"
#include "res/ui_dialog_borders.h"
#include <gb/gb.h>

// SIZES
//
#define WIN_ROWS (uint8_t)18
#define WIN_COLS (uint8_t)20
#define WIN_ROWS_HALF (uint8_t)9
#define WIN_COLS_HALF (uint8_t)10
#define DIALOG_WIN_VERT_OFFSET_PX (uint8_t)112
#define DIALOG_WIN_VERT_OFFSET_TILES (uint8_t)14

#define TEXT_WIDTH_LIMIT 18
#define TEXT_HEIGHT_LIMIT 2

#define VRAM_DBOX_COL_L_IDX 180
#define VRAM_DBOX_COL_R_IDX 176
#define VRAM_DBOX_TOP_IDX 174
#define VRAM_DBOX_BOT_IDX 178

UIState UI_STATE = {
    .state = UI_NONE,
    .dial_seq_idx = 0,
    .seq = NULL,
};

void ui_init(void)
{
    set_win_data(UI_FONT_VRAM_OFFSET, Font_TILE_COUNT, Font_tiles);
    set_win_data(UI_BORDERS_VRAM_OFFSET, ui_dialog_sprites_count, ui_dialog_borders);
}

void ui_draw(void)
{
}

void ui_show_pause(void)
{
    const char *pause_text = "game paused";
    ui_draw_fullscreen(pause_text, 11);
}

void ui_draw_fullscreen(const char *text, u8 len)
{
    const u8 text_break_col = 12;

    u8 col = 0, row = 0, text_id = 0;
    char c = text[text_id++];

    u8 pixel_offset = mod_2(len) != 0 ? 3 : 7;
    move_win(pixel_offset, 0);

    while (c != '\0')
    {
        u8 letter = get_vram_char(c);
        u8ptr addr = get_win_xy_addr(WIN_COLS_HALF - div_2(len) + col, WIN_ROWS_HALF - 1 + row);
        set_vram_byte(addr, letter);

        c = text[text_id++];
        col++;

        if (col >= text_break_col)
        {
            row += 1;
            col = 0;
        }
    }
}

/// @brief draw a maximum of 2 lines inside the dialog box.
void ui_draw_typewitter(const String *text, u8 start)
{
    SHOW_WIN;

    u8 text_id = 0;
    u8 col_id = 0;
    u8 line = 1;
    u8 vram_c = 'a';

    while (text_id < text->len)
    {
        char text_c = text->str[text_id++];
        vram_c = get_vram_char(text_c);

        if (text_c == '\n')
        {
            line = 2;
            col_id = 0;
            continue;
        }

        col_id++;

        u8 *addr = get_win_xy_addr(start + col_id, line);
        set_vram_byte(addr, vram_c);

        // NOTE(JuH) this will run vsync 4 times
        CALL_4(vsync)
    }
}

void ui_draw_dialog_borders(void)
{
    u8ptr corner_left = get_win_xy_addr(0, 0);
    u8ptr corner_right = get_win_xy_addr(19, 0);
    u8ptr corner_b_left = get_win_xy_addr(0, 3);
    u8ptr corner_b_right = get_win_xy_addr(19, 3);

    // NOTE(JuH) this is a layout in vram - it should not change
    set_vram_byte(corner_left, UI_BORDERS_VRAM_OFFSET);
    set_vram_byte(corner_right, UI_BORDERS_VRAM_OFFSET + (u8)2);
    set_vram_byte(corner_b_left, UI_BORDERS_VRAM_OFFSET + (u8)6);
    set_vram_byte(corner_b_right, UI_BORDERS_VRAM_OFFSET + (u8)4);

    for (u8 i = 1; i < 3; i++)
    {
        u8ptr col_l = get_win_xy_addr(0, i);
        u8ptr col_r = get_win_xy_addr(19, i);

        set_vram_byte(col_l, VRAM_DBOX_COL_L_IDX);
        set_vram_byte(col_r, VRAM_DBOX_COL_R_IDX);
    }

    for (u8 i = 1; i < 19; ++i)
    {
        u8ptr line_up = get_win_xy_addr(i, 0);
        u8ptr line_bot = get_win_xy_addr(i, 3);

        set_vram_byte(line_up, VRAM_DBOX_TOP_IDX);
        set_vram_byte(line_bot, VRAM_DBOX_BOT_IDX);
    }
}

void ui_dialog_step(void)
{
    if (!ui_running_seq())
    {
        return;
    }

    const DialogSequence *seq = UI_STATE.seq;

    for (u8 i = UI_STATE.dial_seq_idx; i < DSEQ_MAX_COUNT; i++)
    {
        fill_win_rect(1, 1, 18, 2, UI_FONT_VRAM_OFFSET);

        SequenceAction action = seq->actions[i];
        const String *text = seq->texts[i];

        if (action == SEQ_NONE)
        {
            UI_STATE.state = UI_NONE;
            break;
        }

        ui_draw_typewitter(text, 0);

        if (action == SEQ_USER_INPUT)
        {
            UI_STATE.dial_seq_idx = i + 1;
            UI_STATE.state |= UI_WAIT_USER_INPUT;
            return;
        }
        else
        {
            UI_STATE.state &= ~UI_WAIT_USER_INPUT;
        }

        if (action == SEQ_TIMER)
        {
            UI_STATE.dial_seq_idx = i + 1;
            UI_STATE.state |= UI_WAIT_TIMER;
            return;
        }
        else
        {
            UI_STATE.state &= ~UI_WAIT_TIMER;
        }
    }
}

void ui_start_dialog_seq(const DialogSequence *seq)
{
    SHOW_WIN;

    // NOTE(JuH) 7 is the number of pixels hidden by default by the win layer (IDK why yet 7 exactly...)
    move_win(7, DIALOG_WIN_VERT_OFFSET_PX);
    ui_draw_dialog_borders();

    UI_STATE.state = UI_RUNNING_SEQ;
    UI_STATE.dial_seq_idx = 0;
    UI_STATE.seq = seq;
}

void ui_end_dialog_seq(void)
{
    fill_win_rect(0, 0, 20, 4, UI_FONT_VRAM_OFFSET);
    move_win(0, 0);
    ui_reset_state();

    HIDE_WIN;
}

BOOLEAN ui_wait_user_input(void)
{
    return (UI_STATE.state & UI_WAIT_USER_INPUT) != 0;
}

BOOLEAN ui_wait_timer(void)
{
    return (UI_STATE.state & UI_WAIT_TIMER) != 0;
}

BOOLEAN ui_running_seq(void)
{
    return (UI_STATE.state & UI_RUNNING_SEQ) != 0;
}

void ui_reset_state(void)
{
    UI_STATE.dial_seq_idx = 0;
    UI_STATE.state = UI_NONE;
    UI_STATE.seq = NULL;
}
