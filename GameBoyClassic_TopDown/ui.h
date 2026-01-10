#ifndef __UI_INCLUDE
#define __UI_INCLUDE

#include "res/dialogs.h"

void ui_init(void);
void ui_draw(void);
void ui_show_pause(void);
void draw_centered_text(const char *text, u8 len);

void ui_typewritter_draw(const String *text, u8 start);
void play_dialog_sequence(const DialogSequence *seq);

#endif