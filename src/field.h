#pragma once

/* -------------------------------- INCLUDES -------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------- DEFINES -------------------------------- */

#define FIELD_MAX_WIDTH (255)

#if FIELD_MAX_WIDTH > UINT8_MAX
#error FIELD_MAX_WIDTH too big
#endif

/* -------------------------------- TYPEDEFS -------------------------------- */

typedef struct 
{
    uint8_t cells[FIELD_MAX_WIDTH];
    uint8_t width;    
} field_s;

typedef enum 
{
    CELL_EMPTY = 0,
    CELL_PLAYER_1 = 1,
    CELL_PLAYER_2 = 2,
    CELL_UNDEFINED = 3
} cell_e;

/* -------------------------- FUNCTIONS PROTOTYPES -------------------------- */

bool fieldTryInit( field_s *field, uint8_t width );
void fieldDraw( const field_s *field );
