#pragma once

/* -------------------------------- INCLUDES -------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------- DEFINES -------------------------------- */

#define FIELD_WIDTH (4)

#if FIELD_WIDTH > UINT8_MAX
#error FIELD_MAX_WIDTH too big
#endif

/* -------------------------------- TYPEDEFS -------------------------------- */

typedef struct 
{
    uint8_t cells[FIELD_WIDTH];
} field_s;

typedef enum 
{
    CELL_EMPTY = 0,
    CELL_PLAYER_1 = 1,
    CELL_PLAYER_2 = 2,
    CELL_UNDEFINED = 3
} cell_e;

typedef enum
{
    STEP_NORMAL,
    STEP_WIN,
    STEP_NOT_AVAILABLE,
} step_e;
/* -------------------------- FUNCTIONS PROTOTYPES -------------------------- */

void fieldDraw( const field_s *field );
void fieldDrawCursor( const field_s *field, uint8_t column, cell_e chip );
step_e fieldPutChip( field_s *field, uint8_t column, cell_e chip, bool is_print );
bool fieldIsFull( const field_s *field );
int my_puts( const char *s );

