#pragma once

/* -------------------------------- INCLUDES -------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------- DEFINES -------------------------------- */

#define FIELD_WIDTH (10)

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
    CELL_BOTH = 3
} cell_e;

typedef enum
{
    STEP_NORMAL,
    STEP_WIN,
    STEP_NOT_AVAILABLE,
} step_e;

typedef enum
{
    TURN_NORMAL,
    TURN_WIN_PLAYER_1,
    TURN_WIN_PLAYER_2,
    TURN_DEAD_HEAT,
    TURN_NOT_AVAILABLE,
} turn_e;
/* -------------------------- FUNCTIONS PROTOTYPES -------------------------- */

void fieldDraw( const field_s *field );
void fieldDrawCursor( const field_s *field, uint8_t column, cell_e chip );
bool fieldMoveAvailable( const field_s *field, uint8_t column );
step_e fieldPutChip( field_s *field, uint8_t column, cell_e chip, bool is_print );
turn_e fieldTurn( field_s *field, uint8_t column_first_player, uint8_t column_second_player, bool is_print );
bool fieldIsFull( const field_s *field );


