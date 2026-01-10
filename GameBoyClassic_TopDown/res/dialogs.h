#ifndef __dialogs_INCLUDE
#define __dialogs_INCLUDE

#include "../math.h"

typedef struct String
{
    const char *str;
    u8 len;
} String;

typedef enum SequenceAction
{
    SEQ_NONE,
    SEQ_TIMER,
    SEQ_USER_INPUT,
} SequenceAction;

typedef struct DialogSequence
{
    String* texts[5];
    SequenceAction actions[5];
} DialogSequence;

/// Hello World Sequence for testing
/// ======
extern const String hello_world_1;
extern const String hello_world_2;
extern const DialogSequence hello_world_sequence;
/// end

#endif