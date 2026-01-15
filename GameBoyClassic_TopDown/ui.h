#ifndef __UI_INCLUDE
#define __UI_INCLUDE

#include "res/dialogs.h"

typedef enum
{
    UI_NONE = 0,
    UI_RUNNING_SEQ = 1,
    UI_WAIT_USER_INPUT = 2,
    UI_WAIT_TIMER = 4,
} UIStates;
typedef struct
{
    UIStates state;
    u8 dial_seq_idx;
    const DialogSequence* seq;
} UIState;

extern UIState UI_STATE;

void ui_init(void);
void ui_draw(void);
void ui_show_pause(void);

void ui_draw_dialog_borders(void);
void ui_draw_fullscreen(const char *text, u8 len);
void ui_draw_typewitter(const String *text, u8 start);

void ui_start_dialog_seq(const DialogSequence *seq);
void ui_dialog_step(void);
void ui_end_dialog_seq(void);

BOOLEAN ui_wait_user_input(void);
BOOLEAN ui_wait_timer(void);
BOOLEAN ui_running_seq(void);
void ui_reset_state(void);

#endif